//
//  PhaserProtocol.cpp
//  AgentRuntimeEnvironment
//
//  Created by Hamed KHANDAN on 5/21/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

// KFoundation
#include <kfoundation/Ptr.h>

// KnoRBA
#include <knorba/type/all.h>
#include <knorba/Runtime.h>
#include <knorba/Agent.h>
#include <knorba/Group.h>

// Self
#include "PhaserProtocol.h"

namespace knorba {
namespace protocol {
  
// --- STATIC FIELDS --- //
  
  const SPtr<KString> PhaserProtocol::OP_PHASE = KS("knorba.phaser.phase");
  const SPtr<KString> PhaserProtocol::OP_RELEASE = KS("knorba.phaser.release");
  
  
// --- STATIC METHODS --- //
  
  void PhaserProtocol::init(Runtime& rt) {
    rt.registerMessageFormat(OP_PHASE, KType::LONGINT);
    rt.registerMessageFormat(OP_RELEASE, KType::LONGINT);
  }
  
  
// --- (DE)CONSTRUCTORS --- //
  
  PhaserProtocol::PhaserProtocol(Agent* agent, PPtr<KString> role)
  : Protocol(agent),
    _rt(agent->getRuntime()),
    _synchronizeCond(true)
  {
    _phase = new KLongint(0);
    
    _role = role;
    _nRelease = 0;
    _leader = KGlobalUid::zero();
    _isLeader = _rt.isHead();
    _hasLeader = false;
    _stopFlag = false;
    
    registerHandler((phandler_t)&PhaserProtocol::handleOpPhase, OP_PHASE);
    registerHandler((phandler_t)&PhaserProtocol::handleOpRelease, OP_RELEASE);
  }
  
  
// --- METHODS --- //

  void PhaserProtocol::releaseAll() {
    _nRelease = 0;
    _agent->send(_role, OP_RELEASE, _phase.AS(KValue));
    _synchronizeCond.release();
  }
  
  
  int PhaserProtocol::getNMembers() const {
    return _agent->getPeers(_role)->getCount() + 1;
  }
  
  
  void PhaserProtocol::reset() {
    _nRelease = 0;
    _phase = 0;
  }
  
  
  void PhaserProtocol::synchronize() {
    if(_stopFlag) {
      return;
    }
    
    _phase->set(_phase->get() + 1);
    
    if(getNMembers() == 0) {
      return;
    }
    
    if(_isLeader) {
      _nRelease++;
      if(_nRelease >= getNMembers()) {
        releaseAll();
        return;
      }
    } else {
      if(_hasLeader) {
        _agent->send(_leader, OP_PHASE, _phase.AS(KValue));
      } else {
        _agent->send(_role, OP_PHASE, _phase.AS(KValue));
      }
    }
    
    _synchronizeCond.block();
  }
  
  
  void PhaserProtocol::timedSynchronize(k_integer_t msecs) {
    // Nothing;
  }
  
  
  k_longint_t PhaserProtocol::getPhase() const {
    return _phase->get();
  }
  
  
  // Handlers //
  
  void PhaserProtocol::handleOpPhase(PPtr<Message> msg) {
    if(!_isLeader) {
      return;
    }
    
    _nRelease++;
    
    if(_nRelease >= getNMembers()) {
      releaseAll();
    }
  }
  
  
  void PhaserProtocol::handleOpRelease(PPtr<Message> msg) {
    if(!_hasLeader) {
      _leader = msg->getSender();
      _hasLeader = true;
    }
    
    _synchronizeCond.release();
  }
  
  
  // Inherited from Protocol //
  
  void PhaserProtocol::handlePeerDisconnected(PPtr<KString> role,
      const k_guid_t& guid)
  {
    if(!_isLeader) {
      return;
    }
    
    if(!role->equals(_role)) {
      return;
    }
    
    if(_nRelease >= getNMembers()) {
      releaseAll();
    }
  }
  
  
  void PhaserProtocol::finalize() {
    if(_synchronizeCond.isBlocked()) {
      _synchronizeCond.release();
    }
    _stopFlag = true;
  }
  
  
  bool PhaserProtocol::isAlive() const {
    return _synchronizeCond.isBlocked();
  }
  
} // namespace protocol
} // namespace knorba
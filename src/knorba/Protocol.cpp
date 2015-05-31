//
//  Protocol.cpp
//  KnoRBA
//
//  Created by Hamed KHANDAN on 2/22/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

// KFoundation
#include <kfoundation/Ptr.h>
#include <kfoundation/OutputStream.h>
#include <kfoundation/ObjectStreamReader.h>

// Internal
#include "Agent.h"
#include "type/KString.h"

// Self
#include "Protocol.h"

namespace knorba {
  
// --- (DE)CONSTRUCTORS --- //
  
  Protocol::Protocol(Agent* owner)
  : _agent(owner)
  {
    _agent->registerProtocol(this);
  }
  
  
  Protocol::~Protocol() {
    _agent->unregisterProtocol(this);
  }
  
  
// --- METHODS --- //
  
  void Protocol::registerHandler(phandler_t handler, PPtr<KString> opcode) {
    k_longint_t hash = opcode->getHashCode();
    _handlerMap[hash] = handler;
  }
  
  
  Protocol::phandler_t Protocol::getHandlerForOpcodeHash(const k_longint_t hash)
  {
    map_t::iterator it = _handlerMap.find(hash);
    
    if(it == _handlerMap.end()) {
      return NULL;
    }
    
    return it->second;
  }
  
  
  void Protocol::handlePeerConnectionReuqest(PPtr<KString> role,
      const k_guid_t& guid)
  {
    // Nothing;
  }
  
  
  void Protocol::handlePeerDisconnected(PPtr<KString> role, const k_guid_t& guid) {
    // Nothing;
  }
  
  
  void Protocol::finalize() {
    // Nothing;
  }
  
  
  bool Protocol::isAlive() const {
    return false;
  }
  
} // namespace knorba
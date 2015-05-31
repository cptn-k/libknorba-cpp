//
//  GroupingProtocol.cpp
//  AgentRuntimeEnvironment
//
//  Created by Hamed KHANDAN on 5/13/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

// KFoundation
#include <kfoundation/Ptr.h>

// KnoRBA
#include <knorba/Agent.h>
#include <knorba/Group.h>
#include <knorba/type/all.h>

#include "GroupingProtocol.h"

namespace knorba {
namespace protocol {
  
// --- STATIC FIELDS --- //
  
  const SPtr<KString> GroupingProtocol::OP_HELLO = KS("knorba.grouping.hello");
  
  
// --- STATIC METHODS --- //
  
  void GroupingProtocol::init(Runtime& rt) {
    rt.registerMessageFormat(OP_HELLO, KType::LONGINT);
  }
  
  
// --- (DE)CONSTRUCTORS --- //
  
  GroupingProtocol::GroupingProtocol(Agent* owner, PPtr<KString> role,
      k_longint_t groupId)
  : Protocol(owner)
  {
    _role = role;
    _groupId = new KLongint(groupId);
    registerHandler((phandler_t)&GroupingProtocol::handleOpHello, OP_HELLO);
  }
  
  
// --- METHODS --- //
  
  void GroupingProtocol::start() {
    if(_agent->getRuntime().getNodeCount() == 1) {
      onAllMembersConnected();
    } else {
      _agent->sendToAll(OP_HELLO, _groupId.AS(KValue));
    }
  }
  
  
  k_integer_t GroupingProtocol::getNMembers() const {
    return _agent->getPeers(_role)->getCount() + 1;
  }
  
  
  // Virtual Methods //
  
  void GroupingProtocol::onMemberConnected(const k_guid_t& guid) {
    // Nothing;
  }
  
  
  void GroupingProtocol::onMemberDisconnected(const k_guid_t& guid) {
    // Nothing;
  }
  
  
  void GroupingProtocol::onAllMembersConnected() {
    // Nothing;
  }
  
  
  // Handlers //
  
  void GroupingProtocol::handleOpHello(PPtr<Message> msg) {
    k_longint_t groupId = msg->getPayload().AS(KLongint)->get();
    
    if(groupId == _groupId->get()) {
      const k_guid_t& sender = msg->getSender();
      
      if(!_agent->getPeers(_role)->containts(sender)) {
        _agent->send(sender, OP_HELLO, _groupId.AS(KValue));
        _agent->addPeer(_role, msg->getSender());
        onMemberConnected(msg->getSender());
        if(_agent->getRuntime().getNodeCount() == getNMembers()) {
          onAllMembersConnected();
        }
      }
    }
  }
  
  
  // Inherited from Protocol //
  
  void GroupingProtocol::handlePeerDisconnected(PPtr<KString> role,
      const k_guid_t& guid)
  {
    if(role->equals(_role)) {
      onMemberDisconnected(guid);
    }
  }
  
} // namespace protocol
} // namespace knorba
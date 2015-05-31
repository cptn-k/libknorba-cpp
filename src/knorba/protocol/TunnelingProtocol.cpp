//
//  TunnelingProtocol.cpp
//  AgentRuntimeEnvironment
//
//  Created by Kay Khandan on 3/28/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

// KFoundation
#include <kfoundation/Ptr.h>

// KnoRBA
#include <knorba/Agent.h>
#include <knorba/Group.h>
#include <knorba/Runtime.h>
#include <knorba/type/all.h>

// Self
#include "TunnelingProtocol.h"

namespace knorba {
namespace protocol {
  
// --- STATIC FIELDS --- //
  
  SPtr<KRecordType> TunnelingProtocol::MESSAGE_T;
  SPtr<KRecordType> TunnelingProtocol::ROUTE_T;
    
  const SPtr<KString> TunnelingProtocol::OP_SEND
      = KS("knorba.tunnel.send");
  
  const SPtr<KString> TunnelingProtocol::OP_SEND_BCAST
      = KS("knorba.tunnel.send-bcast");
  
  const SPtr<KString> TunnelingProtocol::OP_RECEIVE
      = KS("knorba.tunnel.receive");
  
  const SPtr<KString> TunnelingProtocol::OP_RECEIVE_BCAST
      = KS("knorba.tunnel.receive-bcast");
  
  const SPtr<KString> TunnelingProtocol::OP_ADD_ROUTE
      = KS("knorba.tunnel.add-route");
  
  const SPtr<KString> TunnelingProtocol::OP_REMOVE_ROUTE
      = KS("knorba.tunnel.remove-rote");
  
  const SPtr<KString> TunnelingProtocol::R_TUNNEL = KS("tunnel");
  const SPtr<KString> TunnelingProtocol::R_CLIENT = KS("client");
  
  
// --- STATIC METHODS --- //
  
  PPtr<KRecordType> TunnelingProtocol::message_t() {
    if(MESSAGE_T.isNull()) {
      MESSAGE_T = new KRecordType("knorba.tunnel.Message");
      MESSAGE_T->addField(/* 0 */ "remoteKernel", KType::GUID)
          ->addField(/* 1 */ "sender", KType::GUID)
          ->addField(/* 2 */ "receiver", KType::GUID)
          ->addField(/* 3 */ "opcode", KType::LONGINT)
          ->addField(/* 4 */ "transactionId", KType::INTEGER)
          ->addField(/* 5 */ "payload", KType::ANY);
      MESSAGE_T.setSelfDestruct();
    }
    
    return MESSAGE_T;
  }
  
  
  PPtr<KRecordType> TunnelingProtocol::route_t() {
    if(ROUTE_T.isNull()) {
      ROUTE_T = new KRecordType("knorba.tunnel.Route");
      ROUTE_T->addField("remoteKernel", KType::GUID)
          ->addField("remoteRuntime", KType::GUID);
      ROUTE_T.setSelfDestruct();
    }
    
    return ROUTE_T;
  }
  
  
  void TunnelingProtocol::init(Runtime& rt) {
    rt.registerMessageFormat(OP_SEND        , message_t().AS(KType));
    rt.registerMessageFormat(OP_SEND_BCAST  , message_t().AS(KType));
    rt.registerMessageFormat(OP_RECEIVE     , message_t().AS(KType));
    rt.registerMessageFormat(OP_RECEIVE_BCAST, message_t().AS(KType));
    rt.registerMessageFormat(OP_ADD_ROUTE   , route_t().AS(KType));
    rt.registerMessageFormat(OP_REMOVE_ROUTE, route_t().AS(KType));
  }
  
  
// --- (DE)CONSTRUCTORS ---- //
  
  TunnelingProtocol::TunnelingProtocol(Agent* agent)
  : Protocol(agent)
  {
    // Nothing
  }
  
} // namespace protocol
} // namespace knorba

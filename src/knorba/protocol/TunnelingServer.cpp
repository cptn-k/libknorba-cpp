//
//  TunnelingServer.cpp
//  AgentRuntimeEnvironment
//
//  Created by Kay Khandan on 3/29/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

// KFoundation
#include <kfoundation/Ptr.h>

// KnoRBA
#include <knorba/Agent.h>
#include <knorba/type/all.h>

// Self
#include "TunnelingServer.h"

namespace knorba {
namespace protocol {

// --- (DE)CONSTRUCTORS --- //
  
  TunnelingServer::TunnelingServer(Agent* agent)
  : TunnelingProtocol(agent)
  {
    registerHandler((phandler_t)&TunnelingServer::handleOpSend, OP_SEND);
    registerHandler((phandler_t)&TunnelingServer::handleOpSendBcast, OP_SEND_BCAST);
  }
  
  
// --- METHODS --- //
    
  const k_guid_t& TunnelingServer::getClient() const {
    return _client;
  }
  
  
  void TunnelingServer::opReceive(PPtr<KRecord> data) {
    _agent->send(_client, OP_RECEIVE, data.AS(KValue));
  }
  
  
  void TunnelingServer::opReceiveBcast(PPtr<KRecord> msg) {
    _agent->send(_client, OP_RECEIVE_BCAST, msg.AS(KValue));
  }
  
  
  void TunnelingServer::opAddRoute(const k_guid_t& remoteRt,
      const k_guid_t& remoteKernel)
  {
    Ptr<KRecord> r = new KRecord(route_t());
    r->setGuid(0, remoteKernel);
    r->setGuid(1, remoteRt);
    _agent->send(_client, OP_ADD_ROUTE, r.AS(KValue));
  }
  
  
  void TunnelingServer::opRemoveRoute(const k_guid_t& rt) {
    _agent->send(_client, OP_REMOVE_ROUTE, new KGuid(rt));
  }

  
// Message Handlers //
  
  void TunnelingServer::handleOpSend(PPtr<Message> msg) {
    onSend(msg->getPayload().AS(KRecord));
  }
  
  
  void TunnelingServer::handleOpSendBcast(PPtr<Message> msg) {
    onSendBcast(msg->getPayload().AS(KRecord));
  }
  
  
// Virtual Methods //
  
  void TunnelingServer::onSend(PPtr<KRecord> msg) {
    // Nothing;
  }
  
  
  void TunnelingServer::onSendBcast(PPtr<KRecord> msg) {
    // Nothinig;
  }
  
  
// Inherited from Protocol //
  
  void TunnelingServer::handlePeerConnectionReuqest(PPtr<KString> role,
      const k_guid_t& guid)
  {
    if(role->equals(R_CLIENT)) {
      _client = guid;
      _agent->addPeer(R_CLIENT, guid);
    }
  }

  
} // namespace protocol
} // namespace knorba
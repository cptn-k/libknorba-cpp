//
//  UnixSocketAgentClient.cpp
//  AgentRuntimeEnvironment
//
//  Created by Hamed KHANDAN on 4/9/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

// KFoundation
#include <kfoundation/Ptr.h>

// KnoRBA
#include <knorba/Runtime.h>
#include <knorba/Agent.h>
#include <knorba/type/KString.h>
#include <knorba/type/KGuid.h>

#include "UnixSocketClient.h"

namespace knorba {
namespace protocol {

// --- STATIC FIELDS --- //
  
  const SPtr<KString> UnixSocketClient::OP_ADD_CONNECTION
      = KS("knorba.unix-socket.add-connection");
  
  const SPtr<KString> UnixSocketClient::OP_SET_ADDRESS
      = KS("knorba.unix-socket.set-address");
  
  
// --- STATIC METHODS --- //
  
  void UnixSocketClient::init(Runtime& rt) {
    rt.registerMessageFormat(OP_SET_ADDRESS, KType::STRING);
    rt.registerMessageFormat(OP_ADD_CONNECTION, KType::STRING);
  }
  
  
// --- (DE)CONSTRUCTORS --- //
  
  UnixSocketClient::UnixSocketClient(Agent* owner)
  : Protocol(owner)
  {
    _server = KGuid::zero();
  }
  
  
// --- METHODS --- //
  
  void UnixSocketClient::setServer(const k_guid_t& guid) {
    _server = guid;
  }
  
  
  const k_guid_t& UnixSocketClient::getServer() const {
    return _server;
  }
  
  
  void UnixSocketClient::setAddress(const string& address) {
    _agent->send(_server, OP_SET_ADDRESS, new KString(address));
  }
  
  
  void UnixSocketClient::addConnections(const string& address) {
    _agent->send(_server, OP_ADD_CONNECTION, new KString(address));
  }
  
} // namespace protocol
} // namespace knorba

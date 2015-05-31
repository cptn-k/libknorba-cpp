//
//  ConsoleProtocolClient.cpp
//  AgentRuntimeEnvironment
//
//  Created by Hamed KHANDAN on 4/24/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

#include <kfoundation/Ptr.h>
#include <knorba/Agent.h>
#include <knorba/Group.h>
#include "ConsoleProtocolClient.h"

namespace knorba {
namespace protocol {

  // --- STATIC FIELDS --- //

  SPtr<KRecordType> ConsoleProtocolClient::RELAY_PATH_T;

  const SPtr<KString> ConsoleProtocolClient::OP_PRINT
      = KS("knorba.console.print");
  
  const SPtr<KString> ConsoleProtocolClient::OP_INPUT
      = KS("knorba.console.input");

  const SPtr<KString> ConsoleProtocolClient::OP_START
      = KS("knorba.console.start");
  
  const SPtr<KString> ConsoleProtocolClient::OP_STOP
      = KS("knorba.console.stop");
  
  const SPtr<KString> ConsoleProtocolClient::OP_SUBSCRIBE
      = KS("knorba.console.subscribe");
  
  const SPtr<KString> ConsoleProtocolClient::OP_UNSUBSCRIBE
      = KS("knorba.console.unsubscribe");
  
  const SPtr<KString> ConsoleProtocolClient::OP_ADD_RELAY_PATH
      = KS("knorba.console.add-relay-path");

  const SPtr<KString> ConsoleProtocolClient::OP_REMOVE_RELAY_PATH
      = KS("knorba.console.remove-relay-path");
  
  const SPtr<KString> ConsoleProtocolClient::R_CONSOLE = KS("console");
  

  // --- STATIC METHODS --- //

  void ConsoleProtocolClient::init(Runtime& rt) {
    rt.registerMessageFormat(OP_PRINT, KType::STRING);
    rt.registerMessageFormat(OP_INPUT, KType::STRING);
    rt.registerMessageFormat(OP_START, KType::NOTHING);
    rt.registerMessageFormat(OP_STOP, KType::NOTHING);
    rt.registerMessageFormat(OP_SUBSCRIBE, KType::NOTHING);
    rt.registerMessageFormat(OP_ADD_RELAY_PATH, relay_path_t().AS(KType));
    rt.registerMessageFormat(OP_REMOVE_RELAY_PATH, KType::GUID);
  }


  SPtr<KRecordType> ConsoleProtocolClient::relay_path_t() {
    if(RELAY_PATH_T.isNull()) {
      RELAY_PATH_T = new KRecordType("knorba.console.RelayPath");
      RELAY_PATH_T->addField("sourceRuntime", KType::GUID)
                  ->addField("targetConsole", KType::GUID);
    }
    return RELAY_PATH_T;
  }


  // --- (DE)CONSTRUCTORS --- //

  ConsoleProtocolClient::ConsoleProtocolClient(Agent* owner)
  : Protocol(owner)
  {
    _noServer = true;
    _intro = owner->getAlias() + "@" + owner->getRuntime().getAppName()
        + "[" + Int::toString(owner->getGuid().nodeRank) + "]: ";
    registerHandler((phandler_t)&ConsoleProtocolClient::handleOpInput, OP_INPUT);
  }


  // --- METHODS --- //

  void ConsoleProtocolClient::handleOpInput(PPtr<Message> msg) {
    onInputReceived(msg->getPayload().AS(KString));
  }
  
  
  void ConsoleProtocolClient::setServer(const k_guid_t& guid) {
    _server = guid;
    _noServer = (_server == KGlobalUid::zero());
  }
  
  
  const k_guid_t& ConsoleProtocolClient::getServer() const {
    return _server;
  }

  
  void ConsoleProtocolClient::print(const Streamer& streamer) {
    print(streamer.toString());
  }
  
  
  void ConsoleProtocolClient::print(PPtr<KString> text) {
    print(text->toUtf8String());
  }
  
  
  void ConsoleProtocolClient::print(const string& text) {
    if(_noServer) {
      setServer(_agent->getRuntime().getConsoleGuid());
    }
    
    if(_noServer) {
      LOG << _intro << text << EL_CON;
    } else {
      _agent->send(_server, OP_PRINT, new KString(_intro + text));
    }
  }

  
  void ConsoleProtocolClient::subscribe() {
    _agent->send(_server, OP_SUBSCRIBE, KValue::NOTHING);
  }
  
  
  void ConsoleProtocolClient::unsubscribe() {
    _agent->send(_server, OP_UNSUBSCRIBE, KValue::NOTHING);
  }

  
  void ConsoleProtocolClient::addRelayPath(const k_guid_t& vrt,
      const k_guid_t& console)
  {
    Ptr<KRecord> rec = new KRecord(relay_path_t());
    rec->setGuid(0, vrt);
    rec->setGuid(1, console);
    _agent->send(_server, OP_ADD_RELAY_PATH, rec.AS(KValue));
  }
  
  
  void ConsoleProtocolClient::removeRelayPath(const k_guid_t& vrt) {
    _agent->send(_server, OP_REMOVE_RELAY_PATH, new KGlobalUid(vrt));
  }
  
  
  void ConsoleProtocolClient::start() {
    _agent->send(_server, OP_START, KValue::NOTHING);
  }
  
  
  void ConsoleProtocolClient::stop() {
    _agent->send(_server, OP_STOP, KValue::NOTHING);
  }
  
  
  void ConsoleProtocolClient::onInputReceived(PPtr<KString> input) {
    // Nothing
  }
  
  
  // Inherited from Protocol //
  
  void ConsoleProtocolClient::handlePeerConnectionReuqest(PPtr<KString> role,
      const k_guid_t& guid)
  {
    if(role->equals(R_CONSOLE)) {
      setServer(guid);
      _agent->removeAllPeers(R_CONSOLE);
      _agent->addPeer(R_CONSOLE, guid);
    }
  }

} // namespace protocol
} // namespace knorba
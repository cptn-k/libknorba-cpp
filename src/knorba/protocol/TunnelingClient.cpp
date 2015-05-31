//
//  TunnelingClient.cpp
//  AgentRuntimeEnvironment
//
//  Created by Kay Khandan on 3/29/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

// KFoundation
#include <kfoundation/Ptr.h>

// KnoRBA
#include <knorba/Agent.h>
#include <knorba/Message.h>
#include <knorba/Group.h>
#include <knorba/type/all.h>

// Self
#include "TunnelingClient.h"

namespace knorba {
namespace protocol {
  
// --- STATIC METHODS --- //
  
  void TunnelingClient::logRecord(const ProxyRecord& r) {
    LOG << "remoteRuntime: " << r.remoteRuntime << ", remoteKernel: "
        << r.remoteKernel << ", tunnel: " << r.tunnel << EL;
  }
  
  
  void TunnelingClient::init(Runtime& rt) {
    TunnelingProtocol::init(rt);
  }
  
  
// --- (DE)CONSTRUCTORS --- //

  TunnelingClient::TunnelingClient(Agent* owner)
  : TunnelingProtocol(owner)
  {
    _proxyTable = new Array<ProxyRecord>();
    
    registerHandler((phandler_t)&TunnelingClient::handleOpReceive, OP_RECEIVE);
    registerHandler((phandler_t)&TunnelingClient::handleOpReceiveBcast, OP_RECEIVE_BCAST);
    registerHandler((phandler_t)&TunnelingClient::handleOpAddRoute, OP_ADD_ROUTE);
    registerHandler((phandler_t)&TunnelingClient::handleOpRemoveRoute, OP_REMOVE_ROUTE);
  }
  
  
// --- METHODS --- //
  
//  void TunnelingClient::watchdog() {
//    _isWatchdogRunning = true;
//    
//    System::setThreadName(_agent->getAlias() + " tunnel watchdog");
//    
//    PLOG << "Watchdog started." << EL;
//    
//    while(!_stopFlag) {
//      for(int i = 0; i < 3 && !_stopFlag; i++) {
//        System::sleep(400 + rand()%100);
//      }
//      
//      _proxyTableMutex.lock();
//      
//      for(int i = _proxyTable->getSize() - 1; i >= 0 && !_stopFlag; i--) {
//        ProxyRecord& r = _proxyTable->at(i);
//        
//        if(r.counter <= 0) {
//          LOG << "Timeout expired" << EL;
//          logRecord(r);
//          onConnectionLost(r.remoteRuntime, r.remoteKernel, r.tunnel);
//          _proxyTable->remove(i);
//        } else {
//          _agent->send(r.remoteKernel, OP_PING_Q, KValue::NOTHING);
//          r.counter--;
//          //LOG << "Count down" << EL;
//          logRecord(r);
//        }
//      }
//      
//      _proxyTableMutex.unlock();
//    }
//    
//    _isWatchdogRunning = false;
//    PLOG << "Watchdog ended." << EL;
//  }
  
  
  int TunnelingClient::getProxyRecordIndexForTarget(const k_guid_t& t) const {
    for(int i = _proxyTable->getSize() - 1; i >= 0; i--) {
      if(KGlobalUid::areOnTheSameNode(t, _proxyTable->at(i).remoteRuntime)) {
        return i;
      }
    }
    return -1;
  }
  
  
  void TunnelingClient::addTunnel(const k_guid_t& t) {
    _agent->addPeer(R_TUNNEL, t);
    _agent->send(t, Agent::OP_CONNECT, R_CLIENT.AS(KValue));
  }
  
  
  void TunnelingClient::addRoute(const k_guid_t& rt, const k_guid_t& kernel,
      const k_guid_t& tunnel)
  {
    ProxyRecord r;
    r.remoteRuntime = rt;
    r.remoteKernel = kernel;
    r.tunnel = tunnel;
    _proxyTable->push(r);
  }
  
  
  void TunnelingClient::addVtrRoute(const k_guid_t& vrt, const k_guid_t& kernel) {
    int index = -1;
    for(int i = _proxyTable->getSize() - 1; i >= 0; i--) {
      if(_proxyTable->at(i).remoteKernel == kernel) {
        index = i;
        break;
      }
    }
    
    if(index == -1) {
      PLOG_ERR << "Could not add a route to " << vrt << " because there is "
        << "no tunnel registered to reach corresponding kernel " << kernel
        << EL;
    }
    
    addRoute(vrt, kernel, _proxyTable->at(index).tunnel);
  }
  
  
  void TunnelingClient::removeRoute(const k_guid_t& rt) {
    int index = getProxyRecordIndexForTarget(rt);
    if(index == -1) {
      PLOG_WRN << "Request removal for unregistered route to " << rt << EL;
      return;
    }
    
    KF_SYNCHRONIZED(_proxyTableMutex,
      _proxyTable->remove(index);
    )
    
    PLOG << "Removed route to: " << rt << EL;
  }
  
  
  void TunnelingClient::send(const k_guid_t& receiver, PPtr<Message> msg) {
    int index = getProxyRecordIndexForTarget(receiver);
    
    if(index == -1) {
      PLOG_WRN << "There is no registered route to reach " << receiver
          << ". Message ignored: " << msg->headerToString(_agent->getRuntime())
          << EL;
    } else {
      Ptr<KRecord> tunnelMsg = new KRecord(message_t());
      tunnelMsg->setGuid(0, _proxyTable->at(index).remoteKernel);
      tunnelMsg->setGuid(1, msg->getSender());
      tunnelMsg->setGuid(2, receiver);
      tunnelMsg->setLongint(3, msg->getOpcodeHash());
      tunnelMsg->setInteger(4, msg->getTransactionId());
      tunnelMsg->field<KAny>(5)->setValue(msg->getPayload());
      _agent->send(_proxyTable->at(index).tunnel, OP_SEND, tunnelMsg.AS(KValue));
    }
  }


  void TunnelingClient::sendToAll(PPtr<Message> msg) {
    Ptr<KRecord> tunnelMsg = new KRecord(message_t());
    tunnelMsg->setGuid(0, KGlobalUid::zero());
    tunnelMsg->setGuid(1, msg->getSender());
    tunnelMsg->setGuid(2, KGlobalUid::zero());
    tunnelMsg->setLongint(3, msg->getOpcodeHash());
    tunnelMsg->setInteger(4, msg->getTransactionId());
    tunnelMsg->field<KAny>(5)->setValue(msg->getPayload());
    
    _agent->send(R_TUNNEL, OP_SEND_BCAST, tunnelMsg.AS(KValue));
  }

  
// Message Handlers //
  
  void TunnelingClient::handleOpReceive(PPtr<Message> msg) {
    try {
      onReceive(msg->getPayload().AS(KRecord));
    } catch(KFException& e) {
      LOG << msg->headerToString(_agent->getRuntime()) << EL_CON;
      LOG << *msg->getPayload() << EL_CON;
      LOG << e << EL_CON;
      _agent->quit();
    }
  }
  
  
  void TunnelingClient::handleOpReceiveBcast(PPtr<Message> msg) {
    onReceiveBcast(msg->getPayload().AS(KRecord));
  }
  
  
  void TunnelingClient::handleOpAddRoute(PPtr<Message> msg) {
    PPtr<KRecord> data = msg->getPayload().AS(KRecord);
    addRoute(data->getGuid(1), data->getGuid(0), msg->getSender());
    onAddRoute(data->getGuid(1), data->getGuid(0), msg->getSender());
  }
  
  
  void TunnelingClient::handleOpRemoveRoute(PPtr<Message> msg) {
    k_guid_t rt = msg->getPayload().AS(KGlobalUid)->get();
    
    bool found = false;
    
    _proxyTableMutex.lock();
    
    for(int i = _proxyTable->getSize() - 1; i >= 0; i--) {
      if(KGlobalUid::areOnTheSameApp(_proxyTable->at(i).remoteKernel, rt)) {
        ProxyRecord r = _proxyTable->at(i);
        _proxyTable->remove(i);
        LOG << "Disconnected runtime removed: " << r.remoteRuntime << EL;
        onConnectionLost(r.remoteRuntime, r.remoteKernel, r.tunnel);
        found = true;
      }
    }
    
    _proxyTableMutex.unlock();
    
    if(!found) {
      PLOG_WRN << "Tunnel requesting removal of route to an unregistered "
          "target: " << rt << EL;
    }
  }
  
  
// Virtual Methods //
  
  void TunnelingClient::onReceive(PPtr<KRecord> msg) {
    // Nothing;
  }
  
  
  void TunnelingClient::onReceiveBcast(PPtr<KRecord> msg) {
    // Nothing;
  }
  
  
  void TunnelingClient::onAddRoute(const k_guid_t&, const k_guid_t&, const k_guid_t&) {
    // Nothing;
  }
  
  
  void TunnelingClient::onConnectionLost(const k_guid_t& guid, const k_guid_t& kernel,
      const k_guid_t& proxy)
  {
    // Nothing;
  }
  
  
} // namespace protocol
} // namespace knorba

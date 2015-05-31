//
//  TunnelingClient.h
//  AgentRuntimeEnvironment
//
//  Created by Kay Khandan on 3/29/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

#ifndef __AgentRuntimeEnvironment__TunnelingClient__
#define __AgentRuntimeEnvironment__TunnelingClient__

// KFoundation
#include <kfoundation/Mutex.h>

// Super
#include "TunnelingProtocol.h"

namespace knorba {
  class Group;
}

namespace knorba {
namespace type {
  class KRecord;
}
}

namespace knorba {
namespace protocol {
  
  using namespace kfoundation;
  using namespace knorba;
  
  class TunnelingClient : public TunnelingProtocol {
  
  // --- NESTED TYPES --- //
    
    private: struct ProxyRecord {
      k_guid_t remoteRuntime;
      k_guid_t remoteKernel;
      k_guid_t tunnel;
    };
    
    
  // --- STATIC METHODS --- //
    
    private: static void logRecord(const ProxyRecord& r);
    public: static void init(Runtime& rt);
    
    
  // --- FIELDS --- //
    
    private: Ptr< Array<ProxyRecord> > _proxyTable;
    private: Mutex _proxyTableMutex;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: TunnelingClient(Agent* agent);
    
    
  // --- METHODS --- //
    
    private: int getProxyRecordIndexForTarget(const k_guid_t& t) const;
    public: void addTunnel(const k_guid_t& t);
    
    public: void addRoute(const k_guid_t& rt, const k_guid_t& kernel,
        const k_guid_t& tunnel);
    
    public: void addVtrRoute(const k_guid_t& vtr, const k_guid_t& kernel);
    public: void removeRoute(const k_guid_t& rt);
    public: void send(const k_guid_t& receiver, PPtr<Message> msg);
    public: void sendToAll(PPtr<Message> msg);
    
    // Message Handlers //
    private: void handleOpReceive(PPtr<Message> msg);
    private: void handleOpReceiveBcast(PPtr<Message> msg);
    private: void handleOpAddRoute(PPtr<Message> msg);
    private: void handleOpRemoveRoute(PPtr<Message> msg);
    
    // Virtual Methods //
    public: virtual void onReceive(PPtr<KRecord> msg);
    public: virtual void onReceiveBcast(PPtr<KRecord> msg);
    
    public: virtual void onAddRoute(const k_guid_t& remoteRuntime,
        const k_guid_t& remotePeer, const k_guid_t& tunnel);
    
    public: virtual void onConnectionLost(const k_guid_t& rt, const k_guid_t& kernel,
        const k_guid_t& tunnel);    
  };
  
} // namespace protocol
} // namespace knorba

#endif /* defined(__AgentRuntimeEnvironment__TunnelingClient__) */

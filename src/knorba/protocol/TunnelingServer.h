//
//  TunnelingServer.h
//  AgentRuntimeEnvironment
//
//  Created by Kay Khandan on 3/29/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

#ifndef __AgentRuntimeEnvironment__TunnelingServer__
#define __AgentRuntimeEnvironment__TunnelingServer__

// Super
#include "TunnelingProtocol.h"

namespace knorba {
namespace type {
  class KRecord;
}
}

namespace knorba {
namespace protocol {
  
  class TunnelingServer : public TunnelingProtocol {
    
  // --- FIELDS --- //
    
    private: k_guid_t _client;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: TunnelingServer(Agent* agent);
    
    
  // --- METHODS --- //
    
    public: const k_guid_t& getClient() const;
    public: void opReceive(PPtr<KRecord> data);
    public: void opReceiveBcast(PPtr<KRecord> msg);
    public: void opAddRoute(const k_guid_t& remoteRt, const k_guid_t& remoteKernel);
    public: void opRemoveRoute(const k_guid_t& rt);
    
    // Message Handlers //
    private: void handleOpSend(PPtr<Message> msg);
    private: void handleOpSendBcast(PPtr<Message> msg);
    
    // Virtual Methods //
    public: virtual void onSend(PPtr<KRecord> data);
    public: virtual void onSendBcast(PPtr<KRecord> data);
    
    // Inherited from Protocol //
    public: void handlePeerConnectionReuqest(PPtr<KString> role, const k_guid_t& guid);
    
  };
  
} // namespace protocol
} // namespace knorba

#endif /* defined(__AgentRuntimeEnvironment__TunnelingServer__) */

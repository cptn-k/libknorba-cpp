//
//  UnixSocketAgentClient.h
//  AgentRuntimeEnvironment
//
//  Created by Hamed KHANDAN on 4/9/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

#ifndef __AgentRuntimeEnvironment__UnixSocketAgentClient__
#define __AgentRuntimeEnvironment__UnixSocketAgentClient__

#include <knorba/Protocol.h>

namespace knorba {
  class Runtime;
}

namespace knorba {
namespace type {
  class KString;
} // namespace type
} // namespace knorba

namespace knorba {
namespace protocol {

  class UnixSocketClient : public Protocol {
    
  // --- STATIC FIELDS --- //
    
    public: static const SPtr<KString> OP_SET_ADDRESS;
    public: static const SPtr<KString> OP_ADD_CONNECTION;
    
    
  // --- STATIC METHODS --- //
    
    public: static void init(Runtime& rt);
    
    
  // --- FIELDS --- //
    
    private: k_guid_t _server;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: UnixSocketClient(Agent* owner);
    
    
  // --- METHODS --- //
    
    public: void setServer(const k_guid_t& guid);
    public: const k_guid_t& getServer() const;
    public: void setAddress(const string& address);
    public: void addConnections(const string& address);
    
  };
  
} // namespace protocol
} // namespace knorba

#endif /* defined(__AgentRuntimeEnvironment__UnixSocketAgentClient__) */

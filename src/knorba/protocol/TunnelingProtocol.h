//
//  TunnelingProtocol.h
//  AgentRuntimeEnvironment
//
//  Created by Kay Khandan on 3/28/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

#ifndef __AgentRuntimeEnvironment__TunnelingProtocol__
#define __AgentRuntimeEnvironment__TunnelingProtocol__

#include <knorba/Protocol.h>
#include <knorba/type/definitions.h>

namespace knorba {
  class Group;
  class Agent;
}

namespace knorba {
namespace type {
  class KRecordType;
  class KString;
}
}

namespace knorba {
namespace protocol {
  
  class TunnelingProtocol : public Protocol {
    
  // --- NESTED TYPES --- //
    
    private: struct ProxyTableRecord {
      k_guid_t target;
      k_guid_t proxy;
    };

    
  // --- STATIC FIELDS --- //
    
    private: static SPtr<KRecordType> MESSAGE_T;
    public: static const k_octet_t MESSAGE_T_REMOTE_KERNEL  = 0;
    public: static const k_octet_t MESSAGE_T_SENDER         = 1;
    public: static const k_octet_t MESSAGE_T_RECEIVER       = 2;
    public: static const k_octet_t MESSAGE_T_OPCODE         = 3;
    public: static const k_octet_t MESSAGE_T_TRANSACTION_ID = 4;
    public: static const k_octet_t MESSAGE_T_PAYLOAD        = 5;
    
    private: static SPtr<KRecordType> ROUTE_T;
    public: static const k_octet_t ROUTE_T_REMOTE_KERNEL  = 0;
    public: static const k_octet_t ROUTE_T_REMOTE_RUNTIME = 1;
    
    public: static const SPtr<KString> OP_SEND;
    public: static const SPtr<KString> OP_SEND_BCAST;
    public: static const SPtr<KString> OP_RECEIVE;
    public: static const SPtr<KString> OP_RECEIVE_BCAST;
    public: static const SPtr<KString> OP_ADD_ROUTE;
    public: static const SPtr<KString> OP_REMOVE_ROUTE;
    public: static const SPtr<KString> R_TUNNEL;
    public: static const SPtr<KString> R_CLIENT;
    
    
  // --- STATIC METHODS --- //
    
    public: static PPtr<KRecordType> message_t();
    public: static PPtr<KRecordType> route_t();
    public: static void init(Runtime& rt);

    
  // --- (DE)CONSTRUCTORS --- //
    
    public: TunnelingProtocol(Agent* owner);
    
  };
  
} // namespace protocol
} // namespace knorba

#endif /* defined(__AgentRuntimeEnvironment__TunnelingProtocol__) */

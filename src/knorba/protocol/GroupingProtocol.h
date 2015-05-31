//
//  GroupingProtocol.h
//  AgentRuntimeEnvironment
//
//  Created by Hamed KHANDAN on 5/13/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

#ifndef __AgentRuntimeEnvironment__GroupingProtocol__
#define __AgentRuntimeEnvironment__GroupingProtocol__

#include <knorba/Protocol.h>

namespace knorba {
namespace protocol {
  
  class GroupingProtocol : public Protocol {
    
  // --- STATIC FIELDS --- //
    
    public: static const SPtr<KString> OP_HELLO;

    
  // --- STATIC METHODS --- //
    
    public: static void init(Runtime& rt);
    
    
  // --- FIELDS --- //
    
    private: Ptr<KString> _role;
    private: Ptr<KLongint> _groupId;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: GroupingProtocol(Agent* owner, PPtr<KString> role,
        k_longint_t groupId);
    
    
  // --- METHODS --- //
    
    public: void start();
    public: k_integer_t getNMembers() const;
    
    // Virtual Methods //
    public: virtual void onMemberConnected(const k_guid_t& guid);
    public: virtual void onMemberDisconnected(const k_guid_t& guid);
    public: virtual void onAllMembersConnected();
    
    // Handlers //
    private: void handleOpHello(PPtr<Message> msg);
    
    // Inherited from Protocol //
    public: void handlePeerDisconnected(PPtr<KString> role, const k_guid_t& guid);

    
  };
  
} // namespace protocol
} // namespace knorba

#endif /* defined(__AgentRuntimeEnvironment__GroupingProtocol__) */
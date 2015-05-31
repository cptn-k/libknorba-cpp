//
//  PhaserProtocol.h
//  AgentRuntimeEnvironment
//
//  Created by Hamed KHANDAN on 5/21/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

#ifndef __AgentRuntimeEnvironment__PhaserProtocol__
#define __AgentRuntimeEnvironment__PhaserProtocol__

#include <kfoundation/Condition.h>
#include <kfoundation/Mutex.h>
#include <knorba/Protocol.h>

namespace knorba {
namespace protocol {

  using namespace knorba::type;
  
  class PhaserProtocol : public Protocol {
  
  // --- NESTED TYPES --- //
    
    
  // --- STATIC FIELDS --- //
    
    public: static const SPtr<KString> OP_PHASE;
    public: static const SPtr<KString> OP_RELEASE;
    
    
  // --- STATIC METHODS --- //
    
    public: static void init(Runtime& rt);
    
    
  // --- FIELDS --- //
    
    private: Ptr<KLongint> _phase;
    private: Ptr<KString>   _role;
    private: Runtime&       _rt;
    private: Condition      _synchronizeCond;
    private: k_guid_t        _leader;
    private: bool           _isLeader;
    private: bool           _hasLeader;
    private: int            _nRelease;
    private: bool           _stopFlag;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: PhaserProtocol(Agent* agent, PPtr<KString> role);
    
    
  // --- METHODS --- //
    
    private: void releaseAll();
    public: int getNMembers() const;
    public: void reset();
    public: void synchronize();
    public: void timedSynchronize(k_integer_t msecs);
    public: k_longint_t getPhase() const;
    
    // Handlers //
    public: void handleOpPhase(PPtr<Message> msg);
    public: void handleOpRelease(PPtr<Message> msg);
    
    // Inherited from Protocol //
    public: void handlePeerDisconnected(PPtr<KString> role, const k_guid_t& guid);
    public: void finalize();
    public: bool isAlive() const;
    
  };
  
} // namespace protocol
} // namespace knorba

#endif /* defined(__AgentRuntimeEnvironment__PhaserProtocol__) */

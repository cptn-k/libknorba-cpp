//
//  Protocol.h
//  KnoRBA
//
//  Created by Hamed KHANDAN on 2/22/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

#ifndef __KnoRBA__Protocol__
#define __KnoRBA__Protocol__

// Std
#include <map>

// KFoundation
#include <kfoundation/ManagedObject.h>
#include <kfoundation/Array.h>

// Internal
#include "Message.h"

#define PLOG _agent->log()
#define PLOG_ERR _agent->log(::kfoundation::Logger::ERR)
#define PLOG_WRN _agent->log(::kfoundation::Logger::WRN)

namespace knorba {
  
  using namespace std;
  using namespace kfoundation;
  
  class Agent;
  
  class Protocol {
    
  // --- NESTED TYPES --- //
    
    public: typedef void (Protocol::*phandler_t)(PPtr<Message>);
    private: typedef map<k_longint_t, phandler_t> map_t;

    
  // --- FIELDS --- //
    
    private:   map_t _handlerMap;
    protected: Agent* const _agent;

  
  // --- (DE)CONSTRUCTORS --- //
    
    public: Protocol(Agent* _owner);
    public: ~Protocol();
    
    
  // --- METHODS --- //
  
    protected: void registerHandler(phandler_t handler, PPtr<KString> opcode);
    public: phandler_t getHandlerForOpcodeHash(const k_longint_t hash);
    public: virtual void handlePeerConnectionReuqest(PPtr<KString> role, const k_guid_t& guid);
    public: virtual void handlePeerDisconnected(PPtr<KString> role, const k_guid_t& guid);
    public: virtual void finalize();
    public: virtual bool isAlive() const;
    
  };
  
} // namespace knorba

#endif /* defined(__KnoRBA__Protocol__) */
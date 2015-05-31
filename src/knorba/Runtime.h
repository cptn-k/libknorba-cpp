//
//  Runtime.h
//  KnoRBA
//
//  Created by Hamed KHANDAN on 9/19/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#ifndef __KnoRBA__Runtime__
#define __KnoRBA__Runtime__

#include "type/definitions.h"

namespace knorba {
  
  namespace type {
    class KString;
    class KValue;
    class KType;
  }
  
  class Agent;
  class Group;
  
  using namespace type;
  using namespace kfoundation;
  
  class Runtime {
    public: virtual const k_guid_t& getGuid() const = 0;
    public: virtual void registerType(PPtr<KType> type) = 0;
    public: virtual PPtr<KType> getTypeByHash(const k_longint_t hash) const = 0;
    public: virtual const k_guid_t& getConsoleGuid() const = 0;
    public: virtual const string& getAppName() const = 0;
    public: virtual k_integer_t getNodeCount() const = 0;
    public: virtual bool isHead() const = 0;
    public: virtual void signalQuit() = 0;
    
    public: virtual PPtr<Path> getResourcePathForAgent(const Agent* itself)
        const = 0;
    
    public: virtual PPtr<Path> getDataPathForAgent(const Agent* itself)
        const = 0;
    
    public: virtual const string& getClassNameForAgent(const Agent* itself)
        const = 0;
    
    public: virtual const string& getAliasForAgent(const Agent* itself)
        const = 0;
    
    public: virtual const k_guid_t& getAgentGuidByAlias(const string& alias)
        const = 0;
    
    public: virtual void registerMessageFormat(PPtr<KString> opcode,
        PPtr<KType> payloadType) = 0;
      
    public: virtual PPtr<KType> getMessageFormatByHash(const k_longint_t hash)
        const = 0;
    
    public: virtual PPtr<KString> getMessageOpCodeForHash(
        const k_longint_t hash) const = 0;
      
    public: virtual void send(const k_guid_t& sender, const k_guid_t& receiver,
        const k_longint_t opcode, PPtr<KValue> content,
        const k_integer_t tid) = 0;
      
    public: virtual void send(const k_guid_t& sender, PPtr<Group> receivers,
        const k_longint_t opcode, PPtr<KValue> content,
        const k_integer_t tid) = 0;
      
    public: virtual void sendToAll(const k_guid_t& sender,
        const k_longint_t opcode, PPtr<KValue> content,
        const k_integer_t tid) = 0;
      
    public: virtual void sendToLocals(const k_guid_t& sender,
        const k_longint_t opcode, PPtr<KValue> content,
        const k_integer_t tid) = 0;
  };
  
} // namespace knorba

#endif /* defined(__KnoRBA__Runtime__) */

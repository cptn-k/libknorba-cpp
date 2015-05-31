//
//  AbstractAgentLoader.h
//  AgentRuntimeEnvironment
//
//  Created by Kay Khandan on 11/2/14.
//  Copyright (c) 2014 Kay Khandan. All rights reserved.
//

#ifndef AgentRuntimeEnvironment_AbstractAgentLoader_h
#define AgentRuntimeEnvironment_AbstractAgentLoader_h

// KFoundation
#include <kfoundation/Path.h>

// Internal
#include "type/KType.h"
#include "type/KString.h"

namespace knorba {
  
  using namespace knorba::type;
  using namespace kfoundation;
  
  class Agent;
  class Runtime;
  
  class AgentLoader : public ManagedObject, public SerializingStreamer {
  
  // --- FIELDS --- //
    
    private: string _name;
    private: Ptr<Path> _resources;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: AgentLoader(const string& name, PPtr<Path> reosurces);
    
    
  // --- METHODS --- //
    
    public: virtual void init(Runtime& rt) = 0;
    public: virtual Agent* instantiate(Runtime& rt, const k_guid_t& guid) = 0;
    
    public: const string& getClassName() const;
    public: PPtr<Path> getPathToResources() const;
    
    // Inherited from SerializingStreamer
    public: virtual void serialize(PPtr<ObjectSerializer> serializer) const;
  };
  
} // namespace knorba

#endif

//
//  AbstractAgentLoaded.cpp
//  AgentRuntimeEnvironment
//
//  Created by Kay Khandan on 11/3/14.
//  Copyright (c) 2014 Kay Khandan. All rights reserved.
//

// KFoundation
#include <kfoundation/Ptr.h>

// Internal
#include "Runtime.h"

// Self
#include "AgentLoader.h"

namespace knorba {
  
// --- (DE)CONSTRUCTORS --- //
    
  AgentLoader::AgentLoader(const string& name, PPtr<Path> resources)
  {
    _name = name;
    _resources = resources;
  }
  
  
// --- METHODS --- //
    
  const string& AgentLoader::getClassName() const {
    return _name;
  }
  
  
  PPtr<Path> AgentLoader::getPathToResources() const {
    return _resources;
  }
  
  
  void AgentLoader::serialize(PPtr<ObjectSerializer> serializer) const {
    serializer->object("AgentLoader")
      ->attribute("class", _name);
    
    if(!_resources.isNull()) {
      serializer->attribute("resources", _resources->getString());
    }
    
    serializer->endObject();
  }
    
} // namespace knorba
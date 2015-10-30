/*---[AgentLoader.h]-------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : knorba::AgentLoader::*
 |  Implements: -
 |
 |  Copyright (c) 2013, 2014, 2015, RIKEN (The Institute of Physical and
 |  Chemial Research) All rights reserved.
 |
 |  Author: Hamed KHANDAN (hamed.khandan@port.kobe-u.ac.jp)
 |
 |  This file is distributed under the KnoRBA Free Public License. See
 |  LICENSE.TXT for details.
 |
 *//////////////////////////////////////////////////////////////////////////////

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


  /**
   * Subclass to make custom agent loaders. For applications including embeded
   * systems in which agents are not compiled into individual dynamic
   * libraries, this tool can be used to help ARE to instantiate new agents of 
   * a particular type.
   *
   * @headerfile AgentLoader.h <knorba/AgentLoader.h>
   */

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

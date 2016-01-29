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
#include <kfoundation/SerializingStreamer.h>
#include <kfoundation/KFObject.h>

// Internal
#include "type/definitions.h"

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

  class AgentLoader : public KFObject, public SerializingStreamer {
  
  // --- FIELDS --- //
    
    private: RefConst<KString> _name;
    private: RefConst<Path> _resources;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: AgentLoader(RefConst<KString> name, RefConst<Path> reosurces);
    
    
  // --- METHODS --- //
    
    public: virtual RefConst<Ontology> getOntology() = 0;
    public: virtual Agent* instantiate(Communicator& com) = 0;
    
    public: RefConst<KString> getClassName() const;
    public: RefConst<Path> getPathToResources() const;
    
    // Inherited from SerializingStreamer
    public: virtual void serialize(Ref<ObjectSerializer> serializer) const;
    
  };
  
} // namespace knorba

#endif

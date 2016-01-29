/*---[AgentLoader.cpp]-----------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : -
 |  Implements: knorba::AgentLoader::*
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

// KFoundation
#include <kfoundation/Ref.h>
#include <kfoundation/ObjectSerializer.h>
#include <kfoundation/UString.h>
#include <kfoundation/Path.h>

// Internal
#include "Ontology.h"
#include "Communicator.h"
#include "type/KString.h"

// Self
#include "AgentLoader.h"

namespace knorba {
  
// --- (DE)CONSTRUCTORS --- //

  /**
   * Sole constructor.
   *
   * @param name The class name for this agent.
   * @param resources Path to the resouces directory.
   */

  AgentLoader::AgentLoader(RefConst<KString> name, RefConst<Path> resources)
  {
    _name = name;
    _resources = resources;
  }
  
  
// --- METHODS --- //

  /** Returns the agent class name for this loader */

  RefConst<KString> AgentLoader::getClassName() const {
    return _name;
  }


  /** Returns the path to resources directory. */
  
  RefConst<Path> AgentLoader::getPathToResources() const {
    return _resources;
  }
  
  
  void AgentLoader::serialize(Ref<ObjectSerializer> serializer) const {
    serializer->object(K"AgentLoader")
      ->attribute(K"class", *_name);
    
    if(!_resources.isNull()) {
      serializer->attribute(K"resources", *_resources);
    }
    
    serializer->endObject();
  }
    
} // namespace knorba
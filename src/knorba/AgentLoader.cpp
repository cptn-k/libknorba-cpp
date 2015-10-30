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
#include <kfoundation/Ptr.h>

// Internal
#include "Runtime.h"

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

  AgentLoader::AgentLoader(const string& name, PPtr<Path> resources)
  {
    _name = name;
    _resources = resources;
  }
  
  
// --- METHODS --- //

  /** Returns the agent class name for this loader */

  const string& AgentLoader::getClassName() const {
    return _name;
  }


  /** Returns the path to resources directory. */
  
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
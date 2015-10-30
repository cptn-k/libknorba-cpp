/*---[Group.cpp]-----------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : -
 |  Implements: knorba::Group::*
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
#include <kfoundation/Logger.h>
#include <kfoundation/Mutex.h>
#include <kfoundation/Array.h>

// Internal
#include "type/KGuid.h"

#include "Group.h"

namespace knorba {
  
// --- STATIC FIELDS --- //
  
  SPtr<Group> Group::EMPTY_GROUP;
  
  
// --- STATIC METHODS --- //

  /**
   * Returns a constant empty group.
   */

  SPtr<Group> Group::empty_group() {
    if(EMPTY_GROUP.isNull()) {
      EMPTY_GROUP = new Group();
    }
    return EMPTY_GROUP;
  }
  
  
// --- (DE)CONSTRUCTORS --- //

  /**
   * Constructs an empty group.
   */

  Group::Group()
  : _mutex(true)
  {
    // Nothing
  }

  
// --- METHODS --- //
  
  /**
   * Adds a new GUID, if it is not already added. This method is thread safe.
   *
   * @param guid The GUID to add.
   */

  void Group::add(const k_guid_t& guid) {
    KF_SYNCHRONIZED(_mutex,
      if(!containts(guid)) {
        _members.push(guid);
      }
    )
  }


  /**
   * Adds all the GUIDs in the given group to this one, not already added.
   * This method is thread safe.
   *
   * @param group The gruop of GUIDs to add.
   */

  void Group::add(PPtr<Group> group) {
    KF_SYNCHRONIZED(_mutex,
      for(int i = group->_members.getSize() - 1; i >= 0; i--) {
        _members.push(group->_members.at(i));
      }
    )
  }
  

  /**
   * Removes a GUID from this group, if it exists. This method is thread safe.
   *
   * @param guid The GUID to remove.
   */

  void Group::remove(const k_guid_t& guid) {
    KF_SYNCHRONIZED(_mutex,
      int index = _members.indexOf(guid);
      if(index >= 0) {
        _members.remove(index);
      }
    )
  }
  

  /**
   * Removes all GUIDs in this group. This method is thread safe.
   */

  void Group::clear() {
    KF_SYNCHRONIZED(_mutex,
      _members.clear();
    )
  }


  /**
   * Returns the number of unique GUIDs in this group.
   */
  
  int Group::getCount() const {
    return _members.getSize();
  }
  

  /**
   * Returns the GUID at the given index.
   */

  const k_guid_t& Group::get(int index) const {
    return _members.at(index);
  }


  /**
   * Checks if this group contains the given GUID.
   */
  
  bool Group::containts(const k_guid_t& guid) const {
    return _members.indexOf(guid) >= 0;
  }


  /**
   * Checks if this group is empty.
   */
  
  bool Group::isEmpty() const {
    return _members.isEmpty();
  }
  
  
  void Group::serialize(PPtr<kfoundation::ObjectSerializer> builder) const {
    builder->object("Group")
           ->member("members")
           ->collection();
    
    Ptr<KGuid> value = new KGuid();
    for(int i = 0; i < _members.getSize(); i++) {
      value->set(_members.at(i));
      builder->object<KGuid>(value);
    }
    
    builder->endCollection();
    builder->endObject();
  }
  
} // Namespace knorba

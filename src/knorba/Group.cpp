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
#include <kfoundation/Ref.h>
#include <kfoundation/Logger.h>
#include <kfoundation/Mutex.h>
#include <kfoundation/Array.h>

// Internal
#include "type/KGur.h"

#include "Group.h"

namespace knorba {
  
// --- STATIC FIELDS --- //
  
  StaticRefConst<Group> Group::EMPTY_GROUP = new Group();
  

  
// --- (DE)CONSTRUCTORS --- //

  /**
   * Constructs an empty group.
   */

  Group::Group()
  : _mutex(true),
    _members(new Array<k_gur_t>())
  {
    // Nothing
  }

  
// --- METHODS --- //
  
  /**
   * Adds a new GUID, if it is not already added. This method is thread safe.
   *
   * @param guid The GUID to add.
   */

  void Group::add(const k_gur_t& guid) {
    KF_SYNCHRONIZED(_mutex,
      if(!containts(guid)) {
        _members->push(guid);
      }
    )
  }


  /**
   * Adds all the GUIDs in the given group to this one, not already added.
   * This method is thread safe.
   *
   * @param group The gruop of GUIDs to add.
   */

  void Group::add(RefConst<Group> group) {
    KF_SYNCHRONIZED(_mutex,
      for(int i = group->_members->getSize() - 1; i >= 0; i--) {
        _members->push(group->_members->at(i));
      }
    )
  }
  

  /**
   * Removes a GUID from this group, if it exists. This method is thread safe.
   *
   * @param guid The GUID to remove.
   */

  void Group::remove(const k_gur_t& guid) {
    KF_SYNCHRONIZED(_mutex,
      int index = _members->indexOf(guid);
      if(index >= 0) {
        _members->remove(index);
      }
    )
  }
  

  /**
   * Removes all GUIDs in this group. This method is thread safe.
   */

  void Group::clear() {
    KF_SYNCHRONIZED(_mutex,
      _members->clear();
    )
  }


  /**
   * Returns the number of unique GUIDs in this group.
   */
  
  int Group::getCount() const {
    return _members->getSize();
  }
  

  /**
   * Returns the GUID at the given index.
   */

  const k_gur_t& Group::get(int index) const {
    return _members->at(index);
  }


  /**
   * Checks if this group contains the given GUID.
   */
  
  bool Group::containts(const k_gur_t& guid) const {
    return _members->indexOf(guid) >= 0;
  }


  /**
   * Checks if this group is empty.
   */
  
  bool Group::isEmpty() const {
    return _members->isEmpty();
  }
  
  
  void Group::serialize(Ref<ObjectSerializer> builder) const {
    builder->object(K"Group")
           ->member(K"members")
           ->collection();
    
    KGur value;
    Array<k_gur_t>::Iterator i = _members->getIterator();
    for(value.set(i.first()); i.hasMore(); value.set(i.next())) {
      builder->object(value);
    }
    
    builder->endCollection();
    builder->endObject();
  }
  
} // Namespace knorba
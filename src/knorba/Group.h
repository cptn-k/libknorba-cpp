/*---[Group.h]-------------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : knorba::Group::*
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

#ifndef KNORBA_GROUP_H
#define KNORBA_GROUP_H

// KFoundation
#include <kfoundation/Array.h>
#include <kfoundation/Mutex.h>

// Internal
#include "type/definitions.h"

namespace knorba {
  
  using namespace kfoundation;
  using namespace knorba::type;

  /**
   * Represents a group of agents by their GUIDs. add() methods prevent GUIDs
   * to be duplicate.
   *
   * @headerfile Group.h <knorba/Group.h>
   */

  class Group : public ManagedObject, public SerializingStreamer {
  
  // --- STATIC FIELDS --- //
    
    private: static SPtr<Group> EMPTY_GROUP;
    
    
  // --- FIELDS --- //
    
    private: Array<k_guid_t> _members;
    private: Mutex _mutex;
    
    
  // --- STATIC METHODS --- //

    public: static SPtr<Group> empty_group();
    
  
  // --- (DE)CONSTRUCTOR --- //
    
    public: Group();

    
  // --- METHODS --- //
    
    public: void add(const k_guid_t& guid);
    public: void add(PPtr<Group> group);
    public: void remove(const k_guid_t& guid);
    public: void clear();
    public: int getCount() const;
    public: const k_guid_t& get(int index) const;
    public: bool containts(const k_guid_t& guid) const;
    public: bool isEmpty() const;
    
    // From SerializingStreamer
    public: void serialize(PPtr<ObjectSerializer> buidler) const;
    
  };
  
} // namespace knorba

#endif /* defined(KNORBA_GROUP_H) */

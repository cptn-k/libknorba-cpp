//
//  Group.cpp
//  KnoRBA
//
//  Created by Hamed KHANDAN on 10/8/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

// KFoundation
#include <kfoundation/Ptr.h>
#include <kfoundation/Logger.h>
#include <kfoundation/Mutex.h>
#include <kfoundation/Array.h>

// Internal
#include "type/KGlobalUid.h"

#include "Group.h"

namespace knorba {
  
// --- STATIC FIELDS --- //
  
  SPtr<Group> Group::EMPTY_GROUP;
  
  
// --- STATIC METHODS --- //
  
  SPtr<Group> Group::empty_group() {
    if(EMPTY_GROUP.isNull()) {
      EMPTY_GROUP = new Group();
    }
    return EMPTY_GROUP;
  }
  
  
// --- (DE)CONSTRUCTORS --- //
  
  Group::Group()
  : _mutex(true)
  {
    // Nothing
  }

  
// --- METHODS --- //
  
 
  void Group::add(const k_guid_t& guid) {
    KF_SYNCHRONIZED(_mutex,
      if(!containts(guid)) {
        _members.push(guid);
      }
    )
  }
  
  
  void Group::add(PPtr<Group> group) {
    KF_SYNCHRONIZED(_mutex,
      for(int i = group->_members.getSize() - 1; i >= 0; i--) {
        _members.push(group->_members.at(i));
      }
    )
  }
  
  
  void Group::remove(const k_guid_t& guid) {
    KF_SYNCHRONIZED(_mutex,
      int index = _members.indexOf(guid);
      if(index >= 0) {
        _members.remove(index);
      }
    )
  }
  
  
  void Group::clear() {
    KF_SYNCHRONIZED(_mutex,
      _members.clear();
    )
  }
  
  
  int Group::getCount() const {
    return _members.getSize();
  }
  
  
  const k_guid_t& Group::get(int index) const {
    return _members.at(index);
  }
  
  
  bool Group::containts(const k_guid_t& guid) const {
    return _members.indexOf(guid) >= 0;
  }
  
  
  bool Group::isEmpty() const {
    return _members.isEmpty();
  }
  
  
  void Group::serialize(PPtr<kfoundation::ObjectSerializer> builder) const {
    builder->object("Group")
           ->member("members")
           ->collection();
    
    Ptr<KGlobalUid> value = new KGlobalUid();
    for(int i = 0; i < _members.getSize(); i++) {
      value->set(_members.at(i));
      builder->object<KGlobalUid>(value);
    }
    
    builder->endCollection();
    builder->endObject();
  }
  
} // Namespace knorba

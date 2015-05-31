//
//  Group.h
//  KnoRBA
//
//  Created by Hamed KHANDAN on 10/8/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

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
  
}

#endif /* defined(KNORBA_GROUP_H) */

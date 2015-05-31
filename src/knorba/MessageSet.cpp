//
//  MessageSet.cpp
//  KnoRBA
//
//  Created by Hamed KHANDAN on 10/14/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

// KFoundation
#include <kfoundation/Ptr.h>

#include "Message.h"
#include "Group.h"

#include "MessageSet.h"

namespace knorba {
  
// --- (DE)CONSTRUCTORS --- //
  
  MessageSet::MessageSet() {
    _items = new ManagedArray<Message>();
    _senders = new Group();
  }
  
  
  MessageSet::~MessageSet() {
    // Nothing;
  }
  
  
  
// --- METHODS --- //
  
  void MessageSet::add(Ptr<Message> msg) {
    _items->push(msg);
    _senders->add(msg->getSender());
  }
  
  
  int MessageSet::getSize() const {
    return _items->getSize();
  }
  
  
  PPtr<Message> MessageSet::get(int index) const {
    return _items->at(index);
  }
  
  
  PPtr<Group> MessageSet::getSenders() const {
    return _senders;
  }
  
  
  bool MessageSet::isEmpty() const {
    return _items->isEmpty();
  }
  
  void MessageSet::clear() {
    _items->clear();
  }
  
} // namespace knorba
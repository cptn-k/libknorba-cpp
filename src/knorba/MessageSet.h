/*---[MessageSet.h]--------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : knorba::MessageSet::*
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

#ifndef __KnoRBA__MessageSet__
#define __KnoRBA__MessageSet__

#include <kfoundation/Ref.h>
#include <kfoundation/KFObject.h>

#include "type/definitions.h"

namespace kfoundation {
  template<typename T> class RefArray;
}

namespace knorba {
  
  using namespace kfoundation;
  
  class Message;
  class Group;

  /**
   * Container for a collection of messages. 
   *
   * @headerfile MessageSet.h <knorba/MessageSet.h>
   */
  
  class MessageSet : public KFObject {
    
  // --- FIELDS --- //
    
    private: Ref< RefArray<Message> > _items;
    private: Ref<Group> _senders;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: MessageSet();
    
    
  // --- METHODS --- //
    
    public: void add(Ref<Message> msg);
    public: k_integer_t getSize() const;
    public: Ref<Message> get(int index) const;
    public: Ref<Group> getSenders() const;
    public: bool isEmpty() const;
    public: void clear();
    
  };
}


#endif /* defined(__KnoRBA__MessageSet__) */

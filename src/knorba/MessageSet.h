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

#include <kfoundation/ManagedObject.h>
#include <kfoundation/ManagedArray.h>

namespace knorba {
  
  using namespace kfoundation;
  
  class Message;
  class Group;

  /**
   * Container for a collection of messages. 
   *
   * @headerfile MessageSet.h <knorba/MessageSet.h>
   */
  
  class MessageSet : public ManagedObject {
    
  // --- FIELDS --- //
    
    private: Ptr< ManagedArray<Message> > _items;
    private: Ptr<Group> _senders;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: MessageSet();
    
    
  // --- METHODS --- //
    
    public: void add(Ptr<Message> msg);
    public: int getSize() const;
    public: PPtr<Message> get(int index) const;
    public: PPtr<Group> getSenders() const;
    public: bool isEmpty() const;
    public: void clear();
    
  };
}


#endif /* defined(__KnoRBA__MessageSet__) */

//
//  MessageSet.h
//  KnoRBA
//
//  Created by Hamed KHANDAN on 10/14/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#ifndef __KnoRBA__MessageSet__
#define __KnoRBA__MessageSet__

#include <kfoundation/ManagedObject.h>
#include <kfoundation/ManagedArray.h>

namespace knorba {
  
  using namespace kfoundation;
  
  class Message;
  class Group;
  
  class MessageSet : public ManagedObject {
    
  // --- FIELDS --- //
    
    private: Ptr< ManagedArray<Message> > _items;
    private: Ptr<Group> _senders;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: MessageSet();
    public: ~MessageSet();
    
    
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

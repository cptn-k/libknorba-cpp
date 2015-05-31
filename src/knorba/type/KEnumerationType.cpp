//
//  KEnumerationType.cpp
//  CellMonitorTest-XCodeWrapper
//
//  Created by Hamed KHANDAN on 7/24/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

// KFoundation
#include <kfoundation/Ptr.h>
#include <kfoundation/Int.h>
#include <kfoundation/Array.h>
#include <kfoundation/System.h>

// Internal
#include "KEnumeration.h"
#include "KString.h"

// Self
#include "KEnumerationType.h"


namespace knorba {
namespace type {

  using namespace std;
  using namespace kfoundation;


//\/ KEnumerationType::Item /\/////////////////////////////////////////////////
  
  KEnumerationType::Item::Item(k_octet_t o, const string& s)
    : ordinal(o),
      symbol(s),
      hash(KString::generateHashFor(s))
  {
    // Nothing;
  }
  
  
//\/ KEnumerationType /\///////////////////////////////////////////////////////
  
// --- (DE)CONSTRUCTORS --- //
  
  KEnumerationType::KEnumerationType(const string& name)
  : KType(name),
    _items(new ManagedArray<Item>())
  {
    // Nothing;
  }
  
  
  KEnumerationType::~KEnumerationType() {
    // Nothing;
  }
  
  
// --- METHODS --- //
  
  PPtr<KEnumerationType> KEnumerationType::addMember(
      k_octet_t ordinal, const string& label)
  {
    _items->push(new Item(ordinal, label));
    return getPtr().AS(KEnumerationType);
  }
  
  
  PPtr<KEnumerationType> KEnumerationType::addMember(const string &label) {
    _items->push(new Item(getMaxOrdinal() + 1, label));
    return getPtr().AS(KEnumerationType);
  }
  
  
  string KEnumerationType::getLabelForOrdinal(k_octet_t ordinal) const {
    for(int i = _items->getSize() - 1; i >= 0; i--) {
      if(_items->at(i)->ordinal == ordinal) {
        return _items->at(i)->symbol;
      }
    }
    return "";
  }
  
  
  int KEnumerationType::getOrdinalForLabel(const string &label) const {
    k_longint_t hash = KString::generateHashFor(label);
    for(int i = _items->getSize() - 1; i >= 0; i--) {
      if(_items->at(i)->hash == hash) {
        return _items->at(i)->ordinal;
      }
    }
    return -1;
  }
  
  
  k_octet_t KEnumerationType::getNumberOfMembers() const {
    return _items->getSize();
  }
  
  
  k_octet_t KEnumerationType::getMaxOrdinal() const {
    k_octet_t max = 0;
    for(int i = _items->getSize() - 1; i >= 0; i--) {
      if(_items->at(i)->ordinal > max) {
        max = _items->at(i)->ordinal;
      }
    }
    return max;
  }
  
  
  Ptr< Array<k_octet_t> > KEnumerationType::getAllOrdinals() const {
    Ptr< Array<k_octet_t> > res(new Array<k_octet_t>());
    int n = _items->getSize();
    for(int i = 0; i < n; i++) {
      res->push(_items->at(i)->ordinal);
    }
    return res;
  }
  
  
  string KEnumerationType::getLabelForMemberAtIndex(const k_octet_t index)
  const
  {
    return _items->at(index)->symbol;
  }
  
  
  k_octet_t KEnumerationType::getOrdinalForMemberAtIndex(const k_octet_t index)
  const
  {
    return _items->at(index)->ordinal;
  }
  
  
  k_octet_t KEnumerationType::getOrdinalForValueAtAddress(
      const k_octet_t* const addr) const
  {
    return *(addr);
  }
  
  
  string KEnumerationType::getLabelForValueAtAddress(
      const k_octet_t* const addr) const
  {
    return getLabelForOrdinal(*(addr));
  }
  
  
  void KEnumerationType::setValueAtAddressWithOrdinal(k_octet_t* const addr,
      const k_octet_t ordinal) const
  {
    *(addr) = ordinal;
  }
  
  
  void KEnumerationType::setValueAtAddressWithLabel(k_octet_t* const addr,
      const string& label) const
  {
    *(addr) = getOrdinalForLabel(label);
  }
  

  bool KEnumerationType::isCastableTo(PPtr<KType> t) const {
    if(t->equals(KType::OCTET)) {
      return true;
    }
    
    if(t->equals(KType::INTEGER)) {
      return true;
    }
    
    if(t->equals(KType::LONGINT)) {
      return true;
    }
    
    if(t->equals(KType::REAL)) {
      return true;
    }
    
    return t->equals(getPtr().AS(KType).retain());
  }
  
  
  bool KEnumerationType::isAutomaticCastableTo(PPtr<KType> t) const {
    return isCastableTo(t);
  }
  
  
  bool KEnumerationType::equals(PPtr<KType> t) const {
    if(!t.ISA(KEnumerationType)) {
      return false;
    }
    
    Ptr<KEnumerationType> other = t.AS(KEnumerationType);
    
    int n = _items->getSize();

    if(other->_items->getSize() != n) {
      return false;
    }
    
    for(int i = 0; i < n; i++) {
      if(_items->at(i)->ordinal != other->_items->at(i)->ordinal) {
        return false;
      }
      if(_items->at(i)->hash != other->_items->at(i)->hash) {
        return false;
      }
    }
    
    return true;
  }
  
  
  int KEnumerationType::getSizeInOctets() const {
    return 1;
  }
  
  
  bool KEnumerationType::isPrimitive() const {
    return false;
  }
  
  
  bool KEnumerationType::hasConstantSize() const {
    return true;
  }
  
  
  Ptr<KValue> KEnumerationType::instantiate() const {
    return new KEnumeration(getPtr().AS(KEnumerationType));
  }
  
  
  string KEnumerationType::toKnois() const {
    string str = getTypeName() + " IS enum(";
    int n = _items->getSize();
    for(int i = 0; i < n; i++) {
      if(i > 0) {
        str += ", ";
      }
      PPtr<Item> item = _items->at(i);
      str += item->symbol + ":" + Int(item->ordinal);
    }
    str += ").";
    
    return str;
  }
  
  
} // namespace type
} // namespace knorba

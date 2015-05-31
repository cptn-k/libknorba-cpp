//
//  KRecordType.cpp
//  CellMonitorTest-XCodeWrapper
//
//  Created by Hamed KHANDAN on 7/24/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

// KFoundation
#include <kfoundation/Ptr.h>

// Internal
#include "KRecord.h"
#include "KString.h"
#include "KGridType.h"

// Self
#include "KRecordType.h"

namespace knorba {
namespace type {

//\/ KRecordType::Variable /\//////////////////////////////////////////////////
  
  KRecordType::Field::Field(const string& name, PPtr<KType> type,
                                  unsigned int byteOffset)
  : _name(name),
    _byteOffset(byteOffset)
  {
    _type = type;
    _type.setAutorelease(true);
  }
  

//\/ KRecordType /\////////////////////////////////////////////////////////////
  
// --- (DE)CONSTRUCTORS --- //
  
  KRecordType::KRecordType(const string& name)
  : KType(name),
    _fields(new ManagedArray<Field>()),
    _size(0),
    _hasDynamicFields(false)
  {
    // Nothing;
  }
  
  
  KRecordType::KRecordType(PPtr<KType> fieldType)
  : KType(fieldType->getTypeName() + "Record"),
    _fields(new ManagedArray<Field>()),
    _size(0),
    _hasDynamicFields(false)
  {
    addField("field", fieldType);
  }

  
  KRecordType::~KRecordType() {
    // Nothing;
  }
  
  
// --- METHODS --- //
  
  PPtr<KRecordType> KRecordType::addField(const string& name, Ptr<KType> type)
  {
    _offsetTable[_fields->getSize()] = _size;
    _fields->push(Ptr<Field>(new Field(name, type, _size)));
    
    if(type->hasConstantSize()) {
      _size += type->getSizeInOctets();
    } else {
      _size += sizeof(Ptr<KValue>);
    }
    
    if(type.ISA(KRecordType)) {
      _hasDynamicFields = _hasDynamicFields || type.AS(KRecordType)->hasDynamicFields();
    } else {
      _hasDynamicFields = _hasDynamicFields || !type->hasConstantSize();
    }
    
    return getPtr().AS(KRecordType);
  }
  
  
  int KRecordType::getNumberOfFields() const {
    return _fields->getSize();
  }
  
  
  string KRecordType::getNameOfFieldAtIndex(const int i) const {
    return _fields->at(i)->_name;
  }
  
  
  PPtr<KType> KRecordType::getTypeOfFieldAtIndex(const int i) const {
    return _fields->at(i)->_type;
  }
  
  
  PPtr<KType> KRecordType::getTypeOfFieldWithName(const string &name) const {
    return getTypeOfFieldAtIndex((getIndexForFieldWithName(name)));
  }
  
  
  int KRecordType::getIndexForFieldWithName(const string &name) const {
    for(int i = _fields->getSize() - 1; i >= 0; i--) {
      if(_fields->at(i)->_name == name) {
        return i;
      }
    }
    
    return -1;
  }
  
  
  unsigned int KRecordType::getOffsetOfFieldAtIndex(const int index) const {
    return _fields->at(index)->_byteOffset;
  }
  
  
  bool KRecordType::hasDynamicFields() const {
    return _hasDynamicFields;
  }
  
  
  const k_octet_t* const KRecordType::getOffsetTable() const {
    return _offsetTable;
  }
  
  
  Ptr<KGridType> KRecordType::makeGridType(k_octet_t nDims) const {
    return new KGridType(getPtr().AS(KRecordType), nDims);
  }
  

  bool KRecordType::isCastableTo(PPtr<KType> t) const {
    if(!t.ISA(const KRecordType)) {
      return false;
    }
    
    Ptr<KRecordType> other = t.AS(KRecordType);
    if(other->getNumberOfFields() != getNumberOfFields()) {
      return false;
    }
    
    for(int i = _fields->getSize() - 1; i >= 0; i--) {
      if(!getTypeOfFieldAtIndex(i)->isCastableTo(other->getTypeOfFieldAtIndex(i))) {
        return false;
      }
    }
    
    return true;
  }
  
  
  bool KRecordType::isAutomaticCastableTo(PPtr<KType> t) const {
    if(!t.ISA(KRecordType)) {
      return false;
    }
    
    Ptr<KRecordType> other = t.AS(KRecordType);
    if(other->getNumberOfFields() != getNumberOfFields()) {
      return false;
    }
    
    for(int i = _fields->getSize() - 1; i >= 0; i--) {
      if(!getTypeOfFieldAtIndex(i)->isAutomaticCastableTo(other->getTypeOfFieldAtIndex(i))) {
        return false;
      }
    }
    
    return true;
  }
  
  
  bool KRecordType::equals(PPtr<KType> t) const {
    if(!t.ISA(KRecordType)) {
      return false;
    }
    
    Ptr<KRecordType> other = t.AS(KRecordType);
    if(other->getNumberOfFields() != getNumberOfFields()) {
      return false;
    }
    
    for(int i = _fields->getSize() - 1; i >= 0; i--) {
      if(!getTypeOfFieldAtIndex(i)->equals(other->getTypeOfFieldAtIndex(i))) {
        return false;
      }
    }
    
    return true;
  }
  
  
  int KRecordType::getSizeInOctets() const {
    return _size;
  }
  
  
  bool KRecordType::isPrimitive() const {
    return false;
  }
  
  
  bool KRecordType::hasConstantSize() const {
    return true;
  }
  
  
  Ptr<KValue> KRecordType::instantiate() const {
    return new KRecord(getPtr().AS(KRecordType));
  }
  
  
  string KRecordType::toKnois() const {
    string str = getTypeName() + " IS record(";
    
    int n = _fields->getSize();
    for(int i = 0; i < n; i++) {
      if(i > 0) {
        str += ", ";
      }
      str += _fields->at(i)->_type->getTypeName();
    }
    str += ")";
    
    return str;
  }

} // namespace type
} // namesapce knorba
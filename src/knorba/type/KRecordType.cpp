/*---[KGridType.cpp]-------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : -
 |  Implements: knorba::type::KRecordType::*
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
  
  /**
   * Constructor.
   *
   * @param name Type name
   */

  KRecordType::KRecordType(const string& name)
  : KType(name),
    _fields(new ManagedArray<Field>()),
    _size(0),
    _hasDynamicFields(false)
  {
    // Nothing;
  }


  /**
   * Shortcut constructor for records with signle fields. The type name and
   * field name is automatically infered from the type name of the field.
   *
   * @param fieldType Type of the record's sole field.
   */
  
  KRecordType::KRecordType(PPtr<KType> fieldType)
  : KType(fieldType->getTypeName() + "Record"),
    _fields(new ManagedArray<Field>()),
    _size(0),
    _hasDynamicFields(false)
  {
    addField("field", fieldType);
  }

  
// --- METHODS --- //

  /**
   * Adds a new field to record type represented by this object.
   *
   * @param name Field name.
   * @param type Field type.
   * @return Pointer to self.
   */

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


  /**
   * Returns the number of fields of the record represented by this object.
   */
  
  int KRecordType::getNumberOfFields() const {
    return _fields->getSize();
  }
  

  /**
   * Returns the name of the field at the given index.
   *
   * @param i An index between 0 and getNumberOfFields() - 1.
   */

  string KRecordType::getNameOfFieldAtIndex(const int i) const {
    return _fields->at(i)->_name;
  }


  /**
   * Returns the type of the field at the given index.
   *
   * @param i An index between 0 and getNumberOfFields() - 1.
   */
  
  PPtr<KType> KRecordType::getTypeOfFieldAtIndex(const int i) const {
    return _fields->at(i)->_type;
  }


  /**
   * Returns the type of the field with the given name. Returns a null pointer
   * if such field does not exist.
   *
   * @param name The name of the field to retrieve type for.
   */
  
  PPtr<KType> KRecordType::getTypeOfFieldWithName(const string& name) const {
    int index = getIndexForFieldWithName(name);
    if(index < 0) {
      return NULL;
    }
    return getTypeOfFieldAtIndex(index);
  }
  

  /**
   * Returns the index of the field with the given name. Returns -1 if there
   * is no such field.
   *
   * @param name The name of the field to retrieve index of.
   */

  int KRecordType::getIndexForFieldWithName(const string& name) const {
    for(int i = _fields->getSize() - 1; i >= 0; i--) {
      if(_fields->at(i)->_name == name) {
        return i;
      }
    }
    
    return -1;
  }
  

  /**
   * Memory storage helper method. Returns the offset at which the field with
   * the given index is stored, calculated from the point at which the first
   * field is stored.
   *
   * @param index An Index between 0 to getNumberOfFields() - 1.
   */

  unsigned int KRecordType::getOffsetOfFieldAtIndex(const int index) const {
    return _fields->at(index)->_byteOffset;
  }


  /**
   * Returns `true` iff at least one the fields of the record represented by
   * this object has dynamic length. That is, at least one of the fields is
   * is of type `string`, `raw`, `grid`, or a `record` for which 
   * hasDynamicFields() returns `true`.
   */
  
  bool KRecordType::hasDynamicFields() const {
    return _hasDynamicFields;
  }
  

  const k_octet_t* const KRecordType::getOffsetTable() const {
    return _offsetTable;
  }


  /**
   * Returns a KGridType with cells of the type represented by this object.
   * 
   * @param nDims The number of dimensions of the resulting grid type.
   */
  
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
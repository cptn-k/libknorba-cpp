/*---[KRecord.cpp]---------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : -
 |  Implements: knorba::type::KRecord::*
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

// Std
#include <cassert>
#include <cstring>

// KFoundation
#include <kfoundation/Ref.h>
#include <Kfoundation/UString.h>
#include <kfoundation/System.h>

// Internal
#include "KRecordType.h"
#include "KTruth.h"
#include "KOctet.h"
#include "KInteger.h"
#include "KLongint.h"
#include "KReal.h"
#include "KGur.h"
#include "KEnumeration.h"
#include "KEnumerationType.h"
#include "KAny.h"
#include "KRaw.h"
#include "KString.h"
#include "KGrid.h"
#include "KTypeMismatchException.h"

// Self
#include "KRecord.h"

namespace knorba {
namespace type {

  using namespace kfoundation;


//\/ FIELDS /\\\\\\////////////////////////////////////////////////////////////

#define ENUMERAND(X, Y, N) \
  class K ## X ## Field : public K ## X { \
    private: Ref<KRecord> _owner; \
    private: const kf_int16_t _offset; \
    public: K ## X ## Field(Ref<KRecord> owner, const kf_int16_t offset) \
    : _owner(owner), _offset(offset) {} \
    \
    protected: k_octet_t* getBuffer() { \
      return _owner->getBuffer() + _offset; \
    } \
    \
    protected: const k_octet_t* getBuffer() const { \
      return _owner->getBuffer() + _offset; \
    }\
  };
ENUMERATE_OVER_PRIMITIVE_TYPES
ENUMERATE_OVER_SIZABLE_TYPES
#undef ENUMERAND


  class KEnumerationField : public KEnumeration {
    private: Ref<KRecord> _owner;
    private: const kf_int16_t _offset;

    public: KEnumerationField(RefConst<KEnumerationType> type,
        Ref<KRecord> owner, const kf_int16_t offset)
    : KEnumeration(type),
      _owner(owner),
      _offset(offset)
    {
      // Nothing
    }

    protected: k_octet_t* getBuffer() {
      return _owner->getBuffer() + _offset;
    }

    protected: const k_octet_t* getBuffer() const {
      return _owner->getBuffer() + _offset;
    }
  };


  class KRecordField : public KRecord {
    private: Ref<KRecord> _owner;
    private: const kf_int16_t _offset;

    public: KRecordField(RefConst<KRecordType> type, Ref<KRecord> owner,
        const kf_int16_t offset)
    : KRecord(type),
      _owner(owner),
      _offset(offset)
    {
      // Nothing
    }

    protected: k_octet_t* getBuffer() {
      return _owner->getBuffer() + _offset;
    }

    protected: const k_octet_t* getBuffer() const {
      return _owner->getBuffer() + _offset;
    }
  };


  class KGridField : public KGrid {
    private: Ref<KRecord> _owner;
    private: const kf_int16_t _offset;

    public: KGridField(RefConst<KGridType> type, Ref<KRecord> owner,
        const kf_int16_t offset)
    : KGrid(type),
      _owner(owner),
      _offset(offset)
    {
      // Nothing
    }

    protected: k_octet_t* getBuffer() {
      return _owner->getBuffer() + _offset;
    }

    protected: const k_octet_t* getBuffer() const {
      return _owner->getBuffer() + _offset;
    }
  };

  
//\/ KRecord /\////////////////////////////////////////////////////////////////
  
// --- (DE)CONSTRUCTORS --- //

  KRecord::KRecord(RefConst<KRecordType> type, bool unused) {
    _type = type;
    _offsetTable = _type->getOffsetTable();
    _nFields = type->getNumberOfFields();
    _typeTable = new k_octet_t[_nFields];
    _data = NULL;
    initTypeTable();
  }


  /**
   * Primary constructor; creates internal storage for a record of the given
   * type.
   *
   * @param type This record's type.
   */

  KRecord::KRecord(RefConst<KRecordType> type) {
    _type = type;
    _offsetTable = type->getOffsetTable();
    _nFields = type->getNumberOfFields();
    _typeTable = new k_octet_t[_nFields];
    _data = new k_octet_t[type->getSizeInOctets()];
    _type->initializeBuffer(_data);
    initTypeTable();
  }


  /**
   * Deconstructor. Frees any used memory.
   */
  
  KRecord::~KRecord() {
    delete[] _typeTable;

    if(NOT_NULL(_data)) {
      reset();
      delete[] _data;
    }
  }
  
  
// --- METHODS --- //
  
  void KRecord::initTypeTable() {
    for(int i = _nFields - 1; i >= 0; i--) {
      RefConst<KType> type = _type->getTypeOfFieldAtIndex(i);
      if(type->equals(KType::TRUTH)) {
        _typeTable[i] = 1;
      } else if(type->equals(KType::OCTET)) {
        _typeTable[i] = 2;
      } else if(type->equals(KType::INTEGER)) {
        _typeTable[i] = 3;
      } else if(type->equals(KType::LONGINT)) {
        _typeTable[i] = 4;
      } else if(type->equals(KType::REAL)) {
        _typeTable[i] = 5;
      } else if(type->equals(KType::GUR)) {
        _typeTable[i] = 6;
      } else if(type->equals(KType::STRING)) {
        _typeTable[i] = 7;
      } else if(type->equals(KType::RAW)) {
        _typeTable[i] = 8;
      } else if(type->equals(KType::ANY)) {
        _typeTable[i] = 9;
      } else if(type.ISA(KEnumerationType)) {
        _typeTable[i] = 10;
      } else if(type.ISA(KRecordType)) {
        _typeTable[i] = 11;
      } else if(type.ISA(KGridType)) {
        _typeTable[i] = 12;
      } else {
        throw KFException(K"Unrecognized type: " + *type);
      }
    }
  }


  const k_octet_t* KRecord::getBuffer() const {
    return _data;
  }


  k_octet_t* KRecord::getBuffer() {
    return _data;
  }


  #define ENUMERAND1(X, Y, N) \
  case N: return new K ## X ## Field(this, _offsetTable[index]);

  #define ENUMERAND2(X, Y, N) \
  case N: return new K ## Enumeration ## Field( \
    _type->getTypeOfFieldAtIndex(index).AS(K ## Enumeration ## Type), this, \
    _offsetTable[index]);


  Ref<KValue> KRecord::getField(const k_octet_t index) {
    if(index < 0 || index >= _nFields) {
      throw IndexOutOfBoundException(K"Attemp to access field with index "
          + index + " of a record with " + _nFields + " fields.");
    }

    switch(_typeTable[index]) {
      #define ENUMERAND ENUMERAND1
      ENUMERATE_OVER_PRIMITIVE_TYPES
      ENUMERATE_OVER_SIZABLE_TYPES
      #undef ENUMERAND
      #define ENUMERAND ENUMERAND2
      ENUMERATE_OVER_COMPLEX_TYPES
      #undef ENUMERAND
    }

    throw KFException(K"This is not supposed to happen.");
  }


  RefConst<KValue> KRecord::getField(const k_octet_t index) const {
    if(index < 0 || index >= _nFields) {
      throw IndexOutOfBoundException(K"Attemp to access field with index "
          + index + " of a record with " + _nFields + " fields.");
    }

    switch(_typeTable[index]) {
      #define ENUMERAND ENUMERAND1
      ENUMERATE_OVER_PRIMITIVE_TYPES
      ENUMERATE_OVER_SIZABLE_TYPES
      #undef ENUMERAND
      #define ENUMERAND ENUMERAND2
      ENUMERATE_OVER_COMPLEX_TYPES
      #undef ENUMERAND
    }

    throw KFException(K"This is not supposed to happen.");
  }


  #undef ENUMERAND1
  #undef ENUMERAND2


  Ref<KValue> KRecord::getField(RefConst<UString> name) {
    k_integer_t index = _type->getIndexForFieldWithName(name);
    if(index == KF_NOT_FOUND) {
      throw KFException(K"Field \"" + name + "\" does not exist.");
    }
    return getField(index);
  }


  RefConst<KValue> KRecord::getField(RefConst<UString> name) const {
    k_integer_t index = _type->getIndexForFieldWithName(name);
    if(index == KF_NOT_FOUND) {
      throw KFException(K"Field \"" + name + "\" does not exist.");
    }
    return getField(index);
  }


  #define ENUMERAND(X, Y, N) \
  Y KRecord::get ## X(k_octet_t index) const { \
    if(index < 0 || index >= _nFields) { \
      throw IndexOutOfBoundException(K"Attemp to access field with index " \
          + index + " of a record with " + _nFields + " fields."); \
    } \
    \
    if(_typeTable[index] != N) { \
      throw KFException(K"Attempt to read a field of type " \
          + *_type->getTypeOfFieldAtIndex(index) + " as a " #Y); \
    } \
    \
    return *(Y*)(getBuffer() + _offsetTable[index]); \
  }
  ENUMERATE_OVER_PRIMITIVE_TYPES
  #undef ENUMERAND


  #define ENUMERAND(X, Y, N) \
  Y KRecord::get ## X(RefConst<UString> name) const { \
    k_integer_t index = _type->getIndexForFieldWithName(name); \
    \
    if(index == KF_NOT_FOUND) { \
      throw IndexOutOfBoundException(K"Field \"" + name + "\" does not exist."); \
    } \
    \
    if(_typeTable[index] != N) { \
      throw KFException(K"Attempt to read a field of type " \
        + *_type->getTypeOfFieldAtIndex(index) + " as a " #Y); \
    } \
    \
    return *(Y*)(getBuffer() + _offsetTable[index]); \
  }
  ENUMERATE_OVER_PRIMITIVE_TYPES
  #undef ENUMERAND


  #define ENUMERAND(X, Y, N) \
  void KRecord::set ## X(const k_octet_t index, const Y value) { \
    if(index < 0 || index >= _nFields) { \
      throw IndexOutOfBoundException(K"Attemp to access field with index " \
        + index + " of a record with " + _nFields + " fields."); \
    } \
    \
    if(_typeTable[index] != N) { \
      throw KFException(K"Attempt to read a field of type " \
        + *_type->getTypeOfFieldAtIndex(index) + " as a " #Y); \
    } \
    \
    *(Y*)(getBuffer() + _offsetTable[index]) = value; \
  }
  ENUMERATE_OVER_PRIMITIVE_TYPES
  #undef ENUMERAND


  #define ENUMERAND(X, Y, N)\
  void KRecord::set ## X(const Y value) { \
    set ## X(0, value);\
  }
  ENUMERATE_OVER_PRIMITIVE_TYPES
  #undef ENUMERAND


  #define ENUMERAND(X, Y, N) \
  void KRecord::set ## X(RefConst<UString> name, const Y value) { \
    k_integer_t index = _type->getIndexForFieldWithName(name); \
    \
    if(index == KF_NOT_FOUND) { \
      throw IndexOutOfBoundException(K"Field \"" + name + "\" does not exist."); \
    } \
    \
    if(_typeTable[index] != N) { \
      throw KFException(K"Attempt to read a field of type " \
        + *_type->getTypeOfFieldAtIndex(index) + " as a " #Y); \
    } \
    \
    *(Y*)(getBuffer() + _offsetTable[index]) = value; \
  }
  ENUMERATE_OVER_PRIMITIVE_TYPES
  #undef ENUMERAND


  void KRecord::set(const KRecord& r) {
    _type->copyBuffer(r.getBuffer(), getBuffer());
  }


  void KRecord::reset() {
    _type->cleanupBuffer(getBuffer());
  }

  
  // From KValue
  
  void KRecord::set(RefConst<KValue> other) {
    if(!other->getType()->equals(_type.AS(KType))) {
      throw KTypeMismatchException(_type.AS(KType), other->getType());
    }
    _type->copyBuffer(other.AS(KRecord)->getBuffer(), getBuffer());
  }
  
  
  RefConst<KType> KRecord::getType() const {
    return _type.AS(KType);
  }
  
  
//  k_longint_t KRecord::getTotalSizeInOctets() const {
//    if(!_type->hasDynamicFields()) {
//      return _type->getSizeInOctets();
//    }
//    
//    int n = _type->getNumberOfFields();
//    
//    k_longint_t s = 0;
//    for(int i = 0; i < n; i++) {
//      s += _fields->at(i)->getTotalSizeInOctets();
//    }
//    
//    return s;
//  }
//  
//  
//  void KRecord::readFromBinaryStream(Ref<InputStream> input) {
//    int n = _type->getNumberOfFields();
//    for(int i = 0; i < n; i++) {
//      _fields->at(i)->readFromBinaryStream(input);
//    }
//  }
//  
//  
//  /**
//   * Reads the value of this record from the given stream.
//   *
//   * @note If this record has fields of type `any`, make sure to call 
//   *       `setRuntime()` before calling this method. Failure to do so will
//   *       cause an exception to be thrown.
//   */
//
//  void KRecord::writeToBinaryStream(Ref<OutputStream> output) const {
//    int n = _type->getNumberOfFields();
//    for(int i = 0; i < n; i++) {
//      _fields->at(i)->writeToBinaryStream(output);
//    }
//  }
//
//
//  void KRecord::deserialize(Ref<ObjectToken> head) {
//    head->checkClass(K"KRecord");
//    
//    Ref<Token> token = head->next();
//    
//    int index = 0;
//    while(!token->is(EndObjectToken::TYPE)) {
//      token->validateType(ObjectToken::TYPE);
//      Ref<ObjectToken> object = token.AS(ObjectToken);
//      
//      _fields->at(index)->deserialize(token.AS(ObjectToken));
//      index++;
//      
//      token = token->next();
//    }
//    
//    token->validateType(EndObjectToken::TYPE);
//  }
//  
//  
//  void KRecord::serialize(Ref<ObjectSerializer> builder) const {
//    builder->object(K"KRecord");
//    builder->attribute(K"type", _type->getTypeName());
//    
//    const int len = _type->getNumberOfFields();
//    
//    for(int i = 0; i < len; i++) {
//      builder->member(_type->getNameOfFieldAtIndex(i))
//          ->object<KValue>(_fields->at(i));
//    }
//    
//    builder->endObject();
//  }

//\////////////////////////////////////////////////////////////////////////////
  
} // namespace type
} // namespace knorba
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

// KFoundation
#include <kfoundation/ObjectStreamReader.h>
#include <kfoundation/Path.h>

// Internal
#include "../Runtime.h"
#include "KRecordType.h"
#include "KGrid.h"
#include "KTruth.h"
#include "KOctet.h"
#include "KInteger.h"
#include "KLongint.h"
#include "KReal.h"
#include "KGuid.h"
#include "KEnumeration.h"
#include "KEnumerationType.h"
#include "KAny.h"
#include "KRaw.h"
#include "KString.h"
#include "KTypeMismatchException.h"

// Self
#include "KRecord.h"

#define KRECORD_DATA (NOT_NULL(_data)?_data:(_owner->getBaseAddress() + _offset))

namespace knorba {
namespace type {

  using namespace std;
  using namespace kfoundation;


//\/ KTruthField /\////////////////////////////////////////////////////////////
  
  class KTruthField : public KTruth {
    
  // --- FIELDS --- //
    
    private: PPtr<KRecord> _owner;
    private: const k_octet_t _offset;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: KTruthField(PPtr<KRecord> owner, const k_octet_t offset);
    
    
  // --- METHODS --- //
    
    // Inherited from KTruth
    public: void set(const k_truth_t v);
    public: k_truth_t get() const;
    
  };
  
  
  KTruthField::KTruthField(PPtr<KRecord> owner, const k_octet_t offset)
  : _offset(offset)
  {
    _owner = owner;
  }
  
  
  void KTruthField::set(const k_truth_t v) {
    *((k_truth_t*)(_owner->getBaseAddress() + _offset)) = v;
  }
  
  
  k_truth_t KTruthField::get() const {
    return *((k_truth_t*)(_owner->getBaseAddress() + _offset));
  }
  
  
  //\/ KIntegerField /\//////////////////////////////////////////////////////////
  
  class KIntegerField : public KInteger {
    
  // --- FIELDS --- //
    
    private: PPtr<KRecord> _owner;
    private: const k_octet_t _offset;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: KIntegerField(PPtr<KRecord> owner, const k_octet_t offset);
    public: ~KIntegerField();
    
    
  // --- METHODS --- //
    
    // Inherited from KInteger
    public: void set(const k_integer_t v);
    public: k_integer_t get() const;
    
  };
  
  
  KIntegerField::KIntegerField(PPtr<KRecord> owner, const k_octet_t offset)
  : _offset(offset)
  {
    _owner = owner;
  }
  
  
  KIntegerField::~KIntegerField() {
    // Nothing;
  }
  
  
  void KIntegerField::set(const k_integer_t v) {
    *((k_integer_t*)(_owner->getBaseAddress() + _offset)) = v;
  }
  
  
  k_integer_t KIntegerField::get() const {
    return *((k_integer_t*)(_owner->getBaseAddress() + _offset));
  }
  
  
  //\/ KOctetField /\//////////////////////////////////////////////////////////
  
  class KOctetField : public KOctet {
    
  // --- FIELDS --- //
    
    private: PPtr<KRecord> _owner;
    private: const k_octet_t _offset;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: KOctetField(PPtr<KRecord> owner, const k_octet_t offset);
    
    
  // --- METHODS --- //
    
    // Inherited from KOctet
    public: void set(const k_octet_t v);
    public: k_octet_t get() const;
    
  };
  
  
  KOctetField::KOctetField(PPtr<KRecord> owner, const k_octet_t offset)
  : _offset(offset)
  {
    _owner = owner;
  }
  
  
  void KOctetField::set(const k_octet_t v) {
    *(_owner->getBaseAddress() + _offset) = v;
  }
  
  
  k_octet_t KOctetField::get() const {
    return *(_owner->getBaseAddress() + _offset);
  }
  
  
//\/ KLongintField /\//////////////////////////////////////////////////////////
  
  class KLongintField : public KLongint {
    
  // --- FIELDS --- //
    
    private: PPtr<KRecord> _owner;
    private: const k_octet_t _offset;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: KLongintField(PPtr<KRecord> owner, const k_octet_t offset);
    public: ~KLongintField();
    
    
  // --- METHODS --- //
    
    // Inherited from KLongint
    public: void set(const k_longint_t v);
    public: k_longint_t get() const;
    
  };
  
  
  KLongintField::KLongintField(PPtr<KRecord> owner, const k_octet_t offset)
  : _offset(offset)
  {
    _owner = owner;
  }
  
  
  KLongintField::~KLongintField() {
    // Nothing;
  }
  
  
  void KLongintField::set(const k_longint_t v) {
    *((k_longint_t*)(_owner->getBaseAddress() + _offset)) = v;
  }
  
  
  k_longint_t KLongintField::get() const {
    return *(k_longint_t*)(_owner->getBaseAddress() + _offset);
  }
  
  
//\/ KRealField /\/////////////////////////////////////////////////////////////
  
  class KRealField : public KReal {
    
  // --- FIELDS --- //
    
    private: PPtr<KRecord> _owner;
    private: const k_octet_t _offset;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: KRealField(PPtr<KRecord> owner, const k_octet_t offset);
    public: ~KRealField();
    
    
  // --- METHODS --- //
    
    public: void set(const k_real_t v);
    public: k_real_t get() const;
    
  };

  
  KRealField::KRealField(PPtr<KRecord> owner, const k_octet_t offset)
  : _offset(offset)
  {
    _owner = owner;
  }
  
  
  KRealField::~KRealField() {
    // Nothing;
  }
  
  
  void KRealField::set(const k_real_t v) {
    *(k_real_t*)(_owner->getBaseAddress() + _offset) = v;
  }
  
  
  k_real_t KRealField::get() const {
    return *(k_real_t*)(_owner->getBaseAddress() + _offset);
  }
  
  
//\/ KGlobalUidField /\////////////////////////////////////////////////////////
  
  class KGlobalUidField : public KGuid {
    
  // --- FIELDS --- //
    
    private: PPtr<KRecord> _owner;
    private: const k_octet_t _offset;
    
  
  // --- (DE)CONSTRUCTORS --- //
    
    public: KGlobalUidField(PPtr<KRecord> owner, const k_octet_t offset);
    public: ~KGlobalUidField();
    
  
  // --- METHODS --- //
    
    public: void set(const k_guid_t& v);
    public: k_guid_t get() const;
    
  };
  
  
  KGlobalUidField::KGlobalUidField(PPtr<KRecord> owner, const k_octet_t offset)
  : _offset(offset)
  {
    _owner = owner;
  }
  
  
  KGlobalUidField::~KGlobalUidField() {
    // Nothing;
  }
  
  
  void KGlobalUidField::set(const k_guid_t &v) {
    *((k_guid_t*)(_owner->getBaseAddress() + _offset)) = v;
  }
  
  
  k_guid_t KGlobalUidField::get() const {
    return *((k_guid_t*)(_owner->getBaseAddress() + _offset));
  }
  
  
//\/ KEnumerationField /\//////////////////////////////////////////////////////
  
  class KEnumerationField : public KEnumeration {
    
  // --- FIELDS --- //
    
    private: PPtr<KRecord> _owner;
    private: const k_octet_t _offset;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: KEnumerationField(PPtr<KEnumerationType> type,
        PPtr<KRecord> owner, const k_octet_t offset);
    
    public: ~KEnumerationField();
    
    
  // --- METHODS --- //
    
    public: k_octet_t getOrdinal() const;
    public: void set(const k_octet_t ordinal);

  };
  
  
  KEnumerationField::KEnumerationField(PPtr<KEnumerationType> type,
      PPtr<KRecord> owner, const k_octet_t offset)
  : KEnumeration(type),
    _offset(offset)
  {
    _owner = owner;
  }
  
  
  KEnumerationField::~KEnumerationField() {
    // Nothing;
  }
  
  
  k_octet_t KEnumerationField::getOrdinal() const {
    return *(_owner->getBaseAddress() + _offset);
  }
  
  
  void KEnumerationField::set(const k_octet_t ordinal) {
    *(_owner->getBaseAddress() + _offset) = ordinal;
  }
  
  
//\/ KRecord /\////////////////////////////////////////////////////////////////
  
// --- (DE)CONSTRUCTORS --- //

  /**
   * Primary constructor; creates internal storage for a record of the given
   * type.
   *
   * @param type This record's type.
   */

  KRecord::KRecord(PPtr<KRecordType> type)
  : _nFields(type->getNumberOfFields())
  {
    _type = type;
    _offsetTable = type->getOffsetTable();
    _nFields = _type->getNumberOfFields();
    _hasDynamicFields = _type->hasDynamicFields();
    
    k_longint_t s = type->getSizeInOctets();
    _data = new k_octet_t[s];
    memset(_data, 0, s);
    
    _offset = -1;
    
    makeFields();
    makeDynamicFields();
  }


  /**
   * Creates a new record bound to the first cell of the given grid.
   */
  
  KRecord::KRecord(PPtr<KGrid> grid) {
    _type = grid->getType().AS(KGridType)->getRecordType();
    _nFields = _type->getNumberOfFields();
    _offsetTable = _type->getOffsetTable();
    _hasDynamicFields = _type->hasDynamicFields();
    _data = NULL;
    _owner = grid.AS(KDynamicValue);
    _offset = 0;
    
    makeFields();
  }


  /**
   * Creates a new record bound to the given field of the given record.
   * The given field should be of a record type.
   */
  
  KRecord::KRecord(PPtr<KRecord> record, const k_octet_t fieldIndex) {
    if(fieldIndex > record->_nFields) {
      throw IndexOutOfBoundException("Expected a number between 0 and "
          + Int::toString(_nFields) + ". Given: " + Int::toString(fieldIndex));
    }
    
    bindToRecord(record, fieldIndex);
  }


  /**
   * Creates a new record bound to the given field of the given record.
   * The given field should be of a record type.
   */

  KRecord::KRecord(PPtr<KRecord> record, const string& fieldName) {
    int fieldIndex = record->_type->getIndexForFieldWithName(fieldName);
    if(fieldIndex < 0) {
      throw KFException("Field \"" + fieldName + "\" does not exist.");
    }
    
    bindToRecord(record, fieldIndex);
  }


  /**
   * Deconstructor. Frees any used memory.
   */
  
  KRecord::~KRecord() {
    if( NOT_NULL(_data) ) {
      cleanupDynamicFields();
      delete[] _data;
    }
    
    delete[] _fields;
  }
  
  
// --- METHODS --- //
  
  bool KRecord::isInitialized() {
    return *(k_octet_t*)KRECORD_DATA == 1;
  }
  
  
  void KRecord::setInitialized() {
    *(k_octet_t*)KRECORD_DATA = 1;
  }
  
  
  void KRecord::bindToRecord(PPtr<KRecord> record, const k_octet_t fieldIndex) {
    PPtr<KType> t = record->_type->getTypeOfFieldAtIndex(fieldIndex);
    if(!t.ISA(KRecordType)) {
      throw KFException("Field " + record->_type->getNameOfFieldAtIndex(fieldIndex)
         + " (" + Int::toString(fieldIndex) + ") is not a record.");
    }
    
    _type = t.AS(KRecordType);
    _nFields = _type->getNumberOfFields();
    _hasDynamicFields = _type->hasDynamicFields();
    _offsetTable = _type->getOffsetTable();
    
    _data = NULL;
    _owner = record.AS(KDynamicValue);
    _offset = record->_offsetTable[fieldIndex];
    
    makeFields();
    makeDynamicFields();
  }
  
  
  void KRecord::makeFields() {
    _fields = new Ptr<KValue>[_nFields];
    
    for(int i = _nFields - 1; i >= 0; i--) {
      makeField(i);
    }
  }
  
  
  void KRecord::makeDynamicFields() {
    if(!_type->hasDynamicFields()) {
      return;
    }
    
    for(int i = _nFields - 1; i >= 0; i--) {
      makeDynamicField(i);
    }
    
    setInitialized();
  }
  
  
  void KRecord::makeField(k_octet_t i) {
    PPtr<KType> t = _type->getTypeOfFieldAtIndex(i);
    k_octet_t offset = _offsetTable[i];
    
    if(t->equals(KType::TRUTH)) { // .................................... truth
      
      _fields[i] = new KTruthField(getPtr().AS(KRecord), offset);
      
    } else if(t->equals(KType::OCTET)) { // ............................. octet
      
      _fields[i] = new KOctetField(getPtr().AS(KRecord), offset);
      
    } else if(t->equals(KType::INTEGER)) { // ......................... integer
      
      _fields[i] = new KIntegerField(getPtr().AS(KRecord), offset);
      
    } else if(t->equals(KType::LONGINT)) { // ......................... longint
      
      _fields[i] = new KLongintField(getPtr().AS(KRecord), offset);
      
    } else if(t->equals(KType::REAL)) { // ............................... real
      
      _fields[i] = new KRealField(getPtr().AS(KRecord), offset);
      
    } else if(t->equals(KType::GUID)) { // ............................... guid
      
      _fields[i] = new KGlobalUidField(getPtr().AS(KRecord), offset);
      
    } else if(t.ISA(KEnumerationType)) { // ....................... enumeration
      
      _fields[i] = new KEnumerationField(t.AS(KEnumerationType),
          getPtr().AS(KRecord), offset);
      
    } else if(t.ISA(KRecordType)) { // ................................. record
      
      _fields[i] = new KRecord(getPtr().AS(KRecord), i);
      
    } // .................................................................. end
    
  } // makeFields()
  
  
  void KRecord::makeDynamicField(k_octet_t i) {
    PPtr<KType> t = _type->getTypeOfFieldAtIndex(i);

    if(t->equals(KType::RAW)) { // ........................................ raw
      
      if(isInitialized()) {
        memcpy((void*)&_fields[i], KRECORD_DATA + _offsetTable[i],
            sizeof(Ptr<KRaw>));
        
        _fields[i].setAutorelease(false);
      } else {
        _fields[i] = new KRaw();
        _fields[i].setAutorelease(false);
        memcpy(KRECORD_DATA + _offsetTable[i], (void*)&_fields[i],
            sizeof(Ptr<KRaw>));
      }
      
    } else if(t->equals(KType::STRING)) { // ........................... string
      
      if(isInitialized()) {
        memcpy((void*)&_fields[i], KRECORD_DATA + _offsetTable[i],
            sizeof(Ptr<KString>));
        
        _fields[i].setAutorelease(false);
      } else {
        if(!_fields[i].isNull()) {
          _fields[i].retain();
        }
        
        _fields[i] = new KString();
        _fields[i].setAutorelease(false);
        memcpy(KRECORD_DATA + _offsetTable[i], (void*)&_fields[i],
            sizeof(Ptr<KString>));
      }
      
    } else if(t->equals(KType::ANY)) { // ................................. any
      
      if(isInitialized()) {
        memcpy((void*)&_fields[i], KRECORD_DATA + _offsetTable[i],
            sizeof(Ptr<KAny>));
        
        _fields[i].setAutorelease(false);
      } else {
        if(!_fields[i].isNull()) {
          _fields[i].retain();
        }
        
        _fields[i] = new KAny();
        _fields[i].setAutorelease(false);
        memcpy(KRECORD_DATA + _offsetTable[i], (void*)&_fields[i],
            sizeof(Ptr<KAny>));
      }
      
    } else if(t.ISA(KGridType)) { // ..................................... grid
      
      if(isInitialized()) {
        memcpy((void*)&_fields[i], KRECORD_DATA + _offsetTable[i],
            sizeof(Ptr<KGrid>));
        
        _fields[i].setAutorelease(false);
      } else {
        _fields[i] = new KGridBasic(t.AS(KGridType));
        _fields[i].setAutorelease(false);
        memcpy(KRECORD_DATA + _offsetTable[i], (void*)&_fields[i],
            sizeof(Ptr<KGrid>));
      }
      
    } // .................................................................. end
    
  }
  
  /**
   * Returns wrapper object for the field at the given index.
   */

  PPtr<KValue> KRecord::field(const k_octet_t index) const {
    if(index > _nFields) {
      throw IndexOutOfBoundException("Expected a number between 0 and "
         + Int::toString(_nFields) + ". Given: " + Int::toString(index));
    }    
    return _fields[index];
  }


  /**
   * Returns wrapper object for the field with the given name.
   */
  
  PPtr<KValue> KRecord::field(const string& name) const {
    k_octet_t index = _type->getIndexForFieldWithName(name);
    if(index < 0) {
      throw KFException("Field \"" + name + "\" does not exist.");
    }
    return _fields[index];
  }
  
  
  #define ENUMERAND(X) \
  /** Returns the value of the field at the given index. Index is optional and is assumed 0 if not provided. */\
  PPtr<K ## X> KRecord::get ## X(const k_octet_t index) const {\
    if(index > _nFields) {\
      throw IndexOutOfBoundException("Expected a number between 0 and " \
          + Int::toString(_nFields) + ". Given: " + Int::toString(index));\
    }\
    return _fields[index].AS(K ## X);\
  }\
  /** Returns the value of the field at the given name. */\
  PPtr<K ## X> KRecord::get ## X(const string& name) const {\
    k_octet_t index = _type->getIndexForFieldWithName(name);\
    if(index < 0) {\
      throw KFException("Field \"" + name + "\" does not exist.");\
    }\
    return _fields[_type->getIndexForFieldWithName(name)].AS(K ## X);\
  }\
  /** Sets the field at the given index with the value stored in the given wrapper object. */\
  void KRecord::set ## X(const k_octet_t index, PPtr<K ## X> value) {\
    if(index > _nFields) {\
      throw IndexOutOfBoundException("Expected a number between 0 and " \
          + Int::toString(_nFields) + ". Given: " + Int::toString(index));\
    }\
    _fields[index].release();\
    _fields[index] = value.AS(KValue);\
    _fields[index].retain();\
    memcpy(KRECORD_DATA + _offsetTable[index], (void*)&_fields[index], sizeof(Ptr<KValue>));\
  }\
  /** Sets the field with the given name with the value stored in the given wrapper object. */\
  void KRecord::set ## X(const string& name, PPtr<K ## X> value) {\
    k_octet_t index = _type->getIndexForFieldWithName(name);\
    if(index < 0) {\
      throw KFException("Field \"" + name + "\" does not exist.");\
    }\
    _fields[index] = value.AS(KValue);\
    memcpy(KRECORD_DATA + _offsetTable[index], (void*)&_fields[index], sizeof(Ptr<KValue>));\
  }\
  /** Sets the first field with the value stored in the given wrapper object. */\
  void KRecord::set ## X(PPtr<K ## X> value) {\
    _fields[0] = value.AS(KValue);\
    memcpy(KRECORD_DATA + _offsetTable[0], (void*)&_fields[0], sizeof(Ptr<KValue>));\
  }
  ENUMERATE_OVER_DYNAMIC_TYPES
  #undef ENUMERAND

  
  #define ENUMERAND(X, Y)\
  /** Returns the value of the field at the given index. Index is optional and is assumed 0 if not provided. */\
  Y KRecord::get ## X(const k_octet_t index) const {\
    if(index > _nFields) {\
      throw IndexOutOfBoundException("Expected a number between 0 and " \
        + Int::toString(_nFields) + ". Given: " + Int::toString(index));\
    }\
    return *(Y*)(KRECORD_DATA + _offsetTable[index]);\
  }\
  /** Returns the value of the field with the given name. */\
  Y KRecord::get ## X(const string& name) const {\
    k_octet_t index = _type->getIndexForFieldWithName(name);\
    if(index < 0) {\
      throw KFException("Field \"" + name + "\" does not exist.");\
    }\
    return get ## X(index);\
  }\
  /** Sets the field at the given index with the given value. */\
  void KRecord::set ## X(const k_octet_t index, const Y value) {\
    if(index > _nFields) {\
      throw IndexOutOfBoundException("Expected a number between 0 and " \
        + Int::toString(_nFields) + ". Given: " + Int::toString(index));\
    }\
    *(Y*)(KRECORD_DATA + _offsetTable[index]) = value;\
  }\
  /** Sets the first field with the given value. */\
  void KRecord::set ## X(const Y value) {\
    set ## X(0, value);\
  }\
  /** Sets the field with the given name with the given value. */\
  void KRecord::set ## X(const string& name, const Y value) {\
    k_octet_t index = _type->getIndexForFieldWithName(name);\
    if(index < 0) {\
      throw KFException("Field \"" + name + "\" does not exist.");\
    }\
    set ## X(_type->getIndexForFieldWithName(name), value);\
  }
  ENUMERATE_OVER_PRIMITIVE_TYPES
  #undef ENUMERAND


  /**
   * Sets the value of the enumeration field at the given index with the
   * given ordinal.
   */

  void KRecord::setEnumeration(const k_octet_t index, const k_octet_t ordinal) {
    if(index > _nFields) {
      throw IndexOutOfBoundException("Expected a number between 0 and "
          + Int::toString(_nFields) + ". Given: " + Int::toString(index));
    }
    
    _type->getTypeOfFieldAtIndex(index).AS(KEnumerationType)
        ->setValueAtAddressWithOrdinal(
            KRECORD_DATA + _offsetTable[index], ordinal);
  }


  /**
   * Sets the value of the enumeration field at the given index with the given
   * label.
   */
  
  void KRecord::setEnumeration(const k_octet_t index, const string& label) {
    if(index > _nFields) {
      throw IndexOutOfBoundException("Expected a number between 0 and "
          + Int::toString(_nFields) + ". Given: " + Int::toString(index));
    }
    
    _type->getTypeOfFieldAtIndex(index).AS(KEnumerationType)
        ->setValueAtAddressWithLabel(
            KRECORD_DATA + _offsetTable[index], label);
  }


  /**
   * Returns the label of the enumeration field at the given index.
   */
  
  string KRecord::getEnumerationLabel(const k_octet_t index) const {
    if(index > _nFields) {
      throw IndexOutOfBoundException("Expected a number between 0 and "
          + Int::toString(_nFields) + ". Given: " + Int::toString(index));
    }
    
    return _type->getTypeOfFieldAtIndex(index).AS(KEnumerationType)
        ->getLabelForValueAtAddress(
            KRECORD_DATA + _offsetTable[index]);
  }


  /**
   * Returns the ordinal of the enumeration field at the given index.
   */
  
  k_octet_t KRecord::getEnumerationOrdinal(const k_octet_t index) const {
    if(index > _nFields) {
      throw IndexOutOfBoundException("Expected a number between 0 and "
          + Int::toString(_nFields) + ". Given: " + Int::toString(index));
    }
    
    return _type->getTypeOfFieldAtIndex(index).AS(KEnumerationType)
        ->getOrdinalForValueAtAddress(
            KRECORD_DATA + _offsetTable[index]);
  }


  /**
   * Returns the record at the given index.
   */
  
  PPtr<KRecord> KRecord::getRecord(k_octet_t index) const {
    if(index > _nFields) {
      throw IndexOutOfBoundException("Expected a number between 0 and "
          + Int::toString(_nFields) + ". Given: " + Int::toString(index));
    }
    
    return _fields[index].AS(KRecord);
  }


  /**
   * Returns the record at the field with the given name.
   */
  
  PPtr<KRecord> KRecord::getRecord(const string& name) const {
    k_octet_t index = _type->getIndexForFieldWithName(name);
    if(index < 0) {
      throw KFException("Field \"" + name + "\" does not exist.");
    }
    
    return _fields[index].AS(KRecord);
  }


  /**
   * Wraps the given record around the field at the given index.
   */
  
  void KRecord::getRecord(k_octet_t index, PPtr<KRecord> wrapper) const {
    if(index > _nFields) {
      throw IndexOutOfBoundException("Expected a number between 0 and "
          + Int::toString(_nFields) + ". Given: " + Int::toString(index));
    }
    
    wrapper->wrap(getPtr().AS(KDynamicValue), _offsetTable[index]);
  }


  /**
   * Wraps the given record around the field with the given name.
   */
  
  void KRecord::getRecord(const string& name, PPtr<KRecord> wrapper) const {
    k_octet_t index = _type->getIndexForFieldWithName(name);
    if(index < 0) {
      throw KFException("Field \"" + name + "\" does not exist.");
    }
    
    wrapper->wrap(getPtr().AS(KDynamicValue), _offsetTable[index]);
  }


  void KRecord::wrap(PPtr<KDynamicValue> target, k_longint_t offset) {
    if( NOT_NULL(_data) ) {
      delete[] _data;
      _data = NULL;
    }
    
    _owner = target;
    _owner.setAutorelease(true);
    _owner.retain();
    _offset = offset;
    
    makeDynamicFields();
  }
  
  
  void KRecord::cleanupDynamicFields() {
    if(!_type->hasDynamicFields()) {
      return;
    }
    
    for(int i = _type->getNumberOfFields() - 1; i >= 0; i--) {
      PPtr<KType> t = _type->getTypeOfFieldAtIndex(i);
      if(!t->hasConstantSize()) {
        _fields[i].release();
      } else if(t.ISA(KRecordType)) {
        _fields[i].AS(KRecord)->cleanupDynamicFields();
      }
    }
  }


  /**
   * If this record has a field of `any` type, this method should be called
   * before performing `readFromBinaryStream()`.
   */
  
  void KRecord::setRuntime(Runtime& rt) {
    for(int i = _nFields - 1; i >= 0; i--) {
      if(_fields[i]->getType()->equals(KType::ANY)) {
        _fields[i].AS(KAny)->setRuntime(rt);
      }
    }
  }
  
  
  // Inherited from KRecordBase::KDynamicValue //

  k_octet_t* KRecord::getBaseAddress() const {
    if(NOT_NULL(_data)) {
      return _data;
    } else if(_offset >= 0) {
      return _owner->getBaseAddress() + _offset;
    }
    
    throw MemoryException("The memory containing this record is deleted.");
  }
  
  
  void KRecord::set(PPtr<KValue> other) {
    if(!other->getType()->equals(_type.AS(KType))) {
      throw KTypeMismatchException(_type.AS(KType), other->getType());
    }
    
    PPtr<KRecord> r = other.AS(KRecord);
    for(int i = _type->getNumberOfFields() - 1; i >= 0; i--) {
      _fields[i]->set(r->_fields[i]);
    }
  }
  
  
  PPtr<KType> KRecord::getType() const {
    return _type.AS(KType);
  }
  
  
  k_longint_t KRecord::getTotalSizeInOctets() const {
    if(!_type->hasDynamicFields()) {
      return _type->getSizeInOctets();
    }
    
    int n = _type->getNumberOfFields();
    
    k_longint_t s = 0;
    for(int i = 0; i < n; i++) {
      s += _fields[i]->getTotalSizeInOctets();
    }
    
    return s;
  }
  
  
  void KRecord::readFromBinaryStream(PPtr<InputStream> input) {
    int n = _type->getNumberOfFields();
    for(int i = 0; i < n; i++) {
      _fields[i]->readFromBinaryStream(input);
    }
  }
  
  
  /**
   * Reads the value of this record from the given stream.
   *
   * @note If this record has fields of type `any`, make sure to call 
   *       `setRuntime()` before calling this method. Failure to do so will
   *       cause an exception to be thrown.
   */

  void KRecord::writeToBinaryStream(PPtr<OutputStream> output) const {
    int n = _type->getNumberOfFields();
    for(int i = 0; i < n; i++) {
      _fields[i]->writeToBinaryStream(output);
    }
  }


  void KRecord::deserialize(PPtr<ObjectToken> head) {
    head->checkClass("KRecord");
    
    Ptr<Token> token = head->next();
    
    int index = 0;
    while(!token->is(EndObjectToken::TYPE)) {
      token->validateType(ObjectToken::TYPE);
      PPtr<ObjectToken> object = token.AS(ObjectToken);
      
      _fields[index]->deserialize(token.AS(ObjectToken));
      index++;
      
      token = token->next();
    }
    
    token->validateType(EndObjectToken::TYPE);
  }
  
  
  void KRecord::serialize(PPtr<ObjectSerializer> builder) const {
    builder->object("KRecord");
    builder->attribute("type", _type->getTypeName());
    
    const int len = _type->getNumberOfFields();
    
    for(int i = 0; i < len; i++) {
      builder->member(_type->getNameOfFieldAtIndex(i))
          ->object<KValue>(_fields[i]);
    }
    
    builder->endObject();
  }
  
//\////////////////////////////////////////////////////////////////////////////
  
} // namespace type
} // namespace knorba

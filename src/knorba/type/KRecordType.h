/*---[KReocrdType.h]-------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : knorba::type::KRecordType::*
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

#ifndef KNORBA_TYPE_KRECORDTYPE
#define KNORBA_TYPE_KRECORDTYPE

// KFoundation
#include <kfoundation/ManagedArray.h>

// Super
#include "KType.h"

namespace knorba {
namespace type {
  
  class KRecord;
  class KString;
  class KGridType;

  /**
   * Instantiate to create a custom KnoRBA **record** type. A record is a
   * collection of fields of various types. Usage:
   *
   *     Ptr<KRecordType> dateType = new KRecordType("Date");
   *     myType->addField("year", KType::OCTET)
   *           ->addField("month", KType::OCTET)
   *           ->addField("day", KType::OCTET);
   *
   *     Ptr<KRecordType> entryType = new KRecordType("Entry");
   *     myType->addField("name", KType::STRING)
   *           ->addField("phone", KType::STRING)
   *           ->addField("birthDay", dateType.AS(KType));
   *
   * A record may have fixed or variable size depending the type of its fields.
   * If hadDynamicFields() returns `true` then, there record has variable size.
   *
   * @headerfile KRecordType.h <knorba/type/KRecordType.h>
   */
  
  class KRecordType : public KType {
    
  // --- NESTED TYPES --- //
  
    private: class Field : public ManagedObject {
      public: const unsigned int _byteOffset;
      public: Ptr<KType> _type;
      public: const string _name;
      public: Field(const string& name, PPtr<KType> type,
          unsigned int byteOffset);
    };
    
  
  // --- FIELDS --- //
    
    private: ManagedArray<Field>::Ptr_t _fields;
    private: int _size;
    private: k_octet_t _offsetTable[15];
    private: bool _hasDynamicFields;
    
  
  // --- (DE)CONSTRUCTORS --- //
    
    public: KRecordType(const string& name);
    public: KRecordType(PPtr<KType> fieldType);
    
    
  // --- METHODS --- //
    
    public: PPtr<KRecordType> addField(const string& name, Ptr<KType> type);
    public: int getNumberOfFields() const;
    public: string getNameOfFieldAtIndex(const int i) const;
    public: PPtr<KType> getTypeOfFieldAtIndex(const int i) const;
    public: PPtr<KType> getTypeOfFieldWithName(const string& name) const;
    public: int getIndexForFieldWithName(const string& name) const;
    public: unsigned int getOffsetOfFieldAtIndex(const int index) const;
    public: bool hasDynamicFields() const;
    public: const k_octet_t* const getOffsetTable() const;
    public: Ptr<KGridType> makeGridType(k_octet_t nDims) const;
    
    // Inherited from KType
    public: bool isCastableTo(PPtr<KType> t) const;
    public: bool isAutomaticCastableTo(PPtr<KType> t) const;
    public: bool equals(PPtr<KType> t) const;
    public: int  getSizeInOctets() const;
    public: bool isPrimitive() const;
    public: bool hasConstantSize() const;
    public: Ptr<KValue> instantiate() const;
    public: string toKnois() const;
    
  };
    
} // namespace type
} // namespace knorba

#endif /* defined(KNORBA_TYPE_KRECORDTYPE) */

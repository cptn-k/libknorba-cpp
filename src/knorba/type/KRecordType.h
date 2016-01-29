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
#include <kfoundation/RefArray.h>

// Super
#include "KType.h"

namespace kfoundation {
  class UString;
  class ObjectToken;
}

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
  
    private: class Field : public KFObject {
      public: const k_integer_t _byteOffset;
      public: RefConst<KType> _type;
      public: RefConst<UString> _name;
      public: Field(RefConst<UString> name, RefConst<KType> type,
          k_integer_t byteOffset);
    };
    
  
  // --- FIELDS --- //
    
    private: Ref< RefArray<Field> > _fields;
    private: k_integer_t _size;
    private: k_octet_t _offsetTable[16];
    private: bool _hasDynamicFields;
    
  
  // --- (DE)CONSTRUCTORS --- //
    
    public: KRecordType(RefConst<UString> name);
    public: KRecordType(RefConst<KType> fieldType);
    
    
  // --- METHODS --- //
    
    public: Ref<KRecordType> addField(RefConst<UString> name, RefConst<KType> type);
    public: k_octet_t getNumberOfFields() const;
    public: RefConst<UString> getNameOfFieldAtIndex(const k_octet_t i) const;
    public: RefConst<KType> getTypeOfFieldAtIndex(const k_octet_t i) const;
    public: RefConst<KType> getTypeOfFieldWithName(RefConst<UString> name) const;
    public: k_integer_t getIndexForFieldWithName(RefConst<UString> name) const;
    public: k_integer_t getOffsetOfFieldAtIndex(const k_octet_t index) const;
    public: bool hasDynamicFields() const;
    public: const k_octet_t* const getOffsetTable() const;
    public: Ref<KGridType> makeGridType(k_octet_t nDims) const;
    
    // Inherited from KType
    public: bool isCastableTo(RefConst<KType> t) const;
    public: bool isAutomaticCastableTo(RefConst<KType> t) const;
    public: bool equals(RefConst<KType> t) const;
    public: k_integer_t getSizeInOctets() const;
    public: bool isPrimitive() const;
    public: bool hasConstantSize() const;
    public: Ref<KValue> instantiate() const;
    public: RefConst<UString> toKnois() const;
    public: k_longint_t getStreamSizeInOctets(const k_octet_t* buffer) const;

    public: void writeBufferToStream(const k_octet_t* buffer,
        Ref<OutputStream> stream) const;

    public: void writeStreamToBuffer(Ref<InputStream> stream,
        RefConst<Ontology> ontology, k_octet_t* buffer) const;

    public: void serializeBuffer(const k_octet_t* buffer,
        Ref<ObjectSerializer> serializer) const;

    public: void deserializeIntoBuffer(Ref<ObjectToken> head,
        RefConst<Ontology> ontology, k_octet_t* buffer) const;

    public: void initializeBuffer(k_octet_t* buffer) const;
    public: void cleanupBuffer(k_octet_t* buffer) const;
    public: void copyBuffer(const k_octet_t* src, k_octet_t* target) const;

  };
    
} // namespace type
} // namespace knorba

#endif /* defined(KNORBA_TYPE_KRECORDTYPE) */
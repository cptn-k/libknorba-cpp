/*---[KGridType.h]---------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : knorba::type::KGridType::*
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

#ifndef KNORBA_TYPE_KGRIDTYPE
#define KNORBA_TYPE_KGRIDTYPE

// Super
#include "KType.h"

namespace kfoundation {
  class Range;
  class ObjectToken;
  class Tuple;
}

namespace knorba {
namespace type {

  class KGridBasic;
  class KRecordType;

  /**
   * Instantiate to create a custom KnoRBA **grid** type. A KnoRBA grid is a 
   * multi-dimensional array of KnoRBA **record**. Therefore, creating a grid
   * type often involves creating a record type in advance:
   *
   *     Ptr<KRecordType> myRecordType = new KRecordType("MyRecordType");
   *     myRecordType->addField("pressure", KType::REAL)
   *                 ->addField("temperature", KType::REAL);
   *     Ptr<KGridType> myGridType = new KGridType(myRecordType, 3);
   *
   * In the special case that each cell of the grid has a simple value,
   * there is a shortcut constructor:
   *
   *     Ptr<KGridType> myGridTupe = new KGridType(KType::INTEGER, 2);
   *
   * In the above case, a new instance of KRecordType will be created
   * internally which can be retrieved via getRecordType() method.
   *
   * @note In KnoRBA strongly-typed system, two grid types with the same record
   *       type but different number of dimensions are considered as different
   *       types.
   *
   * @headerfile KGridType.h <knorba/type/KGridType.h>
   */

  class KGridType : public KType {
    
  // --- FIELDS --- //
  
    private: RefConst<KRecordType> _elementType;
    private: k_octet_t _nDimensions;
    
  
  // --- (DE)CONSTRUCTORS --- //
    
    public: KGridType(RefConst<KRecordType> recordTypes, k_octet_t nDimensions);

    
  // --- METHODS --- //
    
    public: RefConst<KRecordType> getRecordType() const;
    public: k_octet_t getNDimensions() const;

    private: void deleteData(k_octet_t* origin, const k_longint_t begin,
        const k_longint_t count) const;
    
    public: void setSize(k_octet_t* grid, const Tuple& size, bool copy) const;
    
    public: void makeWindow(const k_octet_t* origin, k_octet_t* target,
       const Range& window) const;


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

#endif /* defined(KNORBA_TYPE_KGRIDTYPE) */
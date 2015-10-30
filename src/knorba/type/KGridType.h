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
  
    private: Ptr<KRecordType> _elementType;
    private: k_octet_t _nDimensions;
    
  
  // --- (DE)CONSTRUCTORS --- //
    
    public: KGridType(PPtr<KType> recordTypes, k_octet_t nDimensions);
    public: KGridType(PPtr<KRecordType> recordTypes, k_octet_t nDimensions);

    
  // --- METHODS --- //
    
    public: PPtr<KRecordType> getRecordType() const;
    public: short int getNDimensions() const;
    
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

#endif /* defined(KNORBA_TYPE_KGRIDTYPE) */

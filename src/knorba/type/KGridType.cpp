/*---[KGridType.cpp]-------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : -
 |  Implements: knorba::type::KGridType::*
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
#include <kfoundation/Int.h>

// Internal
#include "KGrid.h"
#include "KRecordType.h"

// Self
#include "KGridType.h"

namespace knorba {
namespace type {
  
  using namespace kfoundation;
  
// --- (DE)CONSTRUCTORS --- //

  /**
   * Constructs a representation of a custom KnoRBA grid with the given cell
   * type and given number of dimensions.
   *
   * @param recordType The type of grid cells.
   * @param nDimensions Number of grid dimensions.
   */

  KGridType::KGridType(PPtr<KType> recordType, k_octet_t nDimension)
  : KType("grid(" + Int::toString(nDimension) + ") OF " + recordType->getTypeName())
  {
    _elementType = new KRecordType("grid." + recordType->getTypeName());
    _elementType->addField("field", recordType);
    _nDimensions = nDimension;
  }
  

  /**
   * Constructs a representation of a custom KnoRBA grid type with simple cell
   * type. This method will construct an instance of KRecordType internally,
   * with only one field who's type is the given argument.
   *
   * @param recordType The type of the sole record member of each cell.
   * @param nDimensions Number of grid dimensions.
   */

  KGridType::KGridType(PPtr<KRecordType> recordType, k_octet_t nDimension)
  : KType("grid(" + Int::toString(nDimension) + ") OF " + recordType->getTypeName())
  {
    _elementType = recordType;
    _nDimensions = nDimension;
  }
  

// --- METHODS --- //

  /**
   * Returns the type of grid cells.
   */

  PPtr<KRecordType> KGridType::getRecordType() const {
    return _elementType;
  }


  /**
   * Returns the number of dimensions.
   */
  
  short int KGridType::getNDimensions() const {
    return _nDimensions;
  }
  
  
  bool KGridType::isCastableTo(PPtr<KType> t) const {
    if(!t.ISA(KGridType)) {
      return false;
    }
    
    Ptr<KGridType> other = t.AS(KGridType);
    
    if(_nDimensions != other->_nDimensions) {
      return false;
    }
    
    return _elementType->isCastableTo(other->_elementType.AS(KType));
  }
  
  
  bool KGridType::isAutomaticCastableTo(PPtr<KType> t) const {
    if(!t.ISA(KGridType)) {
      return false;
    }
    
    Ptr<KGridType> other = t.AS(KGridType);
    
    if(_nDimensions != other->_nDimensions) {
      return false;
    }
    
    return _elementType->isAutomaticCastableTo(other->_elementType.AS(KType));
  }


  /**
   * Checks if type represented by this object is equivalant to the type
   * represented by the given argument.
   * Returns `true` iff (1) `t` is an instance of KGridType,
   * (2) record type of the grid type represented by this object is equivalant
   * of the record type of the given grid type, and (3) the number of
   * dimensions of this object equals to that of the given object.
   */

  bool KGridType::equals(PPtr<KType> t) const {
    if(!t.ISA(KGridType)) {
      return false;
    }
    
    Ptr<KGridType> other = t.AS(KGridType);
    
    return _elementType->equals(other->_elementType.AS(KType))
        && _nDimensions == other->_nDimensions;
  }
  
  
  int KGridType::getSizeInOctets() const {
    return 0;
  }
  
  
  bool KGridType::isPrimitive() const {
    return false;
  }
  
  
  bool KGridType::hasConstantSize() const {
    return false;
  }
  
  
  Ptr<KValue> KGridType::instantiate() const {
    return new KGridBasic(getPtr().AS(KGridType));
  }
  
  
  string KGridType::toKnois() const {
    return "grid(" + Int(_nDimensions) + ") OF " + _elementType->getTypeName();
  }
  
} // namespace type
} // namespace knorba

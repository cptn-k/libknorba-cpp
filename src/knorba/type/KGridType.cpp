//
//  KGridType.cpp
//  CellMonitorTest-XCodeWrapper
//
//  Created by Hamed KHANDAN on 7/24/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

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
  
  KGridType::KGridType(PPtr<KType> recordType, k_octet_t nDimension)
  : KType("grid(" + Int::toString(nDimension) + ") OF " + recordType->getTypeName())
  {
    _elementType = new KRecordType("grid." + recordType->getTypeName());
    _elementType->addField("field", recordType);
    _nDimensions = nDimension;
  }
  
  
  KGridType::KGridType(PPtr<KRecordType> recordType, k_octet_t nDimension)
  : KType("grid(" + Int::toString(nDimension) + ") OF " + recordType->getTypeName())
  {
    _elementType = recordType;
    _nDimensions = nDimension;
  }
  
  
  KGridType::~KGridType() {
    // Nothing;
  }
  
  
// --- METHODS --- //
  
  PPtr<KRecordType> KGridType::getRecordType() const {
    return _elementType;
  }
  
  
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

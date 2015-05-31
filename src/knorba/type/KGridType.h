//
//  KGridType.h
//  CellMonitorTest-XCodeWrapper
//
//  Created by Hamed KHANDAN on 7/24/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#ifndef KNORBA_TYPE_KGRIDTYPE
#define KNORBA_TYPE_KGRIDTYPE

// Super
#include "KType.h"

namespace knorba {
namespace type {

  class KGridBasic;
  class KRecordType;

  class KGridType : public KType {
    
  // --- FIELDS --- //
  
    private: Ptr<KRecordType> _elementType;
    private: k_octet_t _nDimensions;
    
  
  // --- (DE)CONSTRUCTORS --- //
    
    public: KGridType(PPtr<KType> recordTypes, k_octet_t nDimensions);
    public: KGridType(PPtr<KRecordType> recordTypes, k_octet_t nDimensions);
    public: ~KGridType();
    
    
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

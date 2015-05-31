//
//  KRecordType.h
//  CellMonitorTest-XCodeWrapper
//
//  Created by Hamed KHANDAN on 7/24/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

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
    public: ~KRecordType();
    
    
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

//
//  KEnumerationType.h
//  CellMonitorTest-XCodeWrapper
//
//  Created by Hamed KHANDAN on 7/24/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#ifndef KNORBA_TYPE_KENUMERATIONTYPE
#define KNORBA_TYPE_KENUMERATIONTYPE

// KFoundation
#include <kfoundation/ManagedArray.h>
#include <kfoundation/Array.h>

// Super
#include "KType.h"


namespace knorba {
namespace type {

  using namespace kfoundation;
  
  class KEnumeration;

  
  class KEnumerationType : public KType {
    
  // --- NESTED TYPES --- //

    private: struct Item : public ManagedObject {
      k_octet_t ordinal;
      k_longint_t hash;
      string symbol;
      Item(k_octet_t o, const string& s);
    };
    
    
  // --- FIELDS --- //
    
    private: ManagedArray<Item>::Ptr_t _items;

    
  // --- (DE)CONSTRUCTORS --- //
    
    public: KEnumerationType(const string& name);
    public: ~KEnumerationType();
    
    
  // --- METHODS --- //
    
    public: PPtr<KEnumerationType> addMember(k_octet_t ordinal, const string& label);
    public: PPtr<KEnumerationType> addMember(const string& label);
    
    public: string getLabelForOrdinal(const k_octet_t ordinal) const;
    public: int getOrdinalForLabel(const string& label) const;
    public: k_octet_t getNumberOfMembers() const;
    public: k_octet_t getMaxOrdinal() const;
    public: Array<k_octet_t>::Ptr_t getAllOrdinals() const;
    public: string getLabelForMemberAtIndex(const k_octet_t index) const;
    public: k_octet_t getOrdinalForMemberAtIndex(const k_octet_t index) const;
    
    public: k_octet_t getOrdinalForValueAtAddress(const k_octet_t* const addr)
            const;
    
    public: string getLabelForValueAtAddress(const k_octet_t* const addr) const;
    
    public: void setValueAtAddressWithOrdinal(k_octet_t* const addr,
            const k_octet_t ordinal) const;
    
    public: void setValueAtAddressWithLabel(k_octet_t* const addr,
            const string& label) const;
    
    
    // Inherited rom KType
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

#endif /* defined(KNORBA_TYPE_KENUMERATIONTYPE) */

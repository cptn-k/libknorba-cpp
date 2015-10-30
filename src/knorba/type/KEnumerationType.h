/*---[KEnumerationType.h]--------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : knorba::type::KEnumerationType::*
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

  /**
   * Instantiate to create custom KnoRBA **enumeration** type. Usage:
   *
   *      Ptr<KEnumerationType> myEnum = new KEnumerationType("season");
   *      myEnum->addMember("spring")
   *            ->addMember("summer")
   *            ->addMember("fall")
   *            ->addMember("winter");
   *
   * To instantiate,
   *
   *     Ptr<KEnumeration> value = new KEnumeration(myEnum);
   *
   * or
   *
   *     Ptr<KEnumeration> value = myEnum->instantiate().AS(KEnumeration);
   * .
   * @headerfile KEnumerationType.h <knorba/type/KEnumerationType.h>
   */
  
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

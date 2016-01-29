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
#include <kfoundation/RefArray.h>

// Super
#include "KType.h"

namespace kfoundation {
  class Int;
  class UString;
}

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

    private: class Item : public KFObject {
      public: k_octet_t _ordinal;
      public: RefConst<UString> _label;
      public: Item(k_octet_t ordinal, RefConst<UString> label);
    };


  // --- FIELDS --- //
    
    private: Ref< RefArray<Item> > _items;

    
  // --- (DE)CONSTRUCTORS --- //
    
    public: KEnumerationType(RefConst<UString> name);
    
    
  // --- METHODS --- //
    
    private: k_octet_t getMaxOrdinal() const;

    public: Ref<KEnumerationType> addMember(k_octet_t ordinal,
            RefConst<UString> label);

    public: Ref<KEnumerationType> addMember(RefConst<UString> label);
    
    public: RefConst<UString> getLabelForOrdinal(const k_octet_t ordinal) const;
    public: k_octet_t getOrdinalForLabel(RefConst<UString> label) const;
    public: k_octet_t getNumberOfMembers() const;
    public: k_octet_t getOrdinalForMemberAtIndex(const k_octet_t index) const;
    public: RefConst<UString> getLabelForMemberAtIndex(const k_octet_t index) const;

    public: k_octet_t getOrdinalForValueAtAddress(const k_octet_t* const addr)
            const;
    
    public: RefConst<UString> getLabelForValueAtAddress(
            const k_octet_t* const addr) const;
    
    public: void setValueAtAddressWithOrdinal(k_octet_t* const addr,
            const k_octet_t ordinal) const;
    
    public: void setValueAtAddressWithLabel(k_octet_t* const addr,
            RefConst<UString> label) const;
    
    
    // Inherited rom KType
    public: bool isCastableTo(RefConst<KType> t) const;
    public: bool isAutomaticCastableTo(RefConst<KType> t) const;
    public: bool equals(RefConst<KType> t) const;
    public: int  getSizeInOctets() const;
    public: bool isPrimitive() const;
    public: bool hasConstantSize() const;
    public: Ref<KValue> instantiate() const;
    public: RefConst<UString> toKnois() const;

    public: void writeBufferToStream(const k_octet_t* buffer,
        Ref<OutputStream> stream) const;

    public: void writeStreamToBuffer(Ref<InputStream> stream,
        RefConst<Ontology> ontology, k_octet_t* buffer) const;

    public: void serializeBuffer(const k_octet_t* buffer,
        Ref<ObjectSerializer> serializer) const;

    public: void deserializeIntoBuffer(Ref<ObjectToken> head,
        RefConst<Ontology> ontology, k_octet_t* buffer) const;
  };

} // namespace type
} // namespace knorba

#endif /* defined(KNORBA_TYPE_KENUMERATIONTYPE) */
/*---[KEnumeration.h]------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : knorba::type::KEnumeration::*
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

#ifndef KNORBA_TYPE_KENUMERATION
#define KNORBA_TYPE_KENUMERATION

// Super
#include "KValue.h"

namespace knorba {
namespace type {

  class KEnumerationType;
  class KType;

//\/ KEnumeration /\///////////////////////////////////////////////////////////

  /**
   * Wrapper class and C++ representation for KnoRBA `enumeration`. To use, the
   * corresponding KEnumerationType shold be defined in advance:
   *
   *     Ptr<KEnumerationType> fruitEnum = new KEnumerationType("Fruit");
   *     fruitEnum->addMember("apple")
   *              ->addMember("orange")
   *              ->addMember("banana");
   *
   *     Ptr<KEnumeration> myFruit = new KEnumeration(fruitEnum, "apple");
   *
   * @headerfile KEnumeration.h <knorba/type/KEnumeration.h>
   */

  class KEnumeration : public KValue {

  // --- STATIC FIELDS --- //

    public: static const k_octet_t SIZE_IN_OCTETS;

    
  // --- FIELDS --- //
    
    private: RefConst<KEnumerationType> _type;
    private: k_octet_t _value;
    
    
  // --- (DE)COSTRUCTORS --- //
    
    public: KEnumeration(RefConst<KEnumerationType> type);
    public: KEnumeration(RefConst<KEnumerationType> type, const k_octet_t ordinal);
    public: KEnumeration(RefConst<KEnumerationType> type, RefConst<UString> label);
    
    
  // --- METHODS --- //

    protected: virtual k_octet_t* getBuffer();
    protected: virtual const k_octet_t* getBuffer() const;

    public: k_octet_t getOrdinal() const;
    public: RefConst<UString> getLabel() const;
    public: void set(const k_octet_t ordinal);
    public: void set(RefConst<UString> label);
    
    // Inherited from KValue
    public: RefConst<KType> getType() const;

    // Inherited from KValue::SerializingStreamer::Streamer
    public: void printToStream(Ref<OutputStream> stream) const;
    public: RefConst<UString> toString() const;
    
  };


} // namespace type
} // namespace knorba

#endif /* defined(KNORBA_TYPE_KENUMERATION) */
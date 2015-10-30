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
    
  // --- FIELDS --- //
    
    private: Ptr<KEnumerationType> _type;
    private: k_octet_t _value;
    
    
  // --- (DE)COSTRUCTORS --- //
    
    public: KEnumeration(PPtr<KEnumerationType> type);
    public: KEnumeration(PPtr<KEnumerationType> type, const k_octet_t ordinal);
    public: KEnumeration(PPtr<KEnumerationType> type, const string& label);
    
    
  // --- METHODS --- //
    
    public: virtual k_octet_t getOrdinal() const;
    public: string getLabel() const;
    public: virtual void set(const k_octet_t value);
    public: void set(const string& value);
    
    // Inherited from KValue
    public: void set(PPtr<KValue> other);
    public: PPtr<KType> getType() const;
    public: k_longint_t getTotalSizeInOctets() const;
    public: void readFromBinaryStream(PPtr<InputStream> input);
    public: void writeToBinaryStream(PPtr<OutputStream> output) const;

    // Inherited from KValue::StreamDeserializer
    public: void deserialize(PPtr<ObjectToken> headToken);
    
    // Inherited from KValue::SerializingStreamer
    void serialize(PPtr<ObjectSerializer> builder) const;
    
    // Inherited from KValue::SerializingStreamer::Streamer
    void printToStream(ostream& os) const;
    
  };
  
} // namespace type
} // namespace knorba

#endif /* defined(KNORBA_TYPE_KENUMERATION) */
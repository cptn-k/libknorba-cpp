/*---[KInteger.h]----------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : knorba::type::KInteger::*
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

#ifndef KNORBA_TYPE_KINTEGER
#define KNORBA_TYPE_KINTEGER

// Super
#include "KValue.h"

namespace knorba {
namespace type {

  using namespace kfoundation;
  
  class KType;
  
  
//\/ KInteger /\///////////////////////////////////////////////////////////////

  /**
   * Wrapper class form KnoRBA `integer` type. A value of type `integer` is a
   * 32-bit (4-octet) 2's complement signed integer between KInteger::MAX_VALUE
   * and KInteger::MIN_VALUE. The scalar type associated with this class is
   * `knorba::type::k_integer_t`.
   *
   * @headerfile KInteger.h <knorba/type/KInteger.h>
   */

  class KInteger : public KValue {
    
  // --- STATIC FIELDS --- //
    
    public: static const k_integer_t MAX_VALUE;
    public: static const k_integer_t MIN_VALUE;
  
    
  // --- FIELDS --- //
    
    private: k_integer_t _value;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: KInteger();
    public: KInteger(const k_integer_t v);

    
  // --- METHODS --- //
    
    public: virtual k_integer_t get() const;
    public: virtual void set(const k_integer_t v);
    
    // Inherited from KValue
    public: PPtr<KType> getType() const;
    public: k_longint_t getTotalSizeInOctets() const;
    public: void readFromBinaryStream(PPtr<InputStream> input);
    public: void writeToBinaryStream(PPtr<OutputStream> output) const;
    public: void set(PPtr<KValue> other);

    // From KValue::StreamDeserializer
    public: void deserialize(PPtr<ObjectToken> headToken);

    // From KValue::SerializingStreamer
    void serialize(PPtr<ObjectSerializer> builder) const;
    
    // From KValue::SerilizingStreamer::Streamer
    void printToStream(ostream& os) const;
  };


} // namespace type
} // namespace knorba

#endif /* defined(KNORBA_TYPE_KINTEGER) */

/*---[KLongint.h]----------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : knorba::type::KLongint::*
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

#ifndef KONRBA_TYPE_KLONGINT
#define KONRBA_TYPE_KLONGINT

// Super
#include "KValue.h"

namespace knorba {
namespace type {

  using namespace kfoundation;
  
  class KType;

//\/ KLongInt /\///////////////////////////////////////////////////////////////

  /**
   * Wrapper class for KnoRBA `longint` type. A value of `longint` type is
   * a 2's complement 64-bit (8-octet) signed integer between
   * KLongint::MIN_VALUE and KLongint::MAX_VALUE. The scalar type associated
   * with this class is `knorba::type::k_longint_t`.
   *
   * @headerfile KLongint.h <knorba/type/KLongint.h>
   */

  class KLongint : public KValue {
        
  // --- STATIC FIELDS --- //
    
    public: static const k_longint_t MIN_VALUE;
    public: static const k_longint_t MAX_VALUE;
    
    
  // --- FIELDS --- //
    
    private: k_longint_t _value;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: KLongint();
    public: KLongint(const k_longint_t v);
    
    
  // --- METHODS --- //
    
    public: virtual void set(const k_longint_t v);
    public: virtual k_longint_t get() const;
    
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

#endif /* defined(KONRBA_TYPE_KLONGINT) */

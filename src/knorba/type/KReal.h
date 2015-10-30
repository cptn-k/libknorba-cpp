/*---[KReal.h]-------------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : knorba::type::KReal::*
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

#ifndef KNORBA_TYPE_KREAL
#define KNORBA_TYPE_KREAL

// Super
#include "KValue.h"

namespace knorba {
namespace type {

  using namespace kfoundation;
  
  class KType;
  
//\/ KReal /\//////////////////////////////////////////////////////////////////

  /**
   * Wrapper class for KnoRBA `real` type. A value of `real` type is a 64-bit
   * (8-octet) IEEE 754 floating point number. The scalar type associated with
   * this class is `knorba::type::k_real_t`. Special values, NaN (not a number),
   * and infinity, are stored in `KReal::NAN` and `KReal::INFINITY` respectively.
   * Negative infinity is simply `-KReal::INFINITY`.
   *
   * @headerfile KReal.h <knorba/type/KReal.h>
   */


  class KReal : public KValue {
    
  // --- STATIC FIELDS --- //

    public: static const k_real_t INFINITY;
    public: static const k_real_t NAN;

    
  // --- FIELDS --- //
    
    private: k_real_t _value;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: KReal();
    public: KReal(const k_real_t v);
    
    
  // --- METHODS --- //
    
    public: virtual void set(const k_real_t v);
    public: virtual k_real_t get() const;
    public: bool isNaN() const;
    public: bool isInfinity() const;
    
    // Inherited from KValue
    public: void set(PPtr<KValue> other);
    public: PPtr<KType> getType() const;
    public: k_longint_t getTotalSizeInOctets() const;
    public: void readFromBinaryStream(PPtr<InputStream> input);
    public: void writeToBinaryStream(PPtr<OutputStream> output) const;

    // Inherited from KValue::StreamDeserializer
    public: void deserialize(PPtr<ObjectToken> headToken);
    
    // Inherited from KValue::SerializingStreamer
    public: void serialize(PPtr<ObjectSerializer> builder) const;
    
    // Inherited from KValue::SerializingStreamer::Streamer
    public: void printToStream(ostream& os) const;
    
  };
  
} // namespace type
} // namespace knorba

#endif /* defined(KNORBA_TYPE_KREAL) */

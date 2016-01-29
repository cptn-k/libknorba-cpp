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
    public: static const k_octet_t SIZE_IN_OCTETS;


  // --- STATIC METHODS --- //

    public: static inline k_real_t readFromBuffer(const k_octet_t* buffer);
    public: static inline void writeToBuffer(const k_real_t value,
        k_octet_t* buffer);

    
  // --- FIELDS --- //
    
    private: k_real_t _value;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: KReal();
    public: KReal(const k_real_t v);
    
    
  // --- METHODS --- //

    protected: virtual k_octet_t* getBuffer();
    protected: virtual const k_octet_t* getBuffer() const;

    public: void set(const k_real_t v);
    public: k_real_t get() const;
    public: bool isNaN() const;
    public: bool isInfinity() const;
    
    // Inherited from KValue
    public: RefConst<KType> getType() const;

    // Inherited from KValue::SerializingStreamer::Streamer
    public: void printToStream(Ref<OutputStream> stream) const;
    public: RefConst<UString> toString() const;
    
  };


  inline k_real_t KReal::readFromBuffer(const k_octet_t* buffer) {
    return *(k_real_t*)buffer;
  }


  inline void KReal::writeToBuffer(const k_real_t value, k_octet_t* buffer) {
    *(k_real_t*)buffer = value;
  }
  
} // namespace type
} // namespace knorba

#endif /* defined(KNORBA_TYPE_KREAL) */
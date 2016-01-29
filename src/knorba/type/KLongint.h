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
    public: static const k_octet_t   SIZE_IN_OCTETS;


  // --- STATIC METHODS --- //

    public: static inline k_longint_t readFromBuffer(const k_octet_t* buffer);
    public: static inline void writeToBuffer(const k_longint_t value,
        k_octet_t* buffer);

    
  // --- FIELDS --- //
    
    private: k_longint_t _value;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: KLongint();
    public: KLongint(const k_longint_t v);
    
    
  // --- METHODS --- //

    protected: virtual k_octet_t* getBuffer();
    protected: virtual const k_octet_t* getBuffer() const;

    public: void set(const k_longint_t v);
    public: k_longint_t get() const;
    
    // Inherited from KValue
    public: RefConst<KType> getType() const;
    
    // Inherited from KValue::SerializingStreamer::Streamer
    public: void printToStream(Ref<OutputStream> stream) const;
    public: RefConst<UString> toString() const;

  };


  inline k_longint_t KLongint::readFromBuffer(const k_octet_t* buffer) {
    return *(k_longint_t*)buffer;
  }


  inline void KLongint::writeToBuffer(const k_longint_t value,
      k_octet_t* buffer)
  {
    *(k_longint_t*)buffer = value;
  }

  
} // namespace type
} // namespace knorba

#endif /* defined(KONRBA_TYPE_KLONGINT) */
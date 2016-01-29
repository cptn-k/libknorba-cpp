/*---[KOctet.h]------------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : knorba::type::KOctet::*
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

#ifndef KNORBA_TYPE_KOCTET
#define KNORBA_TYPE_KOCTET

// Super
#include "KValue.h"

namespace knorba {
namespace type {
  
  using namespace kfoundation;

  /**
   * Wrapper class for KnoRBA `octet` type. A value of type `octet` is an 8-bit 
   * unsigned integer between 0 and 255. Scalar type associated with this class
   * is `knorba::type::k_octet_t`.
   *
   * Implementation transparency is KnoRBA is partially achieved by having
   * every value to be expressed by a sequence of octets.
   *
   * @headerfile KOctet.h <knorba/type/KOctet.h>
   */

  class KOctet : public KValue {
    
  // --- STATIC METHODS --- //
    
    public: static k_octet_t parseHex(char ch);
    public: static k_octet_t parseHex(const char* chars);
    
    
  // --- FIELDS --- //
    
    private: k_octet_t _value;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: KOctet();
    public: KOctet(const k_octet_t v);
    
    
  // --- METHODS --- //

    protected: virtual k_octet_t* getBuffer();
    protected: virtual const k_octet_t* getBuffer() const;
    
    public: k_octet_t get() const;
    public: void set(const k_octet_t v);
    
    // Inherited from KValue //
    public: RefConst<KType> getType() const;

    // From KValue::SerilizingStreamer::Streamer
    public: void printToStream(Ref<OutputStream> stream) const;
    public: RefConst<UString> toString() const;

  };
  
} // namespace type
} // namespace knorba

#endif /* defined(KNORBA_TYPE_KOCTET) */
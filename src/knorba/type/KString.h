/*---[KString.h]-----------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : knorba::type::KString::*
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

#ifndef KNORBA_TYPE_KSTRING
#define KNORBA_TYPE_KSTRING

// Super
#include "KValue.h"

namespace kfoundation {
  class UString;
}

namespace knorba {
namespace type {
  
  using namespace kfoundation;
  
//\/ KString /\////////////////////////////////////////////////////////////////

  /**
   * Wrapper class and C++ representation of KnoRBA `string` type. KnoRBA
   * `string`s are encoded in UTF-8.
   *
   * @headerfile KString.h <knorba/type/KString.h>
   */

  class KString : public KValue, public Comparable<KString>,
      public Comparable<UString>
  {

  // --- NESTED TYPES --- //

    public: typedef struct {
      k_integer_t size;
      k_longint_t hash;
      k_octet_t*  data;
    } header_t;


  // --- STATIC FIELDS --- //

    private: static const k_octet_t HEADER_SIZE;

    
  // --- STATIC METHODS --- //

    public: static k_longint_t generateHashFor(const k_octet_t* s,
        k_longint_t size);

    public: static inline void initBuffer(k_octet_t* buffer);

    public: static void writeCStringToBuffer(const char* cstr,
        k_octet_t* buffer);

    public: static void writeDataToBuffer(const k_octet_t* data,
        const k_integer_t size, k_octet_t* buffer);

    public: static void writeDuplicateToBuffer(const k_octet_t* src,
        k_octet_t* buffer);

    public: static void cleanupBuffer(k_octet_t* buffer);


  // --- FIELDS --- //
    
    private: header_t _value;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: KString();
    public: KString(const char* cstr);
    public: KString(RefConst<UString> src);
    public: KString(RefConst<KString> src);
    public: ~KString();
    
    
  // --- METHODS --- //

    private: inline header_t* getHeader();
    private: inline const header_t* getHeader() const;

    protected: virtual k_octet_t* getBuffer();
    protected: virtual const k_octet_t* getBuffer() const;

    public: k_integer_t getSize() const;
    public: const k_octet_t* getData() const;
    public: k_longint_t getHashCode() const;
    public: void set(const char* cstr);
    public: void set(const k_octet_t* octets, const k_integer_t size);
    public: void set(RefConst<UString> src);
    public: void set(RefConst<KString> src);
    public: bool hashEquals(const k_longint_t& hash) const;

    // Inherited from Comparable<KString>
    public: bool equals(RefConst<KString> other) const;

    // Inherited from Comparable<UString>
    public: bool equals(RefConst<UString> other) const;
    
    // Inherited from KValue
    public: void set(RefConst<KValue> other);
    public: RefConst<KType> getType() const;

    // Inherited from KValue::SerializingStreamer::Streamer
    public: void printToStream(Ref<OutputStream> stream) const;
    public: RefConst<UString> toString() const;
    
  };


  inline void KString::initBuffer(k_octet_t* buffer) {
    ((header_t*)buffer)->data = NULL;
  }


  inline KString::header_t* KString::getHeader() {
    return (header_t*)getBuffer();
  }


  inline const KString::header_t* KString::getHeader() const {
    return (const header_t*)getBuffer();
  }


} // type
} // knorba

#endif /* defined(KNORBA_TYPE_KSTRING) */
/*---[KRaw.h]--------------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : knorba::type::KRaw::*
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

#ifndef KNORBA_TYPE_KRAW
#define KNORBA_TYPE_KRAW

// Internal
#include "KValue.h"


namespace kfoundation {
  class BufferInputStream;
}


namespace knorba {
namespace type {

//\/ KRaw /\///////////////////////////////////////////////////////////////////

  /**
   * Wrapper class and C++ representation for KnoRBA `raw` type. A value of
   * raw type is a continues sequence of arbitrary octets.
   *
   * @headerfile KRaw.h <knorba/type/KRaw.h>
   */

  class KRaw : public KValue {

  // --- NESTED TYPES --- //

    public: typedef struct {
      k_longint_t size;
      k_octet_t* data;
    } header_t;


  // --- STATIC METHODS --- //

    public: static void initBuffer(k_octet_t* buffer);
    public: static void writeToBuffer(const k_octet_t* data,
        const k_longint_t size, k_octet_t* target);
    public: static void writeToBuffer(const k_octet_t* src, k_octet_t* target);
    public: static void cleanupBuffer(k_octet_t* buffer);


  // --- STATIC FIELDS --- //

    public: static const k_octet_t HEADER_SIZE;


  // --- FIELDS --- //
    
    private: header_t _buffer;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: KRaw();
    public: ~KRaw();
    
    
  // --- METHODS --- //

    protected: virtual k_octet_t* getBuffer();
    protected: virtual const k_octet_t* getBuffer() const;

    public: void set(const k_octet_t* data, const k_longint_t size);
    public: k_longint_t getSize() const;
    public: const k_octet_t* getData() const;
    public: Ref<InputStream> getDataAsStream() const;
    public: void readData(Ref<InputStream> stream);
    public: void writeData(Ref<OutputStream> stream) const;

    // Inherited from KValue
    public: void set(RefConst<KValue> other);
    public: RefConst<KType> getType() const;

  };


} // namespace type
} // namespace knorba

#endif /* defined(KNORBA_TYPE_KRAW) */
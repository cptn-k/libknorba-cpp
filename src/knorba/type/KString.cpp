/*---[KString.cpp]---------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : -
 |  Implements: knorba::type::KString::*
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

// Std
#include <cstring>
#include <cassert>

// CityHash
#include <cityhash/city.h>

// KFondation
#include <kfoundation/Ref.h>
#include <kfoundation/UString.h>
#include <kfoundation/IndexOutOfBoundException.h>
#include <kfoundation/OutputStream.h>
#include <kfoundation/ObjectStreamReader.h>
#include <kfoundation/ObjectSerializer.h>

//Internal
#include "KType.h"
#include "KLongint.h"
#include "KTypeMismatchException.h"

// Self
#include "KString.h"


namespace knorba {
namespace type {

  using namespace kfoundation;

//\/ KStringUString /\/////////////////////////////////////////////////////////

  class KStringUString : public UString {

  // --- FIELDS --- //

    private: mutable RefConst<KString> _src;


  // --- CONSTRUCTORS --- //

    public: KStringUString(RefConst<KString> src);


  // --- METHODS --- //

    public: void releaseOwner() const;
    public: const kf_octet_t* getOctets() const;
    public: kf_int32_t getOctetCount() const;

  };


  KStringUString::KStringUString(RefConst<KString> src) {
    _src = src;
  }


  void KStringUString::releaseOwner() const {
    _src = NULL;
  }


  const kf_octet_t* KStringUString::getOctets() const {
    return _src->getData();
  }


  kf_int32_t KStringUString::getOctetCount() const {
    return _src->getSize();
  }


//\/ KString /\////////////////////////////////////////////////////////////////

// --- STATIC FIELDS --- //

  const k_octet_t KString::HEADER_SIZE = /*size:*/ 4 + /*hash:*/ 8;


// --- STATIC METHODS --- //

  /**
   * Generates 64-bit CityHash hashcode for the given sequence of octets.
   *
   * @param s Pointer to the begining of the sequence.
   * @param size The number of octets in the sequence.
   */
  
  k_longint_t KString::generateHashFor(const k_octet_t *s, k_longint_t size) {
    return CityHash64((char*)s, size);
  }


  void KString::writeCStringToBuffer(const char* cstr, k_octet_t* buffer) {
    writeDataToBuffer((k_octet_t*)cstr, (k_integer_t)strlen(cstr), buffer);
  }
  

  void KString::writeDataToBuffer(const k_octet_t* data,
      const k_integer_t size, k_octet_t* buffer)
  {
    cleanupBuffer(buffer);
    header_t* header = (header_t*)buffer;
    header->size = size;
    header->hash = generateHashFor(data, size);
    header->data = new k_octet_t[size];
    memcpy(header->data, data, size);
  }


  void KString::writeDuplicateToBuffer(const k_octet_t* src, k_octet_t* buffer)
  {
    cleanupBuffer(buffer);
    memcpy(buffer, src, sizeof(header_t));
    const header_t* srcHeader = (const header_t*)src;
    header_t* dstHeader = (header_t*)buffer;
    dstHeader->data = new k_octet_t[srcHeader->size];
    memcpy(dstHeader->data, srcHeader->data, srcHeader->size);
  }


  inline void KString::cleanupBuffer(k_octet_t* buffer) {
    header_t* header = (header_t*)buffer;
    if(NOT_NULL(header->data)) {
      delete header->data;
    }
    memset(buffer, 0, sizeof(header_t));
  }


// --- (DE)CONSTRUCTORS --- //

  /**
   * Constructor; creates an empty KnoRBA string.
   */

  KString::KString() {
    initBuffer((k_octet_t*)&_value);
  }


  KString::KString(const char* cstr) {
    initBuffer((k_octet_t*)&_value);
    writeCStringToBuffer(cstr, (k_octet_t*)&_value);
  }


  KString::KString(RefConst<UString> src) {
    initBuffer((k_octet_t*)&_value);
    writeDataToBuffer(src->getOctets(), src->getOctetCount(), (k_octet_t*)&_value);
  }


  KString::KString(RefConst<KString> src) {
    initBuffer((k_octet_t*)&_value);
    writeDuplicateToBuffer((k_octet_t*)&src->_value, (k_octet_t*)&_value);
  }


  /**
   * Deconstructor. Deletes the internal buffer.
   */
  
  KString::~KString() {
    cleanupBuffer((k_octet_t*)&_value);
  }
  
  
// --- METHODS --- //

  k_octet_t* KString::getBuffer() {
    return (k_octet_t*)&_value;
  }


  const k_octet_t* KString::getBuffer() const {
    return (const k_octet_t*)&_value;
  }

  
  /**
   * Returns the number of octets in the stored string.
   */

  k_integer_t KString::getSize() const {
    return ((header_t*)getBuffer())->size;
  }


  const k_octet_t* KString::getData() const {
    return ((header_t*)getBuffer())->data;
  }


  /**
   * Returns the hashcode of the stored string (64-bit CityHash).
   */

  k_longint_t KString::getHashCode() const {
    return ((header_t*)getBuffer())->hash;
  }


  void KString::set(const char* cstr) {
    writeCStringToBuffer(cstr, getBuffer());
  }
  
  
  void KString::set(const k_octet_t* octets, const k_integer_t size) {
    writeDataToBuffer(octets, size, getBuffer());
  }


  void KString::set(RefConst<UString> src) {
    writeDataToBuffer(src->getOctets(), src->getOctetCount(), getBuffer());
  }


  void KString::set(RefConst<KString> src) {
    writeDuplicateToBuffer(src->getBuffer(), getBuffer());
  }


  /**
   * Checks if the hashcode of this string is equal to the given value.
   */

  bool KString::hashEquals(const k_longint_t& hash) const {
    return getHashCode() == hash;
  }


  /**
   * Checks if string stored in this object is equal to the one stored by the
   * given parameter.
   */
  
  bool KString::equals(RefConst<KString> other) const {
    return getHashCode() == other->getHashCode();
  }


  bool KString::equals(RefConst<UString> other) const {
    return toString()->equals(other);
  }


  void KString::set(RefConst<KValue> other) {
    if(!other->getType()->equals(KType::STRING)) {
      throw KTypeMismatchException(getType(), other->getType());
    }

    set(other.AS(KString));
  }
  
  
  RefConst<KType> KString::getType() const {
    return KType::STRING;
  }
  
  
  void KString::printToStream(Ref<OutputStream> stream) const {
    stream->write(getData(), getSize());
  }


  RefConst<UString> KString::toString() const {
    return new KStringUString(RefConst<KString>(this));
  }

} // type
} // knorba
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
#include <kfoundation/LongInt.h>
#include <kfoundation/UniChar.h>
#include <kfoundation/IndexOutOfBoundException.h>
#include <kfoundation/OutputStream.h>
#include <kfoundation/ObjectStreamReader.h>

//Internal
#include "KType.h"
#include "KLongint.h"
#include "KTypeMismatchException.h"

// Self
#include "KString.h"

#define K_STRING_HEADER_SIZE 16
#define K_STRING_HASHCODE_OFFSET 8

namespace knorba {
namespace type {

  using namespace std;
  using namespace kfoundation;
  
  
// --- STATIC METHODS --- //

  /**
   * Generates 64-bit CityHash hashcode for the given string.
   */

  k_longint_t KString::generateHashFor(const wstring &ws) {
    return KString(ws).getHashCode();
  }


  /**
   * Generates 64-bit CityHash hashcode for the given string.
   */
  
  k_longint_t KString::generateHashFor(const string &s) {
    return KString(s).getHashCode();
  }


  /**
   * Generates 64-bit CityHash hashcode for the given sequence of octets.
   *
   * @param s Pointer to the begining of the sequence.
   * @param size The number of octets in the sequence.
   */
  
  k_longint_t KString::generateHashFor(const k_octet_t *s, k_longint_t size) {
    return CityHash64((char*)s, size);
  }
  
  
// --- (DE)CONSTRUCTORS --- //

  /**
   * Constructor; creates an empty KnoRBA string.
   */

  KString::KString() {
    _buffer = NULL;
  }


  /**
   * Constructor; copies the stored value from the given string.
   *
   * @param s Initial value.
   */
  
  KString::KString(const string& s) {
    _buffer = NULL;
    set(s);
  }


  /**
   * Constructor; copies the stored value from the given string.
   *
   * @param str Initial value.
   */
  
  KString::KString(const wstring& str) {
    _buffer = NULL;
    set(str);
  }


  /**
   * Deconstructor. Deletes the internal buffer.
   */
  
  KString::~KString() {
    if( NOT_NULL(_buffer) ) {
      delete[] _buffer;
    }
  }
  
  
// --- METHODS --- //
  
  void KString::reallocateBuffer(const k_longint_t nOctets) {
    k_octet_t* buffer = new k_octet_t[nOctets + K_STRING_HEADER_SIZE + 1];
    *(k_longint_t*)buffer = nOctets;
    buffer[nOctets + K_STRING_HEADER_SIZE] = 0;
    
    setBuffer(buffer);
  }
  
  
  k_octet_t* KString::getStringHead() const {
    return getBuffer() + K_STRING_HEADER_SIZE;
  }
  
  
  void KString::setHashCode(const k_longint_t code) {
    *(k_longint_t*)(getBuffer() + K_STRING_HASHCODE_OFFSET) = code;
  }


  /**
   * Returns the hashcode of the stored string (64-bit CityHash).
   */
  
  k_longint_t KString::getHashCode() const {
    if( IS_NULL(getBuffer()) ) {
      return 0;
    }
    
    return *(k_longint_t*)(getBuffer() + K_STRING_HASHCODE_OFFSET);
  }
  

  /**
   * Returns the number of octets in the stored string.
   */

  k_longint_t KString::getNOctets() const {
    if( IS_NULL(getBuffer()) ) {
      return 0;
    }
    
    return *(k_longint_t*)getBuffer();
  }


  /**
   * Sets the stored value from the given string.
   */
  
  void KString::set(const string& str) {
    k_longint_t nOctets = 0;
    k_longint_t nCodePoints = str.length();
    
    for(int i = 0; i < nCodePoints; i++) {
      nOctets += UniChar::getNumberOfUtf8Octets(str[i]);
    }
    
    reallocateBuffer(nOctets);
    
    k_octet_t* p = getStringHead();
    for(int i = 0; i < nCodePoints; i++) {
      p += UniChar::writeUtf8(str[i], p);
    }
    *p = 0;
    
    assert(p == getStringHead() + nOctets);
    
    setHashCode(KString::generateHashFor(getStringHead(), nOctets));
  }


  /**
   * Sets the stored value from the given string.
   */
  
  void KString::set(const wstring& str) {
    k_longint_t nOctets = 0;
    k_longint_t nCodePoints = str.length();
    
    for(int i = 0; i < nCodePoints; i++) {
      nOctets += UniChar::getNumberOfUtf8Octets(str[i]);
    }
    
    reallocateBuffer(nOctets);
    
    k_octet_t* p = getStringHead();
    for(int i = 0; i < nCodePoints; i++) {
      p += UniChar::writeUtf8(str[i], p);
    }
    *p = 0;
    
    assert(p == getStringHead() + nOctets);
    
    setHashCode(KString::generateHashFor(getStringHead(), nOctets));
  }
  

  /**
   * Returns the number of code points (characters) in this string.
   */

  k_longint_t KString::getNCodePoints() const {
    if( IS_NULL(getBuffer()) ) {
      return 0;
    }
    
    k_longint_t nCodePoints = 0;
    k_octet_t* p = getStringHead();
    k_octet_t* end = p + getNOctets();
    wchar_t ch = 0;
    while(p < end) {
      p += UniChar::readUtf8(p, ch);
      nCodePoints++;
    }
    return nCodePoints;
  }


  /**
   * Converts the stored string into C++ wstring.
   */
  
  wstring KString::toWString() const {
    if( IS_NULL(getBuffer()) ) {
      return wstring();
    }
    
    k_octet_t* p = getStringHead();
    k_octet_t* end = p + getNOctets();
    
    wchar_t tmp[getNOctets()];
    
    k_longint_t size = 0;
    while(p < end) {
      p += UniChar::readUtf8(p, tmp[size]);
      size++;
    }
    
    return wstring(tmp, size);
  }


  /**
   * Returns the pointer to the internal buffer where UTF-8 encoded string
   * is stored.
   */

  const char* KString::getUtf8CStr() const {
    return (const char*)getStringHead();
  }


  /**
   * Creates a new std::string object containing the same UTF-8 representation
   * as this object.
   */

  string KString::toUtf8String() const {
    if( IS_NULL(getBuffer()) ) {
      return string();
    }
    
    return string((char*)getStringHead(), getNOctets());
  }
  

  /**
   * Returns codepoint (character) at the given index.
   */

  wchar_t KString::getCodePointAt(const k_longint_t index) const {
    if( IS_NULL(getBuffer()) ) {
      throw KFException("String is empty");
    }
    
    k_octet_t* p = getStringHead();
    k_octet_t* end = p + getNOctets();
    wchar_t ch = 0;
    
    for(int i = 0; i < index && *p != 0 && p <= end; i++) {
      p += UniChar::readUtf8(p, ch);
      if(p > end) {
        throw IndexOutOfBoundException("Reached the end of string");
      }
    }
    
    return ch;
  }
  

  /**
   * Returns the octet at the given index.
   */

  k_octet_t KString::getOctetAt(const k_longint_t index) const {
    if( IS_NULL(getBuffer()) ) {
      throw KFException("String is empty");
    }
    
    if(index >= getNOctets()) {
      throw IndexOutOfBoundException("Asked for octet " + LongInt(index)
          + " of string of size " + LongInt(getNOctets()));
    }
    
    return getStringHead()[index];
  }


  /**
   * Checks if this object and the given std::wstring object represent the same
   * string. This method works by comparing hashcodes.
   */
  
  bool KString::equals(const wstring &ws) const {
    return getHashCode() == generateHashFor(ws);
  }


  /**
   * Checks if this object and the UTF-8 encoded std::string object represent
   * the same string. This method works by comparing hashcode.
   */
  
  bool KString::equals(const string &s) const {
    return getHashCode() == generateHashFor(s);
  }


  /**
   * Checks if string stored in this object is equal to the one stored by the
   * given parameter.
   */
  
  bool KString::equals(PPtr<KString> str) const {
    return getHashCode() == str->getHashCode();
  }


  /**
   * Checks if the hashcode of this string is equal to the given value.
   */

  bool KString::hashEquals(const k_longint_t& hash) const {
    return getHashCode() == hash;
  }


  void KString::set(PPtr<KValue> other) {
    if(!other->getType()->equals(KType::STRING)) {
      throw KTypeMismatchException(getType(), other->getType());
    }
    
    PPtr<KString> str = other.AS(KString);
    k_longint_t nOctets = str->getNOctets();
    reallocateBuffer(nOctets);
    memcpy(getBuffer(), str->getBuffer(), nOctets + K_STRING_HEADER_SIZE);
  }
  
  
  PPtr<KType> KString::getType() const {
    return KType::STRING;
  }
  
  
  k_longint_t KString::getTotalSizeInOctets() const {
    return K_STRING_HEADER_SIZE + getNOctets();
  }
  

  void KString::readFromBinaryStream(PPtr<InputStream> input) {
    Ptr<KLongint> value = new KLongint();
    value->readFromBinaryStream(input);
    k_longint_t nOctets = value->get();
    reallocateBuffer(nOctets);
    value->readFromBinaryStream(input);
    setHashCode(value->get());
    input->read(getStringHead(), (int)nOctets);
    *(getStringHead() + nOctets) = 0;
  }
  
  
  void KString::writeToBinaryStream(PPtr<OutputStream> output) const {
    k_longint_t nOctets = getNOctets();
    Ptr<KLongint> value = new KLongint(nOctets);
    value->writeToBinaryStream(output);
    value->set(getHashCode());
    value->writeToBinaryStream(output);
    output->write(getStringHead(), (kf_int32_t)nOctets);
  }
  
  
  void KString::deserialize(PPtr<ObjectToken> headToken) {
    headToken->validateClass("KString");
    
    Ptr<Token> token = headToken->next();
    token->validateType(AttributeToken::TYPE);
    
    PPtr<AttributeToken> attrib = token.AS(AttributeToken);
    attrib->validateName("value");
    
    set(attrib->getValue());
    
    token = token->next();
    token->validateType(EndObjectToken::TYPE);
  }

  
  void KString::serialize(PPtr<ObjectSerializer> builder) const {
    builder->object("KString")
           ->attribute("value", toUtf8String())
           ->endObject();
  }

  
  void KString::printToStream(ostream &os) const {
    for(k_octet_t *p = getStringHead(), *end = p + getNOctets(); p < end; p++) {
      os.put(*p);
    }
  }


} // type
} // knorba

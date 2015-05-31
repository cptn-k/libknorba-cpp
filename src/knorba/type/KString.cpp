//
//  KString.cpp
//  CellMonitorTest-XCodeWrapper
//
//  Created by Hamed KHANDAN on 7/18/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

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
  
  k_longint_t KString::generateHashFor(const wstring &ws) {
    return KString(ws).getHashCode();
  }
  
  
  k_longint_t KString::generateHashFor(const string &s) {
    return KString(s).getHashCode();
  }
  
  
  k_longint_t KString::generateHashFor(const k_octet_t *s, k_longint_t size) {
    return CityHash64((char*)s, size);
  }
  
  
// --- (DE)CONSTRUCTORS --- //
  
  KString::KString() {
    _buffer = NULL;
  }
  
  
  KString::KString(const string& s) {
    _buffer = NULL;
    set(s);
  }
  
  
  KString::KString(const wstring& str) {
    _buffer = NULL;
    set(str);
  }
  
  
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
  
  
  k_longint_t KString::getHashCode() const {
    if( IS_NULL(getBuffer()) ) {
      return 0;
    }
    
    return *(k_longint_t*)(getBuffer() + K_STRING_HASHCODE_OFFSET);
  }
  
  
  k_longint_t KString::getNOctets() const {
    if( IS_NULL(getBuffer()) ) {
      return 0;
    }
    
    return *(k_longint_t*)getBuffer();
  }
  
  
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
  

  const char* KString::getUtf8CStr() const {
    return (const char*)getStringHead();
  }
  
  
  string KString::toUtf8String() const {
    if( IS_NULL(getBuffer()) ) {
      return string();
    }
    
    return string((char*)getStringHead(), getNOctets());
  }
  
  
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
  
  
  bool KString::equals(const wstring &ws) const {
    return getHashCode() == generateHashFor(ws);
  }
  
  
  bool KString::equals(const string &s) const {
    return getHashCode() == generateHashFor(s);
  }
  
  
  bool KString::equals(PPtr<KString> str) const {
    return getHashCode() == str->getHashCode();
  }
  
  
  bool KString::equals(const ManagedObject& other) const {
    return ((const KString&)other).getHashCode() == getHashCode();
  }
  
  
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
  
  
  void KString::readFromObjectStream(PPtr<ObjectToken> headToken) {
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

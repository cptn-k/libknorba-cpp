//
//  KOctet.cpp
//  CellMonitorTest-XCodeWrapper
//
//  Created by Hamed KHANDAN on 8/8/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

// KFoundation
#include <kfoundation/IOException.h>
#include <kfoundation/InputStream.h>
#include <kfoundation/OutputStream.h>
#include <kfoundation/ObjectStreamReader.h>

// Internal
#include "KTypeMismatchException.h"
#include "KType.h"

// Self
#include "KOctet.h"

namespace knorba {
namespace type {

// --- STATIC METHODS --- //
  
  k_octet_t KOctet::parseHex(char ch) {
    if(ch >= 'a' && ch <= 'f') {
      return 10 + ch - 'a';
    } else if(ch >= 'A' && ch <= 'F') {
      return 10 + ch - 'A';
    } else if(ch >= '0' && ch <= '9') {
      return ch - '0';
    }
    
    return 0;
  }
  
  
  k_octet_t KOctet::parseHex(const char *chars) {
    k_octet_t v = parseHex(chars[0]);
    v = v << 4;
    v += parseHex(chars[1]);
    return v;
  }
  
  
// --- (DE)CONSTRUCTORS --- //
  
  KOctet::KOctet() {
    _value = 0;
  }
  
  
  KOctet::KOctet(const k_octet_t v) {
    _value = v;
  }
  
  
// --- METHODS --- //
  
  k_octet_t KOctet::get() const {
    return _value;
  }
  
  
  void KOctet::set(const k_octet_t v) {
    _value = v;
  }
  
  
  void KOctet::set(PPtr<KValue> other) {
    if(!other->getType()->equals(KType::OCTET)) {
      throw KTypeMismatchException(getType(), other->getType());
    }
    
    set(other.AS(KOctet)->get());
  }
  
  
  PPtr<KType> KOctet::getType() const {
    return KType::OCTET;
  }
  
  
  k_longint_t KOctet::getTotalSizeInOctets() const {
    return 1;
  }
  
  
  void KOctet::readFromBinaryStream(PPtr<InputStream> input) {
    k_integer_t v = input->read();
    if(v < 0) {
      throw IOException("Not enough bytes to read");
    }
    set((k_octet_t)v);
  }
  
  
  void KOctet::writeToBinaryStream(PPtr<OutputStream> output) const {
    output->write(get());
  }
  
  
  void KOctet::readFromObjectStream(PPtr<ObjectToken> headToken) {
    headToken->validateClass("KOctet");
    
    Ptr<Token> token = headToken->next();
    token->validateType(AttributeToken::TYPE);
    
    Ptr<AttributeToken> attrib = token.AS(AttributeToken);
    set(Int::parse(attrib->validateName("value")->getValue()));
    
    token = token->next();
    token->validateType(EndObjectToken::TYPE);
  }
  
  
  void KOctet::serialize(PPtr<ObjectSerializer> builder) const {
    builder->object("KOctet")
    ->attribute("value", get())
    ->endObject();
  }
  
  
  void KOctet::printToStream(ostream &os) const {
    os << (int)get();
  }
  
  
  
  
} // namespace type
} // namespace knorba

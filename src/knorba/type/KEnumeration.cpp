//
//  KEnumeration.cpp
//  CellMonitorTest-XCodeWrapper
//
//  Created by Hamed KHANDAN on 7/24/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

// KFoundation
#include <kfoundation/IOException.h>
#include <kfoundation/InputStream.h>
#include <kfoundation/OutputStream.h>
#include <kfoundation/ObjectStreamReader.h>

// Internal
#include "KEnumerationType.h"
#include "KTypeMismatchException.h"

// Self
#include "KEnumeration.h"

#define K_ENUMERATION_SIZE 1

namespace knorba {
namespace type {
  
  using namespace kfoundation;
  
//\/ KEnumeration /\///////////////////////////////////////////////////////////
  
// --- (DE)CONSTRUCTORS --- //
  
  KEnumeration::KEnumeration(PPtr<KEnumerationType> type) {
    _type = type;
    _value = type->getOrdinalForMemberAtIndex(0);
  }
  
  
  KEnumeration::KEnumeration(PPtr<KEnumerationType> type, const k_octet_t val) {
    _type = type;
    _value = val;
  }
  
  
// --- METHODS --- //
  
  k_octet_t KEnumeration::getOrdinal() const {
    return _value;
  }
  
  
  string KEnumeration::getLabel() const {
    return _type->getLabelForOrdinal(getOrdinal());
  }
  
  
  void KEnumeration::set(const k_octet_t value) {
    _value = value;
  }
  
  
  void KEnumeration::set(const string& value) {
    int v = _type->getOrdinalForLabel(value);
    if(v == -1) {
      throw KFException("\"" + value + "\" is not a valid label.");
    }
    set(v);
  }

  
  void KEnumeration::set(PPtr<KValue> other) {
    if(!other->getType()->equals(_type.AS(KType))) {
      throw KTypeMismatchException(getType(), other->getType());
    }
    
    set(other.AS(KEnumeration)->getOrdinal());
  }
  
  
  PPtr<KType> KEnumeration::getType() const {
    return _type.AS(KType);
  }
  
  
  k_longint_t KEnumeration::getTotalSizeInOctets() const {
    return K_ENUMERATION_SIZE;
  }
  
  
  void KEnumeration::readFromBinaryStream(PPtr<InputStream> input) {
    int v = input->read();
    if(v == -1) {
      throw IOException("Not enough data to read");
    }
    set((k_octet_t)v);
  }
  
  
  void KEnumeration::writeToBinaryStream(PPtr<OutputStream> output) const {
    output->write(getOrdinal());
  }

  
  void KEnumeration::readFromObjectStream(PPtr<ObjectToken> headToken) {
    headToken->validateClass("KEnumeration");
    
    Ptr<Token> token = headToken->next();
    token->validateType(AttributeToken::TYPE);
    
    PPtr<AttributeToken> attrib = token.AS(AttributeToken);
    attrib->validateName("value");
    
    int ordinal = _type->getOrdinalForLabel(attrib->getValue());
    if(ordinal == -1) {
      throw KFException("Value " + attrib->getValue()
                        + " is not a member of enumeration "
                        + _type->getTypeName());
    }
    
    set((k_octet_t)ordinal);
    
    token = token->next();
    token->validateType(EndObjectToken::TYPE);
  }
  
  
  void KEnumeration::serialize(PPtr<ObjectSerializer> builder) const {
    builder->object("KEnumeration")
      ->attribute("value", getLabel())
      ->endObject();
  }
  
  
  void KEnumeration::printToStream(ostream &os) const {
    os << getLabel();
  }
  
  
} // namespace type
} // namespace knorba
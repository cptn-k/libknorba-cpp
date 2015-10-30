/*---[KEnumeration.cpp]----------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : -
 |  Implements: knorba::type::KEnumeration::*
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

  /**
   * Constructor; initiates the stored value with the first enumeration member.
   *
   * @param type The type for the stored value.
   */

  KEnumeration::KEnumeration(PPtr<KEnumerationType> type) {
    _type = type;
    _value = type->getOrdinalForMemberAtIndex(0);
  }


  /**
   * Constructor; initializes the stored value with the given ordinal.
   *
   * @param type The type for the stored value.
   * @param ordinal Ordinal of the initial value.
   */
  
  KEnumeration::KEnumeration(PPtr<KEnumerationType> type,
      const k_octet_t ordinal)
  {
    _type = type;
    _value = ordinal;
  }


  /**
   * Constructor; initializes the stored value with the given label.
   *
   * @param type The type for the stored value.
   * @param value Label of the initial value.
   */

  KEnumeration::KEnumeration(PPtr<KEnumerationType> type, const string& label) {
    _type = type;
    _value = _type->getOrdinalForLabel(label);
  }
  
  
// --- METHODS --- //

  /**
   * Returns the ordinal of the stored value.
   */

  k_octet_t KEnumeration::getOrdinal() const {
    return _value;
  }


  /**
   * Returns the label for the stored value.
   */
  
  string KEnumeration::getLabel() const {
    return _type->getLabelForOrdinal(_value);
  }


  /**
   * Sets the stored value with the given ordinal.
   */
  
  void KEnumeration::set(const k_octet_t ordinal) {
    _value = ordinal;
  }


  /**
   * Sets the stored value with the given label.
   */
  
  void KEnumeration::set(const string& label) {
    int v = _type->getOrdinalForLabel(label);
    if(v == -1) {
      throw KFException("\"" + label + "\" is not a valid label.");
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

  
  void KEnumeration::deserialize(PPtr<ObjectToken> headToken) {
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
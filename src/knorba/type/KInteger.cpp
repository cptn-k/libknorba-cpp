/*---[KInteger.cpp]--------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : -
 |  Implements: knorba::type::KInteger::*
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
#include <kfoundation/Logger.h>

// Internal
#include "KType.h"
#include "KTypeMismatchException.h"

// Self
#include "KInteger.h"


namespace knorba {
namespace type {
  
  using namespace std;
  using namespace kfoundation;

  
//\/ KInteger /\///////////////////////////////////////////////////////////////
  
// --- STATIC FIELDS --- //

  /** The maximum possible value of KnoRBA `integer`. */
  const k_integer_t KInteger::MAX_VALUE = 2147483647;

  /** The minimum possible value of KnoRBA `integer` */
  const k_integer_t KInteger::MIN_VALUE = -2147483647;
  
  
// --- (DE)CONSTRUCTORS --- //

  /**
   * Constructor; sets the stored value to 0.
   */

  KInteger::KInteger() {
    _value = 0;
  }


  /**
   * Constructor; sets the stored value to the given argument.
   *
   * @param v The initial value.
   */
  
  KInteger::KInteger(const k_integer_t v) {
    _value = v;
  }
  
  
// --- METHODS --- //

  /**
   * Returns the stored value.
   */

  k_integer_t KInteger::get() const {
    return _value;
  }


  /**
   * Sets the stored value.
   *
   * @param v The value to set to.
   */
  
  void KInteger::set(const k_integer_t v) {
    _value = v;
  }
  
  
  void KInteger::set(PPtr<KValue> other) {
    if(!other->getType()->equals(KType::INTEGER)) {
      throw KTypeMismatchException(getType(), other->getType());
    }
    
    set(other.AS(KInteger)->get());
  }
  
  
  PPtr<KType> KInteger::getType() const {
    return KType::INTEGER;
  }
  
  
  k_longint_t KInteger::getTotalSizeInOctets() const {
    return 4;
  }
  
  
  void KInteger::readFromBinaryStream(PPtr<InputStream> input) {
    k_integer_t v;
    if(input->read((kf_octet_t*)&v, 4) < 4) {
      throw IOException("Not enough bytes to read");
    }
    set(v);
  }
  
  
  void KInteger::writeToBinaryStream(PPtr<OutputStream> output) const {
    k_integer_t v = get();
    output->write((kf_octet_t*)&v, 4);
  }
  

  void KInteger::deserialize(PPtr<ObjectToken> headToken) {
    headToken->validateClass("KInteger");
    
    Ptr<Token> token = headToken->next();
    token->validateType(AttributeToken::TYPE);
    
    Ptr<AttributeToken> attrib = token.AS(AttributeToken);
    set(Int::parse(attrib->validateName("value")->getValue()));
    
    token = token->next();
    token->validateType(EndObjectToken::TYPE);
  }
  
  
  void KInteger::serialize(PPtr<ObjectSerializer> builder) const {
    builder->object("KInteger")
           ->attribute("value", get())
           ->endObject();
  }
  
  
  void KInteger::printToStream(ostream &os) const {
    os << get();
  }
  
  

} // type
} // knorba
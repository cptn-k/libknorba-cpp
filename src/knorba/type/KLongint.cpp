/*---[KLongint.cpp]--------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : -
 |  Implements: knorba::type::KLongint::*
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
#include "KType.h"
#include "KTypeMismatchException.h"

// Self
#include "KLongint.h"

#define K_LONGINT_SIZE 8

namespace knorba {
namespace type {

  using namespace std;

//\/ KLongint /\///////////////////////////////////////////////////////////////
  
// --- STATIC FIELDS --- //

  /** The minimum possible value for `longint` type. */
  const k_longint_t KLongint::MIN_VALUE = -9223372036854775807;

  /** The maximum possible value for `longint` type. */
  const k_longint_t KLongint::MAX_VALUE = 9223372036854775807;
  
  
// --- (DE)CONSTUCTOR --- //

  /**
   * Constructor; sets the stored value to 0.
   */

  KLongint::KLongint() {
    _value = 0;
  }


  /**
   * Constructor; sets the stored value to the given argument.
   *
   * @param v The value to set to.
   */
  
  KLongint::KLongint(const k_longint_t v) {
    _value = v;
  }
  
  
// --- METHODS --- //

  /**
   * Sets the stored value.
   *
   * @param v The value to set to.
   */

  void KLongint::set(const k_longint_t v) {
    _value = v;
  }


  /**
   * Returns the stored value.
   */
  
  k_longint_t KLongint::get() const {
    return _value;
  }


  void KLongint::set(PPtr<KValue> other) {
    if(!other->getType()->equals(KType::LONGINT)) {
      throw KTypeMismatchException(getType(), other->getType());
    }
    
    set(other.AS(KLongint)->get());
  }

  
  PPtr<KType> KLongint::getType() const {
    return KType::LONGINT;
  }
  
  
  k_longint_t KLongint::getTotalSizeInOctets() const {
    return K_LONGINT_SIZE;
  }
  
  
  void KLongint::readFromBinaryStream(PPtr<InputStream> input) {
    k_longint_t v;
    if(input->read((kf_octet_t*)&v, K_LONGINT_SIZE) < K_LONGINT_SIZE) {
      throw IOException("Read failed.");
    }
    set(v);
  }
  
  
  void KLongint::writeToBinaryStream(PPtr<OutputStream> output) const {
    k_longint_t v = get();
    output->write((kf_octet_t*)&v, K_LONGINT_SIZE);
  }
  

  void KLongint::deserialize(PPtr<ObjectToken> headToken) {
    headToken->validateClass("KLongint");
    
    Ptr<Token> token = headToken->next();
    token->validateType(AttributeToken::TYPE);
    
    PPtr<AttributeToken> attrib = token.AS(AttributeToken);
    set(LongInt::parse(attrib->validateName("value")->getValue()));
    
    token = token->next();
    token->validateType(EndObjectToken::TYPE);
  }

  
  void KLongint::serialize(PPtr<ObjectSerializer> builder) const {
     builder->object("KLongint")
            ->attribute("value", get())
            ->endObject();
  }
  
  
  void KLongint::printToStream(ostream &os) const {
    os << get();
  }


} // type
} // knorba

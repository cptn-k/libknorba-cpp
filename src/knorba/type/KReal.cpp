/*---[KReal.cpp]-----------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : -
 |  Implements: knorba::type::KReal::*
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
#include <cassert>

// KFoundation
#include <kfoundation/IOException.h>
#include <kfoundation/InputStream.h>
#include <kfoundation/OutputStream.h>
#include <kfoundation/ObjectStreamReader.h>

// Internal
#include "KType.h"
#include "KTypeMismatchException.h"

// Self
#include "KReal.h"

#define K_REAL_SIZE 8


namespace knorba {
namespace type {
  
  using namespace std;

  
//\/ KReal /\//////////////////////////////////////////////////////////////////
  
// --- STATIC FIELDS --- //

  /** IEEE 754 representation of positive infinity. */
  const double KReal::INFINITY = 0x7FF0000000000000;

  /** IEEE 754 representation of not-a-number (NaN). */
  const double KReal::NAN      = 0x7FFFFFFFFFFFFFFF;

  
// --- (DE)CONSTRUCTORS --- //

  /**
   * Constructor; initiates the stored value with 0.
   */

  KReal::KReal() {
    _value = 0;
  }
  

  /**
   * Constructor; initiates the sotred value with the given argument.
   *
   * @param v Initial value.
   */

  KReal::KReal(const k_real_t v) {
    _value = v;
  }
  
  
// --- METHODS --- //

  /**
   * Sets the stored value to the one provided.
   *
   * @param v The value to assign.
   */

  void KReal::set(const k_real_t v) {
    _value = v;
  }
  

  /**
   * Returns the stored value.
   */

  k_real_t KReal::get() const {
    return _value;
  }


  /**
   * Returns `true` iff the stored value is NaN.
   */

  bool KReal::isNaN() const {
    k_real_t v = get();
    k_longint_t l = *(k_longint_t*)((k_octet_t*)&v);
    return ((l & 0x7FF0000000000000) == 0x7FF0000000000000)
        && ((l & 0x000FFFFFFFFFFFFF) != 0);
  }
  

  /**
   * Returns `true` iff the stored value is positive or negative infinity.
   */

  bool KReal::isInfinity() const {
    const k_real_t v = get();
    return (v == INFINITY) || (v == -INFINITY);
  }
  
  
  void KReal::set(PPtr<KValue> other) {
    if(!other->getType()->equals(KType::REAL)) {
      throw KTypeMismatchException(getType(), other->getType());
    }
    
    set(other.AS(KReal)->get());
  }
  
  
  PPtr<KType> KReal::getType() const {
    return KType::REAL;
  }
  
  
  k_longint_t KReal::getTotalSizeInOctets() const {
    assert(sizeof(k_real_t) == K_REAL_SIZE);
    return K_REAL_SIZE;
  }
  
  
  void KReal::readFromBinaryStream(PPtr<InputStream> input) {
    k_real_t v;
    if(input->read((kf_octet_t*)&v, K_REAL_SIZE) < K_REAL_SIZE) {
      throw IOException("Not enough data to read.");
    }
    set(v);
  }
  
  
  void KReal::writeToBinaryStream(PPtr<OutputStream> output) const {
    k_real_t v = get();
    output->write((kf_octet_t*)&v, K_REAL_SIZE);
  }
  
  
  void KReal::deserialize(PPtr<ObjectToken> headToken) {
    headToken->validateClass("KReal");
    
    Ptr<Token> token = headToken->next();
    token->validateType(AttributeToken::TYPE);
    
    PPtr<AttributeToken> attrib = token.AS(AttributeToken);
    set(Double::parse(attrib->validateName("value")->getValue()));
    
    token = token->next();
    token->validateType(EndObjectToken::TYPE);
  }

  
  void KReal::serialize(PPtr<ObjectSerializer> builder) const {
    builder->object("KReal")
           ->attribute("value", get())
           ->endObject();
  }
  
  
  void KReal::printToStream(ostream &os) const {
    os << get();
  }
  
  
} // namespace type
} // namespace knorba
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
#include <kfoundation/Ref.h>
#include <kfoundation/IOException.h>
#include <kfoundation/InputStream.h>
#include <kfoundation/OutputStream.h>
#include <kfoundation/ObjectStreamReader.h>
#include <kfoundation/Int.h>
#include <kfoundation/ObjectSerializer.h>

// Internal
#include "KType.h"
#include "KTypeMismatchException.h"

// Self
#include "KInteger.h"


namespace knorba {
namespace type {

  using namespace kfoundation;

  
//\/ KInteger /\///////////////////////////////////////////////////////////////
  
// --- STATIC FIELDS --- //

  /** The maximum possible value of KnoRBA `integer`. */
  const k_integer_t KInteger::MAX_VALUE = 2147483647;

  /** The minimum possible value of KnoRBA `integer` */
  const k_integer_t KInteger::MIN_VALUE = -2147483647;

  const k_octet_t KInteger::SIZE_IN_OCTETS = 4;
  
  
// --- (DE)CONSTRUCTORS --- //

  /**
   * Constructor; sets the stored value to 0.
   */

  KInteger::KInteger() {
    // No initialization to save CPU time
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

  k_octet_t* KInteger::getBuffer() {
    return (k_octet_t*)&_value;
  }


  const k_octet_t* KInteger::getBuffer() const {
    return (k_octet_t*)&_value;
  }


  /**
   * Returns the stored value.
   */

  k_integer_t KInteger::get() const {
    return readFromBuffer(getBuffer());
  }


  /**
   * Sets the stored value.
   *
   * @param v The value to set to.
   */
  
  void KInteger::set(const k_integer_t v) {
    writeToBuffer(v, getBuffer());
  }
  
  
  RefConst<KType> KInteger::getType() const {
    return KType::INTEGER;
  }
  
  
  void KInteger::printToStream(Ref<OutputStream> stream) const {
    Int(get()).printToStream(stream);
  }


  RefConst<UString> KInteger::toString() const {
    return Int(get()).toString();
  }

} // type
} // knorba
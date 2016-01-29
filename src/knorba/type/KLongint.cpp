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
#include <kfoundation/Ref.h>
#include <kfoundation/IOException.h>
#include <kfoundation/InputStream.h>
#include <kfoundation/OutputStream.h>
#include <kfoundation/ObjectStreamReader.h>
#include <kfoundation/LongInt.h>
#include <kfoundation/ObjectSerializer.h>

// Internal
#include "KType.h"
#include "KTypeMismatchException.h"

// Self
#include "KLongint.h"

namespace knorba {
namespace type {

//\/ KLongint /\///////////////////////////////////////////////////////////////
  
// --- STATIC FIELDS --- //

  /** The minimum possible value for `longint` type. */ 
  const k_longint_t KLongint::MIN_VALUE = -9223372036854775807;

  /** The maximum possible value for `longint` type. */
  const k_longint_t KLongint::MAX_VALUE = 9223372036854775807;

  const k_octet_t KLongint::SIZE_IN_OCTETS = 8;
  
  
// --- (DE)CONSTUCTOR --- //

  /**
   * Constructor; sets the stored value to 0.
   */

  KLongint::KLongint() {
    // No initialization to save CPU time.
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

  k_octet_t* KLongint::getBuffer() {
    return (k_octet_t*)&_value;
  }


  const k_octet_t* KLongint::getBuffer() const {
    return (k_octet_t*)&_value;
  }


  /**
   * Sets the stored value.
   *
   * @param v The value to set to.
   */

  void KLongint::set(const k_longint_t v) {
    writeToBuffer(_value, getBuffer());
  }


  /**
   * Returns the stored value.
   */
  
  k_longint_t KLongint::get() const {
    return readFromBuffer(getBuffer());
  }


  RefConst<KType> KLongint::getType() const {
    return KType::LONGINT;
  }
  
  
  void KLongint::printToStream(Ref<OutputStream> stream) const {
    LongInt(get()).printToStream(stream);
  }


  RefConst<UString> KLongint::toString() const {
    return LongInt(get()).toString();
  }

} // type
} // knorba
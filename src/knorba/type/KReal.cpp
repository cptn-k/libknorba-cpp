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
#include <kfoundation/Ref.h>
#include <kfoundation/IOException.h>
#include <kfoundation/InputStream.h>
#include <kfoundation/OutputStream.h>
#include <kfoundation/ObjectStreamReader.h>
#include <kfoundation/UString.h>
#include <kfoundation/Double.h>
#include <kfoundation/ObjectSerializer.h>

// Internal
#include "KType.h"
#include "KTypeMismatchException.h"

// Self
#include "KReal.h"


namespace knorba { 
namespace type {
  
  
//\/ KReal /\//////////////////////////////////////////////////////////////////
  
// --- STATIC FIELDS --- //

  /** IEEE 754 representation of positive infinity. */
  const double KReal::INFINITY = 0x7FF0000000000000;

  /** IEEE 754 representation of not-a-number (NaN). */
  const double KReal::NAN      = 0x7FFFFFFFFFFFFFFF;

  const k_octet_t KReal::SIZE_IN_OCTETS = 8;

  
// --- (DE)CONSTRUCTORS --- //

  /**
   * Constructor; initiates the stored value with 0.
   */

  KReal::KReal() {
    // No initialization to save CPU time
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

  k_octet_t* KReal::getBuffer() {
    return (k_octet_t*)&_value;
  }


  const k_octet_t* KReal::getBuffer() const {
    return (k_octet_t*)&_value;
  }


  /**
   * Sets the stored value to the one provided.
   *
   * @param v The value to assign.
   */

  void KReal::set(const k_real_t v) {
    writeToBuffer(v, getBuffer());
  }
  

  /**
   * Returns the stored value.
   */

  k_real_t KReal::get() const {
    return readFromBuffer(getBuffer());
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
  
  
  RefConst<KType> KReal::getType() const {
    return KType::REAL;
  }
  
  
  void KReal::printToStream(Ref<OutputStream> stream) const {
    Double(get()).printToStream(stream);
  }


  RefConst<UString> KReal::toString() const {
    return Double(get()).toString();
  }

} // namespace type
} // namespace knorba
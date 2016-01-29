/*---[KOctet.cpp]----------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : -
 |  Implements: knorba::type::KOctet::*
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
#include <kfoundation/UString.h>
#include <kfoundation/Int.h>
#include <kfoundation/ObjectSerializer.h>

// Internal
#include "KTypeMismatchException.h"
#include "KType.h"

// Self
#include "KOctet.h"

namespace knorba {
namespace type {

// --- STATIC METHODS --- // 

  /**
   * Parses a k_octet_t value from a single digit hexadecimal representation.
   */

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
  

  /**
   * Parses a k_octet_t from a hexadecimal representation stored in a c-style
   * string.
   */

  k_octet_t KOctet::parseHex(const char *chars) {
    k_octet_t v = parseHex(chars[0]);
    v = v << 4;
    v += parseHex(chars[1]);
    return v;
  }
  
  
// --- (DE)CONSTRUCTORS --- //

  /**
   * Constructor; sets the stored value to 0.
   */

  KOctet::KOctet() {
    // No initialization to save CPU time
  }


  /**
   * Constructor; sets the stored value to the given argument.
   */
  
  KOctet::KOctet(const k_octet_t v) {
    _value = v;
  }
  
  
// --- METHODS --- //

  k_octet_t* KOctet::getBuffer() {
    return &_value;
  }


  const k_octet_t* KOctet::getBuffer() const {
    return &_value;
  }


  /**
   * Returns the stored value.
   */

  k_octet_t KOctet::get() const {
    return *getBuffer();
  }


  /**
   * Sets the stored value.
   *
   * @param v The value to set to.
   */
  
  void KOctet::set(const k_octet_t v) {
    *getBuffer() = v;
  }
  

  RefConst<KType> KOctet::getType() const {
    return KType::OCTET;
  }
  
  
  void KOctet::printToStream(Ref<OutputStream> os) const {
    Int(get()).printToStream(os);
  }


  RefConst<UString> KOctet::toString() const {
    return Int(get()).toString();
  }

} // namespace type
} // namespace knorba
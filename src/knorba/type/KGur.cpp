/*---[KGuid.cpp]-----------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : -
 |  Implements: knorba::type::KGuid::*
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
#include <cstring>
#include <cassert>
#include <cstdlib>

// KFoundation
#include <kfoundation/Ref.h>
#include <kfoundation/IOException.h>
#include <kfoundation/InputStream.h>
#include <kfoundation/OutputStream.h>
#include <kfoundation/ObjectStreamReader.h>
#include <kfoundation/PrintWriter.h>
#include <kfoundation/BufferOutputStream.h>
#include <kfoundation/UString.h>
#include <kfoundation/ObjectSerializer.h>

// Internal
#include "KType.h"
#include "KOctet.h"
#include "KInteger.h"
#include "KTypeMismatchException.h"

// Self
#include "KGur.h"

namespace knorba {
namespace type {

  using namespace kfoundation;
  
  
//\/ KGuid /\/////////////////////////////////////////////////////////////
  
// --- STATIC FIELDS --- //
  
  const StaticRefConst<KGur> KGur::NULL_VALUE = new KGur(null_value());
  const k_octet_t KGur::SIZE_IN_OCTETS = 16;


// --- STATIC METHODS --- //

  k_gur_t KGur::null_value() {
    k_gur_t value;
    memset(&value, 0, SIZE_IN_OCTETS);
    value.localRef.index = -1;
    return value;
  }


// --- (DE)CONSTRUCTORS --- //


  /**
   * Constructor; initiates the stored value with zero().
   */
  
  KGur::KGur() {
    // No initialization to save CPU time.
  }


  /**
   * Constructor; initiates the stored value with the given argument.
   */

  KGur::KGur(const k_gur_t& v) {
    _value = v;
  }
  
  
// --- METHODS --- //

  k_octet_t* KGur::getBuffer() {
    return (k_octet_t*)&_value;
  }


  const k_octet_t* KGur::getBuffer() const {
    return (k_octet_t*)&_value;
  }


  /**
   * Returns the stored value.
   */

  const k_gur_t& KGur::get() const {
    return readFromBuffer(getBuffer());
  }


  /**
   * Sets the stored value to the given value.
   */
  
  void KGur::set(const k_gur_t& v) {
    writeToBuffer(v, getBuffer());
  }


  bool KGur::appIdEquals(const k_gur_t& other) const {
    return memcmp(_value.appId, other.appId, sizeof(k_appid_t)) == 0;
  }


  void KGur::randomizeKey() {
    kf_int16_t key = 0;
    while (key == 0) {
      key = rand()%16711670 - 8355830;
    }
    _value.localRef.key = key;
  }


  void KGur::randomizeAppId() {
    for(int i = 7; i >= 0; i--) {
      _value.appId[i] = rand()%255;
    }
  }


  void KGur::printToStream(Ref<OutputStream> stream, format_t format) const {
    bool printAppId = false;
    bool printAppIdSerparator = false;
    bool printManagerAndIndex = false;
    bool printKey = false;

    switch (format) {
      case APP_MANAGER_INDEX_KEY:
        printKey = true;
        /* no break */

      case APP_MANAGER_INDEX:
        printAppIdSerparator = true;
        printManagerAndIndex = true;
        /* no break */

      case APP:
        printAppId = true;
        break;

      case MANAGER_INDEX_KEY:
        printKey = true;
        /* no break */

      case MANAGER_INDEX:
        printManagerAndIndex = true;
        break;
    }

    Ref<PrintWriter> pw = new PrintWriter(stream);

    if(printAppId) {
      pw->printHex(_value.appId, 8);
    }

    if(printAppIdSerparator) {
      pw->print(':');
    }

    if(printManagerAndIndex) {
      pw->printHex(_value.localRef.manager)
        ->print('.')
        ->printHex((kf_octet_t*)&_value.localRef.index, 4);
    }

    if(printKey) {
      pw->print('(')
        ->printHex((kf_octet_t*)&_value.localRef.key, 2)
        ->print(')');
    }
  }


  Ref<UString> KGur::toString(format_t format) const {
    Ref<BufferOutputStream> stream = new BufferOutputStream();
    printToStream(stream.AS(OutputStream), format);
    return stream->getString();
  }


  // Inherited from KValue
  
  RefConst<KType> KGur::getType() const {
    return KType::GUR;
  }
  
  
  bool KGur::equals(RefConst<KGur> other) const {
    return memcmp(&_value, &other->_value, 16) == 0;
  }
  

  void KGur::printToStream(Ref<OutputStream> stream) const {
    printToStream(stream, APP_MANAGER_INDEX_KEY);
  }


  RefConst<UString> KGur::toString() const {
    return toString(APP_MANAGER_INDEX_KEY);
  }


  bool operator==(const k_gur_t& first, const k_gur_t& second) {
    return (memcmp(first.appId, second.appId, sizeof(k_appid_t)) == 0)
        && first.localRef == second.localRef;
  }

} // type
} // knorba
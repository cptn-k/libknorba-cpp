/*---[KAny.cpp]------------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : -
 |  Implements: knorba::type::KAny::*
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
#include <kfoundation/OutputStream.h>
#include <kfoundation/ObjectSerializer.h>

// Internal
#include "KType.h"
#include "KLongint.h"
#include "KTypeMismatchException.h"
#include "KString.h"

// Self
#include "KAny.h"

namespace knorba {
namespace type {
  
  
//\/ KAny /\///////////////////////////////////////////////////////////////////

// --- STATIC METHODS --- //

  void KAny::initBuffer(k_octet_t* buffer) {
    *(kf_uref_t*)buffer = RefBase::NULL_REF;
  }


  void KAny::writeToBuffer(Ref<KValue> value, k_octet_t *buffer) {
    cleaupBuffer(buffer);
    kf_uref_t* ref = (kf_uref_t*)buffer;
    *ref = value.get();
    System::getMasterMemoryManager().getManagerAtIndex(ref->manager)
      .retain(ref->index, ref->key);
  }


  Ref<KValue> KAny::readFromBuffer(const k_octet_t* buffer) {
    return Ref<KValue>(*(kf_uref_t*)buffer);
  }


  void KAny::cleaupBuffer(k_octet_t* buffer) {
    kf_uref_t* ref = (kf_uref_t*)buffer;
    if(NOT_NULL_REF(*ref)) {
      System::getMasterMemoryManager().getManagerAtIndex(ref->manager)
          .release(ref->index, ref->key);
      *ref = RefBase::NULL_REF;
    }
  }


// --- (DE)CONSTRUCTORS --- //


  /**
   * Constructor; Initiates the stored value with `nothing` (KValue::NOTHING).
   */

  KAny::KAny() {
    initBuffer((k_octet_t*)&_value);
  }


  /**
   * Constructor; Initiates the stored value with the given argument.
   */
  
  KAny::KAny(Ref<KValue> value) {
    initBuffer((k_octet_t*)&_value);
    writeToBuffer(value, (k_octet_t*)&_value);
  }

  
// --- METHODS --- //

  k_octet_t* KAny::getBuffer() {
    return (k_octet_t*)&_value;
  }


  const k_octet_t* KAny::getBuffer() const {
    return (k_octet_t*)&_value;
  }


  /**
   * Returns the stored value.
   */

  Ref<KValue> KAny::get() const {
    return readFromBuffer(getBuffer());
  }


  /**
   * Sets the stored value.
   */
  
  void KAny::set(Ref<KValue> v) {
    writeToBuffer(v, getBuffer());
  }


// Inherited from KValue //

  void KAny::set(RefConst<KValue> other) {
    if(!other->getType()->equals(KType::ANY)) {
      throw KTypeMismatchException(getType(), other->getType());
    }
    set(other.AS(KAny)->get());
  }
  
  
  RefConst<KType> KAny::getType() const {
    return KType::ANY;
  }
    
  
} // namespace type
} // namespace knorba
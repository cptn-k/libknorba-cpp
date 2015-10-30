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
#include <kfoundation/Ptr.h>

// Internal
#include "../Runtime.h"
#include "KType.h"
#include "KLongint.h"
#include "KTypeMismatchException.h"
#include "KString.h"

// Self
#include "KAny.h"

#define K_ANY_HEADER_SIZE 8

namespace knorba {
namespace type {
  
  
//\/ KAny /\///////////////////////////////////////////////////////////////////
  
// --- (DE)CONSTRUCTORS --- //


  /**
   * Constructor; Initiates the stored value with `nothing` (KValue::NOTHING).
   */

  KAny::KAny() {
    _value = KValue::NOTHING;
    _rt = NULL;
  }


  /**
   * Constructor; Initiates the stored value with the given argument.
   */
  
  KAny::KAny(Ptr<KValue> value) {
    _value = value;
    _rt = NULL;
  }

  
// --- METHODS --- //
  

  /**
   * Returns the stored value.
   */

  PPtr<KValue> KAny::getValue() const {
    return _value;
  }


  /**
   * Sets the stored value.
   */
  
  void KAny::setValue(PPtr<KValue> v) {
    _value = v;
  }


  /**
   * This object needs a reference to runtime in order perform
   * readFromBinaryStream() operation.
   *
   * @param rt Reference to the current runtime.
   */
  
  void KAny::setRuntime(Runtime& rt) {
    _rt = &rt;
  }
  
  
// Inherited from KValue //

  void KAny::set(PPtr<KValue> other) {
    if(!other->getType()->equals(KType::ANY)) {
      throw KTypeMismatchException(getType(), other->getType());
    }
    
    _value = other.AS(KAny)->_value;
  }
  
  
  PPtr<KType> KAny::getType() const {
    return KType::ANY;
  }
  
  
  k_longint_t KAny::getTotalSizeInOctets() const {
    return K_ANY_HEADER_SIZE + _value->getTotalSizeInOctets();
  }



  /**
   * Writes the internal value by decoding the given InputStream.
   *
   * @note Call setRuntime() before calling this method; otherwise an exception
   *       will be thrown.
   *
   * @param input The InputStream to read the value from.
   */

  void KAny::readFromBinaryStream(PPtr<InputStream> input) {
    if(IS_NULL(_rt)) {
      throw KFException("Cannot read binary stream. setRuntime() should be "
          "done before readFromBinaryStream().");
    }
    
    Ptr<KLongint> hash = new KLongint();
    hash->readFromBinaryStream(input);
  
    PPtr<KType> type = _rt->getTypeByHash(hash->get());
    
    if(type.isNull()) {
      throw KFException("No type is registered for hashcode "
          + LongInt::toString(hash->get()));
    }
    
    _value = type->instantiate();
    _value->readFromBinaryStream(input);
  }
  
  
  void KAny::writeToBinaryStream(PPtr<OutputStream> output) const {
    Ptr<KLongint> hash = new KLongint();
    hash->set(_value->getType()->getTypeNameHash());
    hash->writeToBinaryStream(output);
    _value->writeToBinaryStream(output);
  }
  
  /** This operation is not supported. */
  void KAny::deserialize(PPtr<ObjectToken> head) {
    throw KFException("KAny does not support deserialization from object stream");
  }
  
  
  void KAny::serialize(PPtr<ObjectSerializer> builder) const {
    builder->object("KAny")
           ->attribute("dataType", _value->getType()->getTypeName())
           ->member("value")->object<KValue>(_value)
           ->endObject();
  }
  
  
} // namespace type
} // namespace knorba
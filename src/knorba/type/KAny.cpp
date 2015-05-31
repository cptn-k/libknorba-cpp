//
//  KAny.cpp
//  CellMonitorTest-XCodeWrapper
//
//  Created by Hamed KHANDAN on 8/12/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

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
  
  KAny::KAny() {
    _value = KValue::NOTHING;
    _rt = NULL;
  }
  
  
  KAny::KAny(Ptr<KValue> value) {
    _value = value;
    _rt = NULL;
  }
  
  
  KAny::~KAny() {
    // Nothing;
  }
  
  
// --- METHODS --- //
  
  PPtr<KValue> KAny::getValue() const {
    return _value;
  }

  
  void KAny::setValue(PPtr<KValue> v) {
    _value = v;
  }
  
  
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
  
  
  void KAny::readFromObjectStream(PPtr<ObjectToken> head) {
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
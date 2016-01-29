/*---[KValue.h]------------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : knorba::type::KNothing::*
 |  Implements: knorba::type::KValue::*
 |              knorba::type::KNothing::*
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

// KFoundation
#include <kfoundation/Ref.h>
#include <kfoundation/InputStream.h>
#include <kfoundation/OutputStream.h>
#include <kfoundation/ObjectStreamReader.h>
#include <kfoundation/ObjectSerializer.h>

// Internal
#include "KType.h"
#include "KTypeMismatchException.h"
#include "../Ontology.h"

// Self
#include "KValue.h"

namespace knorba {
namespace type {


//\/ KNothing /\///////////////////////////////////////////////////////////////
    
  class KNothing : public KValue {
    
  // --- CONSTRUCTOR --- //
    
    public: KNothing();
    
  // --- METHODS --- //

    protected: k_octet_t* getBuffer();
    protected: const k_octet_t* getBuffer() const;
    
    public: void set(RefConst<KValue> other);
    public: RefConst<KType> getType() const;
    public: k_longint_t getTotalSizeInOctets() const;
    public: void readFromBinaryStream(Ref<InputStream> input);
    public: void writeToBinaryStream(Ref<OutputStream> output) const;

    // From KValue::StreamDeserializer
    public: void deserialize(Ref<ObjectToken> headToken);
    
    // From KValue::SerializingStreamer
    public: void serialize(Ref<ObjectSerializer> builder) const;
    
  };
  
  
  KNothing::KNothing() {
    // nothing;
  }


  k_octet_t* KNothing::getBuffer() {
    return NULL;
  }


  const k_octet_t* KNothing::getBuffer() const {
    return NULL;
  }
  
  
  void KNothing::set(RefConst<KValue> other) {
    if(!other->getType()->equals(KType::NOTHING)) {
      throw KTypeMismatchException(getType(), other->getType());
    }
  }


  RefConst<KType> KNothing::getType() const {
    return KType::NOTHING;
  }


  void KNothing::readFromBinaryStream(Ref<InputStream> input) {
    // Nothing;
  }

  
  void KNothing::writeToBinaryStream(Ref<OutputStream> output) const {
    // Nothing
  }

  
  void KNothing::deserialize(Ref<ObjectToken> token) {
    token->validateClass(K"KNothing");
    token->next();
  }

  
  void KNothing::serialize(Ref<ObjectSerializer> builder) const {
    builder->object(K"KNothing")->endObject();
  }


//\/ KValue /\/////////////////////////////////////////////////////////////////
  
// --- STATIC FIELDS --- //
  
  const StaticRef<KValue> KValue::NOTHING = new KNothing();


// --- METHODS --- //

  k_longint_t KValue::getTotalSizeInOctets() const {
    return getType()->getStreamSizeInOctets(getBuffer());
  }


  void KValue::set(RefConst<KValue> other) {
    if(!other->getType()->equals(getType())) {
      throw KTypeMismatchException(getType(), other->getType());
    }

    memcpy(getBuffer(), other->getBuffer(), getTotalSizeInOctets());
  }


  void KValue::readFromBinaryStream(Ref<InputStream> input,
      RefConst<Ontology> ontology)
  {
    getType()->writeStreamToBuffer(input, ontology, getBuffer());
  }


  void KValue::writeToBinaryStream(Ref<OutputStream> output) const {
    getType()->writeBufferToStream(getBuffer(), output);
  }


  void KValue::deserialize(Ref<ObjectToken> headToken,
      RefConst<Ontology> ontology)
  {
    getType()->deserializeIntoBuffer(headToken, ontology, getBuffer());
  }


  void KValue::serialize(Ref<ObjectSerializer> serializer) const {
    getType()->serializeBuffer(getBuffer(), serializer);
  }


} // namespace type
} // namespace knorba
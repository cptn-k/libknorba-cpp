//
//  KValue.cpp
//  CellMonitorTest-XCodeWrapper
//
//  Created by Hamed KHANDAN on 7/18/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

// KFoundation
#include <kfoundation/InputStream.h>
#include <kfoundation/OutputStream.h>
#include <kfoundation/ObjectStreamReader.h>
#include <kfoundation/ObjectSerializer.h>

// Internal
#include "KEnumeration.h"
#include "KInteger.h"
#include "KLongint.h"
#include "KReal.h"
#include "KRecord.h"
#include "KString.h"
#include "KTruth.h"
#include "KTypeMismatchException.h"

// Self
#include "KValue.h"

namespace knorba {
namespace type {


//\/ KNothing /\///////////////////////////////////////////////////////////////
    
    class KNothing : public KValue {
      
    // --- CONSTRUCTOR --- //
      
      public: KNothing();
      
    // --- METHODS --- //
      
      public: void set(PPtr<KValue> other);
      public: PPtr<KType> getType() const;
      public: k_longint_t getTotalSizeInOctets() const;
      public: void readFromBinaryStream(PPtr<InputStream> input);
      public: void writeToBinaryStream(PPtr<OutputStream> output) const;
      public: void readFromObjectStream(PPtr<ObjectToken> headToken);
      
      // From KValue::SerializingStreamer
      public: void serialize(PPtr<ObjectSerializer> builder) const;
    };
    
    
    KNothing::KNothing() {
      // nothing;
    }
    
    
    void KNothing::set(PPtr<KValue> other) {
      if(!other->getType()->equals(KType::NOTHING)) {
        throw KTypeMismatchException(getType(), other->getType());
      }
    }
  
  
    PPtr<KType> KNothing::getType() const {
      return KType::NOTHING;
    }

    
    k_longint_t KNothing::getTotalSizeInOctets() const {
      return 0;
    }
    
    
    void KNothing::readFromBinaryStream(PPtr<InputStream> input) {
      // Nothing;
    }
  
    
    void KNothing::writeToBinaryStream(PPtr<OutputStream> output) const {
      // Nothing
    }
  
    
    void KNothing::readFromObjectStream(PPtr<ObjectToken> token) {
      token->validateClass("KNothing");
      token->next();
    }
  
    
    void KNothing::serialize(PPtr<ObjectSerializer> builder) const {
      builder->object("KNothing")->endObject();
    }
  

//\/ KValue /\/////////////////////////////////////////////////////////////////
  
// --- STATIC FIELDS --- //
  
  const SPtr<KValue> KValue::NOTHING(new KNothing());
  
  
} // namespace type
} // namespace knorba
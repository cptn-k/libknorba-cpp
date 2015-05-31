//
//  KValue.h
//  CellMonitorTest-XCodeWrapper
//
//  Created by Hamed KHANDAN on 7/18/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#ifndef KNORBA_TYPE_KVALUE
#define KNORBA_TYPE_KVALUE

// Internal
#include "definitions.h"

// Super
#include <kfoundation/ManagedObject.h>
#include <kfoundation/SerializingStreamer.h>

namespace kfoundation {
  class InputStream;
  class OutputStream;
  class ObjectToken;
}

namespace knorba {
namespace type {
  
  using namespace kfoundation;
  
  class KType;
  
  class KValue : public ManagedObject, public SerializingStreamer {
    
  // --- STATIC FIELDS --- //
    
    public: static const SPtr<KValue> NOTHING;
    
  
  // --- PURE VIRTUAL METHODS --- //
    
    public: virtual void set(PPtr<KValue> other) = 0;
    public: virtual PPtr<KType> getType() const = 0;
    public: virtual k_longint_t getTotalSizeInOctets() const = 0;
    public: virtual void readFromBinaryStream(PPtr<InputStream> input) = 0;
    public: virtual void writeToBinaryStream(PPtr<OutputStream> output) const = 0;
    public: virtual void readFromObjectStream(PPtr<ObjectToken> headToken) = 0;
    
    // Forward from SerializingStreamer
    virtual void serialize(PPtr<ObjectSerializer> serializer) const = 0;
    
  };
  

} // namespace type
} // namespace knorba

#endif /* defined(KNORBA_TYPE_KVALUE) */
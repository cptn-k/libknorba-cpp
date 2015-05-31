//
//  KAny.h
//  CellMonitorTest-XCodeWrapper
//
//  Created by Hamed KHANDAN on 8/12/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#ifndef KNORBA_TYPE_KANY
#define KNORBA_TYPE_KANY

// KFoundation
#include <kfoundation/ObjectStreamReader.h>

// Super
#include "KValue.h"

namespace knorba {
  class Runtime;
}

namespace knorba {
namespace type {
  
  using namespace kfoundation;
  
  
//\/ KAny /\///////////////////////////////////////////////////////////////////
  
  class KAny : public KValue {
    
  // --- FIELDS --- //
    
    private: Ptr<KValue> _value;
    private: Runtime* _rt;
  
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: KAny();
    public: KAny(Ptr<KValue> value);
    public: ~KAny();
    
    
  // --- METHODS --- //
    
    public: PPtr<KValue> getValue() const;
    public: void setValue(PPtr<KValue> v);
    public: void setRuntime(Runtime& rt);
    
    // Inherited from KValue //
    public: void set(PPtr<KValue> other);
    public: PPtr<KType> getType() const;
    public: k_longint_t getTotalSizeInOctets() const;
    public: void readFromBinaryStream(PPtr<InputStream> input);
    public: void writeToBinaryStream(PPtr<OutputStream> output) const;
    public: void readFromObjectStream(PPtr<ObjectToken> headToken);
    
    // Inherited from KValue::SerializingStreamer //
    void serialize(PPtr<ObjectSerializer> builder) const;
    
  };
  
  
} // namespace type
} // namespace knorba

#endif /* defined(KNORBA_TYPE_KANY) */

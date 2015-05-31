//
//  KLongInteger.h
//  CellMonitorTest-XCodeWrapper
//
//  Created by Hamed KHANDAN on 7/18/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#ifndef KONRBA_TYPE_KLONGINT
#define KONRBA_TYPE_KLONGINT

// Super
#include "KValue.h"

namespace knorba {
namespace type {

  using namespace kfoundation;
  
  class KType;

//\/ KLongInt /\///////////////////////////////////////////////////////////////
  
  class KLongint : public KValue {
        
  // --- STATIC FIELDS --- //
    
    public: static const k_longint_t MIN_VALUE;
    public: static const k_longint_t MAX_VALUE;
    
    
  // --- FIELDS --- //
    
    private: k_longint_t _value;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: KLongint();
    public: KLongint(const k_longint_t v);
    
    
  // --- METHODS --- //
    
    public: virtual void set(const k_longint_t v);
    public: virtual k_longint_t get() const;
    
    // Inherited from KValue
    public: void set(PPtr<KValue> other);
    public: PPtr<KType> getType() const;
    public: k_longint_t getTotalSizeInOctets() const;
    public: void readFromBinaryStream(PPtr<InputStream> input);
    public: void writeToBinaryStream(PPtr<OutputStream> output) const;
    public: void readFromObjectStream(PPtr<ObjectToken> headToken);
    
    // Inherited from KValue::SerializingStreamer
    void serialize(PPtr<ObjectSerializer> builder) const;
    
    // Inherited from KValue::SerializingStreamer::Streamer
    void printToStream(ostream& os) const;
  };
  
} // namespace type
} // namespace knorba

#endif /* defined(KONRBA_TYPE_KLONGINT) */

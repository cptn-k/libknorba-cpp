//
//  KReal.h
//  CellMonitorTest-XCodeWrapper
//
//  Created by Hamed KHANDAN on 7/18/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#ifndef KNORBA_TYPE_KREAL
#define KNORBA_TYPE_KREAL

// Super
#include "KValue.h"

namespace knorba {
namespace type {

  using namespace kfoundation;
  
  class KType;
  
//\/ KReal /\//////////////////////////////////////////////////////////////////
  
  class KReal : public KValue {
    
  // --- STATIC FIELDS --- //

    public: static const k_real_t INFINITY;
    public: static const k_real_t NaN;

    
  // --- FIELDS --- //
    
    private: k_real_t _value;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: KReal();
    public: KReal(const k_real_t v);
    
    
  // --- METHODS --- //
    
    public: virtual void set(const k_real_t v);
    public: virtual k_real_t get() const;
    public: bool isNaN() const;
    public: bool isInfinity() const;
    
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

#endif /* defined(KNORBA_TYPE_KREAL) */

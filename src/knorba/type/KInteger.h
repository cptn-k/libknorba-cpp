//
//  KInteger.h
//  CellMonitorTest-XCodeWrapper
//
//  Created by Hamed KHANDAN on 7/18/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#ifndef KNORBA_TYPE_KINTEGER
#define KNORBA_TYPE_KINTEGER

// Super
#include "KValue.h"

namespace knorba {
namespace type {

  using namespace kfoundation;
  
  class KType;
  
  
//\/ KInteger /\///////////////////////////////////////////////////////////////

  class KInteger : public KValue {
    
  // --- STATIC FIELDS --- //
    
    public: static const k_integer_t MAX_VALUE;
    public: static const k_integer_t MIN_VALUE;
  
    
  // --- FIELDS --- //
    
    private: k_integer_t _value;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: KInteger();
    public: KInteger(const k_integer_t v);

    
  // --- METHODS --- //
    
    public: virtual k_integer_t get() const;
    public: virtual void set(const k_integer_t v);
    
    // Inherited from KValue
    public: PPtr<KType> getType() const;
    public: k_longint_t getTotalSizeInOctets() const;
    public: void readFromBinaryStream(PPtr<InputStream> input);
    public: void writeToBinaryStream(PPtr<OutputStream> output) const;
    public: void readFromObjectStream(PPtr<ObjectToken> headToken);
    public: void set(PPtr<KValue> other);

    // From KValue::SerializingStreamer
    void serialize(PPtr<ObjectSerializer> builder) const;
    
    // From KValue::SerilizingStreamer::Streamer
    void printToStream(ostream& os) const;
  };


} // namespace type
} // namespace knorba

#endif /* defined(KNORBA_TYPE_KINTEGER) */

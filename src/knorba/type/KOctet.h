//
//  KOctet.h
//  CellMonitorTest-XCodeWrapper
//
//  Created by Hamed KHANDAN on 8/8/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#ifndef KNORBA_TYPE_KOCTET
#define KNORBA_TYPE_KOCTET

// Super
#include "KValue.h"

namespace knorba {
namespace type {
  
  using namespace kfoundation;
  
  class KOctet : public KValue {
    
  // --- STATIC METHODS --- //
    
    public: static k_octet_t parseHex(char ch);
    public: static k_octet_t parseHex(const char* chars);
    
    
  // --- FIELDS --- //
    
    private: k_octet_t _value;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: KOctet();
    public: KOctet(const k_octet_t v);
    
    
  // --- METHODS --- //
    
    public: virtual k_octet_t get() const;
    public: virtual void set(const k_octet_t v);
    
    // Inherited from KValue //
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
  
}
}

#endif /* defined(KNORBA_TYPE_KOCTET) */

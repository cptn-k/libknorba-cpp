//
//  KTruth.h
//  CellMonitorTest-XCodeWrapper
//
//  Created by Hamed KHANDAN on 7/18/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#ifndef KNORBA_TYPE_KTRUTH
#define KNORBA_TYPE_KTRUTH

// Super
#include "KValue.h"

namespace knorba {
namespace type {
  
//\/ Global /\/////////////////////////////////////////////////////////////////
  
  k_truth_t k_truth_not(const k_truth_t& a);
  k_truth_t k_truth_and(const k_truth_t& a, const k_truth_t& b);
  k_truth_t k_truth_or(const k_truth_t& a, const k_truth_t& b);
  k_truth_t k_truth_xor(const k_truth_t& a, const k_truth_t& b);


//\/ KTruth /\/////////////////////////////////////////////////////////////////
  
  class KTruth : public KValue {
    
  // --- FIELDS --- //
    
    private: k_truth_t _value;
    
  
  // --- STATIC METHODS --- //
    
    public: static string toString(const k_truth_t v);
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: KTruth();
    public: KTruth(const k_truth_t v);

    
  // --- METHODS --- //
    
    public: virtual void set(const k_truth_t v);
    public: virtual k_truth_t get() const;
    
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

#endif /* defined(KNORBA_TYPE_KTRUTH) */

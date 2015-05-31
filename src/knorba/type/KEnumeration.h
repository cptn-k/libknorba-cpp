//
//  KEnumeration.h
//  CellMonitorTest-XCodeWrapper
//
//  Created by Hamed KHANDAN on 7/24/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#ifndef KNORBA_TYPE_KENUMERATION
#define KNORBA_TYPE_KENUMERATION

// Super
#include "KValue.h"

namespace knorba {
namespace type {

  class KEnumerationType;
  class KType;

//\/ KEnumeration /\///////////////////////////////////////////////////////////
  
  class KEnumeration : public KValue {
    
  // --- FIELDS --- //
    
    private: Ptr<KEnumerationType> _type;
    private: k_octet_t _value;
    
    
  // --- (DE)COSTRUCTORS --- //
    
    public: KEnumeration(PPtr<KEnumerationType> type);
    public: KEnumeration(PPtr<KEnumerationType> type, const k_octet_t val);
    
    
  // --- METHODS --- //
    
    public: virtual k_octet_t getOrdinal() const;
    public: string getLabel() const;
    public: virtual void set(const k_octet_t value);
    public: void set(const string& value);
    
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

#endif /* defined(KNORBA_TYPE_KENUMERATION) */
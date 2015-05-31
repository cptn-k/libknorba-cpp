//
//  KGUID.h
//  CellMonitorTest-XCodeWrapper
//
//  Created by Hamed KHANDAN on 7/18/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#ifndef KNORBA_TYPE_KGUID
#define KNORBA_TYPE_KGUID

// KFoundation
#include <kfoundation/Logger.h>

// Suepr
#include "KValue.h"

namespace knorba {
namespace type {

//\/ KGlobalUid /\/////////////////////////////////////////////////////////////
  
  class KGlobalUid : public KValue {
    
  // --- STATIC FIELDS --- //
    
    private: static k_guid_t ZERO;
    private: static bool IS_ZERO_INITIALIZED;
    
  
  // --- FIELDS --- //
    
    private: k_guid_t _value;
    
    
  // --- STATIC METHODS --- //
    
    public: static const k_guid_t& zero();
    public: static void randomizeAppId(k_guid_t& target);
    public: static void randomizeKey(k_guid_t& target);
    public: static bool areOnTheSameApp(const k_guid_t& first, const k_guid_t& second);
    public: static bool areOnTheSameNode(const k_guid_t& first, const k_guid_t& second);
    public: static string toString(const k_guid_t& value);
    public: static string toShortString(const k_guid_t& value);
    public: static string appIdToString(const k_guid_t& value);
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: KGlobalUid();
    public: KGlobalUid(const k_guid_t& v);
    
    
  // --- METHODS --- //
    
    public: virtual k_guid_t get() const;
    public: virtual void set(const k_guid_t& v);
    
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

  
  Logger::Stream& operator<<(Logger::Stream& stream, const k_guid_t& guid);
  ostream& operator<<(ostream& os, const k_guid_t& guid);
  bool operator==(const k_guid_t& a, const k_guid_t& b);
  
} // namespace type
} // namespace knorba

#endif /* defined(KNORBA_TYPE_KGUID) */

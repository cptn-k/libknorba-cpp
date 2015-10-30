/*---[KGuid.h]-------------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : knorba::type::KGuid::*
 |  Implements: -
 |
 |  Copyright (c) 2013, 2014, 2015, RIKEN (The Institute of Physical and
 |  Chemial Research) All rights reserved.
 |
 |  Author: Hamed KHANDAN (hamed.khandan@port.kobe-u.ac.jp)
 |
 |  This file is distributed under the KnoRBA Free Public License. See
 |  LICENSE.TXT for details.
 |
 *//////////////////////////////////////////////////////////////////////////////

#ifndef KNORBA_TYPE_KGUID
#define KNORBA_TYPE_KGUID

// KFoundation
#include <kfoundation/Logger.h>

// Suepr
#include "KValue.h"

namespace knorba {
namespace type {

//\/ KGuid /\/////////////////////////////////////////////////////////////
  
  /**
   * Wrapper class for KnoRBA `GUID` (Globally Unique Identifier). 
   * GUID is a segmented 8-bit value as represented by `knorba::type::k_guid_t`.
   *
   * @headerfile KGuid.h <knorba/type/KGuid.h>
   */

  class KGuid : public KValue {
    
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
    
    public: KGuid();
    public: KGuid(const k_guid_t& v);
    
    
  // --- METHODS --- //
    
    public: virtual k_guid_t get() const;
    public: virtual void set(const k_guid_t& v);
    
    // Inherited from KValue
    public: void set(PPtr<KValue> other);
    public: PPtr<KType> getType() const;
    public: k_longint_t getTotalSizeInOctets() const;
    public: void readFromBinaryStream(PPtr<InputStream> input);
    public: void writeToBinaryStream(PPtr<OutputStream> output) const;

    // Inherited from KValue::StreamDeserializer
    public: void deserialize(PPtr<ObjectToken> headToken);
    
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
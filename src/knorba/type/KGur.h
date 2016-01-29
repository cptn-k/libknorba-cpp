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
  class Runtime;
}

namespace knorba {
namespace type {

//\/ KGuid /\/////////////////////////////////////////////////////////////
  
  /**
   * Wrapper class for KnoRBA `GUID` (Globally Unique Identifier). 
   * GUID is a segmented 8-bit value as represented by `knorba::type::k_guid_t`.
   *
   * @headerfile KGuid.h <knorba/type/KGuid.h>
   */

  class KGur : public KValue, public Comparable<KGur> {

  // --- NESTED TYPES --- //

    public: typedef enum {
      APP,
      APP_MANAGER_INDEX,
      APP_MANAGER_INDEX_KEY,
      MANAGER_INDEX,
      MANAGER_INDEX_KEY
    } format_t;


  // --- STATIC FIELDS --- //
    
    public: static const StaticRefConst<KGur> NULL_VALUE;
    public: static const k_octet_t SIZE_IN_OCTETS;


  // --- STATIC METHODS --- //

    public: static k_gur_t null_value();
    public: static inline const k_gur_t& readFromBuffer(const k_octet_t* const buffer);
    public: static inline void writeToBuffer(const k_gur_t& value,
        k_octet_t* const buffer);

  
  // --- FIELDS --- //
    
    private: k_gur_t _value;
    

  // --- (DE)CONSTRUCTORS --- //
    
    public: KGur();
    public: KGur(const k_gur_t& v);
    
    
  // --- METHODS --- //

    protected: virtual k_octet_t* getBuffer();
    protected: virtual const k_octet_t* getBuffer() const;

    public: const k_gur_t& get() const;
    public: void set(const k_gur_t& v);
    public: bool appIdEquals(const k_gur_t& other) const;
    public: void randomizeKey();
    public: void randomizeAppId();
    public: void printToStream(Ref<OutputStream> stream, format_t format) const;
    public: Ref<UString> toString(format_t format) const;

    // Inherited from KValue
    public: RefConst<KType> getType() const;

    // Inherited from Comparable<KGur>
    public: bool equals(RefConst<KGur> other) const;
    
    // Inherited from KValue::SerializingStreamer::Streamer
    public: void printToStream(Ref<OutputStream> stream) const;
    public: RefConst<UString> toString() const;
    
  };


  inline const k_gur_t& KGur::readFromBuffer(const k_octet_t* const buffer) {
    return *(k_gur_t*)buffer;
  }


  inline void KGur::writeToBuffer(const k_gur_t& value, k_octet_t* const buffer)
  {
    *(k_gur_t*)buffer = value;
  }


} // namespace type
} // namespace knorba

#endif /* defined(KNORBA_TYPE_KGUID) */
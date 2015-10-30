/*---[KAny.h]--------------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : knorba::type::KAny::*
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

  /**
   * Wrapper class and C++ representation of KnoRBA `any` type. A value of
   * `any` type can store a value of any other type.
   *
   * @headerfile KAny.h <knorba/type/KAny.h>
   */

  class KAny : public KValue {
    
  // --- FIELDS --- //
    
    private: Ptr<KValue> _value;
    private: Runtime* _rt;
  
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: KAny();
    public: KAny(Ptr<KValue> value);

    
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

    // Inherited from KValue::StreamDeserializer
    public: void deserialize(PPtr<ObjectToken> headToken);
    
    // Inherited from KValue::SerializingStreamer //
    void serialize(PPtr<ObjectSerializer> builder) const;
    
  };
  
  
} // namespace type
} // namespace knorba

#endif /* defined(KNORBA_TYPE_KANY) */

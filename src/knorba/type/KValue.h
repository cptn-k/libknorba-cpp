/*---[KValue.cpp]----------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : knorba::type::KValue::*
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

#ifndef KNORBA_TYPE_KVALUE
#define KNORBA_TYPE_KVALUE

// Internal
#include "definitions.h"

// Super
#include <kfoundation/ManagedObject.h>
#include <kfoundation/SerializingStreamer.h>
#include <kfoundation/StreamDeserializer.h>

namespace kfoundation {
  class InputStream;
  class OutputStream;
  class ObjectToken;
}

namespace knorba {
namespace type {
  
  using namespace kfoundation;
  
  class KType;


  /**
   * Abstract superclass for all KnoRBA type-wrapper classes. 
   * Wrapper classes are responsible for storing, managing, serializing,
   * and deseralizaing KnoRBA binary data format.
   *
   * @headerfile KValue.h <knorba/type/KValue.h>
   */

  class KValue : public ManagedObject, public SerializingStreamer,
      public StreamDeserializer
  {
    
  // --- STATIC FIELDS --- //

    /** Wrapper for KnoRBA `nothing` literal */
    public: static const SPtr<KValue> NOTHING;
    
  
  // --- PURE VIRTUAL METHODS --- //

    /**
     * Copies the value for this KValue from another one. The given KValue
     * should be of the same type as this one. I.e. 
     * `this->getType()->equals(other->getType())` shoule return `true`.
     */

    public: virtual void set(PPtr<KValue> other) = 0;


    /**
     * Returns the KnoRBA type for the stored value.
     */

    public: virtual PPtr<KType> getType() const = 0;


    /**
     * Returns the size of the stored value when serialized.
     */

    public: virtual k_longint_t getTotalSizeInOctets() const = 0;


    /**
     * Sets the stored value by deserializing the given input stream.
     *
     * @param input The input stream to deserialize from.
     */

    public: virtual void readFromBinaryStream(PPtr<InputStream> input) = 0;


    /**
     * Serializes the stored value on to the given output stream.
     *
     * @param output The output stream to serialize to.
     */

    public: virtual void writeToBinaryStream(PPtr<OutputStream> output) const = 0;


    // Forward from StreamDeserializer

    /**
     * Implements compatibility with kfoundation::StreamDeserializer
     * interface.
     */

    public: virtual void deserialize(PPtr<ObjectToken> headToken) = 0;


    // Forward from SerializingStreamer

    /**
     * Implements compatibility with kfoundation::SerializingStreamer interface.
     */

    virtual void serialize(PPtr<ObjectSerializer> serializer) const = 0;
    
  };
  

} // namespace type
} // namespace knorba

#endif /* defined(KNORBA_TYPE_KVALUE) */
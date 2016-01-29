/*---[KType.h]-------------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : knorba::type::KType::*
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

#ifndef KNORBA_TYPE_KTYPE
#define KNORBA_TYPE_KTYPE

// Internal
#include "definitions.h"

// Super
#include <kfoundation/KFObject.h>
#include <kfoundation/Streamer.h>
#include <kfoundation/Comparable.h>

namespace kfoundation {
  class InputStream;
  class UString;
  class ObjectToken;
}

namespace knorba {
  class Ontology;
}

namespace knorba {
namespace type {

  using namespace kfoundation;
  
  
//\/ Forward Declerations /\///////////////////////////////////////////////////
  
  class KValue;
  class KString;
  
  
//\/ KType /\//////////////////////////////////////////////////////////////////

  /**
   * Represents a KnoRBA type, and offers useful runtime information about them.
   * KType and its derivatives interact with KValue and its derivatives as
   * follows.
   *
   * Type to value:
   *
   *     Ptr<KValue> value = type->instantiate();
   *
   * Value to type:
   *
   *     Ptr<KType> type = value->getType();
   *
   * KnoRBA primitive types are represented by constant memebers of KType:
   * TRUTH, OCTET, INTEGER, LONGINT, REAL, GUID, STRING, RAW, ANY, and NOTHING.
   *
   * Compound types are represented by KType derivatives, KEnumerationType,
   * KRecordType and KGridType.
   * 
   * @note In case of types for which hasConstantSize() returns `false`, 
   *       getSizeInOctets() always returns 0. You need to invoke
   *       KValue::getTotalSizeInOctets() on each instance to determine their
   *       size.
   *
   * @headerfile KType.h <knorba/type/KType.h>
   */

  class KType : public KFObject, public Streamer, public Comparable<KType> {
    
  // --- STATIC FIELDS --- //
    
    public: static const StaticRef<KType> TRUTH;
    public: static const StaticRef<KType> OCTET;
    public: static const StaticRef<KType> INTEGER;
    public: static const StaticRef<KType> LONGINT;
    public: static const StaticRef<KType> REAL;
    public: static const StaticRef<KType> GUR;
    public: static const StaticRef<KType> STRING;
    public: static const StaticRef<KType> RAW;
    public: static const StaticRef<KType> ANY;
    public: static const StaticRef<KType> NOTHING;
    
    
  // --- FIELDS --- //
    
    private: RefConst<UString> _typeName;
    private: k_longint_t _hashCode;

    
  // --- CONSTRUCTOR --- //
    
    protected: KType(RefConst<UString> typeName);


  // --- STATIC METHODS --- //

//    public: static Ref<KType> fromKnois(RefConst<UString> str);
//    public: static Ref<KType> fromKnois(Ref<InputStream> stream);

    
  // --- METHODS --- //
    
    public: RefConst<UString> getTypeName() const;
    public: k_longint_t getHashCode() const;

    /**
     * Checks if the type represented by this object is castable to the given
     * type.
     */

    public: virtual bool isCastableTo(RefConst<KType> t) const = 0;


    /**
     * Checks if this type can be automatically casted to the given type
     * by KnoIL language interpreter.
     */

    public: virtual bool isAutomaticCastableTo(RefConst<KType> t) const = 0;


    /**
     * If hasConstantSize() returns `true`, this method returns the amount of
     * octets a value of this type consumes when stored in memory or sent over
     * a stream; otherwise it resturns 0.
     */

    public: virtual k_integer_t  getSizeInOctets() const = 0;


    /**
     * Returns `true` iif this object represents a primitive type.
     */

    public: virtual bool isPrimitive() const = 0;


    /**
     * Returns true iif the type represented by this object has constant size.
     * Types with variable size are `string` (KType::STRING),
     * `raw` (KType::RAW), and `grid` (KGridType). The rest of them have 
     * constant sizes.
     */

    public: virtual bool hasConstantSize() const = 0;


    /**
     * Returns an instance of an appropriate subclass of KValue corresponding
     * to the type represented by this object. For example,
     *
     *      KType::INTEGER->instantiate()
     *
     * will return an instance of KInteger.
     */

    public: virtual Ref<KValue> instantiate() const = 0;

    public: virtual k_longint_t getStreamSizeInOctets(const k_octet_t* buffer)
        const;

    public: virtual void writeBufferToStream(const k_octet_t* buffer,
        Ref<OutputStream> stream) const = 0;

    public: virtual void writeStreamToBuffer(Ref<InputStream> stream,
        RefConst<Ontology> ontology, k_octet_t* buffer) const = 0;

    public: virtual void serializeBuffer(const k_octet_t* buffer,
        Ref<ObjectSerializer> serializer) const = 0;

    public: virtual void deserializeIntoBuffer(Ref<ObjectToken> head,
        RefConst<Ontology> ontology, k_octet_t* buffer) const = 0;

    public: virtual void initializeBuffer(k_octet_t* buffer) const;
    public: virtual void cleanupBuffer(k_octet_t* buffer) const;
    
    public: virtual void copyBuffer(const k_octet_t* src, k_octet_t* target)
        const;

    public: virtual RefConst<UString> toKnois() const;


    // From Comparable<KType>

    /**
     * Checks if type represented by this object is equivalant to the one
     * represented by the given argument.
     */

    public: virtual bool equals(RefConst<KType> t) const;
    
    
    // From Streamer
    public: virtual void printToStream(Ref<OutputStream> os) const;
    public: virtual RefConst<UString> toString() const;
    
  };

} // namespace type
} // namespace knorba

#endif /* defined(KNORBA_TYPE_KTYPE) */
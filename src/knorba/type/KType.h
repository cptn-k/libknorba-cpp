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

// Std
#include <string>

// Internal
#include "definitions.h"

// Super
#include <kfoundation/ManagedObject.h>
#include <kfoundation/Streamer.h>

namespace knorba {
namespace type {

  using namespace std;
  using namespace kfoundation;
  
  
//\/ Forward Declerations /\///////////////////////////////////////////////////
  
  class KValue;
  
  
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

  class KType : public ManagedObject, public Streamer {
    
  // --- STATIC FIELDS --- //
    
    public: static const SPtr<KType> TRUTH;
    public: static const SPtr<KType> OCTET;
    public: static const SPtr<KType> INTEGER;
    public: static const SPtr<KType> LONGINT;
    public: static const SPtr<KType> REAL;
    public: static const SPtr<KType> GUID;
    public: static const SPtr<KType> STRING;
    public: static const SPtr<KType> RAW;
    public: static const SPtr<KType> ANY;
    public: static const SPtr<KType> NOTHING;
    
    
  // --- FIELDS --- //
    
    private: string _typeName;
    private: k_longint_t _nameHash;
    
    
  // --- CONSTRUCTOR --- //
    
    protected: KType(string name);
    
    
  // --- METHODS --- //
    
    public: const string& getTypeName() const;
    public: k_longint_t getTypeNameHash() const;

    /**
     * Checks if the type represented by this object is castable to the given
     * type.
     */

    public: virtual bool isCastableTo(PPtr<KType> t) const = 0;


    /**
     * Checks if this type can be automatically casted to the given type
     * by KnoIL language interpreter.
     */

    public: virtual bool isAutomaticCastableTo(PPtr<KType> t) const = 0;


    /**
     * Checks if type represented by this object is equivalant to the one
     * represented by the given argument.
     */

    public: virtual bool equals(PPtr<KType> t) const;


    /**
     * If hasConstantSize() returns `true`, this method returns the amount of
     * octets a value of this type consumes when stored in memory or sent over
     * a stream; otherwise it resturns 0.
     */

    public: virtual int  getSizeInOctets() const = 0;


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

    public: virtual Ptr<KValue> instantiate() const = 0;

    public: virtual string toKnois() const;
    
    // From Streamer
    void printToStream(ostream& os) const;
    
  };

} // namespace type
} // namespace knorba

#endif /* defined(KNORBA_TYPE_KTYPE) */

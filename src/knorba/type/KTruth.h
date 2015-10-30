/*---[KTruth.h]------------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : knorba::type::KTruth::*
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

#ifndef KNORBA_TYPE_KTRUTH
#define KNORBA_TYPE_KTRUTH

// Super
#include "KValue.h"

namespace knorba {
namespace type {
  
//\/ Global /\/////////////////////////////////////////////////////////////////

  k_truth_t tnot(const k_truth_t& a);
  k_truth_t tand(const k_truth_t& a, const k_truth_t& b);
  k_truth_t tor(const k_truth_t& a, const k_truth_t& b);
  k_truth_t txor(const k_truth_t& a, const k_truth_t& b);


//\/ KTruth /\/////////////////////////////////////////////////////////////////


  /**
   * Wrapper class for KnoRBA 3-state `truth` type. A value of type `truth`
   * can be either `T` (for true), `F` (for false), or `X` (for unknown). 
   * These values are represented by `knorba::type::T`, `knorba::type::F`, 
   * and `knorba::type::X`, respectively. The scalar type associated with this
   * wrapper class is `knorba::type::k_truth_t`.
   *
   * KnoRBA employes 3-state logic rather than the common Boolean logic mostly
   * because it simplifies implementation of KnoIL interpreter and makes it
   * easier for a group of agents to engage in collaborative descision making.
   *
   * @headerfile KTruth.h <knorba/type/KTruth.h>
   */

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

    // Inherited from KValue::StreamDeserializer
    public: void deserialize(PPtr<ObjectToken> headToken);

    // Inherited from KValue::SerializingStreamer
    public: void serialize(PPtr<ObjectSerializer> builder) const;
    
    // Inherited from KValue::SerializingStreamer::Streamer
    public: void printToStream(ostream& os) const;
    
  };
    
} // namespace type
} // namespace knorba

#endif /* defined(KNORBA_TYPE_KTRUTH) */

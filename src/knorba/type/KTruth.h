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

  // --- STATIC FIELDS --- //

    public: static const StaticRefConst<UString> STR_T;
    public: static const StaticRefConst<UString> STR_F;
    public: static const StaticRefConst<UString> STR_X;
    public: static const k_truth_t F = 0;
    public: static const k_truth_t T = 1;
    public: static const k_truth_t X = 2;

    
  // --- FIELDS --- //
    
    private: k_truth_t _value;
    
  
  // --- STATIC METHODS --- //
    
    public: static RefConst<UString> toString(const k_truth_t v);
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: KTruth();
    public: KTruth(const k_truth_t v);

    
  // --- METHODS --- //

    protected: virtual k_octet_t* getBuffer();
    protected: virtual const k_octet_t* getBuffer() const;

    public: void set(const k_truth_t v);
    public: k_truth_t get() const;
    
    // Inherited from KValue
    public: RefConst<KType> getType() const;

    // Inherited from KValue::SerializingStreamer::Streamer
    public: void printToStream(Ref<OutputStream> os) const;
    
  };
    
} // namespace type
} // namespace knorba

#endif /* defined(KNORBA_TYPE_KTRUTH) */
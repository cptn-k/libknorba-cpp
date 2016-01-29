/*---[KTruth.cpp]----------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : -
 |  Implements: knorba::type::KTruth::*
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

// KFoundation
#include <kfoundation/Ref.h>
#include <kfoundation/IOException.h>
#include <kfoundation/InputStream.h>
#include <kfoundation/OutputStream.h>
#include <kfoundation/ObjectStreamReader.h>
#include <kfoundation/UString.h>
#include <kfoundation/ObjectSerializer.h>

// Internal
#include "KType.h"
#include "KTypeMismatchException.h"

// Self
#include "KTruth.h"


namespace knorba {
namespace type {

  using namespace kfoundation;

  
//\/ Global /\/////////////////////////////////////////////////////////////////
  
  namespace {
    const k_truth_t NOT_TABLE[3] = {KTruth::F, KTruth::T, KTruth::X};
  
    const k_truth_t AND_TABLE[3][3] = {
      {KTruth::F, KTruth::F, KTruth::F},
      {KTruth::F, KTruth::T, KTruth::X},
      {KTruth::F, KTruth::X, KTruth::X}
    };
    
    const k_truth_t OR_TABLE[3][3] = {
      {KTruth::T, KTruth::T, KTruth::T},
      {KTruth::T, KTruth::F, KTruth::X},
      {KTruth::T, KTruth::X, KTruth::X}
    };
    
    const k_truth_t XOR_TABLE[3][3] = {
      {KTruth::F, KTruth::T, KTruth::X},
      {KTruth::T, KTruth::F, KTruth::X},
      {KTruth::X, KTruth::X, KTruth::X}
    };
  }


  /** 3-state logical *not* */
  k_truth_t tnot(const k_truth_t& a) {
    return NOT_TABLE[(unsigned char)a];
  }


  /** 3-state logical *and* */ 
  k_truth_t tand(const k_truth_t& a, const k_truth_t& b) {
    return AND_TABLE[(unsigned char)a][(unsigned char)b];
  }


  /** 3-state logical *or* */
  k_truth_t tor(const k_truth_t& a, const k_truth_t& b) {
    return OR_TABLE[(unsigned char)a][(unsigned char)b];
  }


  /** 3-state logical *exclusive or* */
  k_truth_t txor(const k_truth_t& a, const k_truth_t& b) {
    return XOR_TABLE[(unsigned char)a][(unsigned char)b];
  }
  
  
//\/ KTruth /\/////////////////////////////////////////////////////////////////
  
  
// --- STATIC METHODS --- //

  /**
   * Returns string representation of the given scalar `truth` value.
   */

  RefConst<UString> KTruth::toString(const k_truth_t v) {
    switch (v) {
      case KTruth::F:
        return STR_F;
        
      case KTruth::T:
        return STR_T;
        
      case KTruth::X:
        return STR_X;
    }
    
    return K"???";
  }
  
  
// --- (DE)CONSTRUCTORS --- //

  /**
   * Constructor; sets the stored value to `X`.
   */

  KTruth::KTruth() {
    // No initialization to save CPU time
  }


  /**
   * Constructor; sets the stored value to the given argument.
   *
   * @param v Initial value.
   */

  KTruth::KTruth(const k_truth_t v) {
    _value = v;
  }
  
  
// --- METHODS --- //

  k_octet_t* KTruth::getBuffer() {
    return &_value;
  }


  const k_octet_t* KTruth::getBuffer() const {
    return &_value;
  }


  /**
   * Sets the stored value to the given argument.
   */

  void KTruth::set(const k_truth_t v) {
    *getBuffer() = v;
  }


  /**
   * Returns the stored value.
   */
  
  k_truth_t KTruth::get() const {
    return *getBuffer();
  }


  RefConst<KType> KTruth::getType() const {
    return KType::TRUTH;
  }
  
  
  void KTruth::printToStream(Ref<OutputStream> os) const {
    toString(get())->printToStream(os);
  }
  
} // namespace type
} // namespace knorba
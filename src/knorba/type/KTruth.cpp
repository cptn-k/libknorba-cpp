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
#include <kfoundation/IOException.h>
#include <kfoundation/InputStream.h>
#include <kfoundation/OutputStream.h>
#include <kfoundation/ObjectStreamReader.h>

// Internal
#include "KType.h"
#include "KTypeMismatchException.h"

// Self
#include "KTruth.h"


namespace knorba {
namespace type {

  using namespace std;
  using namespace kfoundation;

  
//\/ Global /\/////////////////////////////////////////////////////////////////
  
  namespace {
    const k_truth_t NOT_TABLE[3] = {F, T, X};
  
    const k_truth_t AND_TABLE[3][3] = {
      {F, F, F},
      {F, T, X},
      {F, X, X}
    };
    
    const k_truth_t OR_TABLE[3][3] = {
      {T, T, T},
      {T, F, X},
      {T, X, X}
    };
    
    const k_truth_t XOR_TABLE[3][3] = {
      {F, T, X},
      {T, F, X},
      {X, X, X}
    };
  }
  
  /** 3-state logical *not* */
  k_truth_t k_truth_not(const k_truth_t& a) {
    return NOT_TABLE[(unsigned char)a];
  }
  
  /** 3-state logical *and* */
  k_truth_t k_truth_and(const k_truth_t& a, const k_truth_t& b) {
    return AND_TABLE[(unsigned char)a][(unsigned char)b];
  }
  
  /** 3-state logical *or* */
  k_truth_t k_truth_or(const k_truth_t& a, const k_truth_t& b) {
    return OR_TABLE[(unsigned char)a][(unsigned char)b];
  }
  
  /** 3-state logical *exclusive or* */
  k_truth_t k_truth_xor(const k_truth_t& a, const k_truth_t& b) {
    return XOR_TABLE[(unsigned char)a][(unsigned char)b];
  }
  
  
//\/ KTruth /\/////////////////////////////////////////////////////////////////
  
  
// --- STATIC METHODS --- //

  /**
   * Returns string representation of the given scalar `truth` value.
   */

  string KTruth::toString(const k_truth_t v) {
    switch (v) {
      case F:
        return "F";
        
      case T:
        return "T";
        
      case X:
        return "X";
    }
    
    return "???";
  }
  
  
// --- (DE)CONSTRUCTORS --- //

  /**
   * Constructor; sets the stored value to `X`.
   */

  KTruth::KTruth() {
    _value = X;
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

  /**
   * Sets the stored value to the given argument.
   */

  void KTruth::set(const k_truth_t v) {
    _value = v;
  }


  /**
   * Returns the stored value.
   */
  
  k_truth_t KTruth::get() const {
    return _value;
  }


  void KTruth::set(PPtr<KValue> other) {
    if(!other->getType()->equals(KType::TRUTH)) {
      throw KFException("Incompatible types. Expected: " + getType()->toString()
                        + ". Provided: " + other->getType()->toString());
    }
    set(other.AS(KTruth)->get());
  }


  PPtr<KType> KTruth::getType() const {
    return KType::TRUTH;
  }
  
  
  k_longint_t KTruth::getTotalSizeInOctets() const {
    return 1;
  }
  
  
  void KTruth::readFromBinaryStream(PPtr<InputStream> input) {
    int v = input->read();
    if(v == -1) {
      throw IOException("Read failed");
    }
    set((k_truth_t)v);
  }
  
  
  void KTruth::writeToBinaryStream(PPtr<OutputStream> output) const {
    output->write((kf_octet_t)get());
  }
  
  
  void KTruth::deserialize(PPtr<ObjectToken> headToken) {
    headToken->validateClass("KTruth");
    
    Ptr<Token> token = headToken->next();
    token->validateType(Token::ATTRIBUTE);
    
    PPtr<AttributeToken> attrib = token.AS(AttributeToken);
    const string& str = attrib->validateName("value")->getValue();
    
    if(str.length() == 0) {
      set(X);
      return;
    }
    
    char ch = attrib->getValue()[0];
    
    switch (ch) {
      case 't':
      case 'T':
        set(T);
        break;
        
      case 'f':
      case 'F':
        set(F);
        break;
        
      case 'x':
      case 'X':
      default:
        set(X);
        break;
    }
    
    token = token->next();
    token->validateType(EndObjectToken::TYPE);
  }
  
  
  void KTruth::serialize(PPtr<ObjectSerializer> builder) const {
    builder->object("KTruth")
           ->attribute("value", toString(get()))
           ->endObject();
  }
  
  
  void KTruth::printToStream(ostream &os) const {
    os << toString(get());
  }
  
} // namespace type
} // namespace knorba
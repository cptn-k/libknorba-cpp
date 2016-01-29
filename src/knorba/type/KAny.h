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

  // --- STATIC METHODS --- //

    public: static void initBuffer(k_octet_t* buffer);
    public: static void writeToBuffer(Ref<KValue> value, k_octet_t* buffer);
    public: static Ref<KValue> readFromBuffer(const k_octet_t* buffer);
    public: static void cleaupBuffer(k_octet_t* buffer);


  // --- FIELDS --- //
    
    private: kf_uref_t _value;
  
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: KAny();
    public: KAny(Ref<KValue> value);

    
  // --- METHODS --- //

    protected: virtual k_octet_t* getBuffer();
    protected: virtual const k_octet_t* getBuffer() const;

    public: Ref<KValue> get() const;
    public: void set(Ref<KValue> v);

    // Inherited from KValue //
    public: void set(RefConst<KValue> other);
    public: RefConst<KType> getType() const;

  };
  
  
} // namespace type
} // namespace knorba

#endif /* defined(KNORBA_TYPE_KANY) */
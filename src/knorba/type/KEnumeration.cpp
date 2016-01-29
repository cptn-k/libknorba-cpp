/*---[KEnumeration.cpp]----------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : -
 |  Implements: knorba::type::KEnumeration::*
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
#include <kfoundation/Int.h>
#include <kfoundation/ObjectSerializer.h>

// Internal
#include "KEnumerationType.h"
#include "KTypeMismatchException.h"

// Self
#include "KEnumeration.h"

namespace knorba {
namespace type { 
  
  using namespace kfoundation;

//\/ KEnumeration /\///////////////////////////////////////////////////////////

// --- STATIC FIELDS --- //

  const k_octet_t KEnumeration::SIZE_IN_OCTETS = 1;


// --- (DE)CONSTRUCTORS --- //

  /**
   * Constructor; initiates the stored value with the first enumeration member.
   *
   * @param type The type for the stored value.
   */

  KEnumeration::KEnumeration(RefConst<KEnumerationType> type) {
    _type = type;
    _value = type->getOrdinalForMemberAtIndex(0);
  }


  /**
   * Constructor; initializes the stored value with the given ordinal.
   *
   * @param type The type for the stored value.
   * @param ordinal Ordinal of the initial value.
   */
  
  KEnumeration::KEnumeration(RefConst<KEnumerationType> type,
      const k_octet_t ordinal)
  {
    _type = type;
    _value = ordinal;
  }


  /**
   * Constructor; initializes the stored value with the given label.
   *
   * @param type The type for the stored value.
   * @param value Label of the initial value.
   */

  KEnumeration::KEnumeration(RefConst<KEnumerationType> type,
      RefConst<UString> label)
  {
    _type = type;
    _value = _type->getOrdinalForLabel(label);
  }
  
  
// --- METHODS --- //

  k_octet_t* KEnumeration::getBuffer() {
    return &_value;
  }


  const k_octet_t* KEnumeration::getBuffer() const {
    return &_value;
  }


  /**
   * Returns the ordinal of the stored value.
   */

  k_octet_t KEnumeration::getOrdinal() const {
    return *getBuffer();
  }


  /**
   * Returns the label for the stored value.
   */
  
  RefConst<UString> KEnumeration::getLabel() const {
    return _type->getLabelForOrdinal(*getBuffer());
  }


  /**
   * Sets the stored value with the given ordinal.
   */
  
  void KEnumeration::set(const k_octet_t ordinal) {
    *getBuffer() = ordinal;
  }


  /**
   * Sets the stored value with the given label.
   */
  
  void KEnumeration::set(RefConst<UString> label) {
    *getBuffer() = _type->getOrdinalForLabel(label);
  }

  
  RefConst<KType> KEnumeration::getType() const {
    return _type.AS(KType);
  }
  
  
  void KEnumeration::printToStream(Ref<OutputStream> stream) const {
    getLabel()->printToStream(stream);
  }


  RefConst<UString> KEnumeration::toString() const {
    return getLabel();
  }

} // namespace type
} // namespace knorba
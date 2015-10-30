/*---[KTypeMismatchException.cpp]------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : -
 |  Implements: knorba::type::KTypeMismatchException::*
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

// Internal
#include "KType.h"

#include "KTypeMismatchException.h"

namespace knorba {
namespace type {

  string makeMessage(PPtr<KType> expected, PPtr<KType> provided) {
    return "Type mismatch. \"" + expected->getTypeName() + "\" expected but \""
        + provided->getTypeName() + "\" is provided.";
  }

  
  /**
   * Constructor.
   *
   * @param expected The expected type.
   * @param privided The provided type.
   */

  KTypeMismatchException::KTypeMismatchException(PPtr<KType> expected,
      PPtr<KType> provided)
  : KFException(makeMessage(expected, provided))
  {
    setName("KTypeMismatchException");
  }
  
} // type
} // knorba
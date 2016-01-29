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

// KFoundation
#include <kfoundation/Ref.h>
#include <kfoundation/StringPrintWriter.h>
#include <kfoundation/UString.h>

// Internal
#include "KType.h"

#include "KTypeMismatchException.h"

namespace knorba {
namespace type {

   RefConst<UString> KTypeMismatchException::makeMessage(
    RefConst<KType> expected, RefConst<KType> provided)
  {
    StringPrintWriter pw;
    pw << "Type mismatch. \"" << expected->getTypeName()
       << "\" expected but \"" << provided->getTypeName()
       << "\" is provided.";
    return pw.toString();
  }

  
  /**
   * Constructor.
   *
   * @param expected The expected type.
   * @param privided The provided type.
   */

  KTypeMismatchException::KTypeMismatchException(RefConst<KType> expected,
      RefConst<KType> provided)
  : KFException(makeMessage(expected, provided))
  {
    setName(K"KTypeMismatchException");
  }
  
} // type
} // knorba
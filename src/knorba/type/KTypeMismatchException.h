/*---[KTypeMismatchException.h]--------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : knorba::type::KTypeMismatchException::*
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

#ifndef __KnoRBA__KTypeMismatchException__
#define __KnoRBA__KTypeMismatchException__

#include <kfoundation/Ptr.h>
#include <kfoundation/KFException.h>

namespace knorba {
namespace type {
  
  using namespace kfoundation;
  
  class KType;

  /**
   * Exception indicating mismatch of two KnoRBA types.
   * 
   * @headerfile KTypeMismatchException.h <knorba/type/KTypeMismatchException.h>
   */
  
  class KTypeMismatchException : public KFException {
    
    public: KTypeMismatchException(PPtr<KType> expected, PPtr<KType> provided);
    
  };
  
} // namespace type
} // namespace knorba

#endif /* defined(__KnoRBA__KTypeMismatchException__) */

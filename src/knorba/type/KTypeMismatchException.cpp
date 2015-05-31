//
//  KTypeMismatchException.cpp
//  KnoRBA
//
//  Created by Hamed KHANDAN on 12/4/14.
//  Copyright (c) 2014 Kay Khandan. All rights reserved.
//

// Internal
#include "KType.h"

#include "KTypeMismatchException.h"

namespace knorba {
namespace type {
  
  string makeMessage(PPtr<KType> expected, PPtr<KType> provided) {
    return "Type mismatch. \"" + expected->getTypeName() + "\" expected but \""
        + provided->getTypeName() + "\" is provided.";
  }
  
  KTypeMismatchException::KTypeMismatchException(PPtr<KType> expected,
      PPtr<KType> provided)
  : KFException(makeMessage(expected, provided))
  {
    setName("KTypeMismatchException");
  }
  
} // type
} // knorba
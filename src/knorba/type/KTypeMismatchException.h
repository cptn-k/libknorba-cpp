//
//  KTypeMismatchException.h
//  KnoRBA
//
//  Created by Hamed KHANDAN on 12/4/14.
//  Copyright (c) 2014 Kay Khandan. All rights reserved.
//

#ifndef __KnoRBA__KTypeMismatchException__
#define __KnoRBA__KTypeMismatchException__

#include <kfoundation/Ptr.h>
#include <kfoundation/KFException.h>

namespace knorba {
namespace type {
  
  using namespace kfoundation;
  
  class KType;
  
  class KTypeMismatchException : public KFException {
    
    public: KTypeMismatchException(PPtr<KType> expected, PPtr<KType> provided);
    
  };
  
} // namespace type
} // namespace knorba

#endif /* defined(__KnoRBA__KTypeMismatchException__) */

//
//  KDynamicValue.h
//  KnoRBA
//
//  Created by Hamed KHANDAN on 1/9/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

#ifndef KNORBA_TYPE_KDYNAMICVALUE
#define KNORBA_TYPE_KDYNAMICVALUE

#include "KValue.h"

namespace knorba {
namespace type {

  /**
   * Common denominator of KGrid and KRecord.
   */
  
  class KDynamicValue : public KValue {
    public: virtual k_octet_t* getBaseAddress() const = 0;
  };
  
} // type
} // knorba

#endif // KNORBA_TYPE_KDYNAMICVALUE

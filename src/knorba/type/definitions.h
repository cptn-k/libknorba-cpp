//
//  definitions.h
//  KnoRBA
//
//  Created by Hamed KHANDAN on 3/20/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

#ifndef KnoRBA_definitions_h
#define KnoRBA_definitions_h

#include <kfoundation/definitions.h>

namespace knorba {
namespace type {
  
  using namespace kfoundation;
  
  typedef kf_octet_t k_octet_t;
  typedef kf_int32_t k_integer_t;
  typedef kf_int64_t k_longint_t;
  typedef double     k_real_t;
  
  typedef k_octet_t k_truth_t;
  
  const k_truth_t F = 0;
  const k_truth_t T = 1;
  const k_truth_t X = 2;
  
  typedef k_octet_t k_appid_t[8];
  
  typedef struct {
    k_appid_t   appId;
    kf_int16_t  nodeRank;
    kf_int16_t  key;
    k_integer_t lid;
  } k_guid_t;

} // namespace type
} // namespace knorba

#endif

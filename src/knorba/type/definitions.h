/*---[definitions.h]-------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : knorba::type::k_octet_t
 |              knorba::type::k_integer_t
 |              knorba::type::k_longint_t
 |              knorba::type::k_real_t
 |              knorba::type::k_truth_t
 |              knorba::type::F
 |              knorba::type::T
 |              knorba::type::X
 |              knorba::type::k_appid_t
 |              knorba::type::k_guid_t
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

#ifndef KnoRBA_definitions_h
#define KnoRBA_definitions_h

#include <kfoundation/definitions.h>

namespace knorba {
namespace type {
  
  using namespace kfoundation;

  /** C equivalant for KnoRBA octet type (8-bit unsigned integer) */
  typedef kf_octet_t k_octet_t;

  /** C equivalant for KnoRBA integer type (32-bit signed integer) */
  typedef kf_int32_t k_integer_t;

  /** C equivalant for KnoRBA longint type (64-bit signed integer) */
  typedef kf_int64_t k_longint_t;

  /** C equivalant for KnoRBA real type (64-bit floating point) */
  typedef double     k_real_t;

  /** C equivalant for KnoRBA truth type (3-state logic) */
  typedef k_octet_t k_truth_t;

  /** AppId part of GUID */
  typedef k_octet_t k_appid_t[8];

  /** GUID (Gloablly Unique ID) */
  typedef struct {
    k_appid_t   appId;    ///< AppID
    kf_uref_t   localRef;
  } k_gur_t;

  class KType;
  class KEnumerationType;
  class KRecordType;
  class KGridType;
  class KValue;
  class KTruth;
  class KOctet;
  class KInteger;
  class KLongint;
  class KGur;
  class KEnumeration;
  class KRaw;
  class KString;
  class KAny;
  class KRecord;
  class KGrid;

  bool operator ==(const k_gur_t& first, const k_gur_t& second);
  
} // namespace type
} // namespace knorba

#endif
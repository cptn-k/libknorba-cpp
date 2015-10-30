/*---[KGuid.cpp]-----------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : -
 |  Implements: knorba::type::KGuid::*
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

// Std
#include <cstring>
#include <cassert>
#include <cstdlib>

// KFoundation
#include <kfoundation/Int.h>
#include <kfoundation/IOException.h>
#include <kfoundation/InputStream.h>
#include <kfoundation/OutputStream.h>
#include <kfoundation/ObjectStreamReader.h>
#include <kfoundation/Logger.h>

// Internal
#include "KType.h"
#include "KOctet.h"
#include "KInteger.h"
#include "KTypeMismatchException.h"

// Self
#include "KGuid.h"

#define K_GUID_SIZE 16

namespace knorba {
namespace type {

  using namespace std;  
  using namespace kfoundation;
  
  
//\/ KGuid /\/////////////////////////////////////////////////////////////
  
// --- STATIC FIELDS --- //
  
  k_guid_t KGuid::ZERO;
  bool KGuid::IS_ZERO_INITIALIZED = false;
  
  
// --- STATIC METHODS --- //

  /**
   * Returns reference to internally stored zero constant.
   */

  const k_guid_t& KGuid::zero() {
    if(IS_ZERO_INITIALIZED) {
      return ZERO;
    }
    assert(sizeof(k_guid_t) == K_GUID_SIZE);
    memset(&ZERO, 0, K_GUID_SIZE);
    return ZERO;
  }


  /**
   * Rewrites the AppID part of the given argument with a random value.
   */
  
  void KGuid::randomizeAppId(k_guid_t& target) {
    for(int i = 0; i < 8; i++) {
      target.appId[i] = rand()%255;
    }
  }


  /**
   * Replaces the *key* part of the given argument with a random number.
   */
  
  void KGuid::randomizeKey(k_guid_t& target) {
    target.key = rand()%16711670 - 8355830;
  }


  /**
   * Returns `true` iff the two given arguments have the same AppID.
   */
  
  bool KGuid::areOnTheSameApp(const k_guid_t &first, const k_guid_t &second) {
    return memcmp(&first.appId, &second.appId, sizeof(k_appid_t)) == 0;
  }
  

  /**
   * Returns `true` iff two given arguments have the same AppID and node rank.
   */

  bool KGuid::areOnTheSameNode(const k_guid_t& first, const k_guid_t& second) {
    return areOnTheSameApp(first, second) && first.nodeRank == second.nodeRank;
  }


  char* printBytes(char* buffer, const k_octet_t* bytes, const int count) {
    for(int i = 0; i < count; i++) {
      buffer += sprintf(buffer, "%02X", bytes[i]);
    }
    return buffer;
  }
  

  /**
   * Returns the string representation of the given value.
   */

  string KGuid::toString(const k_guid_t& value) {
    char buffer[50];
    char* p = buffer;
    
    p = printBytes(p, value.appId, 8);
    sprintf(p, ":%04X:%08X", value.nodeRank, value.lid);
    
    return string(buffer);
  }


  /**
   * Returns a string containing NodeRank and LocalId parts of the given value.
   */
  
  string KGuid::toShortString(const k_guid_t& value) {
    char buffer[20];
    sprintf(buffer, "%04X:%08X", value.nodeRank, value.lid);
    return string(buffer);
  }
  

  /**
   * Returns the AppId part of the given value as a string.
   */

  string KGuid::appIdToString(const k_guid_t& value) {
    char buffer[20];
    char* p = buffer;
    p = printBytes(p, value.appId, 8);
    return string(buffer);
  }
  
  
// --- (DE)CONSTRUCTORS --- //


  /**
   * Constructor; initiates the stored value with zero().
   */
  
  KGuid::KGuid() {
    _value = zero();
  }
  
  /**
   * Constructor; initiates the stored value with the given argument.
   */

  KGuid::KGuid(const k_guid_t& v) {
    _value = v;
  }
  
  
// --- METHODS --- //

  /**
   * Returns the stored value.
   */

  k_guid_t KGuid::get() const {
    return _value;
  }


  /**
   * Sets the stored value to the given value.
   */
  
  void KGuid::set(const k_guid_t& v) {
    _value = v;
  }
  
  
  void KGuid::set(PPtr<KValue> other) {
    if(!other->getType()->equals(KType::GUID)) {
      throw KTypeMismatchException(getType(), other->getType());
    }
    set(other.AS(KGuid)->get());
  }
  
  
  PPtr<KType> KGuid::getType() const {
    return KType::GUID;
  }
  
  
  k_longint_t KGuid::getTotalSizeInOctets() const {
    return K_GUID_SIZE;
  }

  
  void KGuid::readFromBinaryStream(PPtr<InputStream> input) {
    k_guid_t v;
    
    if(input->read((kf_octet_t*)&v.appId, 8) < 8) {
      throw IOException("Not enough data to read");
    }
    
    k_octet_t bytes[8];
    
    if(input->read(bytes, 8) < 8) {
      throw IOException("Not enough data to read");
    }
    
    if(System::isBigEndian()) {
      k_octet_t tmp = bytes[0];
      bytes[0] = bytes[1];
      bytes[1] = tmp;
      
      tmp = bytes[2];
      bytes[2] = bytes[3];
      bytes[3] = tmp;
      
      tmp = bytes[4];
      bytes[4] = bytes[7];
      bytes[7] = bytes[5];
      bytes[5] = bytes[6];
      bytes[6] = bytes[7];
      bytes[7] = tmp;
    }
    
    v.nodeRank = *(kf_int16_t*)bytes;
    v.key = *(kf_int16_t*)(bytes + 2);
    v.lid = *(k_integer_t*)(bytes + 4);
    
    set(v);
  }
  
  
  void KGuid::writeToBinaryStream(PPtr<OutputStream> output) const {
    k_guid_t v = get();
    
    output->write((kf_octet_t*)&v.appId, 8);
    
    k_octet_t bytes[8];
    *(kf_int16_t*)bytes = v.nodeRank;
    *(kf_int16_t*)(bytes + 2) = v.key;
    *(k_integer_t*)(bytes + 4) = v.lid;
    
    if(System::isBigEndian()) {
      k_octet_t tmp = bytes[0];
      bytes[0] = bytes[1];
      bytes[1] = tmp;
      
      tmp = bytes[2];
      bytes[2] = bytes[3];
      bytes[3] = tmp;
      
      tmp = bytes[4];
      bytes[4] = bytes[7];
      bytes[7] = bytes[5];
      bytes[5] = bytes[6];
      bytes[6] = bytes[7];
      bytes[7] = tmp;
    }
    
    output->write(bytes, 8);
  }
  
  
  void KGuid::deserialize(PPtr<ObjectToken> headToken) {
    headToken->validateClass("KGUID");
    
    Ptr<Token> token = headToken->next();
    token->validateType(AttributeToken::TYPE);
    
    PPtr<AttributeToken> attrib = token.AS(AttributeToken);
    
    if(attrib->checkName("alias")) {
      throw KFException("not implemented");
    } else if(attrib->checkName("value")) {
      throw ParseException("Deserialization of KGUID from value is not supported.");
    }
    
    token = token->next();
    token->validateType(EndObjectToken::TYPE);
  }
  
  
  void KGuid::serialize(PPtr<ObjectSerializer> builder) const {
    builder->object("KGUID")
           ->attribute("value", KGuid::toString(get()))
           ->endObject();
  }
  
  
  void KGuid::printToStream(ostream &os) const {
    os << toString(get());
  }


  /**
   * KFoundation logger support for `k_guid_t`.
   */
  
  Logger::Stream& operator<<(Logger::Stream& stream, const k_guid_t& guid) {
    stream << KGuid::toString(guid);
    return stream;
  }


  /**
   * C++ output stream support for `k_guid_t`.
   */

  ostream& operator<<(ostream& os, const k_guid_t& guid) {
    os << KGuid::toString(guid);
    return os;
  }
  

  /**
   * Equality operator for `k_guid_t` type.
   */

  bool operator==(const k_guid_t& a, const k_guid_t& b) {
    return memcmp(&a, &b, sizeof(k_guid_t)) == 0;
  }

} // type
} // knorba

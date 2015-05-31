//
//  KGUID.cpp
//  CellMonitorTest-XCodeWrapper
//
//  Created by Hamed KHANDAN on 7/18/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

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
#include "KGlobalUid.h"

#define K_GUID_SIZE 16

namespace knorba {
namespace type {

  using namespace std;  
  using namespace kfoundation;
  
  
//\/ KGlobalUid /\/////////////////////////////////////////////////////////////
  
// --- STATIC FIELDS --- //
  
  k_guid_t KGlobalUid::ZERO;
  bool KGlobalUid::IS_ZERO_INITIALIZED = false;
  
  
// --- STATIC METHODS --- //
  
  const k_guid_t& KGlobalUid::zero() {
    if(IS_ZERO_INITIALIZED) {
      return ZERO;
    }
    assert(sizeof(k_guid_t) == K_GUID_SIZE);
    memset(&ZERO, 0, K_GUID_SIZE);
    return ZERO;
  }
  
  
  void KGlobalUid::randomizeAppId(k_guid_t& target) {
    for(int i = 0; i < 8; i++) {
      target.appId[i] = rand()%255;
    }
  }
  
  
  void KGlobalUid::randomizeKey(k_guid_t& target) {
    target.key = rand()%16711670 - 8355830;
  }
  
  
  bool KGlobalUid::areOnTheSameApp(const k_guid_t &first, const k_guid_t &second) {
    return memcmp(&first.appId, &second.appId, sizeof(k_appid_t)) == 0;
  }
  
  
  bool KGlobalUid::areOnTheSameNode(const k_guid_t& first, const k_guid_t& second) {
    return areOnTheSameApp(first, second) && first.nodeRank == second.nodeRank;
  }
  
  
  char* printBytes(char* buffer, const k_octet_t* bytes, const int count) {
    for(int i = 0; i < count; i++) {
      buffer += sprintf(buffer, "%02X", bytes[i]);
    }
    return buffer;
  }
  
  
  string KGlobalUid::toString(const k_guid_t& value) {
    char buffer[50];
    char* p = buffer;
    
    p = printBytes(p, value.appId, 8);
    sprintf(p, ":%04X:%08X", value.nodeRank, value.lid);
    
    return string(buffer);
  }
  
  
  string KGlobalUid::toShortString(const k_guid_t& value) {
    char buffer[20];
    sprintf(buffer, "%04X:%08X", value.nodeRank, value.lid);
    return string(buffer);
  }
  
  
  string KGlobalUid::appIdToString(const k_guid_t& value) {
    char buffer[20];
    char* p = buffer;
    p = printBytes(p, value.appId, 8);
    return string(buffer);
  }
  
  
// --- (DE)CONSTRUCTORS --- //
  
  KGlobalUid::KGlobalUid() {
    _value = zero();
  }
  
  
  KGlobalUid::KGlobalUid(const k_guid_t& v) {
    _value = v;
  }
  
  
// --- METHODS --- //
  
  k_guid_t KGlobalUid::get() const {
    return _value;
  }
  
  
  void KGlobalUid::set(const k_guid_t& v) {
    _value = v;
  }
  
  
  void KGlobalUid::set(PPtr<KValue> other) {
    if(!other->getType()->equals(KType::GUID)) {
      throw KTypeMismatchException(getType(), other->getType());
    }
    set(other.AS(KGlobalUid)->get());
  }
  
  
  PPtr<KType> KGlobalUid::getType() const {
    return KType::GUID;
  }
  
  
  k_longint_t KGlobalUid::getTotalSizeInOctets() const {
    return K_GUID_SIZE;
  }

  
  void KGlobalUid::readFromBinaryStream(PPtr<InputStream> input) {
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
  
  
  void KGlobalUid::writeToBinaryStream(PPtr<OutputStream> output) const {
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
  
  
  void KGlobalUid::readFromObjectStream(PPtr<ObjectToken> headToken) {
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
  
  
  void KGlobalUid::serialize(PPtr<ObjectSerializer> builder) const {
    builder->object("KGUID")
           ->attribute("value", KGlobalUid::toString(get()))
           ->endObject();
  }
  
  
  void KGlobalUid::printToStream(ostream &os) const {
    os << toString(get());
  }
  
  
  Logger::Stream& operator<<(Logger::Stream& stream, const k_guid_t& guid) {
    stream << KGlobalUid::toString(guid);
    return stream;
  }
  
  
  ostream& operator<<(ostream& os, const k_guid_t& guid) {
    os << KGlobalUid::toString(guid);
    return os;
  }
  
  
  bool operator==(const k_guid_t& a, const k_guid_t& b) {
    return memcmp(&a, &b, sizeof(k_guid_t)) == 0;
  }

} // type
} // knorba

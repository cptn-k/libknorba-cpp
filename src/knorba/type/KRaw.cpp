//
//  KRaw.cpp
//  CellMonitorTest-XCodeWrapper
//
//  Created by Hamed KHANDAN on 7/18/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

// Std
#include <cmath>

// KFoundation
#include <kfoundation/IOException.h>
#include <kfoundation/InputStream.h>
#include <kfoundation/OutputStream.h>
#include <kfoundation/ObjectStreamReader.h>
#include <kfoundation/BufferInputStream.h>
#include <kfoundation/Path.h>

// Internal
#include "KType.h"
#include "KLongint.h"
#include "KTypeMismatchException.h"

// Self
#include "KRaw.h"

#define K_RAW_HEADER_SIZE 8
#define K_RAW_READ_BUFFER_SIZE ((kf_int32_t)1024)

namespace knorba {
namespace type {
  
  using namespace std;
  
//\/ KRaw /\///////////////////////////////////////////////////////////////////
    
// --- (DE)CONSTRUCTORS --- //
  
  KRaw::KRaw() {
    _buffer = NULL;
  }
  
  
  KRaw::~KRaw() {
    if(NOT_NULL(_buffer)) {
      delete[] _buffer;
    }
  }
  

  void KRaw::reallocateBuffer(const k_longint_t size) {
    k_octet_t* buffer = new k_octet_t[size + K_RAW_HEADER_SIZE];
    *(k_longint_t*)buffer = size;
    setBuffer(buffer);
  }
  
  
  void KRaw::set(const k_octet_t* data, const k_longint_t size) {
    reallocateBuffer(size);
    memcpy(getBuffer() + K_RAW_HEADER_SIZE, data, size);
  }
  
  
  const k_octet_t* KRaw::getData() const {
    k_octet_t* b = getBuffer();
    
    if(IS_NULL(b)) {
      return NULL;
    }
    
    return b + K_RAW_HEADER_SIZE;
  }

  
  k_longint_t KRaw::getNOctets() const {
    k_octet_t* b = getBuffer();
        
    if(IS_NULL(b)) {
      return 0;
    }
    
    return *(k_longint_t*)b;
  }
  
  
  void KRaw::writeDataToFile(PPtr<Path> path) {
    ofstream ofs(path->getString().c_str(),
                 ios_base::out | ios_base::binary | ios_base::trunc);
    
    if(getData() != NULL) {
      ofs.write((char*)getData(), getNOctets());
    }
    
    ofs.close();
  }
  
  
  Ptr<BufferInputStream> KRaw::getDataAsInputStream() const {
    return new BufferInputStream(getData(), (kf_int32_t)getNOctets(), false);
  }
  
  
  void KRaw::set(PPtr<KValue> other) {
    if(!other->getType()->equals(KType::RAW)) {
      throw KTypeMismatchException(getType(), other->getType());
    }
    
    set(other.AS(KRaw)->getData(), other.AS(KRaw)->getNOctets());
  }
  
  
  PPtr<KType> KRaw::getType() const {
    return KType::RAW;
  }
  
  
  k_longint_t KRaw::getTotalSizeInOctets() const {
    return K_RAW_HEADER_SIZE + getNOctets();
  }
  
  
  void KRaw::readDataFromFile(PPtr<Path> path) {
    ifstream ifs(path->getString().c_str(), ios_base::in | ios_base::binary);
    ifs.seekg(0, ios_base::end);
    
    k_longint_t size = ifs.tellg();
    reallocateBuffer(size);
    
    ifs.seekg(0);
    ifs.read((char*)getData(), size);
    ifs.close();
    
    if(size == -1) {
      size = 0;
    }
  }

  
  void KRaw::readFromBinaryStream(PPtr<InputStream> input) {
    Ptr<KLongint> size = new KLongint();
    size->readFromBinaryStream(input);
    k_longint_t nOctets = size->get();
    
    reallocateBuffer(nOctets);
    
    k_longint_t readOctets = input->read(
        getBuffer() + K_RAW_HEADER_SIZE, (kf_int32_t)nOctets);
    
    if(readOctets != nOctets) {
      throw IOException("Mismatch number of octets read. Read: "
                        + LongInt(readOctets) + ", Expected: "
                        + LongInt(nOctets));
    }
  }
  
  
  void KRaw::writeToBinaryStream(PPtr<OutputStream> output) const {
    k_longint_t nOctets = getNOctets();
    
    Ptr<KLongint> size = new KLongint(nOctets);
    size->writeToBinaryStream(output);
    
    if(nOctets > 0) {
      output->write(getData(), (kf_int32_t)nOctets);
    }
  }
  
  
  void KRaw::readFromObjectStream(PPtr<ObjectToken> headToken) {
    throw KFException("Operation not supported");
  }
  
  
  void KRaw::serialize(PPtr<ObjectSerializer> builder) const {
    builder->object("KRaw")
      ->attribute("size", getNOctets())
      ->endObject();
  }

  
} // namespace type
} // namespace knorba

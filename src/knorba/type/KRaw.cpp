/*---[KRaw.cpp]------------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : -
 |  Implements: knorba::type::KRaw::*
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
#include <cmath>
#include <cstring>

// KFoundation
#include <kfoundation/Ref.h>
#include <kfoundation/IOException.h>
#include <kfoundation/InputStream.h>
#include <kfoundation/OutputStream.h>
#include <kfoundation/ObjectStreamReader.h>
#include <kfoundation/BufferInputStream.h>
#include <kfoundation/ObjectSerializer.h>

// Internal
#include "KType.h"
#include "KLongint.h"
#include "KTypeMismatchException.h"

// Self
#include "KRaw.h"

namespace knorba {
namespace type {

//\/ KRawOutputStream /\///////////////////////////////////////////////////////

  class KRawOutputStream : public BufferInputStream {
    private: RefConst<KRaw> _src;
    public: KRawOutputStream(RefConst<KRaw> src);
  };


  KRawOutputStream::KRawOutputStream(RefConst<KRaw> src)
  : BufferInputStream(src->getData(), (kf_int32_t)src->getSize(), false)
  {
    _src = src;
  }


//\/ LinkedBuffer /\///////////////////////////////////////////////////////////

  class LinkedBuffer : public KFObject {
    public: k_octet_t* buffer = NULL;
    public: Ref<LinkedBuffer> next = NULL;
    public: k_integer_t size = 0;
    public: ~LinkedBuffer();
    public: k_longint_t getTotalSize() const;
    public: k_octet_t* stitch() const;
  };


  LinkedBuffer::~LinkedBuffer() {
    if(NOT_NULL(buffer)) {
      delete[] buffer;
    }
  }


//\/ KRaw /\///////////////////////////////////////////////////////////////////

// --- STATIC FIELDS --- //

  const k_octet_t KRaw::HEADER_SIZE = /*size:*/ 8;


// --- STATIC METHODS --- //

  void KRaw::initBuffer(k_octet_t* buffer) {
    memset(buffer, 0, sizeof(header_t));
  }


  void KRaw::writeToBuffer(const k_octet_t* data, const k_longint_t size,
      k_octet_t* target)
  {
    cleanupBuffer(target);
    header_t* header = (header_t*)target;
    header->size = size;
    header->data = new k_octet_t[size];
    memcpy(header->data, data, size);
  }


  void KRaw::writeToBuffer(const k_octet_t* src, k_octet_t* target) {
    cleanupBuffer(target);
    header_t* srcHeader = (header_t*)src;
    header_t* tgtHeader = (header_t*)target;
    tgtHeader->size = srcHeader->size;
    tgtHeader->data = new k_octet_t[srcHeader->size];
    memcpy(tgtHeader->data, srcHeader->data, srcHeader->size);
  }


  void KRaw::cleanupBuffer(k_octet_t* buffer) {
    header_t* header = (header_t*)buffer;
    if(NOT_NULL(header->data)) {
      delete header->data;
    }
    initBuffer(buffer);
  }


// --- (DE)CONSTRUCTORS --- //

  /**
   * Constructor; initializes the stored value with a raw string of size 0.
   */

  KRaw::KRaw() {
    initBuffer((k_octet_t*)&_buffer);
  }


  /**
   * Deconstructor. Deletes the internally allocated buffer.
   */
  
  KRaw::~KRaw() {
    cleanupBuffer((k_octet_t*)&_buffer);
  }


// --- METHODS --- //

  k_octet_t* KRaw::getBuffer() {
    return (k_octet_t*)&_buffer;
  }


  const k_octet_t* KRaw::getBuffer() const {
    return (k_octet_t*)&_buffer;
  }
  

  /**
   * Sets the internally stored value to the given buffer.
   *
   * @param data The buffer to copy data from.
   * @param size The number of octets to copy.
   */
  
  void KRaw::set(const k_octet_t* data, const k_longint_t size) {
    writeToBuffer(data, size, getBuffer());
  }


  /**
   * Returns the number of octets of the stored data.
   */

  k_longint_t KRaw::getSize() const {
    return ((header_t*)getBuffer())->size;
  }


  /**
   * Returns pointer to the begining of the internal buffer.
   */
  
  const k_octet_t* KRaw::getData() const {
    return ((header_t*)getBuffer())->data;
  }


  Ref<InputStream> KRaw::getDataAsStream() const {
    return new KRawOutputStream(RefConst<KRaw>(this));
  }


  void KRaw::readData(Ref<InputStream> stream) {
    const kf_int32_t CHUNK_SIZE = 2^10;

    Ref<LinkedBuffer> head = new LinkedBuffer();
    Ref<LinkedBuffer> current = head;

    while(true) {
      current->buffer = new k_octet_t[CHUNK_SIZE];
      current->size = stream->read(current->buffer, CHUNK_SIZE);
      if(current->size == CHUNK_SIZE) {
        current->next = new LinkedBuffer();
        current = current->next;
      } else {
        break;
      }
    }

    k_longint_t size = 0;
    for(Ref<LinkedBuffer> b = head; !b.isNull(); b = b->next) {
      size += b->size;
    }

    k_octet_t* buffer = getBuffer();
    cleanupBuffer(buffer);
    header_t* header = (header_t*)buffer;
    k_octet_t* ptr = header->data;

    for(Ref<LinkedBuffer> b = head; !b.isNull(); b = b->next) {
      memcpy(ptr, b->buffer, b->size);
      ptr += b->size;
    }
  }


  void KRaw::writeData(Ref<OutputStream> stream) const {
    stream->write(getData(), (kf_int32_t)getSize());
  }

  
  void KRaw::set(RefConst<KValue> other) {
    if(!other->getType()->equals(KType::RAW)) {
      throw KTypeMismatchException(getType(), other->getType());
    }
    const KRaw& otherRef = *other.AS(KRaw);
    set(otherRef.getData(), otherRef.getSize());
  }
  
  
  RefConst<KType> KRaw::getType() const {
    return KType::RAW;
  }

} // namespace type
} // namespace knorba
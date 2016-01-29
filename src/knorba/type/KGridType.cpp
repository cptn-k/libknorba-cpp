/*---[KGridType.cpp]-------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : -
 |  Implements: knorba::type::KGridType::*
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

// KFoundation
#include <kfoundation/Ref.h>
#include <kfoundation/Int.h>
#include <kfoundation/UString.h>
#include <kfoundation/StringPrintWriter.h>
#include <kfoundation/RangeIterator.h>
#include <kfoundation/ObjectStreamReader.h>

// Internal
#include "../Ontology.h"
#include "KGrid.h"
#include "KRecordType.h"
#include "KInteger.h"

// Self
#include "KGridType.h"

namespace knorba {
namespace type {
  
  using namespace kfoundation;
  
// --- (DE)CONSTRUCTORS --- //

  /**
   * Constructs a representation of a custom KnoRBA grid type with simple cell
   * type. This method will construct an instance of KRecordType internally,
   * with only one field who's type is the given argument.
   *
   * @param recordType The type of the sole record member of each cell.
   * @param nDimensions Number of grid dimensions.
   */

  KGridType::KGridType(RefConst<KRecordType> recordType, k_octet_t nDimension)
  : KType(K"grid(" + nDimension + ") OF " + recordType->getTypeName())
  {
    _elementType = recordType;
    _nDimensions = nDimension;
  }
  

// --- METHODS --- //

  /**
   * Returns the type of grid cells.
   */

  RefConst<KRecordType> KGridType::getRecordType() const {
    return _elementType;
  }


  /**
   * Returns the number of dimensions.
   */
  
  k_octet_t KGridType::getNDimensions() const {
    return _nDimensions;
  }


  void KGridType::deleteData(k_octet_t* origin, const k_longint_t begin,
      const k_longint_t count) const
  {
    const KRecordType& elementType = *_elementType;
    const k_integer_t elementSize = elementType.getSizeInOctets();
    origin += elementSize * elementSize;
    for(k_longint_t i = count - 1; i >= 0; i--) {
      elementType.cleanupBuffer(origin);
      origin += elementSize;
    }
  }


  void KGridType::setSize(k_octet_t* grid, const Tuple& size, bool copy) const
  {
    KGrid::window_header_t* header = (KGrid::window_header_t*)grid;
    KGrid::shared_header_t* sharedHeader = header->sharedHeader;
    if(IS_NULL(sharedHeader)) {
      header->sharedHeader = new KGrid::shared_header_t;
      sharedHeader = header->sharedHeader;
      sharedHeader->dimensions = new k_integer_t[_nDimensions];
      sharedHeader->nDimensions = _nDimensions;
      sharedHeader->nWindows = 1;
      sharedHeader->data = NULL;
    }
    if(IS_NULL(header->windowBegin)) {
      header->windowBegin = new k_integer_t[_nDimensions];
    }
    if(IS_NULL(header->windowEnd)) {
      header->windowEnd = new k_integer_t[_nDimensions];
    }
    
    for(int i = size.getSize() - 1; i >= 0; i--) {
      sharedHeader->dimensions[i] = size.at(i);
      header->windowBegin[i] = 0;
      header->windowEnd[i] = size.at(i);
    }

    k_octet_t* oldData = sharedHeader->data;
    k_longint_t oldNElements = KGrid::headerGetNElements(header);

    k_integer_t elementSize = _elementType->getSizeInOctets();
    k_longint_t nElements = size.productAll() * elementSize;
    
    if(nElements == 0) {
      sharedHeader->data = NULL;
    } else {
      sharedHeader->data = new k_octet_t[nElements * elementSize];
    }

    if(NOT_NULL(oldData)) {
      if(copy) {
        k_longint_t nCopy = (oldNElements > nElements)?nElements:oldNElements;
        memcpy(sharedHeader->data, oldData, nCopy * elementSize);
        if(oldNElements > nElements) {
          deleteData(oldData, nCopy, oldNElements);
        }
      } else {
        deleteData(oldData, 0, oldNElements);
      }
      delete[] oldData;
    }
  }
  
  
  void KGridType::makeWindow(const k_octet_t* origin, k_octet_t* target,
      const Range& window) const
  {
    const KGrid::window_header_t* originHeader
        = (KGrid::window_header_t*)origin;

    KGrid::window_header_t* targetHeader = (KGrid::window_header_t*)target;

    *targetHeader = *originHeader;
    
    KGrid::headerSetWindow(targetHeader, window);
    targetHeader->sharedHeader->nWindows++;
  }


  bool KGridType::isCastableTo(RefConst<KType> t) const {
    if(!t.ISA(KGridType)) {
      return false;
    }
    
    RefConst<KGridType> other = t.AS(KGridType);
    
    if(_nDimensions != other->_nDimensions) {
      return false;
    }
    
    return _elementType->isCastableTo(other->_elementType.AS(KType));
  }
  
  
  bool KGridType::isAutomaticCastableTo(RefConst<KType> t) const {
    if(!t.ISA(KGridType)) {
      return false;
    }
    
    RefConst<KGridType> other = t.AS(KGridType);
    
    if(_nDimensions != other->_nDimensions) {
      return false;
    }
    
    return _elementType->isAutomaticCastableTo(other->_elementType.AS(KType));
  }


  /**
   * Checks if type represented by this object is equivalant to the type
   * represented by the given argument.
   * Returns `true` iff (1) `t` is an instance of KGridType,
   * (2) record type of the grid type represented by this object is equivalant
   * of the record type of the given grid type, and (3) the number of
   * dimensions of this object equals to that of the given object.
   */

  bool KGridType::equals(RefConst<KType> t) const {
    if(!t.ISA(KGridType)) {
      return false;
    }
    
    RefConst<KGridType> other = t.AS(KGridType);
    
    return _elementType->equals(other->_elementType.AS(KType))
        && _nDimensions == other->_nDimensions;
  }
  
  
  k_integer_t KGridType::getSizeInOctets() const {
    return sizeof(KGrid::window_header_t);
  }
  
  
  bool KGridType::isPrimitive() const {
    return false;
  }
  
  
  bool KGridType::hasConstantSize() const {
    return false;
  }
  
  
  Ref<KValue> KGridType::instantiate() const {
    return new KGrid(this);
  }
  
  
  RefConst<UString> KGridType::toKnois() const {
    StringPrintWriter pw;
    pw << "grid<" << _nDimensions << ") OF " << _elementType->getTypeName();
    return pw.toString();
  }


  k_longint_t KGridType::getStreamSizeInOctets(const k_octet_t* buffer) const {
    KGrid::window_header_t* header = (KGrid::window_header_t*)buffer;
    Range window = KGrid::headerGetWindow(header);

    const KRecordType& elementType = *_elementType;
    const k_integer_t elementSize = elementType.getSizeInOctets();

    k_longint_t n = 1 + KInteger::SIZE_IN_OCTETS * window.getNDimensions();

    if(elementType.hasDynamicFields()) {
      k_octet_t* base = header->sharedHeader->data;

      for(RangeIterator i(window); i.hasMore(); i.next()) {
        const k_octet_t* record = base +
          KGrid::headerGetOrdinalForIndex(header, i) * elementSize;
        n += elementType.KType::getStreamSizeInOctets(record);
      }
      return n;
    }

    return n + window.getVolume() * elementSize;
  }
  
  
  void KGridType::writeBufferToStream(const k_octet_t* buffer,
      Ref<OutputStream> stream) const
  {
    const KGrid::window_header_t* header = (const KGrid::window_header_t*)buffer;
    
    Range window = KGrid::headerGetWindow(header);
    k_octet_t nDims = window.getNDimensions();
    
    stream->write(nDims);

    for(int i = 0; i < nDims; i++) {
      k_integer_t d = window.getSize().at(i);
      KType::INTEGER->writeBufferToStream((k_octet_t*)&d, stream);
    }
    
    if(IS_NULL(header->sharedHeader)) {
      return;
    }

    k_octet_t* origin = header->sharedHeader->data;
    const KRecordType& elementType = *_elementType;
    k_integer_t elementSize = elementType.getSizeInOctets();
    
    for(RangeIterator i(window); i.hasMore(); i.next()) {
      k_octet_t* record = origin + KGrid::headerGetOrdinalForIndex(header, i)
          * elementSize;
      elementType.writeBufferToStream(record, stream);
    }
  }
  
  
  void KGridType::writeStreamToBuffer(Ref<InputStream> stream,
      RefConst<Ontology> ontology, k_octet_t* buffer) const
  {
    KGrid::window_header_t* header = (KGrid::window_header_t*)buffer;
    
    k_octet_t nDims;
    KType::OCTET->writeStreamToBuffer(stream, NULL, &nDims);
    
    Tuple size(nDims);
    for(int i = 0; i < nDims; i++) {
      k_integer_t d;
      KType::INTEGER->writeStreamToBuffer(stream, NULL, (k_octet_t*)&d);
      size.at(i) = d;
    }
    
    setSize(buffer, size, false);
    
    k_octet_t* origin = header->sharedHeader->data;
    const KRecordType& elementType = *_elementType;
    k_integer_t elementSize = elementType.getSizeInOctets();
    
    for(RangeIterator i(size); i.hasMore(); i.next()) {
      k_octet_t* record = origin + KGrid::headerGetOrdinalForIndex(header, i)
        * elementSize;
      elementType.writeStreamToBuffer(stream, ontology, record);
    }
  }
  
  
  void KGridType::serializeBuffer(const k_octet_t* buffer,
      Ref<ObjectSerializer> serializer) const
  {
    const KGrid::window_header_t* header = (const KGrid::window_header_t*)buffer;
    
    Range window = KGrid::headerGetWindow(header);

    serializer->object(K"KGrid")
      ->attribute(K"size", window.getSize());
    
    if(NOT_NULL(header->sharedHeader)) {
      k_octet_t* origin = header->sharedHeader->data;
      const KRecordType& elementType = *_elementType;
      k_integer_t elementSize = elementType.getSizeInOctets();
      
      for(RangeIterator i(window); i.hasMore(); i.next()) {
        k_octet_t* record = origin + KGrid::headerGetOrdinalForIndex(header, i)
            * elementSize;
        elementType.serializeBuffer(record, serializer);
      }
    }
    
    serializer->endObject();
  }

  
  struct list_t {
    k_octet_t* data;
    list_t* next;
    
    list_t() {
      data = NULL;
      next = NULL;
    }
    
    ~list_t() {
      if(NOT_NULL(data)) {
        delete[] data;
      }
      if(NOT_NULL(next)) {
        delete next;
      }
    }
  };

  
  void KGridType::deserializeIntoBuffer(Ref<ObjectToken> head,
      RefConst<Ontology> ontology, k_octet_t *buffer) const
  {
    
    head->validateClass(K"KGrid");

    Ref<Token> token = head->next();
    while(token->is(Token::ATTRIBUTE)) {
      token = token->next();
    }

    const KRecordType& elementType = *_elementType;
    const k_integer_t elementSize = elementType.getSizeInOctets();
    
    list_t* begin = NULL;
    list_t* current = NULL;
    int count = 0;

    if(token->is(Token::OBJECT)) {
      Ref<ObjectToken> obj = token.AS(ObjectToken);
      while(obj->checkClass(K"KRecord")) {
        if(IS_NULL(begin)) {
          begin = new list_t();
          current = begin;
        } else {
          current->next = new list_t();
          current = current->next;
        }
        
        count++;
        
        current->data = new k_octet_t[elementSize];
        elementType.deserializeIntoBuffer(obj, ontology, current->data);
        
        token = token->next();
        if(!token->is(Token::OBJECT)) {
          break;
        }

        obj = token.AS(ObjectToken);
      }
    }

    token->validateType(EndObjectToken::TYPE);
    
    Tuple1D size(count);
    setSize(buffer, size, false);
    
    if(count > 0) {
      k_octet_t* p = ((KGrid::window_header_t*)buffer)->sharedHeader->data;
      
      for(current = begin; NOT_NULL(current); current = current->next) {
        memcpy(p, current->data, elementSize);
        p += elementSize;
      }
      
      delete begin;
    }
  }
  
  
  void KGridType::initializeBuffer(k_octet_t* buffer) const {
    memset(buffer, 0, sizeof(KGrid::window_header_t));
  }
  
  
  void KGridType::cleanupBuffer(k_octet_t* buffer) const {
    KGrid::window_header_t* header = (KGrid::window_header_t*)buffer;
    if(NOT_NULL(header->windowBegin)) {
      delete[] header->windowBegin;
    }
    if(NOT_NULL(header->windowEnd)) {
      delete[] header->windowEnd;
    }
    
    if(NOT_NULL(header->sharedHeader)) {
      KGrid::shared_header_t* sharedHeader = header->sharedHeader;
      sharedHeader->nWindows--;
      if(sharedHeader->nWindows == 0) {
        if(NOT_NULL(sharedHeader->data)) {
          delete[] sharedHeader->data;
        }
        if(NOT_NULL(sharedHeader->dimensions)) {
          delete[] sharedHeader->dimensions;
        }
        delete sharedHeader;
      }
    }
    
    memset(buffer, 0, sizeof(KGrid::window_header_t));
  }


  void KGridType::copyBuffer(const k_octet_t* src, k_octet_t* target) const {
    cleanupBuffer(target);
    const KGrid::window_header_t* srcHeader = (const KGrid::window_header_t*)src;
    KGrid::window_header_t* tgtHeader = (KGrid::window_header_t*)target;
    setSize(target, KGrid::headerGetSize(srcHeader), false);
    k_octet_t* srcData = srcHeader->sharedHeader->data;
    k_octet_t* tgtData = tgtHeader->sharedHeader->data;
    const KRecordType& elementType = *_elementType;
    const k_longint_t elementSize = elementType.getSizeInOctets();
    const k_longint_t nElements = KGrid::headerGetNElements(srcHeader);
    for(k_longint_t i = nElements - 1; i >= 0; i--) {
      elementType.copyBuffer(srcData, tgtData);
      srcData += elementSize;
      tgtData += elementSize;
    }
  }

} // namespace type
} // namespace knorba
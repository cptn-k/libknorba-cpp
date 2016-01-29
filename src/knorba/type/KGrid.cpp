//
//  AbstractKGrid.cpp
//  KnoRBA
//
//  Created by Hamed KHANDAN on 10/15/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#include <cstring>

// KFoundation
#include <kfoundation/IOException.h>
#include <kfoundation/RangeIterator.h>
#include <kfoundation/InputStream.h>
#include <kfoundation/OutputStream.h>
#include <kfoundation/ObjectStreamReader.h>

// Internal
#include "KInteger.h"
#include "KRecordType.h"
#include "KTypeMismatchException.h"

// Self
#include "KGrid.h"

namespace knorba {
namespace type {

//\/ KGridRecord /\////////////////////////////////////////////////////////////

  KGridRecord::KGridRecord(RefConst<KRecordType> t)
  : KRecord(t)
  {
    // Nothing;
  }


//\/ KGridRecordImpl /\////////////////////////////////////////////////////////

  class KGridRecordImpl : public KGridRecord {

  // --- FIELDS --- //

    private: Ref<KGrid> _owner;
    private: KGrid::window_header_t _header;
    private: k_octet_t* _origin;
    private: k_octet_t* _buffer;
    private: k_integer_t _elementSize;
    private: k_longint_t _maxOrdinal;


  // --- (DE)CONSTRUCTORS --- //

    public: KGridRecordImpl(Ref<KGrid> owner,
        const KGrid::window_header_t& header);


  // --- METHODS --- //

    protected: k_octet_t* getBuffer();
    protected: const k_octet_t* getBuffer() const;
    public: KGridRecord& at(const Tuple& index);
    public: KGridRecord& atWindow(const Tuple& index);

  };


// --- CONSTRUCTOR --- //

  KGridRecordImpl::KGridRecordImpl(Ref<KGrid> owner,
      const KGrid::window_header_t& header)
  : KGridRecord(owner->getType().AS(KGridType)->getRecordType())
  {
    _header = header;
    _origin = header.sharedHeader->data;
    _buffer = _origin;
    
    _elementSize = owner->getType().AS(KGridType)->getRecordType()
        ->getSizeInOctets();
    
    _maxOrdinal = KGrid::headerGetNElements(&header) - 1;
  }


// --- METHODS --- //

  k_octet_t* KGridRecordImpl::getBuffer() {
    return _buffer;
  }


  const k_octet_t* KGridRecordImpl::getBuffer() const {
    return _buffer;
  }


  KGridRecord& KGridRecordImpl::at(const Tuple& index) {
    k_longint_t ordinal = KGrid::headerGetOrdinalForIndex(&_header, index);
    if(ordinal > _maxOrdinal || ordinal < 0) {
      throw IndexOutOfBoundException(K"Index " + index + " does not point "
          "to a valid location in a grid of size "
          + KGrid::headerGetSize(&_header));
    }

    _buffer = _origin + ordinal * _elementSize;
    return *this;
  }


  KGridRecord& KGridRecordImpl::atWindow(const Tuple& index) {
    Tuple globalIndex = KGrid::headerMapWindowToGlobal(&_header, index);
    return at(globalIndex);
  }


//\/ KGrid /\//////////////////////////////////////////////////////////////////

// --- STATIC METHODS --- //

  void KGrid::headerSetWindow(window_header_t* header, const Range& window) {
    k_octet_t nDims = header->sharedHeader->nDimensions;
    if(nDims != window.getNDimensions()) {
      throw IndexOutOfBoundException(K"Window dimensions does not match grid's."
          " Given: " + window.getNDimensions() + ", Expected: " + nDims);
    }

    Range global = Range(headerGetSize(header));
    if(!global.contains(window)) {
      throw IndexOutOfBoundException(K"Cannot create a window of dimensions "
          + window + " over a grid of size " + global.getBegin());
    }

    const Tuple& begin = window.getBegin();
    const Tuple& end = window.getEnd();

    for(int i = nDims - 1; i >= 0; i--) {
      header->windowBegin[i] = begin.at(i);
      header->windowEnd[i] = end.at(i);
    }
  }


  k_octet_t KGrid::headerGetRetainCount(const window_header_t* header) {
    return header->sharedHeader->nWindows;
  }


  Tuple KGrid::headerGetSize(const window_header_t* header) {
    shared_header_t* sharedHeader = header->sharedHeader;
    Tuple t(sharedHeader->nDimensions);
    for(int i = sharedHeader->nDimensions - 1; i >= 0; i--) {
      t.at(i) = sharedHeader->dimensions[i];
    }
    return t;
  }


  Range KGrid::headerGetWindow(const window_header_t* header) {
    k_octet_t nDims = header->sharedHeader->nDimensions;
    Tuple begin(nDims);
    Tuple end(nDims);
    for(int i = nDims - 1; i >= 0; i--) {
      begin.at(i) = header->windowBegin[i];
      end.at(i) = header->windowEnd[i];
    }
    return Range(begin, end);
  }


  k_longint_t KGrid::headerGetOrdinalForIndex(const window_header_t* header,
      const Tuple& index)
  {
    if(IS_NULL(header->sharedHeader)) {
      return -1;
    }

    k_octet_t s = header->sharedHeader->nDimensions;
    k_integer_t* size = header->sharedHeader->dimensions;

    k_longint_t ordinal = 0;
    k_longint_t p = 1;

    for(int i = 0; i < s; i++) {
      ordinal += index.at(i) * p;
      p = p * size[i];
    }

    return ordinal;
  }


// --- (DE)CONSTRUCTORS --- //
    
  KGrid::KGrid(RefConst<KGridType> type) {
    _type = type;
    _type->initializeBuffer((kf_octet_t*)&_header);
  }


  KGrid::KGrid(RefConst<KGridType> type, const Tuple& dimensions) {
    _type = type;
    _type->initializeBuffer((kf_octet_t*)&_header);
    _type->setSize((k_octet_t*)&_header, dimensions, false);
  }


  KGrid::~KGrid() {
    _type->cleanupBuffer(getBuffer());
  }


// --- METHODS --- //

  k_octet_t* KGrid::getBuffer() {
    return (k_octet_t*)&_header;
  }


  const k_octet_t* KGrid::getBuffer() const {
    return (k_octet_t*)&_header;
  }


  Tuple KGrid::getSize() const {
    return headerGetSize(getHeader());
  }


  Range KGrid::getWindow() const {
    return headerGetWindow(getHeader());
  }


  void KGrid::grow(const Tuple& size) {
    _type->setSize(getBuffer(), size, true);
  }


  void KGrid::reset() {
    _type->setSize(getBuffer(), Tuple::zero(_type->getNDimensions()), false);
  }


  void KGrid::reset(const Tuple& dimensions) {
    _type->setSize(getBuffer(), dimensions, false);
  }


  void KGrid::reset(RefConst<KGrid> origin, const Range& window) {
    _type->makeWindow(origin->getBuffer(), getBuffer(), window);
  }


  void KGrid::moveWindow(const Range& window) {
    headerSetWindow(getHeader(), window);
  }


  Ref<KGridRecord> KGrid::makeRecord() const {
    return new KGridRecordImpl(this, *getHeader());
  }


  Ref<KGrid> KGrid::makeWindow(const Range& range) const {
    KGrid* grid = new KGrid(_type);
    grid->reset(this, range);
    return grid;
  }


  /**
   * Copies the values of the given range of cells from the given offset of
   * another grid to the given offset of this grid.
   *
   * @param src The grid to copy values from.
   * @param srcOffset Source offset.
   * @param dstOffset Destination offset.
   * @param size Size of the range of values to copy.
   */
  
  void KGrid::copyFrom(RefConst<KGrid> src, const Tuple& srcOffset,
    const Tuple& dstOffset, const Tuple& size)
  {
    Ref<KGridRecord> srcRecordRef = src->makeRecord();
    Ref<KGridRecord> dstRecordRef = makeRecord();

    KGridRecord& srcRecord = *srcRecordRef;
    KGridRecord& dstRecord = *dstRecordRef;
    
    Tuple translation = dstOffset - srcOffset;

    for(RangeIterator i(srcOffset, srcOffset + size); i.hasMore(); i.next()) {
      Tuple j = i + translation;
      dstRecord.at(j).set(srcRecord.at(i));
    }
  }


  void KGrid::set(RefConst<KValue> other) {
    if(!other->getType()->equals(getType())) {
      throw KTypeMismatchException(getType(), other->getType());
    }

    RefConst<KGrid> src = other.AS(KGrid);
    Range srcRange = src->getWindow();

    reset(srcRange.getSize());

    Ref<KGridRecord> srcRecord = src->makeRecord();
    Ref<KGridRecord> dstRecord = makeRecord();

    KGridRecord& srcRef = *srcRecord;
    KGridRecord& dstRef = *dstRecord;

    if(srcRange.getBegin() == Tuple::zero(srcRange.getNDimensions())) {
      for(RangeIterator i(getWindow()); i.hasMore(); i.next()) {
        dstRef.at(i).set(srcRef.at(i));
      }
    } else {
      Tuple offset = srcRange.getBegin();
      for(RangeIterator i(srcRange.getSize()); i.hasMore(); i.next()) {
        Tuple j = i + offset;
        dstRef.at(i).set(srcRef.at(j));
      }
    }
  }


  RefConst<KType> KGrid::getType() const {
    return _type.AS(KType);
  }
  
  
//\/ KGridVector /\////////////////////////////////////////////////////////////
    
#define VEC_INITIAL_CAPACITY 16
#define VEC_GROWTH_RATE 2
    
    
// --- (DE)CONSTRUCTORS --- //
    
  KGridVector::KGridVector(Ref<KRecordType> elementType)
  : KGrid(new KGridType(elementType, 1))
  {
    grow();
  }

    
// --- METHODS --- //
    
  void KGridVector::grow() {
    k_integer_t capacity = getSize().at(0);
    if(capacity == 0) {
      capacity = VEC_INITIAL_CAPACITY;
    } else {
      capacity = capacity * VEC_GROWTH_RATE;
    }
    KGrid::grow(Tuple1D(capacity));
  }
  
  
  Tuple KGridVector::basicAdd() {
    Range window = getWindow();
    const Tuple& size = window.getEnd();
    Tuple capacity = getSize();

    if(size == capacity) {
      grow();
    }

    moveWindow(Range(window.getBegin(), size + 1));
    return size;
  }


  void KGridVector::basicInsert(k_integer_t index) {
    Range window = getWindow();
    const Tuple& size = window.getEnd();
    Tuple capacity = getSize();

    if(index >= size.at(0)) {
      throw IndexOutOfBoundException(K"Cannot insert at index " + index
          + " in an vector of size " + size.at(0));
    }

    if(size == capacity) {
      grow();
    }

    moveWindow(Range(window.getBegin(), size + 1));

    Ref<KGridRecord> src = makeRecord();
    Ref<KGridRecord> dst = makeRecord();
    
    if(index < size.at(0) - 1) {
      for(RangeIterator i(Tuple1D(index), size - 1); i.hasMore(); i.next()) {
        dst->at(i + 1).set(src->at(i));
      }
    }
    
    src->at(Tuple1D(index)).reset();
  }


  /**
   * Adds a new record to the top of this vector, and slides the given wrapper
   * record on it. Usage:
   *
   *      vector->add(record)->setInteger(128);
   *
   * @return Same pointer as given arugment.
   */

  Ref<KGridRecord> KGridVector::add(Ref<KGridRecord> wrapper) {
    wrapper->at(basicAdd());
    return wrapper;
  }


  Ref<KGridRecord> KGridVector::add() {
    return add(makeRecord());
  }


  /**
   * Adds a new record at the given index and slides the given wrapper record
   * on it. Usage:
   * 
   *     vector->insert(record, 4)->setInteger(128);
   *
   * @return Same pointer as the first argument.
   */
  
  Ref<KGridRecord> KGridVector::insert(Ref<KGridRecord> wrapper,
      const k_integer_t index)
  {
    basicInsert(index);
    wrapper->at(Tuple1D(index));
    return wrapper;
  }


  Ref<KGridRecord> KGridVector::insert(const k_integer_t index) {
    return insert(makeRecord(), index);
  }


  /**
   * Removes the record at the given index, shifting all records at higher
   * indexes downwards.
   */

  void KGridVector::remove(const k_integer_t index) {
    Range window = getWindow();
    const Tuple& size = window.getEnd();

    if(index >= size.at(0)) {
      throw IndexOutOfBoundException(K"Cannot remove element "
          + index + " from a vector of size " + size.at(0));
    }

    Ref<KGridRecord> src = makeRecord();
    Ref<KGridRecord> dst = makeRecord();

    for(RangeIterator i(Tuple1D(index), size - 2); i.hasMore(); i.next()) {
      src->at(i).set(dst->at(i + 1));
    }

    src->at(Tuple1D(index)).reset();

    moveWindow(Range(window.getBegin(), size - 1));
  }
  

  /**
   * Removes the last record.
   */

  void KGridVector::removeLast() {
    remove(getWindow().getEnd().at(0) - 1);
  }
  

  /**
   * Slides the given wrapper record on the last item in the vector.
   */

  Ref<KGridRecord> KGridVector::last(Ref<KGridRecord> wrapper) const {
    wrapper->at(getWindow().getEnd() - 1);
    return wrapper;
  }


} // namespace type
} // namespace knorba

#undef VEC_INITIAL_CAPACITY
#undef VEC_GROWTH_RATE
//
//  AbstractKGrid.cpp
//  KnoRBA
//
//  Created by Hamed KHANDAN on 10/15/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

// Std
#include <cassert>

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

#define K_GRID_H1 1
#define K_GRID_H2 4
#define K_GRID_H3 8

namespace knorba {
namespace type {
    
//\/ KGrid /\//////////////////////////////////////////////////////////////////
    
// --- (DE)CONSTRUCTORS --- //
    
  KGrid::KGrid(PPtr<KGridType> type)
  : _elementSize(type->getRecordType()->getSizeInOctets())
  {
    _type = type;
  }
    
    
// --- METHODS --- //
    
  void KGrid::cleanupDynamicFields() {
    if(getType().AS(KGridType)->getRecordType()->hasDynamicFields()) {
      KRecord r(getPtr().AS(KGrid));
      for(RangeIterator it(getRange()); it.hasMore(); it.next()) {
        at(it, r).cleanupDynamicFields();
      }
    }
  }
  
  
  PPtr<KRecord> KGrid::at(const Tuple& index, PPtr<KRecord> wrapper) const
  throw(IndexOutOfBoundException)
  {
    wrapper->setOffset(getOffsetForIndex(index));
    return wrapper;
  }
  
  
  KRecord& KGrid::at(const Tuple& index, KRecord &wrapper) const
  throw(IndexOutOfBoundException)
  {
    wrapper.setOffset(getOffsetForIndex(index));
    return wrapper;
  }
  
  
  void KGrid::copyFrom(PPtr<KGrid> src, const Tuple& srcOffset,
    const Tuple& dstOffset, const Tuple& size)
  {
    Ptr<KRecord> srcRecord = new KRecord(src);
    Ptr<KRecord> dstRecord = new KRecord(getPtr().AS(KGrid));
    
    Tuple translation = dstOffset - srcOffset;
    
    for(RangeIterator i(srcOffset, srcOffset + size); i.hasMore(); i.next()) {
      Tuple j = i + translation;
      at(j, dstRecord)->set(src->at(i, srcRecord).AS(KValue));
    }
  }
  
  
  void KGrid::set(PPtr<KValue> other) {
    if(!other->getType()->equals(_type.AS(KType))) {
      throw KTypeMismatchException(_type.AS(KType), other->getType());
    }
    
    PPtr<KGrid> otherGrid = other.AS(KGrid);
    resetWithSize(otherGrid->getRange().getSize());
    
    Ptr<KRecord> srcRecord = new KRecord(otherGrid);
    Ptr<KRecord> dstRecord = new KRecord(getPtr().AS(KGrid));
    
    for(RangeIterator i(getRange()); i.hasMore(); i.next()) {
      at(i, dstRecord)->set(otherGrid->at(i, srcRecord).AS(KValue));
    }
  }
  
  
  PPtr<KType> KGrid::getType() const {
    return _type.AS(KType);
  }
  
  
  k_longint_t KGrid::getTotalSizeInOctets() const {
    const Tuple& s = getRange().getSize();
    PPtr<KRecordType> t = _type->getRecordType();
    
    k_longint_t n = KType::OCTET->getSizeInOctets()
                  + KType::INTEGER->getSizeInOctets() * s.getSize();
    
    if(t->hasDynamicFields()) {
      KRecord wrapper(getPtr().AS(KGrid));
      for(RangeIterator it(getRange()); it.hasMore(); it.next()) {
        n += at(it, wrapper).getTotalSizeInOctets();
      }
      return n;
    } else {
      n += s.productAll() * _elementSize;
    }
    
    return n;
  }
  
  
  void KGrid::readFromBinaryStream(PPtr<InputStream> input) {
    int nDims = input->read();
    
    if(nDims == -1) {
      throw IOException("Not enough data to read");
    }
    
    Tuple dims(nDims);
    
    Ptr<KInteger> size = new KInteger();
    for(int i = 0; i < nDims; i++) {
      size->readFromBinaryStream(input);
      dims.at(i) = size->get();
    }
    
    resetWithSize(dims);
    
    KRecord record(getPtr().AS(KGrid));
    
    for(RangeIterator it(getRange()); it.hasMore(); it.next()) {
      at(it, record).readFromBinaryStream(input);
    }
  }
  
  
  void KGrid::writeToBinaryStream(PPtr<OutputStream> output) const {
    const Tuple& dims = getRange().getSize();
    output->write(dims.getSize());
    
    Ptr<KInteger> size = new KInteger();
    for(int i = 0; i < dims.getSize(); i++) {
      size->set(dims.at(i));
      size->writeToBinaryStream(output);
    }
    
    KRecord record(getPtr().AS(KGrid));
    
    for(RangeIterator it(getRange()); it.hasMore(); it.next()) {
      at(it, record).writeToBinaryStream(output);
    }
  }

  
  void KGrid::readFromObjectStream(PPtr<ObjectToken> headToken) {
    headToken->validateClass("KGrid");
    
    Ptr<Token> token = headToken->next();
    while(token->is(Token::ATTRIBUTE)) {
      token = token->next();
    }
    
    Ptr< ManagedArray<KRecord> > records = new ManagedArray<KRecord>();
    
    if(token->is(Token::OBJECT)) {
      PPtr<ObjectToken> obj = token.AS(ObjectToken);
      while(obj->checkClass("KRecord")) {
        Ptr<KRecord> rec = new KRecord(_type->getRecordType());
        rec->readFromObjectStream(obj);
        records->push(rec);
        
        token = token->next();
        if(!token->is(Token::OBJECT)) {
          break;
        }
        
        obj = token.AS(ObjectToken);
      }
    }
    
    token->validateType(EndObjectToken::TYPE);
    
    resetWithSize(Tuple1D(records->getSize()));
    
    Ptr<KRecord> record = new KRecord(getPtr().AS(KGrid));
    
    for(RangeIterator it(getRange()); it.hasMore(); it.next()) {
      at(it, record)->set(records->at(it.at(0)).AS(KValue));
    }
  }

  
  void KGrid::serialize(PPtr<ObjectSerializer> serializer) const {
    serializer->object("KGrid")
              ->attribute("range", getRange().toString());
    
    KRecord record(getPtr().AS(KGrid));
    
    for(RangeIterator it(getRange()); it.hasMore(); it.next()) {
      serializer->member(it.toString());
      at(it, record).serialize(serializer);
    }
    
    serializer->endObject();
  }
  

//\/ KGridBasic /\/////////////////////////////////////////////////////////////
    
// --- (DE)CONSTRUCTORS --- //
    
  KGridBasic::KGridBasic(PPtr<KGridType> type)
  : KGrid(type)
  {
    _buffer = NULL;
  }
  
  
  KGridBasic::KGridBasic(PPtr<KGridType> type, const Tuple& dims, bool clear)
  : KGrid(type)
  {
    _buffer = NULL;
    resetWithSize(dims, clear);
  }
  
  
  KGridBasic::~KGridBasic() {
    if(NOT_NULL(_buffer)) {
      cleanupDynamicFields();
      delete[] _buffer;
    }
  }
    
    
// --- METHODS --- //

 
  k_longint_t KGridBasic::getOffsetForIndex(const Tuple& index) const
  throw(IndexOutOfBoundException)
  {
    kf_octet_t s = index.getSize();
    const Tuple& size = _range.getSize();
    
    if(s == 0) {
      throw IndexOutOfBoundException("Attempt to access element " + index
          + " of an empty grid");
    }
    
    kf_int64_t ordinal = 0;
    kf_int64_t p = 1;
    
    for(int i = 0; i < s; i++) {
      ordinal += index.at(i) * p;
      p = p * size.at(i);
    }
    
    if(ordinal > _nElements) {
      throw IndexOutOfBoundException("Index " + index + " does not point "
          " to a valid location in a grid of range " + getRange());
    }
    
    return ordinal * _elementSize;
  }
  
  
  void KGridBasic::resetWithSize(const Tuple& size, bool clear) {
    _range = Range(size);
    _nElements = _range.getVolume();
    k_longint_t nBytes = _nElements * _elementSize;
    
    if(NOT_NULL(_buffer)) {
      cleanupDynamicFields();
      delete[] _buffer;
    }
    
    _buffer = new k_octet_t[nBytes];
    if(clear || getType().AS(KGridType)->getRecordType()->hasDynamicFields())
    {
      memset(_buffer, 0, nBytes);
    }
  }
    
    
//\/ KGridWindow /\////////////////////////////////////////////////////////////
    
// --- (DE)CONSTRUCTORS --- //
    
    KGridWindow::KGridWindow(PPtr<KGrid> physical, const Range& physicalRange)
    : KGrid(physical->getType().AS(KGridType))
    {
      _physical = physical;
      setWindow(physicalRange);
    }
    
    
    KGridWindow::KGridWindow(PPtr<KGrid> physical, const Range& physicalRange,
        const Tuple& virtualOffset)
    : KGrid(physical->getType().AS(KGridType))
    {
      _physical = physical;
      setWindow(physicalRange, virtualOffset);
    }
  
    
// --- METHODS --- //
  
  void KGridWindow::setSource(PPtr<KGrid> physical) {
    _physical = physical;
  }
  
  
  void KGridWindow::setWindow(const Range& physicalRange) {
    if(!_physical->getRange().contains(physicalRange)) {
      throw IndexOutOfBoundException("Provided range " + physicalRange
          + " exceeds size of the physical grid " + _physical->getRange());
    }
    
    _range = physicalRange;
    _translation = Tuple::zero(_range.getNDimensions());
  }
  
  
  void KGridWindow::setWindow(const Range& physicalRange,
                              const Tuple& virtualOffset)
  {
    if(!_physical->getRange().contains(physicalRange)) {
      throw IndexOutOfBoundException("Provided range " + physicalRange
          + " exceeds size of the physical grid " + _physical->getRange());
    }
    
    _range = physicalRange;
    _translation = virtualOffset - _range.getBegin();
  }
  
  
  Tuple KGridWindow::getVirtualOffset() const {
    return _range.getBegin() + _translation;
  }
  
  
  Range KGridWindow::getVirtualRagne() const {
    return _range.translate(_translation);
  }
  
  
  PPtr<KRecord> KGridWindow::atVirtual(const Tuple& index,
      PPtr<KRecord> wrapper) const
  {
    return at(index + _translation, wrapper);
  }
  
  
  KRecord& KGridWindow::atVirtual(const Tuple& index, KRecord& wrapper) const {
    return at(index + _translation, wrapper);
  }
  
  
  void KGridWindow::resetWithSize(const Tuple &size, bool clear) {
    throw KFException("Operation not supported.");
  }
  
  
  const Range& KGridWindow::getRange() const {
    return _range;
  }
  
    
//\/ KGridVector /\////////////////////////////////////////////////////////////
    
#define VEC_INITIAL_CAPACITY 16
#define VEC_GROWTH_RATE 2
    
    
// --- (DE)CONSTRUCTORS --- //
    
    KGridVector::KGridVector(PPtr<KGridType> type)
    : KGrid(type),
      _size(0)
    {
      if(type->getNDimensions() != 1) {
        throw KFException("Type of vector should have one dimension. The "
            "supplied type has " + Int::toString(type->getNDimensions()));
      }
      
      _buffer = NULL;
      _capacity = 0;
      grow();
    }
    
    
    KGridVector::KGridVector(PPtr<KRecordType> elementType)
    : KGrid(new KGridType(elementType, 1)),
      _size(0)
    {
      _buffer = NULL;
      _capacity = 0;
      grow();
    }
    
    
    KGridVector::~KGridVector() {
      if(NOT_NULL(_buffer)) {
        cleanupDynamicFields();
        delete[] _buffer;
      }
    }
    
    
// --- METHODS --- //
    
    
    void KGridVector::grow() {
      k_integer_t oldCapacity = _capacity;
      
      if(IS_NULL(_buffer)) {
        _capacity = VEC_INITIAL_CAPACITY;
      } else {
        _capacity *= VEC_GROWTH_RATE;
      }
      
      k_octet_t* newBuffer = new k_octet_t[_capacity * _elementSize];
      memset(newBuffer, 0, _capacity * _elementSize);
      
      if(NOT_NULL(_buffer)) {
        memcpy(newBuffer, _buffer, oldCapacity*_elementSize);
        delete[] _buffer;
      }
        
      _buffer = newBuffer;
    }
    
    
    k_integer_t KGridVector::basicAdd() {
      k_integer_t s = _size.get();
      
      if(s == _capacity) {
        grow();
      }
      
      _size.set(s + 1);
      _range = Range(_size);
      return s;
    }
    
    
    PPtr<KRecord> KGridVector::add(PPtr<KRecord> wrapper) {
      return at(Tuple1D(basicAdd()), wrapper);
    }
    
  
    void KGridVector::basicInsert(k_integer_t index) {
      k_integer_t s = _size.get();
      
      if(index > s) {
        throw IndexOutOfBoundException("Cannot insert at index " + Int(index)
            + " in an vector of size " + Int(s));
      }
      
      if(s == _capacity) {
        grow();
      }
      
      _size.set(s + 1);
      _range = Range(_size);
      
      if(index < s - 1) {
        memcpy(_buffer + _elementSize * (index + 1),
               _buffer + _elementSize * index,
               _elementSize * (s - index));
      }
      
      memset(_buffer + _elementSize * index, 0, _elementSize);
    }
    
    
    PPtr<KRecord> KGridVector::insert(PPtr<KRecord> wrapper, const k_integer_t index) {
      basicInsert(index);
      return at(Tuple1D(index), wrapper);
    }
    
  
    void KGridVector::remove(const k_integer_t index) {
      k_integer_t s = _size.get();
      
      if(index >= s) {
        throw IndexOutOfBoundException("Cannot remove element "
            + Int::toString(index) + " from a vector of size "
            + Int::toString(s));
      }
      
      Ptr<KRecord> r = new KRecord(getPtr().AS(KGrid));
      at(Tuple1D(index), r)->cleanupDynamicFields();
      
      if(index < s - 1) {
        memcpy(_buffer + _elementSize * index,
               _buffer + _elementSize * (index + 1),
               (s - index - 1) * _elementSize);
      }
      
      _size.set(s - 1);
      _range = Range(_size);
    }
    
    
    void KGridVector::removeLast() {
      remove(_size.get() - 1);
    }
    
    
    PPtr<KRecord> KGridVector::last(PPtr<KRecord> wrapper) const {
      return at(_size - 1, wrapper);
    }
    
  
    k_integer_t KGridVector::getNElements() const {
      return _size.get();
    }
    
    
    void KGridVector::clear() {
      cleanupDynamicFields();
      _size.set(0);
      _range = Range(_size);
    }
    
    
    // Inherited from KGrid //
    
    void KGridVector::resetWithSize(const Tuple& size, bool clear) {
      if(size.getSize() != 1) {
        throw KFException("The supplied size should have one dimensions. The "
            "supplied size has " + Int::toString(size.getSize()));
      }
      
      _capacity = size.at(0);
      
      k_octet_t* newBuffer = new k_octet_t[_capacity * _elementSize];
      memset(newBuffer, 0, _capacity * _elementSize);
      
      if(NOT_NULL(_buffer)) {
        cleanupDynamicFields();
        delete[] _buffer;
      }
      
      _buffer = newBuffer;
      _size.set(_capacity);
      _range = Range(_size);
    }
    
    
    k_longint_t KGridVector::getOffsetForIndex(const Tuple& index) const
    throw(IndexOutOfBoundException)
    {
      if(index.getSize() != 1) {
        throw IndexOutOfBoundException("Given index has invalid size: "
            + index + ". Needed a tuple of size 1.");
      }
      
      return index.at(0) * _elementSize;
    }
    
    
#undef VEC_INITIAL_CAPACITY
#undef VEC_GROWTH_RATE

    
  } // namespace type
} // namespace knorba
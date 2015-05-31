//
//  AbstractKGrid.h
//  KnoRBA
//
//  Created by Hamed KHANDAN on 10/15/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#ifndef __KnoRBA__AbstractKGrid__
#define __KnoRBA__AbstractKGrid__

// KFoundation
#include <kfoundation/Tuple.h>
#include <kfoundation/Range.h>
#include <kfoundation/ManagedArray.h>

// Internal
#include "KGridType.h"
#include "KRecord.h"

// Super
#include "KDynamicValue.h"

namespace knorba {
namespace type {
  
//\/ KGrid /\//////////////////////////////////////////////////////////////////
  
  class KGrid : public KDynamicValue {
    
  // --- FIELDS --- //
    
    private: Ptr<KGridType> _type;
    protected: const k_integer_t _elementSize;
    
    
  // --- CONSTRUCTOR --- //
      
    public: KGrid(PPtr<KGridType> type);
    
    
  // --- METHODS --- //
    protected: void cleanupDynamicFields();
    public: virtual const Range& getRange() const = 0;
    public: virtual void resetWithSize(const Tuple& size, bool clear = false) = 0;
    public: virtual k_longint_t getOffsetForIndex(const Tuple& index) const
            throw(IndexOutOfBoundException) = 0;
    
    public: PPtr<KRecord> at(const Tuple& index, PPtr<KRecord> wrapper) const
        throw(IndexOutOfBoundException);
    
    public: KRecord& at(const Tuple& index, KRecord& wrapper) const
         throw(IndexOutOfBoundException);
    
    public: void copyFrom(const PPtr<KGrid> src, const Tuple& srcOffset,
        const Tuple& dstOffset, const Tuple& size);
    
    // Inherited from KDynamicValue::KValue
    public: void set(PPtr<KValue> other);
    public: PPtr<KType> getType() const;
    public: k_longint_t getTotalSizeInOctets() const;
    public: void writeToBinaryStream(PPtr<OutputStream> output) const;
    public: void readFromBinaryStream(PPtr<InputStream> input);
    public: void readFromObjectStream(PPtr<ObjectToken> headToken);
    
    // Inherited from KValue::SerializingStreamer
    void serialize(PPtr<ObjectSerializer> builder) const;
    
  };
  

//\/ KGridBasic /\/////////////////////////////////////////////////////////////
    
    class KGridBasic : public KGrid {
    
    // --- FIELDS --- //
      
      private: Range       _range;
      private: k_octet_t*  _buffer;
      private: k_longint_t _nElements;
      
      
    // --- (DE)CONSTRUCTOR --- //
      
      public: KGridBasic(PPtr<KGridType> type);
      public: KGridBasic(PPtr<KGridType> type, const Tuple& dims,
              bool clear = false);
      
      public: ~KGridBasic();
      
      
    // --- METHODS --- //
      
      // Inherited from KGrid
      public: inline const Range& getRange() const;
      public: void resetWithSize(const Tuple& size, bool clear = false);
      public: k_longint_t getOffsetForIndex(const Tuple& index) const
              throw(IndexOutOfBoundException);
      
      // Inhertied from KGrid::KDynamicValue
      public: inline k_octet_t* getBaseAddress() const;
      
    };
  
  
    inline const Range& KGridBasic::getRange() const {
      return _range;
    }
  
  
    inline k_octet_t* KGridBasic::getBaseAddress() const {
      return _buffer;
    }

  
//\/ KGridWindow /\////////////////////////////////////////////////////////////
  
  class KGridWindow : public KGrid {
    
  // --- FIELDS --- //
    
    private: Ptr<KGrid>  _physical;
    private: Range _range;
    private: Tuple _translation;
    
    
  // --- (DE)CONSTRUCTOR --- //
    
    public: KGridWindow(PPtr<KGrid> physical, const Range& physicalRange);
    public: KGridWindow(PPtr<KGrid> physical, const Range& physicalRange, const Tuple& virtualOffset);
    
    
  // --- METHODS --- //
    
    public: void setSource(PPtr<KGrid> physical);
    public: void setWindow(const Range& physicalRange);
    public: void setWindow(const Range& physicalRange, const Tuple& virtualOffset);
    public: Tuple getVirtualOffset() const;
    public: Range getVirtualRagne() const;
    public: PPtr<KRecord> atVirtual(const Tuple& index, PPtr<KRecord> wrapper) const;
    public: KRecord& atVirtual(const Tuple& index, KRecord& wrapper) const;
    
    // Inherited from KGrid //
    public: const Range& getRange() const;
    public: void resetWithSize(const Tuple& size, bool clear = false);
    public: inline k_longint_t getOffsetForIndex(const Tuple& index) const
            throw(IndexOutOfBoundException);
    
    // Inhertied from KGrid::KDynamicValue //
    public: inline k_octet_t* getBaseAddress() const;
    
  };
  
  inline k_octet_t* KGridWindow::getBaseAddress() const {
    return _physical->getBaseAddress();
  }
  
  
  inline k_longint_t KGridWindow::getOffsetForIndex(const Tuple& index) const
  throw(IndexOutOfBoundException)
  {
    return _physical->getOffsetForIndex(index);
  }
  
  
//\/ KVectorGrid /\////////////////////////////////////////////////////////////
    
  class KGridVector : public KGrid {
    
  // --- FIELDS --- //
    
    private: Tuple1D     _size;
    private: Range       _range;
    private: k_integer_t _capacity;
    private: k_octet_t*  _buffer;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: KGridVector(PPtr<KGridType> type);
    public: KGridVector(PPtr<KRecordType> elementType);
    public: ~KGridVector();
    
    
  // --- METHODS --- //
    private: void grow();
    private: k_integer_t basicAdd();
    public:  PPtr<KRecord> add(PPtr<KRecord> wrapper);
    private: void basicInsert(k_integer_t index);
    public:  PPtr<KRecord> insert(PPtr<KRecord> wrapper, const k_integer_t index);
    public:  void remove(const k_integer_t index);
    public:  void removeLast();
    public:  PPtr<KRecord> last(PPtr<KRecord> wrapper) const;
    public:  void clear();
    public:  inline k_integer_t getNElements() const;
    
    // Inherited from KGrid //
    public: inline const Range& getRange() const;
    public: void resetWithSize(const Tuple& size, bool clear = false);
    public: k_longint_t getOffsetForIndex(const Tuple& index) const
            throw(IndexOutOfBoundException);
    
    // Inhertied from KGrid::KDynamicValue //
    public: inline k_octet_t* getBaseAddress() const;
    
  };


  inline const Range& KGridVector::getRange() const {
    return _range;
  }
  
  
  inline k_octet_t* KGridVector::getBaseAddress() const {
    return _buffer;
  }
  
    
} // namespace type
} // namespace knorba

#endif /* defined(__KnoRBA__AbstractKGrid__) */
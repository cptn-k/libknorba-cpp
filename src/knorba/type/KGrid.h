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

  /**
   * Wrapper class and C++ representation of KnoRBA `grid` type. KnoRBA `grid`
   * is a multi-dimensional array of records. Both KGrid and KRecord are
   * optimized together for high performance computing. KGrid takes full
   * advantage of range arithmatics objects in KFoundation.
   *
   * All values of a grid are squizzed into a continues portion of memory for
   * better allocation and cache performance. No KRecord object is allocated
   * internally. Allocating a KRecord for each cell would consume too much
   * uncessary memory, and also cause fragmentation, decreasing cache
   * performance. Therefore the design pattern chosen for this object is
   * *sliding record*, that is, only one KRecord is allocated and is slid
   * over desired cells of the grid. However the KRecord is not allocated
   * internally because it would make it impossible to read and write 
   * on the same grid using more than one thread. Therefore, eahc thread should
   * allocate the slidding KRecord externally.
   *
   * KGrid itslef is an abstract class. Use one of the implementations provided
   * in the same header:
   * 
   * * KGridBasic -- internally allocated multi-dimensional array of cells.
   * * KGridWindow -- a sub array of an externally allocated KGrid.
   * * KGridVector -- a one dimensional dynamically resizable array of cells.
   *
   * The basic usage of KGrid with simple record type is as follows:
   *
   *     #include <kfoundation/Tuple.h>
   *     #include <kfoundation/RangeIterator.h>
   *     using namespace kfoundation;
   *
   *     Ptr<KGridType> gridType = new KGridType(KType::REAL, 2);
   *     Ptr<KGrid> grid = new KGridBasic(gridType, Tuple(100, 100));
   *     Ptr<KRecord> r = new KRecord(grid);
   *     for(RangeIterator i(grid->getSize()); i.hasMore(); i.next()) {
   *       grid->at(i, r)->setInteger(i.at(0));
   *     }
   *
   * The at() method slides the given KRecord on to the cell at the given index.
   *
   *     grid->at(Tuple2D(11, 12), r);
   *     r->setInteger(124);
   *
   * In case the cell record type has more than one field, the name or index
   * of the field can be supplied to KRecord::setXXX() and KRecord::getXX()
   * methods.
   *
   *     r->setInteger("year", 1981);
   *
   * @headerfile KGrid.h <knorba/type/KGrid.h>
   */

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

    // Inherited from KValue::StreamDeserializer
    public: void deserialize(PPtr<ObjectToken> headToken);
    
    // Inherited from KValue::SerializingStreamer
    void serialize(PPtr<ObjectSerializer> builder) const;
    
  };
  

//\/ KGridBasic /\/////////////////////////////////////////////////////////////

    /**
     * Basic variant of KGrid. Most often, this is the class to use for creating
     * and manipulating KnoRBA `grid`.
     *
     * Read documentation for KGrid for more details.
     *
     * @headerfile KGrid.h <knorba/type/KGrid.h>
     */

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

  /**
   * Places a virtual index range over a portion of an existing grid. This 
   * class is specially usefull in high-performance scenarios when exchanging
   * boundaries between nodes, and computing over a range of values using
   * multiple nodes and multiple threads.
   *
   * Read documentation for KGrid for more details.
   *
   * @headerfile KGrid.h <knorba/type/KGrid.h>
   */

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

  /**
   * One-dimensional variable-length flavour of KGrid. This `grid` implementaion
   * has only one dimension but in return, it has dynamic size and supports
   * common vector operations.
   *
   * Read documentation for KGrid for more details.
   *
   * @headerfile KGrid.h <knorba/type/KGrid.h>
   */

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
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
#include <kfoundation/RefArray.h>

// Internal
#include "KGridType.h"
#include "KRecord.h"

namespace knorba {
namespace type {

//\/ KGridRecord /\////////////////////////////////////////////////////////////

  class KGridRecord : public KRecord {
    public: KGridRecord(RefConst<KRecordType> t);
    protected: virtual k_octet_t* getBuffer() = 0;
    protected: virtual const k_octet_t* getBuffer() const = 0;
    public: virtual KGridRecord& at(const Tuple& index) = 0;
    public: virtual KGridRecord& atWindow(const Tuple& index) = 0;
  };
  

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

  class KGrid : public KValue {

  // --- NESTED TYPES --- //

    public: typedef struct {
      k_octet_t nDimensions;
      k_octet_t nWindows;
      k_integer_t* dimensions;
      k_octet_t* data;
    } shared_header_t;


    public: typedef struct {
      k_integer_t* windowBegin;
      k_integer_t* windowEnd;
      shared_header_t* sharedHeader;
    } window_header_t;


  // --- STATIC METHODS --- //

    public: static void headerSetWindow(window_header_t* header,
        const Range& window);
    public: static k_octet_t headerGetRetainCount(const window_header_t* header);
    public: static Tuple headerGetSize(const window_header_t* header);
    public: static k_longint_t headerGetNElements(const window_header_t* header);
    public: static Range headerGetWindow(const window_header_t* header);
    public: static Tuple headerMapWindowToGlobal(const window_header_t* header,
        const Tuple& index);
    public: static k_longint_t headerGetOrdinalForIndex(
        const window_header_t* header, const Tuple& index);


  // --- FIELDS --- //
    
    private: RefConst<KGridType> _type;
    private: window_header_t _header;

    
  // --- CONSTRUCTOR --- //
      
    public: KGrid(RefConst<KGridType> type);
    public: KGrid(RefConst<KGridType> type, const Tuple& dimensions);
    public: ~KGrid();
    
    
  // --- METHODS --- //

    protected: virtual k_octet_t* getBuffer();
    protected: virtual const k_octet_t* getBuffer() const;
    private: inline window_header_t* getHeader();
    private: inline const window_header_t* getHeader() const;

    public: Tuple getSize() const;
    public: Range getWindow() const;

    protected: void grow(const Tuple& size);
    public: void reset();
    public: void reset(const Tuple& size);
    public: void reset(RefConst<KGrid> origin, const Range& window);
    public: void moveWindow(const Range& window);

    public: Ref<KGridRecord> makeRecord() const;
    public: Ref<KGrid> makeWindow(const Range& range) const;

    public: void copyFrom(RefConst<KGrid> src, const Tuple& srcOffset,
        const Tuple& dstOffset, const Tuple& size);
    
    // Inherited from KDynamicValue::KValue
    public: virtual void set(RefConst<KValue> other);
    public: RefConst<KType> getType() const;

  };


  inline KGrid::window_header_t* KGrid::getHeader() {
    return (window_header_t*)getBuffer();
  }


  inline const KGrid::window_header_t* KGrid::getHeader() const {
    return (window_header_t*)getBuffer();
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
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: KGridVector(Ref<KRecordType> elementType);
    public: ~KGridVector();
    
    
  // --- METHODS --- //

    private: void grow();
    private: Tuple basicAdd();
    private: void basicInsert(k_integer_t index);
    public: Ref<KGridRecord> add(Ref<KGridRecord> wrapper);
    public: Ref<KGridRecord> add();
    public: Ref<KGridRecord> insert(Ref<KGridRecord> wrapper, const k_integer_t index);
    public: Ref<KGridRecord> insert(const k_integer_t index);
    public: void remove(const k_integer_t index);
    public: void removeLast();
    public: Ref<KGridRecord> last(Ref<KGridRecord> wrapper) const;
    public: Ref<KGridRecord> last() const;

  };


} // namespace type
} // namespace knorba

#endif /* defined(__KnoRBA__AbstractKGrid__) */
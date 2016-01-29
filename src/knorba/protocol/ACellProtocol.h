//
//  ACellProtocol.h
//  AgentRuntimeEnvironment
//
//  Created by Kay Khandan on 5/21/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

#ifndef __AgentRuntimeEnvironment__ACellProtocol__
#define __AgentRuntimeEnvironment__ACellProtocol__

// KFoundation
#include <kfoundation/Direction.h>
#include <kfoundation/Range.h>
#include <kfoundation/Condition.h>
#include <kfoundation/Thread.h>

// Super
#include <knorba/Protocol.h>

namespace kfoundation {
  template<typename T> class ManagedArray;
  class ProximityIterator;
}


namespace knorba {
namespace protocol {
  
  using namespace knorba::type;
  
  class ACellProtocol : public Protocol {
    
  // --- NESTED TYPES --- //
    
    private: class KTuple : public Tuple {
      public: SPtr<KGridType> TYPE;
      public: KTuple();
      public: KTuple(int s);
      public: KTuple(PPtr<KGrid> g);
      public: void toKGrid(PPtr<KGrid> g);
    };
    
    
    private: class Partition : public KFObject {
      private: SPtr<KRecordType> TYPE;
      public: SPtr<KRecordType> type();
      
      public: k_guid_t guid;
      public: Tuple index;
      public: Range range;
      public: Direction direction;
      public: bool isAdjacent;
      
      public: Partition();
      public: Partition(k_guid_t guid, PPtr<Partition> reference, PPtr<KRecord> r);
      public: void toKRecord(PPtr<KRecord> r) const;
    };
    
    
  // --- STATIC FIELDS --- //
    
    public: static SPtr<KGridType> PARTITION_MAP_T;
    
    public : static SPtr<KString> OP_INDEX_Q;
    public : static SPtr<KString> OP_INDEX_A;
    public : static SPtr<KString> OP_PARTITION_MAP;
    public : static SPtr<KString> OP_BORDER;
    
    
  // --- STATIC METHODS --- //
    
    public: static void init(Runtime& rt);
    

  // --- FIELDS --- //
    
    private: Ptr<KRecordType> _cellType;
    private: PPtr<KString> _role;
    private: Ptr<Partition> _localPartition;
    private: Tuple _globalSize;
    private: k_octet_t _nDimensions;
    private: k_octet_t _proximityRadius;
    
    private: Ptr<KGridBasic> _layer1;
    private: Ptr<KGridBasic> _layer2;
    private: PPtr<KGridBasic> _readLayer;
    private: PPtr<KGridBasic> _writeLayer;
    private: PPtr<KGridWindow> _displayArea;
    private: Ptr< ManagedArray<Partition> > _partitions;
    private: Ptr< ManagedArray<Partition> > _neighbours;
    private: k_integer_t _phase;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: ACellProtocol(Agent* agent, PPtr<KRecordType> cellType,
        PPtr<KString> role);
    
    
  // --- METHODS --- //
    
    private: int getPartitionIndex(const k_guid_t& guid) const;
    private: void createPartitionMap();
    private: void anouncePartitionMap();
    private: void start();
    public: void start(const Tuple& globalSize, const Tuple& index);
    
    // Handlers //
    public: void handleOpIndexQ(PPtr<Message> msg);
    public: void handleOpIndexA(PPtr<Message> msg);
    public: void handleOpPartition(PPtr<Message> msg);
    
    
    // Virtual Methods //
    public: virtual void cellRagnge(PPtr<KGrid> read, PPtr<KGrid> write,
        RangeIterator& it);
    
    public: virtual void phase(PPtr<KGrid> data, k_integer_t phase);
    
    // Inherited from Protocol //
    public: void handlePeerDisconnected(PPtr<KString> role, const k_guid_t& guid);
    
  };
    
} // protocol
} // knorba
  
#endif /* defined(__AgentRuntimeEnvironment__ACellProtocol__) */
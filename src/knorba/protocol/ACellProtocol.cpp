//
//  ACellProtocol.cpp
//  AgentRuntimeEnvironment
//
//  Created by Kay Khandan on 5/21/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

// KFoundation
#include <kfoundation/Ptr.h>
#include <kfoundation/RangeIterator.h>

// KnoRBA
#include <knorba/Agent.h>
#include <knorba/Group.h>
#include <knorba/type/all.h>

// Self
#include "ACellProtocol.h"

namespace knorba {
namespace protocol {
  
//\/ ACellProtocol::Peer /\////////////////////////////////////////////////////
  
  ACellProtocol::Peer::Peer(kf_octet_t nDims)
  : index(nDims),
    direction(nDims)
  {
    // Nothing;
  }
  
  
//\/ ACellProtocol /\//////////////////////////////////////////////////////////

// --- STATIC METHODS --- //

  SPtr<KRecordType> ACellProtocol::PARTITION_T;
  SPtr<KRecordType> ACellProtocol::PARTITION_TABLE_T;
  
  SPtr<KGridType> ACellProtocol::TUPLE_T = new KGridType(KType::INTEGER, 1);
  
  SPtr<KString> ACellProtocol::OP_INDEX_Q = KS("knorba.a-cell.index-q");
  SPtr<KString> ACellProtocol::OP_INDEX_A = KS("knorba.a-cell.index-a");
  SPtr<KString> ACellProtocol::OP_PARTITION_MAP = KS("knorba.a-cell.partition-map");
  SPtr<KString> ACellProtocol::OP_BORDER = KS("knorba.a-cell.border");
  
  
// --- STATIC FIELDS --- //
  
  SPtr<KRecordType> ACellProtocol::partition_t() {
    if(!PARTITION_T.isNull()) {
      PARTITION_T = new KRecordType("knorba.a-cell.Partition");
      PARTITION_T->addField("guid", KType::GUID);
      PARTITION_T->addField("index", TUPLE_T.AS(KType));
    }
    return PARTITION_T;
  }
  
  
  SPtr<KRecordType> ACellProtocol::partition_table_t() {
    if(!PARTITION_TABLE_T.isNull()) {
      Ptr<KGridType> gt = new KGridType(partition_t(), 1);
      
      PARTITION_TABLE_T = new KRecordType("knorba.a-cell.PartitionTable");
      PARTITION_TABLE_T->addField("size", TUPLE_T.AS(KType))
        ->addField("divisor", TUPLE_T.AS(KType))
        ->addField("records", gt.AS(KType));
    }
    
    return PARTITION_TABLE_T;
  }
  
  
  void ACellProtocol::init(Runtime& rt) {
    rt.registerMessageFormat(OP_INDEX_Q, KType::NOTHING);
    rt.registerMessageFormat(OP_INDEX_A, TUPLE_T.AS(KType));
    rt.registerMessageFormat(OP_PARTITION_MAP, partition_t().AS(KType));
  }
  
  
  void ACellProtocol::tupleToGrid(const Tuple& t, PPtr<KGrid> g) {
    Tuple1D s(t.getSize());
    g->resetWithSize(s);
    Ptr<KRecord> rec = new KRecord(g.AS(KGrid));
    for(RangeIterator i(s); i.hasMore(); i.next()) {
      g->at(i, rec)->setInteger(t.at(i.at(0)));
    }
  }
  
  
  Ptr<KGridBasic> ACellProtocol::tupleToGrid(const Tuple& t) {
    Ptr<KGridBasic> g = new KGridBasic(TUPLE_T);
    tupleToGrid(t, g.AS(KGrid));
    return g;
  }
  
  
// --- (DE)CONSTRUCTORS --- //
  
  ACellProtocol::ACellProtocol(Agent* agent, PPtr<KRecordType> cellType,
      PPtr<KString> role)
  : Protocol(agent)
  {
    _cellType = cellType;
    _role = role;
    registerHandler((phandler_t)&ACellProtocol::handleOpIndexQ, OP_INDEX_Q);
    registerHandler((phandler_t)&ACellProtocol::handleOpIndexA, OP_INDEX_A);
    registerHandler((phandler_t)&ACellProtocol::handleOpPartition, OP_PARTITION_MAP);
  }
  
  
// --- METHODS --- //
  
  void ACellProtocol::createPartitionMap() {
    Tuple maxIndex = _localPartition->index;
    
    for(int i = _partitions->getSize() - 1; i >= 0; i--) {
      maxIndex = maxIndex.max(_partitions->at(i)->index);
    }
    
    int d[][] = new int[][_nDimensions];
    for(int i = 0; i < _nDimensions; i++) {
      d[i] = new int[maxIndex.at(i)];
    }
    
    // Here ....
  }
  
  
  void ACellProtocol::start() {
    LOG << "START." << EL;
  }

  
  void ACellProtocol::start(const Tuple& globalSize, const Tuple& index) {
    _globalSize = globalSize;
    _nDimensions = _globalSize.getSize();
    
    _localPartition = new Partition();
    _localPartition->guid = _agent->getGuid();
    _localPartition->index = index;
    _localPartition->range = Range(globalSize);
    _localPartition->direction = Direction(index.getSize());
    _localPartition->isAdjacent = false;
    
    if(_agent->getRuntime().isHead()) {
      if(_agent->getPeers(_role)->isEmpty()) {
        start();
      } else {
        _agent->send(_role, OP_INDEX_Q, KValue::NOTHING);
      }
    }
  }

  
  // Handlers //
  
  void ACellProtocol::handleOpIndexQ(PPtr<Message> msg) {
    Ptr<KRecord> r = new KRecord(Partition::type());
    _localPartition->toKRecord(r);
    _agent->respond(msg, OP_INDEX_A, r.AS(KValue));
  }
  
  
  void ACellProtocol::handleOpIndexA(PPtr<Message> msg) {
    if(!_agent->getRuntime().isHead()) {
      PLOG_ERR << "OP_INDEX_A received by nonleader." << EL;
      return;
    }
    
    if(_partitions.isNull()) {
      _partitions = new ManagedArray<Partition>();
    }
    
    const k_guid_t& sender = msg->getSender();
    
    if(getPartitionIndex(sender) >= 0) {
      PLOG_ERR << "Double response from " << sender << EL;
      return;
    }
    
    Ptr<Partition> p = new Partition(sender, _localPartition,
        msg->getPayload());
    
    _partitions->push(p);
    
    if(_partitions->getSize() == _agent->getPeers(_role)->getCount()) {
      anouncePartitionMap();
      start();
    }
  }
  
  
  void ACellProtocol::handleOpPartition(PPtr<Message> msg) {
    PLOG << *msg->getPayload() << EL;
  }
  
  
  void ACellProtocol::handlePeerDisconnected(PPtr<KString> role,
      const k_guid_t& guid)
  {
    
  }

} // namespace protocol
} // namespace knorba
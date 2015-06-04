//
//  DisplayInfoProtocol.cpp
//  AgentRuntimeEnvironment
//
//  Created by Hamed KHANDAN on 5/21/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

// KFoundation
#include <kfoundation/Ptr.h>
#include <kfoundation/RangeIterator.h>

// KnoRBA
#include <knorba/Agent.h>
#include <knorba/MessageSet.h>
#include <knorba/Group.h>
#include <knorba/type/all.h>

// Self
#include "DisplayInfoProtocol.h"

namespace knorba {
namespace protocol {

//\/ DisplayInfoProtocol::KTuple2D /\//////////////////////////////////////////
  
  SPtr<KRecordType> DisplayInfoProtocol::KTuple2D::TYPE;
  
  SPtr<KRecordType> DisplayInfoProtocol::KTuple2D::type() {
    if(TYPE.isNull()) {
      TYPE = new KRecordType("knorba.display.Tuple2D");
      TYPE->addField("x", KType::INTEGER)
          ->addField("y", KType::INTEGER);
    }
    return TYPE;
  }
  
  
  DisplayInfoProtocol::KTuple2D::KTuple2D()
  : Tuple2D()
  {
    // Nothing;
  }
  
  
  DisplayInfoProtocol::KTuple2D::KTuple2D(PPtr<KRecord> r)
  : Tuple2D(r->getInteger(X), r->getInteger(Y))
  {
    // Nothing;
  }
  
  
  Ptr<KRecord> DisplayInfoProtocol::KTuple2D::toKRecord() const {
    Ptr<KRecord> r = new KRecord(type());
    toKRecord(r);
    return r;
  }
  
  
  void DisplayInfoProtocol::KTuple2D::toKRecord(PPtr<KRecord> r) const {
    r->setInteger(X, getX());
    r->setInteger(Y, getY());
  }
  
  
//\/ DisplayInfoProtocol::Display /\///////////////////////////////////////////
  
  SPtr<KRecordType> DisplayInfoProtocol::Display::TYPE;
  
  
  SPtr<KRecordType> DisplayInfoProtocol::Display::type() {
    if(TYPE.isNull()) {
      TYPE = new KRecordType("knorba.display.Display");
      TYPE->addField("index", KTuple2D::type().AS(KType))
          ->addField("size"   , KTuple2D::type().AS(KType));
    }
    return TYPE;
  }
  
  
  DisplayInfoProtocol::Display::Display() {
    // Nothing;
  }

  
  DisplayInfoProtocol::Display::Display(PPtr<KRecord> r)
  : _index(r->getRecord(INDEX)),
    _size(r->getRecord(SIZE))
  {
    // Nothing;
  }
  
  
  Range DisplayInfoProtocol::Display::getRect() const {
    return Range(_index * _size, (_index + 1) * _size);
  }

  
  const DisplayInfoProtocol::KTuple2D& DisplayInfoProtocol::Display::getIndex()
  const
  {
    return _index;
  }
  
  
  const DisplayInfoProtocol::KTuple2D& DisplayInfoProtocol::Display::getSize()
  const
  {
    return _size;
  }
  
  
  void DisplayInfoProtocol::Display::toKRecord(PPtr<KRecord> r) const {
    _index.toKRecord(r->getRecord(INDEX));
    _size.toKRecord(r->getRecord(SIZE));
  }
  
  
  void DisplayInfoProtocol::Display::serialize(PPtr<ObjectSerializer> s) const {
    s->object("Display")
      ->attribute("index", _index.toString())
      ->attribute("size", _size.toString())
      ->endObject();
  }
  
  
//\/ DisplayInfoProtocol::NodeInfo /\//////////////////////////////////////////
  
  SPtr<KGridType> DisplayInfoProtocol::NodeInfo::TYPE;
  
  
  SPtr<KGridType> DisplayInfoProtocol::NodeInfo::type() {
    if(TYPE.isNull()) {
      TYPE = new KGridType(DisplayInfoProtocol::Display::type(), 1);
    }
    return TYPE;
  }

  
  DisplayInfoProtocol::NodeInfo::NodeInfo(const k_guid_t& guid)
  : _rect(Tuple2D(0, 0), Tuple2D(0, 0)),
    _indexRange(Tuple2D(0, 0), Tuple2D(0, 0))
  {
    _guid = guid;
    _displays = new Array<Display>();
  }
  
  
  DisplayInfoProtocol::NodeInfo::NodeInfo(const k_guid_t& guid, PPtr<KGrid> list)
  {
    _guid = guid;
    _displays = new Array<Display>();
    
    Ptr<KRecord> r = new KRecord(list);
    for(RangeIterator i(list->getRange()); i.hasMore(); i.next()) {
      addDisplay(Display(list->at(i, r)));
    }
  }
  
  
  void DisplayInfoProtocol::NodeInfo::addDisplay(const Display& d) {
    if(_displays->getSize() == 0) {
      _rect = d.getRect();
      _indexRange = Range(d.getIndex(), d.getIndex() + 1);
    } else {
      _rect = _rect.joinWith(d.getRect());
      _indexRange.joinWith(d.getIndex() + 1);
    }
    
    _displays->push(d);
    _indexRange = _indexRange.joinWith(Range(d.getIndex(), d.getIndex()));
  }
  
  
  const k_guid_t& DisplayInfoProtocol::NodeInfo::getGuid() const {
    return _guid;
  }
  
  
  const Range& DisplayInfoProtocol::NodeInfo::getRect() const {
    return _rect;
  }
  
  
  const Range& DisplayInfoProtocol::NodeInfo::getIndexRange() const {
    return _indexRange;
  }
  
  
  int DisplayInfoProtocol::NodeInfo::getNDisplays() const {
    return _displays->getSize();
  }
  
  
  const DisplayInfoProtocol::Display&
  DisplayInfoProtocol::NodeInfo::getDisplayAtIndex(int index) const
  {
    return _displays->at(index);
  }
  
  
  void DisplayInfoProtocol::NodeInfo::toKGrid(PPtr<KGrid> g) const {
    Tuple1D s(_displays->getSize());
    g->resetWithSize(s);
    Ptr<KRecord> r = new KRecord(g);
    for(RangeIterator i(s); i.hasMore(); i.next()) {
      _displays->at(i.at(0)).toKRecord(g->at(i, r));
    }
  }
  
  
  void DisplayInfoProtocol::NodeInfo::serialize(PPtr<ObjectSerializer> s) const {
    s->object("Node")
      ->attribute("guid", KGlobalUid::toString(_guid))
      ->attribute("rect", _rect.toString())
      ->attribute("indexRange", _indexRange.toString());
        
    s->member("displays")->collection();
    
    int size = _displays->getSize();
    for(int i = 0; i < size; i++) {
      s->object(_displays->at(i));
    }
    
    s->endCollection();
    s->endObject();
  }
  
  
// --- STATIC FIELDS --- //
  
  SPtr<KRecordType> DisplayInfoProtocol::SETUP_T;
  
  const SPtr<KString> DisplayInfoProtocol::OP_SETUP_Q
      = KS("knorba.display.setup-q");
  
  const SPtr<KString> DisplayInfoProtocol::OP_SETUP_A
      = KS("knorba.display.setup-a");
  
  const SPtr<KString> DisplayInfoProtocol::OP_DISPLAYS_Q
      = KS("knorba.display.displays-q");
  
  const SPtr<KString> DisplayInfoProtocol::OP_DISPLAYS_A
      = KS("knorba.display.displays-a");
  
  
// --- STATIC METHODS --- //
  
  SPtr<KRecordType> DisplayInfoProtocol::setup_t() {
    if(SETUP_T.isNull()) {
      SETUP_T = new KRecordType("knorba.display.Setup");
      SETUP_T->addField("windowOrigin", KTuple2D::type().AS(KType))
             ->addField("displays", NodeInfo::type().AS(KType));
    }
    return SETUP_T;
  }
  
  
  void DisplayInfoProtocol::init(Runtime &rt) {
    rt.registerMessageFormat(OP_SETUP_A, setup_t().AS(KType));
    rt.registerMessageFormat(OP_SETUP_Q, setup_t().AS(KType));
    rt.registerMessageFormat(OP_DISPLAYS_Q, KType::NOTHING);
    rt.registerMessageFormat(OP_DISPLAYS_A, NodeInfo::type().AS(KType));
  }
  
  
// --- (DE)CONSTRUCTORS --- //
  
  DisplayInfoProtocol::DisplayInfoProtocol(Agent* agent)
  : Protocol(agent)
  {
    registerHandler((phandler_t)&DisplayInfoProtocol::handleOpDisplayQ,
         OP_DISPLAYS_Q);
  }
  
  DisplayInfoProtocol::DisplayInfoProtocol(Agent* agent, PPtr<KString> role)
  : Protocol(agent)
  {
    _role = role;
    _peerInfo = new ManagedArray<NodeInfo>();
    
    registerHandler((phandler_t)&DisplayInfoProtocol::handleOpDisplayQ,
        OP_DISPLAYS_Q);
  }
  
  
// --- METHODS --- //
  
  bool DisplayInfoProtocol::queryLocalDisplays() {
    Ptr<MessageSet> responses = _agent->tsendToLocals(OP_SETUP_Q,
        KValue::NOTHING, 400);
    
    if(responses.isNull()) {
      return false;
    }
    
    if(responses->isEmpty()) {
      return false;
    }
    
    PPtr<Message> msg = responses->get(0);
    PPtr<KRecord> setup = msg->getPayload().AS(KRecord);
    _windowOrigin = KTuple2D(setup->getRecord(SETUP_T_ORIGIN));

    _localInfo = new NodeInfo(_agent->getGuid(),
        setup->getGrid(SETUP_T_DISPLAYS));
    
    _globalRect = _localInfo->getRect();
    _globalIndexRange = _localInfo->getIndexRange();    
    
    return true;
  }
  
  
  bool DisplayInfoProtocol::queryPeerDisplays() {
    if(_role.isNull()) {
      return false;
    }
    
    if(_localInfo.isNull()) {
      if(!queryLocalDisplays()) {
        return false;
      }
    }
    
    Ptr<MessageSet> responses = _agent->tsend(_role, OP_DISPLAYS_Q,
        KValue::NOTHING);
    
    for(int i = responses->getSize() - 1; i >= 0; i--) {
      PPtr<Message> msg = responses->get(i);
      if(!getInfoForRemoteAgent(msg->getSender()).isNull()) {
        continue;
      }
      
      Ptr<NodeInfo> node = new NodeInfo(msg->getSender(),
          msg->getPayload().AS(KGrid));
      
      _peerInfo->push(node);
      
      _globalRect = _globalRect.joinWith(node->getRect());
      _globalIndexRange = _globalIndexRange.joinWith(node->getIndexRange());
    }
    
    return responses->getSize() >= _agent->getPeers(_role)->getCount();
  }
  
  
  bool DisplayInfoProtocol::hasDisplay() {
    return !_localInfo.isNull();
  }
  
  
  bool DisplayInfoProtocol::isFullScreen() {
    if(!hasDisplay()) {
      return false;
    }
    
    return _windowOrigin.equals(Tuple2D(-1, -1));
  }
  
  
  const DisplayInfoProtocol::KTuple2D&
  DisplayInfoProtocol::getWindowOrigin() const
  {
    return _windowOrigin;
  }
  
  
  PPtr<DisplayInfoProtocol::NodeInfo> DisplayInfoProtocol::getLocalInfo()
  const
  {
    return _localInfo;
  }
  
  
  PPtr< ManagedArray<DisplayInfoProtocol::NodeInfo> >
  DisplayInfoProtocol::getRemoteInfo() const
  {
    return _peerInfo;
  }
  
  
  PPtr<DisplayInfoProtocol::NodeInfo>
  DisplayInfoProtocol::getInfoForRemoteAgent(const k_guid_t& guid) const
  {
    if(_peerInfo.isNull()) {
      return NULL;
    }
    
    for(int i = _peerInfo->getSize() - 1; i >= 0; i--) {
      if(_peerInfo->at(i)->getGuid() == guid) {
        return _peerInfo->at(i);
      }
    }
    
    return NULL;
  }
  
  
  const Range& DisplayInfoProtocol::getGlobalRect() const {
    return _globalRect;
  }
  
  
  const Range& DisplayInfoProtocol::getGlobalIndexRange() const {
    return _globalIndexRange;
  }
  
  
  // Handlers //
  
  void DisplayInfoProtocol::handleOpDisplayQ(PPtr<Message> msg) {
    if(_localInfo.isNull()) {
      if(!queryLocalDisplays()) {
        _agent->respond(msg, Agent::OP_NG, new KString("No displays"));
        return;
      }
    }
    
    Ptr<KGrid> g = new KGridBasic(NodeInfo::type());
    _localInfo->toKGrid(g);
    _agent->respond(msg, OP_DISPLAYS_A, g.AS(KValue));
  }
  
  
  // Virtual Methods //
  
  void DisplayInfoProtocol::onAllPeersJoined() {
    // Nothing;
  }
  
} // namespace protocol
} // namespace knorba
/*---[Agent.cpp]-----------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : -
 |  Implements: knorba::Agent::*
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
#include <cstdlib>

// KFoundation
#include <kfoundation/Ptr.h>
#include <kfoundation/Logger.h>
#include <kfoundation/LongInt.h>

// Internal
#include "Message.h"
#include "MessageSet.h"
#include "Group.h"
#include "Runtime.h"
#include "type/KType.h"
#include "type/KRecord.h"
#include "type/KString.h"
#include "type/KGrid.h"
#include "type/KGuid.h"

// Self
#include "Agent.h"

#undef ADLOG
#define ADLOG(X) KF_NOP

#define N_MAX_OPEN_TRANSACTIONS 10

namespace knorba {
  
  using namespace std;
  
//\/ Agent::TransactionRecord /\///////////////////////////////////////////////
  
  Agent::TransactionRecord::TransactionRecord() {
    _count = 0;
    _transactionId = -1;
    _isOpen = false;
    _responses = new MessageSet();
  }
  
  
  Agent::TransactionRecord::~TransactionRecord() {
    // Nothing;
  }
  
  
  void Agent::TransactionRecord::add(Ptr<Message> msg) {
    _count++;
    _responses->add(msg.retain());
  }
  
  
  void Agent::TransactionRecord::reset() {
    _count = 0;
    _transactionId = -1;
    _responses = new MessageSet();
    _isOpen = false;
  }
  

//\/ Agent::MessageThread /\///////////////////////////////////////////////////
  
  Agent::MessageThread::MessageThread(Agent* owner, int rank)
  : Thread(owner->getAlias() + " message handler " + Int::toString(rank)),
    _owner(owner)
  {
    // Nothing;
  }
  
  
  void Agent::MessageThread::run() {
    _owner->messageProcessor();
  }
  
  
  int Agent::MessageThread::getRank() {
    return _rank;
  }
  
  
//\/ Agent::FinalizerThread /\/////////////////////////////////////////////////
  
  Agent::FinalizerThread::FinalizerThread(Agent* owner)
  : Thread(owner->getAlias() + " finalizer"),
    _owner(owner)
  {
    // Nothing;
  }
  
  
  void Agent::FinalizerThread::run() {
    _owner->finalize();
  }
  
  
//\/ Agent /\//////////////////////////////////////////////////////////////////
  
// --- STATIC FIELDS --- //

  /** Default queue size. */
  const int Agent::DEFAULT_QUEUE_SIZE;

  /** Opcode for connect request message */
  const SPtr<KString> Agent::OP_CONNECT = KS("knorba.agent.connect");

  /** Opcode for acknowledge [response] message */
  const SPtr<KString> Agent::OP_ACK     = KS("knorba.agent.ack");

  /** Opcode for NG message [response] message */
  const SPtr<KString> Agent::OP_NG      = KS("knorba.agent.ng");
  
  
// --- (DE)CONSTRUCTOR --- //
  
  /**
   * Sole constructor. Reference to runtime and GUID are provided by runtime
   * when initializing dynamic library containing the agent code.
   *
   * @param rt Reference to runtime access API
   * @param guid The GUID allocated by runtime for this agent.
   * @param queueSize The maximum number of message can be kept in the queue
   *        at any given time. Default value is 16.
   */
  
  Agent::Agent(Runtime& rt, const k_guid_t& guid, int queueSize)
  : _runtime(rt),
    _newMessageCond(true),
    _transactionCond(true),
    _transactionMutex(true),
    _queueMutex(true)
  {
    _guid = guid;
    _isAutoExit = false;
    _quitFlag = false;
    _isFinalized = false;
    _queueSize = queueSize;
    _queueHead = 0;
    _queueTail = 0;
    _queueCount = 0;
    _topThread = 0;
    _nRunningThreads = 0;
    
    _openTransactions = new ManagedArray<TransactionRecord>();
    for(int i = 0; i < N_MAX_OPEN_TRANSACTIONS; i++) {
      Ptr<TransactionRecord> rec = new TransactionRecord();
      rec->_index = i;
      _openTransactions->push(rec);
    }
    
    _messageQueue = new ManagedArray<Message>();
    _messageQueue->setSize(_queueSize);
    
    _messageQueueHelper = new Array<HandlerRecord>();
    _messageQueueHelper->setSize(_queueSize);
    
    registerHandler(&Agent::handleOpConnect, OP_CONNECT);

    LOG << "(O) Agent \"" << getAlias() << "\", GUID: " << guid << EL;
  }
  
  
  /**
   * Deconstructor. It advisable to override finalize() method instead of
   * overriding the deconstructor.
   */
  
  Agent::~Agent() {
    if(isAlive() || _nRunningThreads > 0) {
      log(Logger::ERR) << "Being destructed while alive." << EL;
    }
    
    if(_quitFlag && !_isFinalized) {
      ALOG_WRN << "Agent being destructed wile finalizing. Waiting for "
          "finalization to complete ..." << EL;
      
      while(!_isFinalized) {
        System::sleep(100);
      }
    }
    
    LOG << "(X) Agent \"" << getAlias() << "\", GUID: " << _guid << EL;
  }
  
  
  
// --- METHODS --- //
  
// Message handling //
  
  void Agent::messageProcessor() {
    _nRunningThreads++;
    _topThread++;
    int thisThread = _topThread;
    
    if(thisThread > 1) {
      _newMessageCond.release();
    }
    
    ALOG << "Thread " << thisThread << " in." << EL;
    
    int  msecsToWait = 1;
    bool exitFlag = false;
    
    while(!exitFlag) {
      msecsToWait = 1;
      
      while(_queueCount == 0 && !exitFlag) {
        _newMessageCond.block(System::getCurrentTimeInMiliseconds() + msecsToWait);
        msecsToWait = 1000;
        exitFlag = (thisThread != _topThread) || _quitFlag;
      }
      
      while(_queueCount > 0) {
        _queueMutex.lock();
        PPtr<Message> msg = _messageQueue->at(_queueTail);
        HandlerRecord& hr = _messageQueueHelper->at(_queueTail);
        _messageQueue->at(_queueTail) = NULL;
        
        ADLOG("queue(" << _queueTail << ") >> " << msg->headerToString(_runtime));
        
        _queueTail++;
        if(_queueTail == _queueSize) {
          _queueTail = 0;
        }
        _queueCount--;
        _queueMutex.unlock();
        
        // BEGIN handle message
        try {
          if(NOT_NULL(hr.protocol)) {
            (hr.protocol->*hr.phandler)(msg);
          } else {
            (this->*hr.handler)(msg);
          }
        } catch(KFException& e) {
          ALOG_ERR << "Quitting because of an exception: " << e << EL;
          msg.release();
          quit();
          break;
        }
        
        ADLOG("Message processed: " << msg->headerToString(_runtime));
        
        msg.release();
        // END handle message
        
        
        exitFlag = (thisThread != _topThread) || _quitFlag;
      }
    } // while(!exitFlag)
    
    ALOG << "Thread " << thisThread << " out." << EL;
    
    _nRunningThreads--;
    if(_nRunningThreads == 0) {
      ADLOG("Last thread out.");
    }
  }
  
  
  PPtr<Agent::TransactionRecord> Agent::startTransaction(
      k_integer_t tid, int count)
  {
    PPtr<TransactionRecord> record;
    
    KF_SYNCHRONIZED(_transactionMutex,
      for(int i = 0; i < N_MAX_OPEN_TRANSACTIONS; i++) {
        if(!_openTransactions->at(i)->_isOpen) {
          record = _openTransactions->at(i);
          record->_isOpen = true;
          record->_transactionId = tid;
          record->_count = count;
          break;
        }
      }
    ) // KF_SYNCHRONIZED
    
    if(record.isNull()) {
      throw KFException("Too many open transactions.");
    }
    
    ADLOG("BEGIN transaction(" << tid << "@" << record->_index << ")");
    
    return record;
  }
  
  
  void Agent::wait(PPtr<Agent::TransactionRecord> trans, int msecs) {
    if(_topMessageThread->isTheCurrentThread()) {
      _topMessageThread = new MessageThread(this, _nRunningThreads + 1);
      _topMessageThread->start();
    }

    int count = trans->_count;
    
    int s = 0;
    KF_SYNCHRONIZED(_transactionMutex,
      s = trans->_responses->getSize();
    )
    
    if(msecs > 0) {
      
      kf_int64_t now = System::getCurrentTimeInMiliseconds();
      kf_int64_t then = now + msecs;
      
      while(now < then && s < count) {
        _transactionCond.block(then);
        now = System::getCurrentTimeInMiliseconds();
        KF_SYNCHRONIZED(_transactionMutex,
          s = trans->_responses->getSize();
        )
      }
      
    } else {
      
      while(s < count && !_quitFlag) {
        _transactionCond.block(System::getCurrentTimeInMiliseconds() + 200);
        ADLOG("WAITING transaction(" << trans->_transactionId << ")"
              << " nResponses: " << trans->_responses->getSize());
        KF_SYNCHRONIZED(_transactionMutex,
          s = trans->_responses->getSize();
        )
      }
      
    }
    
    ADLOG("END transaction(" << trans->_transactionId << "@" << trans->_index
          << ")");
  }
  
  
  void Agent::processMessage(PPtr<Message> msg) {
    if(msg->getSender() == _guid) {
      ALOG_ERR << "Received message from self: "
          << msg->headerToString(_runtime) << EL;
    }
    
    bool handledAsTransaction = false;
    
    if(msg->getTransactionId() != -1) {
      int tid = msg->getTransactionId();
      
      _transactionMutex.lock();
      for(int i = 0; i < N_MAX_OPEN_TRANSACTIONS; i++) {
        if(_openTransactions->at(i)->_transactionId == tid) {
          
          PPtr<TransactionRecord> rec = _openTransactions->at(i);
          
          ADLOG(msg->headerToString(_runtime) << " >> transaction(" << tid
                << "@" << rec->_index << ")");
          
          rec->_responses->add(msg);
          if(rec->_count == rec->_responses->getSize()) {
            _transactionCond.releaseAll();
          }
          handledAsTransaction = true;
          break;
        }
      }
      _transactionMutex.unlock();
    }
    
    if(!handledAsTransaction) {
      if(_queueCount == _queueSize)
      {
        ALOG_WRN << "Message queue is full." << EL;
        int tries = 0;
        while(_queueCount == _queueSize && tries < 100) {
          System::sleep(10);
          tries++;
        }
        ALOG_WRN << "Delayed " << tries * 10 << "ms" << EL;
        if(tries == 100) {
          ALOG_ERR << "Time out expired for a full queue. Quitting." << EL;
          quit();
        }
      }
      
      HandlerRecord hr;
      
      k_longint_t hash = msg->getOpcodeHash();
      hr.handler = getHandlerForOpcodeHash(hash);
      hr.protocol = NULL;
      bool isUnhandled = (hr.handler == NULL);
      
      if(isUnhandled && !_protocols.isNull()) {
        for(int i = _protocols->getSize() - 1; i >= 0 && isUnhandled; i--) {
          hr.protocol = _protocols->at(i);
          hr.phandler = hr.protocol->getHandlerForOpcodeHash(hash);
          isUnhandled = (hr.phandler == NULL);
        }
      }
      
      if(!isUnhandled) {
        ADLOG(msg->headerToString(_runtime) << " >> queue(" << _queueHead << ")");
        
        KF_SYNCHRONIZED(_queueMutex,
          _messageQueueHelper->at(_queueHead) = hr;
          _messageQueue->at(_queueHead) = msg;
          _queueHead++;
          if(_queueHead == _queueSize) {
            _queueHead = 0;
          }
          _queueCount++;
        ) // KF_SYNCHRONIZED
        
        _newMessageCond.release();
      }
      
      if(isUnhandled) {
        msg.release();
      }
      
    }// if(!handledAsTransaction)
  } // void Agent::processMessage()


  
// Lifecycle //

  /**
   * FOR INTERNAL USE. Never invoke directly.
   */
  
  void Agent::run() {
    if(isAlive()) {
      LOG_ERR << "Duplicate run request." << EL;
      return;
    }
    
    _topMessageThread = new MessageThread(this, _nRunningThreads + 1);
    _topMessageThread->start();
  }
  
  
  /**
   * Runs the finalizer thread, which stops the message processor thread and
   * runs the finalize() method. If successful, informs the runtime, which
   * will release resources consumed by this agent.
   */
  
  void Agent::quit() {
    if(_quitFlag) {
      return;
    }
    
    Ptr<FinalizerThread> ft = new FinalizerThread(this);
    ft->start();
  }
  

  /**
   * Marks this agent as passive. This method is best to be called once in
   * the constructor. Passive agents will quit automatically when all other
   * non-passive agents quit.
   *
   * @param value If set true the agent will be passive, otherwise it will not.
   */
  
  void Agent::setPassive(bool value) {
    _isAutoExit = value;
  }
  
  
  /**
   * Checks if this agent is marked as passive.
   */
  
  bool Agent::isPassive() const {
    return _isAutoExit;
  }
  
  
  /**
   * Pauses the current thread while making sure the message processor thread
   * is always running.
   *
   * @param msecs Amount of time to sleep, measured in milliseconds.
   */
  
  void Agent::sleep(int msecs) {
    if(_topMessageThread->isTheCurrentThread()) {
      _topMessageThread = new MessageThread(this, _nRunningThreads + 1);
      _topMessageThread->start();
    }
    
    System::sleep(msecs);
  }


  
// Handlers and protocols //
  
  /**
   * Registers a handler for the given opcode.
   *
   * @param h Pointer to handler method
   * @param opcode The opcode that activates the given handler
   */
  
  void Agent::registerHandler(handler_t h, PPtr<KString> opcode) {
    _handlers[opcode->getHashCode()] = h;
  }
  
  
  Agent::handler_t Agent::getHandlerForOpcodeHash(const k_longint_t hash) {
    HandlerMap_t::iterator it = _handlers.find(hash);
    if(it == _handlers.end()) {
      return NULL;
    } else {
      return it->second;
    }
  }


  /**
   * FOR INTERNAL USE. Do not call directly. Activates support for the given
   * protocol in this agent.
   */
  
  void Agent::registerProtocol(Protocol* protocol) {
    if(_protocols.isNull()) {
      _protocols = new Array<Protocol*>();
    }
    
    _protocols->push(protocol);
  }


  /**
   * FOR INTERNAL USE. Do not call directly. Deactivates support for the given
   * protocol.
   */
  
  void Agent::unregisterProtocol(Protocol* p) {
    if(_quitFlag) {
      return;
    }
    
    if(_protocols.isNull()) {
      return;
    }
    
    for(int i = _protocols->getSize() - 1; i >= 0; i--) {
      if(_protocols->at(i) == p) {
        _protocols->remove(i);
      }
    }
  }
  
  
// Peers //
  
  int Agent::getRoleIndexByMember(const k_guid_t& guid) const {
    if(_connections.isNull()) {
      return -1;
    }
    
    for(int i = _connections->getSize() - 1; i >= 0; i--) {
      if(_connections->at(i)->targets->containts(guid)) {
        return i;
      }
    }
    
    return -1;
  }
  
  
  int Agent::getRoleIndexByName(PPtr<KString> role) const {
    if(_connections.isNull()) {
      return -1;
    }
    
    for(int i = _connections->getSize() - 1; i >= 0; i--) {
      if(_connections->at(i)->role->equals(role)) {
        return i;
      }
    }
    
    return -1;
  }
  
  
  /**
   * Adds a peer with the given GUID to the given role.
   *
   * @param role The role for the peer to be added
   * @param guid The GUID of the peer to be added.
   */
  
  void Agent::addPeer(PPtr<KString> role, const k_guid_t& guid) {
    int index = getRoleIndexByName(role);
    if(index < 0) {
      if(_connections.isNull()) {
        _connections = new ManagedArray<Connection>();
        _allPeers = new Group();
      }
      
      Ptr<Connection> c = new Connection();
      c->role = role;
      c->targets = new Group();
      c->targets->add(guid);
      _connections->push(c);
    } else {
      _connections->at(index)->targets->add(guid);
    }
    
    ALOG << "Peer added with role \"" << *role << "\": " << guid << EL;
    
    _allPeers->add(guid);
  }
  
  
  /**
   * Removes the peer with the given GUID from the given role. In case the
   * target GUID is not assigned to the given role, this method will complete
   * successfully without making any changes.
   *
   * @note A remote agent can be assigned to multiple roles. In that case, it
   *       should be removed from each role one at a time.
   *
   * @param role The role the peer to be removed from.
   * @param guid The GUID of the peer to be removed.
   */
  
  void Agent::removePeer(PPtr<KString> role, const k_guid_t& guid) {
    int index = getRoleIndexByName(role);
    if(index >= 0) {
      _connections->at(index)->targets->remove(guid);
      _allPeers->remove(guid);
    }
    
    ALOG << "Peer removed from role \"" << *_connections->at(index)->role
        << "\": " << guid << EL;
  }
  
  
  /**
   * Removes all peers associated with the given role. If the indicated role
   * does not exist, the method will end successfully without any effects.
   *
   * @param role The role to be removed.
   */
  
  void Agent::removeAllPeers(PPtr<KString> role) {
    int index = getRoleIndexByName(role);
    if(index >= 0) {
      ALOG << "All peers with role \"" << *_connections->at(index)->role
        << "\" are removed :" << *_connections->at(index)->targets << EL;
      _connections->remove(index);
    }
  }
  
  
  /**
   * Removes all peers that share the same AppId as in the given GUID.
   *
   * @param guid The AppID part of this GUID will be matched against all peers
   *        of this agent.
   */
  
  void Agent::removeAllPeersWithMatchingAppId(const k_guid_t& guid) {
    if(_connections.isNull()) {
      return;
    }
    
    for(int i = _connections->getSize() - 1; i >= 0; i--) {
      PPtr<Connection> c = _connections->at(i);
      PPtr<Group> g = c->targets;
      for(int j = g->getCount() - 1; j >= 0; j--) {
        if(KGuid::areOnTheSameNode(g->get(j), guid)) {
          c->targets->remove(guid);
          _allPeers->remove(guid);
          
          ALOG << "Peer removed from role \"" << *c->role << "\": " << guid
              << EL;
          
          handlePeerDisconnected(c->role, guid);
        }
      }
    }
  }
  
  
  /**
   * Checks whether or not the given GUID belongs to a registered peer.
   */
  
  bool Agent::isPeer(const k_guid_t& guid) const {
    if(_allPeers.isNull() || _quitFlag) {
      return false;
    }
    
    return _allPeers->containts(guid);
  }
  
  
  /**
   * Returns the role of the peer with the given GUID. Returns NULL if the given
   * GUID does not belong to a peer.
   */
  
  PPtr<KString> Agent::getRole(const k_guid_t& guid) const
  {
    int index = getRoleIndexByMember(guid);
    if(index < 0) {
      return NULL;
    }
    return _connections->at(index)->role;
  }
  
  
  /**
   * Returns all the peers associated with the given role. Returns an empty
   * group of the given role does not exist.
   */
  
  PPtr<Group> Agent::getPeers(PPtr<KString> role) const {
    int index = getRoleIndexByName(role);
    if(index < 0) {
      return Group::empty_group();
    }
    return _connections->at(index)->targets;
  }
  
  
  /**
   * Returns a group of all the registered peers.
   */
  
  PPtr<Group> Agent::getAllPeers() const {
    return _allPeers;
  }
  

// Send //
  
  /**
   * Sends a message to another agent.
   * 
   * @param receiver The GUID of the receiver agent.
   * @param opcode The opcode of the message.
   * @param content The content of the message.
   */
  
  void Agent::send(const k_guid_t receiver, PPtr<KString> opcode,
      PPtr<KValue> content)
  {
    _runtime.send(_guid, receiver, opcode->getHashCode(), content, -1);
  }
  
  
  /**
   * Sends a multicast message to a group of agents.
   *
   * @param receivers Group of receiver agents.
   * @param opcode The opcode of the message.
   * @param content The content of the message.
   */
  
  void Agent::send(PPtr<Group> receivers, PPtr<KString> opcode,
      PPtr<KValue> content)
  {
    _runtime.send(_guid, receivers, opcode->getHashCode(), content, -1);
  }
  
  
  /**
   * Sends a multicast message to all agents with the given role.
   *
   * @param role The role of the target agents.
   * @param opcode The opcode of the message.
   * @param content The content of the message.
   */
  
  void Agent::send(PPtr<KString> role, PPtr<KString> opcode,
      PPtr<KValue> content)
  {
    int index = getRoleIndexByName(role);
    if(index == -1) {
      return;
    }
    send(_connections->at(index)->targets, opcode, content);
  }
  
  
  /**
   * Sends a broadcast message.
   *
   * @param opcode Message opcode.
   * @param content Message content.
   */
  
  void Agent::sendToAll(PPtr<KString> opcode, PPtr<KValue> content) {
    _runtime.sendToAll(_guid, opcode->getHashCode(), content, -1);
  }


  /**
   * Sends a broadcast message to all local agents. 
   *
   * @note Local agents are all the agents running within the same Virtual ARE,
   * plus kernel agents residing on the local machine.
   *
   * @param opcode Message opcode.
   * @param content Message content.
   */
  
  void Agent::sendToLocals(PPtr<KString> opcode, PPtr<KValue> content) {
    _runtime.sendToLocals(_guid, opcode->getHashCode(), content, -1);
  }
  
  
  /**
   * Sends a message in response to a received message. 
   *
   * @note All messages for which Message::needsResponse() returns `true`, i.e.
   *       messages sent using tsendXXX methods, should be responded using this
   *       method.
   *
   * @param msg The message to reply to.
   * @param opcode The opcode of the response.
   * @param content The content of the response.
   */
  
  void Agent::respond(PPtr<Message> msg, PPtr<KString> opcode,
      PPtr<KValue> content)
  {
    _runtime.send(_guid, msg->getSender(), opcode->getHashCode(), content,
      msg->getTransactionId());
  }
  
  
  /**
   * Blocking unicast send. 
   * Sends a message to a remote agent and blocks the current thread
   * until the message is responded or the given timeout expires.
   *
   * @param receiver GUID of the receiving agent.
   * @param opcode Message opcode.
   * @param content Message content.
   * @param timeout Expressed in milliseconds. If set to -1 (default value),
   *        it will cause this method to wait indefinitely until a response is
   *        received. If set to any positive value, this method will wait until
   *        a response is received or until the timeout expires, whichever
   *        happens sooner.
   * @return The response message, if any, or null pointer if none.
   */
  
  Ptr<Message> Agent::tsend(const k_guid_t receiver, PPtr<KString> opcode,
      PPtr<KValue> content, k_integer_t timeout)
  {
    k_integer_t tid = rand();
    PPtr<TransactionRecord> trans = startTransaction(tid, 1);
    _runtime.send(_guid, receiver, opcode->getHashCode(), content, tid);
    wait(trans, timeout);
    
    Ptr<Message> response;
    
    _transactionMutex.lock();
    if(!trans->_responses->isEmpty()) {
      response = trans->_responses->get(0).retain();
    }
    trans->reset();
    _transactionMutex.unlock();
    
    return response.retain();
  }
  
  
  /**
   * Blocking multicast send.
   * Sends a message to a group of remote agents and blocks the current thread
   * until the message is responded by all targets or the given timeout expires.
   *
   * @param receivers Group of receiver agents.
   * @param opcode Message opcode.
   * @param content Message content.
   * @param timeout Expressed in milliseconds. If set to -1 (default value),
   *        it will cause this method to wait indefinitely until a response is
   *        received. If set to any positive value, this method will wait until
   *        all targets reply or until the timeout expires, whichever
   *        happens sooner.
   * @return A MessageSet containing all responses received.
   */

  Ptr<MessageSet> Agent::tsend(PPtr<Group> receivers,
      PPtr<KString> opcode, PPtr<KValue> content, k_integer_t timeout)
  {
    k_integer_t tid = rand();
    PPtr<TransactionRecord> trans = startTransaction(tid, receivers->getCount());
    _runtime.send(_guid, receivers, opcode->getHashCode(), content, tid);
    wait(trans, timeout);
    
    _transactionMutex.lock();
    Ptr<MessageSet> responses = trans->_responses.retain();
    trans->reset();
    _transactionMutex.unlock();
    
    return responses.retain();
  }
  
  
  /**
   * Blocking multicast send to peers.
   * Sends a message to all remote agent with the given role, and blocks the 
   * current thread until all target agents respond or the given timeout 
   * expires.
   *
   * @param receivers The role of receiver peers.
   * @param opcode Message opcode.
   * @param content Message content.
   * @param timeout Expressed in milliseconds. If set to -1 (default value),
   *        it will cause this method to wait indefinitely until a response is
   *        received. If set to any positive value, this method will wait until
   *        a response is received or until the timeout expires, whichever
   *        happens sooner.
   * @return A MessageSet containing all responses received.
   */

  Ptr<MessageSet> Agent::tsend(PPtr<KString> receivers, PPtr<KString> opcode,
      PPtr<KValue> content, k_integer_t timeout)
  {
    int index = getRoleIndexByName(receivers);
    if(index == -1) {
      return new MessageSet();
    }
    
    return tsend(_connections->at(index)->targets, opcode, content, timeout);
  }
  
  
  /**
   * Blocking local broadcast.
   * Sends a message to all local agents, and blocks the current thread until
   * the given timeout expires.
   *
   * @param opcode Message opcode.
   * @param content Message content.
   * @param timeout Expressed in milliseconds. The amount of time to block the
   *                current thread, waiting for responses.
   * @return A MessageSet containing all responses received.
   */

  Ptr<MessageSet> Agent::tsendToLocals(PPtr<KString> opcode,
      PPtr<KValue> content,  k_integer_t timeout)
  {
    if(timeout <= 0) {
      throw KFException("Illegal value for timeout: " + Int(timeout));
    }
    k_integer_t tid = rand();
    PPtr<TransactionRecord> trans = startTransaction(tid, 999999);
    _runtime.sendToLocals(_guid, opcode->getHashCode(), content, tid);
    wait(trans, timeout);
    
    _transactionMutex.lock();
    Ptr<MessageSet> responses = trans->_responses.retain();
    trans->reset();
    _transactionMutex.unlock();
    
    return responses.retain();
  }
  
  
  
// Etc. //

  /**
   * Returns the GUID of this agent.
   */
  const k_guid_t& Agent::getGuid() const {
    return _guid;
  }
  
  
  /**
   * Returns a logger stream into the default logger, beginning with the
   * identity of this agent. Usage:
   *
   *     this->log() << "Hello!" << EL;
   *
   * @param level The log level. Default value is Logger::L3.
   */

  Logger::Stream& Agent::log(const Logger::level_t level) const {
    return System::getLogger().log(level) << "Agent \"" << getAlias() << "\": ";
  }
  
  
  /**
   * Returns reference to runtime interface.
   */

  Runtime& Agent::getRuntime() {
    return _runtime;
  }
  
  
  void Agent::handleOpConnect(PPtr<Message> msg) {
    PPtr<KString> role = msg->getPayload().AS(KString);
    const k_guid_t& guid = msg->getSender();
    handlePeerConnectionRequest(role, guid);
  }
  
  
// Virtual Methods //
    
  /**
   * Override to handle peer connection request. Default behavior is to 
   * forward the request to all protocols, if any.
   *
   * @param role The request role for the new peer.
   * @param guid The GUID of the agent requesting to become a peer.
   * @see handlePeerDisconnected()
   * @see Protocol::handlePeerConnectionRequest
   */

  void Agent::handlePeerConnectionRequest(PPtr<KString> role, const k_guid_t& guid)
  {
    if(!_protocols.isNull()) {
      for(int i = _protocols->getSize() - 1; i >= 0; i--) {
        _protocols->at(i)->handlePeerConnectionReuqest(role, guid);
      }
    }
  }
  
  
  /**
   * Override to handle peer disconnect notifications. Default behavior is to
   * forward the request to all protocols, if any.
   *
   * @param role The role of the peer to be removed.
   * @param guid The GUID of the agent requesting to be removed as peer.
   * @see handlePeerConnectionRequest()
   * @see Protocol::handlePeerDisconnected()
   */

  void Agent::handlePeerDisconnected(PPtr<KString> role, const k_guid_t& guid) {
    if(!_protocols.isNull()) {
      for(int i = _protocols->getSize() - 1; i >= 0; i--) {
        _protocols->at(i)->handlePeerDisconnected(role, guid);
      }
    }
  }
  
  
  /**
   * Override to perform additional tasks when agent is finalizing.
   * Stops the message processor thread.
   *
   * @see isAlive()
   * @see Protocol::finalize()
   */
  void Agent::finalize() {
    _quitFlag = true;
    
    int nAttempts = 0;
    while(_queueHead != _queueTail) {
      System::sleep(100);
      if(nAttempts == 10) {
        ALOG_WRN << "Taking too much time to finalize. There are unhandled "
            "messages in the queue." << EL;
      }
      nAttempts++;
    }
    
    if(!_protocols.isNull()) {
      for(int i = _protocols->getSize() - 1; i >= 0; i--) {
        _protocols->at(i)->finalize();
      }
    }
    
    _newMessageCond.release();
    _transactionCond.release();
    
    while(isAlive()) {
      System::sleep(100);
      if(nAttempts == 10) {
        ALOG_WRN << "Taking too much time to finalize." << EL;
      }
      nAttempts++;
    }
    
    _isFinalized = true;
    
    _runtime.signalQuit();
  }
  
  
  /**
   * Returns true if the agent is alive. As long as this method returns true,
   * the ARE containing this agent will not shut down. Override if there are
   * additional criteria to determine this agent is alive. E.g. other threads
   * are running, connections are open, etc.
   *
   * @see finalize()
   * @see Protocol::isAlive()
   */

  bool Agent::isAlive() {
    if(!_topMessageThread.isNull()) {
      if(_topMessageThread->isRunning()) {
        return true;
      }
    }
    
    if(_protocols.isValid()) {
      for(int i = _protocols->getSize() - 1; i >= 0; i--) {
        if(_protocols->at(i)->isAlive()) {
          return true;
        }
      }
    }
    
    if(_nRunningThreads > 0) {
      return true;
    }
    
    return false;
  }
  
  
} // namespace knorba

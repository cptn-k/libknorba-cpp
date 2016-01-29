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
#include <kfoundation/Ref.h>
#include <kfoundation/Array.h>
#include <kfoundation/RefArray.h>
#include <kfoundation/Dictionary.h>
#include <kfoundation/Path.h>

// Internal
#include "Communicator.h"
#include "Message.h"
#include "MessageSet.h"
#include "Group.h"
#include "Protocol.h"

#include "type/KType.h"
#include "type/KRecord.h"
#include "type/KString.h"
#include "type/KGrid.h"
#include "type/KGur.h"

// Self
#include "Agent.h"

#undef ADLOG
#define ADLOG(X) KF_NOP

#define N_MAX_OPEN_TRANSACTIONS 10

namespace knorba {
  
  using namespace std;
  
//\/ Agent::TransactionRecord /\///////////////////////////////////////////////

  Agent::TransactionRecord::TransactionRecord(k_integer_t id,
       k_integer_t countCond)
  {
    responses = new MessageSet();
    countCondition = countCond;
    transactionId = id;
  }


  void Agent::TransactionRecord::add(Ref<Message> msg) {
    responses->add(msg);
    if(--countCondition == 0) {
      condition.release();
    }
  }


//\/ Agent::MessageThread /\///////////////////////////////////////////////////
  
  Agent::MessageThread::MessageThread(Agent* owner, k_integer_t rank)
  : Thread(owner->getAlias()->toString() + " message handler " + rank),
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
  : Thread(owner->getAlias()->toString() + " finalizer"),
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
  const StaticRefConst<KString> Agent::OP_CONNECT = new KString("knorba.agent.connect");

  /** Opcode for acknowledge [response] message */
  const StaticRefConst<KString> Agent::OP_ACK = new KString("knorba.agent.ack");

  /** Opcode for NG message [response] message */
  const StaticRefConst<KString> Agent::OP_NG = new KString("knorba.agent.ng");
  
  
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
  
  Agent::Agent(Communicator& com)
  : _com(com),
    _transactionMutex(true),
    _queueMutex(true)
  {
    _isAutoExit = false;
    _quitFlag = false;
    _isFinalized = false;
    _nRunningThreads = 0;
    
    _openTransactions = new RefArray<TransactionRecord>();

    registerHandler(&Agent::handleOpConnect, OP_CONNECT);

    LOG << "(O) Agent \"" << *getAlias() << "\", GUID: " << KGur(getGur()) << OVER;
  }
  
  
  /**
   * Deconstructor. It advisable to override finalize() method instead of
   * overriding the deconstructor.
   */
  
  Agent::~Agent() {
    if(isAlive() || _nRunningThreads > 0) {
      log(Logger::ERR) << "Being destructed while alive." << OVER;
    }
    
    if(_quitFlag && !_isFinalized) {
      ALOG_WRN << "Agent being destructed wile finalizing. Waiting for "
          "finalization to complete ..." << OVER;
      
      while(!_isFinalized) {
        System::sleep(100);
      }
    }
    
    LOG << "(X) Agent \"" << *getAlias() << "\", GUID: " << KGur(getGur()) << OVER;
  }
  
  
  
// --- METHODS --- //
  
// Message handling //
  
  void Agent::messageProcessor() {
    _nRunningThreads++;

    ALOG << "Thread " << Thread::getNameOfCurrentThread() << " in." << OVER;
    
    ALOG << "Thread " << Thread::getNameOfCurrentThread() << " out." << OVER;
  }


  void Agent::processMessage(Ref<Message> msg) {
    bool handledAsTransaction = false;

    if(msg->getTransactionId() != -1) {
      k_integer_t tid = msg->getTransactionId();

      _transactionMutex.lock();

      RefArray<TransactionRecord>::Iterator i = _openTransactions->getIterator();
      for(Ref<TransactionRecord> r = i.first(); i.hasMore(); i.next()) {
        if(r->transactionId == tid) {
          ADLOG(msg->headerToString(_runtime) << " >> transaction(" << tid
                << "@" << rec->_index << ")");

          r->responses->add(msg);
          handledAsTransaction = true;

          break;
        }
      }

      _transactionMutex.unlock();
    }

    if(!handledAsTransaction) {
      k_longint_t opcode = msg->getOpcodeHash();
      handler_t handler = getHandlerForOpcodeHash(opcode);
      if(IS_NULL(handler)) {
        Array<Protocol*>::Iterator i = _protocols->getIterator();
        for(Protocol* p = i.first(); i.hasMore(); i.next()) {
          Protocol::phandler_t phandler = p->getHandlerForOpcodeHash(opcode);
          if(NOT_NULL(phandler)) {
            (p->*phandler)(msg);
            break;
          }
        }
      } else {
        (this->*handler)(msg);
      }
    }
  } // void Agent::processMessage()


  k_integer_t Agent::startTransaction(k_integer_t tid, k_integer_t countCond) {
    Ref<TransactionRecord> record = new TransactionRecord(tid, countCond);
    k_integer_t index;

    KF_SYNCHRONIZED(_transactionMutex,
      index = _openTransactions->getSize();
      _openTransactions->push(record);
    ) // KF_SYNCHRONIZED

    ADLOG("BEGIN transaction(" << tid << "@" << record->_index << ")");
    
    return index;
  }
  
  
  Ref<MessageSet> Agent::wait(k_integer_t index, k_integer_t msecs) {
    if(_topMessageThread->isTheCurrentThread()) {
      _topMessageThread = new MessageThread(this, _nRunningThreads + 1);
      _topMessageThread->start();
    }

    Ref<TransactionRecord> r = _openTransactions->at(index);

    if(msecs > 0) {
      kf_int64_t now = System::getCurrentTimeInMiliseconds();
      kf_int64_t then = now + msecs;
      r->condition.block(then);
    } else {
      r->condition.block(System::getCurrentTimeInMiliseconds() + 200);
    }
    
    ADLOG("END transaction(" << trans->_transactionId << "@" << trans->_index
          << ")");

    Ref<MessageSet> result = r->responses;
    _openTransactions->at(index) = NULL;
    _openTransactions->remove(index);
    
    return result;
  }

  
// Lifecycle //

  /**
   * FOR INTERNAL USE. Never invoke directly.
   */
  
  void Agent::run() {
    if(isAlive()) {
      ALOG_ERR << K"Duplicate run request." << OVER;
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
    
    Ref<FinalizerThread> ft = new FinalizerThread(this);
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
  
  void Agent::sleep(k_integer_t msecs) {
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
  
  void Agent::registerHandler(handler_t h, RefConst<KString> opcode) {
    _handlers->at(opcode->getHashCode()) = h;
  }
  
  
  Agent::handler_t Agent::getHandlerForOpcodeHash(const k_longint_t hash) {
    if(!_handlers->containsKey(hash)) {
      return NULL;
    }
    return _handlers->at(hash);
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
  
  k_integer_t Agent::getRoleIndexByMember(const k_gur_t& guid) const {
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
  
  
  k_integer_t Agent::getRoleIndexByName(RefConst<KString> role) const {
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
  
  void Agent::addPeer(RefConst<KString> role, const k_gur_t& guid) {
    k_integer_t index = getRoleIndexByName(role);
    if(index < 0) {
      if(_connections.isNull()) {
        _connections = new RefArray<Connection>();
        _allPeers = new Group();
      }
      
      Ref<Connection> c = new Connection();
      c->role = role;
      c->targets = new Group();
      c->targets->add(guid);
      _connections->push(c);
    } else {
      _connections->at(index)->targets->add(guid);
    }
    
    ALOG << "Peer added with role \"" << *role << "\": " << KGur(guid) << OVER;
    
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
  
  void Agent::removePeer(RefConst<KString> role, const k_gur_t& guid) {
    int index = getRoleIndexByName(role);
    if(index >= 0) {
      _connections->at(index)->targets->remove(guid);
      _allPeers->remove(guid);
    }
    
    ALOG << "Peer removed from role \"" << *_connections->at(index)->role
        << "\": " << KGur(guid) << OVER;
  }
  
  
  /**
   * Removes all peers associated with the given role. If the indicated role
   * does not exist, the method will end successfully without any effects.
   *
   * @param role The role to be removed.
   */
  
  void Agent::removeAllPeers(RefConst<KString> role) {
    int index = getRoleIndexByName(role);
    if(index >= 0) {
      ALOG << "All peers with role \"" << *_connections->at(index)->role
        << "\" are removed :" << *_connections->at(index)->targets << OVER;
      _connections->remove(index);
    }
  }
  
  
  /**
   * Removes all peers that share the same AppId as in the given GUID.
   *
   * @param guid The AppID part of this GUID will be matched against all peers
   *        of this agent.
   */
  
  void Agent::removeAllPeersWithMatchingAppId(const k_gur_t& guid) {
    if(_connections.isNull()) {
      return;
    }

    KGur appid(guid);
    
    for(int i = _connections->getSize() - 1; i >= 0; i--) {
      Ref<Connection> c = _connections->at(i);
      Ref<Group> g = c->targets;
      for(k_integer_t j = g->getCount() - 1; j >= 0; j--) {
        k_gur_t item = g->get(i);
        if(appid.appIdEquals(item)) {
          c->targets->remove(item);
          _allPeers->remove(item);
          
          ALOG << "Peer removed from role \"" << *c->role << "\": "
              << KGur(item) << OVER;
          
          handlePeerDisconnected(c->role, guid);
        }
      }
    }
  }
  
  
  /**
   * Checks whether or not the given GUID belongs to a registered peer.
   */
  
  bool Agent::isPeer(const k_gur_t& guid) const {
    if(_allPeers.isNull() || _quitFlag) {
      return false;
    }
    
    return _allPeers->containts(guid);
  }
  
  
  /**
   * Returns the role of the peer with the given GUID. Returns NULL if the given
   * GUID does not belong to a peer.
   */
  
  RefConst<KString> Agent::getRole(const k_gur_t& guid) const {
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
  
  RefConst<Group> Agent::getPeers(RefConst<KString> role) const {
    int index = getRoleIndexByName(role);
    if(index < 0) {
      return Group::EMPTY_GROUP;
    }
    return _connections->at(index)->targets;
  }
  
  
  /**
   * Returns a group of all the registered peers.
   */
  
  RefConst<Group> Agent::getAllPeers() const {
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
  
  void Agent::send(const k_gur_t receiver, RefConst<KString> opcode,
      Ref<KValue> content)
  {
    _com.send(receiver, opcode->getHashCode(), content, -1);
  }
  
  
  /**
   * Sends a multicast message to a group of agents.
   *
   * @param receivers Group of receiver agents.
   * @param opcode The opcode of the message.
   * @param content The content of the message.
   */
  
  void Agent::send(RefConst<Group> receivers, RefConst<KString> opcode,
      Ref<KValue> content)
  {
    _com.send(receivers, opcode->getHashCode(), content, -1);
  }
  
  
  /**
   * Sends a multicast message to all agents with the given role.
   *
   * @param role The role of the target agents.
   * @param opcode The opcode of the message.
   * @param content The content of the message.
   */
  
  void Agent::send(RefConst<KString> role, RefConst<KString> opcode,
      Ref<KValue> content)
  {
    int index = getRoleIndexByName(role);
    if(index == -1) {
      return;
    }
    _com.send(_connections->at(index)->targets, opcode->getHashCode(), content, -1);
  }
  
  
  /**
   * Sends a broadcast message.
   *
   * @param opcode Message opcode.
   * @param content Message content.
   */
  
  void Agent::sendToAll(RefConst<KString> opcode, Ref<KValue> content) {
    _com.sendToAll(opcode->getHashCode(), content, -1);
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
  
  void Agent::sendToLocals(RefConst<KString> opcode, Ref<KValue> content) {
    _com.sendToLocals(opcode->getHashCode(), content, -1);
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
  
  void Agent::respond(RefConst<Message> msg, RefConst<KString> opcode,
      Ref<KValue> content)
  {
    _com.send(msg->getSender(), opcode->getHashCode(), content,
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
  
  Ref<Message> Agent::tsend(const k_gur_t receiver, RefConst<KString> opcode,
      Ref<KValue> content, k_integer_t timeout)
  {
    k_integer_t tid = rand();
    k_integer_t index = startTransaction(tid, 1);
    _com.send(receiver, opcode->getHashCode(), content, tid);
    Ref<MessageSet> responses = wait(index, timeout);
    if(responses->isEmpty()) {
      return NULL;
    }
    return responses->get(0);
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

  Ref<MessageSet> Agent::tsend(RefConst<Group> receivers,
      RefConst<KString> opcode, Ref<KValue> content, k_integer_t timeout)
  {
    k_integer_t tid = rand();
    int index = startTransaction(tid, receivers->getCount());
    _com.send(receivers, opcode->getHashCode(), content, tid);
    return wait(index, timeout);
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

  Ref<MessageSet> Agent::tsend(RefConst<KString> receivers,
      RefConst<KString> opcode, Ref<KValue> content, k_integer_t timeout)
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

  Ref<MessageSet> Agent::tsendToLocals(RefConst<KString> opcode,
      Ref<KValue> content,  k_integer_t timeout)
  {
    if(timeout <= 0) {
      throw KFException(K"Illegal value for timeout: " + timeout);
    }
    k_integer_t tid = rand();
    k_integer_t index = startTransaction(tid, 999999);
    _com.sendToLocals(opcode->getHashCode(), content, tid);
    return wait(index, timeout);
  }
  
  
  
// Etc. //

  /**
   * Returns the GUID of this agent.
   */
  const k_gur_t& Agent::getGur() const {
    return _com.getGur();
  }
  
  
  /**
   * Returns a logger stream into the default logger, beginning with the
   * identity of this agent. Usage:
   *
   *     this->log() << "Hello!" << EL;
   *
   * @param level The log level. Default value is Logger::L3.
   */

  Ref<PrintWriter> Agent::log(const Logger::level_t level) const {
    return System::getLogger()->log(level) << "Agent \"" << *getAlias() << "\": ";
  }


  RefConst<Path> Agent::getPathToResources() const {
    return _com.getPathToResouces();
  }


  RefConst<Path> Agent::getPathToData() const {
    return _com.getPathToData();
  }


  RefConst<KString> Agent::getAlias() const {
    return _com.getAlias();
  }


  Communicator& Agent::getCommunicator() {
    return _com;
  }


  void Agent::handleOpConnect(Ref<Message> msg) {
    Ref<KString> role = msg->getPayload().AS(KString);
    const k_gur_t& guid = msg->getSender();
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

  void Agent::handlePeerConnectionRequest(RefConst<KString> role,
      const k_gur_t& guid)
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

  void Agent::handlePeerDisconnected(RefConst<KString> role,
    const k_gur_t& guid)
  {
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

    if(!_protocols.isNull()) {
      for(int i = _protocols->getSize() - 1; i >= 0; i--) {
        _protocols->at(i)->finalize();
      }
    }

    while(isAlive()) {
      System::sleep(100);
      if(nAttempts == 10) {
        ALOG_WRN << "Taking too much time to finalize." << OVER;
      }
      nAttempts++;
    }

    _com.quit();
    _isFinalized = true;
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

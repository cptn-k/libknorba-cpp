/*---[Agent.h]-------------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : knorba::Agent::*
 |  Implements: -
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

#ifndef KNORBA_AGENT
#define KNORBA_AGENT

// KFoundation
#include <kfoundation/Condition.h>
#include <kfoundation/Mutex.h>
#include <kfoundation/Logger.h>
#include <kfoundation/Thread.h>
#include <kfoundation/DictionaryDecl.h>

// Internal
#include "type/definitions.h"

#define ALOG log()
#define ALOG_ERR log(::kfoundation::Logger::ERR)
#define ALOG_WRN log(::kfoundation::Logger::WRN)
#define ADLOG(X) ALOG << X << ::kfoundation::EL
#define ADLOG_ERR(X) ALOG_ERR << X << ::kfoundation::EL
#define ADLOG_WEN(X) ALOG_WRN << X << ::kfoundation::EL

namespace kfoundation {
  template<typename T> class Array;
  template<typename T> class RefArray;
  class PrintWriter;
}

namespace knorba {
  namespace type {
    class KString;
  }
}


namespace knorba {
  
//\/ Forwards Declerations /\//////////////////////////////////////////////////

  class MessageSet;
  class Group;
  class Communicator;
  class AgentProperties;
  class Protocol;
  class Message;
  
  using namespace kfoundation;
  using namespace knorba::type;
  
  
//\/ Agent /\//////////////////////////////////////////////////////////////////
  
  /**
   * Extend this class to implement a KnoRBA agent. A KnoRBA app terminates 
   * only if every nonpassive agents (see below) call quit().
   * Make sure to override isAlive() and finalize() methods if your agent
   * creates any user threads.
   * Use Protocol class to implement behaviors shared between various types of
   * agents.
   *
   * Sending and Receiving Messages
   * ==============================
   *
   * The basic implementation of an agent involves implementing a set of
   * message handlers with
   *
   *     void MyAgent::handlerName(PPtr<Message> msg)
   *
   * signiture.
   *
   * Each handler should be explicitly registered to work. This is usually done
   * in the constructor. 
   *
   *     MyAgent::MyAgent(Runtime& rt, k_guid_t& guid)
   *     : Agent(rt, guid)
   *     {
   *         registerHandler((handler_t)&MyAgent::handlerName, OP_CODE);
   *     }
   *
   * OP_CODE shoule be a Ptr<KString>. After registered, the handler will be
   * called any time the agents receives a message with the given opcode.
   *
   * Incomming messages are queued and processed sequentially. That means,
   * first, no two handlers can manipulate the same data at the same time.
   * But if also means that if a handler takes too much time to process a
   * message, it may cause congestion and eventually overload in the message
   * queue. However, you may use blocking tsendXXX methods safely as they
   * internally assure continues execution of message thread, while waiting.
   * Use Agent::sleep() instead of System::sleep() or std::sleep().
   *
   * To communicate with other agents, use sendXXX and tsendXXX methods.
   * Because of asynchronous nature of KnoRBA, primitive send operations are
   * non-blocking. However, you have the option to block the sender agent
   * until the remote agent reponds by using tsendXXX. These methods create
   * a transaction and keep it open until all target remote agents respond.
   *
   * The receving agent can check if it is at the receiving end of a transaction
   * by invocking Message::needsResponse(), and if it is respond using
   * Agent::respond() method.
   *
   * 
   * Peer Management
   * ===============
   * 
   * Each agent can define a set of roles, and each role can be fulfilled 
   * by a set of other agents, known as peers.
   *
   * In KnoRBA peers may disappear unexpectly, or appear at any time.
   * Override handlePeerConnectionRequest() and handlePeerDisconnected() to
   * react to changes as appropriate. 
   * 
   * Use the following methods to mamange peers: addPeer(), removePeer(), 
   * removeAllPeers(), removeAllPeersWithMatchingAppId(), isPeer(), getRole(), 
   * getPeers(), and getAllPeers().
   *
   *
   * Passive Agents
   * ==============
   * 
   * The sole condition for a KnoRBA app to terminate is all agents in that app
   * to terminate. Normal agents terminate only in two ways, either by
   * calling quit() method volunteerly, or when the system is shutting down.
   * Passive agents, on the other hand, will automatically quit when all other
   * nonpassive agents quit. To define make an agent passive, call setPassive()
   * in the constructor.
   *
   * @headerfile Agent.h <knorba/Agent.h>
   */
  
  class Agent {
    
  // --- NESTED TYPES --- //

    /** Pointer to handler method */
    public:  typedef void (Agent::*handler_t)(Ref<Message>);

    private: class TransactionRecord : public KFObject {

    // --- FIELDS --- //

      public: Ref<MessageSet> responses;
      public: Condition       condition;
      public: k_integer_t     transactionId;
      public: k_integer_t     countCondition;


    // --- CONSTRUCTORS --- //

      TransactionRecord(k_integer_t id, k_integer_t countCond);


    // --- METHODS --- //

      void add(Ref<Message> msg);

    };
    
    
    private: class Connection : public KFObject {
      public: k_longint_t hashcode;
      public: RefConst<KString> role;
      public: Ref<Group> targets;
    };
    
    
    private: class MessageThread : public Thread {

    // --- FIELDS --- //

      private: Agent* _owner;
      private: k_integer_t _rank;


    // --- CONSTRUCTORS --- //

      public: MessageThread(Agent* owner, k_integer_t _rank);


    // --- METHODS --- //

      public: void run();
      public: k_integer_t getRank();

    };
    
    
    private: class FinalizerThread : public Thread {
      private: Agent* _owner;
      public: FinalizerThread(Agent* owner);
      public: void run();
    };
    

    
  // --- STATIC FIELDS --- //
    
    public: static const int DEFAULT_QUEUE_SIZE = 16;
    public: static const StaticRefConst<KString> OP_CONNECT;
    public: static const StaticRefConst<KString> OP_ACK;
    public: static const StaticRefConst<KString> OP_NG;
    
    
  // --- FIELDS --- //
    
    // Attribtues //
    private: bool          _isAutoExit;
    private: Communicator& _com;
    private: Ref< RefArray<TransactionRecord> > _openTransactions;

    // Concurrency //
    private: Ref<MessageThread> _topMessageThread;
    private: Mutex       _transactionMutex;
    private: Mutex       _queueMutex;
    private: bool        _quitFlag;
    private: bool        _isFinalized;
    private: k_integer_t _nRunningThreads;
    
    // Message Handling //
    private: Ref< Array<Protocol*> > _protocols;
    private: Ref< Dictionary<k_longint_t, handler_t> > _handlers;

    // Peers //
    private: Ref<Group> _allPeers;
    private: Ref< RefArray<Connection> > _connections;

    
  // --- (DE)CONSTRUCTORS --- //
    
    public: Agent(Communicator& rt);
    public: ~Agent();
    
    
  // --- METHODS --- //
    
    // Message handling //
    private  : void messageProcessor();
    private  : void processMessage(Ref<Message> msg);
    private  : k_integer_t startTransaction(k_integer_t tid, k_integer_t coundCond);
    private  : Ref<MessageSet> wait(k_integer_t index, k_integer_t msecs);

    // Lifecycle //
    public   : void run();
    public   : void quit();
    protected: void setPassive(bool value = true);
    public   : bool isPassive() const;
    protected: void sleep(int msecs);
    
    // Handlers and protocols//
    protected: void registerHandler(handler_t h, RefConst<KString> opcode);
    private  : handler_t getHandlerForOpcodeHash(const k_longint_t hash);
    public   : void registerProtocol(Protocol* protocol);
    public   : void unregisterProtocol(Protocol* protocol);
    
    // Peers //
    private  : k_integer_t getRoleIndexByMember(const k_gur_t& guid) const;
    private  : k_integer_t getRoleIndexByName(RefConst<KString> role) const;
    public   : void addPeer(RefConst<KString> role, const k_gur_t& guid);
    public   : void removePeer(RefConst<KString> role, const k_gur_t& guid);
    public   : void removeAllPeers(RefConst<KString> role);
    public   : void removeAllPeersWithMatchingAppId(const k_gur_t& guid);
    public   : bool isPeer(const k_gur_t& guid) const;
    public   : RefConst<KString> getRole(const k_gur_t& guid) const;
    public   : RefConst<Group> getPeers(RefConst<KString> role) const;
    public   : RefConst<Group> getAllPeers() const;
    
    // Send //
    public: void send(const k_gur_t receiver, RefConst<KString> opcode,
        Ref<KValue> content);
    
    public: void send(RefConst<Group> receivers, RefConst<KString> opcode,
        Ref<KValue> content);
    
    public: void send(RefConst<KString> role, RefConst<KString> opcode,
        Ref<KValue> content);
    
    public: void sendToAll(RefConst<KString> opcode, Ref<KValue> content);
    
    public: void sendToLocals(RefConst<KString> opcode, Ref<KValue> content);
    
    public: void respond(RefConst<Message> msg, RefConst<KString> opcode,
        Ref<KValue> content);
    
    public: Ref<Message> tsend(const k_gur_t receiver, RefConst<KString> opcode,
        Ref<KValue> content, k_integer_t timeout = -1);
    
    public: Ref<MessageSet> tsend(RefConst<Group> receivers,
        RefConst<KString> opcode, Ref<KValue> content,
        k_integer_t timeout = -1);
    
    public: Ref<MessageSet> tsend(RefConst<KString> receivers,
        RefConst<KString> opcode, Ref<KValue> content,
        k_integer_t timeout = -1);
    
    public: Ref<MessageSet> tsendToLocals(RefConst<KString> opcode,
        Ref<KValue> content, k_integer_t timeout);
    
    public: Ref<MessageSet> tsendToAll(RefConst<KString> opcode,
        Ref<KValue> content, k_integer_t timeout);
    
    // Etc. //
    public : const k_gur_t& getGur() const;
    public : Ref<PrintWriter> log(const Logger::level_t level = Logger::L3) const;
    public : RefConst<Path> getPathToResources() const;
    public : RefConst<Path> getPathToData() const;
    public : RefConst<KString> getAlias() const;
    public : Communicator& getCommunicator();
    private: void handleOpConnect(Ref<Message> msg);
    
    // Virtual methods //
    public: virtual void handlePeerConnectionRequest(RefConst<KString> role,
        const k_gur_t& guid);

    public: virtual void handlePeerDisconnected(RefConst<KString> role,
        const k_gur_t& guid);

    public: virtual bool isAlive();
    public: virtual void finalize();
    
  };

} // namespace knorba

#endif /* defined(KNORBA_AGENT) */
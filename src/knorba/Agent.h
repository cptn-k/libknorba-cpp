//
//  Agent.h
//  KnoRBA-XCode-Wrapper
//
//  Created by Hamed KHANDAN on 8/14/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#ifndef KNORBA_AGENT
#define KNORBA_AGENT

// Std
#include <map>

// KFoundation
#include <kfoundation/Array.h>
#include <kfoundation/ManagedArray.h>
#include <kfoundation/Condition.h>
#include <kfoundation/Mutex.h>
#include <kfoundation/Thread.h>

// Internal
#include "Message.h"
#include "Runtime.h"
#include "Protocol.h"
#include "type/definitions.h"

#define ALOG log()
#define ALOG_ERR log(::kfoundation::Logger::ERR)
#define ALOG_WRN log(::kfoundation::Logger::WRN)
#define ADLOG(X) ALOG << X << ::kfoundation::EL
#define ADLOG_ERR(X) ALOG_ERR << X << ::kfoundation::EL
#define ADLOG_WEN(X) ALOG_WRN << X << ::kfoundation::EL


namespace kfoundation {
  class Path;
} // kfoundation


namespace knorba {
  namespace type {
    class KString;
  }
}


namespace knorba {
  
//\/ Forwards Declerations /\//////////////////////////////////////////////////
  
  class MessageSet;
  class Group;
  class Runtime;
  class AgentProperties;
  
  using namespace std;
  using namespace kfoundation;
  using namespace knorba::type;
  
  
//\/ Agent /\//////////////////////////////////////////////////////////////////
  
  class Agent {
    
  // --- NESTED TYPES --- //
    
    public:  typedef void (Agent::*handler_t)(PPtr<Message>);
    private: typedef map<k_longint_t, handler_t> HandlerMap_t;
    
    
    private: struct TransactionRecord : public ManagedObject {
      k_integer_t     _transactionId;
      Ptr<MessageSet> _responses;
      int             _count;
      bool            _isOpen;
      int             _index;
      
      TransactionRecord();
      ~TransactionRecord();
      void add(Ptr<Message> msg);
      void reset();
    };
    
    
    private: struct HandlerRecord {
      public: Protocol* protocol;
      public: Protocol::phandler_t phandler;
      public: handler_t handler;
    };
    
    
    private: class Connection : public ManagedObject {
      public: Ptr<KString> role;
      public: Ptr<Group> targets;
    };
    
    
    private: class MessageThread : public Thread {
      private: Agent* _owner;
      private: int _rank;
      public: MessageThread(Agent* owner, int _rank);
      public: void run();
      public: int getRank();
    };
    
    
    private: class FinalizerThread : public Thread {
      private: Agent* _owner;
      public: FinalizerThread(Agent* owner);
      public: void run();
    };
    
    
    private: typedef map<k_longint_t, Connection> ConnectionMap_t;

    
  // --- STATIC FIELDS --- //
    
    public: static const int DEFAULT_QUEUE_SIZE = 16;
    public: static const SPtr<KString> OP_CONNECT;
    public: static const SPtr<KString> OP_ACK;
    public: static const SPtr<KString> OP_NG;
    
    
  // --- FIELDS --- //
    
    // Attribtues //
    private: k_guid_t _guid;
    private: bool     _isAutoExit;
    private: Runtime& _runtime;
    
    // Message Queue //
    private: Ptr< ManagedArray<Message> > _messageQueue;
    private: Ptr< Array<HandlerRecord> > _messageQueueHelper;
    private: Ptr< ManagedArray<TransactionRecord> > _openTransactions;
    private: int _queueSize;
    private: int _queueHead;
    private: int _queueTail;
    private: int _queueCount;
    
    // Concurrency //
    private: Ptr<Thread> _topMessageThread;
    private: Condition _newMessageCond;
    private: Condition _transactionCond;
    private: Mutex     _transactionMutex;
    private: Mutex     _queueMutex;
    private: bool      _quitFlag;
    private: bool      _isFinalized;
    private: int       _topThread;
    private: int       _nRunningThreads;
    
    // Message Handling //
    private: Ptr< Array<Protocol*> > _protocols;
    private: HandlerMap_t     _handlers;

    // Peers //
    private: Ptr<Group> _allPeers;
    private: Ptr< ManagedArray<Connection> > _connections;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: Agent(Runtime& rt, const k_guid_t& guid,
        int queueSize = DEFAULT_QUEUE_SIZE);
    
    public: virtual ~Agent();
    
    
  // --- METHODS --- //
    
    // Message handling //
    private  : void messageProcessor();
    private  : PPtr<TransactionRecord> startTransaction(k_integer_t tid, int count);
    private  : void wait(PPtr<TransactionRecord> trans, int msecs);
    public   : void processMessage(PPtr<Message> msg);
    
    // Lifecycle //
    public   : void run();
    public   : void quit();
    protected: void setPassive(bool value = true);
    public   : bool isPassive() const;
    protected: void sleep(int msecs);
    
    // Handlers and protocols//
    protected: void registerHandler(handler_t h, const PPtr<KString> opcode);
    private  : handler_t getHandlerForOpcodeHash(const k_longint_t hash);
    public   : void registerProtocol(Protocol* protocol);
    public   : void unregisterProtocol(Protocol* protocol);
    
    // Peers //
    private  : int  getRoleIndexByMember(const k_guid_t& guid) const;
    private  : int  getRoleIndexByName(PPtr<KString> role) const;
    public   : void addPeer(PPtr<KString> role, const k_guid_t& guid);
    public   : void removePeer(PPtr<KString> role, const k_guid_t& guid);
    public   : void removeAllPeers(PPtr<KString> role);
    public   : void removeAllPeersWithMatchingAppId(const k_guid_t& guid);
    public   : bool isPeer(const k_guid_t& guid) const;
    public   : PPtr<KString> getRole(const k_guid_t& guid) const;
    public   : PPtr<Group> getPeers(PPtr<KString> role) const;
    public   : PPtr<Group> getAllPeers() const;
    
    // Send //
    public: void send(const k_guid_t receiver, PPtr<KString> opcode,
        PPtr<KValue> content);
    
    public: void send(PPtr<Group> receivers, PPtr<KString> opcode,
        PPtr<KValue> content);
    
    public: void send(PPtr<KString> role, PPtr<KString> opcode,
        PPtr<KValue> content);
    
    public: void sendToAll(PPtr<KString> opcode, PPtr<KValue> content);
    
    public: void sendToLocals(PPtr<KString> opcode,
        PPtr<KValue> content);
    
    public: void respond(PPtr<Message> msg, PPtr<KString> opcode,
        PPtr<KValue> content);
    
    public: Ptr<Message> tsend(const k_guid_t receiver, PPtr<KString> opcode,
        PPtr<KValue> content, k_integer_t timeout = -1);
    
    public: Ptr<MessageSet> tsend(PPtr<Group> receivers, PPtr<KString> opcode,
        PPtr<KValue> content, k_integer_t timeout = -1);
    
    public: Ptr<MessageSet> tsend(PPtr<KString> receivers, PPtr<KString> opcode,
        PPtr<KValue> content, k_integer_t timeout = -1);
    
    public: Ptr<MessageSet> tsendToLocals(PPtr<KString> opcode,
        PPtr<KValue> content, k_integer_t timeout);
    
    public: Ptr<MessageSet> tsendToAll(PPtr<KString> opcode,
        PPtr<KValue> content, k_integer_t timeout);
    
    // Etc. //
    public : const k_guid_t& getGuid() const;
    public : Logger::Stream& log(const Logger::level_t level = Logger::L3) const;
    public : inline PPtr<Path> getPathToResources() const;
    public : inline PPtr<Path> getPathToData() const;
    public : inline const string& getAlias() const;
    public : Runtime& getRuntime();
    private: void handleOpConnect(PPtr<Message> msg);
    
    // Virtual methods //
    public: virtual void handlePeerConnectionRequest(PPtr<KString> role, const k_guid_t& guid);
    public: virtual void handlePeerDisconnected(PPtr<KString> role, const k_guid_t& guid);
    public: virtual bool isAlive();
    public: virtual void finalize();
    
  };
  
  /**
   * Return the path to resouces for this agent.
   */
  inline PPtr<Path> Agent::getPathToResources() const {
    return _runtime.getResourcePathForAgent(this);
  }
  
  
  /**
   * Return the alias for this agent.
   */
  inline const string& Agent::getAlias() const {
    return _runtime.getAliasForAgent(this);
  }
  
  
  /**
   * Returns the path to the folder in which this agent can store its data.
   */
  inline PPtr<Path> Agent::getPathToData() const {
    return _runtime.getDataPathForAgent(this);
  }
  
} // namespace knorba

#endif /* defined(KNORBA_AGENT) */
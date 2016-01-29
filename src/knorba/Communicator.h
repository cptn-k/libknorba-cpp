//
//  Communicator.h
//  KnoRBA
//
//  Created by Kay Khandan on 1/14/16.
//  Copyright © 2016 Kay Khandan. All rights reserved.
//

#ifndef Communicator_h
#define Communicator_h

//KFoundation
#include <kfoundation/Ref.h>

// Internal
#include "type/definitions.h"

namespace kfoundation {
  class Path;
}

namespace knorba {

  using namespace knorba::type;

  class Message;
  class Group;
  class Ontology;

  class Communicator {
    public: virtual const k_gur_t& getGur() const = 0;
    public: virtual Ref<KString> getAlias() const = 0;
    public: virtual Ref<KString> getAppName() const = 0;
    public: virtual RefConst<Path> getPathToResouces() const = 0;
    public: virtual RefConst<Path> getPathToData() const = 0;
    public: virtual Ref<Ontology> getOntology() = 0;
    public: virtual void quit() = 0;

    public: virtual void send(const k_gur_t& receiver, const k_longint_t opcode,
        RefConst<KValue> content, const k_integer_t tid) = 0;

    public: virtual void send(RefConst<Group> receivers,
        const k_longint_t opcode, RefConst<KValue> content,
        const k_integer_t tid) = 0;

    public: virtual void sendToAll(const k_longint_t opcode,
        RefConst<KValue> content, const k_integer_t tid) = 0;

    public: virtual void sendToLocals(const k_longint_t opcode,
        RefConst<KValue> content, const k_integer_t tid) = 0;

    public: virtual Ref<Message> read() = 0;
    public: virtual void release() = 0;
  };

} // namespace knorba


//int  msecsToWait = 1;
//bool exitFlag = false;
//
//while(!exitFlag) {
//  msecsToWait = 1;
//
//  while(_queueCount == 0 && !exitFlag) {
//    _newMessageCond.block(System::getCurrentTimeInMiliseconds() + msecsToWait);
//    msecsToWait = 1000;
//    exitFlag = (thisThread != _topThread) || _quitFlag;
//  }
//
//  while(_queueCount > 0) {
//    _queueMutex.lock();
//    PPtr<Message> msg = _messageQueue->at(_queueTail);
//    HandlerRecord& hr = _messageQueueHelper->at(_queueTail);
//    _messageQueue->at(_queueTail) = NULL;
//
//    ADLOG("queue(" << _queueTail << ") >> " << msg->headerToString(_runtime));
//
//    _queueTail++;
//    if(_queueTail == _queueSize) {
//      _queueTail = 0;
//    }
//    _queueCount--;
//    _queueMutex.unlock();
//
//    // BEGIN handle message
//    try {
//      if(NOT_NULL(hr.protocol)) {
//        (hr.protocol->*hr.phandler)(msg);
//      } else {
//        (this->*hr.handler)(msg);
//      }
//    } catch(KFException& e) {
//      ALOG_ERR << "Quitting because of an exception: " << e << EL;
//      msg.release();
//      quit();
//      break;
//      }
//
//      ADLOG("Message processed: " << msg->headerToString(_runtime));
//
//      msg.release();
//      // END handle message
//
//
//      exitFlag = (thisThread != _topThread) || _quitFlag;
//      }
//      } // while(!exitFlag)
//
//      ALOG << "Thread " << thisThread << " out." << EL;
//
//      _nRunningThreads--;
//      if(_nRunningThreads == 0) {
//        ADLOG("Last thread out.");
//      }

//if(_queueCount == _queueSize) {
//  ALOG_WRN << "Message queue is full." << EL;
//  int tries = 0;
//  while(_queueCount == _queueSize && tries < 100) {
//    System::sleep(10);
//    tries++;
//  }
//  ALOG_WRN << "Delayed " << tries * 10 << "ms" << EL;
//  if(tries == 100) {
//    ALOG_ERR << "Time out expired for a full queue. Quitting." << EL;
//    quit();
//  }
//  }



#endif /* Communicator_h */
/*---[Protocol.h]----------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : knorba::Protocol::*
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

#ifndef __KnoRBA__Protocol__
#define __KnoRBA__Protocol__

// KFoundation
#include <kfoundation/KFObject.h>
#include <kfoundation/Array.h>

// Internal
#include "Message.h"

#define PLOG _agent->log()
#define PLOG_ERR _agent->log(::kfoundation::Logger::ERR)
#define PLOG_WRN _agent->log(::kfoundation::Logger::WRN)

namespace kfoundation {
  template<typename KY, typename V> class Dictionary;
  class UString;
}

namespace knorba {
  
  using namespace std;
  using namespace kfoundation;
  
  class Agent;


  /**
   * Protocols are the way code reusability in KnoRBA is achieved. A protocol
   * implements a particular behavior, and all agents using that protocol will
   * inherite that behavior. In a way, protocols are realization of horizontal
   * inheritence -- in contrast with vertical inheritence in Object-Oriented
   * programming.
   *
   * Derive this class to define a custom protocol.
   *
   * Just as in Agent class, a Protocol declares a set of message handlers
   * each corresponding to a given opcode. A Protocol can also define roles
   * and reactions to additiona and removal of peers from those roles.
   *
   * Usage:
   *
   *     MyAgent::MyAgent(Runtime& rt, const k_guid_t& guid)
   *     : Agent(rt, guid),
   *       myProtocol(this)
   *     {
   *         ... constructor code ...
   *     }
   *.
   *
   * @headerfile Protocol.h <knorba/Protocol.h>
   */

  class Protocol {
    
  // --- NESTED TYPES --- //

    /** Pointer to protocol message handler */
    public: typedef void (Protocol::*phandler_t)(Ref<Message>);

    
  // --- FIELDS --- //
    
    private:   Ref< Dictionary<k_longint_t, phandler_t> > _handlerMap;
    protected: Agent* const _agent;

  
  // --- (DE)CONSTRUCTORS --- //
    
    public: Protocol(Agent* _owner);
    public: ~Protocol();
    
    
  // --- METHODS --- //
  
    protected: void registerHandler(phandler_t handler, RefConst<KString> opcode);
    public: phandler_t getHandlerForOpcodeHash(const k_longint_t hash);
    public: virtual void handlePeerConnectionReuqest(RefConst<KString> role, const k_gur_t& guid);
    public: virtual void handlePeerDisconnected(RefConst<KString> role, const k_gur_t& guid);
    public: virtual void finalize();
    public: virtual bool isAlive() const;
    
  };
  
} // namespace knorba

#endif /* defined(__KnoRBA__Protocol__) */
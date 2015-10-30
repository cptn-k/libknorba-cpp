/*---[Protocol.cpp]--------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : -
 |  Implements: knorba::Protocol::*
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

// KFoundation
#include <kfoundation/Ptr.h>
#include <kfoundation/OutputStream.h>
#include <kfoundation/ObjectStreamReader.h>

// Internal
#include "Agent.h"
#include "type/KString.h"

// Self
#include "Protocol.h"

namespace knorba {
  
// --- (DE)CONSTRUCTORS --- //

  /**
   * Constructor.
   */

  Protocol::Protocol(Agent* owner)
  : _agent(owner)
  {
    _agent->registerProtocol(this);
  }


  /**
   * Deconstructor.
   */
  
  Protocol::~Protocol() {
    _agent->unregisterProtocol(this);
  }
  
  
// --- METHODS --- //

  /**
   * Registers a handler for the given opcode.
   *
   * @param handler Pointer to handler method
   * @param opcode The opcode that activates the given handler
   */

  void Protocol::registerHandler(phandler_t handler, PPtr<KString> opcode) {
    k_longint_t hash = opcode->getHashCode();
    _handlerMap[hash] = handler;
  }
  
  
  Protocol::phandler_t Protocol::getHandlerForOpcodeHash(const k_longint_t hash)
  {
    map_t::iterator it = _handlerMap.find(hash);
    
    if(it == _handlerMap.end()) {
      return NULL;
    }
    
    return it->second;
  }
  
  
  /** 
   * @fn Protocol::handlePeerConnectionReuqest
   * Override to handle peer connection request.
   *
   * @param role The request role for the new peer.
   * @param guid The GUID of the agent requesting to become a peer.
   * @see handlePeerDisconnected()
   * @see Agent::handlePeerConnectionRequest()
   */

  void Protocol::handlePeerConnectionReuqest(PPtr<KString> role,
      const k_guid_t& guid)
  {

  }


  /**
   * Override to handle peer disconnect notifications.
   *
   * @param role The role of the peer to be removed.
   * @param guid The GUID of the agent requesting to be removed as peer.
   * @see handlePeerConnectionReuqest()
   * @see Agent::handlePeerDisconnected()
   */

  void Protocol::handlePeerDisconnected(PPtr<KString> role, const k_guid_t& guid) {
    // Nothing;
  }
  
  
  /**
   * Override to perform additional tasks when agent using this protocol is
   * finalizing. Stops the message processor thread.
   * 
   * @see isAlive()
   * @see Agent::finalize()
   */

  void Protocol::finalize() {
    // Nothing;
  }


  /**
   * Override if there are
   * additional criteria to determine this agent is alive. E.g. other threads
   * are running, connections are open, etc.
   *
   * @see finalize()
   * @see Agent::isAlive()
   */

  bool Protocol::isAlive() const {
    return false;
  }
  
} // namespace knorba
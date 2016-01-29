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
#include <kfoundation/Ref.h>
#include <kfoundation/Dictionary.h>
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

  void Protocol::registerHandler(phandler_t handler, RefConst<KString> opcode) {
    k_longint_t hash = opcode->getHashCode();
    _handlerMap->at(hash) = handler;
  }
  
  
  Protocol::phandler_t Protocol::getHandlerForOpcodeHash(const k_longint_t hash)
  {
    #warning Performance leak here. Dictionary is searched twice.
    if(_handlerMap->containsKey(hash)) {
      return _handlerMap->at(hash);
    } else {
      return NULL;
    }
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

  void Protocol::handlePeerConnectionReuqest(RefConst<KString> role,
      const k_gur_t& guid)
  {
    // Nothing
  }


  /**
   * Override to handle peer disconnect notifications.
   *
   * @param role The role of the peer to be removed.
   * @param guid The GUID of the agent requesting to be removed as peer.
   * @see handlePeerConnectionReuqest()
   * @see Agent::handlePeerDisconnected()
   */

  void Protocol::handlePeerDisconnected(RefConst<KString> role,
      const k_gur_t& guid)
  {
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
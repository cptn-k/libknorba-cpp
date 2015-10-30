/*---[Message.cpp]---------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : -
 |  Implements: knorba::Message::*
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
#include <kfoundation/Logger.h>
#include <kfoundation/SerializingStreamer.h>

#include "type/KInteger.h"
#include "type/KLongint.h"
#include "type/KRecord.h"
#include "type/KString.h"
#include "Runtime.h"

// Internal
#include "type/KGuid.h"

// Self
#include "Message.h"

namespace knorba {
  
// --- (DE)CONSTRUCTORS --- //

  Message::Message(const kf_octet_t manager, const int index)
  : PoolObject(manager, index)
  {
    // Nothing;
  }
  
  
  
// --- METHODS --- //

  /**
   * Setter. Replaces all values of this object with the ones given in
   * arguments.
   *
   * @param tid Transaction ID.
   * @param opcodeHash Opcode Hash (64-bit CityHash).
   * @param sender GUID of the sender agent.
   * @param payload Payload.
   */

  void Message::set(const k_integer_t tid, const k_longint_t opcodeHash,
      const k_guid_t &sender, PPtr<KValue> payload)
  {
    _transactionId = tid;
    _opcodeHash = opcodeHash;
    _sender = sender;
    _payload = payload;
  }


  /** Returns transaction ID. */
  
  k_integer_t Message::getTransactionId() const {
    return _transactionId;
  }


  /** Returns Opcode Hash */
  
  k_longint_t Message::getOpcodeHash() const {
    return _opcodeHash;
  }


  /** Returns the sender's GUID */
  
  const k_guid_t& Message::getSender() const {
    return _sender;
  }
  

  /** Returns the message payload */

  PPtr<KValue> Message::getPayload() const {
    return _payload;
  }


  /** Checks if the opcode of this message matches the given string */
  
  bool Message::is(PPtr<KString> opcode) const {
    return opcode->getHashCode() == _opcodeHash;
  }


  /** 
   * Checks if this message is blocking the sender for a response. If returns
   * true, use Agent::respond() to respond and use this message as the first
   * argument.
   */
  
  bool Message::needsResponse() const {
    return _transactionId >= 0;
  }
  

  /**
   * Converts header information to string.
   */

  string Message::headerToString(Runtime &rt) const {
    PPtr<KString> opcode = rt.getMessageOpCodeForHash(_opcodeHash);
    string opcodeStr = "unknown";
    if(!opcode.isNull()) {
      opcodeStr = opcode->toUtf8String();
    }
    
    stringstream sstream;
    PPtr<ObjectSerializer> os = new ObjectSerializer(sstream, ObjectSerializer::DUMP);
    os->object("Message")
      ->attribute("opcode", opcodeStr)
      ->attribute("sender", KGuid::toString(_sender))
      ->attribute("transactionId", _transactionId)
      ->attribute("payloadType",_payload->getType()->getTypeName())
      ->endObject();
    return sstream.str();    
  }
  
  
  void Message::serialize(PPtr<ObjectSerializer> os) const {
    os->object("Message")
      ->attribute("sender", KGuid::toString(_sender))
      ->attribute("opcodeHash", _opcodeHash)
      ->attribute("transactionId", _transactionId)
      ->member("payload")->object<KValue>(_payload)
      ->endObject();
  }
  
  
  // Inherited from PoolObject //
  
  void Message::finalize() {
    if(!_payload.isNull()) {
      _payload = NULL;
    }
  }
  
} // namespace knorba

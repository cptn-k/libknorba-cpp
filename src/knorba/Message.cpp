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
#include <kfoundation/BufferOutputStream.h>
#include <kfoundation/KforObjectSerializer.h>
#include <kfoundation/UString.h>

// Internal
#include "Ontology.h"
#include "type/KType.h"
#include "type/KValue.h"
#include "type/KString.h"
#include "type/KGur.h"

// Self
#include "Message.h"

namespace knorba {
  
// --- (DE)CONSTRUCTORS --- //

  Message::Message() {
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
      const k_gur_t &sender, Ref<KValue> payload)
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
  
  const k_gur_t& Message::getSender() const {
    return _sender;
  }
  

  /** Returns the message payload */

  Ref<KValue> Message::getPayload() const {
    return _payload;
  }


  /** Checks if the opcode of this message matches the given string */
  
  bool Message::is(Ref<KString> opcode) const {
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

  Ref<UString> Message::headerToString(RefConst<Ontology> ontology) const {
    RefConst<KString> opcode = ontology->resolveHash(_opcodeHash);

    if(opcode.isNull()) {
      opcode = new KString("unknown");
    }
    
    Ref<BufferOutputStream> stream = new BufferOutputStream();

    Ref<ObjectSerializer> os = new KforObjectSerializer(stream.AS(OutputStream));

    os->object(K"Message")
      ->attribute(K"opcode", *opcode)
      ->attribute(K"sender", KGur(_sender))
      ->attribute(K"transactionId", _transactionId)
      ->attribute(K"payloadType", *_payload->getType()->getTypeName())
      ->endObject();

    return stream->getString();
  }
  
  
  void Message::serialize(Ref<ObjectSerializer> stream) const {
    stream->object(K"Message")
      ->attribute(K"sender", KGur(_sender))
      ->attribute(K"opcodeHash", _opcodeHash)
      ->attribute(K"transactionId", _transactionId)
      ->member(K"payload")->object(*_payload)
      ->endObject();
  }

} // namespace knorba
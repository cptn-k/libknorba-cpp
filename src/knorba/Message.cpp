//
//  Message.cpp
//  KnoRBA
//
//  Created by Hamed KHANDAN on 9/19/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

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
#include "type/KGlobalUid.h"

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
  
  void Message::set(const k_integer_t tid, const k_longint_t opcodeHash,
      const k_guid_t &sender, PPtr<KValue> payload)
  {
    _transactionId = tid;
    _opcodeHash = opcodeHash;
    _sender = sender;
    _payload = payload;
  }
  
  
  k_integer_t Message::getTransactionId() const {
    return _transactionId;
  }
  
  
  k_longint_t Message::getOpcodeHash() const {
    return _opcodeHash;
  }
  
  
  const k_guid_t& Message::getSender() const {
    return _sender;
  }
  
  
  PPtr<KValue> Message::getPayload() const {
    return _payload;
  }
  
  
  bool Message::is(PPtr<KString> opcode) const {
    return opcode->getHashCode() == _opcodeHash;
  }
  
  
  bool Message::needsResponse() const {
    return _transactionId >= 0;
  }
  
  
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
      ->attribute("sender", KGlobalUid::toString(_sender))
      ->attribute("transactionId", _transactionId)
      ->attribute("payloadType",_payload->getType()->getTypeName())
      ->endObject();
    return sstream.str();    
  }
  
  
  void Message::serialize(PPtr<ObjectSerializer> os) const {
    os->object("Message")
      ->attribute("sender", KGlobalUid::toString(_sender))
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

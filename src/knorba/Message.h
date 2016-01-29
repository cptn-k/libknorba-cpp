/*---[Message.h]-----------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : knorba::Message::*
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

#ifndef __KnoRBA__Message__
#define __KnoRBA__Message__

#include <kfoundation/KFObject.h>
#include <kfoundation/Ref.h>
#include <kfoundation/SerializingStreamer.h>

#include "type/definitions.h"

namespace kfoundation {
  class SerializingStreamer;
}

namespace knorba {
  
  using namespace type;
  using namespace kfoundation;
  
  class Ontology;


  /**
   * Represents a KnoRBA message.
   *
   * @note KnoRBA messages carry opcode hash instead of opcode itself. 
   *       If ever needed, use is() method to check the opcode.
   * @note This is a pool-allocated object and should not be instanitated
   *       directly.
   *
   * @headerfile Message.h <knorba/Message.h>
   */

  class Message : public KFObject, public SerializingStreamer {
    
  // --- FIELDS --- //
    
    private: k_integer_t _transactionId;
    private: k_longint_t _opcodeHash;
    private: k_gur_t    _sender;
    private: Ref<KValue> _payload;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: Message();
    
    
  // --- METHODS --- //
    
    public: void set(const k_integer_t tid, const k_longint_t opcodeHash,
        const k_gur_t& sender, Ref<KValue> payload);
    
    public: k_integer_t getTransactionId() const;
    public: k_longint_t getOpcodeHash() const;
    public: const k_gur_t& getSender() const;
    public: Ref<KValue> getPayload() const;
    public: bool is(Ref<KString> opcode) const;
    public: bool needsResponse() const;
    public: Ref<UString> headerToString(RefConst<Ontology> dic) const;

    // Inherited from SerializingStreamer //
    public: void serialize(Ref<ObjectSerializer> stream) const;
    
  };
  
} // namespace knorba

#endif /* defined(__KnoRBA__Message__) */
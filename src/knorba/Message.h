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

#include "type/KType.h"

namespace kfoundation {
  class SerializingStreamer;
}

namespace knorba {
  
  namespace type {
    class KValue;
    class KString;
  }
  
  using namespace type;
  using namespace kfoundation;
  
  class Runtime;


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

  class Message : public PoolObject, public SerializingStreamer {
    
  // --- FIELDS --- //
    
    private: k_integer_t _transactionId;
    private: k_longint_t _opcodeHash;
    private: k_guid_t    _sender;
    private: Ptr<KValue> _payload;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: Message(const kf_octet_t manager, const int index);
    
    
  // --- METHODS --- //
    
    public: void set(const k_integer_t tid, const k_longint_t opcodeHash,
        const k_guid_t& sender, PPtr<KValue> _payload);
    
    public: k_integer_t getTransactionId() const;
    public: k_longint_t getOpcodeHash() const;
    public: const k_guid_t& getSender() const;
    public: PPtr<KValue> getPayload() const;
    public: bool is(PPtr<KString> opcode) const;
    public: bool needsResponse() const;
    public: string headerToString(Runtime& rt) const;
    
    // Inherited from PoolObject //
    public: void finalize();
    
    // Inherited from SerializingStreamer //
    public: void serialize(PPtr<ObjectSerializer> os) const;
    
  };
  
} // namespace knorba

#endif /* defined(__KnoRBA__Message__) */

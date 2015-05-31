//
//  Message.h
//  KnoRBA
//
//  Created by Hamed KHANDAN on 9/19/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

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
  
  class Message : public PoolObject, public SerializingStreamer {
    
  // --- FIELDS --- //
    
    private: k_integer_t _transactionId;
    private: k_longint_t _opcodeHash;
    private: k_guid_t       _sender;
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

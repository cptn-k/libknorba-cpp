/*---[Runtime.h]-----------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : knorba::Runtime::*
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

#ifndef __KnoRBA__Runtime__
#define __KnoRBA__Runtime__

#include "type/definitions.h"

namespace knorba {
  
  namespace type {
    class KString;
    class KValue;
    class KType;
  }
  
  class Agent;
  class Group;
  
  using namespace type;
  using namespace kfoundation;


  /**
   * ARE access interface.
   *
   * @headerfile Runtime.h <knorba/Runtime.h>
   */

  class Runtime {

    /** Returns the GUID of ARE. */
    public: virtual const k_guid_t& getGuid() const = 0;


    /** 
     * Adds a new type to the ARE's type table. 
     * @param type The type to register.
     */

    public: virtual void registerType(PPtr<KType> type) = 0;


    /**
     * Find a registered type by its hashcode.
     * @param hash Hashcode for a type name (64-bit CityHash)
     * @return The type with the given name hash, or `null` if such type does
     *         not exist.
     */

    public: virtual PPtr<KType> getTypeByHash(const k_longint_t hash) const = 0;


    /**
     * Returns the GUID of the default ConsoleAgent assigned to this ARE.
     */

    public: virtual const k_guid_t& getConsoleGuid() const = 0;


    /**
     * Returns the name the current application. This must be the same as the
     * name of corresponding KAR or KAP file.
     */

    public: virtual const string& getAppName() const = 0;


    /**
     * Returns the number of nodes of the cluster this ARE is a part of.
     */

    public: virtual k_integer_t getNodeCount() const = 0;


    /**
     * Returns true if this ARE is the head of its cluster.
     */

    public: virtual bool isHead() const = 0;

    public: virtual void signalQuit() = 0;

    public: virtual PPtr<Path> getResourcePathForAgent(const Agent* itself)
        const = 0;
    
    public: virtual PPtr<Path> getDataPathForAgent(const Agent* itself)
        const = 0;
    
    public: virtual const string& getClassNameForAgent(const Agent* itself)
        const = 0;
    
    public: virtual const string& getAliasForAgent(const Agent* itself)
        const = 0;
    
    public: virtual const k_guid_t& getAgentGuidByAlias(const string& alias)
        const = 0;

    /**
     * Registers a new record in ARE's message type table.
     * @param opcode The opcode for the message type.
     * @param payloadType The type associated with the given opcode.
     */

    public: virtual void registerMessageFormat(PPtr<KString> opcode,
        PPtr<KType> payloadType) = 0;


    /**
     * Returns the message type for the given opcode hash.
     * @param hash The hash code for the opcode to be looked up
     *             (64-bit CityHash).
     * @return The type associated with the given code, or null if such type
     *         does not exist.
     */

    public: virtual PPtr<KType> getMessageFormatByHash(const k_longint_t hash)
        const = 0;
    
    /**
     * Returns the opcode for the given opcode hash.
     * @param hash The hash code for the opcode to be looked up
     *             (64-bit CityHash).
     * @return The opcode associated with the given hash, or null if such opcode
     *         does not exist.
     */

    public: virtual PPtr<KString> getMessageOpCodeForHash(
        const k_longint_t hash) const = 0;
      
    public: virtual void send(const k_guid_t& sender, const k_guid_t& receiver,
        const k_longint_t opcode, PPtr<KValue> content,
        const k_integer_t tid) = 0;
      
    public: virtual void send(const k_guid_t& sender, PPtr<Group> receivers,
        const k_longint_t opcode, PPtr<KValue> content,
        const k_integer_t tid) = 0;
      
    public: virtual void sendToAll(const k_guid_t& sender,
        const k_longint_t opcode, PPtr<KValue> content,
        const k_integer_t tid) = 0;
      
    public: virtual void sendToLocals(const k_guid_t& sender,
        const k_longint_t opcode, PPtr<KValue> content,
        const k_integer_t tid) = 0;
    
  };
  
} // namespace knorba

#endif /* defined(__KnoRBA__Runtime__) */

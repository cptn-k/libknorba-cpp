//
//  ConsoleProtocolClient.h
//  AgentRuntimeEnvironment
//
//  Created by Hamed KHANDAN on 4/24/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

#ifndef __AgentRuntimeEnvironment__ConsoleProtocolClient__
#define __AgentRuntimeEnvironment__ConsoleProtocolClient__

#include <knorba/Protocol.h>
#include <knorba/type/all.h>


namespace knorba {
namespace protocol {
    
  using namespace knorba;
  using namespace knorba::type;

  class ConsoleProtocolClient : public Protocol {
    
  // --- STATIC FIELDS --- //
    
    private: static SPtr<KRecordType> RELAY_PATH_T;
    public: static const SPtr<KString> OP_PRINT;
    public: static const SPtr<KString> OP_INPUT;
    public: static const SPtr<KString> OP_START;
    public: static const SPtr<KString> OP_STOP;
    public: static const SPtr<KString> OP_SUBSCRIBE;
    public: static const SPtr<KString> OP_UNSUBSCRIBE;
    public: static const SPtr<KString> OP_ADD_RELAY_PATH;
    public: static const SPtr<KString> OP_REMOVE_RELAY_PATH;
    public: static const SPtr<KString> R_CONSOLE;
    
    
  // --- STATIC METHODS --- //
    
    public: static void init(Runtime& rt);
    public: static SPtr<KRecordType> relay_path_t();
    
    
  // --- FIELDS --- //
    
    private: k_guid_t _server;
    private: bool _noServer;
    private: string _intro;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: ConsoleProtocolClient(Agent* owner);
    
    
  // --- METHODS --- //
    
    private: void handleOpInput(PPtr<Message> msg);
    public: void setServer(const k_guid_t& guid);
    public: const k_guid_t& getServer() const;
    public: void print(const string& text);
    public: void print(const Streamer& streamer);
    public: void print(PPtr<KString> text);
    public: void subscribe();
    public: void unsubscribe();
    public: void addRelayPath(const k_guid_t& vrt, const k_guid_t& console);
    public: void removeRelayPath(const k_guid_t& vrt);
    public: void start();
    public: void stop();
    public: virtual void onInputReceived(PPtr<KString> input);
    
    // Inherited from Protocol //
    public: void handlePeerConnectionReuqest(PPtr<KString> role, const k_guid_t& guid);
    
  };
  
} // namespace protocol
} // namespace knorba

#endif /* defined(__AgentRuntimeEnvironment__ConsoleProtocolClient__) */
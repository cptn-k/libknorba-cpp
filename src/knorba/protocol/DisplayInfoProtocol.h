//
//  DisplayInfoProtocol.h
//  AgentRuntimeEnvironment
//
//  Created by Hamed KHANDAN on 5/21/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

#ifndef __AgentRuntimeEnvironment__DisplayInfoProtocol__
#define __AgentRuntimeEnvironment__DisplayInfoProtocol__

#include <knorba/Protocol.h>

namespace knorba {
namespace protocol {
  
  class DisplayInfoProtocol : public Protocol {
    
  // --- NESTED TYPES --- //
    
    public: class KTuple2D : public Tuple2D {
      private: static SPtr<KRecordType> TYPE;
      
      public: static const k_octet_t X = 0;
      public: static const k_octet_t Y = 1;
      
      public: static SPtr<KRecordType> type();
      
      public: KTuple2D();
      public: KTuple2D(PPtr<KRecord> r);
      
      public: Ptr<KRecord> toKRecord() const;
      public: void toKRecord(PPtr<KRecord> r) const;
    };
    
    
    public: class Display : public SerializingStreamer {
      private: static SPtr<KRecordType> TYPE;
      
      public: static const k_octet_t INDEX = 0;
      public: static const k_octet_t SIZE = 1;
      
      public: static SPtr<KRecordType> type();
      
      private: KTuple2D _index;
      private: KTuple2D _size;
      
      public: Display();
      public: Display(PPtr<KRecord> r);
      
      public: Range getRect() const;
      public: const KTuple2D& getIndex() const;
      public: const KTuple2D& getSize() const;
      public: void toKRecord(PPtr<KRecord> r) const;
      public: void serialize(PPtr<ObjectSerializer> s) const;
    };

  
    public: class NodeInfo : public KFObject, public SerializingStreamer {
      private: static SPtr<KGridType> TYPE;
      public: static SPtr<KGridType> type();
      
      private: k_guid_t _guid;
      private: Range _rect;
      private: Range _indexRange;
      private: Ptr< Array<Display> > _displays;
      
      public: NodeInfo(const k_guid_t& guid);
      public: NodeInfo(const k_guid_t& guid, PPtr<KGrid> displayList);
      
      public: void addDisplay(const Display& d);
      public: const k_guid_t& getGuid() const;
      public: const Range& getRect() const;
      public: const Range& getIndexRange() const;
      public: int getNDisplays() const;
      public: const Display& getDisplayAtIndex(int index) const;
      public: void toKGrid(PPtr<KGrid> g) const;
      public: void serialize(PPtr<ObjectSerializer> s) const;
    };
    
    
  // --- STATIC FIELDS --- //
    
    private: static SPtr<KRecordType> SETUP_T;
    public: static const k_octet_t SETUP_T_ORIGIN = 0;
    public: static const k_octet_t SETUP_T_DISPLAYS = 1;
    
    public: static const SPtr<KString> OP_SETUP_Q;
    public: static const SPtr<KString> OP_SETUP_A;
    public: static const SPtr<KString> OP_DISPLAYS_Q;
    public: static const SPtr<KString> OP_DISPLAYS_A;
    
    
  // --- STATIC METHODS --- //
    
    public: static SPtr<KRecordType> setup_t();
    public: static void init(Runtime& rt);

    
  // --- FIELDS --- //
    
    private: KTuple2D _windowOrigin;
    private: Ptr<NodeInfo> _localInfo;
    private: Ptr< ManagedArray<NodeInfo> > _peerInfo;
    private: Ptr<KString> _role;
    private: Range _globalRect;
    private: Range _globalIndexRange;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: DisplayInfoProtocol(Agent* agent);
    public: DisplayInfoProtocol(Agent* agent, PPtr<KString> role);
    
    
  // --- METHODS --- //
    
    public: bool queryLocalDisplays();
    public: bool queryPeerDisplays();
    public: bool hasDisplay();
    public: bool isFullScreen();
    public: const KTuple2D& getWindowOrigin() const;
    public: PPtr<NodeInfo> getLocalInfo() const;
    public: PPtr< ManagedArray<NodeInfo> > getRemoteInfo() const;
    public: PPtr<NodeInfo> getInfoForRemoteAgent(const k_guid_t& guid) const;
    public: const Range& getGlobalRect() const;
    public: const Range& getGlobalIndexRange() const;
    
    // Handlers //
    public: void handleOpDisplayQ(PPtr<Message> msg);
    
    // Virtual Methods //
    public: virtual void onAllPeersJoined();
    
  };
  
} // namespace protocol
} // namespace knorba

#endif /* defined(__AgentRuntimeEnvironment__DisplayInfoProtocol__) */

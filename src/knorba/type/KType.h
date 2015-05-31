//
//  KType.h
//  CellMonitorTest-XCodeWrapper
//
//  Created by Hamed KHANDAN on 7/18/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#ifndef KNORBA_TYPE_KTYPE
#define KNORBA_TYPE_KTYPE

// Std
#include <string>

// Internal
#include "definitions.h"

// Super
#include <kfoundation/ManagedObject.h>
#include <kfoundation/Streamer.h>

namespace knorba {
namespace type {

  using namespace std;
  using namespace kfoundation;
  
  
//\/ Forward Declerations /\///////////////////////////////////////////////////
  
  class KValue;
  
  
//\/ KType /\//////////////////////////////////////////////////////////////////

  class KType : public ManagedObject, public Streamer {
    
  // --- STATIC FIELDS --- //
    
    public: static const SPtr<KType> TRUTH;
    public: static const SPtr<KType> OCTET;
    public: static const SPtr<KType> INTEGER;
    public: static const SPtr<KType> LONGINT;
    public: static const SPtr<KType> REAL;
    public: static const SPtr<KType> GUID;
    public: static const SPtr<KType> STRING;
    public: static const SPtr<KType> RAW;
    public: static const SPtr<KType> ANY;
    public: static const SPtr<KType> NOTHING;
    
    
  // --- FIELDS --- //
    
    private: string _typeName;
    private: k_longint_t _nameHash;
    
    
  // --- CONSTRUCTOR --- //
    
    protected: KType(string name);
    
    
  // --- METHODS --- //
    
    public: const string& getTypeName() const;
    public: k_longint_t getTypeNameHash() const;
    
    public: virtual bool isCastableTo(PPtr<KType> t) const = 0;
    public: virtual bool isAutomaticCastableTo(PPtr<KType> t) const = 0;
    public: virtual bool equals(PPtr<KType> t) const;
    public: virtual int  getSizeInOctets() const = 0;
    public: virtual bool isPrimitive() const = 0;
    public: virtual bool hasConstantSize() const = 0;
    public: virtual Ptr<KValue> instantiate() const = 0;
    public: virtual string toKnois() const;
    
    // From Streamer
    void printToStream(ostream& os) const;
  };

} // namespace type
} // namespace knorba

#endif /* defined(KNORBA_TYPE_KTYPE) */

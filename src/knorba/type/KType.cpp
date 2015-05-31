//
//  KType.cpp
//  CellMonitorTest-XCodeWrapper
//
//  Created by Hamed KHANDAN on 7/18/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

// KFoundation
#include <kfoundation/Ptr.h>
#include <kfoundation/System.h>

// Internal
#include "KValue.h"
#include "KTruth.h"
#include "KOctet.h"
#include "KInteger.h"
#include "KLongint.h"
#include "KReal.h"
#include "KGlobalUid.h"
#include "KString.h"
#include "KRaw.h"
#include "KAny.h"

// Self
#include "KType.h"

namespace knorba {
namespace type {
  

//\/ KTruthType /\/////////////////////////////////////////////////////////////
    
    class KTruthType : public KType {
    public:
      KTruthType();
      bool isCastableTo(PPtr<KType> t) const;
      bool isAutomaticCastableTo(PPtr<KType> t) const;
      int  getSizeInOctets() const;
      bool isPrimitive() const;
      bool hasConstantSize() const;
      Ptr<KValue> instantiate() const;
    };
    
    
    KTruthType::KTruthType() : KType("truth") {
      // Nothing;
    }
    
    
    bool KTruthType::isCastableTo(PPtr<KType> t) const {
      return t->equals(KType::TRUTH.AS(KType));
    }
    
    
    bool KTruthType::isAutomaticCastableTo(PPtr<KType> t) const {
      return t->equals(KType::TRUTH.AS(KType));
    }
    
    
    inline int KTruthType::getSizeInOctets() const {
      return 1;
    }
    
    
    inline bool KTruthType::isPrimitive() const {
      return true;
    }
    
    
    inline bool KTruthType::hasConstantSize() const {
      return true;
    }
    
    
    Ptr<KValue> KTruthType::instantiate() const {
      return new KTruth();
    }
  

//\/ KOctetType /\/////////////////////////////////////////////////////////////
    
    class KOctetType : public KType {
    public:
      KOctetType();
      bool isCastableTo(PPtr<KType> t) const;
      bool isAutomaticCastableTo(PPtr<KType> t) const;
      int  getSizeInOctets() const;
      bool isPrimitive() const;
      bool hasConstantSize() const;
      Ptr<KValue> instantiate() const;
    };
    
    
    KOctetType::KOctetType() : KType("octet") {
      // Nothing;
    }
    
    
    bool KOctetType::isCastableTo(PPtr<KType> t) const {
      if(t->equals(KType::OCTET.AS(KType)))
        return true;
      
      if(t->equals(KType::INTEGER.AS(KType)))
        return true;
      
      if(t->equals(KType::LONGINT.AS(KType)))
        return true;
      
      if(t->equals(KType::REAL.AS(KType)))
        return true;
      
      return false;
    }
    
    
    bool KOctetType::isAutomaticCastableTo(PPtr<KType> t) const {
      if(t->equals(KType::OCTET.AS(KType)))
        return true;
      
      if(t->equals(KType::INTEGER.AS(KType)))
        return true;
      
      if(t->equals(KType::LONGINT.AS(KType)))
        return true;
      
      if(t->equals(KType::REAL.AS(KType)))
        return true;
      
      return false;
    }
    
    
    inline int KOctetType::getSizeInOctets() const {
      return 1;
    }
    
    
    inline bool KOctetType::isPrimitive() const {
      return true;
    }
    
    
    inline bool KOctetType::hasConstantSize() const {
      return true;
    }
    
    
    Ptr<KValue> KOctetType::instantiate() const {
      throw new KFException("Octet type is not implemented yet.");
    }


//\/ KIntegerType /\///////////////////////////////////////////////////////////

    class KIntegerType : public KType {
    public:
      KIntegerType();
      bool isCastableTo(PPtr<KType> t) const;
      bool isAutomaticCastableTo(PPtr<KType> t) const;
      int  getSizeInOctets() const;
      bool isPrimitive() const;
      bool hasConstantSize() const;
      Ptr<KValue> instantiate() const;
    };

    
    KIntegerType::KIntegerType() : KType("integer") {
      // Nothing;
    }
    
    
    bool KIntegerType::isCastableTo(PPtr<KType> t) const {
      if(t->equals(KType::OCTET.AS(KType)))
        return true;
      
      if(t->equals(KType::INTEGER.AS(KType)))
        return true;
      
      if(t->equals(KType::LONGINT.AS(KType)))
        return true;
      
      if(t->equals(KType::REAL.AS(KType)))
        return true;
      
      return false;
    }
    
    
    bool KIntegerType::isAutomaticCastableTo(PPtr<KType> t) const {
      if(t->equals(KType::INTEGER.AS(KType)))
        return true;
      
      if(t->equals(KType::LONGINT.AS(KType)))
        return true;
      
      if(t->equals(KType::REAL.AS(KType)))
        return true;
      
      return false;
    }
    
    
    inline int KIntegerType::getSizeInOctets() const {
      return 4;
    }
    
    
    inline bool KIntegerType::isPrimitive() const {
      return true;
    }
    
    
    inline bool KIntegerType::hasConstantSize() const {
      return true;
    }
    
    
    Ptr<KValue> KIntegerType::instantiate() const {
      return new KInteger();
    }
  
    
//\/ KLongintType /\///////////////////////////////////////////////////////////
    
    class KLongintType : public KType {
    public:
      KLongintType();
      bool isCastableTo(PPtr<KType> t) const;
      bool isAutomaticCastableTo(PPtr<KType> t) const;
      int  getSizeInOctets() const;
      bool isPrimitive() const;
      bool hasConstantSize() const;
      Ptr<KValue> instantiate() const;
    };

    
    KLongintType::KLongintType() : KType("longint") {
      // Nothing;
    }
    
    
    bool KLongintType::isCastableTo(PPtr<KType> t) const {
      if(t->equals(KType::OCTET.AS(KType)))
        return true;
      
      if(t->equals(KType::INTEGER.AS(KType)))
        return true;
      
      if(t->equals(KType::LONGINT.AS(KType)))
        return true;
      
      if(t->equals(KType::REAL.AS(KType)))
        return true;
      
      return false;
    }
    
    
    bool KLongintType::isAutomaticCastableTo(PPtr<KType> t) const {
      if(t->equals(KType::LONGINT.AS(KType)))
        return true;
      
      if(t->equals(KType::REAL.AS(KType)))
        return true;
      
      return false;
    }
    
    
    inline int KLongintType::getSizeInOctets() const {
      return 8;
    }
    
    
    inline bool KLongintType::isPrimitive() const {
      return true;
    }
    
    
    inline bool KLongintType::hasConstantSize() const {
      return true;
    }
    
    
    Ptr<KValue> KLongintType::instantiate() const {
      return new KLongint();
    }
  
    
//\/ KRealType /\//////////////////////////////////////////////////////////////
    
    class KRealType : public KType {
    public:
      KRealType();
      bool isCastableTo(PPtr<KType> t) const;
      bool isAutomaticCastableTo(PPtr<KType> t) const;
      int  getSizeInOctets() const;
      bool isPrimitive() const;
      bool hasConstantSize() const;
      Ptr<KValue> instantiate() const;
    };
    
    
    KRealType::KRealType() : KType("real") {
      // Nothing;
    }
    
    
    bool KRealType::isCastableTo(PPtr<KType> t) const {
      if(t->equals(KType::OCTET.AS(KType)))
        return true;
      
      if(t->equals(KType::INTEGER.AS(KType)))
        return true;
      
      if(t->equals(KType::LONGINT.AS(KType)))
        return true;
      
      if(t->equals(KType::REAL.AS(KType)))
        return true;
      
      return false;
    }
    
    
    bool KRealType::isAutomaticCastableTo(PPtr<KType> t) const {
      return t->equals(KType::REAL.AS(KType));
    }
    
    
    inline int KRealType::getSizeInOctets() const {
      return 8;
    }
    
    
    inline bool KRealType::isPrimitive() const {
      return true;
    }
    
    
    inline bool KRealType::hasConstantSize() const {
      return true;
    }
    
    
    Ptr<KValue> KRealType::instantiate() const {
      return new KReal();
    }
  

//\/ KGUIDType /\//////////////////////////////////////////////////////////////

    class KGUIDType : public KType {
    public:
      KGUIDType();
      bool isCastableTo(PPtr<KType> t) const;
      bool isAutomaticCastableTo(PPtr<KType> t) const;
      int  getSizeInOctets() const;
      bool isPrimitive() const;
      bool hasConstantSize() const;
      Ptr<KValue> instantiate() const;
    };

    
    KGUIDType::KGUIDType() : KType("guid") {
      // Nothing;
    }
    
    
    bool KGUIDType::isCastableTo(PPtr<KType> t) const {
      return t->equals(KType::GUID.AS(KType));
    }
    
    
    bool KGUIDType::isAutomaticCastableTo(PPtr<KType> t) const {
      return t->equals(KType::GUID.AS(KType));
    }
    
    
    inline int KGUIDType::getSizeInOctets() const {
      return 16;
    }
    
    
    inline bool KGUIDType::isPrimitive() const {
      return true;
    }
    
    
    inline bool KGUIDType::hasConstantSize() const {
      return true;
    }
    
    
    Ptr<KValue> KGUIDType::instantiate() const {
      return new KGlobalUid();
    }
  

//\/ KStringType /\////////////////////////////////////////////////////////////

    class KStringType : public KType {
    public:
      KStringType();
      bool isCastableTo(PPtr<KType> t) const;
      bool isAutomaticCastableTo(PPtr<KType> t) const;
      int  getSizeInOctets() const;
      bool isPrimitive() const;
      bool hasConstantSize() const;
      Ptr<KValue> instantiate() const;
    };
    
    
    KStringType::KStringType() : KType("string") {
      // Nothing;
    }
    
    
    bool KStringType::isCastableTo(PPtr<KType> t) const {
      return t->equals(KType::STRING.AS(KType));
    }
    
    
    bool KStringType::isAutomaticCastableTo(PPtr<KType> t) const {
      return t->equals(KType::STRING.AS(KType));
    }
    
    
    inline int KStringType::getSizeInOctets() const {
      return 0;
    }
    
    
    inline bool KStringType::isPrimitive() const {
      return true;
    }
    
    
    inline bool KStringType::hasConstantSize() const {
      return false;
    }
    
    
    Ptr<KValue> KStringType::instantiate() const {
      return new KString();
    }
    
  
//\/ KRawType /\///////////////////////////////////////////////////////////////
    
    class KRawType : public KType {
    public:
      KRawType();
      bool isCastableTo(PPtr<KType> t) const;
      bool isAutomaticCastableTo(PPtr<KType> t) const;
      int  getSizeInOctets() const;
      bool isPrimitive() const;
      bool hasConstantSize() const;
      Ptr<KValue> instantiate() const;
    };
    
    
    KRawType::KRawType() : KType("raw") {
      // Nothing;
    }
    
    
    bool KRawType::isCastableTo(PPtr<KType> t) const {
      return t->equals(KType::RAW.AS(KType));
    }
    
    
    bool KRawType::isAutomaticCastableTo(PPtr<KType> t) const {
      return t->equals(KType::RAW.AS(KType));
    }
    
    
    inline int KRawType::getSizeInOctets() const {
      return 0;
    }
  
    
    inline bool KRawType::isPrimitive() const {
      return true;
    }
    
    
    inline bool KRawType::hasConstantSize() const {
      return false;
    }
    
    
    Ptr<KValue> KRawType::instantiate() const {
      return new KRaw();
    }
    
  
//\/ KAnyType /\///////////////////////////////////////////////////////////////
    
    class KAnyType : public KType {
    public:
      KAnyType();
      bool isCastableTo(PPtr<KType> t) const;
      bool isAutomaticCastableTo(PPtr<KType> t) const;
      int  getSizeInOctets() const;
      bool isPrimitive() const;
      bool hasConstantSize() const;
      Ptr<KValue> instantiate() const;
    };

      
    KAnyType::KAnyType() : KType("any") {
      // Nothing;
    }
    
      
    bool KAnyType::isCastableTo(PPtr<KType> t) const {
      return t->equals(KType::ANY.AS(KType));
    }
      
    
    bool KAnyType::isAutomaticCastableTo(PPtr<KType> t) const {
      return t->equals(KType::ANY.AS(KType));
    }
      
    
    inline int KAnyType::getSizeInOctets() const {
      return 0;
    }
      
    
    inline bool KAnyType::isPrimitive() const {
      return true;
    }
      
    
    inline bool KAnyType::hasConstantSize() const {
      return false;
    }
      
    
    Ptr<KValue> KAnyType::instantiate() const {
      return new KAny();
    }
  
 
//\/ KNothingType /\///////////////////////////////////////////////////////////
    
    class KNothingType : public KType {
    public:
      KNothingType();
      bool isCastableTo(PPtr<KType> t) const;
      bool isAutomaticCastableTo(PPtr<KType> t) const;
      int  getSizeInOctets() const;
      bool isPrimitive() const;
      bool hasConstantSize() const;
      Ptr<KValue> instantiate() const;
    };

      
    KNothingType::KNothingType() : KType("nothing") {
      // Nothing;
    }
    
      
    bool KNothingType::isCastableTo(PPtr<KType> t) const {
      return t->equals(KType::NOTHING.AS(KType));
    }
    
      
    bool KNothingType::isAutomaticCastableTo(PPtr<KType> t) const {
      return t->equals(KType::NOTHING.AS(KType));
    }
    
      
    inline int KNothingType::getSizeInOctets() const {
      return 0;
    }
    
      
    inline bool KNothingType::isPrimitive() const {
      return true;
    }
      
    
    inline bool KNothingType::hasConstantSize() const {
      return true;
    }
    
      
    Ptr<KValue> KNothingType::instantiate() const {
      return KValue::NOTHING;
    }

  
//\/ KType /\//////////////////////////////////////////////////////////////////
    
  const SPtr<KType> KType::TRUTH(new KTruthType());
  const SPtr<KType> KType::OCTET(new KOctetType());
  const SPtr<KType> KType::INTEGER(new KIntegerType());
  const SPtr<KType> KType::LONGINT(new KLongintType());
  const SPtr<KType> KType::REAL(new KRealType());
  const SPtr<KType> KType::GUID(new KGUIDType());
  const SPtr<KType> KType::STRING(new KStringType());
  const SPtr<KType> KType::RAW(new KRawType());
  const SPtr<KType> KType::ANY(new KAnyType());
  const SPtr<KType> KType::NOTHING(new KNothingType());

    
  KType::KType(string name) {
    _typeName = name;
    _nameHash = KString::generateHashFor(name);
  }
  
    
  const string& KType::getTypeName() const {
    return _typeName;
  }
  
    
  k_longint_t KType::getTypeNameHash() const {
    return _nameHash;
  }
  
    
  bool KType::equals(PPtr<KType> other) const {
    return other->_nameHash == _nameHash;
  }
  
  
  string KType::toKnois() const {
    return _typeName;
  }
  
    
  void KType::printToStream(ostream &os) const {
    os << toKnois();
  }
      
} // namespace type
} // namespace knorba
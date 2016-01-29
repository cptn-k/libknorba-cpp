//
//  Ontology.hpp
//  KnoRBA
//
//  Created by Kay Khandan on 1/15/16.
//  Copyright © 2016 Kay Khandan. All rights reserved.
//

#ifndef Ontology_hpp
#define Ontology_hpp

#include <kfoundation/Ref.h>
#include <kfoundation/KFObject.h>
#include "type/definitions.h"

namespace knorba {
namespace type {

  class KType;
  class KValue;
  class KString;

} // namespace type
} // namespace knorba


namespace knorba {

  using namespace kfoundation;
  using namespace knorba::type;

  class Ontology : public KFObject {

  // --- METHODS --- //

    public: RefConst<KValue> resolveValue(RefConst<KString> name) const;
    public: RefConst<KValue> resolveValue(const k_longint_t hash) const;
    public: RefConst<KType> resolveType(RefConst<KString> name) const;
    public: RefConst<KType> resolveType(const k_longint_t hash) const;
    public: RefConst<KString> resolveHash(const k_longint_t hash) const;

  };

} // namespace knorba

#endif /* Ontology_hpp */
//
//  Ontology.cpp
//  KnoRBA
//
//  Created by Kay Khandan on 1/15/16.
//  Copyright © 2016 Kay Khandan. All rights reserved.
//

#include "type/KValue.h"
#include "type/KType.h"
#include "type/KString.h"

#include "Ontology.h"

namespace knorba {


  RefConst<KValue> Ontology::resolveValue(RefConst<KString> name) const {
    return NULL;
  }


  RefConst<KValue> Ontology::resolveValue(const k_longint_t hash) const {
    return NULL;
  }


  RefConst<KType> Ontology::resolveType(RefConst<KString> name) const {
    return NULL;
  }


  RefConst<KType> Ontology::resolveType(const k_longint_t hash) const {
    return NULL;
  }


  RefConst<KString> Ontology::resolveHash(const k_longint_t hash) const {
    return NULL;
  }
  

} // namespace knorba


//\/ KnoisParser /\////////////////////////////////////////////////////////////

//  class KnoisParser : public StreamParser {
//    public: KnoisParser(Ref<InputStream> stream);
//    public: Ref<KType> parse();
//    public: Ref<KType> parseSimpleType();
//    public: Ref<KType> parseComplexType();
//    public: Ref<KType> parseEnumerationType(RefConst<UString> name);
//    public: bool parseEnumerationMember(Ref<KEnumerationType> e);
//    public: Ref<KType> parseRecordType(RefConst<UString> name);
//    public: bool parseRecordField(Ref<KRecordType> r);
//    public: Ref<KType> parseGridType(RefConst<UString> name);
//  };
//
//
//  KnoisParser::KnoisParser(Ref<InputStream> stream)
//  : StreamParser(stream)
//  {
//    // Nothing;
//  }
//
//
//  Ref<KType> KnoisParser::parse() {
//    Ref<KType> t = parseSimpleType();
//
//    if(t.isNull()) {
//      t = parseComplexType();
//      if(t.isNull()) {
//        return NULL;
//      }
//    }
//
//    return t;
//  }
//
//
//  Ref<KType> KnoisParser::parseSimpleType() {
//    skipSpacesAndNewLines();
//
//    if(readSequence(K"truth") > 0) {
//      return KType::TRUTH;
//    }
//
//    if(readSequence(K"octet") > 0) {
//      return KType::OCTET;
//    }
//
//    if(readSequence(K"integer") > 0) {
//      return KType::INTEGER;
//    }
//
//    if(readSequence(K"longint") > 0) {
//      return KType::LONGINT;
//    }
//
//    if(readSequence(K"real") > 0) {
//      return KType::REAL;
//    }
//
//    if(readSequence(K"gur") > 0) {
//      return KType::GUR;
//    }
//
//    if(readSequence(K"string") > 0) {
//      return KType::STRING;
//    }
//
//    if(readSequence(K"raw") > 0) {
//      return KType::RAW;
//    }
//
//    if(readSequence(K"any") > 0) {
//      return KType::ANY;
//    }
//
//    if(readSequence(K"nothing") > 0) {
//      return KType::NOTHING;
//    }
//
//    return NULL;
//  }
//
//
//  Ref<KType> KnoisParser::parseComplexType() {
//    skipSpacesAndNewLines();
//
//    Ref<BufferOutputStream> buffer = new BufferOutputStream();
//
//    if(readIdentifier(buffer.AS(OutputStream)) > 0) {
//      skipSpacesAndNewLines();
//      if(readSequence(K"IS") > 0) {
//        Ref<KType> t = parseEnumerationType(buffer->getString());
//        if(!t.isNull()) {
//          return t;
//        }
//
//        t = parseRecordType(buffer->getString());
//        if(!t.isNull()) {
//          return t;
//        }
//
//        t = parseGridType(buffer->getString());
//        if(!t.isNull()) {
//          return t;
//        }
//      }
//    }
//
//    return NULL;
//  }
//
//
//  Ref<KType> KnoisParser::parseEnumerationType(RefConst<UString> name) {
//    skipSpacesAndNewLines();
//    if(readSequence(K"enum") > 0) {
//      Ref<KEnumerationType> t = new KEnumerationType(name);
//
//      skipSpacesAndNewLines();
//      if(readChar(L'(') > 0) {
//        Ref<BufferOutputStream> buffer = new BufferOutputStream();
//
//        while(true) {
//          buffer->clear();
//          skipSpacesAndNewLines();
//          if(readIdentifier(buffer.AS(OutputStream)) > 0) {
//            skipSpacesAndNewLines();
//            if(readChar(':')) {
//              skipSpacesAndNewLines();
//              long int i;
//              if(readNumber(i) > 0) {
//                t->addMember((k_octet_t)i, buffer->getString());
//              } else {
//                throw ParseException(K"Expected a number", getCodeLocation());
//              } // readNumber(i)
//            } else {
//              t->addMember(buffer->getString());
//            }
//            skipSpacesAndNewLines();
//          } else if(readChar(')') > 0) {
//            return t.AS(KType);
//          } else if(readChar(',') <= 0) {
//            throw ParseException(K"Expected ','", getCodeLocation());
//          }
//        } // while
//
//      } else {
//        throw ParseException(K"Expected '('", getCodeLocation());
//      }
//    }
//
//    return NULL;
//  }
//
//
//  Ref<KType> KnoisParser::parseRecordType(RefConst<UString> name) {
//    skipSpacesAndNewLines();
//    if(couldNot(readSequence(K"record"))) {
//      return NULL;
//    }
//
//    skipSpacesAndNewLines();
//    if(couldNot(readChar('('))) {
//      throw ParseException(K"Expected '('", getCodeLocation());
//    }
//
//    Ref<KRecordType> t = new KRecordType(name);
//
//    while(true) {
//      if(parseRecordField(t)) {
//        skipSpacesAndNewLines();
//        if(readChar(',')) {
//          continue;
//        }
//      }
//      if(readChar(')')) {
//        break;
//      } else if(testEndOfStream()) {
//        throw ParseException(K"Reached end of stream while parsing record.", getCodeLocation());
//      } else {
//        throw ParseException(K"Expected ')'");
//      }
//    }
//
//    return t.AS(KType);
//  }
//
//
//  bool KnoisParser::parseRecordField(Ref<KRecordType> r) {
//    skipSpacesAndNewLines();
//
//    Ref<BufferOutputStream> fieldName = new BufferOutputStream();
//
//    if(couldNot(readIdentifier(fieldName.AS(OutputStream)))) {
//      return false;
//    }
//
//    skipSpacesAndNewLines();
//    if(couldNot(readChar(L':'))) {
//      throw ParseException(K"Expected ':'", getCodeLocation());
//    }
//
//    skipSpacesAndNewLines();
//
//#warning Continue when TypeDictionary implemented
//
//    return false;
//  }
//
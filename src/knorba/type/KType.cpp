/*---[KType.cpp]-----------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : knorba::type::KTruthType::*
 |              knorba::type::KOctetType::*
 |              knorba::type::KIntegerType::*
 |              knorba::type::KLongintType::*
 |              knorba::type::KRealType::*
 |              knorba::type::KGUIDType::*
 |              knorba::type::KStringType::*
 |              knorba::type::KRawType::*
 |              knorba::type::KAnyType::*
 |              knorba::type::KNothingType::*
 |  Implements: knorba::type::KType::*
 |              knorba::type::KTruthType::*
 |              knorba::type::KOctetType::*
 |              knorba::type::KIntegerType::*
 |              knorba::type::KLongintType::*
 |              knorba::type::KRealType::*
 |              knorba::type::KGUIDType::*
 |              knorba::type::KStringType::*
 |              knorba::type::KRawType::*
 |              knorba::type::KAnyType::*
 |              knorba::type::KNothingType::*
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

// Std
#include <cstring>

// KFoundation
#include <kfoundation/Ref.h>
#include <kfoundation/System.h>
#include <kfoundation/StreamParser.h>
#include <kfoundation/UString.h>
#include <kfoundation/BufferOutputStream.h>
#include <kfoundation/IOException.h>
#include <kfoundation/ObjectSerializer.h>
#include <kfoundation/Int.h>
#include <kfoundation/LongInt.h>
#include <kfoundation/Double.h>

// Internal
#include "../Ontology.h"
#include "KValue.h"
#include "KTruth.h"
#include "KOctet.h"
#include "KInteger.h"
#include "KLongint.h"
#include "KReal.h"
#include "KGur.h"
#include "KString.h"
#include "KRaw.h"
#include "KAny.h"

// Self
#include "KType.h"

namespace knorba {
namespace type {
  

//\/ KTruthType /\/////////////////////////////////////////////////////////////
    
  class KTruthType : public KType {
    public: KTruthType();
    public: bool isCastableTo(RefConst<KType> t) const;
    public: bool isAutomaticCastableTo(RefConst<KType> t) const;
    public: k_integer_t getSizeInOctets() const;
    public: bool isPrimitive() const;
    public: bool hasConstantSize() const;
    public: Ref<KValue> instantiate() const;

    public: void writeBufferToStream(const k_octet_t* buffer,
        Ref<OutputStream> stream) const;

    public: void writeStreamToBuffer(Ref<InputStream> stream,
        RefConst<Ontology> ontology, k_octet_t* buffer) const;

    public: void serializeBuffer(const k_octet_t* buffer,
        Ref<ObjectSerializer> serializer) const;

    public: void deserializeIntoBuffer(Ref<ObjectToken> head,
        RefConst<Ontology> ontology, k_octet_t* buffer) const;
  };
  
  
  KTruthType::KTruthType() : KType(K"truth") {
    // Nothing;
  }
  
  
  bool KTruthType::isCastableTo(RefConst<KType> t) const {
    return KType::TRUTH->equals(t);
  }
  
  
  bool KTruthType::isAutomaticCastableTo(RefConst<KType> t) const {
    return KType::TRUTH->equals(t);
  }
  
  
  inline k_integer_t KTruthType::getSizeInOctets() const {
    return 1;
  }
  
  
  inline bool KTruthType::isPrimitive() const {
    return true;
  }
  
  
  inline bool KTruthType::hasConstantSize() const {
    return true;
  }
  
  
  Ref<KValue> KTruthType::instantiate() const {
    return new KTruth();
  }


  void KTruthType::writeBufferToStream(const k_octet_t* buffer,
      Ref<OutputStream> stream) const
  {
    stream->write(*buffer);
  }


  void KTruthType::writeStreamToBuffer(Ref<InputStream> stream,
      RefConst<Ontology>, k_octet_t* buffer) const
  {
    int v = stream->read();
    if(v == -1) {
      throw IOException(K"Read failed");
    }
    *buffer = (k_truth_t)v;
  }


  void KTruthType::serializeBuffer(const k_octet_t* buffer,
      Ref<ObjectSerializer> serializer) const
  {
    serializer->object(K"KTruth")
      ->attribute(K"value", KTruth::toString(*buffer))
      ->endObject();
  }


  void KTruthType::deserializeIntoBuffer(Ref<ObjectToken> head,
      RefConst<Ontology>, k_octet_t* buffer) const
  {
    head->validateClass(K"KTruth");

    Ref<Token> token = head->next();
    token->validateType(Token::ATTRIBUTE);

    Ref<AttributeToken> attrib = token.AS(AttributeToken);
    RefConst<UString> str = attrib->validateName(K"value")->getValue();

    if(str->getOctetCount() == 0) {
      *buffer = KTruth::X;
      return;
    }

    char ch = str->at(0);

    switch (ch) {
      case 't':
      case 'T':
        *buffer = KTruth::T;
        break;

      case 'f':
      case 'F':
        *buffer = KTruth::F;
        break;

      case 'x':
      case 'X':
      default:
        *buffer = KTruth::X;
        break;
    }

    token = token->next();
    token->validateType(EndObjectToken::TYPE);
  }


//\/ KOctetType /\/////////////////////////////////////////////////////////////
    
  class KOctetType : public KType {
    public: KOctetType();
    public: bool isCastableTo(RefConst<KType> t) const;
    public: bool isAutomaticCastableTo(RefConst<KType> t) const;
    public: k_integer_t  getSizeInOctets() const;
    public: bool isPrimitive() const;
    public: bool hasConstantSize() const;
    public: Ref<KValue> instantiate() const;

    public: void writeBufferToStream(const k_octet_t* buffer,
        Ref<OutputStream> stream) const;

    public: void writeStreamToBuffer(Ref<InputStream> stream,
        RefConst<Ontology> ontology, k_octet_t* buffer) const;

    public: void serializeBuffer(const k_octet_t* buffer,
        Ref<ObjectSerializer> serializer) const;

    public: void deserializeIntoBuffer(Ref<ObjectToken> head,
        RefConst<Ontology> ontology, k_octet_t* buffer) const;
  };
  
  
  KOctetType::KOctetType() : KType(K"octet") {
    // Nothing;
  }
  
  
  bool KOctetType::isCastableTo(RefConst<KType> t) const {
    if(KType::OCTET->equals(t)) {
      return true;
    }
    
    if(KType::INTEGER->equals(t)) {
      return true;
    }
    
    if(KType::LONGINT->equals(t)) {
      return true;
    }
    
    if(KType::REAL->equals(t)) {
      return true;
    }
    
    return false;
  }
  
  
  bool KOctetType::isAutomaticCastableTo(RefConst<KType> t) const {
    if(KType::OCTET->equals(t)) {
      return true;
    }
    
    if(KType::INTEGER->equals(t)) {
      return true;
    }
    
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
  
  
  Ref<KValue> KOctetType::instantiate() const {
    return new KOctet();
  }


  void KOctetType::writeBufferToStream(const k_octet_t* buffer,
      Ref<OutputStream> stream) const
  {
    stream->write(*buffer);
  }


  void KOctetType::writeStreamToBuffer(Ref<InputStream> stream,
      RefConst<Ontology>, k_octet_t* buffer) const
  {
    k_integer_t v = stream->read();
    if(v < 0) {
      throw IOException(K"Not enough bytes to read");
    }
    *buffer = (k_octet_t)v;
  }


  void KOctetType::serializeBuffer(const k_octet_t* buffer,
      Ref<ObjectSerializer> serializer) const
  {
    serializer->object(K"KOctet")->attribute(K"value", *buffer)->endObject();
  }


  void KOctetType::deserializeIntoBuffer(Ref<ObjectToken> head,
      RefConst<Ontology> ontology, k_octet_t* buffer) const
  {
    head->validateClass(K"KOctet");

    Ref<Token> token = head->next();
    token->validateType(AttributeToken::TYPE);

    Ref<AttributeToken> attrib = token.AS(AttributeToken);
    *buffer = Int::parse(attrib->validateName(K"value")->getValue());

    token = token->next();
    token->validateType(EndObjectToken::TYPE);
  }


//\/ KIntegerType /\///////////////////////////////////////////////////////////

  class KIntegerType : public KType {
    public: KIntegerType();
    public: bool isCastableTo(RefConst<KType> t) const;
    public: bool isAutomaticCastableTo(RefConst<KType> t) const;
    public: int  getSizeInOctets() const;
    public: bool isPrimitive() const;
    public: bool hasConstantSize() const;
    public: Ref<KValue> instantiate() const;

    public: void writeBufferToStream(const k_octet_t* buffer,
        Ref<OutputStream> stream) const;

    public: void writeStreamToBuffer(Ref<InputStream> stream,
        RefConst<Ontology> ontology, k_octet_t* buffer) const;

    public: void serializeBuffer(const k_octet_t* buffer,
        Ref<ObjectSerializer> serializer) const;

    public: void deserializeIntoBuffer(Ref<ObjectToken> head,
        RefConst<Ontology> ontology, k_octet_t* buffer) const;
  };

  
  KIntegerType::KIntegerType() : KType(K"integer") {
    // Nothing;
  }
  
  
  bool KIntegerType::isCastableTo(RefConst<KType> t) const {
    if(KType::OCTET->equals(t)) {
      return true;
    }
    
    if(KType::INTEGER->equals(t)) {
      return true;
    }
    
    if(KType::LONGINT->equals(t)) {
      return true;
    }
    
    if(KType::REAL->equals(t)) {
      return true;
    }
    
    return false;
  }
  
  
  bool KIntegerType::isAutomaticCastableTo(RefConst<KType> t) const {
    if(KType::INTEGER->equals(t)) {
      return true;
    }
    
    if(KType::LONGINT->equals(t)) {
      return true;
    }
    
    if(KType::REAL->equals(t)) {
      return true;
    }
    
    return false;
  }
  
  
  inline k_integer_t KIntegerType::getSizeInOctets() const {
    return KInteger::SIZE_IN_OCTETS;
  }
  
  
  inline bool KIntegerType::isPrimitive() const {
    return true;
  }
  
  
  inline bool KIntegerType::hasConstantSize() const {
    return true;
  }
  
  
  Ref<KValue> KIntegerType::instantiate() const {
    return new KInteger();
  }


  void KIntegerType::writeBufferToStream(const k_octet_t* buffer,
      Ref<OutputStream> stream) const
  {
    stream->write(buffer, KInteger::SIZE_IN_OCTETS);
  }


  void KIntegerType::writeStreamToBuffer(Ref<InputStream> stream,
      RefConst<Ontology>, k_octet_t* buffer) const
  {
    if(stream->read(buffer, KInteger::SIZE_IN_OCTETS)
           < KInteger::SIZE_IN_OCTETS)
    {
      throw IOException(K"Not enough bytes to read");
    }
  }


  void KIntegerType::serializeBuffer(const k_octet_t* buffer,
      Ref<ObjectSerializer> serializer) const
  {
    serializer->object(K"KInteger")
        ->attribute(K"value", KInteger::readFromBuffer(buffer))
        ->endObject();
  }


  void KIntegerType::deserializeIntoBuffer(Ref<ObjectToken> head,
      RefConst<Ontology>, k_octet_t* buffer) const
  {
    head->validateClass(K"KInteger");

    Ref<Token> token = head->next();
    token->validateType(AttributeToken::TYPE);

    Ref<AttributeToken> attrib = token.AS(AttributeToken);
    KInteger::writeToBuffer(
        Int::parse(attrib->validateName(K"value")->getValue()), buffer);

    token = token->next();
    token->validateType(EndObjectToken::TYPE);
  }


//\/ KLongintType /\///////////////////////////////////////////////////////////
    
  class KLongintType : public KType {
    public: KLongintType();
    public: bool isCastableTo(RefConst<KType> t) const;
    public: bool isAutomaticCastableTo(RefConst<KType> t) const;
    public: k_integer_t getSizeInOctets() const;
    public: bool isPrimitive() const;
    public: bool hasConstantSize() const;
    public: Ref<KValue> instantiate() const;

    public: void writeBufferToStream(const k_octet_t* buffer,
        Ref<OutputStream> stream) const;

    public: void writeStreamToBuffer(Ref<InputStream> stream,
        RefConst<Ontology> ontology, k_octet_t* buffer) const;

    public: void serializeBuffer(const k_octet_t* buffer,
        Ref<ObjectSerializer> serializer) const;

    public: void deserializeIntoBuffer(Ref<ObjectToken> head,
        RefConst<Ontology> ontology, k_octet_t* buffer) const;
  };

  
  KLongintType::KLongintType() : KType(K"longint") {
    // Nothing;
  }
  
  
  bool KLongintType::isCastableTo(RefConst<KType> t) const {
    if(KType::OCTET->equals(t)) {
      return true;
    }
    
    if(KType::INTEGER->equals(t)) {
      return true;
    }
    
    if(KType::LONGINT->equals(t)) {
      return true;
    }
    
    if(KType::REAL->equals(t)) {
      return true;
    }
    
    return false;
  }
  
  
  bool KLongintType::isAutomaticCastableTo(RefConst<KType> t) const {
    if(KType::LONGINT->equals(t)) {
      return true;
    }
    
    if(KType::REAL->equals(t)) {
      return true;
    }
    
    return false;
  }
  
  
  inline int KLongintType::getSizeInOctets() const {
    return KLongint::SIZE_IN_OCTETS;
  }
  
  
  inline bool KLongintType::isPrimitive() const {
    return true;
  }
  
  
  inline bool KLongintType::hasConstantSize() const {
    return true;
  }
  
  
  Ref<KValue> KLongintType::instantiate() const {
    return new KLongint();
  }


  void KLongintType::writeBufferToStream(const k_octet_t* buffer,
      Ref<OutputStream> stream) const
  {
    stream->write(buffer, KLongint::SIZE_IN_OCTETS);
  }


  void KLongintType::writeStreamToBuffer(Ref<InputStream> stream,
      RefConst<Ontology>, k_octet_t* buffer) const
  {
    if(stream->read(buffer, KLongint::SIZE_IN_OCTETS)
       < KLongint::SIZE_IN_OCTETS)
    {
      throw IOException(K"Not enough data to read");
    }
  }


  void KLongintType::serializeBuffer(const k_octet_t* buffer,
      Ref<ObjectSerializer> serializer) const
  {
    serializer->object(K"KLongint")
        ->attribute(K"value", KLongint::readFromBuffer(buffer))
        ->endObject();
  }


  void KLongintType::deserializeIntoBuffer(Ref<ObjectToken> head,
      RefConst<Ontology>, k_octet_t* buffer) const
  {
    head->validateClass(K"KLongint");

    Ref<Token> token = head->next();
    token->validateType(AttributeToken::TYPE);

    Ref<AttributeToken> attrib = token.AS(AttributeToken);

    KLongint::writeToBuffer(
        LongInt::parse(attrib->validateName(K"value")->getValue()), buffer);

    token = token->next();
    token->validateType(EndObjectToken::TYPE);
  }

    
//\/ KRealType /\//////////////////////////////////////////////////////////////
    
  class KRealType : public KType {
    public: KRealType();
    public: bool isCastableTo(RefConst<KType> t) const;
    public: bool isAutomaticCastableTo(RefConst<KType> t) const;
    public: k_integer_t  getSizeInOctets() const;
    public: bool isPrimitive() const;
    public: bool hasConstantSize() const;
    public: Ref<KValue> instantiate() const;

    public: void writeBufferToStream(const k_octet_t* buffer,
        Ref<OutputStream> stream) const;

    public: void writeStreamToBuffer(Ref<InputStream> stream,
        RefConst<Ontology> ontology, k_octet_t* buffer) const;

    public: void serializeBuffer(const k_octet_t* buffer,
        Ref<ObjectSerializer> serializer) const;

    public: void deserializeIntoBuffer(Ref<ObjectToken> head,
        RefConst<Ontology> ontology, k_octet_t* buffer) const;
  };
  
  
  KRealType::KRealType() : KType(K"real") {
    // Nothing;
  }
  
  
  bool KRealType::isCastableTo(RefConst<KType> t) const {
    if(KType::OCTET->equals(t)) {
      return true;
    }
    
    if(KType::INTEGER->equals(t)) {
      return true;
    }
    
    if(KType::LONGINT->equals(t)) {
      return true;
    }
    
    if(KType::REAL->equals(t)) {
      return true;
    }
    
    return false;
  }
  
  
  bool KRealType::isAutomaticCastableTo(RefConst<KType> t) const {
    return KType::REAL->equals(t);
  }
  
  
  inline int KRealType::getSizeInOctets() const {
    return KReal::SIZE_IN_OCTETS;
  }
  
  
  inline bool KRealType::isPrimitive() const {
    return true;
  }
  
  
  inline bool KRealType::hasConstantSize() const {
    return true;
  }
  
  
  Ref<KValue> KRealType::instantiate() const {
    return new KReal();
  }


  void KRealType::writeBufferToStream(const k_octet_t* buffer,
      Ref<OutputStream> stream) const
  {
    stream->write(buffer, KReal::SIZE_IN_OCTETS);
  }


  void KRealType::writeStreamToBuffer(Ref<InputStream> stream,
      RefConst<Ontology>, k_octet_t* buffer) const
  {
    if(stream->read(buffer, KReal::SIZE_IN_OCTETS) < KReal::SIZE_IN_OCTETS) {
      throw IOException(K"Not enough data to read.");
    }
  }


  void KRealType::serializeBuffer(const k_octet_t* buffer,
      Ref<ObjectSerializer> serializer) const
  {
    serializer->object(K"KReal")
        ->attribute(K"value", KReal::readFromBuffer(buffer))
        ->endObject();
  }


  void KRealType::deserializeIntoBuffer(Ref<ObjectToken> head,
      RefConst<Ontology>, k_octet_t* buffer) const
  {
    head->validateClass(K"KReal");

    Ref<Token> token = head->next();
    token->validateType(AttributeToken::TYPE);

    Ref<AttributeToken> attrib = token.AS(AttributeToken);
    KReal::writeToBuffer(
        Double::parse(attrib->validateName(K"value")->getValue()), buffer);

    token = token->next();
    token->validateType(EndObjectToken::TYPE);
  }


//\/ KGurType /\///////////////////////////////////////////////////////////////

  class KGurType : public KType {
    public: KGurType();
    public: bool isCastableTo(RefConst<KType> t) const;
    public: bool isAutomaticCastableTo(RefConst<KType> t) const;
    public: k_integer_t  getSizeInOctets() const;
    public: bool isPrimitive() const;
    public: bool hasConstantSize() const;
    public: Ref<KValue> instantiate() const;

    public: void writeBufferToStream(const k_octet_t* buffer,
        Ref<OutputStream> stream) const;

    public: void writeStreamToBuffer(Ref<InputStream> stream,
        RefConst<Ontology> ontology, k_octet_t* buffer) const;

    public: void serializeBuffer(const k_octet_t* buffer,
        Ref<ObjectSerializer> serializer) const;

    public: void deserializeIntoBuffer(Ref<ObjectToken> head,
        RefConst<Ontology> ontology, k_octet_t* buffer) const;
  };

  
  KGurType::KGurType() : KType(K"gur") {
    // Nothing;
  }
  

  bool KGurType::isCastableTo(RefConst<KType> t) const {
    return KType::GUR->equals(t);
  }
  
  
  bool KGurType::isAutomaticCastableTo(RefConst<KType> t) const {
    return KType::GUR->equals(t);
  }
  
  
  inline int KGurType::getSizeInOctets() const {
    return KGur::SIZE_IN_OCTETS;
  }
  
  
  inline bool KGurType::isPrimitive() const {
    return true;
  }
  
  
  inline bool KGurType::hasConstantSize() const {
    return true;
  }
  
  
  Ref<KValue> KGurType::instantiate() const {
    return new KGur();
  }


  void KGurType::writeBufferToStream(const k_octet_t *buffer,
      Ref<OutputStream> stream) const
  {
    stream->write(buffer, 10);
    buffer += 10;
    INTEGER->writeBufferToStream(buffer, stream);
    buffer += 4;
    stream->write(buffer, 2);
  }


  void KGurType::writeStreamToBuffer(Ref<InputStream> stream,
      RefConst<Ontology>, k_octet_t* buffer) const
  {
    if(stream->read(buffer, 10) < 10) {
      throw IOException(K"Not enough data to read");
    }
    buffer += 10;
    INTEGER->writeStreamToBuffer(stream, NULL, buffer);
    buffer += 4;
    if(stream->read(buffer, 2) < 2) {
      throw IOException(K"Not enough data to read");
    }
  }


  void KGurType::serializeBuffer(const k_octet_t* buffer,
      Ref<ObjectSerializer> serializer) const
  {
    serializer->object(K"KGur")
      ->attribute(K"value", KGur(KGur::readFromBuffer(buffer)).toString())
      ->endObject();
  }


  void KGurType::deserializeIntoBuffer(Ref<ObjectToken> head,
      RefConst<Ontology> ontology, k_octet_t* buffer) const
  {
    head->validateClass(K"KGur");

    Ref<Token> token = head->next();
    token->validateType(AttributeToken::TYPE);

    Ref<AttributeToken> attrib = token.AS(AttributeToken);

    if(attrib->checkName(K"alias")) {
      Ref<KString> alias = new KString(attrib->getValue());
      RefConst<KValue> v = ontology->resolveValue(alias);

      bool resolved = !v.isNull();

      if(resolved) {
        resolved = resolved && v->getType()->equals(GUR);
      }

      if(!resolved) {
        throw ParseException(K"Could not resolve alias \"" + *alias
            + "\" to a GUR");
      }

      KGur::writeToBuffer(v.AS(KGur)->get(), buffer);
    } else if(attrib->checkName(K"value")) {
      throw ParseException(K"Deserialization of KGUID from value is not supported.");
    }

    token = token->next();
    token->validateType(EndObjectToken::TYPE);
  }


//\/ KStringType /\////////////////////////////////////////////////////////////

  class KStringType : public KType {
    public: KStringType();
    public: bool isCastableTo(RefConst<KType> t) const;
    public: bool isAutomaticCastableTo(RefConst<KType> t) const;
    public: k_integer_t getSizeInOctets() const;
    public: bool isPrimitive() const;
    public: bool hasConstantSize() const;
    public: Ref<KValue> instantiate() const;

    public: void writeBufferToStream(const k_octet_t* buffer,
        Ref<OutputStream> stream) const;

    public: void writeStreamToBuffer(Ref<InputStream> stream,
        RefConst<Ontology> ontology, k_octet_t* buffer) const;

    public: void serializeBuffer(const k_octet_t* buffer,
        Ref<ObjectSerializer> serializer) const;

    public: void deserializeIntoBuffer(Ref<ObjectToken> head,
        RefConst<Ontology> ontology, k_octet_t* buffer) const;

    public: void initializeBuffer(k_octet_t* buffer) const;
    public: void cleanupBuffer(k_octet_t* buffer) const;
    public: void copyBuffer(const k_octet_t* src, k_octet_t* target) const;
    public: k_longint_t getStreamSizeInOctets(const k_octet_t* buffer) const;
  };
  
  
  KStringType::KStringType() : KType(K"string") {
    // Nothing;
  }
  
  
  bool KStringType::isCastableTo(RefConst<KType> t) const {
    return KType::STRING->equals(t);
  }
  
  
  bool KStringType::isAutomaticCastableTo(RefConst<KType> t) const {
    return KType::STRING->equals(t);
  }
  
  
  inline int KStringType::getSizeInOctets() const {
    return sizeof(KString::header_t);
  }
  
  
  inline bool KStringType::isPrimitive() const {
    return true;
  }
  
  
  inline bool KStringType::hasConstantSize() const {
    return false;
  }
  
  
  Ref<KValue> KStringType::instantiate() const {
    return new KString();
  }


  void KStringType::writeBufferToStream(const k_octet_t* buffer,
      Ref<OutputStream> stream) const
  {
    const KString::header_t* header = (const KString::header_t*)buffer;
    INTEGER->writeBufferToStream((k_octet_t*)&header->size, stream);
    LONGINT->writeBufferToStream((k_octet_t*)&header->hash, stream);
    stream->write(header->data, header->size);
  }


  void KStringType::writeStreamToBuffer(Ref<InputStream> stream,
      RefConst<Ontology>, k_octet_t* buffer) const
  {
    KString::cleanupBuffer(buffer);
    KString::header_t* header = (KString::header_t*)buffer;
    INTEGER->writeStreamToBuffer(stream, NULL, (k_octet_t*)&header->size);
    LONGINT->writeStreamToBuffer(stream, NULL, (k_octet_t*)&header->hash);
    header->data = new k_octet_t[header->size];
    if(stream->read(header->data, header->size) < header->size) {
      throw IOException(K"Not enough data to read");
    }
  }


  void KStringType::serializeBuffer(const k_octet_t* buffer,
      Ref<ObjectSerializer> serializer) const
  {
    const KString::header_t* header = (const KString::header_t*)buffer;
    Ref<UString> str = new UString(header->data, header->size);
    serializer->object(K"KString")->attribute(K"value", str)->endObject();
  }


  void KStringType::deserializeIntoBuffer(Ref<ObjectToken> head,
      RefConst<Ontology>, k_octet_t* buffer) const
  {
    head->validateClass(K"KString");

    Ref<Token> token = head->next();
    token->validateType(AttributeToken::TYPE);

    Ref<AttributeToken> attrib = token.AS(AttributeToken);
    attrib->validateName(K"value");

    RefConst<UString> str = attrib->getValue();
    KString::writeDataToBuffer(str->getOctets(), str->getOctetCount(), buffer);

    token = token->next();
    token->validateType(EndObjectToken::TYPE);
  }


  void KStringType::initializeBuffer(k_octet_t *buffer) const {
    KString::initBuffer(buffer);
  }


  void KStringType::cleanupBuffer(k_octet_t* buffer) const {
    KString::cleanupBuffer(buffer);
  }


  void KStringType::copyBuffer(const k_octet_t* src, k_octet_t* target) const {
    KString::writeDuplicateToBuffer(src, target);
  }


  k_longint_t KStringType::getStreamSizeInOctets(const k_octet_t* buffer)
  const
  {
    const KString::header_t* header = (const KString::header_t*)buffer;
    return KInteger::SIZE_IN_OCTETS + KLongint::SIZE_IN_OCTETS
        + header->size;
  }


//\/ KRawType /\///////////////////////////////////////////////////////////////
    
  class KRawType : public KType {
    public: KRawType();
    public: bool isCastableTo(RefConst<KType> t) const;
    public: bool isAutomaticCastableTo(RefConst<KType> t) const;
    public: int  getSizeInOctets() const;
    public: bool isPrimitive() const;
    public: bool hasConstantSize() const;
    public: Ref<KValue> instantiate() const;
    public: k_longint_t getStreamSizeInOctets(const k_octet_t* buffer) const;

    public: void writeBufferToStream(const k_octet_t* buffer,
        Ref<OutputStream> stream) const;

    public: void writeStreamToBuffer(Ref<InputStream> stream,
        RefConst<Ontology> ontology, k_octet_t* buffer) const;

    public: void serializeBuffer(const k_octet_t* buffer,
        Ref<ObjectSerializer> serializer) const;

    public: void deserializeIntoBuffer(Ref<ObjectToken> head,
        RefConst<Ontology> ontology, k_octet_t* buffer) const;

    public: void initializeBuffer(k_octet_t* buffer) const;
    public: void cleanupBuffer(k_octet_t* buffer) const;
    public: void copyBuffer(const k_octet_t* src, k_octet_t* target) const;
  };
  
  
  KRawType::KRawType() : KType(K"raw") {
    // Nothing;
  }
  
  
  bool KRawType::isCastableTo(RefConst<KType> t) const {
    return KType::RAW->equals(t);
  }
  
  
  bool KRawType::isAutomaticCastableTo(RefConst<KType> t) const {
    return KType::RAW->equals(t);
  }
  
  
  inline int KRawType::getSizeInOctets() const {
    return sizeof(KRaw::header_t);
  }

  
  inline bool KRawType::isPrimitive() const {
    return true;
  }
  
  
  inline bool KRawType::hasConstantSize() const {
    return false;
  }
  
  
  Ref<KValue> KRawType::instantiate() const {
    return new KRaw();
  }


  k_longint_t KRawType::getStreamSizeInOctets(const k_octet_t* buffer) const {
    const KRaw::header_t* header = (const KRaw::header_t*)buffer;
    return KLongint::SIZE_IN_OCTETS + header->size;
  }


  void KRawType::writeBufferToStream(const k_octet_t *buffer,
      Ref<OutputStream> stream) const
  {
    const KRaw::header_t* header = (const KRaw::header_t*)buffer;
    LONGINT->writeBufferToStream((k_octet_t*)&header->size, stream);
    stream->write(header->data, (kf_int32_t)header->size);
  }


  void KRawType::writeStreamToBuffer(Ref<InputStream> stream,
      RefConst<Ontology>, k_octet_t* buffer) const
  {
    KRaw::cleanupBuffer(buffer);
    KRaw::header_t* header = (KRaw::header_t*)buffer;
    LONGINT->writeStreamToBuffer(stream, NULL, (k_octet_t*)&header->size);
    header->data = new k_octet_t[header->size];
    if(stream->read(header->data, (kf_int32_t)header->size) < header->size) {
      throw IOException(K"Not enough data to read");
    }
  }


  void KRawType::serializeBuffer(const k_octet_t*buffer,
      Ref<ObjectSerializer> serializer) const
  {
    const KRaw::header_t* header = (const KRaw::header_t*)buffer;
    serializer->object(K"KRaw")->attribute(K"size", header->size)->endObject();
  }


  void KRawType::deserializeIntoBuffer(Ref<ObjectToken> head,
      RefConst<Ontology>, k_octet_t* buffer) const
  {
    throw KFException(K"Operation not supported");
  }


  void KRawType::initializeBuffer(k_octet_t* buffer) const {
    KRaw::initBuffer(buffer);
  }


  void KRawType::cleanupBuffer(k_octet_t* buffer) const {
    KRaw::cleanupBuffer(buffer);
  }


  void KRawType::copyBuffer(const k_octet_t* src, k_octet_t* target) const {
    KRaw::writeToBuffer(src, target);
  }


//\/ KAnyType /\///////////////////////////////////////////////////////////////
    
  class KAnyType : public KType {
    public: KAnyType();
    public: bool isCastableTo(RefConst<KType> t) const;
    public: bool isAutomaticCastableTo(RefConst<KType> t) const;
    public: k_integer_t getSizeInOctets() const;
    public: bool isPrimitive() const;
    public: bool hasConstantSize() const;
    public: Ref<KValue> instantiate() const;
    public: k_longint_t getStreamSizeInOctets(const k_octet_t* buffer) const;

    public: void writeBufferToStream(const k_octet_t* buffer,
        Ref<OutputStream> stream) const;

    public: void writeStreamToBuffer(Ref<InputStream> stream,
        RefConst<Ontology> ontology, k_octet_t* buffer) const;

    public: void serializeBuffer(const k_octet_t* buffer,
        Ref<ObjectSerializer> serializer) const;

    public: void deserializeIntoBuffer(Ref<ObjectToken> head,
        RefConst<Ontology> ontology, k_octet_t* buffer) const;

    public: void initializeBuffer(k_octet_t* buffer) const;
    public: void cleanupBuffer(k_octet_t* buffer) const;
    public: void copyBuffer(const k_octet_t* src, k_octet_t* target) const;
  };

    
  KAnyType::KAnyType() : KType(K"any") {
    // Nothing;
  }
  
    
  bool KAnyType::isCastableTo(RefConst<KType> t) const {
    return t->equals(KType::ANY.AS(KType));
  }
    
  
  bool KAnyType::isAutomaticCastableTo(RefConst<KType> t) const {
    return KType::ANY->equals(t);
  }
    
  
  inline int KAnyType::getSizeInOctets() const {
    return sizeof(kf_uref_t);
  }
    
  
  inline bool KAnyType::isPrimitive() const {
    return true;
  }
    
  
  inline bool KAnyType::hasConstantSize() const {
    return false;
  }
    
  
  Ref<KValue> KAnyType::instantiate() const {
    return new KAny();
  }


  k_longint_t KAnyType::getStreamSizeInOctets(const k_octet_t* buffer) const {
    Ref<KValue> v = KAny::readFromBuffer(buffer);
    return KLongint::SIZE_IN_OCTETS + v->getTotalSizeInOctets();
  }


  void KAnyType::writeBufferToStream(const k_octet_t* buffer,
      Ref<OutputStream> stream) const
  {
    Ref<KValue> v = KAny::readFromBuffer(buffer);

    k_longint_t hash = v->getType()->getHashCode();
    LONGINT->writeBufferToStream((k_octet_t*)&hash, stream);

    v->writeToBinaryStream(stream);
  }


  void KAnyType::writeStreamToBuffer(Ref<InputStream> stream,
      RefConst<Ontology> ontology, k_octet_t* buffer) const
  {
    k_longint_t hash;
    LONGINT->writeStreamToBuffer(stream, NULL, (k_octet_t*)&hash);

    RefConst<KType> type = ontology->resolveType(hash);

    if(type.isNull()) {
      throw KFException(K"No type is registered for hashcode " + hash);
    }

    Ref<KValue> v = type->instantiate();
    v->readFromBinaryStream(stream, ontology);

    KAny::writeToBuffer(v, buffer);
  }


  void KAnyType::serializeBuffer(const k_octet_t* buffer,
      Ref<ObjectSerializer> serializer) const
  {
    Ref<KValue> v = KAny::readFromBuffer(buffer);
    serializer->object(K"KAny")
      ->attribute(K"dataType", v->getType()->getTypeName())
      ->member(K"value")->object(*v)
      ->endObject();
  }


  void KAnyType::deserializeIntoBuffer(Ref<ObjectToken> head,
      RefConst<Ontology> ontology, k_octet_t* buffer) const
  {
    head->validateClass(K"KAny");

    Ref<Token> token = head->next();
    token->validateType(AttributeToken::TYPE);

    Ref<AttributeToken> attrib = token.AS(AttributeToken);
    attrib->validateName(K"dataType");

    RefConst<KString> typeName = new KString(attrib->getValue());
    RefConst<KType> type = ontology->resolveType(typeName);

    if(type.isNull()) {
      throw KFException(K"Symbol does not reffer a valid type: " + *typeName);
    }

    Ref<KValue> v = type->instantiate();
    v->deserialize(token->next()->asObject(), ontology);

    token->next()->asEndObject()->validateClass(K"KAny");
  }


  void KAnyType::initializeBuffer(k_octet_t* buffer) const {
    KAny::initBuffer(buffer);
  }


  void KAnyType::cleanupBuffer(k_octet_t* buffer) const {
    KAny::cleaupBuffer(buffer);
  }


  void KAnyType::copyBuffer(const k_octet_t* src, k_octet_t* target) const {
    KAny::writeToBuffer(KAny::readFromBuffer(src), target);
  }


//\/ KNothingType /\///////////////////////////////////////////////////////////
    
  class KNothingType : public KType {
    public: KNothingType();
    public: bool isCastableTo(RefConst<KType> t) const;
    public: bool isAutomaticCastableTo(RefConst<KType> t) const;
    public: int  getSizeInOctets() const;
    public: bool isPrimitive() const;
    public: bool hasConstantSize() const;
    public: Ref<KValue> instantiate() const;

    public: void writeBufferToStream(const k_octet_t* buffer,
        Ref<OutputStream> stream) const;

    public: void writeStreamToBuffer(Ref<InputStream> stream,
        RefConst<Ontology> ontology, k_octet_t* buffer) const;

    public: void serializeBuffer(const k_octet_t* buffer,
        Ref<ObjectSerializer> serializer) const;

    public: void deserializeIntoBuffer(Ref<ObjectToken> head,
        RefConst<Ontology> ontology, k_octet_t* buffer) const;

    public: void copyBuffer(const k_octet_t* src, k_octet_t* target) const;
  };


  KNothingType::KNothingType() : KType(K"nothing") {
    // Nothing;
  }
  
    
  bool KNothingType::isCastableTo(RefConst<KType> t) const {
    return KType::NOTHING->equals(t);
  }
  
    
  bool KNothingType::isAutomaticCastableTo(RefConst<KType> t) const {
    return KType::NOTHING->equals(t);
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
  
    
  Ref<KValue> KNothingType::instantiate() const {
    return KValue::NOTHING;
  }


  void KNothingType::writeBufferToStream(const k_octet_t* buffer,
      Ref<OutputStream> stream) const
  {
    // Nothing;
  }


  void KNothingType::writeStreamToBuffer(Ref<InputStream> stream,
      RefConst<Ontology>, k_octet_t* buffer) const
  {
    // Nothing;
  }


  void KNothingType::serializeBuffer(const k_octet_t* buffer,
      Ref<ObjectSerializer> serializer) const
  {
    serializer->object(K"KNothing")->endObject();
  }


  void KNothingType::deserializeIntoBuffer(Ref<ObjectToken> head,
      RefConst<Ontology>, k_octet_t *buffer) const
  {
    head->validateClass(K"KNothing");
    head->next()->validateType(Token::END_OBJECT);
  }


  void KNothingType::copyBuffer(const k_octet_t* src, k_octet_t* target) const {
    // Nothing;
  }
  
  
//\/ KType /\//////////////////////////////////////////////////////////////////

  /** Runtime representation of KnoRBA **truth** type. */
  const StaticRef<KType> KType::TRUTH(new KTruthType());

  /** Runtime representation of KnoRBA **octet** type. */
  const StaticRef<KType> KType::OCTET(new KOctetType());

  /** Runtime representation of KnoRBA **integer** type. */
  const StaticRef<KType> KType::INTEGER(new KIntegerType());

  /** Runtime representation of KnoRBA **longint** type. */
  const StaticRef<KType> KType::LONGINT(new KLongintType());

  /** Runtime representation of KnoRBA **real** type. */
  const StaticRef<KType> KType::REAL(new KRealType());

  /** Runtime representation of KnoRBA **GUID** type. */
  const StaticRef<KType> KType::GUR(new KGurType());

  /** Runtime representation of KnoRBA **string** type. */
  const StaticRef<KType> KType::STRING(new KStringType());

  /** Runtime representation of KnoRBA **raw** type. */
  const StaticRef<KType> KType::RAW(new KRawType());

  /** Runtime representation of KnoRBA **any** type. */
  const StaticRef<KType> KType::ANY(new KAnyType());

  /** Runtime representation of KnoRBA **nothing** type. */
  const StaticRef<KType> KType::NOTHING(new KNothingType());

    
  /**
   * Constructor.
   *
   * @param name Type name
   */

  KType::KType(RefConst<UString> name) {
    _typeName = name;

    _hashCode = KString::generateHashFor(name->getOctets(),
        name->getOctetCount());
  }


  /**
   * Returns the type name.
   */
    
  RefConst<UString> KType::getTypeName() const {
    return _typeName;
  }
  

  /**
   * Returns the hashcode for type name (64-bit CityHash)
   */

  k_longint_t KType::getHashCode() const {
    return _hashCode;
  }


  /**
   * Checks if this object and the given argument represent the same type.
   */
    
  bool KType::equals(RefConst<KType> other) const {
    return other->_hashCode == _hashCode;
  }


  void KType::initializeBuffer(k_octet_t* buffer) const {
    // Nothing;
  }


  void KType::cleanupBuffer(k_octet_t* buffer) const {
    // Nothing;
  }


  void KType::copyBuffer(const k_octet_t* src, k_octet_t* target) const {
    memcpy(target, src, getSizeInOctets());
  }


  k_longint_t KType::getStreamSizeInOctets(const k_octet_t* buffer) const {
    return getSizeInOctets();
  }


  /**
   * Returns type description in KnoIS language.
   */
  
  RefConst<UString> KType::toKnois() const {
    return _typeName;
  }


  /**
   * Implements compatibility with kfoundation::Streamer interface.
   * Internally uses the output of toKnois() methos.
   */

  void KType::printToStream(Ref<OutputStream> stream) const {
    toKnois()->printToStream(stream);
  }


  RefConst<UString> KType::toString() const {
    return toKnois();
  }
      
} // namespace type
} // namespace knorba
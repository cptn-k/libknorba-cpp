/*---[KGridType.cpp]-------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : -
 |  Implements: knorba::type::KRecordType::*
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

// KFoundation
#include <kfoundation/Ref.h>
#include <kfoundation/StringPrintWriter.h>
#include <kfoundation/ObjectStreamReader.h>
#include <kfoundation/OutputStream.h>
#include <kfoundation/InputStream.h>
#include <kfoundation/ObjectStreamReader.h>
#include <kfoundation/ObjectSerializer.h>

// Internal
#include "../Ontology.h"
#include "KRecord.h"
#include "KString.h"
#include "KGridType.h"

// Self
#include "KRecordType.h"

namespace knorba {
namespace type {

//\/ KRecordType::Variable /\//////////////////////////////////////////////////
  
  KRecordType::Field::Field(RefConst<UString> name, RefConst<KType> type,
      k_integer_t byteOffset)
  : _name(name),
    _byteOffset(byteOffset),
    _type(type)
  {
    // Nothing;
  }
  

//\/ KRecordType /\////////////////////////////////////////////////////////////
  
// --- (DE)CONSTRUCTORS --- //
  
  /**
   * Constructor.
   *
   * @param name Type name
   */

  KRecordType::KRecordType(RefConst<UString> name)
  : KType(name),
    _size(0),
    _hasDynamicFields(false)
  {
    // Nothing;
  }


  /**
   * Shortcut constructor for records with signle fields. The type name and
   * field name is automatically infered from the type name of the field.
   *
   * @param fieldType Type of the record's sole field.
   */
  
  KRecordType::KRecordType(RefConst<KType> fieldType)
  : KType(fieldType->getTypeName() + "Record"),
    _size(0),
    _hasDynamicFields(false)
  {
    addField(K"field", fieldType);
  }

  
// --- METHODS --- //

  /**
   * Adds a new field to record type represented by this object.
   *
   * @param name Field name.
   * @param type Field type.
   * @return Pointer to self.
   */

  Ref<KRecordType> KRecordType::addField(RefConst<UString> name,
      RefConst<KType> type)
  {
    _offsetTable[_fields->getSize()] = _size;
    _fields->push(new Field(name, type, _size));
    
    if(type->hasConstantSize()) {
      _size += type->getSizeInOctets();
    } else {
      _size += sizeof(kf_uref_t);
      _hasDynamicFields = false;
    }

    if(type.ISA(KRecordType)) {
      if(type.AS(KRecordType)->hasDynamicFields()) {
        _hasDynamicFields = false;
      }
    }
    
    return this;
  }


  /**
   * Returns the number of fields of the record represented by this object.
   */
  
  k_octet_t KRecordType::getNumberOfFields() const {
    return _fields->getSize();
  }
  

  /**
   * Returns the name of the field at the given index.
   *
   * @param i An index between 0 and getNumberOfFields() - 1.
   */

  RefConst<UString>
  KRecordType::getNameOfFieldAtIndex(const k_octet_t i) const {
    return _fields->at(i)->_name;
  }


  /**
   * Returns the type of the field at the given index.
   *
   * @param i An index between 0 and getNumberOfFields() - 1.
   */
  
  RefConst<KType>
  KRecordType::getTypeOfFieldAtIndex(const k_octet_t i) const {
    return _fields->at(i)->_type;
  }


  /**
   * Returns the type of the field with the given name. Returns a null pointer
   * if such field does not exist.
   *
   * @param name The name of the field to retrieve type for.
   */
  
  RefConst<KType>
  KRecordType::getTypeOfFieldWithName(RefConst<UString> name) const {
    int index = getIndexForFieldWithName(name);
    if(index < 0) {
      return NULL;
    }
    return getTypeOfFieldAtIndex(index);
  }
  

  /**
   * Returns the index of the field with the given name. Returns -1 if there
   * is no such field.
   *
   * @param name The name of the field to retrieve index of.
   */

  k_integer_t KRecordType::getIndexForFieldWithName(RefConst<UString> name) const {
    for(int i = _fields->getSize() - 1; i >= 0; i--) {
      if(_fields->at(i)->_name->equals(name)) {
        return i;
      }
    }
    
    return KF_NOT_FOUND;
  }
  

  /**
   * Memory storage helper method. Returns the offset at which the field with
   * the given index is stored, calculated from the point at which the first
   * field is stored.
   *
   * @param index An Index between 0 to getNumberOfFields() - 1.
   */

  k_integer_t KRecordType::getOffsetOfFieldAtIndex(const k_octet_t index) const {
    return _fields->at(index)->_byteOffset;
  }


  /**
   * Returns `true` iff at least one the fields of the record represented by
   * this object has dynamic length. That is, at least one of the fields is
   * is of type `string`, `raw`, `grid`, or a `record` for which 
   * hasDynamicFields() returns `true`.
   */
  
  bool KRecordType::hasDynamicFields() const {
    return _hasDynamicFields;
  }
  

  const k_octet_t* const KRecordType::getOffsetTable() const {
    return _offsetTable;
  }


  /**
   * Returns a KGridType with cells of the type represented by this object.
   * 
   * @param nDims The number of dimensions of the resulting grid type.
   */
  
  Ref<KGridType> KRecordType::makeGridType(k_octet_t nDims) const {
    return new KGridType(this, nDims);
  }
  

  bool KRecordType::isCastableTo(RefConst<KType> t) const {
    if(!t.ISA(const KRecordType)) {
      return false;
    }
    
    RefConst<KRecordType> other = t.AS(KRecordType);
    if(other->getNumberOfFields() != getNumberOfFields()) {
      return false;
    }
    
    for(int i = _fields->getSize() - 1; i >= 0; i--) {
      if(!getTypeOfFieldAtIndex(i)->isCastableTo(
          other->getTypeOfFieldAtIndex(i)))
      {
        return false;
      }
    }
    
    return true;
  }
  
  
  bool KRecordType::isAutomaticCastableTo(RefConst<KType> t) const {
    if(!t.ISA(KRecordType)) {
      return false;
    }
    
    RefConst<KRecordType> other = t.AS(KRecordType);
    if(other->getNumberOfFields() != getNumberOfFields()) {
      return false;
    }
    
    for(int i = _fields->getSize() - 1; i >= 0; i--) {
      if(!getTypeOfFieldAtIndex(i)->isAutomaticCastableTo(
          other->getTypeOfFieldAtIndex(i)))
      {
        return false;
      }
    }
    
    return true;
  }
  
  
  bool KRecordType::equals(RefConst<KType> t) const {
    if(!t.ISA(KRecordType)) {
      return false;
    }
    
    RefConst<KRecordType> other = t.AS(KRecordType);
    if(other->getNumberOfFields() != getNumberOfFields()) {
      return false;
    }
    
    for(int i = _fields->getSize() - 1; i >= 0; i--) {
      if(!getTypeOfFieldAtIndex(i)->equals(other->getTypeOfFieldAtIndex(i))) {
        return false;
      }
    }
    
    return true;
  }
  
  
  k_integer_t KRecordType::getSizeInOctets() const {
    return _size;
  }
  
  
  bool KRecordType::isPrimitive() const {
    return false;
  }
  
  
  bool KRecordType::hasConstantSize() const {
    return true;
  }
  
  
  Ref<KValue> KRecordType::instantiate() const {
    return new KRecord(this);
  }
  
  
  RefConst<UString> KRecordType::toKnois() const {
    StringPrintWriter pw;

    pw << getTypeName() << " IS record(";
    
    int n = _fields->getSize();
    for(int i = 0; i < n; i++) {
      if(i > 0) {
        pw << ", ";
      }
      pw << _fields->at(i)->_type->getTypeName();
    }

    pw << ')';
    
    return pw.toString();
  }


  k_longint_t KRecordType::getStreamSizeInOctets(const k_octet_t* buffer) const
  {
    if(_hasDynamicFields) {
      k_longint_t s = 0;
      for(int i = _fields->getSize() - 1; i >= 0; i--) {
        s += _fields->at(i)->_type->getStreamSizeInOctets(
            buffer + _offsetTable[i]);
      }
      return s;
    }

    return _size;
  }


  void KRecordType::writeBufferToStream(const k_octet_t* buffer,
      Ref<OutputStream> stream) const
  {
    for(int i = _fields->getSize() - 1; i >= 0; i--) {
      _fields->at(i)->_type->writeBufferToStream(buffer + _offsetTable[i],
          stream);
    }
  }


  void KRecordType::writeStreamToBuffer(Ref<InputStream> stream,
      RefConst<Ontology> ontology, k_octet_t* buffer) const
  {
    for(int i = _fields->getSize() - 1; i >= 0; i--) {
      _fields->at(i)->_type->writeStreamToBuffer(stream, ontology,
        buffer + _offsetTable[i]);
    }
  }


  void KRecordType::serializeBuffer(const k_octet_t* buffer,
      Ref<ObjectSerializer> serializer) const
  {
    serializer->object(K"KRecord");
    serializer->attribute(K"type", toKnois());

    const int len = _fields->getSize();

    for(int i = 0; i < len; i++) {
      serializer->member(_fields->at(i)->_name);
      _fields->at(i)->_type->serializeBuffer(buffer + _offsetTable[i],
          serializer);
    }
    
    serializer->endObject();
  }


  void KRecordType::deserializeIntoBuffer(Ref<ObjectToken> head,
      RefConst<Ontology> ontology, k_octet_t* buffer) const
  {
    head->checkClass(K"KRecord");

    Ref<Token> token = head->next();

    int index = 0;
    while(!token->is(EndObjectToken::TYPE)) {
      token->validateType(ObjectToken::TYPE);
      Ref<ObjectToken> object = token.AS(ObjectToken);

      _fields->at(index)->_type->deserializeIntoBuffer(token.AS(ObjectToken),
          ontology, buffer + _offsetTable[index]);
      index++;

      token = token->next();
    }
    
    token->validateType(EndObjectToken::TYPE);
  }


} // namespace type
} // namesapce knorba
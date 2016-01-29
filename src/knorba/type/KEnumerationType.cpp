/*---[KEnumerationType.cpp]------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : -
 |  Implements: knorba::type::KEnumerationType::*
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
#include <kfoundation/Int.h>
#include <kfoundation/UString.h>
#include <kfoundation/StringPrintWriter.h>
#include <kfoundation/InputStream.h>
#include <kfoundation/OutputStream.h>
#include <kfoundation/IOException.h>
#include <kfoundation/ObjectSerializer.h>
#include <kfoundation/ObjectStreamReader.h>

// Internal
#include "KEnumeration.h"
#include "KString.h"

// Self
#include "KEnumerationType.h"


namespace knorba {
namespace type {

  using namespace kfoundation;

//\/ KEnumerationType::Item /\/////////////////////////////////////////////////

  KEnumerationType::Item::Item(kf_octet_t ordinal, RefConst<UString> label)
  : _ordinal(ordinal),
    _label(label)
  {
    // Nothing;
  }


//\/ KEnumerationType /\///////////////////////////////////////////////////////
  
// --- (DE)CONSTRUCTORS --- //

  /**
   * Constructor.
   *
   * @param name Name for the custom enumeration type.
   */

  KEnumerationType::KEnumerationType(RefConst<UString> name)
  : KType(name)
  {
    // Nothing;
  }


  
// --- METHODS --- //

  /**
   * Adds a member to this enumeration, associating it to an ordinal.
   *
   * @param ordinal Ordinal for the new member.
   * @param label Label for the new member.
   */

  Ref<KEnumerationType> KEnumerationType::addMember(
      k_octet_t ordinal, RefConst<UString> label)
  {
    _items->push(new Item(ordinal, label));
    return this;
  }


  /**
   * Adds a member to this enumeration, automatically assigning an ordinal to
   * the given label. The chosen ordinal number equals maximum ordinal
   * plus one.
   *
   * @param label Label for the new member.
   */
  
  Ref<KEnumerationType> KEnumerationType::addMember(RefConst<UString> label) {
    _items->push(new Item(getMaxOrdinal() + 1, label));
    return this;
  }


  /**
   * Returns the label associated with the given ordinal. Returns an empty
   * string if given an invalid ordinal.
   *
   * @param ordinal The ordinal to find label for.
   */
  
  RefConst<UString>
  KEnumerationType::getLabelForOrdinal(k_octet_t ordinal) const {
    for(int i = _items->getSize() - 1; i >= 0; i--) {
      if(_items->at(i)->_ordinal == ordinal) {
        return _items->at(i)->_label;
      }
    }
    //return NULL;
    throw KFException(K"Invalid ordinal: " + ordinal);
  }


  /**
   * Returns the ordinal associated with the given label. Returns -1 if there
   * is no such label.
   *
   * @param label The label to find the ordinal for.
   */
  
  k_octet_t
  KEnumerationType::getOrdinalForLabel(RefConst<UString> label) const {
    for(int i = _items->getSize() - 1; i >= 0; i--) {
      if(_items->at(i)->_label->equals(label)) {
        return _items->at(i)->_ordinal;
      }
    }
    //return KF_NOT_FOUND;
    throw KFException(K"Invalid label: " + label);
  }
  

  /**
   * Returns the number of members of this enumeration.
   */

  k_octet_t KEnumerationType::getNumberOfMembers() const {
    return _items->getSize();
  }


  k_octet_t KEnumerationType::getOrdinalForMemberAtIndex(const k_octet_t index)
  const
  {
    return _items->at(index)->_ordinal;
  }


  RefConst<UString> KEnumerationType::getLabelForMemberAtIndex(
      const k_octet_t index) const
  {
    return _items->at(index)->_label;
  }


  /**
   * Returns the ordinal for the enumeration value stored at the given memory
   * location.
   *
   * @param addr Pointer to a memory location storing an enumeration value.
   */
  
  k_octet_t KEnumerationType::getOrdinalForValueAtAddress(
      const k_octet_t* const addr) const
  {
    return *(addr);
  }


  /**
   * Returns the label for the enumeration value stored at the given memory
   * location.
   *
   * @param addr Pointer to a memory location storing an enumeration value.
   */
  
  RefConst<UString> KEnumerationType::getLabelForValueAtAddress(
      const k_octet_t* const addr) const
  {
    return getLabelForOrdinal(*(addr));
  }


  /**
   * Stores an enumeration value with the given ordinal at the given memory
   * location.
   *
   * @param addr Pointer to a preallocated memory location.
   * @param ordinal The ordinal of the value to store.
   */
  
  void KEnumerationType::setValueAtAddressWithOrdinal(k_octet_t* const addr,
      const k_octet_t ordinal) const
  {
    *(addr) = ordinal;
  }


  /**
   * Stores an enumeration value with the given label at the given memory
   * address.
   *
   * @param addr Pointer to a preallocated memory location.
   * @param label The label of the value to store.
   */
  
  void KEnumerationType::setValueAtAddressWithLabel(k_octet_t* const addr,
      RefConst<UString> label) const
  {
    *(addr) = getOrdinalForLabel(label);
  }


  bool KEnumerationType::isCastableTo(RefConst<KType> t) const {
    if(t->equals(KType::OCTET)) {
      return true;
    }
    
    if(t->equals(KType::INTEGER)) {
      return true;
    }
    
    if(t->equals(KType::LONGINT)) {
      return true;
    }
    
    if(t->equals(KType::REAL)) {
      return true;
    }
    
    return equals(t);
  }
  
  
  bool KEnumerationType::isAutomaticCastableTo(RefConst<KType> t) const {
    return isCastableTo(t);
  }
  
  
  bool KEnumerationType::equals(RefConst<KType> t) const {
    if(!t.ISA(KEnumerationType)) {
      return false;
    }
    
    RefConst<KEnumerationType> other = t.AS(KEnumerationType);
    
    kf_int32_t n = _items->getSize();

    if(other->_items->getSize() != n) {
      return false;
    }
    
    for(int i = 0; i < n; i++) {
      if(_items->at(i)->_ordinal != other->_items->at(i)->_ordinal) {
        return false;
      }
      if(!_items->at(i)->_label->equals(other->_items->at(i)->_label)) {
        return false;
      }
    }
    
    return true;
  }
  
  
  int KEnumerationType::getSizeInOctets() const {
    return KEnumeration::SIZE_IN_OCTETS;
  }
  
  
  bool KEnumerationType::isPrimitive() const {
    return false;
  }
  
  
  bool KEnumerationType::hasConstantSize() const {
    return true;
  }
  
  
  Ref<KValue> KEnumerationType::instantiate() const {
    return new KEnumeration(this);
  }
  
  
  RefConst<UString> KEnumerationType::toKnois() const {
    StringPrintWriter pw;

    pw << *getTypeName() << " IS enum(";

    kf_int32_t n = _items->getSize();
    for(kf_int32_t i = 0; i < n; i++) {
      if(i > 0) {
        pw << ", ";
      }
      Ref<Item> item = _items->at(i);
      pw << *item->_label << ':' << item->_ordinal;
    }

    pw << ')';

    return pw.toString();
  }


  void KEnumerationType::writeBufferToStream(const k_octet_t* buffer,
      Ref<OutputStream> stream) const
  {
    stream->write(*buffer);
  }


  void KEnumerationType::writeStreamToBuffer(Ref<InputStream> stream,
      RefConst<Ontology>, k_octet_t* buffer) const
  {
    k_integer_t v = stream->read();
    if(v == KF_NOT_FOUND) {
      throw IOException(K"Not enough data to read");
    }
    *buffer = (k_octet_t)v;
  }


  void KEnumerationType::serializeBuffer(const k_octet_t* buffer,
      Ref<ObjectSerializer> serializer) const
  {
    serializer->object(K"KEnumeration")
      ->attribute(K"value", getLabelForOrdinal(*buffer))
      ->endObject();
  }


  void KEnumerationType::deserializeIntoBuffer(Ref<ObjectToken> head,
      RefConst<Ontology>, k_octet_t* buffer) const
  {
    head->validateClass(K"KEnumeration");

    Ref<Token> token = head->next();
    token->validateType(AttributeToken::TYPE);

    Ref<AttributeToken> attrib = token.AS(AttributeToken);
    attrib->validateName(K"value");

    *buffer = (k_octet_t)getOrdinalForLabel(attrib->getValue());

    token = token->next();
    token->validateType(EndObjectToken::TYPE);
  }

} // namespace type
} // namespace knorba
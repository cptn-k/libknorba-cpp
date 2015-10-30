/*---[KRecord.h]-----------------------------------------------m(._.)m--------*\
 |
 |  Project   : KnoRBA C++ Library
 |  Declares  : knorba::type::KRecord::*
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

#ifndef KNORBA_TYPE_KRECORD
#define KNORBA_TYPE_KRECORD

// KFoundation
#include <kfoundation/ManagedArray.h>
#include <kfoundation/Tuple.h>

// Internal
#include "KRecordType.h"

// Super
#include "KDynamicValue.h"

#define ENUMERAND(X, Y)
#define ENUMERATE_OVER_PRIMITIVE_TYPES \
  ENUMERAND(Truth, k_truth_t) \
  ENUMERAND(Octet, k_octet_t) \
  ENUMERAND(Integer, k_integer_t) \
  ENUMERAND(Longint, k_longint_t) \
  ENUMERAND(Real, k_real_t) \
  ENUMERAND(Guid, k_guid_t)
#undef ENUMERAND

#define ENUMERAND(X)
#define ENUMERATE_OVER_DYNAMIC_TYPES \
  ENUMERAND(String) \
  ENUMERAND(Raw) \
  ENUMERAND(Grid) \
  ENUMERAND(Any)
#undef ENUMERAND

using namespace kfoundation;

namespace knorba {
  class Runtime;
}

namespace knorba {
namespace type {

  #define ENUMERAND(X) class K ## X;
  ENUMERATE_OVER_DYNAMIC_TYPES
  #undef ENUMERAND
  
//\/ KRecord /\////////////////////////////////////////////////////////////////

  /**
   * Wrapper class and C++ representation of KnoBRA `record`. A `record` is 
   * an ordered sequence of fields of various types.
   *
   * This class functions in two ways: it can either store values internally,
   * or wrap around a record stored by another KRecord or KGrid object, and
   * provide access to it. See `getRecord(k_octet_t, PPtr<KRecord>)`,
   * `getRecord(const string&, PPtr<KRecord>)`, and `KGrid` for more
   * information.
   *
   * For user's convenience getXXX() and setXXX() methods are overloaded for
   * every KnoRBA type. Usage:
   *
   *     Ptr<KRecordType> dateType = new KRecordType("com.mydomain.Date");
   *     dateType->addField("year", KType::INTEGER)
   *             ->addField("month", KType::OCTET)
   *             ->addField("day", KType::OCTET);
   *     Ptr<KRecord> aDate = new KRecord(dateType);
   *     aDate->setInteger("year", 1981);
   *     aDate->setOctet("month", 6);
   *     aDate->setOctet("day", 2);
   *
   * Getter and setters that receive field index instead of field name are
   * faster:
   *
   *     aDate->setInteger(0, 1981);
   *     aDate->setOctet(1, 6);
   *     aDate->setOctet(2, 2);
   *
   * Alternatively, a set of method that return type-wrappers can be used.
   * These methods are generally slower than above alternatives:
   *
   *     aDate->getInteger("year")->set(1981);
   *     aDate->getOctet("month")->set(6);
   *     aDate->getOctet(2)->set(2);
   *
   * There two small exceptions. For setting enumeration values, there are
   * separate methods for setting by ordinal or by label.
   *
   * The other exception is when accessing a field of type record.
   *
   *     Ptr<KRecordType> logType = new KRecordType("com.mydomain.Log");
   *     logType->addField("date", dateType.AS(KType))
   *            ->addField("note", KType::STRING);
   *     Ptr<KRecord> log = new KRecord(logType);
   *
   * Then, inner fields can be changes as
   *
   *     log->getRecord("date")->setInteger(0, 1981);
   *
   * or, for accessing multiple fields:
   *
   *     Ptr<KRecord> date = log->getRecord("date");
   *     date->setInteger("year", 1981);
   *     date->setOctet("month", 6);
   *     date->setOctet("day", 2);
   *
   * Occasionally, it helps to create a wrapper for the inner record in advance:
   *
   *     Ptr<KRecord> date = new KRecord(dateType);
   *     log->getRecord("date", date);
   *     cout << *date << endl;
   *
   * @note **IMPORTANT.** Field accessor methods do not perform type checking
   *       for performance reasons. Take extereme caution to use the correct
   *       method. It is very easy to harm internal pointers and other values
   *       if a wrong accessor method is used.
   *
   * @headerfile KRecord.h <knorba/type/KRecord.h>
   */

  class KRecord : public KDynamicValue {
    
  // --- FIELDS --- //
    
    private: Ptr<KRecordType>   _type;
    private: const k_octet_t*   _offsetTable;
    private: k_octet_t          _nFields;
    private: k_octet_t*         _data;
    private: Ptr<KValue>*       _fields;
    private: PPtr<KDynamicValue> _owner;
    private: k_longint_t        _offset;
    private: bool               _hasDynamicFields;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: KRecord(PPtr<KRecordType> type);
    public: KRecord(PPtr<KGrid> grid);
    public: KRecord(PPtr<KRecord> record, const k_octet_t fieldIndex);
    public: KRecord(PPtr<KRecord> record, const string& fieldName);    
    public: ~KRecord();
    
    
  // --- METHODS --- //
    
    private: bool isInitialized();
    private: void setInitialized();
    private: void bindToRecord(PPtr<KRecord> record, const k_octet_t fieldIndex);
    private: void makeFields();
    private: void makeDynamicFields();
    private: void makeField(k_octet_t index);
    private: void makeDynamicField(k_octet_t index);
    
    public: PPtr<KValue> field(const k_octet_t index) const;
    public: PPtr<KValue> field(const string& name) const;
    
    #define ENUMERAND(X) \
      public: PPtr<K ## X> get ## X(const k_octet_t index = 0) const;\
      public: PPtr<K ## X> get ## X(const string& name) const;\
      public: void set ## X(const k_octet_t index, PPtr<K ## X> value);\
      public: void set ## X(const string& name, PPtr<K ## X> value);\
      public: void set ## X(PPtr<K ## X> value);
    ENUMERATE_OVER_DYNAMIC_TYPES
    #undef ENUMERAND
    
    #define ENUMERAND(X, Y) \
      public: Y get ## X(const k_octet_t index = 0) const;\
      public: Y get ## X(const string& name) const;\
      public: void set ## X(const k_octet_t index, const Y value);\
      public: void set ## X(const Y value);\
      public: void set ## X(const string& name, const Y value);
    ENUMERATE_OVER_PRIMITIVE_TYPES
    #undef ENUMERAND
    
    public: void setEnumeration(const k_octet_t index, const k_octet_t ordinal);
    public: void setEnumeration(const k_octet_t index, const string& label);
    public: void setEnumeration(const k_octet_t ordinal);
    public: void setEnumeration(const string& label);
    public: string getEnumerationLabel(const k_octet_t index = 0) const;
    public: k_octet_t getEnumerationOrdinal(const k_octet_t index = 0) const;
    
    public: PPtr<KRecord> getRecord(k_octet_t index) const;
    public: PPtr<KRecord> getRecord(const string& name) const;
    public: void getRecord(k_octet_t index, PPtr<KRecord> wrapper) const;
    public: void getRecord(const string& name, PPtr<KRecord> wrapper) const;
    
    public: void wrap(PPtr<KDynamicValue> target, k_longint_t offset);
    public: void initDynamicFields();
    public: void cleanupDynamicFields();
    public: void setRuntime(Runtime& rt);
    
    // Inline Members //
    public: inline void setOffset(k_longint_t offset);
    
    // Template Members //
    public: template<typename T>
      inline PPtr<T> field(const k_octet_t index) const;
    
    public: template<typename T>
      inline PPtr<T> field(const string& name) const;
    
    // Inherited from KDynamicValue
    public: k_octet_t* getBaseAddress() const;
    
    // Inherited from KDynamicValue::KValue
    public: void set(PPtr<KValue> other);
    public: PPtr<KType> getType() const;
    public: k_longint_t getTotalSizeInOctets() const;
    public: void readFromBinaryStream(PPtr<InputStream> input);
    public: void writeToBinaryStream(PPtr<OutputStream> output) const;

    // Inherited from KValue::StreamDeserializer
    public: void deserialize(PPtr<ObjectToken> headToken);
    
    // Inherited from KValue::SerializingStreamer
    public: void serialize(PPtr<ObjectSerializer> builder) const;
    
  };

  
  inline void KRecord::setOffset(k_longint_t offset) {
    _offset = offset;
    if(_hasDynamicFields) {
      makeDynamicFields();
    }
  }
  
  
  template<typename T>
  inline PPtr<T> KRecord::field(const k_octet_t index) const {
    return _fields[index].AS(T);
  }
  
  
  template<typename T>
  inline PPtr<T> KRecord::field(const string& name) const {
    return _fields[_type->getIndexForFieldWithName(name)].AS(T);
  }
  
} // namespace type
} // namespace knorba

#endif /* defined(KNORBA_TYPE_KRECORD) */

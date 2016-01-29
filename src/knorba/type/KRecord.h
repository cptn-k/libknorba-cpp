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

// Internal
#include "KRecordType.h"
#include "KValue.h"

#define ENUMERAND(X, Y, N)
#define ENUMERATE_OVER_PRIMITIVE_TYPES \
  ENUMERAND(Truth  , k_truth_t  , 1) \
  ENUMERAND(Octet  , k_octet_t  , 2) \
  ENUMERAND(Integer, k_integer_t, 3) \
  ENUMERAND(Longint, k_longint_t, 4) \
  ENUMERAND(Real   , k_real_t   , 5) \
  ENUMERAND(Gur    , k_gur_t    , 6)
#undef ENUMERAND

#define ENUMERAND(X, Y, N)
#define ENUMERATE_OVER_SIZABLE_TYPES \
  ENUMERAND(String, KString, 7) \
  ENUMERAND(Raw   , KRaw   , 8) \
  ENUMERAND(Any   , KAny   , 9)
#undef ENUMERAND

#define ENUMERAND(X, Y, N)
#define ENUMERATE_OVER_COMPLEX_TYPES \
  ENUMERAND(Enumeration, KEnumeration, 10) \
  ENUMERAND(Record     , KRecord     , 11) \
  ENUMERAND(Grid       , KGrid       , 12)
#undef ENUMERAND

namespace kfoundation {
  template<typename T> class RefArray;
}

namespace knorba {
namespace type {

  using namespace kfoundation;

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


  class KRecord : public KValue {
    
  // --- FIELDS --- //
    
    private: RefConst<KRecordType> _type;
    private: const k_octet_t* _offsetTable;
    private: k_octet_t  _nFields;
    private: k_octet_t* _typeTable;
    private: k_octet_t* _data;

    
  // --- (DE)CONSTRUCTORS --- //

    protected: KRecord(RefConst<KRecordType> type, bool unused);
    public: KRecord(RefConst<KRecordType> type);
    public: ~KRecord();
    
    
  // --- METHODS --- //

    private: void initTypeTable();

    public: virtual k_octet_t* getBuffer();
    public: virtual const k_octet_t* getBuffer() const;

    public: Ref<KValue> getField(const k_octet_t index);
    public: RefConst<KValue> getField(const k_octet_t index) const;
    
    public: Ref<KValue> getField(RefConst<UString> name);
    public: RefConst<KValue> getField(RefConst<UString> name) const;

    #define ENUMERAND(X, Y, N) \
      public: Y get ## X(const k_octet_t index = 0) const;\
      public: Y get ## X(RefConst<UString> name) const;\
      public: void set ## X(const k_octet_t index, const Y value);\
      public: void set ## X(const Y value);\
      public: void set ## X(RefConst<UString> name, const Y value);

    ENUMERATE_OVER_PRIMITIVE_TYPES
    #undef ENUMERAND
    
    public: void set(const KRecord& r);
    public: void reset();

    // Inherited from KValue
    public: void set(RefConst<KValue> other);
    public: RefConst<KType> getType() const;

  };

} // namespace type
} // namespace knorba

#endif /* defined(KNORBA_TYPE_KRECORD) */
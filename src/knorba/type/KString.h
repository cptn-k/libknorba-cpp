//
//  KString.h
//  CellMonitorTest-XCodeWrapper
//
//  Created by Hamed KHANDAN on 7/18/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

// Used code from boyers/unicode at GitHub

#ifndef KNORBA_TYPE_KSTRING
#define KNORBA_TYPE_KSTRING

// Super
#include "KValue.h"

#define KS(X) (Ptr<KString>(new KString(X)))


namespace knorba {
namespace type {
  
  using namespace std;
  using namespace kfoundation;
    
  /*
   
   +------- Header -------+
     8  bytes
   +----------+-----------+----------------+
   | nOctets  | hash code | data ...       |
   +----------+-----------+----------------+
   
   */

  
//\/ KString /\////////////////////////////////////////////////////////////////
  
  class KString : public KValue {
    
  // --- FIELDS --- //
    
    private: k_octet_t*  _buffer;
    
    
  // --- STATIC METHODS --- //
    
    public: static k_longint_t generateHashFor(const wstring& ws);
    public: static k_longint_t generateHashFor(const string& s);
    public: static k_longint_t generateHashFor(const k_octet_t* s, k_longint_t size);

    
  // --- (DE)CONSTRUCTORS --- //
    
    public: KString();
    public: KString(const string& str);
    public: KString(const wstring& str);
    public: ~KString();
    
    
  // --- METHODS --- //
    
    private: void reallocateBuffer(const k_longint_t nOctets);
    private: k_octet_t* getStringHead() const;
    private: void setHashCode(const k_longint_t code);
    
    public: virtual inline k_octet_t* getBuffer() const;
    public: virtual inline void setBuffer(k_octet_t* const addr);
    
    public: k_longint_t getHashCode() const;
    public: k_longint_t getNOctets() const;
    public: void set(const string& str);
    public: void set(const wstring& str);
    public: k_longint_t getNCodePoints() const;
    public: wstring toWString() const;
    public: const char* getUtf8CStr() const;
    public: string toUtf8String() const;
    public: wchar_t getCodePointAt(const k_longint_t index) const;
    public: k_octet_t getOctetAt(const k_longint_t index) const;
    public: bool equals(const wstring& ws) const;
    public: bool equals(const string& s) const;
    public: bool equals(PPtr<KString> str) const;
    public: bool equals(const ManagedObject& obj) const;
    public: bool hashEquals(const k_longint_t& hash) const;
    
    // Inherited from KValue
    public: PPtr<KType> getType() const;
    public: k_longint_t getTotalSizeInOctets() const;
    public: void readFromBinaryStream(PPtr<InputStream> input);
    public: void writeToBinaryStream(PPtr<OutputStream> output) const;
    public: void readFromObjectStream(PPtr<ObjectToken> headToken);
    public: void set(PPtr<KValue> other);
    
    // Inherited from KValue::SerializingStreamer
    public: void serialize(PPtr<ObjectSerializer> builder) const;
    
    // Inherited from KValue::SerializingStreamer::Streamer
    public: void printToStream(ostream& os) const;
    
  };
  
  
  inline k_octet_t* KString::getBuffer() const {
    return _buffer;
  }
  
  
  inline void KString::setBuffer(k_octet_t* const addr) {
    if( NOT_NULL(_buffer) ) {
      delete[] _buffer;
    }
    
    _buffer = addr;
  }
  

} // type
} // knorba

#endif /* defined(KNORBA_TYPE_KSTRING) */

//
//  KRaw.h
//  CellMonitorTest-XCodeWrapper
//
//  Created by Hamed KHANDAN on 7/18/14.
//  Copyright (c) 2014 RIKEN AICS Advanced Visualization Research Team. All rights reserved.
//

#ifndef KNORBA_TYPE_KRAW
#define KNORBA_TYPE_KRAW

// Internal
#include "KValue.h"


namespace kfoundation {
  class BufferInputStream;
}


namespace knorba {
namespace type {

//\/ KRaw /\///////////////////////////////////////////////////////////////////
  
  
  /*
           Header
          8  bytes   Data
        +----------+-----------------------+
        | nOctets  | ...                   |
        +----------+-----------------------+
   
   */
  
  class KRaw : public KValue {
        
  // --- FIELDS --- //
    
    private: k_octet_t*   _buffer;
    
    
  // --- (DE)CONSTRUCTORS --- //
    
    public: KRaw();
    public: ~KRaw();
    
    
  // --- METHODS --- //
    
    private: void reallocateBuffer(const k_longint_t size);
    
    protected: virtual inline k_octet_t* getBuffer() const;
    protected: virtual inline void setBuffer(k_octet_t* const addr);
    
    public: void set(const k_octet_t* data, const k_longint_t size);
    public: const k_octet_t* getData() const;
    public: k_longint_t getNOctets() const;
    public: void readDataFromFile(PPtr<Path> path);
    public: void writeDataToFile(PPtr<Path> path);
    public: Ptr<BufferInputStream> getDataAsInputStream() const;
    
    // Inherited from KValue
    public: void set(PPtr<KValue> other);
    public: PPtr<KType> getType() const;
    public: k_longint_t getTotalSizeInOctets() const;
    public: void readFromBinaryStream(PPtr<InputStream> input);
    public: void writeToBinaryStream(PPtr<OutputStream> output) const;
    public: void readFromObjectStream(PPtr<ObjectToken> headToken);
    
    // Inherited from KValue::SerializingStreamer
    void serialize(PPtr<ObjectSerializer> builder) const;
    
  };
  
  
  inline k_octet_t* KRaw::getBuffer() const {
    return _buffer;
  }
  
  
  inline void KRaw::setBuffer(k_octet_t* const addr) {
    if( NOT_NULL(_buffer) ) {
      delete[] _buffer;
    }
    
    _buffer = addr;
  }
  
  
} // namespace type
} // namespace knorba

#endif /* defined(KNORBA_TYPE_KRAW) */

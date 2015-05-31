//
//  tester.cpp
//  KnoRBA
//
//  Created by Kay Khandan on 10/28/14.
//  Copyright (c) 2014 Kay Khandan. All rights reserved.
//

#include <cmath>
#include <cassert>

#include <kfoundation/System.h>
#include <kfoundation/Logger.h>
#include <kfoundation/MemoryManager.h>
#include <kfoundation/MasterMemoryManager.h>
#include <kfoundation/BufferInputStream.h>
#include <kfoundation/BufferOutputStream.h>
#include <kfoundation/FileInputStream.h>
#include <kfoundation/FileOutputStream.h>

#include <knorba/type/all.h>

#define KOCT (k_octet_t*)

using namespace std;
using namespace kfoundation;
using namespace knorba::type;


bool tracing = false;

void trace() {
  tracing = true;
  System::getMasterMemoryManager()->getManagerAtIndex(0)->trace(0);
}


void untrace() {
  tracing = false;
  System::getMasterMemoryManager()->getManagerAtIndex(0)->untrace();
}


void memorystat() {
  bool resumeTrace = tracing;
  if(tracing) {
    untrace();
  }
  
  LOG << *System::getMasterMemoryManager()->getManagerAtIndex(0) << EL;
  
  if(resumeTrace) {
    trace();
  }
}

void memorystat(string str) {
  LOG << "<<" << str << ">> " << *System::getMasterMemoryManager()->getManagerAtIndex(0) << EL;
}


void testTruth() {
  LOG << "Testing \"truth\"" << EL;
  
  Ptr<KTruth> val = new KTruth();
  LOG << "val = " << *val << EL;
  
  val->set(T);
  LOG << "val->set(T): " << *val << EL;
  assert(val->get() == T);
  
  Ptr<KTruth> val2 = KType::TRUTH->instantiate().AS(KTruth);
  LOG << "val2 = " << *val2 << EL;
  
  val2->set(X);
  LOG << "val2->set(X): " << *val2 << EL;
  assert(val2->get() == X);
  
  val2->set(val.AS(KValue));
  LOG << "val2->set(val): " << *val2 << EL;
  assert(val2->get() == T);
}


void testInteger() {
  LOG << "Testing \"integer\"" << EL;
  
  Ptr<KInteger> val = new KInteger(123);
  LOG << "val = " << *val << EL;
  
  val->set(321);
  LOG << "val->set(321): " << *val << EL;
  assert(val->get() == 321);
  
  Ptr<KInteger> val2 = KType::INTEGER->instantiate().AS(KInteger);
  LOG << "val2 = " << *val2;
  
  val2->set(9999);
  LOG << "val2->set(9999): " << *val2 << EL;
  assert(val2->get() == 9999);
  
  val2->set(val.AS(KValue));
  LOG << "val2->set(val): " << *val2 << EL;
  assert(val2->get() == 321);
}


void testLongint() {
  LOG << "Testing \"longint\"" << EL;
  
  Ptr<KLongint> val = new KLongint(123);
  LOG << "val = " << *val << EL;
  
  val->set(321);
  LOG << "val->set(321): " << *val << EL;
  assert(val->get() == 321);
  
  Ptr<KLongint> val2 = KType::LONGINT->instantiate().AS(KLongint);
  LOG << "val2 = " << *val2;
  
  val2->set(9999);
  LOG << "val2->set(9999): " << *val2 << EL;
  assert(val2->get() == 9999);
  
  val2->set(val.AS(KValue));
  LOG << "val2->set(val): " << *val2 << EL;
  assert(val2->get() == 321);
}


void testReal() {
  LOG << "Testing \"real\"" << EL;
  
  Ptr<KReal> val = new KReal(456.321);
  LOG << "val = " << *val << EL;
  
  val->set(123.876);
  LOG << "val->set(123.876): " << *val << EL;
  assert(val->get() == 123.876);
  
  Ptr<KReal> val2 = KType::REAL->instantiate().AS(KReal);
  LOG << "val2 = " << *val2;
  
  val2->set(9999.9999);
  LOG << "val2->set(9999.9999): " << *val2 << EL;
  assert(val2->get() == 9999.9999);
  
  val2->set(val.AS(KValue));
  LOG << "val2->set(val): " << *val2 << EL;
  assert(val2->get() == 123.876);
}


void testGlobalUID() {
  LOG << "Testing \"guid\"" << EL;
  
  k_guid_t v = KGlobalUid::zero();
  KGlobalUid::randomizeAppId(v);
  KGlobalUid::randomizeKey(v);
  v.lid = 322;
  
  Ptr<KGlobalUid> val = new KGlobalUid();
  LOG << "val = " << *val << EL;
  
  val->set(v);
  LOG << "val->set(random): " << *val << EL;
  k_guid_t tmp = val->get();
  assert(memcmp(&tmp, &v, sizeof(k_guid_t)) == 0);
  
  Ptr<KGlobalUid> val2 = KType::GUID->instantiate().AS(KGlobalUid);
  LOG << "val2 = " << *val2 << EL;
  
  val2->set(val.AS(KValue));
  LOG << "val2->set(val): " << *val2 << EL;
  tmp = val2->get();
  assert(memcmp(&tmp, &v, sizeof(k_guid_t)) == 0);
}


void testRaw() {
  LOG << "Testing \"raw\"" << EL;
  
  k_octet_t data[12];
  
  for(int i = 0; i < 12; i++) {
    data[i] = i;
  }
  
  Ptr<KRaw> val = new KRaw();
  val->set(data, 12);
  
  LOG << "nOctets = " << val->getNOctets() << EL;
  assert(val->getNOctets() == 12);
  
  val->getData()[2] = 20;
  
  Logger::Stream& stream = LOG;
  for(int i = 0; i < val->getNOctets(); i++) {
    stream << (int)val->getData()[i] << ' ';
    if(i == 2) {
      assert(val->getData()[i] == 20);
    } else {
      assert(val->getData()[i] == i);
    }
  }
  stream << EL;
  
  Ptr<KRaw> val2 = KType::RAW->instantiate().AS(KRaw);
  val2->set(val.AS(KValue));
  
  Logger::Stream& stream2 = LOG;
  for(int i = 0; i < val2->getNOctets(); i++) {
    stream2 << (int)val2->getData()[i] << ' ';
    if(i == 2) {
      assert(val2->getData()[i] == 20);
    } else {
      assert(val2->getData()[i] == i);
    }
  }
  stream2 << EL;
}


void testString() {
  LOG << "Testing \"string\"" << EL;
  
  Ptr<KString> val = new KString();
  LOG << "val = \"" << *val << "\" (hash: " << val->getHashCode() << ")" << EL;
  
  val->set("Hello String!");
  LOG << "val->set(\"Hello String!\"): \"" << *val << "\" (hash: " << val->getHashCode() << ")" << EL;
  assert(val->toUtf8String() == "Hello String!");
  
  Ptr<KString> val2 = KType::STRING->instantiate().AS(KString);
  LOG << "val2 = \"" << *val2 << "\" (hash: " << val2->getHashCode() << ")" << EL;
  
  val2->set(L"日本語は");
  LOG << "val2->set(): " << *val2 << EL;
  
  val2->set(val.AS(KValue));
  LOG << "val2->set(val): \"" << *val2 << "\" (hash: " << val2->getHashCode() << ")" << EL;
  assert(val2->toUtf8String() == "Hello String!");
  assert(val2->getHashCode() == val->getHashCode());
}


int main() {

//  for(int i = 0; i < 1000; i++) {
    testTruth();
    testInteger();
    testLongint();
    testReal();
    testGlobalUID();
    testRaw();
    testString();
//    cout << "-----" << i << "-----" << endl;
//  }
  
  memorystat();
}

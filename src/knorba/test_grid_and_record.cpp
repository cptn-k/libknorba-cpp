//
//  test_grid_and_record.cpp
//  KnoRBA
//
//  Created by Hamed KHANDAN on 1/20/15.
//  Copyright (c) 2015 Kay Khandan. All rights reserved.
//

#include <cassert>

#include <kfoundation/Ptr.h>
#include <kfoundation/FileOutputStream.h>
#include <kfoundation/FileInputStream.h>
#include <kfoundation/RangeIterator.h>
#include <kfoundation/Timer.h>
#include <kfoundation/Path.h>
#include <kfoundation/Logger.h>
#include <kfoundation/MasterMemoryManager.h>
#include <knorba/type/all.h>

using namespace std;
using namespace knorba::type;

k_guid_t makeRandomGuid() {
  k_guid_t guid;
  KGlobalUid::randomizeAppId(guid);
  KGlobalUid::randomizeKey(guid);
  return guid;
}

void testSimpleKRecord() {
  LOG << "Testing simple record" << EL;
  Ptr<KEnumerationType> enumType = new KEnumerationType("EnumType");
  
  enumType->addMember(0, "carrot")
          ->addMember(1, "cow")
          ->addMember(2, "transformer");
  
  Ptr<KRecordType> t = new KRecordType("SimpleRecord");
  t->addField("intField", KType::INTEGER)
   ->addField("longintField", KType::LONGINT)
   ->addField("truthField", KType::TRUTH)
   ->addField("realField", KType::REAL)
   ->addField("guidField", KType::GUID)
   ->addField("enumField", enumType.AS(KType));
  
  Ptr<KRecord> instance1 = new KRecord(t);
  
  // Test 1: set using field number
  instance1->setInteger(0, 122345678);
  instance1->setLongint(1, 987654321);
  instance1->setTruth(2, X);
  instance1->setReal(3, 1234.5678);
  instance1->setGuid(4, makeRandomGuid());
  instance1->setEnumeration(5, "carrot");
  
  LOG << *instance1 << EL;
  
  assert(instance1->getInteger(0) == 122345678);
  assert(instance1->getLongint(1) == 987654321);
  assert(instance1->getTruth(2) == X);
  assert(instance1->getReal(3) == 1234.5678);
  assert(instance1->getEnumerationOrdinal(5) == 0);
  assert(instance1->getEnumerationLabel(5) == "carrot");
  
  
  Ptr<KRecord> instance2 = t->instantiate().AS(KRecord);
  instance2->set(instance1.AS(KValue));
  
  assert(instance2->getInteger(0) == 122345678);
  assert(instance2->getLongint(1) == 987654321);
  assert(instance2->getTruth(2) == X);
  assert(instance2->getReal(3) == 1234.5678);
  assert(instance2->getEnumerationOrdinal(5) == 0);
  assert(instance2->getEnumerationLabel(5) == "carrot");
  
  instance2->setTruth(2, F);
  instance2->field<KEnumeration>("enumField")->set("cow");
  instance2->field<KLongint>(1)->set(112211);
  instance2->field(3).AS(KReal)->set(3.14);
  
  assert(instance2->field<KInteger>("intField")->get() == 122345678);
  assert(instance2->field<KLongint>("longintField")->get() == 112211);
  assert(instance2->field<KTruth>("truthField")->get() == F);
  assert(instance2->field<KReal>("realField")->get() == 3.14);
  assert(instance2->field<KEnumeration>("enumField")->getOrdinal() == 1);
  assert(instance2->field<KEnumeration>("enumField")->getLabel() == "cow");
  
  LOG << *instance2 << EL;
}


void testKRecordWithSubrecord() {
  LOG << "Testing record with sub-record" << EL;
  
  Ptr<KRecordType> subRecordType = new KRecordType("SubRecord");
  subRecordType->addField("integerField", KType::INTEGER)
               ->addField("longintField", KType::LONGINT)
               ->addField("truthField", KType::TRUTH)
               ->addField("realField", KType::REAL);
  
  Ptr<KRecordType> recordType = new KRecordType("Record");
  recordType->addField("realField", KType::REAL)
            ->addField("subRecord1", subRecordType.AS(KType))
            ->addField("truthField", KType::TRUTH)
            ->addField("subRecord2", subRecordType.AS(KType));
  
  Ptr<KRecord> record = new KRecord(recordType);
  
  record->setReal(0, 0);
  record->setTruth(2, F);
  
  PPtr<KRecord> subRec = record->field<KRecord>("subRecord1");
  
  subRec->setInteger(0, 12);
  subRec->setLongint(1, 24);
  subRec->setTruth(2, T);
  subRec->setReal(3, 12.23);
  
  subRec = record->field<KRecord>("subRecord2");
  subRec->setInteger(0, 13);
  subRec->setLongint(1, 26);
  subRec->setTruth(2, X);
  subRec->setReal(3, 56.78);
  
  assert(record->getReal(0) == 0);
  assert(record->getRecord(1)->getInteger(0) == 12);
  assert(record->getRecord(1)->getLongint(1) == 24);
  assert(record->getRecord(1)->getTruth(2) == T);
  assert(record->getRecord(1)->getReal(3) == 12.23);
  assert(record->getRecord("subRecord2")->getInteger("integerField") == 13);
  assert(record->getRecord("subRecord2")->getLongint("longintField") == 26);
  assert(record->getRecord("subRecord2")->field<KTruth>("truthField")->get() == X);
  assert(record->getRecord("subRecord2")->field<KReal>(3)->get() == 56.78);
  
  LOG << "record: " << *record << EL;
  
  Ptr<KRecord> wrapper1 = new KRecord(subRecordType);
  Ptr<KRecord> wrapper2 = new KRecord(subRecordType);
  
  record->getRecord(1, wrapper1);
  record->getRecord("subRecord2", wrapper2);
  
  LOG << "wrapper1: " << *wrapper1 << EL;
  LOG << "wrapper2: " << *wrapper2 << EL;
  
  k_real_t tmp = wrapper1->getReal(3);
  wrapper1->setReal(3, wrapper2->getReal("realField"));
  wrapper2->setReal("realField", tmp);
  
  assert(record->field<KRecord>(1)->getReal(3) == 56.78);
  assert(record->field<KRecord>(3)->getReal(3) == 12.23);
  
  assert(record->getReal(0) == 0);
  assert(record->getTruth(2) == F);
  
  LOG << *record << EL;
}


void testKRecordWithDynamicFields() {
  LOG << "Testing record with dynamic fields" << EL;
  
  Ptr<KGridType> gridType = new KGridType(new KRecordType(KType::REAL), 2);
  Ptr<KRecordType> t = new KRecordType("Record");
  t->addField("rawField", KType::RAW)
   ->addField("truthField1", KType::TRUTH)
   ->addField("stringField", KType::STRING)
   ->addField("truthField2", KType::TRUTH)
   ->addField("gridField", gridType.AS(KType));
  
  Ptr<KRecord> rec = new KRecord(t);
  
  LOG << "rec: " << *rec << EL;
  
  k_octet_t data[12];
  for(int i = 0; i < 12; i++) {
    data[i] = i;
  }
  rec->field<KRaw>(0)->set(data, 12);
  
  rec->field<KString>(2)->set("Hello Dynamic Field!");
  
  PPtr<KGrid> g = rec->field<KGrid>(4);
  g->resetWithSize(Tuple2D(2, 2));
  
  Ptr<KRecord> wrapper = new KRecord(g);
  g->at(Tuple2D(0, 0), wrapper)->setReal(0, 1.1);
  g->at(Tuple2D(1, 0), wrapper)->setReal(0, 2.2);
  g->at(Tuple2D(0, 1), wrapper)->setReal(0, 3.3);
  g->at(Tuple2D(1, 1), wrapper)->setReal(0, 4.4);
  LOG << "rec: " << *rec << EL;
}


void testKGrid() {
  Ptr<KEnumerationType> enumType = new KEnumerationType("EnumType");
  enumType->addMember(0, "carrot")
    ->addMember(1, "cow")
    ->addMember(2, "transformer");
  
  Ptr<KRecordType> rt = new KRecordType("Record");
  rt->addField("realField", KType::REAL)
    ->addField("intField", KType::INTEGER)
    ->addField("stringField", KType::STRING)
    ->addField("enumField", enumType.AS(KType));
  
  Ptr<KGridType> gt = new KGridType(rt, 2);
  
  Ptr<KGridBasic> g = new KGridBasic(gt);
  Ptr<KRecord> record = new KRecord(g.AS(KGrid));
  
  g->resetWithSize(Tuple2D(27, 33));
  for(RangeIterator c(g->getSize()); c.hasMore(); c.next()) {
    int sum = c.at(0) + c.at(1);
    
    g->at(c, record);
    
    record->setReal(0, (k_real_t)c.at(0)/10);
    record->setInteger(1, c.at(1));
    
    record->field<KString>(2)->set(Int::toString(c.at(0)) + " + "
        + Int::toString(c.at(1)) + " = " + Int::toString(sum));
    
    record->setEnumeration(3, sum%3);
  }
  
  Ptr<Path> filePath = System::getCurrentWorkingDirectory();
  filePath = filePath->addSegement("test.knoilb");
  
  Ptr<FileOutputStream> fos = new FileOutputStream(filePath);
  g->writeToBinaryStream(fos.AS(OutputStream));
  fos->close();
  
  
  Ptr<FileInputStream> fis = new FileInputStream(filePath);
  Ptr<KGridBasic> g2 = new KGridBasic(gt);
  g2->readFromBinaryStream(fis.AS(InputStream));
  
  LOG << *g2 << EL;
}


void testKGridIterationSpeed() {
  LOG << "Testing KGrid Iteration Speed" << EL;
  Ptr<KRecordType> velocityType = new KRecordType("Velocity");
  velocityType->addField("vx", KType::REAL)
              ->addField("vy", KType::REAL)
              ->addField("vz", KType::REAL);
  
  Ptr<KRecordType> voxelType = new KRecordType("Voxel");
  voxelType->addField("pressure", KType::REAL)
           ->addField("temprature", KType::REAL)
           ->addField("velocity", velocityType.AS(KType));
  
  Ptr<KGridType> gridType = new KGridType(voxelType, 3);
  Ptr<KGridBasic> grid = new KGridBasic(gridType, Tuple3D(500, 500, 500));
  
  Ptr<KRecord> voxel = new KRecord(grid.AS(KGrid));
  PPtr<KRecord> velocity = voxel->getRecord("velocity");
  
  RangeIterator j(grid->getSize());
  
  Timer t("KGrid Iteration Speed");
  t.start();
  
  long int counter = 0;
  for(RangeIterator i(grid->getSize()); i.hasMore(); i.next()) {
    double n = i.productAll();
    counter++;
    if(counter%100000 == 0) {
      cout.put('.');
      cout.flush();
    }
    grid->at(i, voxel);
    voxel->setReal(0, n/10.0);
    voxel->setReal(1, n/10.0);
    velocity->setReal(0, n);
    velocity->setReal(1, n);
    velocity->setReal(2, n);
  }
  cout << endl;
  LOG << "Traced " << counter << " elements. " << t << EL;
}


int main(int argc, char** argv) {
  for(int i = 0; i < 1000; i++) {
    LOG << "Test epoch #" << i << EL;
    System::getLogger().mute();
    testSimpleKRecord();
    testKRecordWithSubrecord();
    testKRecordWithDynamicFields();
    testKGrid();
    System::getLogger().unmute();
  }
  
  testKGridIterationSpeed();

  LOG << *System::getMasterMemoryManager().getManagerAtIndex(0) << EL;
  
  return 0;
}

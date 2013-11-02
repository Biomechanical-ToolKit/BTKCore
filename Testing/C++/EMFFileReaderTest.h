#ifndef EMFFileReaderTest_h
#define EMFFileReaderTest_h

#include <btkAcquisitionFileReader.h>

CXXTEST_SUITE(EMFFileReaderTest)
{
  CXXTEST_TEST(NoFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("Filename must be specified"));
  };
  
  CXXTEST_TEST(MisspelledFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename("foo.emf");
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("File doesn't exist\nFilename: foo.emf"));
  };

  CXXTEST_TEST(Test)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(EMFFilePathIN + "test.emf");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), 1);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 30);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 30);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 100);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 30);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPointUnit(), "mm");
    
    btk::MetaDataInfo::Pointer date = acq->GetMetaData()->GetChild("TRIAL")->GetChild("DATE")->GetInfo();
    TS_ASSERT_EQUALS(date->ToInt(0), 10);
    TS_ASSERT_EQUALS(date->ToInt(1), 3);
    TS_ASSERT_EQUALS(date->ToInt(2), 1);
    
    btk::MetaDataInfo::Pointer time = acq->GetMetaData()->GetChild("TRIAL")->GetChild("TIME")->GetInfo();
    TS_ASSERT_EQUALS(time->ToInt(0), 11);
    TS_ASSERT_EQUALS(time->ToInt(1), 50);
    TS_ASSERT_EQUALS(time->ToInt(2), 10);

    TS_ASSERT_EQUALS(acq->GetPoint(0)->GetLabel().compare("LTOE"), 0);
    TS_ASSERT_EQUALS(acq->GetPoint(1)->GetLabel().compare("LHEL"), 0);
    TS_ASSERT_EQUALS(acq->GetPoint(2)->GetLabel().compare("LKNE"), 0);
    TS_ASSERT_EQUALS(acq->GetPoint(27)->GetLabel().compare("LFHD"), 0);
    TS_ASSERT_EQUALS(acq->GetPoint(28)->GetLabel().compare("LBHD"), 0);
    TS_ASSERT_EQUALS(acq->GetPoint(29)->GetLabel().compare("RFHD"), 0);
    
    TS_ASSERT_DELTA(acq->GetPoint(1)->GetValues()(3,0), 2206.9, 1.0e-2);
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetValues()(3,0), 0.0, 1.0e-2);
    TS_ASSERT_DELTA(acq->GetPoint(29)->GetValues()(79,2), 984.52, 1.0e-2);
    
    TS_ASSERT_DELTA(acq->GetPoint(3)->GetResiduals()(3), -1.0, 1.0e-2);
    TS_ASSERT_DELTA(acq->GetPoint(29)->GetResiduals()(79), 0.0, 1.0e-2);
    
  };
};

CXXTEST_SUITE_REGISTRATION(EMFFileReaderTest)
CXXTEST_TEST_REGISTRATION(EMFFileReaderTest, NoFile)
CXXTEST_TEST_REGISTRATION(EMFFileReaderTest, MisspelledFile)
CXXTEST_TEST_REGISTRATION(EMFFileReaderTest, Test)
#endif

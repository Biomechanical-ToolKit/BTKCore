#ifndef KistlerDATFileReaderTest_h
#define KistlerDATFileReaderTest_h

#include <btkAcquisitionFileReader.h>

CXXTEST_SUITE(KistlerDATFileReaderTest)
{
  CXXTEST_TEST(NoFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("Filename must be specified"));
  };
  
  CXXTEST_TEST(MisspelledFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename("foo.dat");
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("File doesn't exist\nFilename: foo.dat"));
  };

  CXXTEST_TEST(BioWare17)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(KistlerDATFilePathIN + "BioWare17.dat");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), 1);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 1000);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 5000);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 1000);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 8);
    TS_ASSERT_EQUALS(acq->GetPointUnit(), "mm");
    
    const char* labels[8] = {"Fx12", "Fx34", "Fy14", "Fy23", "Fz1", "Fz2", "Fz3", "Fz4"};
    for (int i = 0 ; i < 8 ; ++i)
    {
      TS_ASSERT_EQUALS(acq->GetAnalog(i)->GetLabel().compare(labels[i]), 0);
      TS_ASSERT_EQUALS(acq->GetAnalog(i)->GetScale(), 1.0);
      TS_ASSERT_EQUALS(acq->GetAnalog(i)->GetOffset(), 0.0);
    }
    
    btk::Analog::Values Fx = acq->GetAnalog(0)->GetValues() + acq->GetAnalog(1)->GetValues();
    btk::Analog::Values Fy = acq->GetAnalog(2)->GetValues() + acq->GetAnalog(3)->GetValues();
    btk::Analog::Values Fz = acq->GetAnalog(4)->GetValues() + acq->GetAnalog(5)->GetValues() + acq->GetAnalog(6)->GetValues() + acq->GetAnalog(7)->GetValues();
    
    TS_ASSERT_DELTA(Fx(0), 10.901220, 1e-5);
    TS_ASSERT_DELTA(Fy(0), 27.975272, 1e-5);
    TS_ASSERT_DELTA(Fz(0), -775.374860, 1e-5);
    
    TS_ASSERT_DELTA(Fx(74), -9.391639, 1e-5);
    TS_ASSERT_DELTA(Fy(74), 20.390443, 1e-5);
    TS_ASSERT_DELTA(Fz(74), -832.520675, 5e-5);
    
    TS_ASSERT_DELTA(Fx(935), -3.305952, 1e-5);
    TS_ASSERT_DELTA(Fy(935), -10.557965, 1e-5);
    TS_ASSERT_DELTA(Fz(935), -837.382074, 1e-5);
  };
  
  CXXTEST_TEST(Truncated)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(KistlerDATFilePathIN + "Truncated.dat");
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::Exception &e, e.what(), std::string("Unexpected end of file."));
  };
  
  CXXTEST_TEST(DLL1)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(KistlerDATFilePathIN + "DLL1.dat");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), 1);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 100);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 500);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 100);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 8);
    TS_ASSERT_EQUALS(acq->GetPointUnit(), "mm");
    
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(0), -0.203036, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(1), -0.203036, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(2), -0.203036, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(0)->GetValues()(3), -0.357869, 1e-5);
    
    TS_ASSERT_DELTA(acq->GetAnalog(7)->GetValues()(496), -68.238548, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(7)->GetValues()(497), -68.238548, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(7)->GetValues()(498), -68.582993, 1e-5);
    TS_ASSERT_DELTA(acq->GetAnalog(7)->GetValues()(499), -68.582993, 1e-5);
  };
};

CXXTEST_SUITE_REGISTRATION(KistlerDATFileReaderTest)
CXXTEST_TEST_REGISTRATION(KistlerDATFileReaderTest, NoFile)
CXXTEST_TEST_REGISTRATION(KistlerDATFileReaderTest, MisspelledFile)
CXXTEST_TEST_REGISTRATION(KistlerDATFileReaderTest, BioWare17)
CXXTEST_TEST_REGISTRATION(KistlerDATFileReaderTest, Truncated)
CXXTEST_TEST_REGISTRATION(KistlerDATFileReaderTest, DLL1)
#endif

#ifndef BSFFileReaderTest_h
#define BSFFileReaderTest_h

#include <btkAcquisitionFileReader.h>

CXXTEST_SUITE(BSFFileReaderTest)
{
  CXXTEST_TEST(NoFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("Filename must be specified"));
  };
  
  CXXTEST_TEST(MisspelledFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename("test.bsf");
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("File doesn't exist\nFilename: test.bsf"));
  };
  
  CXXTEST_TEST(Trial01869)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(BSFFilePathIN + "Trial01868.bsf");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();
    
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 200.0);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 400);
    TS_ASSERT_EQUALS(acq->GetAnalogFrameNumber(), 400);
    TS_ASSERT_EQUALS(acq->GetPointUnit(), "m");
    TS_ASSERT_EQUALS(acq->GetPointUnit(btk::Point::Moment), "Nm");
    
    btk::MetaData::Pointer forcePlatform = acq->GetMetaData()->GetChild("FORCE_PLATFORM");
    TS_ASSERT_EQUALS(forcePlatform->GetChild("USED")->GetInfo()->ToInt(0), 1);
    btk::MetaDataInfo::Pointer corners = forcePlatform->GetChild("CORNERS")->GetInfo();
    btk::MetaDataInfo::Pointer origin = forcePlatform->GetChild("ORIGIN")->GetInfo();
    float fpDims[3] = {0.49784f, 0.49784f, 0.0f}; // Dimensions in meters
    TS_ASSERT_DELTA(corners->ToFloat(0), fpDims[0]/2.0f, 1e-5);
    TS_ASSERT_DELTA(corners->ToFloat(1), -fpDims[1]/2.0f, 1e-5);
    TS_ASSERT_DELTA(corners->ToFloat(2), 0.0f, 1e-5);
    TS_ASSERT_DELTA(corners->ToFloat(3), -fpDims[0]/2.0f, 1e-5);
    TS_ASSERT_DELTA(corners->ToFloat(4), -fpDims[1]/2.0f, 1e-5);
    TS_ASSERT_DELTA(corners->ToFloat(5), 0.0f, 1e-5);
    TS_ASSERT_DELTA(corners->ToFloat(6), -fpDims[0]/2.0f, 1e-5);
    TS_ASSERT_DELTA(corners->ToFloat(7), fpDims[1]/2.0f, 1e-5);
    TS_ASSERT_DELTA(corners->ToFloat(8), 0.0f, 1e-5);
    TS_ASSERT_DELTA(corners->ToFloat(9), fpDims[0]/2.0f, 1e-5);
    TS_ASSERT_DELTA(corners->ToFloat(10), fpDims[1]/2.0f, 1e-5);
    TS_ASSERT_DELTA(corners->ToFloat(11), 0.0f, 1e-5);
    TS_ASSERT_DELTA(origin->ToFloat(0), 0.0f, 1e-5);
    TS_ASSERT_DELTA(origin->ToFloat(1), 0.0f, 1e-5);
    TS_ASSERT_DELTA(origin->ToFloat(2), -fpDims[2]/2.0f, 1e-5);
    
    btk::Analog::Pointer Fx = acq->GetAnalog(0);
    btk::Analog::Pointer Fy = acq->GetAnalog(1);
    btk::Analog::Pointer Fz = acq->GetAnalog(2);
    btk::Analog::Pointer Mx = acq->GetAnalog(3);
    btk::Analog::Pointer My = acq->GetAnalog(4);
    btk::Analog::Pointer Mz = acq->GetAnalog(5);
    
    TS_ASSERT_EQUALS(Fx->GetLabel(), "Fx");
    TS_ASSERT_EQUALS(Fy->GetLabel(), "Fy");
    TS_ASSERT_EQUALS(Fz->GetLabel(), "Fz");
    TS_ASSERT_EQUALS(Mx->GetLabel(), "Mx");
    TS_ASSERT_EQUALS(My->GetLabel(), "My");
    TS_ASSERT_EQUALS(Mz->GetLabel(), "Mz");
    
    TS_ASSERT_EQUALS(Fx->GetUnit(), "N");
    TS_ASSERT_EQUALS(Fy->GetUnit(), "N");
    TS_ASSERT_EQUALS(Fz->GetUnit(), "N");
    TS_ASSERT_EQUALS(Mx->GetUnit(), "Nm");
    TS_ASSERT_EQUALS(My->GetUnit(), "Nm");
    TS_ASSERT_EQUALS(Mz->GetUnit(), "Nm");
    
    double* data = 0;
    data = acq->GetAnalog(0)->GetValues().data();
    TS_ASSERT_DELTA(data[0], 0.123, 5e-4);
    TS_ASSERT_DELTA(data[1], 0.736, 5e-4);
    TS_ASSERT_DELTA(data[2], 0.491, 5e-4);
    TS_ASSERT_DELTA(data[399], 0.0, 5e-4);
    data = acq->GetAnalog(1)->GetValues().data();
    TS_ASSERT_DELTA(data[0], -0.260, 5e-4);
    data = acq->GetAnalog(2)->GetValues().data();
    TS_ASSERT_DELTA(data[0], 0.0, 5e-4);
    data = acq->GetAnalog(3)->GetValues().data();
    TS_ASSERT_DELTA(data[0], 0.0, 5e-4);
    data = acq->GetAnalog(4)->GetValues().data();
    TS_ASSERT_DELTA(data[0], 0.0, 5e-4);
    data = acq->GetAnalog(5)->GetValues().data();
    TS_ASSERT_DELTA(data[0], -0.032, 5e-4);
    TS_ASSERT_DELTA(data[399], -0.446, 5e-4);
  };
};

CXXTEST_SUITE_REGISTRATION(BSFFileReaderTest)
CXXTEST_TEST_REGISTRATION(BSFFileReaderTest, NoFile)
CXXTEST_TEST_REGISTRATION(BSFFileReaderTest, MisspelledFile)
CXXTEST_TEST_REGISTRATION(BSFFileReaderTest, Trial01869)
#endif

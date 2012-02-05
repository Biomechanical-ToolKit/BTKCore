#ifndef AMTIForcePlatformFileReaderTest_h
#define AMTIForcePlatformFileReaderTest_h

#include <btkAcquisitionFileReader.h>

CXXTEST_SUITE(AMTIForcePlatformFileReaderTest)
{
  CXXTEST_TEST(NoFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("Filename must be specified"));
  };
  
  CXXTEST_TEST(MisspelledFile)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename("test.asc");
    TS_ASSERT_THROWS_EQUALS(reader->Update(), const btk::AcquisitionFileReaderException &e, e.what(), std::string("File doesn't exist\nFilename: test.asc"));
  };

  CXXTEST_TEST(Trial1)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(AMTIForcePlatformFilePathIN + "Trial1-FxFyFzMxMyMz.asc");
    reader->Update();
    btk::Acquisition::Pointer acq = reader->GetOutput();

    TS_ASSERT_EQUALS(acq->GetFirstFrame(), 1);
    TS_ASSERT_EQUALS(acq->GetPointFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetPointNumber(), 0);
    TS_ASSERT_EQUALS(acq->GetPointFrameNumber(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogFrequency(), 200);
    TS_ASSERT_EQUALS(acq->GetAnalogNumber(), 6);
    TS_ASSERT_EQUALS(acq->GetPointUnit(btk::Point::Marker), "m");
    TS_ASSERT_EQUALS(acq->GetPointUnit(btk::Point::Moment), "Nm");
    
    btk::MetaData::Pointer forcePlatform = acq->GetMetaData()->GetChild("FORCE_PLATFORM");
    TS_ASSERT_EQUALS(forcePlatform->GetChild("USED")->GetInfo()->ToInt(0), 1);
    btk::MetaDataInfo::Pointer corners = forcePlatform->GetChild("CORNERS")->GetInfo();
    btk::MetaDataInfo::Pointer origin = forcePlatform->GetChild("ORIGIN")->GetInfo();
    float fpDims[3] = {463.6f/1000.0f, 508.0f/1000.0f, 82.6f/1000.0f}; // Dimensions in meters
    TS_ASSERT_DELTA(corners->ToFloat(0), -fpDims[0]/2.0f, 1e-5);
    TS_ASSERT_DELTA(corners->ToFloat(1), fpDims[1]/2.0f, 1e-5);
    TS_ASSERT_DELTA(corners->ToFloat(2), 0.0f, 1e-5);
    TS_ASSERT_DELTA(corners->ToFloat(3), fpDims[0]/2.0f, 1e-5);
    TS_ASSERT_DELTA(corners->ToFloat(4), fpDims[1]/2.0f, 1e-5);
    TS_ASSERT_DELTA(corners->ToFloat(5), 0.0f, 1e-5);
    TS_ASSERT_DELTA(corners->ToFloat(6), fpDims[0]/2.0f, 1e-5);
    TS_ASSERT_DELTA(corners->ToFloat(7), -fpDims[1]/2.0f, 1e-5);
    TS_ASSERT_DELTA(corners->ToFloat(8), 0.0f, 1e-5);
    TS_ASSERT_DELTA(corners->ToFloat(9), -fpDims[0]/2.0f, 1e-5);
    TS_ASSERT_DELTA(corners->ToFloat(10), -fpDims[1]/2.0f, 1e-5);
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
    
    TS_ASSERT_DELTA(-Fx->GetValues()(0), -0.491, 1e-4);
    TS_ASSERT_DELTA(-Fy->GetValues()(0), -0.521, 1e-4);
    TS_ASSERT_DELTA(-Fz->GetValues()(0),  1.079, 1e-4);
    TS_ASSERT_DELTA(-Mx->GetValues()(0),  0.098, 1e-4);
    TS_ASSERT_DELTA(-My->GetValues()(0),  0.000, 1e-4);
    TS_ASSERT_DELTA(-Mz->GetValues()(0),  0.255, 1e-4);
    
    TS_ASSERT_DELTA(-Fx->GetValues()(9), -1.718, 1e-4);
    TS_ASSERT_DELTA(-Fy->GetValues()(21), 18.231, 1e-4);
    TS_ASSERT_DELTA(-Fz->GetValues()(35), 643.298, 1e-4);
    TS_ASSERT_DELTA(-Mx->GetValues()(60), 46.447, 1e-4);
    TS_ASSERT_DELTA(-My->GetValues()(63), 0.610, 1e-4);
    TS_ASSERT_DELTA(-Mz->GetValues()(81), -4.748, 1e-4);
  };
};

CXXTEST_SUITE_REGISTRATION(AMTIForcePlatformFileReaderTest)
CXXTEST_TEST_REGISTRATION(AMTIForcePlatformFileReaderTest, NoFile)
CXXTEST_TEST_REGISTRATION(AMTIForcePlatformFileReaderTest, MisspelledFile)
CXXTEST_TEST_REGISTRATION(AMTIForcePlatformFileReaderTest, Trial1)
#endif

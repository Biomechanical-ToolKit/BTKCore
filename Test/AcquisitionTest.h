#ifndef AcquisitionTest_h
#define AcquisitionTest_h

#include <btkAcquisition.h>

CXXTEST_SUITE(AcquisitionTest)
{
  CXXTEST_TEST(Constructor)
  {
    btk::Acquisition::Pointer test = btk::Acquisition::New();
    TS_ASSERT_EQUALS(test->GetMarkerFrameNumber(), 0);
    TS_ASSERT_EQUALS(test->GetAnalogFrameNumber(), 0);
    TS_ASSERT_EQUALS(test->GetMarkerNumber(), 0);
    TS_ASSERT_EQUALS(test->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(test->GetMetaData()->GetLabel(), "ROOT");
  };
  
  CXXTEST_TEST(SetDurationAndFrequency)
  {
    btk::Acquisition::Pointer test = btk::Acquisition::New();
    test->Init(1, 150, 1, 1);
    test->SetMarkerFrequency(100);
    TS_ASSERT_EQUALS(test->GetMarkerFrequency(), 100);
    TS_ASSERT_EQUALS(test->GetAnalogFrequency(), 100);
    TS_ASSERT_EQUALS(test->GetMarkerFrameNumber(), 150);
    TS_ASSERT_EQUALS(test->GetAnalogFrameNumber(), 150);
  };
  
  CXXTEST_TEST(SetNumber)
  {
    btk::Acquisition::Pointer test = btk::Acquisition::New();
    test->Init(12, 150, 7, 1);
    test->SetMarkerFrequency(100);
    TS_ASSERT_EQUALS(test->GetMarkerNumber(), 12);
    TS_ASSERT_EQUALS(test->GetAnalogNumber(), 7);
    TS_ASSERT_EQUALS(test->GetMarkerFrameNumber(), 150);
    TS_ASSERT_EQUALS(test->GetAnalogFrameNumber(), 150);
    btk::Marker::Pointer ptMarker = test->GetMarker(0);
    TS_ASSERT_EQUALS(ptMarker->GetFrameNumber(), 150);
    btk::Analog::Pointer ptAnalog = test->GetAnalog(0);
    TS_ASSERT_EQUALS(ptAnalog->GetFrameNumber(), 150);
  };
  
  CXXTEST_TEST(SetAnalogFrequencyOnly)
  {
    btk::Acquisition::Pointer test = btk::Acquisition::New();
    test->Init(0, 200, 1, 1);
    TS_ASSERT_EQUALS(test->GetMarkerFrameNumber(), 200);
    TS_ASSERT_EQUALS(test->GetAnalogFrameNumber(), 200);
  }
  
};

CXXTEST_SUITE_REGISTRATION(AcquisitionTest)
CXXTEST_TEST_REGISTRATION(AcquisitionTest, Constructor)
CXXTEST_TEST_REGISTRATION(AcquisitionTest, SetDurationAndFrequency)
CXXTEST_TEST_REGISTRATION(AcquisitionTest, SetNumber)
CXXTEST_TEST_REGISTRATION(AcquisitionTest, SetAnalogFrequencyOnly)
#endif
#ifndef AcquisitionTest_h
#define AcquisitionTest_h

#include <btkAcquisition.h>

CXXTEST_SUITE(AcquisitionTest)
{
  CXXTEST_TEST(Constructor)
  {
    btk::Acquisition::Pointer test = btk::Acquisition::New();
    TS_ASSERT_EQUALS(test->GetPointFrameNumber(), 0);
    TS_ASSERT_EQUALS(test->GetAnalogFrameNumber(), 0);
    TS_ASSERT_EQUALS(test->GetPointNumber(), 0);
    TS_ASSERT_EQUALS(test->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(test->GetMetaData()->GetLabel(), "ROOT");
  };
  
  CXXTEST_TEST(SetDurationAndFrequency)
  {
    btk::Acquisition::Pointer test = btk::Acquisition::New();
    test->Init(1, 150, 1, 1);
    test->SetPointFrequency(100);
    TS_ASSERT_EQUALS(test->GetPointFrequency(), 100);
    TS_ASSERT_EQUALS(test->GetAnalogFrequency(), 100);
    TS_ASSERT_EQUALS(test->GetPointFrameNumber(), 150);
    TS_ASSERT_EQUALS(test->GetAnalogFrameNumber(), 150);
  };
  
  CXXTEST_TEST(SetNumber)
  {
    btk::Acquisition::Pointer test = btk::Acquisition::New();
    test->Init(12, 150, 7, 1);
    test->SetPointFrequency(100);
    TS_ASSERT_EQUALS(test->GetPointNumber(), 12);
    TS_ASSERT_EQUALS(test->GetAnalogNumber(), 7);
    TS_ASSERT_EQUALS(test->GetPointFrameNumber(), 150);
    TS_ASSERT_EQUALS(test->GetAnalogFrameNumber(), 150);
    btk::Point::Pointer ptPoint = test->GetPoint(0);
    TS_ASSERT_EQUALS(ptPoint->GetFrameNumber(), 150);
    btk::Analog::Pointer ptAnalog = test->GetAnalog(0);
    TS_ASSERT_EQUALS(ptAnalog->GetFrameNumber(), 150);
  };
  
  CXXTEST_TEST(SetAnalogFrequencyOnly)
  {
    btk::Acquisition::Pointer test = btk::Acquisition::New();
    test->Init(0, 200, 1, 1);
    TS_ASSERT_EQUALS(test->GetPointFrameNumber(), 200);
    TS_ASSERT_EQUALS(test->GetAnalogFrameNumber(), 200);
  }

  CXXTEST_TEST(InitPointAndAnalog)
  {
    btk::Acquisition::Pointer test = btk::Acquisition::New();
    test->Init(5, 200, 10, 1);
    TS_ASSERT_EQUALS(test->GetPointNumber(), 5);
    TS_ASSERT_EQUALS(test->GetAnalogNumber(), 10);
    TS_ASSERT_EQUALS(test->GetPointFrameNumber(), 200);
    TS_ASSERT_EQUALS(test->GetAnalogFrameNumber(), 200);
  }

  CXXTEST_TEST(InitPointOnly)
  {
    btk::Acquisition::Pointer test = btk::Acquisition::New();
    test->Init(5, 200);
    TS_ASSERT_EQUALS(test->GetPointNumber(), 5);
    TS_ASSERT_EQUALS(test->GetAnalogNumber(), 0);
    TS_ASSERT_EQUALS(test->GetPointFrameNumber(), 200);
    TS_ASSERT_EQUALS(test->GetAnalogFrameNumber(), 200);
  }

  CXXTEST_TEST(InitAnalogOnly)
  {
    btk::Acquisition::Pointer test = btk::Acquisition::New();
    test->Init(0, 200, 10);
    TS_ASSERT_EQUALS(test->GetPointNumber(), 0);
    TS_ASSERT_EQUALS(test->GetAnalogNumber(), 10);
    TS_ASSERT_EQUALS(test->GetPointFrameNumber(), 200);
    TS_ASSERT_EQUALS(test->GetAnalogFrameNumber(), 200);
  }
  
  CXXTEST_TEST(ResizeFromEnd)
  {
    btk::Acquisition::Pointer test = btk::Acquisition::New();
    test->Init(2, 200, 10, 2);
    TS_ASSERT_EQUALS(test->GetFirstFrame(), 1);
    TS_ASSERT_EQUALS(test->GetPointNumber(), 2);
    TS_ASSERT_EQUALS(test->GetAnalogNumber(), 10);
    TS_ASSERT_EQUALS(test->GetPointFrameNumber(), 200);
    TS_ASSERT_EQUALS(test->GetAnalogFrameNumber(), 400);
    test->ResizeFrameFromEnd(50);
    TS_ASSERT_EQUALS(test->GetFirstFrame(), 51);
    TS_ASSERT_EQUALS(test->GetPointNumber(), 2);
    TS_ASSERT_EQUALS(test->GetAnalogNumber(), 10);
    TS_ASSERT_EQUALS(test->GetPointFrameNumber(), 50);
    TS_ASSERT_EQUALS(test->GetAnalogFrameNumber(), 100);
  }
};

CXXTEST_SUITE_REGISTRATION(AcquisitionTest)
CXXTEST_TEST_REGISTRATION(AcquisitionTest, Constructor)
CXXTEST_TEST_REGISTRATION(AcquisitionTest, SetDurationAndFrequency)
CXXTEST_TEST_REGISTRATION(AcquisitionTest, SetNumber)
CXXTEST_TEST_REGISTRATION(AcquisitionTest, SetAnalogFrequencyOnly)
CXXTEST_TEST_REGISTRATION(AcquisitionTest, InitPointAndAnalog)
CXXTEST_TEST_REGISTRATION(AcquisitionTest, InitPointOnly)
CXXTEST_TEST_REGISTRATION(AcquisitionTest, InitAnalogOnly)
CXXTEST_TEST_REGISTRATION(AcquisitionTest, ResizeFromEnd)
#endif

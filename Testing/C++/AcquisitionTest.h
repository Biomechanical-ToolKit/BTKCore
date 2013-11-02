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
    test->ResizeFrameNumberFromEnd(50);
    TS_ASSERT_EQUALS(test->GetFirstFrame(), 151);
    TS_ASSERT_EQUALS(test->GetPointNumber(), 2);
    TS_ASSERT_EQUALS(test->GetAnalogNumber(), 10);
    TS_ASSERT_EQUALS(test->GetPointFrameNumber(), 50);
    TS_ASSERT_EQUALS(test->GetAnalogFrameNumber(), 100);
  }
  
  CXXTEST_TEST(RemoveLastPoint)
  {
    btk::Acquisition::Pointer test = btk::Acquisition::New();
    test->Init(10, 5);
    TS_ASSERT_EQUALS(test->GetFirstFrame(), 1);
    TS_ASSERT_EQUALS(test->GetPointNumber(), 10);
    TS_ASSERT_EQUALS(test->GetPointFrameNumber(), 5);
    test->RemovePoint("uname*10");
    TS_ASSERT_EQUALS(test->GetPointNumber(), 9);
    test->RemovePoint("uname*9");
    TS_ASSERT_EQUALS(test->GetPointNumber(), 8);
    test->RemovePoint("uname*8");
    TS_ASSERT_EQUALS(test->GetPointNumber(), 7);
    test->RemovePoint("uname*7");
    TS_ASSERT_EQUALS(test->GetPointNumber(), 6);
    test->RemovePoint("uname*6");
    TS_ASSERT_EQUALS(test->GetPointNumber(), 5);
    test->RemovePoint("uname*5");
    TS_ASSERT_EQUALS(test->GetPointNumber(), 4);
    test->RemovePoint("uname*4");
    TS_ASSERT_EQUALS(test->GetPointNumber(), 3);
    test->RemovePoint("uname*3");
    TS_ASSERT_EQUALS(test->GetPointNumber(), 2);
    test->RemovePoint("uname*2");
    TS_ASSERT_EQUALS(test->GetPointNumber(), 1);
    test->RemovePoint("uname*1");
    TS_ASSERT_EQUALS(test->GetPointNumber(), 0);
  }
  
  CXXTEST_TEST(SetFirstFrameAdaptEvent)
  {
    btk::Acquisition::Pointer test = btk::Acquisition::New();
    test->SetPointFrequency(100);
    test->AppendEvent(btk::Event::New("FOO", 102, "Right", btk::Event::Manual, "", "", 1));
    test->AppendEvent(btk::Event::New("BAR", 219, "Left", btk::Event::Manual, "", "", 2));
    test->AppendEvent(btk::Event::New("FOO", 305, "General", btk::Event::Manual, "", "", 1));
    test->SetFirstFrame(10, true);
    TS_ASSERT_EQUALS(test->GetFirstFrame(), 10);
    TS_ASSERT_EQUALS(test->GetEvent(0)->GetFrame(), 111);
    TS_ASSERT_DELTA(test->GetEvent(0)->GetTime(), 1.11, 1e-11);
    TS_ASSERT_EQUALS(test->GetEvent(1)->GetFrame(), 228);
    TS_ASSERT_DELTA(test->GetEvent(1)->GetTime(), 2.28, 1e-11);
    TS_ASSERT_EQUALS(test->GetEvent(2)->GetFrame(), 314);
    TS_ASSERT_DELTA(test->GetEvent(2)->GetTime(), 3.14, 1e-11);
  }
  
  CXXTEST_TEST(ResizeParent)
  {
    btk::Acquisition::Pointer test = btk::Acquisition::New();
    test->GetPoints()->InsertItem(btk::Point::New());
    test->GetPoints()->InsertItem(btk::Point::New());
    test->GetPoints()->InsertItem(btk::Point::New());
    test->Resize(3,5);
    TS_ASSERT_EQUALS(test->GetPointNumber(), 3);
    TS_ASSERT_EQUALS(test->GetPointFrameNumber(), 5);
    TS_ASSERT_EQUALS(test->GetPoint(0)->GetFrameNumber(), 5);
    TS_ASSERT_EQUALS(test->GetPoint(1)->GetFrameNumber(), 5);
    TS_ASSERT_EQUALS(test->GetPoint(2)->GetFrameNumber(), 5);
    TS_ASSERT_EQUALS(test->GetPoint(0)->GetParent(), test.get());
    TS_ASSERT_EQUALS(test->GetPoint(1)->GetParent(), test.get());
    TS_ASSERT_EQUALS(test->GetPoint(2)->GetParent(), test.get());
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
CXXTEST_TEST_REGISTRATION(AcquisitionTest, RemoveLastPoint)
CXXTEST_TEST_REGISTRATION(AcquisitionTest, SetFirstFrameAdaptEvent)
CXXTEST_TEST_REGISTRATION(AcquisitionTest, ResizeParent)
#endif

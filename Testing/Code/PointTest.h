#ifndef PointTest_h
#define PointTest_h

#include <btkPoint.h>

USING_PART_OF_NAMESPACE_EIGEN

CXXTEST_SUITE(PointTest)
{
  CXXTEST_TEST(Constructor)
  {
    btk::Point::Pointer test = btk::Point::New("HEEL_R", 200);
    TS_ASSERT_EQUALS(test->GetLabel(), "HEEL_R");
    TS_ASSERT_EQUALS(test->GetDescription(), "         ");
    TS_ASSERT_EQUALS(test->GetValues().rows(), 200);
    TS_ASSERT_EQUALS(test->GetValues().cols(), 3);
    TS_ASSERT_EQUALS(test->GetResiduals().rows(), 200);
    TS_ASSERT_EQUALS(test->GetResiduals().cols(), 1);
    TS_ASSERT_EQUALS(test->GetMasks().rows(), 200);
    TS_ASSERT_EQUALS(test->GetMasks().cols(), 1);
    TS_ASSERT_EQUALS(test->GetType(), btk::Point::Marker);
  };
  
  CXXTEST_TEST(FrameNumber)
  {
    btk::Point::Pointer test = btk::Point::New("HEEL_R", 200);
    long t1 = test->GetTimestamp();
    test->SetFrameNumber(225);
    TS_ASSERT_EQUALS(test->GetValues().rows(), 225);
    TS_ASSERT_EQUALS(test->GetResiduals().rows(), 225);
    TS_ASSERT_EQUALS(test->GetMasks().rows(), 225);
    TS_ASSERT(t1 != test->GetTimestamp());
  };

};

CXXTEST_SUITE_REGISTRATION(PointTest)
CXXTEST_TEST_REGISTRATION(PointTest, Constructor)
CXXTEST_TEST_REGISTRATION(PointTest, FrameNumber)
#endif

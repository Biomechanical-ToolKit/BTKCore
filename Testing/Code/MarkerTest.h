#ifndef MarkerTest_h
#define MarkerTest_h

#include <btkMarker.h>

USING_PART_OF_NAMESPACE_EIGEN

CXXTEST_SUITE(MarkerTest)
{
  CXXTEST_TEST(Constructor)
  {
    btk::Marker::Pointer test = btk::Marker::New("HEEL_R", 200);
    TS_ASSERT_EQUALS(test->GetLabel(), "HEEL_R");
    TS_ASSERT_EQUALS(test->GetDescription(), "");
    TS_ASSERT_EQUALS(test->GetValues().rows(), 200);
    TS_ASSERT_EQUALS(test->GetValues().cols(), 3);
    TS_ASSERT_EQUALS(test->GetResidual().rows(), 200);
    TS_ASSERT_EQUALS(test->GetResidual().cols(), 1);
    TS_ASSERT_EQUALS(test->GetMask().rows(), 200);
    TS_ASSERT_EQUALS(test->GetMask().cols(), 1);
  };
  
  CXXTEST_TEST(FrameNumber)
  {
    btk::Marker::Pointer test = btk::Marker::New("HEEL_R", 200);
    long t1 = test->GetTimestamp();
    test->SetFrameNumber(225);
    TS_ASSERT_EQUALS(test->GetValues().rows(), 225);
    TS_ASSERT_EQUALS(test->GetResidual().rows(), 225);
    TS_ASSERT_EQUALS(test->GetMask().rows(), 225);
    TS_ASSERT(t1 < test->GetTimestamp());
  };
};

CXXTEST_SUITE_REGISTRATION(MarkerTest)
CXXTEST_TEST_REGISTRATION(MarkerTest, Constructor)
#endif
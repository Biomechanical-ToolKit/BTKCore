#ifndef LardTrialTest_h
#define LardTrialTest_h

#include <btklardTrial.h>

CXXTEST_SUITE(LardTrialTest)
{
  CXXTEST_TEST(ExtractTrajectory)
  {
    btk::Node root("root");
    btk::TimeSequence ts1("TS1",4,10,100.0,0.0,btk::TimeSequence::Marker,"mm",&root);
    btk::TimeSequence ts2("TS2",4,10,100.0,0.0,btk::TimeSequence::Marker,"mm",&root);
    auto TS1 = btk::lard::extract_trajectory("TS1",&root);
    auto TS2 = btk::lard::extract_trajectory("TS2",&root);
    
    TS_ASSERT_EQUALS(TS1.isValid(),true);
    TS_ASSERT_EQUALS(TS2.isValid(),true);
    
    TS1.values().setRandom();
    TS1.residuals() << -1.0, 1.0, 0.5, 0.5, 1.0, 0.65, 0.45, 1.0, 0.0, -0.4;
    TS2.values().setRandom();
    TS2.residuals() <<  0.5, 0.0, 1.0, 0.5, 0.5, 1.0, 0.65, 0.45, 1.0,  0.0;
    
    btk::lard::Trajectory mean = (TS1 + TS2) / 2.0;
    
    double* _ts1 = ts1.data();
    double* _ts2 = ts2.data();
    double* _mean = mean.values().data();
    
    TS_ASSERT_EQUALS(_mean[0], 0.);
    TS_ASSERT_EQUALS(_mean[9], 0.);
    TS_ASSERT_EQUALS(_mean[10], 0.);
    TS_ASSERT_EQUALS(_mean[19], 0.);
    TS_ASSERT_EQUALS(_mean[20], 0.);
    TS_ASSERT_EQUALS(_mean[29], 0.);
    for (int i = 1 ; i < 9 ; ++i)
    {
      TS_ASSERT_EQUALS(_mean[i],    (_ts1[i]    + _ts2[i])    * 0.5);
      TS_ASSERT_EQUALS(_mean[i+10], (_ts1[i+10] + _ts2[i+10]) * 0.5);
      TS_ASSERT_EQUALS(_mean[i+20], (_ts1[i+20] + _ts2[i+20]) * 0.5);
    }
    TS_ASSERT_EQUALS(mean.residuals()[0], -1.0);
    TS_ASSERT_EQUALS(mean.residuals()[1], 0.0);
    TS_ASSERT_EQUALS(mean.residuals()[2], 0.0);
    TS_ASSERT_EQUALS(mean.residuals()[3], 0.0);
    TS_ASSERT_EQUALS(mean.residuals()[4], 0.0);
    TS_ASSERT_EQUALS(mean.residuals()[5], 0.0);
    TS_ASSERT_EQUALS(mean.residuals()[6], 0.0);
    TS_ASSERT_EQUALS(mean.residuals()[7], 0.0);
    TS_ASSERT_EQUALS(mean.residuals()[8], 0.0);
    TS_ASSERT_EQUALS(mean.residuals()[9], -1.0);
  };
};

CXXTEST_SUITE_REGISTRATION(LardTrialTest)
CXXTEST_TEST_REGISTRATION(LardTrialTest, ExtractTrajectory)

#endif // LardTrialTest_h
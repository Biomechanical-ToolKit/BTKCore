#ifndef AnalogTest_h
#define AnalogTest_h

#include <btkAnalog.h>

CXXTEST_SUITE(AnalogTest)
{
  CXXTEST_TEST(DataClone)
  {
    btk::Analog::Pointer analog = btk::Analog::New("HEEL_R", 5);
    analog->SetValues(Eigen::Matrix<double,Eigen::Dynamic,1>::Random(5,1));
    btk::Analog::Pointer cloned = analog->Clone();
    for (int i = 0 ; i < 5 ; ++i)
      TS_ASSERT_DELTA(cloned->GetValues().coeff(i),analog->GetValues().coeff(i),1e-15);
  };
};

CXXTEST_SUITE_REGISTRATION(AnalogTest)
CXXTEST_TEST_REGISTRATION(AnalogTest, DataClone)  

#endif // Analog
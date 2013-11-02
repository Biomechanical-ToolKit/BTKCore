#ifndef GammalnTest_h
#define GammalnTest_h

#include <Utilities/maths/gammaln.h>

CXXTEST_SUITE(GammalnTest)
{
  CXXTEST_TEST(TestZero)
  {
    TS_ASSERT_DELTA(gammaln(0.0), std::numeric_limits<double>::infinity(), 1e-15);
  };
  
  CXXTEST_TEST(TestPositive)
  {
    TS_ASSERT_DELTA(gammaln(15.0), 25.191221182738683, 5e-15);
    TS_ASSERT_DELTA(gammaln(1.0), 0.0, 1e-15);
    TS_ASSERT_DELTA(gammaln(2.0), 0.0, 1e-15);
    TS_ASSERT_DELTA(gammaln(1.45), -0.12142059074030688, 1e-15);
    TS_ASSERT_DELTA(gammaln(5.25), 3.5613759103866971, 1e-15);
    TS_ASSERT_DELTA(gammaln(89.0), 309.1641935801469, 1e-15);
    TS_ASSERT_DELTA(gammaln(1.8), -0.07108387291437214, 1e-15);
    TS_ASSERT_DELTA(gammaln(1e-15), 34.538776394910684, 1e-15);
    TS_ASSERT_DELTA(gammaln(3.0), 0.69314718055994529, 1e-15);
  };
  
  CXXTEST_TEST(TestFloat)
  {
    TS_ASSERT_DELTA(gammaln(0.0f), std::numeric_limits<float>::infinity(), 1e-5);
    TS_ASSERT_DELTA(gammaln(1.0f), 0.0f, 1e-5);
    TS_ASSERT_DELTA(gammaln(2.0f), 0.0f, 1e-5);
    TS_ASSERT_DELTA(gammaln(1.45f), -0.121420f, 1e-5);
    TS_ASSERT_DELTA(gammaln(5.25f), 3.561375f, 1e-5);
    TS_ASSERT_DELTA(gammaln(89.0f), 309.164193f, 1e-4);
    TS_ASSERT_DELTA(gammaln(1.8f), -0.071083f, 1e-5);
    TS_ASSERT_DELTA(gammaln(3.0f), 0.693147f, 1e-5);
  };

  CXXTEST_TEST(TestInteger)
  {
    TS_ASSERT_EQUALS(gammaln(0), std::numeric_limits<int>::infinity());
    TS_ASSERT_EQUALS(gammaln(1), 0);
    TS_ASSERT_EQUALS(gammaln(2), 0);
    TS_ASSERT_EQUALS(gammaln(89), 309);
    TS_ASSERT_EQUALS(gammaln(3), 0);
  };
  
  CXXTEST_TEST(TestDoubleComplex)
  {
    std::complex<double> x, y;
    x = gammaln(std::complex<double>(0.78,0.0));
    y = std::complex<double>(0.171825755888914,0.0);
    TS_ASSERT_DELTA(x.real(), y.real(), 1e-15);
    TS_ASSERT_DELTA(x.imag(), y.imag(), 1e-15);
  };
};

CXXTEST_SUITE_REGISTRATION(GammalnTest)
CXXTEST_TEST_REGISTRATION(GammalnTest, TestZero)
CXXTEST_TEST_REGISTRATION(GammalnTest, TestPositive)
CXXTEST_TEST_REGISTRATION(GammalnTest, TestFloat)
CXXTEST_TEST_REGISTRATION(GammalnTest, TestInteger)
CXXTEST_TEST_REGISTRATION(GammalnTest, TestDoubleComplex)

#endif // GammalnTest_h
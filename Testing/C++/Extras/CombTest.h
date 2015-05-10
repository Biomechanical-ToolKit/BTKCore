#ifndef CombTest_h
#define CombTest_h

#include <maths/comb.h>

CXXTEST_SUITE(CombTest)
{
  CXXTEST_TEST(TestFloat)
  {
    TS_ASSERT_DELTA(comb(2.0f, 4.0f), 0.0f, 1e-5);
    TS_ASSERT_DELTA(comb(4.0f, 2.0f), 6.0f, 1e-5);
    TS_ASSERT_DELTA(comb(0.0f, 0.0f), 1.0f, 1e-5);
    TS_ASSERT_DELTA(comb(3.0f, 2.0f), 2.9999999999999996f, 1e-5);
    TS_ASSERT_DELTA(comb(3.5f, 2.45f), 3.6167234401282293f, 1e-5);
    TS_ASSERT_DELTA(comb(25.0f, 78.0f), 0.0f, 1e-5);
    TS_ASSERT_DELTA(comb(25.0f, 25.0f), 1.0f, 1e-5);
    TS_ASSERT_DELTA(comb(25.0f, 22.0f), 2299.9999999999886f, 1e-1); // Less accurate ...
    TS_ASSERT_DELTA(comb(25.4589653f, 22.4567891235f), 2444.6161943794596f, 1e-1); // Less accurate ...
  };
  
  CXXTEST_TEST(TestDouble)
  {
    TS_ASSERT_DELTA(comb(2.0, 4.0), 0.0, 1e-15);
    TS_ASSERT_DELTA(comb(4.0, 2.0), 6.0, 5e-15);
    TS_ASSERT_DELTA(comb(0.0, 0.0), 1.0, 1e-15);
    TS_ASSERT_DELTA(comb(3.0, 2.0), 2.9999999999999996, 1e-13);
    TS_ASSERT_DELTA(comb(3.5, 2.45), 3.6167234401282293, 1e-13);
    TS_ASSERT_DELTA(comb(25.0, 78.0), 0.0, 1e-15);
    TS_ASSERT_DELTA(comb(25.0, 25.0), 1.0, 1e-15);
    TS_ASSERT_DELTA(comb(25.0, 22.0), 2299.9999999999886, 5e-11); // The difference are due to the method used in the computation of gammaln
    TS_ASSERT_DELTA(comb(25.4589653, 22.4567891235), 2444.6161943794596, 5e-11); // The difference are due to the method used in the computation of gammaln
  };
  
  CXXTEST_TEST(TestInteger)
  {
    TS_ASSERT_EQUALS(comb(2, 4), 0);
    TS_ASSERT_EQUALS(comb(4, 2), 6);
    TS_ASSERT_EQUALS(comb(0, 0), 1);
    TS_ASSERT_EQUALS(comb(25, 78), 0);
    TS_ASSERT_EQUALS(comb(25, 25), 1);
    TS_ASSERT_EQUALS(comb(25, 22), 2300);
  };
};

CXXTEST_SUITE_REGISTRATION(CombTest)
CXXTEST_TEST_REGISTRATION(CombTest, TestFloat)
CXXTEST_TEST_REGISTRATION(CombTest, TestDouble)
CXXTEST_TEST_REGISTRATION(CombTest, TestInteger)

#endif // CombTest_h
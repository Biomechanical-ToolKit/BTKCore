#ifndef TypeidTest_h
#define TypeidTest_h

#include <btkTypeid.h>

#include <string>

CXXTEST_SUITE(TypeidTest)
{
  CXXTEST_TEST(AggressiveOptimization)
  {
    // Be sure the ODR works ...
    TS_ASSERT_EQUALS(btk::static_typeid<int>(), btk::static_typeid<int>());
    TS_ASSERT_EQUALS(btk::static_typeid<unsigned>(), btk::static_typeid<unsigned>());
    TS_ASSERT_EQUALS(btk::static_typeid<float>(), btk::static_typeid<float>());
    TS_ASSERT_EQUALS(btk::static_typeid<double>(), btk::static_typeid<double>());
    TS_ASSERT_EQUALS(btk::static_typeid<long double>(), btk::static_typeid<long double>());
    TS_ASSERT_EQUALS(btk::static_typeid<char>(), btk::static_typeid<char>());
    TS_ASSERT_EQUALS(btk::static_typeid<bool>(), btk::static_typeid<bool>());
    TS_ASSERT_EQUALS(btk::static_typeid<std::string>(), btk::static_typeid<std::string>());
    // ... But not too much
    TS_ASSERT_DIFFERS(btk::static_typeid<int>(), btk::static_typeid<unsigned>());
    TS_ASSERT_DIFFERS(btk::static_typeid<bool>(), btk::static_typeid<float>());
    TS_ASSERT_DIFFERS(btk::static_typeid<float>(), btk::static_typeid<double>());
    TS_ASSERT_DIFFERS(btk::static_typeid<double>(), btk::static_typeid<float>());
    TS_ASSERT_DIFFERS(btk::static_typeid<long double>(), btk::static_typeid<char>());
    TS_ASSERT_DIFFERS(btk::static_typeid<const char*>(), btk::static_typeid<bool>());
    TS_ASSERT_DIFFERS(btk::static_typeid<std::string>(), btk::static_typeid<const char*>());
  };
};

CXXTEST_SUITE_REGISTRATION(TypeidTest)
CXXTEST_TEST_REGISTRATION(TypeidTest, AggressiveOptimization)
  
#endif // TypeidTest_h
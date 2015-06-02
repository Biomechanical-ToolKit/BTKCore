#ifndef ExceptionTest_h
#define ExceptionTest_h

#include <btkException.h>

CXXTEST_SUITE(ExceptionTest)
{
  CXXTEST_TEST(Constructor)
  {
    std::string str;
    // Exception
    str = "I am an exception";
    btk::Exception e(str);
    TS_ASSERT_EQUALS(e.what(), str);
    // LogicError
    str = "I am a logical exception";
    btk::LogicError e2(str);
    TS_ASSERT_EQUALS(e2.what(), str);
    // InvalidArgument
    str = "I am an invalid argument";
    btk::InvalidArgument e3(str);
    TS_ASSERT_EQUALS(e3.what(), str);
    // OutOfRangeException
    str = "I am a out of range exception";
    btk::OutOfRangeException e4(str);
    TS_ASSERT_EQUALS(e4.what(), str);
    // DomainError
    str = "I am a domain error";
    btk::DomainError e5(str);
    TS_ASSERT_EQUALS(e5.what(), str);
    // RuntimeError
    str = "I am a runtime error";
    btk::RuntimeError e6(str);
    TS_ASSERT_EQUALS(e6.what(), str);
  };
  
  CXXTEST_TEST(Throw)
  {
    std::string str;
    // LogicError
    str = "I am a logical exception";
    TS_ASSERT_THROWS_EQUALS(throw(btk::LogicError(str)), const btk::Exception &e, e.what(), str);
    // InvalidArgument
    str = "I am an invalid argument";
    TS_ASSERT_THROWS_EQUALS(throw(btk::InvalidArgument(str)), const btk::Exception &e, e.what(), str);
    // OutOfRangeException
    str = "I am a out of range exception";
    TS_ASSERT_THROWS_EQUALS(throw(btk::OutOfRangeException(str)), const btk::Exception &e, e.what(), str);
    // DomainError
    str = "I am a domain error";
    TS_ASSERT_THROWS_EQUALS(throw(btk::DomainError(str)), const btk::Exception &e, e.what(), str);
    // RuntimeError
    str = "I am a runtime error";
    TS_ASSERT_THROWS_EQUALS(throw(btk::RuntimeError(str)), const btk::Exception &e, e.what(), str);
  };
};

CXXTEST_SUITE_REGISTRATION(ExceptionTest)
CXXTEST_TEST_REGISTRATION(ExceptionTest, Constructor)
CXXTEST_TEST_REGISTRATION(ExceptionTest, Throw)

#endif // ExceptionTest_h
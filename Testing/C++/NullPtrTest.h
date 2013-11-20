#ifndef NullPtrTest_h
#define NullPtrTest_h

#include <btkAnalog.h>

CXXTEST_SUITE(NullPtrTest)
{
  CXXTEST_TEST(AnalogFuncPointer)
  {
    btk::Analog::Pointer an;
    TS_ASSERT(an == btk::Analog::Null);
    TS_ASSERT(btk::Analog::Null == an);
    TS_ASSERT_EQUALS((an != btk::Analog::Null), false);
    TS_ASSERT_EQUALS((btk::Analog::Null != an), false);
    an = btk::Analog::New();
    TS_ASSERT(an != btk::Analog::Null);
    TS_ASSERT(btk::Analog::Null != an);
    TS_ASSERT_EQUALS((an == btk::Analog::Null), false);
    TS_ASSERT_EQUALS((btk::Analog::Null == an), false);
    
    btk::Analog::ConstPointer an_;
    TS_ASSERT(an_ == btk::Analog::Null);
    TS_ASSERT(btk::Analog::Null == an_);
    TS_ASSERT_EQUALS((an_ != btk::Analog::Null), false);
    TS_ASSERT_EQUALS((btk::Analog::Null != an_), false);
    an_ = an;
    TS_ASSERT(an_ != btk::Analog::Null);
    TS_ASSERT(btk::Analog::Null != an_);
    TS_ASSERT_EQUALS((an_ == btk::Analog::Null), false);
    TS_ASSERT_EQUALS((btk::Analog::Null == an_), false);
  };
  
  CXXTEST_TEST(AnalogReferenceEqual)
  {
    btk::Analog::Pointer an;
    TS_ASSERT(an == btk::Analog::Null());
    TS_ASSERT(btk::Analog::Null() == an);
    TS_ASSERT_EQUALS((an != btk::Analog::Null()), false);
    TS_ASSERT_EQUALS((btk::Analog::Null() != an), false);
    an = btk::Analog::New();
    TS_ASSERT(an != btk::Analog::Null());
    TS_ASSERT(btk::Analog::Null() != an);
    TS_ASSERT_EQUALS((an == btk::Analog::Null()), false);
    TS_ASSERT_EQUALS((btk::Analog::Null() == an), false);
    
    btk::Analog::ConstPointer an_;
    TS_ASSERT(an_ == btk::Analog::Null());
    TS_ASSERT(btk::Analog::Null() == an_);
    TS_ASSERT_EQUALS((an_ != btk::Analog::Null()), false);
    TS_ASSERT_EQUALS((btk::Analog::Null() != an_), false);
    an_ = an;
    TS_ASSERT(an_ != btk::Analog::Null());
    TS_ASSERT(btk::Analog::Null() != an_);
    TS_ASSERT_EQUALS((an_ == btk::Analog::Null()), false);
    TS_ASSERT_EQUALS((btk::Analog::Null() == an_), false);
  };
};


CXXTEST_SUITE_REGISTRATION(NullPtrTest)
CXXTEST_TEST_REGISTRATION(NullPtrTest, AnalogFuncPointer)
CXXTEST_TEST_REGISTRATION(NullPtrTest, AnalogReferenceEqual)

#endif // NullPtrTest_h
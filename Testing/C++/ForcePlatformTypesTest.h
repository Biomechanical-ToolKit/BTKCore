#ifndef ForcePlatformTypesTest_h
#define ForcePlatformTypesTest_h

#include <btkForcePlatformTypes.h>

CXXTEST_SUITE(ForcePlatformTypesTest)
{
  CXXTEST_TEST(ForcePlatformType1)
  {
    btk::ForcePlatform::Pointer pf = btk::ForcePlatformType1::New();
    TS_ASSERT_EQUALS(pf->GetType(), 1);
    TS_ASSERT_EQUALS(pf->GetOrigin().isZero(), true);
    TS_ASSERT_EQUALS(pf->GetChannelNumber(), 6);
  };

  CXXTEST_TEST(ForcePlatformType2)
  {
    btk::ForcePlatform::Pointer pf = btk::ForcePlatformType2::New();
    TS_ASSERT_EQUALS(pf->GetType(), 2);
    TS_ASSERT_EQUALS(pf->GetChannelNumber(), 6);
  };

  CXXTEST_TEST(ForcePlatformType3)
  {
    btk::ForcePlatform::Pointer pf = btk::ForcePlatformType3::New();
    TS_ASSERT_EQUALS(pf->GetType(), 3);
    TS_ASSERT_EQUALS(pf->GetChannelNumber(), 8);
  };

  CXXTEST_TEST(ForcePlatformType4)
  {
    btk::ForcePlatform::Pointer pf = btk::ForcePlatformType4::New();
    TS_ASSERT_EQUALS(pf->GetType(), 4);
    TS_ASSERT_EQUALS(pf->GetChannelNumber(), 6);
    TS_ASSERT_EQUALS(pf->GetCalMatrix().isIdentity(), true);
    TS_ASSERT_EQUALS(pf->GetCalMatrix().rows(), 6);
    TS_ASSERT_EQUALS(pf->GetCalMatrix().cols(), 6);
  };

  CXXTEST_TEST(ForcePlatformType5)
  {
    btk::ForcePlatform::Pointer pf = btk::ForcePlatformType5::New();
    TS_ASSERT_EQUALS(pf->GetType(), 5);
    TS_ASSERT_EQUALS(pf->GetChannelNumber(), 6);
    TS_ASSERT_EQUALS(pf->GetCalMatrix().isIdentity(), true);
    TS_ASSERT_EQUALS(pf->GetCalMatrix().rows(), 8);
    TS_ASSERT_EQUALS(pf->GetCalMatrix().cols(), 6);
  };

  CXXTEST_TEST(ForcePlatformType6)
  {
    btk::ForcePlatform::Pointer pf = btk::ForcePlatformType6::New();
    TS_ASSERT_EQUALS(pf->GetType(), 6);
    TS_ASSERT_EQUALS(pf->GetChannelNumber(), 12);
    TS_ASSERT_EQUALS(pf->GetCalMatrix().isIdentity(), true);
    TS_ASSERT_EQUALS(pf->GetCalMatrix().rows(), 12);
    TS_ASSERT_EQUALS(pf->GetCalMatrix().cols(), 12);
  };
};

CXXTEST_SUITE_REGISTRATION(ForcePlatformTypesTest)
CXXTEST_TEST_REGISTRATION(ForcePlatformTypesTest, ForcePlatformType1)
CXXTEST_TEST_REGISTRATION(ForcePlatformTypesTest, ForcePlatformType2)
CXXTEST_TEST_REGISTRATION(ForcePlatformTypesTest, ForcePlatformType3) 
CXXTEST_TEST_REGISTRATION(ForcePlatformTypesTest, ForcePlatformType4)
CXXTEST_TEST_REGISTRATION(ForcePlatformTypesTest, ForcePlatformType5)
CXXTEST_TEST_REGISTRATION(ForcePlatformTypesTest, ForcePlatformType6)

#endif // ForcePlatformTypesTest_h

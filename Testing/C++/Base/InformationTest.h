#ifndef InformationTest_h
#define InformationTest_h

#include "InformationTest_classdef.h"

CXXTEST_SUITE(InformationTest)
{
  CXXTEST_TEST(PredefinedKeys)
  {
    MyMetaInfo info;
    info.setInformation<MyMetaInfoName>("Hello");
    info.setInformation<AnotherOption>(true);
    TS_ASSERT_EQUALS(info.information<MyMetaInfoName>(), "Hello");
    TS_ASSERT_EQUALS(info.information<AnotherOption>(), true);
    TS_ASSERT_EQUALS(info.information<AnotherOptionAgain>(40), 40);
    TS_ASSERT_EQUALS(info.information("MyMetaInfoTest").cast<std::string>(), "Hello");
    TS_ASSERT_EQUALS(info.information("AnotherOption").cast<bool>(), true);
    TS_ASSERT_EQUALS(info.information("AnotherOptionAgain",40).cast<int>(), 40);
  };
  
  CXXTEST_TEST(DynamicKeys)
  {
    const char* MyMetaInfoTestName = "MyMetaInfoTest";
    const char* AnotherOptionName = "AnotherOption";
    MyMetaInfo info;
    info.setInformation(MyMetaInfoTestName,"HelloAgain");
    info.setInformation(AnotherOptionName,false);
    TS_ASSERT_EQUALS(info.information(MyMetaInfoTestName).cast<std::string>(), "HelloAgain");
    btk::Any AnotherOptionValue = info.information(AnotherOptionName);
    TS_ASSERT_EQUALS(AnotherOptionValue.isValid(), true);
    TS_ASSERT_EQUALS(AnotherOptionValue.cast<bool>(), false);
    TS_ASSERT_EQUALS(info.information("IDoNotExist", "ButIHaveADefaultValue").cast<std::string>(), "ButIHaveADefaultValue");
  };
};

CXXTEST_SUITE_REGISTRATION(InformationTest)
CXXTEST_TEST_REGISTRATION(InformationTest, PredefinedKeys)
CXXTEST_TEST_REGISTRATION(InformationTest, DynamicKeys)
  
#endif // InformationTest_h
#ifndef ObjectTest_h
#define ObjectTest_h

#include "ObjectTest_classdef.h"

CXXTEST_SUITE(ObjectTest)
{
  CXXTEST_TEST(ObjectDefaultPimpl_Constructor)
  {
    ObjectDefaultPimpl obj;
    TS_ASSERT_EQUALS(obj.timestamp(),0ul);
  };
  
  CXXTEST_TEST(ObjectDefaultPimpl_Modified)
  {
    ObjectDefaultPimpl obj;
    unsigned long ts = obj.timestamp();
    obj.modified();
    TS_ASSERT(ts != obj.timestamp());
  };
  
  CXXTEST_TEST(ObjectCustomPimpl_Constructor)
  {
    ObjectCustomPimpl obj("foo");
    TS_ASSERT_EQUALS(obj.timestamp(),0ul);
    TS_ASSERT_EQUALS(obj.getName(),"foo");
    TS_ASSERT_EQUALS(obj.getVersion(),1);
  };
  
  CXXTEST_TEST(ObjectCustomPimpl_Mutate)
  {
    ObjectCustomPimpl obj("foo");
    unsigned long ts = obj.timestamp();
    obj.setName("bar");
    TS_ASSERT_EQUALS(obj.getName(),"bar");
    TS_ASSERT(ts != obj.timestamp());
    ts = obj.timestamp();
    obj.setName("bar");
    TS_ASSERT_EQUALS(obj.getName(),"bar");
    TS_ASSERT(ts == obj.timestamp());
    obj.setVersion(2);
    TS_ASSERT_EQUALS(obj.getVersion(),2);
    TS_ASSERT(ts != obj.timestamp());
  };
};

CXXTEST_SUITE_REGISTRATION(ObjectTest)
CXXTEST_TEST_REGISTRATION(ObjectTest, ObjectDefaultPimpl_Constructor)
CXXTEST_TEST_REGISTRATION(ObjectTest, ObjectDefaultPimpl_Modified)
CXXTEST_TEST_REGISTRATION(ObjectTest, ObjectCustomPimpl_Constructor)
CXXTEST_TEST_REGISTRATION(ObjectTest, ObjectCustomPimpl_Mutate)

#endif // ObjectTest_h
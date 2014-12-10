#ifndef AnyTest_h
#define AnyTest_h

#include "AnyTest_classdef.h"

CXXTEST_SUITE(AnyTest)
{
  CXXTEST_TEST(Constructor)
  {
    btk::Any a = 12;
    TS_ASSERT_EQUALS(a.cast<int>(),12);
    TS_ASSERT_EQUALS(static_cast<int>(a),12);
    TS_ASSERT_EQUALS(static_cast<float>(a),12.0f);

    btk::Any b = 12.5f;
    TS_ASSERT_EQUALS(b.cast<int>(),12);
    TS_ASSERT_EQUALS(static_cast<int>(b),12);
    TS_ASSERT_EQUALS(b.cast<float>(),12.5f);
    TS_ASSERT_EQUALS(b.cast<double>(),12.5);

    TS_ASSERT(a == a);
    TS_ASSERT(b == b);
    TS_ASSERT(a != b);
    TS_ASSERT(b != a);

    btk::Any c = (int8_t)45;
    TS_ASSERT_EQUALS(c.cast<std::string>(),"45");

    btk::Any d = true;
    TS_ASSERT_EQUALS(d.cast<std::string>(),"true");
    TS_ASSERT_EQUALS(d.cast<int>(),1);

    btk::Any e = std::string("1.45");
    TS_ASSERT_EQUALS(e.cast<float>(),1.45f);
    TS_ASSERT_EQUALS(e.cast<int>(),1);

    btk::Any f = 45;
    TS_ASSERT_EQUALS(45,f);
    TS_ASSERT_EQUALS(f,45);
    TS_ASSERT_DIFFERS(f,false);
    TS_ASSERT_DIFFERS(12.4,f);

    btk::Any::Register<Date, btk::Any::Conversion<Date,std::string>, btk::Any::Conversion<std::string>>();

    btk::Any g = Date{1999,12,19};
    TS_ASSERT_EQUALS(g.cast<std::string>(),"1999-12-19");
    TS_ASSERT_EQUALS(g,Date({1999,12,19}));
    g = std::string("2009-05-02");
    TS_ASSERT_EQUALS(g.cast<Date>(),Date({2009,05,02}));
    TS_ASSERT_EQUALS(g.cast<bool>(),false);

    btk::Any::Unregister<Date>();
    TS_ASSERT_EQUALS(g.cast<Date>(),Date({0,0,0}));

    btk::Any h = int(0xFFFF);
    TS_ASSERT_EQUALS(h.cast<int8_t>(),int8_t(-1));

    btk::Any i = int8_t(-1);
    TS_ASSERT_EQUALS(i.cast<int>(),-1);
    TS_ASSERT_EQUALS(i.cast<double>(),-1.0);
    TS_ASSERT_EQUALS(i.cast<std::string>(),"-1");

    btk::Any n;
    TS_ASSERT_EQUALS(n.isNull(),true);
    n = 12.5f;
    TS_ASSERT_EQUALS(n.isNull(),false);
    TS_ASSERT(n == b);
    TS_ASSERT(b == n);
    TS_ASSERT(n.cast<int>() == a);
  };
};

CXXTEST_SUITE_REGISTRATION(AnyTest)
CXXTEST_TEST_REGISTRATION(AnyTest, Constructor)
 

#endif // AnyTest_h
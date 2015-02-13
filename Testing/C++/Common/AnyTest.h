#ifndef AnyTest_h
#define AnyTest_h

#include "AnyTest_classdef.h"

#include <limits>
#include <vector>

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
    d = false;
    TS_ASSERT_EQUALS(d.cast<std::string>(),"false");
    TS_ASSERT_EQUALS(d.cast<int>(),0);
    TS_ASSERT_EQUALS(d.cast<bool>(),false);
    d = "true";
    TS_ASSERT_EQUALS(d.cast<unsigned int>(),0);
    TS_ASSERT_EQUALS(d.cast<bool>(),true);
    d = "false";
    TS_ASSERT_EQUALS(d.cast<unsigned int>(),0);
    TS_ASSERT_EQUALS(d.cast<bool>(),false);

    btk::Any e = std::string("1.45");
    TS_ASSERT_EQUALS(e.cast<float>(),1.45f);
    TS_ASSERT_EQUALS(e.cast<double>(),1.45);
    // TS_ASSERT_DELTA(e.cast<long double>(),static_cast<long double>(1.45),std::numeric_limits<long double>::epsilon());
    TS_ASSERT_EQUALS(e.cast<int>(),1);
    TS_ASSERT_EQUALS(static_cast<int>(e),1);

    btk::Any f = 45;
    TS_ASSERT_EQUALS(45,f);
    TS_ASSERT_EQUALS(f,45);
    TS_ASSERT_DIFFERS(f,false);
    TS_ASSERT_DIFFERS(12.4,f);

    btk::Any g = Date{1999,12,19};
    TS_ASSERT_EQUALS(g,Date({1999,12,19}));
    TS_ASSERT_EQUALS(g.cast<std::string>(),"");
    TS_ASSERT_EQUALS(g.cast<bool>(),false);
    btk::Any::Register<Date, btk::Any::Conversion<std::string>, btk::Any::Conversion<std::string>>();
    TS_ASSERT_EQUALS(g.cast<std::string>(),"1999-12-19");
    g = std::string("2009-05-02");
    TS_ASSERT_EQUALS(g.cast<Date>(),Date({2009,05,02}));
    TS_ASSERT_EQUALS(g.cast<bool>(),true);
    btk::Any::Unregister<Date>();
    TS_ASSERT_EQUALS(g.cast<Date>(),Date({0,0,0}));
    g = Date{1999,12,19};
    TS_ASSERT_EQUALS(g,Date({1999,12,19}));
    
    g = Foo{1,1};
    TS_ASSERT_EQUALS(g,Foo({1,1}));
    TS_ASSERT_DIFFERS(g,Foo({2,2}));

    btk::Any h = int(0xFFFF);
    TS_ASSERT_EQUALS(h.cast<int8_t>(),int8_t(-1));

    btk::Any i = int8_t(-1);
    TS_ASSERT_EQUALS(i.cast<int>(),-1);
    TS_ASSERT_EQUALS(i.cast<double>(),-1.0);
    TS_ASSERT_EQUALS(i.cast<std::string>(),"-1");

    btk::Any n;
    TS_ASSERT_EQUALS(n.isValid(),false);
    n = 12.5f;
    TS_ASSERT_EQUALS(n.isValid(),true);
    TS_ASSERT(n == b);
    TS_ASSERT(b == n);
    TS_ASSERT(n.cast<int>() == a);
    TS_ASSERT(static_cast<int>(n) == a);
    
    btk::Any m("foo");
    TS_ASSERT_EQUALS(m.cast<std::string>(),"foo");
    TS_ASSERT_EQUALS(m.cast<int>(),0);
    TS_ASSERT_EQUALS(static_cast<int>(m),0);
    TS_ASSERT_EQUALS(static_cast<float>(m),0.0f);
    TS_ASSERT_EQUALS(static_cast<double>(m),0.0);
    TS_ASSERT_EQUALS(static_cast<bool>(m),true);
  };
  
  CXXTEST_TEST(Vector)
  {
    std::vector<int> foo{{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}};
    std::vector<int> bar;
    // Vector constructor
    btk::Any a = foo;
    TS_ASSERT_EQUALS(a.dimensions().empty(),false);
    TS_ASSERT_EQUALS(a.dimensions().size(),1ul);
    if (!a.dimensions().empty())
      TS_ASSERT_EQUALS(a.dimensions()[0],10ul);
    TS_ASSERT_EQUALS(a.size(),10ul);
    TS_ASSERT_EQUALS(a.cast<int>(),1);
    bar = a.cast<std::vector<int>>();
    TS_ASSERT_EQUALS(foo,bar);
    // Initializer constructor
    // - Dimensions and values matching
    foo = {1,2,3,4}; // std::vecotr<int>{{1,2,3,4}};
    a = btk::Any({1,2,3,4},{2,2});
    TS_ASSERT_EQUALS(a.dimensions().empty(),false);
    TS_ASSERT_EQUALS(a.dimensions().size(),2ul);
    if (!a.dimensions().empty())
    {
      TS_ASSERT_EQUALS(a.dimensions()[0],2ul);
      TS_ASSERT_EQUALS(a.dimensions()[1],2ul);
    }
    TS_ASSERT_EQUALS(a.size(),4ul);
    TS_ASSERT_EQUALS(a.cast<int>(),1);
    bar = a.cast<std::vector<int>>();
    TS_ASSERT_EQUALS(foo,bar);
    // - Dimensions and values not matching (less values than dimensions)
    a = btk::Any({1,2,3,4},{3,3});
    TS_ASSERT_EQUALS(a.dimensions().empty(),false);
    TS_ASSERT_EQUALS(a.dimensions().size(),2ul);
    if (!a.dimensions().empty())
    {
      TS_ASSERT_EQUALS(a.dimensions()[0],3ul);
      TS_ASSERT_EQUALS(a.dimensions()[1],3ul);
    }
    TS_ASSERT_EQUALS(a.size(),9ul);
    TS_ASSERT_EQUALS(a.cast<int>(),1);
    bar = a.cast<std::vector<int>>();
    foo = {1,2,3,4,0,0,0,0,0};
    TS_ASSERT_EQUALS(foo,bar);
    // - Dimensions and values not matching (more values than dimensions)
    a = btk::Any(foo,std::vector<size_t>({2,2}));
    TS_ASSERT_EQUALS(a.dimensions().empty(),false);
    TS_ASSERT_EQUALS(a.dimensions().size(),2ul);
    if (!a.dimensions().empty())
    {
      TS_ASSERT_EQUALS(a.dimensions()[0],2ul);
      TS_ASSERT_EQUALS(a.dimensions()[1],2ul);
    }
    TS_ASSERT_EQUALS(a.size(),4ul);
    TS_ASSERT_EQUALS(a.cast<int>(),1);
    bar = a.cast<std::vector<int>>();
    foo = {1,2,3,4};
    TS_ASSERT_EQUALS(foo,bar);
    // Implicit initializer constructor
    foo = {1,2,3,4};
    a = {1,2,3,4};
    TS_ASSERT_EQUALS(a.dimensions().empty(),false);
    TS_ASSERT_EQUALS(a.dimensions().size(),1ul);
    if (!a.dimensions().empty())
      TS_ASSERT_EQUALS(a.dimensions()[0],4ul);
    TS_ASSERT_EQUALS(a.size(),4ul);
    TS_ASSERT_EQUALS(a.cast<int>(),1);
    bar = a.cast<std::vector<int>>();
    TS_ASSERT_EQUALS(foo,bar);
    // (un)Equality
    TS_ASSERT_EQUALS(a,bar);
    btk::Any b = btk::Any({1,2,3,4},{2,2});
    TS_ASSERT_DIFFERS(a,b);
    b = {1,2,3,4};
    TS_ASSERT_EQUALS(a,b);
    // Another test with strings
    a = std::vector<std::string>({"Coco","Vroum","Another"});
    TS_ASSERT_EQUALS(a.cast<std::string>(),"Coco");
    TS_ASSERT_EQUALS(strcmp(a.cast<const char*>(),"Coco"),0);
    a = {"Coco","Vroum","Another"};
    TS_ASSERT_EQUALS(a.cast<std::string>(),"Coco");
    TS_ASSERT_EQUALS(strcmp(a.cast<const char*>(),"Coco"),0);
    auto toto = a.cast<std::vector<const char*>>();
    TS_ASSERT_EQUALS(toto.size(),3);
    TS_ASSERT_EQUALS(strcmp(toto[0],"Coco"),0);
    TS_ASSERT_EQUALS(strcmp(toto[1],"Vroum"),0);
    TS_ASSERT_EQUALS(strcmp(toto[2],"Another"),0);
  };
};

CXXTEST_SUITE_REGISTRATION(AnyTest)
CXXTEST_TEST_REGISTRATION(AnyTest, Constructor)
CXXTEST_TEST_REGISTRATION(AnyTest, Vector)

#endif // AnyTest_h
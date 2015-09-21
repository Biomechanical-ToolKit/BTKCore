#ifndef AnyTest_h
#define AnyTest_h

#include "AnyTest_classdef.h"

#include <btkTypeid.h>

#include <limits>
#include <vector>

CXXTEST_SUITE(AnyTest)
{
  CXXTEST_TEST(Single_Int)
  {
    btk::Any a = 12;
    TS_ASSERT_EQUALS(a.cast<int>(),12);
    TS_ASSERT_EQUALS(static_cast<int>(a),12);
    TS_ASSERT_EQUALS(static_cast<float>(a),12.0f);
  };
  
  CXXTEST_TEST(Single_Float)
  {
    btk::Any b = 12.5f;
    TS_ASSERT_EQUALS(b.cast<int>(),12);
    TS_ASSERT_EQUALS(static_cast<int>(b),12);
    TS_ASSERT_EQUALS(b.cast<float>(),12.5f);
    TS_ASSERT_EQUALS(b.cast<double>(),12.5);
  };
  
  CXXTEST_TEST(Single_Char)
  {
    btk::Any c = 'a';
    TS_ASSERT_EQUALS(c.cast<int>(),97);
    TS_ASSERT_EQUALS(static_cast<float>(c),97.0f);
    TS_ASSERT_EQUALS(c.cast<char>(),'a');
    TS_ASSERT_EQUALS(c.cast<std::string>(),"97"); // Because internally a (signed) char is casted into a (signed) short int before to be converted into a string. This was chosen to priorize int8_t to string conversion.
  };
  
  CXXTEST_TEST(Single_Comparison)
  {
    btk::Any a = 12;
    btk::Any b = 12.5f;
    btk::Any c = "Single_Comparison";
    btk::Any d; // invalid
    
    TS_ASSERT(a == a);
    TS_ASSERT(a == 12);
    TS_ASSERT(12 == a);
    TS_ASSERT(a != 0);
    TS_ASSERT(0 != a);
    
    TS_ASSERT(b == b);
    TS_ASSERT(b == 12.5f);
    TS_ASSERT(12.5f == b);
    TS_ASSERT(b != 0.0f);
    TS_ASSERT(0.0f != b);
    
    TS_ASSERT(c == c);
    TS_ASSERT(c == "Single_Comparison");
    TS_ASSERT("Single_Comparison" == c);
    TS_ASSERT(c != "");
    TS_ASSERT("" != c);
    
    TS_ASSERT(d != d);
    TS_ASSERT(d != "invalid");
    TS_ASSERT("invalid" != d);
    
    TS_ASSERT(a == b); // b is casted to int
    TS_ASSERT(a != c);
    TS_ASSERT(a != d);
    
    TS_ASSERT(b != a);
    TS_ASSERT(b != c);
    TS_ASSERT(b != d);
    
    TS_ASSERT(c != a);
    TS_ASSERT(c != b);
    TS_ASSERT(c != d);
    
    TS_ASSERT(d != a);
    TS_ASSERT(d != b);
    TS_ASSERT(d != c);
  };
  
  CXXTEST_TEST(Single_Comparison2)
  {
    btk::Any a = 12;
    btk::Any b = 12.5f;
    btk::Any n;
    TS_ASSERT_EQUALS(n.isValid(),false);
    n = 12.5f;
    TS_ASSERT_EQUALS(n.isValid(),true);
    TS_ASSERT(n == b);
    TS_ASSERT(b == n);
    TS_ASSERT(n.cast<int>() == a);
    TS_ASSERT(static_cast<int>(n) == a);
    TS_ASSERT(n != a);
    TS_ASSERT(a == n);
  };
  
  CXXTEST_TEST(Single_Int8ToString)
  {
    btk::Any c = (int8_t)45;
    TS_ASSERT_EQUALS(c.cast<std::string>(),"45");
  };
  
  CXXTEST_TEST(Single_BoolAndString)
  {
    btk::Any d = true;
    TS_ASSERT_EQUALS(d.cast<std::string>(),"true");
    TS_ASSERT_EQUALS(d.cast<int>(),1);
    d = false;
    TS_ASSERT_EQUALS(d.cast<std::string>(),"false");
    TS_ASSERT_EQUALS(d.cast<int>(),0);
    TS_ASSERT_EQUALS(d.cast<bool>(),false);
    d = "true";
    TS_ASSERT_EQUALS(d.cast<unsigned int>(),0u);
    TS_ASSERT_EQUALS(d.cast<bool>(),true);
    d = "false";
    TS_ASSERT_EQUALS(d.cast<unsigned int>(),0u);
    TS_ASSERT_EQUALS(d.cast<bool>(),false);
  };
  
  CXXTEST_TEST(Single_String)
  {
    btk::Any e = std::string("1.45");
    TS_ASSERT_EQUALS(e.cast<float>(),1.45f);
    TS_ASSERT_EQUALS(e.cast<double>(),1.45);
    // TS_ASSERT_DELTA(e.cast<long double>(),static_cast<long double>(1.45),std::numeric_limits<long double>::epsilon());
    TS_ASSERT_EQUALS(e.cast<int>(),1);
    TS_ASSERT_EQUALS(static_cast<int>(e),1);
  };  
  
  CXXTEST_TEST(Single_IntImplicitConversion)
  {
    btk::Any f = 45;
    TS_ASSERT_EQUALS(45,f);
    TS_ASSERT_EQUALS(f,45);
    TS_ASSERT_DIFFERS(f,false);
    TS_ASSERT_DIFFERS(12.4,f);
  };
  
  CXXTEST_TEST(Single_CustomTypeRegistered)
  {
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
  };
  
  CXXTEST_TEST(Single_CustomTypeNotRegistered)
  {
    btk::Any g = Foo{1,1};
    TS_ASSERT_EQUALS(g,Foo({1,1}));
    TS_ASSERT_DIFFERS(g,Foo({2,2}));
  };

  CXXTEST_TEST(Single_IntHexadecimal)
  {
    btk::Any h = int(0xFFFF);
    TS_ASSERT_EQUALS(h.cast<int8_t>(),int8_t(-1));
  };
  
  CXXTEST_TEST(Single_Int8)
  {
    btk::Any i = int8_t(-1);
    TS_ASSERT_EQUALS(i.cast<int>(),-1);
    TS_ASSERT_EQUALS(i.cast<double>(),-1.0);
    TS_ASSERT_EQUALS(i.cast<std::string>(),"-1");
  };
  
  CXXTEST_TEST(Single_FromString)
  {
    btk::Any m("foo");
    TS_ASSERT_EQUALS(m.cast<std::string>(),"foo");
    TS_ASSERT_EQUALS(m.cast<int>(),0);
    TS_ASSERT_EQUALS(static_cast<int>(m),0);
    TS_ASSERT_EQUALS(static_cast<float>(m),0.0f);
    TS_ASSERT_EQUALS(static_cast<double>(m),0.0);
    TS_ASSERT_EQUALS(static_cast<bool>(m),true);
  };
  
  CXXTEST_TEST(Single_Enum)
  {
    enum {Foo= 1, Bar = 2, Toto = 4};
    btk::Any m(Toto);
    TS_ASSERT_EQUALS(m.cast<std::string>(),"4");
    TS_ASSERT_EQUALS(m.cast<int>(),4);
  };
  
  CXXTEST_TEST(Single_IntToArray)
  {
    btk::Any a = 12;
    std::vector<int> ref{12};
    TS_ASSERT_EQUALS(a.cast<std::vector<int>>(),ref);
  };
  
  CXXTEST_TEST(Single_CustomToArray)
  {
    btk::Any g = Foo{1,1};
    std::vector<Foo> ref{{1,1}};
    TS_ASSERT_EQUALS(g.cast<std::vector<Foo>>(),ref);
  };
  
  CXXTEST_TEST(Single_Typeid)
  {
    btk::Any b = btk::static_typeid<int>();
    TS_ASSERT_EQUALS(b.cast<btk::typeid_t>(),btk::static_typeid<int>());
    TS_ASSERT_DIFFERS(b.cast<btk::typeid_t>(),btk::static_typeid<float>());
  };
  
  CXXTEST_TEST(Array_Int_Vector)
  {
    std::vector<int> bar, foo{{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}};
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
  };
  
  CXXTEST_TEST(Array_Int_Vector2)
  {
    std::vector<int> foo = {1,2,3,4};
    // Use a vector of dimensions already set
    std::vector<uint8_t> dims = {1,3};
    btk::Any a = btk::Any(foo,dims);
    TS_ASSERT_EQUALS(a.dimensions().empty(),false);
    TS_ASSERT_EQUALS(a.dimensions().size(),2ul);
    if (!a.dimensions().empty())
    {
      TS_ASSERT_EQUALS(a.dimensions()[0],1ul);
      TS_ASSERT_EQUALS(a.dimensions()[1],3ul);
    }
    TS_ASSERT_EQUALS(a.size(),3ul);
    TS_ASSERT_EQUALS(a.cast<int>(),1);
    std::vector<int> bar = a.cast<std::vector<int>>();
    foo = {1,2,3};
    TS_ASSERT_EQUALS(foo,bar);
  };
  
  CXXTEST_TEST(Array_Int_Vector3)
  {
    std::vector<int> foo = {1,2,3,4};
    // - Dimensions and values not matching (more values than dimensions)
    btk::Any a = btk::Any(foo,std::vector<size_t>({2,2}));
    TS_ASSERT_EQUALS(a.dimensions().empty(),false);
    TS_ASSERT_EQUALS(a.dimensions().size(),2ul);
    if (!a.dimensions().empty())
    {
      TS_ASSERT_EQUALS(a.dimensions()[0],2ul);
      TS_ASSERT_EQUALS(a.dimensions()[1],2ul);
    }
    TS_ASSERT_EQUALS(a.size(),4ul);
    TS_ASSERT_EQUALS(a.cast<int>(),1);
    std::vector<int> bar = a.cast<std::vector<int>>();
    foo = {1,2,3,4};
    TS_ASSERT_EQUALS(foo,bar);
  };
  
  CXXTEST_TEST(Array_Int_Initializer)
  {
    // Initializer constructor
    // - Dimensions and values matching
    std::vector<int> foo = {1,2,3,4};
    btk::Any a = btk::Any({1,2,3,4},{2,2});
    TS_ASSERT_EQUALS(a.dimensions().empty(),false);
    TS_ASSERT_EQUALS(a.dimensions().size(),2ul);
    if (!a.dimensions().empty())
    {
      TS_ASSERT_EQUALS(a.dimensions()[0],2ul);
      TS_ASSERT_EQUALS(a.dimensions()[1],2ul);
    }
    TS_ASSERT_EQUALS(a.size(),4ul);
    TS_ASSERT_EQUALS(a.cast<int>(),1);
    std::vector<int> bar = a.cast<std::vector<int>>();
    TS_ASSERT_EQUALS(foo,bar);
  };
  
  CXXTEST_TEST(Array_Int_Initializer2)
  {
    // - Dimensions and values not matching (less values than dimensions)
    btk::Any a = btk::Any({1,2,3,4},{3,3});
    TS_ASSERT_EQUALS(a.dimensions().empty(),false);
    TS_ASSERT_EQUALS(a.dimensions().size(),2ul);
    if (!a.dimensions().empty())
    {
      TS_ASSERT_EQUALS(a.dimensions()[0],3ul);
      TS_ASSERT_EQUALS(a.dimensions()[1],3ul);
    }
    TS_ASSERT_EQUALS(a.size(),9ul);
    TS_ASSERT_EQUALS(a.cast<int>(),1);
    std::vector<int> bar = a.cast<std::vector<int>>();
    std::vector<int> foo = {1,2,3,4,0,0,0,0,0};
    TS_ASSERT_EQUALS(foo,bar);
  };
  
  CXXTEST_TEST(Array_Int_Initializer3)
  {
    // Implicit initializer constructor
    std::vector<int> foo = {1,2,3,4};
    btk::Any a = {1,2,3,4};
    TS_ASSERT_EQUALS(a.dimensions().empty(),false);
    TS_ASSERT_EQUALS(a.dimensions().size(),1ul);
    if (!a.dimensions().empty())
      TS_ASSERT_EQUALS(a.dimensions()[0],4ul);
    TS_ASSERT_EQUALS(a.size(),4ul);
    TS_ASSERT_EQUALS(a.cast<int>(),1);
    for (size_t i = 0 ; i < foo.size() ; ++i)
      TS_ASSERT_EQUALS(a.cast<int>(i),foo[i]);
    std::vector<int> bar = a.cast<std::vector<int>>();
    TS_ASSERT_EQUALS(foo,bar);
  };
  
  CXXTEST_TEST(Array_Int_Comparison)
  {
    btk::Any a = {1,2,3,4};
    std::vector<int> bar = {1,2,3,4};
    TS_ASSERT_EQUALS(a,bar);
    btk::Any b = btk::Any({1,2,3,4},{2,2});
    TS_ASSERT_EQUALS(a,b);
    b = {1,2,3,4};
    TS_ASSERT_EQUALS(a,b);
    b = {1,2,3};
    TS_ASSERT_DIFFERS(a,b);
  };
  
  CXXTEST_TEST(Array_String)
  {
    // Another test with strings
    btk::Any a = std::vector<std::string>({"Coco","Vroum","Another"});
    TS_ASSERT_EQUALS(a.cast<std::string>(),"Coco");
    TS_ASSERT_EQUALS(strcmp(a.cast<const char*>(),"Coco"),0);
    a = {"Coco","Vroum","Another"};
    TS_ASSERT_EQUALS(a.cast<std::string>(),"Coco");
    TS_ASSERT_EQUALS(strcmp(a.cast<const char*>(),"Coco"),0);
    auto toto = a.cast<std::vector<const char*>>();
    TS_ASSERT_EQUALS(toto.size(),3ul);
    TS_ASSERT_EQUALS(strcmp(toto[0],"Coco"),0);
    TS_ASSERT_EQUALS(strcmp(toto[1],"Vroum"),0);
    TS_ASSERT_EQUALS(strcmp(toto[2],"Another"),0);
    a = btk::Any(std::vector<std::string>({"Coco","Vroum","Another"}),std::vector<size_t>(1,4));
    auto bar = a.cast<std::vector<std::string>>();
    TS_ASSERT_EQUALS(bar.size(),4ul);
    TS_ASSERT_EQUALS(bar[0],std::string("Coco"));
    TS_ASSERT_EQUALS(bar[1],std::string("Vroum"));
    TS_ASSERT_EQUALS(bar[2],std::string("Another"));
    TS_ASSERT_EQUALS(bar[3],std::string(""));
    TS_ASSERT_EQUALS(a.cast<std::string>(0),std::string("Coco"));
    TS_ASSERT_EQUALS(a.cast<std::string>(1),std::string("Vroum"));
    TS_ASSERT_EQUALS(a.cast<std::string>(2),std::string("Another"));
    TS_ASSERT_EQUALS(a.cast<std::string>(3),std::string(""));
  };
  
  CXXTEST_TEST(Array_String_Empty)
  {
    std::vector<std::string> p;
    std::vector<size_t> dims{0};
    btk::Any a{p,dims};
    TS_ASSERT_EQUALS(a.isValid(),true);
    TS_ASSERT_EQUALS(a.isEmpty(),true);
    p = {{"foo"}};
    a = p;
    TS_ASSERT_EQUALS(a.isValid(),true);
    TS_ASSERT_EQUALS(a.isEmpty(),false);
    a = std::vector<std::string>{};
    TS_ASSERT_EQUALS(a.isValid(),true);
    TS_ASSERT_EQUALS(a.isEmpty(),true);
  }
  
  CXXTEST_TEST(Array_CustomType)
  {
    btk::Any a = {Date{2009,05,01},Date{2005,12,12},Date{1945,07,23}};
    TS_ASSERT_EQUALS(a.cast<Date>(),Date({2009,05,01}));
    TS_ASSERT_EQUALS(a.cast<Date>(0),Date({2009,05,01}));
    TS_ASSERT_EQUALS(a.cast<Date>(1),Date({2005,12,12}));
    TS_ASSERT_EQUALS(a.cast<Date>(2),Date({1945,07,23}));
    TS_ASSERT_EQUALS(a.cast<std::vector<std::string>>(),std::vector<std::string>({"","",""}));  
    btk::Any::Register<Date, btk::Any::Conversion<std::string>, btk::Any::Conversion<std::string>>();
    TS_ASSERT_EQUALS(a.cast<std::vector<std::string>>(),std::vector<std::string>({"2009-05-01","2005-12-12","1945-07-23"}));  
    TS_ASSERT_EQUALS(a.cast<std::vector<int>>(),std::vector<int>({0,0,0}));  
    btk::Any::Unregister<Date>();
    TS_ASSERT_EQUALS(a.cast<std::vector<std::string>>(),std::vector<std::string>({"","",""}));
  }
    
  CXXTEST_TEST(Array_FromSingle)
  {
    btk::Any a = 1.2f;
    TS_ASSERT_EQUALS(a.cast<std::vector<float>>(),std::vector<float>({1.2f}));
  }
  
  CXXTEST_TEST(Array_FromSingle_CustomType)
  {
    btk::Any a = "2009-05-02";
    btk::Any::Register<Date, btk::Any::Conversion<std::string>, btk::Any::Conversion<std::string>>();
    TS_ASSERT_EQUALS(a.cast<std::vector<Date>>(),std::vector<Date>({Date({2009,05,02})}));
    a = Date{2014,03,01}; // The conversion to a string is correctly "2014-3-1".
    TS_ASSERT_EQUALS(a.cast<std::vector<std::string>>(),std::vector<std::string>({"2014-3-1"}));
    btk::Any::Unregister<Date>();
  }
  
  CXXTEST_TEST(Array_Int_Array)
  {
    std::array<int,4> bar, foo{{1, 2, 3, 4}};
    // Vector constructor
    btk::Any a = foo;
    TS_ASSERT_EQUALS(a.dimensions().empty(),false);
    TS_ASSERT_EQUALS(a.dimensions().size(),1ul);
    if (!a.dimensions().empty())
      TS_ASSERT_EQUALS(a.dimensions()[0],4ul);
    TS_ASSERT_EQUALS(a.size(),4ul);
    TS_ASSERT_EQUALS(a.cast<int>(),1);
    bar = a.cast<std::array<int,4>>();
    TS_ASSERT_EQUALS(foo,bar);
  };
  
  CXXTEST_TEST(Array_Enum)
  {
    enum class myEnum {Foo= 1, Bar = 2, Toto = 4};
    btk::Any m(std::vector<myEnum>{myEnum::Foo,myEnum::Toto});
    TS_ASSERT_EQUALS(m.cast<int>(0),1);
    TS_ASSERT_EQUALS(m.cast<int>(1),4);
    TS_ASSERT_EQUALS(m.cast<std::string>(0),"1");
    TS_ASSERT_EQUALS(m.cast<std::string>(1),"4");
    TS_ASSERT_EQUALS(m.cast<myEnum>(0),myEnum::Foo);
    TS_ASSERT_EQUALS(m.cast<myEnum>(1),myEnum::Toto);
    TS_ASSERT_EQUALS(m.cast<std::vector<int>>(),std::vector<int>({1,4}));
    TS_ASSERT_EQUALS(m.cast<std::vector<std::string>>(),std::vector<std::string>({"1","4"}));
    TS_ASSERT_EQUALS(m.cast<std::vector<myEnum>>(),std::vector<myEnum>({myEnum::Foo,myEnum::Toto}));
  };
  
  CXXTEST_TEST(Array_Char)
  {
    std::array<char,4> foo{{'a','b','c','d'}};
    btk::Any a = foo;
    TS_ASSERT_EQUALS(a.cast<char>(0),'a');
    TS_ASSERT_EQUALS(a.cast<char>(1),'b');
    TS_ASSERT_EQUALS(a.cast<char>(2),'c');
    TS_ASSERT_EQUALS(a.cast<char>(3),'d');
    TS_ASSERT_EQUALS(a.cast<std::vector<char>>(),std::vector<char>({'a','b','c','d'}));
    TS_ASSERT_EQUALS(a.cast<std::vector<int>>(),std::vector<int>({97,98,99,100}));
    TS_ASSERT_EQUALS(a.cast<std::vector<std::string>>(),std::vector<std::string>({"97","98","99","100"}));
    
  }
};

CXXTEST_SUITE_REGISTRATION(AnyTest)
  
CXXTEST_TEST_REGISTRATION(AnyTest, Single_Int)
CXXTEST_TEST_REGISTRATION(AnyTest, Single_Float)
CXXTEST_TEST_REGISTRATION(AnyTest, Single_Char)
CXXTEST_TEST_REGISTRATION(AnyTest, Single_Comparison)
CXXTEST_TEST_REGISTRATION(AnyTest, Single_Comparison2)
CXXTEST_TEST_REGISTRATION(AnyTest, Single_Int8ToString)
CXXTEST_TEST_REGISTRATION(AnyTest, Single_BoolAndString)
CXXTEST_TEST_REGISTRATION(AnyTest, Single_String)
CXXTEST_TEST_REGISTRATION(AnyTest, Single_IntImplicitConversion)
CXXTEST_TEST_REGISTRATION(AnyTest, Single_CustomTypeRegistered)
CXXTEST_TEST_REGISTRATION(AnyTest, Single_CustomTypeNotRegistered)
CXXTEST_TEST_REGISTRATION(AnyTest, Single_IntHexadecimal)
CXXTEST_TEST_REGISTRATION(AnyTest, Single_Int8)
CXXTEST_TEST_REGISTRATION(AnyTest, Single_FromString)
CXXTEST_TEST_REGISTRATION(AnyTest, Single_Enum)
CXXTEST_TEST_REGISTRATION(AnyTest, Single_IntToArray)
CXXTEST_TEST_REGISTRATION(AnyTest, Single_CustomToArray)
CXXTEST_TEST_REGISTRATION(AnyTest, Single_Typeid)

CXXTEST_TEST_REGISTRATION(AnyTest, Array_Int_Vector)
CXXTEST_TEST_REGISTRATION(AnyTest, Array_Int_Vector2)
CXXTEST_TEST_REGISTRATION(AnyTest, Array_Int_Vector3)
CXXTEST_TEST_REGISTRATION(AnyTest, Array_Int_Initializer)
CXXTEST_TEST_REGISTRATION(AnyTest, Array_Int_Initializer2)
CXXTEST_TEST_REGISTRATION(AnyTest, Array_Int_Initializer3)
CXXTEST_TEST_REGISTRATION(AnyTest, Array_Int_Comparison)
CXXTEST_TEST_REGISTRATION(AnyTest, Array_String)
CXXTEST_TEST_REGISTRATION(AnyTest, Array_String_Empty)
CXXTEST_TEST_REGISTRATION(AnyTest, Array_FromSingle)
CXXTEST_TEST_REGISTRATION(AnyTest, Array_FromSingle_CustomType)
CXXTEST_TEST_REGISTRATION(AnyTest, Array_Int_Array)
CXXTEST_TEST_REGISTRATION(AnyTest, Array_Enum)
CXXTEST_TEST_REGISTRATION(AnyTest, Array_Char)

#endif // AnyTest_h
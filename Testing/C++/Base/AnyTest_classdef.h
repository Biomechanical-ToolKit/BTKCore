#ifndef AnyTest_classdef_h
#define AnyTest_classdef_h

#include <btkAny.h>

#include <string>

struct Date
{
  int Year, Month, Day;
  friend bool operator==(const Date& lhs,const Date& rhs) {return ((lhs.Year == rhs.Year) && (lhs.Month == rhs.Month) && (lhs.Day == rhs.Day));};
};

struct Foo
{
  int Bar1, Bar2;
  friend bool operator==(const Foo& lhs,const Foo& rhs) {return ((lhs.Bar1 == rhs.Bar1) && (lhs.Bar2 == rhs.Bar2));};
};

// THE SPECIALIZATION MUST BE WRAPPED IN THE NAMESPACE. THE LATER CANNOT BE PASSED DIRECTLY IN THE FUNCTION DEFINITION
namespace btk
{
  template<>
  struct Any::Helper<std::string,Date>
  {
    static inline Date convert(const std::string& ) {return Date{2009,05,02};};
  };

  template<>
  struct btk::Any::Helper<Date,std::string>
  {
    static inline std::string convert(const Date& val) {return std::to_string(val.Year) + "-" + std::to_string(val.Month) + "-" + std::to_string(val.Day);};
  };
};

#endif
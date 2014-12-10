#ifndef AnyTest_classdef_h
#define AnyTest_classdef_h

#include <btkAny.h>

#include <string>

struct Date
{
  int Year, Month, Day;
  friend bool operator==(const Date& lhs,const Date& rhs) {return ((lhs.Year == rhs.Year) && (lhs.Month == rhs.Month) && (lhs.Day == rhs.Day));};
};

template<> struct btk::Any::Traits<Date> : btk::Any::TraitsBase
{
  enum {ID = Any::TraitsBase::User + 1};
};

template<>
struct btk::Any::Converter::Helper<std::string,Date> : btk::Any::Converter::HelperBase<std::string,Date>
{
  static inline Date convert(const std::string& ) {return Date{2009,05,02};};
};

template<>
struct btk::Any::Converter::Helper<Date,std::string> : btk::Any::Converter::HelperBase<Date,std::string>
{
  static inline std::string convert(const Date& val) {return std::to_string(val.Year) + "-" + std::to_string(val.Month) + "-" + std::to_string(val.Day);};
};

#endif
#ifndef InformationTest_classdef_h
#define InformationTest_classdef_h

#include <btkInformation.h>

// Information options

struct MyMetaInfoName
{
  static inline _BTK_CONSTEXPR_CONST char* key() _BTK_NOEXCEPT {return "MyMetaInfoTest";};
  using ValueType = std::string;
};

BTK_DECLARE_METAINFO(AnotherOption, bool)
BTK_DECLARE_METAINFO(AnotherOptionAgain, int)

// ------------------------------------------------------------------------- //

class MyMetaInfo
{
public:
  template <typename U> typename U::ValueType information(const typename U::ValueType& defaultValue = typename U::ValueType()) const _BTK_NOEXCEPT;
  btk::Any information(const char* key, const btk::Any& defaultValue = btk::Any()) const _BTK_NOEXCEPT;
  template <typename U> void setInformation(const typename U::ValueType& value);
  void setInformation(const char* key, const btk::Any& value);
  
  btk::Information Info;
};

btk::Any MyMetaInfo::information(const char* key, const btk::Any& defaultValue) const _BTK_NOEXCEPT
{
  return this->Info.value(key,std::move(defaultValue));
};

void MyMetaInfo::setInformation(const char* key, const btk::Any& value)
{
  this->Info.add(key,std::move(value));
};

template <typename U>
typename U::ValueType MyMetaInfo::information(const typename U::ValueType& defaultValue) const _BTK_NOEXCEPT
{
  return this->information(U::key(),defaultValue).template cast<typename U::ValueType>();
};

template <typename U>
void MyMetaInfo::setInformation(const typename U::ValueType& value)
{
  this->setInformation(U::key(),value);
};

#endif
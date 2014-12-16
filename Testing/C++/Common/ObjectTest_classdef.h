#ifndef ObjectTest_classdef_h
#define ObjectTest_classdef_h

#include <btkObject.h>
#include <btkObject_p.h>

// ------------------------------------------------------------------------- //

// Test object using default private implementation of the btk::Object class

class ObjectDefaultPimpl : public btk::Object
{
public:
  ObjectDefaultPimpl() = default;
};

// ------------------------------------------------------------------------- //

// Test object using custom private implementation of the d-pointer used in the btk::Object class

struct ObjectCustomPimplPrivate;

class ObjectCustomPimpl : public btk::Object
{
  BTK_DECLARE_PIMPL_ACCESSOR(ObjectCustomPimpl)
    
public:
  const std::string& getName() const;
  void setName(const std::string& name);
  int getVersion() const;
  void setVersion(int ver);
  
  ObjectCustomPimpl() = delete;
  ObjectCustomPimpl(const std::string& name);
};

struct ObjectCustomPimplPrivate : public btk::ObjectPrivate
{
  ObjectCustomPimplPrivate(const std::string& name)
  : ObjectPrivate(), Name(name), Version(1) {};
  std::string Name;
  int Version;
};

ObjectCustomPimpl::ObjectCustomPimpl(const std::string& name)
: btk::Object(*new ObjectCustomPimplPrivate(name))
{};

const std::string& ObjectCustomPimpl::getName() const
{
  auto optr = this->pimpl();
  return optr->Name;
};

void ObjectCustomPimpl::setName(const std::string& val)
{
  auto optr = this->pimpl();
  if (optr->Name != val)
  {
    optr->Name = val;
    this->modified();
  }
};

int ObjectCustomPimpl::getVersion() const
{
  auto optr = this->pimpl();
  return optr->Version;
};

void ObjectCustomPimpl::setVersion(int val)
{
  auto optr = this->pimpl();
  if (optr->Version != val)
  {
    optr->Version = val;
    this->modified();
  }
};

#endif //ObjectTest_classdef_h
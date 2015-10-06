#ifndef AlgorithmTest_classdef_h
#define AlgorithmTest_classdef_h

#include <btkAlgorithm.h>
#include <btkPort.h>
#include <btkNode.h>
#include <btkProperty.h>
#include <btkDataFactory.h>
#include <btkShared.h>


class MyCounterAlgorithm : public btk::Algorithm
{
public:
  MyCounterAlgorithm();
  ~MyCounterAlgorithm() _BTK_NOEXCEPT = default;
  
protected:
  virtual bool fillInputInformation(btk::Port* port) const;
  virtual bool fillOutputInformation(btk::Port* port) const;
  virtual btk::Node* createOutputData(unsigned port) const;
  virtual bool updateOutputData(const std::vector<const btk::Node*>& in, const std::vector<btk::Node*>& out) override;
};

MyCounterAlgorithm::MyCounterAlgorithm()
: btk::Algorithm()
{
  this->setInputsNumber(1);
  this->setOutputsNumber(1);
};

bool MyCounterAlgorithm::fillInputInformation(btk::Port* port) const
{
  port->setInformation<btk::InputDataRequiredTypeid>(btk::static_typeid<btk::Node>());
  return true;
};

bool MyCounterAlgorithm::fillOutputInformation(btk::Port* port) const
{
  port->setInformation<btk::OutputDataTypeid>(btk::static_typeid<btk::Node>());
  return true;
};

btk::Node* MyCounterAlgorithm::createOutputData(unsigned port) const
{
  _BTK_UNUSED(port)
  return new btk::Node("MyCounter");
};

bool MyCounterAlgorithm::updateOutputData(const std::vector<const btk::Node*>& in, const std::vector<btk::Node*>& out)
{
  _BTK_UNUSED(in)
  out[0]->setProperty("counter", out[0]->property("counter").cast<int>() + 1);
  return true;
};

// ------------------------------------------------------------------------- //

class MySourcePrivate;

class MySource : public btk::Node
{
  BTK_DECLARE_PIMPL_ACCESSOR(MySource)
  BTK_DECLARE_NODEID(MySource, btk::Node)
  
public:
  MySource(const std::string& name, int value = 1);
  int counter() const _BTK_NOEXCEPT;
  void setCounter(int value) _BTK_NOEXCEPT;
  
  virtual btk::Any property(const std::string& key) const _BTK_NOEXCEPT override;
  virtual void setProperty(const std::string& key, const btk::Any& value) override;
  
private:
  std::unique_ptr<MySourcePrivate> mp_Pimpl;
};

class MySourcePrivate
{
  BTK_DECLARE_PINT_ACCESSOR(MySource)
  BTK_DECLARE_STATIC_PROPERTIES_BASE(MySource,
    btk::Property<MySource,int,&MySource::counter,&MySource::setCounter>{"counter"}
  )
  
public:
  MySourcePrivate(MySource* pint, int value) :  Counter(value), mp_Pint(pint) {};
  virtual ~MySourcePrivate() = default;
  
  int Counter;
  
protected:
  MySource* mp_Pint;
};

MySource::MySource(const std::string& name, int value)
  : btk::Node(name), mp_Pimpl(new MySourcePrivate(this, value))
{};

int MySource::counter() const _BTK_NOEXCEPT
{
  auto optr = this->pimpl();
  return optr->Counter;
};

void MySource::setCounter(int value) _BTK_NOEXCEPT
{
  auto optr = this->pimpl();
  if (optr->Counter == value)
    return;
  optr->Counter = value;
  this->modified();
};

btk::Any MySource::property(const std::string& key) const _BTK_NOEXCEPT
{
  auto optr = this->pimpl();
  btk::Any value;
  bool caught = optr->staticProperty(key.c_str(),&value);
  if (!caught)
    value = std::move(this->btk::Node::property(key));
  return value;
};

void MySource::setProperty(const std::string& key, const btk::Any& value)
{
  auto optr = this->pimpl();
  bool caught = optr->setStaticProperty(key.c_str(),&value);
  if (!caught)
    this->btk::Node::setProperty(key,value);
};

// ------------------------------------------------------------------------- //

class MySourceCounterAlgorithm : public btk::Algorithm
{
public:
  MySourceCounterAlgorithm();
  ~MySourceCounterAlgorithm() _BTK_NOEXCEPT = default;
  
  void setIncrement(int value);
  
protected:
  virtual bool fillInputInformation(btk::Port* port) const;
  virtual bool fillOutputInformation(btk::Port* port) const;
  virtual btk::Node* createOutputData(unsigned port) const;
  virtual bool updateOutputData(const std::vector<const btk::Node*>& in, const std::vector<btk::Node*>& out);
  
private:
  int m_Increment;
};

MySourceCounterAlgorithm::MySourceCounterAlgorithm()
: btk::Algorithm(), m_Increment(1)
{
  this->setInputsNumber(1);
  this->setOutputsNumber(1);
};

void MySourceCounterAlgorithm::setIncrement(int value)
{
  if (this->m_Increment == value)
    return;
  this->m_Increment = value;
  this->modified();
};

bool MySourceCounterAlgorithm::fillInputInformation(btk::Port* port) const
{
  port->setInformation<btk::InputDataRequiredTypeid>(btk::static_typeid<MySource>());
  return true;
};

bool MySourceCounterAlgorithm::fillOutputInformation(btk::Port* port) const
{
  port->setInformation<btk::OutputDataTypeid>(btk::static_typeid<MySource>());
  return true;
};

btk::Node* MySourceCounterAlgorithm::createOutputData(unsigned port) const
{
  _BTK_UNUSED(port)
  return new MySource("MyCounter");
};

bool MySourceCounterAlgorithm::updateOutputData(const std::vector<const btk::Node*>& in, const std::vector<btk::Node*>& out)
{
  out[0]->setProperty("counter", in[0]->property("counter").cast<int>() + this->m_Increment);
  return true;
};

// ------------------------------------------------------------------------- //

std::shared_ptr<btk::Node> incrementByTwo(MySource* src)
{
  btk::DataFactory<MySource> dfms("test");
  dfms.output(0)->holdData(src);
  MySourceCounterAlgorithm msca;
  msca.setIncrement(2);
  msca.setInput(dfms.output());
  TS_ASSERT_EQUALS(msca.update(), btk::Result::Success);
  return btk::make_shared_node(msca.output(0)->exportData());
}

#endif
#ifndef NodeTest_classdef_h
#define NodeTest_classdef_h

#include <btkNode.h>
#include <btkNode_p.h>
#include <btkProperty.h>

class TestNodePrivate;

class TestNode : public btk::Node
{
  BTK_DECLARE_PIMPL_ACCESSOR(TestNode)
  
public:
  TestNode(const std::string& name, Node* parent = nullptr);
  
  int version() const;
  void setVersion(int value);
  
  int count() const;
};

class TestNodePrivate : public btk::NodePrivate
{
  BTK_DECLARE_PINT_ACCESSOR(TestNode)
  
  BTK_DECLARE_NODEID(TestNode, btk::Node)
  BTK_DECLARE_STATIC_PROPERTIES(TestNode, btk::Node,
    btk::Property<TestNode,int>("version",&TestNode::version,&TestNode::setVersion)
  )
  
public:
  TestNodePrivate(TestNode* pint, const std::string& name) :  btk::NodePrivate(pint,name), Version(1) {};
  int Version;
};

TestNode::TestNode(const std::string& name, Node* parent)
: btk::Node(*new TestNodePrivate(this,name), parent)
{};

int TestNode::version() const
{
  auto optr = this->pimpl();
  return optr->Version;
};

void TestNode::setVersion(int value)
{
  auto optr = this->pimpl();
  if (value == optr->Version)
    return;
  optr->Version = value;
};

int TestNode::count() const
{
  auto optr = this->pimpl();
  int num = 1;
  for (auto it = optr->Children.cbegin() ; it != optr->Children.cend() ; ++it)
  {
    TestNode* node = node_cast<TestNode*>(*it);
    if (node != nullptr)
      num += node->count();
  }
  return num;
};

#endif // NodeTest_classdef_h
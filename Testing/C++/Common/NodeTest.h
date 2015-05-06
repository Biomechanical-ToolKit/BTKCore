#ifndef NodeTest_h
#define NodeTest_h

#include "NodeTest_classdef.h"

CXXTEST_SUITE(NodeTest)
{
  CXXTEST_TEST(Modified)
  {
    btk::Node root("root");
    root.modified(); // To grab the last modified timestamp;
    unsigned long ts = root.timestamp();
    
    btk::Node branchA("branchA",&root);
    btk::Node leafA1("leafA1",&branchA);
    btk::Node leafA2("leafA2",&branchA);
    btk::Node branchB("branchB",&root);
    btk::Node leafB1("leafB1",&branchB);
    btk::Node leafB2("leafB2",&branchB);
    
    TS_ASSERT_EQUALS(root.timestamp(),ts+10ul);
    TS_ASSERT_EQUALS(branchA.timestamp(),ts+4ul);
    TS_ASSERT_EQUALS(leafA1.timestamp(),0ul);
    TS_ASSERT_EQUALS(leafA2.timestamp(),0ul);
    TS_ASSERT_EQUALS(branchB.timestamp(),ts+9ul);
    TS_ASSERT_EQUALS(leafB1.timestamp(),0ul);
    TS_ASSERT_EQUALS(leafB2.timestamp(),0ul);
  }
  
  CXXTEST_TEST(StaticProperty)
  {
    btk::Node node("foo");
    
    TS_ASSERT_EQUALS(node.name(),"foo");
    TS_ASSERT_EQUALS(node.property("name").cast<std::string>(),"foo");
    unsigned long ts = node.timestamp();
    node.setName("bar");
    TS_ASSERT_DIFFERS(node.timestamp(),ts);
    TS_ASSERT_EQUALS(node.name(),"bar");
    TS_ASSERT_EQUALS(node.property("name").cast<std::string>(),"bar");
    ts = node.timestamp();
    node.setProperty("name",std::string("foo"));
    TS_ASSERT_DIFFERS(node.timestamp(),ts);
    TS_ASSERT_EQUALS(node.name(),"foo");
    TS_ASSERT_EQUALS(node.property("name").cast<std::string>(),"foo");
    
    ts = node.timestamp();
    TS_ASSERT_EQUALS(node.description(),"");
    TS_ASSERT_EQUALS(node.property("description").cast<std::string>(),"");
    node.setDescription("I am an empty description, isn't it?");
    TS_ASSERT_EQUALS(node.timestamp(),ts);
    TS_ASSERT_EQUALS(node.description(),"I am an empty description, isn't it?");
    TS_ASSERT_EQUALS(node.property("description").cast<std::string>(),"I am an empty description, isn't it?");
    ts = node.timestamp();
    node.setProperty("description",std::string("Or was I..."));
    TS_ASSERT_EQUALS(node.timestamp(),ts);
    TS_ASSERT_EQUALS(node.description(),"Or was I...");
    TS_ASSERT_EQUALS(node.property("description").cast<std::string>(),"Or was I...");
  };
  
  CXXTEST_TEST(DynamicProperty)
  {
    btk::Node node("foo");
    unsigned long ts = node.timestamp();
    node.setProperty("bar",45);
    TS_ASSERT_DIFFERS(node.timestamp(),ts);
    ts = node.timestamp();
    TS_ASSERT_EQUALS(node.property("bar"),45);
    node.setProperty("bar",45);
    TS_ASSERT_EQUALS(node.timestamp(),ts);
    ts = node.timestamp();
    node.setProperty("bar",std::string("hello"));
    TS_ASSERT_DIFFERS(node.timestamp(),ts);
    ts = node.timestamp();
    node.setProperty("bar","hello");
    TS_ASSERT_EQUALS(node.timestamp(),ts);
    ts = node.timestamp();
    TS_ASSERT_EQUALS(node.property("bar").cast<std::string>(),"hello");
    node.setProperty("bar",btk::Any());
    TS_ASSERT_DIFFERS(node.timestamp(),ts);
    TS_ASSERT_EQUALS(node.property("bar").isValid(),false);
    TS_ASSERT_EQUALS(node.property("my_unknown_dynamic_property").isValid(),false);
  };
  
  CXXTEST_TEST(InheritingClassWithStaticProperty)
  {
    TestNode node("foo");
    
    TS_ASSERT_EQUALS(node.name(),"foo");
    TS_ASSERT_EQUALS(node.property("name").cast<std::string>(),"foo");
    node.setName("bar");
    TS_ASSERT_EQUALS(node.name(),"bar");
    TS_ASSERT_EQUALS(node.property("name").cast<std::string>(),"bar");
    node.setProperty("name",std::string("foo"));
    TS_ASSERT_EQUALS(node.name(),"foo");
    TS_ASSERT_EQUALS(node.property("name").cast<std::string>(),"foo");
    
    TS_ASSERT_EQUALS(node.version(),1);
    TS_ASSERT_EQUALS(node.property("version").cast<int>(),1);
    node.setVersion(2);
    TS_ASSERT_EQUALS(node.version(),2);
    TS_ASSERT_EQUALS(node.property("version").cast<int>(),2);
    node.setProperty("version",3);
    TS_ASSERT_EQUALS(node.version(),3);
    TS_ASSERT_EQUALS(node.property("version").cast<int>(),3);
    
    btk::Node* bar = new TestNode("bar",&node);
    TS_ASSERT_EQUALS(static_cast<TestNode*>(bar)->version(),1);
    TS_ASSERT_EQUALS(bar->property("version").cast<int>(),1);
  };
  
  CXXTEST_TEST(ChildrenStack)
  {
    TestNode root("root");
    TS_ASSERT_EQUALS(root.count(),1);
    TestNode leafA("leafA",&root);
    TS_ASSERT_EQUALS(root.count(),2);
    TestNode leafB("leafB",&root);
    TS_ASSERT_EQUALS(root.count(),3);
    TestNode leafC("leafC",&root);
    TS_ASSERT_EQUALS(root.count(),4);
    btk::Node leafD("leafD",&root);
    TS_ASSERT_EQUALS(root.count(),4);
    TS_ASSERT_EQUALS(root.hasParents(),false);
    TS_ASSERT_EQUALS(root.hasChildren(),true);
    TS_ASSERT_EQUALS(root.children().front(),&leafA);
    TS_ASSERT_EQUALS(root.children().back(),&leafD);
    TS_ASSERT_EQUALS(leafA.hasChildren(),false);
    TS_ASSERT_EQUALS(leafA.hasParents(),true);
    TS_ASSERT_EQUALS(leafA.parents().front(),&root);
    TS_ASSERT_EQUALS(leafB.hasChildren(),false);
    TS_ASSERT_EQUALS(leafB.hasParents(),true);
    TS_ASSERT_EQUALS(leafB.parents().front(),&root);
    TS_ASSERT_EQUALS(leafC.hasChildren(),false);
    TS_ASSERT_EQUALS(leafC.hasParents(),true);
    TS_ASSERT_EQUALS(leafC.parents().front(),&root);
  };
  
  CXXTEST_TEST(ChildrenHeap)
  {
    TestNode root("root");
    btk::Node* foo = new btk::Node("foo",&root);
    TS_ASSERT_DIFFERS(root.timestamp(),0ul);
    TS_ASSERT_EQUALS(root.hasChildren(),true);
    TS_ASSERT_EQUALS(root.children().front(),foo);
    delete foo;
    TS_ASSERT_EQUALS(root.hasChildren(),false);
    foo = new TestNode("foo2",&root);
    TS_ASSERT_EQUALS(root.hasChildren(),true);
    new TestNode("bar",foo);
    TS_ASSERT_EQUALS(foo->hasChildren(),true);
    TS_ASSERT_EQUALS(foo->children().front()->name(),"bar");
    TestNode* col = new TestNode("col",foo);
    btk::Node* sun = new btk::Node("sun",foo);
    TestNode* moon1 = new TestNode("moon",foo);
    TestNode* moon2 = new TestNode("moon",foo);
    TestNode* moon3 = new TestNode("moon",foo);
    TestNode* planet1 = new TestNode("planet1",sun);
    TestNode* planet2 = new TestNode("planet2",sun);
    TestNode* planet3 = new TestNode("planet3",sun);
    
    TS_ASSERT_EQUALS(root.findChild("foo2"),foo);
    TS_ASSERT_EQUALS(root.findChild(),foo);
    TS_ASSERT_EQUALS(root.findChild("foo"),nullptr);
    TS_ASSERT_EQUALS(root.findChild<TestNode*>("foo2"),foo);
    TS_ASSERT_EQUALS(root.findChild<const TestNode*>("foo2"),foo);
    TS_ASSERT_EQUALS(root.findChild<TestNode*>("foo"),nullptr);

    TS_ASSERT_EQUALS(foo->findChild("col"),col);
    TS_ASSERT_EQUALS(foo->findChild("allo"),nullptr);
    TS_ASSERT_EQUALS(foo->findChild("col"),col);
    TS_ASSERT_EQUALS(foo->findChild("allo"),nullptr);
    TS_ASSERT_EQUALS(foo->findChild<TestNode*>("col"),col);
    TS_ASSERT_DIFFERS(foo->findChild<TestNode*>(),col); // Should be "bar"

    TS_ASSERT_EQUALS(foo->findChild("sun"),sun);
    TS_ASSERT_EQUALS(foo->findChild<btk::Node*>("sun"),sun);
    TS_ASSERT_EQUALS(foo->findChild<const btk::Node*>("sun"),sun);
    TS_ASSERT_EQUALS(foo->findChild<TestNode*>("sun"),nullptr);
    TS_ASSERT_EQUALS(foo->findChild<const TestNode*>("sun"),nullptr);

    TS_ASSERT_EQUALS(root.findChild("sun"),sun);
    TS_ASSERT_EQUALS(root.findChild("sun",{},false),nullptr);
    TS_ASSERT_EQUALS(root.findChild<TestNode*>("sun"),nullptr);
    TS_ASSERT_EQUALS(root.findChild<TestNode*>("col"),col);
    TS_ASSERT_EQUALS(root.findChild("col"),col);
    TS_ASSERT_EQUALS(root.findChild("col",{},false),nullptr);
    
    std::list<btk::Node*> moonsA = root.findChildren("moon");
    TS_ASSERT_EQUALS(moonsA.size(), 3);
    
    std::list<TestNode*> moonsB = root.findChildren<TestNode*>("moon");
    TS_ASSERT_EQUALS(moonsB.size(), 3);
    
    std::list<btk::Node*> planetsA = root.findChildren(std::regex("planet.*"));
    TS_ASSERT_EQUALS(planetsA.size(), 3);
    
    std::list<TestNode*> planetsB = sun->findChildren<TestNode*>(std::regex("planet.*"),{},false);
    TS_ASSERT_EQUALS(planetsB.size(), 3);
    
    std::list<btk::Node*> all = root.findChildren();
    TS_ASSERT_EQUALS(all.size(), 10);
  }
  
  CXXTEST_TEST(NullParentAndChild)
  {
    TestNode root("root");
    TS_ASSERT_EQUALS(root.hasChildren(),false);
    TS_ASSERT_EQUALS(root.hasParents(),false);
    root.appendChild(nullptr);
    TS_ASSERT_EQUALS(root.hasChildren(),false);
    root.appendParent(nullptr);
    TS_ASSERT_EQUALS(root.hasParents(),false);
  }
  CXXTEST_TEST(RetrievePath)
  {
    btk::Node root("root");
    TestNode branch1("branch1",&root);
    btk::Node leaf1("leaf1",&branch1);
    TestNode branch2("branch2",&root);
    btk::Node branch2_1("branch2_1",&branch2);
    TestNode leaf2_1_1("leaf2_1_1",&branch2_1);
    
    std::list<const btk::Node*> ref;
    
    auto path1 = root.retrievePath(&branch1);
    TS_ASSERT_EQUALS(path1.size(), 2);
    ref = {&root,&branch1};
    TS_ASSERT(path1 == ref);
    
    auto path2 = root.retrievePath(&leaf1);
    TS_ASSERT_EQUALS(path2.size(), 3);
    ref = {&root,&branch1,&leaf1};
    TS_ASSERT(path2 == ref);
    
    auto path3 = branch1.retrievePath(&leaf1);
    TS_ASSERT_EQUALS(path3.size(), 2);
    ref = {&branch1,&leaf1};
    TS_ASSERT(path3 == ref);
    
    auto path4 = root.retrievePath(&leaf2_1_1);
    TS_ASSERT_EQUALS(path4.size(), 4);
    ref = {&root,&branch2,&branch2_1,&leaf2_1_1};
    TS_ASSERT(path4 == ref);
    
    auto path5 = branch2.retrievePath(&leaf2_1_1);
    TS_ASSERT_EQUALS(path5.size(), 3);
    ref = {&branch2,&branch2_1,&leaf2_1_1};
    TS_ASSERT(path5 == ref);
    
    auto path6 = leaf1.retrievePath(&leaf2_1_1);
    TS_ASSERT_EQUALS(path6.size(), 0);
  };
};

CXXTEST_SUITE_REGISTRATION(NodeTest)
CXXTEST_TEST_REGISTRATION(NodeTest, Modified)
CXXTEST_TEST_REGISTRATION(NodeTest, StaticProperty)
CXXTEST_TEST_REGISTRATION(NodeTest, DynamicProperty)
CXXTEST_TEST_REGISTRATION(NodeTest, InheritingClassWithStaticProperty)
CXXTEST_TEST_REGISTRATION(NodeTest, ChildrenStack)
CXXTEST_TEST_REGISTRATION(NodeTest, ChildrenHeap)
CXXTEST_TEST_REGISTRATION(NodeTest, NullParentAndChild)
CXXTEST_TEST_REGISTRATION(NodeTest, RetrievePath)
  
#endif // ObjectTest_h
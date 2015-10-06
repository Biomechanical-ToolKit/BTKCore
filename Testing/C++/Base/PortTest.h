#ifndef NodeTest_h
#define NodeTest_h

#include <btkPort.h>

#include "NodeTest_classdef.h"

CXXTEST_SUITE(PortTest)
{
  CXXTEST_TEST(HoldData)
  {
    btk::Port port(0,nullptr);
    TestNode* node = new TestNode("input");
    port.setInformation<btk::InputDataRequiredTypeid>(btk::static_typeid<btk::Node>());
    port.setInformation<btk::OutputDataTypeid>(btk::static_typeid<TestNode>());
    port.holdData(node);
    TS_ASSERT_EQUALS(port.data(0), node);
    TS_ASSERT_EQUALS(node->isCastable(btk::static_typeid<btk::Node>()), true);
    TS_ASSERT_EQUALS(node->isCastable(btk::static_typeid<TestNode>()), true);
    TS_ASSERT_EQUALS(port.information<btk::OutputDataTypeid>(), btk::static_typeid<TestNode>());
    TS_ASSERT_EQUALS(node->isCastable(port.information<btk::InputDataRequiredTypeid>()), true);
    TS_ASSERT_EQUALS(node->isCastable(port.information<btk::OutputDataTypeid>()), true);
  };
};

CXXTEST_SUITE_REGISTRATION(PortTest)
CXXTEST_TEST_REGISTRATION(PortTest, HoldData)

#endif
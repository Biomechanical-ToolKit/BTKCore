#ifndef RigTest_h
#define RigTest_h

#include "RigTest_classdef.h"

#include <btkRig.h>
#include <btkSegment.h>
#include <btkJoint.h>

CXXTEST_SUITE(RigTest)
{
  CXXTEST_TEST(SetupWithoutParenting)
  {
    TS_WARN("TODO");
    // btk::Model model("test");
    // TestRig rig;
    // rig.setup(&model);
    //
    // TS_ASSERT_EQUALS(model.children().size(),2ul);
    // TS_ASSERT_EQUALS(model.rig(), nullptr);
    // TS_ASSERT_EQUALS(model.segments()->children().size(), 2ul);
    // TS_ASSERT_EQUALS(model.joints()->children().size(), 1ul);
  };
  
  CXXTEST_TEST(SetupInsideModel)
  {
    TS_WARN("TODO");
    // TestRig* rig = new TestRig; // Must be an allocated pointer
    // btk::Model model("test",rig);
    //
    // TS_ASSERT_EQUALS(model.children().size(),3ul);
    // TS_ASSERT_EQUALS(model.rig(), rig);
    // TS_ASSERT_EQUALS(model.segments()->children().size(), 2ul);
    // TS_ASSERT_EQUALS(model.joints()->children().size(), 1ul);
  };
  
};

CXXTEST_SUITE_REGISTRATION(RigTest)
CXXTEST_TEST_REGISTRATION(RigTest, SetupWithoutParenting)
CXXTEST_TEST_REGISTRATION(RigTest, SetupInsideModel)

#endif // RigTest_h
#ifndef ModelTest_h
#define ModelTest_h

#include <btkModel.h>
#include <btkUpperLimbModelConfigurator.h>

CXXTEST_SUITE(ModelTest)
{
  CXXTEST_TEST(DefaultUpperLimb)
  {
    btk::Model::Pointer model = btk::Model::New();
    model->Init(btk::UpperLimbModelConfigurator::New());
    TS_ASSERT_EQUALS(model->GetSegmentNumber(), 7);
    TS_ASSERT_EQUALS(model->GetSegment(0)->GetLabel(), "Trunk");
    TS_ASSERT_EQUALS(model->GetSegment(1)->GetLabel(), "LArm");
    TS_ASSERT_EQUALS(model->GetSegment(2)->GetLabel(), "LForearm");
    TS_ASSERT_EQUALS(model->GetSegment(3)->GetLabel(), "LHand");
    TS_ASSERT_EQUALS(model->GetSegment(4)->GetLabel(), "RArm");
    TS_ASSERT_EQUALS(model->GetSegment(5)->GetLabel(), "RForearm");
    TS_ASSERT_EQUALS(model->GetSegment(6)->GetLabel(), "RHand");
    TS_ASSERT_EQUALS(model->GetJointNumber(), 7);
    TS_ASSERT_EQUALS(model->GetJoint(0)->GetLabel(), "LShoulder");
    TS_ASSERT_EQUALS(model->GetJoint(1)->GetLabel(), "LElbow");
    TS_ASSERT_EQUALS(model->GetJoint(2)->GetLabel(), "LWrist");
    TS_ASSERT_EQUALS(model->GetJoint(3)->GetLabel(), "RShoulder");
    TS_ASSERT_EQUALS(model->GetJoint(4)->GetLabel(), "RElbow");
    TS_ASSERT_EQUALS(model->GetJoint(5)->GetLabel(), "RWrist");
    TS_ASSERT_EQUALS(model->GetJoint(6)->GetLabel(), "Thorax");
  };
  
  CXXTEST_TEST(LeftUpperLimb)
  {
    btk::Model::Pointer model = btk::Model::New();
    model->Init(btk::UpperLimbModelConfigurator::New(), btk::Left);
    TS_ASSERT_EQUALS(model->GetSegmentNumber(), 4);
    TS_ASSERT_EQUALS(model->GetSegment(0)->GetLabel(), "Trunk");
    TS_ASSERT_EQUALS(model->GetSegment(1)->GetLabel(), "LArm");
    TS_ASSERT_EQUALS(model->GetSegment(2)->GetLabel(), "LForearm");
    TS_ASSERT_EQUALS(model->GetSegment(3)->GetLabel(), "LHand");
    TS_ASSERT_EQUALS(model->GetJointNumber(), 4);
    TS_ASSERT_EQUALS(model->GetJoint(0)->GetLabel(), "LShoulder");
    TS_ASSERT_EQUALS(model->GetJoint(1)->GetLabel(), "LElbow");
    TS_ASSERT_EQUALS(model->GetJoint(2)->GetLabel(), "LWrist");
    TS_ASSERT_EQUALS(model->GetJoint(3)->GetLabel(), "Thorax");
  };
  
  CXXTEST_TEST(RightUpperLimb)
  {
    btk::Model::Pointer model = btk::Model::New();
    model->Init(btk::UpperLimbModelConfigurator::New(), btk::Right);
    TS_ASSERT_EQUALS(model->GetSegmentNumber(), 4);
    TS_ASSERT_EQUALS(model->GetSegment(0)->GetLabel(), "Trunk");
    TS_ASSERT_EQUALS(model->GetSegment(1)->GetLabel(), "RArm");
    TS_ASSERT_EQUALS(model->GetSegment(2)->GetLabel(), "RForearm");
    TS_ASSERT_EQUALS(model->GetSegment(3)->GetLabel(), "RHand");
    TS_ASSERT_EQUALS(model->GetJointNumber(), 4);
    TS_ASSERT_EQUALS(model->GetJoint(0)->GetLabel(), "RShoulder");
    TS_ASSERT_EQUALS(model->GetJoint(1)->GetLabel(), "RElbow");
    TS_ASSERT_EQUALS(model->GetJoint(2)->GetLabel(), "RWrist");
    TS_ASSERT_EQUALS(model->GetJoint(3)->GetLabel(), "Thorax");
  };
  
  CXXTEST_TEST(BothUpperLimb)
  {
    btk::Model::Pointer model = btk::Model::New();
    model->Init(btk::UpperLimbModelConfigurator::New(), btk::Both);
    TS_ASSERT_EQUALS(model->GetSegmentNumber(), 7);
    TS_ASSERT_EQUALS(model->GetSegment(0)->GetLabel(), "Trunk");
    TS_ASSERT_EQUALS(model->GetSegment(1)->GetLabel(), "LArm");
    TS_ASSERT_EQUALS(model->GetSegment(2)->GetLabel(), "LForearm");
    TS_ASSERT_EQUALS(model->GetSegment(3)->GetLabel(), "LHand");
    TS_ASSERT_EQUALS(model->GetSegment(4)->GetLabel(), "RArm");
    TS_ASSERT_EQUALS(model->GetSegment(5)->GetLabel(), "RForearm");
    TS_ASSERT_EQUALS(model->GetSegment(6)->GetLabel(), "RHand");
    TS_ASSERT_EQUALS(model->GetJointNumber(), 7);
    TS_ASSERT_EQUALS(model->GetJoint(0)->GetLabel(), "LShoulder");
    TS_ASSERT_EQUALS(model->GetJoint(1)->GetLabel(), "LElbow");
    TS_ASSERT_EQUALS(model->GetJoint(2)->GetLabel(), "LWrist");
    TS_ASSERT_EQUALS(model->GetJoint(3)->GetLabel(), "RShoulder");
    TS_ASSERT_EQUALS(model->GetJoint(4)->GetLabel(), "RElbow");
    TS_ASSERT_EQUALS(model->GetJoint(5)->GetLabel(), "RWrist");
    TS_ASSERT_EQUALS(model->GetJoint(6)->GetLabel(), "Thorax");
  };
  
  CXXTEST_TEST(RemoveSegment)
  {
    btk::Model::Pointer model = btk::Model::New();
    model->Init(btk::UpperLimbModelConfigurator::New(), btk::Both);
    model->RemoveSegment(1); // Left arm
    TS_ASSERT_EQUALS(model->GetJointNumber(), 5);
    TS_ASSERT_EQUALS(model->GetJoint(0)->GetLabel(), "LWrist");
    TS_ASSERT_EQUALS(model->GetJoint(1)->GetLabel(), "RShoulder");
    TS_ASSERT_EQUALS(model->GetJoint(2)->GetLabel(), "RElbow");
    TS_ASSERT_EQUALS(model->GetJoint(3)->GetLabel(), "RWrist");
    TS_ASSERT_EQUALS(model->GetJoint(4)->GetLabel(), "Thorax");
  };
  
  CXXTEST_TEST(Clone)
  {
    btk::Model::Pointer model = btk::Model::New();
    model->Init(btk::UpperLimbModelConfigurator::New(), btk::Left);
    btk::Model::Pointer cloned = model->Clone();
    TS_ASSERT_EQUALS(cloned->GetSegmentNumber(), 4);
    TS_ASSERT_EQUALS(cloned->GetSegment(0)->GetLabel(), "Trunk");
    TS_ASSERT_EQUALS(cloned->GetSegment(1)->GetLabel(), "LArm");
    TS_ASSERT_EQUALS(cloned->GetSegment(2)->GetLabel(), "LForearm");
    TS_ASSERT_EQUALS(cloned->GetSegment(3)->GetLabel(), "LHand");
    TS_ASSERT_EQUALS(cloned->GetJointNumber(), 4);
    TS_ASSERT_EQUALS(cloned->GetJoint(0)->GetLabel(), "LShoulder");
    TS_ASSERT_EQUALS(cloned->GetJoint(1)->GetLabel(), "LElbow");
    TS_ASSERT_EQUALS(cloned->GetJoint(2)->GetLabel(), "LWrist");
    TS_ASSERT_EQUALS(cloned->GetJoint(3)->GetLabel(), "Thorax");
    TS_ASSERT_DIFFERS(cloned->GetJoint(0)->GetProximalSegment(), model->GetSegment(0));
    TS_ASSERT_DIFFERS(cloned->GetJoint(0)->GetDistalSegment(), model->GetSegment(1));
    TS_ASSERT_DIFFERS(cloned->GetJoint(1)->GetProximalSegment(), model->GetSegment(1));
    TS_ASSERT_DIFFERS(cloned->GetJoint(1)->GetDistalSegment(), model->GetSegment(2));
    TS_ASSERT_DIFFERS(cloned->GetJoint(2)->GetProximalSegment(), model->GetSegment(2));
    TS_ASSERT_DIFFERS(cloned->GetJoint(2)->GetDistalSegment(), model->GetSegment(3));
    TS_ASSERT_EQUALS(cloned->GetJoint(0)->GetProximalSegment(), cloned->GetSegment(0));
    TS_ASSERT_EQUALS(cloned->GetJoint(0)->GetDistalSegment(), cloned->GetSegment(1));
    TS_ASSERT_EQUALS(cloned->GetJoint(1)->GetProximalSegment(), cloned->GetSegment(1));
    TS_ASSERT_EQUALS(cloned->GetJoint(1)->GetDistalSegment(), cloned->GetSegment(2));
    TS_ASSERT_EQUALS(cloned->GetJoint(2)->GetProximalSegment(), cloned->GetSegment(2));
    TS_ASSERT_EQUALS(cloned->GetJoint(2)->GetDistalSegment(), cloned->GetSegment(3));
  };
};

CXXTEST_SUITE_REGISTRATION(ModelTest)
CXXTEST_TEST_REGISTRATION(ModelTest, DefaultUpperLimb)
CXXTEST_TEST_REGISTRATION(ModelTest, LeftUpperLimb)
CXXTEST_TEST_REGISTRATION(ModelTest, RightUpperLimb)
CXXTEST_TEST_REGISTRATION(ModelTest, BothUpperLimb)
CXXTEST_TEST_REGISTRATION(ModelTest, RemoveSegment)
CXXTEST_TEST_REGISTRATION(ModelTest, Clone)

#endif // ModelTest_h
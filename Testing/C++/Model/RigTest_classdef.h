#ifndef RigTest_classdef_h
#define RigTest_classdef_h

#include <btkRig.h>
#include <btkModel.h>
#include <btkSegment.h>
#include <btkJoint.h>

#include <btkMacros.h> // BTK_UNUSED

class TestRig : public btk::Rig
{
public:
  TestRig(btk::Node* parent = nullptr);
  
  virtual bool setup(btk::Model* model) const noexcept override;
  virtual bool calibrate(btk::Model* model, btk::Node* trials) noexcept override;
  virtual bool generate(btk::Model* model, btk::Node* trials) const noexcept override;
  virtual void finalize(btk::Node* trials) const noexcept override;
};

TestRig::TestRig(btk::Node* parent)
: btk::Rig("TestRig",0,0, parent)
{};

bool TestRig::setup(btk::Model* model) const noexcept
{
  btk::Segment* segA = new btk::Segment("SegA",0,0,model->segments());
  btk::Segment* segB = new btk::Segment("SegB",0,0,model->segments());
  new btk::Joint("B/A",segA,segB,model->joints());
  return true;
};

bool TestRig::calibrate(btk::Model* model, Node* trials) noexcept
{
  _BTK_UNUSED(model)
  _BTK_UNUSED(trials)
  // auto segA = model->segments()->findChild<btk::Segment*>("SegA");
  // auto segB = model->segments()->findChild<btk::Segment*>("SegB");
  // btk::Trial* trial = trials->findChild<btk::Trial*>();
  // // auto markers = trial->averageMarkers({"TT","LM","LE","HEEL","MET2","MET5"});
  // // if (markers["HEEL"])
  // // auto heel = markers["HEEL"];
  //
  // btk::map::Marker TT(trial,"TT"), LM(trial,"LM"), LE(trial,"LE"),
  //                  HEEL(trial,"HEEL"), MET2(trial,"MET2"), MET5(trial,"MET5");
  // if (TT.averageInPlace() && LM.averageInPlace() && LE.averageInPlace())
  // {
  //   // btk::Geometry::Vector u = (TT-LM).normalize(),
  //   //                       v = (TT-LE).normalize(),
  //   //                       w = u.cross(v);
  // }
  return true;
};

bool TestRig::generate(btk::Model* model, Node* trials) const noexcept
{
  _BTK_UNUSED(model)
  _BTK_UNUSED(trials)
  // auto trial = trials->findChild<btk::Trial*>();
  // auto TT = btk::lard::to_mapped_trajectory(trial,"TT");
  // auto LM = btk::lard::to_mapped_trajectory(trial,"LM");
  // auto LE = btk::lard::to_mapped_trajectory(trial,"LE");
  // auto HEEL = btk::lard::to_mapped_trajectory(trial,"HEEL");
  // auto MET2 = btk::lard::to_mapped_trajectory(trial,"MET2");
  // auto MET5 = btk::lard::to_mapped_trajectory(trial,"MET5");
  //
  // if (TT.isValid() && LM.isValid() && LE.isValid())
  // {
  //   btk::lard::Vector u = (TT-LM).normalized(),
  //                      v = (TT-LE).normalized();
  //   // btk::lard::to_pose_timesequence("SegA", u, v, u.cross(v), TT, trial);
  // }
  return true;
};

void TestRig::finalize(btk::Node* trials) const noexcept
{
  _BTK_UNUSED(trials)
};

#endif // RigTest_classdef_h
#ifndef PluginGaitTest_h
#define PluginGaitTest_h

#include <btkPluginGait.h>
#include <btkBodyEnums.h>
#include <btkModel.h>
#include <btkSegment.h>
#include <btkJoint.h>
#include <btkTrial.h>

#include "PluginGaitTest_utils.h"

CXXTEST_SUITE(PluginGaitTest)
{
  CXXTEST_TEST(SetupBothLowerBody)
  {
    btk::Model model("test", new btk::PluginGait(btk::BodyRegion::Lower, btk::BodySide::Both));
    auto segments = model.segments()->children();
    auto itS = segments.cbegin();
    TS_ASSERT_EQUALS(segments.size(), 7ul);
    TS_ASSERT_EQUALS((*itS)->name(), "Pelvis"); ++itS;
    TS_ASSERT_EQUALS((*itS)->name(), "L.Thigh"); ++itS;
    TS_ASSERT_EQUALS((*itS)->name(), "L.Shank"); ++itS;
    TS_ASSERT_EQUALS((*itS)->name(), "L.Foot"); ++itS;
    TS_ASSERT_EQUALS((*itS)->name(), "R.Thigh"); ++itS;
    TS_ASSERT_EQUALS((*itS)->name(), "R.Shank"); ++itS;
    TS_ASSERT_EQUALS((*itS)->name(), "R.Foot");
    auto joints = model.joints()->children();
    auto itJ = joints.cbegin();
    TS_ASSERT_EQUALS(joints.size(), 9ul);
    TS_ASSERT_EQUALS((*itJ)->name(), "L.Hip"); ++itJ;
    TS_ASSERT_EQUALS((*itJ)->name(), "L.Knee"); ++itJ;
    TS_ASSERT_EQUALS((*itJ)->name(), "L.Ankle"); ++itJ;
    TS_ASSERT_EQUALS((*itJ)->name(), "L.FootProgress"); ++itJ;
    TS_ASSERT_EQUALS((*itJ)->name(), "R.Hip"); ++itJ;
    TS_ASSERT_EQUALS((*itJ)->name(), "R.Knee"); ++itJ;
    TS_ASSERT_EQUALS((*itJ)->name(), "R.Ankle"); ++itJ;
    TS_ASSERT_EQUALS((*itJ)->name(), "R.FootProgress"); ++itJ;
    TS_ASSERT_EQUALS((*itJ)->name(), "Pelvis");
  };
  
  CXXTEST_TEST(SetupLeftLowerBody)
  {
    btk::Model model("test", new btk::PluginGait(btk::BodyRegion::Lower, btk::BodySide::Left));
    auto segments = model.segments()->children();
    auto itS = segments.cbegin();
    TS_ASSERT_EQUALS(segments.size(), 4ul);
    TS_ASSERT_EQUALS((*itS)->name(), "Pelvis"); ++itS;
    TS_ASSERT_EQUALS((*itS)->name(), "L.Thigh"); ++itS;
    TS_ASSERT_EQUALS((*itS)->name(), "L.Shank"); ++itS;
    TS_ASSERT_EQUALS((*itS)->name(), "L.Foot");
    auto joints = model.joints()->children();
    auto itJ = joints.cbegin();
    TS_ASSERT_EQUALS(joints.size(), 5ul);
    TS_ASSERT_EQUALS((*itJ)->name(), "L.Hip"); ++itJ;
    TS_ASSERT_EQUALS((*itJ)->name(), "L.Knee"); ++itJ;
    TS_ASSERT_EQUALS((*itJ)->name(), "L.Ankle"); ++itJ;
    TS_ASSERT_EQUALS((*itJ)->name(), "L.FootProgress"); ++itJ;
    TS_ASSERT_EQUALS((*itJ)->name(), "Pelvis");
  };
  
  CXXTEST_TEST(SetupRightLowerBody)
  {
    btk::Model model("test", new btk::PluginGait(btk::BodyRegion::Lower, btk::BodySide::Right));
    auto segments = model.segments()->children();
    auto itS = segments.cbegin();
    TS_ASSERT_EQUALS(segments.size(), 4ul);
    TS_ASSERT_EQUALS((*itS)->name(), "Pelvis"); ++itS;
    TS_ASSERT_EQUALS((*itS)->name(), "R.Thigh"); ++itS;
    TS_ASSERT_EQUALS((*itS)->name(), "R.Shank"); ++itS;
    TS_ASSERT_EQUALS((*itS)->name(), "R.Foot");
    auto joints = model.joints()->children();
    auto itJ = joints.cbegin();
    TS_ASSERT_EQUALS(joints.size(), 5ul);
    TS_ASSERT_EQUALS((*itJ)->name(), "R.Hip"); ++itJ;
    TS_ASSERT_EQUALS((*itJ)->name(), "R.Knee"); ++itJ;
    TS_ASSERT_EQUALS((*itJ)->name(), "R.Ankle"); ++itJ;
    TS_ASSERT_EQUALS((*itJ)->name(), "R.FootProgress"); ++itJ;
    TS_ASSERT_EQUALS((*itJ)->name(), "Pelvis");
  };
  
  CXXTEST_TEST(SetupLeftFullBody)
  {
    btk::Model model("test", new btk::PluginGait(btk::BodyRegion::Full, btk::BodySide::Left));
    auto segments = model.segments()->children();
    auto itS = segments.cbegin();
    auto joints = model.joints()->children();
    auto itJ = joints.cbegin();
    
    TS_ASSERT_EQUALS(segments.size(), 10ul);
    TS_ASSERT_EQUALS(joints.size(), 12ul);
    
    // Upper
    TS_ASSERT_EQUALS((*itS)->name(), "Head"); ++itS;
    TS_ASSERT_EQUALS((*itS)->name(), "Torso"); ++itS;
    TS_ASSERT_EQUALS((*itS)->name(), "L.Clavicle"); ++itS;
    TS_ASSERT_EQUALS((*itS)->name(), "L.Arm"); ++itS;
    TS_ASSERT_EQUALS((*itS)->name(), "L.Forearm"); ++itS;
    TS_ASSERT_EQUALS((*itS)->name(), "L.Hand"); ++itS;
    TS_ASSERT_EQUALS((*itJ)->name(), "L.Shoulder"); ++itJ;
    TS_ASSERT_EQUALS((*itJ)->name(), "L.Elbow"); ++itJ;
    TS_ASSERT_EQUALS((*itJ)->name(), "L.Wrist"); ++itJ;
    TS_ASSERT_EQUALS((*itJ)->name(), "Neck"); ++itJ;
    TS_ASSERT_EQUALS((*itJ)->name(), "Thorax"); ++itJ;
    TS_ASSERT_EQUALS((*itJ)->name(), "Head"); ++itJ;
    // Lower
    TS_ASSERT_EQUALS((*itS)->name(), "Pelvis"); ++itS;
    TS_ASSERT_EQUALS((*itS)->name(), "L.Thigh"); ++itS;
    TS_ASSERT_EQUALS((*itS)->name(), "L.Shank"); ++itS;
    TS_ASSERT_EQUALS((*itS)->name(), "L.Foot");
    TS_ASSERT_EQUALS((*itJ)->name(), "L.Hip"); ++itJ;
    TS_ASSERT_EQUALS((*itJ)->name(), "L.Knee"); ++itJ;
    TS_ASSERT_EQUALS((*itJ)->name(), "L.Ankle"); ++itJ;
    TS_ASSERT_EQUALS((*itJ)->name(), "L.FootProgress"); ++itJ;
    TS_ASSERT_EQUALS((*itJ)->name(), "Pelvis"); ++itJ;
    // Full
    TS_ASSERT_EQUALS((*itJ)->name(), "Spine");
  };
  
  CXXTEST_TEST(SetupBothFullBody)
  {
    btk::Model model("test", new btk::PluginGait(btk::BodyRegion::Full, btk::BodySide::Both));
    auto segments = model.segments()->children();
    auto itS = segments.cbegin();
    auto joints = model.joints()->children();
    auto itJ = joints.cbegin();
    
    TS_ASSERT_EQUALS(segments.size(), 17ul);
    TS_ASSERT_EQUALS(joints.size(), 19ul);
    
    // Upper
    TS_ASSERT_EQUALS((*itS)->name(), "Head"); ++itS;
    TS_ASSERT_EQUALS((*itS)->name(), "Torso"); ++itS;
    // Left Upper
    TS_ASSERT_EQUALS((*itS)->name(), "L.Clavicle"); ++itS;
    TS_ASSERT_EQUALS((*itS)->name(), "L.Arm"); ++itS;
    TS_ASSERT_EQUALS((*itS)->name(), "L.Forearm"); ++itS;
    TS_ASSERT_EQUALS((*itS)->name(), "L.Hand"); ++itS;
    TS_ASSERT_EQUALS((*itJ)->name(), "L.Shoulder"); ++itJ;
    TS_ASSERT_EQUALS((*itJ)->name(), "L.Elbow"); ++itJ;
    TS_ASSERT_EQUALS((*itJ)->name(), "L.Wrist"); ++itJ;
    // Right Upper
    TS_ASSERT_EQUALS((*itS)->name(), "R.Clavicle"); ++itS;
    TS_ASSERT_EQUALS((*itS)->name(), "R.Arm"); ++itS;
    TS_ASSERT_EQUALS((*itS)->name(), "R.Forearm"); ++itS;
    TS_ASSERT_EQUALS((*itS)->name(), "R.Hand"); ++itS;
    TS_ASSERT_EQUALS((*itJ)->name(), "R.Shoulder"); ++itJ;
    TS_ASSERT_EQUALS((*itJ)->name(), "R.Elbow"); ++itJ;
    TS_ASSERT_EQUALS((*itJ)->name(), "R.Wrist"); ++itJ;
    // Upper
    TS_ASSERT_EQUALS((*itJ)->name(), "Neck"); ++itJ;
    TS_ASSERT_EQUALS((*itJ)->name(), "Thorax"); ++itJ;
    TS_ASSERT_EQUALS((*itJ)->name(), "Head"); ++itJ;
    
    // Lower
    TS_ASSERT_EQUALS((*itS)->name(), "Pelvis"); ++itS;
    // Left lower
    TS_ASSERT_EQUALS((*itS)->name(), "L.Thigh"); ++itS;
    TS_ASSERT_EQUALS((*itS)->name(), "L.Shank"); ++itS;
    TS_ASSERT_EQUALS((*itS)->name(), "L.Foot"); ++itS;
    TS_ASSERT_EQUALS((*itJ)->name(), "L.Hip"); ++itJ;
    TS_ASSERT_EQUALS((*itJ)->name(), "L.Knee"); ++itJ;
    TS_ASSERT_EQUALS((*itJ)->name(), "L.Ankle"); ++itJ;
    TS_ASSERT_EQUALS((*itJ)->name(), "L.FootProgress"); ++itJ;
    // Right lower
    TS_ASSERT_EQUALS((*itS)->name(), "R.Thigh"); ++itS;
    TS_ASSERT_EQUALS((*itS)->name(), "R.Shank"); ++itS;
    TS_ASSERT_EQUALS((*itS)->name(), "R.Foot");
    TS_ASSERT_EQUALS((*itJ)->name(), "R.Hip"); ++itJ;
    TS_ASSERT_EQUALS((*itJ)->name(), "R.Knee"); ++itJ;
    TS_ASSERT_EQUALS((*itJ)->name(), "R.Ankle"); ++itJ;
    TS_ASSERT_EQUALS((*itJ)->name(), "R.FootProgress"); ++itJ;
    // Lower
    TS_ASSERT_EQUALS((*itJ)->name(), "Pelvis"); ++itJ;
    // Full
    TS_ASSERT_EQUALS((*itJ)->name(), "Spine");
  };

  // ----------------------------------------------------------------------- //
  
  CXXTEST_TEST(CalibrateLeftFullBody_OneFrame)
  {
    auto rig = new btk::PluginGait(btk::BodyRegion::Full, btk::BodySide::Left);
    rig->setMarkerDiameter(16.0); // mm
    rig->setLeftFootFlat(true);
    rig->setLeftLegLength(940.0); // mm
    rig->setLeftKneeWidth(110.0); // mm
    rig->setLeftAnkleWidth(70.0); // mm
    btk::Model model("test", rig);
    
    btk::Node root("root");
    pig_generate_static_trial_oneframe(&root);
    rig->calibrate(&model, &root);
    
    TS_ASSERT_DELTA(rig->interAsisDistance(), 228.693, 5e-4);
    TS_ASSERT_DELTA(rig->property("interAsisDistance").cast<double>(), 228.693, 5e-4);
    TS_ASSERT_DELTA(rig->leftAsisTrochanterAPDistance(), 72.512, 5e-4);
    TS_ASSERT_DELTA(rig->property("leftAsisTrochanterAPDistance").cast<double>(), 72.512, 5e-4);
    TS_ASSERT_DELTA(rig->headOffset(), 0.0945073, 1e-4);
    TS_ASSERT_DELTA(rig->property("headOffset").cast<double>(), 0.0945073, 1e-4);
    TS_ASSERT_DELTA(rig->leftStaticPlantarFlexionOffset(), 0.197172, 1e-4);
    TS_ASSERT_DELTA(rig->property("leftStaticPlantarFlexionOffset").cast<double>(), 0.197172, 1e-4);
    TS_ASSERT_DELTA(rig->leftStaticRotationOffset(), 0.0164273, 1e-4);
    TS_ASSERT_DELTA(rig->property("leftStaticRotationOffset").cast<double>(), 0.0164273, 1e-4);
    
  }
  
  CXXTEST_TEST(CalibrateRightFullBody_OneFrame)
  {
    auto rig = new btk::PluginGait(btk::BodyRegion::Full, btk::BodySide::Right);
    rig->setMarkerDiameter(16.0); // mm
    rig->setRightFootFlat(true);
    rig->setRightLegLength(940.0); // mm
    rig->setRightKneeWidth(120.0); // mm
    rig->setRightAnkleWidth(70.0); // mm
    btk::Model model("test", rig);
    
    btk::Node root("root");
    pig_generate_static_trial_oneframe(&root);
    rig->calibrate(&model, &root);
    
    TS_ASSERT_DELTA(rig->interAsisDistance(), 228.693, 5e-4);
    TS_ASSERT_DELTA(rig->property("interAsisDistance").cast<double>(), 228.693, 5e-4);
    TS_ASSERT_DELTA(rig->rightAsisTrochanterAPDistance(), 72.512, 5e-4);
    TS_ASSERT_DELTA(rig->property("rightAsisTrochanterAPDistance").cast<double>(), 72.512, 5e-4);
    TS_ASSERT_DELTA(rig->headOffset(), 0.0945073, 1e-4);
    TS_ASSERT_DELTA(rig->property("headOffset").cast<double>(), 0.0945073, 1e-4);
    TS_ASSERT_DELTA(rig->rightStaticPlantarFlexionOffset(), 0.208874, 1e-4);
    TS_ASSERT_DELTA(rig->property("rightStaticPlantarFlexionOffset").cast<double>(), 0.208874, 1e-4);
    TS_ASSERT_DELTA(rig->rightStaticRotationOffset(), -0.0179406, 1e-4);
    TS_ASSERT_DELTA(rig->property("rightStaticRotationOffset").cast<double>(), -0.0179406, 1e-4);
    
  }
  
  CXXTEST_TEST(CalibrateBothFullBody_OneFrame)
  {
    auto rig = new btk::PluginGait(btk::BodyRegion::Full, btk::BodySide::Both);
    rig->setMarkerDiameter(16.0); // mm
    rig->setLeftFootFlat(true);
    rig->setLeftLegLength(940.0); // mm
    rig->setLeftKneeWidth(110.0); // mm
    rig->setLeftAnkleWidth(70.0); // mm
    rig->setRightFootFlat(true);
    rig->setRightLegLength(940.0); // mm
    rig->setRightKneeWidth(120.0); // mm
    rig->setRightAnkleWidth(70.0); // mm
    btk::Model model("test", rig);
    
    btk::Node root("root");
    pig_generate_static_trial_oneframe(&root);
    rig->calibrate(&model, &root);
    
    TS_ASSERT_DELTA(rig->interAsisDistance(), 228.693, 5e-4);
    TS_ASSERT_DELTA(rig->property("interAsisDistance").cast<double>(), 228.693, 5e-4);
    TS_ASSERT_DELTA(rig->headOffset(), 0.0945073, 1e-4);
    TS_ASSERT_DELTA(rig->property("headOffset").cast<double>(), 0.0945073, 1e-4);
    
    TS_ASSERT_DELTA(rig->leftAsisTrochanterAPDistance(), 72.512, 5e-4);
    TS_ASSERT_DELTA(rig->property("leftAsisTrochanterAPDistance").cast<double>(), 72.512, 5e-4);
    TS_ASSERT_DELTA(rig->leftStaticPlantarFlexionOffset(), 0.197172, 1e-4);
    TS_ASSERT_DELTA(rig->property("leftStaticPlantarFlexionOffset").cast<double>(), 0.197172, 1e-4);
    TS_ASSERT_DELTA(rig->leftStaticRotationOffset(), 0.0164273, 1e-4);
    TS_ASSERT_DELTA(rig->property("leftStaticRotationOffset").cast<double>(), 0.0164273, 1e-4);
    
    TS_ASSERT_DELTA(rig->rightAsisTrochanterAPDistance(), 72.512, 5e-4);
    TS_ASSERT_DELTA(rig->property("rightAsisTrochanterAPDistance").cast<double>(), 72.512, 5e-4);
    TS_ASSERT_DELTA(rig->rightStaticPlantarFlexionOffset(), 0.208874, 1e-4);
    TS_ASSERT_DELTA(rig->property("rightStaticPlantarFlexionOffset").cast<double>(), 0.208874, 1e-4);
    TS_ASSERT_DELTA(rig->rightStaticRotationOffset(), -0.0179406, 1e-4);
    TS_ASSERT_DELTA(rig->property("rightStaticRotationOffset").cast<double>(), -0.0179406, 1e-4);
  };
  
  CXXTEST_TEST(CalibrateBothUpperBody_FullFrames)
  {
    auto rig = new btk::PluginGait(btk::BodyRegion::Upper, btk::BodySide::Both);
    btk::Model model("test", rig);
    
    btk::Node root("root");
    pig_generate_trial_from_file(&root, _BTK_TDD_C3D_IN_"others/PiGCalibration-FlatFoot-Full.c3d");
    TS_ASSERT_EQUALS(root.children().size(),1u);
    rig->calibrate(&model, &root);
    
    TS_ASSERT_EQUALS(rig->interAsisDistance(), 0.0);
    TS_ASSERT_DELTA(rig->headOffset(), 0.353677, 1e-4);
    TS_ASSERT_EQUALS(rig->leftAsisTrochanterAPDistance(), 0.0);
    TS_ASSERT_EQUALS(rig->leftStaticPlantarFlexionOffset(), 0.0);
    TS_ASSERT_EQUALS(rig->leftStaticRotationOffset(), 0.0);
    TS_ASSERT_EQUALS(rig->rightAsisTrochanterAPDistance(), 0.0);
    TS_ASSERT_EQUALS(rig->rightStaticPlantarFlexionOffset(), 0.0);
    TS_ASSERT_EQUALS(rig->rightStaticRotationOffset(), 0.0);
  };

  CXXTEST_TEST(CalibrateBothLowerBody_FullFrames)
  {
    auto rig = new btk::PluginGait(btk::BodyRegion::Lower, btk::BodySide::Both);
    rig->setMarkerDiameter(16.0); // mm
    rig->setLeftFootFlat(true);
    rig->setLeftLegLength(800.0); // mm
    rig->setLeftKneeWidth(105.0); // mm
    rig->setLeftAnkleWidth(55.0); // mm
    rig->setRightFootFlat(true);
    rig->setRightLegLength(800.0); // mm
    rig->setRightKneeWidth(105.0); // mm
    rig->setRightAnkleWidth(55.0); // mm
    btk::Model model("test", rig);
    
    btk::Node root("root");
    pig_generate_trial_from_file(&root, _BTK_TDD_C3D_IN_"others/PiGCalibration-FlatFoot-Full.c3d");
    TS_ASSERT_EQUALS(root.children().size(),1u);
    rig->calibrate(&model, &root);
    
    TS_ASSERT_DELTA(rig->interAsisDistance(), 199.902, 5e-4);
    TS_ASSERT_EQUALS(rig->headOffset(), 0.0);
    TS_ASSERT_DELTA(rig->leftAsisTrochanterAPDistance(), 54.48, 5e-4);
    TS_ASSERT_DELTA(rig->leftStaticPlantarFlexionOffset(), 0.115969, 1e-4);
    TS_ASSERT_DELTA(rig->leftStaticRotationOffset(), 0.00446829, 1e-4);
    TS_ASSERT_DELTA(rig->rightAsisTrochanterAPDistance(), 54.48, 5e-4);
    TS_ASSERT_DELTA(rig->rightStaticPlantarFlexionOffset(), 0.162755, 1e-4);
    TS_ASSERT_DELTA(rig->rightStaticRotationOffset(), -0.0163829, 1e-4);
  };

  CXXTEST_TEST(CalibrateBothFullBody_FullFrames)
  {
    auto rig = new btk::PluginGait(btk::BodyRegion::Full, btk::BodySide::Both);
    rig->setMarkerDiameter(16.0); // mm
    rig->setLeftFootFlat(true);
    rig->setLeftLegLength(800.0); // mm
    rig->setLeftKneeWidth(105.0); // mm
    rig->setLeftAnkleWidth(55.0); // mm
    rig->setRightFootFlat(true);
    rig->setRightLegLength(800.0); // mm
    rig->setRightKneeWidth(105.0); // mm
    rig->setRightAnkleWidth(55.0); // mm
    btk::Model model("test", rig);
    
    btk::Node root("root");
    pig_generate_trial_from_file(&root, _BTK_TDD_C3D_IN_"others/PiGCalibration-FlatFoot-Full.c3d");
    TS_ASSERT_EQUALS(root.children().size(),1u);
    rig->calibrate(&model, &root);
    
    TS_ASSERT_DELTA(rig->interAsisDistance(), 199.902, 5e-4);
    TS_ASSERT_DELTA(rig->headOffset(), 0.353677, 1e-4);
    TS_ASSERT_DELTA(rig->leftAsisTrochanterAPDistance(), 54.48, 5e-4);
    TS_ASSERT_DELTA(rig->leftStaticPlantarFlexionOffset(), 0.115969, 1e-4);
    TS_ASSERT_DELTA(rig->leftStaticRotationOffset(), 0.00446829, 1e-4);
    TS_ASSERT_DELTA(rig->rightAsisTrochanterAPDistance(), 54.48, 5e-4);
    TS_ASSERT_DELTA(rig->rightStaticPlantarFlexionOffset(), 0.162755, 1e-4);
    TS_ASSERT_DELTA(rig->rightStaticRotationOffset(), -0.0163829, 1e-4);
  };
  
  CXXTEST_TEST(CalibrateBothFullBody_HoleFrames)
  {
    auto rig = new btk::PluginGait(btk::BodyRegion::Full, btk::BodySide::Both);
    rig->setMarkerDiameter(16.0); // mm
    rig->setLeftFootFlat(true);
    rig->setLeftLegLength(940.0); // mm
    rig->setLeftKneeWidth(110.0); // mm
    rig->setLeftAnkleWidth(70.0); // mm
    rig->setRightFootFlat(true);
    rig->setRightLegLength(940.0); // mm
    rig->setRightKneeWidth(120.0); // mm
    rig->setRightAnkleWidth(70.0); // mm
    btk::Model model("test", rig);
    
    btk::Node root("root");
    pig_generate_trial_from_file(&root, _BTK_TDD_C3D_IN_"others/PiGCalibration-FlatFoot-Hole.c3d");
    TS_ASSERT_EQUALS(root.children().size(),1u);
    rig->calibrate(&model, &root);
    
    TS_ASSERT_DELTA(rig->interAsisDistance(), 227.043, 5e-4);
    TS_ASSERT_DELTA(rig->headOffset(), 0.123762, 1e-4);
    TS_ASSERT_DELTA(rig->leftAsisTrochanterAPDistance(), 72.512, 5e-4);
    TS_ASSERT_DELTA(rig->leftStaticPlantarFlexionOffset(), 0.197419, 1e-4);
    TS_ASSERT_DELTA(rig->leftStaticRotationOffset(), 0.0178294, 1e-4);
    TS_ASSERT_DELTA(rig->rightAsisTrochanterAPDistance(), 72.512, 5e-4);
    TS_ASSERT_DELTA(rig->rightStaticPlantarFlexionOffset(), 0.209483, 1e-4);
    TS_ASSERT_DELTA(rig->rightStaticRotationOffset(), -0.0183813, 1e-4);
  };
  
  CXXTEST_TEST(CalibrateBothFullBody_NoOption)
  {
    auto rig = new btk::PluginGait(btk::BodyRegion::Lower, btk::BodySide::Both);
    rig->setMarkerDiameter(14.0); // mm
    rig->setInterAsisDistance(255.0); // mm
    rig->setLeftLegLength(905.0); // mm
    rig->setLeftKneeWidth(105.0); // mm
    rig->setLeftAnkleWidth(75.0); // mm
    rig->setRightLegLength(905.0); // mm
    rig->setRightKneeWidth(105.5); // mm
    rig->setRightAnkleWidth(75.0); // mm
    btk::Model model("test", rig);
    
    btk::Node root("root");
    pig_generate_trial_from_file(&root, _BTK_TDD_C3D_IN_"others/PiGCalibration2-NoOption.c3d");
    TS_ASSERT_EQUALS(root.children().size(),1u);
    rig->calibrate(&model, &root);

    TS_ASSERT_DELTA(rig->interAsisDistance(), 255.0, 1e-15);
    TS_ASSERT_DELTA(rig->leftAsisTrochanterAPDistance(), 68.004, 5e-3);
    TS_ASSERT_DELTA(rig->leftStaticPlantarFlexionOffset(), 0.202148, 1e-4);
    TS_ASSERT_DELTA(rig->leftStaticRotationOffset(), 0.0270116, 1e-4);
    TS_ASSERT_DELTA(rig->rightAsisTrochanterAPDistance(), 68.004, 5e-3);
    TS_ASSERT_DELTA(rig->rightStaticPlantarFlexionOffset(), 0.233063, 2e-4);
    TS_ASSERT_DELTA(rig->rightStaticRotationOffset(), 0.00807751, 2e-4);
  };

  // ----------------------------------------------------------------------- //
  
  CXXTEST_TEST(GenerateBothLowerBody_OneFrame)
  {
    auto rig = new btk::PluginGait(btk::BodyRegion::Lower, btk::BodySide::Both);
    rig->setMarkerDiameter(16.0); // mm
    rig->setLeftFootFlat(true);
    rig->setLeftLegLength(940.0); // mm
    rig->setLeftKneeWidth(110.0); // mm
    rig->setLeftAnkleWidth(70.0); // mm
    rig->setRightFootFlat(true);
    rig->setRightLegLength(940.0); // mm
    rig->setRightKneeWidth(120.0); // mm
    rig->setRightAnkleWidth(70.0); // mm
    btk::Model model("test", rig);
    
    btk::Node rootCalibration("rootCalibration");
    pig_generate_trial_from_file(&rootCalibration, _BTK_TDD_C3D_IN_"others/PiGCalibration-FlatFoot-One.c3d");
    TS_ASSERT(rig->calibrate(&model, &rootCalibration));
    btk::Node rootDynamic("rootDynamic");
    pig_generate_trial_from_file(&rootDynamic, _BTK_TDD_C3D_IN_"others/PiGMotion-FlatFoot-One.c3d");
    TS_ASSERT(rig->generate(&model, &rootDynamic));
    
    auto trial = rootDynamic.findChild<btk::Trial*>();
    pig_compare_segment_motion(trial, "Pelvis", {"PELO","PELA","PELL","PELP"}, {5e-4});
    pig_compare_segment_motion(trial, "R.Thigh", {"RFEO","RFEA","RFEL","RFEP"}, {5e-4});
    pig_compare_segment_motion(trial, "L.Thigh", {"LFEO","LFEA","LFEL","LFEP"}, {5e-4});
    pig_compare_segment_motion(trial, "R.Shank", {"RTIO","RTIA","RTIL","RTIP"}, {5e-4});
    pig_compare_segment_motion(trial, "L.Shank", {"LTIO","LTIA","LTIL","LTIP"}, {5e-4});
    pig_compare_segment_motion(trial, "R.Foot", {"RFOO","RFOA","RFOL","RFOP"}, {1e4}); // 1e4 : Not the same origin
    pig_compare_segment_motion(trial, "L.Foot", {"LFOO","LFOA","LFOL","LFOP"}, {1e4}); // 1e4 : Not the same origin
      
  };
  
  CXXTEST_TEST(GenerateBothUpperBody_OneFrame)
  {
    auto rig = new btk::PluginGait(btk::BodyRegion::Upper, btk::BodySide::Both);
    rig->setMarkerDiameter(16.0); // mm
    rig->setLeftShoulderOffset(50.0); // mm
    rig->setRightShoulderOffset(50.0); // mm
    rig->setLeftElbowWidth(80.0); // mm
    rig->setRightElbowWidth(80.0); // mm
    rig->setLeftWristWidth(40.0); // mm
    rig->setRightWristWidth(40.0); // mm
    rig->setLeftHandThickness(30.0); // mm
    rig->setRightHandThickness(30.0); // mm
    btk::Model model("test", rig);
    
    btk::Node rootCalibration("rootCalibration");
    pig_generate_trial_from_file(&rootCalibration, _BTK_TDD_C3D_IN_"others/PiGCalibration-FlatFoot-One.c3d");
    TS_ASSERT(rig->calibrate(&model, &rootCalibration));
    btk::Node rootDynamic("rootDynamic");
    pig_generate_trial_from_file(&rootDynamic, _BTK_TDD_C3D_IN_"others/PiGMotion-FlatFoot-One.c3d");
    TS_ASSERT(rig->generate(&model, &rootDynamic));
    
    auto trial = rootDynamic.findChild<btk::Trial*>();
    pig_compare_segment_motion(trial, "Head", {"HEDO","HEDA","HEDL","HEDP"}, {1e4}); // 1e4 : Not the same origin
    pig_compare_segment_motion(trial, "Torso", {"TRXO","TRXA","TRXL","TRXP"}, {5e-4});
    pig_compare_segment_motion(trial, "L.Clavicle", {"LCLO","LCLA","LCLL","LCLP"}, {5e-4});
    pig_compare_segment_motion(trial, "R.Clavicle", {"RCLO","RCLA","RCLL","RCLP"}, {5e-4});
    pig_compare_segment_motion(trial, "L.Arm", {"LHUO","LHUA","LHUL","LHUP"}, {5e-4});
    pig_compare_segment_motion(trial, "R.Arm", {"RHUO","RHUA","RHUL","RHUP"}, {5e-4});
    pig_compare_segment_motion(trial, "L.Forearm", {"LRAO","LRAA","LRAL","LRAP"}, {5e-4});
    pig_compare_segment_motion(trial, "R.Forearm", {"RRAO","RRAA","RRAL","RRAP"}, {5e-4});
    pig_compare_segment_motion(trial, "L.Hand", {"LHNO","LHNA","LHNL","LHNP"}, {5e-4});
    pig_compare_segment_motion(trial, "R.Hand", {"RHNO","RHNA","RHNL","RHNP"}, {5e-4});
  };
  
  CXXTEST_TEST(GenerateBothLowerBody_HoleFrames)
  {
    auto rig = new btk::PluginGait(btk::BodyRegion::Lower, btk::BodySide::Both);
    rig->setMarkerDiameter(16.0); // mm
    rig->setLeftFootFlat(true);
    rig->setLeftLegLength(940.0); // mm
    rig->setLeftKneeWidth(110.0); // mm
    rig->setLeftAnkleWidth(70.0); // mm
    rig->setRightFootFlat(true);
    rig->setRightLegLength(940.0); // mm
    rig->setRightKneeWidth(120.0); // mm
    rig->setRightAnkleWidth(70.0); // mm
    btk::Model model("test", rig);
    
    btk::Node rootCalibration("rootCalibration");
    pig_generate_trial_from_file(&rootCalibration, _BTK_TDD_C3D_IN_"others/PiGCalibration-FlatFoot-Hole.c3d");
    TS_ASSERT(rig->calibrate(&model, &rootCalibration));
    btk::Node rootDynamic("rootDynamic");
    pig_generate_trial_from_file(&rootDynamic, _BTK_TDD_C3D_IN_"others/PiGMotion-FlatFoot-Hole.c3d");
    TS_ASSERT(rig->generate(&model, &rootDynamic));
    
    auto trial = rootDynamic.findChild<btk::Trial*>();
    pig_compare_segment_motion(trial, "Pelvis", {"PELO","PELA","PELL","PELP"}, {1e-3});
    pig_compare_segment_motion(trial, "R.Thigh", {"RFEO","RFEA","RFEL","RFEP"}, {75e-5});
    pig_compare_segment_motion(trial, "L.Thigh", {"LFEO","LFEA","LFEL","LFEP"}, {1e-3});
    pig_compare_segment_motion(trial, "R.Shank", {"RTIO","RTIA","RTIL","RTIP"}, {7e-4});
    pig_compare_segment_motion(trial, "L.Shank", {"LTIO","LTIA","LTIL","LTIP"}, {1e-3});
    pig_compare_segment_motion(trial, "R.Foot", {"RFOO","RFOA","RFOL","RFOP"}, {1e4}); // 1e4 : Not the same origin
    pig_compare_segment_motion(trial, "L.Foot", {"LFOO","LFOA","LFOL","LFOP"}, {1e4}); // 1e4 : Not the same origin
  };
  
  CXXTEST_TEST(GenerateBothUpperBody_HoleFrames)
  {
    auto rig = new btk::PluginGait(btk::BodyRegion::Upper, btk::BodySide::Both);
    rig->setMarkerDiameter(16.0); // mm
    rig->setLeftShoulderOffset(50.0); // mm
    rig->setRightShoulderOffset(50.0); // mm
    rig->setLeftElbowWidth(80.0); // mm
    rig->setRightElbowWidth(80.0); // mm
    rig->setLeftWristWidth(40.0); // mm
    rig->setRightWristWidth(40.0); // mm
    rig->setLeftHandThickness(30.0); // mm
    rig->setRightHandThickness(30.0); // mm
    btk::Model model("test", rig);
    
    btk::Node rootCalibration("rootCalibration");
    pig_generate_trial_from_file(&rootCalibration, _BTK_TDD_C3D_IN_"others/PiGCalibration-FlatFoot-Hole.c3d");
    TS_ASSERT(rig->calibrate(&model, &rootCalibration));
    btk::Node rootDynamic("rootDynamic");
    pig_generate_trial_from_file(&rootDynamic, _BTK_TDD_C3D_IN_"others/PiGMotion-FlatFoot-Hole.c3d");
    TS_ASSERT(rig->generate(&model, &rootDynamic));
    
    auto trial = rootDynamic.findChild<btk::Trial*>();
    // NOTE: The accuracy is not the same than for the other unit tests. The reason is not known
    pig_compare_segment_motion(trial, "Head", {"HEDO","HEDA","HEDL","HEDP"}, {1e4}); // 1e4 : Not the same origin
    pig_compare_segment_motion(trial, "Torso", {"TRXO","TRXA","TRXL","TRXP"}, {1e-3});
    pig_compare_segment_motion(trial, "L.Clavicle", {"LCLO","LCLA","LCLL","LCLP"}, {1e-3});
    pig_compare_segment_motion(trial, "R.Clavicle", {"RCLO","RCLA","RCLL","RCLP"}, {1e-3});
    pig_compare_segment_motion(trial, "L.Arm", {"LHUO","LHUA","LHUL","LHUP"}, {1e-3});
    pig_compare_segment_motion(trial, "R.Arm", {"RHUO","RHUA","RHUL","RHUP"}, {1e-3});
    pig_compare_segment_motion(trial, "L.Forearm", {"LRAO","LRAA","LRAL","LRAP"}, {1e-3});
    pig_compare_segment_motion(trial, "R.Forearm", {"RRAO","RRAA","RRAL","RRAP"}, {1e-3});
    pig_compare_segment_motion(trial, "L.Hand", {"LHNO","LHNA","LHNL","LHNP"}, {1e-3});
    pig_compare_segment_motion(trial, "R.Hand", {"RHNO","RHNA","RHNL","RHNP"}, {15e-4});
  };
  
  CXXTEST_TEST(GenerateBothFullBody_FullFrames)
  {
    auto rig = new btk::PluginGait(btk::BodyRegion::Full, btk::BodySide::Both);
    rig->setMarkerDiameter(16.0); // mm
    rig->setLeftShoulderOffset(50.0); // mm
    rig->setRightShoulderOffset(50.0); // mm
    rig->setLeftElbowWidth(80.0); // mm
    rig->setRightElbowWidth(80.0); // mm
    rig->setLeftWristWidth(40.0); // mm
    rig->setRightWristWidth(40.0); // mm
    rig->setLeftHandThickness(30.0); // mm
    rig->setRightHandThickness(30.0); // mm
    rig->setLeftFootFlat(true);
    rig->setLeftLegLength(800.0); // mm
    rig->setLeftKneeWidth(105.0); // mm
    rig->setLeftAnkleWidth(55.0); // mm
    rig->setRightFootFlat(true);
    rig->setRightLegLength(800.0); // mm
    rig->setRightKneeWidth(105.0); // mm
    rig->setRightAnkleWidth(55.0); // mm
    btk::Model model("test", rig);
    
    btk::Node rootCalibration("rootCalibration");
    pig_generate_trial_from_file(&rootCalibration, _BTK_TDD_C3D_IN_"others/PiGCalibration-FlatFoot-Full.c3d");
    TS_ASSERT(rig->calibrate(&model, &rootCalibration));
    btk::Node rootDynamic("rootDynamic");
    pig_generate_trial_from_file(&rootDynamic, _BTK_TDD_C3D_IN_"others/PiGMotion-FlatFoot-Full.c3d");
    TS_ASSERT(rig->generate(&model, &rootDynamic));
    
    auto trial = rootDynamic.findChild<btk::Trial*>();
    // NOTE: The accuracy is not the same than for the other unit tests. The reason is not known
    pig_compare_segment_motion(trial, "Head", {"HEDO","HEDA","HEDL","HEDP"}, {1e4}); // 1e4 : Not the same origin
    pig_compare_segment_motion(trial, "Torso", {"TRXO","TRXA","TRXL","TRXP"}, {5e-4});
    pig_compare_segment_motion(trial, "L.Clavicle", {"LCLO","LCLA","LCLL","LCLP"}, {5e-4});
    pig_compare_segment_motion(trial, "R.Clavicle", {"RCLO","RCLA","RCLL","RCLP"}, {5e-4});
    pig_compare_segment_motion(trial, "L.Arm", {"LHUO","LHUA","LHUL","LHUP"}, {5e-4});
    pig_compare_segment_motion(trial, "R.Arm", {"RHUO","RHUA","RHUL","RHUP"}, {6e-4});
    pig_compare_segment_motion(trial, "L.Forearm", {"LRAO","LRAA","LRAL","LRAP"}, {5e-4});
    pig_compare_segment_motion(trial, "R.Forearm", {"RRAO","RRAA","RRAL","RRAP"}, {5e-4});
    pig_compare_segment_motion(trial, "L.Hand", {"LHNO","LHNA","LHNL","LHNP"}, {6e-4});
    pig_compare_segment_motion(trial, "R.Hand", {"RHNO","RHNA","RHNL","RHNP"}, {5e-4});
    pig_compare_segment_motion(trial, "Pelvis", {"PELO","PELA","PELL","PELP"}, {5e-4});
    pig_compare_segment_motion(trial, "R.Thigh", {"RFEO","RFEA","RFEL","RFEP"}, {5e-4});
    pig_compare_segment_motion(trial, "L.Thigh", {"LFEO","LFEA","LFEL","LFEP"}, {5e-4});
    pig_compare_segment_motion(trial, "R.Shank", {"RTIO","RTIA","RTIL","RTIP"}, {5e-4});
    pig_compare_segment_motion(trial, "L.Shank", {"LTIO","LTIA","LTIL","LTIP"}, {6e-4});
    pig_compare_segment_motion(trial, "R.Foot", {"RFOO","RFOA","RFOL","RFOP"}, {1e4}); // 1e4 : Not the same origin
    pig_compare_segment_motion(trial, "L.Foot", {"LFOO","LFOA","LFOL","LFOP"}, {1e4}); // 1e4 : Not the same origin
  };
};

CXXTEST_SUITE_REGISTRATION(PluginGaitTest)
CXXTEST_TEST_REGISTRATION(PluginGaitTest, SetupBothLowerBody)
CXXTEST_TEST_REGISTRATION(PluginGaitTest, SetupLeftLowerBody)
CXXTEST_TEST_REGISTRATION(PluginGaitTest, SetupRightLowerBody)
CXXTEST_TEST_REGISTRATION(PluginGaitTest, SetupLeftFullBody)
CXXTEST_TEST_REGISTRATION(PluginGaitTest, SetupBothFullBody)
CXXTEST_TEST_REGISTRATION(PluginGaitTest, CalibrateLeftFullBody_OneFrame)
CXXTEST_TEST_REGISTRATION(PluginGaitTest, CalibrateRightFullBody_OneFrame)
CXXTEST_TEST_REGISTRATION(PluginGaitTest, CalibrateBothFullBody_OneFrame)
CXXTEST_TEST_REGISTRATION(PluginGaitTest, CalibrateBothUpperBody_FullFrames)
CXXTEST_TEST_REGISTRATION(PluginGaitTest, CalibrateBothLowerBody_FullFrames)
CXXTEST_TEST_REGISTRATION(PluginGaitTest, CalibrateBothFullBody_FullFrames)
CXXTEST_TEST_REGISTRATION(PluginGaitTest, CalibrateBothFullBody_HoleFrames)
CXXTEST_TEST_REGISTRATION(PluginGaitTest, CalibrateBothFullBody_NoOption)
CXXTEST_TEST_REGISTRATION(PluginGaitTest, GenerateBothLowerBody_OneFrame)
CXXTEST_TEST_REGISTRATION(PluginGaitTest, GenerateBothUpperBody_OneFrame)
CXXTEST_TEST_REGISTRATION(PluginGaitTest, GenerateBothLowerBody_HoleFrames)
CXXTEST_TEST_REGISTRATION(PluginGaitTest, GenerateBothUpperBody_HoleFrames)
CXXTEST_TEST_REGISTRATION(PluginGaitTest, GenerateBothFullBody_FullFrames)
#endif // PluginGaitTest_h
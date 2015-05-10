/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2015, Arnaud Barré
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 *     * Redistributions of source code must retain the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *     * Neither the name(s) of the copyright holders nor the names
 *       of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "btkPluginGait.h"
#include "btkPluginGait_p.h"
#include "btkLogger.h"
#include "btkModel.h"
#include "btkBodyEnums.h"
#include "btkSegment.h"
#include "btkJoint.h"
#include "btkLandmarksTranslator.h"
#include "btkRelativeReferenceFrame.h"
#include "btklardModel.h"

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

namespace btk
{
  PluginGaitPrivate::PluginGaitPrivate(PluginGait* pint, const std::string& name, int region, int side)
  : RigPrivate(pint,name,region,side),
    // All the options are null by default
    MarkerDiameter(0.0),
    HeadOffset(0.0),
    RightShoulderOffset(0.0),
    LeftShoulderOffset(0.0),
    RightElbowWidth(0.0),
    LeftElbowWidth(0.0),
    RightWristWidth(0.0),
    LeftWristWidth(0.0),
    RightHandThickness(0.0),
    LeftHandThickness(0.0),
    InterAsisDistance(0.0),
    RightLegLength(0.0),
    LeftLegLength(0.0),
    RightAsisTrochanterAPDistance(0.0),
    LeftAsisTrochanterAPDistance(0.0),
    RightKneeWidth(0.0),
    LeftKneeWidth(0.0),
    RightAnkleWidth(0.0),
    LeftAnkleWidth(0.0),
    RightFootFlat(false), 
    LeftFootFlat(false),
    RightStaticPlantarFlexionOffset(0.0),
    RightStaticRotationOffset(0.0),
    LeftStaticPlantarFlexionOffset(0.0),
    LeftStaticRotationOffset(0.0)
  {};
    
  PluginGaitPrivate::~PluginGaitPrivate() noexcept = default;
  
  Node* PluginGaitPrivate::makePint() const
  {
    auto node = new PluginGait(this->Region, this->Side);
    auto node_pimpl = node->pimpl();
    node_pimpl->MarkerDiameter = this->MarkerDiameter;
    node_pimpl->HeadOffset = this->HeadOffset;
    node_pimpl->RightShoulderOffset = this->RightShoulderOffset;
    node_pimpl->LeftShoulderOffset = this->LeftShoulderOffset;
    node_pimpl->RightElbowWidth = this->RightElbowWidth;
    node_pimpl->LeftElbowWidth = this->LeftElbowWidth;
    node_pimpl->RightWristWidth = this->RightWristWidth;
    node_pimpl->LeftWristWidth = this->LeftWristWidth;
    node_pimpl->RightHandThickness = this->RightHandThickness;
    node_pimpl->LeftHandThickness = this->LeftHandThickness;
    node_pimpl->InterAsisDistance = this->InterAsisDistance;
    node_pimpl->RightLegLength = this->RightLegLength;
    node_pimpl->LeftLegLength = this->LeftLegLength;
    node_pimpl->RightAsisTrochanterAPDistance  = this->RightAsisTrochanterAPDistance ;
    node_pimpl->LeftAsisTrochanterAPDistance = this->LeftAsisTrochanterAPDistance;
    node_pimpl->RightKneeWidth = this->RightKneeWidth;
    node_pimpl->LeftKneeWidth = this->LeftKneeWidth;
    node_pimpl->RightAnkleWidth = this->RightAnkleWidth;
    node_pimpl->LeftAnkleWidth = this->LeftAnkleWidth;
    node_pimpl->RightFootFlat = this->RightFootFlat;
    node_pimpl->LeftFootFlat = this->LeftFootFlat;
    node_pimpl->RightStaticPlantarFlexionOffset = this->RightStaticPlantarFlexionOffset;
    node_pimpl->RightStaticRotationOffset = this->RightStaticRotationOffset;
    node_pimpl->LeftStaticPlantarFlexionOffset = this->LeftStaticPlantarFlexionOffset;
    node_pimpl->LeftStaticRotationOffset = this->LeftStaticRotationOffset;
    return node;
  };
  
  bool PluginGaitPrivate::calibrateLowerLimb(int side, const lard::Trajectory* HJC, std::unordered_map<std::string,lard::MappedTrajectory>* landmarks) noexcept
  {
    std::string prefix;
    double s = 0.0, ankleWidth = 0.0, kneeWidth = 0.0;
    double *staticPlantarFlexionOffset = nullptr, *staticRotationOffset = nullptr;
    bool footFlat = false;
    if (side == BodySide::Left)
    {
      prefix = "L.";
      s = 1.0;
      ankleWidth = this->LeftAnkleWidth;
      kneeWidth = this->LeftKneeWidth;
      footFlat = this->LeftFootFlat;
      staticPlantarFlexionOffset = &(this->LeftStaticPlantarFlexionOffset);
      staticRotationOffset = &(this->LeftStaticRotationOffset);
    }
    else if (side == BodySide::Right)
    {
      prefix = "R.";
      s = -1.0;
      ankleWidth = this->RightAnkleWidth;
      kneeWidth = this->RightKneeWidth;
      footFlat = this->RightFootFlat;
      staticPlantarFlexionOffset = &(this->RightStaticPlantarFlexionOffset);
      staticRotationOffset = &(this->RightStaticRotationOffset);
    }
    else
    {
      Logger::error("PluginGait - Unknown side for the lower limb. Calibration aborted.");
      return false;
    }
    // -----------------------------------------
    // Thigh
    // -----------------------------------------
    // Required landmarks: *.ITB, *.LFE
    const lard::MappedTrajectory& ITB = (*landmarks)[prefix+"ITB"];
    const lard::MappedTrajectory& LFE = (*landmarks)[prefix+"LFE"];
    if (!ITB.isValid() || !LFE.isValid())
    {
      Logger::error("PluginGait - Missing landmarks to define the thigh. Calibration aborted.");
      return false;
    }
    // Compute the knee joint centre (KJC)
    const lard::Trajectory KJC = generate_chord_trajectory((this->MarkerDiameter + kneeWidth) / 2.0, LFE, *HJC, ITB);
    // -----------------------------------------
    // Shank
    // -----------------------------------------
    // Required landmarks: *.LTM, *.LS
    const lard::MappedTrajectory& LTM = (*landmarks)[prefix+"LTM"];
    const lard::MappedTrajectory& LS = (*landmarks)[prefix+"LS"];
    if (!LTM.isValid() || !LS.isValid())
    {
      Logger::error("PluginGait - Missing landmarks to define the shank. Calibration aborted.");
      return false;
    }
    // Compute the ankle joint centre (AJC)
    const lard::Trajectory AJC = generate_chord_trajectory((this->MarkerDiameter + ankleWidth) / 2.0, LTM, KJC, LS);
    
    // -----------------------------------------
    // Foot
    // -----------------------------------------
    // Required landmarks: *.MTH2, *.HEE
    const lard::MappedTrajectory& MTH2 = (*landmarks)[prefix+"MTH2"];
    lard::Trajectory HEE = (*landmarks)[prefix+"HEE"]; // Copy instead of a map due to possible modification on its coordinates if the foot flat option is activated
    if (!MTH2.isValid() || !HEE.isValid())
    {
      Logger::error("PluginGait - Missing landmarks to define the foot. Calibration aborted.");
      return false;
    }
    if (footFlat)
    {
      // Copy the Z-coordinates of MTH2 into HEE
      // FIXME: This assume that the vertical axis is always Z!
      HEE.block<1>(2) = MTH2.block<1>(2);
    }
    // Shank axes
    lard::Vector w = (KJC - AJC).normalized();
    lard::Vector u = s * (KJC - AJC).cross(LS - AJC).normalized();
    lard::Vector v_shank = w.cross(u);
    // Foot reference
    w = (MTH2 - HEE).normalized();
    u = v_shank.cross(w).normalized();
    lard::Motion foot(u,w.cross(u),w,AJC);
    // Uncorrected foot reference
    w = (MTH2 - AJC).normalized();
    u = v_shank.cross(w).normalized();
    lard::Motion uncorrected_foot(u,w.cross(u),w,AJC);

    // Offset angles
    if (foot.isOccluded() || uncorrected_foot.isOccluded())
    {
      Logger::error("PluginGait - Impossible to find a least one valid frame for the foot motion. Calibration aborted.");
      return false;
    }
    lard::Vector::Values offsetAngles = uncorrected_foot.inverse().transform(foot).eulerAngles(1,0,2).mean().values();
    *staticPlantarFlexionOffset = 1.0 * offsetAngles.coeff(0);
    *staticRotationOffset = -1.0 * s * offsetAngles.coeff(1);
    return true;
  };
  
  bool PluginGaitPrivate::generateUpperLimb(Model* model, Trial* trial, int side, const lard::Vector* u_torso, const lard::Vector* o_torso, std::unordered_map<std::string,lard::MappedTrajectory>* landmarks, double sampleRate, double startTime) const noexcept
  {
    std::string prefix;
    double s = 0.0, shoulderOffset = 0.0, elbowWidth = 0.0, wristWidth = 0.0, handThickness = 0.0;
    if (side == BodySide::Left)
    {
      prefix = "L.";
      s = -1.0;
      shoulderOffset = this->LeftShoulderOffset;
      elbowWidth = this->LeftElbowWidth;
      wristWidth = this->LeftWristWidth;
      handThickness = this->LeftHandThickness;
    }
    else if (side == BodySide::Right)
    {
      prefix = "R.";
      s = 1.0;
      shoulderOffset = this->RightShoulderOffset;
      elbowWidth = this->RightElbowWidth;
      wristWidth = this->RightWristWidth;
      handThickness = this->RightHandThickness;
    }
    else
    {
      Logger::error("PluginGait - Unknown side for the upper limb. Motion generation aborted for trial '%s'.", trial->name().c_str());
      return false;
    }
    // Temporary variable use to construct segments' motion
    lard::Vector u, v, w, o;
    // -----------------------------------------
    // Arm & clavicle (optional)
    // -----------------------------------------
    // Required landmarks: *.AC, *.US, *.RS, *.LHE
    const lard::MappedTrajectory& AC = (*landmarks)[prefix+"AC"];
    const lard::MappedTrajectory& US = (*landmarks)[prefix+"US"];
    const lard::MappedTrajectory& RS = (*landmarks)[prefix+"RS"];
    const lard::MappedTrajectory& LHE = (*landmarks)[prefix+"LHE"];
    if (!AC.isValid() || !US.isValid() || !RS.isValid() || !LHE.isValid())
    {
      Logger::error("PluginGait - Missing landmarks to define the arm. Motion generation aborted for trial '%s'.", trial->name().c_str());
      return false;
    }
    // VWM: virtual "wand" marker
    const lard::Trajectory VWM = (AC - *o_torso).cross(*u_torso) + AC;
    // Compute the shoulder joint centre (SJC)
    const lard::Trajectory SJC = generate_chord_trajectory(s * (shoulderOffset + this->MarkerDiameter / 2.0), AC, *o_torso, VWM);
    // Clavicle
    Segment* clavicle = model->segments()->findChild<Segment*>({},{{"side",side},{"part",BodyPart::Clavicle}},false);
    if (clavicle != nullptr)
    {
      w = (*o_torso - SJC).normalized();
      u = w.cross(VWM - SJC).normalized();
      v = w.cross(u);
      lard::export_timesequence(&u, &v, &w, &SJC, prefix+"Clavicle", sampleRate, startTime, trial->timeSequences());
    }
    // Construction of the vector marker (CVM) for the elbow
    const lard::Trajectory MWP = (US + RS) / 2.0; // Middle Wrist Point
    const lard::Trajectory CVM = s * (MWP - LHE).cross(SJC - LHE).normalized() + LHE;
    const lard::Trajectory EJC = generate_chord_trajectory((elbowWidth + this->MarkerDiameter) / 2.0, LHE, SJC, CVM);
    // Compute the wrist joint centre (WJC)
    // NOTE: The scale factor was found experimentaly when comparing with the Vicon data.
    const lard::Trajectory WJC = s * (US - RS).cross(EJC - MWP).normalized() * wristWidth * 0.7 + MWP;
    // Arm frame
    w = (SJC - EJC).normalized();
    v = (WJC - EJC).cross(w).normalized();
    u = v.cross(w);
    lard::export_timesequence(&u, &v, &w, &EJC, prefix+"Arm", sampleRate, startTime, trial->timeSequences());
    // -----------------------------------------
    // Forearm
    // -----------------------------------------
    w = (EJC - WJC).normalized();
    u = v.cross(w); // The 'v' axis is the same than the one defined for the arm
    lard::export_timesequence(&u, &v, &w, &WJC, prefix+"Forearm", sampleRate, startTime, trial->timeSequences());
    // -----------------------------------------
    // Hand
    // -----------------------------------------
    // Required landmark : *.MH2
    const lard::MappedTrajectory& MH2 = (*landmarks)[prefix+"MH2"];
    if (!MH2.isValid())
    {
      Logger::error("PluginGait - Missing landmark 'MH2' to define the hand. Motion generation aborted for trial '%s'.", trial->name().c_str());
      return false;
    }
    // HO: Hand Origin
    const lard::Trajectory HO = generate_chord_trajectory((handThickness + this->MarkerDiameter) / 2.0, MH2, WJC, MWP);
    o =  2.0 * (HO - WJC) + WJC;
    w = (WJC - HO).normalized();
    u = s * w.cross(US - RS).normalized();
    v = w.cross(u);
    lard::export_timesequence(&u, &v, &w, &o, prefix+"Hand", sampleRate, startTime, trial->timeSequences());
    return true;
  };
  
  bool PluginGaitPrivate::generateLowerLimb(Trial* trial, int side, const lard::Trajectory* HJC, std::unordered_map<std::string,lard::MappedTrajectory>* landmarks, double sampleRate, double startTime) const noexcept
  {
    std::string prefix;
    double s = 0.0, ankleWidth = 0.0, kneeWidth = 0.0,
           staticPlantarFlexionOffset = 0.0, staticRotationOffset = 0.0;
    if (side == BodySide::Left)
    {
      prefix = "L.";
      s = -1.0;
      ankleWidth = this->LeftAnkleWidth;
      kneeWidth = this->LeftKneeWidth;
      staticPlantarFlexionOffset = -this->LeftStaticPlantarFlexionOffset;
      staticRotationOffset = -this->LeftStaticRotationOffset;
    }
    else if (side == BodySide::Right)
    {
      prefix = "R.";
      s = 1.0;
      ankleWidth = this->RightAnkleWidth;
      kneeWidth = this->RightKneeWidth;
      staticPlantarFlexionOffset = -this->RightStaticPlantarFlexionOffset;
      staticRotationOffset = this->RightStaticRotationOffset;
    }
    else
    {
      Logger::error("PluginGait - Unknown side for the lower limb. Motion generation aborted for trial '%s'.", trial->name().c_str());
      return false;
    }
    // Temporary variable use to construct segments' motion
    lard::Vector u,v,w,o;
    // -----------------------------------------
    // Thigh
    // -----------------------------------------
    // Required landmarks: *.ITB, *.LFE
    const lard::MappedTrajectory& ITB = (*landmarks)[prefix+"ITB"];
    const lard::MappedTrajectory& LFE = (*landmarks)[prefix+"LFE"];
    if (!ITB.isValid() || !LFE.isValid())
    {
      Logger::error("PluginGait - Missing landmarks to define the thigh. Motion generation aborted for trial '%s'.", trial->name().c_str());
      return false;
    }
    const lard::Trajectory KJC = generate_chord_trajectory((this->MarkerDiameter + kneeWidth) / 2.0, LFE, *HJC, ITB);
    u = s * (*HJC - LFE).cross(ITB - LFE).normalized();
    w = (*HJC - KJC).normalized();
    v = w.cross(u);
    lard::export_timesequence(&u, &v, &w, &KJC, prefix+"Thigh", sampleRate, startTime, trial->timeSequences());
    
    // -----------------------------------------
    // Shank
    // -----------------------------------------
    // Required landmarks: *.LTM, *.LS
    const lard::MappedTrajectory& LTM = (*landmarks)[prefix+"LTM"];
    const lard::MappedTrajectory& LS = (*landmarks)[prefix+"LS"];
    if (!LTM.isValid() || !LS.isValid())
    {
      Logger::error("PluginGait - Missing landmarks to define the shank. Motion generation aborted for trial '%s'.", trial->name().c_str());
      return false;
    }
    // Compute the ankle joint centre (AJC)
    const lard::Trajectory AJC = generate_chord_trajectory((this->MarkerDiameter + ankleWidth) / 2.0, LTM, KJC, LS);
    w = (KJC - AJC).normalized();
    u = s * w.cross(LS - AJC).normalized();
    lard::Vector v_shank = w.cross(u);
    lard::export_timesequence(&u, &v_shank, &w, &AJC, prefix+"Shank", sampleRate, startTime, trial->timeSequences());
    // -----------------------------------------
    // Foot
    // -----------------------------------------
    // Required landmarks: *.MTH2
    const lard::MappedTrajectory& MTH2 = (*landmarks)[prefix+"MTH2"];
    if (!MTH2.isValid())
    {
      Logger::error("PluginGait - Missing landmark to define the foot. Motion generation aborted for trial '%s'.", trial->name().c_str());
      return false;
    }
    
    w = -1.0 * (MTH2 - AJC).normalized();
    u = v_shank.cross(w).normalized();
    v = w.cross(u);
    const double cx = cos(staticRotationOffset), sx = sin(staticRotationOffset),
                 cy = cos(staticPlantarFlexionOffset), sy = sin(staticPlantarFlexionOffset);
    
    const lard::Vector ur = u * cy - w * sy;
    const lard::Vector vr = u * sx*sy + v * cx + w * cy*sx;
    const lard::Vector wr = u * cx*sy - v * sx + w * cx*cy;
    // FIXME: This is not the good origin. None anatomical meaning was discovered to explain this position...
    lard::export_timesequence(&ur, &vr, &wr, &AJC, prefix+"Foot", sampleRate, startTime, trial->timeSequences());
    return true;
  };
};

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

namespace btk
{
  PluginGait::PluginGait(int region, int side, btk::Node* parent)
  : Rig(*new PluginGaitPrivate(this, "PiG", region, side), parent)
  {};

  bool PluginGait::setup(btk::Model* model) const noexcept
  {
    if (model == nullptr)
    {
      Logger::error("Null model passed. Setup aborted.");
      return false;
    }
    auto optr = this->pimpl();
    if (optr->Side == BodySide::Center)
    {
      Logger::error("Impossible to have a 'center' side for the PluginGait rig. Only the enumerators BodySide::Right, BodySide::Left, and BodySide::Both are available for this rig. Setup aborted.");
      return false;
    }
    Node* segments = model->segments();
    Node* joints = model->joints();
    Segment *torso = nullptr, *pelvis = nullptr;
    if (optr->Region & BodyRegion::Upper)
    {
      Segment* head = new Segment("Head", BodyPart::Head, BodySide::Center, segments);
      torso = new Segment("Torso", BodyPart::Torso, BodySide::Center, segments);
      if (optr->Side & BodySide::Left)
      {
        new Segment("L.Clavicle", BodyPart::Clavicle, BodySide::Left, segments);
        Segment* leftArm = new Segment("L.Arm", BodyPart::Arm, BodySide::Left, segments);
        Segment* leftForearm = new Segment("L.Forearm", BodyPart::Forearm, BodySide::Left, segments);
        Segment* leftHand = new Segment("L.Hand", BodyPart::Hand, BodySide::Left, segments);
        new Joint("L.Shoulder", torso, leftArm, joints);
        new Joint("L.Elbow", leftArm, leftForearm, joints);
        new Joint("L.Wrist", leftForearm, leftHand, joints);
      }
      if (optr->Side & BodySide::Right)
      {
        new Segment("R.Clavicle", BodyPart::Clavicle, BodySide::Right, segments);
        Segment* rightArm = new Segment("R.Arm", BodyPart::Arm, BodySide::Right, segments);
        Segment* rightForearm = new Segment("R.Forearm", BodyPart::Forearm, BodySide::Right, segments);
        Segment* rightHand = new Segment("R.Hand", BodyPart::Hand, BodySide::Right, segments);
        new Joint("R.Shoulder", torso, rightArm, joints);
        new Joint("R.Elbow", rightArm, rightForearm, joints);
        new Joint("R.Wrist", rightForearm, rightHand, joints);
      }
      new Joint("Neck", head, torso, joints);
      Joint* globalThorax = new Joint("Thorax", nullptr, torso, joints);
      globalThorax->setDescription("Torso relative to global frame");
      Joint* globalHead = new Joint("Head", nullptr, head, joints);
      globalHead->setDescription("Head relative to global frame");
    }
    if (optr->Region & BodyRegion::Lower)
    {
      pelvis = new Segment("Pelvis", BodyPart::Pelvis, BodySide::Center, segments);
      if (optr->Side & BodySide::Left)
      {
        Segment* leftThigh = new Segment("L.Thigh", BodyPart::Thigh, BodySide::Left, segments);
        Segment* leftShank = new Segment("L.Shank", BodyPart::Shank, BodySide::Left, segments);
        Segment* leftFoot = new Segment("L.Foot", BodyPart::Foot, BodySide::Left, segments);
        new Joint("L.Hip", pelvis, leftThigh, joints);
        new Joint("L.Knee", leftThigh, leftShank, joints);
        new Joint("L.Ankle", leftShank, leftFoot, joints);
        Joint* lfp = new Joint("L.FootProgress", nullptr, leftFoot, joints);
        lfp->setDescription("Left foot relative to global frame");
      }
      if (optr->Side & BodySide::Right)
      {
        Segment* rightThigh = new Segment("R.Thigh", BodyPart::Thigh, BodySide::Right, segments);
        Segment* rightShank = new Segment("R.Shank", BodyPart::Shank, BodySide::Right, segments);
        Segment* rightFoot = new Segment("R.Foot", BodyPart::Foot, BodySide::Right,segments);
        new Joint("R.Hip", pelvis, rightThigh, joints);
        new Joint("R.Knee", rightThigh, rightShank, joints);
        new Joint("R.Ankle", rightShank, rightFoot, joints);
        Joint* rfp = new Joint("R.FootProgress", nullptr, rightFoot, joints);
        rfp->setDescription("Right foot relative to global frame");
      }
      new Joint("Pelvis", nullptr, pelvis, joints);
    }
    if ((optr->Region & BodyRegion::Full) == BodyRegion::Full)
    {
      Joint* spine = new Joint("Spine", torso, pelvis, joints);
      spine->setDescription("Torso relative to pelvis");
    }
    return true;
  };

  bool PluginGait::calibrate(btk::Model* model, Node* trials) noexcept
  {
    if (model == nullptr)
    {
      Logger::error("PluginGait - Null model passed. Calibration aborted.");
      return false;
    }
    if (trials == nullptr)
    {
      Logger::error("PluginGait - Null trials passed. Calibration aborted.");
      return false;
    }
    auto trial = trials->findChild<btk::Trial*>();
    if (trial == nullptr)
    {
       Logger::error("PluginGait - No trial found. Calibration aborted.");
      return false;
    }
    bool ok = false;
    auto landmarks = lard::extract_landmark_trajectories(model, trial, nullptr, nullptr, &ok);
    if (!ok)
    {
      Logger::error("PluginGait - The sampling information are no consistent (sampling rate or start time not the same) between required landmarks. Calibration aborted.");
      return false;
    }
    
    auto optr = this->pimpl();
    Segment* seg = nullptr;
    
    // --------------------------------------------------
    // LOWER LIMB
    // --------------------------------------------------
    if (optr->Region & BodyRegion::Lower)
    {
      lard::Trajectory L_HJC(1); L_HJC.setZeroResiduals();
      lard::Trajectory R_HJC(1); R_HJC.setZeroResiduals();
      auto& _R_HJC = R_HJC.values();
      auto& _L_HJC = L_HJC.values();
      const lard::MappedTrajectory& L_ASIS = landmarks["L.ASIS"];
      const lard::MappedTrajectory& R_ASIS = landmarks["R.ASIS"];
      
      //  - inter ASIS distance
      if (optr->InterAsisDistance == 0.0)
      {
        if (!L_ASIS.isValid() || !R_ASIS.isValid())
        {
          Logger::error("PluginGait - Missing landmarks to compute the inter ASIS distance. Calibration aborted.");
          return false;
        }
        else
          optr->InterAsisDistance = (L_ASIS - R_ASIS).mean().norm();
      }
      //  - leg length
      double leftLegLength = optr->LeftLegLength;
      double rightLegLength = optr->RightLegLength;
      if ((leftLegLength == 0.0) && (rightLegLength > 0.0))
      {
        Logger::warning("PluginGait - Missing model's parameter: unknown left leg length. The algorithm uses only the right leg length.");
        leftLegLength = rightLegLength;
      }
      else if ((leftLegLength > 0.0) && (rightLegLength == 0.0))
      {
        Logger::warning("PluginGait - Missing model's parameter: unknown right leg length. The algorithm uses only the left leg length.");
        rightLegLength = leftLegLength;
      }
      else if ((leftLegLength < 0.0) && (rightLegLength < 0.0))
      {
        Logger::error("PluginGait - Missing model's parameter. Unknown left and right leg lengths. Calibration aborted.");
        return false;
      }
      //  - x_dis parameter
      if (optr->LeftAsisTrochanterAPDistance <= 0.0)
        optr->LeftAsisTrochanterAPDistance = 0.1288 * leftLegLength - 48.56;
      if (optr->RightAsisTrochanterAPDistance <= 0.0)
        optr->RightAsisTrochanterAPDistance = 0.1288 * rightLegLength - 48.56;
      // - hip joint center
      optr->computeHipJointCenter(_L_HJC.data(), -1.0,  leftLegLength * 0.115 - 15.3, optr->LeftAsisTrochanterAPDistance);
      optr->computeHipJointCenter(_R_HJC.data(),  1.0, rightLegLength * 0.115 - 15.3, optr->RightAsisTrochanterAPDistance);
      // -----------------------------------------
      // Pelvis
      // -----------------------------------------
      seg = model->segments()->findChild<Segment*>({},{{"part",BodyPart::Pelvis}},false);
      if (seg == nullptr)
      {
        Logger::error("PluginGait - Missing pelvis segment in the model. Calibration aborted.");
        return false;
      }
      auto relframe = new RelativeReferenceFrame("SF", seg);
      relframe->o()[0] = (_L_HJC.coeff(0) + _R_HJC.coeff(0)) / 2.0;
      relframe->o()[1] = (_L_HJC.coeff(1) + _R_HJC.coeff(1)) / 2.0;
      relframe->o()[2] = (_L_HJC.coeff(2) + _R_HJC.coeff(2)) / 2.0;
      new RelativePosition("L.HJC", _L_HJC.data(), seg);
      new RelativePosition("R.HJC", _R_HJC.data(), seg);
      // Required landmarks: *.ASIS, SC or *.PSIS
      const lard::MappedTrajectory& L_PSIS = landmarks["L.PSIS"];
      const lard::MappedTrajectory& R_PSIS = landmarks["R.PSIS"];
      lard::Trajectory SC = landmarks["SC"];
      if (!L_ASIS.isValid() || !R_ASIS.isValid())
      {
        Logger::error("PluginGait - Missing landmarks (ASISs) to define the pelvis. Calibration aborted.");
        return false;
      }
      if (!SC.isValid() && (!L_PSIS.isValid() || !R_PSIS.isValid()))
      {
        Logger::error("PluginGait - Missing landmarks (SC or PSISs) to define the pelvis. Calibration aborted.");
        return false;
      }
      if (!SC.isValid())
        SC = (L_PSIS + R_PSIS) / 2.0;
      
      const lard::Vector v = (L_ASIS - R_ASIS).normalized();
      const lard::Vector w = ((R_ASIS - SC).cross(L_ASIS - SC)).normalized();
      const lard::Motion pelvis(v.cross(w), v, w, (L_ASIS + R_ASIS) / 2.0);
      // -----------------------------------------
      // Other lower limbs (dependant of the hip joint centers)
      // -----------------------------------------
      if ((optr->Side & BodySide::Left) == BodySide::Left)
      {
        const lard::Trajectory HJC = pelvis.transform(L_HJC.replicate(pelvis.rows()));
        if (!optr->calibrateLowerLimb(BodySide::Left, &HJC, &landmarks))
          return false;
      }
      if ((optr->Side & BodySide::Right) == BodySide::Right)
      {
        const lard::Trajectory HJC = pelvis.transform(R_HJC.replicate(pelvis.rows()));
        if (!optr->calibrateLowerLimb(BodySide::Right, &HJC, &landmarks))
          return false;
       }
    }
    // --------------------------------------------------
    // Other Segment
    // --------------------------------------------------
    auto segments = model->segments()->findChildren<Segment*>();
    for (auto it = segments.cbegin() ; it != segments.cend() ; ++it)
    {
      if ((*it)->part() == BodyPart::Head)
      {
        // -----------------------------------------
        // Head
        // -----------------------------------------
        // Required landmarks: L.HF, L.HB, R.HF and R.HB
        const lard::MappedTrajectory& L_HF = landmarks["L.HF"];
        const lard::MappedTrajectory& L_HB = landmarks["L.HB"];
        const lard::MappedTrajectory& R_HF = landmarks["R.HF"];
        const lard::MappedTrajectory& R_HB = landmarks["R.HB"];
        if (!L_HF.isValid() || !L_HB.isValid() || !R_HF.isValid() || !R_HB.isValid())
        {
          Logger::error("PluginGait - Missing landmarks to define the head. Calibration aborted.");
          return false;
        }
        // NOTE : The markers are first averaged before the computation of the offset!
        const lard::Trajectory _L_HF = L_HF.mean();
        const lard::Trajectory _L_HB = L_HB.mean();
        const lard::Trajectory _R_HF = R_HF.mean();
        const lard::Trajectory _R_HB = R_HB.mean();        
        // WARNING: The origin (set to the middle of the four points) is not the same than Vicon!
        const lard::Vector u = ((_L_HF + _R_HF) / 2.0 - (_L_HB + _R_HB) / 2.0).normalized();
        const lard::Vector w = u.cross((_L_HF + _L_HB) / 2.0 - (_R_HF + _R_HB) / 2.0).normalized();
        const lard::Motion head(u, w.cross(u), w, (_L_HF + _R_HF + _L_HB + _R_HB) / 4.0);
        if (head.isOccluded())
        {
          Logger::error("PluginGait - Impossible to find a least one valid frame for the head motion. Calibration aborted.");
          return false;
        }
        optr->HeadOffset = M_PI - head.eulerAngles(2,0,1).mean().values().z();
      }
    }
    return true;
  };

  bool PluginGait::generate(btk::Model* model, Node* trials) const noexcept
  {
    if (model == nullptr)
    {
      Logger::error("PluginGait - Null model passed. Motion generation aborted.");
      return false;
    }
    if (trials == nullptr)
    {
      Logger::error("PluginGait - Null trials passed. Motion generation aborted.");
      return false;
    }
    int inc = -1;
    auto _trials = trials->findChildren<btk::Trial*>();
    for (auto trial : _trials)
    {
      ++inc;
      if (trial == nullptr)
      {
        Logger::error("PluginGait - Trial #%i is null. Motion generation skipped for this trial.", inc);
        continue;
      }
      double sampleRate = 0.0;
      double startTime = 0.0;
      bool ok = false;
      auto landmarks = lard::extract_landmark_trajectories(model, trial, &sampleRate, &startTime, &ok);
      if (!ok)
      {
        Logger::error("PluginGait - The sampling information are no consistent (sampling rate or start time not the same) between required landmarks. Motion generation aborted for trial '%s'.", trial->name().c_str());
        continue;
      }
      auto optr = this->pimpl();
      Segment* seg = nullptr;
      
      // --------------------------------------------------
      // UPPER LIMB
      // --------------------------------------------------
      if (optr->Region & BodyRegion::Upper)
      {
        // -----------------------------------------
        // Torso
        // -----------------------------------------
        // Required landmarks: C7, T10, SS and XP
        const lard::MappedTrajectory& C7 = landmarks["C7"];
        const lard::MappedTrajectory& T10 = landmarks["T10"];
        const lard::MappedTrajectory& SS = landmarks["SS"];
        const lard::MappedTrajectory& XP = landmarks["XP"];
        if (!C7.isValid() || !T10.isValid() || !SS.isValid() || !XP.isValid())
        {
          Logger::error("PluginGait - Missing landmarks to define the torso. Motion generation aborted for trial '%s'.", trial->name().c_str());
          continue;
        }
        const lard::Vector w = ((XP + T10) / 2.0 - (SS + C7) / 2.0).normalized();
        const lard::Vector v = w.cross((XP + SS) / 2.0 - (T10 + C7) / 2.0).normalized();
        const lard::Vector u = v.cross(w);
        const lard::Vector o = SS - (optr->MarkerDiameter / 2.0 * u);
        lard::export_timesequence(&u, &v, &w, &o, "Torso", sampleRate, startTime, trial->timeSequences());
        // -----------------------------------------
        // Other upper limbs (dependant of the torso)
        // -----------------------------------------
        if ((optr->Side & BodySide::Left) && !optr->generateUpperLimb(model, trial, BodySide::Left, &u,  &o, &landmarks, sampleRate, startTime))
          continue;
        if ((optr->Side & BodySide::Right) && !optr->generateUpperLimb(model, trial, BodySide::Right, &u, &o, &landmarks, sampleRate, startTime))
          continue;
      }
      // --------------------------------------------------
      // LOWER LIMB
      // --------------------------------------------------
      if (optr->Region & BodyRegion::Lower)
      {
        // -----------------------------------------
        // Pelvis
        // -----------------------------------------
        seg = model->segments()->findChild<Segment*>("Pelvis",{},false);
        if (seg == nullptr)
        {
          Logger::error("PluginGait - Missing pelvis segment in the model. Motion generation aborted.");
          return false;
        }
        // Required landmarks: *.ASIS, SC or *.PSIS
        const lard::MappedTrajectory& L_ASIS = landmarks["L.ASIS"];
        const lard::MappedTrajectory& R_ASIS = landmarks["R.ASIS"];
        const lard::MappedTrajectory& L_PSIS = landmarks["L.PSIS"];
        const lard::MappedTrajectory& R_PSIS = landmarks["R.PSIS"];
        lard::Trajectory SC = landmarks["SC"];
        if (!L_ASIS.isValid() || !R_ASIS.isValid())
        {
          Logger::error("PluginGait - Missing landmarks (ASISs) to define the pelvis. Motion generation aborted for trial '%s'.", trial->name().c_str());
          continue;
        }
        if (!SC.isValid() && (!L_PSIS.isValid() || !R_PSIS.isValid()))
        {
          Logger::error("PluginGait - Missing landmarks (SC or PSISs) to define the pelvis. Motion generation aborted for trial '%s'.", trial->name().c_str());
          continue;
        }
        if (!SC.isValid())
          SC = (L_PSIS + R_PSIS) / 2.0;
        const lard::Vector v = (L_ASIS - R_ASIS).normalized();
        const lard::Vector w = ((R_ASIS - SC).cross(L_ASIS - SC)).normalized();
        const lard::Motion pelvis(v.cross(w), v, w, (L_ASIS + R_ASIS) / 2.0);
        const RelativeReferenceFrame* relframe = seg->findChild<const RelativeReferenceFrame*>("SF",{},false);
        if (relframe == nullptr)
        {
          Logger::error("PluginGait - Missing segmental reference frame relative to the pelvis in the model. Motion generation aborted.");
          return false;
        }
        const lard::Motion pelvis_SF = transform_relative_frame(relframe, seg, &pelvis);
        lard::export_timesequence(&pelvis_SF, "Pelvis", sampleRate, startTime, TimeSequence::Pose, "", trial->timeSequences());
        // -----------------------------------------
        // Thigh, shank, foot
        // -----------------------------------------
        if ((optr->Side & BodySide::Left) == BodySide::Left)
        {
          const RelativePosition* relpos = seg->findChild<const RelativePosition*>("L.HJC",{},false);
          if (relpos == nullptr)
          {
            Logger::error("PluginGait - Missing left hip joint center position relative to the pelvis in the model. Motion generation aborted.");
            return false;
          }
          const lard::Trajectory HJC = lard::transform_relative_position(relpos, seg, &pelvis);
          if (!optr->generateLowerLimb(trial, BodySide::Left, &HJC, &landmarks, sampleRate, startTime))
            continue;
        }
        if ((optr->Side & BodySide::Right) == BodySide::Right)
        {
          RelativePosition* relpos = seg->findChild<RelativePosition*>("R.HJC",{},false);
          if (relpos == nullptr)
          {
            Logger::error("PluginGait - Missing right hip joint center position relative to the pelvis in the model. Motion generation aborted.");
            return false;
          }
          const lard::Trajectory HJC = lard::transform_relative_position(relpos, seg, &pelvis);
          if (!optr->generateLowerLimb(trial, BodySide::Right, &HJC, &landmarks, sampleRate, startTime))
            continue;
        }
      }
      // --------------------------------------------------
      // Other Segment
      // --------------------------------------------------
      auto segments = model->segments()->findChildren<Segment*>();
      for (auto it = segments.cbegin() ; it != segments.cend() ; ++it)
      {
        if ((*it)->part() == BodyPart::Head)
        {
          // -----------------------------------------
          // Head
          // -----------------------------------------
          // Required landmarks: L.HF, L.HB, R.HF and R.HB
          const lard::MappedTrajectory& L_HF = landmarks["L.HF"];
          const lard::MappedTrajectory& L_HB = landmarks["L.HB"];
          const lard::MappedTrajectory& R_HF = landmarks["R.HF"];
          const lard::MappedTrajectory& R_HB = landmarks["R.HB"];
          if (!L_HF.isValid() || !L_HB.isValid() || !R_HF.isValid() || !R_HB.isValid())
          {
            Logger::error("PluginGait - Missing landmarks to define the head. Motion generation aborted for trial '%s'.", trial->name().c_str());
            continue;
          }
          const lard::Vector u = ((L_HF + R_HF) / 2.0 - (L_HB + R_HB) / 2.0).normalized();
          const lard::Vector w = u.cross((L_HF + L_HB) / 2.0 - (R_HF + R_HB) / 2.0).normalized();
          const lard::Vector v = w.cross(u);
          const lard::Vector o = (L_HF + R_HF + L_HB + R_HB) / 4.0;
          // Rotate the head frame along its axis Y using the offset "HeadOffset"
          const double cy = cos(optr->HeadOffset), sy = sin(optr->HeadOffset);
          const lard::Vector ur = u * cy - w * sy;
          const lard::Vector wr = u * sy + w * cy;
          lard::export_timesequence(&ur, &v, &wr, &o, "Head", sampleRate, startTime, trial->timeSequences());
        }
      }
    }
    return true;
  };

  void PluginGait::finalize(btk::Node* /*trials*/) const noexcept
  {
  };
  
  // ----------------------------------------------------------------------- //
  
  double PluginGait::markerDiameter() const noexcept
  {
    auto optr = this->pimpl();
    return optr->MarkerDiameter;
  };
  
  void PluginGait::setMarkerDiameter(double value) noexcept
  {
    auto optr = this->pimpl();
    if (fabs(value - optr->MarkerDiameter) < std::numeric_limits<double>::epsilon())
      return;
    optr->MarkerDiameter = value;
    this->modified();
  };

  double PluginGait::headOffset() const noexcept
  {
    auto optr = this->pimpl();
    return optr->HeadOffset;
  };
  
  void PluginGait::setHeadOffset(double value) noexcept
  {
    auto optr = this->pimpl();
    if (fabs(value - optr->HeadOffset) < std::numeric_limits<double>::epsilon())
      return;
    optr->HeadOffset = value;
    this->modified();
  };
  
  double PluginGait::rightShoulderOffset() const noexcept
  {
    auto optr = this->pimpl();
    return optr->RightShoulderOffset;
  };
  
  void PluginGait::setRightShoulderOffset(double value) noexcept
  {
    auto optr = this->pimpl();
    if (fabs(value - optr->RightShoulderOffset) < std::numeric_limits<double>::epsilon())
      return;
    optr->RightShoulderOffset = value;
    this->modified();
  };
  
  double PluginGait::leftShoulderOffset() const noexcept
  {
    auto optr = this->pimpl();
    return optr->LeftShoulderOffset;
  };
  
  void PluginGait::setLeftShoulderOffset(double value) noexcept
  {
    auto optr = this->pimpl();
    if (fabs(value - optr->LeftShoulderOffset) < std::numeric_limits<double>::epsilon())
      return;
    optr->LeftShoulderOffset = value;
    this->modified();
  };
  
  double PluginGait::rightElbowWidth() const noexcept
  {
    auto optr = this->pimpl();
    return optr->RightElbowWidth;
  };
  
  void PluginGait::setRightElbowWidth(double value) noexcept
  {
    auto optr = this->pimpl();
    if (fabs(value - optr->RightElbowWidth) < std::numeric_limits<double>::epsilon())
      return;
    optr->RightElbowWidth = value;
    this->modified();
  };
  
  double PluginGait::leftElbowWidth() const noexcept
  {
    auto optr = this->pimpl();
    return optr->LeftElbowWidth;
  };
  
  void PluginGait::setLeftElbowWidth(double value) noexcept
  {
    auto optr = this->pimpl();
    if (fabs(value - optr->LeftElbowWidth) < std::numeric_limits<double>::epsilon())
      return;
    optr->LeftElbowWidth = value;
    this->modified();
  };
  
  double PluginGait::rightWristWidth() const noexcept
  {
    auto optr = this->pimpl();
    return optr->RightWristWidth;
  };
  
  void PluginGait::setRightWristWidth(double value) noexcept
  {
    auto optr = this->pimpl();
    if (fabs(value - optr->RightWristWidth) < std::numeric_limits<double>::epsilon())
      return;
    optr->RightWristWidth = value;
    this->modified();
  };
  
  double PluginGait::leftWristWidth() const noexcept
  {
    auto optr = this->pimpl();
    return optr->LeftWristWidth;
  };
  
  void PluginGait::setLeftWristWidth(double value) noexcept
  {
    auto optr = this->pimpl();
    if (fabs(value - optr->LeftWristWidth) < std::numeric_limits<double>::epsilon())
      return;
    optr->LeftWristWidth = value;
    this->modified();
  };
  
  double PluginGait::rightHandThickness() const noexcept
  {
    auto optr = this->pimpl();
    return optr->RightHandThickness;
  };
  
  void PluginGait::setRightHandThickness(double value) noexcept
  {
    auto optr = this->pimpl();
    if (fabs(value - optr->RightHandThickness) < std::numeric_limits<double>::epsilon())
      return;
    optr->RightHandThickness = value;
    this->modified();
  };
  
  double PluginGait::leftHandThickness() const noexcept
  {
    auto optr = this->pimpl();
    return optr->LeftHandThickness;
  };
  
  void PluginGait::setLeftHandThickness(double value) noexcept
  {
    auto optr = this->pimpl();
    if (fabs(value - optr->LeftHandThickness) < std::numeric_limits<double>::epsilon())
      return;
    optr->LeftHandThickness = value;
    this->modified();
  };
  
  double PluginGait::interAsisDistance() const noexcept
  {
    auto optr = this->pimpl();
    return optr->InterAsisDistance;
  };
  
  void PluginGait::setInterAsisDistance(double value) noexcept
  {
    auto optr = this->pimpl();
    if (fabs(value - optr->InterAsisDistance) < std::numeric_limits<double>::epsilon())
      return;
    optr->InterAsisDistance = value;
    this->modified();
  };
  
  double PluginGait::rightLegLength() const noexcept
  {
    auto optr = this->pimpl();
    return optr->RightLegLength;
  };
  
  void PluginGait::setRightLegLength(double value) noexcept
  {
    auto optr = this->pimpl();
    if (fabs(value - optr->RightLegLength) < std::numeric_limits<double>::epsilon())
      return;
    optr->RightLegLength = value;
    this->modified();
  };
  
  double PluginGait::leftLegLength() const noexcept
  {
    auto optr = this->pimpl();
    return optr->LeftLegLength;
  };
  
  void PluginGait::setLeftLegLength(double value) noexcept
  {
    auto optr = this->pimpl();
    if (fabs(value - optr->LeftLegLength) < std::numeric_limits<double>::epsilon())
      return;
    optr->LeftLegLength = value;
    this->modified();
  };
  
  double PluginGait::rightAsisTrochanterAPDistance() const noexcept
  {
    auto optr = this->pimpl();
    return optr->RightAsisTrochanterAPDistance;
  };
  
  void PluginGait::setRightAsisTrochanterAPDistance(double value) noexcept
  {
    auto optr = this->pimpl();
    if (fabs(value - optr->RightAsisTrochanterAPDistance) < std::numeric_limits<double>::epsilon())
      return;
    optr->RightAsisTrochanterAPDistance = value;
    this->modified();
  };
  
  double PluginGait::leftAsisTrochanterAPDistance() const noexcept
  {
    auto optr = this->pimpl();
    return optr->LeftAsisTrochanterAPDistance;
  };
  
  void PluginGait::setLeftAsisTrochanterAPDistance(double value) noexcept
  {
    auto optr = this->pimpl();
    if (fabs(value - optr->LeftAsisTrochanterAPDistance) < std::numeric_limits<double>::epsilon())
      return;
    optr->LeftAsisTrochanterAPDistance = value;
    this->modified();
  };
  
  double PluginGait::rightKneeWidth() const noexcept
  {
    auto optr = this->pimpl();
    return optr->RightKneeWidth;
  };
  
  void PluginGait::setRightKneeWidth(double value) noexcept
  {
    auto optr = this->pimpl();
    if (fabs(value - optr->RightKneeWidth) < std::numeric_limits<double>::epsilon())
      return;
    optr->RightKneeWidth = value;
    this->modified();
  };
  
  double PluginGait::leftKneeWidth() const noexcept
  {
    auto optr = this->pimpl();
    return optr->LeftKneeWidth;
  };
  
  void PluginGait::setLeftKneeWidth(double value) noexcept
  {
    auto optr = this->pimpl();
    if (fabs(value - optr->LeftKneeWidth) < std::numeric_limits<double>::epsilon())
      return;
    optr->LeftKneeWidth = value;
    this->modified();
  };
  
  double PluginGait::rightAnkleWidth() const noexcept
  {
    auto optr = this->pimpl();
    return optr->RightAnkleWidth;
  };
  
  void PluginGait::setRightAnkleWidth(double value) noexcept
  {
    auto optr = this->pimpl();
    if (fabs(value - optr->RightAnkleWidth) < std::numeric_limits<double>::epsilon())
      return;
    optr->RightAnkleWidth = value;
    this->modified();
  };
  
  double PluginGait::leftAnkleWidth() const noexcept
  {
    auto optr = this->pimpl();
    return optr->LeftAnkleWidth;
  };
  
  void PluginGait::setLeftAnkleWidth(double value) noexcept
  {
    auto optr = this->pimpl();
    if (fabs(value - optr->LeftAnkleWidth) < std::numeric_limits<double>::epsilon())
      return;
    optr->LeftAnkleWidth = value;
    this->modified();
  };
  
  bool PluginGait::rightFootFlat() const noexcept
  {
    auto optr = this->pimpl();
    return optr->RightFootFlat;
  };
  
  void PluginGait::setRightFootFlat(bool value) noexcept
  {
    auto optr = this->pimpl();
    if (value == optr->RightFootFlat)
      return;
    optr->RightFootFlat = value;
    this->modified();
  };
  
  bool PluginGait::leftFootFlat() const noexcept
  {
    auto optr = this->pimpl();
    return optr->LeftFootFlat;
  };
  
  void PluginGait::setLeftFootFlat(bool value) noexcept
  {
    auto optr = this->pimpl();
    if (value == optr->LeftFootFlat)
      return;
    optr->LeftFootFlat = value;
    this->modified();
  };
  
  double PluginGait::rightStaticPlantarFlexionOffset() const noexcept
  {
    auto optr = this->pimpl();
    return optr->RightStaticPlantarFlexionOffset;
  };
  
  double PluginGait::rightStaticRotationOffset() const noexcept
  {
    auto optr = this->pimpl();
    return optr->RightStaticRotationOffset;
  };
  
  double PluginGait::leftStaticPlantarFlexionOffset() const noexcept
  {
    auto optr = this->pimpl();
    return optr->LeftStaticPlantarFlexionOffset;
  };
  
  double PluginGait::leftStaticRotationOffset() const noexcept
  {
    auto optr = this->pimpl();
    return optr->LeftStaticRotationOffset;
  };
  
  // ----------------------------------------------------------------------- //
  
  /*
   * Generate a landmarks translator for the PiG model.
   * The following list presents the label used in the PiG model and its translation in BTK:
   *  - LFHD: L.HF   (Left front head: approximately over the left)
   *  - LBHD: L.HB   (Left back head: on the back of the head)
   *  - RFHD: R.HF   (Right front head: approximately over the)
   *  - RBHD: R.HB   (Right back head: on the back of the head)
   *  - C7:   C7     (Seventh cervical vertebra: also named vertebra prominens)
   *  - T10:  T10    (Spinous Process of the 10th thoracic vertebrae)
   *  - CLAV: SS     (Suprasternal notch: superior border of the manubrium of the sternum, also known as the jugular notch)
   *  - STRN: XP     (Xiphoid process: extension to the lower part of the sternum)
   *  - LSHO: L.AC   (Left acromion (Acromio-clavicular joint))
   *  - RSHO: R.AC   (Right acromion (Acromio-clavicular joint))
   *  - LELB: L.LHE  (Left lateral humerus epicondyle)
   *  - LWRB: L.US   (Left ulnar styloid process (wrist bar pinkie)
   *  - LWRA: L.RS   (Left radius styloid process (wrist bar thumb)
   *  - RELB: R.LHE  (Right lateral humerus epicondyle)
   *  - RWRB: R.US   (Right ulnar styloid process)
   *  - RWRA: R.RS   (Right radius styloid process)
   *  - LFIN: L.MH2  (Left head of the second metacarpus)
   *  - RFIN: R.MH2  (Right head of the second metacarpus)
   *  - LASI: L.ASIS (Left Anterior Superior Iliac Spine)
   *  - RASI: R.ASIS (Right Anterior Superior Iliac Spine)
   *  - LPSI: L.PSIS (Left Posterior Superior Iliac Spine)
   *  - RPSI: R.PSIS (Right Posterior Superior Iliac Spine)
   *  - LTHI: L.ITB  (Left Iliotibial Band)
   *  - RTHI: R.ITB  (Right Iliotibial Band)
   *  - LKNE: L.LFE  (Left lateral epicondyle of the femur)
   *  - RKNE: R.LFE  (Right lateral epicondyle of the femur)
   *  - LTIB: L.LS   (Left lateral shank marker)
   *  - RTIB: R.LS   (Right lateral shank marker)
   *  - LANK: L.LTM  (Left lateral malleolus of the tibia)
   *  - RANK: R.LTM  (Right lateral malleolus of the tibia)
   *  - LTOE: L.MTH2 (Head of the second metatarsal for the left)
   *  - RTOE: R.MTH2 (Head of the second metatarsal for the right)
   *  - LHEE: L.HEE  (Left heel marker)
   *  - RHEE: R.HEE  (Right heel marker)
   */
  LandmarksTranslator* PluginGait::defaultLandmarksTranslator()
  {
    auto translator = this->findChild<LandmarksTranslator*>("LandmarksTranslator",{},false);
    if (translator == nullptr)
    {  
      translator = new LandmarksTranslator("LandmarksTranslator",{
        {"LFHD", "L.HF"},
        {"LBHD", "L.HB"},
        {"RFHD", "R.HF"},
        {"RBHD", "R.HB"},
        {"C7"  , "C7"}, // same
        {"T10" , "T10"}, // same
        {"CLAV", "SS"},
        {"STRN", "XP"},
        {"LSHO", "L.AC"},
        {"RSHO", "R.AC"},
        {"LELB", "L.LHE"},
        {"LWRB", "L.US"},
        {"LWRA", "L.RS"},
        {"RELB", "R.LHE"},
        {"RWRB", "R.US"},
        {"RWRA", "R.RS"},
        {"LFIN", "L.MH2"},
        {"RFIN", "R.MH2"},
        {"LASI", "L.ASIS"},
        {"RASI", "R.ASIS"},
        {"LPSI", "L.PSIS"},
        {"RPSI", "R.PSIS"},
        {"SACR", "SC"},
        {"LTHI", "L.ITB"},
        {"RTHI", "R.ITB"},
        {"LKNE", "L.LFE"},
        {"RKNE", "R.LFE"},
        {"LTIB", "L.LS"},
        {"RTIB", "R.LS"},
        {"LANK", "L.LTM"},
        {"RANK", "R.LTM"},
        {"LTOE", "L.MTH2"},
        {"RTOE", "R.MTH2"},
        {"LHEE", "L.HEE"},
        {"RHEE", "R.HEE"},
      },this);
    }
    return translator;
  };
};

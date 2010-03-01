/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2010, Arnaud Barré
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

#include "btkStaticCalibrationProcedure.h"
#include "btkUpperLimbModelConfigurator.h"

#include <Eigen/Array>
#include <Eigen/Geometry>
#include <Eigen/LU>

namespace btk
{
 /**
   * @class StaticCalibrationProcedure
   * @brief TODO
   *
   * @ingroup BTKModels
   */
  
  /**
   * @typedef StaticCalibrationProcedure::Pointer
   * Smart pointer associated with a StaticCalibrationProcedure object.
   */
  
  /**
   * @typedef StaticCalibrationProcedure::ConstPointer
   * Smart pointer associated with a const StaticCalibrationProcedure object.
   */
    
  /**
   * @fn static Pointer StaticCalibrationProcedure::New();
   * Creates a smart pointer associated with a StaticCalibrationProcedure object.
   */
  
 /**
   * @fn StaticCalibrationProcedure::StaticCalibrationProcedure()
   * Constructor.
   */
  
  /**
   * Calibrate the model.
   *
   * @todo Add a long description
   */
  void StaticCalibrationProcedure::Calibrate(Model::Pointer output, Acquisition::Pointer input, Subject::Pointer s, LandmarksTranslator::Pointer lt) const
  {
    Subject::Pointer subject;
    if (!s.get())
    {
      Acquisition::MetaDataConstIterator itSubject = input->GetMetaData()->FindChild("SUBJECT");
      if  (itSubject != input->EndMetaData())
      {
        MetaDataInfo::Pointer sex = (*itSubject)->ExtractChildInfo("SEX", MetaDataInfo::Char, 2);
        MetaDataInfo::Pointer weight = (*itSubject)->ExtractChildInfo("WEIGHT", MetaDataInfo::Real, 1);
        if (sex.get() && weight.get())
        {
          subject = Subject::New();
          subject->SetSex(sex->ToString(0).substr(0,1).compare("F") == 0 ? Subject::Female : Subject::Male);
          subject->SetWeight(weight->ToDouble(0));
        }
      }
    }
    else
      subject = s;
    if (!subject.get())
    {
      btkErrorMacro("No subject's informations. Impossible to initialize the model.");
      return;
    }
    
    // Used landmarks for this model
    std::map<std::string, Position> landmarks;
    landmarks["C7"]; // Seventh cervical vertebra: also named vertebra prominens
    landmarks["T8"]; // Eighth thoracic vertebra: same level as the xiphoid process
    landmarks["SS"]; // Suprasternal notch: superior border of the manubrium of the sternum
    landmarks["XP"]; // Xiphoid process: extension to the lower part of the sternum
    landmarks["AC_L"];
    landmarks["AA_L"];
    landmarks["GT_L"];
    landmarks["LHE_L"];
    landmarks["MHE_L"];
    landmarks["US_L"];
    landmarks["RS_L"];
    landmarks["MH5_L"];
    landmarks["MH2_L"];
    landmarks["AC_R"];
    landmarks["AA_R"];
    landmarks["GT_R"];
    landmarks["LHE_R"];
    landmarks["MHE_R"];
    landmarks["US_R"];
    landmarks["RS_R"];
    landmarks["MH5_R"];
    landmarks["MH2_R"];
    std::string landmark = "";
    std::map<std::string, Position>::iterator itl = landmarks.begin();
    while (itl != landmarks.end())
    {
      if (!lt.get() || (landmark = lt->GetTranslation(itl->first)).empty())
        landmark = itl->first;
      Acquisition::PointConstIterator itP = input->FindPoint(landmark);
      if (itP == input->EndPoint())
        landmarks.erase(itl++);
      else
      {
        itl->second = (*itP)->GetValues().colwise().sum() / input->GetPointFrameNumber();
        //std::cout << std::endl << itl->first << ": " << itl->second.transpose();
        ++itl;
      }
    }
    // Calibration
    Eigen::Matrix<double, 3, 1> temp;
    for (Model::SegmentIterator it = output->BeginSegment() ; it != output->EndSegment() ; ++it)
    {
      Segment::Pointer seg = *it;
      if (seg->GetType() == Segment::Trunk)
      {
        // -----------------------------------------
        // Trunk
        // -----------------------------------------
        // Required landmarks: SS, C7, XP and T8
        Position SS, C7, XP, T8;
        if (!this->FindPosition(&landmarks, "SS", &SS) || !this->FindPosition(&landmarks, "C7", &C7) || !this->FindPosition(&landmarks, "XP", &XP) || !this->FindPosition(&landmarks, "T8", &T8))
        {
          btkErrorMacro("Missing landmarks to define the trunk. Calibration aborted.");
          return;
        }
        Frame& f0 = seg->GetMotion()->operator[](0);        
        // Note: The anatomical frame will be expressed in the technical frame at the end of the computation.
        // CJC: AF Origin
        Frame thorax;
        Frame::Axis tX = (SS - C7).normalized();
        thorax.SetAxisX(tX);
        temp = (SS + C7) / 2.0 - (XP + T8) / 2.0;
        temp = tX.cross(temp); 
        temp.normalize();
        thorax.SetAxisZ(temp);
        thorax.SetAxisY(temp.cross(tX));
        thorax.SetOrigin(C7);
        double depth = (SS - C7).norm();
        double r, percent;
        if (subject->GetSex() == Subject::Female)
        {
          r = 14.0 * M_PI / 180.0; 
          percent = 0.53;
        }
        else
        {
          r = 8.0 * M_PI / 180.0;
          percent = 0.55;
        }
        // The inverse of the matrix rot is known. Not necessary to compute it.
        Eigen::Matrix<double, 4, 4> rotInv = Eigen::Matrix<double, 4, 4>::Identity();
        rotInv.block(0,0,2,2) << cos(r), -sin(r), sin(r), cos(r);
        Eigen::Matrix<double, 4, 1> jc; 
        jc << percent * depth, 0.0, 0.0, 1.0;
        Position CJC = (thorax.GetMatrix() * rotInv * jc).block(0,0,3,1);
        seg->GetRelativeAF().SetOrigin(CJC);
        // AF Y axis
        //temp = CJC - (XP + T8) / 2.0;
        //temp.normalize();
        seg->GetRelativeAF().SetAxisY((CJC - (XP + T8) / 2.0).normalized());
        // AF Z axis
        temp = (SS - C7).cross(seg->GetRelativeAF().GetAxisY());
        temp.normalize();
        seg->GetRelativeAF().SetAxisZ(temp);
        // AF X axis
        seg->GetRelativeAF().SetAxisX(seg->GetRelativeAF().GetAxisY().cross(temp));
        // TF Origin
        f0.SetOrigin(C7);
        // TF Y axis
        //temp = (C7 + SS) / 2.0 - (XP + T8) / 2.0;
        //temp.normalize();
        f0.SetAxisY(((C7 + SS) / 2.0 - (XP + T8) / 2.0).normalized());
        // TF Z axis
        temp = (SS - C7).cross(f0.GetAxisY());
        temp.normalize();
        f0.SetAxisZ(temp);
        // TF X axis
        f0.SetAxisX(f0.GetAxisY().cross(temp));
        // AF expressed in TF
        Frame::Matrix frameInv;
        f0.GetMatrix().computeInverse(&frameInv);
        seg->GetRelativeAF().SetMatrix(frameInv * seg->GetRelativeAF().GetMatrix());
      }
      else if (seg->GetType() == Segment::Arm)
      {
        // -----------------------------------------
        // Arm
        // -----------------------------------------
        // Required landmarks: C7, XP, T8, AA_*, GT_*, MHE_* and LHE_*
        Position SS, C7, XP, T8, AA, GT, MHE, LHE;
        if (!this->FindPosition(&landmarks, "SS", &SS) || !this->FindPosition(&landmarks, "C7", &C7) || !this->FindPosition(&landmarks, "XP", &XP) || !this->FindPosition(&landmarks, "T8", &T8)
            || ((seg->GetSide() == Segment::Central) && true)
            || ((seg->GetSide() == Segment::Left) && (!this->FindPosition(&landmarks, "AA_L", &AA) || !this->FindPosition(&landmarks, "GT_L", &GT) || !this->FindPosition(&landmarks, "MHE_L", &MHE) || !this->FindPosition(&landmarks, "LHE_L", &LHE)))
            || ((seg->GetSide() == Segment::Right) && (!this->FindPosition(&landmarks, "AA_R", &AA) || !this->FindPosition(&landmarks, "GT_R", &GT) || !this->FindPosition(&landmarks, "MHE_R", &MHE) || !this->FindPosition(&landmarks, "LHE_R", &LHE))))
        {
          btkErrorMacro("Missing landmarks to define the arm or unusable segment's side. Calibration aborted.");
          return;
        }
        Frame& f0 = seg->GetMotion()->operator[](0);
        // SJC: AF Origin
        Frame thorax;
        Frame::Axis tX = (SS - C7).normalized();
        thorax.SetAxisX(tX);
        temp = (SS + C7) / 2.0 - (XP + T8) / 2.0;
        temp = tX.cross(temp); 
        temp.normalize();
        thorax.SetAxisZ(temp);
        thorax.SetAxisY(temp.cross(tX));
        thorax.SetOrigin(AA);
        double depth = (SS - C7).norm();
        double r, percent;
        //std::cout << std::endl << thorax << std::endl;
        if (subject->GetSex() == Subject::Female)
        {
          r = -5.0 * M_PI / 180.0; 
          percent = 0.53;
        }
        else
        {
          r = -11.0 * M_PI / 180.0;
          percent = 0.43;
        }
        Eigen::Matrix<double, 4, 4> rotInv = Eigen::Matrix<double, 4, 4>::Identity();
        rotInv.block(0,0,2,2) << cos(r), -sin(r), sin(r), cos(r);
        Eigen::Matrix<double, 4, 1> jc; 
        jc << percent * depth, 0.0, 0.0, 1.0;
        Position SJC = (thorax.GetMatrix() * rotInv * jc).block(0,0,3,1);
        seg->GetRelativeAF().SetOrigin(SJC);
        // TF Y axis
        //temp = GT - (LHE + MHE) / 2.0;
        //temp.normalize();
        f0.SetAxisY((GT - (LHE + MHE) / 2.0).normalized());
        // TF X axis
        if (seg->GetSide() == Segment::Left)
          temp = f0.GetAxisY().cross(MHE - LHE);
        else
          temp = f0.GetAxisY().cross(LHE - MHE);
        temp.normalize();
        f0.SetAxisX(temp);
        // TF Z axis
        f0.SetAxisZ(f0.GetAxisX().cross(f0.GetAxisY()));
        // TF Origin
        f0.SetOrigin(GT);
        // AF Y axis
        //temp = SJC - (LHE + MHE) / 2.0;
        //temp.normalize();
        seg->GetRelativeAF().SetAxisY((SJC - (LHE + MHE) / 2.0).normalized());
        // AF X axis
        if (seg->GetSide() == Segment::Left)
          temp = seg->GetRelativeAF().GetAxisY().cross(MHE - LHE);
        else
          temp = seg->GetRelativeAF().GetAxisY().cross(LHE - MHE);
        temp.normalize();
        seg->GetRelativeAF().SetAxisX(temp);
        // AF Z axis
        seg->GetRelativeAF().SetAxisZ(seg->GetRelativeAF().GetAxisX().cross(seg->GetRelativeAF().GetAxisY()));
        // AF expressed in TF
        Frame::Matrix frameInv;
        f0.GetMatrix().computeInverse(&frameInv);
        seg->GetRelativeAF().SetMatrix(frameInv * seg->GetRelativeAF().GetMatrix());
      }
      else if (seg->GetType() == Segment::Forearm)
      {
        // -----------------------------------------
        // Forearm
        // -----------------------------------------
        // Required landmarks: US_*, RS_*, MHE_* and LHE_*
        Position US, RS, MHE, LHE;
        if (((seg->GetSide() == Segment::Central) && true)
            || ((seg->GetSide() == Segment::Left) && (!this->FindPosition(&landmarks, "US_L", &US) || !this->FindPosition(&landmarks, "RS_L", &RS) || !this->FindPosition(&landmarks, "MHE_L", &MHE) || !this->FindPosition(&landmarks, "LHE_L", &LHE)))
            || ((seg->GetSide() == Segment::Right) && (!this->FindPosition(&landmarks, "US_R", &US) || !this->FindPosition(&landmarks, "RS_R", &RS) || !this->FindPosition(&landmarks, "MHE_R", &MHE) || !this->FindPosition(&landmarks, "LHE_R", &LHE))))
        {
          btkErrorMacro("Missing landmarks to define the forearm or unusable segment's side. Calibration aborted.");
          return;
        }
        Frame& f0 = seg->GetMotion()->operator[](0);
        // EJC: TF Origin
        Position EJC = (LHE + MHE) / 2.0 ;
        f0.SetOrigin(EJC);
        // TF Y axis
        //temp = (LHE + MHE) / 2.0 - (US + RS) / 2.0;
        //temp.normalize();
        f0.SetAxisY(((LHE + MHE) / 2.0 - (US + RS) / 2.0).normalized());
        // TF X axis
        if (seg->GetSide() == Segment::Left)
          temp = f0.GetAxisY().cross(US - RS);
        else
          temp = f0.GetAxisY().cross(RS - US);
        temp.normalize();
        f0.SetAxisX(temp);
        // TF Z axis
        f0.SetAxisZ(f0.GetAxisX().cross(f0.GetAxisY()));
      }
      else if (seg->GetType() == Segment::Hand)
      {
        // -----------------------------------------
        // Hand
        // -----------------------------------------
        bool fourMarkersConfig = true;
        // Required landmarks: US_*, RS_*, MH2_* and/or MH5_*
        Position US, RS, MH2, MH5, MH;
        if (((seg->GetSide() == Segment::Central) && true)
            || ((seg->GetSide() == Segment::Left) && (!this->FindPosition(&landmarks, "US_L", &US) || !this->FindPosition(&landmarks, "RS_L", &RS)))
            || ((seg->GetSide() == Segment::Right) && (!this->FindPosition(&landmarks, "US_R", &US) || !this->FindPosition(&landmarks, "RS_R", &RS))))
        {
          btkErrorMacro("Missing landmarks to define the hand or unusable segment's side. Calibration aborted.");
          return;
        }
        else if (((seg->GetSide() == Segment::Left) && this->FindPosition(&landmarks, "MH2_L", &MH2) && !this->FindPosition(&landmarks, "MH5_L", &MH5))
                 || ((seg->GetSide() == Segment::Right) && this->FindPosition(&landmarks, "MH2_R", &MH2) && !this->FindPosition(&landmarks, "MH5_R", &MH5)))
        {
          fourMarkersConfig = false;
          MH = MH2;
        }
        else if (((seg->GetSide() == Segment::Left) && !this->FindPosition(&landmarks, "MH2_L", &MH2) && this->FindPosition(&landmarks, "MH5_L", &MH5))
                 || ((seg->GetSide() == Segment::Right) && !this->FindPosition(&landmarks, "MH2_R", &MH2) && this->FindPosition(&landmarks, "MH5_R", &MH5)))
        {
          fourMarkersConfig = false;
          MH = MH5;
        }
        else if (((seg->GetSide() == Segment::Left) && (!this->FindPosition(&landmarks, "MH2_L", &MH2) || !this->FindPosition(&landmarks, "MH5_L", &MH5)))
                || ((seg->GetSide() == Segment::Right) && (!this->FindPosition(&landmarks, "MH2_R", &MH2) || !this->FindPosition(&landmarks, "MH5_R", &MH5))))
        {
           btkErrorMacro("Missing landmarks to define the hand or unusable segment's side. Calibration aborted.");
           return;
        }
        Frame& f0 = seg->GetMotion()->operator[](0);
        // WJC: TF Origin
        Position WJC = (US + RS) / 2.0;
        f0.SetOrigin(WJC);
        if (fourMarkersConfig)
        {
          // TF Y axis
          f0.SetAxisY(((US + RS) / 2.0 - (MH2 + MH5) / 2.0).normalized());
          // TF X axis
          if (seg->GetSide() == Segment::Left)
            temp = f0.GetAxisY().cross(MH5 - MH2);
          else
            temp = f0.GetAxisY().cross(MH2 - MH5);
          temp.normalize();
          f0.SetAxisX(temp);
        }
        else
        {
          btkErrorMacro("Hand orientation matrix is computed with only one marker distally. Be sure it is located in the middle of the hand.");
          // TF Y axis
          f0.SetAxisY(((US + RS) / 2.0 - MH).normalized());
          // TF X axis
          temp = f0.GetAxisY().cross(RS - US);
          temp.normalize();
          f0.SetAxisX(temp);
        }
        // TF Z axis
        f0.SetAxisZ(f0.GetAxisX().cross(f0.GetAxisY()));
      }
    }
  };
  
  bool StaticCalibrationProcedure::FindPosition(std::map<std::string, Position>* landmarks, const std::string& label, Position* coords) const
  {
    std::map<std::string, Position>::const_iterator it = landmarks->find(label);
    if (it == landmarks->end())
      return false;
    else
      *coords = it->second;
    return true;
  };
};  
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

#include "btkBasicMotionBuilder.h"
#include <Eigen/Array>

namespace btk
{
  /**
   * @class BasicMotionBuilder
   * @brief Build segments' motion by using the same segment defintion that define in the class StaticCalibrationProcedure.
   *
   * @ingroup BTKModels
   */

  /**
   * @typedef BasicMotionBuilder::Pointer
   * Smart pointer associated with a BasicMotionBuilder object.
   */

  /**
   * @typedef BasicMotionBuilder::ConstPointer
   * Smart pointer associated with a const BasicMotionBuilder object.
   */
   
  /**
   * @fn BasicMotionBuilder::Pointer BasicMotionBuilder::New()
   * Creates a smart pointer associated with a BasicMotionBuilder object.
   */
   
  /**
   * Build segment's motion using markers informations and basic relations between them.
   */
  void BasicMotionBuilder::Build(Model::Pointer output, Acquisition::Pointer input, LandmarksTranslator::Pointer lt) const
  {
    // Used landmarks for this model
    std::map<std::string, Point::Values*> landmarks;
    landmarks["C7"];
    landmarks["T8"];
    landmarks["SS"];
    landmarks["XP"];
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
    std::map<std::string, Point::Values*>::iterator itl = landmarks.begin();
    while (itl != landmarks.end())
    {
      if (!lt.get() || (landmark = lt->GetTranslation(itl->first)).empty())
        landmark = itl->first;
      Acquisition::PointConstIterator itP = input->FindPoint(landmark);
      if (itP == input->EndPoint())
        landmarks.erase(itl++);
      else
      {
        itl->second = &((*itP)->GetValues());
        ++itl;
      }
    }
    // Frame number
    int fn = input->GetPointFrameNumber();
    // Motion
    Point::Values Y, temp;
    for (Model::SegmentIterator it = output->BeginSegment() ; it != output->EndSegment() ; ++it)
    {
      Segment::Pointer seg = *it;
      if (seg->GetType() == Segment::Trunk)
      {
        // -----------------------------------------
        // Trunk
        // -----------------------------------------
        // Required landmarks: SS, C7, XP and T8
        Point::Values *SS, *C7, *XP, *T8;
        if (!this->FindValues(&landmarks, "SS", &SS) || !this->FindValues(&landmarks, "C7", &C7) || !this->FindValues(&landmarks, "XP", &XP) || !this->FindValues(&landmarks, "T8", &T8))
        {
          btkErrorMacro("Missing landmarks to define trunk's motion. Frame builder aborted.");
          return;
        }
        if ((SS->rows() != fn) || (C7->rows() != fn) || (XP->rows() != fn) || (T8->rows() != fn))
        {
          btkErrorMacro("Incompatible number of frames for markers. Frame builder aborted.");
          return;
        }
        seg->SetMotionFrameNumber(fn);
        temp = (*C7 + *SS) / 2.0 - (*XP + *T8) / 2.0;
        Y = temp.cwise() / (temp.rowwise().norm() * Eigen::Matrix<double, 1,3>::Ones());
        
        temp = (*SS - *C7);
        Position z;
        Motion::Pointer mot = seg->GetMotion();
        for (int i = 0 ; i < fn ; ++i)
        {
          z.coeffRef(0) = temp.coeff(i,1) * Y.coeff(i,2) - temp.coeff(i,2) * Y.coeff(i,1);
          z.coeffRef(1) = temp.coeff(i,2) * Y.coeff(i,0) - temp.coeff(i,0) * Y.coeff(i,2);
          z.coeffRef(2) = temp.coeff(i,0) * Y.coeff(i,1) - temp.coeff(i,1) * Y.coeff(i,0);
          z.normalize();
          
          mot->operator[](i).SetAxisX(Y.coeff(i,1) * z.coeff(2) - Y.coeff(i,2) * z.coeff(1),
                                      Y.coeff(i,2) * z.coeff(0) - Y.coeff(i,0) * z.coeff(2),
                                      Y.coeff(i,0) * z.coeff(1) - Y.coeff(i,1) * z.coeff(0));
          mot->operator[](i).SetAxisY(Y.coeff(i,0), Y.coeff(i,1), Y.coeff(i,2));
          mot->operator[](i).SetAxisZ(z);
          mot->operator[](i).SetOrigin(C7->coeff(i,0), C7->coeff(i,1), C7->coeff(i,2));
        }
      }
      else if (seg->GetType() == Segment::Arm)
      {
        // -----------------------------------------
        // Arm
        // -----------------------------------------
        // Required landmarks: GT_*, MHE_* and LHE_*
        Point::Values *GT, *MHE, *LHE;
        if (((seg->GetSide() == Segment::Central) && true)
            || ((seg->GetSide() == Segment::Left) && (!this->FindValues(&landmarks, "GT_L", &GT) || !this->FindValues(&landmarks, "MHE_L", &MHE) || !this->FindValues(&landmarks, "LHE_L", &LHE)))
            || ((seg->GetSide() == Segment::Right) && (!this->FindValues(&landmarks, "GT_R", &GT) || !this->FindValues(&landmarks, "MHE_R", &MHE) || !this->FindValues(&landmarks, "LHE_R", &LHE))))
        {
          btkErrorMacro("Missing landmarks to define arm's motion or unusable segment's side. Frame builder aborted.");
          return;
        }
        if ((GT->rows() != fn) || (MHE->rows() != fn) || (LHE->rows() != fn))
        {
          btkErrorMacro("Incompatible number of frames for markers. Frame builder aborted.");
          return;
        }
        seg->SetMotionFrameNumber(fn);
        temp = *GT - (*LHE + *MHE) / 2.0;
        Y = temp.cwise() / (temp.rowwise().norm() * Eigen::Matrix<double, 1,3>::Ones());
        if (seg->GetSide() == Segment::Left)
          temp = *MHE - *LHE;
        else
          temp = *LHE - *MHE;
        Position x;
        Motion::Pointer mot = seg->GetMotion();
        for (int i = 0 ; i < fn ; ++i)
        {
          x.coeffRef(0) = Y.coeff(i,1) * temp.coeff(i,2) - Y.coeff(i,2) * temp.coeff(i,1);
          x.coeffRef(1) = Y.coeff(i,2) * temp.coeff(i,0) - Y.coeff(i,0) * temp.coeff(i,2);
          x.coeffRef(2) = Y.coeff(i,0) * temp.coeff(i,1) - Y.coeff(i,1) * temp.coeff(i,0);
          x.normalize();
          
          mot->operator[](i).SetAxisX(x);
          mot->operator[](i).SetAxisY(Y.coeff(i,0), Y.coeff(i,1), Y.coeff(i,2));
          mot->operator[](i).SetAxisZ(x.coeff(1) * Y.coeff(i,2) - x.coeff(2) * Y.coeff(i,1),
                                      x.coeff(2) * Y.coeff(i,0) - x.coeff(0) * Y.coeff(i,2),
                                      x.coeff(0) * Y.coeff(i,1) - x.coeff(1) * Y.coeff(i,0));
          mot->operator[](i).SetOrigin(GT->coeff(i,0), GT->coeff(i,1), GT->coeff(i,2));
        }
      }
      else if (seg->GetType() == Segment::Forearm)
      {
        // -----------------------------------------
        // Forearm
        // -----------------------------------------
        // Required landmarks: US_*, RS_*, MHE_* and LHE_*
        Point::Values *US, *RS, *MHE, *LHE;
        if (((seg->GetSide() == Segment::Central) && true)
            || ((seg->GetSide() == Segment::Left) && (!this->FindValues(&landmarks, "US_L", &US) || !this->FindValues(&landmarks, "RS_L", &RS) || !this->FindValues(&landmarks, "MHE_L", &MHE) || !this->FindValues(&landmarks, "LHE_L", &LHE)))
            || ((seg->GetSide() == Segment::Right) && (!this->FindValues(&landmarks, "US_R", &US) || !this->FindValues(&landmarks, "RS_R", &RS) || !this->FindValues(&landmarks, "MHE_R", &MHE) || !this->FindValues(&landmarks, "LHE_R", &LHE))))
        {
          btkErrorMacro("Missing landmarks to define forearm's motion or unusable segment's side. Frame builder aborted.");
          return;
        }
        if ((US->rows() != fn) || (RS->rows() != fn) || (MHE->rows() != fn) || (LHE->rows() != fn))
        {
          btkErrorMacro("Incompatible number of frames for markers. Frame builder aborted.");
          return;
        }
        seg->SetMotionFrameNumber(fn);
        Point::Values EJC = (*LHE + *MHE) / 2.0;
        temp = EJC - (*US + *RS) / 2.0;
        Y = temp.cwise() / (temp.rowwise().norm() * Eigen::Matrix<double, 1,3>::Ones());
        if (seg->GetSide() == Segment::Left)
          temp = *US - *RS;
        else
          temp = *RS - *US;
        Position x;
        Motion::Pointer mot = seg->GetMotion();
        for (int i = 0 ; i < fn ; ++i)
        {
          x.coeffRef(0) = Y.coeff(i,1) * temp.coeff(i,2) - Y.coeff(i,2) * temp.coeff(i,1);
          x.coeffRef(1) = Y.coeff(i,2) * temp.coeff(i,0) - Y.coeff(i,0) * temp.coeff(i,2);
          x.coeffRef(2) = Y.coeff(i,0) * temp.coeff(i,1) - Y.coeff(i,1) * temp.coeff(i,0);
          x.normalize();
          
          mot->operator[](i).SetAxisX(x);
          mot->operator[](i).SetAxisY(Y.coeff(i,0), Y.coeff(i,1), Y.coeff(i,2));
          mot->operator[](i).SetAxisZ(x.coeff(1) * Y.coeff(i,2) - x.coeff(2) * Y.coeff(i,1),
                                      x.coeff(2) * Y.coeff(i,0) - x.coeff(0) * Y.coeff(i,2),
                                      x.coeff(0) * Y.coeff(i,1) - x.coeff(1) * Y.coeff(i,0));
          mot->operator[](i).SetOrigin(EJC.coeff(i,0), EJC.coeff(i,1), EJC.coeff(i,2));
        }
      }
      else if (seg->GetType() == Segment::Hand)
      {
        // -----------------------------------------
        // Hand
        // -----------------------------------------
        bool fourMarkersConfig = true;
        // Required landmarks: US_*, RS_*, MH2_* and MH5_*
        Point::Values *US, *RS, *MH2, *MH5, *MH;
        if (((seg->GetSide() == Segment::Central) && true)
            || ((seg->GetSide() == Segment::Left) && (!this->FindValues(&landmarks, "US_L", &US) || !this->FindValues(&landmarks, "RS_L", &RS)))
            || ((seg->GetSide() == Segment::Right) && (!this->FindValues(&landmarks, "US_R", &US) || !this->FindValues(&landmarks, "RS_R", &RS))))
        {
          btkErrorMacro("Missing landmarks to define hand's motion or unusable segment's side. Frame builder aborted.");
          return;
        }
        else if (((seg->GetSide() == Segment::Left) && this->FindValues(&landmarks, "MH2_L", &MH2) && !this->FindValues(&landmarks, "MH5_L", &MH5))
                 || ((seg->GetSide() == Segment::Right) && this->FindValues(&landmarks, "MH2_R", &MH2) && !this->FindValues(&landmarks, "MH5_R", &MH5)))
        {
          fourMarkersConfig = false;
          MH = MH2;
          MH5 = MH; // Only for the row size test.
        }
        else if (((seg->GetSide() == Segment::Left) && !this->FindValues(&landmarks, "MH2_L", &MH2) && this->FindValues(&landmarks, "MH5_L", &MH5))
                 || ((seg->GetSide() == Segment::Right) && !this->FindValues(&landmarks, "MH2_R", &MH2) && this->FindValues(&landmarks, "MH5_R", &MH5)))
        {
          fourMarkersConfig = false;
          MH = MH5;
          MH2 = MH; // Only for the row size test
        }
        else if (((seg->GetSide() == Segment::Left) && (!this->FindValues(&landmarks, "MH2_L", &MH2) || !this->FindValues(&landmarks, "MH5_L", &MH5)))
                || ((seg->GetSide() == Segment::Right) && (!this->FindValues(&landmarks, "MH2_R", &MH2) || !this->FindValues(&landmarks, "MH5_R", &MH5))))
        {
           btkErrorMacro("Missing landmarks to define the hand or unusable segment's side. Calibration aborted.");
           return;
        }
        if ((US->rows() != fn) || (RS->rows() != fn) || (MH2->rows() != fn) || (MH5->rows() != fn))
        {
          btkErrorMacro("Incompatible number of frames for markers. Frame builder aborted.");
          return;
        }
        seg->SetMotionFrameNumber(fn);
        Point::Values WJC = (*US + *RS) / 2.0;
        if (fourMarkersConfig)
        {
          temp = WJC - (*MH2 + *MH5) / 2.0;
          Y = temp.cwise() / (temp.rowwise().norm() * Eigen::Matrix<double, 1,3>::Ones());
          if (seg->GetSide() == Segment::Left)
            temp = *MH5 - *MH2;
          else
            temp = *MH2 - *MH5;
        }
        else
        {
          btkErrorMacro("Hand orientation matrix is computed with only one marker distally. Be sure it is located in the middle of the hand.");
          temp = WJC - *MH;
          Y = temp.cwise() / (temp.rowwise().norm() * Eigen::Matrix<double, 1,3>::Ones());
          temp = *RS - *US;
        }
        Position x;
        Motion::Pointer mot = seg->GetMotion();
        for (int i = 0 ; i < fn ; ++i)
        {
          x.coeffRef(0) = Y.coeff(i,1) * temp.coeff(i,2) - Y.coeff(i,2) * temp.coeff(i,1);
          x.coeffRef(1) = Y.coeff(i,2) * temp.coeff(i,0) - Y.coeff(i,0) * temp.coeff(i,2);
          x.coeffRef(2) = Y.coeff(i,0) * temp.coeff(i,1) - Y.coeff(i,1) * temp.coeff(i,0);
          x.normalize();
          
          mot->operator[](i).SetAxisX(x);
          mot->operator[](i).SetAxisY(Y.coeff(i,0), Y.coeff(i,1), Y.coeff(i,2));
          mot->operator[](i).SetAxisZ(x.coeff(1) * Y.coeff(i,2) - x.coeff(2) * Y.coeff(i,1),
                                      x.coeff(2) * Y.coeff(i,0) - x.coeff(0) * Y.coeff(i,2),
                                      x.coeff(0) * Y.coeff(i,1) - x.coeff(1) * Y.coeff(i,0));
          mot->operator[](i).SetOrigin(WJC.coeff(i,0), WJC.coeff(i,1), WJC.coeff(i,2));
        }
      }
      // Set the final number of frames for the model (only affect the model's frame number as segments were already set to this number of frames)
      output->SetMotionFrameNumber(fn);
    }
  };
  
  bool BasicMotionBuilder::FindValues(std::map<std::string, Point::Values*>* landmarks, const std::string& label, Point::Values** values) const
  {
    std::map<std::string, Point::Values*>::const_iterator it = landmarks->find(label);
    if (it == landmarks->end())
      return false;
    else
      *values = it->second;
    return true;
  };
};

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
 * LIABILITY, OR TORT (INCLUDING 0NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "btkUpperLimbModelConfigurator.h"
#include "btkModel.h"

namespace btk
{ 
  /**
   * @typedef UpperLimbModelConfigurator
   * Model configuration for the upper limb.
   */
  
  template class ModelConfiguratorTemplate<ModelConfigurator::UpperLimb>; // MUST be here to resolve LNK2001 error in MSVC
  
  /**
   * Specialization for the modeling of the upper part of the body with 1 trunk, 1 left arm, forearm, hand and 1 right arm, forearm, hand.
   */
  template <> void UpperLimbModelConfigurator::Configure(Model* model, Side s)
  {
    if (s == None)
    {
      btkErrorMacro("The side 'None' cannot be used for the upper limb model. Model configuration aborted.");
      return;
    }
    Segment::Pointer trunk = Segment::New(Segment::Trunk, Segment::Central, "Trunk", "");
    model->AppendSegment(trunk);
    if ((s == Left) || (s == Both))
    {
      Segment::Pointer leftArm = Segment::New(Segment::Arm, Segment::Left, "LArm", "");
      Segment::Pointer leftForearm = Segment::New(Segment::Forearm, Segment::Left, "LForearm", "");
      Segment::Pointer leftHand = Segment::New(Segment::Hand, Segment::Left, "LHand", "");
      model->AppendSegment(leftArm);
      model->AppendSegment(leftForearm);
      model->AppendSegment(leftHand);
      model->AppendJoint(trunk, leftArm, "LShoulder", "");
      model->AppendJoint(leftArm, leftForearm, "LElbow", "");
      model->AppendJoint(leftForearm, leftHand, "LWrist", "");
      /*
      model->AppendJoint(Joint::Shoulder, Joint::Left, trunk, leftArm, "LShoulder", "");
      model->AppendJoint(Joint::Elbow, Joint::Left, leftArm, leftForearm, "LElbow", "");
      model->AppendJoint(Joint::Wrist, Joint::Left, leftForearm, leftHand, "LWrist");
      */
    }
    if ((s == Right) || (s == Both))
    {
      Segment::Pointer rightArm = Segment::New(Segment::Arm, Segment::Right, "RArm", "");
      Segment::Pointer rightForearm = Segment::New(Segment::Forearm, Segment::Right, "RForearm", "");
      Segment::Pointer rightHand = Segment::New(Segment::Hand, Segment::Right, "RHand", "");
      model->AppendSegment(rightArm);
      model->AppendSegment(rightForearm);
      model->AppendSegment(rightHand);
      /*
      model->AppendJoint(Joint::Shoulder, Joint::Right, trunk, rightArm, "RShoulder", "");
      model->AppendJoint(Joint::Elbow, Joint::Right, rightArm, rightForearm, "Elbow_R", "");
      model->AppendJoint(Joint::Wrist, Joint::Right, rightForearm, rightHand, "RWrist", "");
      */
      model->AppendJoint(trunk, rightArm, "RShoulder", "");
      model->AppendJoint(rightArm, rightForearm, "RElbow", "");
      model->AppendJoint(rightForearm, rightHand, "RWrist", "");
    }
    //Segment::Pointer global = Segment::New(Segment::Global, Segment::Central, "Global", "Global frame");
    //model->AppendSegment(global);
    model->AppendJoint(Segment::Pointer(), trunk, "Thorax", "Trunk relative to global frame");
  };
};
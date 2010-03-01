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

#include "btkModelJCSKinematicFilter.h"

#include <Eigen/Geometry> // M_PI for MSVC

namespace btk
{
 /**
   * @class ModelJCSKinematicFilter
   * @brief Compute relative kinematic (angles and translations) using the joint coordinate system (JCS) proposed by Grood & Suntay (1983).
   *
   * By default, the translations are not calculated. Use the method SetTranslationsCalculationState() to enable the calculation.
   *
   * This filter gives you the possibility to choose the axes sequence for each joint. For this, you have to use the method SetSequenceJCS().
   * For example:
   * @code
   * // model, arm and forearm already defined
   * btk::Joint::Pointer jnt = btk::Joint::New(forearm, hand, "Wrist");
   * model->AppendJoint(jnt);
   * btk::ModelJCSKinematicFilter::Pointer mjkf = btk::ModelJCSKinematicFilter::New();
   * mjkf->SetInput(model);
   * mjkf->SetSequenceJCS("Wrist", btk::AxisZ, btk::AxisY, btk::AxisX); // Wrist kinematic using the sequence Z-Y'-X''
   * btk::Acquisition::Pointer output = mjkf->GetOutput();
   * output->Update();
   * // Wrist angles are in the point labeled WristAngle.
   * @endcode
   *
   * The default sequence is Z-X'-Y'', then you don't have to set the sequence for each joint (if you want to calculate the kinematic with the sequence is Z-X'-Y'').
   *
   * Moreover, even, if it is advised to give a unique label for each joint, you can append the same joint many times
   * and look differences in the kinematics when choosing different sequences. For example:
   * @code
   * // model, arm and forearm already defined
   * btk::Joint::Pointer jnt = btk::Joint::New(arm, forearm, "Elbow");
   * model->AppendJoint(jnt);
   * model->AppendJoint(jnt); // Added a second time
   * btk::ModelJCSKinematicFilter::Pointer mjkf = btk::ModelJCSKinematicFilter::New();
   * mjkf->SetInput(model);
   * mjkf->SetSequenceJCS(0, btk::AxisZ, btk::AxisX, btk::AxisY); // Elbow kinematic using the sequence Z-X'-Y''
   * mjkf->SetSequenceJCS(1, btk::AxisZ, btk::AxisYE, btk::AxisX); // Second elbow kinematic using the sequence Z-Y'-X''
   * btk::Acquisition::Pointer output = mjkf->GetOutput();
   * output->Update();
   * // Angles can be compared by comparing values in the points "ElbowAngles" and "ElbowAngles2".
   * @endcode
   *
   * As presented previously in the code, you can set the sequence by using the joint's label or its index in the model.
   * If more than one joint use the same label, then all these joints will use the given sequence.
   * Sequences given by index have priority on sequences given by label. Then if you give for the same joint a sequnce by using the joint's label
   * and the join't index used in the model, the final selected sequence will be the second.
   *
   * @par Reference
   * Grood, E.S., & Suntay, W.J.@n
   * <em>A joint coordinate system for the clinical description of three-dimensional motions: Application to the knee.</em>@n
   * Journal of Biomechanical Engeering, 105, <b>1983</b>, 136-144.@n
   * @n
   * Ge Wu, Peter R. Cavanagh@n
   * <em>ISB recommendations for standardization in the reporting of kinematic data.</em>@n
   * Journal of Biomechanics, 28 (10), <b>1995</b>, 1257-1261.@n
   * @n
   * Ge Wu, Sorin Siegler, Paul Allard, Chris Kirtley, Alberto Leardini, Dieter Rosenbaum, Mike Whittle, Darryl D. D’Lima, Luca Cristofolini, Hartmut Witte, Oskar Schmid, Ian Stokes.@n
   * <em>ISB recommendation on definitions of joint coordinate system of various joints for the reporting of human joint motion—part I: ankle, hip, and spine</em>
   * Journal of Biomechanics, 35 (4), <b>2002</b>, 543-548@n
   * @n
   * Ge Wu, Frans C.T. van der Helm, H.E.J. (DirkJan) Veeger, Mohsen Makhsous, Peter Van Roy, Carolyn Anglin, Jochem Nagels, Andrew R. Karduna, Kevin McQuade, Xuguang Wang, Frederick W. Werner, Bryan Buchholz.@n
   * <em>ISB recommendation on definitions of joint coordinate systems of various joints for the reporting of human joint motion—Part II: shoulder, elbow, wrist and hand</em>.
   * Journal of Biomechanics, 38 (5), <b>2005</b>, 981-992
   *
   * @ingroup BTKModels
   */
   
  /**
   * @struct SequenceJCS
   * @brief Structure to easily set the axes used in a JCS sequence.
   */
  /**
   * @enum SequenceJCS::Axis
   * Enums used to specify the order of the axes to calculate the JCS kinematics.
   * A sequence is composed of three axes (e0, e1, e2). The first (e0) comes from the proximal segment.
   * The third (e2) comes from the distal segment. The second (e1, also know as the floating axis)
   * comes from the cross product of the third axis with the first axis (e2 x e0).
   *
   * For exemple the sequence Z-X'-Y'' means that the first axis correspond to the 
   * Z axis of the proximal segment. The thrid axis (Y'') comes from the distal segment.
   * Finaly, the second axis (X') comes from the cross product of Y'' with Z.
   */
  /**
   * @var SequenceJCS::Axis SequenceJCS::AxisX
   * Corresponds to the axis X of a segmental frame.
   */
  /**
   * @var SequenceJCS::Axis SequenceJCS::AxisY
   * Corresponds to the axis Y of a segmental frame.
   */
  /**
   * @var SequenceJCS::Axis SequenceJCS::AxisZ
   * Corresponds to the axis Z of a segmental frame.
   */
  /**
   * @var SequenceJCS::e0
   * Correspond to the axis e0 in the JCS sequence.
   */
  /**
   * @var SequenceJCS::e1
   * Correspond to the axis e1 in the JCS sequence.
   */
  /**
   * @var SequenceJCS::e2
   * Correspond to the axis e2 in the JCS sequence.
   */
  
  /**
   * @fn static SequenceJCS SequenceJCS::Default()
   * Returns the sequence Z-X'-Y''.
   */
   
  /**
   * Equal operator.
   */
  bool operator==(const SequenceJCS& lhs, const SequenceJCS& rhs)
  {
    if ((lhs.e0 != rhs.e0) || (lhs.e1 != rhs.e1) || (lhs.e2 != rhs.e2))
      return false;
    return true;
  };
  
  /**
   * @fn friend bool SequenceJCS::operator!= (const SequenceJCS& lhs, const SequenceJCS& rhs)
   * Inequal operator
   */
  
  /**
   * @typedef ModelJCSKinematicFilter::Pointer
   * Smart pointer associated with a ModelJCSKinematicFilter object.
   */
  
  /**
   * @typedef ModelJCSKinematicFilter::ConstPointer
   * Smart pointer associated with a const ModelJCSKinematicFilter object.
   */
    
  /**
   * @fn static Pointer ModelJCSKinematicFilter::New();
   * Creates a smart pointer associated with a ModelJCSKinematicFilter object.
   */
   
  /**
   * @fn void ModelJCSKinematicFilter::SetInput(Model::Pointer input)
   * Sets the model to calculate the kinematics.
   */
  
  /**
   * @fn Model::Pointer ModelJCSKinematicFilter::GetInput()
   * Returns the model to used to calculate the kinematics.
   */
  
  /**
   * @fn Acquisition::Pointer ModelJCSKinematicFilter::GetOutput()
   * Gets the output created with this process.
   */
   
  /**
   * Returns the sequence for the first joint using the label @a label.
   * If no exlicit sequence was defined, then the default sequence is returned: Z-X'-Y''.
   */
  const SequenceJCS ModelJCSKinematicFilter::GetSequenceJCS(const std::string& label) const
  {
    for (std::map<std::string, SequenceJCS>::const_iterator it = this->m_SequencesByLabel.begin() ; it != this->m_SequencesByLabel.end() ; ++it)
    {
      if (it->first.compare(label) == 0)
        return it->second;
    }
    return SequenceJCS::Default();
  };
  
  /**
   * Returns the sequence assigned to the @a idx th joint in the model.
   */
  const SequenceJCS ModelJCSKinematicFilter::GetSequenceJCS(int idx) const
  {
    for (std::map<int, SequenceJCS>::const_iterator it = this->m_SequencesByIndex.begin() ; it != this->m_SequencesByIndex.end() ; ++it)
    {
      if (it->first == idx)
        return it->second;
    }
    return SequenceJCS::Default();
  };
  
  /**
   * Set all the joints with the label @a label to the sequence @a axes.
   */
  void ModelJCSKinematicFilter::SetSequenceJCS(const std::string& label, const SequenceJCS& axes)
  {
    std::map<std::string, SequenceJCS>::const_iterator it = this->m_SequencesByLabel.find(label);
    if ((it != this->m_SequencesByLabel.end()) && (it->second == axes))
      return;
    this->m_SequencesByLabel[label] = axes;
    this->Modified();
  };
  
  /**
   * Sets the sequence for the joint at the index @a idx in the model to the value @a axes.
   */
  void ModelJCSKinematicFilter::SetSequenceJCS(int idx, const SequenceJCS& axes)
  {
    std::map<int, SequenceJCS>::const_iterator it = this->m_SequencesByIndex.find(idx);
    if ((it != this->m_SequencesByIndex.end()) && (it->second == axes))
      return;
    this->m_SequencesByIndex[idx] = axes;
    this->Modified();
  };
  
  /**
   * Set all the joints with the labe @a label to the sequence composed of the axis @a e0, @a e1, @a e2.
   */
  void ModelJCSKinematicFilter::SetSequenceJCS(const std::string& label, SequenceJCS::Axis e0, SequenceJCS::Axis e1, SequenceJCS::Axis e2)
  {
    SequenceJCS seq;
    seq.e0 = e0;
    seq.e1 = e1;
    seq.e2 = e2;
    this->SetSequenceJCS(label, seq);
  };
  
  /**
   * Sets the sequence for the joint at the index @a idx in the model to @a e0, @a e1, @a e2.
   */
  void ModelJCSKinematicFilter::SetSequenceJCS(int idx, SequenceJCS::Axis e0, SequenceJCS::Axis e1, SequenceJCS::Axis e2)
  {
    SequenceJCS seq;
    seq.e0 = e0;
    seq.e1 = e1;
    seq.e2 = e2;
    this->SetSequenceJCS(idx, seq);
  };
  
  /**
   * Clear all defined sequences.
   */
  void ModelJCSKinematicFilter::ClearSequences()
  {
    if (!this->m_SequencesByIndex.empty())
    {
      this->m_SequencesByIndex.clear();
      this->Modified();
    }
    if (!this->m_SequencesByLabel.empty())
    {
      this->m_SequencesByLabel.clear();
      this->Modified();
    }
  };
  
  /**
   * @fn bool ModelJCSKinematicFilter::GetTranslationsCalculationState() const
   * Gets the state of the flag "TranslationsCalculation".
   * If the returned boolean value is equal to true, then the joints' translations will be computed.
   */
  
  /**
   * Sets the state of the flag "TranslationsCalculation" to true or false to compute or not the joints' translations
   */
  void ModelJCSKinematicFilter::SetTranslationsCalculationState(bool enable)
  {
    if (this->m_TranslationsEnabled == enable)
      return;
    this->m_TranslationsEnabled = enable;
    this->Modified();
  }
  
  /**
   * Constructor.
   */
  ModelJCSKinematicFilter::ModelJCSKinematicFilter()
  : ProcessObject(), m_SequencesByIndex(), m_SequencesByLabel()
  {
    this->SetInputNumber(1);
    this->SetOutputNumber(1);
    this->m_TranslationsEnabled = false;
  };
  
  /**
   * @fn Acquisition::Pointer ModelJCSKinematicFilter::GetOutput(int idx)
   * Returns the output at the index @a idx.
   */
  
  /**
   * Creates a Model::Pointer object and return it as a DataObject::Pointer.
   */
  DataObject::Pointer ModelJCSKinematicFilter::MakeOutput(int /* idx */)
  {
    return Acquisition::New();
  };
  
  /**
   * Generates the outputs' data.
   * Computes kinematics by using the default sequence Z-Y'-X''
   */
  void ModelJCSKinematicFilter::GenerateData()
  {
    Model::Pointer input = this->GetInput();
    Acquisition::Pointer output = this->GetOutput();
    
    output->Reset();
    if (input.get())
    {
      Acquisition::PointIterator itAngle, itTranslation;
      if (this->m_TranslationsEnabled)
      {
        output->Init(input->GetJointNumber() * 2, input->GetMotionFrameNumber());
        itAngle = output->BeginPoint();
        itTranslation = output->BeginPoint();
        std::advance(itTranslation, input->GetJointNumber());
      }
      else
      { 
        output->Init(input->GetJointNumber(), input->GetMotionFrameNumber());
        itAngle = output->BeginPoint();
      }
      // Check segment's information and create JCS sequences.
      std::map<int, SequenceJCS> sequences;
      SequenceJCS defaultSeq = SequenceJCS::Default();
      int inc = 0;
      for (Model::JointConstIterator it = input->BeginJoint() ; it != input->EndJoint() ; ++it)
      {
        //std::cout << std::endl << input->GetMotionFrameNumber() << " vs " << (*it)->GetProximalSegment()->GetMotionFrameNumber() << " ; " << (*it)->GetDistalSegment()->GetMotionFrameNumber() << std::endl;
        if ((*it)->GetProximalSegment().get() && ((*it)->GetProximalSegment()->GetMotionFrameNumber() != input->GetMotionFrameNumber())
            || ((*it)->GetDistalSegment().get() && ((*it)->GetDistalSegment()->GetMotionFrameNumber() != input->GetMotionFrameNumber())))
        {
          btkErrorMacro("Incompatible number of frames between segment(s) and the model. Kinematic calculation aborted.");
          return;
        }
        std::map<int, SequenceJCS>::const_iterator itSeq = m_SequencesByIndex.find(inc);
        if (itSeq != m_SequencesByIndex.end())
          sequences[inc] = itSeq->second;
        else
          sequences[inc] = this->GetSequenceJCS((*it)->GetLabel());
        ++inc;
      }
      // Calculate kinematics
      inc = 0;
      for (Model::JointConstIterator it = input->BeginJoint() ; it != input->EndJoint() ; ++it)
      {
         (*itAngle)->SetLabel((*it)->GetLabel() + "Angles");
         (*itAngle)->SetType(Point::Angle);
        // Check if the joint use the global frame as segment.
        AbstractMotion::Pointer proximal;
        AbstractMotion::Pointer distal;
        // Proximal
        if (!(*it)->GetProximalSegment().get())
          proximal = NoMotion::New();
        else
          proximal = (*it)->GetProximalSegment()->GetAFMotion();
        // Distal
        if (!(*it)->GetDistalSegment().get())
          distal = NoMotion::New();
        else
          distal = (*it)->GetDistalSegment()->GetAFMotion();
        // ANGLES
        // ------
        AbstractRotToEuler* rte = RotToEulerFactory(sequences[inc]);
        for (int i = 0 ; i < output->GetPointFrameNumber() ; ++i)
        {
          rte->Extract(proximal->operator[](i).GetMatrix().block(0,0,3,3).transpose() * distal->operator[](i).GetMatrix().block(0,0,3,3), 
                      (*itAngle)->GetValues().coeffRef(i,0), 
                      (*itAngle)->GetValues().coeffRef(i,1), 
                      (*itAngle)->GetValues().coeffRef(i,2));
        }
        (*itAngle)->GetValues() *= 180.0 / M_PI;
        delete rte;
        ++itAngle;
        // TRANSLATIONS
        // ------------
        if (this->m_TranslationsEnabled)
        {
          (*itTranslation)->SetLabel((*it)->GetLabel() + "Translations");
          ++itTranslation;
        }
        ++inc;
      }
    }
  };
};
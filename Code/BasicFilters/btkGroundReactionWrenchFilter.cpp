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

#include "btkGroundReactionWrenchFilter.h"
#include "btkConvert.h"

namespace btk
{
  /**
   * @class GroundReactionWrenchFilter
   * @brief Transform force platform data into ground reaction wrenches.
   *
   * Theses wrenches are expressed in the global frame. The point of application (PWA)
   * of each wrench is calculated from Shimba (1984),
   *
   * @par Reference
   * Shimba T.@n
   * <em>An estimation of center of gravity from force platform data</em>.@n
   * Journal of Biomechanics, <b>1984</b>, 17(1), 53–60.
   *
   * @ingroup BTKBasicFilters
   */
  
  /**
   * @typedef GroundReactionWrenchFilter::Pointer
   * Smart pointer associated with a GroundReactionWrenchFilter object.
   */
  
  /**
   * @typedef GroundReactionWrenchFilter::ConstPointer
   * Smart pointer associated with a const GroundReactionWrenchFilter object.
   */
    
  /**
   * @fn static Pointer GroundReactionWrenchFilter::New();
   * Creates a smart pointer associated with a GroundReactionWrenchFilter object.
   */

  /**
   * @fn bool GroundReactionWrenchFilter::GetThresholdState() const
   * Returns the state of the threshold used to suppress false PWA.
   */

  /**
   * Sets the threshold state.
   */
  void GroundReactionWrenchFilter::SetThresholdState(bool activated)
  {
    if (this->m_ThresholdActivated == activated)
      return;
    this->m_ThresholdActivated = activated;
    this->Modified();
  };

  /**
   * @fn double GroundReactionWrenchFilter::GetThresholdValue() const
   * Returns the value used to suppress PWA computed with a Fz value lower or equal than it.
   * 
   * The threshold must be activated (see GroundReactionWrenchFilter::SetThresholdState) to be used during the computation of the PWA.
   */
  
  /**
   * Sets the threshold value.
   *
   * The threshold must be activated (see GroundReactionWrenchFilter::SetThresholdState) to be used during the computation of the PWA.
   */
  void GroundReactionWrenchFilter::SetThresholdValue(double v)
  {
    if (fabs(this->m_ThresholdValue - v) <= std::numeric_limits<double>::epsilon())
      return;
    if (v < 0.0)
      btkErrorMacro("Negative threshold has no effect on the algorithm because it compares the threshold value with the absolute value of Fz.");
    this->m_ThresholdValue = v;
    this->Modified();
  };

  /**
   * @fn WrenchCollection::Pointer GroundReactionWrenchFilter::GetInput()
   * Gets the input registered with this process.
   */

  /**
   * @fn void GroundReactionWrenchFilter::SetInput(ForcePlatform::Pointer input)
   * Sets the input required with this process. This input is transformed in a collection a force platform with a single force platform.
   */
  
  /**
   * @fn void GroundReactionWrenchFilter::SetInput(ForcePlatformCollection::Pointer input)
   * Sets the input required with this process.
   */
  
  /**
   * @fn PointCollection::Pointer GroundReactionWrenchFilter::GetOutput()
   * Gets the output created with this process.
   */

  /**
   * Constructor. Sets the number of inputs and outputs to 1.
   */
  GroundReactionWrenchFilter::GroundReactionWrenchFilter()
  : ProcessObject()
  {
    this->SetInputNumber(1);
    this->SetOutputNumber(1);
    this->m_ThresholdActivated = false;
    this->m_ThresholdValue = 0.0;
  };

  /**
   * @fn ForcePlatformCollection::Pointer GroundReactionWrenchFilter::GetInput(int idx)
   * Returns the input at the index @a idx.
   */
  
  /**
   * @fn WrenchCollection::Pointer GroundReactionWrenchFilter::GetOutput(int idx)
   * Returns the output at the index @a idx.
   */
  
  /**
   * Creates a WrenchCollection:Pointer object and return it as a DataObject::Pointer.
   */
  DataObject::Pointer GroundReactionWrenchFilter::MakeOutput(int /* idx */)
  {
    return WrenchCollection::New();
  };
  
  /**
   * Generates the outputs' data.
   */
  void GroundReactionWrenchFilter::GenerateData()
  {
    WrenchCollection::Pointer output = this->GetOutput();
    output->Clear();
    ForcePlatformCollection::Pointer input = this->GetInput();
    if (input.get() != 0)
    {
      int inc = 0;
      for (ForcePlatformCollection::ConstIterator it = input->Begin() ; it != input->End() ; ++it)
      {
        ++inc;
        if ((*it)->GetChannelNumber() == 0)
        {
          btkErrorMacro("Unexpected number of analog channels (0) for force platfom #" + ToString(inc));
          continue;
        }
        int frameNumber = (*it)->GetChannel(0)->GetFrameNumber();
        Wrench::Pointer grw = Wrench::New("GRW" + ToString(inc), frameNumber);
        output->InsertItem(grw);
        ForcePlatform::Origin origin;
        // Residuals & masks
        grw->GetPosition()->GetMasks().setZero(frameNumber);
        grw->GetPosition()->GetResiduals().setZero(frameNumber);        
        grw->GetForce()->GetMasks().setZero(frameNumber);
        grw->GetForce()->GetResiduals().setZero(frameNumber);
        grw->GetMoment()->GetMasks().setZero(frameNumber);
        grw->GetMoment()->GetResiduals().setZero(frameNumber);
        // Values
        switch((*it)->GetType())
        {
          // 6 channels
          case 1:
            grw->GetForce()->GetValues().col(0) = (*it)->GetChannel(0)->GetValues();
            grw->GetForce()->GetValues().col(1) = (*it)->GetChannel(1)->GetValues();
            grw->GetForce()->GetValues().col(2) = (*it)->GetChannel(2)->GetValues();
            grw->GetPosition()->GetValues().col(0) = (*it)->GetChannel(3)->GetValues();
            grw->GetPosition()->GetValues().col(1) = (*it)->GetChannel(4)->GetValues();
            grw->GetPosition()->GetValues().col(2).setZero();
            grw->GetMoment()->GetValues().col(0).setZero();
            grw->GetMoment()->GetValues().col(1).setZero();
            grw->GetMoment()->GetValues().col(2) = (*it)->GetChannel(5)->GetValues();
            origin << 0, 0, (*it)->GetOrigin().z();
            if (origin.z()  > 0)
            {
              btkErrorMacro("Vertical offset between the origin of the force platform #" + ToString(inc) + " and the center of the working surface seems to be misconfigured (positive value). The opposite of this offset is used.");
              origin.z() *= -1;
            }
            //this->FinishGRWComputation(grw, origin);
            this->TransformGRWToGlobal(grw, (*it)->GetCorners());
            break;
          case 2:
          case 4:
          case 5:
            grw->GetForce()->GetValues().col(0) = (*it)->GetChannel(0)->GetValues();
            grw->GetForce()->GetValues().col(1) = (*it)->GetChannel(1)->GetValues();
            grw->GetForce()->GetValues().col(2) = (*it)->GetChannel(2)->GetValues();
            grw->GetMoment()->GetValues().col(0) = (*it)->GetChannel(3)->GetValues();
            grw->GetMoment()->GetValues().col(1) = (*it)->GetChannel(4)->GetValues();
            grw->GetMoment()->GetValues().col(2) = (*it)->GetChannel(5)->GetValues();
            origin = (*it)->GetOrigin();
            if (origin.z() > 0)
            {
              btkErrorMacro("Origin for the force platform #" + ToString(inc) + " seems to be located from the center of the working surface instead of the inverse. Data are inverted to locate the center of the working surface from the platform's origin.");
              origin *= -1;
            }
            this->FinishGRWComputation(grw, origin);
            this->TransformGRWToGlobal(grw, (*it)->GetCorners());
            break;
          case 3:
            // Fx
            grw->GetForce()->GetValues().col(0) = (*it)->GetChannel(0)->GetValues() + (*it)->GetChannel(1)->GetValues();
            // Fy
            grw->GetForce()->GetValues().col(1) = (*it)->GetChannel(2)->GetValues() + (*it)->GetChannel(3)->GetValues();
            // Fz
            grw->GetForce()->GetValues().col(2) = (*it)->GetChannel(4)->GetValues() + (*it)->GetChannel(5)->GetValues() + (*it)->GetChannel(6)->GetValues() + (*it)->GetChannel(7)->GetValues();
            // Mx
            grw->GetMoment()->GetValues().col(0) = (*it)->GetOrigin().y() * ((*it)->GetChannel(4)->GetValues() + (*it)->GetChannel(5)->GetValues() - (*it)->GetChannel(6)->GetValues() - (*it)->GetChannel(7)->GetValues());
            // My
            grw->GetMoment()->GetValues().col(1) = (*it)->GetOrigin().x() * ((*it)->GetChannel(5)->GetValues() + (*it)->GetChannel(6)->GetValues() - (*it)->GetChannel(4)->GetValues() - (*it)->GetChannel(7)->GetValues());
            // Mz
            grw->GetMoment()->GetValues().col(2) = (*it)->GetOrigin().y() * ((*it)->GetChannel(1)->GetValues() - (*it)->GetChannel(0)->GetValues()) + (*it)->GetOrigin().x() * ((*it)->GetChannel(2)->GetValues() - (*it)->GetChannel(3)->GetValues());
            origin << 0, 0, (*it)->GetOrigin().z();
            if (origin.z()  > 0)
            {
              btkErrorMacro("Vertical offset between the origin of the force platform #" + ToString(inc) + " and the center of the working surface seems to be misconfigured (positive value). The opposite of this offset is used.");
              origin.z() *= -1;
            }
            this->FinishGRWComputation(grw, origin);
            this->TransformGRWToGlobal(grw, (*it)->GetCorners());
            break;
          case 6:
            btkErrorMacro("Force Platform type 6 is not yet supported. Please, report this to the developers");
            break;
          case 7:
            btkErrorMacro("Force Platform type 7 is not yet supported. Please, report this to the developers");
            break;
          case 11:
            btkErrorMacro("Force Platform type 11 is not yet supported. Please, report this to the developers");
            break;
          case 12:
            btkErrorMacro("Force Platform type 12 is not yet supported. Please, report this to the developers");
            break;
          case 21:
            btkErrorMacro("Force Platform type 21 is not yet supported. Please, report this to the developers");
            break;
        }
      }
    }
  };  
};


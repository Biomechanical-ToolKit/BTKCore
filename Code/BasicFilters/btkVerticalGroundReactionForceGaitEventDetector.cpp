/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2014, Arnaud Barré
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

#include "btkVerticalGroundReactionForceGaitEventDetector.h"

namespace btk
{
  /**
   * @class VerticalGroundReactionForceGaitEventDetector
   * @brief Detect heel strike and toe-off events during gait from vertical ground reaction wrench.
   *
   * To fill exactly the detected events, you need to give some extra information to this filter:
   *  - The value of the first frame of the acquisition where the force platform data were extracted (to be added to the detected frames' index).
   *  - The acquisition's frequency used by the force platform data (to calculate the time related to the event's frame).
   *  - The subject's label (optional).
   * All these informations have to be given to the method SetAcquisitionInformation().
   *
   * To detect the heel strike and toe-off events you can set some options:
   *  - The threshold value used to known when an event occurred (see SetThresholdValue()).
   *  - The mapping between the force plates and the side (left, right, general)  of the events detected (see SetForceplateContextMapping()).
   *  - The region of interest where to detect the events (see SetRegionOfInterest()).
   *
   * The algorithm works as following: Based on the region of interest, the maximum is searched. If the maximum is higher than the threshold set, then the frame of the value on the left side of this maximum lower than the threshold is used to create a heel strike event. On the other hand, the value on the right side of the maximum lower than the threshold is used to create a toe-off event.
   *
   * @note: The design of this class is not perfect as it cannot be used in a pipeline without 
   * to update the part before to know some acquisition's information (first frame, sample frequency, subject's name).
   * This class (or the pipeline mechanism) could be modified in a future version of BTK to make up this problem.
   *
   * @ingroup BTKBasicFilters
   */
  
  /**
   * @typedef VerticalGroundReactionForceGaitEventDetector::Pointer
   * Smart pointer associated with a VerticalGroundReactionForceGaitEventDetector object.
   */
  
  /**
   * @typedef VerticalGroundReactionForceGaitEventDetector::ConstPointer
   * Smart pointer associated with a const VerticalGroundReactionForceGaitEventDetector object.
   */
    
  /**
   * @fn static Pointer VerticalGroundReactionForceGaitEventDetector::New();
   * Creates a smart pointer associated with a VerticalGroundReactionForceGaitEventDetector object.
   */

  /**
   * @fn EventCollection::Pointer VerticalGroundReactionForceGaitEventDetector::GetInput()
   * Gets the input registered with this process.
   */

  /**
   * @fn void VerticalGroundReactionForceGaitEventDetector::SetInput(Wrench::Pointer input)
   * Sets the input required with this process. This input is transformed in a collection wrenches with a single force platform.
   */
  
  /**
   * @fn void VerticalGroundReactionForceGaitEventDetector::SetInput(WrenchCollection::Pointer input)
   * Sets the input required with this process.
   */
  
  /**
   * @fn EventCollection::Pointer VerticalGroundReactionForceGaitEventDetector::GetOutput()
   * Gets the output created with this process.
   */
  
  /**
   * Sets the threshold used to detect gait events.
   */
  void VerticalGroundReactionForceGaitEventDetector::SetThresholdValue(int threshold)
  {
    if (this->m_Threshold == threshold)
      return;
    this->m_Threshold = threshold;
    this->Modified();
  };
  
  /**
   * @fn int VerticalGroundReactionForceGaitEventDetector::GetThresholdValue() const
   * Returns the threshold used to detect gait events.
   */
  
  /**
   * Sets the mapping between the given wrenches and the side of the detected events. If no mapping is given, then all the detected events will be set as "General" events.
   */
  void VerticalGroundReactionForceGaitEventDetector::SetForceplateContextMapping(const std::vector<std::string>& mapping)
  {
    if (this->m_ContextMapping == mapping)
      return;
    this->m_ContextMapping = mapping;
    this->Modified();
  };
  
  /**
   * @fn const std::vector<std::string>& VerticalGroundReactionForceGaitEventDetector::GetForceplateContextMapping() const
   * Returns the mapping between the wrenches in the input and the gait events to detect.
   */
  
  /**
   * Sets the region of interest to use to detect gait events.
   * @warning The boundaries must be set using zero-based indices.
   */
  void VerticalGroundReactionForceGaitEventDetector::SetRegionOfInterest(int lb, int ub)
  {
    if ((this->mp_ROI[0] == lb) && (this->mp_ROI[1] == ub))
      return;
    this->mp_ROI[0] = lb;
    this->mp_ROI[1] = ub;
    this->Modified();
  };
  
  /**
   * @fn const int* VerticalGroundReactionForceGaitEventDetector::GetRegionOfInterest() const;
   * Returns the region of interest to use to detect gait events.
   */
  
  /**
   * Set the informations required to set correctly the detected events.
   */
  void VerticalGroundReactionForceGaitEventDetector::SetAcquisitionInformation(int firstFrame, double freq, const std::string& subjectName)
  {
    if ((this->m_FirstFrame == firstFrame) && (this->m_FrameRate == freq) && (this->m_SubjectName.compare(subjectName) == 0))
      return;
    this->m_FirstFrame = firstFrame;
    this->m_FrameRate = freq;
    this->m_SubjectName = subjectName;
    this->Modified();
  };
  
  /**
   * Returns the informations required to set correctly the detected events.
   */
  void VerticalGroundReactionForceGaitEventDetector::GetAcquisitionInformation(int& firstFrame, double& freq, std::string& subjectName)
  {
    firstFrame = this->m_FirstFrame;
    freq = this->m_FrameRate;
    subjectName = this->m_SubjectName;
  };
  
  /**
   * Constructor.
   * By default, the treshold is set to 10 newtons and the algoritm search events on all the frames.
   */
  VerticalGroundReactionForceGaitEventDetector::VerticalGroundReactionForceGaitEventDetector()
  : ProcessObject(), m_ContextMapping()
  {
    this->SetInputNumber(1);
    this->SetOutputNumber(1);
    this->m_Threshold = 10; // newtons
    this->mp_ROI[0] = -1; this->mp_ROI[1] = -1;
    this->m_FirstFrame = 1;
    this->m_FrameRate = 0.0; // Hz
    this->m_SubjectName = "";
  };
  
  /**
   * @fn Acquisition::Pointer VerticalGroundReactionForceGaitEventDetector::GetInput(int idx)
   * Returns the input at the index @a idx.
   */
  
  /**
   * @fn Acquisition::Pointer VerticalGroundReactionForceGaitEventDetector::GetOutput(int idx)
   * Returns the output at the index @a idx.
   */

  /**
   * Generate the output used by this filter.
   */
  DataObject::Pointer VerticalGroundReactionForceGaitEventDetector::MakeOutput(int /* idx */)
  {
    return EventCollection::New();
  };
  
  /**
   * Algorithm used to detect events during the gait.
   */
  void VerticalGroundReactionForceGaitEventDetector::GenerateData()
  {
    WrenchCollection::Pointer input = this->GetInput();
    if (!input)
    {
      btkErrorMacro("Input data are missing.");
      return;
    }
    
    EventCollection::Pointer output = this->GetOutput();
    output->Clear();
    
    std::vector<std::string> mapping = this->m_ContextMapping;
    int num = input->GetItemNumber();
    if (static_cast<int>(mapping.size()) < num)
    {
      btkWarningMacro("The mapping between the force plates and the events' context is not complete. The missing are automatically set to the 'General' context.");
      mapping.resize(num, "General");
    }
    int inc = 0;
    bool warningNegativeLowerBoundDisplayed = false, warningExceededLowerBoundDisplayed = false,
         warningNegativeUpperBoundDisplayed = false, warningExceededUpperBoundDisplayed = false;
    for (WrenchCollection::ConstIterator it = input->Begin() ; it != input->End() ; ++it)
    {
      int frameNumber = (*it)->GetForce()->GetFrameNumber();
      // Set the boudaries inside the loop in case of a wrench has not the same size than the other
      int lb = 0;
      int ub = frameNumber-1;
      if (this->mp_ROI[0] != -1)
      {
        if ((this->mp_ROI[0] < 0) && (!warningNegativeLowerBoundDisplayed))
        {
          btkWarningMacro("The lower bound for the frames of interest was set to a negative value and is then reset to 0.");
          warningNegativeLowerBoundDisplayed = true;
        }
        else if ((this->mp_ROI[0] >= frameNumber) && (!warningExceededLowerBoundDisplayed))
        {
          btkWarningMacro("The lower bound for the frames of interest was set to a value exceeding the number of rows and is reset to 0.");
          warningExceededLowerBoundDisplayed = true;
        }
        else
          lb = this->mp_ROI[0];
      }
      if (this->mp_ROI[1] != -1)
      {
        if ((this->mp_ROI[1] < 0) && (!warningNegativeUpperBoundDisplayed))
        {
          btkWarningMacro("The upper bound for the frames of interest was set to a negative value and is then reset to the greater index available.");
          warningNegativeUpperBoundDisplayed = true;
        }
        else if ((this->mp_ROI[1] >= frameNumber) && (!warningExceededUpperBoundDisplayed))
        {
          btkWarningMacro("The upper bound for the frames of interest was set to a value exceeding the number of rows and is reset to the greater index available.");
          warningExceededUpperBoundDisplayed = true;
        }
        else
          ub = this->mp_ROI[1];
      }
      // Extract the maximum
      double t = 0.0;
      if (this->m_FrameRate >= 0.0)
        t = 1.0 / this->m_FrameRate;
      int r = 0, c = 0, num = ub-lb;
      Eigen::Matrix<double,Eigen::Dynamic,1> fz = (*it)->GetForce()->GetValues().col(2).block(lb,0,num+1,1);
      if (fz.maxCoeff(&r, &c) > this->m_Threshold)
      {
        int incr = r;
        // Heel Strike
        while (incr >= 0)
        {
          if (fz.coeffRef(incr) < this->m_Threshold)
          {
            int frame = incr+lb+this->m_FirstFrame; // No need to remove 1 as 'incr' starts from 0
            output->InsertItem(btk::Event::New("Foot Strike", frame*t, frame, mapping[inc], Event::Automatic | Event::FromForcePlatform, this->m_SubjectName, "The instant the heel strikes the ground", 1));
            break;
          }
          --incr;
        }
        // Toe Off
        incr = r;
        while (incr <= num)
        {
          if (fz.coeffRef(incr) < this->m_Threshold)
          {
            int frame = incr+lb+this->m_FirstFrame;
            output->InsertItem(btk::Event::New("Foot Off", frame*t, frame, mapping[inc], Event::Automatic | Event::FromForcePlatform, this->m_SubjectName, "The instant the toe leaves the ground", 2));
            break;
          }
          ++incr;
        }
      }
      ++inc;
    }
  };
}
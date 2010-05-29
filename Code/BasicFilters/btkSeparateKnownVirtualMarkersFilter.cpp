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

#include "btkSeparateKnownVirtualMarkersFilter.h"

namespace btk
{
 /**
   * @class SeparateKnownVirtualMarkersFilter
   * @brief Separate a collection of points in fours categories to be able to distinguish real markers from the others.
   * These categories are:
   *  -# markers
   *  -# virtual markers used for frame axes
   *  -# other virtual markers (CenterOfMass, ...)
   *  -# other points (angle, force, moment, power, ...)
   *
   * By default, the list of labels known as virtual markers used for frame axes is:
   *  - HED(O|A|L|P): HEDO, HEDA, HEDL, HELP
   *  - LCL(O|A|L|P)
   *  - LFE(O|A|L|P)
   *  - LFO(O|A|L|P)
   *  - LHN(O|A|L|P)
   *  - LHU(O|A|L|P)
   *  - LRA(O|A|L|P)
   *  - LTI(O|A|L|P)
   *  - LTO(O|A|L|P)
   *  - PEL(O|A|L|P)
   *  - RCL(O|A|L|P)
   *  - RFE(O|A|L|P)
   *  - RFO(O|A|L|P)
   *  - RHN(O|A|L|P)
   *  - RHU(O|A|L|P)
   *  - RRA(O|A|L|P)
   *  - RTI(O|A|L|P)
   *  - RTO(O|A|L|P)
   *  - TRX(O|A|L|P)
   *
   * By default, the list of labels known as other virtual markers is:
   *  - CenterOfMass
   *  - CenterOfMassFloor
   *
   * This filter only copies the pointer associated which each point instead of using a deep copy of it.
   *
   * @ingroup BTKBasicFilters
   */
  
  /**
   * @typedef SeparateKnownVirtualMarkersFilter::Pointer
   * Smart pointer associated with a SeparateKnownVirtualMarkersFilter object.
   */
  
  /**
   * @typedef SeparateKnownVirtualMarkersFilter::ConstPointer
   * Smart pointer associated with a const SeparateKnownVirtualMarkersFilter object.
   */
    
  /**
   * @fn static Pointer SeparateKnownVirtualMarkersFilter::New();
   * Creates a smart pointer associated with a SeparateKnownVirtualMarkersFilter object.
   */

  /**
   * @fn PointCollection::Pointer SeparateKnownVirtualMarkersFilter::GetInput()
   * Gets the input registered with this process.
   */
  
  /**
   * @fn void SeparateKnownVirtualMarkersFilter::SetInput(PointCollection::Pointer input)
   * Sets the input required with this process.
   */
  
  /**
   * @fn PointCollection::Pointer SeparateKnownVirtualMarkersFilter::GetOutput(int idx)
   * Returns the output at the index @a idx.
   */
   
  /**
   * @fn void SeparateKnownVirtualMarkersFilter::AppendKnownVirtualMarkerLabelForAxes(const std::string& label)
   * Appends a label to the list of virtual markers used for the frame axes.
   */
  
  /**
   * @fn void SeparateKnownVirtualMarkersFilter::AppendKnownVirtualMarkerLabelsForAxes(const std::list<std::string>& labels)
   * Appends a list of labels to the list of virtual markers used for the frame axes.
   */
   
  /**
   * @fn void SeparateKnownVirtualMarkersFilter::SetKnownVirtualMarkerLabelsForAxes(const std::list<std::string>& labels)
   * Sets the list of labels for the virtual markers used for the frame axes.
   */
   
  /**
   * @fn const std::list<std::string>& SeparateKnownVirtualMarkersFilter::GetKnownVirtualMarkerLabelsForAxes() const
   * Returns the list of labels for the virtual markers used for the frame axes.
   */
   
  /**
   * @fn void SeparateKnownVirtualMarkersFilter::AppendKnownVirtualMarkerLabelForOthers(const std::string& label)
   * Append a label to the list of virtual markers used in another context than frame axes.
   */
   
  /**
   * @fn void SeparateKnownVirtualMarkersFilter::AppendKnownVirtualMarkerLabelsForOthers(const std::list<std::string>& labels)
   * Append a list of labels to the list of virtual markers used in another context than frame axes.
   */
   
  /**
   * @fn void SeparateKnownVirtualMarkersFilter::SetKnownVirtualMarkerLabelsForOthers(const std::list<std::string>& labels)
   * Sets the list of labels for the virtual markers used in another context than frame axes.
   */
   
  /**
   * @fn const std::list<std::string>& SeparateKnownVirtualMarkersFilter::GetKnownVirtualMarkerLabelsForOthers() const
   * Returns the list of labels for the virtual markers used in another context than frame axes
   */
  
  /**
   * Constructor. Sets the number of inputs to 1 and outputs to 4.
   */
  SeparateKnownVirtualMarkersFilter::SeparateKnownVirtualMarkersFilter()
  : ProcessObject(), m_VirtualMarkerLabelsAxes(), m_VirtualMarkerLabelsOthers()
  {
    this->SetInputNumber(1);
    this->SetOutputNumber(4);
    
    // virtual markers for frame axes
    const char* labels[] = {"HED", "LCL", "LFE", "LFO", "LHN", "LHU", "LRA", "LTI", "LTO", "PEL", "RCL", "RFE", "RFO", "RHN", "RHU", "RRA", "RTI", "RTO", "TRX"};
    size_t ln = sizeof(labels) / sizeof(char*);
    for (size_t i = 0 ; i < ln ; ++i)
    {
      std::string label = std::string(labels[i]);
      this->m_VirtualMarkerLabelsAxes.push_back(label + "O");
      this->m_VirtualMarkerLabelsAxes.push_back(label + "A");
      this->m_VirtualMarkerLabelsAxes.push_back(label + "L");
      this->m_VirtualMarkerLabelsAxes.push_back(label + "P");
    }
    
    // others virtual markers
    this->m_VirtualMarkerLabelsOthers.push_back("CentreOfMass");
    this->m_VirtualMarkerLabelsOthers.push_back("CentreOfMassFloor");
    
  };

  /**
   * @fn Acquisition::Pointer SeparateKnownVirtualMarkersFilter::GetInput(int idx)
   * Returns the input at the index @a idx.
   */
  
  /**
   * Creates a PointCollection:Pointer object and return it as a DataObject::Pointer.
   */
  DataObject::Pointer SeparateKnownVirtualMarkersFilter::MakeOutput(int /* idx */)
  {
    return PointCollection::New();
  };
  
  /**
   * Generates the outputs' data.
   */
  void SeparateKnownVirtualMarkersFilter::GenerateData()
  {
    PointCollection::Pointer output[4];
    output[0] = this->GetOutput(0); // markers
    output[1] = this->GetOutput(1); // virtual markers used for frames axes.
    output[2] = this->GetOutput(2); // other virtual markers
    output[3] = this->GetOutput(3); // other points
    output[0]->Clear();
    output[1]->Clear();
    output[2]->Clear();
    output[3]->Clear();
    
    PointCollection::Pointer input = this->GetInput();
    if (input.get() != 0)
    {
      for(PointCollection::Iterator it = input->Begin() ; it != input->End() ; ++it)
      {
        if ((*it)->GetType() != Point::Marker)
          output[3]->InsertItem(*it);
        else
        {
          // Known virtual markers' label used in frame axes.
          if (this->CheckLabel(this->m_VirtualMarkerLabelsAxes, (*it)->GetLabel()))
            output[1]->InsertItem(*it);
          // Known virtual markers' label used in other context.
          else if (this->CheckLabel(this->m_VirtualMarkerLabelsOthers, (*it)->GetLabel()))
            output[2]->InsertItem(*it);
          // Otherwise it is a marker
          else
            output[0]->InsertItem(*it);
        } 
      }
    }
  };
  
  void SeparateKnownVirtualMarkersFilter::AppendKnownVirtualMarkerLabel(std::list<std::string>& target, const std::string& label)
  {};
  
  void SeparateKnownVirtualMarkersFilter::AppendKnownVirtualMarkerLabels(std::list<std::string>& target, const std::list<std::string>& labels)
  {};
  
  void SeparateKnownVirtualMarkersFilter::SetKnownVirtualMarkerLabels(std::list<std::string>& target, const std::list<std::string>& labels)
  {};
  
};


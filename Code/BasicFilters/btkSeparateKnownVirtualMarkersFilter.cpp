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
   *
   * These categories are:
   *  -# markers
   *  -# virtual markers used for frame virtuals
   *  -# other virtual markers (CenterOfMass, ...)
   *  -# other points (angle, force, moment, power, ...)
   *
   * By default, the list of labels known as virtual markers used for frame virtuals is:
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
   * You can also add a prefix for the markers' label by using the method SetLabelPrefix. This prefix will be concated to the markers' label during the separation.
   *
   * This filter only copies the pointer associated which each point instead of using a deep copy of it.
   *
   * @ingroup BTKBasicFilters
   */
   
  /**
   * @struct SeparateKnownVirtualMarkersFilter::StringAxes
   * @brief Structure to store the four labels used for a frame axes.
   */
  /**
   * @var std::string SeparateKnownVirtualMarkersFilter::StringAxes::Origin
   * Label for one of the point defining the frame (not necessarily the origin).
   */
  /**
   * @var std::string SeparateKnownVirtualMarkersFilter::StringAxes::Axis1
   * Label for one of the point defining the frame (not necessarily the first axis).
   */
  /**
   * @var std::string SeparateKnownVirtualMarkersFilter::StringAxes::Axis2
   * Label for one of the point defining the frame (not necessarily the second axis).
   */
  /**
   * @var std::string SeparateKnownVirtualMarkersFilter::StringAxes::Axis3
   * Label for one of the point defining the frame (not necessarily the second axis).
   */
  /**
   * @fn SeparateKnownVirtualMarkersFilter::StringAxes::StringAxes(const std::string& o, const std::string& a1, const std::string& a2, const std::string& a3)
   * Constructor.
   */
  /**
   * @fn bool SeparateKnownVirtualMarkersFilter::StringAxes::operator==(const StringAxes& lhs, const StringAxes& rhs)
   * Equal operator.
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
   * @fn void SeparateKnownVirtualMarkersFilter::AppendKnownVirtualMarkerLabelForAxes(const std::string& o, const std::string& a1, const std::string& a2, const std::string& a3)
   * Convenient method to append labels used for a frame to the list of virtual markers used for the virtuals.
   */
  
  /**
   * Appends labels used for a frame to the list of virtual markers used for the virtuals.
   */
  void SeparateKnownVirtualMarkersFilter::AppendKnownVirtualMarkerLabelForAxes(const StringAxes& label)
  {
    for (std::list<StringAxes>::const_iterator it = this->m_VirtualMarkerLabelsAxes.begin() ; it != this->m_VirtualMarkerLabelsAxes.end() ; ++it)
    {
      if (*it == label)
        return;
    }
    this->m_VirtualMarkerLabelsAxes.push_back(label);
    this->Modified();
  };
  
  /**
   * Appends a list of labels to the list of virtual markers used for the frame virtuals. Each label
   * is checked before to be inserted to not have duplication.
   */
  void SeparateKnownVirtualMarkersFilter::AppendKnownVirtualMarkerLabelsForAxes(const std::list<StringAxes>& labels)
  {
    if (this->m_VirtualMarkerLabelsAxes == labels)
      return;
    std::list<StringAxes> labels_ = labels;
    std::list<StringAxes>::iterator itL = labels_.begin();
    while (itL != labels_.end())
    {
      for (std::list<StringAxes>::const_iterator it = this->m_VirtualMarkerLabelsAxes.begin() ; it != this->m_VirtualMarkerLabelsAxes.end() ; ++it)
      {
        if (*it == *itL)
        {
          this->m_VirtualMarkerLabelsAxes.push_back(*itL);
          itL = labels_.erase(itL);
          break;
        };
      }
    }
    this->Modified();
  };
   
  /**
   * Sets the list of labels for the virtual markers used for the frame virtuals.
   */
   void SeparateKnownVirtualMarkersFilter::SetKnownVirtualMarkerLabelsForAxes(const std::list<StringAxes>& labels)
   {
     if (this->m_VirtualMarkerLabelsAxes == labels)
       return;
     this->m_VirtualMarkerLabelsAxes = labels;
     this->Modified();
   };
   
  /**
   * @fn const std::list<std::string>& SeparateKnownVirtualMarkersFilter::GetKnownVirtualMarkerLabelsForAxes() const
   * Returns the list of labels for the virtual markers used for the frame virtuals.
   */
  
  /**
   * Append a label to the list of virtual markers used in another context than frame virtuals.
   */
  void SeparateKnownVirtualMarkersFilter::AppendKnownVirtualMarkerLabelForOthers(const std::string& label)
  {
    if (this->FindLabel(&this->m_VirtualMarkerLabelsOthers, label, false))
      return;
    this->m_VirtualMarkerLabelsOthers.push_back(label);
    this->Modified();
  };
   
  /**
   * Append a list of labels to the list of virtual markers used in another context than frame virtuals.
   */
  void SeparateKnownVirtualMarkersFilter::AppendKnownVirtualMarkerLabelsForOthers(const std::list<std::string>& labels)
  {
    if (this->m_VirtualMarkerLabelsOthers == labels)
      return;
    for (std::list<std::string>::const_iterator it = labels.begin() ; it != labels.end() ; ++it)
    {
      if (!this->FindLabel(&this->m_VirtualMarkerLabelsOthers, *it, false))
        this->m_VirtualMarkerLabelsOthers.push_back(*it);
    }
    this->Modified();
  };
   
  /**
   * Sets the list of labels for the virtual markers used in another context than frame virtuals. Each label
    * is checked before to be inserted to not have duplication.
   */
  void SeparateKnownVirtualMarkersFilter::SetKnownVirtualMarkerLabelsForOthers(const std::list<std::string>& labels)
  {
    if (this->m_VirtualMarkerLabelsOthers == labels)
      return;
    this->m_VirtualMarkerLabelsOthers = labels;
    this->Modified();
  };
   
  /**
   * @fn const std::list<std::string>& SeparateKnownVirtualMarkersFilter::GetKnownVirtualMarkerLabelsForOthers() const
   * Returns the list of labels for the virtual markers used in another context than frame virtuals
   */
   
  /**
   * Sets the prefix which will be contated to the markers's label during the separation.
   */
  void SeparateKnownVirtualMarkersFilter::SetLabelPrefix(const std::string& prefix)
  {
    if (this->m_Prefix.compare(prefix) == 0)
      return;
     this->m_Prefix = prefix;
     this->Modified();
  };
  
  /**
   * @fn const std::string& SeparateKnownVirtualMarkersFilter::GetLabelPrefix() const {return this->m_Prefix;}
   * Returns the prefix which will be contated to the markers's label during the separation.
   */
  
  /**
   * Constructor. Sets the number of inputs to 1 and outputs to 4.
   */
  SeparateKnownVirtualMarkersFilter::SeparateKnownVirtualMarkersFilter()
  : ProcessObject(), m_VirtualMarkerLabelsAxes(), m_VirtualMarkerLabelsOthers(), m_Prefix()
  {
    this->SetInputNumber(1);
    this->SetOutputNumber(4);
    
    // virtual markers for frame virtuals
    const char* labels[] = {"HED", "LCL", "LFE", "LFO", "LHN", "LHU", "LRA", "LTI", "LTO", "PEL", "RCL", "RFE", "RFO", "RHN", "RHU", "RRA", "RTI", "RTO", "TRX"};
    size_t ln = sizeof(labels) / sizeof(char*);
    for (size_t i = 0 ; i < ln ; ++i)
    {
      std::string label(labels[i]);
      this->m_VirtualMarkerLabelsAxes.push_back(StringAxes(label + "O", label + "A", label + "L", label + "P"));
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
    output[1] = this->GetOutput(1); // virtual markers used for frames virtuals.
    output[2] = this->GetOutput(2); // other virtual markers
    output[3] = this->GetOutput(3); // other points
    output[0]->Clear();
    output[1]->Clear();
    output[2]->Clear();
    output[3]->Clear();
    
    PointCollection::Pointer input = this->GetInput();
    if (input.get() != 0)
    {
      PointCollection::Pointer markers = PointCollection::New();
      PointCollection::Pointer virtuals = PointCollection::New();
      // Separate points which are not markers
      for(PointCollection::Iterator it = input->Begin() ; it != input->End() ; ++it)
      {
        if ((*it)->GetType() != Point::Marker)
          output[3]->InsertItem(*it);
        else
          markers->InsertItem(*it);
      }
      // Separate points representing frame virtuals
      for (std::list<StringAxes>::const_iterator it = this->m_VirtualMarkerLabelsAxes.begin() ; it != this->m_VirtualMarkerLabelsAxes.end() ; ++it)
      {
        PointCollection::ConstIterator it0 = this->FindLabel(markers.get(), it->Origin, true);
        PointCollection::ConstIterator it1 = this->FindLabel(markers.get(), it->Axis1, true);
        PointCollection::ConstIterator it2 = this->FindLabel(markers.get(), it->Axis2, true);
        PointCollection::ConstIterator it3 = this->FindLabel(markers.get(), it->Axis3, true);
        
        if ((it0 != markers->End()) && (it1 != markers->End()) && (it2 != markers->End()) && (it3 != markers->End()))
        {
          virtuals->InsertItem(*it0);
          virtuals->InsertItem(*it1);
          virtuals->InsertItem(*it2);
          virtuals->InsertItem(*it3);
        }
      }
      // Remove the duplicate label for the virtuals
      for (PointCollection::ConstIterator it = virtuals->Begin() ; it != virtuals->End() ; ++it)
      {
        bool found = false;
        for (PointCollection::ConstIterator itF = output[1]->Begin() ; itF != output[1]->End() ; ++itF)
        {
          if ((*it)->GetLabel().compare((*itF)->GetLabel()) == 0)
          {
            found = true;
            break;
          }
        }
        if (!found)
          output[1]->InsertItem(*it);
      }
      // Copy the known points for axes
      virtuals->Clear();
      for (PointCollection::ConstIterator it = output[1]->Begin() ; it != output[1]->End() ; ++it)
        virtuals->InsertItem(*it);
      // Known virtual markers' label used in other context.
      for (PointCollection::ConstIterator it = markers->Begin() ; it != markers->End() ; ++it)
      {
        if (this->FindLabel(&this->m_VirtualMarkerLabelsOthers, (*it)->GetLabel(), true))
        {
          output[2]->InsertItem(*it);
          virtuals->InsertItem(*it);
        }
      }
      // Append the points known as markers
      for (PointCollection::ConstIterator it = markers->Begin() ; it != markers->End() ; ++it)
      {
        if (this->FindLabel(virtuals.get(), (*it)->GetLabel(), false) == virtuals->End())
          output[0]->InsertItem(*it);
      }
    }
  };
  
  bool SeparateKnownVirtualMarkersFilter::FindLabel(std::list<std::string>* references, const std::string& l, bool usePrefix) const
  {
    if (usePrefix)
    {
      for (std::list<std::string>::const_iterator it = references->begin() ; it != references->end() ; ++it)
      {
        if ((this->m_Prefix + *it).compare(l) == 0)
          return true;
      }
    }
    else
    {
      for (std::list<std::string>::const_iterator it = references->begin() ; it != references->end() ; ++it)
      {
        if (it->compare(l) == 0)
          return true;
      }
    }
    return false;
  };
  
  PointCollection::ConstIterator SeparateKnownVirtualMarkersFilter::FindLabel(PointCollection* references, const std::string& l, bool usePrefix) const
  {
    if (usePrefix)
    {
      for (PointCollection::ConstIterator it = references->Begin() ; it != references->End() ; ++it)
      {
        if ((*it)->GetLabel().compare(this->m_Prefix + l) == 0)
          return it;
      }
      return references->End();
    }
    else
    {
      for (PointCollection::ConstIterator it = references->Begin() ; it != references->End() ; ++it)
      {
        if ((*it)->GetLabel().compare(l) == 0)
          return it;
      }
      return references->End();
    }
  };
};


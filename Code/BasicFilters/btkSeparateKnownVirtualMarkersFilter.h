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

#ifndef __btkSeparateKnownVirtualMarkersFilter_h
#define __btkSeparateKnownVirtualMarkersFilter_h

#include "btkProcessObject.h"
#include "btkPointCollection.h"

namespace btk
{
  class SeparateKnownVirtualMarkersFilter : public ProcessObject
  {
  public:
    typedef SharedPtr<SeparateKnownVirtualMarkersFilter> Pointer;
    typedef SharedPtr<const SeparateKnownVirtualMarkersFilter> ConstPointer;

    static Pointer New() {return Pointer(new SeparateKnownVirtualMarkersFilter());};
    
    // ~SeparateKnownVirtualMarkersFilter(); // Implicit
    
    PointCollection::Pointer GetInput() {return this->GetInput(0);};
    void SetInput(PointCollection::Pointer input) {this->SetNthInput(0, input);};
    PointCollection::Pointer GetOutput(int idx) {return static_pointer_cast<PointCollection>(this->GetNthOutput(idx));};
    
    void AppendKnownVirtualMarkerLabelForAxes(const std::string& label) {this->AppendKnownVirtualMarkerLabel(this->m_VirtualMarkerLabelsAxes, label);};
    void AppendKnownVirtualMarkerLabelsForAxes(const std::list<std::string>& labels) {this->AppendKnownVirtualMarkerLabels(this->m_VirtualMarkerLabelsAxes, labels);};
    void SetKnownVirtualMarkerLabelsForAxes(const std::list<std::string>& labels) {this->SetKnownVirtualMarkerLabels(this->m_VirtualMarkerLabelsAxes, labels);};
    const std::list<std::string>& GetKnownVirtualMarkerLabelsForAxes() const {return this->m_VirtualMarkerLabelsAxes;};
    
    void AppendKnownVirtualMarkerLabelForOthers(const std::string& label) {this->AppendKnownVirtualMarkerLabel(this->m_VirtualMarkerLabelsOthers, label);};
    void AppendKnownVirtualMarkerLabelsForOthers(const std::list<std::string>& labels) {this->AppendKnownVirtualMarkerLabels(this->m_VirtualMarkerLabelsOthers, labels);};
    void SetKnownVirtualMarkerLabelsForOthers(const std::list<std::string>& labels) {this->SetKnownVirtualMarkerLabels(this->m_VirtualMarkerLabelsAxes, labels);};
    const std::list<std::string>& GetKnownVirtualMarkerLabelsForOthers() const {return this->m_VirtualMarkerLabelsOthers;};
    
  protected:
    BTK_BASICFILTERS_EXPORT SeparateKnownVirtualMarkersFilter();
    
    PointCollection::Pointer GetInput(int idx) {return static_pointer_cast<PointCollection>(this->GetNthInput(idx));};  
    BTK_BASICFILTERS_EXPORT virtual DataObject::Pointer MakeOutput(int idx);
    BTK_BASICFILTERS_EXPORT virtual void GenerateData();
    
  private:
    BTK_BASICFILTERS_EXPORT void AppendKnownVirtualMarkerLabel(std::list<std::string>& target, const std::string& label);
    BTK_BASICFILTERS_EXPORT void AppendKnownVirtualMarkerLabels(std::list<std::string>& target, const std::list<std::string>& labels);
    BTK_BASICFILTERS_EXPORT void SetKnownVirtualMarkerLabels(std::list<std::string>& target, const std::list<std::string>& labels);
    bool CheckLabel(const std::list<std::string>& references, const std::string& l) const;
    
    SeparateKnownVirtualMarkersFilter(const SeparateKnownVirtualMarkersFilter& ); // Not implemented.
    SeparateKnownVirtualMarkersFilter& operator=(const SeparateKnownVirtualMarkersFilter& ); // Not implemented.
    
    std::list<std::string> m_VirtualMarkerLabelsAxes;
    std::list<std::string> m_VirtualMarkerLabelsOthers;
  };
  
  inline bool SeparateKnownVirtualMarkersFilter::CheckLabel(const std::list<std::string>& references, const std::string& l) const
  {
    for (std::list<std::string>::const_iterator it = references.begin() ; it != references.end() ; ++it)
    {
      if (it->compare(l) == 0)
        return true;
    }
    return false;
  };
};

#endif // __btkSeparateKnownVirtualMarkersFilter_h



/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2012, Arnaud Barré
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
    struct StringAxes
    {
      StringAxes(const std::string& o, const std::string& a1, const std::string& a2, const std::string& a3)
      : Origin(o), Axis1(a1), Axis2(a2), Axis3(a3)
      {};
      
      friend bool operator==(const StringAxes& lhs, const StringAxes& rhs)
      {
        if ((lhs.Origin.compare(rhs.Origin) == 0)
            && (lhs.Axis1.compare(rhs.Axis1) == 0)
            && (lhs.Axis2.compare(rhs.Axis2) == 0)
            && (lhs.Axis3.compare(rhs.Axis3) == 0))
          return true;
        return false;
      };
      
      std::string Origin;
      std::string Axis1;
      std::string Axis2;
      std::string Axis3;
    };
    
    typedef SharedPtr<SeparateKnownVirtualMarkersFilter> Pointer;
    typedef SharedPtr<const SeparateKnownVirtualMarkersFilter> ConstPointer;

    static Pointer New() {return Pointer(new SeparateKnownVirtualMarkersFilter());};
    
    // ~SeparateKnownVirtualMarkersFilter(); // Implicit
    
    PointCollection::Pointer GetInput() {return this->GetInput(0);};
    void SetInput(PointCollection::Pointer input) {this->SetNthInput(0, input);};
    PointCollection::Pointer GetOutput(int idx) {return static_pointer_cast<PointCollection>(this->GetNthOutput(idx));};
    
    void AppendKnownVirtualMarkerLabelForAxes(const std::string& o, const std::string& a1, const std::string& a2, const std::string& a3) {this->AppendKnownVirtualMarkerLabelForAxes(StringAxes(o,a1,a2,a3));};
    BTK_BASICFILTERS_EXPORT void AppendKnownVirtualMarkerLabelForAxes(const StringAxes& label);
    BTK_BASICFILTERS_EXPORT void AppendKnownVirtualMarkerLabelsForAxes(const std::list<StringAxes>& labels);
    BTK_BASICFILTERS_EXPORT void SetKnownVirtualMarkerLabelsForAxes(const std::list<StringAxes>& labels);
    const std::list<StringAxes>& GetKnownVirtualMarkerLabelsForAxes() const {return this->m_VirtualMarkerLabelsAxes;};
    
    BTK_BASICFILTERS_EXPORT void AppendKnownVirtualMarkerLabelForOthers(const std::string& label);
    BTK_BASICFILTERS_EXPORT void AppendKnownVirtualMarkerLabelsForOthers(const std::list<std::string>& labels);
    BTK_BASICFILTERS_EXPORT void SetKnownVirtualMarkerLabelsForOthers(const std::list<std::string>& labels);
    const std::list<std::string>& GetKnownVirtualMarkerLabelsForOthers() const {return this->m_VirtualMarkerLabelsOthers;};
    
    BTK_BASICFILTERS_EXPORT void SetLabelPrefix(const std::string& prefix);
    const std::string& GetLabelPrefix() const {return this->m_Prefix;};
    
  protected:
    BTK_BASICFILTERS_EXPORT SeparateKnownVirtualMarkersFilter();
    
    PointCollection::Pointer GetInput(int idx) {return static_pointer_cast<PointCollection>(this->GetNthInput(idx));};  
    BTK_BASICFILTERS_EXPORT virtual DataObject::Pointer MakeOutput(int idx);
    BTK_BASICFILTERS_EXPORT virtual void GenerateData();
    
  private:
    BTK_BASICFILTERS_EXPORT bool FindLabel(std::list<std::string>* references, const std::string& l, bool usePrefix) const;
    BTK_BASICFILTERS_EXPORT PointCollection::ConstIterator FindLabel(PointCollection* references, const std::string& l, bool usePrefix) const;
    
    SeparateKnownVirtualMarkersFilter(const SeparateKnownVirtualMarkersFilter& ); // Not implemented.
    SeparateKnownVirtualMarkersFilter& operator=(const SeparateKnownVirtualMarkersFilter& ); // Not implemented.
    
    std::list<StringAxes> m_VirtualMarkerLabelsAxes;
    std::list<std::string> m_VirtualMarkerLabelsOthers;
    std::string m_Prefix;
  };
};

#endif // __btkSeparateKnownVirtualMarkersFilter_h



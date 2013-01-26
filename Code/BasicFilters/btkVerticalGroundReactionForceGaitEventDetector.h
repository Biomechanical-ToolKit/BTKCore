/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2013, Arnaud Barré
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

#ifndef __btkVerticalGroundReactionForceGaitEventDetector_h
#define __btkVerticalGroundReactionForceGaitEventDetector_h

#include "btkProcessObject.h"
#include "btkWrenchCollection.h"
#include "btkEventCollection.h"

#include <vector>
#include <string>

namespace btk
{
  class VerticalGroundReactionForceGaitEventDetector : public ProcessObject
  {
  public:
    typedef SharedPtr<VerticalGroundReactionForceGaitEventDetector> Pointer;
    typedef SharedPtr<const VerticalGroundReactionForceGaitEventDetector> ConstPointer;

    static Pointer New() {return Pointer(new VerticalGroundReactionForceGaitEventDetector());};
    
    // ~WrenchCollection(); // Implicit
    
    WrenchCollection::Pointer GetInput() {return this->GetInput(0);};
    void SetInput(Wrench::Pointer input)
    {
      WrenchCollection::Pointer col = WrenchCollection::New();
      col->InsertItem(input);
      this->SetNthInput(0, col);
    };
    void SetInput(WrenchCollection::Pointer input) {this->SetNthInput(0, input);};
    EventCollection::Pointer GetOutput() {return this->GetOutput(0);};

    BTK_BASICFILTERS_EXPORT void SetThresholdValue(int threshold);
    int GetThresholdValue() const {return this->m_Threshold;};
    
    BTK_BASICFILTERS_EXPORT void SetForceplateContextMapping(const std::vector<std::string>& mapping);
    const std::vector<std::string>& GetForceplateContextMapping() const {return this->m_ContextMapping;};
    
    BTK_BASICFILTERS_EXPORT void SetRegionOfInterest(int lb = -1, int ub = -1);
    const int* GetRegionOfInterest() const {return this->mp_ROI;};
    
    BTK_BASICFILTERS_EXPORT void SetAcquisitionInformation(int firstFrame, double freq, const std::string& subjectName);
    BTK_BASICFILTERS_EXPORT void GetAcquisitionInformation(int& firstFrame, double& freq, std::string& subjectName);

  protected:
    BTK_BASICFILTERS_EXPORT VerticalGroundReactionForceGaitEventDetector();
    
    WrenchCollection::Pointer GetInput(int idx) {return static_pointer_cast<WrenchCollection>(this->GetNthInput(idx));};  
    EventCollection::Pointer GetOutput(int idx) {return static_pointer_cast<EventCollection>(this->GetNthOutput(idx));};
    BTK_BASICFILTERS_EXPORT virtual DataObject::Pointer MakeOutput(int idx);
    BTK_BASICFILTERS_EXPORT virtual void GenerateData();
    
  private:
    VerticalGroundReactionForceGaitEventDetector(const VerticalGroundReactionForceGaitEventDetector& ); // Not implemented.
    VerticalGroundReactionForceGaitEventDetector& operator=(const VerticalGroundReactionForceGaitEventDetector& ); // Not implemented.
    
    int m_Threshold;
    std::vector<std::string> m_ContextMapping;
    int mp_ROI[2];
    int m_FirstFrame;
    double m_FrameRate;
    std::string m_SubjectName;
  };
};

#endif // __btkVerticalGroundReactionForceGaitEventDetector_h

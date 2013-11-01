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

#ifndef __btkSubAcquisitionFilter_h
#define __btkSubAcquisitionFilter_h

#include "btkProcessObject.h"
#include "btkAcquisition.h"

namespace btk
{
  class SubAcquisitionFilter : public ProcessObject
  {
  public:
    typedef btkSharedPtr<SubAcquisitionFilter> Pointer;
    typedef btkSharedPtr<const SubAcquisitionFilter> ConstPointer;
    
    typedef enum {All = 0, PointsOnly, AnalogsOnly, EventsOnly} ExtractionOption; 

    static Pointer New() {return Pointer(new SubAcquisitionFilter());};
    
    // ~SubAcquisitionFilter(); // Implicit
    
    void SetInput(Acquisition::Pointer input) {this->SetNthInput(0, input);};
    Acquisition::Pointer GetInput() {return this->GetInput(0);};
    
    Acquisition::Pointer GetOutput() {return this->GetOutput(0);};
    
    const int* GetFramesIndex() const {return this->mp_FramesIndex;};
    BTK_BASICFILTERS_EXPORT void SetFramesIndex(int lb = -1, int ub = -1);
    ExtractionOption GetExtractionOption() const {return this->m_ExtractionOption;};
    BTK_BASICFILTERS_EXPORT void SetExtractionOption(ExtractionOption option);
    ExtractionOption GetExtractionOption(std::list<int>& ids) const {ids = this->m_Ids; return this->m_ExtractionOption;};
    BTK_BASICFILTERS_EXPORT void SetExtractionOption(ExtractionOption option, const std::list<int>& ids);

  protected:
    BTK_BASICFILTERS_EXPORT SubAcquisitionFilter();
    
    Acquisition::Pointer GetInput(int idx) {return static_pointer_cast<Acquisition>(this->GetNthInput(idx));};  
    Acquisition::Pointer GetOutput(int idx) {return static_pointer_cast<Acquisition>(this->GetNthOutput(idx));};
    BTK_BASICFILTERS_EXPORT virtual DataObject::Pointer MakeOutput(int idx);
    BTK_BASICFILTERS_EXPORT virtual void GenerateData();
    
  private:
    SubAcquisitionFilter(const SubAcquisitionFilter& ); // Not implemented.
    SubAcquisitionFilter& operator=(const SubAcquisitionFilter& ); // Not implemented.
    
    void SubPoints(Acquisition::Pointer out, Acquisition::Pointer in, int bounds[2]);
    void SubAnalogs(Acquisition::Pointer out, Acquisition::Pointer in, int bounds[2]);
    void SubEvents(Acquisition::Pointer out, Acquisition::Pointer in, int bounds[2]);
    
    ExtractionOption m_ExtractionOption;
    int mp_FramesIndex[2];
    std::list<int> m_Ids;
  };
};

#endif // __btkSubAcquisitionFilter_h


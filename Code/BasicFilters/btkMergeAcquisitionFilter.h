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

#ifndef __btkMergeAcquisitionFilter_h
#define __btkMergeAcquisitionFilter_h

#include "btkProcessObject.h"
#include "btkAcquisition.h"

#include <map>

namespace btk
{
  class MergeAcquisitionFilter : public ProcessObject
  {
  public:
    enum {KeepAllFrames = 0, KeepFromHighestFirstFrame};
    
    typedef SharedPtr<MergeAcquisitionFilter> Pointer;
    typedef SharedPtr<const MergeAcquisitionFilter> ConstPointer;

    static Pointer New() {return Pointer(new MergeAcquisitionFilter());};
    
    // ~MergeAcquisitionFilter(); // Implicit
    
    int GetFirstFrameRule() const {return this->m_FirstFrameRule;};
    BTK_BASICFILTERS_EXPORT void SetFirstFrameRule(int rule);
    
    Acquisition::Pointer GetInput(int idx) {return static_pointer_cast<Acquisition>(this->GetNthInput(idx));};
    void SetInput(int idx, Acquisition::Pointer input) {this->SetNthInput(idx, input);};
    Acquisition::Pointer GetOutput() {return this->GetOutput(0);};

  protected:
    BTK_BASICFILTERS_EXPORT MergeAcquisitionFilter();
    
    Acquisition::Pointer GetOutput(int idx) {return static_pointer_cast<Acquisition>(this->GetNthOutput(idx));};
    BTK_BASICFILTERS_EXPORT virtual DataObject::Pointer MakeOutput(int idx);
    BTK_BASICFILTERS_EXPORT virtual void GenerateData();
    
  private:
    void MergeAcquisition(int idx, Acquisition::Pointer out);
    void MergeData(Acquisition::Pointer output, Acquisition::Pointer input, int frameNumber) const;
    void ConcatData(Acquisition::Pointer output, Acquisition::Pointer input) const;
    void RemoveDeprecatedMetaData(MetaData::Pointer in, bool pointScreenToRemove = false) const;
    void CleanMetaData(MetaData::Pointer in) const;
    void UpdateKnownMetaData(Acquisition::Pointer out, MetaData::Pointer in, bool mergeData) const;
    void UpdatePartialForcePlateMetaData(MetaData::Pointer out) const;
    bool UpdateForcePlatformMetaData(MetaData::Pointer out, MetaData::Pointer forcePlatform) const;
    bool CheckForcePlatform(MetaData::Pointer forcePlatform) const;
    int GetMaxInfoForcePlatform(MetaData::Pointer fp, int* rows = 0, int* cols = 0) const;
    bool UpdateTrialMetaData(Acquisition::Pointer out, MetaData::Pointer trial, bool mergeData) const;
    bool UpdateEventContextMetaData(MetaData::Pointer eventContextOut, MetaData::Pointer eventContextIn) const;
    bool CheckEventContext(MetaData::Pointer eventContext) const;
    
    MergeAcquisitionFilter(const MergeAcquisitionFilter& ); // Not implemented.
    MergeAcquisitionFilter& operator=(const MergeAcquisitionFilter& ); // Not implemented.
    
    int m_FirstFrameRule;
  };
};

#endif // __btkMergeAcquisitionFilter_h


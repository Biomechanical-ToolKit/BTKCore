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

#ifndef __btkForcePlatformsExtractor_h
#define __btkForcePlatformsExtractor_h

#include "btkProcessObject.h"
#include "btkAcquisition.h"
#include "btkForcePlatformCollection.h"
#include "btkForcePlatformTypes.h"

namespace btk
{
  class ForcePlatformsExtractor : public ProcessObject
  {
  public:
    typedef SharedPtr<ForcePlatformsExtractor> Pointer;
    typedef SharedPtr<const ForcePlatformsExtractor> ConstPointer;

    static Pointer New() {return Pointer(new ForcePlatformsExtractor());};
    
    // ~ForcePlatformsExtractor(); // Implicit
    
    Acquisition::Pointer GetInput() {return this->GetInput(0);};
    void SetInput(Acquisition::Pointer input) {this->SetNthInput(0, input);};
    ForcePlatformCollection::Pointer GetOutput() {return this->GetOutput(0);};

  protected:
    BTK_BASICFILTERS_EXPORT ForcePlatformsExtractor();
    
    Acquisition::Pointer GetInput(int idx) {return static_pointer_cast<Acquisition>(this->GetNthInput(idx));};  
    ForcePlatformCollection::Pointer GetOutput(int idx) {return static_pointer_cast<ForcePlatformCollection>(this->GetNthOutput(idx));};
    BTK_BASICFILTERS_EXPORT virtual DataObject::Pointer MakeOutput(int idx);
    BTK_BASICFILTERS_EXPORT virtual void GenerateData();
    
  private:
    void ExtractForcePlatformDataCommon(ForcePlatform::Pointer fp, size_t idx, int coefficientsAlreadyExtracted, MetaData::Pointer pOrigin, MetaData::Pointer pCorners, MetaData::Pointer pCalMatrix);
    bool ExtractForcePlatformData(ForcePlatform::Pointer fp, AnalogCollection::Pointer channels, int alreadyExtracted, std::vector<int> channelsIndex);
    bool ExtractForcePlatformDataWithCalibrationMatrix(ForcePlatform::Pointer fp, AnalogCollection::Pointer channels, int alreadyExtracted, std::vector<int> channelsIndex);
    //void ExtractForcePlatformData(ForcePlatformType6::Pointer fp, AnalogCollection::Pointer channels, MetaData::Pointer fpGr);
    //void ExtractForcePlatformData(ForcePlatformType7::Pointer fp, AnalogCollection::Pointer channels, MetaData::Pointer fpGr);
    //void ExtractForcePlatformData(ForcePlatformType11::Pointer fp, AnalogCollection::Pointer channels, MetaData::Pointer fpGr);
    //void ExtractForcePlatformData(ForcePlatformType12::Pointer fp, AnalogCollection::Pointer channels, MetaData::Pointer fpGr);
    //void ExtractForcePlatformData(ForcePlatformType21::Pointer fp, AnalogCollection::Pointer channels, MetaData::Pointer fpGr);
    bool CheckAnalogIndicesForForcePlatform(std::vector<int> channelsIndex, int alreadyExtracted, int numberOfChannelToExtract, int numberOfChannels) const;

    ForcePlatformsExtractor(const ForcePlatformsExtractor& ); // Not implemented.
    ForcePlatformsExtractor& operator=(const ForcePlatformsExtractor& ); // Not implemented.
  };
};

#endif // __btkForcePlatformsExtractor_h


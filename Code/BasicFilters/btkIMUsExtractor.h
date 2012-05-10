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

#ifndef __btkIMUsExtractor_h
#define __btkIMUsExtractor_h

#include "btkProcessObject.h"
#include "btkAcquisition.h"
#include "btkIMUCollection.h"

namespace btk
{
  class IMUsExtractor : public ProcessObject
  {
  public:
    typedef SharedPtr<IMUsExtractor> Pointer;
    typedef SharedPtr<const IMUsExtractor> ConstPointer;

    static Pointer New() {return Pointer(new IMUsExtractor());};
    
    // ~IMUsExtractor(); // Implicit
    
    Acquisition::Pointer GetInput() {return this->GetInput(0);};
    Acquisition::Pointer GetInput(int idx) {return static_pointer_cast<Acquisition>(this->GetNthInput(idx));};  
    void SetInput(Acquisition::Pointer input) {this->SetNthInput(0, input);};
    void SetInput(int idx, Acquisition::Pointer input) {this->SetNthInput(idx, input);};
    IMUCollection::Pointer GetOutput() {return this->GetOutput(0);};

  protected:
    BTK_BASICFILTERS_EXPORT IMUsExtractor();
    
    IMUCollection::Pointer GetOutput(int idx) {return static_pointer_cast<IMUCollection>(this->GetNthOutput(idx));};
    BTK_BASICFILTERS_EXPORT virtual DataObject::Pointer MakeOutput(int idx);
    BTK_BASICFILTERS_EXPORT virtual void GenerateData();
    
  private:
    void ExtractData(IMU* imu, AnalogCollection::Pointer channels, const std::string& label, const std::string desc, std::vector<int> channelsIndex, int numberOfChannelsToExtract, int alreadyExtracted);
  
    IMUsExtractor(const IMUsExtractor& ); // Not implemented.
    IMUsExtractor& operator=(const IMUsExtractor& ); // Not implemented.
  };
};

#endif // __btkIMUsExtractor_h


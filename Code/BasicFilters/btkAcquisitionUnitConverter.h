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

#ifndef __btkAcquisitionUnitConverter_h
#define __btkAcquisitionUnitConverter_h

#include "btkProcessObject.h"
#include "btkAcquisition.h"

namespace btk
{
  class AcquisitionUnitConverter : public ProcessObject
  {
  public:
    typedef enum {Length = 0, Angle = 1, Force = 2, Moment = 3, Power = 4} Unit;
    
    typedef SharedPtr<AcquisitionUnitConverter> Pointer;
    typedef SharedPtr<const AcquisitionUnitConverter> ConstPointer;

    static Pointer New() {return Pointer(new AcquisitionUnitConverter());};
    
    // ~AcquisitionUnitConverter(); // Implicit
    
    Acquisition::Pointer GetInput() {return this->GetInput(0);};
    void SetInput(Acquisition::Pointer input) {this->SetNthInput(0, input);};
    Acquisition::Pointer GetOutput() {return this->GetOutput(0);};
    
    BTK_BASICFILTERS_EXPORT void SetUnit(Unit u, const std::string& s);
    const std::string& GetUnit(Unit u) {return this->m_Units[u];};

  protected:
    BTK_BASICFILTERS_EXPORT AcquisitionUnitConverter();
    
    Acquisition::Pointer GetInput(int idx) {return static_pointer_cast<Acquisition>(this->GetNthInput(idx));};
    Acquisition::Pointer GetOutput(int idx) {return static_pointer_cast<Acquisition>(this->GetNthOutput(idx));};
    BTK_BASICFILTERS_EXPORT virtual DataObject::Pointer MakeOutput(int idx);
    BTK_BASICFILTERS_EXPORT virtual void GenerateData();
    
  private:
    BTK_BASICFILTERS_EXPORT bool ConvertUnit(double* scale, int num, const char** units, const double* scales, const std::string& out, const std::string& in);
    BTK_BASICFILTERS_EXPORT bool CheckUnit(int* idx, int num, const char** units, const std::string& unit);
    inline bool ConvertCalMatrix(std::vector<float>& values, size_t alreadyConverted, size_t total, size_t columnsStep, size_t rows, size_t cols, const double* scales);
    
    AcquisitionUnitConverter(const AcquisitionUnitConverter& ); // Not implemented.
    AcquisitionUnitConverter& operator=(const AcquisitionUnitConverter& ); // Not implemented.
    
    std::vector<std::string> m_Units;
    
    static const char* LengthUnit[5];
    static const double LengthScale[25];
    static const char* AngleUnit[2];
    static const double AngleScale[4];
    static const char* ForceUnit[1];
    static const double ForceScale[1];
    static const char* MomentUnit[5];
    static const double MomentScale[25];
    static const char* PowerUnit[1];
    static const double PowerScale[1];
    static const char* VoltageUnit[2];
    static const double VoltageScale[4];
  };
  
  bool AcquisitionUnitConverter::ConvertCalMatrix(std::vector<float>& values, size_t alreadyConverted, size_t total, size_t columnsStep, size_t rows, size_t cols, const double* scales)
  {
    if (alreadyConverted + rows*cols > total)
      return false;
    else
    {
      for (size_t i = 0 ; i < cols ; ++i)
      {
        for (size_t j = 0 ; j < rows ; ++j)
        {
          values[j + i*columnsStep + alreadyConverted] = static_cast<float>(values[j + i*columnsStep + alreadyConverted] * scales[j]);
        }
      }
    }
    return true; 
  };
};

#endif // __btkAcquisitionUnitConverter_h


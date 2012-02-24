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

#ifndef __btkVTKGRFsFramesSource_h
#define __btkVTKGRFsFramesSource_h

#include "btkWrenchCollection.h"

#include <vtkPolyDataAlgorithm.h>

namespace btk
{
  class VTKGRFsComponents;
  class VTKGRFsFramesSource : public vtkPolyDataAlgorithm
  {
  public:
    BTK_VTK_EXPORT static VTKGRFsFramesSource* New();
    vtkExportedTypeRevisionMacro(VTKGRFsFramesSource, vtkPolyDataAlgorithm, BTK_VTK_EXPORT);
    BTK_VTK_EXPORT void PrintSelf(ostream& os, vtkIndent indent);

    BTK_VTK_EXPORT void SetInput(WrenchCollection::Pointer input);
    
    double GetScaleUnit() {return this->m_Scale;};
    void SetScaleUnit(double s) {this->m_Scale = s;};
    
    bool GetButterflyActivation() const {return this->m_ButterflyActivated;};
    void SetButterflyActivation(bool activated) {this->m_ButterflyActivated = activated;};
    void ButterflyActivationOn() {this->SetButterflyActivation(true);};
    void ButterflyActivationOff() {this->SetButterflyActivation(false);};
    
    BTK_VTK_EXPORT void SetPathVisibility(int idx, bool visible);
    BTK_VTK_EXPORT void ShowPath(int idx);
    BTK_VTK_EXPORT void ShowPaths();
    BTK_VTK_EXPORT void HidePath(int idx);
    BTK_VTK_EXPORT void HidePaths();
    
  protected:
    VTKGRFsFramesSource();
    ~VTKGRFsFramesSource();
    
    BTK_VTK_EXPORT virtual int RequestInformation(vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector);
    BTK_VTK_EXPORT virtual int RequestData(vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector);
    //BTK_VTK_EXPORT virtual int RequestUpdateExtent(vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector);

    BTK_VTK_EXPORT virtual int FillInputPortInformation(int port, vtkInformation* info);
    
  private:
    VTKGRFsFramesSource(const VTKGRFsFramesSource& );  // Not implemented.
    VTKGRFsFramesSource& operator=(const VTKGRFsFramesSource& );  // Not implemented.

    VTKGRFsComponents* mp_GRFsComponents;
    VTKGRFsComponents* mp_GRFsPathIds;
    vtkIntArray* mp_GRFsPathActivated;
    double m_Scale;
    vtkPolyData* mp_ButterflyCache;
    int m_LastCachedFrame;
    bool m_ButterflyActivated;
  };
};
#endif // __btkVTKGRFsFramesSource_h

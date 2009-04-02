/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009, Arnaud Barré
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

#ifndef __btkVTKMarkerFrameSource_h
#define __btkVTKMarkerFrameSource_h

#include "btkMarkerCollection.h"
#include "btkVTKDataObjectAdapter.h"
#include "btkVTKPolyDataAlgorithmAdapter.h"

namespace btk
{
  class VTKMarkerFrameSource : public VTKPolyDataAlgorithmAdapter<MarkerCollection> 
  {
  public:
    static VTKMarkerFrameSource* New();
    vtkTypeRevisionMacro(VTKMarkerFrameSource, VTKPolyDataAlgorithmAdapter<MarkerCollection>);
    void PrintSelf(ostream& os, vtkIndent indent);
    
    int GetIndex() const {return this->m_Index;};
    void SetIndex(int idx);
    
  protected:
    VTKMarkerFrameSource();
    ~VTKMarkerFrameSource() {};
    
    int RequestData(vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector);
    
  private:
    VTKMarkerFrameSource(const VTKMarkerFrameSource& );  // Not implemented.
    VTKMarkerFrameSource& operator=(const VTKMarkerFrameSource& );  // Not implemented.
    
    int m_Index;
  };
};
#endif // __btkVTKMarkerFrameSource_h

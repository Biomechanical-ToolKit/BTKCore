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
 * LOSS OF USE, DATA, OR PROFITS;OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __btkVTKInteractorStyleTrackballCamera_h
#define __btkVTKInteractorStyleTrackballCamera_h

#include "btkConfigure.h"

#include <vtkInteractorStyleTrackballCamera.h>

class vtkUnsignedCharArray;

namespace btk
{
  class VTKInteractorStyleTrackballCamera : public vtkInteractorStyleTrackballCamera
  {
  public:
    BTK_VTK_EXPORT static VTKInteractorStyleTrackballCamera* New();
    vtkExportedTypeRevisionMacro(VTKInteractorStyleTrackballCamera, vtkInteractorStyleTrackballCamera, BTK_VTK_EXPORT);
    BTK_VTK_EXPORT void PrintSelf(ostream& os, vtkIndent indent);
    
    BTK_VTK_EXPORT void Rotate();
    //virtual void EndRotate();
    BTK_VTK_EXPORT void Spin();
    BTK_VTK_EXPORT void Pan();
    BTK_VTK_EXPORT void Dolly();
    using Superclass::Dolly;
    
    vtkGetMacro(RotationEnabled,int);
    vtkSetMacro(RotationEnabled,int);
    vtkBooleanMacro(RotationEnabled,int);
    
    vtkGetMacro(CharEventEnabled,int);
    vtkSetMacro(CharEventEnabled,int);
    vtkBooleanMacro(CharEventEnabled,int);
    
    vtkGetMacro(RubberBandSelection,int);
    vtkSetMacro(RubberBandSelection,int);
    vtkBooleanMacro(RubberBandSelection,int);
    
    BTK_VTK_EXPORT virtual void OnLeftButtonDown();
    BTK_VTK_EXPORT virtual void OnLeftButtonUp();
    BTK_VTK_EXPORT virtual void OnMouseMove();
    BTK_VTK_EXPORT virtual void OnChar();
    
    void GetRubberBandPoints(int pts[4]) const;
    
  protected:
    BTK_VTK_EXPORT VTKInteractorStyleTrackballCamera();
    ~VTKInteractorStyleTrackballCamera();
    
  private:
    //BTK_VTK_EXPORT void ProjectToSphere(int* size, int* pos, double* vec);
    double ProjectToSphere(double r, double x, double y) const;
    
    double m_Radius;
    int mp_RubberBandGeometry[2][2];
    int RotationEnabled;
    int CharEventEnabled;
    vtkUnsignedCharArray* mp_PixelArray;
    int RubberBandSelection;
  };
};

#endif // __btkVTKInteractorStyleTrackballCamera_h
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

#ifndef __btkVTKPlotCycle_h
#define __btkVTKPlotCycle_h

#include "btkConfigure.h"

#include <vtkPlotPoints.h>

class vtkFloatArray;

namespace btk
{
  class VTKPlotCycle : public vtkPlotPoints
  {
  public:
    vtkTypeMacro(VTKPlotCycle, vtkPlotPoints);

    BTK_VTK_EXPORT static VTKPlotCycle* New();
    
    BTK_VTK_EXPORT vtkFloatArray* GetCyclesBoundaries();
    BTK_VTK_EXPORT void SetCyclesBoundaries(vtkFloatArray* data);
    BTK_VTK_EXPORT void SetCyclicDisplay(bool activate);
    BTK_VTK_EXPORT void SetTemporalDisplay(bool activate);
    
    BTK_VTK_EXPORT virtual bool Paint(vtkContext2D* painter);
    BTK_VTK_EXPORT virtual bool PaintLegend(vtkContext2D* painter, const vtkRectf& rect, int legendIndex);
    
    BTK_VTK_EXPORT virtual vtkIdType GetNearestPoint(const vtkVector2f& point, const vtkVector2f& tolerance, vtkVector2f* location);
    
  protected:
    BTK_VTK_EXPORT VTKPlotCycle();
    BTK_VTK_EXPORT ~VTKPlotCycle();

  private:
    VTKPlotCycle(const VTKPlotCycle&); // Not implemented.
    void operator=(const VTKPlotCycle&); // Not implemented.
    
    vtkFloatArray* mp_CyclesBoundaries;
    bool m_CyclicDrawingMode;
  };
};

#endif // __btkVTKPlotCycle_h
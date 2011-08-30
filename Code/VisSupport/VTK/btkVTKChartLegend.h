/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2011, Arnaud Barré
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

#ifndef __btkVTKChartLegend_h
#define __btkVTKChartLegend_h

#include "btkConfigure.h"

#include <vtkChartLegend.h>

namespace btk
{
  class VTKChartLegend : public vtkChartLegend
  {
  public:
    BTK_VTK_EXPORT static VTKChartLegend* New();
    vtkExportedTypeRevisionMacro(VTKChartLegend, vtkChartLegend, BTK_VTK_EXPORT);
    
    virtual ~VTKChartLegend();
    
    const float* GetPadding() const {return this->mp_Padding;};
    void SetPadding(float left, float bottom, float right, float top) {float padding[4] = {left, bottom, right, top}; this->SetPadding(padding);};
    BTK_VTK_EXPORT void SetPadding(float padding[4]);
    
    float GetSymbolWidth() const {return this->m_SymbolWidth;};
    BTK_VTK_EXPORT void SetSymbolWidth(float width);
    
    BTK_VTK_EXPORT virtual bool Paint(vtkContext2D* painter);
    
  protected:
    BTK_VTK_EXPORT VTKChartLegend();
    
    float mp_Padding[4]; // left, bottom, right, top
    float m_SymbolWidth;
    
  private:
     VTKChartLegend(const VTKChartLegend& ); // Not implemented.
     void operator=(const VTKChartLegend& );   // Not implemented.
  };
};

#endif // __btkVTKChartLegend_h

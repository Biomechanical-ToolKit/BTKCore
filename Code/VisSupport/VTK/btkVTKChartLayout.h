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

#ifndef __btkVTKChartLayout_h
#define __btkVTKChartLayout_h

#include "vtkAbstractContextItem.h"
#include "vtkVector.h" // For ivars

namespace btk
{
  class VTKChartTimeSeries;

  class VTKChartLayout : public vtkAbstractContextItem
  {
  public:
    vtkTypeMacro(VTKChartLayout, vtkAbstractContextItem);
    
    static VTKChartLayout* New();

    virtual bool Paint(vtkContext2D *painter);
    
    virtual void SetSize(const vtkVector2i& size);
    virtual vtkVector2i GetSize() const {return this->Size;}

    virtual void SetGutter(const vtkVector2f& gutter);
    virtual vtkVector2f GetGutter() const {return this->Gutter;}

    virtual void SetBorders(int left, int bottom, int right, int top);
    virtual void GetBorders(int borders[4]);

    virtual bool SetChart(const vtkVector2i& position, VTKChartTimeSeries* chart);
    virtual VTKChartTimeSeries* GetChart(const vtkVector2i& position);
    virtual VTKChartTimeSeries* TakeChart(const vtkVector2i &position);
    
    virtual bool SetChartSpan(const vtkVector2i& position, const vtkVector2i& span);
    virtual vtkVector2i GetChartSpan(const vtkVector2i& position);
    
    virtual void UpdateLayout();
    
    virtual void PrintSelf(ostream& os, vtkIndent indent);

  protected:
    VTKChartLayout();
    ~VTKChartLayout();

    // The number of charts in x and y.
    vtkVector2i Size;
    // The gutter between each chart.
    vtkVector2f Gutter;
    int Borders[4];
    bool LayoutIsDirty;

  private:
    class PIMPL;
    PIMPL *Private;
    
    VTKChartLayout(const VTKChartLayout &); // Not implemented.
    void operator=(const VTKChartLayout &); // Not implemented.
  };
};
#endif //__btkVTKChartLayout_h

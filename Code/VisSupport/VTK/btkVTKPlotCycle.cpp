/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2013, Arnaud Barré
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
 
#include "btkVTKPlotCycle.h"

#include <vtkNew.h>
#include <vtkObjectFactory.h>
#include <vtkContext2D.h>
#include <vtkRect.h>
#include <vtkPen.h>
#include <vtkPoints2D.h>
#include <vtkFloatArray.h>

namespace btk
{
  /**
   * @class VTKPlotCycle btkVTKPlotCycle.h
   * @brief Plot data as a continously line or by cycle
   */
  /**
   * @var VTKPlotCycle::mp_CyclesBoundaries
   * Array of floats containing by cycle: 
   * - the index of the frame corresponding to the beginning of the cycle;
   * - the value of the beginning of the cycle;
   * - the index of the frame corresponding to the ending of the cycle.
   * - the value of the ending of the cycle;
   */
  /**
   * @var VTKPlotCycle::m_CyclicDrawingMode
   * Boolean value to know if the data are plotted continously or by cycle.
   */
  
  /**
   * @fn static VTKPlotCycle* VTKPlotCycle::New()
   * Constructs a VTKPlotCycle object and return it as a pointer.
   */
  vtkStandardNewMacro(VTKPlotCycle);
  
  /**
   * Paint the plot.
   */
  bool VTKPlotCycle::Paint(vtkContext2D* painter)
  {
    if (!this->Visible || !this->Points)
       return false;
     
    painter->ApplyPen(this->Pen);
    
    if (!this->m_CyclicDrawingMode || (this->mp_CyclesBoundaries == NULL))
    {
      painter->DrawPoly(this->Points);
      this->vtkPlotPoints::Paint(painter);
    }
    else
    {
      int numCycles = this->mp_CyclesBoundaries->GetNumberOfTuples() / 4;
      float* boundaries = this->mp_CyclesBoundaries->GetPointer(0);
      float* points = vtkFloatArray::SafeDownCast(this->Points->GetData())->GetPointer(0);
      for (int i = 0 ; i < numCycles ; ++i)
      {
        if (i != 0)
          points[static_cast<int>(2*boundaries[4*i])] = boundaries[4*(i-1)+1];
        points[static_cast<int>(2*boundaries[4*i+2])] = boundaries[4*i+3];
        painter->DrawPoly(points+static_cast<int>(2*boundaries[4*i]), static_cast<int>(boundaries[4*i+2]-boundaries[4*i]+1.0f));
      }
    }
     
    return true;
  };
  
  /**
   * Returns the cycle boundaries used when the plot is displayed by cycle.
   */
  vtkFloatArray* VTKPlotCycle::GetCyclesBoundaries()
  {
    return this->mp_CyclesBoundaries;
  };
  
  /**
   * Sets the cycle boundaries used when the plot is displayed by cycle.
   */
  void VTKPlotCycle::SetCyclesBoundaries(vtkFloatArray* data)
  {
    if (this->mp_CyclesBoundaries == data)
      return;
    if (this->mp_CyclesBoundaries != NULL)
      this->mp_CyclesBoundaries->Delete();
    this->mp_CyclesBoundaries = data;
    data->Register(this->mp_CyclesBoundaries);
    this->Modified();
  };
  
  /**
   * Display data by cycle
   */
  void VTKPlotCycle::SetCyclicDisplay(bool activate)
  {
    if (this->m_CyclicDrawingMode == activate)
      return;
    this->m_CyclicDrawingMode = activate;
    this->Modified();
  };
  
  /**
   * Display data continuously
   */
  void VTKPlotCycle::SetTemporalDisplay(bool activate)
  {
    if (this->m_CyclicDrawingMode == !activate)
      return;
    this->m_CyclicDrawingMode = !activate;
    this->Modified();
  };
  
  /**
   * Paint the legend
   */
  bool VTKPlotCycle::PaintLegend(vtkContext2D* painter, const vtkRectf& rect, int)
  {
    painter->ApplyPen(this->Pen);
    painter->DrawLine(rect[0], rect[1]+0.5*rect[3], rect[0]+rect[2], rect[1]+0.5*rect[3]);
    this->Superclass::PaintLegend(painter, rect, 0);
    return true;
  }
  
  /**
   * Function to query a plot for the nearest point to the specified coordinate.
   * Returns the index of the data series with which the point is associated or -1.
   * If in the cycle mode, then it is also checked than location is in the cycles.
   */
  vtkIdType VTKPlotCycle::GetNearestPoint(const vtkVector2f& point, const vtkVector2f& tolerance, vtkVector2f* location)
  {
    vtkIdType index = this->vtkPlotPoints::GetNearestPoint(point, tolerance, location);
    if (this->m_CyclicDrawingMode && (this->mp_CyclesBoundaries != NULL) && (index != -1))
    {
      int numCycles = this->mp_CyclesBoundaries->GetNumberOfTuples() / 4;
      float* boundaries = this->mp_CyclesBoundaries->GetPointer(0);
      int idxCycle = 0;
      for ( ; idxCycle < numCycles ; ++idxCycle)
      {
        if ((static_cast<int>(boundaries[4*idxCycle]) <= index) && (static_cast<int>(boundaries[4*idxCycle+2]) >= index))
          break;
      }
      if (idxCycle >= numCycles)
        index = -1;
    }
    return index;
  };
  
  /**
   * Constructor
   */
  VTKPlotCycle::VTKPlotCycle()
  : vtkPlotPoints()
  {
    this->mp_CyclesBoundaries = NULL;
    this->m_CyclicDrawingMode = false;
  };
  
  /**
   * Destructor
   */
  VTKPlotCycle::~VTKPlotCycle()
  {
    if (this->mp_CyclesBoundaries != NULL)
      this->mp_CyclesBoundaries->Delete();
  };
};
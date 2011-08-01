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

#include "btkVTKChartXY.h"
#include "btkMacro.h"

#include <vtkObjectFactory.h>
#include <vtkAxis.h>
#include <vtkContextScene.h>

namespace btk
{
  /**
   * @class VTKChartXY btkVTKChartXY.h
   * @brief VTK XY chart with additional zooms (horizontal & vertical zoom).
   * 
   * @ingroup BTKVTK
   */
   
  /**
   * @fn static VTKChartXY* VTKChartXY::New()
   * Constructs a VTKChartXY object and return it as a pointer.
   */
  vtkStandardNewMacro(VTKChartXY);
  vtkCxxRevisionMacro(VTKChartXY, "$Revision: 0.1 $");
  
  /**
   * @fn bool VTKChartXY::GetInteractionEnabled() const
   * Get the status of the user interactions (move & zoom).
   */
   
  /**
   * @fn void VTKChartXY::SetInteractionEnabled(bool enabled)
   * Enable / Disable the user interactions (move & zoom).
   */
  
  /**
   * @fn int VTKChartXY::GetZoomMode() const
   * Returns the zoom mode (Both directions (BOTH): 0, only horizontal (HORIZONTAL): 1, only vertical (VERTICAL): 2)
   */
   
  /**
   * @fn void VTKChartXY::SetZoomMode(int mode)
   * Sets the zoom mode (Both directions (BOTH): 0, only horizontal (HORIZONTAL): 1, only vertical (VERTICAL): 2)
   */
  
  /**
   * Overloaded method to move the chart only if the user interaction are enabled.
   */
  bool VTKChartXY::MouseMoveEvent(const vtkContextMouseEvent& mouse)
  {
    if (this->m_InteractionEnabled)
      return this->vtkChartXY::MouseMoveEvent(mouse);
    return true;
  };
  
  /**
   * Overloaded method to zoom in/out on the axis(es) specified by the zoom mode.
   */
  bool VTKChartXY::MouseWheelEvent(const vtkContextMouseEvent& mouse, int delta)
  {
    btkNotUsed(mouse);
    
    if (this->m_InteractionEnabled)
    {
      int start = 0;
      int step = 1;
    
      // Assume the following values for the axis (from enum definition in vtkAxis.h):
      //  - LEFT: 0
      //  - BOTTOM: 1
      //  - RIGHT: 2
      //  - TOP: 3
    
      if (this->m_ZoomMode == VERTICAL)
      {
        start = 0;
        step = 2;
      }
      else if (this->m_ZoomMode == HORIZONTAL)
      {
        start = 1;
        step = 2;
      }
    
      for (int i = start; i < 4; i+=step)
      {
        vtkAxis* axis = this->GetAxis(i);
        double min = axis->GetMinimum();
        double max = axis->GetMaximum();
        double frac = (max - min) * 0.1;
        if (frac > 0.0)
        {
          min += delta*frac;
          max -= delta*frac;
        }
        else
        {
          min -= delta*frac;
          max += delta*frac;
        }
        axis->SetMinimum(min);
        axis->SetMaximum(max);
        axis->RecalculateTickSpacing();
      }
    
      this->RecalculatePlotTransforms();
      this->Scene->SetDirty(true);
    }
    return true;
  };
  
  VTKChartXY::VTKChartXY()
  : vtkChartXY()
  {
    this->m_InteractionEnabled = true;
    this->m_ZoomMode = 0;
  };
};
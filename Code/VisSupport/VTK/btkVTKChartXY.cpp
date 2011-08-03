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
   * @brief VTK XY chart with additional interation (horizontal & vertical zoom, chart's boundaries).
   *
   * The top and bottom axes as well as the left and right axes uses the same range.
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
   * Enable / disable the user interactions (move & zoom).
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
   * @fn bool VTKChartXY::GetBoundsEnabled()
   * Get the status for the use of the bounds on the chart's axes.
   */

  /**
   * @fn void VTKChartXY::SetBoundsEnabled(bool enabled)
   * Enable / disable the use of the bounds on the chart's axes.
   */
  
  /**
   * @fn double* VTKChartXY::GetBounds()
   * Returns the boundaries set for the charts as xMin, xMax, yMin, yMax.
   *
   * If this is enabled (see the method SetBoundsEnabled()), then the zoom interaction will be limited by these bounds. 
   */
  
  /**
   * @fn void VTKChartXY::SetBounds(double xMin, double xMax, double yMin, double yMax)
   * Convenient method to set the chart's bounds.
   */
  
  /**
   * Sets the boundaries for the charts.
   * The input format is xMin, xMax, yMin, yMax.
   * Need to be enabled (see the method SetBoundsEnabled()) to constraint the interactions on the chart.
   */
  void VTKChartXY::SetBounds(double bounds[4])
  {
    if ((bounds[0] == this->mp_Bounds[0]) && (bounds[1] == this->mp_Bounds[1]) && (bounds[2] == this->mp_Bounds[2]) && (bounds[3] == this->mp_Bounds[3]))
      return;
    
    this->mp_Bounds[0] = bounds[0];
    this->mp_Bounds[1] = bounds[1];
    this->mp_Bounds[2] = bounds[2];
    this->mp_Bounds[3] = bounds[3];
    
    this->GetAxis(vtkAxis::BOTTOM)->SetRange(bounds[0], bounds[1]);
    this->GetAxis(vtkAxis::TOP)->SetRange(bounds[0], bounds[1]);
    this->GetAxis(vtkAxis::LEFT)->SetRange(bounds[2], bounds[3]);
    this->GetAxis(vtkAxis::RIGHT)->SetRange(bounds[2], bounds[3]);
    
    this->RecalculatePlotTransforms();
    this->Scene->SetDirty(true);
  };
  
  /**
   * Update the vertical bounds based on the plot's bounds.
   */
  void VTKChartXY::RecalculateBounds()
  {
    // TODO: Could be refactored to be more efficient as RecalculatePlotBounds() do more than expected.
    this->Update();
    vtkAxis* axis = this->GetAxis(vtkAxis::LEFT);
    axis->SetBehavior(0);
    this->RecalculatePlotBounds();
    axis->SetBehavior(1);
    this->SetBounds(this->mp_Bounds[0], this->mp_Bounds[1], axis->GetMinimum(), axis->GetMaximum());
  };
  
  /**
   * Overloaded method to move the chart only if the user interaction are enabled.
   */
  bool VTKChartXY::MouseMoveEvent(const vtkContextMouseEvent& mouse)
  {
    if (this->m_InteractionEnabled)
    {
      for (int i = 0 ; i < 2 ; ++i)
      {
        int mai = 1-i; // Map between the axis and the corresponding index in the array
        vtkAxis* axis = this->GetAxis(mai);
        float pt1[2]; axis->GetPoint1(pt1);
        float pt2[2]; axis->GetPoint2(pt2);
        double min = axis->GetMinimum(); double max = axis->GetMaximum();
        double scale = (max - min) / (double)(pt2[i] - pt1[i]);
        double delta = (mouse.LastScreenPos[i] - mouse.ScreenPos[i]) * scale;
        
        min = axis->GetMinimum() + delta;
        max = axis->GetMaximum() + delta;
        if (this->m_BoundsEnabled && (min < this->mp_Bounds[i*2]))
        {
          min = this->mp_Bounds[i*2];
          max = axis->GetMaximum();
        }
        else if (this->m_BoundsEnabled && (max > this->mp_Bounds[i*2+1]))
        {
          min = axis->GetMinimum();
          max = this->mp_Bounds[i*2+1];
        }
        axis->SetRange(min, max);
        // Because we forces the opposite axes to have the same range it is not necessary to recompute the delta.
        this->GetAxis(mai+2)->SetRange(min, max);
      }
      
      this->RecalculatePlotTransforms();
      this->Scene->SetDirty(true);
    }
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
      for (int i = 0 ; i < 2 ; ++i)
      {
        int mai = 1-i; // Map between the axis and the corresponding index in the array
        if ((this->m_ZoomMode == i) || (this->m_ZoomMode == BOTH)) // i=0: Horizontal, i=1: Vertical.
        {
          vtkAxis* axis = this->GetAxis(mai);
          double min = axis->GetMinimum();
          double max = axis->GetMaximum();
          double frac = (max - min) * 0.05;
          min += delta*frac;
          max -= delta*frac;
          if (this->m_BoundsEnabled)
          {
            if (min < this->mp_Bounds[i*2])
              min = this->mp_Bounds[i*2];
            if (max > this->mp_Bounds[i*2+1])
              max = this->mp_Bounds[i*2+1];
          }
          axis->SetRange(min,max);
          vtkAxis* axis2 = this->GetAxis(mai+2);
          axis2->SetRange(min,max);
          axis->RecalculateTickSpacing();
          axis2->RecalculateTickSpacing();
        }
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
    this->m_BoundsEnabled = false;
    this->mp_Bounds[0] = 0.0;
    this->mp_Bounds[1] = 0.0;
    this->mp_Bounds[2] = 0.0;
    this->mp_Bounds[3] = 0.0;
    
    // Set axes properties
    for (int i = 0 ; i < 4 ; ++i)
    {
      vtkAxis* axis = this->GetAxis(i);
      axis->SetBehavior(1); // Fixed
      axis->SetRange(0.0, 0.0);
    }
  };
};
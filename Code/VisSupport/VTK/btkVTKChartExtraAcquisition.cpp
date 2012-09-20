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
 
#include "btkVTKChartExtraAcquisition.h"

#include <vtkObjectFactory.h>
#include <vtkContextScene.h>
#include <vtkPen.h>
#include <vtkBrush.h>
#include <vtkContext2D.h>
#include <vtkOpenGLContextDevice2D.h>
#include <vtkAxis.h>
#include <vtkgl.h>

namespace btk
{
  vtkStandardNewMacro(VTKChartExtraAcquisition);
  
  vtkCxxSetObjectMacro(VTKChartExtraAcquisition, XAxis, vtkAxis);
  vtkCxxSetObjectMacro(VTKChartExtraAcquisition, YAxis, vtkAxis);
  
  VTKChartExtraAcquisition::VTKChartExtraAcquisition()
  : mp_CurrentFrameFunctor(), mp_RegionOfInterestFunctor(), mp_EventsFunctor()
  {
    // Display events disabled by defaut (no functor)
    this->m_DisplayEvents = 0;
    this->m_EventLineWidth = 0.5f;
    this->m_EventLineTypeFactor = 1;
    
    this->XAxis = NULL;
    this->YAxis = NULL;
  };
  
  VTKChartExtraAcquisition::~VTKChartExtraAcquisition()
  {
    this->SetXAxis(NULL);
    this->SetYAxis(NULL);
  };
  
  /**
   * @fn VTKCurrentFrameFunctor::Pointer VTKChartExtraAcquisition::GetCurrentFrameFunctor() const
   * Returns the functor used to know the current frame to display.
   */
  
  /**
   * Sets the functor used to know the current frame to display.
   */
  void VTKChartExtraAcquisition::SetCurrentFrameFunctor(VTKCurrentFrameFunctor::Pointer functor)
  {
    this->mp_CurrentFrameFunctor = functor;
  };
  
  /**
   * @fn VTKRegionOfInterestFunctor::Pointer VTKChartExtraAcquisition::GetRegionOfInterestFunctor() const
   * Returns the functor used to know the region of interest to display.
   */
  
  /**
   * Sets the functor used to know the region of interest to display.
   */
  void VTKChartExtraAcquisition::SetRegionOfInterestFunctor(VTKRegionOfInterestFunctor::Pointer functor)
  {
    this->mp_RegionOfInterestFunctor = functor;
  };
  
  /**
   * @fn VTKEventsFunctor::Pointer VTKChartExtraAcquisition::GetEventsFunctor() const
   * Returns the functor used to know events' informations.
   */
  
  /**
   * Sets the functor used to know events' informations.
   */
  void VTKChartExtraAcquisition::SetEventsFunctor(VTKEventsFunctor::Pointer functor)
  {
    this->mp_EventsFunctor = functor;
  };
  
  /**
   * @fn float VTKChartExtraAcquisition::GetEventLineWidth() const
   * Returns the width of the vertical line used to display the events.
   */
  
  /**
   * Sets the width of the vertical line used to display the events.
   */
  void VTKChartExtraAcquisition::SetEventLineWidth(float width)
  {
    if (this->m_EventLineWidth == width)
      return;
    this->m_EventLineWidth = width;
    if (this->Scene != NULL) this->Scene->SetDirty(true);
  };
  
  /**
   * @fn int VTKChartExtraAcquisition::GetEventLineTypeFactor() const
   * Returns the factor used in the spacing of the elements in the pattern to draw non-solid line.
   *
   * By default the dash, dot line, etc has a space of few pixels, but this function can scale this spacing.
   */
  
  /**
   * Sets the factor used in the spacing of the elements in the pattern to draw non-solid line.
   *
   * By default the dash, dot line, etc has a space of few pixels, but this function can scale this spacing.
   */
  void VTKChartExtraAcquisition::SetEventLineTypeFactor(int factor)
  {
    if (this->m_EventLineTypeFactor == factor)
      return;
    this->m_EventLineTypeFactor = factor;
    if (this->Scene != NULL) this->Scene->SetDirty(true);
  };
  
  /**
   * @fn int VTKChartExtraAcquisition::GetDisplayEvents() const
   * Get the status of the events display.
   */
   
  /**
   * Enable/Disable the displaying of the events as vertical lines into the chart
   */
  void VTKChartExtraAcquisition::SetDisplayEvents(int enabled)
  {
    if (this->m_DisplayEvents == enabled)
      return;
    this->m_DisplayEvents = enabled;
    this->Modified();
  };
  
  void VTKChartExtraAcquisition::SetAxes(vtkAxis* x, vtkAxis* y)
  {
    this->SetXAxis(x);
    this->SetYAxis(y);
  };

  /**
   * Paint the item in the scene
   */
  bool VTKChartExtraAcquisition::Paint(vtkContext2D *painter)
  {
    // Should not be NULL.
    if ((this->XAxis == NULL) || (this->YAxis == NULL))
      return false;
    
    float pt1X[2]; this->XAxis->GetPoint1(pt1X);
    float pt2X[2]; this->XAxis->GetPoint2(pt2X);
    float pt2Y[2]; this->YAxis->GetPoint2(pt2Y);
    float scaleX = (pt2X[0] - pt1X[0]) / static_cast<float>(this->XAxis->GetMaximum() - this->XAxis->GetMinimum());
    
    // 5. Paint the events
    if ((this->mp_EventsFunctor != NULL) && (this->m_DisplayEvents != 0))
    {
      int inc = 0, typeId = -1, idx = 0;
      double color[3] = {0.0, 0.0, 0.0};
      while ((*this->mp_EventsFunctor)(inc++, typeId, idx, color))
      {
        int lineType = vtkPen::NO_PEN;
        switch (typeId)
        {
        case 0: // General
          lineType = vtkPen::DASH_DOT_DOT_LINE;
          break;
        case 1: // Foot Strike
          lineType = vtkPen::DASH_LINE;
          break;
        case 2: // Foot Off
          lineType = vtkPen::DASH_DOT_LINE;
          break;
        default:
          lineType = vtkPen::DOT_LINE;
        }
        float frameIndex = static_cast<float>(idx);
        if ((frameIndex >= this->XAxis->GetMinimum()) && (frameIndex <= this->XAxis->GetMaximum()))
        {
#ifdef VTK_USE_QT
          const double opacityEvent = 0.75;
          double maxColor = std::max(std::max(color[0], color[1]), color[2]);
          double scaleColor = (maxColor > opacityEvent) ? opacityEvent / maxColor : 1.0;
          unsigned char rgb[3] = {static_cast<unsigned char>(color[0] * scaleColor * 255.0), static_cast<unsigned char>(color[1] * scaleColor * 255.0), static_cast<unsigned char>(color[2] * scaleColor * 255.0)};
#else
          unsigned char rgb[3] = {static_cast<unsigned char>(color[0] * 255.0), static_cast<unsigned char>(color[1] * 255.0), static_cast<unsigned char>(color[2] * 255.0)};
#endif
          painter->GetPen()->SetLineType(lineType);
          painter->GetPen()->SetColor(rgb);
          painter->GetPen()->SetOpacity(191);
          painter->GetPen()->SetWidth(this->m_EventLineWidth);
          float valX = pt1X[0] + (frameIndex - this->XAxis->GetMinimum()) * scaleX;
          // Because VTK 5.6 doesn't give the possibilty to set the line factor given by OpenGL, it is 
          // necessary to modify the OpenGL device directly.
          vtkOpenGLContextDevice2D* device = vtkOpenGLContextDevice2D::SafeDownCast(painter->GetDevice());
          if (device != NULL)
          {
            float x[4] = {valX, pt1X[1], valX, pt2Y[1]};
            device->SetColor4(painter->GetPen()->GetColor());
            device->SetLineWidth(painter->GetPen()->GetWidth());
            device->SetPointSize(painter->GetPen()->GetWidth());
            // Code adapted to set the line type factor
            if (painter->GetPen()->GetLineType() == vtkPen::SOLID_LINE)
              glDisable(GL_LINE_STIPPLE);
            else
              glEnable(GL_LINE_STIPPLE);
            GLushort pattern = 0x0000;
            switch (painter->GetPen()->GetLineType())
            {
            case vtkPen::NO_PEN:
              pattern = 0x0000;
              break;
            case vtkPen::DASH_LINE:
              pattern = 0x00FF;
              break;
            case vtkPen::DOT_LINE:
              pattern = 0x0101;
              break;
            case vtkPen::DASH_DOT_LINE:
              pattern = 0x0C0F;
              break;
            case vtkPen::DASH_DOT_DOT_LINE:
              pattern = 0x1C47;
              break;
            default:
              pattern = 0x0000;
            }
            glLineStipple(this->m_EventLineTypeFactor, pattern);
            // Draw the line
            device->DrawPoly(&x[0], 2);
          }
          else
            painter->DrawLine(valX, pt1X[1], valX, pt2Y[1]);
        }
      }
      // The line stipple must be disabled. Otherwise, under Windows XP, the next drawn line will use it (even if another pen is given).
      glDisable(GL_LINE_STIPPLE);
      glLineStipple(1, 0x0000);
    }
    // 6. Paint the frame line and its bounds
    // 6.1 Bounds
    if (this->mp_RegionOfInterestFunctor != NULL)
    {
      painter->GetPen()->SetLineType(vtkPen::NO_PEN);
      painter->GetBrush()->SetColor(0, 127, 255, 64);
      int lbi = 0, rbi = 0;
      (*this->mp_RegionOfInterestFunctor)(lbi, rbi);
      float left = (static_cast<float>(lbi) - this->XAxis->GetMinimum()) * scaleX;
      float limit = pt2X[0]-pt1X[0];
      if (left >= limit)
        left = limit;
      float right = (this->XAxis->GetMaximum() - static_cast<float>(rbi)) * scaleX;
      if (right >= limit)
        right = limit;
      if ((left > 0.0f) && (lbi > 0))
        painter->DrawRect(pt1X[0],pt1X[1],left,pt2Y[1]-pt1X[1]);
      if ((right > 0.0f) && (rbi > 0))
        painter->DrawRect(pt2X[0]-right,pt2X[1],right,pt2Y[1]-pt1X[1]);
    }
    // 6.2 Frame line
    if (this->mp_CurrentFrameFunctor != NULL)
    {
      float frameIndex = static_cast<float>((*this->mp_CurrentFrameFunctor)());
      if ((frameIndex >= this->XAxis->GetMinimum()) && (frameIndex <= this->XAxis->GetMaximum()))
      {
        painter->GetPen()->SetLineType(vtkPen::SOLID_LINE);
        painter->GetPen()->SetColor(0, 127, 255, 191);
        painter->GetPen()->SetWidth(3.0);
        float valX = pt1X[0] + (frameIndex - this->XAxis->GetMinimum()) * scaleX;
        painter->DrawLine(valX, pt1X[1], valX, pt2Y[1]);
      }
    }
    
    return true;
  };
};
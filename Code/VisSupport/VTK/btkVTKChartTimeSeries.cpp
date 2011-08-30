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

#include "btkVTKChartTimeSeries.h"
#include "btkMacro.h"

#include <vtkObjectFactory.h>
#include <vtkChartLegend.h>
#include <vtkAxis.h>
#include <vtkPlot.h>
#include <vtkPlotGrid.h>
#include <vtkColorSeries.h>
#include <vtkContextScene.h>
#include <vtkPoints2D.h>
#include <vtkContext2D.h>
#include <vtkColor.h>
#include <vtkPlotLine.h>
#include <vtkPen.h>
#include <vtkBrush.h>
#include <vtkTransform2D.h>
#include <vtkContextDevice2D.h>
#include <vtkTextProperty.h>
#include <vtkOpenGLContextDevice2D.h>

#include <vtkstd/list>
#include <vtkgl.h>

namespace btk
{
  /**
   * @class VTKCurrentFrameFunctor btkVTKChartTimeSeries.h
   * @brief Functor to get easily to this chart the current frame displayed.
   *
   * This is usefull when combined with a timer or other view, like a 3D view.
   */
  /**
   * @typedef VTKCurrentFrameFunctor::Pointer
   * Smart pointer associated with a VTKCurrentFrameFunctor object.
   */
  /**
   * @fn virtual int VTKCurrentFrameFunctor::operator()() = 0;
   * Operator used to return the current frame displayed.
   */
   
  /**
   * @class VTKRegionOfInterestFunctor btkVTKChartTimeSeries.h
   * @brief Functor to get easily to this chart the region of interest of the time series.
   */
  /**
   * @typedef VTKRegionOfInterestFunctor::Pointer
   * Smart pointer associated with a VTKCurrentFrameFunctor object.
   */
  /**
   * @fn virtual void VTKRegionOfInterestFunctor::operator()(int& left, int& right) = 0;
   * Operator used to get the left and right bounds of the region of interest.
   */
   
  /**
   * @class VTKEventsFunctor btkVTKChartTimeSeries.h
   * @brief Functor to get easily to this chart the events as types, frames and colors.
   *
   * The types are represented by integer where the values 0, 1 and 2 are for 
   * "General", "Foot strike" and "Foot off" respectively. All of the ohers values have no specific meanings.
   */
  /**
   * @typedef VTKEventsFunctor::Pointer
   * Smart pointer associated with a VTKCurrentFrameFunctor object.
   */
  /**
   * @fn virtual bool VTKEventsFunctor::operator()(int index, int& typeId, int& frame, double rgb[3]) = 0;
   * Operator used to extract each event. Asking for an event out of range returns false.
   */
  
  /**
   * @class VTKPlots btkVTKChartTimeSeries.h
   * @brief List of pointer to vtkPlot objects.
   */
  class VTKPlots: public vtkstd::list<vtkPlot*>
  {};
  
  /**
   * @class VTKChartTimeSeries btkVTKChartTimeSeries.h
   * @brief Chart for the time series data with only a bottom and left axes.
   *
   * This chart has several interaction to constraint pan and zoom
   *  - Select only a horizontal or vertical zoom;
   *  - Pan the chart only into boundaries;
   *
   * You can also enable/disable the interactions .
   *
   * You have also the possibilty to display the current frame and the region of interest of the time series, as well as events.
   * To be able to display these informations, you have to create functors inheriting from VTKCurrentFrameFunctor,  
   * VTKRegionOfInterestFunctor and VTKEventsFunctor. Then set the functor by using the methods SetCurrentFrameFunctor(), SetRegionOfInterestFunctor() and SetEventsFunctor.
   *
   * The events are optional and hidden by default (and there is no functor). To display them, and after setting the functor, you have to use the function DisplayEventsOn().
   *
   * This class take into account the transformation given to the object to paint correctly the legend into the scene (only the scale and translation are supported).
   *
   * NOTE: The vtkAnnotationLink object is not taken into account during the painting.
   * 
   * @ingroup BTKVTK
   */
   
  /**
   * @fn static VTKChartTimeSeries* VTKChartTimeSeries::New()
   * Constructs a VTKChartTimeSeries object and return it as a pointer.
   */
  vtkStandardNewMacro(VTKChartTimeSeries);
  vtkCxxRevisionMacro(VTKChartTimeSeries, "$Revision: 0.1 $");
  
  /**
   * @fn bool VTKChartTimeSeries::GetClippingEnabled() const
   * Returns the status of the clippling for the plots (cannot be drawn outside of the axis.)
   */
   
  /**
   * @fn void VTKChartTimeSeries::SetClippingEnabled(bool enabled)
   * Sets the status of the clippling for the plots (cannot be drawn outside of the axis.)
   */
    
  /**
   * @fn bool VTKChartTimeSeries::GetInteractionEnabled() const
   * Get the status of the user interactions (move & zoom).
   */
   
  /**
   * @fn void VTKChartTimeSeries::SetInteractionEnabled(bool enabled)
   * Enable / disable the user interactions (move & zoom).
   */
  
  /**
   * @fn int VTKChartTimeSeries::GetZoomMode() const
   * Returns the zoom mode (Both directions (BOTH): 0, only horizontal (HORIZONTAL): 1, only vertical (VERTICAL): 2)
   */
   
  /**
   * @fn void VTKChartTimeSeries::SetZoomMode(int mode)
   * Sets the zoom mode (Both directions (BOTH): 0, only horizontal (HORIZONTAL): 1, only vertical (VERTICAL): 2)
   */
  
  /**
   * @fn bool VTKChartTimeSeries::GetBoundsEnabled()
   * Get the status for the use of the bounds on the chart's axes.
   */

  /**
   * @fn void VTKChartTimeSeries::SetBoundsEnabled(bool enabled)
   * Enable / disable the use of the bounds on the chart's axes.
   */
  
  /**
   * @fn double* VTKChartTimeSeries::GetBounds()
   * Returns the boundaries set for the charts as xMin, xMax, yMin, yMax.
   *
   * If this is enabled (see the method SetBoundsEnabled()), then the zoom interaction will be limited by these bounds. 
   */
  
  /**
   * @fn void VTKChartTimeSeries::SetBounds(double xMin, double xMax, double yMin, double yMax)
   * Convenient method to set the chart's bounds.
   */
  
  /**
   * Sets the boundaries for the charts.
   * The input format is xMin, xMax, yMin, yMax.
   * Need to be enabled (see the method SetBoundsEnabled()) to constraint the interactions on the chart.
   */
  void VTKChartTimeSeries::SetBounds(double bounds[4])
  {
    if ((bounds[0] == this->mp_Bounds[0]) && (bounds[1] == this->mp_Bounds[1]) && (bounds[2] == this->mp_Bounds[2]) && (bounds[3] == this->mp_Bounds[3]))
      return;
    
    this->mp_Bounds[0] = bounds[0];
    this->mp_Bounds[1] = bounds[1];
    this->mp_Bounds[2] = bounds[2];
    this->mp_Bounds[3] = bounds[3];
    
    this->mp_AxisX->SetRange(bounds[0], bounds[1]);
    this->mp_AxisY->SetRange(bounds[2], bounds[3]);
    
    this->m_ChartBoundsValid = true;
    this->m_PlotsTransformValid = false;
    this->Scene->SetDirty(true);
  };
  
  /**
   * Recalculates the bounds of the chart, and then of its axes.
   */
  void VTKChartTimeSeries::RecalculateBounds()
  {
    this->Update();
    
    double x[2] = {std::numeric_limits<double>::max(), std::numeric_limits<double>::min()};
    double y[2] = {x[0], x[1]};
    double bounds[4] = { 0.0, 0.0, 0.0, 0.0 };
    bool validBounds = false;
    for (vtkstd::list<vtkPlot*>::iterator it = this->mp_Plots->begin() ; it != this->mp_Plots->end() ; ++it)
    {
      if (!(*it)->GetVisible())
        continue;
      (*it)->GetBounds(bounds);
      x[0] = std::min(x[0], bounds[0]);
      x[1] = std::max(x[1], bounds[1]);
      y[0] = std::min(y[0], bounds[2]);
      y[1] = std::max(y[1], bounds[3]);
      validBounds = true;
    }
    if (!validBounds) // No (visible) plot
    {
      x[0] = 0.0; x[1] = 0.0;
      y[0] = 0.0; y[1] = 0.0;
    }
    
    // The X axis is set to fit exactly the time range
    // The Y axis is larger to see correctly the minimum and maximum values.
    // +/- 5% of the range is added.
    double r = (y[1] - y[0]) / 20.0;
    y[0] -= r; y[1] += r;
    
    this->SetBounds(x[0], x[1], y[0], y[1]);
  };
  
  /**
   * Sets the legend
   */
  void VTKChartTimeSeries::SetLegend(vtkChartLegend* legend)
  {
    if (this->mp_Legend == legend)
      return;
    else if (this->mp_Legend != 0)
      this->mp_Legend->Delete();
    this->mp_Legend = legend;
    this->mp_Legend->SetChart(this);
    this->mp_Legend->Register(this);
  };
  
  /**
   * @fn vtkChartLegend* VTKChartTimeSeries::GetLegend()
   * Returns the legend (NULL by default).
   */
  
  /**
   *  Sets the generator of colors.
   */
  void VTKChartTimeSeries::SetColorSeries(vtkColorSeries* colors)
  {
    if (this->mp_Colors == colors)
      return;
    else if (this->mp_Colors != 0)
      this->mp_Colors->Delete();
    this->mp_Colors = colors;
    this->mp_Colors->Register(this);
  };
  
  /**
   * @fn vtkColorSeries* VTKChartTimeSeries::GetColorSeries()
   * Returns the generator of colors (NULL by default).
   */
   
  /**
   * @fn int VTKChartTimeSeries::GetTitleMargin() const
   * Returns the margin between the border and the title.
   */
  
  /**
   * Sets the margin between the border and the title.
   */ 
  void VTKChartTimeSeries::SetTitleMargin(int margin)
  {
    if (this->m_TitleMargin == margin)
      return;
    this->m_TitleMargin = margin;
    this->Scene->SetDirty(true);
  };
  
  /**
   * @fn const int* VTKChartTimeSeries::GetBorders() const
   * Returns the borders as an array of 4 elements. The order is left, bottom. right, top.
   */
  
  /**
   * Modify the size of the borders. This method doesn't request to repaint the chart.
   */
  void VTKChartTimeSeries::SetBorders(int left, int bottom, int right, int top)
  {
    if ((this->mp_Borders[0] == left) && (this->mp_Borders[1] == bottom) && (this->mp_Borders[2] == right) && (this->mp_Borders[3] == top))
      return;
    this->vtkChart::SetBorders(left, bottom, right, top);
    // Set the borders value after as the method SetBorders(), check the given value and modify them if necessary
    this->mp_Borders[0] = this->Point1[0]; // Left
    this->mp_Borders[1] = this->Point1[1]; // Bottom
    this->mp_Borders[2] = this->Geometry[0] - this->Point2[0]; // Right
    this->mp_Borders[3] = this->Geometry[1] - this->Point2[1]; // Top
    this->mp_AxisX->SetPoint1(this->mp_Borders[0], this->mp_Borders[1]);
    this->mp_AxisY->SetPoint1(this->mp_Borders[0], this->mp_Borders[1]);
    this->mp_AxisX->SetPoint2(this->mp_Borders[2], this->mp_Borders[1]);
    this->mp_AxisY->SetPoint2(this->mp_Borders[0], this->mp_Borders[3]);
    this->m_BordersChanged = true;
  };
  
  /**
   * @fn VTKCurrentFrameFunctor::Pointer VTKChartTimeSeries::GetCurrentFrameFunctor() const
   * Returns the functor used to know the current frame to display.
   */
  
  /**
   * Sets the functor used to know the current frame to display.
   */
  void VTKChartTimeSeries::SetCurrentFrameFunctor(VTKCurrentFrameFunctor::Pointer functor)
  {
    this->mp_CurrentFrameFunctor = functor;
  };
  
  /**
   * @fn VTKRegionOfInterestFunctor::Pointer VTKChartTimeSeries::GetRegionOfInterestFunctor() const
   * Returns the functor used to know the region of interest to display.
   */
  
  /**
   * Sets the functor used to know the region of interest to display.
   */
  void VTKChartTimeSeries::SetRegionOfInterestFunctor(VTKRegionOfInterestFunctor::Pointer functor)
  {
    this->mp_RegionOfInterestFunctor = functor;
  };
  
  /**
   * @fn VTKEventsFunctor::Pointer VTKChartTimeSeries::GetEventsFunctor() const
   * Returns the functor used to know events' informations.
   */
  
  /**
   * Sets the functor used to know events' informations.
   */
  void VTKChartTimeSeries::SetEventsFunctor(VTKEventsFunctor::Pointer functor)
  {
    this->mp_EventsFunctor = functor;
  };
  
  /**
   * @fn float VTKChartTimeSeries::GetEventLineWidth() const
   * Returns the width of the vertical line used to display the events.
   */
  
  /**
   * Sets the width of the vertical line used to display the events.
   */
  void VTKChartTimeSeries::SetEventLineWidth(float width)
  {
    if (this->m_EventLineWidth == width)
      return;
    this->m_EventLineWidth = width;
    this->Scene->SetDirty(true);
  };
  
  /**
   * @fn int VTKChartTimeSeries::GetEventLineTypeFactor() const
   * Returns the factor used in the spacing of the elements in the pattern to draw non-solid line.
   *
   * By default the dash, dot line, etc has a space of few pixels, but this function can scale this spacing.
   */
  
  /**
   * Sets the factor used in the spacing of the elements in the pattern to draw non-solid line.
   *
   * By default the dash, dot line, etc has a space of few pixels, but this function can scale this spacing.
   */
  void VTKChartTimeSeries::SetEventLineTypeFactor(int factor)
  {
    if (this->m_EventLineTypeFactor == factor)
      return;
    this->m_EventLineTypeFactor = factor;
    this->Scene->SetDirty(true);
  };
  
  /**
   * @fn int VTKChartTimeSeries::GetDisplayEvents() const
   * Get the status of the events display.
   */
   
  /**
   * Enable/Disable the displaying of the events as vertical lines into the chart
   */
  void VTKChartTimeSeries::SetDisplayEvents(int enabled)
  {
    if (this->m_DisplayEvents == enabled)
      return;
    this->m_DisplayEvents = enabled;
    this->Modified();
  };
  
  /**
   * @fn void VTKChartTimeSeries::DisplayEventsOn()
   * Enable the displaying of the events as vertical lines into the chart
   */
  
  /**
   * @fn void VTKChartTimeSeries::DisplayEventsOff()
   * Disable the displaying of the events as vertical lines into the chart
   */
   
  /**
   * Update the content of the chart which is not graphical. This function is called by the method Paint().
   */
  void VTKChartTimeSeries::Update()
  {
    for (vtkstd::list<vtkPlot*>::iterator it = this->mp_Plots->begin() ; it != this->mp_Plots->end() ; ++it)
      (*it)->Update();
    if (this->mp_Legend && this->ShowLegend)
      this->mp_Legend->Update();
  };
  
  /**
   * Paint the contents of the chart in the scene.
   */
  bool VTKChartTimeSeries::Paint(vtkContext2D *painter)
  {
    int geometry[] = {this->GetScene()->GetSceneWidth(), this->GetScene()->GetSceneHeight()};
    // Do we have a scene with a valid geometry?
    if ((geometry[0] == 0) || (geometry[1] == 0))
      return false;
    
    // Update the content of the plots, legend, etc. 
    this->Update();
    
    // Update the chart's geometry if necessary
    if ((geometry[0] != this->Geometry[0]) || (geometry[1] != this->Geometry[1]) || this->m_BordersChanged)
    {
      this->SetGeometry(geometry);
      this->vtkChart::SetBorders(this->mp_Borders[0], this->mp_Borders[1], this->mp_Borders[2], this->mp_Borders[3]);
      // But also the axis length
      this->mp_AxisX->SetPoint1(this->Point1[0], this->Point1[1]);
      this->mp_AxisX->SetPoint2(this->Point2[0], this->Point1[1]);
      this->mp_AxisY->SetPoint1(this->Point1[0], this->Point1[1]);
      this->mp_AxisY->SetPoint2(this->Point1[0], this->Point2[1]);
      // And the legend (reset to the right top corner)
      if (this->mp_Legend != 0)
      {
        this->mp_Legend->SetHorizontalAlignment(vtkChartLegend::RIGHT);
        this->mp_Legend->SetVerticalAlignment(vtkChartLegend::TOP);
        this->mp_Legend->SetPoint(this->Point2[0], this->Point2[1]);
      }
      this->m_BordersChanged = false;
      this->m_PlotsTransformValid = false;
    }
    
    // Update the axes
    if (!this->m_ChartBoundsValid)
      this->RecalculateBounds();
    if (!this->m_PlotsTransformValid)
      this->RecalculatePlotsTransform();
    this->mp_AxisX->Update();
    this->mp_AxisY->Update();
    
    // Draw the items used in the chart. The order is important.
    // 0. Init (values used later)
    float pt1X[2]; this->mp_AxisX->GetPoint1(pt1X);
    float pt2X[2]; this->mp_AxisX->GetPoint2(pt2X);
    float pt2Y[2]; this->mp_AxisY->GetPoint2(pt2Y);
    float scaleX = (pt2X[0] - pt1X[0]) / static_cast<float>(this->mp_AxisX->GetMaximum() - this->mp_AxisX->GetMinimum());
    // 1. The grid is in the back
    this->mp_Grid->Paint(painter);
    // 2. Clip the painting area between the axes
    if (this->m_ClippingEnabled)
    {
      float clipF[4] = {this->Point1[0], this->Point1[1], this->Point2[0]-this->Point1[0], this->Point2[1]-this->Point1[1] };
      // 2.1 Check if the scene has a transform and use it
      if (this->Scene->HasTransform())
        this->Scene->GetTransform()->InverseTransformPoints(clipF, clipF, 2);
      int clip[4] = {(int)clipF[0], (int)clipF[1], (int)clipF[2], (int)clipF[3]};
      painter->GetDevice()->SetClipping(clip);
    }
    // 3. Paint the plots
#if 1
    painter->PushMatrix();
    painter->AppendTransform(this->mp_PlotsTransform);
    for (vtkstd::list<vtkPlot*>::iterator it = this->mp_Plots->begin() ; it != this->mp_Plots->end() ; ++it)
      (*it)->Paint(painter);
#else
    bool markerDisplayed = (scaleX > 30.0f); // 30 pixels by unit. FIXME: NOT ENOUGH FOR ANALOG DATA WITH BIGGER SAMPLE RATE
    painter->PushMatrix();
    painter->AppendTransform(this->mp_PlotsTransform);
    for (vtkstd::list<vtkPlot*>::iterator it = this->mp_Plots->begin() ; it != this->mp_Plots->end() ; ++it)
    {
      vtkPlotPoints* points = vtkPlotPoints::SafeDownCast(*it);
      if (points != 0)
        points->SetMarkerStyle(markerDisplayed ? vtkPlotPoints::CIRCLE : vtkPlotPoints::NONE);
      (*it)->Paint(painter);
    }
#endif
    painter->PopMatrix();
    // 4. Disable cliping
    painter->GetDevice()->DisableClipping();
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
        if ((frameIndex >= this->mp_AxisX->GetMinimum()) && (frameIndex <= this->mp_AxisX->GetMaximum()))
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
          float valX = pt1X[0] + (frameIndex - this->mp_AxisX->GetMinimum()) * scaleX;
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
    }
    // 6. Paint the frame line and its bounds
    // 6.1 Bounds
    if (this->mp_RegionOfInterestFunctor != NULL)
    {
      painter->GetPen()->SetLineType(vtkPen::NO_PEN);
#ifdef VTK_USE_QT
      // For the record: If you compile VTK with the support of Qt, then VTK will use
      // the vtkQtLabelRenderStrategy class to render the text in the scene. However, 
      // to do this, they use an image cache mechanism based on images using the format 
      // Qt::Format_ARGB32_Premultiplied. As written in the Qt documentation, using this 
      // format implies that the RGB values cannot be greater than the alpha value, 
      // otherwise, the behavior is unknown. In this case, we have to normalized the RGB 
      // value by the alpha value.
      painter->GetBrush()->SetColor(0, 32, 64, 64);
#else
      painter->GetBrush()->SetColor(0, 127, 255, 64);
#endif
      int lbi, rbi;
      (*this->mp_RegionOfInterestFunctor)(lbi, rbi);
      float left = static_cast<float>(lbi) - this->mp_AxisX->GetMinimum();
      float right = this->mp_AxisX->GetMaximum() - static_cast<float>(rbi);
      if (left > 0.0f)
        painter->DrawRect(pt1X[0],pt1X[1],left*scaleX,pt2Y[1]-pt1X[1]);
      if (right > 1.0f)
        painter->DrawRect(pt2X[0]-right*scaleX,pt2X[1],right*scaleX,pt2Y[1]-pt1X[1]);
    }
    // 6.2 Frame line
    if (this->mp_CurrentFrameFunctor != NULL)
    {    
      float frameIndex = static_cast<float>((*this->mp_CurrentFrameFunctor)());
      if ((frameIndex >= this->mp_AxisX->GetMinimum()) && (frameIndex <= this->mp_AxisX->GetMaximum()))
      {
        painter->GetPen()->SetLineType(vtkPen::SOLID_LINE);
#ifdef VTK_USE_QT
        painter->GetPen()->SetColor(0, 95, 191, 191); // Normalized
#else
        painter->GetPen()->SetColor(0, 127, 255, 191);
#endif
        painter->GetPen()->SetWidth(3.0);
        float valX = pt1X[0] + (frameIndex - this->mp_AxisX->GetMinimum()) * scaleX;
        painter->DrawLine(valX, pt1X[1], valX, pt2Y[1]);
      }
    }
    // 7. The axes
    this->mp_AxisX->Paint(painter);
    this->mp_AxisY->Paint(painter);
    // 8. The legend
    if (this->mp_Legend && this->ShowLegend)
      this->mp_Legend->Paint(painter);
    // 9. The title
    if (this->Title)
    {
      painter->ApplyTextProp(this->TitleProperties);
      float pt[2] = {(this->Point2[0]+this->mp_Borders[2]) / 2, this->Point2[1] + this->mp_Borders[3] - this->m_TitleMargin};
      if (this->GetTransform() != NULL)
        this->GetTransform()->TransformPoints(pt, pt, 1);
      painter->DrawString(pt[0], pt[1], this->Title);
    }
    
    return true;
  };
  
  /**
   * Add a new plot to the chart. Only the type LINE is supported.
   */
  vtkPlot* VTKChartTimeSeries::AddPlot(int type)
  {
    vtkPlot* plot = NULL;
    vtkColor3ub color(0,0,0); // Black by default
    if (this->mp_Colors != 0)
      color = this->mp_Colors->GetColorRepeating(static_cast<int>(this->mp_Plots->size()));
    switch (type)
    {
    case LINE:
      {
      vtkPlotLine *line = vtkPlotLine::New();
      line->GetPen()->SetColor(color.GetData());
      plot = line;
      break;
      }
    case POINTS:
    case BAR:
    default:
      btkErrorMacro("Only the plot type LINE is supported by this chart");
    }
    this->AddPlot(plot);
    return plot;
  };
  
  /**
   * Add the plot previously defined and return its index.
   */
  vtkIdType VTKChartTimeSeries::AddPlot(vtkPlot* plot)
  {
    if (plot == NULL)
       return -1;
     plot->SetXAxis(this->mp_AxisX);
     plot->SetYAxis(this->mp_AxisY);
     this->mp_Plots->push_back(plot);
     vtkIdType plotIndex = this->mp_Plots->size() - 1;
     // Ensure that the bounds of the chart are updated to contain the new plot
     this->m_ChartBoundsValid = false;
     // Mark the scene as dirty to update it.
     this->Scene->SetDirty(true);
    return plotIndex;
  };
  
  /**
   * Removes a plot with the index @a index and request to update the bounds.
   */
  bool VTKChartTimeSeries::RemovePlot(vtkIdType index)
  {
    if (static_cast<vtkIdType>(this->mp_Plots->size()) > index)
    {
      vtkstd::list<vtkPlot*>::iterator it = this->mp_Plots->begin();
      vtkstd::advance(it, index);
      (*it)->Delete();
      this->mp_Plots->erase(it);
      // Ensure that the bounds of the chart are updated to fit well the plots
      this->m_ChartBoundsValid = false;
      // Mark the scene as dirty
      this->Scene->SetDirty(true);
      return true;
    }
    return false;
  };
  
  /**
   * Removes all the plots and reset all the bounds to 0
   */
  void VTKChartTimeSeries::ClearPlots()
  {
    for (vtkstd::list<vtkPlot*>::iterator it = this->mp_Plots->begin() ; it != this->mp_Plots->end() ; ++it)
      (*it)->Delete();
    this->mp_Plots->clear();
    this->m_ChartBoundsValid = false;
    this->Scene->SetDirty(true);
  };
  
  /**
   * Returns the plot at the index @a index.
   */
  vtkPlot* VTKChartTimeSeries::GetPlot(vtkIdType index)
  {
    vtkstd::list<vtkPlot*>::iterator it = this->mp_Plots->begin();
    vtkstd::advance(it, index);
    if (it == this->mp_Plots->end())
    {
      btkErrorMacro("The given index exceed the number of plots");
      return NULL;
    }
    return *it;
  };
  
  /**
   * Returns the number of plots in the chart.
   */
  vtkIdType VTKChartTimeSeries::GetNumberOfPlots()
  {
    return this->mp_Plots->size();
  };
  
  /**
   * Returns the axis baed on the index @a axisIndex.
   * Only the axes BOTTOM and LEFT are supported.
   */
  vtkAxis* VTKChartTimeSeries::GetAxis(int axisIndex)
  {
    vtkAxis* axis = 0;
    switch (axisIndex)
    {
    case 0: // Left
      axis = this->mp_AxisY;
      break;
    case 1: // Bottom
      axis = this->mp_AxisX;
      break;
    default:
      btkErrorMacro("Only two axes are available with this chart: LEFT and BOTTOM.");
    }
    return axis;
  };
  
  /**
   * Returns the number of axes in the chart.
   */
  vtkIdType VTKChartTimeSeries::GetNumberOfAxes()
  {
    return 2;
  };
  
  /**
   * Sets the vtkContextScene for the chart and its axis.
   */
  void VTKChartTimeSeries::SetScene(vtkContextScene* scene)
  {
    this->vtkContextItem::SetScene(scene);
    this->mp_AxisX->SetScene(scene);
    this->mp_AxisY->SetScene(scene);
  };
  
  /**
   * Return true if the supplied x, y coordinate is inside the item.
   * Required for the MouseWheelEvent() method.
   */
  bool VTKChartTimeSeries::Hit(const vtkContextMouseEvent &mouse)
  {
    if ((mouse.ScreenPos[0] > this->Point1[0]) && (mouse.ScreenPos[0] < this->Point2[0])
     && (mouse.ScreenPos[1] > this->Point1[1]) && (mouse.ScreenPos[1] < this->Point2[1]))
      return true;
    else
      return false;
  };
  
  /**
   * Overloaded method to move the chart only if the user interaction are enabled.
   */
  bool VTKChartTimeSeries::MouseMoveEvent(const vtkContextMouseEvent& mouse)
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
        // this->GetAxis(mai+2)->SetRange(min, max);
      }
      
      // this->RecalculatePlotsTransform();
      this->m_PlotsTransformValid = false;
      this->Scene->SetDirty(true);
    }
    return true;
  };
  
  /**
   * Overloaded method to zoom in/out on the axis(es) specified by the zoom mode.
   */
  bool VTKChartTimeSeries::MouseWheelEvent(const vtkContextMouseEvent& mouse, int delta)
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
          // vtkAxis* axis2 = this->GetAxis(mai+2);
          // axis2->SetRange(min,max);
          axis->RecalculateTickSpacing();
          // axis2->RecalculateTickSpacing();
        }
      }      
      // this->RecalculatePlotsTransform();
      this->m_PlotsTransformValid = false;
      this->Scene->SetDirty(true);
    }
    return true;
  };
  
  VTKChartTimeSeries::VTKChartTimeSeries()
  : vtkChart(), mp_CurrentFrameFunctor(), mp_RegionOfInterestFunctor(), mp_EventsFunctor()
  {
    // No legend by defaut.
    this->mp_Legend = 0;
    
    // No generator of color by default
    this->mp_Colors = 0;
    
    // Only two axes (bottom: X axis, left: Y axis)
    this->mp_AxisX = VTKAxis::New();
    this->mp_AxisX->SetPosition(vtkAxis::BOTTOM);
    this->mp_AxisX->SetTitle("X Axis");
    this->mp_AxisX->SetLabelMargin(7.0f);
    this->mp_AxisX->SetVisible(true);
    this->mp_AxisY = VTKAxis::New();
    this->mp_AxisY->SetPosition(vtkAxis::LEFT);
    this->mp_AxisY->SetTitle("Y Axis");
    this->mp_AxisY->SetVisible(true);
    // By default, they will be displayed with a null range and their behavior is fixed
    this->mp_AxisX->SetBehavior(1); // Fixed
    this->mp_AxisX->SetRange(0.0, 0.0);
    this->mp_AxisY->SetBehavior(1); // Fixed
    this->mp_AxisY->SetRange(0.0, 0.0);
    
    // Grid using the defined axes
    this->mp_Grid = vtkPlotGrid::New();
    this->mp_Grid->SetXAxis(this->mp_AxisX);
    this->mp_Grid->SetYAxis(this->mp_AxisY);
    
    // Interaction and bounds
    this->m_ClippingEnabled = true;
    this->m_InteractionEnabled = true;
    this->m_ZoomMode = 0;
    this->m_BoundsEnabled = false;
    this->mp_Bounds[0] = 0.0;
    this->mp_Bounds[1] = 0.0;
    this->mp_Bounds[2] = 0.0;
    this->mp_Bounds[3] = 0.0;
    this->m_ChartBoundsValid = true;
    
    // Borders
    this->mp_Borders[0] = 0;
    this->mp_Borders[1] = 0;
    this->mp_Borders[2] = 0;
    this->mp_Borders[3] = 0;
    
    // Plots
    this->mp_Plots = new VTKPlots();
    
    // Display events disabled by defaut (no functor)
    this->m_DisplayEvents = 0;
    this->m_EventLineWidth = 0.5f;
    this->m_EventLineTypeFactor = 1;
    
    // Linear transformation to scale and translate the plots
    this->mp_PlotsTransform = vtkTransform2D::New();
    this->m_PlotsTransformValid = true;
    
    // Title
    this->TitleProperties->SetVerticalJustificationToTop();
    this->m_TitleMargin = 10;
    
    // Defaut borders for the chart
    this->m_BordersChanged = false;
    this->SetBorders(60, 50, 20, 20); 
  };
  
  VTKChartTimeSeries::~VTKChartTimeSeries()
  {
    for (vtkstd::list<vtkPlot*>::iterator it = this->mp_Plots->begin() ; it != this->mp_Plots->end() ; ++it)
      (*it)->Delete();
    delete this->mp_Plots;
    if (this->mp_Legend)
      this->mp_Legend->Delete();
    if (this->mp_Colors)
      this->mp_Colors->Delete();
    this->mp_AxisX->Delete();
    this->mp_AxisY->Delete();
    this->mp_Grid->Delete();
    this->mp_PlotsTransform->Delete();
  };
  
  /**
   * Update the size of the plot into the dimensions of the scene.
   */
  void VTKChartTimeSeries::RecalculatePlotsTransform()
  {
    // Compute the scales for the plot area to fit the plot inside
    float min[2], max[2];
    // Axis X
    if (this->mp_AxisX->GetMaximum() == this->mp_AxisX->GetMinimum())
      return;
    this->mp_AxisX->GetPoint1(min);
    this->mp_AxisX->GetPoint2(max);
    double scaleX = static_cast<double>(max[0] - min[0]) / (this->mp_AxisX->GetMaximum() - this->mp_AxisX->GetMinimum());
    // Axis Y
    if (this->mp_AxisY->GetMaximum() == this->mp_AxisY->GetMinimum())
      return;
    this->mp_AxisY->GetPoint1(min);
    this->mp_AxisY->GetPoint2(max);
    double scaleY = static_cast<double>(max[1] - min[1]) / (this->mp_AxisY->GetMaximum() - this->mp_AxisY->GetMinimum());

    this->mp_PlotsTransform->Identity();
    this->mp_PlotsTransform->Translate(this->Point1[0], this->Point1[1]);
    this->mp_PlotsTransform->Scale(scaleX, scaleY);
    this->mp_PlotsTransform->Translate(-this->mp_AxisX->GetMinimum(), -this->mp_AxisY->GetMinimum());
    
    this->m_PlotsTransformValid = true;
  };
};
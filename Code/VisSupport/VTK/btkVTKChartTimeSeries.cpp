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
#include <vtkTable.h>
#include <vtkDoubleArray.h>
#include <vtkMath.h>
#include <vtkContextMouseEvent.h>
#if (((VTK_MAJOR_VERSION == 5) && (VTK_MINOR_VERSION >= 10)) || (VTK_MAJOR_VERSION >= 6))
  #include <vtkContextKeyEvent.h>
#endif

#include <vtkstd/list>
#include <vtkgl.h>

#include <limits>

#include "btkConvert.h"

namespace btk
{
  /**
   * @struct VTKChartPlotData
   * @brief Information used by InvokeEvent within VTK. 
   *
   * @note From VTK 5.10: Small struct used by InvokeEvent to send some information about the point
   * that was clicked on. This is an experimental part of the API, subject to change.
   */
  /**
   * @var VTKChartPlotData::SeriesName
   * Name of the extracted serie (plot) ().
   */
  /**
   * @var VTKChartPlotData::Position
   * Coordinates in the chart coordinates system
   */
  /**
   * @var VTKChartPlotData::ScreenPosition
   * Coordinates in the screen coordinates system
   */
  /**
   * @var VTKChartPlotData::Index
   * Index of the serie (plot).
   */

  /**
   * @class VTKChartTimeSeries::VTKPlots btkVTKChartTimeSeries.h
   * @brief List of pointer to vtkPlot objects.
   */
  class VTKChartTimeSeries::VTKPlots: public vtkstd::list<vtkPlot*>
  {};
  
  /**
   * @class VTKChartTimeSeries btkVTKChartTimeSeries.h
   * @brief Chart for the time series data with only a bottom and left axes.
   *
   * This chart has several interaction to constraint pan and zoom
   *  - Select only a horizontal or vertical zoom;
   *  - Pan the chart only into boundaries;
   *  - Zoom box activated by the key SHIFT.
   *
   * You can also enable/disable the interactions .
   *
   * You have also the possibilty to display the current frame and the region of interest of the time series, as well as events.
   * To be able to display these informations, you have to create functors inheriting from VTKCurrentFrameFunctor,  
   * VTKRegionOfInterestFunctor and VTKEventsFunctor. Then set the functor by using the methods SetCurrentFrameFunctor(), SetRegionOfInterestFunctor() and SetEventsFunctor.
   *
   * The events are optional and hidden by default (and there is no functor). To display them, and after setting the functor, you have to use the function DisplayEventsOn().
   * 
   * @ingroup BTKVTK
   */
   
  /**
   * @fn static VTKChartTimeSeries* VTKChartTimeSeries::New()
   * Constructs a VTKChartTimeSeries object and return it as a pointer.
   */
  vtkStandardNewMacro(VTKChartTimeSeries);
  
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
   * @fn void VTKChartTimeSeries::SetBounds(double xMin, double xMax, double yMin, double yMax)
   * Set the min/max limit of each axis.
   */
  
  /**
   * Method to set the min/max limit of each axis.
   */
  void VTKChartTimeSeries::SetBounds(double bounds[4])
  {
    this->mp_AxisX->SetMinimumLimit(bounds[0]);
    this->mp_AxisX->SetMaximumLimit(bounds[1]);
    this->mp_AxisY->SetMinimumLimit(bounds[2]);
    this->mp_AxisY->SetMaximumLimit(bounds[3]);
    this->mp_AxisX->SetRange(this->mp_AxisX->GetMinimumLimit(), this->mp_AxisX->GetMaximumLimit());
    this->mp_AxisY->SetRange(this->mp_AxisY->GetMinimumLimit(), this->mp_AxisY->GetMaximumLimit());
    this->m_PlotsTransformValid = false;
    if (this->Scene != NULL) this->Scene->SetDirty(true);
  };
  
  /**
   * Recalculates the bounds of the chart, and then of its axes.
   */
  void VTKChartTimeSeries::RecalculateBounds()
  {
    double x[2] = {std::numeric_limits<double>::max(), -1.0*std::numeric_limits<double>::max()};
    double y[2] = {x[0], x[1]};
    double bounds[4] = { 0.0, 0.0, 0.0, 0.0 };
    bool validBounds = false;
    for (vtkstd::list<vtkPlot*>::iterator it = this->mp_Plots->begin() ; it != this->mp_Plots->end() ; ++it)
    {
      if (!(*it)->GetVisible())
        continue;
      (*it)->GetBounds(bounds);
      if (bounds[1]-bounds[0] < 0.0) // Skip uninitialized bounds.
        continue;
      x[0] = std::min(x[0], bounds[0]);
      x[1] = std::max(x[1], bounds[1]);
      y[0] = std::min(y[0], bounds[2]);
      y[1] = std::max(y[1], bounds[3]);
      // Look for +/- Inf in the bounds for the Y axis only. Should not be possible for the X axis.
      if (vtkMath::IsInf(y[0]) || vtkMath::IsInf(y[1]))
      {
        y[0] = std::numeric_limits<double>::max();
        y[1] = -1.0*y[0];
        vtkDoubleArray* array = static_cast<vtkDoubleArray*>((*it)->GetInput()->GetColumn(1));
        double* data = array->GetPointer(0);
        for (int i = 0 ; i < array->GetNumberOfTuples() ; ++i)
        {
          if (!vtkMath::IsInf(data[i]))
          {
            if (data[i] < y[0])
              y[0] = data[i];
            else if (data[i] > y[1])
              y[1] = data[i];
          }
        }
      }
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
    
    this->m_PlotsTransformValid = false;
    this->m_ChartBoundsValid = true;
  };
  
  /**
   * Set the geometry (size) of the chart.
   *
   * Compared to the method vtkChart::SetSize, it also activates a flag
   * to notify this modification.
   */
  void VTKChartTimeSeries::SetGeometry(const vtkRectf& rect)
  {
    this->vtkChart::SetSize(rect);
    this->m_GeometryChanged = true;
  };
  
  /**
   * Sets the legend
   */
  void VTKChartTimeSeries::SetLegend(vtkSmartPointer<vtkChartLegend> legend)
  {
    if (this->mp_Legend == legend)
      return;
    this->mp_Legend = legend;
    this->mp_Legend->SetChart(this);
    this->AddItem(this->mp_Legend);
  };
  
  /**
   * @fn vtkChartLegend* VTKChartTimeSeries::GetLegend()
   * Returns the legend (NULL by default).
   */
  
  /**
   *  Sets the generator of colors.
   */
  void VTKChartTimeSeries::SetColorSeries(vtkSmartPointer<vtkColorSeries> colors)
  {
    if (this->mp_Colors == colors)
      return;
    this->mp_Colors = colors;
  };
  
  /**
   * @fn vtkColorSeries* VTKChartTimeSeries::GetColorSeries()
   * Returns the generator of colors (NULL by default).
   */
   
  /**
   * @fn const int* VTKChartTimeSeries::GetBorders() const
   * Returns the length of each border in an array (left, bottom, right, top).
   */ 
  
  /**
   * Modify the size of the borders. This method doesn't request to repaint the chart.
   */
  void VTKChartTimeSeries::SetBorders(int left, int bottom, int right, int top)
  {
    this->vtkChart::SetBorders(left, bottom, right, top);
    // Set the borders value after as the method SetBorders(), check the given value and modify them if necessary
    this->mp_Borders[0] = left;
    this->mp_Borders[1] = bottom;
    this->mp_Borders[2] = right;
    this->mp_Borders[3] = top;
    this->mp_AxisX->SetPoint1(this->Point1[0], this->Point1[1]);
    this->mp_AxisX->SetPoint2(this->Point2[0], this->Point1[1]);
    this->mp_AxisY->SetPoint1(this->Point1[0], this->Point1[1]);
    this->mp_AxisY->SetPoint2(this->Point1[0], this->Point2[1]);
    this->m_GeometryChanged = false;
    this->m_PlotsTransformValid = false;
  };
  
  /**
   * @fn VTKCurrentFrameFunctor::Pointer VTKChartTimeSeries::GetCurrentFrameFunctor() const
   * Returns the functor used to know the current frame to display.
   */
  
  /**
   * @fn void VTKChartTimeSeries::SetCurrentFrameFunctor(VTKCurrentFrameFunctor::Pointer functor)
   * Sets the functor used to know the current frame to display.
   */
  
  /**
   * @fn VTKRegionOfInterestFunctor::Pointer VTKChartTimeSeries::GetRegionOfInterestFunctor() const
   * Returns the functor used to know the region of interest to display.
   */
  
  /**
   * @fn void VTKChartTimeSeries::SetRegionOfInterestFunctor(VTKRegionOfInterestFunctor::Pointer functor)
   * Sets the functor used to know the region of interest to display.
   */
  
  /**
   * @fn VTKEventsFunctor::Pointer VTKChartTimeSeries::GetEventsFunctor() const
   * Returns the functor used to know events' informations.
   */
  
  /**
   * @fn void VTKChartTimeSeries::SetEventsFunctor(VTKEventsFunctor::Pointer functor)
   * Sets the functor used to know events' informations.
   */
  
  /**
   * @fn int VTKChartTimeSeries::GetDisplayZoomBox() const
   * Get the status of the zoom box display.
   */
  
  /**
   * @fn float VTKChartTimeSeries::GetEventLineWidth() const
   * Returns the width of the vertical line used to display the events.
   */
  
  /**
   * @fn void VTKChartTimeSeries::SetEventLineWidth(float width)
   * Sets the width of the vertical line used to display the events.
   */
  
  /**
   * @fn int VTKChartTimeSeries::GetEventLineTypeFactor() const
   * Returns the factor used in the spacing of the elements in the pattern to draw non-solid line.
   *
   * By default the dash, dot line, etc has a space of few pixels, but this function can scale this spacing.
   */
  
  /**
   * @fn void VTKChartTimeSeries::SetEventLineTypeFactor(int factor)
   * Sets the factor used in the spacing of the elements in the pattern to draw non-solid line.
   *
   * By default the dash, dot line, etc has a space of few pixels, but this function can scale this spacing.
   */
  
  /**
   * @fn int VTKChartTimeSeries::GetDisplayEvents() const
   * Get the status of the events display.
   */
   
  /**
   * @fn void VTKChartTimeSeries::SetDisplayEvents(int enabled)
   * Enable/Disable the displaying of the events as vertical lines into the chart
   */
  
  /**
   * @fn void VTKChartTimeSeries::DisplayEventsOn()
   * Convenient method to show events.
   */
  
  /**
   * @fn void VTKChartTimeSeries::DisplayEventsOff()
   * Convenient method to hide events.
   */
  
  /**
   * Enable/Disable the zoom box.
   */
  void VTKChartTimeSeries::SetDisplayZoomBox(bool enabled)
  {
    if (this->m_ZoomBoxDisplayed == enabled)
      return;
    this->m_ZoomBoxDisplayed = enabled;
    this->Modified();
  };
  
  /**
   * Show/Hide the plot for the given @a index.
   */
  void VTKChartTimeSeries::HidePlot(int index, bool isHidden)
  {
    if (static_cast<vtkIdType>(this->mp_Plots->size()) > index)
    {
      vtkstd::list<vtkPlot*>::iterator it = this->mp_Plots->begin();
      vtkstd::advance(it, index);
      if ((*it)->GetVisible() == !isHidden)
        return;
      (*it)->SetVisible(!isHidden);
      // Ensure that the bounds of the chart are updated to fit well the plots
      this->m_ChartBoundsValid = false;
      // Mark the scene as dirty
      if (this->Scene != NULL) this->Scene->SetDirty(true);
    }
  };
  
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
    if (!this->Visible)
      return false;
    vtkVector2i geometry(0, 0);
    if (this->LayoutStrategy == VTKChartTimeSeries::FILL_SCENE)
    {
      geometry = vtkVector2i(this->GetScene()->GetSceneWidth(), this->GetScene()->GetSceneHeight());
      if (geometry.X() != this->Geometry[0] || geometry.Y() != this->Geometry[1])
      {
        this->SetSize(vtkRectf(0.0, 0.0, geometry.X(), geometry.Y()));
        this->m_GeometryChanged = true;
      }
    }

    if (this->m_GeometryChanged)
    {
      this->SetBorders(this->mp_Borders[0], this->mp_Borders[1], this->mp_Borders[2], this->mp_Borders[3]);
    }

    this->Update();

    // Update the axes
    if (!this->m_ChartBoundsValid && !this->mp_Plots->empty())
      this->RecalculateBounds();

    if (!this->m_PlotsTransformValid)
      this->RecalculatePlotTransform();
    
    this->mp_AxisX->Update();
    this->mp_AxisY->Update();
    
    // Update the clipping if necessary
    this->mp_Clip->SetClip(this->Point1[0], this->Point1[1], this->Point2[0]-this->Point1[0], this->Point2[1]-this->Point1[1]);

    // draw background
    if(this->BackgroundBrush)
    {
      painter->GetPen()->SetLineType(vtkPen::NO_PEN);
      painter->ApplyBrush(this->BackgroundBrush);
      painter->DrawRect(this->Point1[0], this->Point1[1], this->Geometry[0], this->Geometry[1]);
    }

    // Use the scene to render most of the chart.
    this->PaintChildren(painter);

    // Draw the selection box if necessary
    if (this->m_ZoomBoxDisplayed)
    {
      painter->GetPen()->SetLineType(vtkPen::DASH_LINE);
      painter->GetBrush()->SetColor(255, 255, 255, 0);
      painter->GetPen()->SetColor(0, 0, 0, 255);
      painter->GetPen()->SetWidth(1.0);
      painter->DrawRect(this->m_ZoomBox.X(), this->m_ZoomBox.Y(), this->m_ZoomBox.Width(), this->m_ZoomBox.Height());
    }

    if (this->Title)
    {
      vtkPoints2D *rect = vtkPoints2D::New();
      rect->InsertNextPoint(this->Point1[0], this->Point2[1]);
      rect->InsertNextPoint(this->Point2[0]-this->Point1[0], 10);
      painter->ApplyTextProp(this->TitleProperties);
      painter->DrawStringRect(rect, this->Title);
      rect->Delete();
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
    if (plot != NULL)
    {
      this->AddPlot(plot);
      plot->Delete();
    }
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
     plot->Register(this);
     this->mp_Plots->push_back(plot);
     vtkIdType plotIndex = this->mp_Plots->size() - 1;
     this->mp_PlotTransform->AddItem(plot);
     // Ensure that the bounds of the chart are updated to contain the new plot
     this->m_ChartBoundsValid = false;
     // Mark the scene as dirty to update it.
     if (this->Scene != NULL) this->Scene->SetDirty(true);
    return plotIndex;
  };
  
  /**
   * Removes and delete a plot with the index @a index and request to update the bounds.
   */
  bool VTKChartTimeSeries::RemovePlot(vtkIdType index)
  {
    if (static_cast<vtkIdType>(this->mp_Plots->size()) > index)
    {
      vtkstd::list<vtkPlot*>::iterator it = this->mp_Plots->begin();
      vtkstd::advance(it, index);
      this->mp_PlotTransform->RemoveItem(*it);
      (*it)->Delete();
      this->mp_Plots->erase(it);
      // Ensure that the bounds of the chart are updated to fit well the plots
      this->m_ChartBoundsValid = false;
      // Mark the scene as dirty
      if (this->Scene != NULL) this->Scene->SetDirty(true);
      return true;
    }
    return false;
  };
  
  
  /**
   * Extract the plot and remove it from the chart wihtout delete it.
   */
  vtkPlot* VTKChartTimeSeries::TakePlot(vtkIdType index)
  {
    vtkPlot* plot = NULL;
    if (static_cast<vtkIdType>(this->mp_Plots->size()) > index)
    {
      vtkstd::list<vtkPlot*>::iterator it = this->mp_Plots->begin();
      vtkstd::advance(it, index);
      plot = *it;
      this->mp_PlotTransform->RemoveItem(*it);
      this->mp_Plots->erase(it);
      this->m_ChartBoundsValid = false;
      if (this->Scene != NULL) this->Scene->SetDirty(true);
    }
    return plot;
  };
  
  /**
   * Removes all the plots and reset all the bounds to 0
   */
  void VTKChartTimeSeries::ClearPlots()
  {
    for (vtkstd::list<vtkPlot*>::iterator it = this->mp_Plots->begin() ; it != this->mp_Plots->end() ; ++it)
    {
      this->mp_PlotTransform->RemoveItem(*it);
      (*it)->Delete();
    }
    this->mp_Plots->clear();
    this->m_ChartBoundsValid = false;
    if (this->Scene != NULL) this->Scene->SetDirty(true);
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
   * Retrieve the closest point near the mouse location and set it in the @a plotIndex structure.
   */
  bool VTKChartTimeSeries::LocatePointInPlots(const vtkContextMouseEvent& mouse, VTKChartPlotData& plotIndex)
  {
    if (this->mp_Plots->empty())
      return false;
    vtkVector2i pos(mouse.ScreenPos);
    if ((pos[0] > this->Point1[0]) && (pos[0] < this->Point2[0]) && (pos[1] > this->Point1[1]) && (pos[1] < this->Point2[1]))
    {
      vtkVector2f plotPos, position((float)pos.X(), (float)pos.Y());
      vtkTransform2D* transform = this->mp_PlotTransform->GetTransform();
      transform->InverseTransformPoints(position.GetData(), position.GetData(), 1);
      // Use a tolerance of +/- 2.5 and +/- 5 pixels for the X and Y direction respectively
      vtkVector2f tolerance((2.5/transform->GetMatrix()->GetElement(0,0)),
                            (5.0/transform->GetMatrix()->GetElement(1,1)));
                            
      // Iterate through the visible plots and return on the first hit
      for (vtkstd::list<vtkPlot*>::iterator it = this->mp_Plots->begin() ; it != this->mp_Plots->end() ; ++it)
      {
        vtkPlot* plot = *it;
        if (plot && plot->GetVisible())
        {
          int seriesIndex = plot->GetNearestPoint(position, tolerance, &plotPos);
          if (seriesIndex >= 0)
          {
            plotIndex.SeriesName = plot->GetLabel();
            plotIndex.Position = plotPos;
            plotIndex.ScreenPosition = mouse.ScreenPos;
            plotIndex.Index = seriesIndex;
            return true;
          }
        }
      }
    }
    return false;
  };
  
  /**
   * Return true if the supplied x, y coordinate is inside the plot area.
   */
  bool VTKChartTimeSeries::Hit(const vtkContextMouseEvent &mouse)
  {
    
    vtkVector2i pos(mouse.ScreenPos);
    if (this->GetVisible() &&
        (pos[0] > this->Point1[0]) &&
        (pos[0] < this->Point2[0]) &&
        (pos[1] > this->Point1[1]) &&
        (pos[1] < this->Point2[1]))
    {
      return true;
    }
    else
    {
      return false;
    }
  }

  /**
   * Return true if the supplied x, y coordinate is inside the item.
   */
  bool VTKChartTimeSeries::Hit2(const vtkContextMouseEvent& mouse)
  {
    vtkRectf size = this->GetSize();
    vtkVector2i pos(mouse.ScreenPos);
    if (this->GetVisible() &&
        (pos[0] > size.GetX()) && (pos[1] > size.GetY()) &&
        (pos[0] < (size.GetX() + size.GetWidth())) && (pos[1] < (size.GetY() + size.GetHeight())))
    {
      return true;
    }
    else
    {
      return false;
    }
  };

  /**
   * Do nothing
   */
  bool VTKChartTimeSeries::MouseEnterEvent(const vtkContextMouseEvent& )
  {
    return true;
  };
  
  /**
   * Do nothing
   */
  bool VTKChartTimeSeries::MouseLeaveEvent(const vtkContextMouseEvent& )
  {
    return true;
  };
  
  /**
   * Activate the zoom box if the mouse interactions are enabled and the shift key is activated.
   * Otherwise, the pan action is activated
   */
  bool VTKChartTimeSeries::MouseButtonPressEvent(const vtkContextMouseEvent& mouse)
  {
    if (!this->m_InteractionEnabled)
      return false;
    if (mouse.Button == vtkContextMouseEvent::LEFT_BUTTON)
    {
      this->m_ZoomBox.Set(mouse.Pos.X(), mouse.Pos.Y(), 0.0, 0.0);
#if (((VTK_MAJOR_VERSION == 5) && (VTK_MINOR_VERSION >= 10)) || (VTK_MAJOR_VERSION >= 6))
      // Zoom box
      if ((mouse.GetModifiers() & vtkContextMouseEvent::SHIFT_MODIFIER) == vtkContextMouseEvent::SHIFT_MODIFIER)
        this->m_ZoomBoxDisplayed |= true;
      // Pan
      else
        this->m_ZoomBoxDisplayed |= false;
#endif
      return true;
    }
    return false;
  };

  /**
   * Pan or grow the zoom box depending of the action activated.
   */
  bool VTKChartTimeSeries::MouseMoveEvent(const vtkContextMouseEvent& mouse)
  {
    if (!this->m_InteractionEnabled)
      return true;
    
    if (mouse.Button == vtkContextMouseEvent::LEFT_BUTTON)
    {
      // Pan
      if (!this->m_ZoomBoxDisplayed)
      {
        // Figure out how much the mouse has moved by in plot coordinates - pan
        vtkVector2d screenPos(mouse.ScreenPos.Cast<double>().GetData());
        vtkVector2d lastScreenPos(mouse.LastScreenPos.Cast<double>().GetData());
        vtkVector2d pos(0.0, 0.0);
        vtkVector2d last(0.0, 0.0);

        // Go from screen to scene coordinates to work out the delta
        vtkTransform2D *transform = this->mp_PlotTransform->GetTransform();
        transform->InverseTransformPoints(screenPos.GetData(), pos.GetData(), 1);
        transform->InverseTransformPoints(lastScreenPos.GetData(), last.GetData(), 1);
        vtkVector2d delta(last[0]-pos[0],last[1]-pos[1]);

        // Now move the axes and recalculate the transform
        delta[0] = delta[0] > 0 ?
          std::min(delta[0], this->mp_AxisX->GetMaximumLimit() - this->mp_AxisX->GetMaximum()) :
          std::max(delta[0], this->mp_AxisX->GetMinimumLimit() - this->mp_AxisX->GetMinimum());
        delta[1] = delta[1] > 0 ?
          std::min(delta[1], this->mp_AxisY->GetMaximumLimit() - this->mp_AxisY->GetMaximum()) :
          std::max(delta[1], this->mp_AxisY->GetMinimumLimit() - this->mp_AxisY->GetMinimum());
        this->mp_AxisX->SetMinimum(this->mp_AxisX->GetMinimum() + delta[0]);
        this->mp_AxisX->SetMaximum(this->mp_AxisX->GetMaximum() + delta[0]);
        this->mp_AxisY->SetMinimum(this->mp_AxisY->GetMinimum() + delta[1]);
        this->mp_AxisY->SetMaximum(this->mp_AxisY->GetMaximum() + delta[1]);

        this->RecalculatePlotTransform();
        this->Scene->SetDirty(true);
      }
      // Zoom
      else
      {
        float pts[4];
        this->mp_AxisX->GetPoint1(pts);
        pts[2] = this->mp_AxisX->GetPoint2()[0];
        pts[3] = this->mp_AxisY->GetPoint2()[1];
        
        float mouseX = mouse.Pos.X();
        if ((mouseX < pts[0]) || (mouseX > pts[2]))
        {
          if (this->m_ZoomBox.GetWidth() < 0.0f)
            mouseX = pts[0] + 1.0f; // Border
          else
            mouseX = pts[2] - 1.0f; // Border
        }
        
        float mouseY = mouse.Pos.Y();
        if ((mouseY < pts[1]) || (mouseY > pts[3]))
        {
          if (this->m_ZoomBox.GetHeight() < 0.0f)
            mouseY = pts[1] + 1.0f; // Border
          else
            mouseY = pts[3] - 1.0f; // Border
        }
        
        this->m_ZoomBox.SetWidth(mouseX - this->m_ZoomBox.X());
        this->m_ZoomBox.SetHeight(mouseY - this->m_ZoomBox.Y());
        this->Scene->SetDirty(true);
      }
    }
    else if (mouse.Button == vtkContextMouseEvent::NO_BUTTON)
    {
      this->Scene->SetDirty(true);
    }
    return true;
  };

  /**
   * If the zoom box is activated, then this method finish the action and zoom in the selection.
   */
  bool VTKChartTimeSeries::MouseButtonReleaseEvent(const vtkContextMouseEvent& mouse)
  {
    if (!this->m_InteractionEnabled)
      return false;
    
    if (mouse.Button == vtkContextMouseEvent::LEFT_BUTTON)
    {
      // Zoom
      this->m_ZoomBox.SetWidth(mouse.Pos.X() - this->m_ZoomBox.X());
      this->m_ZoomBox.SetHeight(mouse.Pos.Y() - this->m_ZoomBox.Y());
      if (this->m_ZoomBoxDisplayed && fabs(this->m_ZoomBox.Width()) > 0.5f && fabs(this->m_ZoomBox.Height()) > 0.5f)
      {
        // Zoom into the chart by the specified amount, and recalculate the bounds
        vtkVector2f point2(mouse.Pos);
        
        float pixelMin[2], pixelMax[2];
        pixelMin[0] = this->m_ZoomBox[0] + (this->m_ZoomBox[2] > 0 ? 0 : this->m_ZoomBox[2]);
        pixelMin[1] = this->m_ZoomBox[1] + (this->m_ZoomBox[3] > 0 ? 0 : this->m_ZoomBox[3]);
        pixelMax[0] = pixelMin[0] + fabs(this->m_ZoomBox[2]);
        pixelMax[1] = pixelMin[1] + fabs(this->m_ZoomBox[3]);
        float sceneMin[2], sceneMax[2];
        this->mp_PlotTransform->GetTransform()->InverseTransformPoints(pixelMin, sceneMin, 1);
        this->mp_PlotTransform->GetTransform()->InverseTransformPoints(pixelMax, sceneMax, 1);
        this->mp_AxisX->SetRange(sceneMin[0], sceneMax[0]);
        this->mp_AxisY->SetRange(sceneMin[1], sceneMax[1]);

        this->m_PlotsTransformValid = false;
        // Mark the scene as dirty
        this->Scene->SetDirty(true);
        
      }
      this->m_ZoomBoxDisplayed = false;
      return true;
    }
    return false;
  };

  /**
   * Zoom/Unzoom hover the mouse pointer.
   *
   * If the Shift key if activated, then the zoom is only on the horizontal axis.
   */
  bool VTKChartTimeSeries::MouseWheelEvent(const vtkContextMouseEvent& mouse, int delta)
  {
    if (!this->m_InteractionEnabled)
      return true;
    
    bool horizontalZoomModeOnly = false;
#if (((VTK_MAJOR_VERSION == 5) && (VTK_MINOR_VERSION >= 10)) || (VTK_MAJOR_VERSION >= 6))
    if ((mouse.GetModifiers() & vtkContextMouseEvent::SHIFT_MODIFIER) == vtkContextMouseEvent::SHIFT_MODIFIER)
      horizontalZoomModeOnly = true;
#endif
    horizontalZoomModeOnly |= (this->m_ZoomMode == HORIZONTAL);
    
    float pt[2];
    this->mp_PlotTransform->GetTransform()->InverseTransformPoints(mouse.Pos.GetData(), pt, 1);
    // Get the bounds of each plot.
    for (int i = 0; i < 2; ++i)
    {
      if (horizontalZoomModeOnly && (i == 0))
        continue;
      vtkAxis* axis = this->GetAxis(i);
      double oldRange = axis->GetMaximum() - axis->GetMinimum();
      double offsetCenter = (pt[1-i] - axis->GetMinimum()) - oldRange * 0.5;
      double min = axis->GetMinimum();
      double max = axis->GetMaximum();
      double frac = (max - min) * 0.05;
      min += delta*frac;
      max -= delta*frac;
      double shift = offsetCenter * oldRange / (max - min) - offsetCenter;
      min += shift;
      max += shift;
      axis->SetMinimum(min);
      axis->SetMaximum(max);
      // axis->RecalculateTickSpacing();
    }

    this->RecalculatePlotTransform();

    // Mark the scene as dirty
    this->Scene->SetDirty(true);

    return true;
  };

#if (((VTK_MAJOR_VERSION == 5) && (VTK_MINOR_VERSION >= 10)) || (VTK_MAJOR_VERSION >= 6))
  /**
   * Do nothing (Eat the event).
   */
  bool VTKChartTimeSeries::KeyPressEvent(const vtkContextKeyEvent& key)
  {
    btkNotUsed(key);
    return true;
  }
#endif
  
  /**
   * Constructor
   */
  VTKChartTimeSeries::VTKChartTimeSeries()
  : vtkChart(), m_ZoomBox()
  {
    // No legend by defaut.
    // No generator of color by default
    
    // Grid using the defined axes
    vtkPlotGrid* grid = vtkPlotGrid::New();
    this->AddItem(grid);
    grid->Delete();
    
    // The plots are drawn on top of the grid, in a clipped, transformed area.
    this->mp_Clip = vtkSmartPointer<vtkContextClip>::New();
    this->AddItem(this->mp_Clip);
    
    this->mp_PlotTransform = vtkSmartPointer<vtkContextTransform>::New();
    this->mp_Clip->AddItem(this->mp_PlotTransform); // Child list maintains ownership.
    
    // Only two axes (bottom: X axis, left: Y axis)
    // - X axis
    this->mp_AxisX = VTKAxis::New();
    this->mp_AxisX->SetPosition(vtkAxis::BOTTOM);
    this->mp_AxisX->SetTitle("X Axis");
    this->mp_AxisX->SetLabelMargin(7.0f);
    this->mp_AxisX->SetVisible(true);
    // - Y axis
    this->mp_AxisY = VTKAxis::New();
    this->mp_AxisY->SetPosition(vtkAxis::LEFT);
    this->mp_AxisY->SetTitle("Y Axis");
    this->mp_AxisY->SetVisible(true);
    // By default, they will be displayed with a null range and their behavior is fixed
    this->mp_AxisX->SetBehavior(vtkAxis::FIXED); // Fixed
    this->mp_AxisX->SetRange(0.0, 0.0);
    this->mp_AxisY->SetBehavior(vtkAxis::FIXED); // Fixed
    this->mp_AxisY->SetRange(0.0, 0.0);
    
    this->mp_ExtraAcquisition = VTKChartExtraAcquisition::New();
    this->mp_ExtraAcquisition->SetAxes(this->mp_AxisX, this->mp_AxisY);
    this->AddItem(this->mp_ExtraAcquisition);
    
    this->AddItem(this->mp_AxisX);
    this->AddItem(this->mp_AxisY);
    
    grid->SetXAxis(this->mp_AxisX);
    grid->SetYAxis(this->mp_AxisY);
    
    // Interaction and bounds
    // this->m_ClippingEnabled = true;
    this->m_InteractionEnabled = true;
    this->m_ZoomMode = BOTH;
    // this->m_BoundsEnabled = false;
    //     this->mp_Bounds[0] = 0.0;
    //     this->mp_Bounds[1] = 0.0;
    //     this->mp_Bounds[2] = 0.0;
    //     this->mp_Bounds[3] = 0.0;
    this->m_ChartBoundsValid = true;
    this->m_ZoomBoxDisplayed = false;
    
    // Borders
    this->mp_Borders[0] = 0;
    this->mp_Borders[1] = 0;
    this->mp_Borders[2] = 0;
    this->mp_Borders[3] = 0;
    
    // Plots
    this->mp_Plots = new VTKPlots();
    
    // Defaut borders for the chart
    this->m_GeometryChanged = false;
    this->SetBorders(60, 20, 20, 20);
  };
  
  VTKChartTimeSeries::~VTKChartTimeSeries()
  {
    for (vtkstd::list<vtkPlot*>::iterator it = this->mp_Plots->begin() ; it != this->mp_Plots->end() ; ++it)
      (*it)->Delete();
    delete this->mp_Plots;
    this->mp_AxisX->Delete();
    this->mp_AxisY->Delete();
    this->mp_ExtraAcquisition->Delete();
  };
  
  /**
   * Update the size of the plot into the dimensions of the scene.
   */
  void VTKChartTimeSeries::RecalculatePlotTransform()
  {
    // this->CalculatePlotTransform(this->mp_AxisX, this->mp_AxisY, this->mp_PlotTransform->GetTransform());
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

    this->mp_PlotTransform->GetTransform()->Identity();
    this->mp_PlotTransform->GetTransform()->Translate(this->Point1[0], this->Point1[1]);
    this->mp_PlotTransform->GetTransform()->Scale(scaleX, scaleY);
    this->mp_PlotTransform->GetTransform()->Translate(-this->mp_AxisX->GetMinimum(), -this->mp_AxisY->GetMinimum());
    
    this->m_PlotsTransformValid = true;
  };
};
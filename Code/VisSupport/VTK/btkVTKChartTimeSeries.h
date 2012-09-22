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

#ifndef __btkVTKChartTimeSeries_h
#define __btkVTKChartTimeSeries_h

#include "btkConfigure.h"
#include "btkVTKAxis.h"
#include "btkVTKChartExtraAcquisition.h"

#include <vtkChart.h>
#include <vtkVector.h> // vtkRectf
#include <vtkColorSeries.h>
#include <vtkContextClip.h>
#include <vtkContextTransform.h>
#include <vtkChartLegend.h>

class vtkChartLegend;
class vtkColorSeries;

namespace btk
{
  /**
   * FROM VTK 5.10: Small struct used by InvokeEvent to send some information about the point
   * that was clicked on. This is an experimental part of the API, subject to
   * change.
   */
  struct VTKChartPlotData
  {
    vtkStdString SeriesName;
    vtkVector2f Position;
    vtkVector2i ScreenPosition;
    int Index;
  };
  
  class VTKChartTimeSeries : public vtkChart
  {
  public:
    vtkTypeMacro(VTKChartTimeSeries, vtkChart);
    
    enum {HORIZONTAL, VERTICAL, BOTH}; // Zoom mode
    
    BTK_VTK_EXPORT static VTKChartTimeSeries* New();
    
    virtual ~VTKChartTimeSeries();
    
    bool GetInteractionEnabled() const {return this->m_InteractionEnabled;};
    void SetInteractionEnabled(bool enabled) {this->m_InteractionEnabled = enabled;};
    
    int GetZoomMode() const {return this->m_ZoomMode;};
    void SetZoomMode(int mode) {this->m_ZoomMode = mode;};
    
    void SetBounds(double xMin, double xMax, double yMin, double yMax) {double bounds[4] = {xMin, xMax, yMin, yMax}; this->SetBounds(bounds);};
    BTK_VTK_EXPORT void SetBounds(double bounds[4]);
    BTK_VTK_EXPORT virtual void RecalculateBounds();
    BTK_VTK_EXPORT void SetGeometry(const vtkRectf& rect);
    
    BTK_VTK_EXPORT void SetLegend(vtkSmartPointer<vtkChartLegend> legend);
    vtkChartLegend* GetLegend() {return this->mp_Legend;};
    
    BTK_VTK_EXPORT void SetColorSeries(vtkSmartPointer<vtkColorSeries> colors);
    vtkColorSeries* GetColorSeries() {return this->mp_Colors;};
    
    const int* GetBorders() const {return this->mp_Borders;};
    BTK_VTK_EXPORT void SetBorders(int left, int bottom, int right, int top);
    
    VTKCurrentFrameFunctor::Pointer GetCurrentFrameFunctor() const {return this->mp_ExtraAcquisition->GetCurrentFrameFunctor();};
    void SetCurrentFrameFunctor(VTKCurrentFrameFunctor::Pointer functor) {this->mp_ExtraAcquisition->SetCurrentFrameFunctor(functor);};
    VTKRegionOfInterestFunctor::Pointer GetRegionOfInterestFunctor() const {return this->mp_ExtraAcquisition->GetRegionOfInterestFunctor();};
    void SetRegionOfInterestFunctor(VTKRegionOfInterestFunctor::Pointer functor) {this->mp_ExtraAcquisition->SetRegionOfInterestFunctor(functor);};
    VTKEventsFunctor::Pointer GetEventsFunctor() const {return this->mp_ExtraAcquisition->GetEventsFunctor();};
    void SetEventsFunctor(VTKEventsFunctor::Pointer functor) {this->mp_ExtraAcquisition->SetEventsFunctor(functor);};
    
    int GetDisplayEvents() const {return this->mp_ExtraAcquisition->GetDisplayEvents();};
    void SetDisplayEvents(int enabled) {return this->mp_ExtraAcquisition->SetDisplayEvents(enabled);};
    void DisplayEventsOn() {this->SetDisplayEvents(1);};
    void DisplayEventsOff() {this->SetDisplayEvents(0);};
    
    float GetEventLineWidth() const {return this->mp_ExtraAcquisition->GetEventLineWidth();};
    void SetEventLineWidth(float width) {return this->mp_ExtraAcquisition->SetEventLineWidth(width);};
    
    int GetEventLineTypeFactor() const {return this->mp_ExtraAcquisition->GetEventLineTypeFactor();};
    void SetEventLineTypeFactor(int factor) {return this->mp_ExtraAcquisition->SetEventLineTypeFactor(factor);};
    
    bool GetDisplayZoomBox() const {return this->m_ZoomBoxDisplayed;};
    BTK_VTK_EXPORT void SetDisplayZoomBox(bool enabled);
    
    BTK_VTK_EXPORT virtual void Update();
    BTK_VTK_EXPORT virtual bool Paint(vtkContext2D *painter);
    
    BTK_VTK_EXPORT virtual vtkPlot* AddPlot(int type);
    BTK_VTK_EXPORT virtual vtkIdType AddPlot(vtkPlot* plot);
    BTK_VTK_EXPORT virtual bool RemovePlot(vtkIdType index);
    BTK_VTK_EXPORT virtual vtkPlot* TakePlot(vtkIdType index);
    BTK_VTK_EXPORT virtual void ClearPlots();
    BTK_VTK_EXPORT virtual vtkPlot* GetPlot(vtkIdType index);
    BTK_VTK_EXPORT virtual vtkIdType GetNumberOfPlots();
    BTK_VTK_EXPORT virtual vtkAxis* GetAxis(int axisIndex);
    BTK_VTK_EXPORT virtual vtkIdType GetNumberOfAxes();
    BTK_VTK_EXPORT virtual void HidePlot(int index, bool isHidden);
    BTK_VTK_EXPORT virtual bool LocatePointInPlots(const vtkContextMouseEvent& mouse, VTKChartPlotData& plotIndex);
    
    BTK_VTK_EXPORT bool Hit(const vtkContextMouseEvent& mouse);
    BTK_VTK_EXPORT bool Hit2(const vtkContextMouseEvent& mouse);
    BTK_VTK_EXPORT virtual bool MouseEnterEvent(const vtkContextMouseEvent& mouse);
    BTK_VTK_EXPORT virtual bool MouseLeaveEvent(const vtkContextMouseEvent& mouse);
    BTK_VTK_EXPORT virtual bool MouseButtonPressEvent(const vtkContextMouseEvent& mouse);
    BTK_VTK_EXPORT virtual bool MouseButtonReleaseEvent(const vtkContextMouseEvent& mouse);
    BTK_VTK_EXPORT virtual bool MouseMoveEvent(const vtkContextMouseEvent& mouse);
    BTK_VTK_EXPORT virtual bool MouseWheelEvent(const vtkContextMouseEvent& mouse, int delta);
#if (((VTK_MAJOR_VERSION == 5) && (VTK_MINOR_VERSION >= 10)) || (VTK_MAJOR_VERSION >= 6))
    BTK_VTK_EXPORT virtual bool KeyPressEvent(const vtkContextKeyEvent& key);
#endif
    
    BTK_VTK_EXPORT void RecalculatePlotTransform();
    
#if ((VTK_MAJOR_VERSION == 5) && (VTK_MINOR_VERSION < 10))
    enum {FILL_SCENE = 0, FILL_RECT, AXES_TO_RECT};
    vtkSetMacro(LayoutStrategy, int);
    vtkGetMacro(LayoutStrategy, int);
    void SetBackgroundBrush(vtkBrush *brush);
    vtkBrush* GetBackgroundBrush();
  protected:
    int LayoutStrategy;
    vtkSmartPointer<vtkBrush> BackgroundBrush;
#endif
    
  protected:
    BTK_VTK_EXPORT VTKChartTimeSeries();
    
  private:
    class VTKPlots;
    
    VTKChartTimeSeries(const VTKChartTimeSeries& ); // Not implemented.
    void operator=(const VTKChartTimeSeries& );   // Not implemented.
    
    VTKChartExtraAcquisition* mp_ExtraAcquisition;
    
    bool m_InteractionEnabled;
    int m_ZoomMode;
    vtkRectf m_ZoomBox;
    bool m_ZoomBoxDisplayed;
    int mp_Borders[4]; // left, bottom, right, top
    
    vtkSmartPointer<vtkChartLegend> mp_Legend;
    vtkSmartPointer<vtkColorSeries> mp_Colors;
    vtkSmartPointer<vtkContextClip> mp_Clip;
    vtkSmartPointer<vtkContextTransform> mp_PlotTransform;
    VTKAxis* mp_AxisX;
    VTKAxis* mp_AxisY;
    VTKPlots* mp_Plots;
    bool m_GeometryChanged;
    bool m_ChartBoundsValid;
    bool m_PlotsTransformValid;
  };
};

#endif // __btkVTKChartTimeSeries_h

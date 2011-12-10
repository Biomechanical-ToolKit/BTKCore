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

#ifndef __btkVTKChartTimeSeries_h
#define __btkVTKChartTimeSeries_h

#include "btkConfigure.h"
#include "btkSharedPtr.h"
#include "btkVTKAxis.h"

#include <vtkChart.h>
#include <vtkVector.h> // vtkRectf
#include <vtkContextScene.h> // vtkContextMouseEvent

class vtkChartLegend;
class vtkColorSeries;
class vtkPlotGrid;

namespace btk
{
  class VTKPlots;
  
  class VTKCurrentFrameFunctor
  {
  public:
    typedef SharedPtr<VTKCurrentFrameFunctor> Pointer;
    virtual ~VTKCurrentFrameFunctor() {};
    virtual int operator()() = 0;
  protected:
    VTKCurrentFrameFunctor() {};
  private:
    VTKCurrentFrameFunctor(const VTKCurrentFrameFunctor& ); // Not implemented.
    VTKCurrentFrameFunctor& operator=(const VTKCurrentFrameFunctor& ); // Not implemented.
  };
  
  class VTKRegionOfInterestFunctor
  {
  public:
    typedef SharedPtr<VTKRegionOfInterestFunctor> Pointer;
    virtual ~VTKRegionOfInterestFunctor() {};
    virtual void operator()(int& left, int& right) = 0;
  protected:
    VTKRegionOfInterestFunctor() {};
  private:
    VTKRegionOfInterestFunctor(const VTKRegionOfInterestFunctor& ); // Not implemented.
    VTKRegionOfInterestFunctor& operator=(const VTKRegionOfInterestFunctor& ); // Not implemented.
  };
  
  class VTKEventsFunctor
  {
  public:
    typedef SharedPtr<VTKEventsFunctor> Pointer;
    virtual ~VTKEventsFunctor() {};
    virtual bool operator()(int index, int& typeId, int& frame, double rgb[3]) = 0;
  protected:
    VTKEventsFunctor() {};
  private:
    VTKEventsFunctor(const VTKEventsFunctor& ); // Not implemented.
    VTKEventsFunctor& operator=(const VTKEventsFunctor& ); // Not implemented.
  };
  
  class VTKChartTimeSeries : public vtkChart
  {
  public:
    enum {HORIZONTAL, VERTICAL, BOTH}; // Zoom mode
    
    BTK_VTK_EXPORT static VTKChartTimeSeries* New();
    vtkExportedTypeRevisionMacro(VTKChartTimeSeries, vtkChart, BTK_VTK_EXPORT);
    
    virtual ~VTKChartTimeSeries();
    
    bool GetClippingEnabled() const {return this->m_ClippingEnabled;};
    void SetClippingEnabled(bool enabled) {this->m_ClippingEnabled = enabled;};
    
    bool GetInteractionEnabled() const {return this->m_InteractionEnabled;};
    void SetInteractionEnabled(bool enabled) {this->m_InteractionEnabled = enabled;};
    
    int GetZoomMode() const {return this->m_ZoomMode;};
    void SetZoomMode(int mode) {this->m_ZoomMode = mode;};
    
    bool GetBoundsEnabled() {return this->m_BoundsEnabled;};
    void SetBoundsEnabled(bool enabled) {this->m_BoundsEnabled = enabled;};
    double* GetBounds() {return this->mp_Bounds;};
    void SetBounds(double xMin, double xMax, double yMin, double yMax) {double bounds[4] = {xMin, xMax, yMin, yMax}; this->SetBounds(bounds);};
    BTK_VTK_EXPORT void SetBounds(double bounds[4]);
    BTK_VTK_EXPORT virtual void RecalculateBounds();
    
    BTK_VTK_EXPORT void SetLegend(vtkChartLegend* legend);
    vtkChartLegend* GetLegend() {return this->mp_Legend;};
    
    BTK_VTK_EXPORT void SetColorSeries(vtkColorSeries* colors);
    vtkColorSeries* GetColorSeries() {return this->mp_Colors;};
    
    int GetTitleMargin() const {return this->m_TitleMargin;};
    BTK_VTK_EXPORT void SetTitleMargin(int margin);
    
    const int* GetBorders() const {return this->mp_Borders;};
    BTK_VTK_EXPORT void SetBorders(int left, int bottom, int right, int top);
    
    VTKCurrentFrameFunctor::Pointer GetCurrentFrameFunctor() const {return this->mp_CurrentFrameFunctor;};
    BTK_VTK_EXPORT void SetCurrentFrameFunctor(VTKCurrentFrameFunctor::Pointer functor);
    VTKRegionOfInterestFunctor::Pointer GetRegionOfInterestFunctor() const {return this->mp_RegionOfInterestFunctor;};
    BTK_VTK_EXPORT void SetRegionOfInterestFunctor(VTKRegionOfInterestFunctor::Pointer functor);
    VTKEventsFunctor::Pointer GetEventsFunctor() const {return this->mp_EventsFunctor;};
    BTK_VTK_EXPORT void SetEventsFunctor(VTKEventsFunctor::Pointer functor);
    
    int GetDisplayEvents() const {return this->m_DisplayEvents;};
    BTK_VTK_EXPORT void SetDisplayEvents(int enabled);
    void DisplayEventsOn() {this->SetDisplayEvents(1);};
    void DisplayEventsOff() {this->SetDisplayEvents(0);};
    
    float GetEventLineWidth() const {return this->m_EventLineWidth;};
    void SetEventLineWidth(float width);
    
    int GetEventLineTypeFactor() const {return this->m_EventLineTypeFactor;};
    void SetEventLineTypeFactor(int factor);
    
    BTK_VTK_EXPORT void ResetZoom();
    BTK_VTK_EXPORT void ApplyZoom(const vtkRectf& box);
    const vtkRectf& GetZoomBox() const {return this->m_ZoomBox;};
    int GetDisplayZoomBox() const {return this->m_ZoomBoxDisplayed;};
    BTK_VTK_EXPORT void SetDisplayZoomBox(int enabled);
    void DisplayZoomBoxOn() {this->SetDisplayZoomBox(1);};
    void DisplayZoomBoxOff() {this->SetDisplayZoomBox(0);};
    
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
    
    vtkTransform2D* GetPlotsTransform() const {return this->mp_PlotsTransform;};
    
    bool Hit(const vtkContextMouseEvent& mouse) {return this->Hit(mouse.ScreenPos[0], mouse.ScreenPos[1]);};
    BTK_VTK_EXPORT bool Hit(int x, int y);
    BTK_VTK_EXPORT bool Hit2(int x, int y);
    BTK_VTK_EXPORT virtual bool MouseButtonPressEvent(const vtkContextMouseEvent& mouse);
    BTK_VTK_EXPORT virtual bool MouseButtonReleaseEvent(const vtkContextMouseEvent& mouse);
    BTK_VTK_EXPORT virtual bool MouseMoveEvent(const vtkContextMouseEvent& mouse);
    BTK_VTK_EXPORT virtual bool MouseWheelEvent(const vtkContextMouseEvent& mouse, int delta);
    
  protected:
    BTK_VTK_EXPORT VTKChartTimeSeries();

    BTK_VTK_EXPORT void RecalculatePlotsTransform();
    
  private:
    VTKChartTimeSeries(const VTKChartTimeSeries& ); // Not implemented.
    void operator=(const VTKChartTimeSeries& );   // Not implemented.
    
    bool m_ClippingEnabled;
    bool m_InteractionEnabled;
    int m_ZoomMode;
    bool m_BoundsEnabled;
    double mp_Bounds[4]; // xMin, xMax, yMin, yMax
    int mp_Borders[4]; // left, bottom, right, top
    bool m_BordersChanged;
    vtkRectf m_ZoomBox;
    int m_ZoomBoxDisplayed;
    
    vtkChartLegend* mp_Legend;
    vtkColorSeries* mp_Colors;
    VTKAxis* mp_AxisX;
    VTKAxis* mp_AxisY;
    vtkPlotGrid* mp_Grid;
    VTKPlots* mp_Plots;
    vtkTransform2D* mp_PlotsTransform;
    bool m_ChartBoundsValid;
    bool m_PlotsTransformValid;
    int m_TitleMargin;
    
    VTKCurrentFrameFunctor::Pointer mp_CurrentFrameFunctor;
    VTKRegionOfInterestFunctor::Pointer mp_RegionOfInterestFunctor;
    VTKEventsFunctor::Pointer mp_EventsFunctor;
    int m_DisplayEvents;
    float m_EventLineWidth;
    int m_EventLineTypeFactor;
  };
};

#endif // __btkVTKChartTimeSeries_h

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

#ifndef __btkVTKAxesWidget_h
#define __btkVTKAxesWidget_h

#include "btkConfigure.h"

#include <vtkObject.h>
#include <vtkCommand.h>

class vtkActor2D;
class vtkPolyData;
class vtkAxesActor;
class vtkRenderer;
class vtkRenderWindowInteractor;

namespace btk
{
  class VTKAxesWidgetObserver;
  
  class VTKAxesWidget : public vtkObject
  {
  public:
    BTK_VTK_EXPORT static VTKAxesWidget* New();
    vtkTypeMacro(VTKAxesWidget, vtkObject);
    BTK_VTK_EXPORT void PrintSelf(ostream& os, vtkIndent indent);
    
    BTK_VTK_EXPORT void SetAxesActor(vtkAxesActor* actor);
    vtkAxesActor* GetAxesActor() {return this->mp_AxesActor;};
    BTK_VTK_EXPORT void SetParentRenderer(vtkRenderer* ren);
    vtkRenderer* GetParentRenderer() {return this->mp_ParentRenderer;};
    BTK_VTK_EXPORT void SetInteractor(vtkRenderWindowInteractor* iren);
    vtkRenderWindowInteractor* GetInteractor() {return this->mp_WindowInteractor;};
    BTK_VTK_EXPORT void SetViewport(double minX, double minY, double maxX, double maxY);
    BTK_VTK_EXPORT double* GetViewport();
    
    double GetZoomFactor() const {return this->m_ZoomFactor;};
    BTK_VTK_EXPORT void SetZoomFactor(double s);
    
    BTK_VTK_EXPORT void SetEnabled(int);
    BTK_VTK_EXPORT void ExecuteEvent(vtkObject *o, unsigned long event, void *calldata);
    
  protected:
    BTK_VTK_EXPORT VTKAxesWidget();
    BTK_VTK_EXPORT ~VTKAxesWidget();

  private:
    class VTKAxesWidgetObserver : public vtkCommand
    {
    public:
      static VTKAxesWidgetObserver* New() {return new VTKAxesWidgetObserver();};
      virtual void Execute(vtkObject* obj, unsigned long event, void* calldata)
      {
        if (this->mp_AxesWidget)
          this->mp_AxesWidget->ExecuteEvent(obj, event, calldata);
      }
      VTKAxesWidget* GetAxesWidget() {return this->mp_AxesWidget;};
      void SetAxesWidget(VTKAxesWidget* widget) {this->mp_AxesWidget = widget;};
    private:  
      VTKAxesWidgetObserver()
      {
        this->mp_AxesWidget = 0;
      };
      VTKAxesWidget *mp_AxesWidget;
    };
  
    VTKAxesWidget(const VTKAxesWidget&);  // Not implemented
    void operator=(const VTKAxesWidget&);  // Not implemented
    
    vtkRenderer* mp_Renderer;
    vtkRenderer* mp_ParentRenderer;
    vtkAxesActor* mp_AxesActor;
    VTKAxesWidgetObserver* mp_Observer;
    vtkRenderWindowInteractor* mp_WindowInteractor;
    unsigned long mp_StartEventObserverId;
    double m_ZoomFactor;
  };
};

#endif // __btkVTKAxesWidget_h

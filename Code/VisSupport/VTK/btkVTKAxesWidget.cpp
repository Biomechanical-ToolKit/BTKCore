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
#include "btkVTKAxesWidget.h"

#include <vtkCamera.h>
#include <vtkCoordinate.h>
#include <vtkObjectFactory.h>
#include <vtkAxesActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

namespace btk
{

  /**
   * @class VTKAxesWidget btkVTKAxesWidget.h
   * @brief VTK widget displaying global axes orientation
   *
   * By default, this widget is not visible. To display it, use the method VTKAxesWidget::SetEnabled().
   * However, before enable it, you must set the interactor (@see VTKAxesWidget::SetInteractor()) 
   * and the parent's renderer (@see VTKAxesWidget::SetParentRenderer()).
   * This class contains its own vtkAxesActor.
   *
   * This class is inpired of the class vtkOrientationMarkerWidget in VTK.
   * 
   * @ingroup BTKVTK
   */
   
  /**
   * @fn static VTKAxesWidget* VTKAxesWidget::New()
   * Constructs a VTKAxesWidget object and return it as a pointer.
   */
  vtkStandardNewMacro(VTKAxesWidget);
  vtkCxxRevisionMacro(VTKAxesWidget, "$Revision: 0.1 $");
  
  /**
   * Prints object informations.
   */
  void VTKAxesWidget::PrintSelf(ostream& os, vtkIndent indent)
  {
    this->Superclass::PrintSelf(os, indent);
    os << indent << "AxesActor: " << this->mp_AxesActor << endl;
  };
  
  /**
   * Sets axes geometry.
   */
  void VTKAxesWidget::SetAxesActor(vtkAxesActor* actor)
  {
    vtkSetObjectBodyMacro(mp_AxesActor, vtkAxesActor, actor);
  };
  
  /**
   * @fn vtkAxesActor* VTKAxesWidget::GetAxesActor()
   * Returns axes object.
   */
  
  /**
   * Set the parents' renderer.
   */
  void VTKAxesWidget::SetParentRenderer(vtkRenderer* ren)
  {
    vtkSetObjectBodyMacro(mp_ParentRenderer, vtkRenderer, ren);
  };
  
  /**
   * @fn vtkRenderer* VTKAxesWidget::GetParentRenderer()
   * Returns the parent's renderer.
   */
  
  /**
   * Sets the reauired windows interactor to catch commands.
   */
  void VTKAxesWidget::SetInteractor(vtkRenderWindowInteractor* iren)
  {
    if (this->mp_WindowInteractor == iren)
      return;
    this->mp_WindowInteractor = iren;
    this->Modified();
  };
  
  /**
   * @fn vtkRenderWindowInteractor* VTKAxesWidget::GetInteractor()
   * Returns the interactor.
   */
  
  /**
   * Sets position and size of the widget in the parent's view.
   */
  void VTKAxesWidget::SetViewport(double minX, double minY,
                                  double maxX, double maxY)
  {
    this->mp_Renderer->SetViewport(minX, minY, maxX, maxY);
  };
  
  /**
   * Returns position and size of the widget.
   */
  double* VTKAxesWidget::GetViewport()
  {
    return this->mp_Renderer->GetViewport();
  };
  
  /**
   * Sets the visibility state of the widget.
   */
  void VTKAxesWidget::SetEnabled(int enabling)
  {
    if (!this->mp_WindowInteractor)
      vtkErrorMacro("The interactor must be set prior to enabling/disabling widget");
    
    if (enabling)
    {
      if (this->mp_AxesActor->GetVisibility())
        return;
      if (!this->mp_ParentRenderer)
      {
        vtkErrorMacro("The parent renderer must be set prior to enabling this widget");
        return;
      }
      this->mp_ParentRenderer->GetRenderWindow()->AddRenderer(this->mp_Renderer);
      if (this->mp_ParentRenderer->GetRenderWindow()->GetNumberOfLayers() < 2)
        this->mp_ParentRenderer->GetRenderWindow()->SetNumberOfLayers(2);
        
      this->mp_AxesActor->SetVisibility(1);
      // We need to copy the camera before the compositing observer is called.
      // Compositing temporarily changes the camera to display an image.
      this->mp_StartEventObserverId = 
        this->mp_ParentRenderer->AddObserver(vtkCommand::StartEvent, this->mp_Observer, 1);
      this->InvokeEvent(vtkCommand::EnableEvent, NULL);
    }
    else
    {
      if (!this->mp_AxesActor->GetVisibility())
        return;
      
      this->mp_AxesActor->SetVisibility(0);
      if (this->mp_ParentRenderer)
      {
        if (this->mp_ParentRenderer->GetRenderWindow())
        {
          this->mp_ParentRenderer->GetRenderWindow()->RemoveRenderer(this->mp_Renderer);
          this->mp_AxesActor->ReleaseGraphicsResources(this->mp_ParentRenderer->GetRenderWindow());
        }
        if (this->mp_StartEventObserverId != 0)
          this->mp_ParentRenderer->RemoveObserver(this->mp_StartEventObserverId);      
      }
      this->InvokeEvent(vtkCommand::DisableEvent, NULL);
    }
  };
  
  /**
   * Method called by the observer to update axes orientation.
   */
  void VTKAxesWidget::ExecuteEvent(vtkObject* /* o */,
                                   unsigned long /* event */,
                                   void* /*calldata */)
  {
    if (!this->mp_ParentRenderer)
      return;
    
    vtkCamera* camera = this->mp_ParentRenderer->GetActiveCamera();
    double pos[3], fp[3], viewup[3];
    camera->GetPosition(pos);
    camera->GetFocalPoint(fp);
    camera->GetViewUp(viewup);
    
    camera = this->mp_Renderer->GetActiveCamera();
    camera->SetPosition(pos);
    camera->SetFocalPoint(fp);
    camera->SetViewUp(viewup);
    this->mp_Renderer->ResetCamera();
  };
  
  /**
   * Constructor.
   */
  VTKAxesWidget::VTKAxesWidget()
  : vtkObject()
  {
    this->mp_Observer = VTKAxesWidgetObserver::New();
    this->mp_Observer->SetAxesWidget(this);
    this->mp_Renderer = vtkRenderer::New();
    this->mp_Renderer->SetViewport(0.0, 0.0, 0.2, 0.2);
    this->mp_Renderer->SetLayer(1);
    this->mp_Renderer->InteractiveOff();
    this->mp_AxesActor = vtkAxesActor::New();
    this->mp_AxesActor->SetVisibility(0);
    this->mp_Renderer->AddActor(this->mp_AxesActor);
    this->mp_ParentRenderer = 0;
    this->mp_WindowInteractor = 0;
    this->mp_StartEventObserverId = 0;
  };
  
  /**
   * Destructor.
   */
  VTKAxesWidget::~VTKAxesWidget()
  {
    this->mp_Observer->Delete();
    this->mp_AxesActor->Delete();
    this->SetParentRenderer(0);
    this->mp_Renderer->Delete();
  };
};
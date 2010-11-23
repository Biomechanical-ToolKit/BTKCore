/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2010, Arnaud Barré
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
 * LOSS OF USE, DATA, OR PROFITS;OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __btkVTKPickerInteractionCallback_h
#define __btkVTKPickerInteractionCallback_h

#include "btkVTKCommandEvents.h"
#include "btkVTKFrustumFromTwoPoints.h"

#include <vtkRenderWindow.h>
#include <vtkPolyData.h>
#include <vtkMapper.h>
#include <vtkPolyDataMapper.h>
#include <vtkCell.h>
#include <vtkPointData.h>
#include <vtkIdTypeArray.h>
#include <vtkRenderWindowInteractor.h>
#include <btkVTKInteractorStyleTrackballCamera.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkCellPicker.h>
#include <vtkExtractGeometry.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPlanes.h>
#include <vtkPropCollection.h>

#include <vtkFrustumSource.h>
#include <vtkProperty.h>

namespace btk
{
  static int VTKPickerMouseMotion = 0;

  /**
   * Simple way to add mouse picking to an interactor style.
   * Mouse picking are done with the left click. 
   * The sequence Ctrl + left click keeps previous selected markers.
   * Using the sequence Alt + left click send the event VTKMarkersToggleTrajectoryEvent
   * @ingroup BTKVTK
   */
  void VTKPickerInteractionCallback(
      vtkObject* /* object */, 
      unsigned long event, 
      void* clientdata, 
      void* /* calldata */)
  { 
    VTKInteractorStyleTrackballCamera* style = static_cast<VTKInteractorStyleTrackballCamera*>(clientdata);
    vtkRenderWindowInteractor* iren = style->GetInteractor();
    if (!iren)
      return;
    vtkRenderer* renderer = iren->GetRenderWindow()->GetRenderers()->GetFirstRenderer();
    vtkCellPicker* picker = vtkCellPicker::SafeDownCast(iren->GetPicker());
    if ((!renderer) || (!picker))
      return;
    switch(event)
    { 
      case vtkCommand::LeftButtonPressEvent: 
        VTKPickerMouseMotion = 0;
/*
        if (iren->GetAltKey())
          style->RubberBandSelectionOn();
*/
        style->OnLeftButtonDown();
        break;
      case vtkCommand::LeftButtonReleaseEvent:
        if (VTKPickerMouseMotion == 0)
        {
          int* pick = iren->GetEventPosition();
          picker->Pick((double)pick[0], (double)pick[1], 0.0, renderer);
          if (picker->GetCellId() >= 0 )
          {
            vtkActor* pickedActor = picker->GetActor();
            if (!pickedActor)
              return;
            vtkPolyData* pd = vtkPolyData::SafeDownCast(pickedActor->GetMapper()->GetInput());
            if (!pd)
              return;
            vtkCell* pickedCell = pd->GetCell(picker->GetCellId());
            int pnt = pickedCell->GetPointId(0);
            vtkIdTypeArray* inputPointIds = vtkIdTypeArray::SafeDownCast(pd->GetPointData()->GetArray("MarkersIds"));
            if ( inputPointIds )
            { 
              int id = static_cast<int>(inputPointIds->GetTuple1(pnt));
              if (iren->GetControlKey() && iren->GetShiftKey())
                iren->InvokeEvent(VTKMarkersToggleTrajectoryEvent, static_cast<void*>(&id));
              else if (iren->GetControlKey())
                iren->InvokeEvent(VTKMarkersPickedEvent, static_cast<void*>(&id));
              else
                iren->InvokeEvent(VTKMarkerPickedEvent, static_cast<void*>(&id));
            } 
          }
        }
/*
        else
        {
          if (style->GetRubberBandSelection() != 0)
          {
            int pts[4]; style->GetRubberBandPoints(pts);
            btk::VTKFrustumFromTwoPoints* fftp = btk::VTKFrustumFromTwoPoints::New();
            vtkPlanes* frustum = fftp->Generate(pts[0], pts[1], pts[2], pts[3], renderer);
            
            vtkProp* prop;
            vtkActor* actor;
            vtkPolyDataMapper* mapper = NULL;
            vtkPropCollection* props = renderer->GetViewProps();
            props->InitTraversal();
            while ((prop = props->GetNextProp()) != NULL)
            {
              if ( prop->GetPickable() && prop->GetVisibility() )
              {
                if ((actor = vtkActor::SafeDownCast(prop)) != NULL)
                {
                  if ((mapper = vtkPolyDataMapper::SafeDownCast(actor->GetMapper())) != NULL);
                    break;
                }
              }
            }
            if (mapper)
            {
              vtkExtractGeometry* extractGeometry = vtkExtractGeometry::New();
              extractGeometry->SetImplicitFunction(frustum);
              extractGeometry->SetInput(mapper->GetInput());
              //extractGeometry->ExtractInsideOff();
              extractGeometry->Update();
              //extractGeometry->PrintSelf(std::cout, vtkIndent(2));
              extractGeometry->GetOutput()->PrintSelf(std::cout, vtkIndent(2));
              //mapper->GetInput()->PrintSelf(std::cout, vtkIndent(2));
              extractGeometry->Delete();
              
#if 0
              vtkFrustumSource* fs = vtkFrustumSource::New();
              fs->SetPlanes(frustum);
              fs->Update();
              vtkPolyDataMapper* mapper = vtkPolyDataMapper::New();
              mapper->SetInput(fs->GetOutput());
              vtkProperty* prop = vtkProperty::New();
              prop->SetColor(1, 0.0, 0.0);
              prop->SetRepresentation(VTK_WIREFRAME);
              prop->SetAmbient(0.5);
              prop->SetDiffuse(0.0);
              prop->SetSpecular(0.0);
              vtkActor* actor = vtkActor::New();
              actor->SetMapper(mapper);
              actor->SetProperty(prop);
              actor->PickableOff();
              renderer->AddViewProp(actor);
              // Cleanup for ground
              mapper->Delete();
              actor->Delete();
              prop->Delete();
#endif
              
            }
            frustum->Delete();
            fftp->Delete();
          }
        }
*/
        style->OnLeftButtonUp();
        break;
      case vtkCommand::MouseMoveEvent: 
        VTKPickerMouseMotion = 1;
        style->OnMouseMove();
        break;
    } 
  };
};
#endif // __btkVTKPickerInteractionCallback_h

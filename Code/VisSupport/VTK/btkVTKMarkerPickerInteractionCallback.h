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

#ifndef __btkVTKMarkerPickerInteractionCallback_h
#define __btkVTKMarkerPickerInteractionCallback_h

#include "btkVTKCommandEvents.h"
#include "btkVTKFrustumFromTwoPoints.h"
#include "btkVTKInteractorStyleTrackballFixedUpCamera.h"

#include <vtkRenderWindow.h>
#include <vtkPolyData.h>
#include <vtkMapper.h>
#include <vtkPolyDataMapper.h>
#include <vtkCell.h>
#include <vtkCellArray.h>
#include <vtkPointData.h>
#include <vtkIdTypeArray.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkCellPicker.h>
#include <vtkExtractPolyDataGeometry.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPlanes.h>
#include <vtkPropCollection.h>
#include <vtkTransform.h>
/*
#include <vtkFrustumSource.h>
#include <vtkProperty.h>
*/

namespace btk
{
  static int VTKPickerMouseMotion = 0;

  /**
   * Add mouse picking to the interactor VTKInteractorStyleTrackballCamera style.
   * Multiple choices are available, by priority:
   *  - In static mode (no mouse mouvement):
   *     - LMB + Ctrl + Shift: Send the event VTKToggleTrajectoryMarkerPickedEvent when the cell picker found a marker (gives you the possibility to display marker's trajectory).
   *     - LMB + Ctrl: Send the event VTKToggleMarkerPickedEvent when the cell picker found a marker (gives you the possibility to keep the previous selection).
   *     - LMB (Left Mouse Button): Send the event VTKMarkerPickedEvent when the cell picker found a marker.
   *  - In dynamic mode (after mouse mouvement):
  *      _ LMB + Shift: Send the event VTKToggleMarkersSelectedEvent when the marker(s) was (were) selected by the rubber band. 
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
                iren->InvokeEvent(VTKToggleMarkerTrajectoryPickedEvent, static_cast<void*>(&id));
              else if (iren->GetControlKey())
                iren->InvokeEvent(VTKToggleMarkerPickedEvent, static_cast<void*>(&id));
              else
                iren->InvokeEvent(VTKMarkerPickedEvent, static_cast<void*>(&id));
            } 
          }
        }
        else
        {
          if (style->GetState() == btk_VTKISTC_RUBBER)
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
                  if ((mapper = vtkPolyDataMapper::SafeDownCast(actor->GetMapper())) != NULL)
                    break;
                }
              }
            }
            if (mapper)
            {
              vtkExtractPolyDataGeometry* extractGeometry = vtkExtractPolyDataGeometry::New();
              vtkTransform* s = vtkTransform::New();
              s->Scale(actor->GetScale());
              frustum->SetTransform(s);
              extractGeometry->SetImplicitFunction(frustum);
              extractGeometry->SetInput(mapper->GetInput());
              extractGeometry->Update();
              vtkPolyData* selection = extractGeometry->GetOutput();
              vtkIdTypeArray* inputPointIds = vtkIdTypeArray::SafeDownCast(selection->GetPointData()->GetArray("MarkersIds"));
              if ( inputPointIds )
              { 
                selection->GetPolys()->InitTraversal();
                vtkIdType numIds;
                vtkIdType* ptIds;
                vtkIdList* selectionIds = vtkIdList::New();
                while (selection->GetPolys()->GetNextCell(numIds, ptIds))
                  selectionIds->InsertUniqueId(inputPointIds->GetTuple1(ptIds[0]));
                if (selectionIds->GetNumberOfIds() != 0)
                  iren->InvokeEvent(VTKToggleMarkersSelectedEvent, static_cast<void*>(selectionIds));
                selectionIds->Delete();
              }
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
              s->Delete();
            }
            frustum->Delete();
            fftp->Delete();
          }
        }
        style->OnLeftButtonUp();
        break;
      case vtkCommand::MouseMoveEvent: 
        VTKPickerMouseMotion = 1;
        style->OnMouseMove();
        break;
    } 
  };
};
#endif // __btkVTKMarkerPickerInteractionCallback_h

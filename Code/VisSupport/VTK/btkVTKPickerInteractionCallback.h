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

#include <vtkRenderWindow.h>
#include <vtkPolyData.h>
#include <vtkMapper.h>
#include <vtkCell.h>
#include <vtkPointData.h>
#include <vtkIdTypeArray.h>
#include <vtkRenderWindowInteractor.h>
#include <btkVTKInteractorStyleTrackballCamera.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkCellPicker.h>

namespace btk
{
  static int mouseMotion = 0;

  /**
   * Simple way to add mouse picking to interactor style btk::VTKInteractorStyleTrackballCamera.
   * Mouse picking are done with the left click. Shift + left click keeps previous selected markers.
   * 
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
        mouseMotion = 0;
        style->OnLeftButtonDown();
        break;
      case vtkCommand::LeftButtonReleaseEvent: 
        if (mouseMotion == 0)
        { 
          int *pick = iren->GetEventPosition();
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
              if (iren->GetShiftKey())
                iren->InvokeEvent(VTKMarkersPickedEvent, static_cast<void*>(&id));
              else
                iren->InvokeEvent(VTKMarkerPickedEvent, static_cast<void*>(&id));
            } 
          }
        } 
        style->OnLeftButtonUp();
        break;
      case vtkCommand::MouseMoveEvent: 
        mouseMotion = 1;
        style->OnMouseMove();
        break;
    } 
  };
};
#endif // __btkVTKPickerInteractionCallback_h

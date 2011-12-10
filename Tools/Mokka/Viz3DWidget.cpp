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

#include "Viz3DWidget.h"
#include "Acquisition.h"

#include <btkVTKAxesWidget.h>
#include <btkVTKMarkerPickerInteractionCallback.h>
#include <btkVTKRubberRenderInteractionCallback.h>
#include <btkVTKInteractorStyleTrackballFixedUpCamera.h>

#include <vtkRenderWindow.h>
#include <vtkCellPicker.h>
#include <vtkCallbackCommand.h>
#include <vtkAxesActor.h>
#include <vtkIdList.h>
#include <vtkCamera.h>
#include <vtkCollectionIterator.h>
#include <vtkCaptionActor2D.h>
#include <vtkTextProperty.h>
#include <vtkTextActor.h>

#include <QKeyEvent>
#include <QToolTip>

Viz3DWidget::Viz3DWidget(QWidget* parent)
: QVTKWidget(parent)
{
  // Member
  this->mp_Acquisition = 0;
  this->mp_Renderer = vtkRenderer::New();
  this->mp_AxesWidget = btk::VTKAxesWidget::New();
  this->mp_EventQtSlotConnections = vtkEventQtSlotConnect::New();
  this->m_CameraConfigurationSaved = false;
  this->mp_CamFocalPoint[0] = 0.0; this->mp_CamFocalPoint[1] = 0.0; this->mp_CamFocalPoint[2] = 0.0;
  this->mp_CamPosition[0] = 0.0; this->mp_CamPosition[1] = 0.0; this->mp_CamPosition[2] = 0.0;
  this->mp_CamViewUp[0] = 0.0; this->mp_CamViewUp[1] = 0.0; this->mp_CamViewUp[2] = 0.0;
  
  // No need to send mouse events to VTK when a mouse button isn't down
  this->setMouseTracking(false);
}

Viz3DWidget::~Viz3DWidget()
{
  this->mp_Renderer->Delete();
  this->mp_AxesWidget->Delete();
  this->mp_EventQtSlotConnections->Delete();
};

void Viz3DWidget::initialize()
{
  // VTK UI
  //this->mp_Renderer->TwoSidedLightingOn();
  vtkRenderWindow* renwin = vtkRenderWindow::New();
  renwin->AddRenderer(this->mp_Renderer);
  renwin->LineSmoothingOn();
#if 0
  renwin->DoubleBufferOff(); // Required for Windows 7 (32-bit and 64-bit) when selecting markers with the rubber (no playback mode).
#endif
  //renwin->PolygonSmoothingOn();
  this->SetRenderWindow(renwin);
  renwin->Delete();
  // VTK cell picker
  vtkCellPicker* cellPicker = vtkCellPicker::New();
  cellPicker->SetTolerance(0.005);
  this->GetRenderWindow()->GetInteractor()->SetPicker(cellPicker);
  cellPicker->Delete();
  // VTK interaction style
  btk::VTKInteractorStyleTrackballFixedUpCamera* style = btk::VTKInteractorStyleTrackballFixedUpCamera::New();
  style->CharEventEnabledOff();
  style->RightButtonEnabledOff(); // Conflict with the right click button for the contextual menu.
  vtkCallbackCommand* pickerMouseInteraction = vtkCallbackCommand::New();
  pickerMouseInteraction->SetClientData(style);
  pickerMouseInteraction->SetCallback(&btk::VTKPickerInteractionCallback);
  style->AddObserver(vtkCommand::LeftButtonPressEvent, pickerMouseInteraction);
  style->AddObserver(vtkCommand::MouseMoveEvent, pickerMouseInteraction);
  style->AddObserver(vtkCommand::LeftButtonReleaseEvent, pickerMouseInteraction);
  this->GetRenderWindow()->GetInteractor()->SetInteractorStyle(style);
  pickerMouseInteraction->Delete();
  style->Delete();
  // Rubber callback
  vtkCallbackCommand* rubberRenderInteraction = vtkCallbackCommand::New();
  rubberRenderInteraction->SetClientData(style);
  rubberRenderInteraction->SetCallback(&btk::VTKRubberRenderInteractionCallback);
  this->mp_Renderer->AddObserver(vtkCommand::EndEvent, rubberRenderInteraction);
  rubberRenderInteraction->Delete();
  
  // VTK WIDGET
  this->mp_AxesWidget->SetParentRenderer(this->mp_Renderer);
  this->mp_AxesWidget->SetInteractor(this->GetRenderWindow()->GetInteractor());
  this->mp_AxesWidget->SetZoomFactor(1.25);
  vtkAxesActor* axesActor = this->mp_AxesWidget->GetAxesActor();
  axesActor->SetShaftTypeToCylinder();
  axesActor->SetNormalizedLabelPosition(1.25, 1.25, 1.25);
  axesActor->SetCylinderRadius(0.500 * axesActor->GetCylinderRadius());
  axesActor->SetConeRadius(1.025 * axesActor->GetConeRadius());
  axesActor->GetXAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToNone();
  axesActor->GetYAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToNone();
  axesActor->GetZAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToNone();
  axesActor->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(10);
  axesActor->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(10);
  axesActor->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(10);
  this->mp_AxesWidget->SetEnabled(1);
  
  // Links between VTK & Qt
  this->mp_EventQtSlotConnections->Connect(
      this->GetRenderWindow()->GetInteractor(), 
      btk::VTKMarkerPickedEvent,
      this, 
      SLOT(selectPickedMarker(vtkObject*, unsigned long, void*, void*)));
  this->mp_EventQtSlotConnections->Connect(
      this->GetRenderWindow()->GetInteractor(), 
      btk::VTKToggleMarkerPickedEvent,
      this, 
      SLOT(togglePickedMarker(vtkObject*, unsigned long, void*, void*)));
  this->mp_EventQtSlotConnections->Connect(
      this->GetRenderWindow()->GetInteractor(), 
      btk::VTKToggleMarkersSelectedEvent,
      this, 
      SLOT(toggleSelectedMarkers(vtkObject*, unsigned long, void*, void*)));
  this->mp_EventQtSlotConnections->Connect(
      this->GetRenderWindow()->GetInteractor(), 
      btk::VTKToggleMarkerTrajectoryPickedEvent,
      this, 
      SLOT(toggleTrajectoryMarker(vtkObject*, unsigned long, void*, void*)));
      
  // this->saveProjectionCameraConfiguration();
};

void Viz3DWidget::restoreProjectionCameraConfiguration()
{
  if (!this->m_CameraConfigurationSaved) // No configuration saved
    return;
  vtkCamera* cam = this->mp_Renderer->GetActiveCamera();
  cam->SetPosition(this->mp_CamPosition);
  cam->SetViewUp(this->mp_CamViewUp);
  cam->SetFocalPoint(this->mp_CamFocalPoint);
  this->mp_Renderer->ResetCamera();
  cam->Zoom(1.6);
  this->m_CameraConfigurationSaved = false;
};

void Viz3DWidget::saveProjectionCameraConfiguration()
{
  if (this->m_CameraConfigurationSaved) // Configuration already saved
    return;
  vtkCamera* cam = this->mp_Renderer->GetActiveCamera();
  cam->GetPosition(this->mp_CamPosition);
  cam->GetViewUp(this->mp_CamViewUp);
  cam->GetFocalPoint(this->mp_CamFocalPoint);
  this->m_CameraConfigurationSaved = true;
};

void Viz3DWidget::copyProjectionCameraConfiguration(Viz3DWidget* source)
{
  this->mp_CamPosition[0] = source->mp_CamPosition[0];
  this->mp_CamPosition[1] = source->mp_CamPosition[1];
  this->mp_CamPosition[2] = source->mp_CamPosition[2];
  this->mp_CamViewUp[0] = source->mp_CamViewUp[0];
  this->mp_CamViewUp[1] = source->mp_CamViewUp[1];
  this->mp_CamViewUp[2] = source->mp_CamViewUp[2];
  this->mp_CamFocalPoint[0] = source->mp_CamFocalPoint[0];
  this->mp_CamFocalPoint[1] = source->mp_CamFocalPoint[1];
  this->mp_CamFocalPoint[2] = source->mp_CamFocalPoint[2];
};

void Viz3DWidget::setOrthogonalView(int view)
{
  double n[3];
  static_cast<btk::VTKInteractorStyleTrackballFixedUpCamera*>(this->GetRenderWindow()->GetInteractor()->GetInteractorStyle())->GetGlobalUp(n);
  vtkCamera* cam = this->mp_Renderer->GetActiveCamera();
  cam->SetFocalPoint(0.0,0.0,0.0);
  if (n[0] == 1.0)
  {
    cam->SetViewUp(0.0,1.0,0.0);
    cam->SetRoll(0.0);
    switch (view)
    {
    case Top:
      cam->SetPosition(1.0,0.0,0.0);
      break;
    case Bottom:
      cam->SetPosition(-1.0,0.0,0.0);
      cam->SetRoll(180.0);
      break;
    case Left:
      cam->SetPosition(0.0,0.0,1.0);
      cam->SetRoll(90.0);
      break;
    case Right:
      cam->SetPosition(0.0,0.0,-1.0);
      cam->SetRoll(-90.0);
      break;
    case Back:
      cam->SetPosition(0.0,1.0,0.0);
      cam->SetViewUp(1.0,0.0,0.0);
      cam->SetRoll(90.0);
      break;
    case Front:
      cam->SetPosition(0.0,-1.0,0.0);
      cam->SetViewUp(1.0,0.0,0.0);
      cam->SetRoll(90.0);
      break;
    default:
      qDebug("Unknown orthogonal view.");
    }
  }
  else if (n[1] == 1.0)
  {
    cam->SetViewUp(0.0,1.0,0.0);
    cam->SetRoll(0.0);
    switch (view)
    {
    case Top:
      cam->SetViewUp(0.0,0.0,-1.0);
      cam->SetPosition(0.0,1.0,0.0);
      break;
    case Bottom:
      cam->SetViewUp(0.0,0.0,1.0);
      cam->SetPosition(0.0,-1.0,0.0);
      break;
    case Left:
      cam->SetPosition(-1.0,0.0,0.0);
      break;
    case Right:
      cam->SetPosition(1.0,0.0,0.0);
      break;
    case Back:
      cam->SetPosition(0.0,0.0,-1.0);
      break;
    case Front:
      cam->SetPosition(0.0,0.0,1.0);
      break;
    default:
      qDebug("Unknown orthogonal view.");
    }
  }
  else if (n[2] == 1.0)
  {
    cam->SetViewUp(0.0,1.0,0.0);
    cam->SetRoll(0.0);
    switch (view)
    {
    case Top:
      cam->SetPosition(0.0,0.0,1.0);
      break;
    case Bottom:
      cam->SetPosition(0.0,0.0,-1.0);
      cam->SetRoll(180.0);
      break;
    case Left:
      cam->SetPosition(-1.0,0.0,0.0);
      cam->SetRoll(90.0);
      break;
    case Right:
      cam->SetPosition(1.0,0.0,0.0);
      cam->SetRoll(-90.0);
      break;
    case Back:
      cam->SetPosition(0.0,1.0,0.0);
      cam->SetViewUp(0.0,0.0,1.0);
      break;
    case Front:
      cam->SetPosition(0.0,-1.0,0.0);
      cam->SetViewUp(0.0,0.0,1.0);
      break;
    default:
      qDebug("Unknown orthogonal view.");
    }
  }
  else
    qDebug("Unsupported global up vector.");
  this->mp_Renderer->ResetCamera();
  cam->Zoom(1.6);
  this->GetRenderWindow()->Render();
}

void Viz3DWidget::copy(Viz3DWidget* source)
{
  // Clean the 3D view
  this->mp_Renderer->RemoveAllViewProps();
  
  // Copy the acquisition pointer
  this->setAcquisition(source->acquisition());
  // Add vtkViewProp to the new QVtkDialogWidget
  vtkRenderer* sourceRenderer = source->mp_Renderer;
  vtkCollectionIterator* it = sourceRenderer->GetViewProps()->NewIterator();
  it->InitTraversal();
  while (!it->IsDoneWithTraversal())
  {
    this->mp_Renderer->AddViewProp(static_cast<vtkProp*>(it->GetCurrentObject()));
    it->GoToNextItem();
  }
  // Copy camera orientation
  vtkCamera* sourceCamera = sourceRenderer->GetActiveCamera();
  vtkCamera* targetCamera = this->mp_Renderer->GetActiveCamera();
  targetCamera->SetPosition(sourceCamera->GetPosition()); 
  targetCamera->SetFocalPoint(sourceCamera->GetFocalPoint()); 
  targetCamera->SetViewUp(sourceCamera->GetViewUp());
  targetCamera->SetViewAngle(sourceCamera->GetViewAngle());
  this->mp_Renderer->ResetCamera();
  // FIXME: Add a zoom member or static variable in Viz3DWidget.h
  targetCamera->Zoom(1.6);
};

void Viz3DWidget::selectPickedMarker(vtkObject* /* caller */, unsigned long /* vtk_event */, void* /* client_data */, void* call_data)
{
  int id = *static_cast<int*>(call_data);
  emit pickedMarkerChanged(id);
};

void Viz3DWidget::togglePickedMarker(vtkObject* /* caller */, unsigned long /* vtk_event */, void* /* client_data */, void* call_data)
{
  int id = *static_cast<int*>(call_data);
  emit pickedMarkerToggled(id);
};

void Viz3DWidget::toggleSelectedMarkers(vtkObject* /* caller */, unsigned long /* vtk_event */, void* /* client_data */, void* call_data)
{
  QList<int> ids;
  vtkIdList* selectionIds = static_cast<vtkIdList*>(call_data);
  for (int i = 0 ; i < selectionIds->GetNumberOfIds() ; ++i)
    ids << selectionIds->GetId(i);
  emit selectedMarkersToggled(ids);
};

void Viz3DWidget::toggleTrajectoryMarker(vtkObject* /* caller */, unsigned long /* vtk_event */, void* /* client_data */, void* call_data)
{
  int id = *static_cast<int*>(call_data);
  emit trajectoryMarkerToggled(id);
};

void Viz3DWidget::render()
{
  this->GetRenderWindow()->Render();
};

void Viz3DWidget::show(bool s)
{
  vtkActorCollection* actors = this->mp_Renderer->GetActors();
  actors->InitTraversal();
  vtkActor* actor = actors->GetNextItem(); // Ground
  actor = actors->GetNextItem();
  while (actor)
  {
    actor->SetVisibility(s ? true : false);
    actor = actors->GetNextItem();
  }
  if (this->isVisible())
    this->GetRenderWindow()->Render();
};

bool Viz3DWidget::event(QEvent* event)
{
  if ((event->type() == QEvent::ToolTip) && this->mp_Acquisition)
  {
    QHelpEvent* helpEvent = static_cast<QHelpEvent*>(event);
    vtkCellPicker* picker = static_cast<vtkCellPicker*>(this->GetRenderWindow()->GetInteractor()->GetPicker());
    picker->Pick((double)helpEvent->x(), (double)(this->height()-helpEvent->y()-1), 0.0, this->mp_Renderer);
    if (picker->GetCellId() >= 0 )
    {
      vtkActor* pickedActor = picker->GetActor();
      if (pickedActor)
      {
        vtkPolyData* pd = vtkPolyData::SafeDownCast(pickedActor->GetMapper()->GetInput());
        if (pd)
        { 
          vtkCell* pickedCell = pd->GetCell(picker->GetCellId());
          int pnt = pickedCell->GetPointId(0);
          vtkIdTypeArray* inputPointIds = vtkIdTypeArray::SafeDownCast(pd->GetPointData()->GetArray("MarkersIds"));
          if (inputPointIds)
          { 
            int id = static_cast<int>(inputPointIds->GetTuple1(pnt));
            QToolTip::showText(helpEvent->globalPos(), this->mp_Acquisition->pointLabel(id));
            return true;
          } 
        }
      }
    }
    QToolTip::hideText();
    event->ignore();
    return true;
  }
  return QWidget::event(event);
};

void Viz3DWidget::keyPressEvent(QKeyEvent* event)
{
  event->accept(); // Keyboard events are not sent to VTK
  //this->QVTKWidget::keyPressEvent(event);
};

void Viz3DWidget::keyReleaseEvent(QKeyEvent* event)
{
  event->accept(); // Keyboard events are not sent to VTK
  //this->QVTKWidget::keyReleaseEvent(event);
};

void Viz3DWidget::mousePressEvent(QMouseEvent* event)
{
  this->GetRenderWindow()->GetInteractor()->SetAltKey(event->modifiers() == Qt::AltModifier ? 1 : 0);
  this->QVTKWidget::mousePressEvent(event);
};

void Viz3DWidget::resizeEvent(QResizeEvent* event)
{
  this->QVTKWidget::resizeEvent(event);
  QSize size = event->size();
  const double l = 100.0;
  double width = l / static_cast<double>(size.width());
  double height = l / static_cast<double>(size.height());
  this->mp_AxesWidget->SetViewport(0, 0, width, height);
};
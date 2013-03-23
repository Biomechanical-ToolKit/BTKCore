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

#include "MultiViewWidget.h"
#include "Acquisition.h"
#include "CompositeView.h"
#include "ChartWidget.h"
#include "ChartDialog.h"
#include "ChartExportDialog.h"
#include "LoggerVTKOutput.h"
#include "LoggerMessage.h"
#include "Viz3DWidget.h"
#include "VideoWidget.h"

#include <btkVTKInteractorStyleTrackballFixedUpCamera.h>
#include <btkVTKForcePlatformsSource.h>
#include <btkVTKGroundSource.h>
#include <btkVTKGRFsFramesSource.h>
#include <btkVTKCommandEvents.h>
#include <btkVTKMarkersFramesSource.h>
#include <btkVTKSegmentsFramesSource.h>

#include <QDragEnterEvent>
#include <QUrl>
#include <QSplitter>
#include <QCheckBox>

#include <vtkInformation.h>
#include <vtkIdList.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkActor.h>
#include <vtkCompositeDataPipeline.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkstd/list>
#include <vtkstd/map>
#include <vtkStreamingDemandDrivenPipeline.h>
#include <vtkCellPicker.h>
#include <vtkCallbackCommand.h>
#include <vtkAxesActor.h>
#include <vtkPlaneSource.h>
#include <vtkObjectBase.h>
#include <vtkIdTypeArray.h>
#include <vtkFloatArray.h>
#include <vtkCamera.h>

enum {VTK_GROUND, VTK_FORCE_PLATFORMS, VTK_MARKERS, VTK_GRFS, VTK_GRFS_VECTOR, VTK_GRFS_PWA, VTK_SEGMENTS};

class vtkStreamingDemandDrivenPipelineCollection : public vtkstd::list<vtkStreamingDemandDrivenPipeline*>
{};

class vtkProcessMap : public vtkstd::map<int, vtkObjectBase*>
{};

class vtkActorMap : public vtkstd::map<int, vtkActor*>
{};

float EventsFrameMapperFunctor::operator()(int index, int side, int shift)
{
  float x = -1.0f;
  if (this->m_Activated)
  {
    int idx = index - shift;
    if (side == 0) // Right
      x = this->extractCyclePercent(0, idx);
    else if (side == 1)
      x = this->extractCyclePercent(1, idx);
    else
      x = this->extractCyclePercent(2, idx);
  }
  else
    x = static_cast<float>(index);
  return x;
};

float EventsFrameMapperFunctor::extractCyclePercent(int side, int idx)
{
  float x = -1.0f;
  int numCycles = (*this->mp_CyclesBoundaries)[side]->GetNumberOfTuples() / 4;
  float* boundaries = (*this->mp_CyclesBoundaries)[side]->GetPointer(0);
  for (int idxCycle = 0 ; idxCycle < numCycles ; ++idxCycle)
  {
    // Do not include events at 0% and 100%
    if ((static_cast<int>(boundaries[4*idxCycle]) < idx) && (static_cast<int>(boundaries[4*idxCycle+2]) > idx))
    {
      x = (*this->mp_HorizontalAbscissa)[side+1]->GetPointer(0)[idx];
      break;
    }
  }
  return x;
};

EventsFrameMapperFunctor::EventsFrameMapperFunctor(vtkFloatArray* (*abscissa)[4], vtkFloatArray* (*boundaries)[3])
{
  this->mp_HorizontalAbscissa = abscissa;
  this->mp_CyclesBoundaries = boundaries;
  this->m_Activated = false;
};

MultiViewWidget::MultiViewWidget(QWidget* parent)
: AbstractMultiView(parent), m_VideoDelays(),
  m_ForcePlatformColor(127, 127, 127), m_ForceVectorColor(255, 255, 0),
  m_View3dActions(), m_ViewChartActions(), mp_EventsFrameMapperFunctor()
{
  this->mp_EventFilterObject = 0;
  this->mp_Acquisition = 0;
  this->mp_Model = 0;
  this->mp_EventQtSlotConnections = vtkEventQtSlotConnect::New();
  this->mp_VTKProc = new vtkProcessMap();
  this->mp_VTKActor = new vtkActorMap();
  this->mp_Syncro = new vtkStreamingDemandDrivenPipelineCollection();
  for (int i = 0 ; i < 4 ; ++i)
  {
    this->mp_PointChartAbscissa[i] = vtkFloatArray::New();
    this->mp_AnalogChartAbscissa[i] = vtkFloatArray::New();
  }
  this->mp_PointChartAbscissa[0]->SetName("HorizontalAxis_Point_Frame");
  this->mp_AnalogChartAbscissa[0]->SetName("HorizontalAxis_Analog_Frame");
  this->mp_PointChartAbscissa[1]->SetName("HorizontalAxis_Point_RightCycle");
  this->mp_AnalogChartAbscissa[1]->SetName("HorizontalAxis_Analog_RightCycle");
  this->mp_PointChartAbscissa[2]->SetName("HorizontalAxis_Point_LeftCycle");
  this->mp_AnalogChartAbscissa[2]->SetName("HorizontalAxis_Analog_LeftCycle");
  this->mp_PointChartAbscissa[3]->SetName("HorizontalAxis_Point_GeneralCycle");
  this->mp_AnalogChartAbscissa[3]->SetName("HorizontalAxis_Analog_GeneralCycle");
  for (int i = 0 ; i < 3 ; ++i)
  {
    this->mp_PointChartCycleBoundaries[i] = vtkFloatArray::New();
    this->mp_AnalogChartCycleBoundaries[i] = vtkFloatArray::New();
  }
  this->mp_PointChartCycleBoundaries[0]->SetName("PointRightCycleBoundaries");
  this->mp_AnalogChartCycleBoundaries[0]->SetName("AnalogRightCycleBoundaries");
  this->mp_PointChartCycleBoundaries[1]->SetName("PointLeftCycleBoundaries");
  this->mp_AnalogChartCycleBoundaries[1]->SetName("AnalogLeftCycleBoundaries");
  this->mp_PointChartCycleBoundaries[2]->SetName("PointGeneralCycleBoundaries");
  this->mp_AnalogChartCycleBoundaries[2]->SetName("AnalogGeneralCycleBoundaries");
  this->mp_EventsFrameMapperFunctor = EventsFrameMapperFunctor::New(&(this->mp_PointChartAbscissa), &(this->mp_PointChartCycleBoundaries));
  this->mp_Mappers = vtkMapperCollection::New();
  this->mp_GroupOrientationMenu = new QMenu(tr("Ground Orientation"),this);
  this->mp_ActionGroundOrientationAutomatic = new QAction(tr("Automatic"),this);
  this->mp_ActionGroundOrientationAutomatic->setCheckable(true);
  this->mp_ActionGroundOrientationPlaneXY = new QAction(tr("Plane XY"),this);
  this->mp_ActionGroundOrientationPlaneXY->setCheckable(true);
  this->mp_ActionGroundOrientationPlaneYZ = new QAction(tr("Plane YZ"),this);
  this->mp_ActionGroundOrientationPlaneYZ->setCheckable(true);
  this->mp_ActionGroundOrientationPlaneZX = new QAction(tr("Plane ZX"),this);
  this->mp_ActionGroundOrientationPlaneZX->setCheckable(true);
  QActionGroup* groundOrientationActionGroup = new QActionGroup(this);
  groundOrientationActionGroup->addAction(this->mp_ActionGroundOrientationAutomatic);
  groundOrientationActionGroup->addAction(this->mp_ActionGroundOrientationPlaneXY);
  groundOrientationActionGroup->addAction(this->mp_ActionGroundOrientationPlaneYZ);
  groundOrientationActionGroup->addAction(this->mp_ActionGroundOrientationPlaneZX);
  this->mp_ActionGroundOrientationAutomatic->setChecked(true);
  this->mp_GroupOrientationMenu->addAction(this->mp_ActionGroundOrientationAutomatic);
  this->mp_GroupOrientationMenu->addAction(this->mp_ActionGroundOrientationPlaneXY);
  this->mp_GroupOrientationMenu->addAction(this->mp_ActionGroundOrientationPlaneYZ);
  this->mp_GroupOrientationMenu->addAction(this->mp_ActionGroundOrientationPlaneZX);
  
  this->mp_MarkerTrajectoryLengthMenu = new QMenu(tr("Marker Trajectory Length"),this);
  this->mp_ActionMarkerTrajectoryFull = new QAction(tr("All Frames"),this);
  this->mp_ActionMarkerTrajectoryFull->setCheckable(true);
  this->mp_ActionMarkerTrajectory25 = new QAction(tr("25 Frames"),this);
  this->mp_ActionMarkerTrajectory25->setCheckable(true);
  this->mp_ActionMarkerTrajectory50 = new QAction(tr("50 Frames"),this);
  this->mp_ActionMarkerTrajectory50->setCheckable(true);
  this->mp_ActionMarkerTrajectory100 = new QAction(tr("100 Frames"),this);
  this->mp_ActionMarkerTrajectory100->setCheckable(true);
  this->mp_ActionMarkerTrajectory200 = new QAction(tr("200 Frames"),this);
  this->mp_ActionMarkerTrajectory200->setCheckable(true);
  QActionGroup* trajectoryLengthActionGroup = new QActionGroup(this);
  trajectoryLengthActionGroup->addAction(this->mp_ActionMarkerTrajectoryFull);
  trajectoryLengthActionGroup->addAction(this->mp_ActionMarkerTrajectory25);
  trajectoryLengthActionGroup->addAction(this->mp_ActionMarkerTrajectory50);
  trajectoryLengthActionGroup->addAction(this->mp_ActionMarkerTrajectory100);
  trajectoryLengthActionGroup->addAction(this->mp_ActionMarkerTrajectory200);
  this->mp_ActionMarkerTrajectory100->setChecked(true);
  this->mp_MarkerTrajectoryLengthMenu->addAction(this->mp_ActionMarkerTrajectoryFull);
  this->mp_MarkerTrajectoryLengthMenu->addAction(this->mp_ActionMarkerTrajectory25);
  this->mp_MarkerTrajectoryLengthMenu->addAction(this->mp_ActionMarkerTrajectory50);
  this->mp_MarkerTrajectoryLengthMenu->addAction(this->mp_ActionMarkerTrajectory100);
  this->mp_MarkerTrajectoryLengthMenu->addAction(this->mp_ActionMarkerTrajectory200);
  this->mp_ForceButterflyActivationAction = new QAction(tr("Toggle GRF Butterfly"),this);
  this->mp_ForceButterflyActivationAction->setEnabled(false);
  
  this->mp_ChartHorizontalAxisUnitMenu = new QMenu(tr("Horizontal Axis Unit"),this);
  this->mp_ContextualChartHorizontalAxisUnitMenu = new QMenu(tr("Horizontal Axis Unit"),this);
  this->mp_ActionChartAxisFrame = new QAction(tr("Frame"),this);
  this->mp_ActionChartAxisFrame->setCheckable(true);
  this->mp_ActionChartAxisTime = new QAction(tr("Time"),this);
  this->mp_ActionChartAxisTime->setCheckable(true);
  QMenu* menuCycle = new QMenu(tr("Cycle"),this);
  QActionGroup* chartBottomAxisDisplay = new QActionGroup(this);
  chartBottomAxisDisplay->addAction(this->mp_ActionChartAxisFrame);
  chartBottomAxisDisplay->addAction(this->mp_ActionChartAxisTime);
  this->mp_ActionChartAxisFrame->setChecked(true);
  this->mp_ChartHorizontalAxisUnitMenu->addAction(this->mp_ActionChartAxisFrame);
  this->mp_ChartHorizontalAxisUnitMenu->addAction(this->mp_ActionChartAxisTime);
  this->mp_ChartHorizontalAxisUnitMenu->addAction(menuCycle->menuAction());
  this->mp_ContextualChartHorizontalAxisUnitMenu->addAction(this->mp_ActionChartAxisFrame);
  this->mp_ContextualChartHorizontalAxisUnitMenu->addAction(this->mp_ActionChartAxisTime);
  for (int i = 0 ; i < ChartCycleSettingsManager::maxCycleSettings ; ++i)
  {
    this->mp_ActionCycleSettings[i] = new QAction(chartBottomAxisDisplay);
    this->mp_ActionCycleSettings[i]->setCheckable(true);
    this->mp_ActionCycleSettings[i]->setVisible(false);
    menuCycle->addAction(this->mp_ActionCycleSettings[i]);
    this->mp_ContextualChartHorizontalAxisUnitMenu->addAction(this->mp_ActionCycleSettings[i]);
  }
  menuCycle->addSeparator();
  this->mp_ManageChartCycleSettings = new QAction(tr("Manage Cycle Settings"),this);
  menuCycle->addAction(this->mp_ManageChartCycleSettings);
  this->mp_ChartCycleSettingsManager = 0;
  
  connect(groundOrientationActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(changeGroundOrientation()));
  connect(trajectoryLengthActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(changeMarkerTrajectoryLength()));
  connect(this->mp_ForceButterflyActivationAction, SIGNAL(triggered(bool)), this, SLOT(changeForceButterflyActivation()));
  connect(chartBottomAxisDisplay, SIGNAL(triggered(QAction*)), this, SLOT(updateChartHorizontalAxisUnit()));
  
  this->setViewPrototype(ViewFactory<CompositeView>);
};

MultiViewWidget::~MultiViewWidget()
{
  this->mp_EventQtSlotConnections->Delete();
  for (vtkProcessMap::iterator it = this->mp_VTKProc->begin() ; it != this->mp_VTKProc->end() ; ++it)
    it->second->Delete();
  delete this->mp_VTKProc;
  for (vtkActorMap::iterator it = this->mp_VTKActor->begin() ; it != this->mp_VTKActor->end() ; ++it)
    it->second->Delete();
  delete this->mp_VTKActor;
  delete this->mp_Syncro;
  for (int i = 0 ; i < 4 ; ++i)
  {
    this->mp_PointChartAbscissa[i]->Delete();
    this->mp_AnalogChartAbscissa[i]->Delete();
  }
  for (int i = 0 ; i < 3 ; ++i)
  {
    this->mp_PointChartCycleBoundaries[i]->Delete();
    this->mp_AnalogChartCycleBoundaries[i]->Delete();
  }
  this->mp_Mappers->Delete();
  vtkAlgorithm::SetDefaultExecutivePrototype(0);
};

void MultiViewWidget::initialize()
{
  this->AbstractMultiView::initialize();
  
  CompositeView* sv = static_cast<CompositeView*>(static_cast<QGridLayout*>(this->layout())->itemAtPosition(0,0)->widget());
  vtkRenderer* renderer = static_cast<Viz3DWidget*>(sv->view(CompositeView::Viz3D))->renderer();
  ChartWidget* chart = static_cast<ChartWidget*>(sv->view(CompositeView::Chart));
  VideoWidget* video = static_cast<VideoWidget*>(sv->view(CompositeView::MediaVideo));
  
  //vtkMapper::GlobalImmediateModeRenderingOn(); // For large dataset.
  
  // VTK PIPELINE
  // Static data
  // Simple ground grid
  btk::VTKGroundSource* ground = btk::VTKGroundSource::New();
  vtkPolyDataMapper* mapper = vtkPolyDataMapper::New();
  mapper->SetInputConnection(ground->GetOutputPort(0));
  vtkProperty* prop = vtkProperty::New();
  prop->SetRepresentation(VTK_WIREFRAME);
  prop->SetColor(0.8, 0.8, 0.8);
  prop->SetAmbient(0.5);
  prop->SetDiffuse(0.0);
  prop->SetSpecular(0.0);
  vtkActor* actor = vtkActor::New();
  actor->SetMapper(mapper);
  actor->SetProperty(prop);
  vtkProperty* propBackface = vtkProperty::New();
  propBackface->DeepCopy(prop);
  propBackface->SetColor(1.0, 0.0, 0.0);
  actor->SetBackfaceProperty(propBackface);
  actor->PickableOff();
  renderer->AddViewProp(actor);
  (*this->mp_VTKActor)[VTK_GROUND] = actor;
  // Cleanup for ground
  mapper->Delete();
  prop->Delete();
  propBackface->Delete();
  // Camera
  renderer->GetActiveCamera()->Elevation(-60);
  renderer->ResetCamera();
  renderer->GetActiveCamera()->Zoom(1.6);
  
  // Dynamic data
  // Pipeline for force plaforms (plane)
  btk::VTKForcePlatformsSource* forcePlaforms = btk::VTKForcePlatformsSource::New();
  mapper = vtkPolyDataMapper::New();
  mapper->SetInputConnection(forcePlaforms->GetOutputPort(0));
  prop = vtkProperty::New();
  prop->SetColor(this->m_ForcePlatformColor.redF(), this->m_ForcePlatformColor.greenF(), this->m_ForcePlatformColor.blueF());
  prop->SetOpacity(0.9);
  prop->SetAmbient(0.5);
  prop->SetDiffuse(0.0);
  prop->SetSpecular(0.0);
  actor = vtkActor::New();
  actor->SetMapper(mapper);
  actor->SetScale(0.005);
  actor->SetProperty(prop);
  actor->PickableOff();
  renderer->AddActor(actor);
  (*this->mp_VTKActor)[VTK_FORCE_PLATFORMS] = actor;
  // Cleanup for force platforms (plane)
  mapper->Delete();
  prop->Delete();
  // Pipeline for force plaforms (axes)
  mapper = vtkPolyDataMapper::New();
  mapper->SetInputConnection(forcePlaforms->GetOutputPort(1));
  mapper->SetScalarModeToUseCellData();
  actor = vtkActor::New();
  actor->SetMapper(mapper);
  actor->SetScale(0.005);
  actor->SetProperty(prop);
  actor->PickableOff();
  renderer->AddActor(actor);
  // Cleanup for force platforms (axes)
  mapper->Delete();
  actor->Delete();
  // Pipeline for force plaforms (index)
  mapper = vtkPolyDataMapper::New();
  mapper->SetInputConnection(forcePlaforms->GetOutputPort(2));
  prop = vtkProperty::New();
  prop->SetColor(1.0, 1.0, 1.0);
  prop->SetOpacity(0.9);
  prop->SetAmbient(0.5);
  prop->SetDiffuse(0.0);
  prop->SetSpecular(0.0);
  actor = vtkActor::New();
  actor->SetMapper(mapper);
  actor->SetScale(0.005);
  actor->SetProperty(prop);
  actor->PickableOff();
  renderer->AddActor(actor);
  // Cleanup for force platforms (index)
  mapper->Delete();
  actor->Delete();
  prop->Delete();
  // Require to play with VTK information's keys TIME_*
  vtkCompositeDataPipeline* prototype = vtkCompositeDataPipeline::New();
  vtkAlgorithm::SetDefaultExecutivePrototype(prototype);
  prototype->Delete();
  // Pipeline for markers
  btk::VTKMarkersFramesSource* markers = btk::VTKMarkersFramesSource::New();
  // - Display marker's position
  mapper = vtkPolyDataMapper::New();
  mapper->SetInputConnection(markers->GetOutputPort(0));
  mapper->SetLookupTable(markers->GetMarkerColorLUT());
  mapper->SetScalarModeToUsePointFieldData();
  mapper->UseLookupTableScalarRangeOn();
  mapper->SelectColorArray("Colors");
  this->mp_Mappers->AddItem(mapper);
  actor = vtkActor::New();
  actor->SetMapper(mapper);
  actor->SetScale(0.005);
  renderer->AddActor(actor);
  mapper->Delete();
  actor->Delete();
  // - Display marker's trajectory
  mapper = vtkPolyDataMapper::New();
  mapper->SetInputConnection(markers->GetOutputPort(1));
  mapper->SetLookupTable(markers->GetMarkerColorLUT());
  mapper->SetScalarModeToUsePointFieldData();
  mapper->UseLookupTableScalarRangeOn();
  mapper->SelectColorArray("Colors");
  this->mp_Mappers->AddItem(mapper);
  actor = vtkActor::New();
  actor->SetMapper(mapper);
  actor->PickableOff();
  actor->SetScale(0.005);
  actor->UseBoundsOff(); // Do not use this actor to compute the camera's bounds and its clipping planes.
  renderer->AddActor(actor);
  mapper->Delete();
  actor->Delete();
  // Pipeline for segments
  btk::VTKSegmentsFramesSource* segments = btk::VTKSegmentsFramesSource::New();
  // - Links
  mapper = vtkPolyDataMapper::New();
  mapper->SetInputConnection(segments->GetOutputPort(0));
  mapper->SetLookupTable(segments->GetSegmentColorLUT());
  mapper->SetScalarModeToUseCellData();
  mapper->UseLookupTableScalarRangeOn();
  mapper->SelectColorArray("Colors");
  this->mp_Mappers->AddItem(mapper);
  actor = vtkActor::New();
  actor->SetMapper(mapper);
  actor->GetProperty()->SetLineWidth(2);
  actor->SetScale(0.005);
  actor->UseBoundsOff();
  renderer->AddActor(actor);
  mapper->Delete();
  actor->Delete();
  // - Faces
  mapper = vtkPolyDataMapper::New();
  mapper->SetInputConnection(segments->GetOutputPort(1));
  mapper->SetLookupTable(segments->GetSegmentColorLUT());
  mapper->SetScalarModeToUseCellData();
  mapper->UseLookupTableScalarRangeOn();
  mapper->SelectColorArray("Colors");
  this->mp_Mappers->AddItem(mapper);
  actor = vtkActor::New();
  actor->GetProperty()->SetOpacity(0.5);
  actor->SetMapper(mapper);
  actor->SetScale(0.005);
  actor->UseBoundsOff();
  renderer->AddActor(actor);
  mapper->Delete();
  actor->Delete();
  // Pipeline for GRFs
  btk::VTKGRFsFramesSource* GRFs = btk::VTKGRFsFramesSource::New();
  mapper = vtkPolyDataMapper::New();
  mapper->SetInputConnection(GRFs->GetOutputPort(0));
  this->mp_Mappers->AddItem(mapper);
  prop = vtkProperty::New();
  prop->SetColor(this->m_ForceVectorColor.redF(), this->m_ForceVectorColor.greenF(), this->m_ForceVectorColor.blueF());
  prop->SetAmbient(0.5);
  prop->SetDiffuse(0.0);
  prop->SetSpecular(0.0);
  actor = vtkActor::New();
  actor->SetMapper(mapper);
  actor->SetScale(0.005);
  actor->SetProperty(prop);
  actor->PickableOff();
  renderer->AddActor(actor);
  (*this->mp_VTKActor)[VTK_GRFS_VECTOR] = actor;
  // Cleanup for GRFs.
  mapper->Delete();
  prop->Delete();
  // - Display GRF's PWA trajectory
  mapper = vtkPolyDataMapper::New();
  mapper->SetInputConnection(GRFs->GetOutputPort(1));
  this->mp_Mappers->AddItem(mapper);
  prop = vtkProperty::New();
  prop->SetColor(this->m_ForcePlatformColor.redF(), this->m_ForcePlatformColor.greenF(), this->m_ForcePlatformColor.blueF());
  prop->SetPointSize(2.0);
  actor = vtkActor::New();
  actor->SetMapper(mapper);
  actor->SetScale(0.005);
  actor->SetProperty(prop);
  actor->PickableOff();
  actor->UseBoundsOff();
  renderer->AddActor(actor);
  (*this->mp_VTKActor)[VTK_GRFS_PWA] = actor;
  // Cleanup for GRFs trajectory.
  mapper->Delete();
  prop->Delete();
  // Synchro between dynamic data
  this->mp_Syncro->push_back(vtkStreamingDemandDrivenPipeline::SafeDownCast(markers->GetExecutive()));
  this->mp_Syncro->push_back(vtkStreamingDemandDrivenPipeline::SafeDownCast(segments->GetExecutive()));
  this->mp_Syncro->push_back(vtkStreamingDemandDrivenPipeline::SafeDownCast(GRFs->GetExecutive()));
  // Store VTK process to be reused later.
  (*this->mp_VTKProc)[VTK_GROUND] = ground;
  (*this->mp_VTKProc)[VTK_FORCE_PLATFORMS] = forcePlaforms;
  (*this->mp_VTKProc)[VTK_MARKERS] = markers;
  (*this->mp_VTKProc)[VTK_GRFS] = GRFs;
  (*this->mp_VTKProc)[VTK_SEGMENTS] = segments;

  // Links between VTK & Qt
  this->mp_EventQtSlotConnections->Connect(
      markers, 
      btk::VTKMarkersListUpdatedEvent,
      this, 
      SLOT(updateDisplayedMarkersList(vtkObject*, unsigned long, void*, void*)));
      
  // Initialize the charts
  chart->setPointHorizontalData(&(this->mp_PointChartAbscissa), &(this->mp_PointChartCycleBoundaries));
  chart->setAnalogHorizontalData(&(this->mp_AnalogChartAbscissa), &(this->mp_AnalogChartCycleBoundaries));
  
  // Initialize the video delays
  video->setDelays(&this->m_VideoDelays);
  
  // Redirect the VTK message to the Logger
  vtkOutputWindow* w = LoggerVTKOutput::New();
  vtkOutputWindow::SetInstance(w);
  w->Delete();
};

void MultiViewWidget::setAcquisition(Acquisition* acq)
{
  if (this->mp_Acquisition)
    disconnect(this->mp_Acquisition, 0, this, 0);
  this->mp_Acquisition = acq;
  // Views
  for (QList<AbstractView*>::iterator it = this->m_Views.begin() ; it != this->m_Views.end() ; ++it)
    static_cast<CompositeView*>(*it)->setAcquisition(this->mp_Acquisition);
  // BTK->VTK connection
  btk::VTKMarkersFramesSource* markers = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
  markers->SetInput(0, this->mp_Acquisition->btkAllMarkers());
  btk::VTKGRFsFramesSource* GRFs = btk::VTKGRFsFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_GRFS]);
  GRFs->SetInput(this->mp_Acquisition->btkGroundReactionWrenches());
  btk::VTKForcePlatformsSource* forcePlaforms = btk::VTKForcePlatformsSource::SafeDownCast((*this->mp_VTKProc)[VTK_FORCE_PLATFORMS]);
  forcePlaforms->SetInput(this->mp_Acquisition->btkForcePlatforms());
  btk::VTKSegmentsFramesSource* segments = btk::VTKSegmentsFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_SEGMENTS]);
  segments->SetInput(this->mp_Acquisition->btkAllMarkers());
  // Object connection
  connect(this->mp_Acquisition, SIGNAL(markersRadiusChanged(QVector<int>, QVector<double>)), this, SLOT(setMarkersRadius(QVector<int>, QVector<double>)));
  connect(this->mp_Acquisition, SIGNAL(markersColorChanged(QVector<int>, QVector<QColor>)), this, SLOT(setMarkersColor(QVector<int>, QVector<QColor>)));
  connect(this->mp_Acquisition, SIGNAL(markersVisibilityChanged(QVector<int>, QVector<bool>)), this, SLOT(setMarkersVisibility(QVector<int>, QVector<bool>)));
  connect(this->mp_Acquisition, SIGNAL(markersTrajectoryVisibilityChanged(QVector<int>, QVector<bool>)), this, SLOT(setMarkersTrajectoryVisibility(QVector<int>, QVector<bool>)));
  connect(this->mp_Acquisition, SIGNAL(markersConfigurationReset(QList<int>, QList<bool>, QList<bool>, QList<double>, QList<QColor>)), this, SLOT(setMarkersConfiguration(QList<int>, QList<bool>, QList<bool>, QList<double>, QList<QColor>)));
  connect(this->mp_Acquisition, SIGNAL(firstFrameChanged(int)), this, SLOT(updateFramesIndex(int)));
  connect(this->mp_Acquisition, SIGNAL(videosDelayChanged(QVector<int>, QVector<qint64>)), this, SLOT(setVideoDelays(QVector<int>, QVector<qint64>)));
  connect(this->mp_Acquisition, SIGNAL(analogsValuesChanged(QVector<int>)), this, SLOT(updateAnalogValuesModification(QVector<int>)));
}

void MultiViewWidget::setModel(Model* m)
{
  if (this->mp_Model)
    disconnect(this->mp_Model, 0, this, 0);
  this->mp_Model = m;
  // Object connection
  connect(this->mp_Model, SIGNAL(segmentsColorChanged(QVector<int>, QVector<QColor>)), this, SLOT(setSegmentsColor(QVector<int>, QVector<QColor>)));
  connect(this->mp_Model, SIGNAL(segmentDefinitionChanged(int)), this, SLOT(updateSegmentDefinition(int)));
  connect(this->mp_Model, SIGNAL(segmentsVisibilityChanged(QVector<int>, QVector<bool>)), this, SLOT(setSegmentsVisibility(QVector<int>, QVector<bool>)));
  connect(this->mp_Model, SIGNAL(segmentsSurfaceVisibilityChanged(QVector<int>, QVector<bool>)), this, SLOT(setSegmentsSurfaceVisibility(QVector<int>, QVector<bool>)));
}

void MultiViewWidget::setChartCycleSettingsManager(ChartCycleSettingsManager* manager)
{
  if (this->mp_ChartCycleSettingsManager != 0)
    this->mp_ChartCycleSettingsManager->disconnect(this);
  this->mp_ChartCycleSettingsManager = manager;

  if (this->mp_ChartCycleSettingsManager != 0)
  {
    connect(this->mp_ChartCycleSettingsManager, SIGNAL(settingAdded()), this, SLOT(addChartCycleSettingAction()));
    connect(this->mp_ChartCycleSettingsManager, SIGNAL(settingModified(int)), this, SLOT(updateChartCycleSettingAction(int)));
    connect(this->mp_ChartCycleSettingsManager, SIGNAL(settingRemoved(int)), this, SLOT(removeChartCycleSettingAction(int)));
    connect(this->mp_ChartCycleSettingsManager, SIGNAL(settingsUpdated()), this, SLOT(updateChartCycleSettingActions()));
    connect(this->mp_ChartCycleSettingsManager, SIGNAL(currentSettingChanged(int)), this, SLOT(updateCurrentChartCycleSettingAction(int)));
  }
  this->updateChartCycleSettingActions();
};

void MultiViewWidget::setView3dActions(QList<QAction*> actions)
{
  this->m_View3dActions = actions;
  for (QList<AbstractView*>::iterator it = this->m_Views.begin() ; it != this->m_Views.end() ; ++it)
  {
    CompositeView* sv = static_cast<CompositeView*>(*it);
    QWidget* w = sv->view(CompositeView::Viz3D);
    w->insertActions(0, this->m_View3dActions);
    w->setContextMenuPolicy(Qt::ActionsContextMenu);
  }
};

void MultiViewWidget::setViewChartActions(QList<QAction*> actions)
{
  this->m_ViewChartActions = actions;
  for (QList<AbstractView*>::iterator it = this->m_Views.begin() ; it != this->m_Views.end() ; ++it)
    static_cast<ChartWidget*>(static_cast<CompositeView*>(*it)->view(CompositeView::Chart))->addActions(this->m_ViewChartActions);
};

void MultiViewWidget::load()
{
  if (!this->mp_Acquisition)
    return;
  
  QString markerUnit = this->mp_Acquisition->pointUnit(Point::Marker);
  double scale = 1.0;
  if (markerUnit.compare("mm") != 0)
  {
    if (markerUnit.compare("m") == 0)
      scale = 1000.0;
    else if (markerUnit.compare("in") == 0)
      scale = 25.4;
    else
      LOG_WARNING("Unknown unit. Impossible to scale correctly the data in the 3D views");
  }
  
  // Update the 3D view
  btk::VTKGroundSource* ground = btk::VTKGroundSource::SafeDownCast((*this->mp_VTKProc)[VTK_GROUND]);
  ground->SetInput(this->mp_Acquisition->btkAcquisition());
  ground->Update();
  btk::VTKGRFsFramesSource* GRFs = btk::VTKGRFsFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_GRFS]);
  btk::VTKMarkersFramesSource* markers = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
  btk::VTKForcePlatformsSource* forcePlaforms = btk::VTKForcePlatformsSource::SafeDownCast((*this->mp_VTKProc)[VTK_FORCE_PLATFORMS]);
  btk::VTKSegmentsFramesSource* segments = btk::VTKSegmentsFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_SEGMENTS]);
  GRFs->SetScaleUnit(scale);
  markers->SetScaleUnit(scale);
  forcePlaforms->SetScaleUnit(scale);
  segments->ClearDefinitions(); // Reset the segments.
  segments->SetScaleUnit(scale);

  this->updateCameras();
  this->updateDisplay(this->mp_Acquisition->firstFrame());
  
  // Update the X axis values for the charts
  this->mp_PointChartAbscissa[0]->Initialize(); // Reset
  this->mp_PointChartAbscissa[0]->SetNumberOfValues(this->mp_Acquisition->pointFrameNumber());
  this->mp_AnalogChartAbscissa[0]->Initialize(); // Reset
  this->mp_AnalogChartAbscissa[0]->SetNumberOfValues(this->mp_Acquisition->analogFrameNumber());
  double sub = 1.0 / (double)this->mp_Acquisition->analogSamplePerPointFrame();
  for (int i = 0 ; i < this->mp_Acquisition->pointFrameNumber() ; ++i)
  {
    double val = static_cast<double>(this->mp_Acquisition->firstFrame() + i);
    // Point
    this->mp_PointChartAbscissa[0]->SetValue(i, val);
    // Analog
    int inc = i * this->mp_Acquisition->analogSamplePerPointFrame();
    for (int j = 0 ; j < this->mp_Acquisition->analogSamplePerPointFrame() ; ++j)
      this->mp_AnalogChartAbscissa[0]->SetValue(inc + j, val + static_cast<double>(j) * sub);
  }
  this->updateChartHorizontalAxisUnit();
  
  // Update video delays
  this->m_VideoDelays.clear();
  for (QMap<int, Video*>::const_iterator it = this->mp_Acquisition->videos().begin() ; it != this->mp_Acquisition->videos().end() ; ++it)
    this->m_VideoDelays.insert(it.key(), it.value()->delay);
  
  // Force the update for the generation of the force platforms and their forces (In case there is no 3D view, they are not updated).
  forcePlaforms->Update();
  GRFs->Update();
  // Force the computation of the direction angle
  this->mp_Acquisition->btkWrenchDirectionAngles()->Update();
  
  // Active the content of each view
  for (QList<AbstractView*>::const_iterator it = this->views().begin() ; it != this->views().end() ; ++it)
    static_cast<CompositeView*>(*it)->show(true);
  
  this->mp_ForceButterflyActivationAction->setEnabled(!this->mp_Acquisition->btkForcePlatforms()->IsEmpty());
};

void MultiViewWidget::setCurrentFrameFunctor(btk::VTKCurrentFrameFunctor::Pointer functor)
{
  for (QList<AbstractView*>::iterator it = this->m_Views.begin() ; it != this->m_Views.end() ; ++it)
  {
    static_cast<ChartWidget*>(static_cast<CompositeView*>(*it)->view(CompositeView::Chart))->setCurrentFrameFunctor(functor);
    static_cast<VideoWidget*>(static_cast<CompositeView*>(*it)->view(CompositeView::MediaVideo))->setCurrentFrameFunctor(functor);
  }
};

void MultiViewWidget::setRegionOfInterestFunctor(btk::VTKRegionOfInterestFunctor::Pointer functor)
{
  for (QList<AbstractView*>::iterator it = this->m_Views.begin() ; it != this->m_Views.end() ; ++it)
    static_cast<ChartWidget*>(static_cast<CompositeView*>(*it)->view(CompositeView::Chart))->setRegionOfInterestFunctor(functor);
};

void MultiViewWidget::setEventsFunctor(btk::VTKEventsFunctor::Pointer functor)
{
  functor->SetFrameMapper(this->mp_EventsFrameMapperFunctor);
  for (QList<AbstractView*>::iterator it = this->m_Views.begin() ; it != this->m_Views.end() ; ++it)
    static_cast<ChartWidget*>(static_cast<CompositeView*>(*it)->view(CompositeView::Chart))->setEventsFunctor(functor);
};

void MultiViewWidget::setEventFilterObject(QObject* filter)
{
  this->mp_EventFilterObject = filter;
  for (QList<AbstractView*>::iterator it = this->m_Views.begin() ; it != this->m_Views.end() ; ++it)
    for (int i = 0 ; i < (*it)->viewStack->count() ; ++i)
      (*it)->viewStack->widget(i)->installEventFilter(this->mp_EventFilterObject);
};

static const int MultiViewWidgetMagic = 0x0abc;
static const int MultiViewWidgetVersion1 = 0x0100; // 1.0 
static const int MultiViewWidgetVersion2 = 0x0200; // 2.0 
static const int MultiViewWidgetViewId = 0x0001;
static const int MultiViewWidgetSplitterId = 0x0002;

QByteArray MultiViewWidget::saveLayout() const
{
  QByteArray data;
  QDataStream stream(&data, QIODevice::WriteOnly);
  stream.setVersion(QDataStream::Qt_4_6);
  
  stream << qint32(MultiViewWidgetMagic);
  stream << qint32(MultiViewWidgetVersion2);
  
  if (!this->saveLayout(stream, static_cast<QGridLayout*>(this->layout())->itemAtPosition(0,0)->widget()))
    return QByteArray();
  
  return data;
};

bool MultiViewWidget::saveLayout(QDataStream& stream, QWidget* w) const
{
  QSplitter* splitter = qobject_cast<QSplitter*>(w);
  if (splitter)
  {
    float ratio = 0.0f;
    QList<int> sizes = splitter->sizes();
    if (splitter->orientation() == Qt::Vertical)
      ratio = (float)sizes[0] / (float)splitter->height();
    else
      ratio = (float)sizes[0] / (float)splitter->width();
    
    stream << qint32(MultiViewWidgetSplitterId);
    stream << qint32(splitter->orientation());
    stream << ratio;
    
    if (!this->saveLayout(stream, splitter->widget(0)) || !this->saveLayout(stream, splitter->widget(1)))
      return false;
  }
  else
  {
    CompositeView* view = qobject_cast<CompositeView*>(w);
    if (view == 0)
    {
      qCritical("Impossible to save the layout of the view. One of the given widget, is not a view.");
      return false;
    }
    int index = view->convertComboIndexToEnumIndex(view->viewCombo->currentIndex());
    stream << qint32(MultiViewWidgetViewId);
    stream << qint32(index);
    // WARNING: The following code is very dependant of the order of the widget set in the composite view.
    //          Its modification must be reflected here. Otherwise, the layout will be corrupted.
    // View options
    switch(index)
    {
    case CompositeView::Viz3DProjection:
      {
      double focalPoint[3], position[3], viewUp[3];
      static_cast<Viz3DWidget*>(view->view(CompositeView::Viz3D))->projectionCamera(focalPoint, position, viewUp);
      for (int i = 0 ; i < 3 ; ++i)
        stream << focalPoint[i];
      for (int i = 0 ; i < 3 ; ++i)
        stream << position[i];
      for (int i = 0 ; i < 3 ; ++i)
        stream << viewUp[i];
      break;
      }
    case CompositeView::Viz3DOrthogonal:
      stream << qint32(static_cast<QComboBox*>(view->optionStack->currentWidget())->currentIndex());
      break;
    case CompositeView::ChartPoint:
      stream << qint32(static_cast<QCheckBox*>(view->optionStack->currentWidget()->layout()->itemAt(0)->layout()->itemAt(0)->widget())->checkState());
      stream << qint32(static_cast<QCheckBox*>(view->optionStack->currentWidget()->layout()->itemAt(0)->layout()->itemAt(2)->widget())->checkState());
      stream << qint32(static_cast<QCheckBox*>(view->optionStack->currentWidget()->layout()->itemAt(0)->layout()->itemAt(4)->widget())->checkState());
      break;
    case CompositeView::ChartAnalog:
      stream << qint32(static_cast<QComboBox*>(view->optionStack->currentWidget()->layout()->itemAt(0)->layout()->itemAt(0)->widget())->currentIndex());
      break;
    }
  }
  return true;
};

bool MultiViewWidget::restoreLayout(const QByteArray& state)
{
  QByteArray sd = state;
  QDataStream stream(&sd, QIODevice::ReadOnly);
  
  qint32 magic;
  qint32 version;
  
  stream >> magic;
  stream >> version;
  if (magic != MultiViewWidgetMagic)
  {
    qCritical("The data given to set the layout of the views are not recognized.");
    return false;
  }
  if (version == 0x0100)
  {
    stream.setVersion(QDataStream::Qt_4_6);
    this->closeAll();
    CompositeView* view = static_cast<CompositeView*>(static_cast<QGridLayout*>(this->layout())->itemAtPosition(0,0)->widget());
    return this->restoreLayout(stream, view, this->size());
  }
  else if (version == 0x0200)
  {
    stream.setVersion(QDataStream::Qt_4_6);
    this->closeAll();
    CompositeView* view = static_cast<CompositeView*>(static_cast<QGridLayout*>(this->layout())->itemAtPosition(0,0)->widget());
    return this->restoreLayout2(stream, view, this->size());
  }
  else
    qCritical("Unknown version number for serialized data containing the layout of the views. You may have used a new version containing some major changes in the data format.");
  return false;
};

bool MultiViewWidget::restoreLayout(QDataStream& stream, CompositeView* view, const QSize& size)
{
  int id;
  stream >> id;
  if (id == MultiViewWidgetSplitterId)
  {
    AbstractView* views[2];
    QSize viewsSize[2];
    QList<int> sizes;
    QSplitter* splitter = 0;
    
    if (!this->restoreLayoutView(stream, view, size, &splitter, views, viewsSize, sizes)
        || !this->restoreLayout(stream, static_cast<CompositeView*>(views[0]), viewsSize[0])
        || !this->restoreLayout(stream, static_cast<CompositeView*>(views[1]), viewsSize[1]))
      return false;
    
    splitter->setSizes(sizes);
  }
  else if (id == MultiViewWidgetViewId)
  {
    int index;
    stream >> index;
    view->viewCombo->setCurrentIndex(view->convertEnumIndexToComboIndex(index));
    int optionIndex = view->optionStackIndexFromViewComboIndex(index);
    switch(index)
    {
    case CompositeView::Viz3DOrthogonal:
      {
      int index2;
      stream >> index2;
      static_cast<QComboBox*>(view->optionStack->widget(optionIndex))->setCurrentIndex(index2);
      break;
      }
    case CompositeView::ChartPoint:
      {
      int checked;
      stream >> checked;
      static_cast<QCheckBox*>(view->optionStack->widget(optionIndex)->layout()->itemAt(0)->layout()->itemAt(0)->widget())->setCheckState(checked == 0 ? Qt::Unchecked : Qt::Checked);
      stream >> checked;
      static_cast<QCheckBox*>(view->optionStack->widget(optionIndex)->layout()->itemAt(0)->layout()->itemAt(2)->widget())->setCheckState(checked == 0 ? Qt::Unchecked : Qt::Checked);
      stream >> checked;
      static_cast<QCheckBox*>(view->optionStack->widget(optionIndex)->layout()->itemAt(0)->layout()->itemAt(4)->widget())->setCheckState(checked == 0 ? Qt::Unchecked : Qt::Checked);
      break;
      }
    case CompositeView::ChartAnalog:
      {
      int index2;
      stream >> index2;
      static_cast<QComboBox*>(view->optionStack->widget(optionIndex)->layout()->itemAt(0)->layout()->itemAt(0)->widget())->setCurrentIndex(index2);
      break;
      }
    }
  }
  else
  {
    qCritical("Unknown ID when extracting data to set the layout of the views.");
    return false;
  }
  return true;
};

bool MultiViewWidget::restoreLayout2(QDataStream& stream, CompositeView* view, const QSize& size)
{
  int id;
  stream >> id;
  if (id == MultiViewWidgetSplitterId)
  {
    AbstractView* views[2];
    QSize viewsSize[2];
    QList<int> sizes; 
    QSplitter* splitter = 0;
    
    if (!this->restoreLayoutView(stream, view, size, &splitter, views, viewsSize, sizes)
        || !this->restoreLayout2(stream, static_cast<CompositeView*>(views[0]), viewsSize[0])
        || !this->restoreLayout2(stream, static_cast<CompositeView*>(views[1]), viewsSize[1]))
      return false;
    
    splitter->setSizes(sizes);
  }
  else if (id == MultiViewWidgetViewId)
  {
    int index;
    stream >> index;
    view->viewCombo->setCurrentIndex(view->convertEnumIndexToComboIndex(index));
    int optionIndex = view->optionStackIndexFromViewComboIndex(index);
    switch(index)
    {
    case CompositeView::Viz3DProjection:
      {
      double focalPoint[3], position[3], viewUp[3];
      for (int i = 0 ; i < 3 ; ++i)
        stream >> focalPoint[i];
      for (int i = 0 ; i < 3 ; ++i)
        stream >> position[i];
      for (int i = 0 ; i < 3 ; ++i)
        stream >> viewUp[i];
      static_cast<Viz3DWidget*>(view->view(CompositeView::Viz3D))->setProjectionCamera(focalPoint, position, viewUp);
      break;
      }
    case CompositeView::Viz3DOrthogonal:
      {
      // this->restoreLayoutViewViz3DCamera(stream, static_cast<CompositeView*>(view));
      int index2;
      stream >> index2;
      static_cast<QComboBox*>(view->optionStack->widget(optionIndex))->setCurrentIndex(index2);
      break;
      }
    case CompositeView::ChartPoint:
      {
      int checked;
      stream >> checked;
      static_cast<QCheckBox*>(view->optionStack->widget(optionIndex)->layout()->itemAt(0)->layout()->itemAt(0)->widget())->setCheckState(checked == 0 ? Qt::Unchecked : Qt::Checked);
      stream >> checked;
      static_cast<QCheckBox*>(view->optionStack->widget(optionIndex)->layout()->itemAt(0)->layout()->itemAt(2)->widget())->setCheckState(checked == 0 ? Qt::Unchecked : Qt::Checked);
      stream >> checked;
      static_cast<QCheckBox*>(view->optionStack->widget(optionIndex)->layout()->itemAt(0)->layout()->itemAt(4)->widget())->setCheckState(checked == 0 ? Qt::Unchecked : Qt::Checked);
      break;
      }
    case CompositeView::ChartAnalog:
      {
      int index2;
      stream >> index2;
      static_cast<QComboBox*>(view->optionStack->widget(optionIndex)->layout()->itemAt(0)->layout()->itemAt(0)->widget())->setCurrentIndex(index2);
      break;
      }
    }
  }
  else
  {
    qCritical("Unknown ID when extracting data to set the layout of the views.");
    return false;
  }
  return true;
};

bool MultiViewWidget::restoreLayoutView(QDataStream& stream, CompositeView* view, const QSize& size, QSplitter** splitter, AbstractView* views[2], QSize viewsSize[2], QList<int>& sizes)
{
  int orientation;
  float ratio;
  int wol; //width or length
  stream >> orientation;
  stream >> ratio;
  
  *splitter = this->split(view, orientation, views);
  if (orientation == Qt::Horizontal)
  {
    wol = size.width() - (*splitter)->handleWidth();
    viewsSize[0].setWidth(ratio * wol);
    viewsSize[1].setWidth((1.0 - ratio) * wol);
    viewsSize[0].setHeight(size.height());
    viewsSize[1].setHeight(size.height());
  }
  else if (orientation == Qt::Vertical)
  {
    wol = size.height() - (*splitter)->handleWidth();
    viewsSize[0].setWidth(size.width());
    viewsSize[1].setWidth(size.width());
    viewsSize[0].setHeight(ratio * wol);
    viewsSize[1].setHeight((1.0 - ratio) * wol);
  }
  else
  {
    qCritical("Unknown orientation when extracting data to set the layout of the views.");
    return false;
  }
  sizes << ratio * wol << (1.0 - ratio) * wol;
  
  return true;
}

void MultiViewWidget::restoreLayout3DOnly()
{
  QByteArray data;
  QDataStream stream(&data, QIODevice::WriteOnly);
  stream.setVersion(QDataStream::Qt_4_6);
  
  stream << qint32(MultiViewWidgetMagic);
  stream << qint32(MultiViewWidgetVersion1);
  stream << qint32(MultiViewWidgetViewId);
  stream << qint32(1); // 1: Perspective 3D
  
  this->restoreLayout(data);
};

void MultiViewWidget::restoreLayout3DVerbose()
{
  QByteArray data;
  QDataStream stream(&data, QIODevice::WriteOnly);
  stream.setVersion(QDataStream::Qt_4_6);
  
  stream << qint32(MultiViewWidgetMagic);
  stream << qint32(MultiViewWidgetVersion1);
  stream << qint32(MultiViewWidgetSplitterId);
  stream << qint32(Qt::Vertical);
  stream << 0.8f;
  stream << qint32(MultiViewWidgetViewId);
  stream << qint32(1); // 1: Perspective 3D
  stream << qint32(MultiViewWidgetViewId);
  stream << qint32(7); // 7: Logger
  
  this->restoreLayout(data);
};

void MultiViewWidget::restoreLayout3DCharts()
{
  QByteArray data;
  QDataStream stream(&data, QIODevice::WriteOnly);
  stream.setVersion(QDataStream::Qt_4_6);
  
  stream << qint32(MultiViewWidgetMagic);
  stream << qint32(MultiViewWidgetVersion1);
  stream << qint32(MultiViewWidgetSplitterId);
  stream << qint32(Qt::Horizontal);
  stream << 0.5f;
  stream << qint32(MultiViewWidgetViewId);
  stream << qint32(1); // 1: Perspective 3D
  stream << qint32(MultiViewWidgetSplitterId);
  stream << qint32(Qt::Vertical);
  stream << 0.5f;
  stream << qint32(MultiViewWidgetViewId);
  stream << qint32(4); // 4: Point chart
  stream << qint32(Qt::Checked); // Component X displayed
  stream << qint32(Qt::Checked); // Component Y displayed
  stream << qint32(Qt::Checked); // Component Z displayed
  stream << qint32(MultiViewWidgetViewId);
  stream << qint32(5); // 5: Analog chart
  stream << qint32(0); // Collapsed mode
  
  this->restoreLayout(data);
};

ChartDialog* MultiViewWidget::createChartDialog(QWidget* parent)
{
  ChartDialog* dlg = new ChartDialog(parent);
  ChartWidget* source = static_cast<ChartWidget*>(static_cast<CompositeView*>(this->m_Views.first())->view(CompositeView::Chart));
  dlg->chart->copy(source);
  if (source->horizontalDisplayMode() == ChartWidget::CyclicDisplay)
  {
    dlg->chart->detachFunctors();
    dlg->chart->setHorizontalAxisUnit(tr("Frames"), 1.0, 0.0, false);
  }
  QList<QAction*> actions = dlg->chart->chartContent()->actions();
  dlg->chart->chartContent()->removeAction(actions[0]);
  dlg->chart->chartContent()->removeAction(actions[1]);
  dlg->chart->chartContent()->removeAction(actions[3]);
  dlg->chart->chartContent()->removeAction(actions[6]);
  dlg->chart->setAcceptDrops(false);
  return dlg;
};

void MultiViewWidget::updateFramesIndex(int ff)
{
  Q_UNUSED(ff)
  // Chart
  double sub = 1.0 / (double)this->mp_Acquisition->analogSamplePerPointFrame();
  for (int i = 0 ; i < this->mp_Acquisition->pointFrameNumber() ; ++i)
  {
    double val = static_cast<double>(this->mp_Acquisition->firstFrame() + i); 
    // Point
    this->mp_PointChartAbscissa[0]->SetValue(i, val);
    // Analog
    int inc = i * this->mp_Acquisition->analogSamplePerPointFrame();
    for (int j = 0 ; j < this->mp_Acquisition->analogSamplePerPointFrame() ; ++j)
      this->mp_AnalogChartAbscissa[0]->SetValue(inc + j, val + static_cast<double>(j) * sub);
  }
  // Video
  for (QMap<int, Video*>::const_iterator it = this->mp_Acquisition->videos().begin() ; it != this->mp_Acquisition->videos().end() ; ++it)
    this->m_VideoDelays[it.key()] = it.value()->delay;
  // Display
  for (QList<AbstractView*>::const_iterator it = this->m_Views.begin() ; it != this->m_Views.end() ; ++it)
  {
    static_cast<ChartWidget*>(static_cast<CompositeView*>(*it)->view(CompositeView::Chart))->updateHorizontalAxis();
    static_cast<CompositeView*>(*it)->render();
  }
};

void MultiViewWidget::appendNewSegments(const QList<int>& ids, const QList<Segment*>& segments)
{
  btk::VTKSegmentsFramesSource* segmentsFramesSource = btk::VTKSegmentsFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_SEGMENTS]);
  QVector<QColor> colors(segments.size());
  int inc = 0;
  for (QList<Segment*>::const_iterator it = segments.begin() ; it != segments.end() ; ++it)
  {
    colors[inc] = (*it)->color;
    if (ids[inc] >= segmentsFramesSource->GetNumberOfDefinitions())
    {
      int id = segmentsFramesSource->AppendDefinition((*it)->mesh, (*it)->surfaceVisible);
      if (id != ids[inc])
        qDebug("The generated segment's ID in VTK is not the same than in Qt...");
      else
        segmentsFramesSource->SetSegmentVisibility(id, (*it)->visible);
    }
    else
    {
      segmentsFramesSource->SetDefinition(ids[inc], (*it)->mesh);
      segmentsFramesSource->SetSegmentVisibility(ids[inc], (*it)->visible);
      segmentsFramesSource->SetSegmentSurfaceVisibility(ids[inc], (*it)->surfaceVisible);
    }
    ++inc;
  }
  this->setSegmentsColor(ids.toVector(), colors);
};

void MultiViewWidget::clearSegments()
{
  btk::VTKSegmentsFramesSource* segmentsFramesSource = btk::VTKSegmentsFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_SEGMENTS]);
  segmentsFramesSource->ClearDefinitions();
};

void MultiViewWidget::setSegmentsColor(const QVector<int>& ids, const QVector<QColor>& colors)
{
  btk::VTKSegmentsFramesSource* segments = btk::VTKSegmentsFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_SEGMENTS]);
  vtkLookupTable* segmentsColorsLUT = segments->GetSegmentColorLUT();
  for (int i = 0 ; i < ids.count() ; ++i)
  {
    int num = segmentsColorsLUT->GetNumberOfTableValues();
    QColor color = colors[i];
    int j = 0;
    while (j < num)
    {
      double* c = segmentsColorsLUT->GetTableValue(j);
      if ((color.redF() == c[0]) && (color.greenF() == c[1]) && (color.blueF() == c[2]))
      {
        segments->SetSegmentColorIndex(ids[i], j);
        break;
      }
      ++j;
    }
    if (j >= num)
    {
      segmentsColorsLUT->SetNumberOfTableValues(num + 1);
      segmentsColorsLUT->SetTableValue(num, color.redF(), color.greenF(), color.blueF());
      segmentsColorsLUT->SetTableRange(0, num + 1);
      segments->SetSegmentColorIndex(ids[i], num);
    }
  }
  this->updateSegmentsDisplay();
};

void MultiViewWidget::setSegmentDefinition(int id, const QVector<int>& markerIds, const QVector<Pair>& links, const QVector<Triad>& faces)
{
  btk::VTKSegmentsFramesSource* segments = btk::VTKSegmentsFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_SEGMENTS]);
  segments->SetDefinition(id, markerIds.toStdVector(), links.toStdVector(), faces.toStdVector());
  this->updateSegmentsDisplay();
};

void MultiViewWidget::updateSegmentDefinition(int id)
{
  btk::VTKSegmentsFramesSource* segments = btk::VTKSegmentsFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_SEGMENTS]);
  segments->SetDefinition(id, this->mp_Model->segments().value(id)->mesh);
  this->updateSegmentsDisplay();
};

void MultiViewWidget::setSegmentsVisibility(const QVector<int>& ids, const QVector<bool>& visibles)
{
  btk::VTKSegmentsFramesSource* segmentsFramesSource = btk::VTKSegmentsFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_SEGMENTS]);
  for (int i = 0 ; i < ids.count() ; ++i)
    segmentsFramesSource->SetSegmentVisibility(ids[i], visibles[i]);
  this->updateSegmentsDisplay();
};

void MultiViewWidget::setSegmentsSurfaceVisibility(const QVector<int>& ids, const QVector<bool>& visibles)
{
  btk::VTKSegmentsFramesSource* segmentsFramesSource = btk::VTKSegmentsFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_SEGMENTS]);
  for (int i = 0 ; i < ids.count() ; ++i)
    segmentsFramesSource->SetSegmentSurfaceVisibility(ids[i], visibles[i]);
  this->updateSegmentsDisplay();
};

void MultiViewWidget::updateHiddenSegments(const QList<int>& ids)
{
  btk::VTKSegmentsFramesSource* segmentsFramesSource = btk::VTKSegmentsFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_SEGMENTS]);
  segmentsFramesSource->ShowSegments();
  for (int i = 0 ; i < ids.count() ; ++i)
    segmentsFramesSource->HideSegment(ids[i]);
  this->updateSegmentsDisplay();
};

int MultiViewWidget::appendNewSegment(Segment* s)
{
  btk::VTKSegmentsFramesSource* segments = btk::VTKSegmentsFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_SEGMENTS]);
  QVector<QColor> colors(1, s->color);
  int id = segments->AppendDefinition(s->mesh, s->surfaceVisible);
  this->setSegmentsColor(QVector<int>(1,id), colors);
  return id;
};

int MultiViewWidget::segmentColorIndex(int id)
{
  btk::VTKSegmentsFramesSource* segments = btk::VTKSegmentsFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_SEGMENTS]);
  return segments->GetSegmentColorIndex(id);
};

void MultiViewWidget::setMarkerRadius(int id, double r)
{
  btk::VTKMarkersFramesSource* markers = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
  markers->SetMarkerRadius(id, r);
};

void MultiViewWidget::setMarkersRadius(const QVector<int>& ids, const QVector<double>& radii)
{
  btk::VTKMarkersFramesSource* markers = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
  for (int i = 0 ; i < ids.count() ; ++i)
    markers->SetMarkerRadius(ids[i], radii[i]);
  this->updateMarkersDisplay();
};

void MultiViewWidget::setMarkersColor(const QVector<int>& ids, const QVector<QColor>& colors)
{
  btk::VTKMarkersFramesSource* markers = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
  vtkLookupTable* markersColorsLUT = markers->GetMarkerColorLUT();
  for (int i = 0 ; i < ids.count() ; ++i)
  {
    int num = markersColorsLUT->GetNumberOfTableValues();
    QColor color = colors[i];
    int j = 0;
    while (j < num)
    {
      double* c = markersColorsLUT->GetTableValue(j);
      if ((color.redF() == c[0]) && (color.greenF() == c[1]) && (color.blueF() == c[2]))
      {
        markers->SetMarkerColorIndex(ids[i], j);
        break;
      }
      ++j;
    }
    if (j >= num)
    {
      markersColorsLUT->SetNumberOfTableValues(num + 1);
      markersColorsLUT->SetTableValue(num, color.redF(), color.greenF(), color.blueF());
      markersColorsLUT->SetTableRange(0, num + 1);
      markers->SetMarkerColorIndex(ids[i], num);
    }
  }
  this->updateMarkersDisplay();
};

void MultiViewWidget::updateVisibleMarkers(const QList<int>& ids)
{
  btk::VTKMarkersFramesSource* markersFramesSource = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
  markersFramesSource->HideMarkers();
  for (int i = 0 ; i < ids.count() ; ++i)
    markersFramesSource->ShowMarker(ids[i]);
  this->updateMarkersDisplay();
};

void MultiViewWidget::updateHiddenMarkers(const QList<int>& ids)
{
  btk::VTKMarkersFramesSource* markersFramesSource = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
  markersFramesSource->ShowMarkers();
  for (int i = 0 ; i < ids.count() ; ++i)
    markersFramesSource->HideMarker(ids[i]);
  this->updateMarkersDisplay();
};

void MultiViewWidget::updateTrackedMarkers(const QList<int>& ids)
{
  btk::VTKMarkersFramesSource* markersFramesSource = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
  markersFramesSource->HideTrajectories();
  for (int i = 0 ; i < ids.count() ; ++i)
    markersFramesSource->ShowTrajectory(ids[i]);
  this->updateMarkersDisplay();
};

void MultiViewWidget::updateTrackedGRFPaths(const QList<int>& ids)
{
  btk::VTKGRFsFramesSource* GRFs = btk::VTKGRFsFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_GRFS]);
  GRFs->HidePaths();
  for (int i = 0 ; i < ids.count() ; ++i)
    GRFs->ShowPath(ids[i]);
  GRFs->GetOutput()->GetInformation()->Remove(vtkDataObject::DATA_TIME_STEPS());
  this->mp_Mappers->InitTraversal();
  vtkMapper* mapper;
  while ((mapper = this->mp_Mappers->GetNextItem()) != NULL)
    mapper->Modified();
  this->updateViews();
};

void MultiViewWidget::setMarkersVisibility(const QVector<int>& ids, const QVector<bool>& visibles)
{
  btk::VTKMarkersFramesSource* markersFramesSource = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
  for (int i = 0 ; i < ids.count() ; ++i)
    markersFramesSource->SetMarkerVisibility(ids[i], visibles[i]);
  this->updateMarkersDisplay();
};

void MultiViewWidget::setMarkersTrajectoryVisibility(const QVector<int>& ids, const QVector<bool>& visibles)
{
  btk::VTKMarkersFramesSource* markersFramesSource = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
  for (int i = 0 ; i < ids.count() ; ++i)
    markersFramesSource->SetTrajectoryVisibility(ids[i], visibles[i]);
  this->updateMarkersDisplay();
};

void MultiViewWidget::markersConfiguration(const QList<int>& ids, QList<bool>& visibles, QList<bool>& trajectories, QList<double>& radii, QList<QColor>& colors)
{
  btk::VTKMarkersFramesSource* markers = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
  for (int i = 0 ; i < ids.count() ; ++i)
  {
    int id = ids[i];
    visibles << markers->GetMarkerVisibility(id);
    trajectories << markers->GetTrajectoryVisibility(id);
    radii << markers->GetMarkerRadius(id);
    double* c = markers->GetMarkerColor(id);
    colors << QColor(static_cast<int>(c[0]*255.0), static_cast<int>(c[1]*255.0), static_cast<int>(c[2]*255.0));
  };
};

void MultiViewWidget::setMarkersConfiguration(const QList<int>& ids, const QList<bool>& visibles, const QList<bool>& trajectories, const QList<double>& radii, const QList<QColor>& colors)
{
  btk::VTKMarkersFramesSource* markers = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
  for (int i = 0 ; i < ids.count() ; ++i)
  {
    int id = ids[i];
    markers->SetMarkerVisibility(id, visibles[i]);
    markers->SetTrajectoryVisibility(id, trajectories[i]);
    markers->SetMarkerRadius(id, radii[i]);
  }
  this->setMarkersColor(ids.toVector(), colors.toVector()); // Will update the display
};

double MultiViewWidget::markerRadius(int id)
{
  btk::VTKMarkersFramesSource* markers = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
  return markers->GetMarkerRadius(id);
};

void MultiViewWidget::setMarkerColorIndex(int id, int idx)
{
  btk::VTKMarkersFramesSource* markers = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
  markers->SetMarkerColorIndex(id, idx);
};

int MultiViewWidget::markerColorIndex(int id)
{
  btk::VTKMarkersFramesSource* markers = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
  return markers->GetMarkerColorIndex(id);
};

void MultiViewWidget::setMarkerVisibility(int id, bool visible)
{
  btk::VTKMarkersFramesSource* markers = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
  markers->SetMarkerVisibility(id, visible);
};

bool MultiViewWidget::markerVisibility(int id)
{
  btk::VTKMarkersFramesSource* markers = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
  return markers->GetMarkerVisibility(id);
};

double* MultiViewWidget::markerColorValue(int c)
{
  btk::VTKMarkersFramesSource* markers = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
  return markers->GetMarkerColorLUT()->GetTableValue(c);
}

bool MultiViewWidget::appendNewMarkerColor(const QColor& color, int* idx)
{
  bool modified = false;
  vtkLookupTable* markersColorsLUT = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS])->GetMarkerColorLUT();
  int num = markersColorsLUT->GetNumberOfTableValues();
  int i = 0;
  for (i = 0 ; i < num ; ++i)
  {
    double* c = markersColorsLUT->GetTableValue(i);
    if ((color.redF() == c[0]) && (color.greenF() == c[1]) && (color.blueF() == c[2]))
      break;
  }
  if (i >= num)
  {
    markersColorsLUT->SetNumberOfTableValues(num + 1);
    markersColorsLUT->SetTableValue(num, color.redF(), color.greenF(), color.blueF());
    markersColorsLUT->SetTableRange(0, num + 1);
    i = num;
    modified = true;
  }
  *idx = i;
  return modified;
};

const QString MultiViewWidget::groundNormalAsString() const
{
  QString str = "+Z";
  btk::VTKGroundSource* ground = btk::VTKGroundSource::SafeDownCast((*this->mp_VTKProc)[VTK_GROUND]);
  double n[3]; ground->GetNormal(n);
  if (n[0] == 1.0)
    str = "+X";
  else if (n[1] == 1.0)
    str = "+Y";
  return str;
};

void MultiViewWidget::setDefaultGroundOrientation(int index)
{
  btk::VTKGroundSource* ground = btk::VTKGroundSource::SafeDownCast((*this->mp_VTKProc)[VTK_GROUND]);
  if (index == 0)
    ground->SetAutomaticDefaultOrientation(btk::VTKGroundSource::PlaneXY);
  else if (index == 1)
    ground->SetAutomaticDefaultOrientation(btk::VTKGroundSource::PlaneYZ);
  else if (index == 2)
    ground->SetAutomaticDefaultOrientation(btk::VTKGroundSource::PlaneZX);
  else
  {
    qDebug("Unknown index to set the default ground orientation");
    return;
  }
    
  if (ground->GetOrientation() == btk::VTKGroundSource::Automatic)
  {
    this->updateCameras();
    this->updateViews();
  }
};

void MultiViewWidget::setDefaultBackgroundColor(const QColor& color)
{
  for (QList<AbstractView*>::const_iterator it = this->views().begin() ; it != this->views().end() ; ++it)
    static_cast<Viz3DWidget*>(static_cast<CompositeView*>(*it)->view(CompositeView::Viz3D))->renderer()->SetBackground(color.redF(), color.greenF(), color.blueF());
  this->updateViews();
};

void MultiViewWidget::setDefaultGridFrontColor(const QColor& color)
{
  (*this->mp_VTKActor)[VTK_GROUND]->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
  this->updateViews();
};

void MultiViewWidget::setDefaultGridBackColor(const QColor& color)
{
  (*this->mp_VTKActor)[VTK_GROUND]->GetBackfaceProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
  this->updateViews();
};

void MultiViewWidget::setDefaultSegmentColor(const QColor& color)
{
  this->mp_Model->setDefaultSegmentColor(color);
  vtkLookupTable* segmentsColorsLUT = btk::VTKSegmentsFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_SEGMENTS])->GetSegmentColorLUT();
  segmentsColorsLUT->SetTableValue(0, color.redF(), color.greenF(), color.blueF());
  this->updateViews();
};

void MultiViewWidget::setDefaultMarkerRadius(double r)
{
  this->mp_Acquisition->setDefaultMarkerRadius(r);
  btk::VTKMarkersFramesSource* markers = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
  markers->SetDefaultMarkerRadius(r);
};

void MultiViewWidget::setDefaultMarkerColor(const QColor& color)
{
  this->mp_Acquisition->setDefaultMarkerColor(color);
  vtkLookupTable* markersColorsLUT = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS])->GetMarkerColorLUT();
  markersColorsLUT->SetTableValue(0, color.redF(), color.greenF(), color.blueF());
  this->updateViews();
};

void MultiViewWidget::setMarkerTrajectoryLength(int index)
{
  if (index == 0) // All
    this->mp_ActionMarkerTrajectoryFull->trigger();
  else if (index == 1) // 25 frames
    this->mp_ActionMarkerTrajectory25->trigger();
  else if (index == 2) // 50 frames
    this->mp_ActionMarkerTrajectory50->trigger();
  else if (index == 3) // 100 frames
    this->mp_ActionMarkerTrajectory100->trigger();
  else if (index == 4) // 200 frames
    this->mp_ActionMarkerTrajectory200->trigger();
};

void MultiViewWidget::showForcePlatformAxes(bool isShown)
{
  btk::VTKForcePlatformsSource* forcePlaforms = btk::VTKForcePlatformsSource::SafeDownCast((*this->mp_VTKProc)[VTK_FORCE_PLATFORMS]);
  forcePlaforms->SetShowAxes(isShown);
  this->updateMarkersDisplay();
};

void MultiViewWidget::showForcePlatformIndex(bool isShown)
{
  btk::VTKForcePlatformsSource* forcePlaforms = btk::VTKForcePlatformsSource::SafeDownCast((*this->mp_VTKProc)[VTK_FORCE_PLATFORMS]);
  forcePlaforms->SetShowIndex(isShown);
  this->updateMarkersDisplay();
};

void MultiViewWidget::setForcePlatformColor(const QColor& color)
{
  this->m_ForcePlatformColor = color;
  (*this->mp_VTKActor)[VTK_FORCE_PLATFORMS]->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
  (*this->mp_VTKActor)[VTK_GRFS_PWA]->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
  this->updateViews();
};

void MultiViewWidget::setForceVectorColor(const QColor& color)
{
  this->m_ForceVectorColor = color;
  (*this->mp_VTKActor)[VTK_GRFS_VECTOR]->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
  this->updateViews();
};

void MultiViewWidget::setGRFButterflyActivation(bool activated)
{
  btk::VTKGRFsFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_GRFS])->SetButterflyActivation(activated);
  this->updateDisplay();
};

void MultiViewWidget::setVideoDelay(int id, double d)
{
  this->m_VideoDelays[id] = static_cast<qint64>(d * 1000.0);
};

void MultiViewWidget::setVideoDelays(QVector<int> ids, QVector<qint64> delays)
{
  for (int i = 0 ; i < ids.count() ; ++i)
    this->m_VideoDelays[ids[i]] = delays[i];
  this->updateViews();
};

void MultiViewWidget::updateDisplayedMarkersList(vtkObject* /* caller */, unsigned long /* vtk_event */, void* /* client_data */, void* call_data)
{
  vtkIntArray* indexes = static_cast<vtkIntArray*>(call_data);
  if (!indexes)
    return;
    
  QVector<int> ids = QVector<int>(indexes->GetSize());
  for (int i = 0; i < ids.size() ; ++i)
    ids[i] = (indexes->GetValue(i) == -1 ? 0 : 1);
  emit visibleMarkersChanged(ids);
};

void MultiViewWidget::clear()
{
  for (QList<AbstractView*>::const_iterator it = this->views().begin() ; it != this->views().end() ; ++it)
    static_cast<CompositeView*>(*it)->show(false);
  this->mp_ForceButterflyActivationAction->setEnabled(false);
};

void MultiViewWidget::circleSelectedMarkers(const QList<int>& ids)
{
  btk::VTKMarkersFramesSource* markers = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
  markers->ClearSelectedMarkers();
  for (QList<int>::const_iterator it = ids.begin() ; it != ids.end() ; ++it)
    markers->SetSelectedMarker(*it);
  this->updateMarkersDisplay();
};

void MultiViewWidget::updateSegmentsDisplay()
{
  btk::VTKSegmentsFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_SEGMENTS])->GetOutput()->GetInformation()->Remove(vtkDataObject::DATA_TIME_STEPS());
  this->mp_Mappers->InitTraversal();
  vtkMapper* mapper;
  while ((mapper = this->mp_Mappers->GetNextItem()) != NULL)
    mapper->Modified();
  this->updateViews();
};

void MultiViewWidget::updateMarkersDisplay()
{
  btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS])->GetOutput()->GetInformation()->Remove(vtkDataObject::DATA_TIME_STEPS());
  this->mp_Mappers->InitTraversal();
  vtkMapper* mapper;
  while ((mapper = this->mp_Mappers->GetNextItem()) != NULL)
    mapper->Modified();
  this->updateViews();
};

void MultiViewWidget::updateDisplay()
{
  // Force to update the markers even if the required frame was the last updated.
  btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS])->GetOutput()->GetInformation()->Remove(vtkDataObject::DATA_TIME_STEPS());
  // Same thing for the segments
  btk::VTKSegmentsFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_SEGMENTS])->GetOutput()->GetInformation()->Remove(vtkDataObject::DATA_TIME_STEPS());
  // And also the GRFs
  btk::VTKGRFsFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_GRFS])->GetOutput()->GetInformation()->Remove(vtkDataObject::DATA_TIME_STEPS());
  // Update
  this->mp_Mappers->InitTraversal();
  vtkMapper* mapper;
  while ((mapper = this->mp_Mappers->GetNextItem()) != NULL)
    mapper->Modified();
  this->updateViews();
};

void MultiViewWidget::updateDisplay(int frame)
{
  // Force to update the markers even if the required frame was the last updated.
  btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS])->GetOutput()->GetInformation()->Remove(vtkDataObject::DATA_TIME_STEPS());
  // Same thing for the segments
  btk::VTKSegmentsFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_SEGMENTS])->GetOutput()->GetInformation()->Remove(vtkDataObject::DATA_TIME_STEPS());
  // And also the GRFs
  btk::VTKGRFsFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_GRFS])->GetOutput()->GetInformation()->Remove(vtkDataObject::DATA_TIME_STEPS());
  // Update
  double t = static_cast<double>(frame - this->mp_Acquisition->firstFrame());
  for (vtkStreamingDemandDrivenPipelineCollection::iterator it = this->mp_Syncro->begin() ; it != this->mp_Syncro->end() ; ++it)
    (*it)->SetUpdateTimeStep(0, t);
  this->mp_Mappers->InitTraversal();
  vtkMapper* mapper;
  while ((mapper = this->mp_Mappers->GetNextItem()) != NULL)
    mapper->Modified();
  this->updateViews();
};

void MultiViewWidget::showAllMarkers()
{
  btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS])->ShowMarkers();
  this->updateMarkersDisplay();
};

void MultiViewWidget::hideAllMarkers()
{
  btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS])->HideMarkers();
  this->updateMarkersDisplay();
};

void MultiViewWidget::adaptViewsForPlaybackOn()
{
  // Normal drawing of the rubber band in the 3D view
  for (QList<AbstractView*>::iterator it = this->m_Views.begin() ; it != this->m_Views.end() ; ++it)
    static_cast<btk::VTKInteractorStyleTrackballFixedUpCamera*>(static_cast<Viz3DWidget*>(static_cast<CompositeView*>(*it)->view(CompositeView::Viz3D))->GetRenderWindow()->GetInteractor()->GetInteractorStyle())->ForceRubberBandDrawingOff();
  // Start the video playback
  for (QList<AbstractView*>::iterator it = this->m_Views.begin() ; it != this->m_Views.end() ; ++it)
    static_cast<VideoWidget*>(static_cast<CompositeView*>(*it)->view(CompositeView::MediaVideo))->start();
};

void MultiViewWidget::adaptViewsForPlaybackOff()
{
  // Force to draw the rubber band in the 3D view
  for (QList<AbstractView*>::iterator it = this->m_Views.begin() ; it != this->m_Views.end() ; ++it)
    static_cast<btk::VTKInteractorStyleTrackballFixedUpCamera*>(static_cast<Viz3DWidget*>(static_cast<CompositeView*>(*it)->view(CompositeView::Viz3D))->GetRenderWindow()->GetInteractor()->GetInteractorStyle())->ForceRubberBandDrawingOn();
  this->updateViews(); // To have the front and back buffer with the same content.
  // Stop the video playback
  for (QList<AbstractView*>::iterator it = this->m_Views.begin() ; it != this->m_Views.end() ; ++it)
    static_cast<VideoWidget*>(static_cast<CompositeView*>(*it)->view(CompositeView::MediaVideo))->stop();
};

void MultiViewWidget::updateChartHorizontalAxisUnit()
{
  if (this->mp_ActionChartAxisFrame->isChecked())
    this->displayChartHorizontalAxisAsFrame();
  else if (this->mp_ActionChartAxisTime->isChecked())
    this->displayChartHorizontalAxisAsTime();
  else
  {
    for (int i = 0 ; i < ChartCycleSettingsManager::maxCycleSettings ; ++i)
    {
      if (this->mp_ActionCycleSettings[i]->isChecked())
      {
        this->displayChartHorizontalAxisAsCycle(i);
        break;
      }
    }
  }
};

void MultiViewWidget::setChartHorizontalAxisUnit(int index)
{
  if (index == 0)
    this->setChartHorizontalAxisUnitToFrame();
  else if (index == 1)
    this->setChartHorizontalAxisUnitToTime();
  else
    this->setChartHorizontalAxisUnitToCycle(2-index);
};

void MultiViewWidget::setChartHorizontalAxisUnitToFrame()
{
  this->mp_ActionChartAxisFrame->trigger();
};

void MultiViewWidget::setChartHorizontalAxisUnitToTime()
{
  this->mp_ActionChartAxisTime->trigger();
};

void MultiViewWidget::setChartHorizontalAxisUnitToCycle(int index)
{
  if (index < ChartCycleSettingsManager::maxCycleSettings)
    this->mp_ActionCycleSettings[index]->trigger();
};

void MultiViewWidget::displayChartHorizontalAxisAsFrame()
{
  for (QList<AbstractView*>::iterator it = this->m_Views.begin() ; it != this->m_Views.end() ; ++it)
  {
    ChartWidget* w = static_cast<ChartWidget*>(static_cast<CompositeView*>(*it)->view(CompositeView::Chart));
    if (w->horizontalDisplayMode() == ChartWidget::CyclicDisplay)
      w->setHorizontalAxisRange(this->mp_Acquisition->firstFrame(), this->mp_Acquisition->lastFrame());
    w->setHorizontalAxisUnit(tr("Frame"), 1.0, 0.0);
  }
  this->mp_EventsFrameMapperFunctor->SetActivated(false);
  
  this->mp_ChartCycleSettingsManager->blockSignals(true);
  this->mp_ChartCycleSettingsManager->setCurrentSetting(-1);
  this->mp_ChartCycleSettingsManager->blockSignals(false);
};

void MultiViewWidget::displayChartHorizontalAxisAsTime()
{
  double f = this->mp_Acquisition->btkAcquisition() ? this->mp_Acquisition->pointFrequency() : 1.0;
  for (QList<AbstractView*>::iterator it = this->m_Views.begin() ; it != this->m_Views.end() ; ++it)
  {
    ChartWidget* w = static_cast<ChartWidget*>(static_cast<CompositeView*>(*it)->view(CompositeView::Chart));
    if (w->horizontalDisplayMode() == ChartWidget::CyclicDisplay)
      w->setHorizontalAxisRange(this->mp_Acquisition->firstFrame(), this->mp_Acquisition->lastFrame());
    w->setHorizontalAxisUnit(tr("Time (s)"), 1.0 / f, -1.0);
  }
  this->mp_EventsFrameMapperFunctor->SetActivated(false);
  
  this->mp_ChartCycleSettingsManager->blockSignals(true);
  this->mp_ChartCycleSettingsManager->setCurrentSetting(-1);
  this->mp_ChartCycleSettingsManager->blockSignals(false);
};

void MultiViewWidget::displayChartHorizontalAxisAsCycle(int index)
{
  ChartCycleSetting setting = this->mp_ChartCycleSettingsManager->setting(index);
  if (this->mp_Acquisition->btkAcquisition() != NULL)
  {
    QList<int> rightBegin, rightEnd, leftBegin, leftEnd, generalBegin, generalEnd;
    // Extract the events use for the right/left/general cycles
    for (QMap<int,Event*>::const_iterator it = this->mp_Acquisition->events().begin() ; it != this->mp_Acquisition->events().end() ; ++it)
    {
      Event* e = it.value();
      if ((e->frame < this->mp_Acquisition->firstFrame()) || (e->frame > this->mp_Acquisition->lastFrame()))
        continue;
      if (e->context.compare("Right", Qt::CaseInsensitive) == 0)
      {
        if (e->label.compare(setting.rightEvents[0], Qt::CaseInsensitive) == 0)
          rightBegin.push_back(e->frame);
        if (e->label.compare(setting.rightEvents[1], Qt::CaseInsensitive) == 0)
          rightEnd.push_back(e->frame);
      }
      else if (e->context.compare("Left", Qt::CaseInsensitive) == 0)
      {
        if (e->label.compare(setting.leftEvents[0], Qt::CaseInsensitive) == 0)
          leftBegin.push_back(e->frame);
        if (e->label.compare(setting.leftEvents[1], Qt::CaseInsensitive) == 0)
          leftEnd.push_back(e->frame);
      }
      else
      {
        if (e->label.compare(setting.generalEvents[0], Qt::CaseInsensitive) == 0)
          generalBegin.push_back(e->frame);
        if (e->label.compare(setting.generalEvents[1], Qt::CaseInsensitive) == 0)
          generalEnd.push_back(e->frame);
      }
    }
    // Sort them, create array with the definition of the cycles and fill abscissa for the cycles
    qSort(rightBegin); qSort(rightEnd); qSort(leftBegin); qSort(leftEnd); qSort(generalBegin); qSort(generalEnd);
    QList<int> rightCycles, leftCycles, generalCycles;
    this->createCyclesFromEventsFrame(1, rightBegin, rightEnd);
    this->createCyclesFromEventsFrame(2, leftBegin, leftEnd);
    this->createCyclesFromEventsFrame(3, generalBegin, generalEnd);
  }
  this->mp_EventsFrameMapperFunctor->SetActivated(true);
  
  for (QList<AbstractView*>::iterator it = this->m_Views.begin() ; it != this->m_Views.end() ; ++it)
  {
    ChartWidget* w = static_cast<ChartWidget*>(static_cast<CompositeView*>(*it)->view(CompositeView::Chart));
    w->setDataToCycleMatchingRules(setting.rightLabelRule, setting.rightLabelRuleText, setting.leftLabelRule, setting.leftLabelRuleText);
    w->setHorizontalAxisUnit(setting.horizontalAxisTitle, 1.0, 0.0, true);
    w->setHorizontalAxisRange(0.0, this->mp_Acquisition->btkAcquisition() ? 100.0 : 0.0);
  }
  
  this->mp_ChartCycleSettingsManager->blockSignals(true);
  this->mp_ChartCycleSettingsManager->setCurrentSetting(index);
  this->mp_ChartCycleSettingsManager->blockSignals(false);
  
  this->updateViews();
};

// ctx (context) - 1: Right, 2: Left, 3: General
void MultiViewWidget::createCyclesFromEventsFrame(int ctx, const QList<int>& begin, const QList<int>& end)
{
  this->mp_PointChartAbscissa[ctx]->Initialize(); // Reset
  this->mp_PointChartAbscissa[ctx]->SetNumberOfValues(this->mp_Acquisition->pointFrameNumber());
  this->mp_AnalogChartAbscissa[ctx]->Initialize(); // Reset
  this->mp_AnalogChartAbscissa[ctx]->SetNumberOfValues(this->mp_Acquisition->analogFrameNumber());
  this->mp_PointChartCycleBoundaries[ctx-1]->Initialize();
  this->mp_AnalogChartCycleBoundaries[ctx-1]->Initialize();
  double sub = 1.0 / (double)this->mp_Acquisition->analogSamplePerPointFrame();
  for (int i = 0 ; i < this->mp_Acquisition->pointFrameNumber() ; ++i)
  {
    // Point
    this->mp_PointChartAbscissa[ctx]->SetValue(i, 0.0);
    // Analog
    int inc = i * this->mp_Acquisition->analogSamplePerPointFrame();
    for (int j = 0 ; j < this->mp_Acquisition->analogSamplePerPointFrame() ; ++j)
      this->mp_AnalogChartAbscissa[ctx]->SetValue(inc + j, 0.0);
  }
  
  if (!begin.isEmpty() && !end.isEmpty())
  {
    QList<int>::const_iterator itBegin = begin.begin();
    for (QList<int>::const_iterator itEnd = end.begin(); itEnd != end.end() ; ++itEnd)
    {
      if (*itEnd > *itBegin)
      {
        // To use the closest "begin" event with the current "end" event
        QList<int>::const_iterator itNextBegin = itBegin; ++itNextBegin;
        while (itNextBegin != begin.end())
        {
          if (*itNextBegin < *itEnd)
            ++itBegin;
          else
            break;
          ++itNextBegin;
        };
        
        this->mp_PointChartCycleBoundaries[ctx-1]->InsertNextValue(*itBegin - this->mp_Acquisition->firstFrame());
        this->mp_PointChartCycleBoundaries[ctx-1]->InsertNextValue(0.0f);
        this->mp_PointChartCycleBoundaries[ctx-1]->InsertNextValue(*itEnd - this->mp_Acquisition->firstFrame());
        this->mp_PointChartCycleBoundaries[ctx-1]->InsertNextValue(100.0f);
        this->mp_AnalogChartCycleBoundaries[ctx-1]->InsertNextValue((*itBegin - this->mp_Acquisition->firstFrame()) * this->mp_Acquisition->analogSamplePerPointFrame());
        this->mp_AnalogChartCycleBoundaries[ctx-1]->InsertNextValue(0.0f);
        this->mp_AnalogChartCycleBoundaries[ctx-1]->InsertNextValue((*itEnd - this->mp_Acquisition->firstFrame()) * this->mp_Acquisition->analogSamplePerPointFrame());
        this->mp_AnalogChartCycleBoundaries[ctx-1]->InsertNextValue(100.0f);
        double scale = 100.0 / static_cast<double>(*itEnd - *itBegin);
        for (int i = *itBegin ; i <= *itEnd ; ++i)
        {
          int idx = i - this->mp_Acquisition->firstFrame();
          double val = static_cast<double>(i - *itBegin) * scale;
          // Point
          this->mp_PointChartAbscissa[ctx]->SetValue(idx, val);
          // Analog
          int inc = idx * this->mp_Acquisition->analogSamplePerPointFrame(); 
          for (int j = 0 ; j < this->mp_Acquisition->analogSamplePerPointFrame() ; ++j)
            this->mp_AnalogChartAbscissa[ctx]->SetValue(inc + j, val + j * sub);
        }
        ++itBegin;
        if (itBegin == begin.end())
          break;
      }
    }
  }
};

void MultiViewWidget::showChartEvent(bool visible)
{
  for (QList<AbstractView*>::iterator it = this->m_Views.begin() ; it != this->m_Views.end() ; ++it)
  {
    ChartWidget* w = static_cast<ChartWidget*>(static_cast<CompositeView*>(*it)->view(CompositeView::Chart));
    w->setEventDisplay(visible);
  }
};

void MultiViewWidget::setDefaultPlotLineWidth(double width)
{
  for (QList<AbstractView*>::iterator it = this->m_Views.begin() ; it != this->m_Views.end() ; ++it)
  {
    ChartWidget* w = static_cast<ChartWidget*>(static_cast<CompositeView*>(*it)->view(CompositeView::Chart));
    w->setDefaultLineWidth(width);
  }
};

void MultiViewWidget::exportChartToImage(btk::VTKChartTimeSeries* chart)
{
  emit pausePlaybackRequested(true);
  if (this->mp_ChartExporter == 0)
    this->mp_ChartExporter = new ChartExportDialog(this);
  this->mp_ChartExporter->setChart(chart);
  this->mp_ChartExporter->exec();
  emit pausePlaybackRequested(false);
};

void MultiViewWidget::dragEnterEvent(QDragEnterEvent *event)
{
  if (event->mimeData()->hasFormat("text/uri-list"))
    event->acceptProposedAction();
};

void MultiViewWidget::dropEvent(QDropEvent *event)
{
  QList<QUrl> urls = event->mimeData()->urls();
  if (urls.isEmpty())
      return;

  QString filename = urls.first().toLocalFile();
  if (filename.isEmpty())
      return;
  
  event->acceptProposedAction();
  emit fileDropped(filename);
};

AbstractView* MultiViewWidget::createView(AbstractView* fromAnother)
{
  CompositeView* sv = static_cast<CompositeView*>(this->AbstractMultiView::createView(fromAnother));
  sv->copyOptions(static_cast<CompositeView*>(fromAnother));
  // Viz3D final settings
  Viz3DWidget* viz3D = static_cast<Viz3DWidget*>(sv->view(CompositeView::Viz3D));
  connect(viz3D, SIGNAL(pickedMarkerChanged(int)), this, SIGNAL(pickedMarkerChanged(int)));
  connect(viz3D, SIGNAL(pickedMarkerToggled(int)), this, SIGNAL(pickedMarkerToggled(int)));
  connect(viz3D, SIGNAL(selectedMarkersToggled(QList<int>)), this, SIGNAL(selectedMarkersToggled(QList<int>)));
  connect(viz3D, SIGNAL(trajectoryMarkerToggled(int)), this, SIGNAL(trajectoryMarkerToggled(int)));
  viz3D->addActions(this->m_View3dActions);
  viz3D->setContextMenuPolicy(Qt::ActionsContextMenu);
  // Chart final settings
  ChartWidget* chart2D = static_cast<ChartWidget*>(sv->view(CompositeView::Chart));
  chart2D->addActions(this->m_ViewChartActions);
  connect(chart2D, SIGNAL(pausePlaybackRequested(bool)), this, SIGNAL(pausePlaybackRequested(bool)));
  connect(chart2D, SIGNAL(exportToImageRequested(btk::VTKChartTimeSeries*)), this, SLOT(exportChartToImage(btk::VTKChartTimeSeries*)));
  // Event filter
  if (this->mp_EventFilterObject)
  {
    for (int i = 0 ; i < sv->viewStack->count() ; ++i)
      sv->viewStack->widget(i)->installEventFilter(this->mp_EventFilterObject);
  }
  return sv;
};

void MultiViewWidget::changeGroundOrientation()
{
  btk::VTKGroundSource* ground = btk::VTKGroundSource::SafeDownCast((*this->mp_VTKProc)[VTK_GROUND]);
  if (this->mp_ActionGroundOrientationAutomatic->isChecked())
    ground->SetOrientation(btk::VTKGroundSource::Automatic);
  else if (this->mp_ActionGroundOrientationPlaneXY->isChecked())
    ground->SetOrientation(btk::VTKGroundSource::PlaneXY);
  else if (this->mp_ActionGroundOrientationPlaneYZ->isChecked())
    ground->SetOrientation(btk::VTKGroundSource::PlaneYZ);
  else if (this->mp_ActionGroundOrientationPlaneZX->isChecked())
    ground->SetOrientation(btk::VTKGroundSource::PlaneZX);
    
  this->updateCameras();
  this->updateViews();
};

void MultiViewWidget::changeMarkerTrajectoryLength()
{
  btk::VTKMarkersFramesSource* markers = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
  if (this->mp_ActionMarkerTrajectoryFull->isChecked())
    markers->SetTrajectoryLength(-1);
  else if (this->mp_ActionMarkerTrajectory25->isChecked())
    markers->SetTrajectoryLength(25);
  else if (this->mp_ActionMarkerTrajectory50->isChecked())
    markers->SetTrajectoryLength(50);
  else if (this->mp_ActionMarkerTrajectory100->isChecked())
    markers->SetTrajectoryLength(100);
  else if (this->mp_ActionMarkerTrajectory200->isChecked())
    markers->SetTrajectoryLength(200);
    
  this->updateMarkersDisplay();
};

void MultiViewWidget::changeForceButterflyActivation()
{
  btk::VTKGRFsFramesSource* GRFs = btk::VTKGRFsFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_GRFS]);
  GRFs->SetButterflyActivation(!GRFs->GetButterflyActivation());
  this->updateDisplay();
};

void MultiViewWidget::updateAnalogValuesModification(const QVector<int>& ids)
{
  Q_UNUSED(ids);
  
  QVector<bool> paths(this->mp_Acquisition->btkGroundReactionWrenches()->GetItemNumber());
  btk::VTKGRFsFramesSource* GRFs = btk::VTKGRFsFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_GRFS]);
  for (int i = 0 ; i < paths.count() ; ++i )
    paths[i] = GRFs->GetPathVisibility(i);
  GRFs->Update();
  for (int i = 0 ; i < paths.count() ; ++i )
    GRFs->SetPathVisibility(i, paths[i]);
  btk::VTKGRFsFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_GRFS])->GetOutput()->GetInformation()->Remove(vtkDataObject::DATA_TIME_STEPS());
  
  for (QList<AbstractView*>::const_iterator it = this->m_Views.begin() ; it != this->m_Views.end() ; ++it)
  {
    static_cast<ChartWidget*>(static_cast<CompositeView*>(*it)->view(CompositeView::Chart))->refreshPlots();
    static_cast<CompositeView*>(*it)->render();
  }
};

void MultiViewWidget::addChartCycleSettingAction()
{
  int index = -1;
  if ((this->mp_ChartCycleSettingsManager != 0) && ((index = this->mp_ChartCycleSettingsManager->count()-1) >= 0))
  {
    this->mp_ActionCycleSettings[index]->setText(this->mp_ChartCycleSettingsManager->setting(index).name);
    this->mp_ActionCycleSettings[index]->setVisible(true);
  }
};

void MultiViewWidget::updateChartCycleSettingActions()
{
  this->updateChartCycleSettingActions(0);
};

void MultiViewWidget::updateChartCycleSettingActions(int startIndex)
{
  int numSettings = 0;
  if (this->mp_ChartCycleSettingsManager != 0)
  {
    numSettings = this->mp_ChartCycleSettingsManager->count();
    for (int i = startIndex ; i < numSettings ; ++i)
    {
      this->mp_ActionCycleSettings[i]->setText(this->mp_ChartCycleSettingsManager->setting(i).name);
      this->mp_ActionCycleSettings[i]->setVisible(true);
      this->mp_ActionCycleSettings[i]->setChecked(false);
    }
  }
  for (int i = numSettings ; i < ChartCycleSettingsManager::maxCycleSettings ; ++i)
  {
    this->mp_ActionCycleSettings[i]->setText("");
    this->mp_ActionCycleSettings[i]->setVisible(false);
    this->mp_ActionCycleSettings[i]->setChecked(false);
  }
};

void MultiViewWidget::updateChartCycleSettingAction(int index)
{
  if ((this->mp_ChartCycleSettingsManager != 0) && (index < this->mp_ChartCycleSettingsManager->count()))
  {
    this->mp_ActionCycleSettings[index]->setText(this->mp_ChartCycleSettingsManager->setting(index).name);
    this->mp_ActionCycleSettings[index]->setVisible(true);
    if (this->mp_ChartCycleSettingsManager->currentSetting() == index)
      this->displayChartHorizontalAxisAsCycle(index);
  }
};

void MultiViewWidget::removeChartCycleSettingAction(int index)
{
  this->updateChartCycleSettingActions(index);
};

void MultiViewWidget::updateCurrentChartCycleSettingAction(int index)
{
  if (index >= 0)
    this->mp_ActionCycleSettings[index]->trigger();
  else
    this->mp_ActionChartAxisFrame->trigger();
};

void MultiViewWidget::updateCameras()
{
  btk::VTKGroundSource* ground = btk::VTKGroundSource::SafeDownCast((*this->mp_VTKProc)[VTK_GROUND]);
  ground->Update();
  double n[3]; ground->GetNormal(n);
  for (QList<AbstractView*>::iterator it = this->m_Views.begin() ; it != this->m_Views.end() ; ++it)
  {
    Viz3DWidget* viz3d = static_cast<Viz3DWidget*>(static_cast<CompositeView*>(*it)->view(CompositeView::Viz3D));
    static_cast<btk::VTKInteractorStyleTrackballFixedUpCamera*>(viz3d->GetRenderWindow()->GetInteractor()->GetInteractorStyle())->SetGlobalUp(n);
  }
};

void MultiViewWidget::updateViews()
{
  btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS])->Update(); // To update the markers' list when only charts are displayed.
  for (QList<AbstractView*>::const_iterator it = this->m_Views.begin() ; it != this->m_Views.end() ; ++it)
    static_cast<CompositeView*>(*it)->render();
};
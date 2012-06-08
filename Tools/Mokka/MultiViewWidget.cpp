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

#include "MultiViewWidget.h"
#include "Acquisition.h"
#include "CompositeView.h"
#include "ChartWidget.h"
#include "LoggerVTKOutput.h"
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
#include <vtkRenderWindow.h>
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
#include <vtkCamera.h>

enum {VTK_GROUND, VTK_FORCE_PLATFORMS, VTK_MARKERS, VTK_GRFS, VTK_GRFS_VECTOR, VTK_GRFS_PWA, VTK_SEGMENTS};

class vtkStreamingDemandDrivenPipelineCollection : public vtkstd::list<vtkStreamingDemandDrivenPipeline*>
{};

class vtkProcessMap : public vtkstd::map<int, vtkObjectBase*>
{};

class vtkActorMap : public vtkstd::map<int, vtkActor*>
{};

MultiViewWidget::MultiViewWidget(QWidget* parent)
: AbstractMultiView(parent), m_VideoDelays(),
  m_ForcePlatformColor(127, 127, 127), m_ForceVectorColor(255, 255, 0),
  m_View3dActions(), m_ViewChartActions()
{
  this->mp_EventFilterObject = 0;
  this->mp_Acquisition = 0;
  this->mp_Model = 0;
  this->mp_EventQtSlotConnections = vtkEventQtSlotConnect::New();
  this->mp_VTKProc = new vtkProcessMap();
  this->mp_VTKActor = new vtkActorMap();
  this->mp_Syncro = new vtkStreamingDemandDrivenPipelineCollection();
  this->mp_PointChartFrames = vtkDoubleArray::New();
  this->mp_PointChartFrames->SetName("BottomAxis");
  this->mp_AnalogChartFrames = vtkDoubleArray::New();
  this->mp_AnalogChartFrames->SetName("BottomAxis");
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
  
  this->mp_ChartBottomAxisDisplayMenu = new QMenu(tr("Chart Unit: X Axis"),this);
  this->mp_ActionChartAxisFrame = new QAction(tr("Frame"),this);
  this->mp_ActionChartAxisFrame->setCheckable(true);
  this->mp_ActionChartAxisTime = new QAction(tr("Time"),this);
  this->mp_ActionChartAxisTime->setCheckable(true);
  QActionGroup* chartBottomAxisDisplay = new QActionGroup(this);
  chartBottomAxisDisplay->addAction(this->mp_ActionChartAxisFrame);
  chartBottomAxisDisplay->addAction(this->mp_ActionChartAxisTime);
  this->mp_ActionChartAxisFrame->setChecked(true);
  this->mp_ChartBottomAxisDisplayMenu->addAction(this->mp_ActionChartAxisFrame);
  this->mp_ChartBottomAxisDisplayMenu->addAction(this->mp_ActionChartAxisTime);
  
  connect(groundOrientationActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(changeGroundOrientation()));
  connect(trajectoryLengthActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(changeMarkerTrajectoryLength()));
  connect(this->mp_ForceButterflyActivationAction, SIGNAL(triggered(bool)), this, SLOT(changeForceButterflyActivation()));
  connect(chartBottomAxisDisplay, SIGNAL(triggered(QAction*)), this, SLOT(updateChartUnitAxisX()));
  
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
  this->mp_PointChartFrames->Delete();
  this->mp_AnalogChartFrames->Delete();
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
  mapper->SetInput(ground->GetOutput());
  vtkProperty* prop = vtkProperty::New();
  prop->SetRepresentation(VTK_WIREFRAME);
  prop->SetColor(0.8, 0.8, 0.8);
  prop->SetAmbient(0.5);
  prop->SetDiffuse(0.0);
  prop->SetSpecular(0.0);
  vtkActor* actor = vtkActor::New();
  actor->SetMapper(mapper);
  actor->SetProperty(prop);
  actor->PickableOff();
  renderer->AddViewProp(actor);
  (*this->mp_VTKActor)[VTK_GROUND] = actor;
  // Cleanup for ground
  mapper->Delete();
  prop->Delete();
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
  actor->UseBoundsOff();
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
  chart->setPointFrameArray(this->mp_PointChartFrames);
  chart->setAnalogFrameArray(this->mp_AnalogChartFrames);
  
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
  if (markerUnit.compare("m") == 0)
    scale = 1000.0;
  
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
  this->mp_PointChartFrames->Initialize(); // Reset
  this->mp_PointChartFrames->SetNumberOfValues(this->mp_Acquisition->pointFrameNumber());
  this->mp_AnalogChartFrames->Initialize(); // Reset
  this->mp_AnalogChartFrames->SetNumberOfValues(this->mp_Acquisition->analogFrameNumber());
  double sub = 1.0 / (double)this->mp_Acquisition->analogSamplePerPointFrame();
  for (int i = 0 ; i < this->mp_Acquisition->pointFrameNumber() ; ++i)
  {
    double val = static_cast<double>(this->mp_Acquisition->firstFrame() + i);
    // Point
    this->mp_PointChartFrames->SetValue(i, val);
    // Analog
    int inc = i * this->mp_Acquisition->analogSamplePerPointFrame();
    this->mp_AnalogChartFrames->SetValue(inc, val);
    for (int j = 1 ; j < this->mp_Acquisition->analogSamplePerPointFrame() ; ++j)
      this->mp_AnalogChartFrames->SetValue(inc + j, val + j * sub);
  }
  this->updateChartUnitAxisX();
  
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

void MultiViewWidget::updateFramesIndex(int ff)
{
  Q_UNUSED(ff)
  // Chart
  double sub = 1.0 / (double)this->mp_Acquisition->analogSamplePerPointFrame();
  for (int i = 0 ; i < this->mp_Acquisition->pointFrameNumber() ; ++i)
  {
    // Point
    this->mp_PointChartFrames->SetValue(i, this->mp_Acquisition->firstFrame() + i);
    // Analog
    int inc = i * this->mp_Acquisition->analogSamplePerPointFrame();
    double val = static_cast<double>(this->mp_Acquisition->firstFrame() + i);
    this->mp_AnalogChartFrames->SetValue(inc, val);
    for (int j = 1 ; j < this->mp_Acquisition->analogSamplePerPointFrame() ; ++j)
      this->mp_AnalogChartFrames->SetValue(inc + j, val + j * sub);
  }
  // Video
  for (QMap<int, Video*>::const_iterator it = this->mp_Acquisition->videos().begin() ; it != this->mp_Acquisition->videos().end() ; ++it)
    this->m_VideoDelays[it.key()] = it.value()->delay;
  // Display
  for (QList<AbstractView*>::const_iterator it = this->m_Views.begin() ; it != this->m_Views.end() ; ++it)
  {
    static_cast<ChartWidget*>(static_cast<CompositeView*>(*it)->view(CompositeView::Chart))->updateAxisX();
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
      segmentsFramesSource->AppendDefinition((*it)->mesh, (*it)->surfaceVisible);
    else
    {
      segmentsFramesSource->SetDefinition(ids[inc], (*it)->mesh);
      segmentsFramesSource->SetSegmentVisibility(ids[inc], (*it)->visible ? 1 : 0);
      segmentsFramesSource->SetSegmentSurfaceVisibility(ids[inc], (*it)->surfaceVisible ? 1 : 0);
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
    segmentsFramesSource->SetSegmentVisibility(ids[i], visibles[i] ? 1 : 0);
  this->updateSegmentsDisplay();
};

void MultiViewWidget::setSegmentsSurfaceVisibility(const QVector<int>& ids, const QVector<bool>& visibles)
{
  btk::VTKSegmentsFramesSource* segmentsFramesSource = btk::VTKSegmentsFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_SEGMENTS]);
  for (int i = 0 ; i < ids.count() ; ++i)
    segmentsFramesSource->SetSegmentSurfaceVisibility(ids[i], visibles[i] ? 1 : 0);
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
    markersFramesSource->SetMarkerVisibility(ids[i], visibles[i] ? 1 : 0);
  this->updateMarkersDisplay();
};

void MultiViewWidget::setMarkersTrajectoryVisibility(const QVector<int>& ids, const QVector<bool>& visibles)
{
  btk::VTKMarkersFramesSource* markersFramesSource = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
  for (int i = 0 ; i < ids.count() ; ++i)
    markersFramesSource->SetTrajectoryVisibility(ids[i], visibles[i] ? 1 : 0);
  this->updateMarkersDisplay();
};

void MultiViewWidget::setMarkersConfiguration(const QList<int>& ids, const QList<bool>& visibles, const QList<bool>& trajectories, const QList<double>& radii, const QList<QColor>& colors)
{
  btk::VTKMarkersFramesSource* markers = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
  for (int i = 0 ; i < ids.count() ; ++i)
  {
    markers->SetMarkerVisibility(ids[i], visibles[i] ? 1 : 0);
    markers->SetTrajectoryVisibility(ids[i], trajectories[i] ? 1 : 0);
    markers->SetMarkerRadius(ids[i], radii[i]);
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

void MultiViewWidget::setDefaultGridColor(const QColor& color)
{
  (*this->mp_VTKActor)[VTK_GROUND]->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
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
  // Stop the video playback
  for (QList<AbstractView*>::iterator it = this->m_Views.begin() ; it != this->m_Views.end() ; ++it)
    static_cast<VideoWidget*>(static_cast<CompositeView*>(*it)->view(CompositeView::MediaVideo))->stop();
};

void MultiViewWidget::updateChartUnitAxisX()
{
  if (this->mp_ActionChartAxisFrame->isChecked())
    this->displayChartBottomAxisAsFrame();
  else if (this->mp_ActionChartAxisTime->isChecked())
    this->displayChartBottomAxisAsTime();
};

void MultiViewWidget::setFrameAsChartUnitAxisX()
{
  this->mp_ActionChartAxisFrame->trigger();
};

void MultiViewWidget::setTimeAsChartUnitAxisX()
{
  this->mp_ActionChartAxisTime->trigger();
};

void MultiViewWidget::displayChartBottomAxisAsFrame()
{
  for (QList<AbstractView*>::iterator it = this->m_Views.begin() ; it != this->m_Views.end() ; ++it)
  {
    ChartWidget* w = static_cast<ChartWidget*>(static_cast<CompositeView*>(*it)->view(CompositeView::Chart));
    w->setUnitAxisX("Frame", 1.0, 0.0);
  }
}

void MultiViewWidget::displayChartBottomAxisAsTime()
{
  double f = this->mp_Acquisition->btkAcquisition() ? this->mp_Acquisition->pointFrequency() : 1.0;
  for (QList<AbstractView*>::iterator it = this->m_Views.begin() ; it != this->m_Views.end() ; ++it)
  {
    ChartWidget* w = static_cast<ChartWidget*>(static_cast<CompositeView*>(*it)->view(CompositeView::Chart));
    w->setUnitAxisX("Time (s)", 1.0 / f, -1.0);
  }
}

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
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
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "MultiViewWidget.h"
#include "Acquisition.h"
#include "CompositeView.h"
#include "Viz3DWidget.h"
#include "UserRoles.h"

#include <btkVTKForcePlatformsSource.h>
#include <btkVTKGRFsFramesSource.h>
#include <btkVTKCommandEvents.h>
#include <btkVTKMarkersFramesSource.h>

#include <QDragEnterEvent>
#include <QUrl>
#include <QTableWidgetItem>

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

enum {VTK_GROUND, VTK_FORCE_PLATFORMS, VTK_MARKERS, VTK_GRFS};

class vtkStreamingDemandDrivenPipelineCollection : public vtkstd::list<vtkStreamingDemandDrivenPipeline*>
{};

class vtkProcessMap : public vtkstd::map<int, vtkObjectBase*>
{};


MultiViewWidget::MultiViewWidget(QWidget* parent)
: AbstractMultiView(parent)
{
  this->mp_Acquisition = 0;
  this->mp_EventQtSlotConnections = vtkEventQtSlotConnect::New();
  this->mp_VTKProc = new vtkProcessMap();
  this->mp_Syncro = new vtkStreamingDemandDrivenPipelineCollection();
  this->mp_Mappers = vtkMapperCollection::New();
  this->mp_GroupOrientationMenu = new QMenu(tr("Ground Orientation"),this);
  this->mp_ActionGroundOrientationPlaneXY = new QAction(tr("Plane XY"),this);
  this->mp_ActionGroundOrientationPlaneXY->setCheckable(true);
  this->mp_ActionGroundOrientationPlaneYZ = new QAction(tr("Plane YZ"),this);
  this->mp_ActionGroundOrientationPlaneYZ->setCheckable(true);
  this->mp_ActionGroundOrientationPlaneZX = new QAction(tr("Plane ZX"),this);
  this->mp_ActionGroundOrientationPlaneZX->setCheckable(true);
  this->mp_ActionGroundOrientationAutomatic = new QAction(tr("Automatic"),this);
  this->mp_ActionGroundOrientationAutomatic->setCheckable(true);
  
  QActionGroup* groundOrientationActionGroup = new QActionGroup(this);
  //groundOrientationActionGroup->addAction(this->actionGroundOrientationAutomatic);
  groundOrientationActionGroup->addAction(this->mp_ActionGroundOrientationPlaneXY);
  groundOrientationActionGroup->addAction(this->mp_ActionGroundOrientationPlaneYZ);
  groundOrientationActionGroup->addAction(this->mp_ActionGroundOrientationPlaneZX);
  this->mp_ActionGroundOrientationPlaneXY->setChecked(true);
  this->mp_GroupOrientationMenu->addAction(this->mp_ActionGroundOrientationPlaneXY);
  this->mp_GroupOrientationMenu->addAction(this->mp_ActionGroundOrientationPlaneYZ);
  this->mp_GroupOrientationMenu->addAction(this->mp_ActionGroundOrientationPlaneZX);
  
  connect(groundOrientationActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(changeGroundOrientation()));
  
  this->setViewPrototype(new CompositeView);
};

MultiViewWidget::~MultiViewWidget()
{
  this->mp_EventQtSlotConnections->Delete();
   for (vtkProcessMap::iterator it = this->mp_VTKProc->begin() ; it != this->mp_VTKProc->end() ; ++it)
     it->second->Delete();
   delete this->mp_VTKProc;
   delete this->mp_Syncro;
   this->mp_Mappers->Delete();
   vtkAlgorithm::SetDefaultExecutivePrototype(0);
};

void MultiViewWidget::initialize()
{
  this->AbstractMultiView::initialize();
  
  CompositeView* sv = static_cast<CompositeView*>(this->gridLayout()->itemAtPosition(0,0)->widget());
  vtkRenderer* renderer = static_cast<Viz3DWidget*>(sv->stackedWidget->widget(CompositeView::Viz3D))->renderer();
  
  // // BTK PIPELINE
  // //btk::SpecializedPointsExtractor::Pointer markersExtractor = btk::SpecializedPointsExtractor::New();
  // btk::SeparateKnownVirtualMarkersFilter::Pointer virtualMarkersSeparator = btk::SeparateKnownVirtualMarkersFilter::New();
  // //virtualMarkersSeparator->SetInput(markersExtractor->GetOutput());
  // btk::ForcePlatformsExtractor::Pointer forcePlatformsExtractor = btk::ForcePlatformsExtractor::New();
  // btk::GroundReactionWrenchFilter::Pointer GRWsFilter = btk::GroundReactionWrenchFilter::New();
  // GRWsFilter->SetThresholdValue(5.0); // PWA are not computed from vertical forces lower than 5 newtons.
  // GRWsFilter->SetThresholdState(true);
  // GRWsFilter->SetInput(forcePlatformsExtractor->GetOutput());
  // btk::DownsampleFilter<btk::WrenchCollection>::Pointer GRWsDownsampler = btk::DownsampleFilter<btk::WrenchCollection>::New();
  // GRWsDownsampler->SetInput(GRWsFilter->GetOutput());
  // // Store BTK process to be reused later.
  // //this->m_BTKProc[BTK_MARKERS] = markersExtractor;
  // this->m_BTKProc[BTK_VIRTUALS_MARKERS] = virtualMarkersSeparator;
  // this->m_BTKProc[BTK_FORCE_PLATFORMS] = forcePlatformsExtractor;
  // this->m_BTKProc[BTK_GRWS] = GRWsFilter;
  // this->m_BTKProc[BTK_GRWS_DOWNSAMPLED] = GRWsDownsampler;
  
  // VTK PIPELINE
  // Static data
  // Simple ground grid
  vtkPlaneSource* ground = vtkPlaneSource::New();
  ground->SetXResolution(30);
  ground->SetYResolution(30);
  ground->SetOrigin(-15.0, -15.0, 0.0);
  ground->SetPoint1(15.0, -15.0, 0.0);
  ground->SetPoint2(-15.0, 15.0, 0.0);
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
  // Cleanup for ground
  mapper->Delete();
  actor->Delete();
  prop->Delete();
  // Camera
  renderer->GetActiveCamera()->Elevation(-60);
  renderer->ResetCamera();
  renderer->GetActiveCamera()->Zoom(1.6);
  
  // Dynamic data
  // Pipeline for force plaforms (plane)
  btk::VTKForcePlatformsSource* forcePlaforms = btk::VTKForcePlatformsSource::New();
  //forcePlaforms->SetInput(forcePlatformsExtractor->GetOutput());
  mapper = vtkPolyDataMapper::New();
  mapper->SetInputConnection(forcePlaforms->GetOutputPort(0));
  prop = vtkProperty::New();
  prop->SetColor(1.0, 1.0, 0.0);
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
  // Cleanup for force platforms (plane)
  mapper->Delete();
  actor->Delete();
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
  //markers->SetInput(0, virtualMarkersSeparator->GetOutput(0));
  //markers->SetInput(1, virtualMarkersSeparator->GetOutput(2));
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
  actor->SetScale(0.005);
  actor->PickableOff();
  renderer->AddActor(actor);
  mapper->Delete();
  actor->Delete();
  // Pipeline for GRFs
  btk::VTKGRFsFramesSource* GRFs = btk::VTKGRFsFramesSource::New();
  //GRFs->SetInput(GRWsDownsampler->GetOutput());
  mapper = vtkPolyDataMapper::New();
  mapper->SetInputConnection(GRFs->GetOutputPort());
  this->mp_Mappers->AddItem(mapper);
  prop = vtkProperty::New();
  prop->SetColor(1.0, 1.0, 0.0);
  prop->SetAmbient(0.5);
  prop->SetDiffuse(0.0);
  prop->SetSpecular(0.0);
  actor = vtkActor::New();
  actor->SetMapper(mapper);
  actor->SetScale(0.005);
  actor->SetProperty(prop);
  actor->PickableOff();
  renderer->AddActor(actor);
  // Cleanup for GRFs.
  mapper->Delete();
  actor->Delete();
  prop->Delete();
  // Synchro between dynamic data
  this->mp_Syncro->push_back(vtkStreamingDemandDrivenPipeline::SafeDownCast(markers->GetExecutive()));
  this->mp_Syncro->push_back(vtkStreamingDemandDrivenPipeline::SafeDownCast(GRFs->GetExecutive()));
  // Store VTK process to be reused later.
  (*this->mp_VTKProc)[VTK_GROUND] = ground;
  (*this->mp_VTKProc)[VTK_FORCE_PLATFORMS] = forcePlaforms;
  (*this->mp_VTKProc)[VTK_MARKERS] = markers;
  (*this->mp_VTKProc)[VTK_GRFS] = GRFs;

  // Links between VTK & Qt
  this->mp_EventQtSlotConnections->Connect(
      markers, 
      btk::VTKMarkersListUpdateEvent,
      this, 
      SLOT(updateDisplayedMarkersList(vtkObject*, unsigned long, void*, void*)));
};

void MultiViewWidget::setAcquisition(Acquisition* acq)
{
  if (this->mp_Acquisition)
    disconnect(this->mp_Acquisition, 0, this, 0);
  this->mp_Acquisition = acq;
  // BTK->VTK connection
  btk::VTKMarkersFramesSource* markers = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
  markers->SetInput(0, this->mp_Acquisition->btkMarkers());
  markers->SetInput(1, this->mp_Acquisition->btkVirtualMarkers());
  btk::VTKGRFsFramesSource* GRFs = btk::VTKGRFsFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_GRFS]);
  GRFs->SetInput(this->mp_Acquisition->btkGroundReactionWrenches());
  btk::VTKForcePlatformsSource* forcePlaforms = btk::VTKForcePlatformsSource::SafeDownCast((*this->mp_VTKProc)[VTK_FORCE_PLATFORMS]);
  forcePlaforms->SetInput(this->mp_Acquisition->btkForcePlatforms());
  // Object connection
  connect(this->mp_Acquisition, SIGNAL(markersRadiusChanged(QVector<int>, QVector<double>)), this, SLOT(setMarkersRadius(QVector<int>, QVector<double>)));
  connect(this->mp_Acquisition, SIGNAL(markersColorChanged(QVector<int>, QVector<QColor>)), this, SLOT(setMarkersColor(QVector<int>, QVector<QColor>)));
}

void MultiViewWidget::load()
{
  if (!this->mp_Acquisition)
    return;
  
  /*
  //btk::SpecializedPointsExtractor::Pointer markersExtractor = static_pointer_cast<btk::SpecializedPointsExtractor>(this->m_BTKProc[BTK_MARKERS]);
  btk::SeparateKnownVirtualMarkersFilter::Pointer virtualMarkersSeparator = static_pointer_cast<btk::SeparateKnownVirtualMarkersFilter>(this->m_BTKProc[BTK_VIRTUALS_MARKERS]);
  btk::ForcePlatformsExtractor::Pointer forcePlatformsExtractor = static_pointer_cast<btk::ForcePlatformsExtractor>(this->m_BTKProc[BTK_FORCE_PLATFORMS]);
  btk::DownsampleFilter<btk::WrenchCollection>::Pointer GRWsDownsampler = static_pointer_cast< btk::DownsampleFilter<btk::WrenchCollection> >(this->m_BTKProc[BTK_GRWS_DOWNSAMPLED]);
  //markersExtractor->SetInput(acq);
  virtualMarkersSeparator->SetInput(acq->GetPoints());
  forcePlatformsExtractor->SetInput(acq);
  GRWsDownsampler->SetUpDownRatio(acq->GetNumberAnalogSamplePerFrame());
  */
  
  QString markerUnit = this->mp_Acquisition->pointUnit(Point::Marker);
  double scale = 1.0;
  if (markerUnit.compare("m") == 0)
    scale = 1000.0;
  
  btk::VTKGRFsFramesSource* GRFs = btk::VTKGRFsFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_GRFS]);
  btk::VTKMarkersFramesSource* markers = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
  btk::VTKForcePlatformsSource* forcePlaforms = btk::VTKForcePlatformsSource::SafeDownCast((*this->mp_VTKProc)[VTK_FORCE_PLATFORMS]);
  GRFs->SetScaleUnit(scale);
  markers->SetScaleUnit(scale);
  forcePlaforms->SetScaleUnit(scale);
  
  // Generate the lists for the graphs (Must be set after the update of the virtual makers separator filter)
  /*
  QFont f;
  QListWidgetItem* lwi;
  // List for the points
  QListWidget* lwPoint = new QListWidget;
  lwi = new QListWidgetItem("Deselect All");
  lwi->setFlags(lwi->flags() & ~Qt::ItemIsUserCheckable);
  lwPoint->addItem(lwi);
  lwi = new QListWidgetItem(QString());
  f = this->font();
  f.setPixelSize(5);
  lwi->setFont(f);
  lwi->setFlags(lwi->flags() & ~(Qt::ItemIsSelectable | Qt::ItemIsEnabled));
  lwPoint->addItem(lwi);
  // - Markers
  btk::PointCollection::Pointer m = virtualMarkersSeparator->GetOutput(0);
  if (!m->IsEmpty())
  {
    lwi = new QListWidgetItem(tr("Markers")); 
    lwi->setFlags(lwi->flags() & ~Qt::ItemIsSelectable);
    f = this->font();
    f.setBold(true);
    lwi->setFont(f);
    lwPoint->addItem(lwi);
    for (btk::PointCollection::ConstIterator it = m->Begin() ; it != m->End() ; ++it)
    {
      lwi = new QListWidgetItem(QString::fromStdString((*it)->GetLabel()));
      lwi->setCheckState(Qt::Unchecked);
      lwPoint->addItem(lwi);
    }
  }
  // - Virtual markers
  btk::PointCollection::Pointer vm = virtualMarkersSeparator->GetOutput(2);
  if (!vm->IsEmpty())
  {
    lwi = new QListWidgetItem(tr("Virtual Markers")); 
    lwi->setFlags(lwi->flags() & ~Qt::ItemIsSelectable);
    f = this->font();
    f.setBold(true);
    lwi->setFont(f);
    lwPoint->addItem(lwi);
    for (btk::PointCollection::ConstIterator it = vm->Begin() ; it != vm->End() ; ++it)
    {
      lwi = new QListWidgetItem(QString::fromStdString((*it)->GetLabel()));
      lwi->setCheckState(Qt::Unchecked);
      lwPoint->addItem(lwi);
    }
  }
  // - Sort others points
  QVector<QStringList> sop(5);
  for (btk::Acquisition::PointConstIterator it = acq->BeginPoint() ; it != acq->EndPoint() ; ++it)
  {
    if ((*it)->GetType() == btk::Point::Angle)
      sop[0].push_back(QString::fromStdString((*it)->GetLabel()));
    else if ((*it)->GetType() == btk::Point::Force)
      sop[1].push_back(QString::fromStdString((*it)->GetLabel()));
    else if ((*it)->GetType() == btk::Point::Moment)
      sop[2].push_back(QString::fromStdString((*it)->GetLabel()));
    else if ((*it)->GetType() == btk::Point::Power)
      sop[3].push_back(QString::fromStdString((*it)->GetLabel()));
    else if ((*it)->GetType() == btk::Point::Scalar)
      sop[4].push_back(QString::fromStdString((*it)->GetLabel()));
    //else
    //  btkErrorMacro("Unknown point's type. Cannot be sorted to display its values.");
  }
  // - Angles
  if (!sop[0].empty())
  {
    lwi = new QListWidgetItem(tr("Angles")); 
    lwi->setFlags(lwi->flags() & ~Qt::ItemIsSelectable);
    f = this->font();
    f.setBold(true);
    lwi->setFont(f);
    lwPoint->addItem(lwi);
    for (QStringList::const_iterator it = sop[0].begin() ; it != sop[0].end() ; ++it)
    {
      lwi = new QListWidgetItem(*it);
      lwi->setCheckState(Qt::Unchecked);
      lwPoint->addItem(lwi);
    }
  }
  // - Forces
  if (!sop[1].empty())
  {
    lwi = new QListWidgetItem(tr("Forces")); 
    lwi->setFlags(lwi->flags() & ~Qt::ItemIsSelectable);
    f = this->font();
    f.setBold(true);
    lwi->setFont(f);
    lwPoint->addItem(lwi);
    for (QStringList::const_iterator it = sop[1].begin() ; it != sop[1].end() ; ++it)
    {
      lwi = new QListWidgetItem(*it);
      lwi->setCheckState(Qt::Unchecked);
      lwPoint->addItem(lwi);
    }
  }
  // - Moments
  if (!sop[2].empty())
  {
    lwi = new QListWidgetItem(tr("Moments")); 
    lwi->setFlags(lwi->flags() & ~Qt::ItemIsSelectable);
    f = this->font();
    f.setBold(true);
    lwi->setFont(f);
    lwPoint->addItem(lwi);
    for (QStringList::const_iterator it = sop[2].begin() ; it != sop[2].end() ; ++it)
    {
      lwi = new QListWidgetItem(*it);
      lwi->setCheckState(Qt::Unchecked);
      lwPoint->addItem(lwi);
    }
  }
  // - Powers
  if (!sop[3].empty())
  {
    lwi = new QListWidgetItem(tr("Powers")); 
    lwi->setFlags(lwi->flags() & ~Qt::ItemIsSelectable);
    f = this->font();
    f.setBold(true);
    lwi->setFont(f);
    lwPoint->addItem(lwi);
    for (QStringList::const_iterator it = sop[3].begin() ; it != sop[3].end() ; ++it)
    {
      lwi = new QListWidgetItem(*it);
      lwi->setCheckState(Qt::Unchecked);
      lwPoint->addItem(lwi);
    }
  }
  // - Scalars
  if (!sop[4].empty())
  {
    lwi = new QListWidgetItem(tr("Scalars")); 
    lwi->setFlags(lwi->flags() & ~Qt::ItemIsSelectable);
    f = this->font();
    f.setBold(true);
    lwi->setFont(f);
    lwPoint->addItem(lwi);
    for (QStringList::const_iterator it = sop[4].begin() ; it != sop[4].end() ; ++it)
    {
      lwi = new QListWidgetItem(*it);
      lwi->setCheckState(Qt::Unchecked);
      lwPoint->addItem(lwi);
    }
  }
  // List for the analog channels
  QListWidget* lwAnalog = new QListWidget;
  lwi = new QListWidgetItem("Deselect All");
  lwi->setFlags(lwi->flags() & ~Qt::ItemIsUserCheckable);
  lwAnalog->addItem(lwi);
  lwi = new QListWidgetItem(QString());
  f = this->font();
  f.setPixelSize(5);
  lwi->setFont(f);
  lwi->setFlags(lwi->flags() & ~(Qt::ItemIsSelectable | Qt::ItemIsEnabled));
  lwAnalog->addItem(lwi);
  for (btk::Acquisition::AnalogConstIterator it = acq->BeginAnalog() ; it != acq->EndAnalog() ; ++it)
  {
    lwi = new QListWidgetItem(QString::fromStdString((*it)->GetLabel()));
    lwi->setCheckState(Qt::Unchecked);
    lwAnalog->addItem(lwi);
  }
  delete lwPoint;
  delete lwAnalog;
  */
  
  // Active the content of each view
  for (QList<AbstractView*>::const_iterator it = this->views().begin() ; it != this->views().end() ; ++it)
  {
    CompositeView* view = static_cast<CompositeView*>(*it);
    //view->setFunctionComboBoxOption(CompositeView::GraphPoint, lwPoint, 1);
    //view->setFunctionComboBoxOption(CompositeView::GraphAnalogChannel, lwAnalog, 1);
    view->show(true);
  }
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
  this->updateDisplay();
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
  this->updateDisplay();
};

void MultiViewWidget::updateHiddenMarkers(const QList<int>& ids)
{
  btk::VTKMarkersFramesSource* markersFramesSource = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
  markersFramesSource->ShowMarkers();
  for (int i = 0 ; i < ids.count() ; ++i)
    markersFramesSource->HideMarker(ids[i]);
  this->updateDisplay();
};

void MultiViewWidget::updateTailedMarkers(const QList<int>& ids)
{
  btk::VTKMarkersFramesSource* markersFramesSource = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
  markersFramesSource->HideTrajectories();
  for (int i = 0 ; i < ids.count() ; ++i)
    markersFramesSource->ShowTrajectory(ids[i]);
  this->updateDisplay();
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

void MultiViewWidget::setGroundOrientation(double x, double y, double z)
{
  vtkPlaneSource* ground = vtkPlaneSource::SafeDownCast((*this->mp_VTKProc)[VTK_GROUND]);
  ground->SetNormal(x,y,z);
  this->updateViews();
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

void MultiViewWidget::updateDisplayedMarkersList(vtkObject* /* caller */, unsigned long /* vtk_event */, void* /* client_data */, void* call_data)
{
  vtkIdTypeArray* indexes = static_cast<vtkIdTypeArray*>(call_data);
  if (!indexes)
    return;
    
  QVector<int> ids = QVector<int>(indexes->GetSize());
  for (int i = 0; i < ids.size() ; ++i)
    ids[i] = indexes->GetValue(i);
  emit visibleMarkersChanged(ids);
};

void MultiViewWidget::clear()
{
  for (QList<AbstractView*>::const_iterator it = this->views().begin() ; it != this->views().end() ; ++it)
    static_cast<CompositeView*>(*it)->show(false);
  this->updateDisplay(this->mp_Acquisition->firstFrame());
};

void MultiViewWidget::circleSelectedMarkers(QList<QTableWidgetItem*> items)
{
  btk::VTKMarkersFramesSource* markers = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
  markers->ClearSelectedMarkers();
  for (QList<QTableWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
  {
    if ((*it)->checkState() == Qt::Checked)
      markers->SetSelectedMarker((*it)->data(markerId).toInt());
  }
};

void MultiViewWidget::circleSelectedMarkers(const QList<int>& ids)
{
  btk::VTKMarkersFramesSource* markers = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
  markers->ClearSelectedMarkers();
  for (QList<int>::const_iterator it = ids.begin() ; it != ids.end() ; ++it)
    markers->SetSelectedMarker(*it);
  this->updateDisplay();
};

void MultiViewWidget::updateDisplay()
{
  // Force to update the markers even if the required frame was the last updated.
  btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS])->GetOutput()->GetInformation()->Remove(vtkDataObject::DATA_TIME_STEPS());
  // Update
  this->mp_Mappers->InitTraversal();
  vtkMapper* mapper = this->mp_Mappers->GetNextItem();
  while (mapper)
  {
    mapper->Modified();
    mapper = this->mp_Mappers->GetNextItem();
  }
  this->updateViews();
};

void MultiViewWidget::updateDisplay(int frame)
{
  // Force to update the markers even if the required frame was the last updated.
  btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS])->GetOutput()->GetInformation()->Remove(vtkDataObject::DATA_TIME_STEPS());
  // Update
  double t = static_cast<double>(frame - this->mp_Acquisition->firstFrame());
  for (vtkStreamingDemandDrivenPipelineCollection::iterator it = this->mp_Syncro->begin() ; it != this->mp_Syncro->end() ; ++it)
    (*it)->SetUpdateTimeStep(0, t);
  this->mp_Mappers->InitTraversal();
  vtkMapper* mapper = this->mp_Mappers->GetNextItem();
  while (mapper)
  {
    mapper->Modified();
    mapper = this->mp_Mappers->GetNextItem();
  }
  this->updateViews();
};

void MultiViewWidget::showSelectedMarkers(const QList<QTableWidgetItem*>& items)
{
  for (QList<QTableWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
  {
    (*it)->setCheckState(Qt::Checked);
    btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS])->ShowMarker((*it)->data(markerId).toInt());
  }
  this->updateDisplay();
};

void MultiViewWidget::hideSelectedMarkers(const QList<QTableWidgetItem*>& items)
{
  for (QList<QTableWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
  {
    (*it)->setCheckState(Qt::Unchecked);
    btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS])->HideMarker((*it)->data(markerId).toInt());
  }
  this->updateDisplay();
};

void MultiViewWidget::showAllMarkers()
{
  btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS])->ShowMarkers();
  this->updateDisplay();
};

void MultiViewWidget::hideAllMarkers()
{
  btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS])->HideMarkers();
  this->updateDisplay();
};

void MultiViewWidget::updateMarkerVisibility(QTableWidgetItem* item)
{
  btk::VTKMarkersFramesSource* markers = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
  // Hide/show markers
  if (item->checkState() == Qt::Checked)
    markers->ShowMarker(item->data(markerId).toInt());
  else if (item->checkState() == Qt::Unchecked)
    markers->HideMarker(item->data(markerId).toInt());
  // Hide/show trajectories
  if (item->data(markerTrajectoryActived).toBool())
    markers->ShowTrajectory(item->data(markerId).toInt());
  else
    markers->HideTrajectory(item->data(markerId).toInt());
  // Update current frame
  this->updateDisplay();
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
  connect(static_cast<Viz3DWidget*>(sv->stackedWidget->widget(CompositeView::Viz3D)), SIGNAL(pickedMarkerChanged(int)), this, SIGNAL(pickedMarkerChanged(int)));
  connect(static_cast<Viz3DWidget*>(sv->stackedWidget->widget(CompositeView::Viz3D)), SIGNAL(pickedMarkersChanged(int)), this, SIGNAL(pickedMarkersChanged(int)));
  static_cast<Viz3DWidget*>(sv->stackedWidget->widget(CompositeView::Viz3D))->installEventFilter(this->parent()->parent());
  return sv;
};

void MultiViewWidget::changeGroundOrientation()
{
  if (this->mp_ActionGroundOrientationPlaneXY->isChecked())
    this->setGroundOrientation(0.0, 0.0, 1.0);
  else if (this->mp_ActionGroundOrientationPlaneYZ->isChecked())
    this->setGroundOrientation(1.0, 0.0, 0.0);
  else if (this->mp_ActionGroundOrientationPlaneZX->isChecked())
    this->setGroundOrientation(0.0, 1.0, 0.0);
};

void MultiViewWidget::updateViews()
{
  for (QList<AbstractView*>::const_iterator it = this->views().begin() ; it != this->views().end() ; ++it)
    static_cast<CompositeView*>(*it)->render();
};
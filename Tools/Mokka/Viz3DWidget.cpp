#include "Viz3DWidget.h"
#include "UserRoles.h"

#include <btkDownsampleFilter.h>
#include <btkWrenchCollection.h>
#include <btkSpecializedPointsExtractor.h>
#include <btkForcePlatformsExtractor.h>
#include <btkGroundReactionWrenchFilter.h>
#include <btkVTKForcePlatformsSource.h>
#include <btkVTKGRFsFramesSource.h>
#include <btkVTKPickerInteractionCallback.h>
#include <btkVTKCommandEvents.h>
#include <btkVTKInteractorStyleTrackballCamera.h>
#include <btkVTKMarkersFramesSource.h>

#include <QDragEnterEvent>
#include <QUrl>

#include <vtkInformation.h>
#include <vtkIdList.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkCompositeDataPipeline.h>
#include <vtkCamera.h>
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

enum {BTK_MARKERS, BTK_VIRTUALS_MARKERS, BTK_FORCE_PLATFORMS, BTK_GRWS, BTK_GRWS_DOWNSAMPLED};
enum {VTK_GROUND, VTK_FORCE_PLATFORMS, VTK_MARKERS, VTK_GRFS};

class vtkStreamingDemandDrivenPipelineCollection : public vtkstd::list<vtkStreamingDemandDrivenPipeline*>
{};

class vtkProcessMap : public vtkstd::map<int, vtkObjectBase*>
{};

Viz3DWidget::Viz3DWidget(QWidget* parent)
: QVTKWidget(parent), m_BTKProc()
{
  // Member
  this->mp_Renderer = vtkRenderer::New();
  this->mp_AxesWidget = btk::VTKAxesWidget::New();
  this->mp_EventQtSlotConnections = vtkEventQtSlotConnect::New();
  this->mp_VTKProc = new vtkProcessMap();
  this->mp_Syncro = new vtkStreamingDemandDrivenPipelineCollection();
  this->mp_Mappers = vtkMapperCollection::New();
  this->m_FirstFrame = 1;
}

Viz3DWidget::~Viz3DWidget()
{
  this->mp_Renderer->Delete();
  this->mp_AxesWidget->Delete();
  this->mp_EventQtSlotConnections->Delete();
  for (vtkProcessMap::iterator it = this->mp_VTKProc->begin() ; it != this->mp_VTKProc->end() ; ++it)
    it->second->Delete();
  //(*this->mp_VTKProc)[VTK_GROUND]->Delete();
  //(*this->mp_VTKProc)[VTK_FORCE_PLATFORMS]->Delete();
  //(*this->mp_VTKProc)[VTK_MARKERS]->Delete();
  //(*this->mp_VTKProc)[VTK_GRFS]->Delete();
  delete this->mp_VTKProc;
  delete this->mp_Syncro;
  this->mp_Mappers->Delete();
  vtkAlgorithm::SetDefaultExecutivePrototype(0);
};

void Viz3DWidget::initialize()
{
  // BTK PIPELINE
  btk::SpecializedPointsExtractor::Pointer markersExtractor = btk::SpecializedPointsExtractor::New();
  btk::SeparateKnownVirtualMarkersFilter::Pointer virtualMarkersSeparator = btk::SeparateKnownVirtualMarkersFilter::New();
  virtualMarkersSeparator->SetInput(markersExtractor->GetOutput());
  btk::ForcePlatformsExtractor::Pointer forcePlatformsExtractor = btk::ForcePlatformsExtractor::New();
  btk::GroundReactionWrenchFilter::Pointer GRWsFilter = btk::GroundReactionWrenchFilter::New();
  GRWsFilter->SetThresholdValue(5.0); // PWA are not computed from vertical forces lower than 5 newtons.
  GRWsFilter->SetThresholdState(true);
  GRWsFilter->SetInput(forcePlatformsExtractor->GetOutput());
  btk::DownsampleFilter<btk::WrenchCollection>::Pointer GRWsDownsampler = btk::DownsampleFilter<btk::WrenchCollection>::New();
  GRWsDownsampler->SetInput(GRWsFilter->GetOutput());
  // Store BTK process to be reused later.
  this->m_BTKProc[BTK_MARKERS] = markersExtractor;
  this->m_BTKProc[BTK_VIRTUALS_MARKERS] = virtualMarkersSeparator;
  this->m_BTKProc[BTK_FORCE_PLATFORMS] = forcePlatformsExtractor;
  this->m_BTKProc[BTK_GRWS] = GRWsFilter;
  this->m_BTKProc[BTK_GRWS_DOWNSAMPLED] = GRWsDownsampler;
  
  // VTK UI
  //this->mp_Renderer->TwoSidedLightingOn();
  vtkRenderWindow* renwin = vtkRenderWindow::New();
  renwin->AddRenderer(this->mp_Renderer);
  this->SetRenderWindow(renwin);
  renwin->Delete();
  // VTK cell picker
  vtkCellPicker* cellPicker = vtkCellPicker::New();
  cellPicker->SetTolerance(0.001);
  this->GetRenderWindow()->GetInteractor()->SetPicker(cellPicker);
  cellPicker->Delete();
  // VTK interaction style
  btk::VTKInteractorStyleTrackballCamera* style = btk::VTKInteractorStyleTrackballCamera::New();
  vtkCallbackCommand* pickerMouseInteraction = vtkCallbackCommand::New();
  pickerMouseInteraction->SetClientData(style);
  pickerMouseInteraction->SetCallback(&btk::VTKPickerInteractionCallback);
  style->AddObserver(vtkCommand::LeftButtonPressEvent, pickerMouseInteraction);
  style->AddObserver(vtkCommand::MouseMoveEvent, pickerMouseInteraction);
  style->AddObserver(vtkCommand::LeftButtonReleaseEvent, pickerMouseInteraction);
  this->GetRenderWindow()->GetInteractor()->SetInteractorStyle(style);
  pickerMouseInteraction->Delete();
  style->Delete();
  
  // VTK WIDGET
  this->mp_AxesWidget->SetParentRenderer(this->mp_Renderer);
  this->mp_AxesWidget->SetInteractor(this->GetRenderWindow()->GetInteractor());
  this->mp_AxesWidget->SetViewport(0.0, 0.0, 0.15, 0.2);
  vtkAxesActor* axesActor = this->mp_AxesWidget->GetAxesActor();
  axesActor->SetShaftTypeToCylinder();
  axesActor->SetTotalLength( 1.25, 1.25, 1.25 );
  axesActor->SetCylinderRadius( 0.500 * axesActor->GetCylinderRadius() );
  axesActor->SetConeRadius( 1.025 * axesActor->GetConeRadius() );
  axesActor->SetSphereRadius( 1.500 * axesActor->GetSphereRadius() );
  this->mp_AxesWidget->SetEnabled(1);
  
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
  this->mp_Renderer->AddViewProp(actor);
  // Cleanup for ground
  mapper->Delete();
  actor->Delete();
  prop->Delete();
  // Pipeline for force plaforms
  btk::VTKForcePlatformsSource* forcePlaforms = btk::VTKForcePlatformsSource::New();
  forcePlaforms->SetInput(forcePlatformsExtractor->GetOutput());
  mapper = vtkPolyDataMapper::New();
  mapper->SetInputConnection(forcePlaforms->GetOutputPort());
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
  this->mp_Renderer->AddActor(actor);
  // Cleanup for force platforms.
  mapper->Delete();
  actor->Delete();
  prop->Delete();
  // Dynamic data
  // Require to play with VTK information's keys TIME_*
  vtkCompositeDataPipeline* prototype = vtkCompositeDataPipeline::New();
  vtkAlgorithm::SetDefaultExecutivePrototype(prototype);
  prototype->Delete();
  // Pipeline for markers
  btk::VTKMarkersFramesSource* markers = btk::VTKMarkersFramesSource::New();
  markers->SetInput(0, virtualMarkersSeparator->GetOutput(0));
  markers->SetInput(1, virtualMarkersSeparator->GetOutput(2));
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
  this->mp_Renderer->AddActor(actor);
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
  this->mp_Renderer->AddActor(actor);
  mapper->Delete();
  actor->Delete();
  // Pipeline for GRFs
  btk::VTKGRFsFramesSource* GRFs = btk::VTKGRFsFramesSource::New();
  GRFs->SetInput(GRWsDownsampler->GetOutput());
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
  this->mp_Renderer->AddActor(actor);
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
  
  // Camera
  this->mp_Renderer->GetActiveCamera()->Elevation(-60);
  this->mp_Renderer->ResetCamera();
  this->mp_Renderer->GetActiveCamera()->Zoom(1.6);
  
  // Links between VTK & Qt
  this->mp_EventQtSlotConnections->Connect(
      markers, 
      btk::VTKMarkersListUpdateEvent,
      this, 
      SLOT(updateDisplayedMarkersList(vtkObject*, unsigned long, void*, void*)));
  this->mp_EventQtSlotConnections->Connect(
      this->GetRenderWindow()->GetInteractor(), 
      btk::VTKMarkerPickedEvent,
      this, 
      SLOT(selectPickedMarker(vtkObject*, unsigned long, void*, void*)));
  this->mp_EventQtSlotConnections->Connect(
      this->GetRenderWindow()->GetInteractor(), 
      btk::VTKMarkersPickedEvent,
      this, 
      SLOT(selectPickedMarkers(vtkObject*, unsigned long, void*, void*)));
};

void Viz3DWidget::setMarkerRadius(int id, double r)
{
  btk::VTKMarkersFramesSource* markers = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
  markers->SetMarkerRadius(id, r);
};

double Viz3DWidget::markerRadius(int id)
{
  btk::VTKMarkersFramesSource* markers = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
  return markers->GetMarkerRadius(id);
};

void Viz3DWidget::setMarkerColorIndex(int id, int idx)
{
  btk::VTKMarkersFramesSource* markers = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
  markers->SetMarkerColorIndex(id, idx);
};

int Viz3DWidget::markerColorIndex(int id)
{
  btk::VTKMarkersFramesSource* markers = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
  return markers->GetMarkerColorIndex(id);
};

void Viz3DWidget::setMarkerVisibility(int id, bool visible)
{
  btk::VTKMarkersFramesSource* markers = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
  markers->SetMarkerVisibility(id, visible);
};

bool Viz3DWidget::markerVisibility(int id)
{
  btk::VTKMarkersFramesSource* markers = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
  return markers->GetMarkerVisibility(id);
};

double* Viz3DWidget::markerColorValue(int c)
{
  btk::VTKMarkersFramesSource* markers = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
  return markers->GetMarkerColorLUT()->GetTableValue(c);
}

void Viz3DWidget::setGroundOrientation(double x, double y, double z)
{
  vtkPlaneSource* ground = vtkPlaneSource::SafeDownCast((*this->mp_VTKProc)[VTK_GROUND]);
  ground->SetNormal(x,y,z);
  this->GetRenderWindow()->Render();
}

bool Viz3DWidget::appendNewMarkerColor(const QColor& color, int* idx)
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

btk::SeparateKnownVirtualMarkersFilter::Pointer Viz3DWidget::load(btk::Acquisition::Pointer acq)
{
  this->m_FirstFrame = acq->GetFirstFrame();
  
  btk::SpecializedPointsExtractor::Pointer markersExtractor = static_pointer_cast<btk::SpecializedPointsExtractor>(this->m_BTKProc[BTK_MARKERS]);
  btk::SeparateKnownVirtualMarkersFilter::Pointer virtualMarkersSeparator = static_pointer_cast<btk::SeparateKnownVirtualMarkersFilter>(this->m_BTKProc[BTK_VIRTUALS_MARKERS]);
  btk::ForcePlatformsExtractor::Pointer forcePlatformsExtractor = static_pointer_cast<btk::ForcePlatformsExtractor>(this->m_BTKProc[BTK_FORCE_PLATFORMS]);
  btk::DownsampleFilter<btk::WrenchCollection>::Pointer GRWsDownsampler = static_pointer_cast< btk::DownsampleFilter<btk::WrenchCollection> >(this->m_BTKProc[BTK_GRWS_DOWNSAMPLED]);
  markersExtractor->SetInput(acq);
  forcePlatformsExtractor->SetInput(acq);
  GRWsDownsampler->SetUpDownRatio(acq->GetNumberAnalogSamplePerFrame());
  
  // Update the VTK Pipeline
  vtkActorCollection* actors = this->mp_Renderer->GetActors();
  actors->InitTraversal();
  vtkActor* actor = actors->GetNextItem(); // Ground
  while (actor)
  {
    actor->VisibilityOn();
    actor = actors->GetNextItem();
  }
  std::string markerUnit = acq->GetPointUnit();
  double scale = 1.0;
  if (markerUnit.compare("m") == 0)
    scale = 1000.0;
    
  btk::VTKGRFsFramesSource* GRFs = btk::VTKGRFsFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_GRFS]);
  btk::VTKMarkersFramesSource* markers = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
  btk::VTKForcePlatformsSource* forcePlaforms = btk::VTKForcePlatformsSource::SafeDownCast((*this->mp_VTKProc)[VTK_FORCE_PLATFORMS]);
  GRFs->SetScaleUnit(scale);
  markers->SetScaleUnit(scale);
  forcePlaforms->SetScaleUnit(scale);
  
  virtualMarkersSeparator->Update();
  return virtualMarkersSeparator;
};

void Viz3DWidget::updateDisplayedMarkersList(vtkObject* caller, unsigned long /* vtk_event */, void* /* client_data */, void* call_data)
{
  vtkIdTypeArray* indexes = static_cast<vtkIdTypeArray*>(call_data);
  if (!indexes)
    return;
    
  QVector<int> ids = QVector<int>(indexes->GetSize());
  for (size_t i = 0; i < ids.size() ; ++i)
    ids[i] = indexes->GetValue(i);
  emit visibleMarkersChanged(ids);
};

void Viz3DWidget::selectPickedMarker(vtkObject* caller, unsigned long /* vtk_event */, void* /* client_data */, void* call_data)
{
  int id = *static_cast<int*>(call_data);
  emit pickedMarkerChanged(id);
};

void Viz3DWidget::selectPickedMarkers(vtkObject* caller, unsigned long /* vtk_event */, void* /* client_data */, void* call_data)
{
  int id = *static_cast<int*>(call_data);
  emit pickedMarkersChanged(id);
};

void Viz3DWidget::clear()
{
  vtkActorCollection* actors = this->mp_Renderer->GetActors();
  actors->InitTraversal();
  vtkActor* actor = actors->GetNextItem(); // Ground
  actor = actors->GetNextItem();
  while (actor)
  {
    actor->VisibilityOff();
    actor = actors->GetNextItem();
  }
  this->GetRenderWindow()->Render();
};

void Viz3DWidget::circleSelectedMarkers(QList<QTableWidgetItem*> items)
{
  btk::VTKMarkersFramesSource* markers = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
  markers->ClearSelectedMarkers();
  for (QList<QTableWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
  {
    if ((*it)->checkState() == Qt::Checked)
      markers->SetSelectedMarker((*it)->data(markerId).toInt());
  }
};

void Viz3DWidget::updateDisplay()
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
  this->GetRenderWindow()->Render();
};

void Viz3DWidget::updateDisplay(int frame)
{
  // Force to update the markers even if the required frame was the last updated.
  btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS])->GetOutput()->GetInformation()->Remove(vtkDataObject::DATA_TIME_STEPS());
  // Update
  double t = static_cast<double>(frame - this->m_FirstFrame);
  for (vtkStreamingDemandDrivenPipelineCollection::iterator it = this->mp_Syncro->begin() ; it != this->mp_Syncro->end() ; ++it)
    (*it)->SetUpdateTimeStep(0, t);
  this->mp_Mappers->InitTraversal();
  vtkMapper* mapper = this->mp_Mappers->GetNextItem();
  while (mapper)
  {
    mapper->Modified();
    mapper = this->mp_Mappers->GetNextItem();
  }
  this->GetRenderWindow()->Render();
};

void Viz3DWidget::showSelectedMarkers(const QList<QTableWidgetItem*>& items)
{
  for (QList<QTableWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
  {
    (*it)->setCheckState(Qt::Checked);
    btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS])->ShowMarker((*it)->data(markerId).toInt());
  }
  this->updateDisplay();
};

void Viz3DWidget::hideSelectedMarkers(const QList<QTableWidgetItem*>& items)
{
  for (QList<QTableWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
  {
    (*it)->setCheckState(Qt::Unchecked);
    btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS])->HideMarker((*it)->data(markerId).toInt());
  }
  this->updateDisplay();
};

void Viz3DWidget::showAllMarkers()
{
  btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS])->ShowMarkers();
  this->updateDisplay();
};

void Viz3DWidget::hideAllMarkers()
{
  btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS])->HideMarkers();
  this->updateDisplay();
};

void Viz3DWidget::updateMarkerVisibility(QTableWidgetItem* item)
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

void Viz3DWidget::dragEnterEvent(QDragEnterEvent *event)
{
  if (event->mimeData()->hasFormat("text/uri-list"))
    event->acceptProposedAction();
};

void Viz3DWidget::dropEvent(QDropEvent *event)
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
/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009, Arnaud Barré
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

#include "MainWindow.h"
#include "Metadata.h"
#include "About.h"

#include <btkAcquisitionFileReader.h>
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
#include <btkVTKAxesWidget.h>
#include <btkVTKMarkersFramesSource.h>

#include <QFileDialog>
#include <QFileInfo>
#include <QDateTime>
#include <QCloseEvent>
#include <QTableWidgetItem>
#include <QColorDialog>
#include <QMessageBox>
#include <QSettings>

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

enum {BTK_READER, BTK_MARKERS, BTK_FORCE_PLATFORMS, BTK_GRWS, BTK_GRWS_DOWNSAMPLED};
enum {VTK_GROUND, VTK_FORCE_PLATFORMS, VTK_MARKERS, VTK_GRFS};

static int markerId = Qt::UserRole + 1;
static int eventFrame = Qt::UserRole + 2;
static int validEvent = Qt::UserRole + 3;

class vtkStreamingDemandDrivenPipelineCollection : public vtkstd::list<vtkStreamingDemandDrivenPipeline*>
{};

class vtkProcessMap : public vtkstd::map<int, vtkObjectBase*>
{};

void MainWindow::closeEvent(QCloseEvent* event)
{
  if (this->isOkToContinue())
  {
    this->writeSettings();
    event->accept();
  }
  else
    event->ignore();
};

bool MainWindow::eventFilter(QObject* obj, QEvent* event)
{
  if ((event->type() == QEvent::KeyPress) && this->frameSlider->isEnabled())
  {
    QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
    // special case for 'markerRadiusSpinBox' widget
    if ((obj->objectName().compare("markerRadiusSpinBox") == 0) && (keyEvent->key() == Qt::Key_Escape))
    {
      this->markersTable->clearSelection();
      return true;
    }
    // general case
    if (keyEvent->matches(QKeySequence::MoveToPreviousChar))
    {
      this->displayPreviousFrame(1);
      return true;
    }
    else if (keyEvent->matches(QKeySequence::MoveToNextChar))
    {
      this->displayNextFrame(1);
      return true;
    }
    else if (keyEvent->key() == Qt::Key_Space)
    {
      this->toggleTimer();
      return true;
    }
    else if (keyEvent->key() == Qt::Key_Escape)
    {
      if (obj->objectName().compare("eventsTable") == 0)
        this->eventsTable->clearSelection();
      else
        this->markersTable->clearSelection();
      return true;
    }
    else
      return false;
  }
  return QMainWindow::eventFilter(obj, event);
};

MainWindow::MainWindow(QWidget* parent)
:QMainWindow(parent), m_BTKProc(), m_LastDirectory(".")
{
  // Members
  this->mp_Syncro = new vtkStreamingDemandDrivenPipelineCollection();
  this->mp_Timer = new QTimer(this);
  this->mp_Renderer = vtkRenderer::New();
  this->mp_Mappers = vtkMapperCollection::New();
  this->mp_EventQtSlotConnections = vtkEventQtSlotConnect::New();
  this->mp_VTKProc = new vtkProcessMap();
  this->m_FirstFrame = 1;
  this->mp_MetadataDlg = new Metadata(this);
  this->mp_AxesWidget = 0;
  this->m_PlaybackStep = 1;
  this->m_PlaybackDelay = 33; // 33 msec
  this->mp_PlayIcon = new QIcon(QString::fromUtf8(":/Resources/Images/player_play.png"));
  this->mp_PauseIcon = new QIcon(QString::fromUtf8(":/Resources/Images/player_pause.png"));
  this->mp_DownArrow = new QIcon(QString::fromUtf8(":/Resources/Images/disclosureTriangleSmallDownBlack.png"));
  this->mp_RightArrow = new QIcon(QString::fromUtf8(":/Resources/Images/disclosureTriangleSmallRightBlack.png"));

  // Finalize UI
  // Qt UI
  this->setupUi(this);
#ifdef Q_OS_MAC
  QFont f = informationsDock->font();
  f.setPointSize(10);
  this->informationsDock->setFont(f);
  this->markersDock->setFont(f);
  this->markerPropertiesButton->setFont(f);
  this->eventsDock->setFont(f);
  this->eventInformationsButton->setFont(f);
  this->eventsTable->setFont(f);
  f.setPointSize(11);
  this->showMarkersButton->setFont(f);
  this->hideMarkersButton->setFont(f);
#endif
  this->markersDock->setVisible(false);
  this->eventsDock->setVisible(false);
  this->informationsDock->setVisible(false);
  this->informationsDock->setFloating(true); // To not show a blinking rectangle at the startup
  this->action_FileOpen->setShortcut(QKeySequence::Open);
  this->actionClose->setShortcut(QKeySequence::Close);
  this->actionSave->setShortcut(QKeySequence::Save);
  this->actionSave_As->setShortcut(QKeySequence::SaveAs);
  this->actionCut->setShortcut(QKeySequence::Cut);
  this->actionCopy->setShortcut(QKeySequence::Copy);
  this->actionPaste->setShortcut(QKeySequence::Paste);
  this->actionSelect_All->setShortcut(QKeySequence::SelectAll);
  this->mp_PlaybackSpeedActionGroup = new QActionGroup(this);
  this->mp_PlaybackSpeedActionGroup->addAction(actionRealtime);
  this->mp_PlaybackSpeedActionGroup->addAction(action1_2);
  this->mp_PlaybackSpeedActionGroup->addAction(action1_5);
  this->mp_PlaybackSpeedActionGroup->addAction(action1_10);
  this->mp_PlaybackSpeedActionGroup->addAction(actionFull_Frames);
  this->mp_GroundOrientationActionGroup = new QActionGroup(this);
  this->mp_GroundOrientationActionGroup->addAction(actionPlane_XY);
  this->mp_GroundOrientationActionGroup->addAction(actionPlane_YZ);
  this->mp_GroundOrientationActionGroup->addAction(actionPlane_ZX);
  this->markerRadiusSpinBox->clear();
  this->eventTimeSpinBox->clear();
  for (int i = 0 ; i < maxRecentFiles ; ++i)
  {
      this->mp_ActionRecentFiles[i] = new QAction(this);
      this->mp_ActionRecentFiles[i]->setVisible(false);
      connect(this->mp_ActionRecentFiles[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
      this->menuOpen_Recent->addAction(this->mp_ActionRecentFiles[i]);
  }
  this->mp_ActionSeparatorRecentFiles = this->menuOpen_Recent->addSeparator();
  this->menuOpen_Recent->addAction(this->actionClear_Menu);
  connect(this->actionClear_Menu, SIGNAL(triggered()), this, SLOT(clearRecentFiles()));
  QAction* actionMarkersDockView = this->markersDock->toggleViewAction();
  QAction* actionEventsDockView = this->eventsDock->toggleViewAction();
  QAction* actionInformationsDockView = this->informationsDock->toggleViewAction();
  actionMarkersDockView->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_M));
  actionEventsDockView->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));
  actionInformationsDockView->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_I));
  actionMarkersDockView->setText(QObject::tr("Markers List"));
  actionEventsDockView->setText(QObject::tr("Events List"));
  this->menuView->addAction(actionInformationsDockView);
  this->menuView->addAction(actionMarkersDockView);
  this->menuView->addAction(actionEventsDockView);

  // Qt UI: Undo/Redo
  this->mp_UndoStack = new QUndoStack(this);
  //connect(this->mp_UndoStack, SIGNAL(indexChanged(int)), this, SLOT(setAcquisitionModifed(int)));
  QAction* actionUndo = this->mp_UndoStack->createUndoAction(this);
  actionUndo->setShortcut(QKeySequence::Undo);
  //actionUndo->setEnabled(false);
  QAction* actionRedo = this->mp_UndoStack->createRedoAction(this);
  actionRedo->setShortcut(QKeySequence::Redo);
  //actionRedo->setEnabled(false);
  this->menuEdit->insertAction(this->actionCut, actionUndo);
  this->menuEdit->insertAction(this->actionCut, actionRedo);
  this->menuEdit->insertSeparator(this->actionCut);
  // VTK UI
  //this->mp_Renderer->TwoSidedLightingOn();
  vtkRenderWindow* renwin = vtkRenderWindow::New();
  renwin->AddRenderer(this->mp_Renderer);
  this->qvtkWidget->SetRenderWindow(renwin);
  renwin->Delete();
  // VTK cell picker
  vtkCellPicker* cellPicker = vtkCellPicker::New();
  cellPicker->SetTolerance(0.001);
  this->qvtkWidget->GetRenderWindow()->GetInteractor()->SetPicker(cellPicker);
  cellPicker->Delete();
  // VTK interaction style
  btk::VTKInteractorStyleTrackballCamera* style = btk::VTKInteractorStyleTrackballCamera::New();
  vtkCallbackCommand* pickerMouseInteraction = vtkCallbackCommand::New();
  pickerMouseInteraction->SetClientData(style);
  pickerMouseInteraction->SetCallback(&btk::VTKPickerInteractionCallback);
  style->AddObserver(vtkCommand::LeftButtonPressEvent, pickerMouseInteraction);
  style->AddObserver(vtkCommand::MouseMoveEvent, pickerMouseInteraction);
  style->AddObserver(vtkCommand::LeftButtonReleaseEvent, pickerMouseInteraction);
  this->qvtkWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(style);
  pickerMouseInteraction->Delete();
  style->Delete();

  // Qt Signal/Slot connection
  connect(this->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
  connect(this->actionEdit_Metadata, SIGNAL(triggered()), this, SLOT(editMetadata()));
  connect(this->action_FileOpen, SIGNAL(triggered()), this, SLOT(openFile()));
  connect(this->actionClose, SIGNAL(triggered()), this, SLOT(closeFile()));
  connect(this->action_Quit, SIGNAL(triggered()), this, SLOT(close()));
  connect(this->mp_PlaybackSpeedActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(changePlaybackParameters()));
  connect(this->mp_GroundOrientationActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(changeGroundOrientation()));

  connect(this->frameSlider, SIGNAL(valueChanged(int)), this, SLOT(updateActiveEvent(int)));
  connect(this->frameSlider, SIGNAL(valueChanged(int)), this, SLOT(updateDisplay(int)));
  connect(this->playButton, SIGNAL(clicked()), this, SLOT(toggleTimer()));
  connect(this->mp_Timer, SIGNAL(timeout()), this, SLOT(displayNextFrame()));
  connect(this->markersTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(updateMarkerVisibility(QTableWidgetItem*)));
  connect(this->markersTable, SIGNAL(itemSelectionChanged()), this, SLOT(displayMarkerProperties()));
  connect(this->markersTable, SIGNAL(itemSelectionChanged()), this, SLOT(toggleMarkersVisibilityButtons()));
  connect(this->markersTable, SIGNAL(itemSelectionChanged()), this, SLOT(circleSelectedMarkers()));
  connect(this->markerRadiusSlider, SIGNAL(valueChanged(int)), this, SLOT(updateMarkerRadiusSpinBox(int)));
  connect(this->markerRadiusSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateMarkerRadius(double)));
  connect(this->markerColorButton, SIGNAL(clicked(bool)), this, SLOT(updateMarkerColor()));
  connect(this->showMarkersButton, SIGNAL(clicked()), this, SLOT(showAllMarkers()));
  connect(this->hideMarkersButton, SIGNAL(clicked()), this, SLOT(hideAllMarkers()));
  connect(this->eventsTable, SIGNAL(itemSelectionChanged()), this, SLOT(displayEventInformations()));
  connect(this->eventsTable, SIGNAL(itemSelectionChanged()), this, SLOT(updateEventsButtonsState()));
  connect(this->showEventButton, SIGNAL(clicked()), this, SLOT(showEvent()));
  connect(this->markersDock, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)), this, SLOT(markersDockLocationChanged(Qt::DockWidgetArea)));
  connect(this->eventsDock, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)), this, SLOT(eventsDockLocationChanged(Qt::DockWidgetArea)));
  connect(this->eventInformationsButton, SIGNAL(clicked()), this, SLOT(toggleEventInformations()));
  connect(this->markerPropertiesButton, SIGNAL(clicked()), this, SLOT(toggleMarkerProperties()));
  
  // Qt/VTK connections
  this->mp_EventQtSlotConnections->Connect(
      this->qvtkWidget->GetRenderWindow()->GetInteractor(), 
      btk::VTKMarkerPickedEvent,
      this, 
      SLOT(selectPickedMarker(vtkObject*, unsigned long, void*, void*)));
  this->mp_EventQtSlotConnections->Connect(
      this->qvtkWidget->GetRenderWindow()->GetInteractor(), 
      btk::VTKMarkersPickedEvent,
      this, 
      SLOT(selectPickedMarkers(vtkObject*, unsigned long, void*, void*)));

  // Event filter
  this->qvtkWidget->installEventFilter(this);
  this->frameSlider->installEventFilter(this);
  this->markersTable->installEventFilter(this);
  this->eventsTable->installEventFilter(this);
  this->markerRadiusSpinBox->installEventFilter(this);

  // Settings
  QCoreApplication::setOrganizationName("BTK");
  QCoreApplication::setOrganizationDomain("btk.org");
  QCoreApplication::setApplicationName("MainWindow");
  this->readSettings();
  this->setCurrentFile("");
};

MainWindow::~MainWindow()
{
  //std::cout << "Deleting" << std::endl;
  delete this->mp_Syncro;
  //this->mp_Markers->Delete();
  this->mp_AxesWidget->Delete();
  this->mp_EventQtSlotConnections->Delete();
  this->mp_Mappers->Delete();
  this->mp_Renderer->Delete();
  (*this->mp_VTKProc)[VTK_GROUND]->Delete();
  (*this->mp_VTKProc)[VTK_FORCE_PLATFORMS]->Delete();
  (*this->mp_VTKProc)[VTK_MARKERS]->Delete();
  (*this->mp_VTKProc)[VTK_GRFS]->Delete();
  delete this->mp_VTKProc;
  //this->mp_Ground->Delete();
  vtkAlgorithm::SetDefaultExecutivePrototype(0);
  delete this->mp_PlayIcon;
  delete this->mp_PauseIcon;
  delete this->mp_DownArrow;
  delete this->mp_RightArrow;
};

void MainWindow::initialize()
{
  // BTK PIPELINE
  btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
  reader->SetDisableFilenameExceptionState(true);
  btk::SpecializedPointsExtractor::Pointer markersExtractor = btk::SpecializedPointsExtractor::New();
  markersExtractor->SetInput(reader->GetOutput());
  btk::ForcePlatformsExtractor::Pointer forcePlatformsExtractor = btk::ForcePlatformsExtractor::New();
  forcePlatformsExtractor->SetInput(reader->GetOutput());
  btk::GroundReactionWrenchFilter::Pointer GRWsFilter = btk::GroundReactionWrenchFilter::New();
  GRWsFilter->SetThresholdValue(5.0); // PWA are not computed from vertical forces lower than 5 newtons.
  GRWsFilter->SetThresholdState(true);
  GRWsFilter->SetInput(forcePlatformsExtractor->GetOutput());
  btk::DownsampleFilter<btk::WrenchCollection>::Pointer GRWsDownsampler = btk::DownsampleFilter<btk::WrenchCollection>::New();
  GRWsDownsampler->SetInput(GRWsFilter->GetOutput());
  // Store BTK process to be reused later.
  this->m_BTKProc[BTK_READER] = reader;
  this->m_BTKProc[BTK_MARKERS] = markersExtractor;
  this->m_BTKProc[BTK_FORCE_PLATFORMS] = forcePlatformsExtractor;
  this->m_BTKProc[BTK_GRWS] = GRWsFilter;
  this->m_BTKProc[BTK_GRWS_DOWNSAMPLED] = GRWsDownsampler;

  // VTK WIDGET
  this->mp_AxesWidget = btk::VTKAxesWidget::New();
  this->mp_AxesWidget->SetParentRenderer(this->mp_Renderer);
  this->mp_AxesWidget->SetInteractor(this->qvtkWidget->GetRenderWindow()->GetInteractor());
  int* size = this->mp_Renderer->GetSize();
  double xr = 150.0 / static_cast<double>(size[0]);
  double yr = 150.0 / static_cast<double>(size[1]);
  this->mp_AxesWidget->SetViewport(0.0, 0.0, xr, yr);
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
  // First render
  this->mp_Renderer->GetActiveCamera()->Elevation(-60);
  this->mp_Renderer->ResetCamera(); 
  this->qvtkWidget->update();
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
  //forcePlaforms->Delete();
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
  markers->SetInput(markersExtractor->GetOutput());
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
  // Cleanup for markers
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
  //GRFs->Delete();
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
  
  vtkCamera* cam = this->mp_Renderer->GetActiveCamera();
  cam->Zoom(1.6);

  // Links between VTK & Qt
  this->mp_EventQtSlotConnections->Connect(
      markers, 
      btk::VTKMarkersListUpdateEvent,
      this, 
      SLOT(updateDisplayedMarkersList(vtkObject*, unsigned long, void*, void*)));
};

void MainWindow::about()
{ 
  About aboutDlg(this);
#ifdef Q_OS_MAC
  QFont f = aboutDlg.rights->font();
  f.setPointSize(11);
  aboutDlg.release->setFont(f);
  aboutDlg.copyright->setFont(f);
  aboutDlg.rights->setFont(f);
#endif
  aboutDlg.exec();
};

void MainWindow::editMetadata()
{
  this->mp_MetadataDlg->exec();
};

void MainWindow::setCurrentFile(const QString& filename) 
{ 
  this->setWindowModified(false); 
  QString shownName = trUtf8("No File"); 
  if (!filename.isEmpty())
  { 
    shownName = QFileInfo(filename).fileName(); 
    this->m_RecentFiles.removeAll(filename); 
    this->m_RecentFiles.prepend(filename); 
  } 
  this->updateRecentFileActions(); 
  this->setWindowTitle(trUtf8("%1[*] - %2").arg(shownName).arg(trUtf8("Mokka"))); 
};

void MainWindow::updateRecentFileActions()
{
  QMutableStringListIterator i(m_RecentFiles); 
  while (i.hasNext())
  {
    if (!QFile::exists(i.next())) 
      i.remove(); 
  }
  for (int i = 0 ; i < maxRecentFiles ; ++i)
  { 
    if (i < this->m_RecentFiles.count())
    {
      QString text = QFileInfo(this->m_RecentFiles[i]).fileName(); 
      this->mp_ActionRecentFiles[i]->setText(text); 
      this->mp_ActionRecentFiles[i]->setData(m_RecentFiles[i]); 
      this->mp_ActionRecentFiles[i]->setVisible(true); 
    } 
    else
      this->mp_ActionRecentFiles[i]->setVisible(false); 
  } 
  this->actionClear_Menu->setEnabled(!this->m_RecentFiles.isEmpty());
  this->mp_ActionSeparatorRecentFiles->setVisible(!this->m_RecentFiles.isEmpty());
};

void MainWindow::openRecentFile()
{
  if (this->isOkToContinue())
  { 
    QAction* pAction = qobject_cast<QAction*>(sender()); 
    if (pAction) 
      this->loadFile(pAction->data().toString()); 
  }
}

void MainWindow::clearRecentFiles()
{
  this->m_RecentFiles.clear();
  this->updateRecentFileActions();
};

bool MainWindow::isOkToContinue() 
{ 
  if (this->isWindowModified())
  {
    QMessageBox messageBox(QMessageBox::Question, 
                           trUtf8("MainWindow"),
                           trUtf8("The document has been modified.\nDo you want to save your changes?"), 
                           QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                           this, Qt::Sheet);
    messageBox.setDefaultButton(QMessageBox::Yes);
    messageBox.setEscapeButton(QMessageBox::Cancel);
    switch(messageBox.exec())
    {
        case QMessageBox::Yes:
            //this->saveFile();
            break;
        case QMessageBox::Cancel:
            return false;
            break;
    }
  } 
  return true; 
};

void MainWindow::openFile()
{
  QFileDialog open(this,
                   trUtf8("Open Acquisition"),
                   this->m_LastDirectory,
                   trUtf8("Acquisition Files (*.c3d *.trc)"));
  if (open.exec())
  {
    this->m_LastDirectory = open.directory().absolutePath();
    QStringList filenames = open.selectedFiles();
    if (!filenames.isEmpty())
      this->loadFile(filenames.first());
  }
};

void MainWindow::loadFile(const QString& filename)
{
  QApplication::setOverrideCursor(Qt::WaitCursor);
  this->closeFile();
  btk::AcquisitionFileReader::Pointer reader = static_pointer_cast<btk::AcquisitionFileReader>(this->m_BTKProc[BTK_READER]);
  if (reader->GetFilename().compare(filename.toStdString()))
    reader->SetAcquisitionIO();
  reader->SetFilename(filename.toStdString());
  QMessageBox error(QMessageBox::Critical, "File error", "Error occured during the file reading", QMessageBox::Ok , this);
  try
  {
    reader->Update();
  }
  catch (btk::Exception& e)
  {
    QApplication::restoreOverrideCursor();
    reader->ResetState();
    error.setInformativeText(e.what());
    error.exec();
    return;

  }
  catch (std::exception& e)
  {
    QApplication::restoreOverrideCursor();
    reader->ResetState();
    error.setInformativeText("Unexpected error: " + QString(e.what()));
    error.exec();
    return;
  }
  catch (...)
  {
    QApplication::restoreOverrideCursor();
    reader->ResetState();
    error.setInformativeText("Unknown error.");
    error.exec();
    return;
  }
  this->setCurrentFile(filename);
  btk::Acquisition::Pointer acq = reader->GetOutput();
  this->m_FirstFrame = acq->GetFirstFrame();
  this->changePlaybackParameters();
  // Informations Dock
  QFileInfo fI = QFileInfo(filename);
  this->fileNameValue->setText(fI.fileName());
  this->documentTypeValue->setText(fI.suffix().toUpper() + QString(" Document"));
  double sizeDouble = static_cast<double>(fI.size());
  QString sizeText;
  if (sizeDouble <= 1024.0)
    sizeText = QString::number(sizeDouble) + " bytes";
  else
  {
    sizeDouble /= 1024.0;
    if (sizeDouble <= 1024.0)
      sizeText = QString::number(sizeDouble) + " KB";
    else
    {
      sizeDouble /= 1024.0;
      sizeText = QString::number(sizeDouble) + " MB";
    }
  }
  this->fileSizeValue->setText(sizeText);
  this->creationDateValue->setText(fI.created().toString());
  this->modificationDateValue->setText(fI.lastModified().toString());
  switch(reader->GetAcquisitionIO()->GetFileType())
  {
    case btk::AcquisitionFileIO::TypeNotApplicable:
      this->fileFormatValue->setText("NA");
      break;
    case btk::AcquisitionFileIO::ASCII:
      this->fileFormatValue->setText("ASCII");
      break;
    case btk::AcquisitionFileIO::Binary:
      this->fileFormatValue->setText("Binary");
      break;
  }
  switch(reader->GetAcquisitionIO()->GetByteOrder())
  {
    case btk::AcquisitionFileIO::OrderNotApplicable:
      this->byteOrderValue->setText("NA");
      break;
    case btk::AcquisitionFileIO::IEEE_LittleEndian:
      this->byteOrderValue->setText("IEEE Little Endian");
      break;
    case btk::AcquisitionFileIO::VAX_LittleEndian:
      this->byteOrderValue->setText("VAX Little Endian");
      break;
    case btk::AcquisitionFileIO::IEEE_BigEndian:
      this->byteOrderValue->setText("IEEE Big Endian");
      break;
  }
  switch(reader->GetAcquisitionIO()->GetStorageFormat())
  {
    case btk::AcquisitionFileIO::StorageNotApplicable:
      this->storageFormatValue->setText("NA");
      break;
    case btk::AcquisitionFileIO::Float:
      this->storageFormatValue->setText("Float");
      break;
    case btk::AcquisitionFileIO::Integer:
      this->storageFormatValue->setText("Integer");
      break;
  }
  this->pointNumberValue->setText(QString::number(acq->GetPointNumber()));
  this->pointFrequencyValue->setText(QString::number(acq->GetPointFrequency()) + " Hz");
  this->analogNumberValue->setText(QString::number(acq->GetAnalogNumber()));
  this->analogFrequencyValue->setText(QString::number(acq->GetAnalogFrequency()) + " Hz");
  this->eventNumberValue->setText(QString::number(acq->GetEventNumber()));
  this->informationsDock->updateGeometry();
  // BTK pipeline settings
  static_pointer_cast< btk::DownsampleFilter<btk::WrenchCollection> >(this->m_BTKProc[BTK_GRWS_DOWNSAMPLED])->SetUpDownRatio(static_cast<int>(acq->GetAnalogFrequency() / (acq->GetPointFrequency())));
  // UI settings
  // Markers
  this->markersTable->blockSignals(true);
  btk::PointCollection::Pointer markers = static_pointer_cast<btk::SpecializedPointsExtractor>(this->m_BTKProc[BTK_MARKERS])->GetOutput();
  markers->Update();
  this->markersTable->setRowCount(markers->GetItemNumber());
  //QBrush defaultLabelColor = QBrush(QColor(Qt::gray));
  int row = 0;
  for (btk::PointCollection::ConstIterator it = markers->Begin() ; it != markers->End() ; ++it)
  {
    QTableWidgetItem* labelItem = new QTableWidgetItem();
    labelItem->setData(markerId, row);
    labelItem->setCheckState(Qt::Checked);
    labelItem->setText(QString::fromStdString((*it)->GetLabel()));
    //labelItem->setForeground(defaultLabelColor);
    this->markersTable->setItem(row, 0, labelItem);
    ++row;
  }
  this->markersTable->blockSignals(false);
  this->showMarkersButton->setEnabled(true);
  this->hideMarkersButton->setEnabled(true);
  //this->markersDock->setEnabled(true);
  // Events
  btk::EventCollection::Pointer events = acq->GetEvents();
  this->eventsTable->setColumnCount(4);
  this->eventsTable->setRowCount(events->GetItemNumber());
  this->eventsTable->setColumnHidden(3, true);
  row = 0;
  std::string previousSubject = "";
  double pointFrequency = acq->GetPointFrequency();
  for (btk::EventCollection::ConstIterator it = events->Begin() ; it != events->End() ; ++it)
  {
    QTableWidgetItem* timeItem = new QTableWidgetItem();
    QTableWidgetItem* contextItem = new QTableWidgetItem();
    QTableWidgetItem* labelItem = new QTableWidgetItem();
    QTableWidgetItem* subjectItem = new QTableWidgetItem();
    timeItem->setText(QString::number((*it)->GetTime()));
    int frameIndex = static_cast<int>((*it)->GetTime() * pointFrequency);
    timeItem->setData(eventFrame, frameIndex);
    labelItem->setText(QString::fromStdString((*it)->GetLabel()));
    contextItem->setText(QString::fromStdString((*it)->GetContext()));
    subjectItem->setText(QString::fromStdString((*it)->GetSubject()));
    this->eventsTable->setItem(row, 0, timeItem);
    this->eventsTable->setItem(row, 1, contextItem);
    this->eventsTable->setItem(row, 2, labelItem);
    this->eventsTable->setItem(row, 3, subjectItem);
    if ((previousSubject.compare((*it)->GetSubject()) != 0) && !previousSubject.empty())
       this->eventsTable->setColumnHidden(3, false);
    previousSubject = (*it)->GetSubject();
    if ((frameIndex < acq->GetFirstFrame()) || (frameIndex > acq->GetLastFrame()))
    {
      timeItem->setData(validEvent, false);
      //timeItem->setForeground(defaultLabelColor);
      //contextItem->setForeground(defaultLabelColor);
      //labelItem->setForeground(defaultLabelColor);
      //subjectItem->setForeground(defaultLabelColor);
    }
    else
      timeItem->setData(validEvent, true);
    
    ++row;
  }
  this->eventsTable->resizeColumnsToContents();
  this->eventsTable->sortItems(0);
  this->eventsTable->horizontalHeader()->setStretchLastSection(true);
  //this->newEventButton->setEnabled(true);
  this->newEventButton->setEnabled(false);
  this->deleteEventButton->setEnabled(false);
  this->downEventButton->setEnabled(false);
  this->upEventButton->setEnabled(false);
  this->showEventButton->setEnabled(false);
  //this->eventsDock->setEnabled(true);
  // Frames
  this->frameSlider->setMinimum(acq->GetFirstFrame());
  this->frameSlider->setMaximum(acq->GetLastFrame());
  this->frameSlider->setEnabled(true);
  this->playButton->setEnabled(true);
  // Update the 3D view
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
  btk::VTKGRFsFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_GRFS])->SetScaleUnit(scale);
  btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS])->SetScaleUnit(scale);
  btk::VTKForcePlatformsSource::SafeDownCast((*this->mp_VTKProc)[VTK_FORCE_PLATFORMS])->SetScaleUnit(scale);
  
  // Fill Metadata
  this->mp_MetadataDlg->load(acq->GetMetaData());
  // Display Docks
  if (markers->GetItemNumber() != 0)
    this->markersDock->setVisible(true);
  if (events->GetItemNumber() != 0)
    this->eventsDock->setVisible(true);

  QApplication::restoreOverrideCursor();
  
  this->frameSlider->setValue(this->frameSlider->minimum());
  this->updateDisplay(this->frameSlider->minimum());
  
  this->actionClose->setEnabled(true);
  this->actionEdit_Metadata->setEnabled(true);
  //this->actionSave->setEnabled(true); 
  //this->actionSave_As->setEnabled(true);
};

void MainWindow::closeFile()
{
  this->actionClose->setEnabled(false);
  this->actionEdit_Metadata->setEnabled(true);
  this->actionSave->setEnabled(false); 
  this->actionSave_As->setEnabled(false);
  this->setCurrentFile("");
  // Informations Dock
  this->fileNameValue->setText("");
  this->documentTypeValue->setText("");
  this->fileSizeValue->setText("");
  this->creationDateValue->setText("");
  this->modificationDateValue->setText("");
  this->fileFormatValue->setText("");
  this->byteOrderValue->setText("");
  this->storageFormatValue->setText("");
  this->pointNumberValue->setText("");
  this->pointFrequencyValue->setText("");
  this->analogNumberValue->setText("");
  this->analogFrequencyValue->setText("");
  this->eventNumberValue->setText("");
  // Markers dock
  this->markersTable->clearSelection();
  this->markersTable->clear();
  this->markersTable->setRowCount(0);
  this->showMarkersButton->setEnabled(false);
  this->hideMarkersButton->setEnabled(false);
  this->markersDock->setVisible(false);
  // Events dock
  this->eventsTable->clearSelection();
  this->eventsTable->clear();
  this->eventsTable->setRowCount(0);
  this->updateEventsButtonsState();
  this->eventsDock->setVisible(false);
  // Metadata
  this->mp_MetadataDlg->reset();
  // Playback
  this->frameSlider->setEnabled(false);
  this->playButton->setEnabled(false);
  this->showMarkersButton->setEnabled(false);
  this->hideMarkersButton->setEnabled(false);
  this->frameSlider->blockSignals(true);
  this->frameSlider->setMinimum(0);
  this->frameSlider->setValue(this->frameSlider->minimum());
  this->lcdNumber->display(this->frameSlider->minimum());
  this->frameSlider->blockSignals(false);
  vtkActorCollection* actors = this->mp_Renderer->GetActors();
  actors->InitTraversal();
  vtkActor* actor = actors->GetNextItem(); // Ground
  actor = actors->GetNextItem();
  while (actor)
  {
    actor->VisibilityOff();
    actor = actors->GetNextItem();
  }
  this->qvtkWidget->GetRenderWindow()->Render();
};

void MainWindow::changePlaybackParameters()
{
  btk::Acquisition::Pointer acq = static_pointer_cast<btk::AcquisitionFileReader>(this->m_BTKProc[BTK_READER])->GetOutput();
  double pointFrequency = acq->GetPointFrequency();
  
  // Compute playback step and delay
  if ((pointFrequency == 0.0) || (actionFull_Frames->isChecked()))
  {
    this->m_PlaybackDelay = 40; // 25 Hz
    this->m_PlaybackStep = 1;
  }
  else
  {
    double divider = 1.0;
    if (actionRealtime->isChecked())
      divider = 1.0;
    else if (action1_2->isChecked())
      divider = 0.5;
    else if (action1_5->isChecked())
      divider = 0.2;
    else if (action1_10->isChecked())
      divider = 0.1;

    double freq = pointFrequency * divider;
    int finalFreq = 1;
    if (freq >= 24.0)
    {
      double r = 1.0;
      for (int i = 24 ; i <= 30 ; ++i)
      {
        double residual = freq / static_cast<double>(i) - static_cast<double>(static_cast<int>(freq) / i);
        if (residual < r)
        {
          finalFreq = i;
          r = residual;
        }
      }
      this->m_PlaybackDelay = 1000 / finalFreq;
      this->m_PlaybackStep = freq / finalFreq;
    }
    else
    {
      this->m_PlaybackDelay = static_cast<int>(1000.0 / freq);
      this->m_PlaybackStep = 1;
    }
  }

  //std::cout << this->m_PlaybackDelay << " ms ; " << this->m_PlaybackStep << std::endl;

  // Relaunch playback if necessary
  if (this->mp_Timer->isActive())
  {
    this->mp_Timer->stop();
    this->mp_Timer->start(this->m_PlaybackDelay);
  }
};

void MainWindow::changeGroundOrientation()
{
  vtkPlaneSource* ground = vtkPlaneSource::SafeDownCast((*this->mp_VTKProc)[VTK_GROUND]);
  if (actionPlane_XY->isChecked())
    ground->SetNormal(0.0, 0.0, 1.0);
  else if (actionPlane_YZ->isChecked())
    ground->SetNormal(1.0, 0.0, 0.0);
  else if (actionPlane_ZX->isChecked())
    ground->SetNormal(0.0, 1.0, 0.0);
  this->qvtkWidget->GetRenderWindow()->Render();
};

void MainWindow::updateDisplay(int frame)
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
  this->qvtkWidget->GetRenderWindow()->Render();
};

void MainWindow::toggleTimer()
{
  if (this->mp_Timer->isActive())
  {
    this->mp_Timer->stop();
    this->playButton->setIcon(*this->mp_PlayIcon);
  }
  else
  {
    this->mp_Timer->start(this->m_PlaybackDelay);
    this->playButton->setIcon(*this->mp_PauseIcon);
  }
};

void MainWindow::displayPreviousFrame()
{
  this->displayPreviousFrame(this->m_PlaybackStep);
};

void MainWindow::displayPreviousFrame(int step)
{
  if ((this->frameSlider->value() - step) >= this->frameSlider->minimum())
    this->frameSlider->setValue(this->frameSlider->value() - step);
  else
    this->frameSlider->setValue(this->frameSlider->maximum());
};

void MainWindow::displayNextFrame()
{
  this->displayNextFrame(this->m_PlaybackStep);
};

void MainWindow::displayNextFrame(int step)
{
  if ((this->frameSlider->value() + step) <= this->frameSlider->maximum())
    this->frameSlider->setValue(this->frameSlider->value() + step);
  else
    this->frameSlider->setValue(this->frameSlider->minimum());
};

void MainWindow::updateMarkerRadiusSpinBox(int v)
{
  double r = static_cast<double>(v) / 10.0;
  this->markerRadiusSpinBox->setValue(r);
};

void MainWindow::toggleMarkersVisibilityButtons()
{
  QList<QTableWidgetItem*> items = this->markersTable->selectedItems();
  if (items.count() > 1)
  {
    this->showMarkersButton->setText(QObject::tr("Show"));
    this->hideMarkersButton->setText(QObject::tr("Hide"));
    disconnect(this->showMarkersButton, 0, this, 0);
    disconnect(this->hideMarkersButton, 0, this, 0);
    connect(this->showMarkersButton, SIGNAL(clicked()), this, SLOT(showSelectedMarkers()));
    connect(this->hideMarkersButton, SIGNAL(clicked()), this, SLOT(hideSelectedMarkers()));
  }
  else
  {
    this->showMarkersButton->setText(QObject::tr("Show All"));
    this->hideMarkersButton->setText(QObject::tr("Hide All"));
    disconnect(this->showMarkersButton, 0, this, 0);
    disconnect(this->hideMarkersButton, 0, this, 0);
    connect(this->showMarkersButton, SIGNAL(clicked()), this, SLOT(showAllMarkers()));
    connect(this->hideMarkersButton, SIGNAL(clicked()), this, SLOT(hideAllMarkers()));
  }
};

void MainWindow::showSelectedMarkers()
{
  QList<QTableWidgetItem*> items = this->markersTable->selectedItems();
  this->markersTable->blockSignals(true);
  for (QList<QTableWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
  {
    (*it)->setCheckState(Qt::Checked);
    btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS])->ShowMarker((*it)->data(markerId).toInt());
  }
  this->markersTable->blockSignals(false);
  this->updateDisplay(this->frameSlider->value());
};

void MainWindow::hideSelectedMarkers()
{
  QList<QTableWidgetItem*> items = this->markersTable->selectedItems();
  this->markersTable->blockSignals(true);
  for (QList<QTableWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
  {
    (*it)->setCheckState(Qt::Unchecked);
    btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS])->HideMarker((*it)->data(markerId).toInt());
  }
  this->markersTable->blockSignals(false);
  this->updateDisplay(this->frameSlider->value());
};

void MainWindow::showAllMarkers()
{
  this->markersTable->blockSignals(true);
  for (int row = 0 ; row < this->markersTable->rowCount() ; ++row)
    this->markersTable->item(row, 0)->setCheckState(Qt::Checked);
  this->markersTable->blockSignals(false);
  btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS])->ShowMarkers();
  this->updateDisplay(this->frameSlider->value());
};

void MainWindow::hideAllMarkers()
{
  this->markersTable->blockSignals(true);
  for (int row = 0 ; row < this->markersTable->rowCount() ; ++row)
    this->markersTable->item(row, 0)->setCheckState(Qt::Unchecked);
  this->markersTable->blockSignals(false);
  btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS])->HideMarkers();
  this->updateDisplay(this->frameSlider->value());
};

void MainWindow::updateMarkerVisibility(QTableWidgetItem* item)
{
  btk::VTKMarkersFramesSource* markers = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
  if (item->checkState() == Qt::Checked)
    markers->ShowMarker(item->data(markerId).toInt());
  if (item->checkState() == Qt::Unchecked)
    markers->HideMarker(item->data(markerId).toInt());
  this->updateDisplay(this->frameSlider->value());
};

void MainWindow::updateMarkerRadius(double r)
{
  this->markerRadiusSlider->blockSignals(true);
  this->markerRadiusSlider->setValue(r * 10.0);
  this->markerRadiusSlider->blockSignals(false);
  QList<QTableWidgetItem*> items = this->markersTable->selectedItems();
  for (QList<QTableWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
    btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS])->SetMarkerRadius((*it)->data(markerId).toInt(), r);
  this->updateDisplay(this->frameSlider->value());
};

void MainWindow::updateMarkerColor()
{
  QColor color = QColorDialog::getColor(Qt::white, this);
  if (color.isValid())
  {
    QString ss = "background-color: rgb( %1, %2, %3);";
    this->markerColorButton->setStyleSheet(ss.arg(color.red()).arg(color.green()).arg(color.blue()));
    QList<QTableWidgetItem*> items = this->markersTable->selectedItems();
    vtkIdTypeArray* ids = vtkIdTypeArray::New();
    ids->SetNumberOfValues(items.count());
    int i = 0;
    for (QList<QTableWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
    {
      ids->SetValue(i, (*it)->data(markerId).toInt());
      ++i;
    }
    btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS])->SetMarkersColor(ids, color.redF(), color.greenF(), color.blueF());
    this->updateDisplay(this->frameSlider->value());
    ids->Delete();
  }
};

void MainWindow::displayMarkerProperties()
{
  this->markerRadiusSpinBox->blockSignals(true);
  this->markerRadiusSlider->blockSignals(true);
  QList<QTableWidgetItem*> items = this->markersTable->selectedItems();
  if (items.isEmpty())
  {
    this->markerLabelEdit->setText("");
    this->markerLabelEdit->setEnabled(false);
    this->markerRadiusSpinBox->setEnabled(false);
    this->markerRadiusSlider->setEnabled(false);
    this->markerRadiusSlider->setValue(this->markerRadiusSlider->minimum());
    this->markerColorButton->setEnabled(false);
    this->markerRadiusSpinBox->clear();
    this->markerColorButton->setStyleSheet("");
    this->markerDescEdit->setText("");
    this->markerDescEdit->setEnabled(false);
  }
  else
  {
    if (items.count() != 1)
    {
      this->markerLabelEdit->setText("");
      this->markerDescEdit->setText("");
      this->markerLabelEdit->setEnabled(false);
      this->markerDescEdit->setEnabled(false);
    }
    else
    {
      int idx = items.first()->data(markerId).toInt();
      btk::PointCollection::Pointer markers = static_pointer_cast<btk::SpecializedPointsExtractor>(this->m_BTKProc[BTK_MARKERS])->GetOutput();
      btk::PointCollection::ConstIterator it = markers->Begin();
      std::advance(it, idx);
      this->markerLabelEdit->setText(QString::fromStdString((*it)->GetLabel()));
      this->markerDescEdit->setText(QString::fromStdString((*it)->GetDescription()));
      this->markerLabelEdit->setEnabled(true);
      this->markerDescEdit->setEnabled(true);
    }
    this->markerRadiusSpinBox->setEnabled(true);
    this->markerColorButton->setEnabled(true);
    this->markerRadiusSlider->setEnabled(true);
    bool sameRadius = true;
    bool sameColor = true;
    QList<QTableWidgetItem*>::const_iterator it = items.begin();
    btk::VTKMarkersFramesSource* markers = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
    double r = markers->GetMarkerRadius((*it)->data(markerId).toInt());
    vtkIdType c = markers->GetMarkerColorIndex((*it)->data(markerId).toInt());
    ++it;
    while (it != items.end())
    {
      if (r != markers->GetMarkerRadius((*it)->data(markerId).toInt()))
        sameRadius = false;
      if (c != markers->GetMarkerColorIndex((*it)->data(markerId).toInt()))
        sameColor = false;

      if (!sameRadius && !sameColor)
        break;
      ++it;
    }
    // radius
    if (!sameRadius)
    {
      this->markerRadiusSpinBox->clear();
      this->markerRadiusSlider->setValue(this->markerRadiusSlider->minimum());
    }
    else
    {
      this->markerRadiusSpinBox->setValue(r);
      this->markerRadiusSlider->setValue(r * 10.0);
    }
    // color
    if (!sameColor)
      this->markerColorButton->setStyleSheet("");
    else
    {
      double* rgba = markers->GetMarkerColorLUT()->GetTableValue(c);
      QString ss = "background-color: rgb( %1, %2, %3);";
      this->markerColorButton->setStyleSheet(
          ss.arg(static_cast<int>(rgba[0] * 255))
            .arg(static_cast<int>(rgba[1] * 255))
            .arg(static_cast<int>(rgba[2] * 255)));
    }
  }
  this->markerRadiusSpinBox->blockSignals(false);
  this->markerRadiusSlider->blockSignals(false);
};

void MainWindow::toggleMarkerProperties()
{
  if (!this->markerProperties->isVisible())
  {
    this->markerProperties->setVisible(true);
    this->markerPropertiesButton->setIcon(*this->mp_DownArrow);
  }
  else
  {
    this->markerProperties->setVisible(false);
    this->markerPropertiesButton->setIcon(*this->mp_RightArrow);
  }
};


void MainWindow::circleSelectedMarkers()
{
  btk::VTKMarkersFramesSource* markers = btk::VTKMarkersFramesSource::SafeDownCast((*this->mp_VTKProc)[VTK_MARKERS]);
  markers->ClearSelectedMarkers();
  QList<QTableWidgetItem*> items = this->markersTable->selectedItems();
  bool validSelection = false;
  for (QList<QTableWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
  {
    if ((*it)->checkState() == Qt::Checked)
    {
      markers->SetSelectedMarker((*it)->data(markerId).toInt());
      validSelection = true;
    }
  }
  if (validSelection || items.isEmpty())
    this->updateDisplay(this->frameSlider->value());
};

void MainWindow::showEvent()
{
  this->eventsTable->setStyleSheet("selection-color: rgb(255,0,0);");

  int row = this->eventsTable->currentRow();
  int frame = this->eventsTable->item(row, 0)->data(eventFrame).toInt();
  this->frameSlider->setValue(frame);
};

void MainWindow::updateActiveEvent(int frame)
{
  //this->eventsTable->blockSignals(true);
  QBrush activeEventColor = QBrush(QColor(Qt::red));
  QBrush defaultEventColor = QBrush(QColor(Qt::black));
  QBrush inactiveEventColor = QBrush(QColor(Qt::gray));
  bool eventFound = false;
  for (int row = 0 ; row < this->eventsTable->rowCount() ; ++row)
  {
    if (frame == this->eventsTable->item(row, 0)->data(eventFrame).toInt())
    {
      if (row == this->eventsTable->currentRow())
      {
        this->eventsTable->setStyleSheet("selection-color: rgb(255,0,0);");
        eventFound = true;
      }
      this->eventsTable->item(row, 0)->setForeground(activeEventColor);
      this->eventsTable->item(row, 1)->setForeground(activeEventColor);
      this->eventsTable->item(row, 2)->setForeground(activeEventColor);
      this->eventsTable->item(row, 3)->setForeground(activeEventColor);
    }
    else
    {
      if (!eventFound)
        this->eventsTable->setStyleSheet("");
      if (this->eventsTable->item(row, 0)->data(validEvent).toInt())
      {
        this->eventsTable->item(row, 0)->setForeground(defaultEventColor);
        this->eventsTable->item(row, 1)->setForeground(defaultEventColor);
        this->eventsTable->item(row, 2)->setForeground(defaultEventColor);
        this->eventsTable->item(row, 3)->setForeground(defaultEventColor);
      }
      else
      {
        this->eventsTable->item(row, 0)->setForeground(inactiveEventColor);
        this->eventsTable->item(row, 1)->setForeground(inactiveEventColor);
        this->eventsTable->item(row, 2)->setForeground(inactiveEventColor);
        this->eventsTable->item(row, 3)->setForeground(inactiveEventColor);
      }
    }
  }
  //this->eventsTable->blockSignals(false);
};

void MainWindow::displayEventInformations()
{
  QList<QTableWidgetItem*> items = this->eventsTable->selectedItems();
  if (items.isEmpty())
  {
    this->eventLabelEdit->setEnabled(false);
    this->eventContextCombo->setEnabled(false);
    this->eventTimeSpinBox->setEnabled(false);
    this->eventSubjectEdit->setEnabled(false);
    this->eventLabelEdit->clear();
    this->eventContextCombo->clear();
    this->eventTimeSpinBox->clear();
    this->eventSubjectEdit->clear();
  }
  else
  {
    int row = this->eventsTable->currentRow();
    if (this->frameSlider->value() == this->eventsTable->item(row, 0)->data(eventFrame).toInt())
      this->eventsTable->setStyleSheet("selection-color: rgb(255,0,0);");
    else
      this->eventsTable->setStyleSheet("");
    this->eventLabelEdit->setEnabled(true);
    this->eventContextCombo->setEnabled(true);
    this->eventTimeSpinBox->setEnabled(true);
    this->eventSubjectEdit->setEnabled(true);
    this->eventTimeSpinBox->setValue(this->eventsTable->item(row, 0)->text().toDouble());
    this->eventContextCombo->setItemText(0, this->eventsTable->item(row, 1)->text());
    this->eventLabelEdit->setText(this->eventsTable->item(row, 2)->text());
    this->eventSubjectEdit->setText(this->eventsTable->item(row, 3)->text());
  }
};

void MainWindow::toggleEventInformations()
{
  if (!this->eventInformations->isVisible())
  {
    this->eventInformations->setVisible(true);
    this->eventInformationsButton->setIcon(*this->mp_DownArrow);
  }
  else
  {
    this->eventInformations->setVisible(false);
    this->eventInformationsButton->setIcon(*this->mp_RightArrow);
  }
};

void MainWindow::updateEventsButtonsState()
{
  //this->newEventButton->setEnabled(true);
  QList<QTableWidgetItem*> items = this->eventsTable->selectedItems();
  if (!items.isEmpty())
  {
    int row = this->eventsTable->currentRow();
    //this->deleteEventButton->setEnabled(true);
    //this->downEventButton->setEnabled(true);
    //this->upEventButton->setEnabled(true);
    int valid = this->eventsTable->item(row, 0)->data(validEvent).toInt();
    if (valid)
      this->showEventButton->setEnabled(true);
    else
      this->showEventButton->setEnabled(false);
  }
  else
  {
    this->deleteEventButton->setEnabled(false);
    this->showEventButton->setEnabled(false);
    this->downEventButton->setEnabled(false);
    this->upEventButton->setEnabled(false);
  }
};

void MainWindow::updateDisplayedMarkersList(vtkObject* caller, unsigned long /* vtk_event */, void* /* client_data */, void* call_data)
{
  vtkIdTypeArray* indexes = static_cast<vtkIdTypeArray*>(call_data);
  if (!indexes)
    return;

  QBrush defaultLabelColor = QBrush(QColor(Qt::gray));
  QBrush displayLabelColor = QBrush(QColor(Qt::black));
  this->markersTable->blockSignals(true);
  for (int row = 0 ; row < this->markersTable->rowCount() ; ++row)
  {
    QTableWidgetItem* item = this->markersTable->item(row, 0);
    if (indexes->GetValue(row) && (item->checkState() == Qt::Checked))
      item->setForeground(displayLabelColor);
    else
      item->setForeground(defaultLabelColor);
  }
  this->markersTable->blockSignals(false);
};

void MainWindow::selectPickedMarker(vtkObject* caller, unsigned long /* vtk_event */, void* /* client_data */, void* call_data)
{
  int id = *static_cast<int*>(call_data);
  QList<QTableWidgetItem*> items = this->markersTable->selectedItems();
  if (items.count() == 1)
  {
    if (items.first()->data(markerId).toInt() == id)
    {
      this->markersTable->clearSelection();
      return;
    }
  }
  for (int row = 0 ; row < this->markersTable->rowCount() ; ++row)
  {
    QTableWidgetItem* item = this->markersTable->item(row, 0);
    if (item->data(markerId).toInt() == id)
    {
      this->markersTable->setCurrentCell(row, 0);
      break;
    }
  }
};

void MainWindow::selectPickedMarkers(vtkObject* caller, unsigned long /* vtk_event */, void* /* client_data */, void* call_data)
{
  int id = *static_cast<int*>(call_data);
  QList<QTableWidgetItem*> items = this->markersTable->selectedItems();
  // Previously selected?
  for (QList<QTableWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
  {
    if ((*it)->data(markerId).toInt() == id)
    {
      this->markersTable->setCurrentItem((*it), QItemSelectionModel::Deselect);
      return;
    }
  }
  // To be selected
  for (int row = 0 ; row < this->markersTable->rowCount() ; ++row)
  {
    QTableWidgetItem* item = this->markersTable->item(row, 0);
    if (item->data(markerId).toInt() == id)
    {
      this->markersTable->setCurrentItem(item, QItemSelectionModel::Select);
      break;
    }
  }
};

void MainWindow::markersDockLocationChanged(Qt::DockWidgetArea area)
{
  QSettings settings;
  settings.beginGroup("MarkersDock");
  settings.setValue("area", area);
  settings.endGroup();
};

void MainWindow::eventsDockLocationChanged(Qt::DockWidgetArea area)
{
  QSettings settings;
  settings.beginGroup("EventsDock");
  settings.setValue("area", area);
  settings.endGroup();
};

void MainWindow::readSettings()
{   
  QSettings settings;
  // MainWidow
  settings.beginGroup("MainWindow");
  // - Size & position
  this->move(settings.value("pos", QPoint(100, 100)).toPoint());
  this->resize(settings.value("size", QSize(1024, 768)).toSize());
  // - Recent files submenu
  this->m_RecentFiles = settings.value("recentFiles").toStringList(); 
  this->updateRecentFileActions();
  // - Last directory
  this->m_LastDirectory = settings.value("lastDirectory", ".").toString();
  settings.endGroup();
  // MarkersDock
  settings.beginGroup("MarkersDock");
  //this->markersDock->setVisible(settings.value("visible", false).toBool());
  this->markersDock->setFloating(settings.value("floating", false).toBool());
  this->markersDock->move(settings.value("pos", this->markersDock->pos()).toPoint());
  this->markersDock->resize(settings.value("size", this->markersDock->size()).toSize());
  if (!this->markersDock->isFloating())
    addDockWidget(static_cast<Qt::DockWidgetArea>(settings.value("area", Qt::RightDockWidgetArea).toInt()), this->markersDock);
  settings.endGroup();
  // EventsDock
  settings.beginGroup("EventsDock");
  //this->eventsDock->setVisible(settings.value("visible", false).toBool());
  this->eventsDock->setFloating(settings.value("floating", false).toBool());
  this->eventsDock->move(settings.value("pos", this->eventsDock->pos()).toPoint());
  this->eventsDock->resize(settings.value("size", this->eventsDock->size()).toSize());
  if (!this->eventsDock->isFloating())
    addDockWidget(static_cast<Qt::DockWidgetArea>(settings.value("area", Qt::RightDockWidgetArea).toInt()), this->eventsDock);
  settings.endGroup();
};

void MainWindow::writeSettings()
{
  QSettings settings;
  // MainWindow
  // - Size & position 
  settings.beginGroup("MainWindow");
  settings.setValue("pos", this->pos());
  settings.setValue("size", this->size());
  // - Recent files submenu
  settings.setValue("recentFiles", this->m_RecentFiles);
  // - Last directory
  settings.setValue("lastDirectory", this->m_LastDirectory);
  settings.endGroup();
  // MarkersDock
  settings.beginGroup("MarkersDock");
  //settings.setValue("visible", this->markersDock->isVisible());
  settings.setValue("floating", this->markersDock->isFloating());
  settings.setValue("pos", this->markersDock->pos());
  settings.setValue("size", this->markersDock->size());
  //settings.setValue("area", this->markersDock->area());
  settings.endGroup();
  // EventsDock
  settings.beginGroup("EventsDock");
  //settings.setValue("visible", this->eventsDock->isVisible());
  settings.setValue("floating", this->eventsDock->isFloating());
  settings.setValue("pos", this->eventsDock->pos());
  settings.setValue("size", this->eventsDock->size());
  //settings.setValue("area", this->eventsDock->area());
  settings.endGroup();
};

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

#include "mokkaConfigure.h"
#include "MainWindow.h"
#include "About.h"
#include "Acquisition.h"
#include "Model.h"
#include "FileInfoDockWidget.h"
#include "ImportAssistantDialog.h"
#include "LoggerMessage.h"
#include "LoggerWidget.h"
#include "Metadata.h"
#include "ModelDockWidget.h"
#include "ProgressWidget.h"
#include "UndoCommands.h"
#include "UpdateChecker.h"
#include "Preferences.h"
#include "TimeEventFunctors.h"
#include "NewLayoutDialog.h"
#include "UserDefined.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QCloseEvent>
#include <QMessageBox>
#include <QSettings>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QUndoStack>

MainWindow::MainWindow(QWidget* parent)
:QMainWindow(parent), m_LastDirectory("."), m_RecentFiles(), m_UserLayouts()
{
  // Members
  this->mp_Acquisition = new Acquisition(this);
  this->mp_Model = new Model(this);
  this->mp_MetadataDlg = new Metadata(this);
  this->mp_ModelDock = new ModelDockWidget(this);
  this->mp_FileInfoDock = new FileInfoDockWidget(this);
  this->mp_ImportAssistant = new ImportAssistantDialog(this);
  this->mp_UpdateChecker = new UpdateChecker(btkStringifyMacro(MOKKA_VERSION_STRING), 
                                              "http://b-tk.googlecode.com/svn/doc/Mokka/latestMokka",
                                              ":/Resources/Images/Mokka_128.png", this);
#ifdef Q_OS_MAC
  this->mp_MacMenuBar = 0;
  this->mp_Preferences = new Preferences(0); // No parent: to be independant of the main window
#else
  this->mp_Preferences = new Preferences(this);
#endif
  this->mp_Preferences->setUserLayouts(&this->m_UserLayouts);
  // Finalize UI
  this->mp_FileInfoDock->setVisible(false);
  this->mp_FileInfoDock->setFloating(true);
  this->mp_FileInfoDock->move(100,100);
  this->addDockWidget(Qt::RightDockWidgetArea, this->mp_ModelDock); 
  this->mp_ModelDock->setVisible(false);
  this->setupUi(this);
  this->menuExport->menuAction()->setEnabled(false);
  this->menuVisual_Configuration->addAction(this->mp_ModelDock->deselectConfigurationAction());
  this->menuVisual_Configuration->addAction(this->mp_ModelDock->clearConfigurationsAction());
  QAction* actionModelDockView = this->mp_ModelDock->toggleViewAction();
#ifdef Q_OS_MAC
  actionModelDockView->setShortcut(QKeySequence(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_M));
#else
  actionModelDockView->setShortcut(QKeySequence(Qt::ControlModifier + Qt::Key_M));
#endif
  this->menuView->addAction(actionModelDockView);
  this->menuSettings->addMenu(this->multiView->groundOrientationMenu());
  this->menuSettings->addMenu(this->multiView->markerTrajectoryLengthMenu());
  this->menuSettings->addMenu(this->timeEventControler->playbackSpeedMenu());
  QAction* actionInformationsDockView = this->mp_FileInfoDock->toggleViewAction();
  actionInformationsDockView->setShortcut(QKeySequence(Qt::ControlModifier + Qt::Key_I));
  this->menuView->addAction(actionInformationsDockView);
  this->menuView->addAction(this->actionViewMetadata);
  this->timeEventControler->playbackSpeedMenu()->menuAction()->setEnabled(true);
#ifdef Q_OS_MAC
  QFont f = this->font();
  f.setPointSize(10);
  this->mp_FileInfoDock->setFont(f);
  // To share the menu between windows (main window, preferences, options, ...)
  this->mp_MacMenuBar = this->menuBar();
  this->mp_MacMenuBar->setParent(0);
  this->setMenuBar(0);
  QAction* actionMinimise = new QAction(tr("Minimize"), this);
  actionMinimise->setShortcut(QKeySequence(Qt::ControlModifier + Qt::Key_M));
  this->menuWindow->insertAction(this->menuLayouts->menuAction(), actionMinimise);
  this->menuWindow->insertSeparator(this->menuLayouts->menuAction());
  connect(actionMinimise,  SIGNAL(triggered()), this, SLOT(showMinimized()));
#endif
  this->mp_FileInfoDock->reset(); // Force to update geometry
  this->action_FileOpen->setShortcut(QKeySequence::Open);
  this->actionClose->setShortcut(QKeySequence::Close);
  this->actionSave->setShortcut(QKeySequence::Save);
  this->actionSave_As->setShortcut(QKeySequence::SaveAs);
  this->actionCut->setShortcut(QKeySequence::Cut);
  this->actionCopy->setShortcut(QKeySequence::Copy);
  this->actionPaste->setShortcut(QKeySequence::Paste);
  this->actionSelect_All->setShortcut(QKeySequence::SelectAll);
  this->actionHelp->setShortcut(QKeySequence::HelpContents);
  this->actionLayout3DOnly->setShortcut(QKeySequence(Qt::ControlModifier + Qt::Key_1));
  this->actionLayout3DVerbose->setShortcut(QKeySequence(Qt::ControlModifier + Qt::Key_2));
  this->actionLayout3DCharts->setShortcut(QKeySequence(Qt::ControlModifier + Qt::Key_3));
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
  this->actionImportVideo->setEnabled(false); // Only enabled when an acquisition is opened or imported.
  this->mp_ActionSeparatorUserLayouts = this->menuLayouts->addSeparator();
  QActionGroup* layoutActionGroup = new QActionGroup(this);
  layoutActionGroup->addAction(this->actionLayout3DOnly);
  layoutActionGroup->addAction(this->actionLayout3DVerbose);
  layoutActionGroup->addAction(this->actionLayout3DCharts);
  for (int i = 0 ; i < maxUserLayouts ; ++i)
  {
    this->mp_ActionUserLayouts[i] = new QAction(layoutActionGroup);
    this->mp_ActionUserLayouts[i]->setCheckable(true);
    this->mp_ActionUserLayouts[i]->setVisible(false);
    connect(this->mp_ActionUserLayouts[i], SIGNAL(triggered()), this, SLOT(restoreLayout()));
    this->menuLayouts->addAction(this->mp_ActionUserLayouts[i]);
    if (i < 6)
      this->mp_ActionUserLayouts[i]->setShortcut(QKeySequence(Qt::ControlModifier + static_cast<Qt::Key>(52+i))); // Starting from Qt::Key_4
    else if (i == 6)
      this->mp_ActionUserLayouts[i]->setShortcut(QKeySequence(Qt::ControlModifier + Qt::Key_0));
    // layoutActionGroup->addAction(this->mp_ActionUserLayouts[i]);
  }
  this->timeEventControler->acquisitionOptionsButtonMenu->menu()->insertMenu(this->timeEventControler->playbackSpeedMenu()->menuAction(), this->multiView->groundOrientationMenu());
  this->timeEventControler->acquisitionOptionsButtonMenu->menu()->insertMenu(this->multiView->groundOrientationMenu()->menuAction(), this->multiView->markerTrajectoryLengthMenu());
  this->multiView->initialize();
  // Contextual menu for the 3D views
  QList<QAction*> actions3d;
  actions3d.push_back(this->mp_ModelDock->newSegmentAction());
  QAction* sep1 = new QAction(this); sep1->setSeparator(true);
  actions3d.push_back(sep1);
  actions3d.push_back(this->mp_ModelDock->selectAllMarkersAction());
  actions3d.push_back(this->mp_ModelDock->hideSelectedMarkersAction());
  actions3d.push_back(this->mp_ModelDock->unhideSelectedMarkersAction());
  actions3d.push_back(this->mp_ModelDock->activeTrajectorySelectedMarkersAction());
  actions3d.push_back(this->mp_ModelDock->unactiveTrajectorySelectedMarkersAction());
  actions3d.push_back(this->mp_ModelDock->deleteSelectedMarkersAction());
  QAction* sep2 = new QAction(this); sep2->setSeparator(true);
  actions3d.push_back(sep2);
  actions3d.push_back(this->timeEventControler->insertEventMenu()->menuAction());
  actions3d.push_back(this->timeEventControler->playbackSpeedMenu()->menuAction());
  this->multiView->setView3dActions(actions3d);
  // Contextual menu for the charts (append to the default list)
  QList<QAction*> actionsChart;
  QAction* sep3 = new QAction(this); sep3->setSeparator(true);
  actionsChart.push_back(sep3);
  actionsChart.push_back(this->timeEventControler->insertEventMenu()->menuAction());
  actionsChart.push_back(this->timeEventControler->playbackSpeedMenu()->menuAction());
  this->multiView->setViewChartActions(actionsChart);
  
  //this->mp_ImportAssistant->resize(this->mp_ImportAssistant->width(), this->mp_ImportAssistant->height());
  this->mp_ImportAssistant->layout()->setSizeConstraint(QLayout::SetFixedSize);
  
  // Setting the acquisition
  this->timeEventControler->setAcquisition(this->mp_Acquisition); // Must be set before the multiview.
  this->multiView->setAcquisition(this->mp_Acquisition);
  this->mp_ModelDock->setAcquisition(this->mp_Acquisition);
  this->mp_FileInfoDock->setAcquisition(this->mp_Acquisition);
  // Setting the model
  this->mp_ModelDock->setModel(this->mp_Model);
  this->multiView->setModel(this->mp_Model);
  
  // Qt UI: Undo/Redo
  this->mp_UndoStack = new QUndoStack(this); // One to command all.
  this->mp_MarkerConfigurationUndoStack = new QUndoStack(this);
  connect(this->mp_MarkerConfigurationUndoStack, SIGNAL(cleanChanged(bool)), this->mp_ModelDock, SLOT(setConfigurationModified(bool)));
  this->mp_AcquisitionUndoStack = new QUndoStack(this);
  connect(this->mp_AcquisitionUndoStack, SIGNAL(indexChanged(int)), this, SLOT(setAcquisitionModified(int)));
  QAction* actionUndo = this->mp_UndoStack->createUndoAction(this);
  actionUndo->setShortcut(QKeySequence::Undo);
  QAction* actionRedo = this->mp_UndoStack->createRedoAction(this);
  actionRedo->setShortcut(QKeySequence::Redo);
  this->menuEdit->insertAction(this->actionCut, actionUndo);
  this->menuEdit->insertAction(this->actionCut, actionRedo);
  this->menuEdit->insertSeparator(this->actionCut);

  // Qt Signal/Slot connection
  // Menu
  connect(this->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
  connect(this->actionVisit_BTK_website, SIGNAL(triggered()), this, SLOT(visitBTKWebsite()));
  connect(this->actionHelp, SIGNAL(triggered()), this, SLOT(help()));
  connect(this->actionViewMetadata, SIGNAL(triggered()), this, SLOT(viewMetadata()));
  connect(this->action_FileOpen, SIGNAL(triggered()), this, SLOT(openFile()));
  connect(this->actionSave, SIGNAL(triggered()), this, SLOT(saveFile()));
  connect(this->actionSave_As, SIGNAL(triggered()), this, SLOT(saveAsFile()));
  connect(this->actionClose, SIGNAL(triggered()), this, SLOT(closeFile()));
  connect(this->action_Quit, SIGNAL(triggered()), this, SLOT(close()));
  connect(this->actionImportAssistant, SIGNAL(triggered()), this, SLOT(importAssistant()));
  connect(this->actionImportC3D, SIGNAL(triggered()), this, SLOT(importC3D()));
  connect(this->actionImportTRB, SIGNAL(triggered()), this, SLOT(importTRB()));
  connect(this->actionImportTRC, SIGNAL(triggered()), this, SLOT(importTRC()));
  connect(this->actionImportANB, SIGNAL(triggered()), this, SLOT(importANB()));
  connect(this->actionImportANC, SIGNAL(triggered()), this, SLOT(importANC()));
  connect(this->actionImportCAL, SIGNAL(triggered()), this, SLOT(importForcePlatformCAL()));
  connect(this->actionImportOrthoTrakXLS, SIGNAL(triggered()), this, SLOT(importOrthoTrakXLS()));
  connect(this->actionImportTDF, SIGNAL(triggered()), this, SLOT(importTDF()));
  connect(this->actionImportRIC, SIGNAL(triggered()), this, SLOT(importRIC()));
  connect(this->actionImportRIF, SIGNAL(triggered()), this, SLOT(importRIF()));
  connect(this->actionImportRAH, SIGNAL(triggered()), this, SLOT(importRAH()));
  connect(this->actionImportRAW, SIGNAL(triggered()), this, SLOT(importRAW()));
  connect(this->actionImportANG, SIGNAL(triggered()), this, SLOT(importANG()));
  connect(this->actionImportEMG, SIGNAL(triggered()), this, SLOT(importEMG()));
  connect(this->actionImportMOM, SIGNAL(triggered()), this, SLOT(importMOM()));
  connect(this->actionImportPWR, SIGNAL(triggered()), this, SLOT(importPWR()));
  connect(this->actionImportGR, SIGNAL(triggered()), this, SLOT(importGRx()));
  connect(this->actionImportVideo, SIGNAL(triggered()), this, SLOT(importVideos()));
  connect(this->actionExportC3D, SIGNAL(triggered()), this, SLOT(exportC3D()));
  connect(this->actionExportTRC, SIGNAL(triggered()), this, SLOT(exportTRC()));
  connect(this->actionExportANB, SIGNAL(triggered()), this, SLOT(exportANB()));
  connect(this->actionExportANC, SIGNAL(triggered()), this, SLOT(exportANC()));
  connect(this->actionExportCAL, SIGNAL(triggered()), this, SLOT(exportCAL()));
  connect(this->actionPreferences, SIGNAL(triggered()), this, SLOT(showPreferences()));
  connect(this->actionSelect_All, SIGNAL(triggered()), this, SLOT(selectAll()));
  connect(this->actionCopy, SIGNAL(triggered()), this, SLOT(copy()));
  connect(this->actionSaveCurrentLayout, SIGNAL(triggered()), this, SLOT(saveCurrentLayout()));
  connect(this->actionManageUserLayouts, SIGNAL(triggered()), this, SLOT(manageUserLayouts()));
  connect(this->actionLayout3DOnly, SIGNAL(triggered()), this, SLOT(restoreLayout3DOnly()));
  connect(this->actionLayout3DVerbose, SIGNAL(triggered()), this, SLOT(restoreLayout3DVerbose()));
  connect(this->actionLayout3DCharts, SIGNAL(triggered()), this, SLOT(restoreLayout3DCharts()));
  // MultiView
  connect(this->multiView, SIGNAL(fileDropped(QString)), this, SLOT(openFileDropped(QString)));
  connect(this->multiView, SIGNAL(visibleMarkersChanged(QVector<int>)), this->mp_ModelDock, SLOT(updateDisplayedMarkers(QVector<int>)));
  connect(this->multiView, SIGNAL(pickedMarkerChanged(int)), this, SLOT(selectPickedMarker(int)));
  connect(this->multiView, SIGNAL(pickedMarkerToggled(int)), this, SLOT(togglePickedMarker(int)));
  connect(this->multiView, SIGNAL(selectedMarkersToggled(QList<int>)), this, SLOT(selectSelectedMarkers(QList<int>)));
  connect(this->multiView, SIGNAL(trajectoryMarkerToggled(int)), this, SLOT(toggleMarkerTrajectory(int)));
  connect(this->multiView, SIGNAL(pausePlaybackRequested(bool)), this, SLOT(playPausePlayback(bool)));
  // Model dock
  connect(this->mp_ModelDock, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)), this, SLOT(modelDockLocationChanged(Qt::DockWidgetArea)));
  connect(this->mp_ModelDock, SIGNAL(markerLabelChanged(int, QString)), this, SLOT(setPointLabel(int, QString)));
  connect(this->mp_ModelDock, SIGNAL(markersRadiusChanged(QVector<int>, double)), this, SLOT(setMarkersRadius(QVector<int>, double)));
  connect(this->mp_ModelDock, SIGNAL(markersColorChanged(QVector<int>, QColor)), this, SLOT(setMarkersColor(QVector<int>, QColor)));
  connect(this->mp_ModelDock, SIGNAL(markersDescriptionChanged(QVector<int>, QString)), this, SLOT(setPointsDescription(QVector<int>, QString)));
  connect(this->mp_ModelDock, SIGNAL(pointLabelChanged(int, QString)), this, SLOT(setPointLabel(int, QString)));
  connect(this->mp_ModelDock, SIGNAL(pointsDescriptionChanged(QVector<int>, QString)), this, SLOT(setPointsDescription(QVector<int>, QString)));
  connect(this->mp_ModelDock, SIGNAL(pointsRemoved(QList<int>)), this, SLOT(removePoints(QList<int>)));
  connect(this->mp_ModelDock, SIGNAL(analogLabelChanged(int, QString)), this, SLOT(setAnalogLabel(int, QString)));
  connect(this->mp_ModelDock, SIGNAL(analogsUnitChanged(QVector<int>, QString)), this, SLOT(setAnalogsUnit(QVector<int>, QString)));
  connect(this->mp_ModelDock, SIGNAL(analogsGainChanged(QVector<int>, Analog::Gain)), this, SLOT(setAnalogsGain(QVector<int>, Analog::Gain)));
  connect(this->mp_ModelDock, SIGNAL(analogsOffsetChanged(QVector<int>, int)), this, SLOT(setAnalogsOffset(QVector<int>, int)));
  connect(this->mp_ModelDock, SIGNAL(analogsScaleChanged(QVector<int>, double)), this, SLOT(setAnalogsScale(QVector<int>, double)));
  connect(this->mp_ModelDock, SIGNAL(analogsDescriptionChanged(QVector<int>, QString)), this, SLOT(setAnalogsDescription(QVector<int>, QString)));
  connect(this->mp_ModelDock, SIGNAL(analogsRemoved(QList<int>)), this, SLOT(removeAnalogs(QList<int>)));
  connect(this->mp_ModelDock, SIGNAL(configurationSaved()), this->mp_MarkerConfigurationUndoStack, SLOT(setClean()));
  connect(this->mp_ModelDock, SIGNAL(newSegmentsInserted(QList<int>, QList<Segment*>)), this->multiView, SLOT(appendNewSegments(QList<int>, QList<Segment*>)));
  connect(this->mp_ModelDock, SIGNAL(segmentsCleared()), this->multiView, SLOT(clearSegments()));
  connect(this->mp_ModelDock, SIGNAL(markerSelectionChanged(QList<int>)), this->multiView, SLOT(circleSelectedMarkers(QList<int>)));
  connect(this->mp_ModelDock, SIGNAL(markerHiddenSelectionChanged(QList<int>)), this->multiView, SLOT(updateHiddenMarkers(QList<int>)));
  connect(this->mp_ModelDock, SIGNAL(markerTrajectorySelectionChanged(QList<int>)), this->multiView, SLOT(updateTrackedMarkers(QList<int>)));
  connect(this->mp_ModelDock->markerRadiusSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateSelectedMarkersRadius(double)));
  connect(this->mp_ModelDock, SIGNAL(segmentLabelChanged(int, QString)), this, SLOT(setSegmentLabel(int, QString)));
  connect(this->mp_ModelDock, SIGNAL(segmentsColorChanged(QVector<int>, QColor)), this, SLOT(setSegmentsColor(QVector<int>, QColor)));
  connect(this->mp_ModelDock, SIGNAL(segmentsDescriptionChanged(QVector<int>, QString)), this, SLOT(setSegmentsDescription(QVector<int>, QString)));
  connect(this->mp_ModelDock, SIGNAL(segmentLinksChanged(int, QVector<int>, QVector< QPair<int,int> >)), this, SLOT(setSegmentLinks(int, QVector<int>, QVector< QPair<int,int> >)));
  connect(this->mp_ModelDock, SIGNAL(segmentsRemoved(QList<int>)), this, SLOT(removeSegments(QList<int>)));
  connect(this->mp_ModelDock, SIGNAL(segmentCreated(Segment*)), this, SLOT(insertSegment(Segment*)));
  connect(this->mp_ModelDock, SIGNAL(segmentHiddenSelectionChanged(QList<int>)), this->multiView, SLOT(updateHiddenSegments(QList<int>)));
  connect(this->mp_ModelDock, SIGNAL(videosRemoved(QList<int>)), this, SLOT(removeVideos(QList<int>)));
  connect(this->mp_ModelDock, SIGNAL(videosDelayChanged(QVector<int>, qint64)), this, SLOT(setVideosDelay(QVector<int>, qint64)));
  connect(this->mp_ModelDock->videoDelaySpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateSelectedVideosDelay(double)));
  // Time Event
  connect(this->timeEventControler, SIGNAL(currentFrameChanged(int)), this->multiView, SLOT(updateDisplay(int)));
  connect(this->timeEventControler, SIGNAL(regionOfInterestChanged(int,int)), this, SLOT(setRegionOfInterest(int,int)));
  connect(this->timeEventControler, SIGNAL(eventFrameModified(int,int)), this, SLOT(setEventFrame(int,int)));
  connect(this->timeEventControler, SIGNAL(eventsModified(QList<int>, QList<Event*>)), this, SLOT(setEvents(QList<int>, QList<Event*>)));
  connect(this->timeEventControler, SIGNAL(eventsRemoved(QList<int>)), this, SLOT(removeEvents(QList<int>)));
  connect(this->timeEventControler, SIGNAL(eventInserted(Event*)), this, SLOT(insertEvent(Event*)));
  connect(this->timeEventControler, SIGNAL(playbackStarted()), this->multiView, SLOT(forceRubberBandDrawingOff()));
  connect(this->timeEventControler, SIGNAL(playbackStopped()), this->multiView, SLOT(forceRubberBandDrawingOn()));
  connect(this->timeEventControler, SIGNAL(acquisitionReframed(int)), this, SLOT(reframeAcquisition(int)));
  
  // Functors to link the TimeEventWidget object with the VTK charts.
  // Give an easy way to update the displayed current frame and bounds in the charts.
  this->multiView->setCurrentFrameFunctor(CurrentFrameFunctor::New(this->timeEventControler));
  this->multiView->setRegionOfInterestFunctor(RegionOfInterestFunctor::New(this->timeEventControler));
  this->multiView->setEventsFunctor(EventsFunctor::New(this->timeEventControler));
  
#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
  this->menuHelp->addSeparator();
  QAction* actionCheckUpdate = this->menuHelp->addAction(tr("Check for Updates..."));
  actionCheckUpdate->setMenuRole(QAction::ApplicationSpecificRole);
  connect(actionCheckUpdate, SIGNAL(triggered()), this->mp_UpdateChecker, SLOT(check()));
#endif

  // Event filter
  this->multiView->setEventFilterObject(this);
  this->mp_ModelDock->installEventFilter(this);
  this->timeEventControler->installEventFilter(this);
  this->mp_ModelDock->modelTree->installEventFilter(this);
  this->mp_ModelDock->segmentLabelEdit->installEventFilter(this);
  this->mp_ModelDock->segmentDescEdit->installEventFilter(this);
  this->mp_ModelDock->markerLabelEdit->installEventFilter(this);
  this->mp_ModelDock->markerRadiusSpinBox->installEventFilter(this);
  this->mp_ModelDock->markerDescEdit->installEventFilter(this);
  this->mp_ModelDock->pointLabelEdit->installEventFilter(this);
  this->mp_ModelDock->pointDescEdit->installEventFilter(this);
  this->mp_ModelDock->analogLabelEdit->installEventFilter(this);
  this->mp_ModelDock->analogUnitEdit->installEventFilter(this);
  this->mp_ModelDock->analogScaleSpinBox->installEventFilter(this);
  this->mp_ModelDock->analogOffsetSpinBox->installEventFilter(this);
  this->mp_ModelDock->analogDescEdit->installEventFilter(this);
  this->mp_ModelDock->videoDelaySpinBox->installEventFilter(this);
  
  // Settings
  QCoreApplication::setOrganizationName("BTK");
  QCoreApplication::setOrganizationDomain("btk.org");
  QCoreApplication::setApplicationName("Mokka");
  this->readSettings();
  this->setCurrentFile("");
  
  // Preferences connections. Must be set after the reading of the settings.
  connect(this->mp_Preferences, SIGNAL(useDefaultConfigurationStateChanged(bool)), this, SLOT(setPreferenceUseDefaultConfiguration(bool)));
  connect(this->mp_Preferences, SIGNAL(defaultConfigurationPathChanged(QString)), this, SLOT(setPreferenceDefaultConfigurationPath(QString)));
  connect(this->mp_Preferences, SIGNAL(defaultGroundOrientationChanged(int)), this, SLOT(setPreferenceDefaultOrientation(int)));
  connect(this->mp_Preferences, SIGNAL(useEventEditorWhenInsertingStateChanged(bool)), this, SLOT(setPreferenceUseEventEditorWhenInserting(bool)));
  connect(this->mp_Preferences, SIGNAL(defaultSegmentColorChanged(QColor)), this, SLOT(setPreferenceDefaultSegmentColor(QColor)));
  connect(this->mp_Preferences, SIGNAL(defaultMarkerColorChanged(QColor)), this, SLOT(setPreferenceDefaultMarkerColor(QColor)));
  connect(this->mp_Preferences, SIGNAL(defaultMarkerRadiusChanged(double)), this, SLOT(setPreferenceDefaultMarkerRadius(double)));
  connect(this->mp_Preferences, SIGNAL(defaultMarkerTrajectoryLengthChanged(int)), this, SLOT(setPreferenceDefaultTrajectoryLength(int)));
  connect(this->mp_Preferences, SIGNAL(showForcePlatformAxesChanged(int)), this, SLOT(setPreferenceShowForcePlatformAxes(int)));
  connect(this->mp_Preferences, SIGNAL(showForcePlatformIndexChanged(int)), this, SLOT(setPreferenceShowForcePlatformIndex(int)));
  connect(this->mp_Preferences, SIGNAL(defaultForcePlateColorChanged(QColor)), this, SLOT(setPreferenceDefaultForcePlateColor(QColor)));
  connect(this->mp_Preferences, SIGNAL(defaultForceVectorColorChanged(QColor)), this, SLOT(setPreferenceDefaultForceVectorColor(QColor)));
  connect(this->mp_Preferences, SIGNAL(automaticCheckUpdateStateChanged(bool)), this, SLOT(setPreferenceAutomaticCheckUpdate(bool)));
#ifdef Q_OS_MAC
  connect(this->mp_Preferences, SIGNAL(userLayoutRemoved(int)), this, SLOT(removeUserLayout(int)));
  connect(this->mp_Preferences, SIGNAL(userLayoutLabelChanged(int, QString)), this, SLOT(relabelUserLayout(int, QString)));
  connect(this->mp_Preferences, SIGNAL(userLayoutDropped(int, int)), this, SLOT(updateDroppedUserLayouts(int, int)));
#else
  connect(this->mp_Preferences, SIGNAL(userLayoutsChanged(QList<QVariant>, int)), this, SLOT(updateUserLayouts(QList<QVariant>, int)));
#endif
};

MainWindow::~MainWindow()
{
  delete this->mp_Acquisition;
#ifdef Q_OS_MAC
  delete this->mp_MacMenuBar;
  delete this->mp_Preferences;
#endif
};

void MainWindow::closeEvent(QCloseEvent* event)
{
  if (this->isOkToContinue() && this->mp_ModelDock->isOkToContinue())
  {
    this->writeSettings();
#ifdef Q_OS_MAC
    this->mp_Preferences->hide();
#endif
    event->accept();
  }
  else
    event->ignore();
};

bool MainWindow::eventFilter(QObject* obj, QEvent* event)
{
  QLineEdit* lineEdit;
  QAbstractSpinBox* spinBox;
  if (((lineEdit = qobject_cast<QLineEdit*>(obj)) != 0) && (event->type() == QEvent::KeyPress))
  {
    QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
    if (keyEvent->key() == Qt::Key_Escape)
    {
      while (lineEdit->isUndoAvailable())
        lineEdit->undo();
    }
    else if (keyEvent->matches(QKeySequence::Undo) && !lineEdit->isUndoAvailable())
    {
      this->mp_UndoStack->undo();
    }
    else if (keyEvent->matches(QKeySequence::Redo) && !lineEdit->isRedoAvailable())
      this->mp_UndoStack->redo();
    return false;
  }
  else if (((spinBox = qobject_cast<QAbstractSpinBox*>(obj)) != 0) && (event->type() == QEvent::KeyPress))
  {
    QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
    if (keyEvent->matches(QKeySequence::Undo))
      this->mp_UndoStack->undo();
    if (keyEvent->matches(QKeySequence::Redo))
      this->mp_UndoStack->redo();
    else if (keyEvent->key() == Qt::Key_Space)
    {
      this->timeEventControler->togglePlayback();
      return true;
    }
    return false;
  }
  else if ((event->type() == QEvent::KeyPress) && this->timeEventControler->isEnabled())
  {
    QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
    if (keyEvent->matches(QKeySequence::MoveToPreviousChar))
    {
      this->timeEventControler->previousFrame();
      return true;
    }
    else if (keyEvent->matches(QKeySequence::MoveToNextChar))
    {
      this->timeEventControler->nextFrame();
      return true;
    }
    else if (keyEvent->key() == Qt::Key_Space)
    {
      this->timeEventControler->togglePlayback();
      return true;
    }
    else if (keyEvent->key() == Qt::Key_Escape)
    {
      this->mp_ModelDock->modelTree->clearSelection();
      return true;
    }
    else
      return false;
  }
  return QMainWindow::eventFilter(obj, event);
};

void MainWindow::checkSoftwareUpdateStartup()
{
#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
  #if defined(NDEBUG)
    QSettings settings;
    if (settings.value("Preferences/checkUpdateStartup", true).toBool())
      this->mp_UpdateChecker->check(true);
  #endif
#endif
};

void MainWindow::about()
{
  About aboutDlg(this);
  aboutDlg.exec();
};

void MainWindow::help()
{
#if defined(Q_OS_MAC)
  QDesktopServices::openUrl(QUrl("file:///" + QCoreApplication::applicationDirPath() + "/../Resources/MokkaHelp/index.html", QUrl::TolerantMode));
#elif defined(Q_OS_WIN)
  QDesktopServices::openUrl(QUrl("file:///" + QCoreApplication::applicationDirPath() + "/MokkaHelp/index.html", QUrl::TolerantMode));
#elif defined(Q_OS_LINUX)
  QMessageBox::information(this, "Mokka", "The help in not yet integrated with Linux. Need more informations about application's deployment under Linux.");
#endif
};

void MainWindow::visitBTKWebsite()
{
  QDesktopServices::openUrl(QUrl("http://b-tk.googlecode.com", QUrl::TolerantMode));
};

void MainWindow::setAcquisitionModified(int modified)
{
  if (modified == this->mp_AcquisitionUndoStack->cleanIndex())
  {
    this->actionSave->setEnabled(false);
    this->setWindowModified(false);
  }
  else
  {
    this->actionSave->setEnabled(true);
    this->setWindowModified(true);
    this->mp_MetadataDlg->warningIconLabel->setVisible(true);
    this->mp_MetadataDlg->warningMsgLabel->setVisible(true);
  }
};

void MainWindow::viewMetadata()
{
  this->mp_MetadataDlg->exec();
};

void MainWindow::setCurrentFile(const QString& filename) 
{ 
  this->setWindowModified(false); 
  QString shownName = tr("No File"); 
  if (!filename.isEmpty())
  { 
    shownName = QFileInfo(filename).fileName(); 
    this->m_RecentFiles.removeAll(filename); 
    this->m_RecentFiles.prepend(filename); 
  } 
  this->updateRecentFileActions(); 
  this->setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("Mokka"))); 
};

void MainWindow::updateRecentFileActions()
{
  QMutableStringListIterator i(this->m_RecentFiles); 
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
      this->mp_ActionRecentFiles[i]->setData(this->m_RecentFiles[i]); 
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
  if (this->isOkToContinue() && this->mp_ModelDock->isOkToContinue())
  { 
    QAction* pAction = qobject_cast<QAction*>(sender()); 
    if (pAction) 
      this->openFile(pAction->data().toString()); 
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
    // bool acquisitionModified = false;
    // for (int i = 0 ; i < this->mp_AcquisitionUndoStack->index() ; ++i)
    // {
    //   if (static_cast<const UndoCommand*>(this->mp_AcquisitionUndoStack->command(i))->commandType() == UndoCommand::AcquisitionCmd)
    //   {
    //     acquisitionModified = true;
    //     break;
    //   }
    // }
    // if (acquisitionModified)
    // {
      QMessageBox messageBox(this);
      messageBox.setIcon(QMessageBox::Information);
      messageBox.setText(tr("The document has been modified."));
      messageBox.setInformativeText(tr("Do you want to save your changes?"));
#ifdef Q_OS_MAC
      messageBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
      messageBox.setWindowFlags(Qt::Sheet);
      messageBox.setWindowModality(Qt::WindowModal);
      messageBox.setDefaultButton(QMessageBox::Save);
#else
      messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
      messageBox.setDefaultButton(QMessageBox::Yes);
#endif
      messageBox.setEscapeButton(QMessageBox::Cancel);
      switch(messageBox.exec())
      {
          case QMessageBox::Yes:
          case QMessageBox::Save:
              this->saveFile();
              break;
          case QMessageBox::Cancel:
              return false;
              break;
      }
    // }
  }
  return true; 
};

void MainWindow::playPausePlayback(bool paused)
{
  static bool previouslyActived = false;
  if (paused && (previouslyActived = this->timeEventControler->playbackStatus()))
    this->timeEventControler->stopPlayback();
  else if (!paused && !this->timeEventControler->playbackStatus() && previouslyActived)
    this->timeEventControler->startPlayback();
};

void MainWindow::updateUserLayoutActions()
{
  int inc = 0;
  for (int i = 0 ; i < maxUserLayouts ; ++i)
  { 
    if (inc < this->m_UserLayouts.count())
    {
      this->mp_ActionUserLayouts[i]->setText(this->m_UserLayouts[inc].toString().replace("&", "&&")); 
      this->mp_ActionUserLayouts[i]->setData(this->m_UserLayouts[inc+1]);
      this->mp_ActionUserLayouts[i]->setVisible(true);
    } 
    else
      this->mp_ActionUserLayouts[i]->setVisible(false); 
    inc += 2;
  } 
  this->mp_ActionSeparatorUserLayouts->setVisible(!this->m_UserLayouts.isEmpty());
  this->actionManageUserLayouts->setEnabled(!this->m_UserLayouts.isEmpty());
};

void MainWindow::restoreLayout()
{
  QAction* pAction = qobject_cast<QAction*>(sender()); 
  if (pAction)
  {
    LOG_INFO(tr("Restoring layout's views: ") + pAction->text());
    if (!this->multiView->restoreLayout(pAction->data().toByteArray()))
    {
      LOG_WARNING("Impossible to restore the previous layout.");
      this->restoreLayout3DOnly();
    }
    else
    {
      int index = -1;
      for (int i = 0 ; i < maxUserLayouts ; ++i)
      {
        if (this->mp_ActionUserLayouts[i] == pAction)
        {
          index = i;
          break;
        }
      }
      if (index != -1)
      {
        QSettings settings;
        settings.setValue("MainWindow/currentLayout", index + 3);
      }
      else
        LOG_WARNING("Impossible to save the restored layout: unknown index.");
    }
  }
};

void MainWindow::restoreLayout3DOnly()
{
  LOG_INFO(tr("Restoring layout's views: 3D Only"));
  this->multiView->restoreLayout3DOnly();
  QSettings settings;
  settings.setValue("MainWindow/currentLayout", 0);
};

void MainWindow::restoreLayout3DVerbose()
{
  LOG_INFO(tr("Restoring layout's views: 3D Verbose"));
  this->multiView->restoreLayout3DVerbose();
  QSettings settings;
  settings.setValue("MainWindow/currentLayout", 1);
};

void MainWindow::restoreLayout3DCharts()
{
  LOG_INFO(tr("Restoring layout's views: 3D And Charts"));
  this->multiView->restoreLayout3DCharts();
  QSettings settings;
  settings.setValue("MainWindow/currentLayout", 2);
};

void MainWindow::saveCurrentLayout()
{
  if (this->m_UserLayouts.count() >= maxUserLayouts * 2)
  {
    QMessageBox error(QMessageBox::Warning, "Layout manager", "The number of predefined layouts is reached.", QMessageBox::Ok , this);
#ifdef Q_OS_MAC
    error.setWindowFlags(Qt::Sheet);
    error.setWindowModality(Qt::WindowModal);
#endif
    error.setInformativeText("Use the layout manager to remove some of them.");
    error.exec();
  }
  else
  {
    NewLayoutDialog nld(&(this->m_UserLayouts), this);
    if (nld.exec())
    {
      this->m_UserLayouts.push_back(nld.layoutName());
      this->m_UserLayouts.push_back(this->multiView->saveLayout());
      this->updateUserLayoutActions();
      int index = this->m_UserLayouts.count() / 2 - 1;
      this->mp_ActionUserLayouts[index]->setChecked(true);
      QSettings settings;
      settings.setValue("MainWindow/userLayouts", this->m_UserLayouts);
      settings.setValue("MainWindow/currentLayout", index+3);
      this->mp_Preferences->refreshUserLayouts();
      LOG_INFO(tr("Saving the new user's layout: ") + nld.layoutName());
    }
  }
};

void MainWindow::manageUserLayouts()
{
  this->mp_Preferences->showLayoutsPreferences();
  this->showPreferences();
};

void MainWindow::play()
{
  this->timeEventControler->togglePlayback();
};

void MainWindow::loadConfiguration(const QString& filename)
{
  this->mp_ModelDock->loadConfiguration(filename);
};

void MainWindow::openFile()
{
  if (this->isOkToContinue())
  {
    QString filename = QFileDialog::getOpenFileName(this, "",
                         this->m_LastDirectory,
                         tr("Acquisition Files (*.anb *.anc *.ang *.c3d *.emf *.gr* *.mom *.pwr *.rah *.raw *.ric *rif *tdf *.trb *.trc *.xls);;"
                            "ANB Files (*.anb);;"
                            "ANC Files (*.anc);;"
                            "ANG Files (*.ang);;"
                            "C3D Files (*.c3d);;"
                            "EMF Ascension Files (*.emf);;"
                            "EMG Files (*.emg);;"
                            "GR* Files (*.gr*);;"
                            "MOM Files (*.mom);;"
                            "PWR Files (*.pwr);;"
                            "RAH Files (*.rah);;"
                            "RAW Files (*.raw);;"
                            "RIC Files (*.ric);;"
                            "RIF Files (*.rif);;"
                            "TDF Files (*.tdf);;"
                            "TRB Files (*.trb);;"
                            "TRC Files (*.trc);;"
                            "XLS OrthoTrak Files (*.xls)"));
    if (!filename.isEmpty())
      this->openFile(filename);
  }
};

void MainWindow::openFileDropped(const QString& filename)
{
  if (this->isOkToContinue() && this->mp_ModelDock->isOkToContinue())
    this->openFile(filename);
};

void MainWindow::openFile(const QString& filename)
{
  QFileInfo fI(filename);
  LOG_INFO(tr("Loading acquisition from file: ") + fI.fileName());
  ProgressWidget pw(this);
  pw.show();
  pw.setProgressValue(10);
  bool noOpenError = this->mp_Acquisition->load(filename);
  this->loadAcquisition(noOpenError, &pw);
  if (noOpenError)
  {
    this->setCurrentFile(filename);
    this->m_LastDirectory = fI.absolutePath();
  }
};

void MainWindow::loadAcquisition(bool noOpenError, ProgressWidget* pw)
{
  QApplication::setOverrideCursor(Qt::WaitCursor);
  if (!noOpenError)
  {
    pw->hide();
    QApplication::restoreOverrideCursor();
    QMessageBox error(QMessageBox::Warning, "File error", "Error occurred during the file reading.", QMessageBox::Ok , this);
#ifdef Q_OS_MAC
    error.setWindowFlags(Qt::Sheet);
    error.setWindowModality(Qt::WindowModal);
#endif
    error.setInformativeText("<nobr>Check the logger for more informations.</nobr>");
    error.exec();
    return;
  }
  this->reset();
  
  pw->setProgressValue(40);
  
  this->multiView->load();
  
  pw->setProgressValue(90);
  
  this->timeEventControler->load();
  this->mp_MetadataDlg->load(this->mp_Acquisition->btkAcquisition()->GetMetaData());
  this->mp_ModelDock->load();
  if (this->mp_Acquisition->hasPoints() || this->mp_Acquisition->hasAnalogs())
    this->mp_ModelDock->setVisible(true);
  if (this->mp_ModelDock->currentConfigurationIndex() == -1) // No configuration loaded
  {
    QSettings settings;
    bool defaultConfigurationUsed = settings.value("Preferences/defaultConfigurationUsed", false).toBool();
    QString defaultConfigurationPath = settings.value("Preferences/defaultConfigurationPath", "").toString();
    if (defaultConfigurationUsed && !defaultConfigurationPath.isEmpty())
      this->mp_ModelDock->loadConfiguration(defaultConfigurationPath);
  }
  
  pw->setProgressValue(100);

  QApplication::restoreOverrideCursor();
  
  this->timeEventControler->setEnabled(true);
  this->actionClose->setEnabled(true);
  this->actionViewMetadata->setEnabled(true);
  this->actionSave_As->setEnabled(true);
  this->actionImportVideo->setEnabled(true);
  this->menuExport->menuAction()->setEnabled(true);
};

void MainWindow::saveFile()
{
  if (!this->mp_Acquisition->fileName().isEmpty())
    this->saveFile(this->m_RecentFiles.first());
  else
    this->saveAsFile();
};

void MainWindow::saveAsFile()
{
  QString file;
  if (this->mp_Acquisition->fileName().isEmpty())
    file = this->m_LastDirectory + "/untitled.c3d";
  else
    file = this->m_RecentFiles.first();
  QString suffix = QFileInfo(file).suffix().toUpper();
  if ((suffix.compare("C3D") != 0) && (suffix.compare("ANB") != 0) && (suffix.compare("ANC") != 0) && (suffix.compare("TRC") != 0))
  {
    file += ".c3d";
    suffix = "C3D";
  }
  QString selectedFilter = suffix + " Files (*." + suffix.toLower() + ")";
  QString filename = QFileDialog::getSaveFileName(this, "",
                       file,
                       tr("ANB Files (*.anb);;"
                          "ANC Files (*.anc);;"
                          "C3D Files (*.c3d);;"
                          "TRC Files (*.trc)"),
                       &selectedFilter);
  if (!filename.isEmpty())
    this->saveFile(filename);
};

void MainWindow::saveFile(const QString& filename)
{
  QFileInfo fI(filename);
  LOG_INFO(tr("Saving acquisition to file: ") + fI.fileName());
  
  QApplication::setOverrideCursor(Qt::WaitCursor);
  
  QMap<int, QVariant> properties;
  this->setAcquisitionProperties(properties);
  if (!this->mp_Acquisition->save(filename,properties))
  {
    QApplication::restoreOverrideCursor();
    QMessageBox error(QMessageBox::Warning, "File error", "Error occurred during the file saving.", QMessageBox::Ok , this);
#ifdef Q_OS_MAC
    error.setWindowFlags(Qt::Sheet);
    error.setWindowModality(Qt::WindowModal);
#endif
    error.setInformativeText("<nobr>Check the logger for more informations.</nobr>");
    error.exec();
    return;
  }
  this->mp_AcquisitionUndoStack->setClean();
  this->setCurrentFile(filename);
  QApplication::restoreOverrideCursor();
  this->setWindowModified(false);
  this->m_LastDirectory = fI.absolutePath();
};

void MainWindow::closeFile()
{
  if (this->isOkToContinue() && this->mp_ModelDock->isOkToContinue())
  {
    LOG_INFO(tr("Closing acquisition."));
    this->reset();
    this->mp_ModelDock->setVisible(false);
    this->mp_FileInfoDock->reset();
    this->mp_Acquisition->clear();
  }
}

void MainWindow::importAssistant()
{
  this->mp_ModelDock->setEnabled(false);
  this->mp_ImportAssistant->clear(this->m_LastDirectory);
  this->mp_ImportAssistant->appendAcquisitionRadioButton->setEnabled(this->actionClose->isEnabled());
  bool accepted = this->mp_ImportAssistant->exec();
  this->mp_ModelDock->setEnabled(true); // If the dock is not enabled before the import, then its signals are not emited.
  if (accepted)
  {
    if (this->mp_ImportAssistant->newAcquisitionRadioButton->isChecked())
      this->mp_Acquisition->clear();
    this->importAcquisitions(this->mp_ImportAssistant->filenames(), this->mp_ImportAssistant->keepAllFrameRadioButton->isChecked());
  }
};

void MainWindow::importC3D()
{
  this->importAcquisition(tr("C3D Files (*.c3d)"));
};

void MainWindow::importTRC()
{
  this->importAcquisition(tr("TRC Files (*.trc)"));
};

void MainWindow::importTRB()
{
  this->importAcquisition(tr("TRB Files (*.trb)"));
};

void MainWindow::importANC()
{
  this->importAcquisition(tr("ANC Files (*.anc)"));
};

void MainWindow::importANB()
{
  this->importAcquisition(tr("ANB Files (*.anb)"));
};

void MainWindow::importForcePlatformCAL()
{
  this->importAcquisition(tr("CAL Files (*.cal)"));
};

void MainWindow::importOrthoTrakXLS()
{
  this->importAcquisition(tr("OrthoTrak XLS Files (*.xls)"));
};

void MainWindow::importTDF()
{
  this->importAcquisition(tr("TDF Files (*.tdf)"));
};

void MainWindow::importRAH()
{
  this->importAcquisition(tr("RAH Files (*.rah)"));
};

void MainWindow::importRAW()
{
  this->importAcquisition(tr("RAW Files (*.raw)"));
};

void MainWindow::importRIC()
{
  this->importAcquisition(tr("RIC Files (*.ric)"));
};

void MainWindow::importRIF()
{
  this->importAcquisition(tr("RIF Files (*.rif)"));
};

void MainWindow::importGRx()
{
#if 1
  this->importAcquisition(tr("GR* Files (*.gr*)"));
#else
  QStringList filenames = QFileDialog::getOpenFileNames(this, "",
                            this->m_LastDirectory,
                            tr("GR* Files (*.gr*)"));
  this->importAcquisitions(filenames);
#endif
};

void MainWindow::importANG()
{
  this->importAcquisition(tr("ANG Files (*.ang)"));
};

void MainWindow::importEMG()
{
  this->importAcquisition(tr("EMG Files (*.emg)"));
};

void MainWindow::importMOM()
{
  this->importAcquisition(tr("MOM Files (*.mom)"));
};

void MainWindow::importPWR()
{
  this->importAcquisition(tr("PWR Files (*.pwr)"));
};

void MainWindow::importEMF()
{
  this->importAcquisition(tr("EMF Ascension Files (*.emf)"));
};

void MainWindow::importAcquisition(const QString& filter)
{
  QString filename = QFileDialog::getOpenFileName(this, "",
                       this->m_LastDirectory,
                       filter);
  if (!filename.isEmpty())
    this->importAcquisitions(QStringList(filename));
};

void MainWindow::importAcquisitions(const QStringList& filenames, bool allFramesKept)
{
  LOG_INFO(tr("Importing acquisition(s)."));
  if (!filenames.isEmpty())
  {
    QString title = this->windowTitle();
    ProgressWidget pw(this);
    pw.show();
    pw.setProgressValue(10);
    bool noImportError = this->mp_Acquisition->importFrom(filenames, allFramesKept);
    this->loadAcquisition(noImportError, &pw);
    this->setWindowTitle(title);
    pw.hide();
    if (!noImportError)
      return;
    this->setWindowModified(true);
    this->actionSave->setEnabled(true);
    this->m_LastDirectory = QFileInfo(filenames.last()).absolutePath();
  }
}

void MainWindow::importVideos()
{
  QStringList filenames = QFileDialog::getOpenFileNames(this, "",
                            this->m_LastDirectory,
                            tr("Video Files (*.avi *.mov *.mpeg *.mpg *.ogg *.wmv);;"
                               "All Files (*);;"
                               "AVI Files (*.avi);;"
                               "MOV Files (*.mov);;"
                               "MPEG Files (*.mpeg *.mpg);;"
                               "OGG Files (*.ogg);;"
                               "WMV Files (*.wmv)"));
  if (!filenames.isEmpty())
  {
    this->mp_Acquisition->importVideos(filenames);
    this->setWindowModified(true);
  }
};

void MainWindow::exportC3D()
{
  this->exportAcquisition(tr("C3D Files (*.c3d)"));
};

void MainWindow::exportTRC()
{
  this->exportAcquisition(tr("TRC Files (*.trc)"));
};

void MainWindow::exportANB()
{
  this->exportAcquisition(tr("ANB Files (*.anb)"));
};

void MainWindow::exportANC()
{
  this->exportAcquisition(tr("ANC Files (*.anc)"));
};

void MainWindow::exportCAL()
{
  this->exportAcquisition(tr("CAL Files (*.cal)"));
};

void MainWindow::showPreferences()
{
#ifdef Q_OS_MAC
  this->mp_Preferences->hide(); // Force the preferences to go back to the top.
  this->mp_Preferences->show();
#else
  // Update the data for the user layouts
  int userLayoutIndex = -1;
  for (int i = 0 ; i < maxUserLayouts ; ++i)
  {
    if (this->mp_ActionUserLayouts[i]->isChecked())
    {
      userLayoutIndex = i;
      break;
    }
  }
  this->mp_Preferences->setPreference(Preferences::UserLayoutIndex, userLayoutIndex);
  this->mp_Preferences->setPreference(Preferences::UserLayouts, this->m_UserLayouts);
  if (this->mp_Preferences->exec())
    this->mp_Preferences->saveSettings();
  else
    this->mp_Preferences->resetSettings();
#endif
};

void MainWindow::selectAll()
{
  QWidget* w = QApplication::focusWidget();
  LoggerWidget* logger = qobject_cast<LoggerWidget*>(w);
  if (w != 0)
    logger->selectAll();
};

void MainWindow::copy()
{
  QWidget* w = QApplication::focusWidget();
  LoggerWidget* logger = qobject_cast<LoggerWidget*>(w);
  if (w != 0)
    logger->copySelectedItemsToClipboard();
};

void MainWindow::exportAcquisition(const QString& filter)
{
  QString filename = QFileDialog::getSaveFileName(this, "",
                       this->m_LastDirectory,
                       filter);
  if (!filename.isEmpty())
  {
    LOG_INFO(tr("Exporting acquisition to file: ") + QFileInfo(filename).fileName());
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QMap<int, QVariant> properties;
    this->setAcquisitionProperties(properties);
    if (!this->mp_Acquisition->exportTo(filename, properties, this->timeEventControler->leftBound(), this->timeEventControler->rightBound()))
    {
      QApplication::restoreOverrideCursor();
      QMessageBox error(QMessageBox::Warning, "File error", "Error occurred during the file exporting.", QMessageBox::Ok , this);
#ifdef Q_OS_MAC
      error.setWindowFlags(Qt::Sheet);
      error.setWindowModality(Qt::WindowModal);
#endif
      error.setInformativeText("<nobr>Check the logger for more informations.</nobr>");
      error.exec();
      return;
    }
    this->m_LastDirectory = QFileInfo(filename).absolutePath();
    QApplication::restoreOverrideCursor();
  }
};

void MainWindow::reset()
{
  this->mp_AcquisitionUndoStack->clear();
  this->mp_MarkerConfigurationUndoStack->clear();
  this->actionClose->setEnabled(false);
  this->actionViewMetadata->setEnabled(false);
  this->actionSave->setEnabled(false);
  this->actionSave_As->setEnabled(false);
  this->actionImportVideo->setEnabled(false);
  this->menuExport->menuAction()->setEnabled(false);
  this->setCurrentFile("");
  // Time & Event Controler
  this->timeEventControler->reset();
  this->timeEventControler->setEnabled(false);
  // Model dock
  this->mp_ModelDock->reset();
  // Metadata
  this->mp_MetadataDlg->reset();
  // Multivew
  this->multiView->clear();
};

void MainWindow::updateSelectedMarkersRadius(double r)
{
  QList<int> ids = this->mp_ModelDock->selectedMarkers();
  for (int i = 0 ; i < ids.count() ; ++i)
    this->multiView->setMarkerRadius(ids[i], r);
  this->multiView->updateDisplay();
};

void MainWindow::updateSelectedVideosDelay(double d)
{
  QList<int> ids = this->mp_ModelDock->selectedVideos();
  for (int i = 0 ; i < ids.count() ; ++i)
    this->multiView->setVideoDelay(ids[i], d);
  this->multiView->updateDisplay();
};

void MainWindow::selectPickedMarker(int id)
{
  QList<int> ids;
  ids << id;
  this->mp_ModelDock->selectMarkers(ids);
};

void MainWindow::togglePickedMarker(int id)
{
  QList<int> ids = this->mp_ModelDock->selectedMarkers();
  int idx = ids.indexOf(id);
  if (idx == -1)
    ids << id;
  else
    ids.removeAt(idx);
  this->mp_ModelDock->selectMarkers(ids);
};

void MainWindow::selectSelectedMarkers(const QList<int>& ids)
{
  QList<int> selectedIds = this->mp_ModelDock->selectedMarkers();
  for (QList<int>::const_iterator it = ids.begin() ; it != ids.end() ; ++it)
  {
    int idx = selectedIds.indexOf(*it);
    if (idx == -1)
      selectedIds << *it;
    else
      selectedIds.removeAt(idx);
  }
  this->mp_ModelDock->selectMarkers(selectedIds);
};

void MainWindow::toggleMarkerTrajectory(int id)
{
  QList<int> ids = this->mp_ModelDock->tailedMarkers();
  int idx = ids.indexOf(id);
  if (idx == -1)
    ids << id;
  else
    ids.removeAt(idx);
  this->mp_ModelDock->setTrackedMarkers(ids);
};

void MainWindow::toggleEditActions(QWidget* old, QWidget* now)
{
  Q_UNUSED(old);
  if (now != NULL)
  {
    if (qobject_cast<LoggerWidget*>(now) != 0)
    {
      this->actionCopy->setEnabled(true);
      this->actionSelect_All->setEnabled(true);
    }
    else
    {
      bool e = false;
      if ((now->inherits("QLineEdit")
           || now->inherits("QAbstractSpinBox")
           || now->inherits("QTextEdit")))
        e = true;
      this->actionCut->setEnabled(e);
      this->actionCopy->setEnabled(e);
      this->actionPaste->setEnabled(e);
      this->actionSelect_All->setEnabled(e);
    }
  }
};

void MainWindow::modelDockLocationChanged(Qt::DockWidgetArea area)
{
  QSettings settings;
  settings.beginGroup("MarkersDock"); // For compatibilty
  settings.setValue("area", area);
  settings.endGroup();
};

void MainWindow::setPointLabel(int id, const QString& label)
{
  // TODO: Group two undo commands to modify the acquisition and the configuration
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new EditPointLabel(this->mp_Acquisition, id, label)));
};

void MainWindow::setMarkersRadius(const QVector<int>& ids, double radius)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_MarkerConfigurationUndoStack, new EditMarkersRadius(this->mp_Acquisition, ids, radius)));
};

void MainWindow::setMarkersColor(const QVector<int>& ids, const QColor& color)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_MarkerConfigurationUndoStack, new EditMarkersColor(this->mp_Acquisition, ids, color)));
};

void MainWindow::setPointsDescription(const QVector<int>& ids, const QString& desc)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new EditPointDescription(this->mp_Acquisition, ids, desc)));
};

void MainWindow::removePoints(const QList<int>& ids)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new RemovePoints(this->mp_Acquisition, ids)));
};

void MainWindow::setAnalogLabel(int id, const QString& label)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new EditAnalogLabel(this->mp_Acquisition, id, label)));
};

void MainWindow::setAnalogsDescription(const QVector<int>& ids, const QString& desc)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new EditAnalogDescription(this->mp_Acquisition, ids, desc)));
};

void MainWindow::setAnalogsUnit(const QVector<int>& ids, const QString& unit)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new EditAnalogsUnit(this->mp_Acquisition, ids, unit)));
};

void MainWindow::setAnalogsGain(const QVector<int>& ids, Analog::Gain gain)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new EditAnalogsGain(this->mp_Acquisition, ids, gain)));
};

void MainWindow::setAnalogsOffset(const QVector<int>& ids, int offset)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new EditAnalogsOffset(this->mp_Acquisition, ids, offset)));
};

void MainWindow::setAnalogsScale(const QVector<int>& ids, double scale)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new EditAnalogsScale(this->mp_Acquisition, ids, scale)));
};

void MainWindow::removeAnalogs(const QList<int>& ids)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new RemoveAnalogs(this->mp_Acquisition, ids)));
};

void MainWindow::setSegmentLabel(int id, const QString& label)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_MarkerConfigurationUndoStack, new EditSegmentLabel(this->mp_Model, id, label)));
};

void MainWindow::setSegmentsColor(const QVector<int>& ids, const QColor& color)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_MarkerConfigurationUndoStack, new EditSegmentsColor(this->mp_Model, ids, color)));
};

void MainWindow::setSegmentsDescription(const QVector<int>& ids, QString desc)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_MarkerConfigurationUndoStack, new EditSegmentsDescription(this->mp_Model, ids, desc)));
};

void MainWindow::setSegmentLinks(int id, const QVector<int>& markerIds, const QVector< QPair<int,int> >& links)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_MarkerConfigurationUndoStack, new EditSegmentLinks(this->mp_Model, id, markerIds, links)));
};

void MainWindow::removeSegments(const QList<int>& ids)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_MarkerConfigurationUndoStack, new RemoveSegments(this->mp_Model, ids)));
};

void MainWindow::insertSegment(Segment* seg)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_MarkerConfigurationUndoStack, new InsertSegment(this->mp_Model, seg)));
};

void MainWindow::setVideosDelay(const QVector<int>& ids, qint64 delay)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new EditVideosDelay(this->mp_Acquisition, ids, delay)));
};

void MainWindow::removeVideos(const QList<int>& ids)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new RemoveVideos(this->mp_Acquisition, ids)));
};

void MainWindow::setEventFrame(int id, int frame)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new EditEventFrame(this->mp_Acquisition, id, frame)));
};

void MainWindow::setEvents(QList<int> ids, QList<Event*> events)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new SetEvents(this->mp_Acquisition, ids, events)));
};

void MainWindow::removeEvents(const QList<int>& ids)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new RemoveEvents(this->mp_Acquisition, ids)));
};

void MainWindow::setRegionOfInterest(int lf,int ff)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new EditRegionOfInterest(this->mp_Acquisition, lf, ff)));
};

void MainWindow::insertEvent(Event* e)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new InsertEvent(this->mp_Acquisition, e)));
};

void MainWindow::reframeAcquisition(int ff)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new ReframeAcquisition(this->mp_Acquisition, ff)));
};

void MainWindow::setPreferenceUseDefaultConfiguration(bool isUsed)
{
  QSettings settings;
  settings.setValue("Preferences/defaultConfigurationUsed", isUsed);
  QString path = settings.value("Preferences/defaultConfigurationPath").toString();
  if (isUsed && !path.isEmpty() && (this->mp_ModelDock->modelConfigurationComboBox->currentIndex() == -1))
    this->mp_ModelDock->loadConfiguration(path);
};

void MainWindow::setPreferenceDefaultConfigurationPath(const QString& path)
{
  QSettings settings;
  settings.setValue("Preferences/defaultConfigurationPath", path);
  
  bool isUsed = settings.value("Preferences/defaultConfigurationUsed").toBool();
  if (isUsed && !path.isEmpty() && (this->mp_ModelDock->modelConfigurationComboBox->currentIndex() == -1))
    this->mp_ModelDock->loadConfiguration(path);
};

void MainWindow::setPreferenceUseEventEditorWhenInserting(bool isUsed)
{
  QSettings settings;
  settings.setValue("Preferences/openEventEditorWhenInserting", isUsed);
  this->timeEventControler->setOpenEditorWhenInsertingEventFlag(isUsed);
};

void MainWindow::setPreferenceDefaultOrientation(int index)
{
  QSettings settings;
  settings.setValue("Preferences/defaultPlaneOrientation", index);
  this->multiView->setDefaultGroundOrientation(index);
};

void MainWindow::setPreferenceDefaultSegmentColor(const QColor& color)
{
  QSettings settings;
  settings.setValue("Preferences/defaultSegmentColor", color);
  
  QTreeWidgetItem* segmentsRoot = this->mp_ModelDock->modelTree->topLevelItem(ModelDockWidget::SegmentsItem);
  QVector<int> ids;
  for (int i = 0 ; i < segmentsRoot->childCount() ; ++i)
  {
    int id = segmentsRoot->child(i)->data(0,SegmentId).toInt();
    if (this->multiView->segmentColorIndex(id) == 0)
      ids.push_back(id);
  }
  QVector<QColor> colors(ids.count(), color);
  this->mp_Model->blockSignals(true);
  this->mp_Model->setSegmentsColor(ids, colors);
  this->mp_Model->blockSignals(false);
  this->mp_ModelDock->setSegmentsColor(ids, colors);
  this->multiView->setDefaultSegmentColor(color);
};

void MainWindow::setPreferenceDefaultMarkerColor(const QColor& color)
{
  QSettings settings;
  settings.setValue("Preferences/defaultMarkerColor", color);
  
  QTreeWidgetItem* markersRoot = this->mp_ModelDock->modelTree->topLevelItem(ModelDockWidget::MarkersItem);
  QVector<int> ids;
  for (int i = 0 ; i < markersRoot->childCount() ; ++i)
  {
    int id = markersRoot->child(i)->data(0,PointId).toInt();
    if (this->multiView->markerColorIndex(id) == 0)
      ids.push_back(id);
  }
  QVector<QColor> colors(ids.count(), color);
  this->mp_Acquisition->blockSignals(true);
  this->mp_Acquisition->setMarkersColor(ids, colors);
  this->mp_Acquisition->blockSignals(false);
  this->mp_ModelDock->setMarkersColor(ids, colors);
  this->multiView->setDefaultMarkerColor(color);
};

void MainWindow::setPreferenceDefaultMarkerRadius(double radius)
{
  QSettings settings;
  settings.setValue("Preferences/defaultMarkerRadius", radius);
  this->multiView->setDefaultMarkerRadius(radius);
};

void MainWindow::setPreferenceDefaultTrajectoryLength(int index)
{
  QSettings settings;
  settings.setValue("Preferences/defaultMarkerTrajectoryLength", index);
  this->multiView->setMarkerTrajectoryLength(index);
};

void MainWindow::setPreferenceShowForcePlatformAxes(int index)
{
  QSettings settings;
  settings.setValue("Preferences/showForcePlatformAxes", index);
  this->multiView->showForcePlatformAxes(index == 0);
};

void MainWindow::setPreferenceShowForcePlatformIndex(int index)
{
  QSettings settings;
  settings.setValue("Preferences/showForcePlatformIndex", index);
  this->multiView->showForcePlatformIndex(index == 0);
};

void MainWindow::setPreferenceDefaultForcePlateColor(const QColor& color)
{
  QSettings settings;
  settings.setValue("Preferences/defaultForcePlateColor", color);
  this->multiView->setForcePlatformColor(color);
};

void MainWindow::setPreferenceDefaultForceVectorColor(const QColor& color)
{
  QSettings settings;
  settings.setValue("Preferences/defaultForceVectorColor", color);
  this->multiView->setForceVectorColor(color);
};

void MainWindow::setPreferenceAutomaticCheckUpdate(bool isChecked)
{
  QSettings settings;
  settings.setValue("Preferences/checkUpdateStartup", isChecked);
};

void MainWindow::removeUserLayout(int index)
{
  this->m_UserLayouts.removeAt(index*2); // Name
  this->m_UserLayouts.removeAt(index*2); // Data
  QSettings settings;
  settings.setValue("MainWindow/userLayouts", this->m_UserLayouts);
  int layoutIndex = settings.value("MainWindow/currentLayout", -1).toInt();
  if ((layoutIndex != -1) && (layoutIndex == index + 3))
  {
    settings.setValue("MainWindow/currentLayout", 0); // Reset to the default view.
    this->actionLayout3DOnly->setChecked(true);
  }
  this->updateUserLayoutActions();
};

void MainWindow::relabelUserLayout(int index, const QString& label)
{
  this->m_UserLayouts[index*2] = label;
  this->updateUserLayoutActions();
  QSettings settings;
  settings.setValue("MainWindow/userLayouts", this->m_UserLayouts);
};

void MainWindow::updateDroppedUserLayouts(int newRow, int oldRow)
{
  QVariant name = this->m_UserLayouts.takeAt(oldRow*2);
  QVariant data = this->m_UserLayouts.takeAt(oldRow*2);
  this->m_UserLayouts.insert(newRow*2, data);
  this->m_UserLayouts.insert(newRow*2, name);
  int checkedRow = -1;
  if (this->mp_ActionUserLayouts[oldRow]->isChecked())
  {
    checkedRow = newRow;
    this->mp_ActionUserLayouts[checkedRow]->setChecked(true);
  }
  else
  {
    for (int i = 0 ; i < maxUserLayouts ; ++i)
    {
      if (this->mp_ActionUserLayouts[i]->isChecked())
      {
        checkedRow = i;
        break;
      }
    }
    if (checkedRow != -1)
    {
      if ((newRow > oldRow) && (oldRow < checkedRow))
      {
        checkedRow -= 1;
        this->mp_ActionUserLayouts[checkedRow]->setChecked(true);
      }
      else if ((newRow < oldRow) && (newRow < checkedRow))
      {
        checkedRow += 1;
        this->mp_ActionUserLayouts[checkedRow]->setChecked(true);
      }
    }
  }
  this->updateUserLayoutActions();
  QSettings settings;
  if (checkedRow != -1)
    settings.setValue("MainWindow/currentLayout", checkedRow+3);
  settings.setValue("MainWindow/userLayouts", this->m_UserLayouts);
};

void MainWindow::updateUserLayouts(const QList<QVariant>& layouts, int index)
{
  this->m_UserLayouts = layouts;
  this->updateUserLayoutActions();
  QSettings settings;
  if (index != -1)
  {
    this->mp_ActionUserLayouts[index]->setChecked(true);
    settings.setValue("MainWindow/currentLayout", index+3);
  }
  settings.setValue("MainWindow/userLayouts", this->m_UserLayouts);
};

void MainWindow::setAcquisitionProperties(QMap<int, QVariant>& properties)
{
  properties.clear();
  QString strProp = this->multiView->groundNormalAsString();
  properties.insert(Acquisition::yScreen, strProp);
  if (strProp.compare("+X") == 0)
    strProp = "+Y";
  else
    strProp = "+X";
  properties.insert(Acquisition::xScreen, strProp);
};

void MainWindow::readSettings()
{
  QSettings settings;
  // MainWidow
  settings.beginGroup("MainWindow");
  // - Size & position
  this->move(settings.value("pos", QPoint(100, 100)).toPoint());
  this->resize(settings.value("size", QSize(1024, 768)).toSize());
  if (settings.value("maximized", false).toBool())
    this->showMaximized();
  else
    this->showNormal();
  // - Recent files submenu
  this->m_RecentFiles = settings.value("recentFiles").toStringList(); 
  this->updateRecentFileActions();
  // - Last directory
  this->m_LastDirectory = settings.value("lastDirectory", ".").toString();
  // - Layout
  this->m_UserLayouts = settings.value("userLayouts").toList();
  this->updateUserLayoutActions();
  this->mp_Preferences->refreshUserLayouts();
  int layoutIndex = settings.value("currentLayout", 0).toInt();
  switch (layoutIndex)
  {
  case 0:
    this->actionLayout3DOnly->trigger();
    break;
  case 1:
    this->actionLayout3DVerbose->trigger();
    break;
  case 2:
    this->actionLayout3DCharts->trigger();
    break;
  default:
    if (layoutIndex-3 < maxUserLayouts)
      this->mp_ActionUserLayouts[layoutIndex-3]->trigger();
  }
  settings.endGroup();
  
  // MarkersDock
  settings.beginGroup("MarkersDock"); // Should be ModelDock but for compatibility...
  this->mp_ModelDock->setFloating(settings.value("floating", false).toBool());
  this->mp_ModelDock->move(settings.value("pos", this->mp_ModelDock->pos()).toPoint());
  this->mp_ModelDock->resize(settings.value("size", this->mp_ModelDock->size()).toSize());
  if (!this->mp_ModelDock->isFloating())
    this->addDockWidget(static_cast<Qt::DockWidgetArea>(settings.value("area", Qt::RightDockWidgetArea).toInt()), this->mp_ModelDock);
  QStringList recentVisualConfigName = settings.value("recentVisualConfigName").toStringList();
  QStringList recentVisualConfigFilename = settings.value("recentVisualConfigFilename").toStringList(); 
  this->mp_ModelDock->setVisualConfigurations(recentVisualConfigName, recentVisualConfigFilename);
  this->mp_ModelDock->setRecentColor(0, settings.value("recentColor1", QColor()).value<QColor>());
  this->mp_ModelDock->setRecentColor(1, settings.value("recentColor2", QColor()).value<QColor>());
  this->mp_ModelDock->setRecentColor(2, settings.value("recentColor3", QColor()).value<QColor>());
  this->mp_ModelDock->setRecentColor(3, settings.value("recentColor4", QColor()).value<QColor>());
  this->mp_ModelDock->setRecentColor(4, settings.value("recentColor5", QColor()).value<QColor>());
  settings.endGroup();

  // Preferences
  settings.beginGroup("Preferences");
  bool defaultConfigurationUsed = settings.value("defaultConfigurationUsed", false).toBool();
  QString defaultConfigurationPath = settings.value("defaultConfigurationPath", "").toString();
  bool openEventEditorWhenInserting = settings.value("openEventEditorWhenInserting", true).toBool();
  int defaultPlaneOrientation = settings.value("defaultPlaneOrientation", 0).toInt();
  QColor defaultSegmentColor = settings.value("defaultSegmentColor", QColor(255,255,255)).value<QColor>();
  QColor defaultMarkerColor = settings.value("defaultMarkerColor", QColor(255,255,255)).value<QColor>();
  double defaultMarkerRadius = settings.value("defaultMarkerRadius", 8.0).toDouble();
  int defaultMarkerTrajectoryLength = settings.value("defaultMarkerTrajectoryLength", 3).toInt();
  int showForcePlatformAxes = settings.value("showForcePlatformAxes", 0).toInt();
  int showForcePlatformIndex = settings.value("showForcePlatformIndex", 0).toInt();
  QColor defaultForcePlateColor = settings.value("defaultForcePlateColor", QColor(255,255,0)).value<QColor>();
  QColor defaultForceVectorColor = settings.value("defaultForceVectorColor", QColor(255,255,0)).value<QColor>();
  bool checkUpdateStartup = settings.value("checkUpdateStartup", true).toBool();
  settings.endGroup();
  this->mp_Preferences->lastDirectory = this->m_LastDirectory;
  this->mp_Preferences->defaultConfigurationCheckBox->setChecked(defaultConfigurationUsed);
  this->mp_Preferences->defaultConfigurationLineEdit->setText(defaultConfigurationPath);
  this->mp_Preferences->openEventEditorCheckBox->setChecked(openEventEditorWhenInserting);
  this->mp_Preferences->defaultPlaneOrientationComboBox->setCurrentIndex(defaultPlaneOrientation);
  colorizeButton(this->mp_Preferences->defaultSegmentColorButton, defaultSegmentColor);
  colorizeButton(this->mp_Preferences->defaultMarkerColorButton, defaultMarkerColor);
  this->mp_Preferences->defaultMarkerRadiusSpinBox->setValue(defaultMarkerRadius);
  this->mp_Preferences->defaultMarkerTrajectoryLengthComboBox->setCurrentIndex(defaultMarkerTrajectoryLength);
  this->mp_Preferences->showForcePlatformAxesComboBox->setCurrentIndex(showForcePlatformAxes);
  this->mp_Preferences->showForcePlatformIndexComboBox->setCurrentIndex(showForcePlatformIndex);
  colorizeButton(this->mp_Preferences->defaultForcePlateColorButton, defaultForcePlateColor);
  colorizeButton(this->mp_Preferences->defaultForceVectorColorButton, defaultForceVectorColor);
  this->mp_Preferences->automaticCheckUpdateCheckBox->setChecked(checkUpdateStartup);

#ifdef Q_OS_WIN
  this->mp_Preferences->setPreference(Preferences::DefaultConfigurationUse, defaultConfigurationUsed);
  this->mp_Preferences->setPreference(Preferences::DefaultConfigurationPath, defaultConfigurationPath);
  this->mp_Preferences->setPreference(Preferences::EventEditorWhenInserting, openEventEditorWhenInserting);
  this->mp_Preferences->setPreference(Preferences::DefaultGroundOrientation, defaultPlaneOrientation);
  this->mp_Preferences->setPreference(Preferences::DefaultSegmentColor, defaultSegmentColor);
  this->mp_Preferences->setPreference(Preferences::DefaultMarkerColor, defaultMarkerColor);
  this->mp_Preferences->setPreference(Preferences::DefaultMarkerRadius, defaultMarkerRadius);
  this->mp_Preferences->setPreference(Preferences::DefaultTrajectoryLength, defaultMarkerTrajectoryLength);
  this->mp_Preferences->setPreference(Preferences::ForcePlatformAxesDisplay, showForcePlatformAxes);
  this->mp_Preferences->setPreference(Preferences::ForcePlatformIndexDisplay, showForcePlatformIndex);
  this->mp_Preferences->setPreference(Preferences::DefaultForcePlateColor, defaultForcePlateColor);
  this->mp_Preferences->setPreference(Preferences::DefaultForceVectorColor, defaultForceVectorColor);
  this->mp_Preferences->setPreference(Preferences::UserLayoutIndex, layoutIndex);
  this->mp_Preferences->setPreference(Preferences::UserLayouts, this->m_UserLayouts);
  this->mp_Preferences->setPreference(Preferences::AutomaticCheckUpdateUse, checkUpdateStartup);
#endif
  
  if (defaultConfigurationUsed && !defaultConfigurationPath.isEmpty())
    this->mp_ModelDock->loadConfiguration(defaultConfigurationPath);
  this->timeEventControler->setOpenEditorWhenInsertingEventFlag(openEventEditorWhenInserting);
  this->multiView->setDefaultGroundOrientation(defaultPlaneOrientation);
  this->multiView->setDefaultSegmentColor(defaultSegmentColor);
  this->multiView->setDefaultMarkerColor(defaultMarkerColor);
  this->multiView->setDefaultMarkerRadius(defaultMarkerRadius);
  this->multiView->setMarkerTrajectoryLength(defaultMarkerRadius);
  this->multiView->showForcePlatformAxes(showForcePlatformAxes == 0);
  this->multiView->showForcePlatformIndex(showForcePlatformIndex == 0);
  this->multiView->setForcePlatformColor(defaultForcePlateColor);
  this->multiView->setForceVectorColor(defaultForceVectorColor);
};

void MainWindow::writeSettings()
{
  QSettings settings;
  // MainWindow
  // - Size & position 
  settings.beginGroup("MainWindow");
  QDesktopWidget* d = QApplication::desktop();
  if (this->isMaximized() && (this->width() == d->width()))
    settings.setValue("maximized", true);
  else
  {
    settings.setValue("maximized", false);
    settings.setValue("pos", this->pos());
    settings.setValue("size", this->size());
  }
  // - Recent files submenu
  settings.setValue("recentFiles", this->m_RecentFiles);
  // - Last directory
  settings.setValue("lastDirectory", this->m_LastDirectory);
  settings.endGroup();
  // MarkersDock
  settings.beginGroup("MarkersDock"); // Should be ModelDock but for compatibility...
  //settings.setValue("visible", this->mp_ModelDock->isVisible());
  settings.setValue("floating", this->mp_ModelDock->isFloating());
  settings.setValue("pos", this->mp_ModelDock->pos());
  settings.setValue("size", this->mp_ModelDock->size());
  //settings.setValue("area", this->mp_ModelDock->area());
  QStringList recentVisualConfigFilename; 
  QStringList recentVisualConfigName;
  this->mp_ModelDock->visualConfigurations(recentVisualConfigName, recentVisualConfigFilename);
  settings.setValue("recentVisualConfigFilename", recentVisualConfigFilename);
  settings.setValue("recentVisualConfigName", recentVisualConfigName);
  settings.setValue("recentColor1", this->mp_ModelDock->recentColor(0));
  settings.setValue("recentColor2", this->mp_ModelDock->recentColor(1));
  settings.setValue("recentColor3", this->mp_ModelDock->recentColor(2));
  settings.setValue("recentColor4", this->mp_ModelDock->recentColor(3));
  settings.setValue("recentColor5", this->mp_ModelDock->recentColor(4));
  settings.endGroup();
};
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
#include "ChartCycleSettingsManager.h"
#include "ChartDialog.h"
#include "CompositeView.h"
#include "ExportASCIIDialog.h"
#include "ExportSTLDialog.h"
#include "ExportImageSeriesDialog.h"
#include "FileInfoDockWidget.h"
#include "ImportAssistantDialog.h"
#include "LoggerMessage.h"
#include "LoggerWidget.h"
#include "Metadata.h"
#include "ModelDockWidget.h"
#include "ProgressWidget.h"
#include "UndoCommands.h"
#include "UpdateManager.h"
#include "Preferences.h"
#include "TimeEventFunctors.h"
#include "NewLayoutDialog.h"
#include "NewSegmentDialog.h"
#include "UserDefined.h"
#include "Viz3DWidget.h"

#include "ToolsData.h"
#include "ToolsManager.h"

#include <btkASCIIFileWriter.h>
#include <btkMultiSTLFileWriter.h>
#include <btkMetaDataUtils.h>

#include <QFileDialog>
#include <QFileInfo>
#include <QCloseEvent>
#include <QMessageBox>
#include <QSettings>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QUndoStack>

MainWindow::MainWindow(QWidget* parent)
:QMainWindow(parent), m_LastDirectory("."), m_RecentFiles(), m_UserLayouts(), m_ToolCharts()
{
  // Members
  this->mp_Acquisition = new Acquisition(this);
  this->mp_Model = new Model(this);
  this->mp_MetadataDlg = new Metadata(this);
  this->mp_ModelDock = new ModelDockWidget(this);
  this->mp_FileInfoDock = new FileInfoDockWidget(this);
  this->mp_ImportAssistant = new ImportAssistantDialog(this);
  this->mp_SegmentEditor = 0;
  this->mp_Updater = new UpdateManager(MOKKA_VERSION_STRING,
                                       "http://b-tk.googlecode.com/svn/doc/Mokka/latestMokka",
                                       ":/Resources/Images/Mokka_128.png", this);
  this->mp_ChartCycleSettingsManager = new ChartCycleSettingsManager(this);
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
  QAction* actionModelDockView = this->mp_ModelDock->toggleViewAction();
#ifdef Q_OS_MAC
  actionModelDockView->setShortcut(QKeySequence(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_M));
#else
  actionModelDockView->setShortcut(QKeySequence(Qt::ControlModifier + Qt::Key_M));
#endif
  QAction* actionInformationsDockView = this->mp_FileInfoDock->toggleViewAction();
  actionInformationsDockView->setShortcut(QKeySequence(Qt::ControlModifier + Qt::Key_I));
  // Menu File
  this->menuExport->menuAction()->setEnabled(false);
  // Menu View
  this->menuView->addAction(actionModelDockView);
  this->menuView->addAction(actionInformationsDockView);
  this->menuView->addAction(this->actionViewMetadata);
  // Menu Settings
  this->menuVisual_Configuration->addAction(this->mp_ModelDock->deselectConfigurationAction());
  this->menuVisual_Configuration->addAction(this->mp_ModelDock->clearConfigurationsAction());
  this->menuSettings->addMenu(this->timeEventControler->playbackSpeedMenu());
  this->menuSettings->addSeparator();
  this->menuSettings->addMenu(this->multiView->groundOrientationMenu());
  this->menuSettings->addMenu(this->multiView->markerTrajectoryLengthMenu());
  this->menuSettings->addAction(this->multiView->forceButterflyActivationAction());
  this->menuSettings->addSeparator();
  this->menuSettings->addMenu(this->multiView->chartHorizontalAxisUnitMenu());
  // Menu Tools
  this->mp_ToolsManager = new ToolsManager(new ToolsData(this), this->menuTools, this);
  this->mp_ToolsManager->menuModel()->addAction(this->mp_ModelDock->newSegmentAction());
  this->mp_ToolsManager->menuAcquisition()->addAction(this->timeEventControler->actionZoomUnzoomRegionOfInterest);
  this->mp_ToolsManager->menuAcquisition()->addAction(this->timeEventControler->actionCropRegionOfInterest);
  this->mp_ToolsManager->menuAcquisition()->addSeparator();
  this->mp_ToolsManager->menuAcquisition()->addAction(this->timeEventControler->actionReframeFromOne);
  this->mp_ToolsManager->menuEvent()->addSeparator();
  this->mp_ToolsManager->menuEvent()->addMenu(this->timeEventControler->insertEventMenu());
  this->mp_ToolsManager->menuEvent()->addSeparator();
  this->mp_ToolsManager->menuEvent()->addAction(this->timeEventControler->actionEditSelectedEvents);
  this->mp_ToolsManager->menuEvent()->addAction(this->timeEventControler->actionRemoveSelectedEvents);
  this->mp_ToolsManager->menuEvent()->addAction(this->timeEventControler->actionClearEvents);
  this->mp_ToolsManager->menuMarker()->addAction(this->actionToolCreateMarker);
  this->mp_ToolsManager->menuMarker()->addSeparator();
  this->mp_ToolsManager->menuMarker()->addAction(this->actionToolComputeMarkerDistance);
  this->mp_ToolsManager->menuMarker()->addAction(this->actionToolComputeMarkerAngle);
  this->mp_ToolsManager->menuMarker()->addAction(this->actionToolComputeVectorAngle);
  this->actionToolCreateMarker->setEnabled(false);
  this->actionToolComputeMarkerDistance->setEnabled(false);
  this->actionToolComputeMarkerAngle->setEnabled(false);
  this->actionToolComputeVectorAngle->setEnabled(false);
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
  this->actionReloadFile->setShortcut(QKeySequence::Refresh);
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
  this->timeEventControler->acquisitionOptionsButtonMenu->menu()->insertAction(this->timeEventControler->actionReframeFromOne, this->multiView->forceButterflyActivationAction());
  this->timeEventControler->insertEventMenu()->actions()[0]->setShortcut(QKeySequence(Qt::AltModifier + Qt::Key_7));  // Right Foot Strike
  this->timeEventControler->insertEventMenu()->actions()[1]->setShortcut(QKeySequence(Qt::AltModifier + Qt::Key_8));  // Right Toe Off
  this->timeEventControler->insertEventMenu()->actions()[2]->setShortcut(QKeySequence(Qt::AltModifier + Qt::Key_9));  // Right Other (unlabled)
  this->timeEventControler->insertEventMenu()->actions()[4]->setShortcut(QKeySequence(Qt::AltModifier + Qt::Key_4));  // Left Foot Strike
  this->timeEventControler->insertEventMenu()->actions()[5]->setShortcut(QKeySequence(Qt::AltModifier + Qt::Key_5));  // Left Toe Off
  this->timeEventControler->insertEventMenu()->actions()[6]->setShortcut(QKeySequence(Qt::AltModifier + Qt::Key_6));  // Left Other (unlabled)
  this->timeEventControler->insertEventMenu()->actions()[8]->setShortcut(QKeySequence(Qt::AltModifier + Qt::Key_1));  // General Foot Strike
  this->timeEventControler->insertEventMenu()->actions()[9]->setShortcut(QKeySequence(Qt::AltModifier + Qt::Key_2));  // General Toe Off
  this->timeEventControler->insertEventMenu()->actions()[10]->setShortcut(QKeySequence(Qt::AltModifier + Qt::Key_3)); // General Other (unlabled)
  
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
  actions3d.push_back(this->multiView->forceButterflyActivationAction());
  QAction* sep2b = new QAction(this); sep2b->setSeparator(true);
  actions3d.push_back(sep2b);
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
  // Setting the chart cycle manager
  this->multiView->setChartCycleSettingsManager(this->mp_ChartCycleSettingsManager);
  this->mp_Preferences->setChartCycleSettingsManager(this->mp_ChartCycleSettingsManager);
  
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
  connect(this->actionVisit_Mokka_website, SIGNAL(triggered()), this, SLOT(visitMokkaWebsite()));  
  connect(this->actionHelp, SIGNAL(triggered()), this, SLOT(help()));
  connect(this->actionViewMetadata, SIGNAL(triggered()), this, SLOT(viewMetadata()));
  connect(this->action_FileOpen, SIGNAL(triggered()), this, SLOT(openFile()));
  connect(this->actionReloadFile, SIGNAL(triggered()), this, SLOT(reloadFile()));
  connect(this->actionOpenFileLocation, SIGNAL(triggered()), this, SLOT(openFileLocation()));
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
  connect(this->actionImportEMF, SIGNAL(triggered()), this, SLOT(importEMF()));
  connect(this->actionImportAMTI, SIGNAL(triggered()), this, SLOT(importAMTI()));
  connect(this->actionImportDelsysEMG, SIGNAL(triggered()), this, SLOT(importDelsysEMG()));
  connect(this->actionImportCLB, SIGNAL(triggered()), this, SLOT(importCLB()));
  connect(this->actionImportVideo, SIGNAL(triggered()), this, SLOT(importVideos()));
  connect(this->actionExportC3D, SIGNAL(triggered()), this, SLOT(exportC3D()));
  connect(this->actionExportTRC, SIGNAL(triggered()), this, SLOT(exportTRC()));
  connect(this->actionExportANB, SIGNAL(triggered()), this, SLOT(exportANB()));
  connect(this->actionExportANC, SIGNAL(triggered()), this, SLOT(exportANC()));
  connect(this->actionExportCAL, SIGNAL(triggered()), this, SLOT(exportCAL()));
  connect(this->actionExportSTL, SIGNAL(triggered()), this, SLOT(exportSTL()));
  connect(this->actionExportASCII, SIGNAL(triggered()), this, SLOT(exportASCII()));
  connect(this->actionExportImageSeries, SIGNAL(triggered()), this, SLOT(exportImageSeries()));
  connect(this->actionPreferences, SIGNAL(triggered()), this, SLOT(showPreferences()));
  connect(this->actionSelect_All, SIGNAL(triggered()), this, SLOT(selectAll()));
  connect(this->actionCopy, SIGNAL(triggered()), this, SLOT(copy()));
  connect(this->actionSaveCurrentLayout, SIGNAL(triggered()), this, SLOT(saveCurrentLayout()));
  connect(this->actionManageUserLayouts, SIGNAL(triggered()), this, SLOT(manageUserLayouts()));
  connect(this->actionLayout3DOnly, SIGNAL(triggered()), this, SLOT(restoreLayout3DOnly()));
  connect(this->actionLayout3DVerbose, SIGNAL(triggered()), this, SLOT(restoreLayout3DVerbose()));
  connect(this->actionLayout3DCharts, SIGNAL(triggered()), this, SLOT(restoreLayout3DCharts()));
  connect(this->actionToolCreateMarker, SIGNAL(triggered()), this, SLOT(createMarkerFromMarkersSelection()));
  connect(this->actionToolComputeMarkerDistance, SIGNAL(triggered()), this, SLOT(computeDistanceFromMarkersSelection()));
  connect(this->actionToolComputeMarkerAngle, SIGNAL(triggered()), this, SLOT(computeAngleFromMarkersSelection()));
  connect(this->actionToolComputeVectorAngle, SIGNAL(triggered()), this, SLOT(computeAngleFromMarkersSelection2()));
  // MultiView
  connect(this->multiView, SIGNAL(fileDropped(QString)), this, SLOT(openFileDropped(QString)));
  connect(this->multiView, SIGNAL(visibleMarkersChanged(QVector<int>)), this->mp_ModelDock, SLOT(updateDisplayedMarkers(QVector<int>)));
  connect(this->multiView, SIGNAL(pickedMarkerChanged(int)), this, SLOT(selectPickedMarker(int)));
  connect(this->multiView, SIGNAL(pickedMarkerToggled(int)), this, SLOT(togglePickedMarker(int)));
  connect(this->multiView, SIGNAL(selectedMarkersToggled(QList<int>)), this, SLOT(selectSelectedMarkers(QList<int>)));
  connect(this->multiView, SIGNAL(trajectoryMarkerToggled(int)), this, SLOT(toggleMarkerTrajectory(int)));
  connect(this->multiView, SIGNAL(pausePlaybackRequested(bool)), this, SLOT(playPausePlayback(bool)));
  connect(this->multiView->manageChartCycleSettingsAction(), SIGNAL(triggered()), this, SLOT(manageChartCycleSettings()));
  // // Model dock
  connect(this->mp_ModelDock, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)), this, SLOT(modelDockLocationChanged(Qt::DockWidgetArea)));
  connect(this->mp_ModelDock, SIGNAL(markerLabelChanged(int, QString)), this, SLOT(setPointLabel(int, QString)));
  connect(this->mp_ModelDock, SIGNAL(markersRadiusChanged(QVector<int>, double)), this, SLOT(setMarkersRadius(QVector<int>, double)));
  connect(this->mp_ModelDock, SIGNAL(markersColorChanged(QVector<int>, QColor)), this, SLOT(setMarkersColor(QVector<int>, QColor)));
  connect(this->mp_ModelDock, SIGNAL(markersDescriptionChanged(QVector<int>, QString)), this, SLOT(setPointsDescription(QVector<int>, QString)));
  connect(this->mp_ModelDock, SIGNAL(markersVisibilityChanged(QVector<int>, bool)), this, SLOT(setMarkersVisibility(QVector<int>, bool)));
  connect(this->mp_ModelDock, SIGNAL(markersTrajectoryVisibilityChanged(QVector<int>, bool)), this, SLOT(setMarkersTrajectoryVisibility(QVector<int>, bool)));
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
  connect(this->mp_ModelDock, SIGNAL(wrenchPositionSelectionChanged(QList<int>)), this->multiView, SLOT(updateTrackedGRFPaths(QList<int>)));
  connect(this->mp_ModelDock->markerRadiusSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateSelectedMarkersRadius(double)));
  connect(this->mp_ModelDock, SIGNAL(segmentLabelChanged(int, QString)), this, SLOT(setSegmentLabel(int, QString)));
  connect(this->mp_ModelDock, SIGNAL(segmentsColorChanged(QVector<int>, QColor)), this, SLOT(setSegmentsColor(QVector<int>, QColor)));
  connect(this->mp_ModelDock, SIGNAL(segmentsDescriptionChanged(QVector<int>, QString)), this, SLOT(setSegmentsDescription(QVector<int>, QString)));
  connect(this->mp_ModelDock, SIGNAL(segmentsVisibilityChanged(QVector<int>, bool)), this, SLOT(setSegmentsVisibility(QVector<int>, bool)));
  connect(this->mp_ModelDock, SIGNAL(segmentsSurfaceVisibilityChanged(QVector<int>, bool)), this, SLOT(setSegmentsSurfaceVisibility(QVector<int>, bool)));
  connect(this->mp_ModelDock, SIGNAL(segmentsRemoved(QList<int>)), this, SLOT(removeSegments(QList<int>)));
  connect(this->mp_ModelDock, SIGNAL(segmentCreationRequested()), this, SLOT(createSegment()));
  connect(this->mp_ModelDock, SIGNAL(segmentEditionRequested()), this, SLOT(editSegment()));
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
  connect(this->timeEventControler, SIGNAL(playbackStarted()), this->multiView, SLOT(adaptViewsForPlaybackOn()));
  connect(this->timeEventControler, SIGNAL(playbackStopped()), this->multiView, SLOT(adaptViewsForPlaybackOff()));
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
  connect(actionCheckUpdate, SIGNAL(triggered()), this->mp_Updater, SLOT(checkUpdate()));
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
  connect(this->mp_Preferences, SIGNAL(defaultTimeBarEventDisplayChanged(int)), this, SLOT(setPreferenceDefaultTimeBarEventDisplay(int)));
  connect(this->mp_Preferences, SIGNAL(useEventEditorWhenInsertingStateChanged(bool)), this, SLOT(setPreferenceUseEventEditorWhenInserting(bool)));
  connect(this->mp_Preferences, SIGNAL(defaultBackgroundColorChanged(QColor)), this, SLOT(setPreferenceDefaultBackgroundColor(QColor)));
  connect(this->mp_Preferences, SIGNAL(defaultGridFrontColorChanged(QColor)), this, SLOT(setPreferenceDefaultGridFrontColor(QColor)));
  connect(this->mp_Preferences, SIGNAL(defaultGridBackColorChanged(QColor)), this, SLOT(setPreferenceDefaultGridBackColor(QColor)));
  connect(this->mp_Preferences, SIGNAL(defaultSegmentColorChanged(QColor)), this, SLOT(setPreferenceDefaultSegmentColor(QColor)));
  connect(this->mp_Preferences, SIGNAL(defaultMarkerColorChanged(QColor)), this, SLOT(setPreferenceDefaultMarkerColor(QColor)));
  connect(this->mp_Preferences, SIGNAL(defaultMarkerRadiusChanged(double)), this, SLOT(setPreferenceDefaultMarkerRadius(double)));
  connect(this->mp_Preferences, SIGNAL(defaultMarkerTrajectoryLengthChanged(int)), this, SLOT(setPreferenceDefaultTrajectoryLength(int)));
  connect(this->mp_Preferences, SIGNAL(showForcePlatformAxesChanged(int)), this, SLOT(setPreferenceShowForcePlatformAxes(int)));
  connect(this->mp_Preferences, SIGNAL(showForcePlatformIndexChanged(int)), this, SLOT(setPreferenceShowForcePlatformIndex(int)));
  connect(this->mp_Preferences, SIGNAL(defaultForcePlateColorChanged(QColor)), this, SLOT(setPreferenceDefaultForcePlateColor(QColor)));
  connect(this->mp_Preferences, SIGNAL(defaultForceVectorColorChanged(QColor)), this, SLOT(setPreferenceDefaultForceVectorColor(QColor)));
  connect(this->mp_Preferences, SIGNAL(defaultGRFButterflyActivationChanged(int)), this, SLOT(setPreferenceDefaultGRFButterflyActivation(int)));
  connect(this->mp_Preferences, SIGNAL(showForcePathChanged(int)), this, SLOT(setPreferenceShowForcePath(int)));
  connect(this->mp_Preferences, SIGNAL(defaultPlotLineWidthChanged(double)), this, SLOT(setPreferencePlotLineWidth(double)));
  connect(this->mp_Preferences, SIGNAL(showChartEventChanged(int)), this, SLOT(setPreferenceShowChartEvent(int)));
  connect(this->mp_Preferences, SIGNAL(chartHorizontalAxisUnitChanged(int)), this, SLOT(setPreferenceChartHorizontalAxisUnit(int)));
  connect(this->mp_Preferences, SIGNAL(automaticCheckUpdateStateChanged(bool)), this, SLOT(setPreferenceAutomaticCheckUpdate(bool)));
  connect(this->mp_Preferences, SIGNAL(subscribeDevelopmentChannelStateChanged(bool)), this, SLOT(setPreferenceSubscribeDevelopmentChannel(bool)));
#ifdef Q_OS_MAC
  connect(this->mp_Preferences, SIGNAL(userLayoutRemoved(int)), this, SLOT(removeUserLayout(int)));
  connect(this->mp_Preferences, SIGNAL(userLayoutLabelChanged(int, QString)), this, SLOT(relabelUserLayout(int, QString)));
  connect(this->mp_Preferences, SIGNAL(userLayoutDropped(int, int)), this, SLOT(updateDroppedUserLayouts(int, int)));
#else
  connect(this->mp_Preferences, SIGNAL(userLayoutsChanged(QList<QVariant>, int)), this, SLOT(updateUserLayouts(QList<QVariant>, int)));
#endif

  this->mp_Updater->finalizeUpdate();
};

MainWindow::~MainWindow()
{
  delete this->mp_Acquisition;
#ifdef Q_OS_MAC
  delete this->mp_MacMenuBar;
  delete this->mp_Preferences;
#endif
};

Acquisition* MainWindow::acquisition() const
{
  return this->mp_Acquisition;
};

QList<int> MainWindow::explorerSelectedItems(ModelItemTypes type) const
{
  return this->mp_ModelDock->selectedItems(type);
};

void MainWindow::pushUndoCommand(QUndoCommand* cmd) const
{
  this->mp_UndoStack->push(cmd);
};

QUndoStack* MainWindow::undoStack(int i) const
{
  if (i == 0)
    return this->mp_AcquisitionUndoStack;
  else if (i == 1)
    return this->mp_MarkerConfigurationUndoStack;
  else
    qDebug("Invalid index for the available undo stacks.");
  return 0;
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
    // Frame by frame
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
    // 10 frames by 10 frames
    else if (((keyEvent->modifiers() & Qt::AltModifier) == Qt::AltModifier) && (keyEvent->key() == Qt::Key_Left))
    {
      this->timeEventControler->previousFrame(10);
      return true;
    }
    else if (((keyEvent->modifiers() & Qt::AltModifier) == Qt::AltModifier) && (keyEvent->key() == Qt::Key_Right))
    {
      this->timeEventControler->nextFrame(10);
      return true;
    }
    // Playback
    else if (keyEvent->key() == Qt::Key_Space)
    {
      this->timeEventControler->togglePlayback();
      return true;
    }
    // Marker(s) deselection
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
      this->mp_Updater->checkUpdate(true);
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
  QDesktopServices::openUrl(QUrl("file:///" + QString(MOKKA_LINUX_SHARE_PATH) + "/doc/btk/mokka/index.html", QUrl::TolerantMode));
#endif
};

void MainWindow::visitBTKWebsite()
{
  QDesktopServices::openUrl(QUrl("http://b-tk.googlecode.com", QUrl::TolerantMode));
};

void MainWindow::visitMokkaWebsite()
{
  QDesktopServices::openUrl(QUrl("http://b-tk.googlecode.com/svn/web/mokka/index.html", QUrl::TolerantMode));
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
    while (this->m_RecentFiles.count() > maxRecentFiles)
      this->m_RecentFiles.pop_back();
  } 
  this->updateRecentFileActions();
  this->setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("Mokka")));
};

void MainWindow::updateRecentFileActions()
{
  int num = qMin(this->m_RecentFiles.count(), (int)maxRecentFiles);
  for (int i = 0 ; i < num ; ++i)
  { 
    QString text = QFileInfo(this->m_RecentFiles[i]).fileName();
    this->mp_ActionRecentFiles[i]->setText(text);
    this->mp_ActionRecentFiles[i]->setData(this->m_RecentFiles[i]);
    this->mp_ActionRecentFiles[i]->setVisible(true);
  }
  for (int i = num ; i < maxRecentFiles ; ++i)
    this->mp_ActionRecentFiles[i]->setVisible(false); 
  this->actionClear_Menu->setEnabled(!this->m_RecentFiles.isEmpty());
  this->mp_ActionSeparatorRecentFiles->setVisible(!this->m_RecentFiles.isEmpty());
};

void MainWindow::openRecentFile()
{
  if (this->isOkToContinue() && this->mp_ModelDock->isOkToContinue())
  { 
    QAction* pAction = qobject_cast<QAction*>(sender()); 
    if (pAction) 
    {
      QString filename = pAction->data().toString();
      QMutableStringListIterator i(this->m_RecentFiles); 
      while (i.hasNext())
      {
        if ((filename.compare(i.next()) == 0) && !QFile::exists(filename))
        {
          LOG_WARNING("The file '" + i.value() + "' doesn't exist anymore and is removed from the list of the recent files.");
          i.remove();
          this->updateRecentFileActions();
          break;
        }
      }
      this->openFile(filename);
    }
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
  // this->actionManageUserLayouts->setEnabled(!this->m_UserLayouts.isEmpty());
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
      int layoutIndex; 
      QString layoutName = nld.layoutName(&layoutIndex);
      if (layoutIndex == -1) // New
      {
        this->m_UserLayouts.push_back(layoutName);
        this->m_UserLayouts.push_back(this->multiView->saveLayout());
        layoutIndex = this->m_UserLayouts.count() / 2 - 1;
      }
      else
        this->m_UserLayouts[layoutIndex*2+1] = this->multiView->saveLayout();
      this->updateUserLayoutActions();
      this->mp_ActionUserLayouts[layoutIndex]->setChecked(true);
      QSettings settings;
      settings.setValue("MainWindow/userLayouts", this->m_UserLayouts);
      settings.setValue("MainWindow/currentLayout", layoutIndex+3);
      this->mp_Preferences->refreshUserLayouts();
      LOG_INFO(tr("Saving the new user's layout: ") + layoutName);
    }
  }
};

void MainWindow::manageUserLayouts()
{
#ifndef Q_OS_MAC
  this->mp_Preferences->showLayoutsPreferences();
  this->showPreferences();
#else
  this->showPreferences();
  this->mp_Preferences->showLayoutsPreferences();
#endif
};

void MainWindow::manageChartCycleSettings()
{
#ifndef Q_OS_MAC
  this->mp_Preferences->showChartPreferences();
  this->showPreferences();
#else
  this->showPreferences();
  this->mp_Preferences->showChartPreferences();
#endif
  
};

void MainWindow::createMarkerFromMarkersSelection()
{
  QList<int> selectedMarkers;
  if (this->extractSelectedMarkers(selectedMarkers))
  {
    if (selectedMarkers.size() < 2)
    {
      QMessageBox error(QMessageBox::Warning, "Tools", "A minimum of two markers is required to create an averaged marker.", QMessageBox::Ok , this);
#ifdef Q_OS_MAC
      error.setWindowFlags(Qt::Sheet);
      error.setWindowModality(Qt::WindowModal);
#endif
      error.exec();
    }
    else
    {
      QList<int> ids;
      QList<bool> visibles, trajectories;
      QList<double> radii;
      QList<QColor> colors;
      // Extract All the IDs used by the markers and virtual markers
      QList<QTreeWidgetItem*> roots;
      roots << this->mp_ModelDock->modelTree->topLevelItem(ModelDockWidget::MarkersItem);
      roots << this->mp_ModelDock->modelTree->topLevelItem(ModelDockWidget::VirtualMarkersItem);
      for (QList<QTreeWidgetItem*>::iterator itR = roots.begin() ; itR != roots.end() ; ++itR)
      {
        for (int j = 0 ; j < (*itR)->childCount() ; ++j)
          ids << (*itR)->child(j)->data(0, PointId).toInt();
      }
      // Create the new marker and reapply the configuration (reset in the VTK pipeline)
      for (QList<AbstractView*>::const_iterator it = this->multiView->views().begin() ; it != this->multiView->views().end() ; ++it)
        static_cast<CompositeView*>(*it)->view(CompositeView::Viz3D)->setUpdatesEnabled(false);
      QApplication::setOverrideCursor(Qt::WaitCursor);
      this->multiView->markersConfiguration(ids, visibles, trajectories, radii, colors);
      this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new CreateAveragedMarker(this->mp_Acquisition, selectedMarkers)));
      this->multiView->setMarkersConfiguration(ids, visibles, trajectories, radii, colors);
      QApplication::restoreOverrideCursor();
      for (QList<AbstractView*>::const_iterator it = this->multiView->views().begin() ; it != this->multiView->views().end() ; ++it)
        static_cast<CompositeView*>(*it)->view(CompositeView::Viz3D)->setUpdatesEnabled(true);
    }
  }
};

void MainWindow::computeDistanceFromMarkersSelection()
{
  QList<int> selectedMarkers;
  if (this->extractSelectedMarkers(selectedMarkers))
  {
    if (selectedMarkers.size() != 2)
    {
      QMessageBox error(QMessageBox::Warning, "Tools", "Only two markers must be selected to compute the distance between them.", QMessageBox::Ok , this);
#ifdef Q_OS_MAC
      error.setWindowFlags(Qt::Sheet);
      error.setWindowModality(Qt::WindowModal);
#endif
      error.exec();
    }
    else
    {
      ChartDialog* chartDialog = this->multiView->createChartDialog(this);
      this->showChartTool(chartDialog, chartDialog->computeDistance(selectedMarkers[0], selectedMarkers[1]));
    }
  }
};

void MainWindow::computeAngleFromMarkersSelection()
{
  QList<int> selectedMarkers;
  if (this->extractSelectedMarkers(selectedMarkers))
  {
    if (selectedMarkers.size() != 3)
    {
      QMessageBox error(QMessageBox::Warning, "Tools", "Only three markers must be selected to compute the angle between them.", QMessageBox::Ok , this);
#ifdef Q_OS_MAC
      error.setWindowFlags(Qt::Sheet);
      error.setWindowModality(Qt::WindowModal);
#endif
      error.exec();
    }
    else
    {
      ChartDialog* chartDialog = this->multiView->createChartDialog(this);
      this->showChartTool(chartDialog, chartDialog->computeAngleFromMarkers(selectedMarkers[0], selectedMarkers[1], selectedMarkers[2]));
    }
  }
};

void MainWindow::computeAngleFromMarkersSelection2()
{
  QList<int> selectedMarkers;
  if (this->extractSelectedMarkers(selectedMarkers))
  {
    if ((selectedMarkers.size() != 3) && (selectedMarkers.size() != 4))
    {
      QMessageBox error(QMessageBox::Warning, "Tools", "Three of four markers must be selected to compute the angle between the two constructed vectors.", QMessageBox::Ok , this);
#ifdef Q_OS_MAC
      error.setWindowFlags(Qt::Sheet);
      error.setWindowModality(Qt::WindowModal);
#endif
      error.exec();
    }
    else
    {
      ChartDialog* chartDialog = this->multiView->createChartDialog(this);
      this->showChartTool(chartDialog, chartDialog->computeAngleFromVectors(selectedMarkers));
    }
  }
};

bool MainWindow::extractSelectedMarkers(QList<int>& selectedMarkers)
{
  selectedMarkers.clear();
  bool mixedSelection = false;
  QList<QTreeWidgetItem*> items = this->mp_ModelDock->modelTree->selectedItems();
  for (QList<QTreeWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
  {
    if ((*it)->type() != MarkerType)
    {
      mixedSelection = true;
      break;
    }
    else
      selectedMarkers.push_back((*it)->data(0,PointId).toInt());
  }
  
  if (mixedSelection)
  {
    QMessageBox error(QMessageBox::Warning, "Tools", "The selection is not only composed of markers.", QMessageBox::Ok , this);
#ifdef Q_OS_MAC
    error.setWindowFlags(Qt::Sheet);
    error.setWindowModality(Qt::WindowModal);
#endif
    error.exec();
    return false;
  }
  return true;
}

void MainWindow::showChartTool(ChartDialog* chartDialog, bool computed)
{
  // Clean current hidden tool charts
  QList<ChartDialog*>::iterator it = this->m_ToolCharts.begin();
  while (it != this->m_ToolCharts.end())
  {
    if (!(*it)->isVisible())
    {
      (*it)->deleteLater();
      it = this->m_ToolCharts.erase(it);
    }
    else
      ++it;
  }
  // Modeless dialog
  if (computed)
  {
    this->m_ToolCharts << chartDialog;
    chartDialog->installEventFilter(this);
    connect(this->timeEventControler, SIGNAL(currentFrameChanged(int)), chartDialog, SLOT(updateChartRendering()));
    chartDialog->show();
    chartDialog->raise();
    chartDialog->activateWindow();
  }
  else
    chartDialog->deleteLater();
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
    QStringList formats;
    this->mp_Acquisition->supportedReadFileFormats(formats);
    QStringList allFormats;
    QString byFormat;
    foreach(const QString& str, formats)
    {
      QString ext = "*." + str.split(" ")[0].toLower();
      if (allFormats.indexOf(ext) == -1)
        allFormats.append(ext);
      byFormat += (!byFormat.isEmpty() ? ";;" : "") + str + " Files (" + ext + ")";
    }
    QString allFormat = "Acquisition Files (" + allFormats.join(" ") + ");;";
    
    QString filename = QFileDialog::getOpenFileName(this, "",
                         this->m_LastDirectory,
                         allFormat + byFormat);
    if (!filename.isEmpty())
      this->openFile(filename);
  }
};

void MainWindow::reloadFile()
{
  this->mp_ActionRecentFiles[0]->trigger();
};

void MainWindow::openFileLocation()
{
  if (this->mp_Acquisition->fileName().isEmpty())
  {
    qDebug("Empty filename. Impossible to open a file explorer.");
    return;
  }
  QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(this->mp_Acquisition->fileName()).canonicalPath()));
};

void MainWindow::openFileDropped(const QString& filename)
{
  if (this->isOkToContinue() && this->mp_ModelDock->isOkToContinue())
    this->openFile(filename);
};

void MainWindow::openFile(const QString& filename)
{
  QFileInfo fI(filename);
  bool specialCaseAMTI = false;
  
  // Special case for AMTI file
  if (fI.suffix().toUpper().compare("ASC") == 0)
  {
    specialCaseAMTI = true;
    this->mp_ModelDock->setEnabled(false);
    this->mp_ImportAssistant->acquisitionSystemComboBox->setCurrentIndex(this->mp_ImportAssistant->stackedWidget->indexOf(this->mp_ImportAssistant->amtiPage)-1);
    this->mp_ImportAssistant->acquisitionSystemComboBox->setEnabled(false);
    this->mp_ImportAssistant->newAcquisitionRadioButton->setChecked(true);
    this->mp_ImportAssistant->appendAcquisitionRadioButton->setEnabled(false);
    this->mp_ImportAssistant->keepAllFrameRadioButton->setChecked(true);
    this->mp_ImportAssistant->keepHighestFirstFrameRadioButton->setEnabled(false);
    this->mp_ImportAssistant->amtiForceMomentLineEdit->setText(filename);
    this->mp_ImportAssistant->amtiForceMomentLineEdit->setReadOnly(true);
    this->mp_ImportAssistant->amtiForceMomentButton->setEnabled(false);
    this->mp_ImportAssistant->importOptionFrame->setVisible(false);
    bool accepted = this->mp_ImportAssistant->exec();
    this->mp_ModelDock->setEnabled(true); // If the dock is not enabled before the import, then its signals are not emited.
    if (!accepted)
      return;
  }
  
  LOG_INFO(tr("Loading acquisition from file: ") + fI.fileName());
  ProgressWidget pw(this);
  pw.show();
  pw.setProgressValue(10);
  bool noOpenError = false;
  if (specialCaseAMTI)
  {
    this->mp_Acquisition->clear();
    noOpenError = this->importAssistantAMTI(filename, this->mp_ImportAssistant->amtiInformationsComboBox->currentIndex(), true, true);
  }
  else
    noOpenError = this->mp_Acquisition->load(filename);
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
  if (this->mp_Acquisition->hasPoints() || this->mp_Acquisition->hasAnalogs() || this->mp_Acquisition->hasVideos())
    this->mp_ModelDock->setVisible(true);
  if (this->mp_ModelDock->currentConfigurationIndex() == -1) // No configuration loaded
  {
    QSettings settings;
    bool defaultConfigurationUsed = settings.value("Preferences/defaultConfigurationUsed", false).toBool();
    QString defaultConfigurationPath = settings.value("Preferences/defaultConfigurationPath", "").toString();
    if (defaultConfigurationUsed && !defaultConfigurationPath.isEmpty())
      this->mp_ModelDock->loadConfiguration(defaultConfigurationPath);
  }
  this->mp_ModelDock->setGroundRectionForcePathsVisibility(this->mp_Preferences->showForcePathComboBox->currentIndex() == 0);
  
  pw->setProgressValue(100);

  QApplication::restoreOverrideCursor();
  
  this->timeEventControler->setEnabled(true);
  this->actionClose->setEnabled(true);
  this->actionViewMetadata->setEnabled(true);
  this->actionSave_As->setEnabled(true);
  this->menuExport->menuAction()->setEnabled(true);
  this->actionReloadFile->setEnabled(true);
  this->actionOpenFileLocation->setEnabled(true);
  // Tools Menu
  this->actionToolCreateMarker->setEnabled(true);
  this->actionToolComputeMarkerDistance->setEnabled(true);
  this->actionToolComputeMarkerAngle->setEnabled(true);
  this->actionToolComputeVectorAngle->setEnabled(true);
  this->mp_ToolsManager->setActionsEnabled(true);
  
  // Append new events' label to define cycle
  QStringList eventsLabel = this->mp_ChartCycleSettingsManager->eventsLabel();
  for (QMap<int, Event*>::const_iterator it = this->mp_Acquisition->events().begin() ; it != this->mp_Acquisition->events().end() ; ++it)
  {
    if (!eventsLabel.contains(it.value()->label))
      eventsLabel.append(it.value()->label);
  }
  this->mp_ChartCycleSettingsManager->setEventsLabel(eventsLabel);
};

void MainWindow::saveFile()
{
  if (!this->mp_Acquisition->fileName().isEmpty() && this->mp_Acquisition->canBeSaved(this->mp_Acquisition->fileName()))
    this->saveFile(this->mp_Acquisition->fileName());
  else
    this->saveAsFile();
};

void MainWindow::saveAsFile()
{
  // Generate the filename and the selected filter
  QString file;
  if (this->mp_Acquisition->fileName().isEmpty())
    file = this->m_LastDirectory + "/untitled.c3d";
  else
    file = this->m_RecentFiles.first();
  QString suffix = QFileInfo(file).suffix().toUpper();
  QStringList formats;
  this->mp_Acquisition->supportedWrittenFileFormats(formats);
  bool compatibleSuffix = false;
  for (QStringList::const_iterator it = formats.begin() ; it != formats.end() ; ++it)
  {
    if (suffix.compare(*it, Qt::CaseInsensitive) == 0)
    {
      compatibleSuffix = true;
      break;
    }
  }
  if (!compatibleSuffix)
  {
    file += ".c3d";
    suffix = "C3D";
  }
  QString selectedFilter = suffix + " Files (*." + suffix.toLower() + ")";
  // Generate all the filters
  QString byFormat;
  foreach(const QString& str, formats)
  {
    QString ext = "*." + str.split(" ")[0].toLower();
    byFormat += (!byFormat.isEmpty() ? ";;" : "") + str + " Files (" + ext + ")";
  }
  
  // Dialog box
  QString filename = QFileDialog::getSaveFileName(this, "",
                       file,
                       byFormat,
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
  // Special case for the tools (Chart dialog, etc.)
  for (QList<ChartDialog*>::iterator it = this->m_ToolCharts.begin() ; it != this->m_ToolCharts.end() ; ++it)
  {
    if (QApplication::activeWindow() == *it)
    {
      (*it)->setVisible(false);
      (*it)->deleteLater();
      this->m_ToolCharts.erase(it);
      return;
    }
  }
  // General behavior
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
  this->mp_ImportAssistant->clear(this->m_LastDirectory);
  this->importAssistant(this->mp_ImportAssistant->acquisitionSystemComboBox->currentIndex());
  this->actionReloadFile->setEnabled(false); // Because the imported file(s) from the assistant cannot be reloaded
  this->actionOpenFileLocation->setEnabled(false);
}

void MainWindow::importAssistant(int systemIndex, bool systemLocked, bool allFramesKeptOnly)
{
  this->mp_ModelDock->setEnabled(false);
  this->mp_ImportAssistant->acquisitionSystemComboBox->setCurrentIndex(systemIndex);
  this->mp_ImportAssistant->acquisitionSystemComboBox->setEnabled(systemLocked ? false : true);
  this->mp_ImportAssistant->appendAcquisitionRadioButton->setEnabled(this->actionClose->isEnabled());
  this->mp_ImportAssistant->keepHighestFirstFrameRadioButton->setEnabled(allFramesKeptOnly ? false : true);
  bool accepted = this->mp_ImportAssistant->exec();
  this->mp_ModelDock->setEnabled(true); // If the dock is not enabled before the import, then its signals are not emited.
  if (accepted)
  {
    if (this->mp_ImportAssistant->newAcquisitionRadioButton->isChecked())
      this->mp_Acquisition->clear();
    if (this->mp_ImportAssistant->stackedWidget->currentWidget() == this->mp_ImportAssistant->amtiPage)
    {
      QStringList filenames = this->mp_ImportAssistant->filenames();
      if (!filenames.isEmpty())
      {
        LOG_INFO(tr("Importing acquisition."));
        QSettings settings;
        QString title = this->windowTitle();
        ProgressWidget pw(this);
        pw.show();
        pw.setProgressValue(10);
        
        bool allFramesKept = this->mp_ImportAssistant->keepAllFrameRadioButton->isChecked();
        int infoIndex = this->mp_ImportAssistant->amtiInformationsComboBox->currentIndex();
        LOG_INFO(tr("Loading acquisition for importation from file: ") + QFileInfo(filenames[0]).fileName());
        bool noImportError = this->importAssistantAMTI(filenames[0], infoIndex, allFramesKept);
        
        this->loadAcquisition(noImportError, &pw);
        this->setWindowTitle(title);
        pw.hide();
        if (!noImportError)
          return;
        settings.setValue("ImportAssistant/lastAcquisitionSystem", this->mp_ImportAssistant->acquisitionSystemComboBox->currentIndex());
        this->setWindowModified(true);
        this->actionSave->setEnabled(true);
        this->m_LastDirectory = QFileInfo(filenames.last()).absolutePath();
      }
    }
    else if (this->mp_ImportAssistant->stackedWidget->currentWidget() == this->mp_ImportAssistant->videoPage)
    {
      QStringList filenames = this->mp_ImportAssistant->filenames();
      if (!filenames.isEmpty())
      {
        LOG_INFO(tr("Importing videos."));
        QSettings settings;
        QString title = this->windowTitle();
        ProgressWidget pw(this);
        pw.show();
        pw.setProgressValue(10);
        
        bool allFramesKept = this->mp_ImportAssistant->keepAllFrameRadioButton->isChecked();
        int ff = this->mp_ImportAssistant->videoFirstFrameSpinBox->value();
        double freq = static_cast<double>(this->mp_ImportAssistant->videoFrequencySpinBox->value());
        double duration = this->mp_ImportAssistant->videoDurationDoubleSpinBox->value();
        bool noImportError = this->mp_Acquisition->importFromVideos(filenames, allFramesKept, ff, freq, duration);
        
        this->loadAcquisition(noImportError, &pw);
        this->setWindowTitle(title);
        pw.hide();
        if (!noImportError)
          return;
        settings.setValue("ImportAssistant/lastAcquisitionSystem", this->mp_ImportAssistant->acquisitionSystemComboBox->currentIndex());
        this->setWindowModified(true);
        this->actionSave->setEnabled(true);
        this->m_LastDirectory = QFileInfo(filenames.last()).absolutePath();
      }
    }
    else
    {
      if (this->importAcquisitions(this->mp_ImportAssistant->filenames(), this->mp_ImportAssistant->keepAllFrameRadioButton->isChecked(), false))
      {
        QSettings settings;
        settings.setValue("ImportAssistant/lastAcquisitionSystem", this->mp_ImportAssistant->acquisitionSystemComboBox->currentIndex());
      }
    }
  }
};

bool MainWindow::importAssistantAMTI(const QString& filename, int infoIndex, bool allFramesKept, bool fromOpenAction)
{
  QSettings settings;
  bool noImportError = false;
  if (infoIndex == 0)
  {
    QList<QVariant> dims = this->mp_ImportAssistant->amtiDimensions();
    noImportError = this->mp_Acquisition->importFromAMTI(filename, allFramesKept, dims, fromOpenAction);
    if (noImportError)
      settings.setValue("ImportAssistant/AMTIDimensions", dims);
  }
  else
  {
    QList<QVariant> corners = this->mp_ImportAssistant->amtiCorners();
    QList<QVariant> origin = this->mp_ImportAssistant->amtiOrigin();
    noImportError = this->mp_Acquisition->importFromAMTI(filename, allFramesKept, corners, origin, fromOpenAction);
    if (noImportError)
    {
      settings.setValue("ImportAssistant/AMTICorners", corners);
      settings.setValue("ImportAssistant/AMTIOrigin", origin);
    }
  }
  if (noImportError)
    settings.setValue("ImportAssistant/AMTILastInfoUsed", infoIndex);
  return noImportError;
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
  this->importAcquisition(tr("XLS OrthoTrak Files (*.xls)"));
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
  this->importAcquisition(tr("EMG BTS Bioengineering Files (*.emg)"));
};

void MainWindow::importMOM()
{
  this->importAcquisition(tr("MOM Files (*.mom)"));
};

void MainWindow::importPWR()
{
  this->importAcquisition(tr("PWR Files (*.pwr)"));
};

void MainWindow::importAMTI()
{
  this->mp_ImportAssistant->clear(this->m_LastDirectory);
  this->mp_ImportAssistant->importOptionFrame->setVisible(false);
  this->importAssistant(this->mp_ImportAssistant->stackedWidget->indexOf(this->mp_ImportAssistant->amtiPage)-1, true, true);
};

void MainWindow::importEMF()
{
  this->importAcquisition(tr("EMF Ascension Files (*.emf)"));
};

void MainWindow::importDelsysEMG()
{
  this->importAcquisition(tr("EMG Delsys Files (*.emg)"));
};

void MainWindow::importCLB()
{
  this->importAcquisition(tr("CLB Files (*.clb)"));
};

void MainWindow::importAcquisition(const QString& filter)
{
  QString filename = QFileDialog::getOpenFileName(this, "",
                       this->m_LastDirectory,
                       filter);
  if (!filename.isEmpty())
  {
    QString f = this->mp_Acquisition->fileName();
    this->importAcquisitions(QStringList(filename));
    this->mp_Acquisition->setFileName(f);
  }
};

bool MainWindow::importAcquisitions(const QStringList& filenames, bool allFramesKept, bool keepWindowTitle)
{
  bool noImportError = false;
  if (!filenames.isEmpty())
  {
    LOG_INFO(tr("Importing acquisition(s)."));
    QString title = this->windowTitle();
    bool reloadStatus = this->actionReloadFile->isEnabled();
    bool openFileLocationStatus = this->actionOpenFileLocation->isEnabled();
    ProgressWidget pw(this);
    pw.show();
    pw.setProgressValue(10);
    noImportError = this->mp_Acquisition->importFrom(filenames, allFramesKept);
    this->loadAcquisition(noImportError, &pw);
    if (keepWindowTitle)
      this->setWindowTitle(title);
    this->actionReloadFile->setEnabled(reloadStatus);
    this->actionOpenFileLocation->setEnabled(openFileLocationStatus);
    pw.hide();
    if (noImportError)
    {
      this->setWindowModified(true);
      this->actionSave->setEnabled(true);
      this->m_LastDirectory = QFileInfo(filenames.last()).absolutePath();
    }
  }
  return noImportError;
}

void MainWindow::importVideos()
{
  if (!this->mp_Acquisition->btkAcquisition())
  {
    this->mp_ImportAssistant->clear(this->m_LastDirectory);
    this->mp_ImportAssistant->newAcquisitionRadioButton->setChecked(true);
    this->mp_ImportAssistant->keepAllFrameRadioButton->setChecked(true);
    this->mp_ImportAssistant->importOptionFrame->setVisible(false);
    this->importAssistant(this->mp_ImportAssistant->stackedWidget->indexOf(this->mp_ImportAssistant->videoPage)-1, true, true);
  }
  else
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

void MainWindow::exportSTL()
{
  ExportSTLDialog exporterDlg(this);
  QFileInfo fI(this->mp_Acquisition->fileName());
  exporterDlg.pathLineEdit->setText(fI.canonicalPath());
  exporterDlg.filePrefixLineEdit->setText(fI.baseName());
  for (QMap<int, Segment*>::const_iterator it = this->mp_Model->segments().begin() ; it != this->mp_Model->segments().end() ; ++it)
    exporterDlg.segmentListWidget->addItem(it.value()->label);
  // FIXME: Adding items seems to select the first one but doesn't highlight it. Moreoever, it seems impossible to unselect it, so its selection is forced. 
  exporterDlg.segmentListWidget->setCurrentRow(0);
  if (exporterDlg.exec() == QDialog::Accepted)
  {
    QString filePrefix = exporterDlg.pathLineEdit->text() + "/" + exporterDlg.filePrefixLineEdit->text();
    
    int lb, rb;
    if (exporterDlg.allFramesRadioButton->isChecked())
    {
      lb = this->mp_Acquisition->firstFrame();
      rb = this->mp_Acquisition->lastFrame();
    } 
    else if (exporterDlg.selectedFramesRadioButton->isChecked())
    {
      lb = this->timeEventControler->leftBound();
      rb = this->timeEventControler->rightBound();
    }
    else
    {
      lb = rb = this->timeEventControler->currentFrame();
    }
    
    LOG_INFO("Exporting segment '" + exporterDlg.segmentListWidget->currentItem()->text() + "' to STL files: " + filePrefix + "*.stl");
    bool noExportError = false;
    try
    {
      QApplication::setOverrideCursor(Qt::WaitCursor);
      
      btk::MultiSTLFileWriter::Pointer exporter = btk::MultiSTLFileWriter::New();
      exporter->SetInputAcquisition(this->mp_Acquisition->btkAcquisition());
      exporter->SetInputMesh(this->mp_Model->segments().value(exporterDlg.segmentListWidget->currentRow())->mesh);
      exporter->SetFilePrefix(qPrintable(filePrefix));
      exporter->SetFramesOfInterest(lb,rb);
      exporter->Update();
      
      QApplication::restoreOverrideCursor();
      noExportError = true;
    }
    catch (btk::Exception& e)
    {
      LOG_ERROR(e.what());
    }
    catch (std::exception& e)
    {
      LOG_ERROR("Unexpected error: " + QString(e.what()));
    }
    catch (...)
    {
      LOG_ERROR("Unknown error.");
    }
    
    if (!noExportError)
    {
      QApplication::restoreOverrideCursor();
      QMessageBox error(QMessageBox::Warning, "File error", "Error occurred during the file exporting.", QMessageBox::Ok , this);
#ifdef Q_OS_MAC
      error.setWindowFlags(Qt::Sheet);
      error.setWindowModality(Qt::WindowModal);
#endif
      error.setInformativeText("<nobr>Check the logger for more informations.</nobr>");
      error.exec();
    }
  }
};

void MainWindow::exportASCII()
{
  ExportASCIIDialog exporterDlg(this);
  exporterDlg.fillPoints(this->mp_ModelDock->modelTree->topLevelItem(ModelDockWidget::MarkersItem), this->mp_ModelDock->modelTree->topLevelItem(ModelDockWidget::VirtualMarkersItem), this->mp_ModelDock->modelTree->topLevelItem(ModelDockWidget::ModelOutputsItem));
  exporterDlg.fillForcePlates(this->mp_ModelDock->modelTree->topLevelItem(ModelDockWidget::ForcePlatesItem));
  exporterDlg.fillAnalogs(this->mp_ModelDock->modelTree->topLevelItem(ModelDockWidget::AnalogsItem));
  if (exporterDlg.exec() == QDialog::Accepted)
  {
    QString ext = "." + exporterDlg.suggestedExtension();
    QString file;
    if (this->mp_Acquisition->fileName().isEmpty())
      file = this->m_LastDirectory + "/untitled" + ext;
    else
      file = this->m_LastDirectory + "/" + QFileInfo(this->m_RecentFiles.first()).baseName() + ext;
    QString filename = QFileDialog::getSaveFileName(this, "", file, "All Files (*)");
    if (!filename.isEmpty())
    {
      bool noExportError = false;
      
      LOG_INFO("Exporting data to ASCII/Text file " + filename);
      try
      {
        QApplication::setOverrideCursor(Qt::WaitCursor);
        
        int lb, rb;
        if (exporterDlg.allFramesRadioButton->isChecked())
        {
          lb = this->mp_Acquisition->firstFrame();
          rb = this->mp_Acquisition->lastFrame();
        } 
        else if (exporterDlg.selectedFramesRadioButton->isChecked())
        {
          lb = this->timeEventControler->leftBound();
          rb = this->timeEventControler->rightBound();
        }
        else
        {
          lb = rb = this->timeEventControler->currentFrame();
        }
        
        btk::Acquisition::Pointer acquisition = btk::Acquisition::New();
        acquisition->SetFirstFrame(this->mp_Acquisition->firstFrame());
        acquisition->SetPointFrequency(this->mp_Acquisition->pointFrequency());
        acquisition->SetPointUnits(this->mp_Acquisition->btkAcquisition()->GetPointUnits());
        btk::PointCollection::Pointer points = acquisition->GetPoints();
        btk::AnalogCollection::Pointer analogs = acquisition->GetAnalogs();
        btk::EventCollection::Pointer events = acquisition->GetEvents();
        std::vector<int16_t> forcePlatePointsIndices;
        // Append the data
        if ((exporterDlg.pointsTreeWidget->topLevelItemCount() != 0) && (exporterDlg.pointsTreeWidget->topLevelItem(0)->checkState(0) != Qt::Unchecked))
        {
          // Title: Markers, Angles, Moments, etc.
          for (int i = 0 ; i < exporterDlg.pointsTreeWidget->topLevelItem(0)->childCount() ; ++i)
          {
            QTreeWidgetItem* item = exporterDlg.pointsTreeWidget->topLevelItem(0)->child(i);
            if (item->checkState(0) != Qt::Unchecked)
            {
              for (int j = 0 ; j < item->childCount() ; ++j)
              {
                QTreeWidgetItem* child = item->child(j);
                if ((child->checkState(0) == Qt::Checked) && !child->isHidden())
                  points->InsertItem(this->mp_Acquisition->btkAcquisition()->GetPoint(this->mp_Acquisition->points().value(child->data(0,PointId).toInt())->btkidx));
              }
            }
          }
        }
        if ((exporterDlg.forcePlatesTreeWidget->topLevelItemCount() != 0) && (exporterDlg.forcePlatesTreeWidget->topLevelItem(0)->checkState(0) != Qt::Unchecked))
        {
          int numPoints = points->GetItemNumber();
          // Title: Force platfom reaction #i.
          for (int i = 0 ; i < exporterDlg.forcePlatesTreeWidget->topLevelItem(0)->childCount() ; ++i)
          {
            QTreeWidgetItem* item = exporterDlg.forcePlatesTreeWidget->topLevelItem(0)->child(i);
            if (item->checkState(0) != Qt::Unchecked)
            {
              // Only 4 child is possible
              Q_ASSERT(item->childCount() == 4);
              for (int j = 0 ; j < 4 ; ++j)
              {
                QTreeWidgetItem* child = item->child(j);
                if ((child->checkState(0) == Qt::Checked) && !child->isHidden())
                {
                  int id = child->data(0, ForcePlateId).toInt();
                  int idxFp = (id - 65535) / 4;
                  int idxCpt = (id - 65535) % 4; 
                  if (idxCpt < 3)
                    points->InsertItem(this->mp_Acquisition->btkGroundReactionWrenches()->GetItem(idxFp)->GetComponent(idxCpt));
                  else
                    points->InsertItem(this->mp_Acquisition->btkWrenchDirectionAngles()->GetItem(idxFp));
                  forcePlatePointsIndices.push_back(numPoints++);
                }
              }
            }
          }
        }
        if ((exporterDlg.analogsTreeWidget->topLevelItemCount() != 0) && (exporterDlg.analogsTreeWidget->topLevelItem(0)->checkState(0) != Qt::Unchecked))
        {
          for (int i = 0 ; i < exporterDlg.analogsTreeWidget->topLevelItem(0)->childCount() ; ++i)
          {
            QTreeWidgetItem* item = exporterDlg.analogsTreeWidget->topLevelItem(0)->child(i);
            if ((item->checkState(0) == Qt::Checked) && !item->isHidden())
              analogs->InsertItem(this->mp_Acquisition->btkAcquisition()->GetAnalog(this->mp_Acquisition->analogs().value(item->data(0,AnalogId).toInt())->btkidx));
          }
        }
        if (exporterDlg.writeEventsCheckBox->checkState() == Qt::Checked)
        {
          for (QMap<int,Event*>::const_iterator it = this->mp_Acquisition->events().begin() ; it != this->mp_Acquisition->events().end() ; ++it)
          {
            Event* e = it.value();
            if ((e->frame >= lb) && (e->frame <= rb))
            {
              events->InsertItem(btk::Event::New(e->label.toStdString(), e->time, e->frame, e->context.toStdString(), btk::Event::Unknown,
                                                 e->subject.toStdString(), e->description.toStdString(), e->iconId));
            }
          }
        }
        btk::MetaData::Pointer exportOptions = MetaDataCreateChild(acquisition->GetMetaData(), "BTK_ASCII_EXPORT_OPTIONS");
        btk::MetaDataCreateChild(exportOptions, "FORCEPLATE_POINTS_SEPARATELY", forcePlatePointsIndices);
        if (exporterDlg.writeHeaderCheckBox->checkState() == Qt::Unchecked)
          btk::MetaDataCreateChild(exportOptions, "NO_HEADER", (int8_t)1);
        acquisition->Resize(points->GetItemNumber(), this->mp_Acquisition->btkAcquisition()->GetPointFrameNumber(), analogs->GetItemNumber(), this->mp_Acquisition->btkAcquisition()->GetNumberAnalogSamplePerFrame());
        
        btk::ASCIIFileWriter::Pointer exporter = btk::ASCIIFileWriter::New();
        exporter->SetInput(acquisition);
        exporter->SetFilename(filename.toStdString());
        exporter->SetSeparator(exporterDlg.choosedSeparator().toStdString());
        exporter->SetFramesOfInterest(lb,rb);
        exporter->Update();
        QApplication::restoreOverrideCursor();
        noExportError = true;
      }
      catch (btk::Exception& e)
      {
        LOG_ERROR(e.what());
      }
      catch (std::exception& e)
      {
        LOG_ERROR("Unexpected error: " + QString(e.what()));
      }
      catch (...)
      {
        LOG_ERROR("Unknown error.");
      }
      
      if (!noExportError)
      {
        QApplication::restoreOverrideCursor();
        QMessageBox error(QMessageBox::Warning, "File error", "Error occurred during the file exporting.", QMessageBox::Ok , this);
#ifdef Q_OS_MAC
        error.setWindowFlags(Qt::Sheet);
        error.setWindowModality(Qt::WindowModal);
#endif
        error.setInformativeText("<nobr>Check the logger for more informations.</nobr>");
        error.exec();
      }
    }
  }
}

void MainWindow::exportImageSeries()
{
  QMessageBox error(QMessageBox::Warning, "Export Image Error", "", QMessageBox::Ok , this);
#ifdef Q_OS_MAC
  error.setWindowFlags(Qt::Sheet);
  error.setWindowModality(Qt::WindowModal);
#endif

  QList<Viz3DWidget*> viz3ds;
  QList<QPixmap> viewsInfo;
  for (QList<AbstractView*>::const_iterator it = this->multiView->views().begin() ; it != this->multiView->views().end() ; ++it)
  {
    Viz3DWidget* viz3dwidget = qobject_cast<Viz3DWidget*>(static_cast<CompositeView*>(*it)->currentView());
    if (viz3dwidget != 0)
    {
      const int size = 384;
      viewsInfo.push_back(QPixmap::grabWidget(viz3dwidget, viz3dwidget->width()/2-size/2, viz3dwidget->height()/2-size/2 ,size, size));
      viz3ds.push_back(viz3dwidget);
    }
  }
  if (viz3ds.isEmpty())
  {
    error.setText("Impossible to export any image without a visible 3D view.");
    error.exec();
    return;
  }
  
  ExportImageSeriesDialog exporterDlg(this);
  QFileInfo fI(this->mp_Acquisition->fileName());
  exporterDlg.pathLineEdit->setText(fI.canonicalPath());
  exporterDlg.filePrefixLineEdit->setText(fI.baseName());
  exporterDlg.setViewsInfo(&viewsInfo);
  
  if (exporterDlg.exec() != QDialog::Accepted)
    return;
    
  if (!QDir().mkpath(exporterDlg.pathLineEdit->text()))
  { 
    error.setText("Impossible to create the destination folder.");
    error.exec();
    return;
  }
 
  int lb, rb;
  if (exporterDlg.allFramesRadioButton->isChecked())
  {
    lb = this->mp_Acquisition->firstFrame();
    rb = this->mp_Acquisition->lastFrame();
  }
  else if (exporterDlg.selectedFramesRadioButton->isChecked())
  {
    lb = this->timeEventControler->leftBound();
    rb = this->timeEventControler->rightBound();
  }
  else
  {
    lb = rb = this->timeEventControler->currentFrame();
  }
  LOG_INFO("Exporting image series");
  QString filename_base = exporterDlg.pathLineEdit->text() + "/" + exporterDlg.filePrefixLineEdit->text();
  bool noExportError = false;
  int num = (int)log10(std::fabs((float)rb))+1;
  Viz3DWidget* viz3d = viz3ds[exporterDlg.viewComboBox->currentIndex()];
  try
  {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    // backup the current frame
    int current_frame = this->timeEventControler->currentFrame();
    // revert to the start
    this->timeEventControler->setCurrentFrame(lb);
    // Export!
    int previousFrame = lb;
    ProgressWidget pw(this);
    pw.label->setText("Exporting images...");
    pw.progressBar->setRange(lb,rb);
    pw.setProgressValue(previousFrame);
#ifdef Q_OS_WIN
    // The progress dialog is also saved in the exported images under Windows XP...
    if (QSysInfo::windowsVersion() >= QSysInfo::WV_WINDOWS7)
      pw.show();
#else
    pw.show();
#endif
    
    if (exporterDlg.pngRadioButton->isChecked())
    {
      LOG_INFO("Exporting frames to images: " + filename_base + "*.png");
      do
      {
        QString filename = filename_base + QString("%1.png").arg(QString::number(this->timeEventControler->currentFrame()), num, QChar('0'));
        viz3d->renderToPng(filename);
        previousFrame = this->timeEventControler->currentFrame();
        this->timeEventControler->nextFrame();
        pw.setProgressValue(previousFrame);
      }
      while (previousFrame != rb);
    }
    else if (exporterDlg.jpegRadioButton->isChecked())
    {
      LOG_INFO("Exporting frames to images: " + filename_base + "*.jpg");
      do
      {
        QString filename = filename_base + QString("%1.jpg").arg(QString::number(this->timeEventControler->currentFrame()), num, QChar('0'));
        viz3d->renderToJpeg(filename);
        previousFrame = this->timeEventControler->currentFrame();
        this->timeEventControler->nextFrame();
        pw.setProgressValue(previousFrame);
      }
      while (previousFrame != rb);
    }
    else
    {
      LOG_INFO("Exporting frames to images: " + filename_base + "*.tif");
      do
      {
        QString filename = filename_base + QString("%1.tif").arg(QString::number(this->timeEventControler->currentFrame()), num, QChar('0'));
        viz3d->renderToTiff(filename);
        previousFrame = this->timeEventControler->currentFrame();
        this->timeEventControler->nextFrame();
        pw.setProgressValue(previousFrame);
      }
      while (previousFrame != rb);
    }
    // restore the current frame
    this->timeEventControler->setCurrentFrame(current_frame);
    pw.hide();
    QApplication::restoreOverrideCursor();
    noExportError = true;
  }
  catch (std::exception& e)
  {
    LOG_ERROR("Unexpected error: " + QString(e.what()));
  }
  catch (...)
  {
    LOG_ERROR("Unknown error.");
  }

  if (!noExportError)
  {
    QApplication::restoreOverrideCursor();
    error.setText("Error occurred during the file exporting.");
    error.setInformativeText("<nobr>Check the logger for more informations.</nobr>");
    error.exec();
  }
}

void MainWindow::showPreferences()
{
#ifdef Q_OS_MAC
  this->mp_Preferences->show();
  this->mp_Preferences->raise();
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
  if (logger != 0)
    logger->selectAll();
};

void MainWindow::copy()
{
  QWidget* w = QApplication::focusWidget();
  LoggerWidget* logger = qobject_cast<LoggerWidget*>(w);
  if (logger != 0)
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
  this->actionReloadFile->setEnabled(false);
  this->actionOpenFileLocation->setEnabled(false);
  this->actionClose->setEnabled(false);
  this->actionViewMetadata->setEnabled(false);
  this->actionSave->setEnabled(false);
  this->actionSave_As->setEnabled(false);
  this->menuExport->menuAction()->setEnabled(false);
  this->setCurrentFile("");
  // Tools Menu
  this->actionToolCreateMarker->setEnabled(false);
  this->actionToolComputeMarkerDistance->setEnabled(false);
  this->actionToolComputeMarkerAngle->setEnabled(false);
  this->actionToolComputeVectorAngle->setEnabled(false);
  this->mp_ToolsManager->setActionsEnabled(false);
  // Tools modeless window
  QList<ChartDialog*>::iterator it = this->m_ToolCharts.begin();
  while (it != this->m_ToolCharts.end())
  {
    (*it)->setVisible(false);
    (*it)->deleteLater();
    it = this->m_ToolCharts.erase(it);
  }
  // Model dock
  this->mp_ModelDock->reset();
  // Metadata
  this->mp_MetadataDlg->reset();
  // Multivew
  this->multiView->clear();
  // Time & Event Controler
  this->timeEventControler->reset();
  this->timeEventControler->setEnabled(false);
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

void MainWindow::setMarkersVisibility(const QVector<int>& ids, bool visible)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_MarkerConfigurationUndoStack, new EditMarkersVisibility(this->mp_Acquisition, ids, visible)));
};

void MainWindow::setMarkersTrajectoryVisibility(const QVector<int>& ids, bool visible)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_MarkerConfigurationUndoStack, new EditMarkersTrajectoryVisibility(this->mp_Acquisition, ids, visible)));
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

void MainWindow::setSegmentsVisibility(const QVector<int>& ids, bool visible)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_MarkerConfigurationUndoStack, new EditSegmentsVisibility(this->mp_Model, ids, visible)));
};

void MainWindow::setSegmentsSurfaceVisibility(const QVector<int>& ids, bool visible)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_MarkerConfigurationUndoStack, new EditSegmentsSurfaceVisibility(this->mp_Model, ids, visible)));
};

void MainWindow::removeSegments(const QList<int>& ids)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_MarkerConfigurationUndoStack, new RemoveSegments(this->mp_Model, ids)));
};

void MainWindow::createSegment()
{
  this->editSegment(true);
};

void MainWindow::editSegment()
{
  this->editSegment(false);
};

void MainWindow::editSegment(bool isNew)
{
  // Keep only the selected segment
  int segmentId = -1;
  QList<int> segmentIds, unvisibleSegmentIds;
  QTreeWidgetItem* segmentsRoot = this->mp_ModelDock->modelTree->topLevelItem(ModelDockWidget::SegmentsItem);
  for (int i = 0 ; i < segmentsRoot->childCount() ; ++i)
  {
    QTreeWidgetItem* item = segmentsRoot->child(i);
    int id = item->data(0,SegmentId).toInt();
    if ((item->checkState(ModelDockWidget::VisibleHeader) == Qt::Unchecked) || item->isHidden())
    {
      unvisibleSegmentIds << id;
      segmentIds << id;
    }
    else if ((item->isSelected()) && !isNew)
      segmentId = id;
    else
      segmentIds << id;
  }
  
  Segment* seg = 0;
  QList<int> selectedMarkerIds, unselectedMarkerIds, unvisibleMarkerIds;
  QList<NewSegmentDialog::MarkerInfo> markersInfo;
  QList<QTreeWidgetItem*> roots;
  roots << this->mp_ModelDock->modelTree->topLevelItem(ModelDockWidget::MarkersItem);
  roots << this->mp_ModelDock->modelTree->topLevelItem(ModelDockWidget::VirtualMarkersItem);
  if (isNew)
  {
    // Create a fake segment
    seg = new Segment("", "", this->mp_Model->defaultSegmentColor(), selectedMarkerIds.toVector(), QVector<Pair>(), QVector<Triad>());
    seg->surfaceVisible = true;
    // Check if a previous fake segment already exists but was canceled
    if (segmentsRoot->childCount() != 0)
    {
      QTreeWidgetItem* segmentItem = segmentsRoot->child(segmentsRoot->childCount()-1);
      if (segmentItem->text(0).compare("MOKKA_CANCELED_SEGMENT") == 0)
      {
        segmentId = segmentItem->data(0,SegmentId).toInt();
        QList<int> idList; idList << segmentId;
        QList<Segment*> segList; segList << seg;
        this->multiView->appendNewSegments(idList, segList);
        segmentsRoot->removeChild(segmentItem);
        delete segmentItem;
        unvisibleSegmentIds.pop_back();
        segmentIds.pop_back();
      }
    }
    // Not found
    if (segmentId == -1)
      segmentId = this->multiView->appendNewSegment(seg);
    // Keep only the selected markers
    for (QList<QTreeWidgetItem*>::iterator itR = roots.begin() ; itR != roots.end() ; ++itR)
    {
      for (int i = 0 ; i < (*itR)->childCount() ; ++i)
      {
        QTreeWidgetItem* item = (*itR)->child(i);
        int id = item->data(0,PointId).toInt();
        if ((item->checkState(ModelDockWidget::VisibleHeader) == Qt::Unchecked) || item->isHidden())
        {
          unvisibleMarkerIds << id;
          unselectedMarkerIds << id;
          continue;
        }
        else if (!item->isSelected())
          unselectedMarkerIds << id;
        else 
          selectedMarkerIds << id;
        markersInfo.push_back(NewSegmentDialog::MarkerInfo(id, item->text(ModelDockWidget::LabelHeader), item->isSelected()));
      }
    }
  }
  else if (segmentId != -1)
  {
    // Extract the selected segment
    seg = this->mp_Model->segments().value(segmentId);
    // Force the visibility of the surface
    this->multiView->setSegmentsSurfaceVisibility(QVector<int>(1,segmentId), QVector<bool>(1,true));
    // Keep only the markers used by the segment
    for (QList<QTreeWidgetItem*>::iterator itR = roots.begin() ; itR != roots.end() ; ++itR)
    {
      for (int i = 0 ; i < (*itR)->childCount() ; ++i)
      {
        QTreeWidgetItem* item = (*itR)->child(i);
        int id = item->data(0,PointId).toInt();
        if ((item->checkState(ModelDockWidget::VisibleHeader) == Qt::Unchecked) || item->isHidden())
        {
          unvisibleMarkerIds << id;
          unselectedMarkerIds << id;
        }
        else if (seg->markerIds.indexOf(id) == -1)
        {
          unselectedMarkerIds << id;
          markersInfo.push_back(NewSegmentDialog::MarkerInfo(id, item->text(ModelDockWidget::LabelHeader), false));
        }
        else 
        {
          selectedMarkerIds << id;
          markersInfo.push_back(NewSegmentDialog::MarkerInfo(id, item->text(ModelDockWidget::LabelHeader), true));
        }
      }
    }
  }
  else
  {
    qDebug("No segment selected. Impossible to edit its definition.");
    return;
  }
  
  if (seg != 0)
  {
    bool hasPlayback = this->timeEventControler->playbackStatus();
    this->timeEventControler->stopPlayback();

    // To avoid the update of the 3D views during the building of the segment.
    for (QList<AbstractView*>::const_iterator it = this->multiView->views().begin() ; it != this->multiView->views().end() ; ++it)
      static_cast<CompositeView*>(*it)->view(CompositeView::Viz3D)->setUpdatesEnabled(false);

    this->multiView->updateHiddenMarkers(unselectedMarkerIds);
    this->multiView->circleSelectedMarkers(QList<int>());
    this->multiView->updateHiddenSegments(segmentIds);
    
    if (this->mp_SegmentEditor == 0)
    {
      this->mp_SegmentEditor = new NewSegmentDialog(this);
      this->mp_SegmentEditor->viz3D->setGlobalFrameVisible(false);
      this->mp_SegmentEditor->viz3D->copy(static_cast<Viz3DWidget*>(static_cast<CompositeView*>(this->multiView->views()[0])->view(CompositeView::Viz3D)));
      // Connections
      connect(this->mp_SegmentEditor, SIGNAL(markerSelectionChanged(QList<int>)), this->multiView, SLOT(circleSelectedMarkers(QList<int>)));
      connect(this->mp_SegmentEditor, SIGNAL(markerVisibleSelectionChanged(QList<int>)), this->multiView, SLOT(updateVisibleMarkers(QList<int>)));
      connect(this->mp_SegmentEditor, SIGNAL(segmentDefinitionChanged(int, QVector<int>, QVector<Pair>, QVector<Triad>)), this->multiView, SLOT(setSegmentDefinition(int, QVector<int>, QVector<Pair>, QVector<Triad>)));
    }
    this->mp_SegmentEditor->initialize(seg, segmentId, markersInfo, !isNew);
    // Show only markers and segments
    // WARNING: THIS METHOD IS SENSITIVE TO THE ORDER OF THE VTK OBJECT CONSTRUCTION IN THE MUTLIVIEW.
    vtkActorCollection* actors = this->mp_SegmentEditor->viz3D->renderer()->GetActors();
    actors->InitTraversal();
    vtkActor* actor = actors->GetNextItem();
    int inc = 0;
    while (actor)
    {
      // 4: Marker coordinates
      // 6: Segment links
      // 7: Segment face
      if (inc == 4)
        actor->UseBoundsOn(); // To be centered around the markers
      else if ((inc != 4) && (inc != 6) && (inc != 7))
        actor->SetVisibility(0);
      actor = actors->GetNextItem();
      ++inc;
    }
    this->mp_SegmentEditor->viz3D->renderer()->ResetCamera();
  
    bool canceled = false;
    if (isNew)
    {
      if (this->mp_SegmentEditor->exec() == QDialog::Accepted)
      {
        seg->label = this->mp_SegmentEditor->segmentLabelEdit->text();
        seg->description = this->mp_SegmentEditor->segmentDescriptionEdit->text();
        seg->surfaceVisible = !seg->faces.isEmpty();
        this->mp_UndoStack->push(new MasterUndoCommand(this->mp_MarkerConfigurationUndoStack, new InsertSegment(this->mp_Model, seg)));
      }
      else
      {
        // To know that a canceled segment exist in VTK
        QTreeWidgetItem* segmentItem = new QTreeWidgetItem(QStringList("MOKKA_CANCELED_SEGMENT"));
        segmentItem->setData(0, SegmentId, segmentId);
        segmentsRoot->addChild(segmentItem);
        segmentItem->setHidden(true);
        unvisibleSegmentIds << segmentId;
        delete seg;
        canceled = true;
      }
    }
    else
    {
      // Copy the current definition of the mesh
      QVector<int> oldMarkerIds = seg->markerIds;
      QVector<Pair> oldLinks = seg->links;
      QVector<Triad> oldFaces = seg->faces;
      bool oldSurfaceVisible = seg->surfaceVisible;
      if (this->mp_SegmentEditor->exec() == QDialog::Accepted)
      {
        if ((seg->markerIds != oldMarkerIds) || (seg->links != oldLinks) || (seg->faces != oldFaces))
        {
          QVector<int> markerIds = seg->markerIds;
          QVector<Pair> links = seg->links;
          QVector<Triad> faces = seg->faces;
          bool surfaceVisible = faces.isEmpty() ? seg->surfaceVisible : true;
          // Need to reset the mesh informations to be able to use the undo/redo actions.
          seg->markerIds = oldMarkerIds;
          seg->links = oldLinks;
          seg->faces = oldFaces;
          seg->surfaceVisible = oldSurfaceVisible;
          QUndoCommand* cmd = new QUndoCommand;
          new EditSegmentDefinition(this->mp_Model, segmentId, markerIds, links, faces, cmd);
          new EditSegmentsSurfaceVisibility(this->mp_Model, QVector<int>(1,segmentId), surfaceVisible, cmd);
          this->mp_UndoStack->push(new MasterUndoCommand(this->mp_MarkerConfigurationUndoStack, cmd));
        }
      }
      else
      {
        // Force the segment defintion to be as before the edition
        seg->surfaceVisible = oldSurfaceVisible;
        this->multiView->setSegmentsSurfaceVisibility(QVector<int>(1,segmentId), QVector<bool>(1,oldSurfaceVisible));
        this->mp_Model->setSegmentDefinition(segmentId, oldMarkerIds, oldLinks, oldFaces);
      }
    }
    
    // Reset the markers visibility and selection
    this->multiView->updateHiddenMarkers(unvisibleMarkerIds);
    if (isNew && canceled)
      this->multiView->circleSelectedMarkers(selectedMarkerIds);
    else
      this->multiView->circleSelectedMarkers(QList<int>());
    this->multiView->updateHiddenSegments(unvisibleSegmentIds);
    // Reset 3D view (as the actors are shared between all the 3D views)
    actors = this->mp_SegmentEditor->viz3D->renderer()->GetActors();
    actors->InitTraversal();
    actor = actors->GetNextItem();
    inc = 0;
    while (actor)
    {
      if (inc == 4)
        actor->UseBoundsOff();
      actor->SetVisibility(1);
      actor = actors->GetNextItem();
      ++inc;
    }  
    // Enable the update
    for (QList<AbstractView*>::const_iterator it = this->multiView->views().begin() ; it != this->multiView->views().end() ; ++it)
      static_cast<CompositeView*>(*it)->view(CompositeView::Viz3D)->setUpdatesEnabled(true);
    // Restart the playback (if necessary)
    if (hasPlayback)
      this->timeEventControler->startPlayback();
  }
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
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new InsertEvents(this->mp_Acquisition, e)));
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

void MainWindow::setPreferenceDefaultTimeBarEventDisplay(int index)
{
  QSettings settings;
  settings.setValue("Preferences/defaultTimeBarEventDisplay", index);
  this->timeEventControler->setTimeEventTicksDisplay(index);
};

void MainWindow::setPreferenceDefaultBackgroundColor(const QColor& color)
{
  QSettings settings;
  settings.setValue("Preferences/defaultBackgroundColor", color);
  this->multiView->setDefaultBackgroundColor(color);
};

void MainWindow::setPreferenceDefaultGridFrontColor(const QColor& color)
{
  QSettings settings;
  // Keep the 'defaultGridColor' keyword to stay compatible with the previous settings
  settings.setValue("Preferences/defaultGridColor", color);
  this->multiView->setDefaultGridFrontColor(color);
};

void MainWindow::setPreferenceDefaultGridBackColor(const QColor& color)
{
  QSettings settings;
  settings.setValue("Preferences/defaultGridBackColor", color);
  this->multiView->setDefaultGridBackColor(color);
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

void MainWindow::setPreferenceDefaultGRFButterflyActivation(int index)
{
  QSettings settings;
  settings.setValue("Preferences/defaultButterflyActivation", index);
  this->multiView->setGRFButterflyActivation(index == 0); // 0: ON ; 1: OFF
};

void MainWindow::setPreferenceShowForcePath(int index)
{
  QSettings settings;
  settings.setValue("Preferences/showForcePath", index);
  this->mp_ModelDock->setGroundRectionForcePathsVisibility(index == 0);
};

void MainWindow::setPreferencePlotLineWidth(double width)
{
  QSettings settings;
  settings.setValue("Preferences/defaultPlotLineWidth", width);
  this->multiView->setDefaultPlotLineWidth(width);
};

void MainWindow::setPreferenceShowChartEvent(int index)
{
  QSettings settings;
  settings.setValue("Preferences/showChartEvent", index);
  this->multiView->showChartEvent(index == 0);
};

void MainWindow::setPreferenceChartHorizontalAxisUnit(int index)
{
  QSettings settings;
  settings.setValue("Preferences/chartUnitAxisX", index);
  this->multiView->setChartHorizontalAxisUnit(index);
};

void MainWindow::setPreferenceAutomaticCheckUpdate(bool isChecked)
{
  QSettings settings;
  settings.setValue("Preferences/checkUpdateStartup", isChecked);
};

void MainWindow::setPreferenceSubscribeDevelopmentChannel(bool isChecked)
{
  QSettings settings;
  settings.setValue("Preferences/developmentChannelSubscription", isChecked);
  this->mp_Updater->acceptDevelopmentUpdate(isChecked);
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
  
  // Chart cycle settings
  settings.beginGroup("ChartCycleSettings");
  QStringList eventsLabel = settings.value("eventsLabel").toStringList();
  int numSettings = settings.value("settingsNumber", -1).toInt();
  QList<QVariant> byteCycleSettings = settings.value("settingsDefinition", -1).toList();
  settings.endGroup();
  if (eventsLabel.isEmpty())
  {
    eventsLabel.push_back("Foot Strike");
    eventsLabel.push_back("Foot Off");
  }
  this->mp_ChartCycleSettingsManager->setEventsLabel(eventsLabel);
  if (numSettings == -1)
  {
    ChartCycleSetting ccs;
    ccs.name = "Cycle: Full Gait";
    ccs.horizontalAxisTitle = "Gait Cycle (%GC)";
    ccs.calculationMethod = 0; // 0: Normalization
    ccs.calculationMethodOption = NULL;
    ccs.rightEvents[0] = "Foot Strike";
    ccs.rightEvents[1] = "Foot Strike";
    ccs.leftEvents[0] = "Foot Strike";
    ccs.leftEvents[1] = "Foot Strike";
    ccs.generalEvents[0] = "Foot Strike";
    ccs.generalEvents[1] = "Foot Strike";
    ccs.rightLabelRule = 0; // 0: Starts with
    ccs.rightLabelRuleText = "R";
    ccs.leftLabelRule = 0;
    ccs.leftLabelRuleText = "L";
    this->mp_ChartCycleSettingsManager->addSetting(ccs);
  }
  else
    this->mp_ChartCycleSettingsManager->importSettings(byteCycleSettings);
  // Force to update the content related to cycles into preferences
  this->mp_Preferences->setChartCycleSettingsManager(this->mp_ChartCycleSettingsManager);

  // Preferences
  settings.beginGroup("Preferences");
  bool defaultConfigurationUsed = settings.value("defaultConfigurationUsed", false).toBool();
  QString defaultConfigurationPath = settings.value("defaultConfigurationPath", "").toString();
  bool openEventEditorWhenInserting = settings.value("openEventEditorWhenInserting", true).toBool();
  int defaultPlaneOrientation = settings.value("defaultPlaneOrientation", 0).toInt();
  int defaultTimeBarEventDisplay = settings.value("defaultTimeBarEventDisplay", 0).toInt();
  QColor defaultBackgroundColor = settings.value("defaultBackgroundColor", QColor(0,0,0)).value<QColor>();
  QColor defaultGridFrontColor = settings.value("defaultGridColor", QColor(204,204,204)).value<QColor>(); // Keep the 'defaultGridColor' keyword to stay compatible with the previous settings
  QColor defaultGridBackColor = settings.value("defaultGridBackColor", QColor(255,0,0)).value<QColor>();
  QColor defaultSegmentColor = settings.value("defaultSegmentColor", QColor(255,255,255)).value<QColor>();
  QColor defaultMarkerColor = settings.value("defaultMarkerColor", QColor(255,255,255)).value<QColor>();
  double defaultMarkerRadius = settings.value("defaultMarkerRadius", 8.0).toDouble();
  int defaultMarkerTrajectoryLength = settings.value("defaultMarkerTrajectoryLength", 3).toInt();
  int showForcePlatformAxes = settings.value("showForcePlatformAxes", 0).toInt();
  int showForcePlatformIndex = settings.value("showForcePlatformIndex", 0).toInt();
  QColor defaultForcePlateColor = settings.value("defaultForcePlateColor", QColor(255,255,0)).value<QColor>();
  QColor defaultForceVectorColor = settings.value("defaultForceVectorColor", QColor(255,255,0)).value<QColor>();
  int defaultButterflyActivation = settings.value("defaultButterflyActivation", 1).toInt();
  int showForcePath = settings.value("showForcePath", 1).toInt();
  double defaultPlotLineWidth = settings.value("defaultPlotLineWidth", 1.0).toDouble();
  int showChartEvent = settings.value("showChartEvent", 0).toInt();
  int chartUnitAxisX = settings.value("chartUnitAxisX", 0).toInt();
  bool checkUpdateStartup = settings.value("checkUpdateStartup", true).toBool();
  bool developmentChannelSubscription = settings.value("developmentChannelSubscription", false).toBool();
  settings.endGroup();
  this->mp_Preferences->lastDirectory = this->m_LastDirectory;
  this->mp_Preferences->defaultConfigurationCheckBox->setChecked(defaultConfigurationUsed);
  this->mp_Preferences->defaultConfigurationLineEdit->setText(defaultConfigurationPath);
  this->mp_Preferences->openEventEditorCheckBox->setChecked(openEventEditorWhenInserting);
  this->mp_Preferences->defaultPlaneOrientationComboBox->setCurrentIndex(defaultPlaneOrientation);
  this->mp_Preferences->defaultTimeBarEventDisplayComboBox->setCurrentIndex(defaultTimeBarEventDisplay);
  colorizeButton(this->mp_Preferences->defaultBackgroundColorButton, defaultBackgroundColor);
  colorizeButton(this->mp_Preferences->defaultGridFrontColorButton, defaultGridFrontColor);
  colorizeButton(this->mp_Preferences->defaultGridBackColorButton, defaultGridBackColor);
  colorizeButton(this->mp_Preferences->defaultSegmentColorButton, defaultSegmentColor);
  colorizeButton(this->mp_Preferences->defaultMarkerColorButton, defaultMarkerColor);
  this->mp_Preferences->defaultMarkerRadiusSpinBox->setValue(defaultMarkerRadius);
  this->mp_Preferences->defaultMarkerTrajectoryLengthComboBox->setCurrentIndex(defaultMarkerTrajectoryLength);
  this->mp_Preferences->showForcePlatformAxesComboBox->setCurrentIndex(showForcePlatformAxes);
  this->mp_Preferences->showForcePlatformIndexComboBox->setCurrentIndex(showForcePlatformIndex);
  colorizeButton(this->mp_Preferences->defaultForcePlateColorButton, defaultForcePlateColor);
  colorizeButton(this->mp_Preferences->defaultForceVectorColorButton, defaultForceVectorColor);
  this->mp_Preferences->defaultGRFButterflyActivationComboBox->setCurrentIndex(defaultButterflyActivation);
  this->mp_Preferences->showForcePathComboBox->setCurrentIndex(showForcePath);
  this->mp_Preferences->defaultPlotLineWidthSpinBox->setValue(defaultPlotLineWidth);
  this->mp_Preferences->defaultChartEventDisplayComboBox->setCurrentIndex(showChartEvent);
  this->mp_Preferences->defaultChartUnitAxisXComboBox->setCurrentIndex(chartUnitAxisX);
  this->mp_Preferences->automaticCheckUpdateCheckBox->setChecked(checkUpdateStartup);
  this->mp_Preferences->subscribeDevelopmentChannelCheckBox->setChecked(developmentChannelSubscription);

#ifdef Q_OS_WIN
  this->mp_Preferences->setPreference(Preferences::DefaultConfigurationUse, defaultConfigurationUsed);
  this->mp_Preferences->setPreference(Preferences::DefaultConfigurationPath, defaultConfigurationPath);
  this->mp_Preferences->setPreference(Preferences::EventEditorWhenInserting, openEventEditorWhenInserting);
  this->mp_Preferences->setPreference(Preferences::DefaultGroundOrientation, defaultPlaneOrientation);
  this->mp_Preferences->setPreference(Preferences::DefaultTimeBarEventDisplay, defaultTimeBarEventDisplay);
  this->mp_Preferences->setPreference(Preferences::DefaultBackgroundColor, defaultBackgroundColor);
  this->mp_Preferences->setPreference(Preferences::DefaultGridFrontColor, defaultGridFrontColor);
  this->mp_Preferences->setPreference(Preferences::DefaultGridBackColor, defaultGridBackColor);
  this->mp_Preferences->setPreference(Preferences::DefaultSegmentColor, defaultSegmentColor);
  this->mp_Preferences->setPreference(Preferences::DefaultMarkerColor, defaultMarkerColor);
  this->mp_Preferences->setPreference(Preferences::DefaultMarkerRadius, defaultMarkerRadius);
  this->mp_Preferences->setPreference(Preferences::DefaultTrajectoryLength, defaultMarkerTrajectoryLength);
  this->mp_Preferences->setPreference(Preferences::ForcePlatformAxesDisplay, showForcePlatformAxes);
  this->mp_Preferences->setPreference(Preferences::ForcePlatformIndexDisplay, showForcePlatformIndex);
  this->mp_Preferences->setPreference(Preferences::DefaultForcePlateColor, defaultForcePlateColor);
  this->mp_Preferences->setPreference(Preferences::DefaultForceVectorColor, defaultForceVectorColor);
  this->mp_Preferences->setPreference(Preferences::DefaultGRFButterflyActivation, defaultButterflyActivation);
  this->mp_Preferences->setPreference(Preferences::ForcePathDisplay, showForcePath);
  this->mp_Preferences->setPreference(Preferences::DefaultPlotLineWidth, defaultPlotLineWidth);
  this->mp_Preferences->setPreference(Preferences::ChartEventDisplay, showChartEvent);
  this->mp_Preferences->setPreference(Preferences::chartUnitAxisX, chartUnitAxisX);
  this->mp_Preferences->setPreference(Preferences::UserLayoutIndex, layoutIndex);
  this->mp_Preferences->setPreference(Preferences::UserLayouts, this->m_UserLayouts);
  this->mp_Preferences->setPreference(Preferences::AutomaticCheckUpdateUse, checkUpdateStartup);
  this->mp_Preferences->setPreference(Preferences::DevelopmentChannelSubscriptionUsed, developmentChannelSubscription);
#endif
  
  if (defaultConfigurationUsed && !defaultConfigurationPath.isEmpty())
    this->mp_ModelDock->loadConfiguration(defaultConfigurationPath);
  this->timeEventControler->setOpenEditorWhenInsertingEventFlag(openEventEditorWhenInserting);
  this->multiView->setDefaultGroundOrientation(defaultPlaneOrientation);
  this->timeEventControler->setTimeEventTicksDisplay(defaultTimeBarEventDisplay);
  this->multiView->setDefaultBackgroundColor(defaultBackgroundColor);
  this->multiView->setDefaultGridFrontColor(defaultGridFrontColor);
  this->multiView->setDefaultGridBackColor(defaultGridBackColor);
  this->multiView->setDefaultSegmentColor(defaultSegmentColor);
  this->multiView->setDefaultMarkerColor(defaultMarkerColor);
  this->multiView->setDefaultMarkerRadius(defaultMarkerRadius);
  this->multiView->setMarkerTrajectoryLength(defaultMarkerRadius);
  this->multiView->showForcePlatformAxes(showForcePlatformAxes == 0);
  this->multiView->showForcePlatformIndex(showForcePlatformIndex == 0);
  this->multiView->setForcePlatformColor(defaultForcePlateColor);
  this->multiView->setForceVectorColor(defaultForceVectorColor);
  this->multiView->setGRFButterflyActivation(defaultButterflyActivation == 0);
  this->multiView->setDefaultPlotLineWidth(defaultPlotLineWidth);
  this->multiView->showChartEvent(showChartEvent == 0);
  this->multiView->setChartHorizontalAxisUnit(chartUnitAxisX);
  this->mp_Updater->acceptDevelopmentUpdate(developmentChannelSubscription);
  
  // Import assistant
  settings.beginGroup("ImportAssistant");
  this->mp_ImportAssistant->acquisitionSystemComboBox->setCurrentIndex(settings.value("lastAcquisitionSystem", -1).toInt());
  this->mp_ImportAssistant->amtiInformationsComboBox->setCurrentIndex(settings.value("AMTILastInfoUsed", 0).toInt());
  this->mp_ImportAssistant->setAmtiDimensions(settings.value("AMTIDimensions", QList<QVariant>()).toList());
  this->mp_ImportAssistant->setAmtiGeometry(settings.value("AMTICorners", QList<QVariant>()).toList(),
                                            settings.value("AMTIOrigin", QList<QVariant>()).toList());
  settings.endGroup();
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
  // Chart cycle settings
  settings.beginGroup("ChartCycleSettings");
  settings.setValue("eventsLabel", this->mp_ChartCycleSettingsManager->eventsLabel());
  settings.setValue("settingsNumber", this->mp_ChartCycleSettingsManager->count());
  settings.setValue("settingsDefinition", this->mp_ChartCycleSettingsManager->exportSettings());
  settings.endGroup();
};
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

#include "mokkaConfigure.h"
#include "MainWindow.h"
#include "About.h"
#include "Acquisition.h"
#include "FileInfoDockWidget.h"
#include "ImportAssistantDialog.h"
#include "Metadata.h"
#include "ModelDockWidget.h"
#include "ProgressWidget.h"
#include "UndoCommands.h"
#include "UserRoles.h"
#include "UpdateChecker.h"
#include "Preferences.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QCloseEvent>
#include <QMessageBox>
#include <QSettings>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QUndoStack>

MainWindow::MainWindow(QWidget* parent)
:QMainWindow(parent), m_LastDirectory(".")
{
  // Members
  this->mp_Acquisition = new Acquisition(this);
  this->mp_MetadataDlg = new Metadata(this);
  this->mp_ModelDock = new ModelDockWidget(this);
  this->mp_FileInfoDock = new FileInfoDockWidget(this);
  this->mp_ImportAssistant = new ImportAssistantDialog(this);
  this->mp_UpdateChecker = new UpdateChecker(xstr(MOKKA_VERSION_STRING), "http://b-tk.googlecode.com/svn/latestMokka",
                                             ":/Resources/Images/Mokka_128.png", this);
#ifdef Q_OS_MAC
  this->mp_Preferences = new Preferences(0); // No parent: to be independant of the main window
#else
  this->mp_Preferences = new Preferences(this);
#endif
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
  actionModelDockView->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_M));
  this->menuView->addAction(actionModelDockView);
  this->menuSettings->addMenu(this->multiView->groundOrientationMenu());
  this->menuSettings->addMenu(this->multiView->markerTrajectoryLengthMenu());
  this->menuSettings->addMenu(this->timeEventControler->playbackSpeedMenu());
  QAction* actionInformationsDockView = this->mp_FileInfoDock->toggleViewAction();
  actionInformationsDockView->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_I));
  this->menuView->addAction(actionInformationsDockView);
  this->menuView->addAction(this->actionViewMetadata);
  this->timeEventControler->playbackSpeedMenu()->menuAction()->setEnabled(true);
#ifdef Q_OS_MAC
  this->mp_Preferences->setMenuBar(this->menuBar()); // After setupUi
  QFont f = this->font();
  f.setPointSize(10);
  this->mp_FileInfoDock->setFont(f);
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
  this->timeEventControler->acquisitionOptionsButtonMenu->menu()->insertMenu(this->timeEventControler->playbackSpeedMenu()->menuAction(), this->multiView->groundOrientationMenu());
  this->timeEventControler->acquisitionOptionsButtonMenu->menu()->insertMenu(this->multiView->groundOrientationMenu()->menuAction(), this->multiView->markerTrajectoryLengthMenu());
  this->multiView->initialize();
  QList<QAction*> actions;
  actions.push_back(this->mp_ModelDock->selectAllMarkersAction());
  actions.push_back(this->mp_ModelDock->hideSelectedMarkersAction());
  actions.push_back(this->mp_ModelDock->unhideSelectedMarkersAction());
  actions.push_back(this->mp_ModelDock->activeTrajectorySelectedMarkersAction());
  actions.push_back(this->mp_ModelDock->unactiveTrajectorySelectedMarkersAction());
  actions.push_back(this->mp_ModelDock->deleteSelectedMarkersAction());
  QAction* sep1 = new QAction(this); sep1->setSeparator(true);
  actions.push_back(sep1);
  actions.push_back(this->timeEventControler->insertEventMenu()->menuAction());
  actions.push_back(this->timeEventControler->playbackSpeedMenu()->menuAction());
  this->multiView->setViewActions(actions);
  //this->mp_ImportAssistant->resize(this->mp_ImportAssistant->width(), this->mp_ImportAssistant->height());
  this->mp_ImportAssistant->layout()->setSizeConstraint(QLayout::SetFixedSize);
  
  // Setting the acquisition
  this->multiView->setAcquisition(this->mp_Acquisition);
  this->mp_ModelDock->setAcquisition(this->mp_Acquisition);
  this->mp_FileInfoDock->setAcquisition(this->mp_Acquisition);
  this->timeEventControler->setAcquisition(this->mp_Acquisition);
  
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
  connect(this->actionImportRIC, SIGNAL(triggered()), this, SLOT(importRIC()));
  connect(this->actionImportRAH, SIGNAL(triggered()), this, SLOT(importRAH()));
  connect(this->actionImportRAW, SIGNAL(triggered()), this, SLOT(importRAW()));
  connect(this->actionImportANG, SIGNAL(triggered()), this, SLOT(importANG()));
  connect(this->actionImportMOM, SIGNAL(triggered()), this, SLOT(importMOM()));
  connect(this->actionImportPWR, SIGNAL(triggered()), this, SLOT(importPWR()));
  connect(this->actionImportGR, SIGNAL(triggered()), this, SLOT(importGRx()));
  connect(this->actionExportC3D, SIGNAL(triggered()), this, SLOT(exportC3D()));
  connect(this->actionExportTRC, SIGNAL(triggered()), this, SLOT(exportTRC()));
  connect(this->actionExportANB, SIGNAL(triggered()), this, SLOT(exportANB()));
  connect(this->actionExportANC, SIGNAL(triggered()), this, SLOT(exportANC()));
  connect(this->actionPreferences, SIGNAL(triggered()), this, SLOT(showPreferences()));
  // MultiView
  connect(this->multiView, SIGNAL(fileDropped(QString)), this, SLOT(openFileDropped(QString)));
  connect(this->multiView, SIGNAL(visibleMarkersChanged(QVector<int>)), this->mp_ModelDock, SLOT(updateDisplayedMarkers(QVector<int>)));
  connect(this->multiView, SIGNAL(pickedMarkerChanged(int)), this, SLOT(selectPickedMarker(int)));
  connect(this->multiView, SIGNAL(pickedMarkerToggled(int)), this, SLOT(togglePickedMarker(int)));
  connect(this->multiView, SIGNAL(selectedMarkersToggled(QList<int>)), this, SLOT(selectSelectedMarkers(QList<int>)));
  connect(this->multiView, SIGNAL(trajectoryMarkerToggled(int)), this, SLOT(toggleMarkerTrajectory(int)));
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
  connect(this->mp_ModelDock, SIGNAL(markerSelectionChanged(QList<int>)), this->multiView, SLOT(circleSelectedMarkers(QList<int>)));
  connect(this->mp_ModelDock, SIGNAL(markerHiddenSelectionChanged(QList<int>)), this->multiView, SLOT(updateHiddenMarkers(QList<int>)));
  connect(this->mp_ModelDock, SIGNAL(markerTrajectorySelectionChanged(QList<int>)), this->multiView, SLOT(updateTrackedMarkers(QList<int>)));
  connect(this->mp_ModelDock->markerRadiusSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateSelectedMarkersRadius(double)));
  // Time Event
  connect(this->timeEventControler, SIGNAL(currentFrameChanged(int)), this->multiView, SLOT(updateDisplay(int)));
  connect(this->timeEventControler, SIGNAL(regionOfInterestChanged(int,int)), this, SLOT(setRegionOfInterest(int,int)));
  connect(this->timeEventControler, SIGNAL(eventFrameModified(int,int)), this, SLOT(setEventFrame(int,int)));
  connect(this->timeEventControler, SIGNAL(eventsModified(QList<int>, QList<Event*>)), this, SLOT(setEvents(QList<int>, QList<Event*>)));
  connect(this->timeEventControler, SIGNAL(eventsRemoved(QList<int>)), this, SLOT(removeEvents(QList<int>)));
  connect(this->timeEventControler, SIGNAL(eventInserted(Event*)), this, SLOT(insertEvent(Event*)));
  connect(this->timeEventControler, SIGNAL(playbackStarted()), this->multiView, SLOT(forceRubberBandDrawingOff()));
  connect(this->timeEventControler, SIGNAL(playbackStopped()), this->multiView, SLOT(forceRubberBandDrawingOn()));
  
#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
  this->menuHelp->addSeparator();
  QAction* actionCheckUpdate = this->menuHelp->addAction(tr("Check for Updates..."));
  actionCheckUpdate->setMenuRole(QAction::ApplicationSpecificRole);
  connect(actionCheckUpdate, SIGNAL(triggered()), this->mp_UpdateChecker, SLOT(check()));
#endif

  // Event filter
  this->multiView->installEventFilter(this);
  this->mp_ModelDock->installEventFilter(this);
  this->timeEventControler->installEventFilter(this);
  this->mp_ModelDock->modelTree->installEventFilter(this);
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
  
  // Settings
  QCoreApplication::setOrganizationName("BTK");
  QCoreApplication::setOrganizationDomain("btk.org");
  QCoreApplication::setApplicationName("Mokka");
  this->readSettings();
  this->setCurrentFile("");
  
#if defined(Q_OS_WIN) || defined(Q_OS_MAC)
  #if defined(NDEBUG)
    QSettings settings;
    if (settings.value("Preferences/checkUpdateStartup", true).toBool())
      this->mp_UpdateChecker->check(true);
  #endif
#endif

  // Preferences connections. Must be set after the reading of the settings.
  connect(this->mp_Preferences, SIGNAL(useDefaultConfigurationStateChanged(bool)), this, SLOT(setPreferenceUseDefaultConfiguration(bool)));
  connect(this->mp_Preferences, SIGNAL(defaultConfigurationPathChanged(QString)), this, SLOT(setPreferenceDefaultConfigurationPath(QString)));
  connect(this->mp_Preferences, SIGNAL(defaultGroundOrientationChanged(int)), this, SLOT(setPreferenceDefaultOrientation(int)));
  connect(this->mp_Preferences, SIGNAL(useEventEditorWhenInsertingStateChanged(bool)), this, SLOT(setPreferenceUseEventEditorWhenInserting(bool)));
  connect(this->mp_Preferences, SIGNAL(defaultMarkerColorChanged(QColor)), this, SLOT(setPreferenceDefaultMarkerColor(QColor)));
  connect(this->mp_Preferences, SIGNAL(defaultMarkerRadiusChanged(double)), this, SLOT(setPreferenceDefaultMarkerRadius(double)));
  connect(this->mp_Preferences, SIGNAL(defaultMarkerTrajectoryLengthChanged(int)), this, SLOT(setPreferenceDefaultTrajectoryLength(int)));
  connect(this->mp_Preferences, SIGNAL(showForcePlatformAxesChanged(int)), this, SLOT(setPreferenceShowForcePlatformAxes(int)));
  connect(this->mp_Preferences, SIGNAL(showForcePlatformIndexChanged(int)), this, SLOT(setPreferenceShowForcePlatformIndex(int)));
  connect(this->mp_Preferences, SIGNAL(defaultForcePlateColorChanged(QColor)), this, SLOT(setPreferenceDefaultForcePlateColor(QColor)));
  connect(this->mp_Preferences, SIGNAL(defaultForceVectorColorChanged(QColor)), this, SLOT(setPreferenceDefaultForceVectorColor(QColor)));
  connect(this->mp_Preferences, SIGNAL(automaticCheckUpdateStateChanged(bool)), this, SLOT(setPreferenceAutomaticCheckUpdate(bool)));
};

MainWindow::~MainWindow()
{
  delete this->mp_Acquisition;
#ifdef Q_OS_MAC
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
                         tr("Acquisition Files (*.c3d *.emf *.rah *.raw *.ric *.trc *.trb);; \
                             C3D Files (*.c3d);; \
                             EMF Ascension Files (*.emf);; \
                             RAH Files (*.rah);; \
                             RAW Files (*.raw);; \
                             RIC Files (*.ric);; \
                             TRB Files (*.trb);; \
                             TRC Files (*.trc)"));
    if (!filename.isEmpty())
      this->openFile(filename);
  }
};

void MainWindow::openFileDropped(const QString& filename)
{
  if (this->isOkToContinue())
    this->openFile(filename);
};

void MainWindow::openFile(const QString& filename)
{
  ProgressWidget pw(this);
  pw.show();
  pw.setProgressValue(10);
  QString errMsg = this->mp_Acquisition->load(filename);
  this->loadAcquisition(errMsg, &pw);
  if (errMsg.isEmpty())
  {
    this->setCurrentFile(filename);
    this->m_LastDirectory = QFileInfo(filename).absolutePath();
  }
};

void MainWindow::loadAcquisition(const QString& errMsg, ProgressWidget* pw)
{
  QApplication::setOverrideCursor(Qt::WaitCursor);
  if (!errMsg.isEmpty())
  {
    pw->hide();
    QApplication::restoreOverrideCursor();
    QMessageBox error(QMessageBox::Warning, "File error", "Error occurred during the file reading.", QMessageBox::Ok , this);
#ifdef Q_OS_MAC
    error.setWindowFlags(Qt::Sheet);
    error.setWindowModality(Qt::WindowModal);
#endif
    error.setDefaultButton(QMessageBox::Ok);
    error.setInformativeText(errMsg);
    error.exec();
    return;
  }
  this->reset();
  
  pw->setProgressValue(40);
  
  this->multiView->load();
  this->multiView->updateDisplay(this->mp_Acquisition->firstFrame()); // Required
  
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
  this->menuExport->menuAction()->setEnabled(true);
};

void MainWindow::saveFile()
{
  this->saveFile(this->m_RecentFiles.first());
};

void MainWindow::saveAsFile()
{
  QString file = this->m_RecentFiles.first();
  QString suffix = QFileInfo(file).suffix();
  QString selectedFilter = suffix.toUpper() + " Files (*." + suffix.toLower() + ")";
  QString filename = QFileDialog::getSaveFileName(this, "",
                       file,
                       tr("C3D Files (*.c3d);; \
                           TRC Files (*.trc)"),
                       &selectedFilter);
  if (!filename.isEmpty())
  {
    this->m_LastDirectory = QFileInfo(filename).absolutePath();
    this->saveFile(filename);
  }
};

void MainWindow::saveFile(const QString& filename)
{
  QApplication::setOverrideCursor(Qt::WaitCursor);
  
  QMap<int, QVariant> properties;
  this->setAcquisitionProperties(properties);
  QString errMsg = this->mp_Acquisition->save(filename,properties);
  if (!errMsg.isEmpty())
  {
    QApplication::restoreOverrideCursor();
    QMessageBox error(QMessageBox::Warning, "File error", "Error occurred during the file saving.", QMessageBox::Ok , this);
#ifdef Q_OS_MAC
    error.setWindowFlags(Qt::Sheet);
    error.setWindowModality(Qt::WindowModal);
#endif
    error.setInformativeText(errMsg);
    error.exec();
    return;
  }
  this->mp_AcquisitionUndoStack->setClean();
  this->setCurrentFile(filename);
  QApplication::restoreOverrideCursor();
  this->setWindowModified(false);
};

void MainWindow::closeFile()
{
  if (this->isOkToContinue() && this->mp_ModelDock->isOkToContinue())
  {
    this->reset();
    this->mp_FileInfoDock->reset();
    this->mp_Acquisition->clear();
  }
}

void MainWindow::importAssistant()
{
  this->mp_ModelDock->setEnabled(false);
  this->mp_ImportAssistant->clear(this->m_LastDirectory);
  this->mp_ImportAssistant->appendAcquisitionRadioButton->setEnabled(this->actionClose->isEnabled());
  if (this->mp_ImportAssistant->exec())
  {
    if (this->mp_ImportAssistant->newAcquisitionRadioButton->isChecked())
      this->mp_Acquisition->clear();
    this->importAcquisitions(this->mp_ImportAssistant->filenames());
  }
  this->mp_ModelDock->setEnabled(true);
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

void MainWindow::importAcquisitions(const QStringList& filenames)
{
  if (!filenames.isEmpty())
  {
    QString title = this->windowTitle();
    QString importWarnings;
    ProgressWidget pw(this);
    pw.show();
    pw.setProgressValue(10);
    QString errMsg = this->mp_Acquisition->importFrom(filenames, importWarnings);
    this->loadAcquisition(errMsg, &pw);
    this->setWindowTitle(title);
    this->setWindowModified(true);
    pw.hide();
    if (!importWarnings.isEmpty())
    {
      QMessageBox error(QMessageBox::Warning, tr("Import warnings"), tr("Warnings appears during importing."), QMessageBox::Ok , this);
#ifdef Q_OS_MAC
      error.setWindowFlags(Qt::Sheet);
      error.setWindowModality(Qt::WindowModal);
#endif
      error.setInformativeText(importWarnings);
      error.exec();
    }
  }
}

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

void MainWindow::showPreferences()
{
#ifdef Q_OS_MAC
  this->mp_Preferences->hide(); // Force the preferences to go back to the top.
  this->mp_Preferences->show();
#else
  if (this->mp_Preferences->exec())
    this->mp_Preferences->saveSettings();
#endif
};

void MainWindow::exportAcquisition(const QString& filter)
{
  QString filename = QFileDialog::getSaveFileName(this, "",
                       this->m_LastDirectory,
                       filter);
  if (!filename.isEmpty())
  {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QMap<int, QVariant> properties;
    this->setAcquisitionProperties(properties);
    QString errMsg = this->mp_Acquisition->exportTo(filename, properties, this->timeEventControler->leftBound(), this->timeEventControler->rightBound());
    if (!errMsg.isEmpty())
    {
      QApplication::restoreOverrideCursor();
      QMessageBox error(QMessageBox::Warning, "File error", "Error occurred during the file exporting.", QMessageBox::Ok , this);
#ifdef Q_OS_MAC
      error.setWindowFlags(Qt::Sheet);
      error.setWindowModality(Qt::WindowModal);
#endif
      error.setInformativeText(errMsg);
      error.exec();
      return;
    }
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
  this->menuExport->menuAction()->setEnabled(false);
  this->setCurrentFile("");
  // Time & Event Controler
  this->timeEventControler->reset();
  this->timeEventControler->setEnabled(false);
  // Model dock
  this->mp_ModelDock->reset();
  this->mp_ModelDock->setVisible(false);
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

void MainWindow::setPreferenceDefaultMarkerColor(const QColor& color)
{
  QSettings settings;
  settings.setValue("Preferences/defaultMarkerColor", color);
  
  QTreeWidgetItem* markersRoot = this->mp_ModelDock->modelTree->topLevelItem(0);
  QVector<int> ids;
  for (int i = 0 ; i < markersRoot->childCount() ; ++i)
  {
    int id = markersRoot->child(i)->data(0,pointId).toInt();
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
  this->mp_Preferences->setPreference(Preferences::DefaultMarkerColor, defaultMarkerColor);
  this->mp_Preferences->setPreference(Preferences::DefaultMarkerRadius, defaultMarkerRadius);
  this->mp_Preferences->setPreference(Preferences::DefaultTrajectoryLength, defaultMarkerTrajectoryLength);
  this->mp_Preferences->setPreference(Preferences::ForcePlatformAxesDisplay, showForcePlatformAxes);
  this->mp_Preferences->setPreference(Preferences::ForcePlatformIndexDisplay, showForcePlatformIndex);
  this->mp_Preferences->setPreference(Preferences::DefaultForcePlateColor, defaultForcePlateColor);
  this->mp_Preferences->setPreference(Preferences::DefaultForceVectorColor, defaultForceVectorColor);
  this->mp_Preferences->setPreference(Preferences::AutomaticCheckUpdateUse, checkUpdateStartup);
#endif  
  
  if (defaultConfigurationUsed && !defaultConfigurationPath.isEmpty())
    this->mp_ModelDock->loadConfiguration(defaultConfigurationPath);
  this->timeEventControler->setOpenEditorWhenInsertingEventFlag(openEventEditorWhenInserting);
  this->multiView->setDefaultGroundOrientation(defaultPlaneOrientation);
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
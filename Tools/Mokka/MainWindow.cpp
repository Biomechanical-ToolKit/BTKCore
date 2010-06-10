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

#include "MainWindow.h"
#include "Metadata.h"
#include "About.h"
#include "ProgressWidget.h"
#include "NewModelConfigurationWidget.h"
#include "UndoCommands.h"
#include "UserRoles.h"

#include <btkAcquisitionFileReader.h>
#include <btkAcquisitionFileWriter.h>

#include <QFileDialog>
#include <QFileInfo>
#include <QDateTime>
#include <QCloseEvent>
#include <QTableWidgetItem>
#include <QColorDialog>
#include <QMessageBox>
#include <QSettings>
#include <QDesktopServices>
#include <QXmlStreamReader>
#include <QTemporaryFile>

MainWindow::MainWindow(QWidget* parent)
:QMainWindow(parent), mp_Acquisition(), m_LastDirectory(".")
{
  // Members
  this->m_SelectedMarkerConfiguration = -1;
  this->mp_Timer = new QTimer(this);
  this->mp_MetadataDlg = new Metadata(this);
  this->mp_PointsEditorDlg = new PointsEditor(this);
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
  this->modelConfigurationComboBox->insertSeparator(99);
  this->modelConfigurationComboBox->insertItem(99,"New ...");
  this->markerProperties->setVisible(false);
  this->markerPropertiesButton->setIcon(*this->mp_RightArrow);
  this->eventsDock->setVisible(false);
  this->eventInformations->setVisible(false);
  this->eventInformationsButton->setIcon(*this->mp_RightArrow);
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
  
  // Viz3D
  this->qvtkWidget->initialize();

  // Qt UI: Undo/Redo
  this->mp_UndoStack = new QUndoStack(this); // One to command all.
  this->mp_MarkerConfigurationUndoStack = new QUndoStack(this);
  connect(this->mp_MarkerConfigurationUndoStack, SIGNAL(indexChanged(int)), this, SLOT(setMarkerConfigurationModified(int)));
  this->mp_AcquisitionUndoStack = new QUndoStack(this);
  connect(this->mp_AcquisitionUndoStack, SIGNAL(indexChanged(int)), this, SLOT(setAcquisitionModified(int)));
  //QAction* actionUndo = new QAction(tr("Undo"), this);
  QAction* actionUndo = this->mp_UndoStack->createUndoAction(this);
  actionUndo->setShortcut(QKeySequence::Undo);
  QAction* actionRedo = this->mp_UndoStack->createRedoAction(this);
  //QAction* actionRedo = new QAction(tr("Redo"), this);
  actionRedo->setShortcut(QKeySequence::Redo);
  this->menuEdit->insertAction(this->actionCut, actionUndo);
  this->menuEdit->insertAction(this->actionCut, actionRedo);
  this->menuEdit->insertSeparator(this->actionCut);

  // Qt Signal/Slot connection
  // Menu
  connect(this->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
  connect(this->actionVisit_BTK_website, SIGNAL(triggered()), this, SLOT(visitBTKWebsite()));
  connect(this->actionEdit_Metadata, SIGNAL(triggered()), this, SLOT(editMetadata()));
  connect(this->actionEdit_Points, SIGNAL(triggered()), this, SLOT(editPoints()));
  connect(this->action_FileOpen, SIGNAL(triggered()), this, SLOT(openFile()));
  connect(this->actionSave, SIGNAL(triggered()), this, SLOT(saveFile()));
  connect(this->actionSave_As, SIGNAL(triggered()), this, SLOT(saveAsFile()));
  connect(this->actionClose, SIGNAL(triggered()), this, SLOT(closeFile()));
  connect(this->action_Quit, SIGNAL(triggered()), this, SLOT(close()));
  connect(this->mp_PlaybackSpeedActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(changePlaybackParameters()));
  connect(this->mp_GroundOrientationActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(changeGroundOrientation()));
  // Playback
  connect(this->frameSlider, SIGNAL(valueChanged(int)), this, SLOT(updateActiveEvent(int)));
  connect(this->frameSlider, SIGNAL(valueChanged(int)), this->qvtkWidget, SLOT(updateDisplay(int)));
  connect(this->playButton, SIGNAL(clicked()), this, SLOT(toggleTimer()));
  connect(this->mp_Timer, SIGNAL(timeout()), this, SLOT(displayNextFrame()));
  // Viz3D
  connect(this->qvtkWidget, SIGNAL(fileDropped(QString)), this, SLOT(openFileDropped(QString)));
  connect(this->qvtkWidget, SIGNAL(visibleMarkersChanged(QVector<int>)), this, SLOT(updateDisplayedMarkersList(QVector<int>)));
  connect(this->qvtkWidget, SIGNAL(pickedMarkerChanged(int)), this, SLOT(selectPickedMarker(int)));
  connect(this->qvtkWidget, SIGNAL(pickedMarkersChanged(int)), this, SLOT(selectPickedMarkers(int)));
  // Markers dock
  connect(this->modelConfigurationComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(selectMarkerConfiguration(int)));
  connect(this->saveModelConfigurationButton, SIGNAL(clicked()), this, SLOT(saveMarkerConfiguration()));
  connect(this->loadModelConfigurationButton, SIGNAL(clicked()), this, SLOT(loadMarkerConfiguration()));
  connect(this->deleteModelConfigurationButton, SIGNAL(clicked()), this, SLOT(eraseMarkerConfiguration()));
  connect(this->markersTable, SIGNAL(itemChanged(QTableWidgetItem*)), this->qvtkWidget, SLOT(updateMarkerVisibility(QTableWidgetItem*)));
  connect(this->markersTable, SIGNAL(itemSelectionChanged()), this, SLOT(displayMarkerProperties()));
  connect(this->markersTable, SIGNAL(itemSelectionChanged()), this, SLOT(toggleMarkersVisibilityButtons()));
  connect(this->markersTable, SIGNAL(itemSelectionChanged()), this, SLOT(circleSelectedMarkers()));
  connect(this->markersTable, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(focusOnMarkerLabelEdition()));
  connect(this->markerRadiusSlider, SIGNAL(valueChanged(int)), this, SLOT(updateMarkerRadiusSpinBox(int)));
  connect(this->markerRadiusSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateMarkerRadius(double)));
  connect(this->showMarkersButton, SIGNAL(clicked()), this, SLOT(showAllMarkers()));
  connect(this->hideMarkersButton, SIGNAL(clicked()), this, SLOT(hideAllMarkers()));
  connect(this->markerPropertiesButton, SIGNAL(clicked()), this, SLOT(toggleMarkerProperties()));
  connect(this->markerLabelEdit, SIGNAL(editingFinished()), this, SLOT(editMarkerLabel()));
  connect(this->markerDescEdit, SIGNAL(editingFinished()), this, SLOT(editMarkerDescription()));
  connect(this->markerRadiusSpinBox, SIGNAL(editingFinished()), this, SLOT(editMarkerRadius()));
  connect(this->markerRadiusSlider, SIGNAL(sliderReleased()), this, SLOT(editMarkerRadius()));
  connect(this->markerColorButton, SIGNAL(clicked(bool)), this, SLOT(editMarkerColor()));
  connect(this->markersDock, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)), this, SLOT(markersDockLocationChanged(Qt::DockWidgetArea)));
  // Events dock
  connect(this->eventsTable, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(focusOnEventEdition()));
  connect(this->eventsTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(updateEventInternalInformations(QTableWidgetItem*)));
  connect(this->eventsTable, SIGNAL(itemSelectionChanged()), this, SLOT(displayEventInformations()));
  connect(this->eventsTable, SIGNAL(itemSelectionChanged()), this, SLOT(updateEventsButtonsState()));
  connect(this->showEventButton, SIGNAL(clicked()), this, SLOT(showEvent()));
  connect(this->newEventButton, SIGNAL(clicked()), this, SLOT(newEvent()));
  connect(this->deleteEventButton, SIGNAL(clicked()), this, SLOT(deleteEvent()));
  connect(this->eventLabelEdit, SIGNAL(editingFinished()), this, SLOT(editEventLabel()));
  connect(this->eventContextCombo, SIGNAL(activated(QString)), this, SLOT(editEventContext(QString)));
  connect(this->eventTimeSpinBox, SIGNAL(editingFinished()), this, SLOT(editEventTime()));
  connect(this->eventSubjectEdit, SIGNAL(editingFinished()), this, SLOT(editEventSubject()));
  connect(this->eventsDock, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)), this, SLOT(eventsDockLocationChanged(Qt::DockWidgetArea)));
  connect(this->eventInformationsButton, SIGNAL(clicked()), this, SLOT(toggleEventInformations()));

  // Event filter
  this->qvtkWidget->installEventFilter(this);
  this->frameSlider->installEventFilter(this);
  this->markersTable->installEventFilter(this);
  this->eventsTable->installEventFilter(this);
  this->markerRadiusSpinBox->installEventFilter(this);
  this->markerLabelEdit->installEventFilter(this);
  this->markerDescEdit->installEventFilter(this);
  this->eventLabelEdit->installEventFilter(this);
  this->eventContextCombo->installEventFilter(this);
  this->eventTimeSpinBox->installEventFilter(this);
  this->eventSubjectEdit->installEventFilter(this);

  // Settings
  QCoreApplication::setOrganizationName("BTK");
  QCoreApplication::setOrganizationDomain("btk.org");
  QCoreApplication::setApplicationName("Mokka");
  this->readSettings();
  this->setCurrentFile("");
};

MainWindow::~MainWindow()
{
  delete this->mp_PlayIcon;
  delete this->mp_PauseIcon;
  delete this->mp_DownArrow;
  delete this->mp_RightArrow;
};

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
  QLineEdit* lineEdit = qobject_cast<QLineEdit*>(obj);
  if (lineEdit)
  {
    if (event->type() == QEvent::KeyPress)
    {
      QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
      if (keyEvent->key() == Qt::Key_Escape)
      {
        while (lineEdit->isUndoAvailable())
          lineEdit->undo();
        if ((obj->objectName().compare("markerLabelEdit") == 0) || (obj->objectName().compare("markerDescEdit") == 0))
          this->markersTable->setFocus();
        else if (obj->objectName().compare("eventSubjectEdit") == 0)
          this->eventsTable->setFocus();
        else if (obj->objectName().compare("eventLabelEdit") == 0)
        {
          this->eventLabelEdit->setText(this->eventsTable->item(this->eventsTable->currentItem()->row(), 2)->text());
          this->eventsTable->setFocus();
        }
      }
      else if (keyEvent->matches(QKeySequence::Undo) && !lineEdit->isUndoAvailable())
      {
        if (obj->objectName().compare("eventLabelEdit") == 0)
        {
          this->eventLabelEdit->blockSignals(true);
          this->eventLabelEdit->setText(this->eventsTable->item(this->eventsTable->currentItem()->row(), 2)->text());
          this->eventLabelEdit->text();
          this->eventLabelEdit->blockSignals(false);
        }
        this->mp_UndoStack->undo();
      }
      else if (keyEvent->matches(QKeySequence::Redo) && !lineEdit->isRedoAvailable())
        this->mp_UndoStack->redo();
    }
    return false;
  }
  else if ((event->type() == QEvent::KeyPress) && this->frameSlider->isEnabled())
  {
    QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
    // special case for spinbox widget: 'markerRadiusSpinBox' & 'eventTimeSpinBox'
    QDoubleSpinBox* spinBox = qobject_cast<QDoubleSpinBox*>(obj);
    if (spinBox)
    {
      if (keyEvent->key() == Qt::Key_Escape)
      {
        if (obj->objectName().compare("markerRadiusSpinBox") == 0)
          this->markersTable->setFocus();
        else if (obj->objectName().compare("eventTimeSpinBox") == 0)
          this->eventsTable->setFocus();
        else
          return true;
      }  
      if (keyEvent->matches(QKeySequence::Undo))
        this->mp_UndoStack->undo();
      if (keyEvent->matches(QKeySequence::Redo))
        this->mp_UndoStack->redo();
      return false;
      
    }
    else if (obj->objectName().compare("eventContextCombo") == 0)
    {
      if (keyEvent->key() == Qt::Key_Escape)
      {
        this->eventsTable->setFocus();
        return true;
      }
      else
        return false;
    }
    // general case
    else if (keyEvent->matches(QKeySequence::MoveToPreviousChar))
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
    else if ((keyEvent->key() == Qt::Key_Return) || (keyEvent->key() == Qt::Key_Enter))
    {
      if (obj->objectName().compare("eventsTable") == 0)
        this->focusOnEventEdition(2);
      else
        this->focusOnMarkerLabelEdition();
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
#ifdef Q_OS_MAC
  QFont f = aboutDlg.rights->font();
  f.setPointSize(11);
  aboutDlg.release->setFont(f);
  aboutDlg.copyright->setFont(f);
  aboutDlg.rights->setFont(f);
#endif
  aboutDlg.exec();
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
  }
};

void MainWindow::editMetadata()
{
  this->mp_MetadataDlg->exec();
};

void MainWindow::editPoints()
{
  int num = this->markersTable->rowCount();
  for (int i = 0 ; i < num ; ++i)
  {
    QTableWidgetItem* source = this->markersTable->item(i,0);
    QTableWidgetItem* target = this->mp_PointsEditorDlg->tableWidget->item(i,0);
    target->setText(source->text());
    target->setData(pointId, source->data(pointId));
    target->setData(pointType, source->data(pointType));
    target->setData(pointDisabled, source->data(pointDisabled));
    this->mp_PointsEditorDlg->tableWidget->item(i,1)->setText(source->data(pointDescription).toString());
    target->setCheckState(source->data(pointDisabled).toBool() ? Qt::Unchecked : Qt::Checked);
  }
  this->mp_PointsEditorDlg->tableWidget->resizeColumnsToContents();
  this->mp_PointsEditorDlg->setWindowModified(false);
  this->mp_PointsEditorDlg->tableWidget->clearSelection();
  int ret = this->mp_PointsEditorDlg->exec();
  if ((ret == QDialog::Accepted) && (this->mp_PointsEditorDlg->isWindowModified()))
    this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new EditPoints(this)));
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
    bool acquisitionModified = false;
    for (int i = 0 ; i < this->mp_AcquisitionUndoStack->index() ; ++i)
    {
      if (static_cast<const UndoCommand*>(this->mp_AcquisitionUndoStack->command(i))->commandType() == UndoCommand::Acquisition)
      {
        acquisitionModified = true;
        break;
      }
    }
    if (acquisitionModified)
    {
      QMessageBox messageBox(QMessageBox::Question, 
                             trUtf8("Mokka"),
                             trUtf8("The document has been modified.\nDo you want to save your changes?"), 
                             QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                             this, Qt::Sheet);
      messageBox.setDefaultButton(QMessageBox::Yes);
      messageBox.setEscapeButton(QMessageBox::Cancel);
      switch(messageBox.exec())
      {
          case QMessageBox::Yes:
              this->saveFile();
              break;
          case QMessageBox::Cancel:
              return false;
              break;
      }
    }
  } 
  return true; 
};

void MainWindow::openFile()
{
  /*
  QFileDialog open(this,
                   trUtf8("Open Acquisition"),
                   this->m_LastDirectory,
                   trUtf8("Acquisition Files (*.c3d *.trc)"));
  if (open.exec())
  {
    this->m_LastDirectory = open.directory().absolutePath();
    QStringList filenames = open.selectedFiles();
    if (!filenames.isEmpty())
      this->openFile(filenames.first());
  }
  */
  if (this->isOkToContinue())
  {
    QString filename = QFileDialog::getOpenFileName(this,
                         trUtf8("Open Acquisition"),
                         this->m_LastDirectory,
                         trUtf8("Acquisition Files (*.c3d *.trc)"));
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
  QApplication::setOverrideCursor(Qt::WaitCursor);
  ProgressWidget pw(this);
  pw.show();
  this->clearUI();
  btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
  reader->SetFilename(filename.toStdString());
  QMessageBox error(QMessageBox::Critical, "File error", "Error occurred during the file reading", QMessageBox::Ok , this);
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
  
  pw.setProgressValue(25);
  
  this->setCurrentFile(filename);
  this->mp_Acquisition = reader->GetOutput();
  
  this->changePlaybackParameters();
  this->fillFileInformations(filename, reader->GetAcquisitionIO(), this->mp_Acquisition);
  // UI settings
  // Update the 3D view
  btk::SeparateKnownVirtualMarkersFilter::Pointer separator = this->qvtkWidget->load(this->mp_Acquisition);
  this->qvtkWidget->updateDisplay(this->mp_Acquisition->GetFirstFrame()); // Required
  
  pw.setProgressValue(70);
  
  // Markers
  this->markersTable->blockSignals(true);
  btk::PointCollection::Pointer points = this->mp_Acquisition->GetPoints();
  this->markersTable->setRowCount(points->GetItemNumber());
  this->mp_PointsEditorDlg->tableWidget->setRowCount(this->markersTable->rowCount());
  int incPointId = 0, incMarkerId = 0;
  bool hasMarker = false;
  for (btk::PointCollection::ConstIterator it = points->Begin() ; it != points->End() ; ++it)
  {
    QTableWidgetItem* labelItem = new QTableWidgetItem();
    this->markersTable->setItem(incPointId, 0, labelItem);
//this->markersTable->setIndexWidget(this->markersTable->model()->index(incPointId, 1), trajcb);
    labelItem->setText(QString::fromStdString((*it)->GetLabel()));
    labelItem->setData(pointId, incPointId);
    labelItem->setData(pointType, static_cast<int>((*it)->GetType()));
    labelItem->setData(pointLabel, labelItem->text());
    labelItem->setData(pointDescription, QString::fromStdString((*it)->GetDescription()));
    labelItem->setData(pointDisabled, false);
    bool isMarker = findLabel(separator->GetOutput(0), (*it)->GetLabel());
    bool isOtherVirtualMarker = findLabel(separator->GetOutput(2), (*it)->GetLabel());
    if (((*it)->GetType() == btk::Point::Marker) && (isMarker || isOtherVirtualMarker))
    {
      labelItem->setData(markerId, incMarkerId);
      if (isMarker)
        labelItem->setCheckState(Qt::Checked);
      else
      {
        labelItem->setCheckState(Qt::Unchecked);
        this->qvtkWidget->setMarkerVisibility(incMarkerId, false);
      }
      labelItem->setData(markerRadius, 8.0); // TODO: Use default radius from preferences.
      labelItem->setData(markerColorIndex, (int)0); // TODO: Use default color from preferences.
      labelItem->setData(markerTrajectoryActived, false);
      labelItem->setIcon(QIcon(":/Resources/Images/trajectory_unactive.png"));
      this->markersTable->setRowHidden(incPointId, false);
      ++incMarkerId;
    }
    else
      this->markersTable->setRowHidden(incPointId, true);
    this->mp_PointsEditorDlg->tableWidget->setItem(incPointId, 0, new PointEditorTableWidgetItem());
    //TableWidgetItem* pointDescItem = new TableWidgetItem();
    //pointDescItem->setFlags(pointDescItem->flags() & ~(Qt::ItemIsDropEnabled) & ~(Qt::ItemIsSelectable));
    //this->mp_PointsEditorDlg->tableWidget->setItem(incPointId, 1, pointDescItem);
    this->mp_PointsEditorDlg->tableWidget->setItem(incPointId, 1, new PointEditorTableWidgetItem());
    ++incPointId;
    
    if ((*it)->GetType() == btk::Point::Marker)
      hasMarker = true;
  }
  //this->markersTable->resizeColumnsToContents();
  this->markersTable->blockSignals(false);
  this->showMarkersButton->setEnabled(true);
  this->hideMarkersButton->setEnabled(true);
  
  pw.setProgressValue(80);
  
  // Events
  btk::EventCollection::Pointer events = this->mp_Acquisition->GetEvents();
  //this->eventContextCombo->clear();
  //this->eventContextCombo->insertItem(0, "");
  this->eventsTable->blockSignals(true);
  this->eventsTable->setColumnCount(4);
  this->eventsTable->setRowCount(events->GetItemNumber());
  //this->eventsTable->setColumnHidden(3, true);
  int row = 0;
  //std::string previousSubject = "";
  QStringList eventLabelWordList;
  double pointFrequency = this->mp_Acquisition->GetPointFrequency();
  for (btk::EventCollection::ConstIterator it = events->Begin() ; it != events->End() ; ++it)
  {
    NumericalTableWidgetItem* timeItem = new NumericalTableWidgetItem();
    QTableWidgetItem* contextItem = new QTableWidgetItem();
    QTableWidgetItem* labelItem = new QTableWidgetItem();
    QTableWidgetItem* subjectItem = new QTableWidgetItem();
    timeItem->setText(QString::number((*it)->GetTime(), 'f', 2));
    timeItem->setData(eventTime, (*it)->GetTime());
    int frameIndex = static_cast<int>((*it)->GetTime() * pointFrequency);
    timeItem->setData(eventFrame, frameIndex);
    labelItem->setText(QString::fromStdString((*it)->GetLabel()));
    labelItem->setData(eventId, (*it)->GetId());
    labelItem->setData(eventDescription, QString::fromStdString((*it)->GetDescription()));
    eventLabelWordList << labelItem->text();
    contextItem->setText(QString::fromStdString((*it)->GetContext()));
    contextItem->setData(eventContext, contextItem->text());
    subjectItem->setText(QString::fromStdString((*it)->GetSubject()));
    this->eventsTable->setItem(row, 0, timeItem);
    this->eventsTable->setItem(row, 1, contextItem);
    this->eventsTable->setItem(row, 2, labelItem);
    this->eventsTable->setItem(row, 3, subjectItem);
    if (this->eventContextCombo->findText(contextItem->text()) == -1)
      this->eventContextCombo->insertItem(this->eventContextCombo->count(), contextItem->text());
    //if ((previousSubject.compare((*it)->GetSubject()) != 0) && !previousSubject.empty())
    //   this->eventsTable->setColumnHidden(3, false);
    //previousSubject = (*it)->GetSubject();
    if ((frameIndex < this->mp_Acquisition->GetFirstFrame()) || (frameIndex > this->mp_Acquisition->GetLastFrame()))
    {
      timeItem->setData(eventVisible, false);
      //timeItem->setForeground(defaultLabelColor);
      //contextItem->setForeground(defaultLabelColor);
      //labelItem->setForeground(defaultLabelColor);
      //subjectItem->setForeground(defaultLabelColor);
    }
    else
      timeItem->setData(eventVisible, true);
    
    ++row;
  }
  eventLabelWordList.removeDuplicates();
  this->eventsTable->resizeColumnsToContents();
  this->eventsTable->horizontalHeader()->resizeSection(0, 35);
  this->eventsTable->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
  this->eventsTable->horizontalHeader()->resizeSection(1, 50);
  this->eventsTable->horizontalHeader()->setResizeMode(1, QHeaderView::Fixed);
  this->eventsTable->horizontalHeader()->resizeSection(3, 50);
  this->eventsTable->horizontalHeader()->setResizeMode(3, QHeaderView::Fixed);
  this->eventsTable->horizontalHeader()->setResizeMode(2, QHeaderView::Stretch);
  this->eventsTable->sortItems(0);
  this->newEventButton->setEnabled(true);
  this->deleteEventButton->setEnabled(false);
  this->showEventButton->setEnabled(false);
  this->eventsTable->blockSignals(false);

  QCompleter* eventLabelCompleter = new QCompleter(eventLabelWordList, this);
  eventLabelCompleter->setCaseSensitivity(Qt::CaseInsensitive);
  eventLabelCompleter->setCompletionMode(QCompleter::InlineCompletion);
  this->eventLabelEdit->setCompleter(eventLabelCompleter);
  // Frames
  this->frameSlider->setMinimum(this->mp_Acquisition->GetFirstFrame());
  this->frameSlider->setMaximum(this->mp_Acquisition->GetLastFrame());
  this->frameSlider->setEnabled(true);
  this->playButton->setEnabled(true);
  
  pw.setProgressValue(90);
  
  // Fill Metadata
  this->mp_MetadataDlg->load(this->mp_Acquisition->GetMetaData());
  // Display Docks
  if (hasMarker != 0)
    this->markersDock->setVisible(true);
  if (events->GetItemNumber() != 0)
    this->eventsDock->setVisible(true);

  pw.setProgressValue(100);

  QApplication::restoreOverrideCursor();
  
  this->frameSlider->setValue(this->frameSlider->minimum());
  this->qvtkWidget->updateDisplay(this->frameSlider->minimum());
  
  this->actionClose->setEnabled(true);
  this->actionEdit_Metadata->setEnabled(true);
  this->actionEdit_Points->setEnabled(true);
  //this->actionSave->setEnabled(true); 
  this->actionSave_As->setEnabled(true);
  
  this->modelConfigurationComboBox->setEnabled(true);
  this->loadModelConfigurationButton->setEnabled(true);
  
  this->m_LastDirectory = QFileInfo(filename).absolutePath();
  
  pw.setProgressValue(100);
};

void MainWindow::saveFile()
{
  this->saveFile(this->m_RecentFiles.first());
};

void MainWindow::saveAsFile()
{
  QString filename = QFileDialog::getSaveFileName(this,
                       trUtf8("Save As Acquisition"),
                       this->m_RecentFiles.first(),
                       trUtf8("C3D Files (*.c3d);;TRC Files (*.trc)"));
  if (!filename.isEmpty())
  {
    this->m_LastDirectory = QFileInfo(filename).absolutePath();
    this->saveFile(filename);
  }
};

void MainWindow::saveFile(const QString& filename)
{
  QApplication::setOverrideCursor(Qt::WaitCursor);
  btk::Acquisition::Pointer source = this->mp_Acquisition;
  btk::Acquisition::Pointer target = btk::Acquisition::New();
  // Acquisition info
  target->SetFirstFrame(source->GetFirstFrame());
  target->SetPointFrequency(source->GetPointFrequency());
  target->SetAnalogResolution(source->GetAnalogResolution());
  // Event
  int eventRowNum = this->eventsTable->rowCount();
  btk::EventCollection::Pointer targetEvents = target->GetEvents();
  for (int i = 0 ; i < eventRowNum ; ++i)
  {
    if (!this->eventsTable->isRowHidden(i))
    {
      btk::Event::Pointer p = btk::Event::New();
      QTableWidgetItem* item = this->eventsTable->item(i, 0);
      p->SetTime(item->data(eventTime).toDouble());
      item = this->eventsTable->item(i, 1);
      p->SetContext(item->text().toStdString());
      item = this->eventsTable->item(i, 2);
      p->SetLabel(item->text().toStdString());
      p->SetId(item->data(eventId).toInt());
      p->SetDescription(item->data(eventDescription).toString().toStdString());
      item = this->eventsTable->item(i, 3);
      p->SetSubject(item->text().toStdString());
      targetEvents->InsertItem(p);
    }
  }
  // Metadata
  target->SetMetaData(source->GetMetaData()->Clone());
  // Point
  int inc = 0;
  int pointRowNum = this->markersTable->rowCount();
  btk::PointCollection::Pointer sourcePoints = source->GetPoints();
  btk::PointCollection::Pointer targetPoints = target->GetPoints();
  for (int i = 0 ; i < pointRowNum ; ++i)
  {
    QTableWidgetItem* item = this->markersTable->item(i, 0);
    if (!item->data(pointDisabled).toBool())
    {
      btk::Point::Pointer p = sourcePoints->GetItem(item->data(pointId).toInt())->Clone() ;
      p->SetLabel(item->data(pointLabel).toString().toStdString());
      p->SetDescription(item->data(pointDescription).toString().toStdString());
      targetPoints->InsertItem(p);
      ++inc;
    }
  }
  // Analog
  target->SetAnalogs(source->GetAnalogs()->Clone());
  // Final setup
  target->Resize(inc, source->GetPointFrameNumber(), source->GetAnalogNumber(), source->GetNumberAnalogSamplePerFrame());
  // BTK writer
  btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
  writer->SetFilename(filename.toStdString());
  writer->SetInput(target);
  QMessageBox error(QMessageBox::Critical, "File error", "Error occurred during the file saving", QMessageBox::Ok , this);
  try
  {
    writer->Update();
  }
  catch (btk::Exception& e)
  {
    QApplication::restoreOverrideCursor();
    error.setInformativeText(e.what());
    error.exec();
    return;
  }
  catch (std::exception& e)
  {
    QApplication::restoreOverrideCursor();
    error.setInformativeText("Unexpected error: " + QString(e.what()));
    error.exec();
    return;
  }
  catch (...)
  {
    QApplication::restoreOverrideCursor();
    error.setInformativeText("Unknown error.");
    error.exec();
    return;
  }
  this->mp_AcquisitionUndoStack->setClean();
  this->setCurrentFile(filename);
  this->fillFileInformations(filename, writer->GetAcquisitionIO(), target);
  QApplication::restoreOverrideCursor();
  this->setWindowModified(false);
};

void MainWindow::closeFile()
{
  if (this->isOkToContinue() && this->isOkToContinue2())
    this->clearUI();
}

void MainWindow::clearUI()
{
  this->mp_AcquisitionUndoStack->clear();
  this->mp_MarkerConfigurationUndoStack->clear();
  this->actionClose->setEnabled(false);
  this->actionEdit_Metadata->setEnabled(false);
  this->actionEdit_Points->setEnabled(false);
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
  this->m_SelectedMarkerConfiguration = -1;
  this->modelConfigurationComboBox->setCurrentIndex(-1);
  this->modelConfigurationComboBox->setEnabled(false);
  this->saveModelConfigurationButton->setEnabled(false);
  this->loadModelConfigurationButton->setEnabled(false);
  this->deleteModelConfigurationButton->setEnabled(false);
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
  // Viz3D
  this->qvtkWidget->clear();
};

void MainWindow::fillFileInformations(const QString& filename, btk::AcquisitionFileIO::Pointer io, btk::Acquisition::Pointer acq)
{
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
  switch(io->GetFileType())
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
  switch(io->GetByteOrder())
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
  switch(io->GetStorageFormat())
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
};

void MainWindow::changePlaybackParameters()
{
  double pointFrequency = this->mp_Acquisition->GetPointFrequency();
  
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
  // Relaunch playback if necessary
  if (this->mp_Timer->isActive())
  {
    this->mp_Timer->stop();
    this->mp_Timer->start(this->m_PlaybackDelay);
  }
};

void MainWindow::changeGroundOrientation()
{
  if (actionPlane_XY->isChecked())
    this->qvtkWidget->setGroundOrientation(0.0, 0.0, 1.0);
  else if (actionPlane_YZ->isChecked())
    this->qvtkWidget->setGroundOrientation(1.0, 0.0, 0.0);
  else if (actionPlane_ZX->isChecked())
    this->qvtkWidget->setGroundOrientation(0.0, 1.0, 0.0);
};

void MainWindow::play()
{
  if (this->mp_Acquisition)
    this->toggleTimer();
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
  this->qvtkWidget->showSelectedMarkers(items);
  this->markersTable->blockSignals(false);
};

void MainWindow::hideSelectedMarkers()
{
  QList<QTableWidgetItem*> items = this->markersTable->selectedItems();
  this->markersTable->blockSignals(true);
  this->qvtkWidget->hideSelectedMarkers(items);
  this->markersTable->blockSignals(false);
};

void MainWindow::showAllMarkers()
{
  this->markersTable->blockSignals(true);
  for (int row = 0 ; row < this->markersTable->rowCount() ; ++row)
    this->markersTable->item(row, 0)->setCheckState(Qt::Checked);
  this->markersTable->blockSignals(false);
  this->qvtkWidget->showAllMarkers();
};

void MainWindow::hideAllMarkers()
{
  this->markersTable->blockSignals(true);
  for (int row = 0 ; row < this->markersTable->rowCount() ; ++row)
    this->markersTable->item(row, 0)->setCheckState(Qt::Unchecked);
  this->markersTable->blockSignals(false);
  this->qvtkWidget->hideAllMarkers();
};

void MainWindow::updateMarkerRadius(double r)
{
  this->markerRadiusSlider->blockSignals(true);
  this->markerRadiusSlider->setValue(r * 10.0);
  this->markerRadiusSlider->blockSignals(false);
  QList<QTableWidgetItem*> items = this->markersTable->selectedItems();
  for (QList<QTableWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
    this->qvtkWidget->setMarkerRadius((*it)->data(markerId).toInt(), r);
  this->qvtkWidget->updateDisplay(this->frameSlider->value());
};

void MainWindow::updateMarkerRadiusSpinBox(int v)
{
  double r = static_cast<double>(v) / 10.0;
  this->markerRadiusSpinBox->setValue(r);
};

void MainWindow::displayMarkerProperties()
{
  this->markerRadiusSpinBox->blockSignals(true);
  this->markerRadiusSlider->blockSignals(true);
  this->markerLabelEdit->blockSignals(true);
  this->markerDescEdit->blockSignals(true);
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
      QTableWidgetItem* item = items.first();
      this->markerLabelEdit->setText(item->data(pointLabel).toString());
      this->markerDescEdit->setText(item->data(pointDescription).toString());
      this->markerLabelEdit->setEnabled(true);
      this->markerDescEdit->setEnabled(true);
    }
    this->markerRadiusSpinBox->setEnabled(true);
    this->markerColorButton->setEnabled(true);
    this->markerRadiusSlider->setEnabled(true);
    bool sameRadius = true;
    bool sameColor = true;
    QList<QTableWidgetItem*>::const_iterator it = items.begin();
    double r = this->qvtkWidget->markerRadius((*it)->data(markerId).toInt());
    int c = this->qvtkWidget->markerColorIndex((*it)->data(markerId).toInt());
    ++it;
    while (it != items.end())
    {
      if (r != this->qvtkWidget->markerRadius((*it)->data(markerId).toInt()))
        sameRadius = false;
      if (c != this->qvtkWidget->markerColorIndex((*it)->data(markerId).toInt()))
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
      double* rgba = this->qvtkWidget->markerColorValue(c);
      QString ss = "background-color: rgb( %1, %2, %3);";
      this->markerColorButton->setStyleSheet(
          ss.arg(static_cast<int>(rgba[0] * 255))
            .arg(static_cast<int>(rgba[1] * 255))
            .arg(static_cast<int>(rgba[2] * 255)));
    }
  }
  this->markerRadiusSpinBox->blockSignals(false);
  this->markerRadiusSlider->blockSignals(false);
  this->markerLabelEdit->blockSignals(false);
  this->markerDescEdit->blockSignals(false);
};

void MainWindow::focusOnMarkerLabelEdition()
{
  this->markerProperties->setVisible(true);
  this->markerPropertiesButton->setIcon(*this->mp_DownArrow);
  this->markerLabelEdit->setFocus();
  this->markerLabelEdit->selectAll();
  
  this->markersTable->scrollToItem(this->markersTable->currentItem());
};

void MainWindow::setMarkerConfigurationModified(int modified)
{
  if (modified == this->mp_MarkerConfigurationUndoStack->cleanIndex())
  {
    if (!this->modelConfigurationComboBox->itemData(this->m_SelectedMarkerConfiguration, visualConfigNew).toBool())
    {
      this->saveModelConfigurationButton->setEnabled(false);
      this->modelConfigurationComboBox->setItemText(this->m_SelectedMarkerConfiguration, this->modelConfigurationComboBox->itemData(this->m_SelectedMarkerConfiguration, visualConfigName).toString());
      this->modelConfigurationComboBox->setItemData(this->m_SelectedMarkerConfiguration, false, visualConfigChanged);
    }
  }
  else
  {
    if (!this->modelConfigurationComboBox->itemData(this->m_SelectedMarkerConfiguration, visualConfigChanged).toBool())
    {
      this->modelConfigurationComboBox->setItemText(this->m_SelectedMarkerConfiguration, "*" + this->modelConfigurationComboBox->itemData(this->m_SelectedMarkerConfiguration, visualConfigName).toString());
      this->modelConfigurationComboBox->setItemData(this->m_SelectedMarkerConfiguration, true, visualConfigChanged);
      this->saveModelConfigurationButton->setEnabled(true);
    }
  }
};

bool MainWindow::isOkToContinue2()
{
  if (this->m_SelectedMarkerConfiguration == -1)
    return true;
    
  bool newConfig = this->modelConfigurationComboBox->itemData(this->m_SelectedMarkerConfiguration, visualConfigNew).toBool();
    
  if (this->modelConfigurationComboBox->itemData(this->m_SelectedMarkerConfiguration, visualConfigChanged).toBool())
  {
    QString message = "The visual configuration has been modified.\nDo you want to save your changes?";
    if (newConfig)
      message += "\n\nThis configuration is a new one and will be removed if you do not save it.";
    
    QMessageBox messageBox(QMessageBox::Question, 
                           trUtf8("Mokka"),
                           trUtf8(message.toAscii().constData()), 
                           QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                           this, Qt::Sheet);
    messageBox.setDefaultButton(QMessageBox::Yes);
    messageBox.setEscapeButton(QMessageBox::Cancel);
    int res = messageBox.exec();
    if (res == QMessageBox::Yes)
    {
      if (newConfig)
      {
        QString name = this->modelConfigurationComboBox->itemData(this->m_SelectedMarkerConfiguration, visualConfigName).toString();
        QString filename = QFileDialog::getSaveFileName(this,
                             trUtf8("Save Model Visual Configuration"),
                             this->m_LastDirectory + "/" + name + ".mvc",
                             trUtf8("Model Visual Configuration Files (*.mvc)"));
        if (filename.isEmpty())
          return false;
        this->modelConfigurationComboBox->setItemData(this->m_SelectedMarkerConfiguration, filename, visualConfigFilename);
        
      }
      return this->saveMarkerConfiguration(this->m_SelectedMarkerConfiguration);
    }
    else if (res == QMessageBox::No)
    {
      if (!this->modelConfigurationComboBox->itemData(this->m_SelectedMarkerConfiguration, visualConfigNew).toBool())
      {
        this->modelConfigurationComboBox->setItemData(this->m_SelectedMarkerConfiguration, false, visualConfigChanged);
        this->modelConfigurationComboBox->setItemText(this->m_SelectedMarkerConfiguration, this->modelConfigurationComboBox->itemData(this->m_SelectedMarkerConfiguration, visualConfigName).toString());
      }
      else
        this->eraseMarkerConfiguration();
    }
    else if (res == QMessageBox::Cancel)
      return false;
  }
  return true;
};

void MainWindow::selectMarkerConfiguration(int index)
{
  if (index != this->m_SelectedMarkerConfiguration)
  {
    if (this->isOkToContinue2())
    {
      int idx = this->modelConfigurationComboBox->currentIndex(); // Update the index in the case where one configuration has been removed.
      if ((idx == (this->modelConfigurationComboBox->count() - 1)) || (idx == -1))
        this->newMarkerConfiguration();
      else
        this->loadMarkerConfiguration(this->modelConfigurationComboBox->itemData(index, visualConfigFilename).toString(),0);
    }
    else
    {
      this->modelConfigurationComboBox->blockSignals(true);
      this->modelConfigurationComboBox->setCurrentIndex(this->m_SelectedMarkerConfiguration);
      this->modelConfigurationComboBox->blockSignals(false);
    }
  }
  this->qvtkWidget->setFocus();
};

void MainWindow::newMarkerConfiguration()
{
  NewModelConfigurationWidget configNameWidget(this);
  if (this->mp_Acquisition)
  {
    QFileInfo fileInfo(this->m_RecentFiles.first());
    
    QString name = fileInfo.baseName();
    int i = 0, inc = 0;
    while (i < this->modelConfigurationComboBox->count() - 2)
    {
      if (this->modelConfigurationComboBox->itemText(i).compare(name) == 0)
      {
        ++inc;
        name = fileInfo.baseName() + "-" + QString::number(inc);
        i = 0;
      }
      else
        ++i;
    }
    configNameWidget.setConfigurationName(name);
  }
  this->modelConfigurationComboBox->blockSignals(true);
  if (configNameWidget.exec())
  {
    this->m_SelectedMarkerConfiguration = 0;
    this->modelConfigurationComboBox->insertItem(this->m_SelectedMarkerConfiguration, "*" + configNameWidget.configurationName());
    this->modelConfigurationComboBox->setItemData(this->m_SelectedMarkerConfiguration, true, visualConfigChanged);
    this->modelConfigurationComboBox->setItemData(this->m_SelectedMarkerConfiguration, true, visualConfigNew);
    this->modelConfigurationComboBox->setItemData(this->m_SelectedMarkerConfiguration, configNameWidget.configurationName(), visualConfigName);
    
    this->saveModelConfigurationButton->setEnabled(true);
    this->deleteModelConfigurationButton->setEnabled(true);
    //this->mp_MarkerConfigurationUndoStack->clear();

    // Save this new configuration in a temporary file.
    QTemporaryFile file;
    if (file.open())
    {
      this->modelConfigurationComboBox->setItemData(this->m_SelectedMarkerConfiguration, file.fileName(), visualConfigFilename);
      this->saveMarkerConfiguration(this->m_SelectedMarkerConfiguration);
      file.close();
    }
    else
    {
      QMessageBox messageBox(QMessageBox::Warning, 
                             trUtf8("Mokka"),
                             trUtf8("Impossible to create a temporary file for the new configuration."),
                             QMessageBox::Yes,
                             this, Qt::Sheet);
      messageBox.setDefaultButton(QMessageBox::Yes);
      messageBox.exec();
    }
  }
  this->modelConfigurationComboBox->setCurrentIndex(this->m_SelectedMarkerConfiguration);
  this->modelConfigurationComboBox->blockSignals(false);

  this->qvtkWidget->setFocus();
};

void MainWindow::saveMarkerConfiguration()
{
  int index = this->modelConfigurationComboBox->currentIndex();
  
  if (this->modelConfigurationComboBox->itemData(index, visualConfigNew).toBool())
  {
    QString name = this->modelConfigurationComboBox->itemData(index, visualConfigName).toString();
    QString filename = QFileDialog::getSaveFileName(this,
                         trUtf8("Save Model Visual Configuration"),
                         this->m_LastDirectory + "/" + name + ".mvc",
                         trUtf8("Model Visual Configuration Files (*.mvc)"));
    if (filename.isEmpty())
      return;
    this->modelConfigurationComboBox->setItemData(index, filename, visualConfigFilename);
  }
  
  if (this->saveMarkerConfiguration(index))
  {
    this->saveModelConfigurationButton->setEnabled(false);
    this->modelConfigurationComboBox->setItemText(index, this->modelConfigurationComboBox->itemData(index, visualConfigName).toString());
    this->modelConfigurationComboBox->setItemData(index, false, visualConfigChanged);
    this->modelConfigurationComboBox->setItemData(index, false, visualConfigNew);
  }
  this->qvtkWidget->setFocus();
};

bool MainWindow::saveMarkerConfiguration(int index)
{
  QMessageBox messageBox(QMessageBox::Critical, 
                         trUtf8("Mokka"),
                         trUtf8("Cannot write file: "),
                         QMessageBox::Ok,
                         this, Qt::Sheet);
  messageBox.setDefaultButton(QMessageBox::Ok);
  
  //QString filename = "";
  //QString name = this->modelConfigurationComboBox->itemData(index, visualConfigName).toString();
  //if (!this->modelConfigurationComboBox->itemData(index, visualConfigNew).toBool())
  //  filename = this->modelConfigurationComboBox->itemData(index, visualConfigFilename).toString();
  QString name = this->modelConfigurationComboBox->itemData(index, visualConfigName).toString();
  QString filename = this->modelConfigurationComboBox->itemData(index, visualConfigFilename).toString();
  
  QFile file(filename);
  if (!file.open(QFile::WriteOnly | QFile::Text))
  {
    messageBox.setText(messageBox.text() + filename + "\n" + file.errorString());
    messageBox.exec();
    return false;
  }
  
  QXmlStreamWriter xmlWriter(&file);
  xmlWriter.setAutoFormatting(true);
  xmlWriter.writeStartDocument();
  xmlWriter.writeStartElement("MokkaModelVisualConfiguration");
  xmlWriter.writeAttribute("name", name);
  xmlWriter.writeAttribute("version", "1.0");
  xmlWriter.writeStartElement("MarkersList");
  for (int i = 0 ; i < this->markersTable->rowCount() ; ++i)
  {
    if (!this->markersTable->isRowHidden(i))
    {
      xmlWriter.writeStartElement("Marker");
      QTableWidgetItem* item = this->markersTable->item(i,0);
      xmlWriter.writeAttribute("label", item->data(pointLabel).toString());
      xmlWriter.writeAttribute("radius", item->data(markerRadius).toString());
      double* color = this->qvtkWidget->markerColorValue(item->data(markerColorIndex).toInt());
      xmlWriter.writeAttribute("colorR", QString::number(color[0]));
      xmlWriter.writeAttribute("colorG", QString::number(color[1]));
      xmlWriter.writeAttribute("colorB", QString::number(color[2]));
      xmlWriter.writeEndElement();
    }
  }
  xmlWriter.writeEndElement(); // MarkersConfiguration
  xmlWriter.writeEndElement(); // MokkaModelVisualConfiguration
  xmlWriter.writeEndDocument();
  file.close();
  if (file.error())
  {
      messageBox.setText(messageBox.text() + filename + "\n" + file.errorString());
      messageBox.exec();
      return false;
  }
  
  //this->modelConfigurationComboBox->setItemData(index, filename, visualConfigFilename);
  
  return true;
};

void MainWindow::loadMarkerConfiguration(const QString& filename)
{
  QString filename2 = filename;
  if (filename2.isEmpty())
  {
    filename2 = QFileDialog::getOpenFileName(this,
                  trUtf8("Open Model Visual Configuration"),
                  this->m_LastDirectory,
                  trUtf8("Model Visual Configuration Files (*.mvc)"));
  }
  
  QString name;
  if (!filename2.isEmpty() && this->loadMarkerConfiguration(filename, &name))
  {
    // TODO: Replace or not an existing configuration
    bool alreadyExist = false;
    
    this->modelConfigurationComboBox->blockSignals(true);
    this->m_SelectedMarkerConfiguration = 0;
    this->modelConfigurationComboBox->insertItem(this->m_SelectedMarkerConfiguration, name);
    this->modelConfigurationComboBox->setItemData(this->m_SelectedMarkerConfiguration, false, visualConfigChanged);
    this->modelConfigurationComboBox->setItemData(this->m_SelectedMarkerConfiguration, false, visualConfigNew);
    this->modelConfigurationComboBox->setItemData(this->m_SelectedMarkerConfiguration, filename, visualConfigFilename);
    this->modelConfigurationComboBox->setItemData(this->m_SelectedMarkerConfiguration, name, visualConfigName);
    this->modelConfigurationComboBox->setCurrentIndex(this->m_SelectedMarkerConfiguration);
    this->modelConfigurationComboBox->blockSignals(false);
  }
  
  this->qvtkWidget->setFocus();
};

bool MainWindow::loadMarkerConfiguration(const QString& filename, QString* name)
{
  QMessageBox messageBox(QMessageBox::Critical, 
                         trUtf8("Mokka"),
                         "",
                         QMessageBox::Ok,
                         this, Qt::Sheet);
  messageBox.setDefaultButton(QMessageBox::Ok);
  
  QFile file(filename);
  if (!file.open(QFile::ReadOnly | QFile::Text))
  {
    messageBox.setText("Cannot read file: " + filename + "\n" + file.errorString());
    messageBox.exec();
    return false;
  }
  
  QStringList labels;
  QList<qreal> radius;
  QList<qreal*> colors;
  QXmlStreamReader xmlReader(&file);
  
  if (xmlReader.readNextStartElement())
  {
    if (xmlReader.name() == "MokkaModelVisualConfiguration" && xmlReader.attributes().value("version") == "1.0")
    {
      if (name)
        *name = xmlReader.attributes().value("name").toString();
      while (xmlReader.readNextStartElement())
      {
        if (xmlReader.name() == "MarkersList")
        {
          while (xmlReader.readNextStartElement())
          {
            if (xmlReader.name() == "Marker")
            {
              QXmlStreamAttributes att = xmlReader.attributes();
              labels << att.value("label").toString();
              radius << att.value("radius").toString().toDouble();
              qreal* color = new qreal[3];
              color[0] = att.value("colorR").toString().toDouble();
              color[1] = att.value("colorG").toString().toDouble();
              color[2] = att.value("colorB").toString().toDouble();
              colors << color;
            }
            else
              xmlReader.skipCurrentElement();
            xmlReader.readNext();
          }
        }
        else
          xmlReader.skipCurrentElement();
      }
    }
    else
      xmlReader.raiseError(QObject::tr("The file is not an MokkaModelVisualConfiguration version 1.0 file."));
  }
 
  file.close();
  if (xmlReader.hasError())
  {
    messageBox.setText("Failed to parse file: " + filename + "\n" + file.errorString());
    messageBox.exec();
    return false;
  }
  else if (file.error() != QFile::NoError)
  {
    messageBox.setText("Cannot read file: " + filename + "\n" + file.errorString());
    messageBox.exec();
    return false;
  }
  
  this->markersTable->blockSignals(true);
  for (int i = 0 ; i < this->markersTable->rowCount() ; ++i)
  {
    if (!this->markersTable->isRowHidden(i))
    {
      QTableWidgetItem* item = this->markersTable->item(i,0);
      int idx = labels.indexOf(item->data(pointLabel).toString());
      if (idx != -1)
      {
        qreal r = radius.at(idx);
        qreal* c = colors.at(idx);
        int idxColor;
        QColor color; color.setRgbF(c[0],c[1],c[2]);
        int idxMarker = item->data(markerId).toInt();
        this->qvtkWidget->appendNewMarkerColor(color, &idxColor);
        this->qvtkWidget->setMarkerRadius(idxMarker, r);
        this->qvtkWidget->setMarkerColorIndex(idxMarker, idxColor);
        item->setData(markerRadius, r);
        item->setData(markerColorIndex, idxColor);
        
        delete[] c;
      }
    }
  }
  this->markersTable->blockSignals(false);
  
  this->saveModelConfigurationButton->setEnabled(false);
  this->deleteModelConfigurationButton->setEnabled(true);
  this->m_SelectedMarkerConfiguration = this->modelConfigurationComboBox->currentIndex();
  
  this->qvtkWidget->updateDisplay();
  
  return true;
};

void MainWindow::eraseMarkerConfiguration()
{
  int index = this->m_SelectedMarkerConfiguration;//this->modelConfigurationComboBox->currentIndex();
  
  if (!this->modelConfigurationComboBox->itemData(index, visualConfigNew).toBool())
  {
    QString filename = this->modelConfigurationComboBox->itemData(index, visualConfigFilename).toString();
    QMessageBox messageBox(QMessageBox::Question, 
                           trUtf8("Mokka"),
                           trUtf8("Do you want to delete the file associated to this visual configuration?"), 
                           QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                           this, Qt::Sheet);
    messageBox.setDefaultButton(QMessageBox::Yes);
    messageBox.setEscapeButton(QMessageBox::Cancel);
    int res = messageBox.exec();
    if (res == QMessageBox::Yes)
    {
      QFileInfo fi(filename);
      fi.absoluteDir().remove(fi.fileName());
    }
    else if (res == QMessageBox::Cancel)
      return;
  }
  this->modelConfigurationComboBox->blockSignals(true);
  this->modelConfigurationComboBox->removeItem(index);
  if (this->modelConfigurationComboBox->count() <= 2)
  {
    this->m_SelectedMarkerConfiguration = -1;
    this->modelConfigurationComboBox->setCurrentIndex(-1);
    this->saveModelConfigurationButton->setEnabled(false);
    this->deleteModelConfigurationButton->setEnabled(false); 
  }
  this->modelConfigurationComboBox->blockSignals(false);
  this->selectMarkerConfiguration(this->m_SelectedMarkerConfiguration);
  
  // Go to the default visual configuration:
  //  - White markers
  // - Radius : 8 mm
  double r = 8.0;
  int idxColor = 0; // White
  this->markersTable->blockSignals(true);
  for (int i = 0 ; i < this->markersTable->rowCount() ; ++i)
  {
    if (!this->markersTable->isRowHidden(i))
    {
      QTableWidgetItem* item = this->markersTable->item(i,0);
      int idxMarker = item->data(markerId).toInt();
      this->qvtkWidget->setMarkerRadius(idxMarker, 8.0);
      this->qvtkWidget->setMarkerColorIndex(idxMarker, (int)0);
      item->setData(markerRadius, r);
      item->setData(markerColorIndex, idxColor);
    }
  }
  this->markersTable->blockSignals(false);
  this->qvtkWidget->updateDisplay();
  
  this->qvtkWidget->setFocus();
};

void MainWindow::editMarkerLabel()
{
  QString label = this->markerLabelEdit->text();
  QTableWidgetItem* item = this->markersTable->currentItem();
  if (label.compare(item->text()) == 0)
    return;
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new EditMarkerLabel(label, item)));
};

void MainWindow::editMarkerDescription()
{
  QString desc = this->markerDescEdit->text();
  QTableWidgetItem* item = this->markersTable->currentItem();
  if (desc.compare(item->data(pointDescription).toString()) == 0)
    return;
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new EditMarkerDescription(desc, item)));
};

void MainWindow::editMarkerRadius()
{
  double r = this->markerRadiusSpinBox->value();
  if (this->markerRadiusSpinBox->text().isEmpty())
    return;
  QList<QTableWidgetItem*> items = this->markersTable->selectedItems();
  if (items.isEmpty())
    return;
  bool modified = false;
  for (QList<QTableWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
  {
    if (r != (*it)->data(markerRadius).toDouble())
    {
      modified = true;
      break;
    }
  }
  if (!modified)
    return;
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_MarkerConfigurationUndoStack, new EditMarkersRadius(r, items, this)));
};

void MainWindow::editMarkerColor()
{
  QColor color = QColorDialog::getColor(Qt::white, this);
  QList<QTableWidgetItem*> items = this->markersTable->selectedItems();
  if (items.isEmpty())
    return;
  if (color.isValid())
  {
    int i;
    bool modified = this->qvtkWidget->appendNewMarkerColor(color, &i);
    if (!modified)
    {
      QList<QTableWidgetItem*> items = this->markersTable->selectedItems();
      for (QList<QTableWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
      {
        if ((*it)->data(markerColorIndex) != i)
        {
          modified = true;
          break;
        }
      }
    }
    if (modified)
      this->mp_UndoStack->push(new MasterUndoCommand(this->mp_MarkerConfigurationUndoStack, new EditMarkersColorIndex(i, items, this)));
  }
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
  this->qvtkWidget->circleSelectedMarkers(this->markersTable->selectedItems());
  this->qvtkWidget->updateDisplay(this->frameSlider->value());
};

void MainWindow::showEvent()
{
  this->eventsTable->setStyleSheet("selection-color: rgb(255,0,0);");

  int row = this->eventsTable->currentRow();
  int frame = this->eventsTable->item(row, 0)->data(eventFrame).toInt();
  this->frameSlider->setValue(frame);
};

void MainWindow::newEvent()
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new NewEvent(this, this->frameSlider->value(), this->mp_Acquisition->GetPointFrequency())));
};

void MainWindow::deleteEvent()
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new DeleteEvent(this)));
};

void MainWindow::updateActiveEvent(int frame)
{
  this->eventsTable->blockSignals(true);
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
      if (this->eventsTable->item(row, 0)->data(eventVisible).toInt())
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
  this->eventsTable->blockSignals(false);
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
    this->eventContextCombo->setCurrentIndex(0);
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
    this->eventTimeSpinBox->setValue(this->eventsTable->item(row, 0)->data(eventTime).toDouble());
    this->eventContextCombo->setCurrentIndex(this->eventContextCombo->findText(this->eventsTable->item(row, 1)->text()));
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

void MainWindow::updateEventInternalInformations(QTableWidgetItem* item)
{
  this->eventsTable->blockSignals(true);
  switch(item->column())
  {
    case 0:
    {
      double pointFrequency = this->mp_Acquisition->GetPointFrequency();
      int frameIndex = static_cast<int>(item->data(eventTime).toDouble() * pointFrequency);
      item->setData(eventFrame, frameIndex);
      if ((frameIndex < this->mp_Acquisition->GetFirstFrame()) || (frameIndex > this->mp_Acquisition->GetLastFrame()))
        item->setData(eventVisible, false);
      else
        item->setData(eventVisible, true);
    }
    default:
      break;
  }
  this->eventsTable->blockSignals(false);
  this->updateActiveEvent(this->frameSlider->value());
};

void MainWindow::updateEventsButtonsState()
{
  this->newEventButton->setEnabled(true);
  QList<QTableWidgetItem*> items = this->eventsTable->selectedItems();
  if (!items.isEmpty())
  {
    int row = this->eventsTable->currentRow();
    this->deleteEventButton->setEnabled(true);
    int valid = this->eventsTable->item(row, 0)->data(eventVisible).toInt();
    if (valid)
      this->showEventButton->setEnabled(true);
    else
      this->showEventButton->setEnabled(false);
  }
  else
  {
    this->deleteEventButton->setEnabled(false);
    this->showEventButton->setEnabled(false);
  }
};

void MainWindow::focusOnEventEdition(int idx)
{
  int selectedIdx = idx;
  QTableWidgetItem* item = this->eventsTable->currentItem();
  if (selectedIdx == -1)
    selectedIdx = item->column();
  
  this->eventInformations->setVisible(true);
  this->eventInformationsButton->setIcon(*this->mp_DownArrow);
  switch(selectedIdx)
  {
  case 0:
    this->eventTimeSpinBox->setFocus();
    this->eventTimeSpinBox->selectAll();
    break;
  case 1:
    this->eventContextCombo->setFocus();
    break;
  case 2:
    this->eventLabelEdit->setFocus();
    this->eventLabelEdit->selectAll();
    break;
  case 3:
    this->eventSubjectEdit->setFocus();
    this->eventSubjectEdit->selectAll();
    break;
  }
  this->eventsTable->scrollToItem(item);
};

void MainWindow::updateDisplayedMarkersList(const QVector<int>& ids)
{
  QBrush defaultLabelColor = QBrush(QColor(Qt::gray));
  QBrush displayLabelColor = QBrush(QColor(Qt::black));
  this->markersTable->blockSignals(true);
  for (int row = 0 ; row < this->markersTable->rowCount() ; ++row)
  {
    QTableWidgetItem* item = this->markersTable->item(row, 0);
    if ((ids.size() > item->data(markerId).toInt()) && ids[item->data(markerId).toInt()] && (item->checkState() == Qt::Checked))
      item->setForeground(displayLabelColor);
    else
      item->setForeground(defaultLabelColor);
  }
  this->markersTable->blockSignals(false);
};

void MainWindow::selectPickedMarker(int id)
{
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

void MainWindow::selectPickedMarkers(int id)
{
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

void MainWindow::editEventLabel()
{
  QString label = this->eventLabelEdit->text();
  QTableWidgetItem* item = this->eventsTable->item(this->eventsTable->currentRow(), 2);
  if (label.compare(item->text()) == 0)
    return;
  int id = 0;
  QString desc = "";
  for (int i = 0 ; i < this->eventsTable->rowCount() ; ++i)
  {
    QTableWidgetItem* itemLabel = this->eventsTable->item(i,2);
    if (itemLabel->text().compare(label) == 0)
    {
      id = itemLabel->data(eventId).toInt();
      desc = itemLabel->data(eventDescription).toString();
      break;
    }
  }
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new EditEventLabel(label, id, desc, item)));
};

void MainWindow::editEventContext(const QString& context)
{
  QTableWidgetItem* item = this->eventsTable->item(this->eventsTable->currentRow(), 1);
  if (context.compare(item->data(eventContext).toString()) == 0)
    return;
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new EditEventContext(context, item)));
};

void MainWindow::editEventTime()
{
  double t = this->eventTimeSpinBox->value();
  if (this->eventTimeSpinBox->text().isEmpty())
    return;
  QTableWidgetItem* item = this->eventsTable->item(this->eventsTable->currentRow(), 0);
  if (fabs(t - item->data(eventTime).toDouble()) < 0.00001)
    return;
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new EditEventTime(t, item)));
};

void MainWindow::editEventSubject()
{
  QString subject = this->eventSubjectEdit->text();
  QTableWidgetItem* item = this->eventsTable->item(this->eventsTable->currentRow(), 3);
  if (subject.compare(item->text()) == 0)
    return;
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new EditEventSubject(subject, item)));
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
  if (settings.value("maximized", false).toBool())
    this->showMaximized();
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
  QStringList recentVisualConfigFilename = settings.value("recentVisualConfigFilename").toStringList(); 
  QStringList recentVisualConfigName = settings.value("recentVisualConfigName").toStringList();
  this->modelConfigurationComboBox->blockSignals(true);
  for (int i = 0 ; i < recentVisualConfigFilename.count() ; ++i)
  {
    this->modelConfigurationComboBox->insertItem(i,recentVisualConfigName.at(i));
    this->modelConfigurationComboBox->setItemData(i,recentVisualConfigName.at(i),visualConfigName);
    this->modelConfigurationComboBox->setItemData(i,recentVisualConfigFilename.at(i),visualConfigFilename);
    this->modelConfigurationComboBox->setItemData(i,false,visualConfigChanged);
    this->modelConfigurationComboBox->setItemData(i,false,visualConfigNew);
  }
  this->modelConfigurationComboBox->blockSignals(false);
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
  settings.setValue("maximized", (this->isMaximized() ? true : false));
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
  QStringList recentVisualConfigFilename; 
  QStringList recentVisualConfigName;
  for (int i = 0 ; i < (this->modelConfigurationComboBox->count() - 2) ; ++i)
  {
    recentVisualConfigFilename << this->modelConfigurationComboBox->itemData(i,visualConfigFilename).toString();
    recentVisualConfigName << this->modelConfigurationComboBox->itemData(i,visualConfigName).toString();
  }
  settings.setValue("recentVisualConfigFilename", recentVisualConfigFilename);
  settings.setValue("recentVisualConfigName", recentVisualConfigName);
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

bool findLabel(btk::PointCollection::Pointer points, const std::string& label)
{
  for (btk::PointCollection::ConstIterator it = points->Begin() ; it != points->End() ; ++it)
  {
    if ((*it)->GetLabel().compare(label) == 0)
      return true;
  }
  return false;
};
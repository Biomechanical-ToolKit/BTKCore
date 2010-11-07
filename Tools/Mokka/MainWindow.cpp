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
#include <QDesktopWidget>

//#define MOKKA_NEW_UI

MainWindow::MainWindow(QWidget* parent)
:QMainWindow(parent), mp_Acquisition(), m_LastDirectory(".")
{
  // Members
  this->mp_AcquisitionQ = new Acquisition(this);
  this->mp_MetadataDlg = new Metadata(this);
  this->mp_PointsEditorDlg = new PointsEditor(this);
  this->mp_ModelDock = new ModelDockWidget(this);
  this->mp_ModelDock->setAcquisition(this->mp_AcquisitionQ);
  this->mp_FileInfoDock = new FileInfoDockWidget(this);
  
  this->m_SelectedMarkerConfiguration = -1;
  this->mp_DownArrow = new QIcon(QString::fromUtf8(":/Resources/Images/disclosureTriangleSmallDownBlack.png"));
  this->mp_RightArrow = new QIcon(QString::fromUtf8(":/Resources/Images/disclosureTriangleSmallRightBlack.png"));
  
  // Finalize UI
  this->mp_FileInfoDock->setVisible(false); 
  this->mp_FileInfoDock->setFloating(true);
  this->mp_FileInfoDock->move(100,100);
  this->addDockWidget(Qt::RightDockWidgetArea, this->mp_ModelDock); 
  this->mp_ModelDock->setVisible(false);
  this->setupUi(this);
#ifdef MOKKA_NEW_UI
  this->menuVisual_Configuration->removeAction(this->actionDeselectCurrentConfiguration);
  this->menuVisual_Configuration->removeAction(this->actionClearConfigurationList);
  this->menuVisual_Configuration->addAction(this->mp_ModelDock->deselectConfigurationAction());
  this->menuVisual_Configuration->addAction(this->mp_ModelDock->clearConfigurationsAction());
  QAction* actionModelDockView = this->mp_ModelDock->toggleViewAction();
  actionModelDockView->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_M));
  this->menuView->addAction(actionModelDockView);
#else
  QAction* actionMarkersDockView = this->markersDock->toggleViewAction();
  actionMarkersDockView->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_M));
  actionMarkersDockView->setText(QObject::tr("Markers List"));
  this->menuView->addAction(actionMarkersDockView);
#endif
  this->menuSettings->addMenu(this->multiView->groundOrientationMenu());
  this->menuSettings->addMenu(this->timeEventControler->playbackSpeedMenu());
  QAction* actionInformationsDockView = this->mp_FileInfoDock->toggleViewAction();
  actionInformationsDockView->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_I));
  this->menuView->addAction(actionInformationsDockView);
  this->menuView->addAction(this->actionViewMetadata);
  this->timeEventControler->playbackSpeedMenu()->menuAction()->setEnabled(true);
  
#ifdef Q_OS_MAC
  QFont f = this->font();
  f.setPointSize(10);
  this->mp_FileInfoDock->setFont(f);
  this->markersDock->setFont(f);
  this->markerPropertiesButton->setFont(f);
  f.setPointSize(11);
  this->showMarkersButton->setFont(f);
  this->hideMarkersButton->setFont(f);
#endif
  this->mp_FileInfoDock->reset(); // Force to update geometry
  this->markersDock->setVisible(false);
  this->modelConfigurationComboBox->insertSeparator(99);
  this->modelConfigurationComboBox->insertItem(99,"New ...");
  this->markerProperties->setVisible(false);
  this->markerPropertiesButton->setIcon(*this->mp_RightArrow);
  this->action_FileOpen->setShortcut(QKeySequence::Open);
  this->actionClose->setShortcut(QKeySequence::Close);
  this->actionSave->setShortcut(QKeySequence::Save);
  this->actionSave_As->setShortcut(QKeySequence::SaveAs);
  this->actionCut->setShortcut(QKeySequence::Cut);
  this->actionCopy->setShortcut(QKeySequence::Copy);
  this->actionPaste->setShortcut(QKeySequence::Paste);
  this->actionSelect_All->setShortcut(QKeySequence::SelectAll);
  this->markerRadiusSpinBox->clear();
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
  
  // Viz3D
  this->multiView->initialize();
  this->timeEventControler->acquisitionOptionsButtonMenu->menu()->insertMenu(this->timeEventControler->playbackSpeedMenu()->menuAction(), this->multiView->groundOrientationMenu());

  // Qt UI: Undo/Redo
  this->mp_UndoStack = new QUndoStack(this); // One to command all.
  this->mp_MarkerConfigurationUndoStack = new QUndoStack(this);
  connect(this->mp_MarkerConfigurationUndoStack, SIGNAL(indexChanged(int)), this, SLOT(setMarkerConfigurationModified(int)));
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
  connect(this->actionViewMetadata, SIGNAL(triggered()), this, SLOT(viewMetadata()));
  connect(this->actionEdit_Points, SIGNAL(triggered()), this, SLOT(editPoints()));
  connect(this->action_FileOpen, SIGNAL(triggered()), this, SLOT(openFile()));
  connect(this->actionSave, SIGNAL(triggered()), this, SLOT(saveFile()));
  connect(this->actionSave_As, SIGNAL(triggered()), this, SLOT(saveAsFile()));
  connect(this->actionClose, SIGNAL(triggered()), this, SLOT(closeFile()));
  connect(this->action_Quit, SIGNAL(triggered()), this, SLOT(close()));
  connect(this->actionDeselectCurrentConfiguration, SIGNAL(triggered()), this, SLOT(deselectCurrentVisualConfiguration()));
  connect(this->actionClearConfigurationList, SIGNAL(triggered()), this, SLOT(clearVisualConfigurationList()));
  // MultiView
  connect(this->multiView, SIGNAL(fileDropped(QString)), this, SLOT(openFileDropped(QString)));
  connect(this->multiView, SIGNAL(visibleMarkersChanged(QVector<int>)), this, SLOT(updateDisplayedMarkersList(QVector<int>)));
  connect(this->multiView, SIGNAL(visibleMarkersChanged(QVector<int>)), this->mp_ModelDock, SLOT(updateDisplayedMarkers(QVector<int>)));
  connect(this->multiView, SIGNAL(pickedMarkerChanged(int)), this, SLOT(selectPickedMarker(int)));
  connect(this->multiView, SIGNAL(pickedMarkersChanged(int)), this, SLOT(selectPickedMarkers(int)));
  // Markers dock
  connect(this->modelConfigurationComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(selectMarkerConfiguration(int)));
  connect(this->saveModelConfigurationButton, SIGNAL(clicked()), this, SLOT(saveMarkerConfiguration()));
  connect(this->loadModelConfigurationButton, SIGNAL(clicked()), this, SLOT(loadMarkerConfiguration()));
  connect(this->deleteModelConfigurationButton, SIGNAL(clicked()), this, SLOT(eraseMarkerConfiguration()));
  connect(this->markersTable, SIGNAL(itemChanged(QTableWidgetItem*)), this->multiView, SLOT(updateMarkerVisibility(QTableWidgetItem*)));
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
  connect(this->mp_ModelDock, SIGNAL(markerTrajectorySelectionChanged(QList<int>)), this->multiView, SLOT(updateTailedMarkers(QList<int>)));
  connect(this->mp_ModelDock->markerRadiusSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateSelectedMarkersRadius(double)));
  // Time Event
  connect(this->timeEventControler, SIGNAL(currentFrameChanged(int)), this->multiView, SLOT(updateDisplay(int)));
  connect(this->timeEventControler, SIGNAL(regionOfInterestChanged(int,int)), this, SLOT(setRegionOfInterest(int,int)));
  connect(this->timeEventControler, SIGNAL(eventsRemoved(QList<int>)), this, SLOT(removeEvents(QList<int>)));
  connect(this->timeEventControler, SIGNAL(eventInserted(Event*)), this, SLOT(insertEvent(Event*)));
  // Acquisition
  connect(this->mp_AcquisitionQ, SIGNAL(informationsChanged(QVector<QString>)), this->mp_FileInfoDock, SLOT(fill(QVector<QString>)));
  connect(this->mp_AcquisitionQ, SIGNAL(pointLabelChanged(int, QString)), this->mp_ModelDock, SLOT(setPointLabel(int, QString)));
  connect(this->mp_AcquisitionQ, SIGNAL(markersRadiusChanged(QVector<int>, QVector<double>)), this->mp_ModelDock, SLOT(setMarkersRadius(QVector<int>, QVector<double>)));
  connect(this->mp_AcquisitionQ, SIGNAL(markersRadiusChanged(QVector<int>, QVector<double>)), this->multiView, SLOT(setMarkersRadius(QVector<int>, QVector<double>)));
  connect(this->mp_AcquisitionQ, SIGNAL(markersColorChanged(QVector<int>, QVector<QColor>)), this->mp_ModelDock, SLOT(setMarkersColor(QVector<int>, QVector<QColor>)));
  connect(this->mp_AcquisitionQ, SIGNAL(markersColorChanged(QVector<int>, QVector<QColor>)), this->multiView, SLOT(setMarkersColor(QVector<int>, QVector<QColor>)));
  connect(this->mp_AcquisitionQ, SIGNAL(pointsDescriptionChanged(QVector<int>, QVector<QString>)), this->mp_ModelDock, SLOT(setPointsDescription(QVector<int>, QVector<QString>)));
  connect(this->mp_AcquisitionQ, SIGNAL(pointsRemoved(QList<int>, QList<Point*>)), this->mp_ModelDock, SLOT(removePoints(QList<int>, QList<Point*>)));
  connect(this->mp_AcquisitionQ, SIGNAL(pointsInserted(QList<int>, QList<Point*>)), this->mp_ModelDock, SLOT(insertPoints(QList<int>, QList<Point*>)));
  connect(this->mp_AcquisitionQ, SIGNAL(analogLabelChanged(int, QString)), this->mp_ModelDock, SLOT(setAnalogLabel(int, QString)));
  connect(this->mp_AcquisitionQ, SIGNAL(analogsUnitChanged(QVector<int>, QVector<QString>)), this->mp_ModelDock, SLOT(setAnalogsUnit(QVector<int>, QVector<QString>)));
  connect(this->mp_AcquisitionQ, SIGNAL(analogsGainChanged(QVector<int>, QVector<Analog::Gain>)), this->mp_ModelDock, SLOT(setAnalogsGain(QVector<int>, QVector<Analog::Gain>)));
  connect(this->mp_AcquisitionQ, SIGNAL(analogsOffsetChanged(QVector<int>, QVector<int>)), this->mp_ModelDock, SLOT(setAnalogsOffset(QVector<int>, QVector<int>)));
  connect(this->mp_AcquisitionQ, SIGNAL(analogsScaleChanged(QVector<int>, QVector<double>)), this->mp_ModelDock, SLOT(setAnalogsScale(QVector<int>, QVector<double>)));
  connect(this->mp_AcquisitionQ, SIGNAL(analogsDescriptionChanged(QVector<int>, QVector<QString>)), this->mp_ModelDock, SLOT(setAnalogsDescription(QVector<int>, QVector<QString>)));
  connect(this->mp_AcquisitionQ, SIGNAL(analogsRemoved(QList<int>, QList<Analog*>)), this->mp_ModelDock, SLOT(removeAnalogs(QList<int>, QList<Analog*>)));
  connect(this->mp_AcquisitionQ, SIGNAL(analogsInserted(QList<int>, QList<Analog*>)), this->mp_ModelDock, SLOT(insertAnalogs(QList<int>, QList<Analog*>)));
  connect(this->mp_AcquisitionQ, SIGNAL(regionOfInterestChanged(int, int)), this->timeEventControler, SLOT(setRegionOfInterest(int, int)));
  connect(this->mp_AcquisitionQ, SIGNAL(eventsRemoved(QList<int>, QList<Event*>)), this->timeEventControler, SLOT(removeEvents(QList<int>, QList<Event*>)));
  connect(this->mp_AcquisitionQ, SIGNAL(eventsInserted(QList<int>, QList<Event*>)), this->timeEventControler, SLOT(insertEvents(QList<int>, QList<Event*>)));

  // Event filter
  this->multiView->installEventFilter(this);
  this->markersTable->installEventFilter(this);
  this->markerRadiusSpinBox->installEventFilter(this);
  this->markerLabelEdit->installEventFilter(this);
  this->markerDescEdit->installEventFilter(this);
  this->timeEventControler->installEventFilter(this);
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
};

MainWindow::~MainWindow()
{
  delete this->mp_AcquisitionQ;
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
      }
      else if (keyEvent->matches(QKeySequence::Undo) && !lineEdit->isUndoAvailable())
      {
        this->mp_UndoStack->undo();
      }
      else if (keyEvent->matches(QKeySequence::Redo) && !lineEdit->isRedoAvailable())
        this->mp_UndoStack->redo();
    }
    return false;
  }
  else if ((event->type() == QEvent::KeyPress) && this->timeEventControler->isEnabled())
  {
    QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
    // special case for spinbox widget: 'markerRadiusSpinBox' & 'eventTimeSpinBox'
    QAbstractSpinBox* spinBox = qobject_cast<QAbstractSpinBox*>(obj);
    if (spinBox)
    {
      if (keyEvent->key() == Qt::Key_Escape)
      {
        if (obj->objectName().compare("markerRadiusSpinBox") == 0)
          this->markersTable->setFocus();
        else
          return true;
      }  
      if (keyEvent->matches(QKeySequence::Undo))
        this->mp_UndoStack->undo();
      if (keyEvent->matches(QKeySequence::Redo))
        this->mp_UndoStack->redo();
      return false;
      
    }
    // general case
    else if (keyEvent->matches(QKeySequence::MoveToPreviousChar))
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
      if (obj->objectName().compare("markersTable") == 0)
        this->markersTable->clearSelection();
      return true;
    }
    else if ((keyEvent->key() == Qt::Key_Return) || (keyEvent->key() == Qt::Key_Enter))
    {
      if (obj->objectName().compare("eventsTable") == 0)
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
    this->mp_MetadataDlg->warningIconLabel->setVisible(true);
    this->mp_MetadataDlg->warningMsgLabel->setVisible(true);
  }
};

void MainWindow::viewMetadata()
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
      if (static_cast<const UndoCommand*>(this->mp_AcquisitionUndoStack->command(i))->commandType() == UndoCommand::AcquisitionCmd)
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

void MainWindow::play()
{
  if (this->mp_Acquisition)
    this->timeEventControler->togglePlayback();
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

  QMessageBox error(QMessageBox::Critical, "File error", "Error occurred during the file reading", QMessageBox::Ok , this);
  QString errMsg;
  this->mp_AcquisitionQ->load(filename, errMsg);
  if (!errMsg.isEmpty())
  {
    QApplication::restoreOverrideCursor();
    error.setInformativeText(errMsg);
    error.exec();
    return;
  }
  this->mp_Acquisition = this->mp_AcquisitionQ->btkAcquisition();
  
  pw.setProgressValue(25);
  
  this->setCurrentFile(filename);
  
  // UI settings
  // Update the 3D view
  btk::SeparateKnownVirtualMarkersFilter::Pointer separator = this->multiView->load(this->mp_Acquisition);
  this->multiView->updateDisplay(this->mp_Acquisition->GetFirstFrame()); // Required
  
#ifdef MOKKA_NEW_UI
  this->mp_ModelDock->load(separator->GetOutput(0), separator->GetOutput(2), separator->GetOutput(3), this->mp_Acquisition->GetAnalogs());
#endif
  this->timeEventControler->load(this->mp_AcquisitionQ);
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
        this->multiView->setMarkerVisibility(incMarkerId, false);
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
  
  pw.setProgressValue(90);
  
  // Fill Metadata
  this->mp_MetadataDlg->load(this->mp_Acquisition->GetMetaData());
  // Display Docks
#ifndef MOKKA_NEW_UI
  if (hasMarker != 0)
    this->markersDock->setVisible(true);
#else
  if (this->mp_AcquisitionQ->hasPoints() || this->mp_AcquisitionQ->hasAnalogs())
    this->mp_ModelDock->setVisible(true);
#endif
  this->timeEventControler->setEnabled(true);

  pw.setProgressValue(100);

  QApplication::restoreOverrideCursor();
  
  this->actionClose->setEnabled(true);
  this->actionViewMetadata->setEnabled(true);
  this->actionEdit_Points->setEnabled(true);
  //this->actionSave->setEnabled(true); 
  this->actionSave_As->setEnabled(true);
  
  this->modelConfigurationComboBox->setEnabled(true);
  this->loadModelConfigurationButton->setEnabled(true);
  if (this->modelConfigurationComboBox->count() != 0)
    this->actionClearConfigurationList->setEnabled(true);
  
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
  /*
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
  */
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
  //this->fillFileInformations(filename, writer->GetAcquisitionIO(), target);
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
  this->actionViewMetadata->setEnabled(false);
  this->actionEdit_Points->setEnabled(false);
  this->actionSave->setEnabled(false); 
  this->actionSave_As->setEnabled(false);
  this->setCurrentFile("");
  // Time & Event Controler
  this->timeEventControler->reset();
  this->timeEventControler->setEnabled(false);
  // Informations Dock
  this->mp_FileInfoDock->reset();
  // Model dock
  this->mp_ModelDock->reset();
  this->mp_ModelDock->setVisible(false);
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
  // Metadata
  this->mp_MetadataDlg->reset();
  // Multivew
  this->multiView->clear();
};

void MainWindow::deselectCurrentVisualConfiguration()
{
  if (this->isOkToContinue2())
  {
    this->modelConfigurationComboBox->setCurrentIndex(-1);
    this->actionDeselectCurrentConfiguration->setEnabled(false);
  }
};

void MainWindow::clearVisualConfigurationList()
{
  this->deselectCurrentVisualConfiguration();
  if (this->modelConfigurationComboBox->currentIndex() == -1)
  {
    this->modelConfigurationComboBox->blockSignals(true);
    int cnt = this->modelConfigurationComboBox->count() - 2;
    for (int i = 0 ; i < cnt ; ++i)
      this->modelConfigurationComboBox->removeItem(0);
    this->modelConfigurationComboBox->blockSignals(false);
    this->actionClearConfigurationList->setEnabled(false);
  }
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
  this->multiView->showSelectedMarkers(items);
  this->markersTable->blockSignals(false);
};

void MainWindow::hideSelectedMarkers()
{
  QList<QTableWidgetItem*> items = this->markersTable->selectedItems();
  this->markersTable->blockSignals(true);
  this->multiView->hideSelectedMarkers(items);
  this->markersTable->blockSignals(false);
};

void MainWindow::showAllMarkers()
{
  this->markersTable->blockSignals(true);
  for (int row = 0 ; row < this->markersTable->rowCount() ; ++row)
    this->markersTable->item(row, 0)->setCheckState(Qt::Checked);
  this->markersTable->blockSignals(false);
  this->multiView->showAllMarkers();
};

void MainWindow::hideAllMarkers()
{
  this->markersTable->blockSignals(true);
  for (int row = 0 ; row < this->markersTable->rowCount() ; ++row)
    this->markersTable->item(row, 0)->setCheckState(Qt::Unchecked);
  this->markersTable->blockSignals(false);
  this->multiView->hideAllMarkers();
};

void MainWindow::updateSelectedMarkersRadius(double r)
{
  QList<int> ids = this->mp_ModelDock->selectedMarkers();
  for (int i = 0 ; i < ids.count() ; ++i)
    this->multiView->setMarkerRadius(ids[i], r);
  this->multiView->updateDisplay();
};

void MainWindow::updateMarkerRadius(double r)
{
  this->markerRadiusSlider->blockSignals(true);
  this->markerRadiusSlider->setValue(r * 10.0);
  this->markerRadiusSlider->blockSignals(false);
  QList<QTableWidgetItem*> items = this->markersTable->selectedItems();
  for (QList<QTableWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
    this->multiView->setMarkerRadius((*it)->data(markerId).toInt(), r);
  this->multiView->updateDisplay(this->timeEventControler->currentFrame());
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
    double r = this->multiView->markerRadius((*it)->data(markerId).toInt());
    int c = this->multiView->markerColorIndex((*it)->data(markerId).toInt());
    ++it;
    while (it != items.end())
    {
      if (r != this->multiView->markerRadius((*it)->data(markerId).toInt()))
        sameRadius = false;
      if (c != this->multiView->markerColorIndex((*it)->data(markerId).toInt()))
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
      double* rgba = this->multiView->markerColorValue(c);
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
  int index = this->modelConfigurationComboBox->currentIndex();
  if (modified == this->mp_MarkerConfigurationUndoStack->cleanIndex())
  {
    if (!this->modelConfigurationComboBox->itemData(index, visualConfigNew).toBool())
    {
      this->saveModelConfigurationButton->setEnabled(false);
      this->modelConfigurationComboBox->setItemText(index, this->modelConfigurationComboBox->itemData(index, visualConfigName).toString());
      this->modelConfigurationComboBox->setItemData(index, false, visualConfigChanged);
    }
  }
  else
  {
    if (!this->modelConfigurationComboBox->itemData(index, visualConfigChanged).toBool())
    {
      this->modelConfigurationComboBox->setItemText(index, "*" + this->modelConfigurationComboBox->itemData(index, visualConfigName).toString());
      this->modelConfigurationComboBox->setItemData(index, true, visualConfigChanged);
      this->saveModelConfigurationButton->setEnabled(true);
    }
  }
};

bool MainWindow::isOkToContinue2()
{
  int index = this->m_SelectedMarkerConfiguration;
  if (index == -1)
    return true;
    
  bool newConfig = this->modelConfigurationComboBox->itemData(index, visualConfigNew).toBool();
    
  if (this->modelConfigurationComboBox->itemData(index, visualConfigChanged).toBool())
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
        QString name = this->modelConfigurationComboBox->itemData(index, visualConfigName).toString();
        QString filename = QFileDialog::getSaveFileName(this,
                             trUtf8("Save Model Visual Configuration"),
                             this->m_LastDirectory + "/" + name + ".mvc",
                             trUtf8("Model Visual Configuration Files (*.mvc)"));
        if (filename.isEmpty())
          return false;
        this->modelConfigurationComboBox->setItemData(index, filename, visualConfigFilename);
        
      }
      return this->saveMarkerConfiguration(index);
    }
    else if (res == QMessageBox::No)
    {
      if (!this->modelConfigurationComboBox->itemData(index, visualConfigNew).toBool())
      {
        this->modelConfigurationComboBox->setItemData(index, false, visualConfigChanged);
        this->modelConfigurationComboBox->setItemText(index, this->modelConfigurationComboBox->itemData(index, visualConfigName).toString());
      }
      else
      {
        this->modelConfigurationComboBox->blockSignals(true);
        this->modelConfigurationComboBox->removeItem(index);
        this->resetMarkerConfiguration();
        if (this->modelConfigurationComboBox->count() == 2)
          this->modelConfigurationComboBox->setCurrentIndex(-1);
        this->modelConfigurationComboBox->blockSignals(false);
      }
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
      if (idx == (this->modelConfigurationComboBox->count() - 1))
        this->newMarkerConfiguration();
      else if (idx == -1)
      {
        this->resetMarkerConfiguration();
        this->m_SelectedMarkerConfiguration = -1;
        this->saveModelConfigurationButton->setEnabled(false);
        this->deleteModelConfigurationButton->setEnabled(false);
        this->actionDeselectCurrentConfiguration->setEnabled(false);
      }
      else
      {
        this->loadMarkerConfiguration(this->modelConfigurationComboBox->itemData(idx, visualConfigFilename).toString(),0);
        this->saveModelConfigurationButton->setEnabled(false);
        this->deleteModelConfigurationButton->setEnabled(true);
        this->m_SelectedMarkerConfiguration = idx;
        this->actionDeselectCurrentConfiguration->setEnabled(true);
        this->actionClearConfigurationList->setEnabled(true);
      }
    }
    else
    {
      this->modelConfigurationComboBox->blockSignals(true);
      this->modelConfigurationComboBox->setCurrentIndex(this->m_SelectedMarkerConfiguration);
      this->modelConfigurationComboBox->blockSignals(false);
    }
  }
  this->multiView->setFocus();
  this->m_SelectedMarkerConfiguration = this->modelConfigurationComboBox->currentIndex();
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
  int index = -1;
  if (configNameWidget.exec())
  {
    index = 0;
    this->modelConfigurationComboBox->insertItem(index, "*" + configNameWidget.configurationName());
    this->modelConfigurationComboBox->setItemData(index, true, visualConfigChanged);
    this->modelConfigurationComboBox->setItemData(index, true, visualConfigNew);
    this->modelConfigurationComboBox->setItemData(index, configNameWidget.configurationName(), visualConfigName);
    
    this->saveModelConfigurationButton->setEnabled(true);
    this->deleteModelConfigurationButton->setEnabled(true);
    this->actionDeselectCurrentConfiguration->setEnabled(true);
    this->actionClearConfigurationList->setEnabled(true);
    //this->mp_MarkerConfigurationUndoStack->clear();

    // Save this new configuration in a temporary file.
    QTemporaryFile file;
    if (file.open())
    {
      this->modelConfigurationComboBox->setItemData(index, file.fileName(), visualConfigFilename);
      this->saveMarkerConfiguration(index);
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
  else
    index = this->m_SelectedMarkerConfiguration;
  this->modelConfigurationComboBox->setCurrentIndex(index);
  this->modelConfigurationComboBox->blockSignals(false);

  this->multiView->setFocus();
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
  this->multiView->setFocus();
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
      double* color = this->multiView->markerColorValue(item->data(markerColorIndex).toInt());
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
  this->mp_MarkerConfigurationUndoStack->setClean();
  return true;
};

void MainWindow::loadMarkerConfiguration(const QString& filename)
{
  if (this->isOkToContinue2())
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
    if (!filename2.isEmpty() && this->loadMarkerConfiguration(filename2, &name))
    {
      bool alreadyExist = false;
      int i = 0;
      for (i = 0 ; i < (this->modelConfigurationComboBox->count() - 2) ; ++i)
      {
        if (name.compare(this->modelConfigurationComboBox->itemData(i,visualConfigName).toString()) == 0)
        {
          alreadyExist = true;
          break;
        }
      }
    
      int index = -1;
      if (!alreadyExist)
      {
        index = 0;
        this->modelConfigurationComboBox->blockSignals(true);
        this->modelConfigurationComboBox->insertItem(index, name);
        this->modelConfigurationComboBox->blockSignals(false);
      }
      else
        index = i;
      
      this->modelConfigurationComboBox->setItemData(index, false, visualConfigChanged);
      this->modelConfigurationComboBox->setItemData(index, false, visualConfigNew);
      this->modelConfigurationComboBox->setItemData(index, filename2, visualConfigFilename);
      this->modelConfigurationComboBox->setItemData(index, name, visualConfigName);
    
      this->modelConfigurationComboBox->blockSignals(true);
      this->modelConfigurationComboBox->setCurrentIndex(index);
      this->modelConfigurationComboBox->blockSignals(false);
    
      this->saveModelConfigurationButton->setEnabled(false);
      this->deleteModelConfigurationButton->setEnabled(true);
      this->actionDeselectCurrentConfiguration->setEnabled(true);
      this->actionClearConfigurationList->setEnabled(true);
    }
  
    this->multiView->setFocus();
  }
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
  QList<double> radius;
  QList<double*> colors;
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
              double* color = new double[3];
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
        double r = radius.at(idx);
        double* c = colors.at(idx);
        int idxColor;
        QColor color; color.setRgbF(c[0],c[1],c[2]);
        int idxMarker = item->data(markerId).toInt();
        this->multiView->appendNewMarkerColor(color, &idxColor);
        this->multiView->setMarkerRadius(idxMarker, r);
        this->multiView->setMarkerColorIndex(idxMarker, idxColor);
        item->setData(markerRadius, r);
        item->setData(markerColorIndex, idxColor);
        
        delete[] c;
      }
    }
  }
  this->markersTable->blockSignals(false);
  
  this->multiView->updateDisplay();
  
  return true;
};

void MainWindow::eraseMarkerConfiguration()
{
  int index = this->m_SelectedMarkerConfiguration;
  
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
  /*
  this->modelConfigurationComboBox->blockSignals(true);
  this->modelConfigurationComboBox->removeItem(index);
  if (this->modelConfigurationComboBox->count() <= 2)
  {
    this->modelConfigurationComboBox->setCurrentIndex(-1);
    this->saveModelConfigurationButton->setEnabled(false);
    this->deleteModelConfigurationButton->setEnabled(false); 
  }
  this->modelConfigurationComboBox->blockSignals(false);
  this->selectMarkerConfiguration(this->m_SelectedMarkerConfiguration);
  */
  this->modelConfigurationComboBox->blockSignals(true);
  this->m_SelectedMarkerConfiguration = -1;
  this->modelConfigurationComboBox->setCurrentIndex(this->m_SelectedMarkerConfiguration);
  this->modelConfigurationComboBox->removeItem(index);
  this->resetMarkerConfiguration();
  this->saveModelConfigurationButton->setEnabled(false);
  this->deleteModelConfigurationButton->setEnabled(false);
  this->actionDeselectCurrentConfiguration->setEnabled(false);
  this->modelConfigurationComboBox->blockSignals(false);
  
  this->multiView->setFocus();
};

// Go to the default visual configuration:

void MainWindow::resetMarkerConfiguration()
{
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
      this->multiView->setMarkerRadius(idxMarker, 8.0);
      this->multiView->setMarkerColorIndex(idxMarker, (int)0);
      item->setData(markerRadius, r);
      item->setData(markerColorIndex, idxColor);
    }
  }
  this->markersTable->blockSignals(false);
  this->multiView->updateDisplay();
}

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
    bool modified = this->multiView->appendNewMarkerColor(color, &i);
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
  this->multiView->circleSelectedMarkers(this->markersTable->selectedItems());
  this->multiView->updateDisplay(this->timeEventControler->currentFrame());
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
#if !defined(MOKKA_NEW_UI)
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
#else
  QList<int> ids;
  ids << id;
  this->mp_ModelDock->selectMarkers(ids);
#endif
};

void MainWindow::selectPickedMarkers(int id)
{
#if !defined(MOKKA_NEW_UI)
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
#else
  QList<int> ids = this->mp_ModelDock->selectedMarkers();
  ids << id;
  this->mp_ModelDock->selectMarkers(ids);
#endif
};

void MainWindow::modelDockLocationChanged(Qt::DockWidgetArea area)
{
  QSettings settings;
  settings.beginGroup("MarkersDock");
  settings.setValue("area", area);
  settings.endGroup();
};

void MainWindow::markersDockLocationChanged(Qt::DockWidgetArea area)
{
  QSettings settings;
  settings.beginGroup("MarkersDock");
  settings.setValue("area", area);
  settings.endGroup();
};

void MainWindow::setPointLabel(int id, const QString& label)
{
  // TODO: Group two undo commands to modify the acquisition and the configuration
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new EditPointLabel(this->mp_AcquisitionQ, id, label)));
};

void MainWindow::setMarkersRadius(const QVector<int>& ids, double radius)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_MarkerConfigurationUndoStack, new EditMarkersRadius2(this->mp_AcquisitionQ, ids, radius)));
};

void MainWindow::setMarkersColor(const QVector<int>& ids, const QColor& color)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_MarkerConfigurationUndoStack, new EditMarkersColor(this->mp_AcquisitionQ, ids, color)));
};

void MainWindow::setPointsDescription(const QVector<int>& ids, const QString& desc)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new EditPointDescription(this->mp_AcquisitionQ, ids, desc)));
};

void MainWindow::removePoints(const QList<int>& ids)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new RemovePoints(this->mp_AcquisitionQ, ids)));
};

void MainWindow::setAnalogLabel(int id, const QString& label)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new EditAnalogLabel(this->mp_AcquisitionQ, id, label)));
};

void MainWindow::setAnalogsDescription(const QVector<int>& ids, const QString& desc)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new EditAnalogDescription(this->mp_AcquisitionQ, ids, desc)));
};

void MainWindow::setAnalogsUnit(const QVector<int>& ids, const QString& unit)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new EditAnalogsUnit(this->mp_AcquisitionQ, ids, unit)));
};

void MainWindow::setAnalogsGain(const QVector<int>& ids, Analog::Gain gain)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new EditAnalogsGain(this->mp_AcquisitionQ, ids, gain)));
};

void MainWindow::setAnalogsOffset(const QVector<int>& ids, int offset)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new EditAnalogsOffset(this->mp_AcquisitionQ, ids, offset)));
};

void MainWindow::setAnalogsScale(const QVector<int>& ids, double scale)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new EditAnalogsScale(this->mp_AcquisitionQ, ids, scale)));
};

void MainWindow::removeAnalogs(const QList<int>& ids)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new RemoveAnalogs(this->mp_AcquisitionQ, ids)));
};

void MainWindow::removeEvents(const QList<int>& ids)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new RemoveEvents(this->mp_AcquisitionQ, ids)));
};

void MainWindow::setRegionOfInterest(int lf,int ff)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new EditRegionOfInterest(this->mp_AcquisitionQ, lf, ff)));
};

void MainWindow::insertEvent(Event* e)
{
  this->mp_UndoStack->push(new MasterUndoCommand(this->mp_AcquisitionUndoStack, new InsertEvent(this->mp_AcquisitionQ, e)));
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
#ifdef MOKKA_NEW_UI
  settings.beginGroup("MarkersDock"); // Should be ModelDock but for compatibility...
  this->mp_ModelDock->setFloating(settings.value("floating", false).toBool());
  this->mp_ModelDock->move(settings.value("pos", this->mp_ModelDock->pos()).toPoint());
  this->mp_ModelDock->resize(settings.value("size", this->mp_ModelDock->size()).toSize());
  if (!this->mp_ModelDock->isFloating())
    this->addDockWidget(static_cast<Qt::DockWidgetArea>(settings.value("area", Qt::RightDockWidgetArea).toInt()), this->mp_ModelDock);
  QStringList recentVisualConfigName = settings.value("recentVisualConfigName").toStringList();
  QStringList recentVisualConfigFilename = settings.value("recentVisualConfigFilename").toStringList(); 
  this->mp_ModelDock->setVisualConfigurations(recentVisualConfigName, recentVisualConfigFilename);
  settings.endGroup();
#else
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
#endif
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
  settings.beginGroup("MarkersDock");
  //settings.setValue("visible", this->markersDock->isVisible());
  settings.setValue("floating", this->markersDock->isFloating());
  settings.setValue("pos", this->markersDock->pos());
  settings.setValue("size", this->markersDock->size());
  //settings.setValue("area", this->markersDock->area());
  QStringList recentVisualConfigFilename; 
  QStringList recentVisualConfigName;
#ifdef MOKKA_NEW_UI
  this->mp_ModelDock->visualConfigurations(recentVisualConfigName, recentVisualConfigFilename);
#else
  for (int i = 0 ; i < (this->modelConfigurationComboBox->count() - 2) ; ++i)
  {
    if (!this->modelConfigurationComboBox->itemData(i,visualConfigNew).toBool())
    {
      recentVisualConfigFilename << this->modelConfigurationComboBox->itemData(i,visualConfigFilename).toString();
      recentVisualConfigName << this->modelConfigurationComboBox->itemData(i,visualConfigName).toString();
    }
  }
#endif
  settings.setValue("recentVisualConfigFilename", recentVisualConfigFilename);
  settings.setValue("recentVisualConfigName", recentVisualConfigName);
  settings.endGroup();
#ifndef MOKKA_NEW_UI
#endif
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
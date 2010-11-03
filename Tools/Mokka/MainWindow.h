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

#ifndef MainWindow_h
#define MainWindow_h

#include "ui_MainWindow.h"
#include "Acquisition.h"
#include "Metadata.h"
#include "ModelDockWidget.h"
#include "PointsEditor.h"
#include "UserRoles.h"

#include <btkAcquisitionFileIO.h>
#include <btkPointCollection.h>

#include <QTimer>
#include <QUndoStack>
#include <QXmlStreamWriter>

class MainWindow : public QMainWindow, public Ui::MainWindow
{
  Q_OBJECT
  
public:
  MainWindow(QWidget* parent = 0);
  ~MainWindow();

  void openFile(const QString& filename);
  void play();

  PointsEditor* mp_PointsEditorDlg;

protected:
    void closeEvent(QCloseEvent* event);
    bool eventFilter(QObject* obj, QEvent* event);

public Q_SLOTS:
  // Qt
  // Menu
  void about();
  void visitBTKWebsite();
  void setAcquisitionModified(int modified);
  void setMarkerConfigurationModified(int modified);
  void editMetadata();
  void editPoints();
  void openRecentFile();
  void clearRecentFiles();
  void openFile();
  void openFileDropped(const QString& filename);
  void saveFile();
  void saveAsFile();
  void closeFile();
  void changePlaybackParameters();
  void changeGroundOrientation();
  void deselectCurrentVisualConfiguration();
  void clearVisualConfigurationList();
  // Markers dock
  void selectMarkerConfiguration(int index);
  void newMarkerConfiguration();
  void saveMarkerConfiguration();
  void loadMarkerConfiguration(const QString& filename = "");
  void eraseMarkerConfiguration();
  void resetMarkerConfiguration();
  void editMarkerLabel();
  void editMarkerDescription();
  void editMarkerRadius();
  void editMarkerColor();
  void updateMarkerRadiusSpinBox(int v);
  void markersDockLocationChanged(Qt::DockWidgetArea area);
  void displayMarkerProperties();
  void circleSelectedMarkers();
  void toggleMarkersVisibilityButtons();
  void showSelectedMarkers();
  void hideSelectedMarkers();
  void showAllMarkers();
  void hideAllMarkers();
  void updateMarkerRadius(double r);
  void toggleMarkerProperties();
  void focusOnMarkerLabelEdition();
  void updateDisplayedMarkersList(const QVector<int>& ids);
  void selectPickedMarker(int id);
  void selectPickedMarkers(int id);
  // Events dock
  void editEventLabel();
  void editEventContext(const QString& context);
  void editEventTime();
  void editEventSubject();
  void newEvent();
  void deleteEvent();
  void updateActiveEvent(int frame);
  void displayEventInformations();
  void toggleEventInformations();
  void updateEventInternalInformations(QTableWidgetItem* item);
  void updateEventsButtonsState();
  void selectEvent();
  void eventsDockLocationChanged(Qt::DockWidgetArea area);
  void focusOnEventEdition(int idx = -1);
  // Model dock
  void modelDockLocationChanged(Qt::DockWidgetArea area);
  void setPointLabel(int id, const QString& label);
  void setMarkersRadius(const QVector<int>& ids, double radius);
  void setMarkersColor(const QVector<int>& ids, const QColor& color);
  void setPointsDescription(const QVector<int>& ids, const QString& desc);
  void removePoints(const QList<int>& ids);
  void setAnalogLabel(int id, const QString& label);
  void setAnalogsDescription(const QVector<int>& ids, const QString& desc);
  void setAnalogsUnit(const QVector<int>& ids, const QString& unit);
  void setAnalogsGain(const QVector<int>& ids, AnalogGain gain);
  void setAnalogsOffset(const QVector<int>& ids, int offset);
  void setAnalogsScale(const QVector<int>& ids, double scale);
  void removeAnalogs(const QList<int>& ids);
  // Time event
  void setRegionOfInterest(int lf,int ff);
  void removeEvents(const QList<int>& ids);
  void insertEvent(Event* e);
  // Playback
  void toggleTimer();
  void displayPreviousFrame();
  void displayNextFrame();
  // Others
  void updateSelectedMarkersRadius(double r);
  
private:
  void saveFile(const QString& filename);
  void clearUI();
  void fillFileInformations(const QString& filename, btk::AcquisitionFileIO::Pointer io, btk::Acquisition::Pointer acq);
  void readSettings(); 
  void writeSettings();
  void updateRecentFileActions();
  void setCurrentFile(const QString& rFilename);
  bool isOkToContinue();
  void displayPreviousFrame(int step);
  void displayNextFrame(int step);
  bool isOkToContinue2();
  bool saveMarkerConfiguration(int index);
  bool loadMarkerConfiguration(const QString& filename, QString* name);
  
  btk::Acquisition::Pointer mp_Acquisition;
  Acquisition* mp_AcquisitionQ;
  Metadata* mp_MetadataDlg;
  ModelDockWidget* mp_ModelDock;
  int m_PlaybackStep;
  int m_PlaybackDelay;

  int m_SelectedMarkerConfiguration;
  QActionGroup* mp_PlaybackSpeedActionGroup;
  QActionGroup* mp_GroundOrientationActionGroup;
  QString m_LastDirectory;
  QTimer* mp_Timer;
  enum { maxRecentFiles = 10 };
  QStringList m_RecentFiles;
  QAction* mp_ActionRecentFiles[maxRecentFiles];
  QAction* mp_ActionSeparatorRecentFiles;
  QUndoStack* mp_UndoStack;
  QUndoStack* mp_AcquisitionUndoStack;
  QUndoStack* mp_MarkerConfigurationUndoStack;
  QIcon* mp_PlayIcon;
  QIcon* mp_PauseIcon;
  QIcon* mp_DownArrow;
  QIcon* mp_RightArrow;
};

bool findLabel(btk::PointCollection::Pointer points, const std::string& label);

class TrajectoryCheckBox : public QCheckBox
{
  Q_OBJECT
  
public:
  TrajectoryCheckBox(QTableWidgetItem* item)
  : QCheckBox()
  {
    this->setStyleSheet(
      "QCheckBox { spacing: 0px; margin-left:5px; }\
       QCheckBox::indicator { width: 13px; height: 13px; }\
       QCheckBox::indicator:unchecked { image: url(:/Resources/Images/trajectory_unactive.png); }\
       QCheckBox::indicator:unchecked:hover { image: url(:/Resources/Images/trajectory_unactive.png); }\
       QCheckBox::indicator:unchecked:pressed { image: url(:/Resources/Images/trajectory_unactive.png); }\
       QCheckBox::indicator:checked { image: url(:/Resources/Images/trajectory_active.png); }\
       QCheckBox::indicator:checked:hover { image: url(:/Resources/Images/trajectory_active.png); }\
       QCheckBox::indicator:checked:pressed { image: url(:/Resources/Images/trajectory_active.png); }\
       QCheckBox::indicator:indeterminate:hover { image: url(:/Resources/Images/trajectory_active.png); }\
       QCheckBox::indicator:indeterminate:pressed { image: url(:/Resources/Images/trajectory_active.png); }"
     );
    this->setCheckState(Qt::Unchecked);
    //this->setDisabled(true);
    this->mp_Item = item;
    connect(this, SIGNAL(stateChanged(int)), this, SLOT(itemChanged(int)));
  };
protected:
/*
  virtual void mousePressEvent(QMouseEvent* event)
  {
    //static_cast<QWidget*>(this->parent())->setFocus();
  }
*/
private Q_SLOTS:
  void itemChanged(int s)
  {
    this->mp_Item->setData(checkState2, s);
  };
  
private:
  QTableWidgetItem* mp_Item;
};
#endif // MainWindow_h

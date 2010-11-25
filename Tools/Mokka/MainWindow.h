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
#include "FileInfoDockWidget.h"
#include "Metadata.h"
#include "ModelDockWidget.h"
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

  void play();
  void loadConfiguration(const QString& filename);
  void openFile(const QString& filename);

protected:
    void closeEvent(QCloseEvent* event);
    bool eventFilter(QObject* obj, QEvent* event);

public Q_SLOTS:
  // Qt
  // Menu
  void about();
  void help();
  void visitBTKWebsite();
  void setAcquisitionModified(int modified);
  void viewMetadata();
  void openRecentFile();
  void clearRecentFiles();
  void openFile();
  void openFileDropped(const QString& filename);
  void saveFile();
  void saveAsFile();
  void closeFile();
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
  void setAnalogsGain(const QVector<int>& ids, Analog::Gain gain);
  void setAnalogsOffset(const QVector<int>& ids, int offset);
  void setAnalogsScale(const QVector<int>& ids, double scale);
  void removeAnalogs(const QList<int>& ids);
  // Time event
  void setRegionOfInterest(int lf,int ff);
  void setEventFrame(int id, int frame);
  void setEvents(QList<int> ids, QList<Event*> events);
  void removeEvents(const QList<int>& ids);
  void insertEvent(Event* e);
  // Others
  void updateSelectedMarkersRadius(double r);
  void selectPickedMarker(int id);
  void togglePickedMarker(int id);
  void selectSelectedMarkers(const QList<int>& ids);
  void toggleMarkerTrajectory(int id);
  
private:
  void saveFile(const QString& filename);
  void reset();
  void readSettings(); 
  void writeSettings();
  void updateRecentFileActions();
  void setCurrentFile(const QString& rFilename);
  bool isOkToContinue();
  
  btk::Acquisition::Pointer mp_Acquisition;
  Acquisition* mp_AcquisitionQ;
  Metadata* mp_MetadataDlg;
  FileInfoDockWidget* mp_FileInfoDock;
  ModelDockWidget* mp_ModelDock;

  QString m_LastDirectory;
  enum { maxRecentFiles = 10 };
  QStringList m_RecentFiles;
  QAction* mp_ActionRecentFiles[maxRecentFiles];
  QAction* mp_ActionSeparatorRecentFiles;
  QUndoStack* mp_UndoStack;
  QUndoStack* mp_AcquisitionUndoStack;
  QUndoStack* mp_MarkerConfigurationUndoStack;
};
#endif // MainWindow_h

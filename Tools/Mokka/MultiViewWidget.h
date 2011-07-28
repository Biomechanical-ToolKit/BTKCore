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

#ifndef MultiViewWidget_h
#define MultiViewWidget_h

#include "AbstractMultiView.h"

#include <vtkEventQtSlotConnect.h>
#include <vtkMapperCollection.h>

#include <QMenu>

// Forward declaration
class AbstractView;
class Acquisition;
struct Segment;
class Model;
class vtkStreamingDemandDrivenPipelineCollection;
class vtkProcessMap;
class vtkDoubleArray;

class MultiViewWidget : public AbstractMultiView
{
  Q_OBJECT
  
public:
  MultiViewWidget(QWidget* parent = 0);
  ~MultiViewWidget();
  // MultiViewWidget(const MultiViewWidget&); // Implicit.
  // MultiViewWidget& operator=(const MultiViewWidget&); // Implicit.
  
  void initialize();
  void setViewActions(QList<QAction*> actions);
  
  void setAcquisition(Acquisition* acq);
  void setModel(Model* m);
  void load();
  
  int segmentColorIndex(int id);
  
  void setMarkerRadius(int id, double r);
  double markerRadius(int id);
  void setMarkerColorIndex(int id, int idx);
  int markerColorIndex(int id);
  void setMarkerVisibility(int id, bool visible);
  bool markerVisibility(int id);
  double* markerColorValue(int c);
  bool appendNewMarkerColor(const QColor& color, int* idx);
  QMenu* groundOrientationMenu() const {return this->mp_GroupOrientationMenu;};
  const QString groundNormalAsString() const;
  QMenu* markerTrajectoryLengthMenu() const {return this->mp_MarkerTrajectoryLengthMenu;};
  
  void setDefaultGroundOrientation(int index);
  void setDefaultSegmentColor(const QColor& color);
  void setDefaultMarkerColor(const QColor& color);
  void setDefaultMarkerRadius(double r);
  void setMarkerTrajectoryLength(int index);
  
  void showForcePlatformAxes(bool isShown);
  void showForcePlatformIndex(bool isShown);
  void setForcePlatformColor(const QColor& color);
  void setForceVectorColor(const QColor& color);
  
  QObject* eventFilterObject() const {return this->mp_EventFilterObject;};
  void setEventFilterObject(QObject* filter);
  
public slots:
  void appendNewSegments(const QList<int>& ids, const QList<Segment*>& segments);
  void clearSegments();
  void setSegmentsColor(const QVector<int>& ids, const QVector<QColor>& colors);
  void setSegmentLink(int id, const QVector<int>& markerIds, const QVector< QPair<int,int> >& links);
  void updateHiddenSegments(const QList<int>& ids);
  void setMarkersRadius(const QVector<int>& ids, const QVector<double>& radii);
  void setMarkersColor(const QVector<int>& ids, const QVector<QColor>& colors);
  void updateHiddenMarkers(const QList<int>& ids);
  void updateTrackedMarkers(const QList<int>& ids);
  void clear();
  void circleSelectedMarkers(const QList<int>& ids);
  void updateSegmentsDisplay();
  void updateMarkersDisplay();
  void updateDisplay();
  void updateDisplay(int frame);
  void showAllMarkers();
  void hideAllMarkers();
  void forceRubberBandDrawingOn();
  void forceRubberBandDrawingOff();

protected:
  void dragEnterEvent(QDragEnterEvent *event);
  void dropEvent(QDropEvent *event);
  
  AbstractView* createView(AbstractView* fromAnother = 0);

signals:
  void fileDropped(const QString& filename);
  void visibleMarkersChanged(const QVector<int>& ids);
  void pickedMarkerChanged(int id);
  void pickedMarkerToggled(int id);
  void selectedMarkersToggled(const QList<int>& ids);
  void trajectoryMarkerToggled(int id);
  
private slots:
  // Qt / VTK
  void updateDisplayedMarkersList(vtkObject* caller, unsigned long vtk_event, void* client_data, void* call_data);
  // Qt
  void changeGroundOrientation();
  void changeMarkerTrajectoryLength();
  
private:
  void updateCameras();
  void updateViews();
  
  QObject* mp_EventFilterObject;
  Acquisition* mp_Acquisition;
  Model* mp_Model;
  vtkEventQtSlotConnect* mp_EventQtSlotConnections;
  vtkProcessMap* mp_VTKProc;
  vtkMapperCollection* mp_Mappers;
  vtkStreamingDemandDrivenPipelineCollection* mp_Syncro;
  vtkDoubleArray* mp_PointChartFrames; // Values for the X axis shared by each point chart.
  vtkDoubleArray* mp_AnalogChartFrames; // Values for the X axis shared by each analog chart.
  QColor m_ForcePlatformColor;
  vtkActor* mp_ForcePlatformActor;
  QColor m_ForceVectorColor;
  vtkActor* mp_ForceVectorActor;
  QMenu* mp_GroupOrientationMenu;
  QAction* mp_ActionGroundOrientationAutomatic;
  QAction* mp_ActionGroundOrientationPlaneXY;
  QAction* mp_ActionGroundOrientationPlaneYZ;
  QAction* mp_ActionGroundOrientationPlaneZX;
  QMenu* mp_MarkerTrajectoryLengthMenu;
  QAction* mp_ActionMarkerTrajectoryFull;
  QAction* mp_ActionMarkerTrajectory25;
  QAction* mp_ActionMarkerTrajectory50;
  QAction* mp_ActionMarkerTrajectory100;
  QAction* mp_ActionMarkerTrajectory200;
  QList<QAction*> m_ViewActions;
};

#endif // MultiViewWidget_h
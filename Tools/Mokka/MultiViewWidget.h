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

#ifndef MultiViewWidget_h
#define MultiViewWidget_h

#include "AbstractMultiView.h"
#include "Model.h" // Pair
#include "ChartCycleSettingsManager.h"

#include <btkVTKChartTimeSeries.h> // VTKCurrentFrameFunctor, VTKRegionOfInterestFunctor, VTKEventsFunctor

#include <vtkEventQtSlotConnect.h>
#include <vtkMapperCollection.h>

#include <QMenu>

// Forward declaration
class AbstractView;
class CompositeView;
class Acquisition;
struct Segment;
class vtkStreamingDemandDrivenPipelineCollection;
class vtkProcessMap;
class vtkActorMap;
class vtkFloatArray;
class ChartDialog;
class ChartExportDialog;

class EventsFrameMapperFunctor : public btk::VTKEventsFrameMapperFunctor
{
public:
  typedef SharedPtr<EventsFrameMapperFunctor> Pointer;
  static EventsFrameMapperFunctor::Pointer New(vtkFloatArray* (*abscissa)[4], vtkFloatArray* (*boundaries)[3]) {return Pointer(new EventsFrameMapperFunctor(abscissa, boundaries));};
  virtual ~EventsFrameMapperFunctor() {};
  virtual float operator()(int index, int side, int shift);
  bool GetActivated() {return this->m_Activated;};
  void SetActivated(bool activated) {this->m_Activated = activated;};
protected:
  EventsFrameMapperFunctor(vtkFloatArray* (*abscissa)[4], vtkFloatArray* (*boundaries)[3]);
private:
  float extractCyclePercent(int side, int idx);
  EventsFrameMapperFunctor(const EventsFrameMapperFunctor& ); // Not implemented.
  EventsFrameMapperFunctor& operator=(const EventsFrameMapperFunctor& ); // Not implemented.  
  vtkFloatArray* (*mp_HorizontalAbscissa)[4];
  vtkFloatArray* (*mp_CyclesBoundaries)[3];
  bool m_Activated;
};

class MultiViewWidget : public AbstractMultiView
{
  Q_OBJECT
  
public:
  MultiViewWidget(QWidget* parent = 0);
  ~MultiViewWidget();
  // MultiViewWidget(const MultiViewWidget&); // Implicit.
  // MultiViewWidget& operator=(const MultiViewWidget&); // Implicit.
  
  void initialize();
  void setView3dActions(QList<QAction*> actions);
  void setViewChartActions(QList<QAction*> actions);
    
  void setAcquisition(Acquisition* acq);
  void setModel(Model* m);
  void setChartCycleSettingsManager(ChartCycleSettingsManager* manager);
  void load();
  
  int appendNewSegment(Segment* s);
  void setSegment(int id, Segment* s);
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
  QAction* forceButterflyActivationAction() const {return this->mp_ForceButterflyActivationAction;};
  QMenu* chartHorizontalAxisUnitMenu() const {return this->mp_ChartHorizontalAxisUnitMenu;};
  QAction* manageChartCycleSettingsAction() const {return this->mp_ManageChartCycleSettings;};
  QMenu* contextualChartHorizontalAxisUnitMenu() const {return this->mp_ContextualChartHorizontalAxisUnitMenu;};
  
  void setDefaultGroundOrientation(int index);
  void setDefaultBackgroundColor(const QColor& color);
  void setDefaultGridFrontColor(const QColor& color);
  void setDefaultGridBackColor(const QColor& color);
  void setDefaultSegmentColor(const QColor& color);
  void setDefaultMarkerColor(const QColor& color);
  void setDefaultMarkerRadius(double r);
  void setMarkerTrajectoryLength(int index);
  
  void showForcePlatformAxes(bool isShown);
  void showForcePlatformIndex(bool isShown);
  void setForcePlatformColor(const QColor& color);
  void setForceVectorColor(const QColor& color);
  void setGRFButterflyActivation(bool activated);
  
  void setVideoDelay(int id, double d);
  
  void setCurrentFrameFunctor(btk::VTKCurrentFrameFunctor::Pointer functor);
  void setRegionOfInterestFunctor(btk::VTKRegionOfInterestFunctor::Pointer functor);
  void setEventsFunctor(btk::VTKEventsFunctor::Pointer functor);
  
  QObject* eventFilterObject() const {return this->mp_EventFilterObject;};
  void setEventFilterObject(QObject* filter);
  
  QByteArray saveLayout() const;
  bool restoreLayout(const QByteArray& state);
  
  ChartDialog* createChartDialog(QWidget* parent = 0);
  
public slots:
  void updateFramesIndex(int ff);
  void appendNewSegments(const QList<int>& ids, const QList<Segment*>& segments);
  void clearSegments();
  void setSegmentsColor(const QVector<int>& ids, const QVector<QColor>& colors);
  void setSegmentDefinition(int id, const QVector<int>& markerIds, const QVector<Pair>& links, const QVector<Triad>& faces);
  void updateSegmentDefinition(int id);
  void setSegmentsVisibility(const QVector<int>& ids, const QVector<bool>& visibles);
  void setSegmentsSurfaceVisibility(const QVector<int>& ids, const QVector<bool>& visibles);
  void updateHiddenSegments(const QList<int>& ids);
  void setMarkersRadius(const QVector<int>& ids, const QVector<double>& radii);
  void setMarkersColor(const QVector<int>& ids, const QVector<QColor>& colors);
  void updateVisibleMarkers(const QList<int>& ids);
  void updateHiddenMarkers(const QList<int>& ids);
  void updateTrackedMarkers(const QList<int>& ids);
  void updateTrackedGRFPaths(const QList<int>& ids);
  void setMarkersVisibility(const QVector<int>& ids, const QVector<bool>& visibles);
  void setMarkersTrajectoryVisibility(const QVector<int>& ids, const QVector<bool>& visibles);
  void markersConfiguration(const QList<int>& ids, QList<bool>& visibles, QList<bool>& trajectories, QList<double>& radii, QList<QColor>& colors);
  void setMarkersConfiguration(const QList<int>& ids, const QList<bool>& visibles, const QList<bool>& trajectories, const QList<double>& radii, const QList<QColor>& colors);
  void clear();
  void circleSelectedMarkers(const QList<int>& ids);
  void updateSegmentsDisplay();
  void updateMarkersDisplay();
  void updateDisplay();
  void updateDisplay(int frame);
  void showAllMarkers();
  void hideAllMarkers();
  void adaptViewsForPlaybackOn();
  void adaptViewsForPlaybackOff();
  void restoreLayout3DOnly();
  void restoreLayout3DVerbose();
  void restoreLayout3DCharts();
  void setVideoDelays(QVector<int> ids, QVector<qint64> delays);
  void updateChartHorizontalAxisUnit();
  void setChartHorizontalAxisUnit(int index);
  void setChartHorizontalAxisUnitToFrame();
  void setChartHorizontalAxisUnitToTime();
  void setChartHorizontalAxisUnitToCycle(int index);
  void showChartEvent(bool visible);
  void setDefaultPlotLineWidth(double width);
  void exportChartToImage(btk::VTKChartTimeSeries* chart);

protected:
  void dragEnterEvent(QDragEnterEvent *event);
  void dropEvent(QDropEvent *event);
  AbstractView* createView(AbstractView* fromAnother = 0);
  bool saveLayout(QDataStream& stream, QWidget* w) const;
  bool restoreLayout(QDataStream& stream, CompositeView* view, const QSize& size);
  bool restoreLayout2(QDataStream& stream, CompositeView* view, const QSize& size);
  bool restoreLayoutView(QDataStream& stream, CompositeView* view, const QSize& size, QSplitter** splitter, AbstractView* views[2], QSize viewsSize[2], QList<int>& sizes);

signals:
  void fileDropped(const QString& filename);
  void visibleMarkersChanged(const QVector<int>& ids);
  void pickedMarkerChanged(int id);
  void pickedMarkerToggled(int id);
  void selectedMarkersToggled(const QList<int>& ids);
  void trajectoryMarkerToggled(int id);
  void pausePlaybackRequested(bool paused);
  
private slots:
  // Qt / VTK
  void updateDisplayedMarkersList(vtkObject* caller, unsigned long vtk_event, void* client_data, void* call_data);
  // Qt
  void changeGroundOrientation();
  void changeMarkerTrajectoryLength();
  void changeForceButterflyActivation();
  void updateAnalogValuesModification(const QVector<int>& ids);
  void addChartCycleSettingAction();
  void updateChartCycleSettingActions();
  void updateChartCycleSettingAction(int index);
  void removeChartCycleSettingAction(int index);
  void updateCurrentChartCycleSettingAction(int index);
  
private:
  void displayChartHorizontalAxisAsFrame();
  void displayChartHorizontalAxisAsTime();
  void displayChartHorizontalAxisAsCycle(int index);
  void updateCameras();
  void updateViews();
  void createCyclesFromEventsFrame(int ctx, const QList<int>& begin, const QList<int>& end);
  void updateChartCycleSettingActions(int startIndex);
  
  QObject* mp_EventFilterObject;
  Acquisition* mp_Acquisition;
  Model* mp_Model;
  QMap<int, qint64> m_VideoDelays;
  vtkEventQtSlotConnect* mp_EventQtSlotConnections;
  vtkProcessMap* mp_VTKProc;
  vtkActorMap* mp_VTKActor;
  vtkMapperCollection* mp_Mappers;
  vtkStreamingDemandDrivenPipelineCollection* mp_Syncro;
  vtkFloatArray* mp_PointChartAbscissa[4]; // Values for the X axis shared by each point chart.
  vtkFloatArray* mp_PointChartCycleBoundaries[3];
  vtkFloatArray* mp_AnalogChartAbscissa[4]; // Values for the X axis shared by each analog chart.
  vtkFloatArray* mp_AnalogChartCycleBoundaries[3];
  QColor m_ForcePlatformColor;
  QColor m_ForceVectorColor;
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
  QAction* mp_ForceButterflyActivationAction;
  QList<QAction*> m_View3dActions;
  QList<QAction*> m_ViewChartActions;
  QMenu* mp_ChartHorizontalAxisUnitMenu;
  QMenu* mp_ContextualChartHorizontalAxisUnitMenu;
  QAction* mp_ActionChartAxisFrame;
  QAction* mp_ActionChartAxisTime;
  ChartExportDialog* mp_ChartExporter;
  ChartCycleSettingsManager* mp_ChartCycleSettingsManager;
  QAction* mp_ActionCycleSettings[ChartCycleSettingsManager::maxCycleSettings];
  QAction* mp_ManageChartCycleSettings;
  EventsFrameMapperFunctor::Pointer mp_EventsFrameMapperFunctor;
};

#endif // MultiViewWidget_h
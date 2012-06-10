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

#ifndef ModelDockWidget_h
#define ModelDockWidget_h

#include "ui_ModelDockWidget.h"
#include "NewModelDialog.h" // ConfigurationItem
#include "Acquisition.h" // Point, Analog, ...
#include "Model.h" // Segment

#include <QDockWidget>
#include <QMenu>
#include <QVector>
#include <QColor>
#include <QXmlStreamReader>

class ModelDockWidget : public QDockWidget, public Ui::ModelDockWidget
{
  Q_OBJECT

public:
  enum {LabelHeader = 0, VisibleHeader = 2, TrajectoryHeader = 1};
  enum {SegmentsItem = 0, MarkersItem, VirtualMarkersItem, AnalogsItem, ForcePlatesItem, ModelOutputsItem, VideosItem};
  
  ModelDockWidget(QWidget* parent = 0);
  ~ModelDockWidget();
  
  void setAcquisition(Acquisition* acq);
  void setModel(Model* m);
  void load();
  void reset();
  void visualConfigurations(QStringList& names, QStringList& filenames);
  void setVisualConfigurations(const QStringList& names, const QStringList& filenames);
  void loadConfiguration(const QString& filename);
  int currentConfigurationIndex() const {return this->m_CurrentConfigurationIndex;};
  QList<int> selectedMarkers() const;
  QList<int> tailedMarkers() const;
  QList<int> selectedVideos() const;
  
  QAction* deselectConfigurationAction() const {return this->mp_DeselectConfiguration;};
  QAction* clearConfigurationsAction() const {return this->mp_ClearConfigurations;};
  QAction* newSegmentAction() const {return this->mp_NewSegment;};
  QAction* deleteSelectedMarkersAction() {return this->mp_DeleteSelectedMarkers;};  
  QAction* selectAllMarkersAction() {return this->mp_SelectAllMarkers;};  
  QAction* hideSelectedMarkersAction() {return this->mp_HideSelectedMarkers;};
  QAction* unhideSelectedMarkersAction() {return this->mp_UnhideSelectedMarkers;};
  QAction* activeTrajectorySelectedMarkersAction() {return this->mp_TrackSelectedMarkers;};
  QAction* unactiveTrajectorySelectedMarkersAction() {return this->mp_UntrackSelectedMarkers;};
  
  void setRecentColor(int idx, const QColor& color);
  const QColor& recentColor(int idx) const {return this->m_RecentColors[idx];};
  
public slots:
  // Configuration
  bool isOkToContinue();
  void selectConfiguration(int idx);
  void setConfigurationModified(bool cleaned);
  void newConfiguration();
  void loadConfiguration();
  void saveConfiguration();
  void removeConfiguration();
  void deselectConfiguration();
  void clearConfigurations();
  // Segments
  void hideSelectedSegments();
  void unhideSelectedSegments();
  void setSegments(const QList<int>& ids, const QList<Segment*>& segments);
  void setSegmentLabel(int id, const QString& label);
  void setSegmentsColor(const QVector<int>& ids, const QVector<QColor>& colors);
  void setSegmentsDescription(const QVector<int>& ids, const QVector<QString>& descs);
  void setSegmentsVisibility(const QVector<int>& ids, const QVector<bool>& visibles);
  void setSegmentsSurfaceVisibility(const QVector<int>& ids, const QVector<bool>& visibles);
  void removeSegments(const QList<int>& ids, const QList<Segment*>& segments);
  void insertSegments(const QList<int>& ids, const QList<Segment*>& segments);
  void editSegmentLabel();
  void editSegmentsDescription();
  void editSegmentsVisibility();
  void editSegmentsSurfaceVisibility();
  // Markers & Points
  void updateDisplayedMarkers(const QVector<int>& ids);
  void setTrackedMarkers(const QList<int>& ids);
  void selectMarkers(QList<int> ids);
  void selectAllMarkers();
  void hideSelectedMarkers();
  void unhideSelectedMarkers();
  void trackSelectedMarkers();
  void untrackSelectedMarkers();
  void selectAllAnalogs();
  void selectAllModelOutputs();
  void selectAllAngles(bool appending = false);
  void selectAllForces(bool appending = false);
  void selectAllMoments(bool appending = false);
  void selectAllPowers(bool appending = false);
  void selectAllScalars(bool appending = false);
  void removeSelectedItems();
  void toggleProperties();
  void displayProperties();
  void editMarkerLabel();
  //void setMarkerLabel(int id, const QString& label);
  void editMarkersDescription();
  //void setMarkerDescription(int id, const QString& desc);
  void updateMarkerRadiusSpinBox(int v);
  void updateMarkerRadius(double r);
  void editMarkersRadius();
  void setMarkersRadius(const QVector<int>& ids, const QVector<double>& radii);
  void editMarkersColor();
  void setMarkersColor(const QVector<int>& ids, const QVector<QColor>& colors);
  void setMarkerColorFromRecent1();
  void setMarkerColorFromRecent2();
  void setMarkerColorFromRecent3();
  void setMarkerColorFromRecent4();
  void setMarkerColorFromRecent5();
  void resetMarkerRecentColors();
  void setMarkerRecentColor1();
  void setMarkerRecentColor2();
  void setMarkerRecentColor3();
  void setMarkerRecentColor4();
  void setMarkerRecentColor5();
  void editMarkersVisibility();
  void setMarkersVisibility(const QVector<int>& ids, const QVector<bool>& visibles);
  void editMarkersTrajectoryVisibility();
  void setMarkersTrajectoryVisibility(const QVector<int>& ids, const QVector<bool>& visibles);
  void setMarkersConfiguration(const QList<int>& ids, const QList<bool>& visibles, const QList<bool>& trajectories, const QList<double>& radii, const QList<QColor>& colors);
  void editPointLabel();
  void setPointLabel(int id, const QString& label);
  void editPointsDescription();
  void setPointsDescription(const QVector<int>& ids, const QVector<QString>& descs);
  void removePoints(const QList<int>& ids, const QList<Point*>& points);
  void insertPoints(const QList<int>& ids, const QList<Point*>& points);
  // Analogs
  void editAnalogLabel();
  void setAnalogLabel(int, const QString& label);
  void editAnalogsUnit();
  void setAnalogsUnit(const QVector<int>& ids, const QVector<QString>& units);
  void editAnalogsGain(int index);
  void setAnalogsGain(const QVector<int>& ids, const QVector<Analog::Gain>& gains);
  void editAnalogsOffset();
  void setAnalogsOffset(const QVector<int>& ids, const QVector<int>& offsets);
  void editAnalogsScale();
  void setAnalogsScale(const QVector<int>& ids, const QVector<double>& scales);
  void editAnalogsDescription();
  void setAnalogsDescription(const QVector<int>& ids, const QVector<QString>& descs);
  void removeAnalogs(const QList<int>& ids, const QList<Analog*>& analogs);
  void insertAnalogs(const QList<int>& ids, const QList<Analog*>& analogs);
  // Force platforms
  void showGroundRectionForcePaths();
  void hideGroundRectionForcePaths();
  void setGroundRectionForcePathsVisibility(bool visible);
  // Videos
  void editVideosDelay();
  void setVideosDelay(const QVector<int>& ids, const QVector<qint64>& delay);
  void updateVideoDelaySpinBox(int d);
  void updateVideoDelay(double d);
  void removeVideos(const QList<int>& ids, const QList<Video*>& videos);
  void insertVideos(const QList<int>& ids, const QList<Video*>& videos);
  void importVideos(const QList<int>& ids, const QList<Video*>& videos);
  
signals:
  void configurationSaved();
  void segmentCreationRequested();
  void segmentEditionRequested();
  void segmentLabelChanged(int id, const QString& label);
  void segmentsColorChanged(const QVector<int>& ids, const QColor& color);
  void segmentsDescriptionChanged(const QVector<int>& ids, QString desc);
  void segmentsVisibilityChanged(const QVector<int>& ids, bool visible);
  void segmentsSurfaceVisibilityChanged(const QVector<int>& ids, bool visible);
  void segmentsRemoved(const QList<int>& ids);
  void segmentsCleared();
  void newSegmentsInserted(const QList<int>& ids, const QList<Segment*>& segments);
  void segmentHiddenSelectionChanged(const QList<int>& ids);
  void markerSelectionChanged(const QList<int>& ids);
  void markerHiddenSelectionChanged(const QList<int>& ids);
  void markerTrajectorySelectionChanged(const QList<int>& ids);
  void wrenchPositionSelectionChanged(const QList<int>& ids);
  void markerLabelChanged(int id, const QString& label);
  void markersRadiusChanged(const QVector<int>& ids, double radius);
  void markersColorChanged(const QVector<int>& ids, const QColor& colors);
  void markersDescriptionChanged(const QVector<int>& ids, const QString& desc);
  void markersVisibilityChanged(const QVector<int>& ids, bool visible);
  void markersTrajectoryVisibilityChanged(const QVector<int>& ids, bool visible);
  void pointLabelChanged(int id, const QString& label);
  void pointsDescriptionChanged(const QVector<int>& ids, const QString& desc);
  void pointsRemoved(const QList<int>& ids);
  void analogLabelChanged(int id, const QString& label);
  void analogsUnitChanged(const QVector<int>& ids, const QString& unit);
  void analogsGainChanged(const QVector<int>& ids, const Analog::Gain gain);
  void analogsOffsetChanged(const QVector<int>& ids, int offset);
  void analogsScaleChanged(const QVector<int>& ids, double scale);
  void analogsDescriptionChanged(const QVector<int>& ids, const QString& desc);
  void analogsRemoved(const QList<int>& ids);
  void videosDelayChanged(const QVector<int>& ids, qint64 delay);
  void videosRemoved(const QList<int>& ids);

protected:
  void changeEvent(QEvent* event);
  
private slots:
  void sendSelectedMarkers();
  void sendModifiedMarkersState(QTreeWidgetItem* item, int column);
  
private:
  void appendSegments(QXmlStreamReader& xmlReader, QList<Segment>& segments) const;
  void setCurrentConfiguration(int idx);
  bool saveConfiguration(int idx);
  void removeConfiguration(int idx);
  void setConfigurationModified(int idx, bool modified);
  void sendHiddenSegments();
  void sendHiddenMarkers();
  void sendTrackedMarkers();
  void sendTrackedGRFPaths();
  QTreeWidgetItem* createSegmentItem(const QString& label, int id, bool checked = true);
  QTreeWidgetItem* createMarkerItem(const QString& label, int id, bool checked = true);
  QTreeWidgetItem* createAnalogItem(const QString& label, int id);
  QTreeWidgetItem* createModelOutputItem(const QString& label, int id);
  QTreeWidgetItem* createVideoItem(const QString& label, int id, bool error);
  QPixmap createSegmentIcon(const QColor& c) const;
  QPixmap createMarkerIcon(const QColor& c, bool circled = false, bool enabled = true) const;
  void editMarkersColor(const QColor& color);
  void pushRecentColor(const QColor& color);
  void drawRecentColors();
  void refresh();
  QTreeWidgetItem* treePointChild(QTreeWidgetItem* parent, int id);
  bool hasChildVisible(QTreeWidgetItem* parent) const;
  bool isOkToModifyAnalog();
  
  int m_CurrentConfigurationIndex;
  QList<ConfigurationItem> m_ConfigurationItems;
  Acquisition* mp_Acquisition;
  Model* mp_Model;
  QVector<int> m_DisplayedIds;
  // Icon
  QIcon* mp_DownArrowIcon;
  QIcon* mp_RightArrowIcon;
  QIcon* mp_SegmentsIcon;
  QIcon* mp_MarkersIcon;
  QIcon* mp_VirtualMarkersIcon;
  QIcon* mp_AnalogsIcon;
  QIcon* mp_ModelOutputsIcon;
  QIcon* mp_ForcePlatesIcon;
  QIcon* mp_VideosIcon;
  QIcon* mp_VideoErrorIcon;
  // Model configuration actions
  QAction* mp_NewConfiguration;
  QAction* mp_LoadConfiguration;
  QAction* mp_SaveConfiguration;
  QAction* mp_RemoveConfiguration;
  QAction* mp_DeselectConfiguration;
  QAction* mp_ClearConfigurations;
  // Segments actions
  QAction* mp_NewSegment;
  QAction* mp_HideSelectedSegments;
  QAction* mp_UnhideSelectedSegments;
  // Points & Analogs actions
  QAction* mp_SelectAllMarkers;
  QAction* mp_HideSelectedMarkers;
  QAction* mp_UnhideSelectedMarkers;
  QAction* mp_TrackSelectedMarkers;
  QAction* mp_UntrackSelectedMarkers;
  QAction* mp_DeleteSelectedMarkers;
  QAction* mp_SelectAllAnalogs;
  QAction* mp_SelectAllModelOutputs;
  QAction* mp_SelectAllAngles;
  QAction* mp_SelectAllForces;
  QAction* mp_SelectAllMoments;
  QAction* mp_SelectAllPowers;
  QAction* mp_SelectAllScalars;
  QAction* mp_RemoveSelectedItems;
  // Marker color actions
  QAction* mp_ResetMarkerColorHistory;
  QAction* mp_SetMarkerColorInHistory1;
  QAction* mp_SetMarkerColorInHistory2;
  QAction* mp_SetMarkerColorInHistory3;
  QAction* mp_SetMarkerColorInHistory4;
  QAction* mp_SetMarkerColorInHistory5;
  // Recent marker's colors
  QVector<QColor> m_RecentColors;
};

#endif // ModelDockWidget_h

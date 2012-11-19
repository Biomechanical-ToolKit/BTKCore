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

#ifndef Acquisition_h
#define Acquisition_h

#include <btkProcessObject.h>
#include <btkAcquisition.h>
#include <btkSeparateKnownVirtualMarkersFilter.h>
#include <btkForcePlatformsExtractor.h>
#include <btkGroundReactionWrenchFilter.h>
#include <btkDownsampleFilter.h>
#include <btkPointCollection.h>
#include <btkForcePlatformCollection.h>
#include <btkWrenchCollection.h>
#include <btkAcquisitionFileIO.h>
#include <btkAMTIForcePlatformFileIO.h> // Special case for AMTI files
#include <btkWrenchDirectionAngleFilter.h>

#include <QObject>
#include <QString>
#include <QVector>
#include <QMap>
#include <QColor>

struct Point
{
  // TODO: static Point* fromBtkPoint(btk::Point::Pointer point)
  typedef enum {Marker, VirtualMarker, VirtualMarkerForFrame, Angle, Force, Moment, Power, Scalar} Type;
  QString label;
  QString description;
  bool visible;
  bool trajectoryVisible;
  double radius;
  QColor color;
  Type type;
  int btkidx;
};

struct Analog
{
  static Analog* fromBtkAnalog(btk::Analog::Pointer analog);
  typedef enum {Unknown = 0, PlusMinus10 = 1, PlusMinus5 = 2, PlusMinus2Dot5 = 3, PlusMinus1Dot25 = 4, 
                PlusMinus1 = 5, PlusMinus0Dot5 = 6, PlusMinus0Dot25 = 7, PlusMinus0Dot1 = 8, PlusMinus0Dot05 = 9} Gain;
  QString label;
  QString description;
  QString unit;
  Gain gain;
  int offset;
  double scale;
  int btkidx;
};

struct Event
{
  // TODO: static Event* fromBtkEvent(btk::Event::Pointer event)
  QString label;
  QString description;
  QString context;
  QString subject;
  double time;
  int frame;
  int iconId;
};

struct Video
{
  QString label;
  QString filename;
  QString path;
  qint64 delay;
  bool error;
};

class Acquisition : public QObject
{
  Q_OBJECT
  
public:
  typedef enum {xScreen, yScreen} Property;

  Acquisition(QObject* parent = 0);
  ~Acquisition();
  
  void supportedReadFileFormats(QStringList& formats);
  void supportedWrittenFileFormats(QStringList& formats);
  bool load(const QString& filename);
  bool save(const QString& filename, const QMap<int, QVariant>& properties);
  bool canBeSaved(const QString& filename);
  bool exportTo(const QString& filename, const QMap<int, QVariant>& properties, int lb, int rb);
  bool importFrom(const QStringList& filenames, bool allFramesKept = true);
  bool importFromAMTI(const QString& filename, bool allFramesKept, const QList<QVariant>& dimensions, bool fromOpenAction = false);
  bool importFromAMTI(const QString& filename, bool allFramesKept, const QList<QVariant>& corners, const QList<QVariant>& origin, bool fromOpenAction = false);
  bool importFromVideos(const QStringList& paths, bool allFramesKept, int ff, double freq, double duration);
  void clear();
  
  const QString& fileName() const {return this->m_Filename;};
  void setFileName(const QString& filename) {this->m_Filename = filename;};
  btk::Acquisition::Pointer btkAcquisition() const {return this->mp_BTKAcquisition;};
  btk::PointCollection::Pointer btkAllMarkers() const {return static_pointer_cast<btk::SeparateKnownVirtualMarkersFilter>(this->m_BTKProcesses[BTK_SORTED_POINTS])->GetOutput(4);};
  btk::PointCollection::Pointer btkMarkers() const {return static_pointer_cast<btk::SeparateKnownVirtualMarkersFilter>(this->m_BTKProcesses[BTK_SORTED_POINTS])->GetOutput(0);};
  btk::PointCollection::Pointer btkVirtualMarkers() const {return static_pointer_cast<btk::SeparateKnownVirtualMarkersFilter>(this->m_BTKProcesses[BTK_SORTED_POINTS])->GetOutput(2);};
  btk::PointCollection::Pointer btkVirtualMarkersForFrame() const {return static_pointer_cast<btk::SeparateKnownVirtualMarkersFilter>(this->m_BTKProcesses[BTK_SORTED_POINTS])->GetOutput(1);};
  btk::PointCollection::Pointer btkOtherPoints() const {return static_pointer_cast<btk::SeparateKnownVirtualMarkersFilter>(this->m_BTKProcesses[BTK_SORTED_POINTS])->GetOutput(3);};
  btk::ForcePlatformCollection::Pointer btkForcePlatforms() const {return static_pointer_cast<btk::ForcePlatformsExtractor>(this->m_BTKProcesses[BTK_FORCE_PLATFORMS])->GetOutput();};
  btk::WrenchCollection::Pointer btkGroundReactionWrenches() const {return static_pointer_cast< btk::DownsampleFilter<btk::WrenchCollection> >(this->m_BTKProcesses[BTK_GRWS_DOWNSAMPLED])->GetOutput();};
  btk::PointCollection::Pointer btkWrenchDirectionAngles() const {return static_pointer_cast<btk::WrenchDirectionAngleFilter>(this->m_BTKProcesses[BTK_DIRECTION_ANGLES])->GetOutput();};
  
  int firstFrame() const {return this->m_FirstFrame;};
  void setFirstFrame(int ff);
  int lastFrame() const {return this->m_LastFrame;};
  void regionOfInterest(int& lb, int& rb) const {lb = this->mp_ROI[0]; rb = this->mp_ROI[1];};
  void setRegionOfInterest(int lb, int rb);
  
  int pointFrameNumber() const {return this->mp_BTKAcquisition->GetPointFrameNumber();}
  double pointFrequency() const {return this->mp_BTKAcquisition->GetPointFrequency();};
  QString pointUnit(Point::Type t) const;
  bool hasPoints() const {return !this->m_Points.empty();};
  int pointCount() const {return this->m_Points.count();};
  const QMap<int, Point*>& points() const {return this->m_Points;};
  const QString& pointLabel(int id) const {return this->m_Points[id]->label;};
  void setPointLabel(int id, const QString& label);
  const QString& pointDescription(int id) const {return this->m_Points[id]->description;};
  void setPointsDescription(const QVector<int>& ids, const QVector<QString>& descs);
  Point::Type pointType(int id) const {return this->m_Points[id]->type;};
  void setPointType(int id, Point::Type p);
  int findMarkerIdFromLabel(const QString& label) const;
  double markerRadius(int id) const {return this->m_Points[id]->radius;};
  void setMarkersRadius(const QVector<int>& ids, const QVector<double>& radii);
  const QColor& markerColor(int id) const {return this->m_Points[id]->color;};
  void setMarkersColor(const QVector<int>& ids, const QVector<QColor>& colors);
  QList<Point*> takePoints(const QList<int>& ids);
  void insertPoints(const QList<int>& ids, const QList<Point*> points);
  int findPointIdFromLabel(const QString& label) const;
  const QColor& defaultMarkerColor() const {return this->m_DefaultMarkerColor;};
  void setDefaultMarkerColor(const QColor& color) {this->m_DefaultMarkerColor = color;};
  void setDefaultMarkerRadius(double r) {this->m_DefaultMarkerRadius = r;};
  bool markerVisible(int id) const {return this->m_Points[id]->visible;};
  void setMarkersVisible(const QVector<int>& ids, const QVector<bool>& visibles);
  bool markerTrajectoryVisible(int id) const {return this->m_Points[id]->trajectoryVisible;};
  void setMarkersTrajectoryVisible(const QVector<int>& ids, const QVector<bool>& visibles);
  void resetMarkersConfiguration(const QList<int>& ids, const QList<bool>& visibles, const QList<bool>& trajectories, const QList<double>& radii, const QList<QColor>& colors);
  int createAveragedMarker(const QList<int>& markerIds);
  int generateNewPointId();
  
  int analogFrameNumber() const {return this->mp_BTKAcquisition->GetAnalogFrameNumber();}
  int analogSamplePerPointFrame() const {return this->mp_BTKAcquisition->GetNumberAnalogSamplePerFrame();};
  double analogFrequency() const {return this->mp_BTKAcquisition->GetAnalogFrequency();};
  bool hasAnalogs() const {return !this->m_Analogs.empty();};
  int analogCount() const {return this->m_Analogs.count();};
  const QMap<int, Analog*>& analogs() const {return this->m_Analogs;};
  const QString& analogLabel(int id) const {return this->m_Analogs[id]->label;};
  void setAnalogLabel(int id, const QString& label);
  const QString& analogDescription(int id) const {return this->m_Analogs[id]->description;};
  void setAnalogsDescription(const QVector<int>& ids, const QVector<QString>& descs);
  const QString& analogUnit(int id) const {return this->m_Analogs[id]->unit;};
  void setAnalogsUnit(const QVector<int>& ids, const QVector<QString>& units);
  Analog::Gain analogGain(int id) const {return this->m_Analogs[id]->gain;};
  void setAnalogsGain(const QVector<int>& ids, const QVector<Analog::Gain>& gains);
  int analogOffset(int id) const {return this->m_Analogs[id]->offset;};
  void setAnalogsOffset(const QVector<int>& ids, const QVector<int>& offsets);
  double analogScale(int id) const {return this->m_Analogs[id]->scale;};
  void setAnalogsScale(const QVector<int>& ids, const QVector<double>& scales);
  QList<Analog*> takeAnalogs(const QList<int>& ids);
  void insertAnalogs(const QList<int>& ids, const QList<Analog*> analogs);
  void emitAnalogsValuesChanged(const QVector<int>& ids);
  int generateNewAnalogId();
  
  bool hasEvents() const {return !this->m_Events.empty();};
  int eventCount() const {return this->m_Events.count();};
  const QMap<int, Event*>& events() const {return this->m_Events;};
  const Event* eventAt(int id) const;
  const QString& eventLabel(int id) const {return this->m_Events[id]->label;};
  const QString& eventDescription(int id) const {return this->m_Events[id]->description;};
  const QString& eventContext(int id) const {return this->m_Events[id]->context;};
  const QString& eventSubject(int id) const {return this->m_Events[id]->subject;};
  int eventFrame(int id) const {return this->m_Events[id]->frame;};
  void setEventFrame(int id, int frame);
  double eventTime(int id) const {return this->m_Events[id]->time;};
  int eventIconId(int id) const {return this->m_Events[id]->iconId;};
  void setEvents(const QList<int>& ids, const QList<Event*> events);
  QList<Event*> takeEvents(const QList<int>& ids);
  void insertEvents(const QList<int>& ids, const QList<Event*> events);
  QList<int> eventIds() const {return this->m_Events.keys();};
  int generateNewEventId();
  
  bool hasVideos() const {return !this->m_Videos.empty();};
  int videosCount() const {return this->m_Videos.count();};
  const QMap<int, Video*>& videos() const {return this->m_Videos;};
  const QString& videoLabel(int id) const {return this->m_Videos[id]->label;};
  const QString& videoFilename(int id) const {return this->m_Videos[id]->filename;};
  const QString& videoPath(int id) const {return this->m_Videos[id]->path;};
  qint64 videoDelay(int id) const {return this->m_Videos[id]->delay;};
  void setVideoDelay(const QVector<int>& ids, const QVector<qint64>& delays);
  bool videoError(int id) const {return this->m_Videos[id]->error;};
  QList<Video*> takeVideos(const QList<int>& ids);
  void insertVideos(const QList<int>& ids, const QList<Video*> videos);
  void importVideos(const QStringList& paths);
  
  double timeFromFrame(int frame);
  
signals:
  void informationsChanged(const QVector<QString>& infos);
  void firstFrameChanged(int ff);
  void regionOfInterestChanged(int lb, int rb);
  void pointLabelChanged(int id, const QString& label);
  void pointsDescriptionChanged(const QVector<int>& ids, const QVector<QString>& descs);
  void pointTypeChanged(int id, Point::Type p);
  void markersRadiusChanged(const QVector<int>& ids, const QVector<double>& radii);
  void markersColorChanged(const QVector<int>& ids, const QVector<QColor>& colors);
  void markersVisibilityChanged(const QVector<int>& ids, const QVector<bool>& visibles);
  void markersTrajectoryVisibilityChanged(const QVector<int>& ids, const QVector<bool>& visibles);
  void markersConfigurationReset(const QList<int>& ids, const QList<bool>& visibles, const QList<bool>& trajectories, const QList<double>& radii, const QList<QColor>& colors);
  void pointsRemoved(const QList<int>& ids, const QList<Point*>& points);
  void pointsInserted(const QList<int>& ids, const QList<Point*>& points);
  void analogLabelChanged(int id, const QString& label);
  void analogsDescriptionChanged(const QVector<int>& ids, const QVector<QString>& descs);
  void analogsUnitChanged(const QVector<int>& ids, const QVector<QString>& units);
  void analogsGainChanged(const QVector<int>& ids, const QVector<Analog::Gain>& gains);
  void analogsOffsetChanged(const QVector<int>& ids, const QVector<int>& offsets);
  void analogsScaleChanged(const QVector<int>& ids, const QVector<double>& scales);
  void analogsRemoved(const QList<int>& ids, const QList<Analog*>& analogs);
  void analogsInserted(const QList<int>& ids, const QList<Analog*>& analogs);
  void analogsValuesChanged(const QVector<int>& ids);
  void eventFrameChanged(int id, int frame);
  void eventsModified(const QList<int>& ids, const QList<Event*>& events);
  void eventsRemoved(const QList<int>& ids, const QList<Event*>& events);
  void eventsInserted(const QList<int>& ids, const QList<Event*>& events);
  void videosRemoved(const QList<int>& ids, const QList<Video*>& videos);
  void videosInserted(const QList<int>& ids, const QList<Video*>& videos);
  void videosImported(const QList<int>& ids, const QList<Video*>& videos);
  void videosDelayChanged(const QVector<int>& ids, const QVector<qint64>& delays);
  
private:
  void emitGeneratedInformations(btk::AcquisitionFileIO::Pointer io);
  bool write(const QString& filename, const QMap<int, QVariant>& properties, int lb, int rb, bool updateInfo = false);
  void loadAcquisition();
  void extractVideos(const std::vector<std::string>& filename, std::vector<double>& delays, bool completeFilename = true);
  bool importFrom(const QList<btk::Acquisition::Pointer>& readers, bool allFramesKept);
  bool importFromAMTI(const QString& filename, bool allFramesKept, btk::AMTIForcePlatformFileIO::Pointer io, bool fromOpenAction);
  
  enum {BTK_SORTED_POINTS, BTK_FORCE_PLATFORMS, BTK_GRWS, BTK_GRWS_DOWNSAMPLED, BTK_DIRECTION_ANGLES};
  
  btk::Acquisition::Pointer mp_BTKAcquisition;
  QMap<int, btk::ProcessObject::Pointer> m_BTKProcesses;
  QString m_Filename;
  int m_FirstFrame;
  int m_LastFrame;
  int mp_ROI[2];
  QMap<int,Point*> m_Points;
  int m_LastPointId;
  QMap<int,Analog*> m_Analogs;
  int m_LastAnalogId;
  QMap<int,Event*> m_Events;
  int m_LastEventId;
  QMap<int,Video*> m_Videos;
  int m_LastVideoId;
  double m_DefaultMarkerRadius;
  QColor m_DefaultMarkerColor;
};

 // frame starts at 1 not 0.
inline double Acquisition::timeFromFrame(int frame)
{
  return static_cast<double>(frame - 1) / this->pointFrequency();
}

#endif // Acquisition_h
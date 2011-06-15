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

#include <QObject>
#include <QString>
#include <QVector>
#include <QMap>
#include <QColor>

struct Point
{
  typedef enum {Marker, VirtualMarker, VirtualMarkerForFrame, Angle, Force, Moment, Power, Scalar} Type;
  QString label;
  QString description;
  double radius;
  QColor color;
  Type type;
  int btkidx;
};

struct Analog
{
  typedef enum {Unknown = 0, PlusMinus10 = 1, PlusMinus5 = 2, PlusMinus2Dot5 = 3, PlusMinus1Dot25 = 4, PlusMinus1 = 5} Gain;
  QString label;
  QString description;
  QString unit;
  Gain gain;
  int offset;
  double scale;
};

struct Event
{
  QString label;
  QString description;
  QString context;
  QString subject;
  double time;
  int frame;
  int iconId;
};

class Acquisition : public QObject
{
  Q_OBJECT
  
public:
  typedef enum {xScreen, yScreen} Property;

  Acquisition(QObject* parent = 0);
  ~Acquisition();
  
  QString load(const QString& filename);
  QString save(const QString& filename, const QMap<int, QVariant>& properties);
  QString exportTo(const QString& filename, const QMap<int, QVariant>& properties, int lb, int rb);
  QString importFrom(const QStringList& filenames, QString& importWarnings);
  void clear();
  
  const QString& fileName() const {return this->m_Filename;};
  btk::Acquisition::Pointer btkAcquisition() const {return this->mp_BTKAcquisition;};
  btk::PointCollection::Pointer btkMarkers() const {return static_pointer_cast<btk::SeparateKnownVirtualMarkersFilter>(this->m_BTKProcesses[BTK_SORTED_POINTS])->GetOutput(0);};
  btk::PointCollection::Pointer btkVirtualMarkers() const {return static_pointer_cast<btk::SeparateKnownVirtualMarkersFilter>(this->m_BTKProcesses[BTK_SORTED_POINTS])->GetOutput(2);};
  btk::PointCollection::Pointer btkOtherPoints() const {return static_pointer_cast<btk::SeparateKnownVirtualMarkersFilter>(this->m_BTKProcesses[BTK_SORTED_POINTS])->GetOutput(3);};
  btk::ForcePlatformCollection::Pointer btkForcePlatforms() const {return static_pointer_cast<btk::ForcePlatformsExtractor>(this->m_BTKProcesses[BTK_FORCE_PLATFORMS])->GetOutput();};
  btk::WrenchCollection::Pointer btkGroundReactionWrenches() const {return static_pointer_cast< btk::DownsampleFilter<btk::WrenchCollection> >(this->m_BTKProcesses[BTK_GRWS_DOWNSAMPLED])->GetOutput();};
  
  int firstFrame() const {return this->m_FirstFrame;};
  int lastFrame() const {return this->m_LastFrame;};
  void regionOfInterest(int& lb, int& rb) const {lb = this->mp_ROI[0]; rb = this->mp_ROI[1];};
  void setRegionOfInterest(int lb, int rb);
  
  double pointFrequency() const {return this->mp_BTKAcquisition->GetPointFrequency();};
  QString pointUnit(Point::Type t) const;
  bool hasPoints() const {return !this->m_Points.empty();};
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
  void resetMarkersColor(const QVector<int>& ids, const QVector<QColor>& colors);
  const QColor& markerColor(int id) const {return this->m_Points[id]->color;};
  void setMarkerColor(int id, const QColor& color);
  void setMarkersColor(const QVector<int>& ids, const QVector<QColor>& colors);
  void resetMarkersRadius(const QVector<int>& ids, const QVector<double>& radii);
  QList<Point*> takePoints(const QList<int>& ids);
  void insertPoints(const QList<int>& ids, const QList<Point*> points);
  int findPointIdFromLabel(const QString& label) const;
  const QColor& defaultMarkerColor() const {return this->m_DefaultMarkerColor;};
  void setDefaultMarkerColor(const QColor& color) {this->m_DefaultMarkerColor = color;};
  void setDefaultMarkerRadius(double r) {this->m_DefaultMarkerRadius = r;};
  
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
  int generateNewEventId();
  
  double timeFromFrame(int frame);
  
signals:
  void informationsChanged(const QVector<QString>& infos);
  void regionOfInterestChanged(int lb, int rb);
  void pointLabelChanged(int id, const QString& label);
  void pointsDescriptionChanged(const QVector<int>& ids, const QVector<QString>& descs);
  void pointTypeChanged(int id, Point::Type p);
  void markersRadiusChanged(const QVector<int>& ids, const QVector<double>& radii);
  void markerColorChanged(int id, const QColor& color);
  void markersColorChanged(const QVector<int>& ids, const QVector<QColor>& colors);
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
  void eventFrameChanged(int id, int frame);
  void eventsModified(const QList<int>& ids, const QList<Event*>& events);
  void eventsRemoved(const QList<int>& ids, const QList<Event*>& events);
  void eventsInserted(const QList<int>& ids, const QList<Event*>& events);
  
private:
  void emitGeneratedInformations(btk::AcquisitionFileIO::Pointer io);
  void write(const QString& filename, const QMap<int, QVariant>& properties, int lb, int rb, QString& errMsg, bool updateInfo = false);
  void loadAcquisition();
  
  enum {BTK_SORTED_POINTS, BTK_FORCE_PLATFORMS, BTK_GRWS, BTK_GRWS_DOWNSAMPLED};
  
  btk::Acquisition::Pointer mp_BTKAcquisition;
  QMap<int, btk::ProcessObject::Pointer> m_BTKProcesses;
  QString m_Filename;
  int m_FirstFrame;
  int m_LastFrame;
  int mp_ROI[2];
  QMap<int,Point*> m_Points;
  QMap<int,Analog*> m_Analogs;
  QMap<int,Event*> m_Events;
  int m_LastEventId;
  double m_DefaultMarkerRadius;
  QColor m_DefaultMarkerColor;
};

 // frame starts at 1 not 0.
inline double Acquisition::timeFromFrame(int frame)
{
  return static_cast<double>(frame - 1) / this->pointFrequency();
}

#endif // Acquisition_h
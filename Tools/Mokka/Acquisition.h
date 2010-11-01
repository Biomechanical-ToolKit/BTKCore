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

#ifndef Acquisition_h
#define Acquisition_h

#include <btkAcquisition.h>

#include <QObject>
#include <QString>
#include <QVector>
#include <QMap>
#include <QColor>

typedef enum {Marker, Angle, Force, Moment, Power, Scalar} PointType;
typedef enum {Unknown = 0, PlusMinus10 = 1, PlusMinus5 = 2, PlusMinus2Dot5 = 3, PlusMinus1Dot25 = 4, PlusMinus1 = 5} AnalogGain;

struct Point
{
  QString label;
  QString description;
  double radius;
  QColor color;
  PointType type;
};

struct Analog
{
  QString label;
  QString description;
  QString unit;
  AnalogGain gain;
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
  Acquisition(QObject* parent = 0);
  ~Acquisition();
  
  void init(const QString& filename, btk::Acquisition::Pointer acquisition);
  void clear();
  const QString& fileName() const {return this->m_Filename;};
  
  int firstFrame() const {return this->m_FirstFrame;};
  int lastFrame() const {return this->m_LastFrame;};
  double pointFrequency() const {return this->m_PointFrequency;};
  
  void regionOfInterest(int& lb, int& rb) const {lb = this->mp_ROI[0]; rb = this->mp_ROI[1];};
  void setRegionOfInterest(int lb, int rb);
  
  bool hasPoints() const {return !this->m_Points.empty();};
  const QString& pointLabel(int id) const {return this->m_Points[id]->label;};
  void setPointLabel(int id, const QString& label);
  const QString& pointDescription(int id) const {return this->m_Points[id]->description;};
  void setPointsDescription(const QVector<int>& ids, const QVector<QString>& descs);
  PointType pointType(int id) const {return this->m_Points[id]->type;};
  void setPointType(int id, PointType p);
  int findMarkers(const QString& name) const;
  double markerRadius(int id) const {return this->m_Points[id]->radius;};
  void setMarkersRadius(const QVector<int>& ids, const QVector<double>& radii);
  const QColor& markerColor(int id) const {return this->m_Points[id]->color;};
  void setMarkerColor(int id, const QColor& color);
  void setMarkersColor(const QVector<int>& ids, const QVector<QColor>& colors);
  QList<Point*> takePoints(const QList<int>& ids);
  void insertPoints(const QList<int>& ids, const QList<Point*> points);
  int findPointIdFromLabel(const QString& label) const;
  
  bool hasAnalogs() const {return !this->m_Analogs.empty();};
  const QString& analogLabel(int id) const {return this->m_Analogs[id]->label;};
  void setAnalogLabel(int id, const QString& label);
  const QString& analogDescription(int id) const {return this->m_Analogs[id]->description;};
  void setAnalogsDescription(const QVector<int>& ids, const QVector<QString>& descs);
  const QString& analogUnit(int id) const {return this->m_Analogs[id]->unit;};
  void setAnalogsUnit(const QVector<int>& ids, const QVector<QString>& units);
  AnalogGain analogGain(int id) const {return this->m_Analogs[id]->gain;};
  void setAnalogsGain(const QVector<int>& ids, const QVector<AnalogGain>& gains);
  int analogOffset(int id) const {return this->m_Analogs[id]->offset;};
  void setAnalogsOffset(const QVector<int>& ids, const QVector<int>& offsets);
  double analogScale(int id) const {return this->m_Analogs[id]->scale;};
  void setAnalogsScale(const QVector<int>& ids, const QVector<double>& scales);
  QList<Analog*> takeAnalogs(const QList<int>& ids);
  void insertAnalogs(const QList<int>& ids, const QList<Analog*> analogs);
  
  bool hasEvents() const {return !this->m_Events.empty();};
  int eventCount() const {return this->m_Events.count();};
  const Event* eventAt(int id) const;
  const QString& eventLabel(int id) const {return this->m_Events[id]->label;};
  const QString& eventDescription(int id) const {return this->m_Events[id]->description;};
  const QString& eventContext(int id) const {return this->m_Events[id]->context;};
  const QString& eventSubject(int id) const {return this->m_Events[id]->subject;};
  int eventFrame(int id) const {return this->m_Events[id]->frame;};
  double eventTime(int id) const {return this->m_Events[id]->time;};
  int eventIconId(int id) const {return this->m_Events[id]->iconId;};
  QList<Event*> takeEvents(const QList<int>& ids);
  void insertEvents(const QList<int>& ids, const QList<Event*> events);
  int generateNewEventId();
  
signals:
  void regionOfInterestChanged(int lb, int rb);
  void pointLabelChanged(int id, const QString& label);
  void pointsDescriptionChanged(const QVector<int>& ids, const QVector<QString>& descs);
  void pointTypeChanged(int id, PointType p);
  void markersRadiusChanged(const QVector<int>& ids, const QVector<double>& radii);
  void markerColorChanged(int id, const QColor& color);
  void markersColorChanged(const QVector<int>& ids, const QVector<QColor>& colors);
  void pointsRemoved(const QList<int>& ids, const QList<Point*>& points);
  void pointsInserted(const QList<int>& ids, const QList<Point*>& points);
  void analogLabelChanged(int id, const QString& label);
  void analogsDescriptionChanged(const QVector<int>& ids, const QVector<QString>& descs);
  void analogsUnitChanged(const QVector<int>& ids, const QVector<QString>& units);
  void analogsGainChanged(const QVector<int>& ids, const QVector<AnalogGain>& gains);
  void analogsOffsetChanged(const QVector<int>& ids, const QVector<int>& offsets);
  void analogsScaleChanged(const QVector<int>& ids, const QVector<double>& scales);
  void analogsRemoved(const QList<int>& ids, const QList<Analog*>& analogs);
  void analogsInserted(const QList<int>& ids, const QList<Analog*>& analogs);
  void eventsRemoved(const QList<int>& ids, const QList<Event*>& events);
  void eventsInserted(const QList<int>& ids, const QList<Event*>& events);
  
private:
  QString m_Filename;
  btk::Acquisition::Pointer mp_Acquisition;
  int m_FirstFrame;
  int m_LastFrame;
  int mp_ROI[2];
  double m_PointFrequency;
  QMap<int,Point*> m_Points;
  QMap<int,Analog*> m_Analogs;
  QMap<int,Event*> m_Events;
  int m_LastEventId;
};

#endif // Acquisition_h
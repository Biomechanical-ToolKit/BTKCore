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

#include "Acquisition.h"
#include "Utilities/StringStreamBuf.h"

#include <btkAcquisitionFileReader.h>
#include <btkAcquisitionFileWriter.h>
#include <btkMetaDataUtils.h>
#include <btkMergeAcquisitionFilter.h>

#include <Qt>
#include <QFileInfo>
#include <QDateTime>
#include <QVariant>

Acquisition::Acquisition(QObject* parent)
: QObject(parent), mp_BTKAcquisition(), m_BTKProcesses(), m_Filename(),
  m_Points(), m_Analogs(), m_Events(), m_DefaultMarkerColor(Qt::white)
{
  this->m_FirstFrame = 0;
  this->m_LastFrame = 0;
  this->mp_ROI[0] = this->m_FirstFrame;
  this->mp_ROI[1] = this->m_LastFrame;
  this->m_LastEventId = -1;
  this->m_DefaultMarkerRadius = 8.0; // mm
  
  // BTK PIPELINE
  btk::SeparateKnownVirtualMarkersFilter::Pointer virtualMarkersSeparator = btk::SeparateKnownVirtualMarkersFilter::New();
  btk::ForcePlatformsExtractor::Pointer forcePlatformsExtractor = btk::ForcePlatformsExtractor::New();
  btk::GroundReactionWrenchFilter::Pointer GRWsFilter = btk::GroundReactionWrenchFilter::New();
  GRWsFilter->SetThresholdValue(5.0); // PWA are not computed from vertical forces lower than 5 newtons.
  GRWsFilter->SetThresholdState(true);
  GRWsFilter->SetInput(forcePlatformsExtractor->GetOutput());
  btk::DownsampleFilter<btk::WrenchCollection>::Pointer GRWsDownsampler = btk::DownsampleFilter<btk::WrenchCollection>::New();
  GRWsDownsampler->SetInput(GRWsFilter->GetOutput());
  this->m_BTKProcesses[BTK_SORTED_POINTS] = virtualMarkersSeparator;
  this->m_BTKProcesses[BTK_FORCE_PLATFORMS] = forcePlatformsExtractor;
  this->m_BTKProcesses[BTK_GRWS] = GRWsFilter;
  this->m_BTKProcesses[BTK_GRWS_DOWNSAMPLED] = GRWsDownsampler;
};

Acquisition::~Acquisition()
{
  this->clear();
};

QString Acquisition::load(const QString& filename)
{
  btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
  reader->SetFilename(filename.toStdString());
  try
  {
    reader->Update();
  }
  catch (btk::Exception& e)
  {
    return e.what();
  }
  catch (std::exception& e)
  {
    return "Unexpected error: " + QString(e.what());
  }
  catch (...)
  {
    return "Unknown error.";
  }
  this->clear();
  this->mp_BTKAcquisition = reader->GetOutput();
  this->loadAcquisition();
  this->m_Filename = filename;
  this->emitGeneratedInformations(reader->GetAcquisitionIO());
  return "";
};

QString Acquisition::save(const QString& filename, const QMap<int, QVariant>& properties)
{
  QString errMsg;
  this->write(filename, properties, this->mp_ROI[0], this->mp_ROI[1], errMsg, true);
  return errMsg;
}

QString Acquisition::exportTo(const QString& filename, const QMap<int, QVariant>& properties, int lb, int rb)
{
  QString errMsg;
  this->write(filename, properties, lb, rb, errMsg);
  return errMsg;
};

QString Acquisition::importFrom(const QStringList& filenames, QString& importWarnings)
{
  // Try to read the given file
  QList<btk::AcquisitionFileReader::Pointer> readers;
  try
  {
    for (int i = 0 ; i < filenames.count() ; ++i)
    {
      btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
      reader->SetFilename(filenames[i].toStdString());
      reader->Update();
      readers << reader;
    }
  }
  catch (btk::Exception& e)
  {
    return e.what();
  }
  catch (std::exception& e)
  {
    return "Unexpected error: " + QString(e.what());
  }
  catch (...)
  {
    return "Unknown error.";
  }
  // Check if the original acquisition need to be cropped
  if (this->mp_BTKAcquisition)
  {
    if ((this->mp_ROI[0] != this->m_FirstFrame) || (this->mp_ROI[1] != this->m_LastFrame))
    {
      // Resize the data.
      this->mp_BTKAcquisition->ResizeFrameNumber(this->mp_BTKAcquisition->GetPointFrameNumber() - (this->m_LastFrame - this->mp_ROI[1]));
      this->mp_BTKAcquisition->ResizeFrameNumberFromEnd(this->mp_BTKAcquisition->GetPointFrameNumber() - (this->m_FirstFrame - this->mp_ROI[0]));
      // Remove the events outside of the bounds
      btk::EventCollection::Iterator it = this->mp_BTKAcquisition->BeginEvent();
      while (it != this->mp_BTKAcquisition->EndEvent())
      {
        if (((*it)->GetFrame() < this->mp_ROI[0]) || ((*it)->GetFrame() > this->mp_ROI[1]))
          it = this->mp_BTKAcquisition->RemoveEvent(it);
        else
          ++it;
      }
    }
  }
  // Launch the merging/concatenation
  int shift = !this->mp_BTKAcquisition ? 0 : 1;
  StringStreamBuf err(std::cerr);
  btk::MergeAcquisitionFilter::Pointer merger = btk::MergeAcquisitionFilter::New();
  merger->SetInput(0, this->mp_BTKAcquisition);
  for (int i = 0 ; i < readers.count() ; ++i)
    merger->SetInput(i+shift, readers[i]->GetOutput());  
  merger->Update();
  this->clear();
  this->mp_BTKAcquisition = merger->GetOutput();
  this->loadAcquisition();
  QVector<QString> infos(16,"N/A"); infos[0] = "N/A                         ";
  emit informationsChanged(infos);
  importWarnings = QString::fromStdString(err.str());
  return "";
};

void Acquisition::clear()
{
  for (QMap<int,Point*>::iterator it = this->m_Points.begin() ; it != this->m_Points.end() ; ++it)
    delete *it;
  this->m_Points.clear();
  for (QMap<int,Analog*>::iterator it = this->m_Analogs.begin() ; it != this->m_Analogs.end() ; ++it)
    delete *it;
  this->m_Analogs.clear();
  for (QMap<int,Event*>::iterator it = this->m_Events.begin() ; it != this->m_Events.end() ; ++it)
    delete *it;
  this->m_Events.clear();
  this->m_LastEventId = -1;
  this->mp_BTKAcquisition = btk::Acquisition::Pointer(); // NULL
}

void Acquisition::setRegionOfInterest(int lb, int rb)
{
  this->mp_ROI[0] = lb;
  this->mp_ROI[1] = rb;
  emit regionOfInterestChanged(lb, rb);
};

QString Acquisition::pointUnit(Point::Type t) const
{
  switch(t)
  {
  case Point::Marker:
  case Point::VirtualMarker:
  case Point::VirtualMarkerForFrame:
    return QString::fromStdString(this->mp_BTKAcquisition->GetPointUnit(btk::Point::Marker));
    break;
  case Point::Angle:
    return QString::fromStdString(this->mp_BTKAcquisition->GetPointUnit(btk::Point::Angle));
    break;
  case Point::Force:
    return QString::fromStdString(this->mp_BTKAcquisition->GetPointUnit(btk::Point::Force));
    break;
  case Point::Moment:
    return QString::fromStdString(this->mp_BTKAcquisition->GetPointUnit(btk::Point::Moment));
    break;
  case Point::Power:
    return QString::fromStdString(this->mp_BTKAcquisition->GetPointUnit(btk::Point::Power));
    break;
  case Point::Scalar:
    return QString::fromStdString(this->mp_BTKAcquisition->GetPointUnit(btk::Point::Scalar));
    break;
  }
  return "";
};

void Acquisition::setPointLabel(int id, const QString& label)
{
  QMap<int,Point*>::iterator it = this->m_Points.find(id);
  if (it != this->m_Points.end())
  {
    (*it)->label = label;
    emit pointLabelChanged(id, label);
  }
};

void Acquisition::setPointsDescription(const QVector<int>& ids, const QVector<QString>& descs)
{
  for (int i = 0 ; i < ids.count() ; ++i)
  {
    QMap<int,Point*>::iterator it = this->m_Points.find(ids[i]);
    if (it != this->m_Points.end())
      (*it)->description = descs[i];
  }
  emit pointsDescriptionChanged(ids, descs);
};

void Acquisition::setPointType(int id, Point::Type p)
{
  QMap<int,Point*>::iterator it = this->m_Points.find(id);
  if (it != this->m_Points.end())
  {
    (*it)->type = p;
    emit pointTypeChanged(id, p);
  }
};

int Acquisition::findMarkerIdFromLabel(const QString& label) const
{
  for (QMap<int,Point*>::const_iterator it = this->m_Points.begin() ; it != this->m_Points.end() ; ++it)
  {
    if ((it.value()->label.compare(label) == 0) && ((it.value()->type == Point::Marker) || (it.value()->type == Point::VirtualMarker)))
      return it.key();
  }
  return -1;
};

void Acquisition::setMarkersRadius(const QVector<int>& ids, const QVector<double>& radii)
{
  for (int i = 0 ; i < ids.count() ; ++i)
  {
    QMap<int,Point*>::iterator it = this->m_Points.find(ids[i]);
    if (it != this->m_Points.end())
      (*it)->radius = radii[i];
  }
  emit markersRadiusChanged(ids, radii);
};

void Acquisition::resetMarkersRadius(const QVector<int>& ids, const QVector<double>& radii)
{
  QList<int> ids_;
  QList<double> radii_;
  for (QMap<int,Point*>::iterator it = this->m_Points.begin() ; it != this->m_Points.end() ; ++it)
  {
    if (((*it)->type != Point::Marker) && ((*it)->type != Point::VirtualMarker))
      continue;
    ids_ << it.key();
    int idx = ids.indexOf(it.key());
    if (idx != -1)
    {
      (*it)->radius = radii[idx];
      radii_ << radii[idx];
    }
    else
    {
      (*it)->radius = this->m_DefaultMarkerRadius;
      radii_ << this->m_DefaultMarkerRadius;
    }
  }
  emit markersRadiusChanged(ids_.toVector(), radii_.toVector());
};

void Acquisition::setMarkerColor(int id, const QColor& color)
{
  QMap<int,Point*>::iterator it = this->m_Points.find(id);
  if (it != this->m_Points.end())
  {
    (*it)->color = color;
    emit markerColorChanged(id, color);
  }
};

void Acquisition::setMarkersColor(const QVector<int>& ids, const QVector<QColor>& colors)
{
  for (int i = 0 ; i < ids.count() ; ++i)
  {
    QMap<int,Point*>::iterator it = this->m_Points.find(ids[i]);
    if (it != this->m_Points.end())
      (*it)->color = colors[i];
  }
  emit markersColorChanged(ids, colors);
};

void Acquisition::resetMarkersColor(const QVector<int>& ids, const QVector<QColor>& colors)
{
  QList<int> ids_;
  QList<QColor> colors_;
  for (QMap<int,Point*>::iterator it = this->m_Points.begin() ; it != this->m_Points.end() ; ++it)
  {
    if (((*it)->type != Point::Marker) && ((*it)->type != Point::VirtualMarker))
      continue;
    ids_ << it.key();
    int idx = ids.indexOf(it.key());
    if (idx != -1)
    {
      (*it)->color = colors[idx];
      colors_ << colors[idx];
    }
    else
    {
      (*it)->color = this->m_DefaultMarkerColor;
      colors_ << this->m_DefaultMarkerColor;
    }
  }
  emit markersColorChanged(ids_.toVector(), colors_.toVector());
};

QList<Point*> Acquisition::takePoints(const QList<int>& ids)
{
  QList<Point*> points;
  for (int i = 0 ; i < ids.count() ; ++i)
  {
    QMap<int,Point*>::iterator it = this->m_Points.find(ids[i]);
    if (it != this->m_Points.end())
    {
      points.push_back(*it);
      this->m_Points.erase(it);
    }
  }
  emit pointsRemoved(ids, points);
  return points;
};

void Acquisition::insertPoints(const QList<int>& ids, const QList<Point*> points)
{
  for (int i = 0 ; i < ids.count() ; ++i)
  {
    QMap<int,Point*>::iterator it = this->m_Points.find(ids[i]);
    if (it == this->m_Points.end())
      this->m_Points.insert(ids[i], points[i]);
    else
      qDebug("A point with the given ID already exists");
  }
  emit pointsInserted(ids, points);
};

int Acquisition::findPointIdFromLabel(const QString& label) const
{
  for (QMap<int,Point*>::const_iterator it = this->m_Points.begin() ; it != this->m_Points.end() ; ++it)
  {
    if ((*it)->label.compare(label) == 0)
      return it.key();
  }
  return -1;
};

void Acquisition::setAnalogLabel(int id, const QString& label)
{
  QMap<int,Analog*>::iterator it = this->m_Analogs.find(id);
  if (it != this->m_Analogs.end())
  {
    (*it)->label = label;
    emit analogLabelChanged(id, label);
  }
};

void Acquisition::setAnalogsDescription(const QVector<int>& ids, const QVector<QString>& descs)
{
  for (int i = 0 ; i < ids.count() ; ++i)
  {
    QMap<int,Analog*>::iterator it = this->m_Analogs.find(ids[i]);
    if (it != this->m_Analogs.end())
      (*it)->description = descs[i];
  }
  emit analogsDescriptionChanged(ids, descs);
};

void Acquisition::setAnalogsUnit(const QVector<int>& ids, const QVector<QString>& units)
{
  for (int i = 0 ; i < ids.count() ; ++i)
  {
    QMap<int,Analog*>::iterator it = this->m_Analogs.find(ids[i]);
    if (it != this->m_Analogs.end())
      (*it)->unit = units[i];
  }
  emit analogsUnitChanged(ids, units);
};

void Acquisition::setAnalogsGain(const QVector<int>& ids, const QVector<Analog::Gain>& gains)
{
  for (int i = 0 ; i < ids.count() ; ++i)
  {
    QMap<int,Analog*>::iterator it = this->m_Analogs.find(ids[i]);
    if (it != this->m_Analogs.end())
      (*it)->gain = gains[i];
  }
  emit analogsGainChanged(ids, gains);
};

void Acquisition::setAnalogsOffset(const QVector<int>& ids, const QVector<int>& offsets)
{
  for (int i = 0 ; i < ids.count() ; ++i)
  {
    QMap<int,Analog*>::iterator it = this->m_Analogs.find(ids[i]);
    if (it != this->m_Analogs.end())
      (*it)->offset = offsets[i];
  }
  emit analogsOffsetChanged(ids, offsets);
};

void Acquisition::setAnalogsScale(const QVector<int>& ids, const QVector<double>& scales)
{
  for (int i = 0 ; i < ids.count() ; ++i)
  {
    QMap<int,Analog*>::iterator it = this->m_Analogs.find(ids[i]);
    if (it != this->m_Analogs.end())
      (*it)->scale = scales[i];
  }
  emit analogsScaleChanged(ids, scales);
};

QList<Analog*> Acquisition::takeAnalogs(const QList<int>& ids)
{
  QList<Analog*> analogs;
  for (int i = 0 ; i < ids.count() ; ++i)
  {
    QMap<int,Analog*>::iterator it = this->m_Analogs.find(ids[i]);
    if (it != this->m_Analogs.end())
    {
      analogs.push_back(*it);
      this->m_Analogs.erase(it);
    }
  }
  emit analogsRemoved(ids, analogs);
  return analogs;
};

void Acquisition::insertAnalogs(const QList<int>& ids, const QList<Analog*> analogs)
{
  for (int i = 0 ; i < ids.count() ; ++i)
  {
    QMap<int,Analog*>::iterator it = this->m_Analogs.find(ids[i]);
    if (it == this->m_Analogs.end())
      this->m_Analogs.insert(ids[i], analogs[i]);
    else
      qDebug("An analog channel with the given ID already exists");
  }
  emit analogsInserted(ids, analogs);
};

const Event* Acquisition::eventAt(int id) const
{
  QMap<int,Event*>::const_iterator it = this->m_Events.find(id);
  if (it != this->m_Events.end())
    return *it;
  return 0;
};

void Acquisition::setEventFrame(int id, int frame)
{
  QMap<int,Event*>::iterator it = this->m_Events.find(id);
  if (it != this->m_Events.end())
  {
    (*it)->frame = frame;
    (*it)->time = this->timeFromFrame(frame);
    emit eventFrameChanged(id, frame);
  }
};

// Warning: Doesn't free the memory of the replaced events
void Acquisition::setEvents(const QList<int>& ids, const QList<Event*> events)
{
  for (int i = 0 ; i < ids.count() ; ++i)
  {
    QMap<int,Event*>::iterator it = this->m_Events.find(ids[i]);
    if (it != this->m_Events.end())
      *it = events[i];
  }
  emit eventsModified(ids, events);
};

QList<Event*> Acquisition::takeEvents(const QList<int>& ids)
{
  QList<Event*> events;
  for (int i = 0 ; i < ids.count() ; ++i)
  {
    QMap<int,Event*>::iterator it = this->m_Events.find(ids[i]);
    if (it != this->m_Events.end())
    {
      events.push_back(*it);
      this->m_Events.erase(it);
    }
  }
  emit eventsRemoved(ids, events);
  return events;
};

void Acquisition::insertEvents(const QList<int>& ids, const QList<Event*> events)
{
  for (int i = 0 ; i < ids.count() ; ++i)
  {
    QMap<int,Event*>::iterator it = this->m_Events.find(ids[i]);
    if (it == this->m_Events.end())
      this->m_Events.insert(ids[i], events[i]);
    else
      qDebug("An event with the given ID already exists");
    if (ids[i] > this->m_LastEventId)
      this->m_LastEventId = ids[i];
  }
  emit eventsInserted(ids, events);
};

int Acquisition::generateNewEventId()
{
  this->m_LastEventId += 1;
  return this->m_LastEventId;
};

void Acquisition::emitGeneratedInformations(btk::AcquisitionFileIO::Pointer io)
{
  QVector<QString> infos(16);
  QFileInfo fI = QFileInfo(this->m_Filename);
  // Filename
  infos[0] = fI.fileName();
  // Extension
  infos[1] = fI.suffix().toUpper() + QString(" Document");
  // File size
  double sizeDouble = static_cast<double>(fI.size());
  if (sizeDouble <= 1024.0)
    infos[2] = QString::number(sizeDouble) + " bytes";
  else
  {
    sizeDouble /= 1024.0;
    if (sizeDouble <= 1024.0)
      infos[2] = QString::number(sizeDouble) + " KB";
    else
    {
      sizeDouble /= 1024.0;
      infos[2] = QString::number(sizeDouble) + " MB";
    }
  }
  // Creation date
  infos[3] = fI.created().toString();
  // Modification date
  infos[4] = fI.lastModified().toString();
  // File type
  switch(io->GetFileType())
  {
    case btk::AcquisitionFileIO::TypeNotApplicable:
      infos[5] = "N/A";
      break;
    case btk::AcquisitionFileIO::ASCII:
      infos[5] = "ASCII";
      break;
    case btk::AcquisitionFileIO::Binary:
      infos[5] = "Binary";
      break;
  }
  // Byte order
  switch(io->GetByteOrder())
  {
    case btk::AcquisitionFileIO::OrderNotApplicable:
      infos[6] ="N/A";
      break;
    case btk::AcquisitionFileIO::IEEE_LittleEndian:
      infos[6] = "IEEE Little Endian";
      break;
    case btk::AcquisitionFileIO::VAX_LittleEndian:
      infos[6] = "VAX Little Endian";
      break;
    case btk::AcquisitionFileIO::IEEE_BigEndian:
      infos[6] = "IEEE Big Endian";
      break;
  }
  // Storage format
  switch(io->GetStorageFormat())
  {
    case btk::AcquisitionFileIO::StorageNotApplicable:
      infos[7] = "N/A";
      break;
    case btk::AcquisitionFileIO::Float:
      infos[7] = "Float";
      break;
    case btk::AcquisitionFileIO::Integer:
      infos[7] = "Integer";
      break;
  }
  // First frame
  infos[8] = QString::number(this->mp_ROI[0]);
  // Last frame
  infos[9] = QString::number(this->mp_ROI[1]);
  // Frame number
  infos[10] = QString::number(this->mp_ROI[1] - this->mp_ROI[0] + 1);
  // Point number
  infos[11] = QString::number(this->m_Points.count());
  // Point frequency
  infos[12] = QString::number(this->pointFrequency()) + " Hz";
  // Analog number
  infos[13] = QString::number(this->m_Analogs.count());
  // Analog frequency
  infos[14] = QString::number(this->analogFrequency()) + " Hz";
  // Event number
  infos[15] = QString::number(this->m_Events.count());
  emit informationsChanged(infos);
};

void Acquisition::write(const QString& filename, const QMap<int, QVariant>& properties, int lb, int rb, QString& errMsg, bool updateInfo)
{
  btk::Acquisition::Pointer source = this->mp_BTKAcquisition;
  btk::Acquisition::Pointer target = btk::Acquisition::New();
  // Acquisition info
  target->SetFirstFrame(lb);
  target->SetPointFrequency(source->GetPointFrequency());
  target->SetAnalogResolution(source->GetAnalogResolution());
  // Event
  btk::EventCollection::Pointer targetEvents = target->GetEvents();
  for (QMap<int,Event*>::const_iterator it = this->m_Events.begin() ; it != this->m_Events.end() ; ++it)
  {
    Event* e = it.value();
    if ((e->frame >= lb) && (e->frame <= rb))
    {
      targetEvents->InsertItem(btk::Event::New(e->label.toStdString(), 
                                             e->time,
                                             e->context.toStdString(),
                                             btk::Event::Unknown,
                                             e->subject.toStdString(),
                                             e->description.toStdString(),
                                             e->iconId));
    }
  }
  // Metadata
  target->SetMetaData(source->GetMetaData()->Clone());
  // - POINT
  btk::MetaData::Pointer point;
  btk::MetaData::Iterator it = target->GetMetaData()->FindChild("POINT");
  if (it != target->GetMetaData()->End())
    point = *it;
  else
  {
    point = btk::MetaData::New("POINT");
    target->GetMetaData()->AppendChild(point);
  }
  QString strProp;
  QMap<int, QVariant>::const_iterator itProp;
  // - POINT:X_SCREEN
  strProp = "+X";
  if ((itProp = properties.find(xScreen)) != properties.end())
    strProp = itProp.value().toString();
  btk::MetaDataCreateChild(point,"X_SCREEN", strProp.toStdString());
  // - POINT:Y_SCREEN
  strProp = "+Z";
  if ((itProp = properties.find(yScreen)) != properties.end())
    strProp = itProp.value().toString();
  btk::MetaDataCreateChild(point,"Y_SCREEN", strProp.toStdString());
  // Point
  int numFramePoint = rb - lb + 1;
  int numPoints = 0;
  btk::PointCollection::Pointer sourcePoints = source->GetPoints();
  btk::PointCollection::Pointer targetPoints = target->GetPoints();
  for (QMap<int,Point*>::const_iterator it = this->m_Points.begin() ; it != this->m_Points.end() ; ++it)
  {
    Point* p = it.value();
    btk::Point::Type type = btk::Point::Marker;
    if (p->type == Point::Angle)
      type = btk::Point::Angle;
    else if (p->type == Point::Force)
      type = btk::Point::Force;
    else if (p->type == Point::Moment)
      type = btk::Point::Moment;
    else if (p->type == Point::Power)
      type = btk::Point::Power;
    else if (p->type == Point::Scalar)
      type = btk::Point::Scalar;
    btk::Point::Pointer sourceP = sourcePoints->GetItem(p->btkidx);
    btk::Point::Pointer targetP = btk::Point::New(p->label.toStdString(), numFramePoint, type, p->description.toStdString());
    targetP->SetValues(sourceP->GetValues().block(lb-this->m_FirstFrame,0,numFramePoint,3));
    targetP->SetResiduals(sourceP->GetResiduals().block(lb-this->m_FirstFrame,0,numFramePoint,1));
    targetP->SetMasks(sourceP->GetMasks().block(lb-this->m_FirstFrame,0,numFramePoint,1));
    targetPoints->InsertItem(targetP);
    ++numPoints;
  }
  // Analog
  int numAnalogs = 0;
  int numFrameAnalog = numFramePoint * source->GetNumberAnalogSamplePerFrame();
  btk::AnalogCollection::Pointer sourceAnalogs = source->GetAnalogs();
  btk::AnalogCollection::Pointer targetAnalogs = target->GetAnalogs();
  for (QMap<int,Analog*>::const_iterator it = this->m_Analogs.begin() ; it != this->m_Analogs.end() ; ++it)
  {
    Analog* a = it.value();
    btk::Analog::Gain gain = btk::Analog::Unknown;
    if (a->gain == Analog::PlusMinus10)
      gain = btk::Analog::PlusMinus10;
    else if (a->gain == Analog::PlusMinus5)
      gain = btk::Analog::PlusMinus5;
    else if (a->gain == Analog::PlusMinus2Dot5)
      gain = btk::Analog::PlusMinus2Dot5;
    else if (a->gain == Analog::PlusMinus1Dot25)
      gain = btk::Analog::PlusMinus1Dot25;
    else if (a->gain == Analog::PlusMinus1)
      gain = btk::Analog::PlusMinus1;
    btk::Analog::Pointer sourceA = sourceAnalogs->GetItem(it.key());
    btk::Analog::Pointer targetA = btk::Analog::New(a->label.toStdString(), numFrameAnalog);
    targetA->SetUnit(a->unit.toStdString());
    targetA->SetGain(gain);
    targetA->SetScale(a->scale);
    targetA->SetOffset(a->offset);
    targetA->SetDescription(a->description.toStdString());
    targetA->SetValues(sourceA->GetValues().block((lb-this->m_FirstFrame)*source->GetNumberAnalogSamplePerFrame(),0,numFrameAnalog,1));
    targetAnalogs->InsertItem(targetA);
    ++numAnalogs;
  }
  // Final setup
  target->Resize(numPoints, numFramePoint, numAnalogs, source->GetNumberAnalogSamplePerFrame());
  // BTK writer
  btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
  writer->SetFilename(filename.toStdString());
  writer->SetInput(target);
  try
  {
    writer->Update();
  }
  catch (btk::Exception& e)
  {
    errMsg = e.what();
  }
  catch (std::exception& e)
  {
    errMsg = "Unexpected error: " + QString(e.what());
  }
  catch (...)
  {
    errMsg = "Unknown error.";
  }
  if (updateInfo)
    this->emitGeneratedInformations(writer->GetAcquisitionIO());
  errMsg = "";
};

void Acquisition::loadAcquisition()
{
  std::string labelPrefix = "";
  btk::MetaData::ConstIterator itSubjects = this->mp_BTKAcquisition->GetMetaData()->FindChild("SUBJECTS");
  if (itSubjects != this->mp_BTKAcquisition->GetMetaData()->End())
  {
    btk::MetaDataInfo::Pointer labelPrefixesInfo = (*itSubjects)->ExtractChildInfo("LABEL_PREFIXES", btk::MetaDataInfo::Char, 2, false);
    if (labelPrefixesInfo)
    {
      labelPrefix = labelPrefixesInfo->ToString(0);
      labelPrefix = labelPrefix.erase(labelPrefix.find_last_not_of(' ') + 1);
      labelPrefix = labelPrefix.erase(0, labelPrefix.find_first_not_of(' '));
    }
  }
  
  btk::SeparateKnownVirtualMarkersFilter::Pointer virtualMarkersSeparator = static_pointer_cast<btk::SeparateKnownVirtualMarkersFilter>(this->m_BTKProcesses[BTK_SORTED_POINTS]);
  btk::ForcePlatformsExtractor::Pointer forcePlatformsExtractor = static_pointer_cast<btk::ForcePlatformsExtractor>(this->m_BTKProcesses[BTK_FORCE_PLATFORMS]);
  btk::DownsampleFilter<btk::WrenchCollection>::Pointer GRWsDownsampler = static_pointer_cast< btk::DownsampleFilter<btk::WrenchCollection> >(this->m_BTKProcesses[BTK_GRWS_DOWNSAMPLED]);
  virtualMarkersSeparator->SetInput(this->mp_BTKAcquisition->GetPoints());
  forcePlatformsExtractor->SetInput(this->mp_BTKAcquisition);
  GRWsDownsampler->SetUpDownRatio(this->mp_BTKAcquisition->GetNumberAnalogSamplePerFrame());
  // Need to update the separator right now.
  virtualMarkersSeparator->SetLabelPrefix(labelPrefix);
  virtualMarkersSeparator->Update();
  
  this->m_FirstFrame = this->mp_BTKAcquisition->GetFirstFrame();
  this->m_LastFrame = this->mp_BTKAcquisition->GetLastFrame();
  this->mp_ROI[0] = this->m_FirstFrame;
  this->mp_ROI[1] = this->m_LastFrame;
  int inc = 0;
  // The orders for the points are important as their ID follows the same rule than in the class btk::VTKMarkersFramesSource
  // Markers
  btk::PointCollection::Pointer points = virtualMarkersSeparator->GetOutput(0);
  for (btk::PointCollection::ConstIterator it = points->Begin() ; it != points->End() ; ++it)
  {
    Point* p = new Point();
    p->label = QString::fromStdString((*it)->GetLabel());
    p->description = QString::fromStdString((*it)->GetDescription());
    p->type = Point::Marker;
    p->radius = this->m_DefaultMarkerRadius;
    p->color = this->m_DefaultMarkerColor;
    p->btkidx = this->mp_BTKAcquisition->GetPoints()->GetIndexOf(*it);
    this->m_Points.insert(inc++, p);
  }
  // Virtual markers (CoM, CoG, ...)
  points = virtualMarkersSeparator->GetOutput(2);
  for (btk::PointCollection::ConstIterator it = points->Begin() ; it != points->End() ; ++it)
  {
    Point* p = new Point();
    p->label = QString::fromStdString((*it)->GetLabel());
    p->description = QString::fromStdString((*it)->GetDescription());
    p->type = Point::VirtualMarker;
    p->radius = this->m_DefaultMarkerRadius;
    p->color = this->m_DefaultMarkerColor;
    p->btkidx = this->mp_BTKAcquisition->GetPoints()->GetIndexOf(*it);
    this->m_Points.insert(inc++, p);
  }
  // Virtual markers used to define frames
  points = virtualMarkersSeparator->GetOutput(1);
  for (btk::PointCollection::ConstIterator it = points->Begin() ; it != points->End() ; ++it)
  {
    Point* p = new Point();
    p->label = QString::fromStdString((*it)->GetLabel());
    p->description = QString::fromStdString((*it)->GetDescription());
    p->type = Point::VirtualMarkerForFrame;
    p->radius = -1.0;
    p->color = QColor::Invalid;
    p->btkidx = this->mp_BTKAcquisition->GetPoints()->GetIndexOf(*it);
    this->m_Points.insert(inc++, p);
  }
  // Other points
  points = virtualMarkersSeparator->GetOutput(3);
  for (btk::PointCollection::ConstIterator it = points->Begin() ; it != points->End() ; ++it)
  {
    Point* p = new Point();
    p->label = QString::fromStdString((*it)->GetLabel());
    p->description = QString::fromStdString((*it)->GetDescription());
    if ((*it)->GetType() == btk::Point::Angle)
      p->type = Point::Angle;
    else if ((*it)->GetType() == btk::Point::Force)
      p->type = Point::Force;
    else if ((*it)->GetType() == btk::Point::Moment)
      p->type = Point::Moment;
    else if ((*it)->GetType() == btk::Point::Power)
      p->type = Point::Power;
    else if ((*it)->GetType() == btk::Point::Scalar)
      p->type = Point::Scalar;
    p->radius = -1.0;
    p->color = QColor::Invalid;
    p->btkidx = this->mp_BTKAcquisition->GetPoints()->GetIndexOf(*it);
    this->m_Points.insert(inc++, p);
  }
  // Analog
  inc = 0;
  for (btk::Acquisition::AnalogIterator it = this->mp_BTKAcquisition->BeginAnalog() ; it != this->mp_BTKAcquisition->EndAnalog() ; ++it)
  {
    Analog* a = new Analog();
    a->label = QString::fromStdString((*it)->GetLabel());
    a->description = QString::fromStdString((*it)->GetDescription());
    a->unit = QString::fromStdString((*it)->GetUnit());
    switch((*it)->GetGain())
    {
    case btk::Analog::Unknown:
      a->gain = Analog::Unknown;
      break;
    case btk::Analog::PlusMinus10:
      a->gain = Analog::PlusMinus10;
      break;
    case btk::Analog::PlusMinus5:
      a->gain = Analog::PlusMinus5;
      break;
    case btk::Analog::PlusMinus2Dot5:
      a->gain = Analog::PlusMinus2Dot5;
      break;
    case btk::Analog::PlusMinus1Dot25:
      a->gain = Analog::PlusMinus1Dot25;
      break;
    case btk::Analog::PlusMinus1:
      a->gain = Analog::PlusMinus1;
      break;
    }
    a->offset = (*it)->GetOffset();
    a->scale = (*it)->GetScale();
    this->m_Analogs.insert(inc++, a);
  }
  // Event
  inc = -1;
  for (btk::Acquisition::EventIterator it = this->mp_BTKAcquisition->BeginEvent() ; it != this->mp_BTKAcquisition->EndEvent() ; ++it)
  {
    ++inc;
    Event* e = new Event();
    e->label = QString::fromStdString((*it)->GetLabel());;
    e->description = QString::fromStdString((*it)->GetDescription());
    e->context = QString::fromStdString((*it)->GetContext());
    e->subject = QString::fromStdString((*it)->GetSubject());
    e->time = (*it)->GetTime();
    e->frame = (*it)->GetFrame();
    e->iconId = (*it)->GetId();
    this->m_Events.insert(inc, e);
  }
  this->m_LastEventId = inc;
};
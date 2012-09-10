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

#include "Acquisition.h"
#include "LoggerMessage.h"

#include <btkAcquisitionFileReader.h>
#include <btkAcquisitionFileWriter.h>
#include <btkMetaDataUtils.h>
#include <btkMergeAcquisitionFilter.h>
#include <btkAcquisitionFileIOFactory.h>
#include <btkConvert.h>

#include <Qt>
#include <QFileInfo>
#include <QDateTime>
#include <QVariant>
#include <QFileInfo>
#include <QDir>

Acquisition::Acquisition(QObject* parent)
: QObject(parent), mp_BTKAcquisition(), m_BTKProcesses(), m_Filename(),
  m_Points(), m_Analogs(), m_Events(), m_DefaultMarkerColor(Qt::white)
{
  this->m_FirstFrame = 0;
  this->m_LastFrame = 0;
  this->mp_ROI[0] = this->m_FirstFrame;
  this->mp_ROI[1] = this->m_LastFrame;
  this->m_LastPointId = -1;
  this->m_LastEventId = -1;
  this->m_LastVideoId = -1;
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
  btk::WrenchDirectionAngleFilter::Pointer wrenchDirectionAngleFilter = btk::WrenchDirectionAngleFilter::New();
  wrenchDirectionAngleFilter->SetInput(GRWsDownsampler->GetOutput());
  this->m_BTKProcesses[BTK_SORTED_POINTS] = virtualMarkersSeparator;
  this->m_BTKProcesses[BTK_FORCE_PLATFORMS] = forcePlatformsExtractor;
  this->m_BTKProcesses[BTK_GRWS] = GRWsFilter;
  this->m_BTKProcesses[BTK_GRWS_DOWNSAMPLED] = GRWsDownsampler;
  this->m_BTKProcesses[BTK_DIRECTION_ANGLES] = wrenchDirectionAngleFilter;
};

Acquisition::~Acquisition()
{
  this->clear();
};

void Acquisition::supportedReadFileFormats(QStringList& formats)
{
  btk::AcquisitionFileIO::Extensions exts = btk::AcquisitionFileIOFactory::GetSupportedReadExtensions();
  for(btk::AcquisitionFileIO::Extensions::ConstIterator it = exts.Begin() ; it != exts.End() ; ++it)
  {
    QString qstr = QString::fromStdString(it->name);
    if (!it->desc.empty())
      qstr += " " + QString::fromStdString(it->desc);
    formats.append(qstr);
  }
};

void Acquisition::supportedWrittenFileFormats(QStringList& formats)
{
  btk::AcquisitionFileIO::Extensions exts = btk::AcquisitionFileIOFactory::GetSupportedWrittenExtensions();
  for(btk::AcquisitionFileIO::Extensions::ConstIterator it = exts.Begin() ; it != exts.End() ; ++it)
  {
    QString qstr = QString::fromStdString(it->name);
    if (!it->desc.empty())
      qstr += " " + QString::fromStdString(it->desc);
    formats.append(qstr);
  }
};

bool Acquisition::load(const QString& filename)
{
  btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
  reader->SetFilename(filename.toStdString());
  try
  {
    reader->Update();
  }
  catch (btk::Exception& e)
  {
    LOG_CRITICAL(e.what());
    return false;
  }
  catch (std::exception& e)
  {
    LOG_CRITICAL("Unexpected error: " + QString(e.what()));
    return false;
  }
  catch (...)
  {
    LOG_CRITICAL("Unknown error.");
    return false;
  }
  this->clear();
  this->mp_BTKAcquisition = reader->GetOutput();
  this->m_Filename = filename;
  this->loadAcquisition();
  this->emitGeneratedInformations(reader->GetAcquisitionIO());
  return true;
};

bool Acquisition::save(const QString& filename, const QMap<int, QVariant>& properties)
{
  return this->write(filename, properties, this->mp_ROI[0], this->mp_ROI[1], true);
}

bool Acquisition::canBeSaved(const QString& filename)
{
  btk::AcquisitionFileIO::Pointer io = btk::AcquisitionFileIOFactory::CreateAcquisitionIO(qPrintable(filename), btk::AcquisitionFileIOFactory::WriteMode);
  return (io ? true : false);
};

bool Acquisition::exportTo(const QString& filename, const QMap<int, QVariant>& properties, int lb, int rb)
{
  return this->write(filename, properties, lb, rb);
};

bool Acquisition::importFrom(const QStringList& filenames, bool allFramesKept)
{
  // Try to read the given file
  QList<btk::Acquisition::Pointer> acquisitions;
  try
  {
    for (int i = 0 ; i < filenames.count() ; ++i)
    {
      LOG_INFO(tr("Loading acquisition for importation from file: ") + QFileInfo(filenames[i]).fileName());
      btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
      reader->SetFilename(filenames[i].toStdString());
      reader->Update();
      acquisitions << reader->GetOutput();
    }
  }
  catch (btk::Exception& e)
  {
    LOG_CRITICAL(e.what());
    return false;
  }
  catch (std::exception& e)
  {
    LOG_CRITICAL("Unexpected error: " + QString(e.what()));
    return false;
  }
  catch (...)
  {
    LOG_CRITICAL("Unknown error.");
    return false;
  }
  return this->importFrom(acquisitions, allFramesKept);
};

bool Acquisition::importFrom(const QList<btk::Acquisition::Pointer>& acquisitions, bool allFramesKept)
{
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
  btk::MergeAcquisitionFilter::Pointer merger = btk::MergeAcquisitionFilter::New();
  merger->SetFirstFrameRule(allFramesKept ? btk::MergeAcquisitionFilter::KeepAllFrames : btk::MergeAcquisitionFilter::KeepFromHighestFirstFrame);
  merger->SetInput(0, this->mp_BTKAcquisition);
  for (int i = 0 ; i < acquisitions.count() ; ++i)
    merger->SetInput(i+shift, acquisitions[i]);
  merger->Update();
  this->clear();
  this->mp_BTKAcquisition = merger->GetOutput();
  this->loadAcquisition();
  QVector<QString> infos(16,"N/A"); infos[0] = "N/A                         ";
  emit informationsChanged(infos);
  return true;
};

bool Acquisition::importFromAMTI(const QString& filename, bool allFramesKept, const QList<QVariant>& dimensions, bool fromOpenAction)
{
  btk::AMTIForcePlatformFileIO::Pointer io = btk::AMTIForcePlatformFileIO::New();
  io->SetDimensions(dimensions[0].toFloat(), dimensions[1].toFloat(), dimensions[2].toFloat());
  return this->importFromAMTI(filename, allFramesKept, io, fromOpenAction);
};

bool Acquisition::importFromAMTI(const QString& filename, bool allFramesKept, const QList<QVariant>& corners, const QList<QVariant>& origin, bool fromOpenAction)
{
  std::vector<float> c(12), o(3);
  for (int i = 0 ; i < 12 ; ++i)
    c[i] = corners[i].toFloat();
  for (int i = 0 ; i < 3 ; ++i)
    o[i] = origin[i].toFloat();
  btk::AMTIForcePlatformFileIO::Pointer io = btk::AMTIForcePlatformFileIO::New();
  io->SetGeometry(c,o);
  return this->importFromAMTI(filename, allFramesKept, io, fromOpenAction);
};

bool Acquisition::importFromAMTI(const QString& filename, bool allFramesKept, btk::AMTIForcePlatformFileIO::Pointer io, bool fromOpenAction)
{
  // Try to read the given file
  QList<btk::Acquisition::Pointer> acquisitions;
  btk::AcquisitionFileReader::Pointer reader;
  try
  {
    reader = btk::AcquisitionFileReader::New();
    reader->SetAcquisitionIO(io);
    reader->SetFilename(filename.toStdString());
    reader->Update();
    acquisitions << reader->GetOutput();
  }
  catch (btk::Exception& e)
  {
    LOG_CRITICAL(e.what());
    return false;
  }
  catch (std::exception& e)
  {
    LOG_CRITICAL("Unexpected error: " + QString(e.what()));
    return false;
  }
  catch (...)
  {
    LOG_CRITICAL("Unknown error.");
    return false;
  }
  bool res = this->importFrom(acquisitions, allFramesKept);
  if (fromOpenAction)
  {
    this->m_Filename = filename;
    this->emitGeneratedInformations(reader->GetAcquisitionIO());
  }
  return res;
};

bool Acquisition::importFromVideos(const QStringList& paths, bool allFramesKept, int ff, double freq, double duration)
{
  btk::Acquisition::Pointer acq = btk::Acquisition::New();
  acq->Init(0,static_cast<int>(freq * duration));
  acq->SetFirstFrame(ff);
  acq->SetPointFrequency(freq);
  btk::MetaData::Pointer point = btk::MetaDataCreateChild(acq->GetMetaData(), "POINT");
  std::vector<std::string> moveFilenames(paths.size());
  for (int i = 0 ; i < static_cast<int>(paths.size()) ; ++i)
    moveFilenames[i] = paths[i].toStdString();
  btk::MetaDataCreateChild(point, "MOVIE_FILENAME", moveFilenames);
  
  QList<btk::Acquisition::Pointer> acquisitions;
  acquisitions << acq;
  
  return this->importFrom(acquisitions, allFramesKept);
};

void Acquisition::clear()
{
  this->m_Filename = "";
  for (QMap<int,Point*>::iterator it = this->m_Points.begin() ; it != this->m_Points.end() ; ++it)
    delete *it;
  this->m_Points.clear();
  for (QMap<int,Analog*>::iterator it = this->m_Analogs.begin() ; it != this->m_Analogs.end() ; ++it)
    delete *it;
  this->m_Analogs.clear();
  for (QMap<int,Event*>::iterator it = this->m_Events.begin() ; it != this->m_Events.end() ; ++it)
    delete *it;
  this->m_Events.clear();
  for (QMap<int,Video*>::iterator it = this->m_Videos.begin() ; it != this->m_Videos.end() ; ++it)
    delete *it;
  this->m_Videos.clear();
  this->m_LastPointId = -1;
  this->m_LastEventId = -1;
  this->m_LastVideoId = -1;
  this->mp_BTKAcquisition = btk::Acquisition::Pointer(); // NULL
}

void Acquisition::setFirstFrame(int ff)
{
  int diff = this->mp_ROI[0] - ff;
  this->m_FirstFrame -= diff;
  this->m_LastFrame -= diff;
  this->mp_ROI[0] -= diff;
  this->mp_ROI[1] -= diff;
  for (QMap<int,Event*>::iterator it = this->m_Events.begin() ; it != this->m_Events.end() ; ++it)
  {
    (*it)->frame -= diff;
    (*it)->time = static_cast<double>((*it)->frame - 1) / this->pointFrequency();
  }
  for (QMap<int,Video*>::iterator it = this->m_Videos.begin() ; it != this->m_Videos.end() ; ++it)
    (*it)->delay -= static_cast<qint64>(static_cast<double>(diff) / this->pointFrequency() * 1000.0);
  emit firstFrameChanged(ff);
};

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
    if ((it.value()->label.compare(label) == 0) && ((it.value()->type == Point::Marker) || (it.value()->type == Point::VirtualMarker) || (it.value()->type == Point::VirtualMarkerForFrame)))
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

void Acquisition::resetMarkersConfiguration(const QList<int>& ids, const QList<bool>& visibles, const QList<bool>& trajectories, const QList<double>& radii, const QList<QColor>& colors)
{
  QList<int> ids_;
  QList<double> radii_;
  QList<QColor> colors_;
  QList<bool> visibles_;
  QList<bool> trajectories_;
  for (QMap<int,Point*>::iterator it = this->m_Points.begin() ; it != this->m_Points.end() ; ++it)
  {
    if (((*it)->type != Point::Marker) && ((*it)->type != Point::VirtualMarker) && ((*it)->type != Point::VirtualMarkerForFrame))
      continue;
    ids_ << it.key();
    int idx = ids.indexOf(it.key());
    if (idx != -1)
    {
      (*it)->radius = radii[idx]; 
      (*it)->color = colors[idx];
      (*it)->visible = visibles[idx];
      (*it)->trajectoryVisible = trajectories[idx];
    }
    else
    {
      (*it)->radius = this->m_DefaultMarkerRadius;
      (*it)->color = this->m_DefaultMarkerColor;
      (*it)->visible = (((*it)->type == Point::Marker) ? true : false);
      (*it)->trajectoryVisible = false;
    }
    radii_ << (*it)->radius;
    colors_ << (*it)->color;
    visibles_ << (*it)->visible;
    trajectories_ << (*it)->trajectoryVisible;
  }
  emit markersConfigurationReset(ids_, visibles_, trajectories_, radii_, colors_);
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

int Acquisition::createAveragedMarker(const QList<int>& markerIds)
{
  btk::PointCollection::Pointer markers = btk::PointCollection::New();
  std::string desc = "Average between the markers ";
  for (int i = 0 ; i < markerIds.count() ; ++i)
  {
    QMap<int,Point*>::iterator it = this->m_Points.find(markerIds[i]);
    if (it != this->m_Points.end())
    {
      if ((*it)->btkidx < this->mp_BTKAcquisition->GetPoints()->GetItemNumber())
      {
        btk::Point::Pointer pt = this->mp_BTKAcquisition->GetPoints()->GetItem((*it)->btkidx);
        if (i != 0)
        {
          if (i == (markerIds.count()-1))
            desc += " and ";
          else
            desc += ", ";
        }
        desc += pt->GetLabel();
        markers->InsertItem(pt);
      }
      else
      {
        qWarning("No marker found in the BTK acquisition with the index %i", (*it)->btkidx);
        return -1;
      }
    }
    else
    {
      qDebug("The marker with the index %i was not found!", markerIds[i]);
      return -1;
    }
  }
  int numMarkers = markers->GetItemNumber();
  if (numMarkers < 2)
  {
    qDebug("At least two markers are required to create an averaged marker!");
    return -1;
  }
  
  btk::Point::Pointer average = btk::Point::New("Uname*" + btk::ToString(this->m_LastPointId+1), this->pointFrameNumber(), btk::Point::Marker, desc);
  for (int i = 0  ; i < this->pointFrameNumber() ; ++i)
  {
    bool valid = true;
    for (btk::PointCollection::ConstIterator it = markers->Begin() ; it != markers->End() ; ++it)
    {
      if ((*it)->GetResiduals().coeff(i) < 0.0)
      {
        valid = false;
        break;
      }
    }
    if (valid)
    {
      double x = 0.0, y = 0.0, z = 0.0;
      for (btk::PointCollection::ConstIterator it = markers->Begin() ; it != markers->End() ; ++it)
      {
        x += (*it)->GetValues().coeff(i,0);
        y += (*it)->GetValues().coeff(i,1);
        z += (*it)->GetValues().coeff(i,2);
      }
      average->SetFrame(i, x / static_cast<double>(numMarkers),
                           y / static_cast<double>(numMarkers),
                           z / static_cast<double>(numMarkers));
    }
    else
      average->SetFrame(i, 0.0, 0.0, 0.0, -1.0);
  }
  this->mp_BTKAcquisition->GetPoints()->InsertItem(average);
  
  Point* p = new Point();
  p->label = QString::fromStdString(average->GetLabel());
  p->description = QString::fromStdString(average->GetDescription());
  p->type = Point::Marker;
  p->visible = true;
  p->trajectoryVisible = false;
  p->radius = this->m_DefaultMarkerRadius;
  p->color = this->m_DefaultMarkerColor;
  p->btkidx = this->mp_BTKAcquisition->GetPoints()->GetItemNumber()-1;
  this->insertPoints(QList<int>() << this->m_LastPointId, QList<Point*>() << p);

  LOG_INFO("Marker " + p->label + " created. " + p->description);
  this->m_LastPointId++;
  return p->btkidx;
};

int Acquisition::generateNewPointId()
{
  this->m_LastPointId += 1;
  return this->m_LastPointId;
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
      qDebug("A point with the given ID already exists: %s", qPrintable((*it)->label));
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

void Acquisition::setMarkersVisible(const QVector<int>& ids, const QVector<bool>& visibles)
{
  for (int i = 0 ; i < ids.count() ; ++i)
  {
    QMap<int,Point*>::iterator it = this->m_Points.find(ids[i]);
    if (it != this->m_Points.end())
      (*it)->visible = visibles[i];
  }
  emit markersVisibilityChanged(ids, visibles);
};

void Acquisition::setMarkersTrajectoryVisible(const QVector<int>& ids, const QVector<bool>& visibles)
{
  for (int i = 0 ; i < ids.count() ; ++i)
  {
    QMap<int,Point*>::iterator it = this->m_Points.find(ids[i]);
    if (it != this->m_Points.end())
      (*it)->trajectoryVisible = visibles[i];
  }
  emit markersTrajectoryVisibilityChanged(ids, visibles);
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

void Acquisition::setVideoDelay(const QVector<int>& ids, const QVector<qint64>& delays)
{
  for (int i = 0 ; i < ids.count() ; ++i)
  {
    QMap<int,Video*>::iterator it = this->m_Videos.find(ids[i]);
    if (it != this->m_Videos.end())
      (*it)->delay = delays[i];
  }
  emit videosDelayChanged(ids, delays);
};

QList<Video*> Acquisition::takeVideos(const QList<int>& ids)
{
  QList<Video*> videos;
  for (int i = 0 ; i < ids.count() ; ++i)
  {
    QMap<int,Video*>::iterator it = this->m_Videos.find(ids[i]);
    if (it != this->m_Videos.end())
    {
      videos.push_back(*it);
      this->m_Videos.erase(it);
    }
  }
  emit videosRemoved(ids, videos);
  return videos;
};


void Acquisition::insertVideos(const QList<int>& ids, const QList<Video*> videos)
{
  for (int i = 0 ; i < ids.count() ; ++i)
  {
    QMap<int,Video*>::iterator it = this->m_Videos.find(ids[i]);
    if (it == this->m_Videos.end())
      this->m_Videos.insert(ids[i], videos[i]);
    else
      qDebug("A video with the given ID already exists");
  }
  emit videosInserted(ids, videos);
};

void Acquisition::importVideos(const QStringList& paths)
{
  QList<int> ids;
  QList<Video*> videos;
  for (QStringList::const_iterator it = paths.begin() ; it != paths.end() ; ++it)
  {
    QFileInfo fI = QFileInfo(*it);
    Video* v = new Video();
    v->label = fI.completeBaseName();
    v->filename = fI.fileName();
    v->path = fI.canonicalPath();
    v->delay = 0; // ms.
    v->error = false;
    int id = ++this->m_LastVideoId;
    this->m_Videos.insert(id, v);
    ids << this->m_LastVideoId;
    videos << v;
    LOG_INFO(tr("Importing video from file: ") + v->filename);
  }
  emit videosImported(ids, videos);
};

bool Acquisition::write(const QString& filename, const QMap<int, QVariant>& properties, int lb, int rb, bool updateInfo)
{
  btk::Acquisition::Pointer source = this->mp_BTKAcquisition;
  btk::Acquisition::Pointer target = btk::Acquisition::New();
  // Acquisition info
  target->SetFirstFrame(lb);
  target->SetPointFrequency(source->GetPointFrequency());
  target->SetAnalogResolution(source->GetAnalogResolution());
  target->SetPointUnits(source->GetPointUnits());
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
  btk::MetaDataCreateChild(point, "X_SCREEN", strProp.toStdString());
  // - POINT:Y_SCREEN
  strProp = "+Z";
  if ((itProp = properties.find(yScreen)) != properties.end())
    strProp = itProp.value().toString();
  btk::MetaDataCreateChild(point, "Y_SCREEN", strProp.toStdString());
  // Video
  std::vector<std::string> movieFilename(this->m_Videos.size());
  std::vector<std::string> movieId(this->m_Videos.size());
  std::vector<float> movieDelay(this->m_Videos.size());
  bool videoCompatibleVicon = true;
  int inc = 0;
  QString fileBaseName = QFileInfo(filename).baseName();
  for (QMap<int,Video*>::const_iterator it = this->m_Videos.begin() ; it != this->m_Videos.end() ; ++it)
  {
    Video* v = it.value();
    movieFilename[inc] = v->filename.toStdString();
    movieDelay[inc] = v->delay / 1000.0f;
    if (videoCompatibleVicon)
    {
      if (v->filename.startsWith(fileBaseName + "." + v->label))
        movieId[inc] = v->label.toStdString();
      else 
        videoCompatibleVicon = false;
    }
    ++inc;
  }
  // - POINT:MOVIE_DELAY
  btk::MetaDataCreateChild(point, "MOVIE_DELAY", movieDelay);
  // Try first to be compatible with VICON using the metadata POINT:MOVIE_ID...
  if (videoCompatibleVicon)
  {
    btk::MetaDataCreateChild(point, "MOVIE_ID", movieId);
    point->RemoveChild("MOVIE_FILENAME");
  }
  // ... Or if it is not possible, then create the metadata POINT:MOVIE_FILENAME
  else
  {
    btk::MetaDataCreateChild(point, "MOVIE_FILENAME", movieFilename);
    point->RemoveChild("MOVIE_ID");
  }
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
    LOG_CRITICAL(e.what());
    return false;
  }
  catch (std::exception& e)
  {
    LOG_CRITICAL("Unexpected error: " + QString(e.what()));
    return false;
  }
  catch (...)
  {
    LOG_CRITICAL("Unknown error.");
    return false;
  }
  if (updateInfo)
    this->emitGeneratedInformations(writer->GetAcquisitionIO());
  return true;
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
  // FIXME: The current solution is not the best if there is more than 32768 markers as the first ID of the model ouputs starts from this value. Maybe a map between the marker's ID and the corresponding index in the VTKMarkersFramesSource should fix definitively this problem
  // Markers
  btk::PointCollection::Pointer points = virtualMarkersSeparator->GetOutput(0);
  for (btk::PointCollection::ConstIterator it = points->Begin() ; it != points->End() ; ++it)
  {
    Point* p = new Point();
    p->label = QString::fromStdString((*it)->GetLabel());
    p->description = QString::fromStdString((*it)->GetDescription());
    p->type = Point::Marker;
    p->visible = true;
    p->trajectoryVisible = false;
    p->radius = this->m_DefaultMarkerRadius;
    p->color = this->m_DefaultMarkerColor;
    p->btkidx = this->mp_BTKAcquisition->GetPoints()->GetIndexOf(*it);
    this->m_Points.insert(virtualMarkersSeparator->GetOutput(4)->GetIndexOf(*it), p);
  }
  // Virtual markers (CoM, CoG, ...)
  points = virtualMarkersSeparator->GetOutput(2);
  for (btk::PointCollection::ConstIterator it = points->Begin() ; it != points->End() ; ++it)
  {
    Point* p = new Point();
    p->label = QString::fromStdString((*it)->GetLabel());
    p->description = QString::fromStdString((*it)->GetDescription());
    p->type = Point::VirtualMarker;
    p->visible = false;
    p->trajectoryVisible = false;
    p->radius = this->m_DefaultMarkerRadius;
    p->color = this->m_DefaultMarkerColor;
    p->btkidx = this->mp_BTKAcquisition->GetPoints()->GetIndexOf(*it);
    this->m_Points.insert(virtualMarkersSeparator->GetOutput(4)->GetIndexOf(*it), p);
  }
  // Virtual markers used to define frames
  points = virtualMarkersSeparator->GetOutput(1);
  for (btk::PointCollection::ConstIterator it = points->Begin() ; it != points->End() ; ++it)
  {
    Point* p = new Point();
    p->label = QString::fromStdString((*it)->GetLabel());
    p->description = QString::fromStdString((*it)->GetDescription());
    p->type = Point::VirtualMarkerForFrame;
    p->visible = false;
    p->trajectoryVisible = false;
    p->radius = this->m_DefaultMarkerRadius;
    p->color = this->m_DefaultMarkerColor;
    p->btkidx = this->mp_BTKAcquisition->GetPoints()->GetIndexOf(*it);
    this->m_Points.insert(virtualMarkersSeparator->GetOutput(4)->GetIndexOf(*it), p);
  }
  this->m_LastPointId = this->m_Points.size();
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
    p->visible = false;
    p->trajectoryVisible = false;
    p->radius = -1.0;
    p->color = QColor::Invalid;
    p->btkidx = this->mp_BTKAcquisition->GetPoints()->GetIndexOf(*it);
    this->m_Points.insert(32767 + inc++, p); // 32767: To distinct clearly the markers from the others points.
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
    case btk::Analog::PlusMinus0Dot5:
      a->gain = Analog::PlusMinus0Dot5;
      break;
    case btk::Analog::PlusMinus0Dot25:
      a->gain = Analog::PlusMinus0Dot25;
      break;
    case btk::Analog::PlusMinus0Dot1:
      a->gain = Analog::PlusMinus0Dot1;
      break;
    case btk::Analog::PlusMinus0Dot05:
      a->gain = Analog::PlusMinus0Dot05;
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
  // Video
  inc = -1;
  btk::Acquisition::MetaDataConstIterator point, pointMovieId;
  if ((point = this->mp_BTKAcquisition->GetMetaData()->FindChild("POINT")) != this->mp_BTKAcquisition->EndMetaData())
  {
    btk::MetaDataInfo::Pointer pointMovieFilename = (*point)->ExtractChildInfo("MOVIE_FILENAME", btk::MetaDataInfo::Char, 2);
    btk::MetaDataInfo::Pointer pointMovieDelay = (*point)->ExtractChildInfo("MOVIE_DELAY", btk::MetaDataInfo::Real, 1);
    btk::MetaDataInfo::Pointer pointMovieId = (*point)->ExtractChildInfo("MOVIE_ID", btk::MetaDataInfo::Char, 2);
    std::vector<double> movieDelay;
    if (pointMovieDelay && pointMovieDelay->HasValues())
      movieDelay = pointMovieDelay->ToDouble();
    // - Using the camera ID  (not standard - used by Vicon)
    if (pointMovieId && pointMovieId->HasValues())
    {
      std::vector<std::string> movieId = pointMovieId->ToString();
      if (pointMovieDelay && pointMovieDelay->HasValues())
        movieDelay = pointMovieDelay->ToDouble();
      movieDelay.resize(movieId.size(), 0.0);
      for (std::vector<std::string>::const_iterator it = movieId.begin() ; it != movieId.end() ; ++it)
      {
        ++inc;
        QString str = QString::fromStdString(*it);
        str = str.trimmed();
        // Look if the video file exists
        QFileInfo fI = QFileInfo(this->m_Filename);
        QString filenamePart = fI.baseName() + "." +  str + ".*";
        QDir dir = QDir(fI.canonicalPath());
        QStringList videoList = dir.entryList(QStringList() << filenamePart, QDir::Files | QDir::Readable);
        QString filename, path;
        bool error = false;
        if (videoList.isEmpty())
        {
          LOG_WARNING("No video file found with the ID " +  str + ".");
          filename = fI.baseName() + "." +  str + ".avi"; // Generic filename based on the ID
          error = true;
        }
        else
        {
          if (videoList.size() > 1)
            LOG_WARNING("More than one file was found with this video ID. Only the first one is loaded.");
          filename = videoList.at(0);
          path = fI.canonicalPath();
        }
        // Set the new video
        Video* m = new Video();
        m->label = str;
        m->filename = filename;
        m->path = path;
        m->delay = static_cast<qint64>(movieDelay[inc] * 1000); // ms.
        m->error = error;
        this->m_Videos.insert(inc, m);
      }
    }
    // Using the video filename (not standard - proposed by Mokka)
    else if (pointMovieFilename)
    {
      std::vector<std::string> movieFilename = pointMovieFilename->ToString();
      movieDelay.resize(movieFilename.size(), 0.0);
      for (std::vector<std::string>::const_iterator it = movieFilename.begin() ; it != movieFilename.end() ; ++it)
      {
        ++inc;
        QString str = QString::fromStdString(*it);
        str = str.trimmed();
        // Look if the video file exists
        QString filename, path;
        bool videoFileFound = false;
        QFileInfo fI = QFileInfo(this->m_Filename);
        // - Global path
        QFileInfo fIg = QFileInfo(str);
        if (fIg.exists())
        {
          filename = fIg.fileName();
          path = fIg.path();
          videoFileFound = true;
        }
        // - Relative path
        if (!videoFileFound)
        {
          QDir dir = QDir(fI.canonicalPath());
          QStringList videoList = dir.entryList(QStringList() << str, QDir::Files | QDir::Readable);
          
          if (videoList.isEmpty())
          {
            LOG_WARNING("No video file found with the name " +  str + ". You need to put the video files in the same folder than the acquisition.");
            filename = str;
          }
          else
          {
            if (videoList.size() > 1)
              LOG_WARNING("More than one file was found with this video name. Only the first one is loaded.");
            filename = videoList.at(0);
            path = fI.canonicalPath();
            videoFileFound = true;
          }
        }
        // Set the new video
        Video* m = new Video();
        // - Check if the label comes from a camera ID as set by Vicon. If yes, only set the ID as label.
        QString label = QFileInfo(filename).completeBaseName();
        QString fileBaseName = fI.baseName() + "."; // Because the format used by Vicon is <acquisition_basename>.<camera_id>.<video_ext>
        if (label.startsWith(fileBaseName))
          label = label.right(label.size() - fileBaseName.size());
        m->label = label;
        m->filename = filename;
        m->path = path;
        m->delay = static_cast<qint64>(movieDelay[inc] * 1000); // ms.
        m->error = !videoFileFound;
        this->m_Videos.insert(inc, m);
      }
    }
  }
  this->m_LastVideoId = inc;
};
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
 
#include "UndoCommands.h"
#include "UserRoles.h"
#include "MainWindow.h"

#include <btkVTKMarkersFramesSource.h>

// ----------------------------------------------- //
//                     GENERAL                     //
// ----------------------------------------------- //

// --------------- EditRegionOfInterest ---------------
EditRegionOfInterest::EditRegionOfInterest(Acquisition* acq, int lb, int rb, QUndoCommand* parent)
: AcquisitionUndoCommand(parent), m_Ids(), m_Events()
{
  this->mp_Acquisition = acq;
  this->mp_ROI[0] = lb;
  this->mp_ROI[1] = rb;
  
  for (QMap<int,Event*>::const_iterator it = this->mp_Acquisition->events().begin() ; it != this->mp_Acquisition->events().end() ; ++it)
  {
    if ((it.value()->frame < this->mp_ROI[0]) || (it.value()->frame > this->mp_ROI[1]))
      this->m_Ids << it.key();
  }
};

EditRegionOfInterest::~EditRegionOfInterest()
{
  for (int i = 0 ; i < this->m_Events.count() ; ++i)
    delete this->m_Events[i];
};

void EditRegionOfInterest::action()
{
  int temp[2];
  this->mp_Acquisition->regionOfInterest(temp[0], temp[1]);
  this->mp_Acquisition->setRegionOfInterest(this->mp_ROI[0], this->mp_ROI[1]);
  this->mp_ROI[0] = temp[0];
  this->mp_ROI[1] = temp[1];
};

void EditRegionOfInterest::undo()
{
  this->action();
  this->mp_Acquisition->insertEvents(this->m_Ids, this->m_Events);
  this->m_Events.clear();
}

void EditRegionOfInterest::redo()
{
  this->action();
  this->m_Events = this->mp_Acquisition->takeEvents(this->m_Ids);
}

// ----------------------------------------------- //
//               POINT/MARKER EDITION              //
// ----------------------------------------------- //

// --------------- EditPointLabel ---------------
EditPointLabel::EditPointLabel(Acquisition* acq, int id, const QString& label, QUndoCommand* parent)
: AcquisitionUndoCommand(parent), m_Label(label)
{
  this->mp_Acquisition = acq;
  this->m_Id = id;
};

void EditPointLabel::action()
{
  QString temp = this->mp_Acquisition->pointLabel(this->m_Id);
  this->mp_Acquisition->setPointLabel(this->m_Id, this->m_Label);
  this->m_Label = temp;
};

// --------------- EditPointDescription ---------------
EditPointDescription::EditPointDescription(Acquisition* acq, const QVector<int>& ids, const QString& desc, QUndoCommand* parent)
: AcquisitionUndoCommand(parent), m_Ids(ids), m_Descriptions(ids.count())
{
  this->mp_Acquisition = acq;
  for (int i = 0 ; i < this->m_Ids.count() ; ++i)
    this->m_Descriptions[i] = desc;
};

void EditPointDescription::action()
{
  QVector<QString> temp(this->m_Ids.count());
  for (int i = 0 ; i < this->m_Ids.count() ; ++i)
    temp[i] = this->mp_Acquisition->pointDescription(this->m_Ids[i]);
  this->mp_Acquisition->setPointsDescription(this->m_Ids, this->m_Descriptions);
  this->m_Descriptions = temp;
};

// --------------- EditMarkersRadius ---------------
EditMarkersRadius::EditMarkersRadius(Acquisition* acq, const QVector<int>& ids, double radius, QUndoCommand* parent)
: ConfigurationUndoCommand(parent), m_Ids(ids), m_Radii(ids.count())
{
  this->mp_Acquisition = acq;
  for (int i = 0 ; i < this->m_Ids.count() ; ++i)
    this->m_Radii[i] = radius;
};

void EditMarkersRadius::action()
{
  QVector<double> temp(this->m_Ids.count());
  for (int i = 0 ; i < this->m_Ids.count() ; ++i)
    temp[i] = this->mp_Acquisition->markerRadius(this->m_Ids[i]);
  this->mp_Acquisition->setMarkersRadius(this->m_Ids, this->m_Radii);
  this->m_Radii = temp;
};

// --------------- EditMarkersColor ---------------
EditMarkersColor::EditMarkersColor(Acquisition* acq, const QVector<int>& ids, const QColor& color, QUndoCommand* parent)
: ConfigurationUndoCommand(parent), m_Ids(ids), m_Colors(ids.count())
{
  this->mp_Acquisition = acq;
  for (int i = 0 ; i < this->m_Ids.count() ; ++i)
    this->m_Colors[i] = color;
};

void EditMarkersColor::action()
{
  QVector<QColor> temp(this->m_Ids.count());
  for (int i = 0 ; i < this->m_Ids.count() ; ++i)
    temp[i] = this->mp_Acquisition->markerColor(this->m_Ids[i]);
  this->mp_Acquisition->setMarkersColor(this->m_Ids, this->m_Colors);
  this->m_Colors = temp;
};

// --------------- RemovePoints ---------------
RemovePoints::RemovePoints(Acquisition* acq, const QList<int>& ids, QUndoCommand* parent)
: AcquisitionUndoCommand(parent), m_Ids(ids), m_Points()
{
  this->mp_Acquisition = acq;
};

RemovePoints::~RemovePoints()
{
  for (int i = 0 ; i < this->m_Points.count() ; ++i)
    delete this->m_Points[i];
};

void RemovePoints::undo()
{
  this->mp_Acquisition->insertPoints(this->m_Ids, this->m_Points);
  this->m_Points.clear();
};

void RemovePoints::redo()
{
  this->m_Points = this->mp_Acquisition->takePoints(this->m_Ids);
};

// ----------------------------------------------- //
//                  ANALOG EDITION                 //
// ----------------------------------------------- //

// --------------- EditAnalogLabel ---------------
EditAnalogLabel::EditAnalogLabel(Acquisition* acq, int id, const QString& label, QUndoCommand* parent)
: AcquisitionUndoCommand(parent), m_Label(label)
{
  this->mp_Acquisition = acq;
  this->m_Id = id;
};

void EditAnalogLabel::action()
{
  QString temp = this->mp_Acquisition->analogLabel(this->m_Id);
  this->mp_Acquisition->setAnalogLabel(this->m_Id, this->m_Label);
  this->m_Label = temp;
};

// --------------- EditAnalogDescription ---------------
EditAnalogDescription::EditAnalogDescription(Acquisition* acq, const QVector<int>& ids, const QString& desc, QUndoCommand* parent)
: AcquisitionUndoCommand(parent), m_Ids(ids), m_Descriptions(ids.count())
{
  this->mp_Acquisition = acq;
  for (int i = 0 ; i < this->m_Ids.count() ; ++i)
    this->m_Descriptions[i] = desc;
};

void EditAnalogDescription::action()
{
  QVector<QString> temp(this->m_Ids.count());
  for (int i = 0 ; i < this->m_Ids.count() ; ++i)
    temp[i] = this->mp_Acquisition->analogDescription(this->m_Ids[i]);
  this->mp_Acquisition->setAnalogsDescription(this->m_Ids, this->m_Descriptions);
  this->m_Descriptions = temp;
};

// --------------- EditAnalogsUnit ---------------
EditAnalogsUnit::EditAnalogsUnit(Acquisition* acq, const QVector<int>& ids, const QString& unit, QUndoCommand* parent)
: AcquisitionUndoCommand(parent), m_Ids(ids), m_Units(ids.count())
{
  this->mp_Acquisition = acq;
  for (int i = 0 ; i < this->m_Ids.count() ; ++i)
    this->m_Units[i] = unit;
};

void EditAnalogsUnit::action()
{
  QVector<QString> temp(this->m_Ids.count());
  for (int i = 0 ; i < this->m_Ids.count() ; ++i)
    temp[i] = this->mp_Acquisition->analogUnit(this->m_Ids[i]);
  this->mp_Acquisition->setAnalogsUnit(this->m_Ids, this->m_Units);
  this->m_Units = temp;
};

// --------------- EditAnalogsGain ---------------
EditAnalogsGain::EditAnalogsGain(Acquisition* acq, const QVector<int>& ids, Analog::Gain gain, QUndoCommand* parent)
: AcquisitionUndoCommand(parent), m_Ids(ids), m_Gains(ids.count())
{
  this->mp_Acquisition = acq;
  for (int i = 0 ; i < this->m_Ids.count() ; ++i)
    this->m_Gains[i] = gain;
};

void EditAnalogsGain::action()
{
  QVector<Analog::Gain> temp(this->m_Ids.count());
  for (int i = 0 ; i < this->m_Ids.count() ; ++i)
    temp[i] = this->mp_Acquisition->analogGain(this->m_Ids[i]);
  this->mp_Acquisition->setAnalogsGain(this->m_Ids, this->m_Gains);
  this->m_Gains = temp;
};

// --------------- EditAnalogsOffset ---------------
EditAnalogsOffset::EditAnalogsOffset(Acquisition* acq, const QVector<int>& ids, int offset, QUndoCommand* parent)
: AcquisitionUndoCommand(parent), m_Ids(ids), m_Offsets(ids.count())
{
  this->mp_Acquisition = acq;
  for (int i = 0 ; i < this->m_Ids.count() ; ++i)
    this->m_Offsets[i] = offset;
};

void EditAnalogsOffset::action()
{
  QVector<int> temp(this->m_Ids.count());
  for (int i = 0 ; i < this->m_Ids.count() ; ++i)
    temp[i] = this->mp_Acquisition->analogOffset(this->m_Ids[i]);
  this->mp_Acquisition->setAnalogsOffset(this->m_Ids, this->m_Offsets);
  this->m_Offsets = temp;
};

// --------------- EditAnalogsScale ---------------
EditAnalogsScale::EditAnalogsScale(Acquisition* acq, const QVector<int>& ids, double scale, QUndoCommand* parent)
: AcquisitionUndoCommand(parent), m_Ids(ids), m_Scales(ids.count())
{
  this->mp_Acquisition = acq;
  for (int i = 0 ; i < this->m_Ids.count() ; ++i)
    this->m_Scales[i] = scale;
};

void EditAnalogsScale::action()
{
  QVector<double> temp(this->m_Ids.count());
  for (int i = 0 ; i < this->m_Ids.count() ; ++i)
    temp[i] = this->mp_Acquisition->analogScale(this->m_Ids[i]);
  this->mp_Acquisition->setAnalogsScale(this->m_Ids, this->m_Scales);
  this->m_Scales = temp;
};

// --------------- RemoveAnalogs ---------------
RemoveAnalogs::RemoveAnalogs(Acquisition* acq, const QList<int>& ids, QUndoCommand* parent)
: AcquisitionUndoCommand(parent), m_Ids(ids), m_Analogs()
{
  this->mp_Acquisition = acq;
};

RemoveAnalogs::~RemoveAnalogs()
{
  for (int i = 0 ; i < this->m_Analogs.count() ; ++i)
    delete this->m_Analogs[i];
};

void RemoveAnalogs::undo()
{
  this->mp_Acquisition->insertAnalogs(this->m_Ids, this->m_Analogs);
  this->m_Analogs.clear();
};

void RemoveAnalogs::redo()
{
  this->m_Analogs = this->mp_Acquisition->takeAnalogs(this->m_Ids);
};

// ----------------------------------------------- //
//                   EVENT EDITION                 //
// ----------------------------------------------- //

// --------------- EditEventFrame ---------------
EditEventFrame::EditEventFrame(Acquisition* acq, int id, int frame, QUndoCommand* parent)
: AcquisitionUndoCommand(parent)
{
  this->mp_Acquisition = acq;
  this->m_Id = id;
  this->m_Frame = frame;
};

void EditEventFrame::action()
{
  int temp = this->mp_Acquisition->eventFrame(this->m_Id);
  this->mp_Acquisition->setEventFrame(this->m_Id, this->m_Frame);
  this->m_Frame = temp;
};

// --------------- SetEvents  ---------------
SetEvents::SetEvents(Acquisition* acq, const QList<int>& ids, const QList<Event*>& events, QUndoCommand* parent)
: AcquisitionUndoCommand(parent), m_Ids(ids), m_Events(events)
{
  this->mp_Acquisition = acq;
};

SetEvents::~SetEvents()
{
  for (int i = 0 ; i < this->m_Events.count() ; ++i)
    delete this->m_Events[i];
};

void SetEvents::action()
{
  QList<Event*> temp;
  for (int i = 0 ; i < this->m_Ids.count() ; ++i)
    temp << this->mp_Acquisition->events().value(this->m_Ids[i], NULL);
  this->mp_Acquisition->setEvents(this->m_Ids, this->m_Events);
  this->m_Events = temp;
};

// --------------- RemoveEvents  ---------------
RemoveEvents::RemoveEvents(Acquisition* acq, const QList<int>& ids, QUndoCommand* parent)
: AcquisitionUndoCommand(parent), m_Ids(ids), m_Events()
{
  this->mp_Acquisition = acq;
};

RemoveEvents::~RemoveEvents()
{
  for (int i = 0 ; i < this->m_Events.count() ; ++i)
    delete this->m_Events[i];
};

void RemoveEvents::undo()
{
  this->mp_Acquisition->insertEvents(this->m_Ids, this->m_Events);
  this->m_Events.clear();
};

void RemoveEvents::redo()
{
  this->m_Events = this->mp_Acquisition->takeEvents(this->m_Ids);
};

// --------------- InsertEvent  ---------------
InsertEvent::InsertEvent(Acquisition* acq, Event* e, QUndoCommand* parent)
: AcquisitionUndoCommand(parent), m_Ids(), m_Events()
{
  this->mp_Acquisition = acq;
  this->m_Ids.push_back(acq->generateNewEventId());
  this->m_Events.push_back(e);
};

InsertEvent::~InsertEvent()
{
  for (int i = 0 ; i < this->m_Events.count() ; ++i)
    delete this->m_Events[i];
};

void InsertEvent::undo()
{
  this->m_Events = this->mp_Acquisition->takeEvents(this->m_Ids);
};

void InsertEvent::redo()
{
  this->mp_Acquisition->insertEvents(this->m_Ids, this->m_Events);
  this->m_Events.clear();
};
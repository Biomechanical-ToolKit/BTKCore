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

#include "Model.h"

Model::Model(QObject* parent)
: QObject(parent), m_Segments(), m_DefaultSegmentColor(Qt::white)
{
  this->m_LastSegmentId = -1;
};

Model::~Model()
{
  this->clear();
};

void Model::clear()
{
  for (QMap<int,Segment*>::iterator it = this->m_Segments.begin() ; it != this->m_Segments.end() ; ++it)
    delete *it;
  this->m_Segments.clear();
  this->m_LastSegmentId = -1;
};

void Model::setSegments(const QList<Segment>& segments)
{
  this->clear();
  QList<Segment*> segList;
  QList<int> ids;
  for (QList<Segment>::const_iterator it = segments.begin() ; it != segments.end() ; ++it)
  {
    Segment* seg = new Segment(*it);
    int id = this->generateNewSegmentId();
    this->m_Segments.insert(id, seg);
    segList << seg;
    ids << id;
  }
  emit segmentsChanged(ids, segList);
};

void Model::setSegmentLabel(int id, const QString& label)
{
  QMap<int,Segment*>::iterator it = this->m_Segments.find(id);
  if (it != this->m_Segments.end())
  {
    (*it)->label = label;
    emit segmentLabelChanged(id, label);
  }
};

void Model::setSegmentsDescription(const QVector<int>& ids, const QVector<QString>& descs)
{
  for (int i = 0 ; i < ids.count() ; ++i)
  {
    QMap<int,Segment*>::iterator it = this->m_Segments.find(ids[i]);
    if (it != this->m_Segments.end())
      (*it)->description = descs[i];
  }
  emit segmentsDescriptionChanged(ids, descs);
};

void Model::setSegmentColor(int id, const QColor& color)
{
  QMap<int,Segment*>::iterator it = this->m_Segments.find(id);
  if (it != this->m_Segments.end())
  {
    (*it)->color = color;
    emit segmentColorChanged(id, color);
  }
};

void Model::setSegmentsColor(const QVector<int>& ids, const QVector<QColor>& colors)
{
  for (int i = 0 ; i < ids.count() ; ++i)
  {
    QMap<int,Segment*>::iterator it = this->m_Segments.find(ids[i]);
    if (it != this->m_Segments.end())
      (*it)->color = colors[i];
  }
  emit segmentsColorChanged(ids, colors);
};

void Model::setSegmentLinks(int id, const QVector<int>& markerIds, const QVector< QPair<int,int> >& links)
{
  QMap<int,Segment*>::iterator it = this->m_Segments.find(id);
  if (it != this->m_Segments.end())
  {
    (*it)->markerIds = markerIds;
    (*it)->links = links;
    emit segmentLinksChanged(id, markerIds, links);
  }
};

QList<Segment*> Model::takeSegments(const QList<int>& ids)
{
  QList<Segment*> segments;
  for (int i = 0 ; i < ids.count() ; ++i)
  {
    QMap<int,Segment*>::iterator it = this->m_Segments.find(ids[i]);
    if (it != this->m_Segments.end())
    {
      segments.push_back(*it);
      this->m_Segments.erase(it);
    }
  }
  emit segmentsRemoved(ids, segments);
  return segments;
};

void Model::insertSegments(const QList<int>& ids, const QList<Segment*> segments)
{
  for (int i = 0 ; i < ids.count() ; ++i)
  {
    QMap<int,Segment*>::iterator it = this->m_Segments.find(ids[i]);
    if (it == this->m_Segments.end())
      this->m_Segments.insert(ids[i], segments[i]);
    else
      qDebug("A segment with the given ID already exists");
  }
  emit segmentsInserted(ids, segments);
};

int Model::findSegmentIdFromLabel(const QString& label) const
{
  for (QMap<int,Segment*>::const_iterator it = this->m_Segments.begin() ; it != this->m_Segments.end() ; ++it)
  {
    if ((*it)->label.compare(label) == 0)
      return it.key();
  }
  return -1;
};

int Model::generateNewSegmentId()
{
  this->m_LastSegmentId += 1;
  return this->m_LastSegmentId;
};
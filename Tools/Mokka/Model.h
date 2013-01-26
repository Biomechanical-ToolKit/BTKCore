/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2013, Arnaud Barré
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

#ifndef Model_h
#define Model_h

#include "btkTriangleMesh.h"

#include <QObject>
#include <QString>
#include <QVector>
#include <QMap>
#include <QColor>

typedef btk::TriangleMesh::VertexLink Pair;
typedef btk::TriangleMesh::VertexFace Triad;

struct Segment
{
  Segment(const QString& l, const QString& d, const QColor& c, const QVector<int>& m, const QVector<Pair>& k, const QVector<Triad>& f, bool v = true, bool sv = false)
  : label(l), description(d), color(c), markerIds(m), links(k), faces(f), mesh(btk::TriangleMesh::New(m.toStdVector(), k.toStdVector(), f.toStdVector()))
  {this->visible = v, this->surfaceVisible = sv; this->isNew = true;};
  Segment(const Segment& toCopy)
  : label(toCopy.label), description(toCopy.description), color(toCopy.color), markerIds(toCopy.markerIds), links(toCopy.links), faces(toCopy.faces), mesh(toCopy.mesh)
  {this->visible = toCopy.visible; this->surfaceVisible = toCopy.surfaceVisible; this->isNew = toCopy.isNew;};
  QString label;
  QString description;
  QColor color;
  QVector<int> markerIds;
  QVector<Pair> links;
  QVector<Triad> faces;
  btk::TriangleMesh::Pointer mesh;
  bool visible;
  bool surfaceVisible;
  bool isNew;
};

class Model : public QObject
{
  Q_OBJECT
  
public:
  Model(QObject* parent = 0);
  ~Model();
  
  void clear();
  
  bool hasSegments() const {return !this->m_Segments.empty();};
  const QMap<int, Segment*>& segments() const {return this->m_Segments;};
  void setSegments(const QList<Segment>& segments);
  const QString& segmentLabel(int id) const {return this->m_Segments[id]->label;};
  void setSegmentLabel(int id, const QString& label);
  const QString& segmentDescription(int id) const {return this->m_Segments[id]->description;};
  void setSegmentsDescription(const QVector<int>& ids, const QVector<QString>& descs);
  const QColor& segmentColor(int id) const {return this->m_Segments[id]->color;};
  void setSegmentColor(int id, const QColor& color);
  void setSegmentsColor(const QVector<int>& ids, const QVector<QColor>& colors);
  const QVector<int>& segmentMarkerIds(int id) const {return this->m_Segments[id]->markerIds;};
  const QVector<Pair>& segmentLinks(int id) const {return this->m_Segments[id]->links;};
  const QVector<Triad>& segmentFaces(int id) const {return this->m_Segments[id]->faces;};
  void setSegmentDefinition(int id, const QVector<int>& markerIds, const QVector<Pair>& links, const QVector<Triad>& faces);
  bool segmentVisible(int id) const {return this->m_Segments[id]->visible;};
  void setSegmentsVisible(const QVector<int>& ids, const QVector<bool>& visibles);
  bool segmentSurfaceVisible(int id) const {return this->m_Segments[id]->surfaceVisible;};
  void setSegmentsSurfaceVisible(const QVector<int>& ids, const QVector<bool>& visibles);
  QList<Segment*> takeSegments(const QList<int>& ids);
  void insertSegments(const QList<int>& ids, const QList<Segment*> segments);
  int findSegmentIdFromLabel(const QString& label) const;
  const QColor& defaultSegmentColor() const {return this->m_DefaultSegmentColor;};
  void setDefaultSegmentColor(const QColor& color) {this->m_DefaultSegmentColor = color;};
  int generateNewSegmentId();
  
signals:
  void segmentsChanged(const QList<int>& ids, const QList<Segment*>& segments);
  void segmentLabelChanged(int id, const QString& label);
  void segmentsDescriptionChanged(const QVector<int>& ids, const QVector<QString>& descs);
  void segmentColorChanged(int id, const QColor& color);
  void segmentsColorChanged(const QVector<int>& ids, const QVector<QColor>& colors);
  void segmentDefinitionChanged(int id);
  void segmentsVisibilityChanged(const QVector<int>& ids, const QVector<bool>& visibles);
  void segmentsSurfaceVisibilityChanged(const QVector<int>& ids, const QVector<bool>& visibles);
  void segmentsRemoved(const QList<int>& ids, const QList<Segment*>& segments);
  void segmentsInserted(const QList<int>& ids, const QList<Segment*>& segments);
  
private:
  QMap<int,Segment*> m_Segments;
  int m_LastSegmentId;
  QColor m_DefaultSegmentColor;
};
#endif // Model_h
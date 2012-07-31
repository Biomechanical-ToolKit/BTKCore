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

#ifndef NewSegmentDialog_h
#define NewSegmentDialog_h

#include "ui_NewSegmentDialog.h"
#include "Model.h" // Pair

#include <QDialog>


class QTreeWidgetItem;

class NewSegmentDialog : public QDialog, public Ui::NewSegmentDialog
{
  Q_OBJECT

public:
  struct MarkerInfo
  {
    MarkerInfo(int i, const QString& l, bool isSelected) : label(l) {this->id = i; this->selected = isSelected;};
    int id;
    QString label;
    bool selected;
  };
  
  NewSegmentDialog(Segment* seg, int segmentId, const QList<MarkerInfo>& markersInfo, bool editMode = false, QWidget* parent = 0);
  // ~NewSegmentDialog();
  
public slots:
  void validate();
  
private slots:
  void circleSelectedMarkers();
  void updateMarkersUsed(QTableWidgetItem* item);
  void pickMarker(int id);
  void togglePickedMarker(int id);
  void pickSelectedMarkers(const QList<int>& ids);
  void removeEdge(QTableWidgetItem* item);
  void removeFace(QTableWidgetItem* item);
  void updateSegmentDefinition();

signals:
  void markerSelectionChanged(const QList<int>& ids);
  void markerVisibleSelectionChanged(const QList<int>& ids);
  void segmentDefinitionChanged(int id, const QVector<int>& markerIds, const QVector<Pair>& links);
  void segmentDefinitionChanged(int id, const QVector<int>& markerIds, const QVector<Pair>& links, const QVector<Triad>& faces);
  
protected:
  bool eventFilter(QObject* obj, QEvent* event);
  void keyPressEvent(QKeyEvent* e);
  
private:
  void updateMarkersSelection(const QList<int>& rows, const QList<int>& ids);
  void eraseEdge(QTableWidgetItem* item);
  void buildEdges(QList<QTableWidgetItem*>* markers = 0, QList<QTableWidgetItem*>* edges = 0);
  void buildFaces();
  
  Segment* mp_Segment;
  int m_SegmentId;
};

#endif // NewSegmentDialog_h
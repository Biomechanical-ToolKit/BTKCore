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
 
#include "NewSegmentDialog.h"
#include "Acquisition.h"
#include "ModelDockWidget.h"
#include "UserDefined.h"
#include "LoggerMessage.h"

#include <QTreeWidgetItem>
#include <QPushButton>
#include <QKeyEvent>

const int FaceId = Qt::UserRole + 100;
const int FaceList = Qt::UserRole + 101;
const int LinkList = Qt::UserRole + 102;
 
NewSegmentDialog::NewSegmentDialog(QWidget* parent)
: QDialog(parent)
{
  this->mp_Segment = 0;
  this->m_SegmentId = 0;
  
  this->setupUi(this);
  this->tabWidget->setCurrentWidget(this->linkTab);
#ifdef Q_OS_MAC
  this->markerInfoLabel->setAttribute(Qt::WA_MacSmallSize);
  this->linkInfoLabel->setAttribute(Qt::WA_MacSmallSize);
  this->faceInfoLabel->setAttribute(Qt::WA_MacSmallSize);
  this->noteLabel->setAttribute(Qt::WA_MacSmallSize);
  this->markersTable->setAttribute(Qt::WA_MacSmallSize);
  this->markersTable->setAttribute(Qt::WA_MacShowFocusRect, 0);
  this->linksTable->setAttribute(Qt::WA_MacSmallSize);
  this->linksTable->setAttribute(Qt::WA_MacShowFocusRect, 0);
  this->facesTable->setAttribute(Qt::WA_MacSmallSize);
  this->facesTable->setAttribute(Qt::WA_MacShowFocusRect, 0);
  this->setWindowFlags(Qt::Sheet);
  this->setWindowModality(Qt::WindowModal);
  this->setMinimumSize(QSize(this->width(), this->height()));
  this->setMaximumSize(QSize(this->width(), this->height()));
#endif
  QHeaderView* tableHeader = this->markersTable->horizontalHeader();
  tableHeader->setDefaultSectionSize(26);
  tableHeader->setResizeMode(0, QHeaderView::Fixed);
  tableHeader->setResizeMode(1, QHeaderView::Stretch);
  this->markersTable->verticalHeader()->setDefaultSectionSize(20);
  tableHeader = this->linksTable->horizontalHeader();
  tableHeader->setDefaultSectionSize(26);
  tableHeader->setResizeMode(0, QHeaderView::Fixed);
  tableHeader->setResizeMode(1, QHeaderView::Stretch);
  tableHeader->setResizeMode(2, QHeaderView::Stretch);
  this->linksTable->verticalHeader()->setDefaultSectionSize(20);
  tableHeader = this->facesTable->horizontalHeader();
  tableHeader->setDefaultSectionSize(26);
  tableHeader->setResizeMode(0, QHeaderView::Fixed);
  tableHeader->setResizeMode(1, QHeaderView::Stretch);
  tableHeader->setResizeMode(2, QHeaderView::Stretch);
  tableHeader->setResizeMode(3, QHeaderView::Stretch);
  this->facesTable->verticalHeader()->setDefaultSectionSize(20);
  this->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
  
  this->viz3D->initialize();
  
  this->markersTable->installEventFilter(this);
  this->linksTable->installEventFilter(this);
  this->facesTable->installEventFilter(this);
  this->viz3D->installEventFilter(this);
  
  // Connections
  connect(this->segmentLabelEdit, SIGNAL(textEdited(QString)), this, SLOT(validate()));
  connect(this->markersTable, SIGNAL(itemSelectionChanged()), this, SLOT(circleSelectedMarkers()));
  connect(this->markersTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(updateMarkersUsed(QTableWidgetItem*)));
  connect(this->linksTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(removeEdge(QTableWidgetItem*)));
  connect(this->linksTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(validate()));
  connect(this->facesTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(removeFace(QTableWidgetItem*)));
  connect(this->viz3D, SIGNAL(pickedMarkerChanged(int)), this, SLOT(pickMarker(int)));
  connect(this->viz3D, SIGNAL(pickedMarkerToggled(int)), this, SLOT(togglePickedMarker(int)));
  connect(this->viz3D, SIGNAL(selectedMarkersToggled(QList<int>)), this, SLOT(pickSelectedMarkers(QList<int>)));
};

void NewSegmentDialog::initialize(Segment* seg, int segmentId, const QList<MarkerInfo>& markersInfo, bool editMode)
{
  this->segmentLabelEdit->blockSignals(true);
  this->markersTable->blockSignals(true);
  this->linksTable->blockSignals(true);
  this->facesTable->blockSignals(true);
  
  this->mp_Segment = seg;
  this->m_SegmentId = segmentId;
  
  if (editMode)
  {
    this->segmentLabelLabel->setVisible(false);
    this->segmentLabelEdit->setVisible(false);
    this->segmentDescriptionLabel->setVisible(false);
    this->segmentDescriptionEdit->setVisible(false);
    this->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    this->buttonBox->button(QDialogButtonBox::Ok)->setDefault(true);
  }
  
  this->markersTable->setRowCount(0);
  this->linksTable->setRowCount(0);
  this->facesTable->setRowCount(0);
  // Fill the tables
  // - To create a new segment
  if (!editMode)
  {
    int markerRowIdx = 0;
    for (QList<MarkerInfo>::const_iterator it = markersInfo.begin() ; it != markersInfo.end() ; ++it)
    {
      // Markers
      this->markersTable->insertRow(markerRowIdx);
      QTableWidgetItem* check = new QTableWidgetItem;
      this->markersTable->setItem(markerRowIdx, 0, check);
      QTableWidgetItem* label = new QTableWidgetItem(it->label);
      label->setData(PointId, it->id);
      this->markersTable->setItem(markerRowIdx, 1, label);
      if (it->selected)
      {
        check->setCheckState(Qt::Checked);
        check->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        label->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
      }
      else
      {
        check->setCheckState(Qt::Unchecked);
        check->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        check->setFlags(check->flags() & ~Qt::ItemIsSelectable);
        label->setFlags(Qt::NoItemFlags);
      }
      ++markerRowIdx;
      
      // Links
      int linkRowIdx = this->linksTable->rowCount();
      QList<MarkerInfo>::const_iterator it2 = it; ++it2;
      for ( ; it2 != markersInfo.end() ; ++it2)
      {
        this->linksTable->insertRow(linkRowIdx);
        QTableWidgetItem* check = new QTableWidgetItem;
        check->setCheckState(Qt::Unchecked);
        check->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        QTableWidgetItem* mkr1 = new QTableWidgetItem(it->label);
        mkr1->setFlags(Qt::ItemIsEnabled);
        mkr1->setData(PointId, it->id);
        QTableWidgetItem* mkr2 = new QTableWidgetItem(it2->label);
        mkr2->setFlags(Qt::ItemIsEnabled);
        mkr2->setData(PointId, it2->id);
        this->linksTable->setItem(linkRowIdx, 0, check);
        this->linksTable->setItem(linkRowIdx, 1, mkr1);
        this->linksTable->setItem(linkRowIdx, 2, mkr2);
        if (!it->selected || !it2->selected)
          this->linksTable->setRowHidden(linkRowIdx, true);
        ++linkRowIdx;
      }
    }
  }
  // To edit an existing segment
  else
  {
    this->segmentLabelEdit->setText("Edition Mode"); // For the validate() method
    int markerRowIdx = 0, checkedMarkers = 0;
    for (QList<MarkerInfo>::const_iterator it = markersInfo.begin() ; it != markersInfo.end() ; ++it)
    {
      // Markers
      this->markersTable->insertRow(markerRowIdx);
      QTableWidgetItem* check = new QTableWidgetItem;
      this->markersTable->setItem(markerRowIdx, 0, check);
      QTableWidgetItem* label = new QTableWidgetItem(it->label);
      label->setData(PointId, it->id);
      this->markersTable->setItem(markerRowIdx, 1, label);
      if (seg->markerIds.indexOf(it->id) != -1)
      {
        check->setCheckState(Qt::Checked);
        check->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        label->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        ++checkedMarkers;
      }
      else
      {
        check->setCheckState(Qt::Unchecked);
        check->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        check->setFlags(check->flags() & ~Qt::ItemIsSelectable);
        label->setFlags(Qt::NoItemFlags);
      }
      ++markerRowIdx;
      
      // Links
      int linkRowIdx = this->linksTable->rowCount();
      QList<MarkerInfo>::const_iterator it2 = it; ++it2;
      for ( ; it2 != markersInfo.end() ; ++it2)
      {
        this->linksTable->insertRow(linkRowIdx);
        QTableWidgetItem* check = new QTableWidgetItem;
        check->setCheckState(Qt::Unchecked);
        check->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        QTableWidgetItem* mkr1 = new QTableWidgetItem(it->label);
        mkr1->setFlags(Qt::ItemIsEnabled);
        mkr1->setData(PointId, it->id);
        QTableWidgetItem* mkr2 = new QTableWidgetItem(it2->label);
        mkr2->setFlags(Qt::ItemIsEnabled);
        mkr2->setData(PointId, it2->id);
        this->linksTable->setItem(linkRowIdx, 0, check);
        this->linksTable->setItem(linkRowIdx, 1, mkr1);
        this->linksTable->setItem(linkRowIdx, 2, mkr2);
        if ((this->mp_Segment->markerIds.indexOf(it->id) == -1) || (this->mp_Segment->markerIds.indexOf(it2->id) == -1))
          this->linksTable->setRowHidden(linkRowIdx, true);
        else
        {
          for (int k = 0 ; k < this->mp_Segment->links.size() ; ++k)
          {
            if ((it->id == this->mp_Segment->links[k].GetIds()[0]) && (it2->id == this->mp_Segment->links[k].GetIds()[1])
              || (it->id == this->mp_Segment->links[k].GetIds()[1]) && (it2->id == this->mp_Segment->links[k].GetIds()[0]))
            {
              check->setCheckState(Qt::Checked);
              break;
            }
          }
        }
        ++linkRowIdx;
      }
    }
    if (checkedMarkers != this->mp_Segment->markerIds.size())
      LOG_WARNING("At least one marker of the definition of the segment is missing.");
    
    // Existing faces
    for (int i = 0 ; i != this->mp_Segment->faces.size() ; ++i)
    {
      const int* ids = this->mp_Segment->faces[i].GetIds();
      QVector<int> markerIds(3);
      QTableWidgetItem* markers[3] = {0,0,0};
      for (int j = 0 ; j < 3 ; ++j)
      {
        for (int k = 0 ; k < this->markersTable->rowCount() ; ++k)
        {
          QTableWidgetItem* item = this->markersTable->item(k,1);
          if (item->data(PointId).toInt() == ids[j])
          {
            markers[j] = item;
            markerIds[j] = ids[j];
            break;
          }
        }
        // Missing markers
        if (markers[j] == 0)
          continue;
      }
      
      qSort(markerIds.begin(),markerIds.end());
      QList<QVariant> linkList;
      for (int j = 0 ; j < markerIds.size() ; ++j)
      {
        for (int k = j+1 ; k < markerIds.size() ; ++k)
        {
          for (int l = 0 ; l < this->linksTable->rowCount() ; ++l)
          {
            int id1 = this->linksTable->item(l,1)->data(PointId).toInt();
            int id2 = this->linksTable->item(l,2)->data(PointId).toInt();
            if ((markerIds[j] == id1) && (markerIds[k] == id2))
            {
              QList<QVariant> faceList = this->linksTable->item(l,0)->data(FaceList).toList();
              if (faceList.indexOf(i) == -1)
                faceList.push_back(i);
              this->linksTable->item(l,0)->setData(FaceList, faceList);
              linkList.push_back(l);
              break;
            }
          }
        }
      }
      
      this->facesTable->insertRow(i);
      QTableWidgetItem* check = new QTableWidgetItem;
      check->setCheckState(Qt::Checked);
      check->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
      check->setData(FaceId, i);
      check->setData(LinkList, linkList);
      QTableWidgetItem* mkr1 = new QTableWidgetItem(markers[0]->text());
      mkr1->setFlags(Qt::ItemIsEnabled);
      mkr1->setData(PointId, ids[0]);
      QTableWidgetItem* mkr2 = new QTableWidgetItem(markers[1]->text());
      mkr2->setFlags(Qt::ItemIsEnabled);
      mkr2->setData(PointId, ids[1]);
      QTableWidgetItem* mkr3 = new QTableWidgetItem(markers[2]->text());
      mkr3->setFlags(Qt::ItemIsEnabled);
      mkr3->setData(PointId, ids[2]);
      this->facesTable->setItem(i, 0, check);
      this->facesTable->setItem(i, 1, mkr1);
      this->facesTable->setItem(i, 2, mkr2);
      this->facesTable->setItem(i, 3, mkr3);
    }
  }
  
  this->segmentLabelEdit->blockSignals(false);
  this->markersTable->blockSignals(false);
  this->linksTable->blockSignals(false);
  this->facesTable->blockSignals(false);
};

void NewSegmentDialog::validate()
{
  if (this->segmentLabelEdit->text().isEmpty())
    this->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
  else
  {
    bool unchecked = true;
    for (int i = 0 ; i < this->linksTable->rowCount() ; ++i)
    {
      if (this->linksTable->item(i,0)->checkState() == Qt::Checked)
      {
        unchecked = false;
        break;
      }
    }
    if (unchecked)
      this->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    else
    {
      this->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
      this->buttonBox->button(QDialogButtonBox::Ok)->setDefault(true);
    }
  }
};

void NewSegmentDialog::circleSelectedMarkers()
{
  QList<int> rows, ids;
  QList<QTableWidgetItem*> items = this->markersTable->selectedItems();
  for (QList<QTableWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
  {
    if ((*it)->column() != 1)
      continue;
    rows.push_back((*it)->row());
    ids.push_back((*it)->data(PointId).toInt());
  }
  // Keep only the three last markers
  while (rows.size() > 3)
  {
    rows.pop_front();
    ids.pop_front();
  }
  this->updateMarkersSelection(rows, ids);
  this->viz3D->render();
};

void NewSegmentDialog::updateMarkersUsed(QTableWidgetItem* item)
{
  QList<int> ids;
  for (int i = 0 ; i < this->markersTable->rowCount() ; ++i)
  {
    if (this->markersTable->item(i,0)->checkState() == Qt::Checked)
      ids.push_back(this->markersTable->item(i,1)->data(PointId).toInt());
  }
  
  this->markersTable->blockSignals(true);
  this->linksTable->blockSignals(true);
  this->facesTable->blockSignals(true);
  QTableWidgetItem* label = this->markersTable->item(item->row(), 1);
  int markerId = label->data(PointId).toInt();
  if (item->checkState() == Qt::Unchecked)
  {
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
    label->setFlags(Qt::NoItemFlags);
    for (int i = 0 ; i < this->linksTable->rowCount() ; ++i)
    {
      if ((this->linksTable->item(i,1)->data(PointId).toInt() == markerId)
          || (this->linksTable->item(i,2)->data(PointId).toInt() == markerId))
      {
        QTableWidgetItem* edgeItem = this->linksTable->item(i,0);
        edgeItem->setCheckState(Qt::Unchecked);
        this->eraseEdge(edgeItem);
        this->linksTable->setRowHidden(i,true);
      }
    }
  }
  else
  {
    item->setFlags(item->flags() | Qt::ItemIsSelectable);
    QTableWidgetItem* label = this->markersTable->item(item->row(), 1);
    label->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    this->updateMarkersSelection(QList<int>() << markerId, QList<int>() << item->row());
    
    int markerId = this->markersTable->item(item->row(),1)->data(PointId).toInt();
    for (int i = 0 ; i < this->linksTable->rowCount() ; ++i)
    {
      QTableWidgetItem* other = 0;
      if (this->linksTable->item(i,1)->data(PointId).toInt() == markerId)
        other = this->linksTable->item(i,2);
      else if (this->linksTable->item(i,2)->data(PointId).toInt() == markerId)
        other = this->linksTable->item(i,1);
      if (other != 0)
      {
        int otherId = other->data(PointId).toInt();
        for (int j = 0 ; j < this->markersTable->rowCount() ; ++j)
        {
          if (this->markersTable->item(j,1)->data(PointId).toInt() == otherId)
          {
            if (this->markersTable->item(j,0)->checkState() == Qt::Checked)
              this->linksTable->setRowHidden(i,false);
            break;
          }
        }
      }
    }
  }
  this->facesTable->blockSignals(false);
  this->linksTable->blockSignals(false);
  this->markersTable->blockSignals(false);
  
  this->updateSegmentDefinition();
  emit markerVisibleSelectionChanged(ids);
  this->viz3D->renderer()->ResetCamera(); // To center the camera around the visible markers.
  this->viz3D->render();
};

void NewSegmentDialog::removeEdge(QTableWidgetItem* item)
{
  this->facesTable->blockSignals(true);
  this->linksTable->blockSignals(true);
  this->eraseEdge(item);
  this->linksTable->blockSignals(false);
  this->facesTable->blockSignals(false);
  this->updateSegmentDefinition();
  this->viz3D->render();
};

void NewSegmentDialog::removeFace(QTableWidgetItem* item)
{
  int faceId = item->data(FaceId).toInt();
  QList<QVariant> links = item->data(LinkList).toList();
  
  this->linksTable->blockSignals(true);
  for (int i = 0 ; i < links.size() ; ++i)
  {
    int pos = links[i].toInt();
    QList<QVariant> faces = this->linksTable->item(pos,0)->data(FaceList).toList();
    faces.removeOne(faceId);
    this->linksTable->item(pos,0)->setData(FaceList, faces);
    if (faces.isEmpty())
      this->linksTable->item(pos,0)->setCheckState(Qt::Unchecked);
  }
  this->linksTable->blockSignals(false);
  
  this->facesTable->blockSignals(true);
  this->facesTable->removeRow(item->row());
  this->facesTable->blockSignals(false);
  this->updateSegmentDefinition();
  this->viz3D->render();
};

void NewSegmentDialog::updateSegmentDefinition()
{
  QList<int> markerIds;
  for (int i = 0 ; i < this->markersTable->rowCount() ; ++i)
  {
    if (this->markersTable->item(i,0)->checkState() == Qt::Checked)
      markerIds.push_back(this->markersTable->item(i,1)->data(PointId).toInt());
  }
  
  QList<Pair> links;
  for (int i = 0 ; i < this->linksTable->rowCount() ; ++i)
  {
    if ((this->linksTable->item(i,0)->checkState() == Qt::Checked) && (!this->linksTable->isRowHidden(i)))
      links.push_back(Pair(this->linksTable->item(i,1)->data(PointId).toInt(), this->linksTable->item(i,2)->data(PointId).toInt()));
  }
  
  QVector<Triad> faces(this->facesTable->rowCount());
  for (int i = 0 ; i < this->facesTable->rowCount() ; ++i)
  {
    faces[i].SetIds(this->facesTable->item(i,1)->data(PointId).toInt(),
                    this->facesTable->item(i,2)->data(PointId).toInt(),
                    this->facesTable->item(i,3)->data(PointId).toInt());
  }
  
  this->mp_Segment->markerIds = markerIds.toVector();
  this->mp_Segment->links = links.toVector();
  this->mp_Segment->faces = faces;
  emit segmentDefinitionChanged(this->m_SegmentId, this->mp_Segment->markerIds, this->mp_Segment->links, this->mp_Segment->faces);
};

bool NewSegmentDialog::eventFilter(QObject* obj, QEvent* event)
{
  if (event->type() == QEvent::KeyPress)
  {
    QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
    this->keyPressEvent(keyEvent);
    return false;
  }
  return QObject::eventFilter(obj, event);
};

void NewSegmentDialog::keyPressEvent(QKeyEvent* e)
{
  if (e->key() == Qt::Key_Escape)
  {
    this->updateMarkersSelection(QList<int>(), QList<int>());
    this->viz3D->render();
  }
  else if (e->key() == Qt::Key_E)
  {
    if (this->markersTable->selectedItems().count()/2 < 2)
      return;
    this->buildEdges();
    this->updateSegmentDefinition();
    this->viz3D->render();
    this->validate();
  }
  else if (e->key() == Qt::Key_F)
  {
    if (this->markersTable->selectedItems().count()/2 < 3)
      return;
    this->buildFaces();
    this->updateSegmentDefinition();
    this->viz3D->render();
    this->validate();
  }
  else
    QDialog::keyPressEvent(e);
};

void NewSegmentDialog::pickMarker(int id)
{
  QList<int> ids, rows;
  ids.push_back(id);
  for (int i = 0 ; i < this->markersTable->rowCount() ; ++i)
  {
    if (this->markersTable->item(i,1)->data(PointId).toInt() == id)
    {
      rows.push_back(i);
      break;
    }
  }
  this->updateMarkersSelection(rows, ids);
};

void NewSegmentDialog::togglePickedMarker(int id)
{
  this->pickSelectedMarkers(QList<int>() << id);
};

void NewSegmentDialog::pickSelectedMarkers(const QList<int>& ids)
{
  // Extract the rows already selected
  QList<int> rows;
  QList<int> pointIds;
  QList<QTableWidgetItem*> items = this->markersTable->selectedItems();
  for (QList<QTableWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
  {
    if ((*it)->column() != 1)
      continue;
    rows.push_back((*it)->row());
    pointIds.push_back((*it)->data(PointId).toInt());
  }
  // Find the row corresponding to the picked markers
  for (int i = 0 ; i < ids.size() ; ++i)
  {
    for (int j = 0 ; j < this->markersTable->rowCount() ; ++j)
    {
      if (this->markersTable->item(j,1)->data(PointId).toInt() == ids[i])
      {
        // Already inside: so the markers was unpicked
        int idx = rows.indexOf(j);
        if (idx != -1)
        {
          rows.removeAt(idx);
          pointIds.removeAt(idx);
        }
        else
        {
          rows.push_back(j);
          pointIds.push_back(ids[i]);
        }
        break;
      }
    }
  }
  // Keep only the three last markers
  while (rows.size() > 3)
  {
    rows.pop_front();
    pointIds.pop_front();
  }
  
  this->updateMarkersSelection(rows, pointIds);
};

void NewSegmentDialog::updateMarkersSelection(const QList<int>& rows, const QList<int>& ids)
{
  this->markersTable->blockSignals(true);
  // New selection of markers
  this->markersTable->clearSelection();
  QItemSelectionModel* selection = this->markersTable->selectionModel();
  for (int i = 0 ; i < rows.size() ; ++i)
    selection->select(this->markersTable->model()->index(rows[i],1), QItemSelectionModel::Select | QItemSelectionModel::Rows);
  this->markersTable->blockSignals(false);
  emit markerSelectionChanged(ids);
};

void NewSegmentDialog::eraseEdge(QTableWidgetItem* item)
{
  QList<QVariant> faces = item->data(FaceList).toList();
  for (int i = 0 ; i < faces.size() ; ++i)
  {
    int faceId = faces[i].toInt();
    QTableWidgetItem* faceItem = 0;
    for (int j = 0 ; j < this->facesTable->rowCount() ; ++j)
    {
      QTableWidgetItem* _item = this->facesTable->item(j,0);
      if (_item->data(FaceId).toInt() == faceId)
      {
        faceItem = _item;
        break;
      }
    } 
    if (faceItem == 0)
      continue;
    QList<QVariant> links = faceItem->data(LinkList).toList();
    for (int j = 0 ; j < links.size() ; ++j)
    {
      QTableWidgetItem* edgeItem = this->linksTable->item(links[j].toInt(),0);
      if (edgeItem == 0)
        continue;
      QList<QVariant> faces2 = edgeItem->data(FaceList).toList();
      faces2.removeOne(faceId);
      if (faces2.indexOf(-1) != -1)
        faces2.push_back(-1);
      edgeItem->setData(FaceList, faces2);
    }
    this->facesTable->removeRow(faceItem->row());
  }
  item->setData(FaceList, QList<QVariant>());
};

void NewSegmentDialog::buildEdges(QList<QTableWidgetItem*>* markers, QList<QTableWidgetItem*>* edges)
{
  QList<int> pointIds;
  QList<QTableWidgetItem*> m, e;
  QList<QTableWidgetItem*> items = this->markersTable->selectedItems();
  for (QList<QTableWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
  {
    if ((*it)->column() != 1)
      continue;
    pointIds.push_back((*it)->data(PointId).toInt());
    m.push_back(*it);
  }
  
  qSort(pointIds.begin(), pointIds.end());
  
  this->linksTable->blockSignals(true);
  for (int i = 0 ; i < pointIds.size() ; ++i)
  {
    for (int j = i+1 ; j < pointIds.size() ; ++j)
    {
      for (int k = 0 ; k < this->linksTable->rowCount() ; ++k)
      {
        int id1 = this->linksTable->item(k,1)->data(PointId).toInt();
        int id2 = this->linksTable->item(k,2)->data(PointId).toInt();
        if ((pointIds[i] == id1) && (pointIds[j] == id2))
        {
          e.push_back(this->linksTable->item(k,0));
          this->linksTable->item(k,0)->setCheckState(Qt::Checked);
          break;
        }
      }
    }
  }
  this->linksTable->blockSignals(false);
  
  if (markers != 0)
    *markers = m;
  if (edges != 0)
    *edges = e;
};

void NewSegmentDialog::buildFaces()
{
  // First check if this face is not aready created
  QVector<int> ptIds(3);
  QList<QTableWidgetItem*> items = this->markersTable->selectedItems();
  int inc = 0;
  for (QList<QTableWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
  {
    if ((*it)->column() != 1)
      continue;
    ptIds[inc++] = (*it)->data(PointId).toInt();
  }
  for (int i = 0 ; i < this->facesTable->rowCount() ; ++i)
  {
    int id1 = this->facesTable->item(i,1)->data(PointId).toInt();
    int id2 = this->facesTable->item(i,2)->data(PointId).toInt();
    int id3 = this->facesTable->item(i,3)->data(PointId).toInt();
    if (((ptIds[0] == id1) || (ptIds[0] == id2) || (ptIds[0] == id3))
      && ((ptIds[1] == id1) || (ptIds[1] == id2) || (ptIds[1] == id3))
      && ((ptIds[2] == id1) || (ptIds[2] == id2) || (ptIds[2] == id3)))
      return;
  }

  QList<QTableWidgetItem*> edges;
  QList<QTableWidgetItem*> markers;
  this->buildEdges(&markers,&edges);
  
  int faceRowIdx = this->facesTable->rowCount();
  QList<QVariant> linkList;
  this->linksTable->blockSignals(true);
  for (int i = 0 ; i < edges.size() ; ++i)
  {
    QList<QVariant> faceList = edges[i]->data(FaceList).toList();
    if (faceList.indexOf(faceRowIdx) == -1)
      faceList.push_back(faceRowIdx);
    edges[i]->setData(FaceList, faceList);
    linkList.push_back(edges[i]->row());
  }
  this->linksTable->blockSignals(false);
  
  this->facesTable->blockSignals(true);
  this->facesTable->insertRow(faceRowIdx);
  QTableWidgetItem* check = new QTableWidgetItem;
  check->setCheckState(Qt::Checked);
  check->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
  check->setData(FaceId, faceRowIdx);
  check->setData(LinkList, linkList);
  QTableWidgetItem* mkr1 = new QTableWidgetItem(markers[0]->text());
  mkr1->setFlags(Qt::ItemIsEnabled);
  mkr1->setData(PointId, markers[0]->data(PointId).toInt());
  QTableWidgetItem* mkr2 = new QTableWidgetItem(markers[1]->text());
  mkr2->setFlags(Qt::ItemIsEnabled);
  mkr2->setData(PointId, markers[1]->data(PointId).toInt());
  QTableWidgetItem* mkr3 = new QTableWidgetItem(markers[2]->text());
  mkr3->setFlags(Qt::ItemIsEnabled);
  mkr3->setData(PointId, markers[2]->data(PointId).toInt());
  this->facesTable->setItem(faceRowIdx, 0, check);
  this->facesTable->setItem(faceRowIdx, 1, mkr1);
  this->facesTable->setItem(faceRowIdx, 2, mkr2);
  this->facesTable->setItem(faceRowIdx, 3, mkr3);
  this->facesTable->blockSignals(false);
};

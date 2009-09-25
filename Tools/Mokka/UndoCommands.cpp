/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009, Arnaud Barré
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

// --------------- EditMarkerLabel ---------------
EditMarkerLabel::EditMarkerLabel(const QString& label, QTableWidgetItem* item, QUndoCommand* parent)
: m_Label(label)
{
  this->mp_Item = item;
};

void EditMarkerLabel::action()
{
  QString temp = this->mp_Item->text();
  this->mp_Item->setData(pointLabel, this->m_Label);
  this->mp_Item->setText(this->m_Label);
  this->m_Label = temp;
  this->mp_Item->tableWidget()->blockSignals(true);
  this->mp_Item->tableWidget()->clearSelection();
  this->mp_Item->tableWidget()->blockSignals(false);
  this->mp_Item->tableWidget()->setCurrentItem(this->mp_Item);
};

// --------------- EditMarkerDescription ---------------
EditMarkerDescription::EditMarkerDescription(const QString& desc, QTableWidgetItem* item, QUndoCommand* parent)
: m_Description(desc)
{
  this->mp_Item = item;
};

void EditMarkerDescription::action()
{
  QString temp = this->mp_Item->data(pointDescription).toString();
  this->mp_Item->setData(pointDescription, this->m_Description);
  this->m_Description = temp;
  this->mp_Item->tableWidget()->blockSignals(true);
  this->mp_Item->tableWidget()->clearSelection();
  this->mp_Item->tableWidget()->blockSignals(false);
  this->mp_Item->tableWidget()->setCurrentItem(this->mp_Item);
};

// --------------- EditMarkersRadius ---------------
EditMarkersRadius::EditMarkersRadius(double r, QList<QTableWidgetItem*> items, MainWindow* w, QUndoCommand* parent)
: m_Items(items), m_Radius(QVector<double>(items.count(), r))
{
  this->mp_Main = w;
};

void EditMarkersRadius::action()
{
  this->mp_Main->markersTable->blockSignals(true);
  this->mp_Main->markersTable->clearSelection();
  int i = 0;
  for (QList<QTableWidgetItem*>::iterator it = this->m_Items.begin() ; it != this->m_Items.end() ; ++it)
  {
    int id = (*it)->data(markerId).toInt();
    double r = (*it)->data(markerRadius).toDouble();
    this->mp_Main->setVTKMarkerRadius(id, this->m_Radius[i]);
    (*it)->setData(markerRadius, this->m_Radius[i]);
    this->mp_Main->markersTable->setCurrentItem(*it, QItemSelectionModel::Select);
    this->m_Radius[i++] = r;
  }
  this->mp_Main->markersTable->blockSignals(false);
  this->mp_Main->displayMarkerProperties();
  this->mp_Main->circleSelectedMarkers();
};

// --------------- EditMarkersColorIndex ---------------
EditMarkersColorIndex::EditMarkersColorIndex(int idx, QList<QTableWidgetItem*> items, MainWindow* w, QUndoCommand* parent)
: m_Items(items), m_Indexes(QVector<int>(items.count(), idx))
{
  this->mp_Main = w;
};

void EditMarkersColorIndex::action()
{
  this->mp_Main->markersTable->blockSignals(true);
  this->mp_Main->markersTable->clearSelection();
  int i = 0;
  for (QList<QTableWidgetItem*>::iterator it = this->m_Items.begin() ; it != this->m_Items.end() ; ++it)
  {
    int id = (*it)->data(markerId).toInt();
    int idx = (*it)->data(markerColorIndex).toInt();
    this->mp_Main->setVTKMarkerColorIndex(id, this->m_Indexes[i]);
    (*it)->setData(markerColorIndex, this->m_Indexes[i]);
    this->mp_Main->markersTable->setCurrentItem(*it, QItemSelectionModel::Select);
    this->m_Indexes[i++] = idx;
  }
  this->mp_Main->markersTable->blockSignals(false);
  this->mp_Main->displayMarkerProperties();
  this->mp_Main->circleSelectedMarkers();
};

// --------------- EditPoints ---------------
EditPoints::EditPoints(MainWindow* w, QUndoCommand* parent)
: m_Items(QVector< pointProp >(w->markersTable->rowCount()))
{
  this->mp_Main = w;
  int num = this->mp_Main->mp_PointsEditorDlg->tableWidget->rowCount();
  for (int i = 0 ; i < num ; ++i)
  {
    QTableWidgetItem* source = this->mp_Main->mp_PointsEditorDlg->tableWidget->item(i,0);
    this->m_Items[i].id = source->data(pointId).toInt();
    this->m_Items[i].disabled = ((source->checkState() == Qt::Unchecked) ? true : false);
    this->m_Items[i].label = source->text();
    this->m_Items[i].description = this->mp_Main->mp_PointsEditorDlg->tableWidget->item(i,1)->text();
  }
};

void EditPoints::action()
{
  int num = this->m_Items.count();
  QVector< pointProp > items = QVector< pointProp >(num);
  for (int i = 0 ; i < num ; ++i)
  {
    QTableWidgetItem* item = this->mp_Main->markersTable->item(i,0);
    //items[i].first = this->mp_Main->markersTable->item(i,0)->data(pointId).toInt();
    //items[i].second = this->mp_Main->markersTable->item(i,0)->data(pointDisabled).toBool();
    items[i].id = item->data(pointId).toInt();
    items[i].disabled = item->data(pointDisabled).toBool();
    items[i].label = item->text();
    items[i].description = item->data(pointDescription).toString();
  }
  this->mp_Main->markersTable->blockSignals(true);
  this->mp_Main->markersTable->clearSelection();
  for (int i = 0 ; i < num ; ++i)
  {
    for (int j = i ; j < num ; ++j)
    {
      QTableWidgetItem* item = this->mp_Main->markersTable->item(j,0);
      int id = item->data(pointId).toInt();
      if (this->m_Items[i].id == id)
      {
        if (j != i)
        {
          item = this->mp_Main->markersTable->takeItem(j, 0);
          this->mp_Main->markersTable->insertRow(i);
          this->mp_Main->markersTable->setItem(i, 0, item);
          this->mp_Main->markersTable->removeRow(j+1);
        }
        if ((item->data(pointType).toInt() == btk::Point::Marker))
        {
          this->mp_Main->setVTKMarkerVisibility(item->data(markerId).toInt(), !(this->m_Items[i].disabled));
          this->mp_Main->markersTable->setRowHidden(i, this->m_Items[i].disabled);
        }
        else
          this->mp_Main->markersTable->setRowHidden(i, true);
        item->setData(pointDisabled, this->m_Items[i].disabled);
        item->setCheckState(!(this->m_Items[i].disabled) ? Qt::Checked : Qt::Unchecked);
        item->setText(this->m_Items[i].label);
        item->setData(pointLabel, this->m_Items[i].label);
        item->setData(pointDescription, this->m_Items[i].description);
        break;
      }
    }
  }
  this->m_Items = items;
  this->mp_Main->markersTable->blockSignals(false);
  this->mp_Main->displayMarkerProperties();
  this->mp_Main->circleSelectedMarkers();
};
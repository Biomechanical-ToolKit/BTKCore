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

#include "MetadataView.h"
#include "ComboBoxDelegate.h"

MetadataView::MetadataView(QWidget* parent)
: QTreeView(parent)
{
  // Model
  this->mp_Model = new QStandardItemModel();
  this->setModel(this->mp_Model);
  this->mp_Model->setColumnCount(7);
  // TreeView
  this->mp_Model->setHorizontalHeaderLabels(QStringList() << "Labels" << "" << "Format" << "Dimensions" << "" << "Values" << "Description");
  QStandardItem* lockHeader = new QStandardItem(QIcon(QString::fromUtf8(":/images/lock.png")), "");
  this->mp_Model->setHorizontalHeaderItem(1, lockHeader);
  QHeaderView* header = this->header();
  header->setMovable(false);
  header->resizeSection(1, 25);
  header->setResizeMode (1, QHeaderView::Fixed);
  header->resizeSection(4, 16);
  header->setResizeMode (4, QHeaderView::Fixed);
  // Delegate
  ComboBoxDelegate* format = new ComboBoxDelegate(this);
  format->setList(QStringList() << "" << "Char" << "Byte" << "Integer" << "Real");
  this->setItemDelegateForColumn(2, format);
  // Connection
  connect(this->mp_Model, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(dispatchChangement(QStandardItem*)));
  
  this->setEditTriggers(QAbstractItemView::NoEditTriggers);
};

void MetadataView::dispatchChangement(QStandardItem* item)
{
  switch (item->index().column())
  {
  case 0: // Label
    break;
  case 1: // Unlock
    break;
  case 2: // Format
    break;
  case 3: // Dimensions
    break;
  case 4: // Expander
    this->expandItem(item);
    break;
  case 5: // Values
    break;
  case 6: // Description
    break;
  }
};

void MetadataView::expandItem(QStandardItem* item)
{
  this->mp_Model->blockSignals(true);
  int state = item->data(checkState2).toInt();
  QModelIndex index = this->mp_Model->indexFromItem(item);
  QStandardItem* parent = item->parent();
  QStandardItem* values = parent->child(index.row(), index.column() + 1);
  int count = values->data(metadataInfoValuesCount).toInt();
  QFont f = values->font();
  if (state == 0) // collapsed
  {
    f.setItalic(true);
    values->setText("<multiple lines>");
    for (int i = 1 ; i < count ; ++i)
      this->setRowHidden(index.row() + i, parent->index(), true);
  }
  else
  {
    f.setItalic(false);
    values->setText(values->data(metadataInfoFirstValue).toString());
    for (int i = 1 ; i < count ; ++i)
      this->setRowHidden(index.row() + i, parent->index(), false);
  }
  values->setFont(f);
  this->mp_Model->blockSignals(false);
};

void MetadataView::constructView(btk::MetaData::Pointer m, QStandardItem* parent)
{
  int idx = 0;
  for (btk::MetaData::ConstIterator it = m->Begin() ; it != m->End() ; ++it)
  {
    // Description
    parent->setChild(idx, 6, new QStandardItem(QString::fromStdString((*it)->GetDescription())));
    // Label
    parent->setChild(idx, 0, new QStandardItem(QString::fromStdString((*it)->GetLabel())));
    // Lock
    QStandardItem* lock = new QStandardItem(); 
    parent->setChild(idx, 1, lock);
    this->setIndexWidget(lock->index(), new Lock((*it)->GetUnlockState()));
    if ((*it)->HasInfo())
    {
      btk::MetaDataInfo::Pointer info = (*it)->GetInfo();
      // Format
      parent->setChild(idx, 2, new QStandardItem(QString::fromStdString((*it)->GetInfo()->GetFormatAsString())));
      // Dimensions
      parent->setChild(idx, 3, new DimensionsItem((*it)->GetInfo()));
      // Values
      QStandardItem* expand = new QStandardItem();
      parent->setChild(idx, 4, expand);
      Expander* e = new Expander((*it)->GetInfo(), expand);
      this->setIndexWidget(expand->index(), e);
      parent->setChild(idx, 5, new ValuesItem(info));
      switch(info->GetFormat())
      {
        case btk::MetaDataInfo::Char:
          for (int i = 1 ; i < static_cast<int>(info->GetValues().size()) ; ++i)
          {
            parent->setChild(++idx, 5, new QStandardItem(QString::fromStdString(info->ToString(i))));
            this->setRowHidden(idx, parent->index(), true);
          }
          break;
        case btk::MetaDataInfo::Byte:
          for (int i = 1 ; i < static_cast<int>(info->GetValues().size()) ; ++i)
          {
            parent->setChild(++idx, 5, new QStandardItem(QString::number(info->ToInt(i))));
            this->setRowHidden(idx, parent->index(), true);
          }
          break;
        case btk::MetaDataInfo::Integer:
          for (int i = 1 ; i < static_cast<int>(info->GetValues().size()) ; ++i)
          {
            parent->setChild(++idx, 5, new QStandardItem(QString::number(info->ToInt(i))));
            this->setRowHidden(idx, parent->index(), true);
          }
          break;
        case btk::MetaDataInfo::Real:
          for (int i = 1 ; i < static_cast<int>(info->GetValues().size()) ; ++i)
          {
            parent->setChild(++idx, 5, new QStandardItem(QString::number(info->ToDouble(i))));
            this->setRowHidden(idx, parent->index(), true);
          }
          break;
      }
    }
    this->constructView((*it), parent->child(idx));
    ++idx;
  }
}

void MetadataView::load(btk::MetaData::Pointer m)
{
  this->mp_Model->blockSignals(true);
  this->mp_Model->setRowCount(0);
  QStandardItem* root = this->mp_Model->invisibleRootItem();
  this->constructView(m, root);
  this->setColumnWidth(0, 200);
  this->setColumnWidth(5, 200);
  this->setColumnWidth(6, 200);
  this->mp_Model->blockSignals(false);
};

void MetadataView::reset()
{
  this->mp_Model->blockSignals(true);
  this->mp_Model->setRowCount(0);
  this->mp_Model->blockSignals(false);
};

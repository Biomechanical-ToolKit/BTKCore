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

#include "Preferences_Layouts.h"

#include <QHeaderView>
#include <QPushButton>
#include <QSettings>
#include <QDragMoveEvent>

LayoutsTableWidget::LayoutsTableWidget(QWidget* parent)
: QTableWidget(parent)
{
  this->mp_UserLayouts = 0;
  
  this->setEditTriggers(QAbstractItemView::DoubleClicked);
  this->setSelectionMode(QAbstractItemView::SingleSelection);
  this->setSelectionBehavior(QAbstractItemView::SelectRows);
  this->setShowGrid(false);
  this->setAlternatingRowColors(true);
  this->setColumnCount(2);
  this->setDragEnabled(true);
  this->setAcceptDrops(true);
  this->setDropIndicatorShown(true);
  this->setDragDropOverwriteMode(false);
  this->setDragDropMode(QAbstractItemView::InternalMove);
  this->setDefaultDropAction(Qt::MoveAction);
  
  QHeaderView* header = 0;
  // Vertical header
  header = this->verticalHeader();
  header->setDefaultSectionSize(20);
  header->setVisible(false);
  // Horizontal header
  header = this->horizontalHeader();
  header->setMovable(false);
  header->resizeSection(1, 25);
  header->setResizeMode(1, QHeaderView::Fixed);
  header->setResizeMode(0, QHeaderView::Stretch);
  header->setVisible(false);
  
  connect(this, SIGNAL(cellChanged(int,int)), this, SLOT(updateLayout(int, int)));
};

void LayoutsTableWidget::refresh()
{
  this->blockSignals(true);
  this->clear();
  this->setRowCount(this->mp_UserLayouts->count() / 2);
  for (int i = 0 ; i < this->rowCount() ; ++i)
  {
    QTableWidgetItem* item = new QTableWidgetItem(this->mp_UserLayouts->operator[](i*2).toString());
    this->setItem(i, 0, item);
    QPushButton* button = new QPushButton("", this);
    button->setFlat(true);
    button->setStyleSheet("QPushButton {image: url(:/Resources/Images/plot_delete.png);} QPushButton:pressed {image: url(:/Resources/Images/plot_delete-down.png);} QPushButton:flat {border: none;}");
    this->setCellWidget(i, 1, button);
    connect(button, SIGNAL(clicked()), this, SLOT(removeLayout()));
  }
  this->blockSignals(false);
};

QRect LayoutsTableWidget::visualRect(const QModelIndex& index) const
{
  QRect rectText = this->QTableWidget::visualRect(index);
  if (index.column() == 0)
  {
    QRect rectButton = this->QTableWidget::visualRect(index.sibling(index.row(),1));
    rectText.setWidth(rectText.width() + rectButton.width());
  }
  return rectText;
};

void LayoutsTableWidget::dragMoveEvent(QDragMoveEvent* event)
{
  if ((dragDropMode() == QAbstractItemView::InternalMove) && ((event->source() != this) || !(event->possibleActions() & Qt::MoveAction)))
    return;
  
  // ignore by default
  event->ignore();
  this->setDropIndicatorShown(true);
      
  QTableWidgetItem* item = this->itemAt(event->pos());
  QList<QTableWidgetItem*> selectedItems = this->selectedItems();
  bool enableDragMove = true;
  if (!item)
    enableDragMove = false;
  else
  {
    int div = (this->item(this->rowCount()-1,0) == item) ? 2 : 1;
    for (QList<QTableWidgetItem*>::const_iterator it = selectedItems.begin() ; it != selectedItems.end() ; ++it)
    {
      // If one item in the dragged row corresponds to the current selected item
      if (*it == item)
      {
        enableDragMove = false;
        break;
      }
      // Or if the mouse is between the current selected item and the next one.
      else if ((*it)->row() == item->row()-1)
      {
        QRect rect = this->visualItemRect(item);
        if ((event->pos().y() - rect.top()) < rect.height() / div)
        {
          enableDragMove = false;
          break;
        }
      }
    }
  }
  if (!enableDragMove)
  {
    this->setDropIndicatorShown(false);
    this->viewport()->update();
    return;
  }
  
  QPoint pos = this->adaptDragDropEventPosition(event->pos(), item);
  QDragMoveEvent evt(pos, event->possibleActions(), event->mimeData(), event->mouseButtons(), event->keyboardModifiers(), event->type());
  
  this->QTableWidget::dragMoveEvent(&evt);
  
  if (evt.isAccepted())
    event->accept();
};

void LayoutsTableWidget::dropEvent(QDropEvent* event)
{
  QPoint pos = this->adaptDragDropEventPosition(event->pos(), this->itemAt(event->pos()));
  
  if (event->source() == this && (event->dropAction() == Qt::MoveAction || this->dragDropMode() == QAbstractItemView::InternalMove))
  {
    this->blockSignals(true);
    int finalRowIdx = this->rowAt(pos.y());
    int newRowIdx = finalRowIdx + (this->dropIndicatorPosition() == QAbstractItemView::BelowItem ? 1 : 0);
    int rowIdx = this->currentRow();
    QTableWidgetItem* takenItem = this->takeItem(rowIdx,0);
    this->insertRow(newRowIdx);
    this->setItem(newRowIdx,0,takenItem);
    QPushButton* button = new QPushButton("", this);
    button->setFlat(true);
    button->setStyleSheet("QPushButton {image: url(:/Resources/Images/plot_delete.png);} QPushButton:pressed {image: url(:/Resources/Images/plot_delete-down.png);} QPushButton:flat {border: none;}");
    this->setCellWidget(newRowIdx, 1, button);
    connect(button, SIGNAL(clicked()), this, SLOT(removeLayout()));
    this->removeRow(rowIdx + (newRowIdx < rowIdx ? 1 : 0));
    this->blockSignals(false);
    emit userLayoutDropped(finalRowIdx - (((finalRowIdx > rowIdx) && (this->dropIndicatorPosition() == QAbstractItemView::AboveItem)) ? 1 : 0), rowIdx);
  }
  event->setDropAction(Qt::IgnoreAction);
  QTableView::dropEvent(event);
  event->accept();
};

void LayoutsTableWidget::updateLayout(int row, int col)
{
  if (col != 0)
    return;
  emit userLayoutLabelChanged(row, this->item(row, col)->text());  
};

void LayoutsTableWidget::removeLayout()
{
  QObject* obj = sender();
  for (int i = 0 ; i < this->rowCount() ; ++i)
  {
    if (this->cellWidget(i,1) == obj)
    {
      this->removeRow(i);
      emit userLayoutRemoved(i);
      break;
    }
  }
}

// Create an event where the value for the y coordinate will give only an AboveItem.
// Special case for the last item which can also give a BelowItem.
QPoint LayoutsTableWidget::adaptDragDropEventPosition(const QPoint& p, QTableWidgetItem* item)
{
  QRect rect = this->visualItemRect(item);
  QPoint pos(p.x(), rect.top());
  if ((this->item(this->rowCount()-1,0) == item) && ((rect.bottom() - p.y()) < rect.height() / 2)) // Below last item 
    pos.setY(rect.bottom());
  return pos;
};

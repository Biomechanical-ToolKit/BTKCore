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

#include "ModelTreeWidget.h"

#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>

ModelTreeWidget::ModelTreeWidget(QWidget* parent)
: QTreeWidget(parent)
{};
  
// void ModelTreeWidget::dragEnterEvent(QDragEnterEvent *event)
// {
//   event->ignore();
// };
// 
// void ModelTreeWidget::dragLeaveEvent(QDragLeaveEvent* event)
// {
//   event->ignore();
// };

void ModelTreeWidget::dragMoveEvent(QDragMoveEvent* event)
{
  // FIXME: For the moment, the motion of the markers is ignored.
  event->ignore();
  return;
#if 0
  if ((dragDropMode() == QAbstractItemView::InternalMove) && ((event->source() != this) || !(event->possibleActions() & Qt::MoveAction)))
    return;
  
  // ignore by default
  event->ignore();
  this->setDropIndicatorShown(true);
      
  QTreeWidgetItem* item = this->itemAt(event->pos());
  QList<QTreeWidgetItem*> selectedItems = this->selectedItems();
  bool enableDragMove = true;
  if (!item)
    enableDragMove = false;
  else
  {
    for (QList<QTreeWidgetItem*>::const_iterator it = selectedItems.begin() ; it != selectedItems.end() ; ++it)
    {
      if ((*it)->parent() != item->parent())
      {
        enableDragMove = false;
        break;
      }
    }
  }
  if (!enableDragMove)
  {
    this->setDropIndicatorShown(false);
    this->viewport()->update();
    return;
  }
  
  QPoint pos = this->adaptDragDropEventPosition(event->pos(), item, selectedItems);
  QDragMoveEvent evt(pos, event->possibleActions(), event->mimeData(), event->mouseButtons(), event->keyboardModifiers(), event->type());
  
  this->QTreeWidget::dragMoveEvent(&evt);
  
  if (evt.isAccepted())
    event->accept();
#endif
};

void ModelTreeWidget::dropEvent(QDropEvent* event)
{
  QPoint pos = this->adaptDragDropEventPosition(event->pos(), this->itemAt(event->pos()), QList<QTreeWidgetItem*>());
  QDropEvent evt(pos, event->possibleActions(), event->mimeData(), event->mouseButtons(), event->keyboardModifiers(), event->type());
  
  this->QTreeWidget::dropEvent(&evt);
  
  if (evt.isAccepted())
    event->accept();
};

// Create an event where the value for the y coordinate will give only an AboveItem.
// Excepted for the last one which will has also the possibility to give BelowItem.
// Excepted for the selected items where the event emulate a BelowItem of the item above.
QPoint ModelTreeWidget::adaptDragDropEventPosition(const QPoint& p, QTreeWidgetItem* item, const QList<QTreeWidgetItem*>& selectedItems)
{
  QRect rect = this->visualItemRect(item);
  QPoint pos(p.x(), rect.top());
  if ((item->parent()->indexOfChild(item) == (item->parent()->childCount() - 1)) && ((rect.bottom() - p.y()) < 2)) // Below last item 
    pos.setY(rect.bottom());
  else if (selectedItems.contains(item))
    pos.setY(this->visualItemRect(this->itemAbove(item)).bottom());
  return pos;
};
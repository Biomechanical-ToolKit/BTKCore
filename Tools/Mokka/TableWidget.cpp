/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2010, Arnaud Barré
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

#include "TableWidget.h"
#include "UserRoles.h"

// ------------------- MarkerTableWidget -------------------
MarkerTableWidget::MarkerTableWidget(QWidget* parent)
: QTableWidget(parent)
{};

void MarkerTableWidget::mousePressEvent(QMouseEvent* event)
{
  QPoint pos = event->pos();
  int x = pos.x();
  QStyle* style = this->style();
  QStyleOptionButton opt;
  QRect checkBoxRect = style->subElementRect(QStyle::SE_ViewItemCheckIndicator, &opt);
  const int checkMargin = style->pixelMetric(QStyle::PM_FocusFrameHMargin, 0, this) + 1;
  const int iconPixelStart = checkBoxRect.x() + checkBoxRect.width() + checkMargin * 3;
  const int iconPixelEnd = iconPixelStart + 16;
  QTableWidgetItem* item;
  if ((x >= iconPixelStart) && (x <= iconPixelEnd) && ((item = this->itemAt(pos)) != 0))
  {
    this->blockSignals(true);
    bool actived = item->data(markerTrajectoryActived).toBool();
    if (actived)
      item->setIcon(QIcon(":/Resources/Images/trajectory_unactive.png"));
    else
      item->setIcon(QIcon(":/Resources/Images/trajectory_active.png"));
    this->blockSignals(false);
    item->setData(markerTrajectoryActived, !actived);
  }
  QTableWidget::mousePressEvent(event);
};

// ------------------- PointEditorTableWidget -------------------
PointEditorTableWidget::PointEditorTableWidget(QWidget* parent)
: QTableWidget(parent)
{};

void PointEditorTableWidget::dropEvent(QDropEvent* event)
{
  QModelIndex cur = this->currentIndex();
  // Check to not drop label column onto description column
  if ((event->source() != this) || 
      (this->indexAt(event->pos()).column() != cur.column()))

  {
    this->viewport()->update();
    return;
  }
  QDropEvent* evt;
  // If the drop is coming from a description, need to create a new drop event.
  if (cur.column() == 1)
  {
    evt = new QDropEvent(QPoint(32, event->pos().y()),
                         event->proposedAction(),
                         event->mimeData(),
                         event->mouseButtons(),
                         event->keyboardModifiers(),
                         event->type());
  }
  else
    evt = new QDropEvent(*event);
  // Dropping
  QTableWidget::dropEvent(evt);
  delete evt;
  // Delete copied row(s)
  if (evt->isAccepted())
  {
    event->accept();
    this->blockSignals(true);
    QModelIndexList indexes = this->selectedIndexes();
    this->clearSelection();
    for (QModelIndexList::iterator it = indexes.begin() ; it != indexes.end() ; ++it)
    {
      if (it->column() == 0)
        this->removeRow(it->row());
    }
    this->blockSignals(false);
  }
};
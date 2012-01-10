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
 
#ifndef Preferences_Layouts_h
#define Preferences_Layouts_h
 
#include <QTableWidget>

class QDragMoveEvent;

class LayoutsTableWidget : public QTableWidget
{
  Q_OBJECT
  
public:
  LayoutsTableWidget(QWidget* parent = 0);
  
  const QList<QVariant>* userLayouts() const {return this->mp_UserLayouts;};
  void setUserLayouts(QList<QVariant>* layouts) {this->mp_UserLayouts = layouts;};
  void refresh();
  virtual QRect visualRect(const QModelIndex& index) const;
  
protected:
  void dragMoveEvent(QDragMoveEvent* event);
  void dropEvent(QDropEvent* event);
  
signals:
  void userLayoutRemoved(int index);
  void userLayoutLabelChanged(int index, const QString& label);
  void userLayoutDropped(int newRow, int oldRow);
  
private slots:
  void updateLayout(int row, int col);
  void removeLayout();
  
private:
  QPoint adaptDragDropEventPosition(const QPoint& p, QTableWidgetItem* item);
  
  QList<QVariant>* mp_UserLayouts;
};

#endif // Preferences_Layouts_h
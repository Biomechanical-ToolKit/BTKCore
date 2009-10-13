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

#include "PointsEditor.h"
#include "UserRoles.h"

PointsEditor::PointsEditor(QWidget* parent)
: QDialog(parent)
{
   this->setupUi(this);
   this->tableWidget->setItemPrototype(new PointEditorTableWidgetItem());
   
#ifdef Q_OS_MAC
  QFont f = this->tableWidget->font();
  f.setPointSize(10);
  this->tableWidget->setFont(f);
  f.setPointSize(11);
  this->insertPointsButton->setFont(f);
  this->removePointsButton->setFont(f);
  f = this->info1->font();
  f.setPointSize(10);
  this->info1->setFont(f);
  this->info2->setFont(f);
#endif

  connect(this->tableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(modifyWindow()));
  connect(this->tableWidget, SIGNAL(itemSelectionChanged()), this, SLOT(togglePointsVisibilityButtons()));
  connect(this->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(showPointType(int)));
  connect(this->insertPointsButton, SIGNAL(clicked()), this, SLOT(insertAllPoints()));
  connect(this->removePointsButton, SIGNAL(clicked()), this, SLOT(removeAllPoints()));
};
 
void PointsEditor::modifyWindow()
{
  this->setWindowModified(true);
};

void PointsEditor::showPointType(int index)
{
  switch (index)
  {
  case 0:
    this->keepOnlyMarkers();
    break;
  case 1:
    this->showAllPoints();
    break;
  case 2:
    this->showOnlyType(btk::Point::Marker);
    break;
  case 3:
    this->showOnlyType(btk::Point::Angle);
    break;
  case 4:
    this->showOnlyType(btk::Point::Force);
    break;
  case 5:
    this->showOnlyType(btk::Point::Moment);
    break;
  case 6:
    this->showOnlyType(btk::Point::Power);
    break;
  case 7:
    this->showOnlyType(btk::Point::Scalar);
    break;
  }
};

void PointsEditor::togglePointsVisibilityButtons()
{
  QList<QTableWidgetItem*> items = this->tableWidget->selectedItems();
  if (items.count() > 1)
  {
    this->insertPointsButton->setText(QObject::tr("Insert"));
    this->removePointsButton->setText(QObject::tr("Remove"));
    disconnect(this->insertPointsButton, 0, this, 0);
    disconnect(this->removePointsButton, 0, this, 0);
    connect(this->insertPointsButton, SIGNAL(clicked()), this, SLOT(insertSelectedPoints()));
    connect(this->removePointsButton, SIGNAL(clicked()), this, SLOT(removeSelectedPoints()));
  }
  else
  {
    this->insertPointsButton->setText(QObject::tr("Insert All"));
    this->removePointsButton->setText(QObject::tr("Remove All"));
    disconnect(this->insertPointsButton, 0, this, 0);
    disconnect(this->removePointsButton, 0, this, 0);
    connect(this->insertPointsButton, SIGNAL(clicked()), this, SLOT(insertAllPoints()));
    connect(this->removePointsButton, SIGNAL(clicked()), this, SLOT(removeAllPoints()));
  }
};

void PointsEditor::insertSelectedPoints()
{
  QList<QTableWidgetItem*> items = this->tableWidget->selectedItems();
  this->tableWidget->blockSignals(true);
  for (QList<QTableWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
    if ((*it)->column() == 0)
      (*it)->setCheckState(Qt::Checked);
  this->tableWidget->blockSignals(false);
  this->setWindowModified(true);
};

void PointsEditor::removeSelectedPoints()
{
  QList<QTableWidgetItem*> items = this->tableWidget->selectedItems();
  this->tableWidget->blockSignals(true);
  for (QList<QTableWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
    if ((*it)->column() == 0)
      (*it)->setCheckState(Qt::Unchecked);
  this->tableWidget->blockSignals(false);
  this->setWindowModified(true);
};

void PointsEditor::insertAllPoints()
{
  this->tableWidget->blockSignals(true);
  int num = this->tableWidget->rowCount();
  for (int row = 0 ; row < num ; ++row)
    this->tableWidget->item(row, 0)->setCheckState(Qt::Checked);
  this->tableWidget->blockSignals(false);
  this->setWindowModified(true);
};

void PointsEditor::removeAllPoints()
{
  this->tableWidget->blockSignals(true);
  int num = this->tableWidget->rowCount();
  for (int row = 0 ; row < num ; ++row)
    this->tableWidget->item(row, 0)->setCheckState(Qt::Unchecked);
  this->tableWidget->blockSignals(false);
  this->setWindowModified(true);
};

void PointsEditor::keepOnlyMarkers()
{
  int num = this->tableWidget->rowCount();
  for (int i = 0 ; i < num ; ++i)
  {
    QTableWidgetItem* item = this->tableWidget->item(i, 0);
    if (item->data(pointType).toInt() == btk::Point::Marker)
      item->setCheckState(Qt::Checked);
    else
      item->setCheckState(Qt::Unchecked);
    this->tableWidget->setRowHidden(i, false);
  }
};

void PointsEditor::showAllPoints()
{
  int num = this->tableWidget->rowCount();
  for (int i = 0 ; i < num ; ++i)
    this->tableWidget->setRowHidden(i, false);
};

void PointsEditor::showOnlyType(btk::Point::Type t)
{
  int num = this->tableWidget->rowCount();
  for (int i = 0 ; i < num ; ++i)
  {
    std::cout << this->tableWidget->item(i, 0)->data(pointType).toInt() << " vs " << static_cast<int>(t) << std::endl;
    if (this->tableWidget->item(i, 0)->data(pointType).toInt() == static_cast<int>(t))
      this->tableWidget->setRowHidden(i, false);
    else
      this->tableWidget->setRowHidden(i, true);
  }
};
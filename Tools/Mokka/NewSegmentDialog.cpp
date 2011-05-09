/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2011, Arnaud Barré
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

#include <QPushButton>
 
NewSegmentDialog::NewSegmentDialog(QWidget* parent)
: QDialog(parent)
{
  this->setupUi(this);
#ifdef Q_OS_MAC
  this->infoLabel->setAttribute(Qt::WA_MacSmallSize);
  this->linksTable->setAttribute(Qt::WA_MacShowFocusRect, 0);
  this->setWindowFlags(Qt::Sheet);
  this->setWindowModality(Qt::WindowModal);
  
  this->setMinimumSize(this->sizeHint());
  this->setMaximumSize(this->sizeHint());
#endif
  QHeaderView* linksTableHeader = this->linksTable->horizontalHeader();
  linksTableHeader->setDefaultSectionSize(26);
  linksTableHeader->setResizeMode(0, QHeaderView::Fixed);
  linksTableHeader->setResizeMode(1, QHeaderView::Stretch);
  linksTableHeader->setResizeMode(2, QHeaderView::Stretch);
  this->linksTable->verticalHeader()->setDefaultSectionSize(20);
  
  this->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
  
  connect(this->segmentLabelEdit, SIGNAL(textEdited(QString)), this, SLOT(validate()));
  connect(this->linksTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(validate()));
};

void NewSegmentDialog::setEditLinksMode()
{
  this->setWindowTitle(tr("Edit links"));
  this->segmentLabelLabel->setVisible(false);
  this->segmentLabelEdit->setVisible(false);
  this->segmentDescriptionLabel->setVisible(false);
  this->segmentDescriptionEdit->setVisible(false);
  this->line->setVisible(false);
  disconnect(this->segmentLabelEdit, SIGNAL(textEdited(QString)), this, 0);
  disconnect(this->linksTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, 0);
  this->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
  this->buttonBox->button(QDialogButtonBox::Ok)->setDefault(true);
}

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
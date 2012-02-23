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
 
#include "NewItemDialog.h"

NewItemDialog::NewItemDialog(QWidget* parent)
: QDialog(parent)
{
  this->setupUi(this);
#ifdef Q_OS_MAC
  this->setWindowFlags(Qt::Sheet);
  this->setWindowModality(Qt::WindowModal);
#endif
  QPushButton* ok = this->buttonBox->button(QDialogButtonBox::Ok);
  ok->setDefault(true);
  ok->setEnabled(false);
  QPushButton* cancel = this->buttonBox->button(QDialogButtonBox::Cancel);
  cancel->setAutoDefault(false);
  
  connect(this->existingRadioButton, SIGNAL(toggled(bool)), this, SLOT(toggleExistingItem(bool)));
  connect(this->newRadioButton, SIGNAL(toggled(bool)), this, SLOT(toggleNewItem(bool)));
  connect(this->existingComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateButtonState(int)));
  connect(this->newLineEdit, SIGNAL(textChanged(QString)), this, SLOT(updateButtonState(QString)));
  
  this->existingComboBox->setEnabled(false);
  this->newRadioButton->setChecked(true);
};

QString NewItemDialog::createdItemText(int* itemIndex) const
{
  int index = -1;
  QString label;
  if (this->existingComboBox->isEnabled())
  {
    index = this->existingComboBox->currentIndex();
    label = this->existingComboBox->currentText();
  }
  else
    label = this->newLineEdit->text();
  if (itemIndex != 0)
    *itemIndex = index;
  return label;
};

void NewItemDialog::toggleExistingItem(bool toggled)
{
  this->existingComboBox->setEnabled(toggled);
  this->existingComboBox->setFocus();
  if (toggled)
    this->updateButtonState(this->existingComboBox->currentIndex());
};

void NewItemDialog::toggleNewItem(bool toggled)
{
  this->newLineEdit->setEnabled(toggled);
  this->newLineEdit->setFocus();
  if (toggled)
    this->updateButtonState(this->newLineEdit->text());
};


void NewItemDialog::updateButtonState(int index)
{
  if (index == -1)
    this->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
  else
    this->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
};

void NewItemDialog::updateButtonState(const QString& name)
{
  static const QColor defaultTextColor = this->newLineEdit->palette().color(QPalette::Text);
  if (name.isEmpty())
  {
    this->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
  }
  else if (this->itemAlreadyExists(name))
  {
    this->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    QPalette p = this->newLineEdit->palette();
    p.setColor(QPalette::Text, Qt::red);
    this->newLineEdit->setPalette(p);
  }
  else
  {
    this->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    QPalette p = this->newLineEdit->palette();
    p.setColor(QPalette::Text, defaultTextColor);
    this->newLineEdit->setPalette(p);
  }
};
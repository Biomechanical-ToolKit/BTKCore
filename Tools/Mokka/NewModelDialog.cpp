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
 
#include "NewModelDialog.h"

#include <QPushButton>
 
NewModelDialog::NewModelDialog(const QList<ConfigurationItem>* configs, QWidget* parent)
: QDialog(parent)
{
  this->mp_Configurations = configs;
  
  this->setupUi(this);
  QPushButton* ok = this->buttonBox->button(QDialogButtonBox::Ok);
  ok->setDefault(true);
  ok->setEnabled(false);
  QPushButton* cancel = this->buttonBox->button(QDialogButtonBox::Cancel);
  cancel->setAutoDefault(false);
  this->errorIconLabel->setVisible(false);
  this->errorMsgLabel->setVisible(false);
  
  connect(this->lineEdit, SIGNAL(textChanged(QString)), this, SLOT(updateButton(QString)));
  //connect(this->lineEdit, SIGNAL(returnPressed()), this, SLOT(validateName()));
};

void NewModelDialog::setConfigurationName(const QString& name)
{
  QString n = name;
  int i = 0, inc = 0;
  while (i < this->mp_Configurations->count())
  {
    if (this->mp_Configurations->operator[](i).name.compare(n) == 0)
    {
      ++inc;
      n = name + "-" + QString::number(inc);
      i = 0;
    }
    else
      ++i;
  }
  this->lineEdit->setText(n);
  this->lineEdit->selectAll();
};

QString NewModelDialog::configurationName() const
{
  return this->lineEdit->text();
};

void NewModelDialog::updateButton(const QString& name)
{
  if (name.isEmpty())
  {
    this->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    this->errorIconLabel->setVisible(false);
    this->errorMsgLabel->setVisible(false);
  }
  else if (this->configurationAlreadyExists(name))
  {
    this->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    this->errorIconLabel->setVisible(true);
    this->errorMsgLabel->setVisible(true);
  }
  else
  {
    this->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    this->errorIconLabel->setVisible(false);
    this->errorMsgLabel->setVisible(false);
  }
};

bool NewModelDialog::configurationAlreadyExists(const QString& name)
{
  for (int i = 0 ; i < this->mp_Configurations->count() ; ++i)
  {
    if (name.compare(this->mp_Configurations->operator[](i).name) == 0)
      return true;
  }
  return false;
};
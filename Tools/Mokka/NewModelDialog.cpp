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
 
#include "NewModelDialog.h"

NewModelDialog::NewModelDialog(const QList<ConfigurationItem>* configs, QWidget* parent)
: NewItemTemplateDialog<ConfigurationItem>(configs, parent)
{
  this->setWindowTitle(tr("New Model"));
  this->label->setText(tr("Create a model from an existing one or a new one"));
};

void NewModelDialog::setConfigurationName(const QString& name)
{
  QString n = name;
  int i = 0, inc = 0;
  while (i < this->mp_Items->count())
  {
    if (this->mp_Items->operator[](i).name.compare(n) == 0)
    {
      ++inc;
      n = name + "-" + QString::number(inc);
      i = 0;
    }
    else
      ++i;
  }
  this->newLineEdit->setText(n);
  this->newLineEdit->selectAll();
  this->newRadioButton->setChecked(true);
};

bool NewModelDialog::itemAlreadyExists(const QString& name)
{
  for (int i = 0 ; i < this->mp_Items->count() ; ++i)
  {
    if (name.compare(this->mp_Items->operator[](i).name) == 0)
      return true;
  }
  return false;
};
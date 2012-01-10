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

#ifndef NewItemDialog_h
#define NewItemDialog_h

#include "ui_NewItemDialog.h"

#include <QDialog>
#include <QPushButton>

template <class T>
class NewItemTemplateDialog : public QDialog, public Ui::NewItemDialog
{
public:
  NewItemTemplateDialog(const QList<T>* items, QWidget* parent = 0);
  // ~NewItemTemplateDialog(); // Implicit
  
  void updateButtonState(const QString& name);

protected:
  virtual bool itemAlreadyExists(const QString& /* name */) {return false;};
  
  const QList<T>* mp_Items;
};

template <class T>
NewItemTemplateDialog<T>::NewItemTemplateDialog(const QList<T>* configs, QWidget* parent)
: QDialog(parent)
{
  this->mp_Items = configs;
  
  this->setupUi(this);
#ifdef Q_OS_MAC
  this->setWindowFlags(Qt::Sheet);
  this->setWindowModality(Qt::WindowModal);
  this->setMinimumSize(350, this->sizeHint().height());
#endif
  QPushButton* ok = this->buttonBox->button(QDialogButtonBox::Ok);
  ok->setDefault(true);
  ok->setEnabled(false);
  QPushButton* cancel = this->buttonBox->button(QDialogButtonBox::Cancel);
  cancel->setAutoDefault(false);
  this->errorIconLabel->setVisible(false);
  this->errorMsgLabel->setVisible(false);
};

template <class T>
void NewItemTemplateDialog<T>::updateButtonState(const QString& name)
{
  if (name.isEmpty())
  {
    this->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    this->errorIconLabel->setVisible(false);
    this->errorMsgLabel->setVisible(false);
  }
  else if (this->itemAlreadyExists(name))
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

#endif // NewItemDialog_h
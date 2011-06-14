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

#include "NewEventDialog.h"

#include <QPushButton>

NewEventDialog::NewEventDialog(Mode m, QWidget* parent)
: QDialog(parent)
{
  this->setupUi(this);
  
#ifdef Q_OS_MAC
  this->labelCheckBox->setAttribute(Qt::WA_MacSmallSize);
  this->labelLabel->setAttribute(Qt::WA_MacSmallSize);
  this->labelEdit->setAttribute(Qt::WA_MacSmallSize);
  this->frameCheckBox->setAttribute(Qt::WA_MacSmallSize);
  this->frameLabel->setAttribute(Qt::WA_MacSmallSize);
  this->frameSpinBox->setAttribute(Qt::WA_MacSmallSize);
  this->subjectCheckBox->setAttribute(Qt::WA_MacSmallSize);
  this->subjectLabel->setAttribute(Qt::WA_MacSmallSize);
  this->subjectEdit->setAttribute(Qt::WA_MacSmallSize);
  this->descriptionCheckBox->setAttribute(Qt::WA_MacSmallSize);
  this->descriptionLabel->setAttribute(Qt::WA_MacSmallSize);
  this->descriptionEdit->setAttribute(Qt::WA_MacSmallSize);
  this->contextCheckBox->setAttribute(Qt::WA_MacSmallSize);
  this->contextLabel->setAttribute(Qt::WA_MacSmallSize);
  this->contextComboBox->setAttribute(Qt::WA_MacSmallSize);
  
  // Fix to see correctly the end of the letters 'j' and 'p'
  this->subjectLabel->setMinimumHeight(12);
  this->descriptionLabel->setMinimumHeight(12);
  
  this->resize(340, 220);
#endif
  // Fix the size
  this->setMaximumSize(this->width(),this->height());
  this->setMinimumSize(this->width(),this->height());
  
  // Field modification
  connect(this->labelEdit, SIGNAL(textChanged(QString)), this, SLOT(validateInformations()));
  connect(this->labelEdit, SIGNAL(textChanged(QString)), this, SLOT(activateLabelCheckBox()));
  connect(this->contextComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(validateInformations()));
  connect(this->contextComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(activateContextCheckBox()));
  connect(this->frameSpinBox, SIGNAL(valueChanged(int)), this, SLOT(validateInformations()));
  connect(this->frameSpinBox, SIGNAL(valueChanged(int)), this, SLOT(activateFrameCheckBox()));
  // CheckBox
  connect(this->labelCheckBox, SIGNAL(stateChanged(int)), this, SLOT(validateInformations()));
  connect(this->contextCheckBox, SIGNAL(stateChanged(int)), this, SLOT(validateInformations()));
  connect(this->frameCheckBox, SIGNAL(stateChanged(int)), this, SLOT(validateInformations()));
  // Options
  if (m == New) // New event
  {
    this->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    this->labelCheckBox->setVisible(false);
    this->contextCheckBox->setVisible(false);
    this->frameCheckBox->setVisible(false);
    this->subjectCheckBox->setVisible(false);
    this->descriptionCheckBox->setVisible(false);
  }
  else // Edition
  {
    this->labelLabel->setVisible(false);
    this->contextLabel->setVisible(false);
    this->frameLabel->setVisible(false);
    this->subjectLabel->setVisible(false);
    this->descriptionLabel->setVisible(false);
    connect(this->contextComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(activateContextCheckBox()));
    connect(this->frameSpinBox, SIGNAL(valueChanged(int)), this, SLOT(activateFrameCheckBox()));
    connect(this->subjectEdit, SIGNAL(textChanged(QString)), this, SLOT(activateSubjectCheckBox()));
    connect(this->descriptionEdit, SIGNAL(textChanged(QString)), this, SLOT(activateDescriptionCheckBox()));
  }
};

void NewEventDialog::setInformations(const QString& label, int context, int frameAndROI[3], const QString& subject, const QString& description)
{
  this->labelEdit->setText(label);
  this->contextComboBox->setCurrentIndex(context);
  this->frameSpinBox->setRange(frameAndROI[1], frameAndROI[2]);
  this->frameSpinBox->setValue(frameAndROI[0]);
  if (frameAndROI[0] == -1)
    this->frameSpinBox->clear();
  this->subjectEdit->setText(subject);
  this->descriptionEdit->setText(description);
  this->unactiveAllCheckBox();
};

void NewEventDialog::validateInformations()
{
  if (((this->labelCheckBox->checkState() == Qt::Checked) && (this->labelEdit->text().trimmed().isEmpty()))
   || ((this->contextCheckBox->checkState() == Qt::Checked) && (this->contextComboBox->currentIndex() == -1))
   || ((this->frameCheckBox->checkState() == Qt::Checked) && (this->frameSpinBox->text().isEmpty())))
    this->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
  else
    this->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
};

void NewEventDialog::unactiveAllCheckBox()
{
  this->labelCheckBox->setCheckState(Qt::Unchecked);
  this->contextCheckBox->setCheckState(Qt::Unchecked);
  this->frameCheckBox->setCheckState(Qt::Unchecked);
  this->subjectCheckBox->setCheckState(Qt::Unchecked);
  this->descriptionCheckBox->setCheckState(Qt::Unchecked);
};

void NewEventDialog::activateLabelCheckBox()
{
  this->labelCheckBox->setCheckState(Qt::Checked);
};

void NewEventDialog::activateContextCheckBox()
{
  this->contextCheckBox->setCheckState(Qt::Checked);
};

void NewEventDialog::activateFrameCheckBox()
{
  this->frameCheckBox->setCheckState(Qt::Checked);
};

void NewEventDialog::activateSubjectCheckBox()
{
  this->subjectCheckBox->setCheckState(Qt::Checked);
};

void NewEventDialog::activateDescriptionCheckBox()
{
  this->descriptionCheckBox->setCheckState(Qt::Checked);
};

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
 
#include "ChartCycleSettingDialog.h"

ChartCycleSettingDialog::ChartCycleSettingDialog(QWidget* parent)
: QDialog(parent)
{
  this->setupUi(this);
  this->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
#ifdef Q_OS_MAC
  this->layout()->setContentsMargins(12,12,12,12);
  QFont f = this->font();
  f.setPointSize(10);
  this->infoLabel->setFont(f);
#endif
  this->calculationMethodOptionsButton->setVisible(false); // Because it is not needed now
  
  connect(this->settingNameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(validateSettings()));
  connect(this->horizontalAxisTitleLineEdit, SIGNAL(textChanged(QString)), this, SLOT(validateSettings()));
  connect(this->calculationMethodComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(enableCalculationMethodOptions(int)));
  connect(this->calculationMethodComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(validateSettings()));
  connect(this->fromRightEventComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(validateSettings()));
  connect(this->toRightEventComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(validateSettings()));
  connect(this->fromLeftEventComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(validateSettings()));
  connect(this->toLeftEventComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(validateSettings()));
  connect(this->fromGeneralEventComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(validateSettings()));
  connect(this->toGeneralEventComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(validateSettings()));
  connect(this->rightLabelRuleComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(validateSettings()));
  connect(this->rightLabelRuleLineEdit, SIGNAL(textChanged(QString)), this, SLOT(validateSettings()));
  connect(this->leftLabelRuleComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(validateSettings()));
  connect(this->leftLabelRuleLineEdit, SIGNAL(textChanged(QString)), this, SLOT(validateSettings()));
};

void ChartCycleSettingDialog::init(const QStringList& events)
{
  this->fromRightEventComboBox->addItems(events);
  this->fromRightEventComboBox->setCurrentIndex(-1);
  this->toRightEventComboBox->addItems(events);
  this->toRightEventComboBox->setCurrentIndex(-1);
  this->fromLeftEventComboBox->addItems(events);
  this->fromLeftEventComboBox->setCurrentIndex(-1);
  this->toLeftEventComboBox->addItems(events);
  this->toLeftEventComboBox->setCurrentIndex(-1);
  this->fromGeneralEventComboBox->addItems(events);
  this->fromGeneralEventComboBox->setCurrentIndex(-1);
  this->toGeneralEventComboBox->addItems(events);
  this->toGeneralEventComboBox->setCurrentIndex(-1);
  this->settingNameLineEdit->setText(tr("Cycle: "));
  this->settingNameLineEdit->setFocus();
};

void ChartCycleSettingDialog::enableCalculationMethodOptions(int index)
{
  // TODO: MUST BE ADAPTED WHEN OTHERS CALCULATION METHODS WILL BE IMPLEMENTED
  if ((index == -1) || (index == 0))
    this->calculationMethodOptionsButton->setEnabled(false);
  else
    this->calculationMethodOptionsButton->setEnabled(true);
};
 
void ChartCycleSettingDialog::validateSettings()
{
  if ((calculationMethodComboBox->currentIndex() == -1)
    || (this->rightLabelRuleComboBox->currentIndex() == -1)
    || (this->leftLabelRuleComboBox->currentIndex() == -1)
    || this->settingNameLineEdit->text().isEmpty()
    || this->horizontalAxisTitleLineEdit->text().isEmpty()
    || this->rightLabelRuleLineEdit->text().isEmpty()
    || this->leftLabelRuleLineEdit->text().isEmpty()
    || (((this->fromRightEventComboBox->currentIndex() == -1) || (this->toRightEventComboBox->currentIndex() == -1))
      && ((this->fromLeftEventComboBox->currentIndex() == -1) || (this->toLeftEventComboBox->currentIndex() == -1))
      && ((this->fromGeneralEventComboBox->currentIndex() == -1) || (this->toGeneralEventComboBox->currentIndex() == -1))))
    this->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
  else
    this->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
};
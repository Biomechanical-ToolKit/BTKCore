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
#include "ChartCycleSettingsManager.h"

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
  this->resize(415, 509);
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

void ChartCycleSettingDialog::init(const QStringList& events, const ChartCycleSetting* setting)
{
  this->init(events);
  this->settingNameLineEdit->setText(setting->name);
  this->horizontalAxisTitleLineEdit->setText(setting->horizontalAxisTitle);
  this->calculationMethodComboBox->setCurrentIndex(setting->calculationMethod);
  // TODO: ADD OPTIONS FOR THE CALCULATION METHOD
  this->fromRightEventComboBox->setCurrentIndex(this->fromRightEventComboBox->findText(setting->rightEvents[0]));
  this->toRightEventComboBox->setCurrentIndex(this->toRightEventComboBox->findText(setting->rightEvents[1]));
  this->fromLeftEventComboBox->setCurrentIndex(this->fromLeftEventComboBox->findText(setting->leftEvents[0]));
  this->toLeftEventComboBox->setCurrentIndex(this->toLeftEventComboBox->findText(setting->leftEvents[1]));
  this->fromGeneralEventComboBox->setCurrentIndex(this->fromGeneralEventComboBox->findText(setting->generalEvents[0]));
  this->toGeneralEventComboBox->setCurrentIndex(this->toGeneralEventComboBox->findText(setting->generalEvents[1]));
  this->rightLabelRuleComboBox->setCurrentIndex(setting->rightLabelRule);
  this->rightLabelRuleLineEdit->setText(setting->rightLabelRuleText);
  this->leftLabelRuleComboBox->setCurrentIndex(setting->leftLabelRule);
  this->leftLabelRuleLineEdit->setText(setting->leftLabelRuleText);
};

void ChartCycleSettingDialog::fillSetting(ChartCycleSetting* setting) const
{
  setting->name = this->settingNameLineEdit->text();
  setting->horizontalAxisTitle = this->horizontalAxisTitleLineEdit->text();
  setting->calculationMethod = this->calculationMethodComboBox->currentIndex();
  setting->calculationMethodOption = NULL; // TODO: ADD OPTIONS FOR THE CALCULATION METHOD
  setting->rightEvents[0] = this->fromRightEventComboBox->currentText();
  setting->rightEvents[1] = this->toRightEventComboBox->currentText();
  setting->leftEvents[0] = this->fromLeftEventComboBox->currentText();
  setting->leftEvents[1] = this->toLeftEventComboBox->currentText();
  setting->generalEvents[0] = this->fromGeneralEventComboBox->currentText();
  setting->generalEvents[1] = this->toGeneralEventComboBox->currentText();
  setting->rightLabelRule = this->rightLabelRuleComboBox->currentIndex();
  setting->rightLabelRuleText = this->rightLabelRuleLineEdit->text();
  setting->leftLabelRule = this->leftLabelRuleComboBox->currentIndex();
  setting->leftLabelRuleText = this->leftLabelRuleLineEdit->text();
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
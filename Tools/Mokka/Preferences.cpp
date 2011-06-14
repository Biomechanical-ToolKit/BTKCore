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
 
#include "Preferences.h"

#include <QFileDialog>
#include <QColorDialog>

Preferences::Preferences(QWidget* parent)
: QDialog(parent), lastDirectory("."), m_Data()
{
  this->setupUi(this);
  
  connect(this->defaultConfigurationButton, SIGNAL(clicked()), this, SLOT(setDefaultConfiguration()));
  connect(this->defaultSegmentColorButton, SIGNAL(clicked()), this, SLOT(setDefaultSegmentColor()));
  connect(this->defaultMarkerColorButton, SIGNAL(clicked()), this, SLOT(setDefaultMarkerColor()));
  connect(this->defaultForcePlateColorButton, SIGNAL(clicked()), this, SLOT(setDefaultForcePlateColor()));
  connect(this->defaultForceVectorColorButton, SIGNAL(clicked()), this, SLOT(setDefaultForceVectorColor()));
  
  this->m_Data[DefaultConfigurationUse] = false;
  this->m_Data[DefaultConfigurationPath] = "";
  this->m_Data[EventEditorWhenInserting] = false;
  this->m_Data[DefaultGroundOrientation] = -1;
  this->m_Data[DefaultSegmentColor] = QColor();
  this->m_Data[DefaultMarkerColor] = QColor();
  this->m_Data[DefaultMarkerRadius] = -1;
  this->m_Data[DefaultTrajectoryLength] = -1;
  this->m_Data[ForcePlatformAxesDisplay] = -1;
  this->m_Data[ForcePlatformIndexDisplay] = -1;
  this->m_Data[DefaultForcePlateColor] = QColor();
  this->m_Data[DefaultForceVectorColor] = QColor();
  this->m_Data[AutomaticCheckUpdateUse] = false;
  
  // Force the General tab to be the current.
  this->tabWidget->setCurrentIndex(0);
};

void Preferences::saveSettings()
{
  bool checked = false;
  int index = -1;
  QColor color;
  
  checked = this->defaultConfigurationCheckBox->checkState() == Qt::Checked;
  if (this->m_Data[DefaultConfigurationUse].toBool() != checked)
  {
    this->m_Data[DefaultConfigurationUse] = checked;
    emit useDefaultConfigurationStateChanged(checked);
  }
    
  QString path = this->defaultConfigurationLineEdit->text();
  if (path.compare(this->m_Data[DefaultConfigurationPath].toString()) != 0)
  {
    this->m_Data[DefaultConfigurationPath] = path;
    emit defaultConfigurationPathChanged(path);
  }
    
  checked = this->openEventEditorCheckBox->checkState() == Qt::Checked;
  if (this->m_Data[EventEditorWhenInserting].toBool() != checked)
  {
    this->m_Data[EventEditorWhenInserting] = checked;
    emit useEventEditorWhenInsertingStateChanged(checked);
  }
  
  index = this->defaultPlaneOrientationComboBox->currentIndex();
  if (this->m_Data[DefaultGroundOrientation].toInt() != index)
  {
    this->m_Data[DefaultGroundOrientation] = index;
    emit defaultGroundOrientationChanged(index);
  }
  
  color = this->defaultSegmentColorButton->property("backgroundColor").value<QColor>();
  if (this->m_Data[DefaultSegmentColor].value<QColor>() != color)
  {
    this->m_Data[DefaultSegmentColor] = color;
    emit defaultSegmentColorChanged(color);
  }
  
  color = this->defaultMarkerColorButton->property("backgroundColor").value<QColor>();
  if (this->m_Data[DefaultMarkerColor].value<QColor>() != color)
  {
    this->m_Data[DefaultMarkerColor] = color;
    emit defaultMarkerColorChanged(color);
  }
  
  double value = this->defaultMarkerRadiusSpinBox->value();
  if (this->m_Data[DefaultMarkerRadius].toDouble() != value)
  {
    this->m_Data[DefaultMarkerRadius] = value;
    emit defaultMarkerRadiusChanged(value);
  }
  
  index = this->defaultMarkerTrajectoryLengthComboBox->currentIndex();
  if (this->m_Data[DefaultTrajectoryLength].toInt() != index)
  {
    this->m_Data[DefaultTrajectoryLength] = index;
    emit defaultMarkerTrajectoryLengthChanged(index);
  }
  
  index = this->showForcePlatformAxesComboBox->currentIndex();
  if (this->m_Data[ForcePlatformAxesDisplay].toInt() != index)
  {
    this->m_Data[ForcePlatformAxesDisplay] = index;
    emit showForcePlatformAxesChanged(index);
  }
  
  index = this->showForcePlatformIndexComboBox->currentIndex();
  if (this->m_Data[ForcePlatformIndexDisplay].toInt() != index)
  {
    this->m_Data[ForcePlatformIndexDisplay] = index;
    emit showForcePlatformIndexChanged(index);
  }
  
  color = this->defaultForcePlateColorButton->property("backgroundColor").value<QColor>();
  if (this->m_Data[DefaultForcePlateColor].value<QColor>() != color)
  {
    this->m_Data[DefaultForcePlateColor] = color;
    emit defaultForcePlateColorChanged(color);
  }
  
  color = this->defaultForceVectorColorButton->property("backgroundColor").value<QColor>();
  if (this->m_Data[DefaultForceVectorColor].value<QColor>() != color)
  {
    this->m_Data[DefaultForceVectorColor] = color;
    emit defaultForceVectorColorChanged(color);
  }
  
  checked = this->automaticCheckUpdateCheckBox->checkState() == Qt::Checked;
  if (this->m_Data[AutomaticCheckUpdateUse].toBool() != checked)
  {
    this->m_Data[AutomaticCheckUpdateUse] = checked;
    emit automaticCheckUpdateStateChanged(checked);
  }
};

void Preferences::resetSettings()
{
  this->defaultConfigurationCheckBox->setChecked(this->m_Data[DefaultConfigurationUse].toBool());
  this->defaultConfigurationLineEdit->setText(this->m_Data[DefaultConfigurationPath].toString());
  this->openEventEditorCheckBox->setChecked(this->m_Data[EventEditorWhenInserting].toBool());
  this->defaultPlaneOrientationComboBox->setCurrentIndex(this->m_Data[DefaultGroundOrientation].toInt());
  colorizeButton(this->defaultSegmentColorButton, this->m_Data[DefaultSegmentColor].value<QColor>());
  colorizeButton(this->defaultMarkerColorButton, this->m_Data[DefaultMarkerColor].value<QColor>() );
  this->defaultMarkerRadiusSpinBox->setValue(this->m_Data[DefaultMarkerRadius].toDouble());
  this->defaultMarkerTrajectoryLengthComboBox->setCurrentIndex(this->m_Data[DefaultTrajectoryLength].toInt());
  this->showForcePlatformAxesComboBox->setCurrentIndex(this->m_Data[ForcePlatformAxesDisplay].toInt());
  this->showForcePlatformIndexComboBox->setCurrentIndex(this->m_Data[ForcePlatformIndexDisplay].toInt());
  colorizeButton(this->defaultForcePlateColorButton, this->m_Data[DefaultForcePlateColor].value<QColor>());
  colorizeButton(this->defaultForceVectorColorButton, this->m_Data[DefaultForceVectorColor].value<QColor>());
  this->automaticCheckUpdateCheckBox->setChecked(this->m_Data[AutomaticCheckUpdateUse].toBool());
  
  this->tabWidget->setCurrentIndex(0);
}

void Preferences::setDefaultConfiguration()
{
  QString filename = this->defaultConfigurationLineEdit->text();
  filename = QFileDialog::getOpenFileName(this,
    tr("Open Model Visual Configuration"),
    filename.isEmpty() ? this->lastDirectory : filename,
    tr("Model Configuration Files (*.mvc *.vst *.vsk);;"
       "Mokka Model Configuration Files (*.mvc);;"
       "Vicon Model Configuration Files (*.vsk *.vst)"));
  if (!filename.isEmpty())
    this->defaultConfigurationLineEdit->setText(filename);
};

void Preferences::setDefaultSegmentColor()
{
  QColor color = QColorDialog::getColor(this->defaultSegmentColorButton->property("backgroundColor").value<QColor>(), this);
  if (color.isValid())
    colorizeButton(this->defaultSegmentColorButton, color);
};

void Preferences::setDefaultMarkerColor()
{
  QColor color = QColorDialog::getColor(this->defaultMarkerColorButton->property("backgroundColor").value<QColor>(), this);
  if (color.isValid())
    colorizeButton(this->defaultMarkerColorButton, color);
};

void Preferences::setDefaultForcePlateColor()
{
  QColor color = QColorDialog::getColor(this->defaultForcePlateColorButton->property("backgroundColor").value<QColor>(), this);
  if (color.isValid())
    colorizeButton(this->defaultForcePlateColorButton, color);
};

void Preferences::setDefaultForceVectorColor()
{
  QColor color = QColorDialog::getColor(this->defaultForceVectorColorButton->property("backgroundColor").value<QColor>(), this);
  if (color.isValid())
    colorizeButton(this->defaultForceVectorColorButton, color);
};
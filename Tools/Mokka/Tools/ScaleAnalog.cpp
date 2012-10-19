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

#include "ScaleAnalog.h"
#include "ScaleAnalogDialog.h"
#include "UndoCommands.h"

#include <QLabel>
#include <QDoubleSpinBox>

void ScaleAnalog::RegisterTool(ToolsManager* manager)
{
  manager->addAnalogTool(tr("Scale"), ToolFactory<ScaleAnalog>);
};

ScaleAnalog::ScaleAnalog(QWidget* parent)
: AbstractTool("Scale Analog", parent)
{};
  
AbstractTool::RunState ScaleAnalog::run(ToolCommands* cmds, ToolsData* const data)
{
  ScaleAnalogDialog dialog(this->parentWidget());
  dialog.initialize(data);
  if (dialog.exec() == QDialog::Accepted)
  {
    QString detail;
    if (dialog.normalizeButton->isChecked())
      detail = "Normalized to 1";
    else
      detail = "Scaled by a factor " + QString::number(dialog.scaleFactorSpinBox->value());
    
    btk::AnalogCollection::Pointer analogs = btk::AnalogCollection::New();
    QList<int> ids = dialog.extractSelectedAnalogChannels(analogs, "_Scaled", detail, data, cmds);
  
    QString log;
    QString log_;
    if (ids.count() == 1)
    {
      log = "The channel ";
      log_ = " was " + detail.toLower();
    }
    else
    {
      log = "The channels ";
      log_ = " were " + detail.toLower();
    }
    
    QList<double> scales;    
    int inc = 0;
    for (btk::AnalogCollection::ConstIterator it = analogs->Begin() ; it != analogs->End() ; ++it)
    {
      if (dialog.normalizeButton->isChecked())
      {
        double scale = 1.0;
        double max = (*it)->GetValues().cwise().abs().maxCoeff();
        if (max > std::numeric_limits<double>::epsilon())
          scale = 1.0 / max;
        scales.push_back(scale);
      }
      else
        scales.push_back(dialog.scaleFactorSpinBox->value());
      log += (inc != 0 ? ", " : "") + QString::fromStdString((*it)->GetLabel());
      ++inc;
    }

    new ScaleAnalogsValues(data->acquisition(), ids, scales, cmds->acquisitionCommand());
    TOOL_LOG_INFO(log + log_);
    return Success;
  }

  return Cancel;
};

// ------------------------------------------------------------------------- //

ScaleAnalogDialog::ScaleAnalogDialog(QWidget* parent)
: AnalogToolOptionDialog("Scale Analog", parent)
{
  QWidget* scaleFactor = new QWidget(this);
  this->normalizeButton = new QRadioButton(this);
  this->scaleButton = new QRadioButton(this);
  this->scaleButton->setChecked(true);
  this->scaleFactorSpinBox = new QDoubleSpinBox(this);
  this->scaleFactorSpinBox->setRange(-10000.0,10000.0);
  this->scaleFactorSpinBox->setDecimals(5);
  this->scaleFactorSpinBox->setValue(1.0);
  this->normalizeButton->setText(tr("Normalize data to 1"));
  this->scaleButton->setText(tr("Scale: "));
  
  QGridLayout* gridLayout = new QGridLayout(scaleFactor);
  gridLayout->addWidget(this->normalizeButton, 0, 0, 1, -1);
  gridLayout->addWidget(this->scaleButton, 1, 0, 1, 1);
  gridLayout->addWidget(this->scaleFactorSpinBox, 1, 1, 1, 1);
  QSpacerItem* horizontalSpacer = new QSpacerItem(40, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
  gridLayout->addItem(horizontalSpacer, 1, 2, 1, 1);
  QLabel* informations = new QLabel(this);
  informations->setWordWrap(true);
  informations->setText(tr("\nScaling your data with a factor below 1 can reduce their accuracy if they are already very small."));
  informations->setMaximumWidth(300);
  gridLayout->addWidget(informations, 2, 0, 1, -1);
  
#ifndef Q_OS_WIN
  QFont f = this->font();
  f.setPointSize(11);
  this->normalizeButton->setFont(f);
  this->scaleButton->setFont(f);
  this->scaleFactorSpinBox->setStyleSheet("QDoubleSpinBox {font-size: 12px;};");
  this->normalizeButton->setMinimumHeight(30);
  scaleFactor->layout()->setSpacing(0);
  f.setItalic(true);
  informations->setFont(f);
#endif

  this->addOption("Scaling rules", scaleFactor);
};

void ScaleAnalogDialog::initializeOptions(const Acquisition* const /* acq */)
{
  QSettings settings;
  int lastScaleRule = settings.value(this->toolSettingsPath() + "lastScaleRule", 1).toInt();
  this->scaleFactorSpinBox->setValue(settings.value(this->toolSettingsPath() + "lastScaleFactor", 1.0).toDouble());
  if (lastScaleRule == 0)
    this->normalizeButton->setChecked(true);
  else
    this->scaleButton->setChecked(true);
};

void ScaleAnalogDialog::saveOptionsSettings()
{
  QSettings settings;
  int lastScaleRule = 0;
  if (this->scaleButton->isChecked())
  {
    lastScaleRule = 1;
    settings.setValue(this->toolSettingsPath() + "lastScaleFactor", this->scaleFactorSpinBox->value());
  }
  settings.setValue(this->toolSettingsPath() + "lastScaleRule", lastScaleRule);
};
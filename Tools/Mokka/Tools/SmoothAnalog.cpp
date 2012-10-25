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

#include "SmoothAnalog.h"
#include "SmoothAnalogDialog.h"
#include "UndoCommands.h"

#include <QDoubleSpinBox>
#include <QComboBox>

void SmoothAnalog::RegisterTool(ToolsManager* manager)
{
  manager->addAnalogTool(tr("Smooth / Linear Envelope"), ToolFactory<SmoothAnalog>);
};

SmoothAnalog::SmoothAnalog(QWidget* parent)
: AbstractTool("Smooth Analog", parent)
{};

static double rmsSmooth(btk::Analog::Values* val, int start, int length)
{
  return sqrt((val->segment(start,length).cwise().square().sum() / static_cast<double>(length)));
};

static double movagSmooth(btk::Analog::Values* val, int start, int length)
{
  return val->segment(start,length).sum() / static_cast<double>(length);
};

AbstractTool::RunState SmoothAnalog::run(ToolCommands* cmds, ToolsData* const data)
{
  SmoothAnalogDialog dialog(this->parentWidget());
  dialog.initialize(data);
  if (dialog.exec() == QDialog::Accepted)
  {
    int windowWidth = 0;
    QString labelSuffix, toolDetail;
    double (*smoother)(btk::Analog::Values*,int,int) = 0;
    if (dialog.rmsMethodButton->isChecked())
    {
      windowWidth = dialog.rmsWindowWidth();
      labelSuffix = "_RMS";
      toolDetail = "Smoothed by RMS";
      smoother = &rmsSmooth;
    }
    else
    {
      windowWidth = dialog.rmsWindowWidth();
      labelSuffix = "_Movag";
      toolDetail = "Smoothed by moving average";
      smoother = &movagSmooth;
    }
    if (windowWidth < 3)
    {
      TOOL_LOG_ERROR("The with of the window is lower than the possible minimum (3). Data processing aborted.");
      return Error;
    }
    else if (windowWidth >= data->acquisition()->analogFrameNumber() / 2)
    {
      TOOL_LOG_ERROR("The with of the window is greater or equal than half of the signal's length. Data processing aborted.");
      return Error;
    }
    btk::AnalogCollection::Pointer analogs = btk::AnalogCollection::New();
    QList<int> ids = dialog.extractSelectedAnalogChannels(analogs, labelSuffix, toolDetail, data, cmds);
    QString log;
    QString log_;
    if (ids.count() == 1)
    {
      log = "The channel ";
      log_ = " was ";
    }
    else
    {
      log = "The channels ";
      log_ = " were ";
    }
    log_ += toolDetail.toLower() + ". Window width: "+ QString::number(windowWidth) + " frames.";
    int inc = 0;
    QSharedPointer< QList<btk::Analog::Values> > values(new QList<btk::Analog::Values>());
    for (btk::AnalogCollection::ConstIterator it = analogs->Begin() ; it != analogs->End() ; ++it)
    {
      // Middle of the signal
      btk::Analog::Values* raw = &((*it)->GetValues());
      values->push_back(*raw);
      btk::Analog::Values* processed = &(values->back());
      int lim = (windowWidth - 1) / 2;
      for (int i = lim ; i < raw->rows()-lim ; ++i)
        processed->coeffRef(i) = smoother(raw, i-lim, windowWidth);
      // The extremities are padded with zeros
      for (int i = 0 ; i < lim ; ++i)
      {
        btk::Analog::Values data(windowWidth);
        data << btk::Analog::Values::Zero(lim-i), raw->segment(0,lim+1+i);
        processed->coeffRef(i) = smoother(&data, 0, windowWidth);
      }
      for (int i = 0 ; i < lim ; ++i)
      {
        int j = raw->rows()-lim+i;
        btk::Analog::Values data(windowWidth);
        data << raw->segment(j-lim,2*lim-i), btk::Analog::Values::Zero(i+1);
        processed->coeffRef(j) = smoother(&data, 0, windowWidth);
      }
      // log
      log += (inc != 0 ? ", '" : "'") + QString::fromStdString((*it)->GetLabel()) + "'";
      ++inc;
    }
    new SetAnalogsValues(data->acquisition(), ids, values, cmds->acquisitionCommand());
    TOOL_LOG_INFO(log + log_);
    return Success;
  }

  return Cancel;
};

// ------------------------------------------------------------------------- //

SmoothAnalogDialog::SmoothAnalogDialog(QWidget* parent)
: AnalogToolOptionDialog("Smooth Analog", parent)
{
  this->m_AnalogNumFrames = 0;
  this->m_AnalogSampleTime = 0;
  
  QWidget* smoothingMethods = new QWidget(this);
  this->rmsMethodButton = new QRadioButton(this);
  this->rmsMethodButton->setText(tr("Root Mean Square:"));
  this->rmsMethodButton->setChecked(true);
  this->rmsWindowWidthSpinBox = new QDoubleSpinBox(this);
  this->rmsWindowWidthSpinBox->setMinimum(1);
  this->rmsWindowWidthSpinBox->setMaximum(1000); // ms
  this->rmsWindowWidthSpinBox->setDecimals(1);
  this->rmsUnitComboBox = new QComboBox(this);
  this->rmsUnitComboBox->addItems(QStringList() << tr("ms") << tr("frames"));
  this->rmsUnitComboBox->setCurrentIndex(0);
  this->movagMethodButton = new QRadioButton(this);
  this->movagMethodButton->setText(tr("Moving average:"));
  this->movagWindowWidthSpinBox = new QDoubleSpinBox(this);
  this->movagWindowWidthSpinBox->setMinimum(1);
  this->movagWindowWidthSpinBox->setMaximum(1000);
  this->movagWindowWidthSpinBox->setDecimals(1);
  this->movagUnitComboBox = new QComboBox(this);
  this->movagUnitComboBox->addItems(QStringList() << tr("ms") << tr("frames"));
  this->movagUnitComboBox->setCurrentIndex(0);
  QLabel* informations = new QLabel(this);
  informations->setText(tr("The time in milliseconds (ms) is approximative and adapted to a number of frames. The number of frames must be odd or it will be adapted. Check the logger for the exact value."));
  informations->setWordWrap(true);
  informations->setAlignment(Qt::AlignJustify | Qt::AlignTop);
  
  QGridLayout* gridLayout = new QGridLayout(smoothingMethods);
  gridLayout->addWidget(this->rmsMethodButton, 0, 0, 1, 1);
  gridLayout->addWidget(this->rmsWindowWidthSpinBox, 0, 1, 1, 1);
  gridLayout->addWidget(this->rmsUnitComboBox, 0, 2, 1, 1);
  gridLayout->addWidget(this->movagMethodButton, 1, 0, 1, 1);
  gridLayout->addWidget(this->movagWindowWidthSpinBox, 1, 1, 1, 1);
  gridLayout->addWidget(this->movagUnitComboBox, 1, 2, 1, 1);
  QSpacerItem* horizontalSpacer = new QSpacerItem(40, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
  gridLayout->addItem(horizontalSpacer, 0, 3, 1, 1);
  gridLayout->addWidget(informations, 2, 0, 1, 3);
  
#ifndef Q_OS_WIN
  QFont f = this->font();
  f.setPointSize(11);
  this->rmsMethodButton->setFont(f);
  this->rmsUnitComboBox->setFont(f);
  this->movagMethodButton->setFont(f);
  this->movagUnitComboBox->setFont(f);
  this->rmsWindowWidthSpinBox->setStyleSheet("QDoubleSpinBox {font-size: 12px;};");
  this->movagWindowWidthSpinBox->setStyleSheet("QDoubleSpinBox {font-size: 12px;};");
  gridLayout->setVerticalSpacing(0);
  f.setItalic(true);
  informations->setFont(f);
  informations->setText("\n" + informations->text());
#else
  QFont f = this->font();
  f.setItalic(true);
  informations->setFont(f);
#endif

  connect(this->rmsUnitComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(adaptRmsWindowWidthRange(int)));
  connect(this->movagUnitComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(adaptMovagWindowWidthRange(int)));

  this->addOption(tr("Smoothing methods"), smoothingMethods);
  
  // FIXME: Why the dialog box is not able to resize it automatically when using a toolbox?
#ifdef Q_OS_WIN
  int width = 540;
#else
  int width = this->sizeHint().width() - 100; // The spacer item takes too much width ...
#endif
  this->resize(width, this->height());
  this->setMinimumWidth(width);
};

int SmoothAnalogDialog::rmsWindowWidth() const
{
  return this->xWindowWidth(this->rmsWindowWidthSpinBox->value(), this->rmsUnitComboBox->currentIndex());
};

int SmoothAnalogDialog::movagWindowWidth() const
{
  return this->xWindowWidth(this->movagWindowWidthSpinBox->value(), this->movagUnitComboBox->currentIndex());
};

int SmoothAnalogDialog::xWindowWidth(int value, int unit) const
{
  int windowWidth = 0;
  if (unit == 0) // ms
    windowWidth = static_cast<int>(std::floor(static_cast<double>(value) / this->m_AnalogSampleTime));
  else
    windowWidth = value;
  // Be sure to have a window with an odd number of coefficient
#if 1
  windowWidth = windowWidth / 2;
  windowWidth = windowWidth*2 + 1;
#else
  if (windowWidth % 2 == 0) // even number
    windowWidth += 1;
#endif
  return windowWidth;
};

void SmoothAnalogDialog::initializeOptions(const Acquisition* const acq)
{
  this->m_AnalogNumFrames = acq->analogFrameNumber();
  if (acq->analogFrequency() == 0.0) // Should be impossible
  {
    qDebug("Smooth Analog Tool - Null analog frequency. Contact the developer. This case is not possible.");
    this->m_AnalogSampleTime = 10.0; // 1 / 100 Hz * 1000 (ms)
  }
  else
    this->m_AnalogSampleTime = 1000.0 / acq->analogFrequency();
  QSettings settings;
  this->rmsUnitComboBox->setCurrentIndex(settings.value(this->toolSettingsPath() + "rmsWindowWidthUnit", 0).toInt());
  this->movagUnitComboBox->setCurrentIndex(settings.value(this->toolSettingsPath() + "movagWindowWidthUnit", 0).toInt());
  
  int rmsWindowWidthValue = settings.value(this->toolSettingsPath() + "rmsWindowWidthValue", 70).toInt();
  this->rmsWindowWidthSpinBox->setValue(rmsWindowWidthValue);
  
  int movagWindowWidthValue = settings.value(this->toolSettingsPath() + "movagWindowWidthValue", 70).toInt();
  this->movagWindowWidthSpinBox->setValue(movagWindowWidthValue);
  
  int lastMethod =  settings.value(this->toolSettingsPath() + "lastMethod", 0).toInt();
  if (lastMethod == 0)
    this->rmsMethodButton->setChecked(true);
  else
    this->movagMethodButton->setChecked(true);
};

void SmoothAnalogDialog::saveOptionsSettings()
{
  QSettings settings;
  int smoothingMethod = 0;
  if (this->rmsMethodButton->isChecked())
  {
    smoothingMethod = 0;
    settings.setValue(this->toolSettingsPath() + "rmsWindowWidthValue", this->rmsWindowWidthSpinBox->value());
    settings.setValue(this->toolSettingsPath() + "rmsWindowWidthUnit", this->rmsUnitComboBox->currentIndex());
  }
  else
  {
    smoothingMethod = 1;
    settings.setValue(this->toolSettingsPath() + "movagWindowWidthValue", this->movagWindowWidthSpinBox->value());
    settings.setValue(this->toolSettingsPath() + "movagWindowWidthUnit", this->movagUnitComboBox->currentIndex());
  }
  settings.setValue(this->toolSettingsPath() + "lastMethod", smoothingMethod);
};

void SmoothAnalogDialog::adaptRmsWindowWidthRange(int index)
{
  if (index == 0) // ms
  {
    this->rmsWindowWidthSpinBox->setRange(3.0 * this->m_AnalogSampleTime, static_cast<double>(this->m_AnalogNumFrames) * this->m_AnalogSampleTime);
    this->rmsWindowWidthSpinBox->setDecimals(1);
  }
  else
  {
    this->rmsWindowWidthSpinBox->setMinimumWidth(this->rmsWindowWidthSpinBox->width());
    this->rmsWindowWidthSpinBox->setRange(3.0, static_cast<double>(this->m_AnalogNumFrames));
    this->rmsWindowWidthSpinBox->setDecimals(0);
  }
};

void SmoothAnalogDialog::adaptMovagWindowWidthRange(int index)
{
  if (index == 0) // ms
  {
    this->movagWindowWidthSpinBox->setRange(3.0 * this->m_AnalogSampleTime, static_cast<double>(this->m_AnalogNumFrames) * this->m_AnalogSampleTime);
    this->movagWindowWidthSpinBox->setDecimals(1);
  }
  else
  {
    this->movagWindowWidthSpinBox->setMinimumWidth(this->movagWindowWidthSpinBox->width());
    this->movagWindowWidthSpinBox->setRange(3.0, static_cast<double>(this->m_AnalogNumFrames));
    this->movagWindowWidthSpinBox->setDecimals(0);
  }
};
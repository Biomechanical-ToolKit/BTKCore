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

#include "FilterAnalog.h"
#include "FilterAnalogDialog.h"
#include "UndoCommands.h"

#include <btkEigen/SignalProcessing/IIRFilterDesign.h>
#include <btkEigen/SignalProcessing/FiltFilt.h>
#include <btkEigen/SignalProcessing/Filter.h>

#include <QDoubleSpinBox>
#include <QComboBox>

void FilterAnalog::RegisterTool(ToolsManager* manager)
{
  manager->addAnalogTool(tr("Filter Analog (Butterworth)"), ToolFactory<FilterAnalog>);
};

FilterAnalog::FilterAnalog(QWidget* parent)
: AbstractTool("Butterworth Analog Filter", parent)
{};

AbstractTool::RunState FilterAnalog::run(ToolCommands* cmds, ToolsData* const data)
{
  FilterAnalogDialog dialog(this->parentWidget());
  dialog.initialize(data);
  if (dialog.exec() == QDialog::Accepted)
  {
    QString detail = "Filtered by a ";
    detail += (dialog.zeroLagComboBox->currentIndex() == 0) ? "zero lag" : "non-zero lag";
    if (dialog.typeComboBox->currentIndex() == 0)
      detail += QString(" low pass butterworth filter (n = %1, fc = %2 Hz)").arg(dialog.orderSpinBox->value()).arg(dialog.lowerCutOffSpinBox->value());
    else if (dialog.typeComboBox->currentIndex() == 1)
      detail += QString(" high pass butterworth filter (n = %1, fc = %2 Hz)").arg(dialog.orderSpinBox->value()).arg(dialog.lowerCutOffSpinBox->value());
    else
      detail += QString(" band pass butterworth filter (n = %1, fcl = %2 Hz, fcu = %3 Hz)").arg(dialog.orderSpinBox->value()).arg(dialog.lowerCutOffSpinBox->value()).arg(dialog.upperCutOffSpinBox->value());
    
    btk::AnalogCollection::Pointer analogs = btk::AnalogCollection::New();
    QList<int> ids = dialog.extractSelectedAnalogChannels(analogs, "_Filtered", detail, data, cmds);
  
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
    
    Eigen::Matrix<double,Eigen::Dynamic,1> a,b;
    const double hfs = 2.0 * data->acquisition()->pointFrequency();
    int n = dialog.orderSpinBox->value();
    bool zeroLagFilter = (dialog.zeroLagComboBox->currentIndex() == 0);
    
    if (dialog.typeComboBox->currentIndex() == 0)
    {
      double wn = dialog.lowerCutOffSpinBox->value() / hfs;
      if (zeroLagFilter) btkEigen::adjustZeroLagButterworth(n, wn);
      btkEigen::butter(&b, &a, n, wn, btkEigen::LowPass);
    }
    else if (dialog.typeComboBox->currentIndex() == 1)
    {
      double wn = dialog.lowerCutOffSpinBox->value() / hfs;
      if (zeroLagFilter) btkEigen::adjustZeroLagButterworth(n, wn);
      btkEigen::butter(&b, &a, n, wn, btkEigen::HighPass);
    }
    else
    {
      double wn[2] = {dialog.lowerCutOffSpinBox->value() / hfs, dialog.upperCutOffSpinBox->value() / hfs};
      if (zeroLagFilter) btkEigen::adjustZeroLagButterworth(n, &wn);
      btkEigen::butter(&b, &a, n, wn, btkEigen::BandPass);
    }
    
    int inc = 0;
    QSharedPointer< QList<btk::Analog::Values> > values(new QList<btk::Analog::Values>());
    for (btk::AnalogCollection::ConstIterator it = analogs->Begin() ; it != analogs->End() ; ++it)
    {
      if (zeroLagFilter)
        values->push_back(btkEigen::filtfilt(b,a,(*it)->GetValues()));
      else
        values->push_back(btkEigen::filter(b,a,(*it)->GetValues()));
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

FilterAnalogDialog::FilterAnalogDialog(QWidget* parent)
: AnalogToolOptionDialog("Filter Analog", parent)
{
  QWidget* filterDesign = new QWidget(this);
  QLabel* typeLabel = new QLabel(this);
  typeLabel->setText(tr("Type:"));
  this->typeComboBox = new QComboBox(this);
  this->typeComboBox->addItems(QStringList() << tr("Low Pass") << tr("High Pass") << tr("Band Pass"));
  QLabel* cutoffLabel = new QLabel(this);
  cutoffLabel->setText(tr("Cutoff:"));
  this->lowerCutOffSpinBox = new QDoubleSpinBox(this);
  this->lowerCutOffSpinBox->setSuffix(" Hz");
  this->lowerCutOffLabel = new QLabel(this);
  this->lowerCutOffLabel->setText(tr("(lower)"));
  this->upperCutOffSpinBox = new QDoubleSpinBox(this);
  this->upperCutOffSpinBox->setSuffix(" Hz");
  this->upperCutOffLabel = new QLabel(this);
  this->upperCutOffLabel->setText(tr("(upper)"));
  QLabel* orderLabel = new QLabel(this);
  orderLabel->setText(tr("Order:"));
  this->orderSpinBox = new QSpinBox(this);
  this->orderSpinBox->setRange(2,10);
  this->orderSpinBox->setValue(2);
  QLabel* zeroLagLabel = new QLabel(this);
  zeroLagLabel->setText(tr("Zero Lag:"));
  this->zeroLagComboBox = new QComboBox(this);
  this->zeroLagComboBox->addItems(QStringList() << tr("Yes") << tr("No"));
  QLabel* informations = new QLabel(this);
  informations->setText(tr("Note: the order and cutoff(s) are final."));
  informations->setWordWrap(true);
  informations->setAlignment(Qt::AlignJustify | Qt::AlignBottom);
  
  QHBoxLayout* hboxLayout1 = new QHBoxLayout;
  hboxLayout1->addWidget(this->typeComboBox);
  QSpacerItem* horizontalSpacer1 = new QSpacerItem(40, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
  hboxLayout1->addItem(horizontalSpacer1);
  
  QHBoxLayout* hboxLayout2 = new QHBoxLayout;
  hboxLayout2->addWidget(this->orderSpinBox);
  QSpacerItem* horizontalSpacer2 = new QSpacerItem(40, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
  hboxLayout2->addItem(horizontalSpacer2);
  
  QHBoxLayout* hboxLayout3 = new QHBoxLayout;
  hboxLayout3->addWidget(this->lowerCutOffSpinBox);
  hboxLayout3->addWidget(lowerCutOffLabel);
  QSpacerItem* horizontalSpacer3 = new QSpacerItem(40, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
  hboxLayout3->addItem(horizontalSpacer3);
  
  QHBoxLayout* hboxLayout4 = new QHBoxLayout;
  hboxLayout4->addWidget(this->upperCutOffSpinBox);
  hboxLayout4->addWidget(upperCutOffLabel);
  QSpacerItem* horizontalSpacer4 = new QSpacerItem(40, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
  hboxLayout4->addItem(horizontalSpacer4);
  
  QHBoxLayout* hboxLayout5 = new QHBoxLayout;
  hboxLayout5->addWidget(this->zeroLagComboBox);
  QSpacerItem* horizontalSpacer5 = new QSpacerItem(40, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
  hboxLayout5->addItem(horizontalSpacer5);
  
  QFrame* line = new QFrame(this);
  line->setObjectName(QString::fromUtf8("line"));
  line->setGeometry(QRect(320, 150, 118, 3));
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
#ifndef Q_OS_WIN
  line->setMinimumHeight(12);
#endif
  
  QGridLayout* gridLayout = new QGridLayout(filterDesign);
  gridLayout->addWidget(typeLabel, 0, 0, 1, 1);
  gridLayout->addLayout(hboxLayout1, 0, 1, 1, 1);
  gridLayout->addWidget(zeroLagLabel, 1, 0, 1, 1);
  gridLayout->addLayout(hboxLayout5, 1, 1, 1, 1);
  gridLayout->addWidget(line, 2, 0, 1, -1);
  gridLayout->addWidget(informations, 3, 0, 1, -1);
  gridLayout->addWidget(orderLabel, 4, 0, 1, 1);
  gridLayout->addLayout(hboxLayout2, 4, 1, 1, 1);
  gridLayout->addWidget(cutoffLabel, 5, 0, 1, 1);
  gridLayout->addLayout(hboxLayout3, 5, 1, 1, 1);
  gridLayout->addLayout(hboxLayout4, 6, 1, 1, 1);
  QSpacerItem* verticalSpacer = new QSpacerItem(10, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
  gridLayout->addItem(verticalSpacer, 7, 1, 1, 1);
  
#ifndef Q_OS_WIN
  QFont f = this->font();
  f.setPointSize(11);
  typeLabel->setFont(f);
  this->typeComboBox->setFont(f);
  cutoffLabel->setFont(f);
  lowerCutOffLabel->setFont(f);
  upperCutOffLabel->setFont(f);
  this->zeroLagComboBox->setFont(f);
  orderLabel->setFont(f);
  this->orderSpinBox->setFont(f);
  zeroLagLabel->setFont(f);
  this->lowerCutOffSpinBox->setStyleSheet("QDoubleSpinBox {font-size: 12px;};");
  this->upperCutOffSpinBox->setStyleSheet("QDoubleSpinBox {font-size: 12px;};");
  this->orderSpinBox->setStyleSheet("QSpinBox {font-size: 12px;};");
  gridLayout->setVerticalSpacing(3);
  f.setItalic(true);
  informations->setFont(f);
  informations->setStyleSheet("margin-bottom: 3px;");
#else
  QFont f = this->font();
  f.setItalic(true);
  informations->setFont(f);
#endif
  connect(this->typeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(adaptCutoffDisplay(int)));
  this->addOption(tr("Butterworth filter design"), filterDesign);
};

void FilterAnalogDialog::initializeOptions(const Acquisition* const acq)
{
  QSettings settings;
  this->typeComboBox->setCurrentIndex(settings.value(this->toolSettingsPath() + "lastType", 0).toInt()); // 0: Low pass
  this->orderSpinBox->setValue(settings.value(this->toolSettingsPath() + "lastOrder", 2).toInt());
  this->zeroLagComboBox->setCurrentIndex(settings.value(this->toolSettingsPath() + "lastZeroLag", 0).toInt()); // 0: Yes
  this->adaptCutoffDisplay(this->typeComboBox->currentIndex());
  this->lowerCutOffSpinBox->setRange(0.01, acq->pointFrequency() / 2.0);
  this->upperCutOffSpinBox->setRange(0.01, acq->pointFrequency() / 2.0);
};

void FilterAnalogDialog::saveOptionsSettings()
{
  int index = this->typeComboBox->currentIndex();
  QSettings settings;
  settings.setValue(this->toolSettingsPath() + "lastType", index);
  settings.setValue(this->toolSettingsPath() + "lastOrder", this->orderSpinBox->value());
  settings.setValue(this->toolSettingsPath() + "lastZeroLag", this->zeroLagComboBox->currentIndex());
  if (index == 0) // Low pass
  {
    settings.setValue(this->toolSettingsPath() + "lowPassCutoffFrequency", this->lowerCutOffSpinBox->value());
  }
  else if (index == 1) // High pass
  {
    settings.setValue(this->toolSettingsPath() + "highPassCutoffFrequency", this->lowerCutOffSpinBox->value());
  }
  else // Band pass
  {
    settings.setValue(this->toolSettingsPath() + "bandPassLowerCutoffFrequency", this->lowerCutOffSpinBox->value());
    settings.setValue(this->toolSettingsPath() + "bandPassUpperCutoffFrequency", this->upperCutOffSpinBox->value());
  }
};

void FilterAnalogDialog::adaptCutoffDisplay(int index)
{
  QSettings settings;
  if (index == 0) // Low pass
  {
    this->lowerCutOffLabel->setVisible(false);
    this->upperCutOffSpinBox->setVisible(false);
    this->upperCutOffLabel->setVisible(false);
    this->lowerCutOffSpinBox->setValue(settings.value(this->toolSettingsPath() + "lowPassCutoffFrequency", 6.0).toDouble());
  }
  else if (index == 1) // High pass
  {
    this->lowerCutOffLabel->setVisible(false);
    this->upperCutOffSpinBox->setVisible(false);
    this->upperCutOffLabel->setVisible(false);
    this->lowerCutOffSpinBox->setValue(settings.value(this->toolSettingsPath() + "highPassCutoffFrequency", 20.0).toDouble());
  }
  else // Band pass
  {
    this->lowerCutOffLabel->setVisible(true);
    this->upperCutOffSpinBox->setVisible(true);
    this->upperCutOffLabel->setVisible(true);
    this->lowerCutOffSpinBox->setValue(settings.value(this->toolSettingsPath() + "bandPassLowerCutoffFrequency", 10.0).toDouble());
    this->upperCutOffSpinBox->setValue(settings.value(this->toolSettingsPath() + "bandPassUpperCutoffFrequency", 500.0).toDouble());
  }
};

bool FilterAnalogDialog::testOptionsValidity()
{
  if (this->upperCutOffSpinBox->isVisible() && (this->lowerCutOffSpinBox->value() > this->upperCutOffSpinBox->value()))
    return false;
  return true;
};
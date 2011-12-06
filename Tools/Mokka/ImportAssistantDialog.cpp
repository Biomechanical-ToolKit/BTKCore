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

#include "ImportAssistantDialog.h"

#include <QFileDialog>
#include <QLineEdit>
#include <QButtonGroup>

ImportAssistantDialog::ImportAssistantDialog(QWidget* parent)
: QDialog(parent), m_Directory("")
{
  this->setupUi(this);
  QButtonGroup* acquisitonRadioButtonGroup = new QButtonGroup(this);
  acquisitonRadioButtonGroup->addButton(this->newAcquisitionRadioButton);
  acquisitonRadioButtonGroup->addButton(this->appendAcquisitionRadioButton);
  QButtonGroup* firstFrameRadioButtonGroup = new QButtonGroup(this);
  firstFrameRadioButtonGroup->addButton(this->keepAllFrameRadioButton);
  firstFrameRadioButtonGroup->addButton(this->keepHighestFirstFrameRadioButton);
  
#ifdef Q_OS_MAC
  this->setWindowFlags(Qt::Sheet);
  this->setWindowModality(Qt::WindowModal);
  this->resize(this->width(), this->height()-1); // FIXME: Only the way to remove the size grip under MacOS X?
#endif
  connect(this->acquisitionSystemComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setAcquisitionSystem(int)));
  connect(this->motionTrajectoryButton, SIGNAL(clicked()), this, SLOT(openMotionTrajectoryFileDialog()));
  connect(this->motionAnalogButton, SIGNAL(clicked()), this, SLOT(openMotionAnalogFileDialog()));
  connect(this->motionForcePlatformButton, SIGNAL(clicked()), this, SLOT(openMotionForcePlatformFileDialog()));
  connect(this->motionOrthoTrakButton, SIGNAL(clicked()), this, SLOT(openMotionOrthoTrakFileDialog()));
  connect(this->eliteTrajectoryButton, SIGNAL(clicked()), this, SLOT(openEliteTrajectoryFileDialog()));
  connect(this->eliteForcePlatformButton, SIGNAL(clicked()), this, SLOT(openEliteForcePlatformFileDialog()));
  connect(this->eliteEMGButton, SIGNAL(clicked()), this, SLOT(openEliteEMGFileDialog()));
  connect(this->eliteAngleButton, SIGNAL(clicked()), this, SLOT(openEliteAngleFileDialog()));
  connect(this->eliteMomentButton, SIGNAL(clicked()), this, SLOT(openEliteMomentFileDialog()));
  connect(this->elitePowerButton, SIGNAL(clicked()), this, SLOT(openElitePowerFileDialog()));
};

void ImportAssistantDialog::clear(const QString& dir)
{
  this->m_Directory = dir;
  this->newAcquisitionRadioButton->setChecked(true);
  this->acquisitionSystemComboBox->setCurrentIndex(-1);
  this->keepAllFrameRadioButton->setChecked(true);
  this->stackedWidget->setCurrentIndex(0);
  this->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
  // Motion page
  this->motionTrajectoryLineEdit->clear();
  this->motionAnalogLineEdit->clear();
  this->motionForcePlatformLineEdit->clear();
  this->motionOrthoTrakLineEdit->clear();
  // Elite page
  this->eliteTrajectoryLineEdit->clear();
  this->eliteForcePlatformLineEdit->clear();
  this->eliteEMGLineEdit->clear();
  this->eliteAngleLineEdit->clear();
  this->eliteMomentLineEdit->clear();
  this->elitePowerLineEdit->clear();
};

QStringList ImportAssistantDialog::filenames() const
{
  QStringList files;
  if (acquisitionSystemComboBox->currentIndex() == 0) // Motion
  {
    files << this->motionTrajectoryLineEdit->text().split(",", QString::SkipEmptyParts)
          << this->motionAnalogLineEdit->text().split(",", QString::SkipEmptyParts)
          << this->motionForcePlatformLineEdit->text().split(",", QString::SkipEmptyParts)
          << this->motionOrthoTrakLineEdit->text().split(",", QString::SkipEmptyParts);
  }
  else // Elite
  {
    files << this->eliteTrajectoryLineEdit->text().split(",", QString::SkipEmptyParts)
          << this->eliteForcePlatformLineEdit->text().split(",", QString::SkipEmptyParts)
          << this->eliteEMGLineEdit->text().split(",", QString::SkipEmptyParts)
          << this->eliteAngleLineEdit->text().split(",", QString::SkipEmptyParts)
          << this->eliteMomentLineEdit->text().split(",", QString::SkipEmptyParts)
          << this->elitePowerLineEdit->text().split(",", QString::SkipEmptyParts);
  }
  return files;
};

void ImportAssistantDialog::setAcquisitionSystem(int index)
{
  this->stackedWidget->setCurrentIndex(index+1);
  this->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
};

void ImportAssistantDialog::openMotionTrajectoryFileDialog()
{
  this->openFileDialog(tr("Trajectory Files (*.trb *.trc)"), this->motionTrajectoryLineEdit);
};

void ImportAssistantDialog::openMotionAnalogFileDialog()
{
  this->openFileDialog(tr("Analog Files (*.anb *.anc)"), this->motionAnalogLineEdit);
};

void ImportAssistantDialog::openMotionForcePlatformFileDialog()
{
  this->openFileDialog(tr("Force Platform Calibration Files (*.cal)"), this->motionForcePlatformLineEdit);
};

void ImportAssistantDialog::openMotionOrthoTrakFileDialog()
{
  this->openFileDialog(tr("OrthoTrak XLS Files (*.xls)"), this->motionOrthoTrakLineEdit);
};

void ImportAssistantDialog::openEliteTrajectoryFileDialog()
{
  this->openFileDialog(tr("Trajectory Files (*.rah *.raw *.ric *.rif)"), this->eliteTrajectoryLineEdit);
};

void ImportAssistantDialog::openEliteForcePlatformFileDialog()
{
  QStringList filenames = QFileDialog::getOpenFileNames(this, "",
                            this->m_Directory,
                            tr("Force Platform Files (*.gr*)"));
  if (!filenames.isEmpty())
  {
    this->m_Directory = QFileInfo(filenames[0]).absolutePath();
    this->eliteForcePlatformLineEdit->setText(filenames.join(","));
  }
};

void ImportAssistantDialog::openEliteEMGFileDialog()
{
  this->openFileDialog(tr("EMG Files (*.emg)"), this->eliteEMGLineEdit);
};

void ImportAssistantDialog::openEliteAngleFileDialog()
{
  this->openFileDialog(tr("Joint Angle Files (*.ang)"), this->eliteAngleLineEdit);
};

void ImportAssistantDialog::openEliteMomentFileDialog()
{
  this->openFileDialog(tr("Joint Moment Files (*.mom)"), this->eliteMomentLineEdit);
};

void ImportAssistantDialog::openElitePowerFileDialog()
{
  this->openFileDialog(tr("Joint Power Files (*.pwr)"), this->elitePowerLineEdit);
};

void ImportAssistantDialog::openFileDialog(const QString& filter, QLineEdit* lineEdit)
{
  QString filename = QFileDialog::getOpenFileName(this, "",
                       this->m_Directory,
                       filter);
  if (!filename.isEmpty())
  {
    this->m_Directory = QFileInfo(filename).absolutePath();
    lineEdit->setText(filename);
  }
};
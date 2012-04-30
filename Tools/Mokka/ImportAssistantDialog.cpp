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

#include "ImportAssistantDialog.h"

#include <QFileDialog>
#include <QLineEdit>
#include <QButtonGroup>

ImportAssistantDialog::ImportAssistantDialog(QWidget* parent)
: QDialog(parent), m_Directory(""), mp_AMTIFileIOCache()
{
  this->setupUi(this);
  QButtonGroup* acquisitonRadioButtonGroup = new QButtonGroup(this);
  acquisitonRadioButtonGroup->addButton(this->newAcquisitionRadioButton);
  acquisitonRadioButtonGroup->addButton(this->appendAcquisitionRadioButton);
  QButtonGroup* firstFrameRadioButtonGroup = new QButtonGroup(this);
  firstFrameRadioButtonGroup->addButton(this->keepAllFrameRadioButton);
  firstFrameRadioButtonGroup->addButton(this->keepHighestFirstFrameRadioButton);
  
  this->videoFileTableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
  this->videoFileTableWidget->horizontalHeader()->setResizeMode(1, QHeaderView::Fixed);
  this->videoFileTableWidget->horizontalHeader()->resizeSection(1, 24);
  this->videoFileTableWidget->horizontalHeader()->setResizeMode(2, QHeaderView::Fixed);
  this->videoFileTableWidget->horizontalHeader()->resizeSection(2, 24);
  
  this->m_OpenFileIcon = QIcon(QString::fromUtf8(":/Resources/Images/folder.png"));
  this->m_DeleteFileIcon = QIcon(QString::fromUtf8(":/Resources/Images/plot_delete.png"));
  
#ifdef Q_OS_MAC
  this->layout()->setContentsMargins(12,12,12,12);
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
  connect(this->amtiForceMomentButton, SIGNAL(clicked()), this, SLOT(openAmtiFileDialog()));
  connect(this->amtiInformationsComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setAmtiInformationUsed(int)));
  connect(this->videoFileTableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(editVideoFileTable(int, int)));
};

void ImportAssistantDialog::clear(const QString& dir)
{
  this->m_Directory = dir;
  
  this->setWindowTitle(tr("Assistant"));
  
  this->acquisitionSystemComboBox->setEnabled(true);
  this->newAcquisitionRadioButton->setEnabled(true);
  this->appendAcquisitionRadioButton->setEnabled(true);
  this->keepAllFrameRadioButton->setEnabled(true);
  this->keepHighestFirstFrameRadioButton->setEnabled(true);

  this->acquisitionSystemComboBox->setCurrentIndex(-1);
  this->newAcquisitionRadioButton->setChecked(true);
  this->keepAllFrameRadioButton->setChecked(true);
  
  this->importOptionFrame->setVisible(true);
  
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
  // AMTI page
  this->amtiForceMomentLineEdit->clear();
  this->amtiForceMomentLineEdit->setReadOnly(false);
  this->amtiForceMomentButton->setEnabled(true);
  // Video page
  this->videoFileTableWidget->clearContents();
  this->videoFileTableWidget->setRowCount(0);
  this->addVideoFile("");
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
  else if (acquisitionSystemComboBox->currentIndex() == 1) // Elite
  {
    files << this->eliteTrajectoryLineEdit->text().split(",", QString::SkipEmptyParts)
          << this->eliteForcePlatformLineEdit->text().split(",", QString::SkipEmptyParts)
          << this->eliteEMGLineEdit->text().split(",", QString::SkipEmptyParts)
          << this->eliteAngleLineEdit->text().split(",", QString::SkipEmptyParts)
          << this->eliteMomentLineEdit->text().split(",", QString::SkipEmptyParts)
          << this->elitePowerLineEdit->text().split(",", QString::SkipEmptyParts);
  }
  else if (acquisitionSystemComboBox->currentIndex() == 2) // AMTI
  {
    files << this->amtiForceMomentLineEdit->text().split(",", QString::SkipEmptyParts);
  }
  else
  {
    for (int i = 0 ; i < this->videoFileTableWidget->rowCount()-1 ; ++i)
      files << this->videoFileTableWidget->item(i,0)->text();
  }
  return files;
};

QList<QVariant> ImportAssistantDialog::amtiDimensions()
{
  QList< QVariant> dims;
  dims << this->amtiWidthSpinBox->value()
       << this->amtiLengthSpinBox->value()
       << this->amtiHeightSpinBox->value();
  return dims;
};

void ImportAssistantDialog::setAmtiDimensions(const QList<QVariant>& dims)
{
  if (dims.isEmpty())
  {
    btk::AMTIForcePlatformFileIO::Pointer io = this->amtiFileIOCache();
    this->amtiWidthSpinBox->setValue(io->GetDimensions()[0]);
    this->amtiLengthSpinBox->setValue(io->GetDimensions()[1]);
    this->amtiHeightSpinBox->setValue(io->GetDimensions()[2]);
  }
  else
  {
    this->amtiWidthSpinBox->setValue(dims[0].toFloat());
    this->amtiLengthSpinBox->setValue(dims[1].toFloat());
    this->amtiHeightSpinBox->setValue(dims[2].toFloat());
  }
};

QList<QVariant> ImportAssistantDialog::amtiOrigin()
{
  QList<QVariant> origin;
  origin << this->amtiOriginX->value()
         << this->amtiOriginY->value()
         << this->amtiOriginZ->value();
  return origin;
};

QList<QVariant> ImportAssistantDialog::amtiCorners()
{
  QList<QVariant> corners;
  corners << this->amtiC1X->value()
          << this->amtiC1Y->value()
          << this->amtiC1Z->value()
          << this->amtiC2X->value()
          << this->amtiC2Y->value()
          << this->amtiC2Z->value()
          << this->amtiC3X->value()
          << this->amtiC3Y->value()
          << this->amtiC3Z->value()
          << this->amtiC4X->value()
          << this->amtiC4Y->value()
          << this->amtiC4Z->value();
  return corners;
};

void ImportAssistantDialog::setAmtiGeometry(const QList<QVariant>& corners, const QList<QVariant>& origin)
{
  if (corners.isEmpty())
  {
    btk::AMTIForcePlatformFileIO::Pointer io = this->amtiFileIOCache();
    this->amtiC1X->setValue(io->GetCorners()[0]);
    this->amtiC1Y->setValue(io->GetCorners()[1]);
    this->amtiC1Z->setValue(io->GetCorners()[2]);
    this->amtiC2X->setValue(io->GetCorners()[3]);
    this->amtiC2Y->setValue(io->GetCorners()[4]);
    this->amtiC2Z->setValue(io->GetCorners()[5]);
    this->amtiC3X->setValue(io->GetCorners()[6]);
    this->amtiC3Y->setValue(io->GetCorners()[7]);
    this->amtiC3Z->setValue(io->GetCorners()[8]);
    this->amtiC4X->setValue(io->GetCorners()[9]);
    this->amtiC4Y->setValue(io->GetCorners()[10]);
    this->amtiC4Z->setValue(io->GetCorners()[11]);
  }
  else
  {
    this->amtiC1X->setValue(corners[0].toFloat());
    this->amtiC1Y->setValue(corners[1].toFloat());
    this->amtiC1Z->setValue(corners[2].toFloat());
    this->amtiC2X->setValue(corners[3].toFloat());
    this->amtiC2Y->setValue(corners[4].toFloat());
    this->amtiC2Z->setValue(corners[5].toFloat());
    this->amtiC3X->setValue(corners[6].toFloat());
    this->amtiC3Y->setValue(corners[7].toFloat());
    this->amtiC3Z->setValue(corners[8].toFloat());
    this->amtiC4X->setValue(corners[9].toFloat());
    this->amtiC4Y->setValue(corners[10].toFloat());
    this->amtiC4Z->setValue(corners[11].toFloat());
  }
  
  if (origin.isEmpty())
  {
    btk::AMTIForcePlatformFileIO::Pointer io = this->amtiFileIOCache();
    this->amtiOriginX->setValue(io->GetOrigin()[0]);
    this->amtiOriginY->setValue(io->GetOrigin()[1]);
    this->amtiOriginZ->setValue(io->GetOrigin()[2]);
  }
  else
  {
    this->amtiOriginX->setValue(origin[0].toFloat());
    this->amtiOriginY->setValue(origin[1].toFloat());
    this->amtiOriginZ->setValue(origin[2].toFloat());
  }
};

void ImportAssistantDialog::setAcquisitionSystem(int index)
{
  this->stackedWidget->setCurrentIndex(index+1);
  this->buttonBox->button(QDialogButtonBox::Ok)->setEnabled((index == -1) ? false : true);
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

void ImportAssistantDialog::openAmtiFileDialog()
{
  this->openFileDialog(tr("AMTI Force & Moment Files (*.asc)"), this->amtiForceMomentLineEdit);
};

void ImportAssistantDialog::setAmtiInformationUsed(int index)
{
  this->amtiStackedWidget->setCurrentIndex(index);
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

btk::AMTIForcePlatformFileIO::Pointer ImportAssistantDialog::amtiFileIOCache()
{
  if (!this->mp_AMTIFileIOCache)
    this->mp_AMTIFileIOCache = btk::AMTIForcePlatformFileIO::New();
  return this->mp_AMTIFileIOCache;
};

void ImportAssistantDialog::editVideoFileTable(int row, int column)
{
  if (column == 0)
    return;
  this->videoFileTableWidget->blockSignals(true);
  if (column == 1) // Open dialog
  {
    if (row != this->videoFileTableWidget->rowCount() - 1) // Set file
    {
      QString filename = QFileDialog::getOpenFileName(this, "",
                                  this->videoFileTableWidget->item(row,0)->text(),
                                  tr("All Files (*)"));
      if (!filename.isEmpty())
      {
        this->m_Directory = QFileInfo(filename).absolutePath();
        this->videoFileTableWidget->item(row,0)->setText(filename);
      }
    }
    else
    {
      QStringList filenames = QFileDialog::getOpenFileNames(this, "",
                                this->m_Directory,
                                tr("All Files (*)"));
      if (!filenames.isEmpty())
      {
        this->m_Directory = QFileInfo(filenames[0]).absolutePath();
        for (QStringList::const_iterator it = filenames.begin() ; it != filenames.end() ; ++it)
          this->addVideoFile(*it);
        this->videoFileTableWidget->removeRow(row); // Empty
        this->addVideoFile("");
      }
    }
  }
  else if (column == 2) // Remove row
  {
    QTableWidgetItem* item = this->videoFileTableWidget->item(row, 2);
    if ((item->flags() & Qt::ItemIsEnabled) == Qt::ItemIsEnabled)
    {
      if (this->videoFileTableWidget->rowCount() != 1)
        this->videoFileTableWidget->removeRow(row);
      else
      {
        this->videoFileTableWidget->item(row, 0)->setText("");
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
      }
    }
  }
  this->videoFileTableWidget->blockSignals(false);
};

void ImportAssistantDialog::addVideoFile(const QString& filename)
{
  QTableWidgetItem* item;
  int index = this->videoFileTableWidget->rowCount();
  this->videoFileTableWidget->insertRow(index);
  item = new QTableWidgetItem; this->videoFileTableWidget->setItem(index,1,item);
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  item->setIcon(this->m_OpenFileIcon);
  item = new QTableWidgetItem; this->videoFileTableWidget->setItem(index,2,item);
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  item->setIcon(this->m_DeleteFileIcon);
  if (filename.isEmpty())
  {
    item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
    item = new QTableWidgetItem("Click on the folder icon to append video files.");
    item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
    QFont f = item->font();
    f.setItalic(true);
    item->setFont(f);
  }
  else
    item = new QTableWidgetItem(filename);
  this->videoFileTableWidget->setItem(index,0,item);
};
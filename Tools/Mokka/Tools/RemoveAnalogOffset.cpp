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

#include "RemoveAnalogOffset.h"
#include "RemoveAnalogOffsetDialog.h"
#include "AcquisitionTool.h"
#include "UndoCommands.h"

#include <btkAnalogOffsetRemover.h>
#include <btkSubAcquisitionFilter.h>
#include <btkAcquisitionFileReader.h>

#include <QFileDialog>

RemoveAnalogOffset::RemoveAnalogOffset(Method m, QWidget* parent)
: AcquisitionTool("Remove Analog Offset", parent)
{
  this->m_Method = m;
};
  
bool RemoveAnalogOffset::run(QUndoCommand* acquisitionParentCmd, Acquisition* const acq)
{
  bool res = false;
  QVector<int> ids;
  btk::AnalogCollection::Pointer analogs;
  
  if (this->m_Method == FromReferenceFile)
  {
    QStringList formats;
    acq->supportedReadFileFormats(formats);
    QStringList allFormats;
    QString byFormat;
    foreach(const QString& str, formats)
    {
      QString ext = "*." + str.split(" ")[0].toLower();
      if (allFormats.indexOf(ext) == -1)
        allFormats.append(ext);
      byFormat += (!byFormat.isEmpty() ? ";;" : "") + str + " (" + ext + ")";
    }
    QString allFormat = "Acquisition Files (" + allFormats.join(" ") + ");;";
    QString filename = QFileDialog::getOpenFileName(this->parentWidget(), "", QFileInfo(acq->fileName()).absolutePath(), allFormat + byFormat);
    if (!filename.isEmpty())
    {
      QMessageBox error(QMessageBox::Warning, "File error", "Error occurred during the reading of the reference file.", QMessageBox::Ok, this->parentWidget());
      error.setInformativeText("<nobr>Check the logger for more informations.</nobr>");
      
      QList<int> channelsIds;
      btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
      reader->SetFilename(filename.toStdString());
      try
      {
        reader->Update();
        analogs = btk::AnalogCollection::New();
        int id = 0;
        for (btk::AnalogCollection::ConstIterator it = acq->btkAcquisition()->BeginAnalog() ; it != acq->btkAcquisition()->EndAnalog() ; ++it)
        {
          if (reader->GetOutput()->FindAnalog((*it)->GetLabel()) != reader->GetOutput()->EndAnalog())
          {
            analogs->InsertItem(*it);
            channelsIds.push_back(id);
          }
          ++id;
        }
      }
      catch (btk::Exception& e)
      {
        LOG_ERROR(e.what());
        error.exec();
        return false;
      }
      catch (std::exception& e)
      {
        LOG_ERROR("Unexpected error: " + QString(e.what()));
        error.exec();
        return false;
      }
      catch (...)
      {
        LOG_ERROR("Unknown error.");
        error.exec();
        return false;
      }
      ids = channelsIds.toVector();
    }
  }
  else if (this->m_Method == FromSelectedFrames)
  {
    RemoveAnalogOffsetDialog dialog(this->parentWidget());
    dialog.initialize(acq);
    if (dialog.exec() == QDialog::Accepted)
    {
      QSettings settings;
      QStringList lastChannelsSelection;
      
      QList<int> channelsIds;
      QTreeWidgetItem* analogsRoot = dialog.treeWidget->topLevelItem(0);
      for (int i = 0 ; i < analogsRoot->childCount() ; ++i)
      {
        QTreeWidgetItem* analogItem = analogsRoot->child(i);
        if (analogItem->checkState(0) == Qt::Checked)
        {
          channelsIds.push_back(analogItem->data(0, Qt::UserRole).toInt());
          lastChannelsSelection.push_back(analogItem->text(0));
        }
      }
      
      int lastReferenceFrames = -1;
      int framesIndex[2] = {-1,-1};
      if (dialog.firstFramesButton->isChecked())
      {
        lastReferenceFrames = 0;
        int numberOfFrames = dialog.firstFramesSpinBox->value();
        settings.setValue("Tools/RemoveAnalogOffset/firstFramesNumber", numberOfFrames);
        framesIndex[0] = 0;
        framesIndex[1] = numberOfFrames - 1;
      }
      else if (dialog.lastFramesButton->isChecked())
      {
        lastReferenceFrames = 1;
        int numberOfFrames = dialog.lastFramesSpinBox->value();
        settings.setValue("Tools/RemoveAnalogOffset/lastFramesNumber", numberOfFrames);
        framesIndex[0] = acq->lastFrame() - numberOfFrames + 1 - acq->firstFrame();
        framesIndex[1] = acq->lastFrame() - acq->firstFrame();
      }
      settings.setValue("Tools/RemoveAnalogOffset/lastChannelsSelection", lastChannelsSelection);
      settings.setValue("Tools/RemoveAnalogOffset/lastReferenceFrames", lastReferenceFrames);
      
      btk::SubAcquisitionFilter::Pointer subAnalogs = btk::SubAcquisitionFilter::New();
      subAnalogs->SetInput(acq->btkAcquisition());
      subAnalogs->SetFramesIndex(framesIndex[0], framesIndex[1]);
      subAnalogs->SetExtractionOption(btk::SubAcquisitionFilter::AnalogsOnly, channelsIds.toStdList());
      subAnalogs->Update();
      analogs = subAnalogs->GetOutput()->GetAnalogs();
      ids = channelsIds.toVector();
    }
  }
  else
  {
    TOOL_LOG_ERROR("Unknown method.");
    return false;
  }

  if (ids.count() == analogs->GetItemNumber())
  {
    QString log;
    if (ids.count() == 1)
      log = "A total of " + QString::number(ids.count()) + " offset was removed for the following channel:";
    else
      log = "A total of " + QString::number(ids.count()) + " offsets were removed for the following channels:";
    QVector<double> offsets(ids.count());
    int inc = 0;
    for (btk::AnalogCollection::ConstIterator it = analogs->Begin() ; it != analogs->End() ; ++it)
    {
      double dc = (*it)->GetValues().sum() / (*it)->GetValues().rows();
      offsets[inc] = -1.0 * dc; // Because the undo command make only a positive shift
      log += "\n\t- " + QString::fromStdString((*it)->GetLabel()) + ": " + QString::number(dc) + " " + QString::fromStdString((*it)->GetUnit());
      ++inc;
    }
    new ShiftAnalogsValues(acq, ids, offsets, acquisitionParentCmd);
    res = true;
    TOOL_LOG_INFO(log);
  }
  else
    TOOL_LOG_ERROR("Unexpected error: The number of IDs is not equal to the number of offsets. Please contact the developers.");

  return res;
};

// ------------------------------------------------------------------------- //

RemoveAnalogOffsetDialog::RemoveAnalogOffsetDialog(QWidget* parent)
: QDialog(parent)
{
  this->setupUi(this);
#ifdef Q_OS_MAC
  this->layout()->setContentsMargins(12,12,12,12);
  QFont f = this->font();
  f.setPointSize(10);
  this->treeWidget->setFont(f);
  f.setPointSize(11);
  this->firstFramesButton->setFont(f);
  this->lastFramesButton->setFont(f);
  this->allFramesButton->setFont(f);
  this->firstFramesSpinBox->setStyleSheet("QSpinBox {font-size: 12px;};");
  this->lastFramesSpinBox->setStyleSheet("QSpinBox {font-size: 12px;};");
  this->allFramesButton->setMinimumHeight(30);
  this->referenceFramesGroupBox->layout()->setSpacing(6);
  this->referenceFramesGroupBox->layout()->setContentsMargins(12,9,12,9);
#endif
  
  connect(this->treeWidget, SIGNAL(itemChanged(QTreeWidgetItem* , int)), this, SLOT(checkAnalogSelection()));
};

void RemoveAnalogOffsetDialog::initialize(const Acquisition* const acq)
{
  QSettings settings;
  QStringList lastChannelsSelection = settings.value("Tools/RemoveAnalogOffset/lastChannelsSelection").toStringList();
  int lastReferenceFrames =  settings.value("Tools/RemoveAnalogOffset/lastReferenceFrames", 0).toInt();
  QIcon analogIcon(QString::fromUtf8(":/Resources/Images/chart_line.png"));
  
  QFont f = this->treeWidget->font();
  f.setBold(true);

  QTreeWidgetItem* analogsRoot = new QTreeWidgetItem(QStringList(QString("Analog channels")));
  analogsRoot->setFont(0, f);
  analogsRoot->setIcon(0, analogIcon);
  analogsRoot->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsTristate);
  this->treeWidget->addTopLevelItem(analogsRoot);
  for (QMap<int, Analog*>::const_iterator it = acq->analogs().begin() ; it != acq->analogs().end() ; ++it)
  {
    QTreeWidgetItem* analogItem = new QTreeWidgetItem(QStringList(it.value()->label));
    analogItem->setIcon(0, analogIcon);
    analogItem->setData(0, Qt::UserRole, it.key());
    analogItem->setCheckState(0, lastChannelsSelection.indexOf(it.value()->label) != -1 ? Qt::Checked : Qt::Unchecked);
    analogItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    analogsRoot->addChild(analogItem);
  }
  analogsRoot->setExpanded(true);
  
  if (analogsRoot->checkState(0) == Qt::Unchecked) // No analog channel founds
    analogsRoot->setCheckState(0, Qt::Checked);
  
  if (acq->analogs().count() > 12)
    this->resize(this->width(),640);
  
  this->firstFramesSpinBox->setMaximum(acq->pointFrameNumber());
  this->lastFramesSpinBox->setMaximum(acq->pointFrameNumber());
  this->firstFramesSpinBox->setValue(settings.value("Tools/RemoveAnalogOffset/firstFramesNumber", 10).toInt());
  this->lastFramesSpinBox->setValue(settings.value("Tools/RemoveAnalogOffset/lastFramesNumber", 10).toInt());
  
  if (lastReferenceFrames == 1)
    this->lastFramesButton->setChecked(true);
  else if (lastReferenceFrames == 2)
    this->allFramesButton->setChecked(true);
  else
    this->firstFramesButton->setChecked(true);
};

void RemoveAnalogOffsetDialog::checkAnalogSelection()
{
  this->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(this->treeWidget->topLevelItem(0)->checkState(0) != Qt::Unchecked);
};
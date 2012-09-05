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

#include "GaitEventAssistantDialog.h"
#include "Acquisition.h"
#include "LoggerMessage.h"
#include "UndoCommands.h"

#include <btkVerticalGroundReactionForceGaitEventDetector.h>

#include <QSettings>
#include <QPropertyAnimation>
  
GaitEventAssistantDialog::GaitEventAssistantDialog(QWidget* parent)
: QDialog(parent)
{
  this->mp_ResizeDetectionOptionAnimation = new QPropertyAnimation(this, "size");
  this->mp_ResizeDetectionOptionAnimation->setDuration(150);
  this->setupUi(this);
#ifdef Q_OS_MAC
  this->layout()->setContentsMargins(12,12,12,12);
  this->layout()->setSpacing(6);
  this->manualMappingPage->layout()->setContentsMargins(6,6,6,6);
  QFont f = this->font();
  f.setPixelSize(11);
  this->framesInterestLabel->setFont(f); this->framesInterestLabel->setVisible(false);
  this->manualMappingTable->setFont(f);
  this->manualMappingLabel->setFont(f);
  this->verticalForceThresholdLabel->setFont(f);
  this->verticalForceThresholdSpinBox->setFont(f);
  this->detectionOptionsButton->setFont(f);
  this->manualMappingTable->setAttribute(Qt::WA_MacShowFocusRect, false);
  this->verticalForceThresholdSpinBox->setAttribute(Qt::WA_MacShowFocusRect, false);
  this->resize(435,435);
#endif
  this->manualMappingTable->verticalHeader()->setDefaultSectionSize(24); 
  QHeaderView* header = this->manualMappingTable->horizontalHeader();
  header->setMovable(false);
  header->resizeSection(3, 35);
  header->setResizeMode(3, QHeaderView::Fixed);
  header->resizeSection(0, 35);
  header->setResizeMode(0, QHeaderView::Fixed);
  header->setResizeMode(2, QHeaderView::Stretch);
  header->setResizeMode(1, QHeaderView::Stretch);
  
  connect(this->mp_ResizeDetectionOptionAnimation, SIGNAL(finished()), this, SLOT(endToggleDetectionOptions()));
  connect(this->mappingMethodComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setMappingMethod(int)));
  connect(this->detectionMethodComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setDetectionMethod(int)));
  connect(this->detectionOptionsButton, SIGNAL(clicked()), this, SLOT(startToggleDetectionOptions()));
  
  QSettings settings;
  this->mappingMethodComboBox->setCurrentIndex(settings.value("Tools/GaitEventAssistant/lastMappingMethod", 0).toInt());
  this->detectionMethodComboBox->setCurrentIndex(settings.value("Tools/GaitEventAssistant/lastDetectionMethod", 0).toInt());
  this->verticalForceThresholdSpinBox->setValue(settings.value("Tools/GaitEventAssistant/verticalForceThresholdValue", 10).toInt());
  
  this->endToggleDetectionOptions();
};

void GaitEventAssistantDialog::initialize(Acquisition* acq)
{
  QSettings settings;
  QList<QVariant> lastManualMapping = settings.value("Tools/GaitEventAssistant/lastManualMapping").toList();
  QStringList mappingText; mappingText << "" << "Right" << "Left" << "General";
  this->manualMappingTable->setRowCount(acq->btkGroundReactionWrenches()->GetItemNumber());
  for (int i = 0 ; i < this->manualMappingTable->rowCount() ; ++i)
  {
    this->manualMappingTable->setItem(i,1, new QTableWidgetItem("Force platform #"+QString::number(i+1)));
    QComboBox* cb = new QComboBox(this);
    cb->addItems(mappingText);
    if (lastManualMapping.count() > i)
      cb->setCurrentIndex(lastManualMapping[i].toInt());
    this->manualMappingTable->setCellWidget(i,2, cb);
    connect(cb, SIGNAL(currentIndexChanged(int)), this, SLOT(checkManualMapping()));
  }
  this->checkManualMapping();
};
  
bool GaitEventAssistantDialog::run(QUndoCommand* acquisitionCommand, Acquisition* acq)
{
  // LOG_INFO(tr("Detecting gait events..."));
  QList<Event*> gaitEvents;
  QList<int> gaitEventIds;
  QString errMsg;
  bool error = true;
  QSettings settings;
  
  btk::EventCollection::Pointer output;
  try
  {
    if (this->detectionMethodComboBox->currentIndex() == VerticalGroundReactionForceDetection)
    {
      btk::WrenchCollection::Pointer forceplates = btk::WrenchCollection::New();
      std::vector<std::string> mapping(this->manualMappingTable->rowCount());
      if (this->mappingMethodComboBox->currentIndex() == ManualMapping)
      {
        QList<QVariant> lastManualMapping;
        for (int i = 0 ; i < this->manualMappingTable->rowCount() ; ++i)
        {
          QComboBox* cb = static_cast<QComboBox*>(this->manualMappingTable->cellWidget(i,2));
          lastManualMapping.append(cb->currentIndex());
          if (cb->currentIndex() == 0)
            continue;
          forceplates->InsertItem(acq->btkGroundReactionWrenches()->GetItem(i));
          mapping[i] = cb->currentText().toStdString();
        }
        settings.setValue("Tools/GaitEventAssistant/lastManualMapping", lastManualMapping);
      }

      btk::VerticalGroundReactionForceGaitEventDetector::Pointer vgrfged = btk::VerticalGroundReactionForceGaitEventDetector::New();
      vgrfged->SetInput(forceplates);
      vgrfged->SetThresholdValue(this->verticalForceThresholdSpinBox->value());
      vgrfged->SetForceplateContextMapping(mapping);
      // FIXME: The region of interest should correspond to the visual boundaries.
      //        Need to modify the class Acquisition
      int bounds[2]; acq->regionOfInterest(bounds[0],bounds[1]);
      vgrfged->SetRegionOfInterest(bounds[0]-acq->firstFrame(),bounds[1]-acq->firstFrame());
      vgrfged->SetAcquisitionInformation(acq->firstFrame(), acq->pointFrequency(), "");
      output = vgrfged->GetOutput();
      output->Update();

      settings.setValue("Tools/GaitEventAssistant/verticalForceThresholdValue", this->verticalForceThresholdSpinBox->value());
    }
    error = false;
  }
  catch (btk::Exception& e)
  {
    LOG_CRITICAL(e.what());
  }
  catch (std::exception& e)
  {
    LOG_CRITICAL("Unexpected error: " + QString(e.what()));
  }
  catch (...)
  {
    LOG_CRITICAL("Unknown error.");
  }
  
  if (!error)
  {
    // TODO: Refactor the code to create a function to convert btk::Event to (mokka::)Event?
    for (btk::EventCollection::ConstIterator it = output->Begin() ; it != output->End() ; ++it)
    {
      Event* e = new Event();
      e->label = QString::fromStdString((*it)->GetLabel());;
      e->description = QString::fromStdString((*it)->GetDescription());
      e->context = QString::fromStdString((*it)->GetContext());
      e->subject = QString::fromStdString((*it)->GetSubject());
      e->time = (*it)->GetTime();
      e->frame = (*it)->GetFrame();
      e->iconId = (*it)->GetId();
      gaitEvents.append(e);
      gaitEventIds.append(acq->generateNewEventId());
    }
    
    if (this->overwriteRadioButton->isChecked())
      new RemoveEvents(acq, acq->eventIds(), acquisitionCommand);
    new InsertEvents(acq, gaitEventIds, gaitEvents, acquisitionCommand);
    int count = gaitEvents.count();
    if (count <= 1)
      LOG_INFO(tr("Detecting gait events... A total of ") + QString::number(count) + tr(" event was detected."));
    else
      LOG_INFO(tr("Detecting gait events... A total of ") + QString::number(count) + tr(" events were detected."));
    
    settings.setValue("Tools/GaitEventAssistant/lastMappingMethod", this->mappingMethodComboBox->currentIndex());
    settings.setValue("Tools/GaitEventAssistant/lastDetectionMethod", this->detectionMethodComboBox->currentIndex());
  }
  else
  {
    QMessageBox error(QMessageBox::Warning, "Gait Event Assistant", "Error occurred during the events detection.", QMessageBox::Ok , this);
    error.setInformativeText("<nobr>Check the logger for more informations.</nobr>");
    error.exec();
  }
  
  return !error;
};

void GaitEventAssistantDialog::setMappingMethod(int index)
{
  QSettings settings;
  this->mappingStack->setCurrentIndex(index+1);
};

void GaitEventAssistantDialog::setDetectionMethod(int index)
{
  QSettings settings;
  this->detectionOptionsStack->setCurrentIndex(index+1);
};

void GaitEventAssistantDialog::startToggleDetectionOptions()
{
  if (this->detectionOptionsStack->currentIndex() == 0)
  {
    
    this->mp_ResizeDetectionOptionAnimation->setStartValue(QSize(this->width(), this->height()));
    this->mp_ResizeDetectionOptionAnimation->setEndValue(QSize(this->width(), this->height()+this->detectionOptionsStack->height()));
  }
  else
  {
    this->mp_ResizeDetectionOptionAnimation->setStartValue(QSize(this->width(), this->height()));
    this->mp_ResizeDetectionOptionAnimation->setEndValue(QSize(this->width(), this->height()-this->detectionOptionsStack->height()));
  }
  this->mp_ResizeDetectionOptionAnimation->start();
};

void GaitEventAssistantDialog::endToggleDetectionOptions()
{
  if (this->detectionOptionsStack->currentIndex() == 0)
  {
    this->detectionOptionsStack->setVisible(true);
    this->detectionOptionsStack->setCurrentIndex(this->detectionOptionsStack->currentIndex()+1);
    this->detectionOptionsButton->setIcon(QIcon(":/Resources/Images/disclosureTriangleSmallDownBlack.png"));
  }
  else
  {
    this->detectionOptionsStack->setVisible(false);
    this->detectionOptionsStack->setCurrentIndex(0);
    this->detectionOptionsButton->setIcon(QIcon(":/Resources/Images/disclosureTriangleSmallRightBlack.png"));
  }
};

void GaitEventAssistantDialog::checkManualMapping()
{
  bool hasMapping = false;
  for (int i = 0 ; i < this->manualMappingTable->rowCount() ; ++i)
  {
    if (static_cast<QComboBox*>(this->manualMappingTable->cellWidget(i,2))->currentIndex() != 0)
    {
      hasMapping = true;
      break;
    }
  }
  this->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(hasMapping);
};
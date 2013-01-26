/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2013, Arnaud Barré
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

#include "GaitEventDetection.h"
#include "GaitEventDetectionDialog.h"
#include "UndoCommands.h"

#include <btkVerticalGroundReactionForceGaitEventDetector.h>

#include <QSettings>

void GaitEventDetection::RegisterTool(ToolsManager* manager)
{
  manager->addEventTool(tr("Gait Event Detection"), ToolFactory<GaitEventDetection>);
};
  
GaitEventDetection::GaitEventDetection(QWidget* parent)
: AbstractTool("Gait Event Detection", parent)
{};
  
AbstractTool::RunState GaitEventDetection::run(ToolCommands* cmds, ToolsData* const data)
{
  GaitEventDetectionDialog dialog(this->parentWidget());
  dialog.initialize(data->acquisition());
  if (dialog.exec() != QDialog::Accepted)
  {
    return Cancel;
  }
  else
  {    
    QList<Event*> gaitEvents;
    QList<int> gaitEventIds;
    QString errMsg;
    bool error = true;
    QSettings settings;
    
    btk::EventCollection::Pointer output;
    try
    {
      if (dialog.detectionMethodComboBox->currentIndex() == VerticalGroundReactionForceDetection)
      {
        btk::WrenchCollection::Pointer forceplates = btk::WrenchCollection::New();
        std::vector<std::string> mapping(dialog.manualMappingTable->rowCount());
        if (dialog.mappingMethodComboBox->currentIndex() == ManualMapping)
        {
          QList<QVariant> lastManualMapping;
          for (int i = 0 ; i < dialog.manualMappingTable->rowCount() ; ++i)
          {
#ifndef Q_OS_MAC
            QComboBox* cb = static_cast<QComboBox*>(dialog.manualMappingTable->cellWidget(i,2)->layout()->itemAt(0)->widget());
#else
            QComboBox* cb = static_cast<QComboBox*>(dialog.manualMappingTable->cellWidget(i,2));
#endif
            lastManualMapping.append(cb->currentIndex());
            if (cb->currentIndex() == 0)
              continue;
            forceplates->InsertItem(data->acquisition()->btkGroundReactionWrenches()->GetItem(i));
            mapping[i] = cb->currentText().toStdString();
          }
          settings.setValue("Tools/GaitEventDetection/lastManualMapping", lastManualMapping);
        }

        btk::VerticalGroundReactionForceGaitEventDetector::Pointer vgrfged = btk::VerticalGroundReactionForceGaitEventDetector::New();
        vgrfged->SetInput(forceplates);
        vgrfged->SetThresholdValue(dialog.verticalForceThresholdSpinBox->value());
        vgrfged->SetForceplateContextMapping(mapping);
        // FIXME: The region of interest should correspond to the visual boundaries.
        //        Need to modify the class Acquisition
        int bounds[2]; data->acquisition()->regionOfInterest(bounds[0],bounds[1]);
        vgrfged->SetRegionOfInterest(bounds[0]-data->acquisition()->firstFrame(),bounds[1]-data->acquisition()->firstFrame());
        vgrfged->SetAcquisitionInformation(data->acquisition()->firstFrame(), data->acquisition()->pointFrequency(), "");
        output = vgrfged->GetOutput();
        output->Update();

        settings.setValue("Tools/GaitEventDetection/verticalForceThresholdValue", dialog.verticalForceThresholdSpinBox->value());
      }
      error = false;
    }
    catch (btk::Exception& e)
    {
      TOOL_LOG_ERROR(e.what());
    }
    catch (std::exception& e)
    {
      TOOL_LOG_ERROR("Unexpected error: " + QString(e.what()));
    }
    catch (...)
    {
      TOOL_LOG_ERROR("Unknown error.");
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
        gaitEventIds.append(data->acquisition()->generateNewEventId());
      }
      
      if (dialog.overwriteRadioButton->isChecked())
        new RemoveEvents(data->acquisition(), data->acquisition()->eventIds(), cmds->acquisitionCommand());
      new InsertEvents(data->acquisition(), gaitEventIds, gaitEvents, cmds->acquisitionCommand());
      int count = gaitEvents.count();
      if (count <= 1)
        TOOL_LOG_INFO(tr("A total of ") + QString::number(count) + tr(" event was detected."));
      else
        TOOL_LOG_INFO(tr("A total of ") + QString::number(count) + tr(" events were detected."));
      
      settings.setValue("Tools/GaitEventDetection/lastMappingMethod", dialog.mappingMethodComboBox->currentIndex());
      settings.setValue("Tools/GaitEventDetection/lastDetectionMethod", dialog.detectionMethodComboBox->currentIndex());
    }
    else
    {
      QMessageBox error(QMessageBox::Warning, "Gait Event Assistant", "Error occurred during the events detection.", QMessageBox::Ok , this->parentWidget());
      error.setInformativeText("<nobr>Check the logger for more informations.</nobr>");
      error.exec();
    }
    
    return (!error ? Success : Error);
  }
};

// ------------------------------------------------------------------------- //

GaitEventDetectionDialog::GaitEventDetectionDialog(QWidget* parent)
: QDialog(parent)
{
  this->setupUi(this);
#ifdef Q_OS_MAC
  this->layout()->setContentsMargins(12,12,12,12);
  this->layout()->setSpacing(6);
  this->manualMappingPage->layout()->setContentsMargins(6,6,6,6);
  QFont f = this->font();
  f.setPixelSize(11);
  this->framesInterestLabel->setFont(f);
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
  
  connect(this->mappingMethodComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setMappingMethod(int)));
  connect(this->detectionMethodComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setDetectionMethod(int)));
  connect(this->detectionOptionsButton, SIGNAL(clicked()), this, SLOT(toggleDetectionOptions()));
  
  QSettings settings;
  this->mappingMethodComboBox->setCurrentIndex(settings.value("Tools/GaitEventDetection/lastMappingMethod", 0).toInt());
  this->detectionMethodComboBox->setCurrentIndex(settings.value("Tools/GaitEventDetection/lastDetectionMethod", 0).toInt());
  this->verticalForceThresholdSpinBox->setValue(settings.value("Tools/GaitEventDetection/verticalForceThresholdValue", 10).toInt());
  
  this->toggleDetectionOptions();
  
  // FIXME: The region of interest should correspond to the visual boundaries.
  //        Need to modify the class Acquisition
  this->framesInterestLabel->setVisible(false);
};

void GaitEventDetectionDialog::initialize(const Acquisition* const acq)
{
  QSettings settings;
  QList<QVariant> lastManualMapping = settings.value("Tools/GaitEventDetection/lastManualMapping").toList();
  QStringList mappingText; mappingText << "" << "Right" << "Left" << "General";
  this->manualMappingTable->setRowCount(acq->btkGroundReactionWrenches()->GetItemNumber());
  for (int i = 0 ; i < this->manualMappingTable->rowCount() ; ++i)
  {
    this->manualMappingTable->setItem(i,1, new QTableWidgetItem("Force platform #"+QString::number(i+1)));
    QComboBox* cb = new QComboBox(this);
    cb->addItems(mappingText);
    if (lastManualMapping.count() > i)
      cb->setCurrentIndex(lastManualMapping[i].toInt());
#ifndef Q_OS_MAC
    QWidget* wdg = new QWidget;
    QHBoxLayout* layout = new QHBoxLayout(wdg);
    layout->addWidget(cb);
    layout->setAlignment(Qt::AlignVCenter);
    layout->setMargin(0);
    cb->setMaximumHeight(20);
    this->manualMappingTable->setCellWidget(i,2, wdg);
#else
    this->manualMappingTable->setCellWidget(i,2, cb);
#endif
    connect(cb, SIGNAL(currentIndexChanged(int)), this, SLOT(checkManualMapping()));
  }
  this->checkManualMapping();
};

void GaitEventDetectionDialog::setMappingMethod(int index)
{
  QSettings settings;
  this->mappingStack->setCurrentIndex(index+1);
};

void GaitEventDetectionDialog::setDetectionMethod(int index)
{
  QSettings settings;
  this->detectionOptionsStack->setCurrentIndex(index+1);
};

void GaitEventDetectionDialog::toggleDetectionOptions()
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

void GaitEventDetectionDialog::checkManualMapping()
{
  bool hasMapping = false;
  for (int i = 0 ; i < this->manualMappingTable->rowCount() ; ++i)
  {
#ifndef Q_OS_MAC
    QComboBox* cb = static_cast<QComboBox*>(this->manualMappingTable->cellWidget(i,2)->layout()->itemAt(0)->widget());
#else
    QComboBox* cb = static_cast<QComboBox*>(this->manualMappingTable->cellWidget(i,2));
#endif
    if (cb->currentIndex() != 0)
    {
      hasMapping = true;
      break;
    }
  }
  this->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(hasMapping);
};
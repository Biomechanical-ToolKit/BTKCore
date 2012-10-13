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
 
#include "AnalogToolOptionDialog.h"
#include "Acquisition.h"
#include "../AbstractTool.h"
#include "../UndoCommands.h"

#include <QSettings>
#include <QPushButton>
#include <QToolBox>
#include <QVBoxLayout>

AnalogToolOptionDialog::AnalogToolOptionDialog(const QString& toolName, QWidget* parent)
: QDialog(parent), m_ToolName(toolName), m_ToolSettingsPath()
{
  this->setWindowTitle(toolName);
  if (!this->m_ToolName.isEmpty())
  {
    this->m_ToolSettingsPath = "Tools/" + toolName + "/";
    this->m_ToolSettingsPath.remove(" ");
  }
  this->setupUi(this);
  this->mp_OptionsToolBox = new QToolBox(this);
  this->mp_OptionsToolBox->setObjectName("optionsToolBox");
  this->mp_OptionsToolBox->layout()->setSpacing(3);
  this->optionsGroupBox->layout()->addWidget(this->mp_OptionsToolBox);
  this->optionsGroupBox->layout()->setContentsMargins(12,3,12,3);
  this->optionsGroupBox->setVisible(false);
  
#ifndef Q_OS_WIN
  this->layout()->setContentsMargins(12,12,12,12);
  // static_cast<QGridLayout*>(this->layout())->setHorizontalSpacing(12);
  //   static_cast<QGridLayout*>(this->layout())->setVerticalSpacing(12);
  this->layout()->setSpacing(12);
  this->processingGroupBox->layout()->setSpacing(12);
  QFont f = this->font();
  f.setPointSize(10);
  this->treeWidget->setFont(f);
  f.setPointSize(11);
  this->overwriteAnalogsButton->setFont(f);
  this->createAnalogsButton->setFont(f);
  this->resize(480, this->height());
  this->analogsGroupBox->setMaximumWidth(219);
#endif
  
  connect(this->treeWidget, SIGNAL(itemChanged(QTreeWidgetItem* , int)), this, SLOT(checkAnalogSelection()));
  connect(this, SIGNAL(accepted()), this, SLOT(saveSettings()));
};

void AnalogToolOptionDialog::initialize(ToolsData* data)
{
  QSettings settings;
  int lastProcessing = settings.value(this->m_ToolSettingsPath + "lastProcessing").toInt();
  QStringList lastChannelsSelection = settings.value(this->m_ToolSettingsPath + "lastChannelsSelection").toStringList();
  QIcon analogIcon(QString::fromUtf8(":/Resources/Images/chart_line.png"));
  
  QList<int> selectedAnalogIds = data->explorerSelectedItems(AnalogType);
  Acquisition* acq = data->acquisition();
  
  QFont f = this->treeWidget->font();
  f.setBold(true);
  QTreeWidgetItem* analogsRoot = new QTreeWidgetItem(QStringList(QString("Analog channels")));
  analogsRoot->setFont(0, f);
  analogsRoot->setIcon(0, analogIcon);
  analogsRoot->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsTristate);
  this->treeWidget->addTopLevelItem(analogsRoot);
  QTreeWidgetItem* visibleItem;
  for (QMap<int, Analog*>::const_iterator it = data->acquisition()->analogs().begin() ; it != acq->analogs().end() ; ++it)
  {
    QTreeWidgetItem* analogItem = new QTreeWidgetItem(QStringList(it.value()->label));
    analogItem->setIcon(0, analogIcon);
    analogItem->setData(0, Qt::UserRole, it.key());
    if (selectedAnalogIds.isEmpty())
      analogItem->setCheckState(0, lastChannelsSelection.indexOf(it.value()->label) != -1 ? Qt::Checked : Qt::Unchecked);
    else
      analogItem->setCheckState(0, selectedAnalogIds.indexOf(it.key()) != -1 ? Qt::Checked : Qt::Unchecked);
    analogItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    analogsRoot->addChild(analogItem);
    if (analogItem->checkState(0) == Qt::Checked)
      visibleItem = analogItem;
  }
  analogsRoot->setExpanded(true);
  
  if (analogsRoot->checkState(0) == Qt::Unchecked) // No analog channel founds
    analogsRoot->setCheckState(0, Qt::Checked);
  else
    this->treeWidget->scrollToItem(visibleItem);
  
  if (acq->analogs().count() > 18)
    this->resize(this->width(),640);
  
  if (lastProcessing == 0)
    this->overwriteAnalogsButton->setChecked(true);
  else
    this->createAnalogsButton->setChecked(true);
  
  this->initializeOptions(acq);
};

QList<int> AnalogToolOptionDialog::selectedAnalogIds() const
{
  QList<int> ids;
  QTreeWidgetItem* analogsRoot = this->treeWidget->topLevelItem(0);
  for (int i = 0 ; i < analogsRoot->childCount() ; ++i)
  {
    QTreeWidgetItem* analogItem = analogsRoot->child(i);
    if (analogItem->checkState(0) == Qt::Checked)
      ids.push_back(analogItem->data(0, Qt::UserRole).toInt());
  }
  return ids;
};

QList<int> AnalogToolOptionDialog::createAnalogChannels(const QString& labelSuffix, const QString& toolDetail, const QList<int>& ids, ToolsData* const data, ToolCommands* cmds) const
{
  btk::AnalogCollection::Pointer temp; // NULL pointer to no get the created channels
  return this->createAnalogChannels(temp, labelSuffix, toolDetail, ids, data, cmds);
};

QList<int> AnalogToolOptionDialog::createAnalogChannels(btk::AnalogCollection::Pointer analogs, const QString& labelSuffix, const QString& toolDetail, const QList<int>& ids, ToolsData* const data, ToolCommands* cmds) const
{
  QList<int> newIds;
  if (this->createAnalogsButton->isChecked())
  {
    int analogNumber = data->acquisition()->btkAcquisition()->GetAnalogNumber();
    btk::AnalogCollection::Pointer generatedAnalogs = btk::AnalogCollection::New();
    std::string stdLabelSuffix = labelSuffix.toStdString();
    std::string stdToolDetail = toolDetail.toStdString();
    for (QList<int>::const_iterator itIdx = ids.begin() ; itIdx != ids.end() ; ++itIdx)
    {
      if (*itIdx >= analogNumber)
      {
        qDebug("Invalid analog ID. Impossible to use it to create new analog channel.");
        continue;
      }
      btk::AnalogCollection::ConstIterator itA = data->acquisition()->btkAcquisition()->BeginAnalog();
      std::advance(itA, *itIdx);
      btk::Analog::Pointer analog = (*itA)->Clone();
      std::string desc = "Generated from channel " + analog->GetLabel() + (stdToolDetail.empty() ? "" : " - " + stdToolDetail); 
      analog->SetDescription(desc);
      analog->SetLabel(analog->GetLabel() + stdLabelSuffix);
      generatedAnalogs->InsertItem(analog);
      if (analogs.get() != 0)
        analogs->InsertItem(analog);
      newIds.push_back(data->acquisition()->generateNewAnalogId());
    }
    if (!newIds.isEmpty())
      new CreateAnalogs(data->acquisition(), newIds, generatedAnalogs, cmds->acquisitionCommand());
  }
  else
  {
    qDebug("Invalid command! The user didn't select the option to create analog channels.");
  }
  return newIds;
};

QList<int> AnalogToolOptionDialog::extractSelectedAnalogChannels(btk::AnalogCollection::Pointer analogs, const QString& labelSuffix, const QString& toolDetail, ToolsData* const data, ToolCommands* cmds) const
{
  QList<int> ids = this->selectedAnalogIds();
  if (this->createAnalogsButton->isChecked())
    ids = this->createAnalogChannels(analogs, labelSuffix, toolDetail, ids, data, cmds);
  else
  {
    int numAnalogs = data->acquisition()->btkAcquisition()->GetAnalogNumber();
    for (int i = 0 ; i < ids.count() ; ++i)
    {
      if (ids[i] < numAnalogs)
      {
        btk::AnalogCollection::Iterator it = data->acquisition()->btkAcquisition()->BeginAnalog();
        std::advance(it, ids[i]);
        analogs->InsertItem(*it);
      }
    }
  }
  return ids;
};


void AnalogToolOptionDialog::addOption(const QString& title, QWidget* content)
{
  content->layout()->setContentsMargins(content->layout()->contentsMargins().left(),0,0,0);
  int index = this->mp_OptionsToolBox->addItem(content, title);
  content->setObjectName(this->mp_OptionsToolBox->objectName() + "_option" + QString::number(index));
  this->optionsGroupBox->setStyleSheet(this->optionsGroupBox->styleSheet() + "\n QWidget#" + content->objectName() +"{background-color:transparent;}");
  this->mp_OptionsToolBox->setCurrentIndex(0);
  this->optionsGroupBox->setVisible(true);
};

void AnalogToolOptionDialog::setDataProcessingVisible(bool visible)
{
  this->processingGroupBox->setVisible(visible);
  if (!visible)
  {
    this->layout()->removeItem(static_cast<QGridLayout*>(this->layout())->itemAtPosition(0,1));
    static_cast<QGridLayout*>(this->layout())->addItem(this->layout()->takeAt(this->layout()->indexOf(this->optionsGroupBox)), 0, 1, 1, 1);
  }
};

void AnalogToolOptionDialog::checkAnalogSelection()
{
  this->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(this->treeWidget->topLevelItem(0)->checkState(0) != Qt::Unchecked);
};

void AnalogToolOptionDialog::saveSettings()
{
  QStringList lastChannelsSelection;
  QTreeWidgetItem* analogsRoot = this->treeWidget->topLevelItem(0);
  for (int i = 0 ; i < analogsRoot->childCount() ; ++i)
  {
    QTreeWidgetItem* analogItem = analogsRoot->child(i);
    if (analogItem->checkState(0) == Qt::Checked)
      lastChannelsSelection.push_back(analogItem->text(0));
  }
  
  int lastProcessing = 1;
  if (this->overwriteAnalogsButton->isChecked())
    lastProcessing = 0;
  
  QSettings settings;
  settings.setValue(this->m_ToolSettingsPath + "lastProcessing", lastProcessing);
  settings.setValue(this->m_ToolSettingsPath + "lastChannelsSelection", lastChannelsSelection);
  
  this->saveOptionsSettings();
};
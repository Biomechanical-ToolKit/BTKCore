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

#include "ChartCycleSettingsManager.h"

ChartCycleSettingsManager::ChartCycleSettingsManager(QObject* parent)
: QObject(parent), m_Settings()
{
  this->m_CurrentSetting = -1;
};

const ChartCycleSetting& ChartCycleSettingsManager::setting(int index) const
{
  Q_ASSERT((index >= 0) && (index < this->m_Settings.size()));
  return this->m_Settings[index];
};

void ChartCycleSettingsManager::addSetting(const ChartCycleSetting& setting)
{
  this->m_Settings.push_back(setting);
  emit settingAdded();
};

void ChartCycleSettingsManager::removeSetting(int index)
{
  Q_ASSERT((index >= 0) && index < this->m_Settings.size());
  this->m_Settings.removeAt(index);
  emit settingRemoved(index);
  if (this->m_CurrentSetting >= index)
    this->setCurrentSetting(this->m_CurrentSetting-1);
  
};

void ChartCycleSettingsManager::setSetting(int index, const ChartCycleSetting& setting)
{
  Q_ASSERT((index >= 0) && index < this->m_Settings.size());
  ChartCycleSetting& current = this->m_Settings[index];
  if (this->compareSetting(setting,current))
  {
    this->m_Settings[index] = setting;
    emit settingModified(index);
  }
};

void ChartCycleSettingsManager::setSettings(const QList<ChartCycleSetting>& settings)
{
  bool isEqual = true;
  if (this->m_Settings.count() != settings.count())
    isEqual = false;
  else
  {
    for (int i = 0 ; i < settings.count() ; ++i)
    {
      if (!this->compareSetting(settings[i], this->m_Settings[i]))
      {
        isEqual = false;
        break;
      }
    }
  }
  if (!isEqual)
  {
    this->m_Settings = settings;
    emit settingsUpdated();
  }
};

void ChartCycleSettingsManager::setCurrentSetting(int index)
{
  if (this->m_CurrentSetting == index)
    return;
  this->m_CurrentSetting = index;
  emit currentSettingChanged(index);
};

static const int ChartCycleSettingsManagerMagic = 0x0abc;
static const int ChartCycleSettingsManagerVersion1 = 0x0100; // 1.0 

void ChartCycleSettingsManager::importSettings(const QList<QVariant>& settings)
{
  QList<ChartCycleSetting> cycleSettings;
  for (QList<QVariant>::const_iterator it = settings.begin() ; it != settings.end() ; ++it)
  {
    QByteArray sd = it->toByteArray();
    QDataStream stream(&(sd), QIODevice::ReadOnly);
    qint32 magic; stream >> magic;
    qint32 version; stream >> version;
    if (magic != ChartCycleSettingsManagerMagic)
    {
      qCritical("The data given to set a cycle setting are not recognized.");
      return;
    }
    if (version == ChartCycleSettingsManagerVersion1)
    {
      stream.setVersion(QDataStream::Qt_4_6);
      
      ChartCycleSetting ccs;
      stream >> ccs.name;
      stream >> ccs.horizontalAxisTitle;
      stream >> ccs.calculationMethod;
      if (ccs.calculationMethod == 0)
        ccs.calculationMethodOption = NULL;
      else
      {
        qCritical("Unknown calculation method to normalize data into cycles. You may have used a new version containing some major changes in the data format.");
        return;
      }
      stream >> ccs.rightEvents[0];
      stream >> ccs.rightEvents[1];
      stream >> ccs.leftEvents[0];
      stream >> ccs.leftEvents[1];
      stream >> ccs.generalEvents[0];
      stream >> ccs.generalEvents[1];
      stream >> ccs.rightLabelRule;
      stream >> ccs.rightLabelRuleText;
      stream >> ccs.leftLabelRule;
      stream >> ccs.leftLabelRuleText;
      cycleSettings.append(ccs);
    }
    else
    {
      qCritical("Unknown version number for serialized data containing a cycle setting. You may have used a new version containing some major changes in the data format.");
      return;
    }
  }
  this->setSettings(cycleSettings);
};

QList<QVariant> ChartCycleSettingsManager::exportSettings() const
{
  QList<QVariant> list;
  for (QList<ChartCycleSetting>::const_iterator it = this->m_Settings.begin() ; it != this->m_Settings.end() ; ++it)
  {
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_4_6);
    stream << qint32(ChartCycleSettingsManagerMagic);
    stream << qint32(ChartCycleSettingsManagerVersion1);
    stream << it->name;
    stream << it->horizontalAxisTitle;
    stream << qint32(it->calculationMethod);
    if (it->calculationMethod != 0)
    {
      // For other methods
    }
    stream << it->rightEvents[0];
    stream << it->rightEvents[1];
    stream << it->leftEvents[0];
    stream << it->leftEvents[1];
    stream << it->generalEvents[0];
    stream << it->generalEvents[1];
    stream << qint32(it->rightLabelRule);
    stream << it->rightLabelRuleText;
    stream << qint32(it->leftLabelRule);
    stream << it->leftLabelRuleText;
    list.append(data);
  }
  return list;
};

bool ChartCycleSettingsManager::compareSetting(const ChartCycleSetting& lhs, const ChartCycleSetting& rhs) const
{
  bool isEqual = true;
  if ((rhs.name != lhs.name)
    || (rhs.horizontalAxisTitle != lhs.horizontalAxisTitle)
    || (rhs.calculationMethod != lhs.calculationMethod)
    // TODO: Improve the check for the options of the calculation method
    // || (rhs.ChartCycleCalculationMethodOption != lhs.ChartCycleCalculationMethodOption)
    || (rhs.rightEvents[0] != lhs.rightEvents[0])
    || (rhs.rightEvents[1] != lhs.rightEvents[1])
    || (rhs.leftEvents[0] != lhs.leftEvents[0])
    || (rhs.leftEvents[1] != lhs.leftEvents[1])
    || (rhs.generalEvents[0] != lhs.generalEvents[0])
    || (rhs.generalEvents[1] != lhs.generalEvents[1])
    || (rhs.rightLabelRule != lhs.rightLabelRule)
    || (rhs.rightLabelRuleText != lhs.rightLabelRuleText)
    || (rhs.leftLabelRule != lhs.leftLabelRule)
    || (rhs.leftLabelRuleText != lhs.leftLabelRuleText))
    isEqual = false;
  return isEqual;
};
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
{};

const ChartCycleSetting& ChartCycleSettingsManager::setting(int index) const
{
  Q_ASSERT((index >= 0) && index < this->m_Settings.size());
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
};

void ChartCycleSettingsManager::setSetting(int index, const ChartCycleSetting& setting)
{
  Q_ASSERT((index >= 0) && index < this->m_Settings.size());
  ChartCycleSetting& current = this->m_Settings[index];
  if (((setting.name != current.name)
    || (setting.horizontalAxisTitle != current.horizontalAxisTitle)
    || (setting.calculationMethod != current.calculationMethod)
    // TODO: Improve the check for the options of the calculation method
    // || (setting.ChartCycleCalculationMethodOption != current.ChartCycleCalculationMethodOption)
    || (setting.rightEvents[0] != current.rightEvents[0])
    || (setting.rightEvents[1] != current.rightEvents[1])
    || (setting.leftEvents[0] != current.leftEvents[0])
    || (setting.leftEvents[1] != current.leftEvents[1])
    || (setting.generalEvents[0] != current.generalEvents[0])
    || (setting.generalEvents[1] != current.generalEvents[1])
    || (setting.rightLabelRule != current.rightLabelRule)
    || (setting.rightLabelRuleText != current.rightLabelRuleText)
    || (setting.leftLabelRule != current.leftLabelRule)
    || (setting.leftLabelRuleText != current.leftLabelRuleText)))
  {
    this->m_Settings[index] = setting;
    emit settingModified(index);
  }
};
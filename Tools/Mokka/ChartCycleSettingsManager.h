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

#ifndef ChartCycleSettingsManager_h
#define ChartCycleSettingsManager_h

#include "ui_About.h"

struct ChartCycleCalculationMethodOption
{};

struct ChartCycleSetting
{
  QString name;
  QString horizontalAxisTitle;
  int calculationMethod;
  ChartCycleCalculationMethodOption* calculationMethodOption;
  QString leftEvents[2];
  QString rightEvents[2];
  QString generalEvents[2];
  int rightLabelRule;
  QString rightLabelRuleText;
  int leftLabelRule;
  QString leftLabelRuleText;
};

class ChartCycleSettingsManager : public QObject
{
  Q_OBJECT
  
public:
  enum {maxCycleSettings = 10};
  
  ChartCycleSettingsManager(QObject* parent = 0);
  
  int count() {return this->m_Settings.count();};
  const ChartCycleSetting& setting(int index) const;
  void addSetting(const ChartCycleSetting& setting);
  void removeSetting(int index);
  void setSetting(int index, const ChartCycleSetting& setting);
  
  int currentSetting() const {return this->m_CurrentSetting;};
  void setCurrentSetting(int index);
  void setSettings(const QList<ChartCycleSetting>& settings);
  
  const QStringList& eventsLabel() const {return this->m_EventsLabel;};
  void setEventsLabel(const QStringList& list) {this->m_EventsLabel = list;};
  
signals:
  void settingAdded();
  void settingModified(int index);
  void settingRemoved(int index);
  void settingsUpdated();
  void currentSettingChanged(int index);

private:
  bool compareSetting(const ChartCycleSetting& lhs, const ChartCycleSetting& rhs) const;

  QStringList m_EventsLabel;
  QList<ChartCycleSetting> m_Settings;
  int m_CurrentSetting;
};
#endif // ChartCycleSettingsManager_h

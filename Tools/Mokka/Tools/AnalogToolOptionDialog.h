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

#ifndef AnalogToolOptionDialog_h
#define AnalogToolOptionDialog_h

#include "ui_AnalogToolOptionDialog.h"

#include <btkAnalogCollection.h>

#include <QDialog>

class ToolsData;
class ToolCommands;
class Acquisition;

class QToolBox;

class AnalogToolOptionDialog : public QDialog, public Ui::AnalogToolOptionDialog
{
  Q_OBJECT
  
public:
  AnalogToolOptionDialog(const QString& toolName, QWidget* parent = 0);
  void initialize(ToolsData* data);
  
  const QString& toolName() {return this->m_ToolName;};
  const QString& toolSettingsPath() {return this->m_ToolSettingsPath;};
  
  QList<int> selectedAnalogIds() const;
  QList<int> createAnalogChannels(const QString& labelSuffix, const QString& toolDetail, const QList<int>& ids, ToolsData* const data, ToolCommands* cmds) const;
  QList<int> createAnalogChannels(btk::AnalogCollection::Pointer analogs, const QString& labelSuffix, const QString& toolDetail, const QList<int>& ids, ToolsData* const data, ToolCommands* cmds) const;
  QList<int> extractSelectedAnalogChannels(btk::AnalogCollection::Pointer analogs, const QString& labelSuffix, const QString& toolDetail, ToolsData* const data, ToolCommands* cmds) const;
  
  void addOption(const QString& title, QWidget* content);

protected slots:
  void testAcceptButton();

protected:
  void setDataProcessingVisible(bool visible);
  virtual void initializeOptions(const Acquisition* const /* acq */) {};
  virtual void saveOptionsSettings() {};
  virtual bool testOptionsValidity() {return true;};
  
private slots:
  void saveSettings();
  
private:
  QString m_ToolName;
  QString m_ToolSettingsPath;
  QToolBox* mp_OptionsToolBox;
};

#endif // AnalogToolOptionDialog_h
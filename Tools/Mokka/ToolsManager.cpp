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

#include "ToolsManager.h"
#include "AbstractTool.h"
#include "ToolsData.h"

#include <QMenu>

#include "ToolsManager_registration.cpp"

ToolsManager::ToolsManager(ToolsData* data, QMenu* toolsMenu, QWidget* parent)
: QObject(parent), m_Tools()
{
  this->mp_ToolsData = data;
  
  this->mp_ModelMenu = new QMenu(tr("Model"), parent);
  this->mp_AcquisitionMenu = new QMenu(tr("Acquisition"), parent);
  this->mp_MarkerMenu = new QMenu(tr("Marker"), parent);
  this->mp_AnalogMenu = new QMenu(tr("Analog"), parent);
  this->mp_EventMenu = new QMenu(tr("Event"), parent);
  
  if (toolsMenu != 0)
  {
    toolsMenu->addAction(this->mp_ModelMenu->menuAction());
    toolsMenu->addAction(this->mp_AcquisitionMenu->menuAction());
    toolsMenu->addAction(this->mp_EventMenu->menuAction());
    toolsMenu->addAction(this->mp_MarkerMenu->menuAction());
    toolsMenu->addAction(this->mp_AnalogMenu->menuAction());
  }
  
  this->init();
  this->setActionsEnabled(false);
};

ToolsManager::~ToolsManager()
{
  if (this->mp_ToolsData != 0)
    delete this->mp_ToolsData;
  
  if (this->mp_ModelMenu->parent() == 0)
    delete this->mp_ModelMenu;
  if (this->mp_AcquisitionMenu->parent() == 0)
    delete this->mp_AcquisitionMenu;
  if (this->mp_EventMenu->parent() == 0)
    delete this->mp_EventMenu;
  if (this->mp_MarkerMenu->parent() == 0)
    delete this->mp_MarkerMenu;
  if (this->mp_AnalogMenu->parent() == 0)
    delete this->mp_AnalogMenu;
};

void ToolsManager::addTool(QMenu* menu, const QString& name, AbstractTool* (*tool)(QWidget*))
{
  QAction* action = new QAction(name, menu);
  menu->addAction(action);
  this->m_Tools.insert(action, tool);
  connect(action, SIGNAL(triggered()), this, SLOT(runTool()));
};

void ToolsManager::setActionsEnabled(bool enabled)
{
  for (QMap<QAction*,Tool>::const_iterator it = this->m_Tools.begin() ; it != this->m_Tools.end() ; ++it)
    it.key()->setEnabled(enabled);
};

void ToolsManager::runTool()
{
  QObject* sender = QObject::sender();
  for (QMap<QAction*,Tool>::const_iterator it = this->m_Tools.begin() ; it != this->m_Tools.end() ; ++it)
  {
    if (it.key() == sender)
    {
      AbstractTool* tool = (*it)(this->parentWidget());
      if (tool->run(this->mp_ToolsData->commands(), this->mp_ToolsData))
        this->mp_ToolsData->commands()->push();
    }
  }
};

void ToolsManager::addTool(QMenu* menu, QAction* action, AbstractTool* (*tool)(QWidget*))
{
  menu->addAction(action);
  this->m_Tools.insert(action, tool);
  connect(action, SIGNAL(triggered()), this, SLOT(runTool()));
};
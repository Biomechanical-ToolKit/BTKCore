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

#include "ToolsData.h"
#include "MainWindow.h"
#include "Acquisition.h"
#include "UndoCommands.h"

ToolsData::ToolsData(MainWindow* mw)
{
  this->mp_MainWindow = mw;
  this->mp_Commands = new ToolCommands;
  this->mp_Commands->mp_MainWindow = mw;
};

ToolsData::~ToolsData()
{
  delete this->mp_Commands;
};

Acquisition* const ToolsData::acquisition() const
{
  return this->mp_MainWindow->acquisition();
}

QList<int> ToolsData::explorerSelectedItems(ModelItemTypes type) const
{
  return this->mp_MainWindow->explorerSelectedItems(type);
};

ToolCommands* ToolsData::commands() const
{
  return this->mp_Commands;
}

// ------------------------------------------------------------------------- //

ToolCommands::ToolCommands()
: m_ToolCmds(2,NULL)
{
  this->mp_MainWindow = 0;
};

QUndoCommand* ToolCommands::acquisitionCommand()
{
  return this->generateCommand(0);
};

QUndoCommand* ToolCommands::visualConfigurationCommand()
{
  return this->generateCommand(1);
};

void ToolCommands::push()
{
  QUndoCommand* cmd = new QUndoCommand;
  for (int i = 0 ; i < this->m_ToolCmds.size() ; ++i)
  {
    if (this->m_ToolCmds[i] != NULL)
    {
      if (this->m_ToolCmds[i]->childCount() != 0)
      {
        new MasterUndoCommand(this->mp_MainWindow->undoStack(i), this->m_ToolCmds[i], cmd);
        this->m_ToolCmds[i] = NULL;
      }
      else
      {
        delete this->m_ToolCmds[i];
        this->m_ToolCmds[i] = NULL;
      }
    }
  }
  if (cmd->childCount() != 0)
    this->mp_MainWindow->pushUndoCommand(cmd);
  else
    delete cmd;
};

QUndoCommand* ToolCommands::generateCommand(int idx)
{
  if (this->m_ToolCmds[idx] == NULL)
    this->m_ToolCmds[idx] = new QUndoCommand;
  return this->m_ToolCmds[idx];
};
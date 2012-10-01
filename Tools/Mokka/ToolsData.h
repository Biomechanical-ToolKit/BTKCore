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

#ifndef ToolsData_h
#define ToolsData_h

#include "UserDefined.h"

#include <QVector>
#include <QList>

class MainWindow;
class Acquisition;
class ToolCommands;

class QUndoCommand;

class ToolsData
{
public:
  
  
  ToolsData(MainWindow* resources);
  ~ToolsData();
  
  Acquisition* const acquisition() const;
  QList<int> explorerSelectedItems(ModelItemTypes type) const;
  ToolCommands* commands() const;
  
private:
  MainWindow* mp_MainWindow;
  ToolCommands* mp_Commands;
};

class ToolCommands
{
public:
  ToolCommands();
  
  QUndoCommand* acquisitionCommand();
  QUndoCommand* visualConfigurationCommand();
  
  void push();
  
private:
  friend class ToolsData;
  
  QUndoCommand* generateCommand(int idx);
  
  MainWindow* mp_MainWindow;
  QVector<QUndoCommand*> m_ToolCmds;
};

#endif // ToolsData_h
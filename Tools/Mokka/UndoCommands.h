/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2010, Arnaud Barré
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 *   * Redistributions of source code must retain the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials
 *     provided with the distribution.
 *   * Neither the name(s) of the copyright holders nor the names
 *     of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written
 *     permission.
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
 
#ifndef UndoCommands_h
#define UndoCommands_h

#include <btkAcquisition.h>

#include <QtGui>
#include <QUndoCommand>

class MainWindow;
class NumericalTableWidgetItem;

class MasterUndoCommand : public QUndoCommand
{
public:
  MasterUndoCommand(QUndoStack* stack, QUndoCommand* cmd, QUndoCommand* parent = 0)
  : QUndoCommand(parent)
  {
    this->mp_Stack = stack;
    this->mp_Stack->push(cmd);
  };
  void undo() {this->mp_Stack->undo();};
  void redo() {this->mp_Stack->redo();};

private:
  QUndoStack* mp_Stack;
};

class UndoCommand : public QUndoCommand
{
public:
  typedef enum {None, Acquisition, Configuration} CommandType;
  
  UndoCommand(QUndoCommand* parent = 0)
  : QUndoCommand(parent)
  {
    this->m_CommandType = None;
  };
  
  CommandType commandType() const {return this->m_CommandType;};
  
protected:
  CommandType m_CommandType;
};

class AcquisitionUndoCommand : public UndoCommand
{
public:
  AcquisitionUndoCommand(QUndoCommand* parent = 0)
  : UndoCommand(parent)
  {
    this->m_CommandType = UndoCommand::Acquisition;
  };
};

class ConfigurationUndoCommand : public UndoCommand
{
public:
  ConfigurationUndoCommand(QUndoCommand* parent = 0)
  : UndoCommand(parent)
  {
    this->m_CommandType = UndoCommand::Configuration;
  };
};

// ----------------------------------------------- //
//               POINT/MARKER EDITION              //
// ----------------------------------------------- //

// --------------- EditMarkerLabel ---------------
class EditMarkerLabel : public AcquisitionUndoCommand
{
public:
  EditMarkerLabel(const QString& label, QTableWidgetItem* item, QUndoCommand* parent = 0);
  virtual void undo() {this->action();};
  virtual void redo() {this->action();};
  
private:
  QString m_Label;
  QTableWidgetItem* mp_Item;
  
  void action();
};

// --------------- EditMarkerDescription ---------------
class EditMarkerDescription : public AcquisitionUndoCommand
{
public:
  EditMarkerDescription(const QString& desc, QTableWidgetItem* item, QUndoCommand* parent = 0);
  virtual void undo() {this->action();};
  virtual void redo() {this->action();};
  
private:
  QString m_Description;
  QTableWidgetItem* mp_Item;
  
  void action();
};

// --------------- EditMarkersRadius ---------------
class EditMarkersRadius : public ConfigurationUndoCommand
{
public:
  EditMarkersRadius(double r, QList<QTableWidgetItem*> items, MainWindow* w, QUndoCommand* parent = 0);
  virtual void undo() {this->action();};
  virtual void redo() {this->action();};
  
private:
  QVector<double> m_Radius;
  QList<QTableWidgetItem*> m_Items;
  MainWindow* mp_Main;
  
  void action();
};

// --------------- EditMarkerColorIndex ---------------
class EditMarkersColorIndex : public ConfigurationUndoCommand
{
public:
  EditMarkersColorIndex(int idx, QList<QTableWidgetItem*> items, MainWindow* w, QUndoCommand* parent = 0);
  virtual void undo() {this->action();};
  virtual void redo() {this->action();};
  
private:
  QVector<int> m_Indexes;
  QList<QTableWidgetItem*> m_Items;
  MainWindow* mp_Main;
  
  void action();
};

// --------------- ReorderMarkers ---------------
class EditPoints : public AcquisitionUndoCommand
{
public:
  EditPoints(MainWindow* w, QUndoCommand* parent = 0);
  virtual void undo() {this->action();};
  virtual void redo() {this->action();};
  
private:
  struct pointProp {
    int id;
    bool disabled;
    QString label;
    QString description;
  };
  QVector< pointProp > m_Items;
  MainWindow* mp_Main;
  
  void action();
};

// ----------------------------------------------- //
//                   EVENT EDITION                 //
// ----------------------------------------------- //

// --------------- EditEventLabel ---------------
class EditEventLabel : public AcquisitionUndoCommand
{
public:
  EditEventLabel(const QString& label, int id, const QString& desc, QTableWidgetItem* item, QUndoCommand* parent = 0);
  virtual void undo() {this->action();};
  virtual void redo() {this->action();};
  
private:
  QString m_Label;
  int m_Id;
  QString m_Desc;
  QTableWidgetItem* mp_Item;
  
  void action();
};

// --------------- EditEventContext -------------
class EditEventContext : public AcquisitionUndoCommand
{
public:
  EditEventContext(const QString& context, QTableWidgetItem* item, QUndoCommand* parent = 0);
  virtual void undo() {this->action();};
  virtual void redo() {this->action();};
  
private:
  QString m_Context;
  QTableWidgetItem* mp_Item;
  
  void action();
};

// --------------- EditEventTime ---------------
class EditEventTime : public AcquisitionUndoCommand
{
public:
  EditEventTime(double t, QTableWidgetItem* item, QUndoCommand* parent = 0);
  virtual void undo() {this->action();};
  virtual void redo() {this->action();};
  
private:
  double m_Time;
  QTableWidgetItem* mp_Item;
  
  void action();
};

// --------------- EditEventSubject ---------------=
class EditEventSubject : public AcquisitionUndoCommand
{
public:
  EditEventSubject(const QString& subject, QTableWidgetItem* item, QUndoCommand* parent = 0);
  virtual void undo() {this->action();};
  virtual void redo() {this->action();};
  
private:
  QString m_Subject;
  QTableWidgetItem* mp_Item;
  
  void action();
};

// --------------- NewEvent ---------------=
class NewEvent : public AcquisitionUndoCommand
{
public:
  NewEvent(MainWindow* w, int frame, double freq, QUndoCommand* parent = 0);
  virtual void undo();
  virtual void redo();
  
private:
  MainWindow* mp_Main;
  int m_Frame;
  double m_Freq;
  NumericalTableWidgetItem* mp_Item;
};

// --------------- DeleteEvent ---------------=
class DeleteEvent : public AcquisitionUndoCommand
{
public:
  DeleteEvent(MainWindow* w, QUndoCommand* parent = 0);
  virtual void undo() {this->action();};
  virtual void redo() {this->action();};
  
private:
  MainWindow* mp_Main;
  QTableWidgetItem* mp_Item;
  
  void action();
};

#endif // UndoCommands_h
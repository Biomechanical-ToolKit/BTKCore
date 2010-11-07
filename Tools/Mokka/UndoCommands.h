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

//#include <btkAcquisition.h>

#include <QColor>
#include <QtGui>
#include <QUndoCommand>

#include "Acquisition.h"

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
  typedef enum {None, AcquisitionCmd, ConfigurationCmd} CommandType;
  
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
    this->m_CommandType = UndoCommand::AcquisitionCmd;
  };
};

class ConfigurationUndoCommand : public UndoCommand
{
public:
  ConfigurationUndoCommand(QUndoCommand* parent = 0)
  : UndoCommand(parent)
  {
    this->m_CommandType = UndoCommand::ConfigurationCmd;
  };
};

// ----------------------------------------------- //
//                     GENERAL                     //
// ----------------------------------------------- //

class EditRegionOfInterest : public AcquisitionUndoCommand
{
public:
  EditRegionOfInterest(Acquisition* acq, int lb, int rb, QUndoCommand* parent = 0);
  virtual void undo() {this->action();};
  virtual void redo() {this->action();};
  
private:
  Acquisition* mp_Acquisition;
  int mp_ROI[2];
  
  void action();
};

// ----------------------------------------------- //
//               POINT/MARKER EDITION              //
// ----------------------------------------------- //

// --------------- EditPointLabel ---------------
class EditPointLabel : public AcquisitionUndoCommand
{
public:
  EditPointLabel(Acquisition* acq, int id, const QString& label, QUndoCommand* parent = 0);
  virtual void undo() {this->action();};
  virtual void redo() {this->action();};
  
private:
  Acquisition* mp_Acquisition;
  int m_Id;
  QString m_Label;
  
  void action();
};

// --------------- EditPointDescription ---------------
class EditPointDescription : public AcquisitionUndoCommand
{
public:
  EditPointDescription(Acquisition* acq, const QVector<int>& ids, const QString& desc, QUndoCommand* parent = 0);
  virtual void undo() {this->action();};
  virtual void redo() {this->action();};
  
private:
  Acquisition* mp_Acquisition;
  QVector<int> m_Ids;
  QVector<QString> m_Descriptions;
  
  void action();
};

// --------------- EditMarkersRadius2 ---------------
// TODO: Rename this class when the markerDock will be removed.
class EditMarkersRadius2 : public ConfigurationUndoCommand
{
public:
  EditMarkersRadius2(Acquisition* acq, const QVector<int>& ids, double radius, QUndoCommand* parent = 0);
  virtual void undo() {this->action();};
  virtual void redo() {this->action();};
  
private:
  Acquisition* mp_Acquisition;
  QVector<int> m_Ids;
  QVector<double> m_Radii;
  
  void action();
};

// --------------- EditMarkersColor ---------------
class EditMarkersColor : public ConfigurationUndoCommand
{
public:
  EditMarkersColor(Acquisition* acq, const QVector<int>& ids, const QColor& color, QUndoCommand* parent = 0);
  virtual void undo() {this->action();};
  virtual void redo() {this->action();};
  
private:
  Acquisition* mp_Acquisition;
  QVector<int> m_Ids;
  QVector<QColor> m_Colors;
  
  void action();
};

// --------------- RemovePoints ---------------
class RemovePoints : public AcquisitionUndoCommand
{
public:
  RemovePoints(Acquisition* acq, const QList<int>& ids, QUndoCommand* parent = 0);
  ~RemovePoints();
  virtual void undo();
  virtual void redo();
  
private:
  Acquisition* mp_Acquisition;
  QList<int> m_Ids;
  QList<Point*> m_Points;
};

// ----------------------------------------------- //
//                  ANALOG EDITION                 //
// ----------------------------------------------- //

// --------------- EditAnalogLabel ---------------
class EditAnalogLabel : public AcquisitionUndoCommand
{
public:
  EditAnalogLabel(Acquisition* acq, int id, const QString& label, QUndoCommand* parent = 0);
  virtual void undo() {this->action();};
  virtual void redo() {this->action();};
  
private:
  Acquisition* mp_Acquisition;
  int m_Id;
  QString m_Label;
  
  void action();
};

// --------------- EditAnalogDescription ---------------
class EditAnalogDescription : public AcquisitionUndoCommand
{
public:
  EditAnalogDescription(Acquisition* acq, const QVector<int>& ids, const QString& desc, QUndoCommand* parent = 0);
  virtual void undo() {this->action();};
  virtual void redo() {this->action();};
  
private:
  Acquisition* mp_Acquisition;
  QVector<int> m_Ids;
  QVector<QString> m_Descriptions;
  
  void action();
};

// --------------- EditAnalogsUnit ---------------
class EditAnalogsUnit : public AcquisitionUndoCommand
{
public:
  EditAnalogsUnit(Acquisition* acq, const QVector<int>& ids, const QString& unit, QUndoCommand* parent = 0);
  virtual void undo() {this->action();};
  virtual void redo() {this->action();};
  
private:
  Acquisition* mp_Acquisition;
  QVector<int> m_Ids;
  QVector<QString> m_Units;
  
  void action();
};

// --------------- EditAnalogsGain ---------------
class EditAnalogsGain : public AcquisitionUndoCommand
{
public:
  EditAnalogsGain(Acquisition* acq, const QVector<int>& ids, Analog::Gain gain, QUndoCommand* parent = 0);
  virtual void undo() {this->action();};
  virtual void redo() {this->action();};
  
private:
  Acquisition* mp_Acquisition;
  QVector<int> m_Ids;
  QVector<Analog::Gain> m_Gains;
  
  void action();
};

// --------------- EditAnalogsOffset ---------------
class EditAnalogsOffset : public AcquisitionUndoCommand
{
public:
  EditAnalogsOffset(Acquisition* acq, const QVector<int>& ids, int offset, QUndoCommand* parent = 0);
  virtual void undo() {this->action();};
  virtual void redo() {this->action();};
  
private:
  Acquisition* mp_Acquisition;
  QVector<int> m_Ids;
  QVector<int> m_Offsets;
  
  void action();
};

// --------------- EditAnalogsScale ---------------
class EditAnalogsScale : public AcquisitionUndoCommand
{
public:
  EditAnalogsScale(Acquisition* acq, const QVector<int>& ids, double scale, QUndoCommand* parent = 0);
  virtual void undo() {this->action();};
  virtual void redo() {this->action();};
  
private:
  Acquisition* mp_Acquisition;
  QVector<int> m_Ids;
  QVector<double> m_Scales;
  
  void action();
};

// --------------- RemoveAnalogs ---------------
class RemoveAnalogs : public AcquisitionUndoCommand
{
public:
  RemoveAnalogs(Acquisition* acq, const QList<int>& ids, QUndoCommand* parent = 0);
  ~RemoveAnalogs();
  virtual void undo();
  virtual void redo();
  
private:
  Acquisition* mp_Acquisition;
  QList<int> m_Ids;
  QList<Analog*> m_Analogs;
};

// ----------------------------------------------- //
//                   EVENT EDITION                 //
// ----------------------------------------------- //

// --------------- RemoveEvents ---------------
class RemoveEvents : public AcquisitionUndoCommand
{
public:
  RemoveEvents(Acquisition* acq, const QList<int>& ids, QUndoCommand* parent = 0);
  ~RemoveEvents();
  virtual void undo();
  virtual void redo();
  
private:
  Acquisition* mp_Acquisition;
  QList<int> m_Ids;
  QList<Event*> m_Events;
};

// --------------- InsertEvent ---------------
class InsertEvent : public AcquisitionUndoCommand
{
public:
  InsertEvent(Acquisition* acq, Event* e, QUndoCommand* parent = 0);
  ~InsertEvent();
  virtual void undo();
  virtual void redo();
  
private:
  Acquisition* mp_Acquisition;
  QList<int> m_Ids;
  QList<Event*> m_Events;
};

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

#endif // UndoCommands_h
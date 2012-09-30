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

#ifndef ToolsManager_h
#define ToolsManager_h


#include <QObject>
#include <QWidget>
#include <QMap>

class AbstractTool;
class ToolsData;

class QAction;
class QMenu;

template <class T> AbstractTool* ToolFactory(QWidget* parent = 0) {return new T(parent);};

class ToolsManager : public QObject
{
  Q_OBJECT
  
public:
  ToolsManager(ToolsData* data, QMenu* toolsMenu, QWidget* parent = 0);
  ~ToolsManager();
  
  QMenu* menuModel() const {return this->mp_ModelMenu;};
  QMenu* menuAcquisition() const {return this->mp_AcquisitionMenu;};
  QMenu* menuMarker() const {return this->mp_MarkerMenu;};
  QMenu* menuAnalog() const {return this->mp_AnalogMenu;};
  QMenu* menuEvent() const {return this->mp_EventMenu;};
  
  void addModelTool(const QString& name, AbstractTool* (*tool)(QWidget*)) {this->addTool(this->mp_ModelMenu, name, tool);};
  void addAcquisitionTool(const QString& name, AbstractTool* (*tool)(QWidget*)) {this->addTool(this->mp_AcquisitionMenu, name, tool);};
  void addMarkerTool(const QString& name, AbstractTool* (*tool)(QWidget*)) {this->addTool(this->mp_MarkerMenu, name, tool);};
  void addAnalogTool(const QString& name, AbstractTool* (*tool)(QWidget*)) {this->addTool(this->mp_AnalogMenu, name, tool);};
  void addEventTool(const QString& name, AbstractTool* (*tool)(QWidget*)) {this->addTool(this->mp_EventMenu, name, tool);};
  
  // void addModelTool(QAction* action, AbstractTool* (*tool)(QWidget*)) {this->addTool(this->mp_ModelMenu, action, tool);};
  // void addAcquisitionTool(QAction* action, AbstractTool* (*tool)(QWidget*)) {this->addTool(this->mp_AcquisitionMenu, action, tool);};
  // void addPointTool(QAction* action, AbstractTool* (*tool)(QWidget*)) {this->addTool(this->mp_MarkerMenu, action, tool);};
  // void addAnalogTool(QAction* action, AbstractTool* (*tool)(QWidget*)) {this->addTool(this->mp_AnalogMenu, action, tool);};
  // void addEventTool(QAction* action, AbstractTool* (*tool)(QWidget*)) {this->addTool(this->mp_EventMenu, action, tool);};
  
  void addTool(QMenu* menu, const QString& name, AbstractTool* (*tool)(QWidget*));
  
  void setActionsEnabled(bool enabled);
  
  QWidget* parentWidget() const {return static_cast<QWidget*>(this->parent());};

private slots:
  void runTool();

private:
  void init();
  void addTool(QMenu* menu, QAction* action, AbstractTool* (*tool)(QWidget*));
  
  typedef AbstractTool* (*Tool)(QWidget*);
  
  ToolsData* mp_ToolsData;
  QMap<QAction*,Tool> m_Tools;
  
  QMenu* mp_ModelMenu;
  QMenu* mp_AcquisitionMenu;
  QMenu* mp_MarkerMenu;
  QMenu* mp_AnalogMenu;
  QMenu* mp_EventMenu;
};

#endif // ToolsManager_h
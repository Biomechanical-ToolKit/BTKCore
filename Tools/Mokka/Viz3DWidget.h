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

#ifndef Viz3DWidget_h
#define Viz3DWidget_h

#include <btkVTKAxesWidget.h>

#include <QVTKWidget.h>
#include <vtkRenderer.h>
#include <vtkEventQtSlotConnect.h>

#include <QList>

class Acquisition;
class vtkStreamingDemandDrivenPipelineCollection;
class vtkProcessMap;

class Viz3DWidget : public QVTKWidget
{
  Q_OBJECT
  
public:
  enum {Top, Bottom, Left, Right, Back, Front};
  Viz3DWidget(QWidget* parent = 0);
  ~Viz3DWidget();
  
  void initialize();
  Acquisition* acquisition() const {return this->mp_Acquisition;};
  void setAcquisition(Acquisition* acq) {this->mp_Acquisition = acq;};
  vtkRenderer* renderer() const {return this->mp_Renderer;};
  void restoreProjectionCameraConfiguration();
  void saveProjectionCameraConfiguration();
  void copyProjectionCameraConfiguration(Viz3DWidget* source);
  void setOrthogonalView(int view);
  
  void copy(Viz3DWidget* source);
  
public slots:
  // Qt / VTK
  void selectPickedMarker(vtkObject* caller, unsigned long vtk_event, void* client_data, void* call_data);
  void togglePickedMarker(vtkObject* caller, unsigned long vtk_event, void* client_data, void* call_data);
  void toggleSelectedMarkers(vtkObject* caller, unsigned long vtk_event, void* client_data, void* call_data);
  void toggleTrajectoryMarker(vtkObject* caller, unsigned long vtk_event, void* client_data, void* call_data);
  // Qt
  void render();
  void show(bool s);
  
signals:
  void pickedMarkerChanged(int id);
  void pickedMarkerToggled(int id);
  void selectedMarkersToggled(const QList<int>& ids);
  void trajectoryMarkerToggled(int id);
  
protected:
  bool event(QEvent* event);
  void keyPressEvent(QKeyEvent* event);
  void keyReleaseEvent(QKeyEvent* event);
  void mousePressEvent(QMouseEvent* event);
  void resizeEvent(QResizeEvent* event);
  
private:
  Acquisition* mp_Acquisition;
  vtkRenderer* mp_Renderer;
  btk::VTKAxesWidget* mp_AxesWidget;
  vtkEventQtSlotConnect* mp_EventQtSlotConnections;
  
  bool m_CameraConfigurationSaved;
  double mp_CamFocalPoint[3];
  double mp_CamPosition[3];
  double mp_CamViewUp[3];
};

#endif // Viz3DWidget_h
/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2010, Arnaud Barré
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

#include <btkAcquisition.h>
#include <btkProcessObject.h>
#include <btkSeparateKnownVirtualMarkersFilter.h>
#include <btkVTKAxesWidget.h>
#include <btkVTKForcePlatformsSource.h>
#include <btkVTKGRFsFramesSource.h>
#include <btkVTKMarkersFramesSource.h>

#include <QTableWidgetItem>

#include <map>

#include <QVTKWidget.h>
#include <vtkRenderer.h>
#include <vtkEventQtSlotConnect.h>
//#include <vtkObject.h>
#include <vtkMapperCollection.h>

class vtkStreamingDemandDrivenPipelineCollection;
class vtkProcessMap;

class Viz3DWidget : public QVTKWidget
{
  Q_OBJECT
  
public:
  Viz3DWidget(QWidget* parent = 0);
  ~Viz3DWidget();
  
  void initialize();
  vtkRenderer* renderer() const {return this->mp_Renderer;};
  
  void setMarkerRadius(int id, double r);
  double markerRadius(int id);
  void setMarkerColorIndex(int id, int idx);
  int markerColorIndex(int id);
  void setMarkerVisibility(int id, bool visible);
  bool markerVisibility(int id);
  double* markerColorValue(int c);
  bool appendNewMarkerColor(const QColor& color, int* idx);
  void setGroundOrientation(double x, double y, double z);
  btk::SeparateKnownVirtualMarkersFilter::Pointer load(btk::Acquisition::Pointer acq);
  
public slots:
  // Qt / VTK
  void updateDisplayedMarkersList(vtkObject* caller, unsigned long vtk_event, void* client_data, void* call_data);
  void selectPickedMarker(vtkObject* caller, unsigned long vtk_event, void* client_data, void* call_data);
  void selectPickedMarkers(vtkObject* caller, unsigned long vtk_event, void* client_data, void* call_data);
  // Qt
  void clear();
  void circleSelectedMarkers(QList<QTableWidgetItem*> items);
  void updateDisplay();
  void updateDisplay(int frame);
  void showSelectedMarkers(const QList<QTableWidgetItem*>& items);
  void hideSelectedMarkers(const QList<QTableWidgetItem*>& items);
  void showAllMarkers();
  void hideAllMarkers();
  void updateMarkerVisibility(QTableWidgetItem* item);
  
signals:
  void fileDropped(const QString& filename);
  void visibleMarkersChanged(const QVector<int>& ids);
  void pickedMarkerChanged(int id);
  void pickedMarkersChanged(int id);
  
protected:
  void dragEnterEvent(QDragEnterEvent *event);
  void dropEvent(QDropEvent *event);
  
private:
  vtkRenderer* mp_Renderer;
  btk::VTKAxesWidget* mp_AxesWidget;
  vtkEventQtSlotConnect* mp_EventQtSlotConnections;
  vtkProcessMap* mp_VTKProc;
  vtkMapperCollection* mp_Mappers;
  vtkStreamingDemandDrivenPipelineCollection* mp_Syncro;
  std::map<int, btk::ProcessObject::Pointer> m_BTKProc;
  int m_FirstFrame;
};

#endif // Viz3DWidget_h
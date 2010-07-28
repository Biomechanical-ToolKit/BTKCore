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

#ifndef MultiViewWidget_h
#define MultiViewWidget_h

#include "AbstractMultiView.h"

#include <btkProcessObject.h>
#include <btkAcquisition.h>
#include <btkSeparateKnownVirtualMarkersFilter.h>

#include <map>

#include <vtkEventQtSlotConnect.h>
#include <vtkMapperCollection.h>

// Forward declaration
class AbstractView;
class QTableWidgetItem;
class vtkStreamingDemandDrivenPipelineCollection;
class vtkProcessMap;

class MultiViewWidget : public AbstractMultiView
{
  Q_OBJECT
  
public:
  MultiViewWidget(QWidget* parent = 0);
  ~MultiViewWidget();
  // MultiViewWidget(const MultiViewWidget&); // Implicit.
  // MultiViewWidget& operator=(const MultiViewWidget&); // Implicit.
  
  void initialize();
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

protected:
  void dragEnterEvent(QDragEnterEvent *event);
  void dropEvent(QDropEvent *event);
  
  AbstractView* createView(AbstractView* fromAnother = 0);

signals:
  void fileDropped(const QString& filename);
  void visibleMarkersChanged(const QVector<int>& ids);
  void pickedMarkerChanged(int id);
  void pickedMarkersChanged(int id);
  
private:
  void updateViews();
  
  vtkEventQtSlotConnect* mp_EventQtSlotConnections;
  vtkProcessMap* mp_VTKProc;
  vtkMapperCollection* mp_Mappers;
  vtkStreamingDemandDrivenPipelineCollection* mp_Syncro;
  std::map<int, btk::ProcessObject::Pointer> m_BTKProc;
  int m_FirstFrame;
};

#endif // MultiViewWidget_h
/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2011, Arnaud Barré
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

#ifndef AbstractChartWidget_h
#define AbstractChartWidget_h

#include <btkVTKChartTimeSeries.h>

#include "Acquisition.h"

#include <QTreeWidgetItem>
#include <QLabel>

#include <QVTKWidget.h>
#include <vtkTable.h>
#include <vtkDoubleArray.h>
#include <vtkPlot.h>
#include <vtkLine.h>
#include <vtkAxis.h>
#include <vtkstd/vector>

class QDragEnterEvent;
class QDropEvent;

class vtkColorSeries;
class ChartOptionsWidget;

class VTKCharts : public vtkstd::vector<btk::VTKChartTimeSeries*>
{};

class VTKChartWidget;

class AbstractChartWidget : public QWidget
{
  Q_OBJECT
  
public:
  AbstractChartWidget(int numCharts, QWidget* parent = 0);
  virtual ~AbstractChartWidget();
  
  virtual void initialize();
  virtual void copy(AbstractChartWidget* source);
  virtual bool acceptDroppedTreeWidgetItem(QTreeWidgetItem* /* item */) {return false;};
  
  QLabel* chartTitle() const {return this->mp_ChartTitleLabel;};
  VTKChartWidget* chartContent() const {return this->mp_ChartContentWidget;};
  QLabel* chartAxisXTitle() const {return this->mp_ChartAxisXLabel;};
  
  Acquisition* acquisition() {return this->mp_Acquisition;};
  virtual void setAcquisition(Acquisition* acq) {this->mp_Acquisition = acq;};
  vtkDoubleArray* frameArray() {return this->mp_ArrayFrames;};
  void setFrameArray(vtkDoubleArray* array);
  ChartOptionsWidget* options() {return mp_ChartOptions;};
  void toggleOptions(const QPoint& pos);
  
  btk::VTKCurrentFrameFunctor::Pointer currentFrameFunctor() const;
  void setCurrentFrameFunctor(btk::VTKCurrentFrameFunctor::Pointer functor);
  
  btk::VTKRegionOfInterestFunctor::Pointer regionOfInterestFunctor() const;
  void setRegionOfInterestFunctor(btk::VTKRegionOfInterestFunctor::Pointer functor);
  
  btk::VTKEventsFunctor::Pointer eventsFunctor() const;
  void setEventsFunctor(btk::VTKEventsFunctor::Pointer functor);
  
  void render();
  void show(bool s);
  
public slots:
  void removePlot(int index);
  void setPlotLineColor(const QList<int>& indices, const QColor& color);
  void setPlotLineWidth(const QList<int>& indices, double value);
  void setChartTitle(const QString& title);
  void updatePlotLabel(int itemId);
  void hidePlots(const QList<int>& itemIds);
  void showPlots(const QList<int>& itemIds);
  void resetZoom();
  void exportToImage();
  void removeAllPlot();
  void toggleEventDisplay();
  
private slots:
  void setLastContextMenuPosition(const QPoint& globalPos);
  
protected:
  virtual void dragEnterEvent(QDragEnterEvent *event);
  virtual void dropEvent(QDropEvent* event);
  
  bool isAlreadyPlotted(int id);
  virtual QString createPlotLabel(int id) = 0;
  virtual bool appendPlotFromDroppedItem(QTreeWidgetItem* item, int* itemId, QString& legend, double* color, double* width) = 0;
  void setPlotsVisible(const QList<int>& itemIds, bool show);
  void checkResetAxes();
  void generateColor(double color[3]);
  
  QLabel* mp_ChartTitleLabel;
  VTKChartWidget* mp_ChartContentWidget;
  QLabel* mp_ChartAxisXLabel;
  
  Acquisition* mp_Acquisition;
  VTKCharts* mp_Charts;
  vtkDoubleArray* mp_ArrayFrames;
  vtkColorSeries* mp_ColorGenerator;
  ChartOptionsWidget* mp_ChartOptions;
  QList<QAction*> m_ViewActions;
  QPoint m_LastContextMenuPosition;
};

class VTKChartWidget : public QVTKWidget
{
  Q_OBJECT
  
public:
  VTKChartWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);
  void setCharts(VTKCharts* charts) {this->mp_Charts = charts;};
  btk::VTKChartTimeSeries* focusedChart(const QPoint& pos) const;
  void resizeCharts();
  
signals:
  void contextMenuRequested(const QPoint& pos);
  
protected:
  virtual void contextMenuEvent(QContextMenuEvent* event);
  virtual bool event(QEvent* event);
  virtual void keyPressEvent(QKeyEvent* event);
  virtual void keyReleaseEvent(QKeyEvent* event);
  virtual void mousePressEvent(QMouseEvent* event);
  virtual void mouseReleaseEvent(QMouseEvent* event);
  virtual void mouseMoveEvent(QMouseEvent* event);
  virtual void resizeEvent(QResizeEvent* event);
  virtual void wheelEvent(QWheelEvent* event);
  
  VTKCharts* mp_Charts;
};

#endif // AbstractChartWidget_h
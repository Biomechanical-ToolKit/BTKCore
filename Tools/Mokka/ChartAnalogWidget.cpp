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

#include "ChartAnalogWidget.h"
#include "ChartOptionsWidget.h"
#include "UserRoles.h"

#include <vtkChartXY.h>
#include <vtkContextScene.h>
#include <vtkContextView.h>
#include <vtkDoubleArray.h>
#include <vtkPlotPoints.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkTable.h>
#include <vtkAxis.h>
#include <vtkTextProperty.h>

#include <QVTKWidget.h>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QTreeWidget>
#include <QDockWidget>
#include <QCursor>

ChartAnalogWidget::ChartAnalogWidget(QWidget* parent)
: QVTKWidget(parent)
{
  this->mp_Acquisition = 0;
  this->mp_VTKChart = 0;
  this->mp_ArrayFrames = 0;
  this->mp_ChartOptions = new ChartOptionsWidget(this);
  this->mp_ChartOptions->setVisible(false);
  this->mp_ChartOptions->clear();
  
  connect(this->mp_ChartOptions, SIGNAL(lineColorChanged(QList<int>, QColor)), this, SLOT(setPlotLineColor(QList<int>, QColor)));
  connect(this->mp_ChartOptions, SIGNAL(lineWidthChanged(QList<int>, double)), this, SLOT(setPlotLineWidth(QList<int>, double)));
  connect(this->mp_ChartOptions, SIGNAL(plotRemoved(int)), this, SLOT(removePlot(int)));
  
  this->setAcceptDrops(true);
}

ChartAnalogWidget::~ChartAnalogWidget()
{
  this->mp_VTKChart->Delete();
  if (this->mp_ArrayFrames != NULL)
    this->mp_ArrayFrames->Delete();
  delete this->mp_ChartOptions;
};

void ChartAnalogWidget::initialize()
{
  vtkContextView* view = vtkContextView::New();
  view->SetInteractor(this->GetInteractor());
  this->SetRenderWindow(view->GetRenderWindow());
  this->GetRenderWindow()->LineSmoothingOn();
  this->mp_VTKChart = vtkChartXY::New();
  this->mp_VTKChart->SetShowLegend(true);
  this->mp_VTKChart->SetAutoAxes(false);
  // this->mp_VTKChart->SetForceAxesToBounds(true); // VTK 5.8
  // ColorSeries?
  //
  view->GetScene()->AddItem(this->mp_VTKChart);
  view->Delete();
  
  this->mp_VTKChart->GetAxis(vtkAxis::BOTTOM)->SetTitle("Frames"); // X axis
  // this->mp_VTKChart->GetAxis(vtkAxis::TOP)->SetVisible(true);
  this->mp_VTKChart->GetAxis(vtkAxis::LEFT)->SetTitle("Values"); // Y axis
  // this->mp_VTKChart->GetAxis(vtkAxis::RIGHT)->SetVisible(true);
  
  this->show(false); // Reset the chart
};

void ChartAnalogWidget::setFrameArray(vtkDoubleArray* array)
{
  if (this->mp_ArrayFrames == array)
    return;
  else if (this->mp_ArrayFrames != NULL)
    this->mp_ArrayFrames->Delete();
  this->mp_ArrayFrames = array;
};

void ChartAnalogWidget::show(bool s)
{
  // Crash when using vtkChartXY::ClearPlots(), should be fixed with VTK 5.8
  for (int i = this->mp_VTKChart->GetNumberOfPlots()-1 ; i >= 0 ; --i)
    this->mp_VTKChart->RemovePlot(i);
  
#if 1  // Some limitations with VTK 5.6
  vtkTable* table = vtkTable::New();
  vtkDoubleArray* arrFra = vtkDoubleArray::New();
  arrFra->SetName("Frames");
  table->AddColumn(arrFra);
  vtkDoubleArray* arrVal = vtkDoubleArray::New();
  arrVal->SetName("Values");
  table->AddColumn(arrVal);
  if (!s) // Reset
  {
    const int numFrames = 2;
    table->SetNumberOfRows(numFrames);
    for (int i = 0; i < numFrames; ++i)
    {
      table->SetValue(i, 0, 0.0);
      table->SetValue(i, 1, 0.0);
    }
  }
  else // Load
  {
    table->SetNumberOfRows(2);
    // X
    table->SetValue(0, 0, (double)this->mp_Acquisition->firstFrame());
    table->SetValue(1, 0, (double)this->mp_Acquisition->lastFrame());
    // Y
    table->SetValue(0, 1, 0.0);
    table->SetValue(1, 1, 0.0);
  }
  vtkPlot* points = this->mp_VTKChart->AddPlot(vtkChart::POINTS);
  points->SetInput(table,0,1);
  arrFra->Delete();
  arrVal->Delete();
  table->Delete();
#else
  if (!s) // Reset
  {
    this->mp_VTKChart->GetAxis(vtkAxis::BOTTOM)->SetRange(0.0, 0.0); // X axis
    this->mp_VTKChart->GetAxis(vtkAxis::LEFT)->SetRange(0.0, 0.0); // Y axis
    this->mp_ChartOptions->setVisible(false);
  }
  else // Load
  {
    this->mp_VTKChart->GetAxis(vtkAxis::BOTTOM)->SetRange((double)this->mp_Acquisition->firstFrame(), 
                                                          (double)this->mp_Acquisition->lastFrame()); // X axis
    this->mp_VTKChart->GetAxis(vtkAxis::LEFT)->SetRange(0.0, 0.0); // Y axis
  }
#endif
};

void ChartAnalogWidget::removePlot(int index)
{
  this->mp_VTKChart->RemovePlot(index + 1); // FIXME: +1 required due to the first plot used to fix the X axis range ... MUST BE REMOVED WITH VTK 5.8
  // Force the update
  this->render();
};

void ChartAnalogWidget::render()
{
#if 1  
  this->mp_VTKChart->RecalculateBounds(); // FIXME: Only when resizing and not dropping things
  this->GetRenderWindow()->Render();
#else
  if ((this->height() > 0) && (this->width() > 0))
  {
    this->mp_VTKChart->RecalculateBounds();
    this->GetRenderWindow()->Render();
  }
#endif
};

void ChartAnalogWidget::setPlotLineColor(const QList<int>& indices, const QColor& color)
{
  for (QList<int>::const_iterator it = indices.begin() ; it != indices.end() ; ++it)
  {
    vtkPlot* plot = this->mp_VTKChart->GetPlot(*it + 1); // FIXME: +1 required due to the first plot used to fix the X axis range ... MUST BE REMOVED WITH VTK 5.8
    if (plot)
      plot->SetColor(color.redF(), color.greenF(), color.blueF());
    else
      qDebug("No plot with the index %i", *it);
  }
  // Force the update
  this->render();
};

void ChartAnalogWidget::setPlotLineWidth(const QList<int>& indices, double value)
{
  for (QList<int>::const_iterator it = indices.begin() ; it != indices.end() ; ++it)
  {
    vtkPlot* plot = this->mp_VTKChart->GetPlot(*it + 1); // FIXME: +1 required due to the first plot used to fix the X axis range ... MUST BE REMOVED WITH VTK 5.8
    if (plot)
      plot->SetWidth(static_cast<float>(value));
    else
      qDebug("No plot with the index %i", *it);
  }
  // Force the update
  this->render();
};

void ChartAnalogWidget::toggleOptions(const QPoint& pos)
{
  if (this->mp_ChartOptions->isVisible())
    this->mp_ChartOptions->setVisible(false);
  else
  {
    this->mp_ChartOptions->move(pos - QPoint(this->mp_ChartOptions->width() / 2, 0));
    this->mp_ChartOptions->setVisible(true);
  }
};

void ChartAnalogWidget::dragEnterEvent(QDragEnterEvent* event)
{
  event->ignore();
  if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist"))
  {
    QTreeWidget* treeWidget = qobject_cast<QTreeWidget*>(event->source());
    if (treeWidget)
    {
      QList<QTreeWidgetItem*> selectedItems = treeWidget->selectedItems();
      for (QList<QTreeWidgetItem*>::const_iterator it = selectedItems.begin() ; it != selectedItems.end() ; ++it)
      {
        if ((*it)->type() != QTreeWidgetItem::UserType + 4) // FIXME: Must be replaced by ModelDockWidget::AnalogType or an enum value for the type analog channel
          return;
      }
    }
    event->setDropAction(Qt::CopyAction); // To have the cross (+) symbol
    event->accept();
  }
};

void ChartAnalogWidget::dropEvent(QDropEvent* event)
{
  event->setDropAction(Qt::IgnoreAction); // Only to know which Analog IDs were dropped.
  event->accept();
  QTreeWidget* treeWidget = qobject_cast<QTreeWidget*>(event->source());
  QList<QTreeWidgetItem*> selectedItems = treeWidget->selectedItems();
  for (QList<QTreeWidgetItem*>::const_iterator it = selectedItems.begin() ; it != selectedItems.end() ; ++it)
  {
    int id = (*it)->data(0, analogId).toInt();
    if (id > this->mp_Acquisition->analogCount())
    {
      qDebug("Analog ID greater than the number of analog channels.");
      continue;
    }
    btk::Analog::Pointer analog = this->mp_Acquisition->btkAcquisition()->GetAnalog(id);
    vtkTable* table = vtkTable::New();
    table->SetNumberOfRows(this->mp_Acquisition->analogFrameNumber()); // Must be set before adding column
    table->AddColumn(this->mp_ArrayFrames);
    vtkDoubleArray* arrVal = vtkDoubleArray::New();
    std::string legend = analog->GetLabel() + " (" + analog->GetUnit() + ")";
    arrVal->SetName(legend.c_str());
    // FIXME: Conflict into VTK 5.6.1 between the documentation and the code to save or not the data. Need to check with VTK 5.8
    arrVal->SetArray(analog->GetValues().data(), analog->GetFrameNumber(), 1); // Would be 0?
    table->AddColumn(arrVal);
    
    vtkPlot* line = this->mp_VTKChart->AddPlot(vtkChart::LINE);
    line->SetInput(table,0,1);
    line->SetWidth(0.75);
    arrVal->Delete();
    table->Delete();
    
    double c[3]; line->GetColor(c);
    int color[3] = {static_cast<int>(c[0]*255.0), static_cast<int>(c[1]*255.0), static_cast<int>(c[2]*255.0)};
    this->mp_ChartOptions->appendPlot(QString::fromStdString(legend), color, line->GetWidth());
  }
};

void ChartAnalogWidget::paintEvent(QPaintEvent* event)
{
  this->QVTKWidget::paintEvent(event);
  // Fix as the interactor used by QVTKWidget does not update the rendering
  // when added in a layout (or the problem is elsewhere in the code of Mokka...)
  this->render();
};
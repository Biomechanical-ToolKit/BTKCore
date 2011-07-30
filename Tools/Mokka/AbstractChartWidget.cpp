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

#include "AbstractChartWidget.h"
#include "ChartOptionsWidget.h"

#include <vtkContextScene.h>
#include <vtkContextActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkInteractorStyle.h>
#include <QVTKWidget.h>

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QTreeWidget>

AbstractChartWidget::AbstractChartWidget(int numCharts, QWidget* parent)
: QWidget(parent)
{
  this->mp_Acquisition = 0;
  this->mp_VTKCharts = new VTKCharts;
  this->mp_VTKCharts->resize(numCharts);
  this->mp_ArrayFrames = 0;
  this->mp_ChartOptions = new ChartOptionsWidget(this);
  this->mp_ChartOptions->setVisible(false);
  this->mp_ChartOptions->clear();
  
  this->setFocusPolicy(Qt::StrongFocus);
  
  QVBoxLayout* layout = new QVBoxLayout(this);
  for (int i = 0 ; i < numCharts ; ++i)
    layout->addWidget(new QVTKWidget(this));
  layout->setContentsMargins(0,0,0,0);
  layout->setSpacing(0);
  
  connect(this->mp_ChartOptions, SIGNAL(lineColorChanged(QList<int>, QColor)), this, SLOT(setPlotLineColor(QList<int>, QColor)));
  connect(this->mp_ChartOptions, SIGNAL(lineWidthChanged(QList<int>, double)), this, SLOT(setPlotLineWidth(QList<int>, double)));
  connect(this->mp_ChartOptions, SIGNAL(plotRemoved(int)), this, SLOT(removePlot(int)));
  
  this->setAcceptDrops(true);
}

AbstractChartWidget::~AbstractChartWidget()
{
  for (size_t i = 0 ; i < this->mp_VTKCharts->size() ; ++i)
  {
    vtkChartXY* chart = this->mp_VTKCharts->operator[](i);
    if (chart != NULL)
      chart->Delete();
  }
  delete this->mp_VTKCharts;
  
  if (this->mp_ArrayFrames != NULL)
    this->mp_ArrayFrames->Delete();
};

void AbstractChartWidget::initialize()
{
  // vtkContextView* view = vtkContextView::New();
  //   view->SetInteractor(this->GetInteractor());
  //   this->SetRenderWindow(view->GetRenderWindow());
  // this->GetRenderWindow()->LineSmoothingOn(); // Strange behavior under MacOX X... Each plot has two colors.
  // this->GetRenderWindow()->DoubleBufferOff(); // Blinking under Windows XP
  
  if (this->mp_VTKCharts->size() == 0)
    return;
  
  for (size_t i = 0 ; i < this->mp_VTKCharts->size() ; ++i)
  {
    vtkRenderer* ren = vtkRenderer::New();
    ren->SetBackground(1.0,1.0,1.0);
    vtkRenderWindow* renwin = static_cast<QVTKWidget*>(this->layout()->itemAt((int)i)->widget())->GetRenderWindow();
    renwin->AddRenderer(ren);
    
    vtkChartXY* chart = vtkChartXY::New(); // Do not delete
    // chart->SetShowLegend(true);
    chart->SetAutoAxes(false);
    // this->mp_VTKChart->SetForceAxesToBounds(true); // VTK 5.8
    // ColorSeries?
    chart->GetAxis(vtkAxis::BOTTOM)->SetTitle("Frames"); // X axis
    // chart->GetAxis(vtkAxis::LEFT)->SetTitle("Values"); // Y axis
    this->mp_VTKCharts->operator[](i) = chart;
    
    vtkContextScene* scene = vtkContextScene::New();
    vtkContextActor* actor = vtkContextActor::New();
    scene->AddItem(chart);
    actor->SetScene(scene);
    ren->AddActor(actor);
    scene->SetRenderer(ren);
    scene->SetInteractorStyle(vtkInteractorStyle::SafeDownCast(renwin->GetInteractor()->GetInteractorStyle()));
    
    actor->Delete();
    scene->Delete();
    ren->Delete();
  }
  
  this->show(false); // Reset the charts
};

void AbstractChartWidget::copy(AbstractChartWidget* source)
{
  // Clean if necessary
  for (size_t i = 0 ; i < this->mp_VTKCharts->size() ; ++i)
  {
    vtkChartXY* chart = this->mp_VTKCharts->operator[](i);
    while (chart->GetNumberOfPlots() > 0)
      chart->RemovePlot(0);
    this->mp_ChartOptions->clear();
  }
  
  // Copy the acquisition pointer
  this->setAcquisition(source->acquisition());
  // Copy the X axis
  this->setFrameArray(source->frameArray());
#if 1
  // FIXME: MUST BE REMOVED WITH VTK 5.8
  // Copy only the first (fake) plot 
  for (size_t i = 0 ; i < this->mp_VTKCharts->size() ; ++i)
  {
    vtkChartXY* sourceChart = source->mp_VTKCharts->operator[](i);
    vtkChartXY* targetChart = this->mp_VTKCharts->operator[](i);
    targetChart->AddPlot(vtkChart::POINTS)->SetInput(sourceChart->GetPlot(0)->GetInput(),0,1);
  }
#else
  // Copy the plots
  for (size_t i = 0 ; i < this->mp_VTKCharts->size() ; ++i)
  {
    vtkChartXY* sourceChart = source->mp_VTKCharts->operator[](i);
    vtkChartXY* targetChart = this->mp_VTKCharts->operator[](i);
    targetChart->AddPlot(vtkChart::POINTS)->SetInput(sourceChart->GetPlot(0)->GetInput(),0,1); // FIXME: MUST BE REMOVED WITH VTK 5.8
    for (int j = 1 ; j < sourceChart->GetNumberOfPlots() ; ++j)
    {
      vtkPlot* targetLine = targetChart->AddPlot(vtkChart::LINE);
      vtkPlot* sourceLine = sourceChart->GetPlot(j);
      targetLine->SetInput(sourceLine->GetInput(), 0, 1);
      targetLine->SetWidth(sourceLine->GetWidth());
      double color[3]; sourceLine->GetColor(color); targetLine->SetColor(color[0], color[1], color[2]);
    }
  }
  // Copy the plots' options
  for (int i = 0 ; i < source->mp_ChartOptions->plotTable->rowCount() ; ++i)
  {
    QTableWidgetItem* item = source->mp_ChartOptions->plotTable->item(i,0);
    QColor c = item->data(ChartOptionsWidget::LineColor).value<QColor>();
    int color[3] = {c.red(), c.green(), c.blue()};
    this->mp_ChartOptions->appendPlot(item->text(), color, item->data(ChartOptionsWidget::LineWidth).toDouble());
  }
#endif
};

void AbstractChartWidget::setFrameArray(vtkDoubleArray* array)
{
  if (this->mp_ArrayFrames == array)
    return;
  else if (this->mp_ArrayFrames != NULL)
    this->mp_ArrayFrames->Delete(); // Same as  Unregister
  this->mp_ArrayFrames = array;
  array->Register(this->mp_ArrayFrames);
};

void AbstractChartWidget::show(bool s)
{
  for (size_t i = 0 ; i < this->mp_VTKCharts->size() ; ++i)
  {
    vtkChartXY* chart = this->mp_VTKCharts->operator[](i);
    
    // Crash when using vtkChartXY::ClearPlots(), should be fixed with VTK 5.8
    for (int i = chart->GetNumberOfPlots()-1 ; i >= 0 ; --i)
      chart->RemovePlot(i);
  
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
      double ff = (double)this->mp_Acquisition->firstFrame();
      double lf = (double)this->mp_Acquisition->lastFrame();
      table->SetValue(0, 0, ff);
      table->SetValue(1, 0, lf);
      chart->GetAxis(vtkAxis::BOTTOM)->SetRange(ff, lf); // X axis
      // Y
      table->SetValue(0, 1, 0.0);
      table->SetValue(1, 1, 0.0);
    }
    vtkPlot* points = chart->AddPlot(vtkChart::POINTS);
    points->SetInput(table,0,1);
    arrFra->Delete();
    arrVal->Delete();
    table->Delete();
#else
    if (!s) // Reset
    {
      chart->GetAxis(vtkAxis::BOTTOM)->SetRange(0.0, 0.0); // X axis
      chart->GetAxis(vtkAxis::LEFT)->SetRange(0.0, 0.0); // Y axis
      this->mp_ChartOptions->setVisible(false);
    }
    else // Load
    {
      chart->GetAxis(vtkAxis::BOTTOM)->SetRange((double)this->mp_Acquisition->firstFrame(), 
                                                            (double)this->mp_Acquisition->lastFrame()); // X axis
      chart->GetAxis(vtkAxis::LEFT)->SetRange(0.0, 0.0); // Y axis
    }
#endif
  }
};

void AbstractChartWidget::removePlot(int index)
{
  for (size_t i = 0 ; i < this->mp_VTKCharts->size() ; ++i)
    this->mp_VTKCharts->operator[](i)->RemovePlot(index + 1); // FIXME: +1 required due to the first plot used to fix the X axis range ... MUST BE REMOVED WITH VTK 5.8
  this->fixAxesVisibility(); // Show the first plot if the others were removed or hidden
  this->render();
};

void AbstractChartWidget::render()
{
  for (size_t i = 0 ; i < this->mp_VTKCharts->size() ; ++i)
  {
    //this->mp_VTKCharts->operator[](i)->RecalculateBounds(); // FIXME: Only when resizing and not dropping things
    QVTKWidget* w = static_cast<QVTKWidget*>(this->layout()->itemAt((int)i)->widget());
    if (w->isVisible())
      w->GetRenderWindow()->Render();
  }
};

void AbstractChartWidget::setPlotLineColor(const QList<int>& indices, const QColor& color)
{
  for (QList<int>::const_iterator it = indices.begin() ; it != indices.end() ; ++it)
  {
    for (size_t i = 0 ; i < this->mp_VTKCharts->size() ; ++i)
    {
      vtkPlot* plot = this->mp_VTKCharts->operator[](i)->GetPlot(*it + 1); // FIXME: +1 required due to the first plot used to fix the X axis range ... MUST BE REMOVED WITH VTK 5.8
      plot->SetColor(color.redF(), color.greenF(), color.blueF());
    }
  }
  this->render();
};

void AbstractChartWidget::setPlotLineWidth(const QList<int>& indices, double value)
{
  for (QList<int>::const_iterator it = indices.begin() ; it != indices.end() ; ++it)
  {
    for (size_t i = 0 ; i < this->mp_VTKCharts->size() ; ++i)
    {
      vtkPlot* plot = this->mp_VTKCharts->operator[](i)->GetPlot(*it + 1); // FIXME: +1 required due to the first plot used to fix the X axis range ... MUST BE REMOVED WITH VTK 5.8
      plot->SetWidth(static_cast<float>(value));
    }
  }
  this->render();
};

void AbstractChartWidget::updatePlotLabel(int itemId)
{
  int plotIdx = -1;
  QString label;
  for (int i = 0 ; i < this->mp_ChartOptions->plotTable->rowCount() ; ++i)
  {
    QTableWidgetItem* item = this->mp_ChartOptions->plotTable->item(i, 0);
    if (item->data(ChartOptionsWidget::ItemId).toInt() == itemId)
    {
      label = this->createPlotLabel(itemId);
      item->setText(label);
      plotIdx = i;
      break;
    }
  }
  if (plotIdx != -1)
  {
    for (size_t i = 0 ; i < this->mp_VTKCharts->size() ; ++i)
    {
      vtkPlot* plot = this->mp_VTKCharts->operator[](i)->GetPlot(plotIdx + 1); // FIXME: +1 required due to the first plot used to fix the X axis range ... MUST BE REMOVED WITH VTK 5.8
      plot->SetLabel(label.toUtf8().constData());
    }
  }
  this->render();
};

void AbstractChartWidget::hidePlots(const QList<int>& itemIds)
{
  this->setPlotsVisible(itemIds, false);
};

void AbstractChartWidget::showPlots(const QList<int>& itemIds)
{
  this->setPlotsVisible(itemIds, true);
};

void AbstractChartWidget::toggleOptions(const QPoint& pos)
{
  if (this->mp_ChartOptions->isVisible())
    this->mp_ChartOptions->setVisible(false);
  else
  {
    this->mp_ChartOptions->move(pos - QPoint(this->mp_ChartOptions->width() / 2, 0));
    this->mp_ChartOptions->setVisible(true);
  }
};

void AbstractChartWidget::dragEnterEvent(QDragEnterEvent* event)
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
        if (!this->acceptDroppedTreeWidgetItem(*it))
          return;
      }
      event->setDropAction(Qt::CopyAction); // To have the cross (+) symbol
      event->accept();
    }
  }
};

void AbstractChartWidget::dropEvent(QDropEvent* event)
{
  event->setDropAction(Qt::IgnoreAction); // Only to know which Analog IDs were dropped.
  event->accept();
  QTreeWidget* treeWidget = qobject_cast<QTreeWidget*>(event->source());
  QList<QTreeWidgetItem*> selectedItems = treeWidget->selectedItems();
  bool plotAdded = false;
  for (QList<QTreeWidgetItem*>::const_iterator it = selectedItems.begin() ; it != selectedItems.end() ; ++it)
  {
    QString legend;
    double color[3] = {1.0, 1.0, 1.0};
    double width = 1.0;
    int itemId = -1;
    if (this->appendPlotFromDroppedItem(*it, &itemId, legend, color, &width))
    {
      int c[3] = {static_cast<int>(color[0]*255.0), static_cast<int>(color[1]*255.0), static_cast<int>(color[2]*255.0)};
      this->mp_ChartOptions->appendPlot(itemId, legend, c, width);
      plotAdded = true;
    }
  }
  // FIXME: SHOULD BE ONLY FOR VTK 5.6 AS AUTOAXES SEEMS TO NO WORK
  if (plotAdded)
  {
    for (size_t i = 0 ; i < this->mp_VTKCharts->size() ; ++i)
      this->mp_VTKCharts->operator[](i)->GetPlot(0)->SetVisible(false);
  }
  this->render();
};

void AbstractChartWidget::paintEvent(QPaintEvent* event)
{
  Q_UNUSED(event)
  //this->QVTKWidget::paintEvent(event);
  // Fix as the interactor used by QVTKWidget does not update the rendering
  // when added in a layout (or the problem is elsewhere in the code of Mokka...)
  //this->render();
};

void AbstractChartWidget::resizeEvent(QResizeEvent* event)
{
  // FIXME: When a chart is splitted, the sender does not update the spacing between ticks nor
  // its number (The split set the sender as not visible). WAIT FOR VTK 5.8 TO FIX THIS.
  
  // Crash when an acquisition is loaded and switching for the first time to a graph
  // Not happens when the view is switched to a graph before the loading of an acquisition...
#if 0
  if (event->oldSize().width() <= 0 || event->oldSize().height() <= 0)
      return;
#else
  Q_UNUSED(event)
  if (!this->isVisible())
    return;
#endif
  for (size_t i = 0 ; i < this->mp_VTKCharts->size() ; ++i)
  {
    this->mp_VTKCharts->operator[](i)->GetAxis(vtkAxis::BOTTOM)->RecalculateTickSpacing(); // X axis
    this->mp_VTKCharts->operator[](i)->GetAxis(vtkAxis::LEFT)->RecalculateTickSpacing(); // Y axis
  }
};

bool AbstractChartWidget::isAlreadyPlotted(int id)
{
  for (int i = 0 ; i < this->mp_ChartOptions->plotTable->rowCount() ; ++i)
  {
    if (this->mp_ChartOptions->plotTable->item(i, 0)->data(ChartOptionsWidget::ItemId).toInt() == id)
      return true;
  }
  return false;
};

void AbstractChartWidget::setPlotsVisible(const QList<int>& itemIds, bool show)
{
  for (int i = 0 ; i < itemIds.count() ; ++i)
  {
    for (int j = 0 ; j < this->mp_ChartOptions->plotTable->rowCount() ; ++j)
    {
      QTableWidgetItem* item = this->mp_ChartOptions->plotTable->item(j, 0);
      if (item->data(ChartOptionsWidget::ItemId).toInt() == itemIds[i])
      {
        if (show)
          this->mp_ChartOptions->plotTable->showRow(j);
        else
          this->mp_ChartOptions->plotTable->hideRow(j);
        for (size_t k = 0 ; k < this->mp_VTKCharts->size() ; ++k)
          this->mp_VTKCharts->operator[](k)->GetPlot(j + 1)->SetVisible(show); // FIXME: +1 required due to the first plot used to fix the X axis range ... MUST BE REMOVED WITH VTK 5.8
      }
    }
  }
  this->fixAxesVisibility(); // Show the first plot if the others were removed or hidden
  this->render();
};

void AbstractChartWidget::fixAxesVisibility()
{
  bool plotVisible = false;
  for (int i = 0 ; i < this->mp_ChartOptions->plotTable->rowCount() ; ++i)
  {
    if (!this->mp_ChartOptions->plotTable->isRowHidden(i))
    {
      plotVisible = true;
      break;
    }
  }
  for (size_t i = 0 ; i < this->mp_VTKCharts->size() ; ++i)
  {
    vtkChartXY* chart = this->mp_VTKCharts->operator[](i);
    if (plotVisible == chart->GetPlot(0)->GetVisible())
    {
      chart->GetPlot(0)->SetVisible(!plotVisible);
      chart->RecalculateBounds();
    }
  }
};
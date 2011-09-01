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
#include "ChartExportDialog.h"

#include <vtkContextScene.h>
#include <vtkContextActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkInteractorStyle.h>
#include <vtkColorSeries.h>
#include <vtkPen.h>
#include <QVTKWidget.h>

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QTreeWidget>
#include <QToolTip>

class ChartViewWidget : public QVTKWidget
{
public:
  ChartViewWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);
  void setChart(btk::VTKChartTimeSeries* chart) {this->mp_Chart = chart;};
protected:
  virtual void contextMenuEvent(QContextMenuEvent* event);
  virtual bool event(QEvent* event);
  virtual void keyPressEvent(QKeyEvent* event);
  virtual void keyReleaseEvent(QKeyEvent* event);
  virtual void mousePressEvent(QMouseEvent* event);
  virtual void mouseReleaseEvent(QMouseEvent* event);
  virtual void mouseMoveEvent(QMouseEvent* event);
  virtual void wheelEvent(QWheelEvent* event);
private:
  btk::VTKChartTimeSeries* mp_Chart;
};

AbstractChartWidget::AbstractChartWidget(int numCharts, QWidget* parent)
: QWidget(parent), m_ViewActions()
{
  QPalette p(this->palette());
  p.setColor(QPalette::Window, Qt::white);
  this->setPalette(p);
  this->setAutoFillBackground(true);
  
  this->mp_Acquisition = 0;
  this->mp_VTKCharts = new VTKCharts;
  this->mp_VTKCharts->resize(numCharts);
  this->mp_ArrayFrames = 0;
  this->mp_ColorGenerator = vtkColorSeries::New();
  this->mp_ColorGenerator->RemoveColor(0); // The first one is the color black.
  this->mp_ChartOptions = new ChartOptionsWidget(this);
  this->mp_ChartOptions->setVisible(false);
  this->mp_ChartOptions->clear();
  
  this->setFocusPolicy(Qt::StrongFocus);
  
  QAction* resetZoomAction = new QAction(tr("Reset Zoom"), this); this->m_ViewActions.push_back(resetZoomAction);
  QAction* exportToAction = new QAction(tr("Export to Image"), this); this->m_ViewActions.push_back(exportToAction);
  QAction* separator = new QAction(this); separator->setSeparator(true); this->m_ViewActions.push_back(separator);
  QAction* toggleEventDisplayAction = new QAction(tr("Toggle Events Display"), this); this->m_ViewActions.push_back(toggleEventDisplayAction);
  QAction* removeAllPlotAction = new QAction(tr("Clear Chart"), this); this->m_ViewActions.push_back(removeAllPlotAction);
  
  QVBoxLayout* layout = new QVBoxLayout(this);
  for (int i = 0 ; i < numCharts ; ++i)
  {
    ChartViewWidget* w = new ChartViewWidget(this);
    w->addActions(this->m_ViewActions);
    w->setContextMenuPolicy(Qt::ActionsContextMenu);
    layout->addWidget(w);
    // No need to send mouse events to VTK when a mouse button isn't down
    w->setMouseTracking(false);
    // w->GetRenderWindow()->SwapBuffersOff();
    // w->GetRenderWindow()->DoubleBufferOff();
    // w->GetRenderWindow()->SetMultiSamples(0);
  }
  layout->setContentsMargins(0,5,0,0);
  layout->setSpacing(0);
  
  connect(this->mp_ChartOptions, SIGNAL(lineColorChanged(QList<int>, QColor)), this, SLOT(setPlotLineColor(QList<int>, QColor)));
  connect(this->mp_ChartOptions, SIGNAL(lineWidthChanged(QList<int>, double)), this, SLOT(setPlotLineWidth(QList<int>, double)));
  connect(this->mp_ChartOptions, SIGNAL(plotRemoved(int)), this, SLOT(removePlot(int)));
  connect(resetZoomAction, SIGNAL(triggered()), this, SLOT(resetZoom()));
  connect(toggleEventDisplayAction, SIGNAL(triggered()), this, SLOT(toggleEventDisplay()));
  connect(removeAllPlotAction, SIGNAL(triggered()), this, SLOT(removeAllPlot()));
  connect(exportToAction, SIGNAL(triggered()), this, SLOT(exportToImage()));
  
  this->setAcceptDrops(true);
}

AbstractChartWidget::~AbstractChartWidget()
{
  for (size_t i = 0 ; i < this->mp_VTKCharts->size() ; ++i)
  {
    btk::VTKChartTimeSeries* chart = this->mp_VTKCharts->operator[](i);
    if (chart != NULL)
      chart->Delete();
  }
  delete this->mp_VTKCharts;
  if (this->mp_ArrayFrames != NULL)
    this->mp_ArrayFrames->Delete();
  this->mp_ColorGenerator->Delete();
};

void AbstractChartWidget::initialize()
{
  if (this->mp_VTKCharts->size() == 0)
    return;
  
  for (size_t i = 0 ; i < this->mp_VTKCharts->size() ; ++i)
  {
    vtkRenderer* ren = vtkRenderer::New();
    ren->SetBackground(1.0,1.0,1.0);
    ChartViewWidget* w = static_cast<ChartViewWidget*>(this->layout()->itemAt((int)i)->widget());
    vtkRenderWindow* renwin = w->GetRenderWindow();
    renwin->AddRenderer(ren);
    
    btk::VTKChartTimeSeries* chart = btk::VTKChartTimeSeries::New(); // Do not delete
    chart->SetBorders(60, 45, 20, 5);
    chart->GetAxis(vtkAxis::BOTTOM)->SetTitle("Frames"); // X axis
    chart->SetBoundsEnabled(true);
    chart->DisplayEventsOn();
    chart->SetColorSeries(this->mp_ColorGenerator);
    this->mp_VTKCharts->operator[](i) = chart;
    w->setChart(chart);
    
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
    btk::VTKChartTimeSeries* chart = this->mp_VTKCharts->operator[](i);
    chart->ClearPlots();
    this->mp_ChartOptions->clear();
  }
  
  // Enable / disable the actions
  for (int i = 0 ; i < this->m_ViewActions.size() ; ++i)
    this->m_ViewActions[i]->setEnabled(source->m_ViewActions[i]->isEnabled());
  
  // Copy the acquisition pointer
  this->setAcquisition(source->acquisition());
  // Copy the functors;
  this->setCurrentFrameFunctor(source->currentFrameFunctor());
  this->setRegionOfInterestFunctor(source->regionOfInterestFunctor());
  this->setEventsFunctor(source->eventsFunctor());
  
  // Copy the X axis
  this->setFrameArray(source->frameArray());
#if 1
  for (size_t i = 0 ; i < this->mp_VTKCharts->size() ; ++i)
  {
    btk::VTKChartTimeSeries* sourceChart = source->mp_VTKCharts->operator[](i);
    btk::VTKChartTimeSeries* targetChart = this->mp_VTKCharts->operator[](i);
    targetChart->SetBounds(sourceChart->GetBounds()[0], sourceChart->GetBounds()[1], 0.0, 0.0);
  }
#else
  // Copy the plots
  for (size_t i = 0 ; i < this->mp_VTKCharts->size() ; ++i)
  {
    btk::VTKChartTimeSeries* sourceChart = source->mp_VTKCharts->operator[](i);
    btk::VTKChartTimeSeries* targetChart = this->mp_VTKCharts->operator[](i);
    for (int j = 0 ; j < sourceChart->GetNumberOfPlots() ; ++j)
    {
      vtkPlot* targetLine = targetChart->AddPlot(vtkChart::LINE);
      vtkPlot* sourceLine = sourceChart->GetPlot(j);
      targetLine->SetInput(sourceLine->GetInput(), 0, 1);
      targetLine->SetWidth(sourceLine->GetWidth());
      double color[3]; sourceLine->GetColor(color); targetLine->SetColor(color[0], color[1], color[2]);
    }
    targetChart->SetBounds(sourceChart->GetBounds()[0], sourceChart->GetBounds()[1], 0.0, 0.0);
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
  if (!s)
    this->mp_ChartOptions->clear();
  for (QList<QAction*>::iterator it = this->m_ViewActions.begin() ; it != this->m_ViewActions.end() ; ++it)
    (*it)->setEnabled(s);
  
  for (size_t i = 0 ; i < this->mp_VTKCharts->size() ; ++i)
  {
    btk::VTKChartTimeSeries* chart = this->mp_VTKCharts->operator[](i);
    chart->SetInteractionEnabled(false);
    chart->ClearPlots();
  
    if (!s) // Reset
    {
      chart->SetBounds(0.0, 0.0, 0.0, 0.0);
      chart->GetAxis(vtkAxis::BOTTOM)->SetLabelsVisible(false);
      chart->GetAxis(vtkAxis::LEFT)->SetLabelsVisible(false);
    }
    else // Load
    {
      chart->SetBounds((double)this->mp_Acquisition->firstFrame(), (double)this->mp_Acquisition->lastFrame(), 0.0, 0.0);
      chart->GetAxis(vtkAxis::BOTTOM)->SetLabelsVisible(true);
      chart->GetAxis(vtkAxis::LEFT)->SetLabelsVisible(true);
    }
  }
};

void AbstractChartWidget::removePlot(int index)
{
  for (size_t i = 0 ; i < this->mp_VTKCharts->size() ; ++i)
    this->mp_VTKCharts->operator[](i)->RemovePlot(index);
  this->checkResetAxes(); // If no more plot or all of them are hidden, then the axes are reset.
#ifdef Q_OS_WIN
  // Fix for Windows XP (and vista?) which doesn't redraw correctly the options.
  // The side effect is a possible blinking of the options but it's better than to see nothing.
  if (QSysInfo::windowsVersion() < QSysInfo::WV_WINDOWS7)
  {
    this->mp_ChartOptions->hide();
    QApplication::processEvents();
    this->render();
    this->mp_ChartOptions->show();
  }
  else
    this->render();
#else
  this->render();
#endif
};

void AbstractChartWidget::render()
{
  for (size_t i = 0 ; i < this->mp_VTKCharts->size() ; ++i)
  {
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
      vtkPlot* plot = this->mp_VTKCharts->operator[](i)->GetPlot(*it);
      plot->SetColor(color.redF(), color.greenF(), color.blueF());
    }
  }
#ifdef Q_OS_WIN
  // Fix for Windows XP (and vista?) which doesn't redraw correctly the options.
  // The side effect is a possible blinking of the options but it's better than to see nothing.
  if (QSysInfo::windowsVersion() < QSysInfo::WV_WINDOWS7)
  {
    this->mp_ChartOptions->hide();
    QApplication::processEvents();
    this->render();
    this->mp_ChartOptions->show();
  }
  else
    this->render();
#else
  this->render();
#endif
};

void AbstractChartWidget::setPlotLineWidth(const QList<int>& indices, double value)
{
  for (QList<int>::const_iterator it = indices.begin() ; it != indices.end() ; ++it)
  {
    for (size_t i = 0 ; i < this->mp_VTKCharts->size() ; ++i)
    {
      vtkPlot* plot = this->mp_VTKCharts->operator[](i)->GetPlot(*it);
      plot->SetWidth(static_cast<float>(value));
    }
  }
#ifdef Q_OS_WIN
  // Fix for Windows XP (and vista?) which doesn't redraw correctly the options.
  // The side effect is a possible blinking of the options but it's better than to see nothing.
  if (QSysInfo::windowsVersion() < QSysInfo::WV_WINDOWS7)
  {
    this->mp_ChartOptions->hide();
    QApplication::processEvents();
    this->render();
    this->mp_ChartOptions->show();
  }
  else
    this->render();
#else
  this->render();
#endif
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
      vtkPlot* plot = this->mp_VTKCharts->operator[](i)->GetPlot(plotIdx);
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

void AbstractChartWidget::resetZoom()
{
  QVTKWidget* w = qobject_cast<QVTKWidget*>(this->childAt(this->mapFromGlobal(QCursor::pos())));
  int idx = -1;
  if ((w != 0) && ((idx = this->layout()->indexOf(w)) != -1))
  {
    this->mp_VTKCharts->operator[](idx)->ResetZoom();
  }
  this->render();
};

void AbstractChartWidget::exportToImage()
{
  QVTKWidget* w = qobject_cast<QVTKWidget*>(this->childAt(this->mapFromGlobal(QCursor::pos())));
  int idx = -1;
  if ((w != 0) && ((idx = this->layout()->indexOf(w)) != -1))
  {
    ChartExportDialog exportDlg(this);
    exportDlg.setChart(this->mp_VTKCharts->operator[](idx));
    exportDlg.exec();
  }
};

void AbstractChartWidget::removeAllPlot()
{
  this->mp_ChartOptions->clear();
  this->show(true); // Easy way to reset the chart.
  this->render();
};

void AbstractChartWidget::toggleEventDisplay()
{
  for (size_t i = 0 ; i < this->mp_VTKCharts->size() ; ++i)
  {
    btk::VTKChartTimeSeries* chart = this->mp_VTKCharts->operator[](i);
    chart->SetDisplayEvents(chart->GetDisplayEvents() == 1 ? 0 : 1);
  }
  this->render();
};

btk::VTKCurrentFrameFunctor::Pointer AbstractChartWidget::currentFrameFunctor() const
{
  btk::VTKCurrentFrameFunctor::Pointer functor;
  if (!this->mp_VTKCharts->empty())
    functor = this->mp_VTKCharts->operator[](0)->GetCurrentFrameFunctor();
  return functor;
};

void AbstractChartWidget::setCurrentFrameFunctor(btk::VTKCurrentFrameFunctor::Pointer functor)
{
  for (size_t i = 0 ; i < this->mp_VTKCharts->size() ; ++i)
    this->mp_VTKCharts->operator[](i)->SetCurrentFrameFunctor(functor);
};

btk::VTKRegionOfInterestFunctor::Pointer AbstractChartWidget::regionOfInterestFunctor() const
{
  btk::VTKRegionOfInterestFunctor::Pointer functor;
  if (!this->mp_VTKCharts->empty())
    functor = this->mp_VTKCharts->operator[](0)->GetRegionOfInterestFunctor();
  return functor;
};

void AbstractChartWidget::setRegionOfInterestFunctor(btk::VTKRegionOfInterestFunctor::Pointer functor)
{
  for (size_t i = 0 ; i < this->mp_VTKCharts->size() ; ++i)
    this->mp_VTKCharts->operator[](i)->SetRegionOfInterestFunctor(functor);
}

btk::VTKEventsFunctor::Pointer AbstractChartWidget::eventsFunctor() const
{
  btk::VTKEventsFunctor::Pointer functor;
  if (!this->mp_VTKCharts->empty())
    functor = this->mp_VTKCharts->operator[](0)->GetEventsFunctor();
  return functor;
};

void AbstractChartWidget::setEventsFunctor(btk::VTKEventsFunctor::Pointer functor)
{
  for (size_t i = 0 ; i < this->mp_VTKCharts->size() ; ++i)
    this->mp_VTKCharts->operator[](i)->SetEventsFunctor(functor);
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
  event->setDropAction(Qt::IgnoreAction); // Only to know which Analog|Marker IDs were dropped.
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
  if (plotAdded)
  {
    for (size_t i = 0 ; i < this->mp_VTKCharts->size() ; ++i)
    {
      btk::VTKChartTimeSeries* chart = this->mp_VTKCharts->operator[](i);
      chart->SetInteractionEnabled(true);
      chart->RecalculateBounds();
    }
  }
  this->render();
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
  QList<QTableWidgetItem*> selectedItems = this->mp_ChartOptions->plotTable->selectedItems();
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
        {
          this->mp_ChartOptions->plotTable->hideRow(j);
          if (selectedItems.contains(item))
            this->mp_ChartOptions->plotTable->clearSelection();
        }
        for (size_t k = 0 ; k < this->mp_VTKCharts->size() ; ++k)
          this->mp_VTKCharts->operator[](k)->GetPlot(j)->SetVisible(show);
      }
    }
  }
  this->checkResetAxes();
  this->render();
};

void AbstractChartWidget::checkResetAxes()
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
    btk::VTKChartTimeSeries* chart = this->mp_VTKCharts->operator[](i);
    chart->SetInteractionEnabled(plotVisible);
    // Force the chart to draw ticks for the X axis.
    if (!plotVisible)
      chart->SetBounds((double)this->mp_Acquisition->firstFrame(), (double)this->mp_Acquisition->lastFrame(), 0.0, 0.0);
  }
};

void AbstractChartWidget::GenerateColor(double color[3])
{
  btk::VTKChartTimeSeries* chart = this->mp_VTKCharts->operator[](0);
  int numPlots = chart->GetNumberOfPlots();
  int colorIndex = numPlots;
  vtkColor3ub c = this->mp_ColorGenerator->GetColorRepeating(colorIndex);
  int inc = 0;
  while (inc < numPlots)
  {
    unsigned char rgb[3];
    chart->GetPlot(inc)->GetPen()->GetColor(rgb);
    if ((c.Red() == rgb[0]) && (c.Green() == rgb[1]) && (c.Blue() == rgb[2]))
    {
      c = this->mp_ColorGenerator->GetColorRepeating(++colorIndex);
      inc = 0;
    }
    ++inc;
  }
  color[0] = c[0] / 255.0;
  color[1] = c[1] / 255.0;
  color[2] = c[2] / 255.0;
};

// -----------------------------------------------------------------------------

ChartViewWidget::ChartViewWidget(QWidget* parent, Qt::WindowFlags f)
: QVTKWidget(parent, f)
{
  this->mp_Chart = 0;
};

void ChartViewWidget::contextMenuEvent(QContextMenuEvent* event)
{
  // No need to send this event to VTK.
  this->QWidget::contextMenuEvent(event);
};

bool ChartViewWidget::event(QEvent* event)
{
  if (event->type() == QEvent::ToolTip)
  {
    QHelpEvent* helpEvent = static_cast<QHelpEvent*>(event);
    
    int pt1[2]; this->mp_Chart->GetPoint1(pt1); // Bottom left
    
    vtkAxis* axisX = this->mp_Chart->GetAxis(vtkAxis::BOTTOM);
    float pt1X[2]; axisX->GetPoint1(pt1X);
    float pt2X[2]; axisX->GetPoint2(pt2X);
    double miniX = axisX->GetMinimum(); double maxiX = axisX->GetMaximum();
    double scaleX = (maxiX - miniX) / (double)(pt2X[0] - pt1X[0]);
    
    vtkAxis* axisY = this->mp_Chart->GetAxis(vtkAxis::LEFT);
    float pt1Y[2]; axisY->GetPoint1(pt1Y);
    float pt2Y[2]; axisY->GetPoint2(pt2Y);
    double miniY = axisY->GetMinimum(); double maxiY = axisY->GetMaximum();
    double scaleY = (maxiY - miniY) / (double)(pt2Y[1] - pt1Y[1]);
    
    QPoint p = helpEvent->pos() - QPoint(pt1[0], -pt1[1]);
    
    vtkVector2f pos((double)p.x() * scaleX + miniX, (double)(this->height()-p.y()-1) * scaleY + miniY), coord, tolerance(5.0*scaleX,5.0*scaleY); // tolerance +/- 5 pixels 

    for (int i = 0 ; i < this->mp_Chart->GetNumberOfPlots() ; ++i)
    {
      vtkPlot* plot = this->mp_Chart->GetPlot(i);
      if (plot->GetVisible() && plot->GetNearestPoint(pos, tolerance, &coord))
      {
        QString str = "Frame: " + QString::number(coord.X()) + "<br/>" + QString::fromUtf8(plot->GetLabel()) + ": " + QString::number(coord.Y(), 'f', 1);
        QToolTip::showText(helpEvent->globalPos(), str);
        return true;
      }
    }
    QToolTip::hideText();
    event->ignore();
    return true;
  }
  return QVTKWidget::event(event);
};

void ChartViewWidget::keyPressEvent(QKeyEvent* event)
{
  // Keyboard events are not sent to VTK.
  this->QWidget::keyPressEvent(event);
};

void ChartViewWidget::keyReleaseEvent(QKeyEvent* event)
{
  // Keyboard events are not sent to VTK.
  this->QWidget::keyReleaseEvent(event);
};

// To not propagate the middle and right click to the charts
void ChartViewWidget::mousePressEvent(QMouseEvent* event)
{
  vtkRenderWindowInteractor* iren = NULL;
  if(this->mRenWin)
    iren = this->mRenWin->GetInteractor();
  if(!iren || !iren->GetEnabled())
    return;
  
  // give vtk event information (without modifiers informations: always set to 0)
  iren->SetEventInformationFlipY(event->x(), event->y(), 0, 0, 0, (event->type() == QEvent::MouseButtonDblClick ? 1 : 0));
  
  // invoke appropriate vtk event only for the left button
  if(event->button() == Qt::LeftButton)
  {
    if (this->mp_Chart)
      this->mp_Chart->SetDisplayZoomBox((event->modifiers() & Qt::ShiftModifier) == Qt::ShiftModifier ? 1 : 0);
    iren->InvokeEvent(vtkCommand::LeftButtonPressEvent, event);
  }
};

// To not propagate the middle and right click to the charts
void ChartViewWidget::mouseReleaseEvent(QMouseEvent* event)
{
  vtkRenderWindowInteractor* iren = NULL;
  if(this->mRenWin)
    iren = this->mRenWin->GetInteractor();
  if(!iren || !iren->GetEnabled())
    return;
    
  // To fix a possible conflict between Qt and VTK when you use a contextual menu event.
  // VTK or the scene doesn't receive the event vtkCommand::LeftButtonReleaseEvent
  // In our case, the zoom box is not applied after using the reset zoom action
  if (this->mp_Chart && (this->mp_Chart->GetDisplayZoomBox() == 1))
  {
    this->mp_Chart->ApplyZoom(this->mp_Chart->GetZoomBox());
    this->mRenWin->Render();
  }
  
  // give vtk event information (without modifiers informations: always set to 0)
  iren->SetEventInformationFlipY(event->x(), event->y(), 0, 0);
  
  // invoke appropriate vtk event only for the left button
  if(event->button() == Qt::LeftButton)
    iren->InvokeEvent(vtkCommand::LeftButtonReleaseEvent, event);
};

// To not propagate the middle and right click to the charts
void ChartViewWidget::mouseMoveEvent(QMouseEvent* event)
{
  vtkRenderWindowInteractor* iren = NULL;
  if(this->mRenWin)
    iren = this->mRenWin->GetInteractor();
  if(!iren || !iren->GetEnabled())
    return;
  
  // give interactor the event information (without modifiers informations: always set to 0)
  iren->SetEventInformationFlipY(event->x(), event->y(), 0, 0);
  
  // invoke vtk event only if the left button is activated
  if ((event->buttons() & Qt::LeftButton) == Qt::LeftButton)
    iren->InvokeEvent(vtkCommand::MouseMoveEvent, event);
};

void ChartViewWidget::wheelEvent(QWheelEvent* event)
{
  vtkRenderWindowInteractor* iren = NULL;
  if(this->mRenWin)
    iren = this->mRenWin->GetInteractor();
  if(!iren || !iren->GetEnabled())
    return;
  
  if (this->mp_Chart)
    this->mp_Chart->SetZoomMode((event->modifiers() & Qt::ShiftModifier) == Qt::ShiftModifier ? btk::VTKChartTimeSeries::HORIZONTAL : btk::VTKChartTimeSeries::BOTH);
  
  // give vtk event information (without modifiers informations: always set to 0)
  iren->SetEventInformationFlipY(event->x(), event->y(), 0, 0);

  // invoke vtk event: if delta is positive, it is a forward wheel event
  if(event->delta() > 0)
    iren->InvokeEvent(vtkCommand::MouseWheelForwardEvent, event);
  else
    iren->InvokeEvent(vtkCommand::MouseWheelBackwardEvent, event);
};
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

#include "ChartWidget.h"
#include "Acquisition.h"
#include "ChartOptionsWidget.h"
#include "ChartExportDialog.h"
#include "UserRoles.h"

#include <btkVTKContextScene.h>

#include <vtkContextActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkInteractorStyle.h>
#include <vtkColorSeries.h>
#include <vtkPen.h>
#include <vtkTransform2D.h>
#include <vtkPlotLine.h>
#include <vtkTable.h>
#include <vtkAxis.h>

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QTreeWidget>
#include <QToolTip>

#ifdef Q_OS_WIN
  #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
  #endif
  #include <windows.h>
#endif

class PointChartData : public AbstractChartData
{
public:
  PointChartData();
  virtual bool acceptDroppedTreeWidgetItem(QTreeWidgetItem* item);
  virtual bool appendPlotFromDroppedItem(Acquisition* acq, vtkColorSeries* colorGenerator, QTreeWidgetItem* item, bool* layoutModified);
  virtual QString createPlotLabel(Acquisition* acq, int id);
  virtual void initialize(vtkColorSeries* colorGenerator);
};

class AnalogChartData : public AbstractChartData
{
public:
  AnalogChartData();
  virtual bool acceptDroppedTreeWidgetItem(QTreeWidgetItem* item);
  virtual bool appendPlotFromDroppedItem(Acquisition* acq, vtkColorSeries* colorGenerator, QTreeWidgetItem* item, bool* layoutModified);
  virtual QString createPlotLabel(Acquisition* acq, int id);
  virtual void initialize(vtkColorSeries* colorGenerator);
  virtual void removePlot(int index, bool* layoutModified);
  virtual void setPlotVisible(int index, bool show, bool* layoutModified);
  virtual void show(Acquisition* acq, bool s, bool* layoutModified);
  void setExpandable(bool expandable);

protected:
  btk::VTKChartTimeSeries* createChart(btk::VTKChartTimeSeries* sourceChart);
  
  bool m_Expanded;
};

ChartWidget::ChartWidget(QWidget* parent)
: QWidget(parent), m_ChartData(2, NULL), m_ViewActions(), m_LastContextMenuPosition()
{
  QPalette p(this->palette());
  p.setColor(QPalette::Window, Qt::white);
  this->setPalette(p);
  this->setAutoFillBackground(true);
  
  this->m_CurrentChartType = -1; // No chart type defined
  this->mp_Acquisition = 0;
  
  this->mp_Scene = btk::VTKContextScene::New();
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
  
  this->mp_ChartContentWidget = new VTKChartWidget(this);
  this->mp_ChartContentWidget->addActions(this->m_ViewActions);
  this->mp_ChartContentWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
  this->mp_ChartContentWidget->setMouseTracking(false); // No need to send mouse events to VTK when a mouse button isn't down
  // this->mp_ChartContentWidget->GetRenderWindow()->SwapBuffersOff();
  // this->mp_ChartContentWidget->GetRenderWindow()->DoubleBufferOff();
  // this->mp_ChartContentWidget->GetRenderWindow()->SetMultiSamples(0);
  connect(this->mp_ChartContentWidget, SIGNAL(contextMenuRequested(QPoint)), this, SLOT(setLastContextMenuPosition(QPoint)));
  
  
#ifdef Q_OS_WIN
  QFont f("Arial", 9);
#else
  QFont f("Arial", 12);
#endif
  f.setStyleStrategy(QFont::PreferAntialias);
  f.setBold(true);

  this->mp_ChartTitleLabel = new QLabel(this);
  this->mp_ChartTitleLabel->setFont(f);
  this->mp_ChartTitleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
  this->mp_ChartTitleLabel->setVisible(false);

  this->mp_ChartAxisXLabel = new QLabel(tr("Frames"), this);
  this->mp_ChartAxisXLabel->setFont(f);
  this->mp_ChartAxisXLabel->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
  this->mp_ChartAxisXLabel->setVisible(true);

  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->addWidget(this->mp_ChartTitleLabel);
  layout->addWidget(this->mp_ChartContentWidget);
  layout->addWidget(this->mp_ChartAxisXLabel);
  layout->setContentsMargins(0,5,0,5);
  layout->setSpacing(0);
  
  connect(this->mp_ChartOptions, SIGNAL(lineColorChanged(QList<int>, QColor)), this, SLOT(setPlotLineColor(QList<int>, QColor)));
  connect(this->mp_ChartOptions, SIGNAL(lineWidthChanged(QList<int>, double)), this, SLOT(setPlotLineWidth(QList<int>, double)));
  connect(this->mp_ChartOptions, SIGNAL(plotRemoved(int)), this, SLOT(removePlot(int)));
  connect(this->mp_ChartOptions, SIGNAL(chartTitleChanged(QString)), this, SLOT(setChartTitle(QString)));
  connect(resetZoomAction, SIGNAL(triggered()), this, SLOT(resetZoom()));
  connect(toggleEventDisplayAction, SIGNAL(triggered()), this, SLOT(toggleEventDisplay()));
  connect(removeAllPlotAction, SIGNAL(triggered()), this, SLOT(removeAllPlot()));
  connect(exportToAction, SIGNAL(triggered()), this, SLOT(exportToImage()));
  
  this->setAcceptDrops(true);
}

ChartWidget::~ChartWidget()
{
  for (int i = 0 ; i < this->m_ChartData.size() ; ++i)
  {
    if (this->m_ChartData[i] != NULL)
      delete this->m_ChartData[i];
  }
  this->mp_Scene->Delete();
  this->mp_ColorGenerator->Delete();
};

void ChartWidget::initialize()
{
  this->m_ChartData[PointChart] = new PointChartData();
  this->m_ChartData[AnalogChart] = new AnalogChartData();
  
  vtkRenderer* ren = vtkRenderer::New();
  ren->SetBackground(1.0,1.0,1.0);
  vtkRenderWindow* renwin = this->mp_ChartContentWidget->GetRenderWindow();
  renwin->AddRenderer(ren);
  
  for (int i = 0 ; i < this->m_ChartData.size() ; ++i)
    this->m_ChartData[i]->initialize(this->mp_ColorGenerator);
  
  vtkContextActor* actor = vtkContextActor::New();
  actor->SetScene(this->mp_Scene);
  ren->AddActor(actor);
  this->mp_Scene->SetRenderer(ren);
  this->mp_Scene->SetInteractorStyle(vtkInteractorStyle::SafeDownCast(renwin->GetInteractor()->GetInteractorStyle()));
  
  actor->Delete();
  ren->Delete();
  
  this->show(false); // Reset the charts
};

void ChartWidget::copy(ChartWidget* source)
{
  // Clean if necessary
  for (int i = 0 ; i < this->m_ChartData.size() ; ++i)
    this->m_ChartData[i]->clear();
  this->mp_ChartOptions->clear();
  
  // Enable / disable the actions
  for (int i = 0 ; i < this->m_ViewActions.size() ; ++i)
    this->m_ViewActions[i]->setEnabled(source->m_ViewActions[i]->isEnabled());
  
  // Copy the data
  for (int i = 0 ; i < this->m_ChartData.size() ; ++i)
    this->m_ChartData[i]->copy(source->m_ChartData[i]);
  
  // Copy the acquisition pointer
  this->setAcquisition(source->acquisition());
  
  // Copy the current chart (to activate the specific options)
  this->m_CurrentChartType = source->m_CurrentChartType;
};

void ChartWidget::setAcquisition(Acquisition* acq)
{
  if (this->mp_Acquisition == acq)
    return;
  if (this->mp_Acquisition != 0)
    disconnect(this->mp_Acquisition, 0, this, 0);
  
  this->mp_Acquisition = acq;
  // Point
  connect(this->mp_Acquisition, SIGNAL(pointLabelChanged(int, QString)), this, SLOT(updatePointPlotLabel(int)));
  connect(this->mp_Acquisition, SIGNAL(pointsRemoved(QList<int>, QList<Point*>)), this, SLOT(hidePointPlots(QList<int>)));
  connect(this->mp_Acquisition, SIGNAL(pointsInserted(QList<int>, QList<Point*>)), this, SLOT(showPointPlots(QList<int>)));
  // Analog
  connect(this->mp_Acquisition, SIGNAL(analogLabelChanged(int, QString)), this, SLOT(updateAnalogPlotLabel(int)));
  connect(this->mp_Acquisition, SIGNAL(analogsUnitChanged(QVector<int>, QVector<QString>)), this, SLOT(updateAnalogPlotsLabel(QVector<int>)));
  connect(this->mp_Acquisition, SIGNAL(analogsRemoved(QList<int>, QList<Analog*>)), this, SLOT(hideAnalogPlots(QList<int>)));
  connect(this->mp_Acquisition, SIGNAL(analogsInserted(QList<int>, QList<Analog*>)), this, SLOT(showAnalogPlots(QList<int>)));
};

void ChartWidget::show(bool s)
{
  if (!s)
    this->mp_ChartOptions->clear();
  for (QList<QAction*>::iterator it = this->m_ViewActions.begin() ; it != this->m_ViewActions.end() ; ++it)
    (*it)->setEnabled(s);
  for (int i = 0 ; i < this->m_ChartData.size() ; ++i)
  {
    bool layoutModified = false;
    this->m_ChartData[i]->show(this->mp_Acquisition, s, &layoutModified);
    if (layoutModified && (this->m_CurrentChartType == i))
      this->mp_ChartContentWidget->resizeCharts();
  }
};

void ChartWidget::displayChart(int chartType)
{
  // Save the current selection in the options
  if (this->m_CurrentChartType != -1)
    this->m_ChartData[this->m_CurrentChartType]->setOptionSelection(this->mp_ChartOptions->selectedPlots());
  
  this->mp_Scene->Clear();
  for (size_t i = 0 ; i < this->m_ChartData[chartType]->charts()->size() ; ++i)
    this->mp_Scene->AddItem(this->m_ChartData[chartType]->chart(i));
  this->mp_ChartContentWidget->setCharts(this->m_ChartData[chartType]->charts());
  this->mp_ChartContentWidget->resizeCharts();
  this->m_CurrentChartType = chartType;
  
  this->setChartTitle(this->m_ChartData[chartType]->title());
  this->mp_ChartOptions->plotTable->clearSelection();
  this->mp_ChartOptions->chartTitleLineEdit->setText(this->m_ChartData[chartType]->title());
  this->updateOptions();
  this->mp_ChartOptions->setSelectedPlots(this->m_ChartData[chartType]->optionSelection());
  this->mp_ChartOptions->setFocus(); // Remove the focus of its children.
};

void ChartWidget::render(bool optionsShown)
{
  if (optionsShown)
  {
#ifdef Q_OS_WIN
    // Fix for Windows XP (and vista?) which doesn't redraw correctly the options.
    // The side effect is a possible blinking of the options but it's better than to see nothing.
    if (QSysInfo::windowsVersion() < QSysInfo::WV_WINDOWS7)
    {
      this->mp_ChartOptions->setFocus();
      QApplication::processEvents(); // For the text placeholder
      this->mp_ChartOptions->hide();
      this->render();
      ::Sleep(20); // 20 ms
      QApplication::processEvents();
      this->mp_ChartOptions->show();
    }
    else
      this->render();
#else
    this->render();
#endif
  }
  else if (this->mp_ChartContentWidget->isVisible())
    this->mp_ChartContentWidget->GetRenderWindow()->Render();
};

void ChartWidget::removePlot(int index)
{
  bool layoutModified;
  this->m_ChartData[this->m_CurrentChartType]->removePlot(index, &layoutModified);
  if (layoutModified)
    this->mp_ChartContentWidget->resizeCharts();
  this->checkResetAxes(); // If no more plot or all of them are hidden, then the axes are reset.
  this->render(true); // Options are shown
};

void ChartWidget::setPlotLineColor(const QList<int>& indices, const QColor& color)
{
  for (QList<int>::const_iterator it = indices.begin() ; it != indices.end() ; ++it)
  {
    for (size_t i = 0 ; i < this->m_ChartData[this->m_CurrentChartType]->charts()->size() ; ++i)
    {
      vtkPlot* plot = this->m_ChartData[this->m_CurrentChartType]->chart(i)->GetPlot(*it);
      plot->SetColor(color.redF(), color.greenF(), color.blueF());
    }
    this->m_ChartData[this->m_CurrentChartType]->plotsProperties()[*it].color = color;
  }
  this->render(true); // Options are shown
};

void ChartWidget::setPlotLineWidth(const QList<int>& indices, double value)
{
  for (QList<int>::const_iterator it = indices.begin() ; it != indices.end() ; ++it)
  {
    for (size_t i = 0 ; i < this->m_ChartData[this->m_CurrentChartType]->charts()->size() ; ++i)
    {
      vtkPlot* plot = this->m_ChartData[this->m_CurrentChartType]->chart(i)->GetPlot(*it);
      plot->SetWidth(static_cast<float>(value));
    }
    this->m_ChartData[this->m_CurrentChartType]->plotsProperties()[*it].lineWidth = value;
  }
  this->render(true); // Options are shown
};

void ChartWidget::setChartTitle(const QString& title)
{
  if (this->mp_ChartTitleLabel->text().compare(title) == 0)
      return;
 
  this->m_ChartData[this->m_CurrentChartType]->setTitle(title);
  this->mp_ChartTitleLabel->setText(title);
  if (title.isEmpty())
    this->mp_ChartTitleLabel->setVisible(false);
  else
    this->mp_ChartTitleLabel->setVisible(true);
#ifdef Q_OS_WIN
  // Fix for Windows XP (and vista?) which doesn't redraw correctly the options.
  // The side effect is a possible blinking of the options but it's better than to see nothing.
  if (QSysInfo::windowsVersion() < QSysInfo::WV_WINDOWS7)
  {
    QApplication::processEvents(); // For the text placeholder
    this->mp_ChartOptions->hide();
    this->update();
    QApplication::processEvents();
    ::Sleep(20);
    this->mp_ChartOptions->show();
  }
  else
    this->update();
#else
  this->update();
#endif
};

void ChartWidget::resetZoom()
{
  btk::VTKChartTimeSeries* chart = this->mp_ChartContentWidget->focusedChart(this->m_LastContextMenuPosition);
  if (chart != 0)
  {
    chart->ResetZoom();
    this->render();
  }
};

void ChartWidget::exportToImage()
{
  btk::VTKChartTimeSeries* chart = this->mp_ChartContentWidget->focusedChart(this->m_LastContextMenuPosition);
  if (chart != 0)
  {
    ChartExportDialog exportDlg(this);
    exportDlg.setChart(chart);
    exportDlg.exec();
  }
};

void ChartWidget::removeAllPlot()
{
  this->mp_ChartOptions->clear();
  this->show(true); // Easy way to reset the chart.
  this->render();
};

void ChartWidget::toggleEventDisplay()
{
  for (size_t i = 0 ; i < this->m_ChartData[this->m_CurrentChartType]->charts()->size() ; ++i)
  {
    btk::VTKChartTimeSeries* chart = this->m_ChartData[this->m_CurrentChartType]->chart(i);
    chart->SetDisplayEvents(chart->GetDisplayEvents() == 1 ? 0 : 1);
  }
  this->render();
};

void ChartWidget::updatePointPlotLabel(int itemId)
{
  this->updatePlotLabel(PointChart, itemId);
};

void ChartWidget::hidePointPlots(const QList<int>& itemIds)
{
  this->setPlotsVisible(PointChart, itemIds, false);
};

void ChartWidget::showPointPlots(const QList<int>& itemIds)
{
  this->setPlotsVisible(PointChart, itemIds, true);
};

void ChartWidget::displayPointComponentX(int state)
{
  this->displayPointComponent(0, state);
};

void ChartWidget::displayPointComponentY(int state)
{
  this->displayPointComponent(1, state);
};

void ChartWidget::displayPointComponentZ(int state)
{
  this->displayPointComponent(2, state);
};

void ChartWidget::updateAnalogPlotLabel(int itemId)
{
  this->updatePlotLabel(AnalogChart, itemId);
};

void ChartWidget::updateAnalogPlotsLabel(const QVector<int>& itemIds)
{
  for (int i = 0 ; i < itemIds.count() ; ++i)
    this->updateAnalogPlotLabel(itemIds[i]);
};

void ChartWidget::hideAnalogPlots(const QList<int>& itemIds)
{
  this->setPlotsVisible(AnalogChart, itemIds, false);
};

void ChartWidget::showAnalogPlots(const QList<int>& itemIds)
{
  this->setPlotsVisible(AnalogChart, itemIds, true);
};

void ChartWidget::setExpandableAnalog(int expandable)
{
  if (this->m_CurrentChartType != AnalogChart)
  {
    qDebug("Trying to activate analog specific function, but the chart doesn't show analog data.");
    return;
  }
  
  AnalogChartData* analogChartData = static_cast<AnalogChartData*>(this->m_ChartData[AnalogChart]);
  analogChartData->setExpandable(expandable == 1);
  this->mp_ChartContentWidget->resizeCharts();
  this->render();
  
  this->setFocus();
};

void ChartWidget::setLastContextMenuPosition(const QPoint& globalPos)
{
  this->m_LastContextMenuPosition = this->mapFromGlobal(globalPos);
};

void ChartWidget::setCurrentFrameFunctor(btk::VTKCurrentFrameFunctor::Pointer functor)
{
  for (int i = 0 ; i < this->m_ChartData.size() ; ++i)
  {
    for (size_t j = 0 ; j < this->m_ChartData[i]->charts()->size() ; ++j)
      this->m_ChartData[i]->chart(j)->SetCurrentFrameFunctor(functor);
  }
};

void ChartWidget::setRegionOfInterestFunctor(btk::VTKRegionOfInterestFunctor::Pointer functor)
{
  for (int i = 0 ; i < this->m_ChartData.size() ; ++i)
  {
    for (size_t j = 0 ; j < this->m_ChartData[i]->charts()->size() ; ++j)
      this->m_ChartData[i]->chart(j)->SetRegionOfInterestFunctor(functor);
  }
};

void ChartWidget::setEventsFunctor(btk::VTKEventsFunctor::Pointer functor)
{
  for (int i = 0 ; i < this->m_ChartData.size() ; ++i)
  {
    for (size_t j = 0 ; j < this->m_ChartData[i]->charts()->size() ; ++j)
      this->m_ChartData[i]->chart(j)->SetEventsFunctor(functor);
  }
};

void ChartWidget::toggleOptions(const QPoint& pos)
{
  if (this->mp_ChartOptions->isVisible())
    this->mp_ChartOptions->setVisible(false);
  else
  {
    this->mp_ChartOptions->move(pos - QPoint(this->mp_ChartOptions->width() / 2, 0));
    this->mp_ChartOptions->setVisible(true);
  }
};

void ChartWidget::updateOptions()
{
  QList<AbstractChartData::PlotProperties>* props = &(this->m_ChartData[this->m_CurrentChartType]->plotsProperties());
  this->mp_ChartOptions->plotTable->setRowCount(props->count());
  for (int i = 0 ; i < props->size(); ++i)
  {
    AbstractChartData::PlotProperties* prop = &(props->operator[](i));
    this->mp_ChartOptions->setPlot(i, prop->label, prop->color, prop->lineWidth, prop->visible);
  }
};

void ChartWidget::dragEnterEvent(QDragEnterEvent* event)
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
        if (!this->m_ChartData[this->m_CurrentChartType]->acceptDroppedTreeWidgetItem(*it))
          return;
      }
      event->setDropAction(Qt::CopyAction); // To have the cross (+) symbol
      event->accept();
    }
  }
};

void ChartWidget::dropEvent(QDropEvent* event)
{
  event->setDropAction(Qt::IgnoreAction); // Only to know which Analog|Marker IDs were dropped.
  event->accept();
  QTreeWidget* treeWidget = qobject_cast<QTreeWidget*>(event->source());
  QList<QTreeWidgetItem*> selectedItems = treeWidget->selectedItems();
  bool plotAdded = false;
  for (QList<QTreeWidgetItem*>::const_iterator it = selectedItems.begin() ; it != selectedItems.end() ; ++it)
  {
    if (this->appendPlotFromDroppedItem(*it))
    {
      this->updateOptions();
      plotAdded = true;
    }
  }
  if (plotAdded)
  {
    for (size_t i = 0 ; i < this->m_ChartData[this->m_CurrentChartType]->charts()->size() ; ++i)
    {
      btk::VTKChartTimeSeries* chart = this->m_ChartData[this->m_CurrentChartType]->chart(i);
      chart->SetInteractionEnabled(true);
      chart->RecalculateBounds();
    }
  }
  this->render();
};

bool ChartWidget::appendPlotFromDroppedItem(QTreeWidgetItem* item)
{
  bool chartLayoutChanged = false;
  bool append = this->m_ChartData[this->m_CurrentChartType]->appendPlotFromDroppedItem(this->mp_Acquisition, this->mp_ColorGenerator, item, &chartLayoutChanged);
  if (chartLayoutChanged)
    this->mp_ChartContentWidget->resizeCharts();
  return append;
};


void ChartWidget::setPlotsVisible(int chartType, const QList<int>& itemIds, bool show)
{
  QList<AbstractChartData::PlotProperties>::const_iterator itProp = this->m_ChartData[chartType]->plotsProperties().begin();
  QList<int>::const_iterator itId = itemIds.begin();
  int index = 0;
  bool regenerateChartsLayout = false;
  while (itProp != this->m_ChartData[chartType]->plotsProperties().end())
  {
    bool found = false;
    if (itProp->id == *itId)
    {
      bool layoutModified = false;
      this->m_ChartData[chartType]->setPlotVisible(index, show, &layoutModified);
      regenerateChartsLayout |= layoutModified;
      found = true;
    }
    else
    {
      ++itProp;
      ++index;
    }
    if (found || (itProp == this->m_ChartData[chartType]->plotsProperties().end()))
    {
      index = 0;
      itProp = this->m_ChartData[chartType]->plotsProperties().begin();
      ++itId;
    }
    if (itId == itemIds.end())
      break;
  }
  if (this->m_CurrentChartType == chartType)
  {
    if (regenerateChartsLayout)
      this->mp_ChartContentWidget->resizeCharts();
    else
      this->checkResetAxes();
    this->updateOptions();
    this->render();
  }
};

void ChartWidget::checkResetAxes()
{
  bool plotVisible = false;
  for (QList<AbstractChartData::PlotProperties>::const_iterator it = this->m_ChartData[this->m_CurrentChartType]->plotsProperties().begin() ; it != this->m_ChartData[this->m_CurrentChartType]->plotsProperties().end() ; ++it)
  {
    if (it->visible)
    {
      plotVisible = true;
      break;
    }
  }  
  for (size_t i = 0 ; i < this->m_ChartData[this->m_CurrentChartType]->charts()->size() ; ++i)
  {
    btk::VTKChartTimeSeries* chart = this->m_ChartData[this->m_CurrentChartType]->chart(i);
    chart->SetInteractionEnabled(plotVisible);
    // Force the chart to draw ticks for the X axis.
    if (!plotVisible)
      chart->SetBounds((double)this->mp_Acquisition->firstFrame(), (double)this->mp_Acquisition->lastFrame(), 0.0, 0.0);
  }
};

void ChartWidget::updatePlotLabel(int chartType, int itemId)
{
  QList<AbstractChartData::PlotProperties>::iterator it = this->m_ChartData[chartType]->plotsProperties().begin();
  int index = 0;
  while(it != this->m_ChartData[chartType]->plotsProperties().end())
  {
    if (it->id == itemId)
    {
      it->label = this->m_ChartData[chartType]->createPlotLabel(this->mp_Acquisition, itemId);
      break;
    }
    ++index;
  }
  if (it != this->m_ChartData[chartType]->plotsProperties().end())
  {
    for (size_t i = 0 ; i < this->m_ChartData[chartType]->charts()->size() ; ++i)
    {
      vtkPlot* plot = this->m_ChartData[chartType]->chart(i)->GetPlot(index);
      plot->SetLabel(it->label.toUtf8().constData());
    }
    if (this->m_CurrentChartType == chartType)
      this->updateOptions();
  }
};

void ChartWidget::displayPointComponent(int idx, int state)
{
  if (this->m_CurrentChartType != PointChart)
  {
    qDebug("Trying to activate point specific function, but the chart doesn't show point data.");
    return;
  }
  
  this->m_ChartData[this->m_CurrentChartType]->chart(idx)->SetVisible((state == Qt::Checked) ? true : false);
  bool axisXTitleVisible = false;
  for (size_t i = 0 ; i < this->m_ChartData[this->m_CurrentChartType]->charts()->size() ; ++i)
    axisXTitleVisible |= this->m_ChartData[this->m_CurrentChartType]->chart(i)->GetVisible();
  this->mp_ChartAxisXLabel->setVisible(axisXTitleVisible);
  this->mp_ChartContentWidget->resizeCharts();
  this->render();
};

// -----------------------------------------------------------------------------

AbstractChartData::AbstractChartData(int num)
: m_Title(), m_PlotsProperties(), m_OptionSelection()
{
  this->mp_Charts = new VTKCharts();
  this->mp_Charts->resize(num, NULL);
  this->mp_Frames = NULL;
};

AbstractChartData::~AbstractChartData()
{
  for (size_t i = 0 ; i < this->mp_Charts->size() ; ++i)
    this->chart(i)->Delete();
  delete this->mp_Charts;
  
  if (this->mp_Frames != NULL)
    this->mp_Frames->Delete();
};

void AbstractChartData::appendPlotProperties(const QString& label, int id, const QColor& color, double lineWidth)
{
  PlotProperties prop;
  prop.label = label;
  prop.id = id;
  prop.color = color;
  prop.lineWidth = lineWidth;
  prop.visible = true;
  this->m_PlotsProperties.push_back(prop);
};

void AbstractChartData::copy(AbstractChartData* source)
{
  this->m_PlotsProperties.clear();
  this->setFrameArray(source->mp_Frames);
  for (size_t i = 0 ; i < this->mp_Charts->size() ; ++i)
  {
    btk::VTKChartTimeSeries* sourceChart = source->chart(i);
    btk::VTKChartTimeSeries* targetChart = this->chart(i);
    targetChart->SetBounds(sourceChart->GetBounds()[0], sourceChart->GetBounds()[1], 0.0, 0.0);
    targetChart->GetAxis(vtkAxis::BOTTOM)->SetLabelsVisible(sourceChart->GetAxis(vtkAxis::BOTTOM)->GetLabelsVisible());
    targetChart->GetAxis(vtkAxis::LEFT)->SetLabelsVisible(sourceChart->GetAxis(vtkAxis::LEFT)->GetLabelsVisible());
    targetChart->SetCurrentFrameFunctor(sourceChart->GetCurrentFrameFunctor());
    targetChart->SetRegionOfInterestFunctor(sourceChart->GetRegionOfInterestFunctor());
    targetChart->SetEventsFunctor(sourceChart->GetEventsFunctor());
  }
}

void AbstractChartData::clear()
{
  for (size_t i = 0 ; i < this->mp_Charts->size() ; ++i)
    this->chart(i)->ClearPlots();
};

void AbstractChartData::generateColor(vtkColorSeries* colorGenerator, double color[3])
{
  int numPlots = this->m_PlotsProperties.size();
  int colorIndex = numPlots;
  vtkColor3ub c = colorGenerator->GetColorRepeating(colorIndex);
  int num = 0, inc = 0, inc2 = 0;
  while (num < numPlots)
  {
    unsigned char rgb[3];
    this->chart(inc2)->GetPlot(inc)->GetPen()->GetColor(rgb);
    if ((c.Red() == rgb[0]) && (c.Green() == rgb[1]) && (c.Blue() == rgb[2]))
    {
      c = colorGenerator->GetColorRepeating(++colorIndex);
      num = 0; inc = 0; inc2 = 0;
    }
    else
      ++num; ++inc;
    if (inc >= this->chart(inc2)->GetNumberOfPlots())
    {
      ++inc2;
      inc = 0;
    }
  }
  color[0] = c[0] / 255.0;
  color[1] = c[1] / 255.0;
  color[2] = c[2] / 255.0;
};

void AbstractChartData::initialize(vtkColorSeries* colorGenerator)
{
  for (size_t i = 0 ; i < this->mp_Charts->size() ; ++i)
  {
    btk::VTKChartTimeSeries* chart = btk::VTKChartTimeSeries::New(); // Do not delete
    static_cast<btk::VTKAxis*>(chart->GetAxis(vtkAxis::BOTTOM))->SetTitleVisible(false); // Frames // X axis
    chart->SetBoundsEnabled(true);
    chart->DisplayEventsOn();
    chart->SetColorSeries(colorGenerator);
    this->mp_Charts->operator[](i) = chart;
  }
};

bool AbstractChartData::isAlreadyPlotted(int id)
{
  for (QList<AbstractChartData::PlotProperties>::iterator it = this->plotsProperties().begin() ; it != this->plotsProperties().end() ; ++it)
  {
    if (it->id == id)
      return true;
  }
  return false;
};

void AbstractChartData::removePlot(int index, bool* layoutModified)
{
  *layoutModified = false;
  for (size_t i = 0 ; i < this->mp_Charts->size() ; ++i)
    this->chart(i)->RemovePlot(index);
  this->m_PlotsProperties.removeAt(index);
};

void AbstractChartData::setFrameArray(vtkDoubleArray* array)
{
  if (this->mp_Frames == array)
    return;
  else if (this->mp_Frames != NULL)
    this->mp_Frames->Delete(); // Same as Unregister
  this->mp_Frames = array;
  array->Register(this->mp_Frames);
};

void AbstractChartData::setPlotVisible(int index, bool show, bool* layoutModified)
{
  *layoutModified = false;
  this->m_PlotsProperties[index].visible = show;
  for (size_t i = 0 ; i < this->mp_Charts->size() ; ++i)
    this->chart(i)->GetPlot(index)->SetVisible(show);
};

void AbstractChartData::show(Acquisition* acq, bool s, bool* layoutModified)
{
  *layoutModified = false;
  this->m_PlotsProperties.clear();
  for (size_t i = 0 ; i < this->mp_Charts->size() ; ++i)
  {
    btk::VTKChartTimeSeries* chart = this->chart(i);
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
      chart->SetBounds((double)acq->firstFrame(), (double)acq->lastFrame(), 0.0, 0.0);
      chart->GetAxis(vtkAxis::BOTTOM)->SetLabelsVisible(true);
      chart->GetAxis(vtkAxis::LEFT)->SetLabelsVisible(true);
    }
  }
};

// -----------------------------------------------------------------------------

PointChartData::PointChartData()
: AbstractChartData(3)
{};

bool PointChartData::acceptDroppedTreeWidgetItem(QTreeWidgetItem* item)
{
  // FIXME: The accepted type must be replaced by ModelDockWidget::(MarkerType|PointType) or enum values corresponding to a point
  if ((item->type() == QTreeWidgetItem::UserType + 2) || (item->type() == QTreeWidgetItem::UserType + 3))
    return true;
  return false;
};

bool PointChartData::appendPlotFromDroppedItem(Acquisition* acq, vtkColorSeries* colorGenerator, QTreeWidgetItem* item, bool* layoutModified)
{
  *layoutModified = false;
  int id = item->data(0, pointId).toInt();
  if (id > acq->pointCount())
  {
    qDebug("Point ID greater than the number of points.");
    return false;
  }
  else if (this->isAlreadyPlotted(id))
    return false;
  btk::Point::Pointer point = acq->btkAcquisition()->GetPoint(acq->points().value(id)->btkidx);
  int numFrames = acq->pointFrameNumber();
  // Need to create 3 table instead of 1 with 4 columns as VTK doesn't recognize the 2 last columns (due to the use of the same data?) 
  vtkTable* tableX = vtkTable::New();
  vtkTable* tableY = vtkTable::New();
  vtkTable* tableZ = vtkTable::New();
  tableX->SetNumberOfRows(numFrames); // Must be set before adding column
  tableX->AddColumn(this->mp_Frames);
  tableY->SetNumberOfRows(numFrames);
  tableY->AddColumn(this->mp_Frames);
  tableZ->SetNumberOfRows(numFrames);
  tableZ->AddColumn(this->mp_Frames);
  vtkDoubleArray* arrValX = vtkDoubleArray::New();
  vtkDoubleArray* arrValY = vtkDoubleArray::New();
  vtkDoubleArray* arrValZ = vtkDoubleArray::New();
  QString label = this->createPlotLabel(acq, id);
  arrValX->SetName(label.toUtf8().constData());
  arrValY->SetName(label.toUtf8().constData());
  arrValZ->SetName(label.toUtf8().constData());
  // FIXME: Conflict into VTK 5.6.1 between the documentation and the code to save or not the data. Need to check with VTK 5.8
  arrValX->SetArray(point->GetValues().data(), numFrames, 1); // Would be 0?
  arrValY->SetArray(point->GetValues().data() + numFrames, numFrames, 1); // Would be 0?
  arrValZ->SetArray(point->GetValues().data() + 2*numFrames, numFrames, 1); // Would be 0?
  tableX->AddColumn(arrValX);
  tableY->AddColumn(arrValY);
  tableZ->AddColumn(arrValZ);

  double color[3];
  const double lineWidth = 1.0;
  vtkPlotLine* plot = 0;
  this->generateColor(colorGenerator, color);
  // X axis 
  plot = vtkPlotLine::New();
  this->chart(0)->AddPlot(plot);
  plot->GetPen()->SetColorF(color);
  plot->SetInput(tableX,0,1);
  plot->SetWidth(lineWidth);
  plot->Delete();
  // Y axis
  plot = vtkPlotLine::New();
  this->chart(1)->AddPlot(plot);
  plot->GetPen()->SetColorF(color);
  plot->SetInput(tableY,0,1);
  plot->SetWidth(lineWidth);
  plot->Delete();
  // Z axis
  plot = vtkPlotLine::New();
  this->chart(2)->AddPlot(plot);
  plot->GetPen()->SetColorF(color);
  plot->SetInput(tableZ,0,1);
  plot->SetWidth(lineWidth);
  plot->Delete();

  arrValX->Delete();
  arrValY->Delete();
  arrValZ->Delete();
  tableX->Delete();
  tableY->Delete();
  tableZ->Delete();
  
  this->appendPlotProperties(label, id, QColor(static_cast<int>(color[0]*255.0), static_cast<int>(color[1]*255.0), static_cast<int>(color[2]*255.0)), lineWidth);

  return true;
};

QString PointChartData::createPlotLabel(Acquisition* acq, int id)
{
  Point* p = acq->points().value(id);
  return p->label + " (" + acq->pointUnit(p->type) + ")";
};

void PointChartData::initialize(vtkColorSeries* colorGenerator)
{
  this->AbstractChartData::initialize(colorGenerator);
  vtkChart* chart = 0;
  chart = this->chart(0);
  chart->GetAxis(vtkAxis::BOTTOM)->SetTitle("Frames"); // X axis
  chart->GetAxis(vtkAxis::LEFT)->SetTitle("X-Axis"); // Y axis
  chart = this->chart(1);
  chart->GetAxis(vtkAxis::BOTTOM)->SetTitle("Frames"); // X axis
  chart->GetAxis(vtkAxis::LEFT)->SetTitle("Y-Axis"); // Y axis
  chart = this->chart(2);
  chart->GetAxis(vtkAxis::BOTTOM)->SetTitle("Frames"); // X axis
  chart->GetAxis(vtkAxis::LEFT)->SetTitle("Z-Axis"); // Y axis
};

// -----------------------------------------------------------------------------

AnalogChartData::AnalogChartData()
: AbstractChartData(1)
{
  this->m_Expanded = false;
};

bool AnalogChartData::acceptDroppedTreeWidgetItem(QTreeWidgetItem* item)
{
  // FIXME: The accepted type must be replaced by ModelDockWidget::AnalogType or an enum value for the type analog channel
  if (item->type() == QTreeWidgetItem::UserType + 4)
    return true;
  return false;
};

bool AnalogChartData::appendPlotFromDroppedItem(Acquisition* acq, vtkColorSeries* colorGenerator, QTreeWidgetItem* item, bool* layoutModified)
{
  *layoutModified = false;
  int id = item->data(0, analogId).toInt();
  if (id > acq->analogCount())
  {
    qDebug("Analog ID greater than the number of analog channels.");
    return false;
  }
  else if (this->isAlreadyPlotted(id))
    return false;
  btk::Analog::Pointer analog = acq->btkAcquisition()->GetAnalog(id);
  vtkTable* table = vtkTable::New();
  table->SetNumberOfRows(acq->analogFrameNumber()); // Must be set before adding column
  table->AddColumn(this->mp_Frames);
  vtkDoubleArray* arrVal = vtkDoubleArray::New();
  QString label = this->createPlotLabel(acq, id);
  arrVal->SetName(label.toUtf8().constData());
  // FIXME: Conflict into VTK 5.6.1 between the documentation and the code to save or not the data. Need to check with VTK 5.8
  arrVal->SetArray(analog->GetValues().data(), analog->GetFrameNumber(), 1); // Would be 0?
  table->AddColumn(arrVal);
  double color[3];
  this->generateColor(colorGenerator, color);
  vtkPlotLine* plot = vtkPlotLine::New();
  if (!this->m_Expanded || (this->chart(0)->GetNumberOfPlots() == 0))
  {
    this->chart(0)->AddPlot(plot);
    if (this->m_Expanded)
      this->chart(0)->GetAxis(vtkAxis::LEFT)->SetTitle(label.toUtf8().constData());
  }
  else
  {
    btk::VTKChartTimeSeries* chart = this->chart(this->mp_Charts->size()-1);
    // Check if the last chart is empty, ready to be used
    if (chart->GetNumberOfPlots() != 0)
    {
      chart = this->createChart(this->chart(0));
      this->mp_Charts->push_back(chart);
      *layoutModified = true;
    }
    chart->GetAxis(vtkAxis::LEFT)->SetTitle(label.toUtf8().constData());
    chart->AddPlot(plot);
  }
  plot->GetPen()->SetColorF(color);
  plot->SetInput(table,0,1);
  plot->SetWidth(1.0);

  arrVal->Delete();
  table->Delete();
  plot->Delete();
  
  this->appendPlotProperties(label, id, QColor(static_cast<int>(color[0]*255.0), static_cast<int>(color[1]*255.0), static_cast<int>(color[2]*255.0)), plot->GetWidth());

  return true;
};

QString AnalogChartData::createPlotLabel(Acquisition* acq, int id)
{
  return acq->analogLabel(id) + " (" + acq->analogUnit(id) + ")";
};

void AnalogChartData::initialize(vtkColorSeries* colorGenerator)
{
  this->AbstractChartData::initialize(colorGenerator);
  this->chart(0)->GetAxis(vtkAxis::BOTTOM)->SetTitle("Frames"); // X axis
  this->chart(0)->GetAxis(vtkAxis::LEFT)->SetTitle("Values"); // Y axis
};

void AnalogChartData::setPlotVisible(int index, bool show, bool* layoutModified)
{
  if (!this->m_Expanded)
    this->AbstractChartData::setPlotVisible(index, show, layoutModified);
  else
  {
    *layoutModified = true;
    this->m_PlotsProperties[index].visible = show;
    this->chart(index)->GetPlot(0)->SetVisible(show);
    this->chart(index)->SetVisible(show);
 
    if (show && (this->chart(this->mp_Charts->size()-1)->GetNumberOfPlots() == 0)) // Remove the fake chart
    {
      VTKCharts::iterator it = this->mp_Charts->begin();
      std::advance(it, this->mp_Charts->size()-1);
      static_cast<btk::VTKContextScene*>(this->chart(0)->GetScene())->RemoveItem(*it);
      (*it)->Delete();
      it = this->mp_Charts->erase(it);
      // static_cast<btk::VTKContextScene*>(this->chart(0)->GetScene())->RemoveItem(this->chart(this->mp_Charts->size()-1));
    }
    else if (!show)
    {
      bool anyChartVisible = false;
      for (size_t i = 0 ; i < this->mp_Charts->size() ; ++i)
      {
        if (this->chart(i)->GetVisible())
        {
          anyChartVisible = true;
          break;
        }
      }
      if (!anyChartVisible) // Create a fake chart
      {
        btk::VTKChartTimeSeries* fakeChart = this->createChart(this->chart(0));
        fakeChart->GetAxis(vtkAxis::LEFT)->SetTitle("Values");
        this->mp_Charts->push_back(fakeChart);
      }
    }
  }
};

void AnalogChartData::removePlot(int index, bool* layoutModified)
{
  if (!this->m_Expanded)
    this->AbstractChartData::removePlot(index, layoutModified);
  else
  {
    this->m_PlotsProperties.removeAt(index);
    btk::VTKChartTimeSeries* chartZero = this->chart(0);
    if (!this->m_PlotsProperties.isEmpty())
    {
      VTKCharts::iterator it = this->mp_Charts->begin();
      std::advance(it, index);
      static_cast<btk::VTKContextScene*>(chartZero->GetScene())->RemoveItem(*it);
      (*it)->Delete();
      it = this->mp_Charts->erase(it);
      *layoutModified = true;
    }
    else
    {
      chartZero->RemovePlot(0);
      chartZero->GetAxis(vtkAxis::LEFT)->SetTitle("Values");
      *layoutModified = false;
    }
  }
};

void AnalogChartData::show(Acquisition* acq, bool s, bool* layoutModified)
{
  if (!this->m_Expanded || s) // All the possible loading or standard reset
    this->AbstractChartData::show(acq, s, layoutModified);
  else
  {
    this->m_PlotsProperties.clear();
    btk::VTKChartTimeSeries* chartZero = this->chart(0);
    VTKCharts::iterator it = this->mp_Charts->begin();
    std::advance(it, 1);
    while (it != this->mp_Charts->end())
    {
      (*it)->ClearPlots();
      static_cast<btk::VTKContextScene*>(chartZero->GetScene())->RemoveItem(*it);
      (*it)->Delete();
      it = this->mp_Charts->erase(it);
    }
    chartZero->GetAxis(vtkAxis::LEFT)->SetTitle("Values");
    chartZero->ClearPlots();
    chartZero->SetBounds(0.0, 0.0, 0.0, 0.0);
    chartZero->GetAxis(vtkAxis::BOTTOM)->SetLabelsVisible(false);
    chartZero->GetAxis(vtkAxis::LEFT)->SetLabelsVisible(false);
  }
};

void AnalogChartData::setExpandable(bool expandable)
{
  if (this->m_Expanded == expandable)
    return;
  this->m_Expanded = expandable;
  
  if (this->m_PlotsProperties.isEmpty())
    return;
  
  btk::VTKChartTimeSeries* chartZero = this->chart(0);
  // Expand of collapose the chart
  if (this->m_Expanded)
  {
    int numPlots = chartZero->GetNumberOfPlots();
    this->mp_Charts->resize(numPlots);
    for (int i = 1 ; i < numPlots ; ++i)
    {
      btk::VTKChartTimeSeries* chart = this->createChart(chartZero);
      vtkPlot* plot = chartZero->TakePlot(1);
      chart->AddPlot(plot);
      chart->GetAxis(vtkAxis::LEFT)->SetTitle(plot->GetLabel());
      plot->Delete();
      this->mp_Charts->operator[](i) = chart;
    }
    chartZero->GetAxis(vtkAxis::LEFT)->SetTitle(chartZero->GetPlot(0)->GetLabel());
  }
  else
  {
    // Can have only 1 plot per chart
    VTKCharts::iterator it = this->mp_Charts->begin();
    std::advance(it, 1); // Go to the second element
    while (it != this->mp_Charts->end())
    {
      chartZero->AddPlot((*it)->GetPlot(0));
      static_cast<btk::VTKContextScene*>(chartZero->GetScene())->RemoveItem(*it);
      (*it)->Delete();
      it = this->mp_Charts->erase(it);
    }
    chartZero->GetAxis(vtkAxis::LEFT)->SetTitle("Values");
  };
}

btk::VTKChartTimeSeries* AnalogChartData::createChart(btk::VTKChartTimeSeries* sourceChart)
{
  btk::VTKChartTimeSeries* targetChart = btk::VTKChartTimeSeries::New();
  static_cast<btk::VTKAxis*>(targetChart->GetAxis(vtkAxis::BOTTOM))->SetTitleVisible(false); // Frames // X axis
  targetChart->SetBounds(sourceChart->GetBounds()[0], sourceChart->GetBounds()[1], 0.0, 0.0);
  targetChart->SetBoundsEnabled(true);
  targetChart->SetCurrentFrameFunctor(sourceChart->GetCurrentFrameFunctor());
  targetChart->SetRegionOfInterestFunctor(sourceChart->GetRegionOfInterestFunctor());
  targetChart->SetEventsFunctor(sourceChart->GetEventsFunctor());
  targetChart->DisplayEventsOn();
  targetChart->SetColorSeries(sourceChart->GetColorSeries());
  sourceChart->GetScene()->AddItem(targetChart);
  return targetChart;
};

// -----------------------------------------------------------------------------

VTKChartWidget::VTKChartWidget(QWidget* parent, Qt::WindowFlags f)
: QVTKWidget(parent, f)
{
  this->mp_Charts = 0;
};

void VTKChartWidget::contextMenuEvent(QContextMenuEvent* event)
{
  // No need to send this event to VTK.
  this->QWidget::contextMenuEvent(event);
};

bool VTKChartWidget::event(QEvent* event)
{
  if (event->type() == QEvent::ToolTip)
  {
    QHelpEvent* helpEvent = static_cast<QHelpEvent*>(event);
    
    btk::VTKChartTimeSeries* chart = this->focusedChart(helpEvent->pos());
    if (chart != 0)
    {
      int pt1[2]; chart->GetPoint1(pt1); // Bottom left
    
      vtkAxis* axisX = chart->GetAxis(vtkAxis::BOTTOM);
      float pt1X[2]; axisX->GetPoint1(pt1X);
      float pt2X[2]; axisX->GetPoint2(pt2X);
      double miniX = axisX->GetMinimum(); double maxiX = axisX->GetMaximum();
      double scaleX = (maxiX - miniX) / (double)(pt2X[0] - pt1X[0]);
    
      vtkAxis* axisY = chart->GetAxis(vtkAxis::LEFT);
      float pt1Y[2]; axisY->GetPoint1(pt1Y);
      float pt2Y[2]; axisY->GetPoint2(pt2Y);
      double miniY = axisY->GetMinimum(); double maxiY = axisY->GetMaximum();
      double scaleY = (maxiY - miniY) / (double)(pt2Y[1] - pt1Y[1]);
    
      QPoint p = helpEvent->pos() - QPoint(pt1[0], -pt1[1]);
    
      vtkVector2f pos((double)p.x() * scaleX + miniX, (double)(this->height()-p.y()-1) * scaleY + miniY), coord, tolerance(5.0*scaleX,5.0*scaleY); // tolerance +/- 5 pixels 

      for (int i = 0 ; i < chart->GetNumberOfPlots() ; ++i)
      {
        vtkPlot* plot = chart->GetPlot(i);
        if (plot->GetVisible() && plot->GetNearestPoint(pos, tolerance, &coord))
        {
          QString str = "Frame: " + QString::number(coord.X()) + "<br/>" + QString::fromUtf8(plot->GetLabel()) + ": " + QString::number(coord.Y(), 'f', 1);
          QToolTip::showText(helpEvent->globalPos(), str);
          return true;
        }
      }
    }
    QToolTip::hideText();
    event->ignore();
    return true;
  }
  else if (event->type() == QEvent::ContextMenu)
  {
    QContextMenuEvent* contextMenuEvent = static_cast<QContextMenuEvent*>(event);
    emit contextMenuRequested(contextMenuEvent->globalPos());
  }
  return QVTKWidget::event(event);
};

void VTKChartWidget::keyPressEvent(QKeyEvent* event)
{
  // Keyboard events are not sent to VTK.
  this->QWidget::keyPressEvent(event);
};

void VTKChartWidget::keyReleaseEvent(QKeyEvent* event)
{
  // Keyboard events are not sent to VTK.
  this->QWidget::keyReleaseEvent(event);
};

// To not propagate the middle and right click to the charts
void VTKChartWidget::mousePressEvent(QMouseEvent* event)
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
    btk::VTKChartTimeSeries* chart = this->focusedChart(event->pos());
    if (chart != 0)
      chart->SetDisplayZoomBox((event->modifiers() & Qt::ShiftModifier) == Qt::ShiftModifier ? 1 : 0);
    iren->InvokeEvent(vtkCommand::LeftButtonPressEvent, event);
  }
};

// To not propagate the middle and right click to the charts
void VTKChartWidget::mouseReleaseEvent(QMouseEvent* event)
{
  vtkRenderWindowInteractor* iren = NULL;
  if(this->mRenWin)
    iren = this->mRenWin->GetInteractor();
  if(!iren || !iren->GetEnabled())
    return;
    
  // To fix a possible conflict between Qt and VTK when you use a contextual menu event.
  // VTK or the scene doesn't receive the event vtkCommand::LeftButtonReleaseEvent
  // In our case, the zoom box is not applied after using the reset zoom action
  btk::VTKChartTimeSeries* chart = this->focusedChart(event->pos());
  if (chart && (chart->GetDisplayZoomBox() == 1))
  {
    chart->ApplyZoom(chart->GetZoomBox());
    this->mRenWin->Render();
  }
  
  // give vtk event information (without modifiers informations: always set to 0)
  iren->SetEventInformationFlipY(event->x(), event->y(), 0, 0);
  
  // invoke appropriate vtk event only for the left button
  if(event->button() == Qt::LeftButton)
    iren->InvokeEvent(vtkCommand::LeftButtonReleaseEvent, event);
};

// To not propagate the middle and right click to the charts
void VTKChartWidget::mouseMoveEvent(QMouseEvent* event)
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

void VTKChartWidget::resizeEvent(QResizeEvent* event)
{
  this->QVTKWidget::resizeEvent(event);
  this->resizeCharts();
};

void VTKChartWidget::wheelEvent(QWheelEvent* event)
{
  vtkRenderWindowInteractor* iren = NULL;
  if(this->mRenWin)
    iren = this->mRenWin->GetInteractor();
  if(!iren || !iren->GetEnabled())
    return;
  
  btk::VTKChartTimeSeries* chart = this->focusedChart(event->pos());
  if (chart != 0)
    chart->SetZoomMode((event->modifiers() & Qt::ShiftModifier) == Qt::ShiftModifier ? btk::VTKChartTimeSeries::HORIZONTAL : btk::VTKChartTimeSeries::BOTH);
  
  // give vtk event information (without modifiers informations: always set to 0)
  iren->SetEventInformationFlipY(event->x(), event->y(), 0, 0);

  // invoke vtk event: if delta is positive, it is a forward wheel event
  if(event->delta() > 0)
    iren->InvokeEvent(vtkCommand::MouseWheelForwardEvent, event);
  else
    iren->InvokeEvent(vtkCommand::MouseWheelBackwardEvent, event);
};

btk::VTKChartTimeSeries* VTKChartWidget::focusedChart(const QPoint& pos) const
{
  btk::VTKChartTimeSeries* chart = 0;
  if (this->mp_Charts != NULL)
  {
    for (size_t i = 0 ; i < this->mp_Charts->size() ; ++i)
    {
      if (this->mp_Charts->operator[](i)->Hit(pos.x(), this->height() - pos.y()))
      {
        chart = this->mp_Charts->operator[](i);
        break;
      }
    }
  }
  return chart;
};

void VTKChartWidget::resizeCharts()
{
  if (this->mp_Charts == NULL)
    return;
  
  const float tickLength = 5.0f;
  // const float minimumTickSpacing = 50.0f;
  const float titleMargin = 10.0f;
  const float labelMargin = 5.0f;
  
  int num = 0 ;
  for (size_t i = 0 ; i < this->mp_Charts->size() ; ++i)
  {
    if (this->mp_Charts->operator[](i)->GetVisible())
      ++num;
  }
  if (num == 0)
    return;
  
  float heightF = static_cast<float>(this->height());
  float heightPerChart = heightF / static_cast<float>(num);
  float scaleY = heightPerChart / heightF;
  
  int inc = 1;
  for (size_t i = 0 ; i < this->mp_Charts->size() ; ++i)
  {
    if (!this->mp_Charts->operator[](i)->GetVisible())
      continue;
      
    this->mp_Charts->operator[](i)->SetBorders(60, static_cast<int>(20.0f / scaleY), 20, static_cast<int>(10.0 / scaleY));
    
    btk::VTKAxis* axisX = static_cast<btk::VTKAxis*>(this->mp_Charts->operator[](i)->GetAxis(vtkAxis::BOTTOM));
    axisX->SetTickLength(tickLength / scaleY);
    axisX->SetTitleMargin(titleMargin / scaleY);
    axisX->SetLabelMargin(labelMargin / scaleY);
    btk::VTKAxis* axisY = static_cast<btk::VTKAxis*>(this->mp_Charts->operator[](i)->GetAxis(vtkAxis::LEFT));
    // axisY->SetMinimumTickSpacing(minimumTickSpacing / scaleY);

    vtkTransform2D* transform = vtkTransform2D::New();
    transform->Translate(0.0f, heightF - heightPerChart * static_cast<float>(inc));
    transform->Scale(1.0f, scaleY);
    this->mp_Charts->operator[](i)->SetTransform(transform);
    axisX->SetTransform(transform);
    axisY->SetTransform(transform);
    transform->Delete();
    
    ++inc;
  }
};
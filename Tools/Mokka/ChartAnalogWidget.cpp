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

#include <btkVTKContextScene.h>

#include <vtkPlotLine.h>
#include <vtkPen.h>

ChartAnalogWidget::ChartAnalogWidget(QWidget* parent)
: AbstractChartWidget(1, parent)
{
  this->m_Expanded = false;
};

ChartAnalogWidget::~ChartAnalogWidget()
{};

void ChartAnalogWidget::setAcquisition(Acquisition* acq)
{
  this->AbstractChartWidget::setAcquisition(acq);
  connect(this->mp_Acquisition, SIGNAL(analogLabelChanged(int, QString)), this, SLOT(updatePlotLabel(int)));
  connect(this->mp_Acquisition, SIGNAL(analogsUnitChanged(QVector<int>, QVector<QString>)), this, SLOT(updatePlotLabel(QVector<int>)));
  connect(this->mp_Acquisition, SIGNAL(analogsRemoved(QList<int>, QList<Analog*>)), this, SLOT(hidePlots(QList<int>)));
  connect(this->mp_Acquisition, SIGNAL(analogsInserted(QList<int>, QList<Analog*>)), this, SLOT(showPlots(QList<int>)));
};

void ChartAnalogWidget::initialize()
{
  this->AbstractChartWidget::initialize();
  
  vtkChart* chart = this->mp_Charts->operator[](0);
  chart->GetAxis(vtkAxis::BOTTOM)->SetTitle("Frames"); // X axis
  chart->GetAxis(vtkAxis::LEFT)->SetTitle("Values"); // Y axis
};


bool ChartAnalogWidget::acceptDroppedTreeWidgetItem(QTreeWidgetItem* item)
{
  // FIXME: The accepted type must be replaced by ModelDockWidget::AnalogType or an enum value for the type analog channel
  if (item->type() == QTreeWidgetItem::UserType + 4)
    return true;
  return false;
};

void ChartAnalogWidget::removePlot(int index)
{
  if (!this->m_Expanded)
    this->AbstractChartWidget::removePlot(index);
  else
  {
    btk::VTKChartTimeSeries* chartZero = this->mp_Charts->operator[](0);
    if (this->mp_ChartOptions->plotTable->rowCount() != 0)
    {
      VTKCharts::iterator it = this->mp_Charts->begin();
      std::advance(it, index);
      static_cast<btk::VTKContextScene*>(chartZero->GetScene())->RemoveItem(*it);
      (*it)->Delete();
      it = this->mp_Charts->erase(it);
    }
    else
    {
      chartZero->RemovePlot(0);
      chartZero->GetAxis(vtkAxis::LEFT)->SetTitle("Values");
      this->checkResetAxes(); // If no more plot or all of them are hidden, then the axes are reset.
    }
    this->mp_ChartContentWidget->resizeCharts();
    this->render(true); // Options are shown
  }
};

void ChartAnalogWidget::setPlotLineColor(const QList<int>& indices, const QColor& color)
{
  if (!this->m_Expanded)
    this->AbstractChartWidget::setPlotLineColor(indices, color);
  else
  {
    for (QList<int>::const_iterator it = indices.begin() ; it != indices.end() ; ++it)
    {
      vtkPlot* plot = this->mp_Charts->operator[](*it)->GetPlot(0);
      plot->SetColor(color.redF(), color.greenF(), color.blueF());
    }
    this->render(true); // Options are shown
  }
};

void ChartAnalogWidget::setPlotLineWidth(const QList<int>& indices, double value)
{
  if (!this->m_Expanded)
    this->AbstractChartWidget::setPlotLineWidth(indices, value);
  else
  {
    for (QList<int>::const_iterator it = indices.begin() ; it != indices.end() ; ++it)
    {
      vtkPlot* plot = this->mp_Charts->operator[](*it)->GetPlot(0);
      plot->SetWidth(static_cast<float>(value));
    }
    this->render(true); // Options are shown
  }
};

void ChartAnalogWidget::updatePlotLabel(const QVector<int>& itemIds)
{
  for (int i = 0 ; i < itemIds.count() ; ++i)
    this->AbstractChartWidget::updatePlotLabel(itemIds[i]);
};

void ChartAnalogWidget::setExpandableChart(int expandable)
{
  bool expanded = (expandable == 1); // otherwise collapsed
  if (this->m_Expanded == expanded)
    return;
  this->m_Expanded = expanded;
  
  if (this->mp_ChartOptions->plotTable->rowCount() == 0)
    return;
  
  btk::VTKChartTimeSeries* chartZero = this->mp_Charts->operator[](0);
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
  this->mp_ChartContentWidget->resizeCharts();
  this->render();
  
  this->setFocus();
};

QString ChartAnalogWidget::createPlotLabel(int id)
{
  return this->mp_Acquisition->analogLabel(id) + " (" + this->mp_Acquisition->analogUnit(id) + ")";
};

bool ChartAnalogWidget::appendPlotFromDroppedItem(QTreeWidgetItem* item, int* itemId, QString& legend, double* color, double* width)
{
  int id = item->data(0, analogId).toInt();
  if (id > this->mp_Acquisition->analogCount())
  {
    qDebug("Analog ID greater than the number of analog channels.");
    return false;
  }
  else if (this->isAlreadyPlotted(id))
    return false;
  btk::Analog::Pointer analog = this->mp_Acquisition->btkAcquisition()->GetAnalog(id);
  vtkTable* table = vtkTable::New();
  table->SetNumberOfRows(this->mp_Acquisition->analogFrameNumber()); // Must be set before adding column
  table->AddColumn(this->mp_ArrayFrames);
  vtkDoubleArray* arrVal = vtkDoubleArray::New();
  legend = this->createPlotLabel(id);
  arrVal->SetName(legend.toUtf8().constData());
  // FIXME: Conflict into VTK 5.6.1 between the documentation and the code to save or not the data. Need to check with VTK 5.8
  arrVal->SetArray(analog->GetValues().data(), analog->GetFrameNumber(), 1); // Would be 0?
  table->AddColumn(arrVal);
  this->generateColor(color);
  vtkPlotLine* plot = vtkPlotLine::New();
  if (!this->m_Expanded || (this->mp_Charts->operator[](0)->GetNumberOfPlots() == 0))
  {
    this->mp_Charts->operator[](0)->AddPlot(plot);
    if (this->m_Expanded)
      this->mp_Charts->operator[](0)->GetAxis(vtkAxis::LEFT)->SetTitle(legend.toUtf8().constData());
  }
  else
  {
    btk::VTKChartTimeSeries* chart = this->createChart(this->mp_Charts->operator[](0));
    chart->GetAxis(vtkAxis::LEFT)->SetTitle(legend.toUtf8().constData());
    chart->AddPlot(plot);
    this->mp_Charts->push_back(chart);
    this->mp_ChartContentWidget->resizeCharts();
  }
  plot->GetPen()->SetColorF(color);
  plot->SetInput(table,0,1);
  plot->SetWidth(1.0);
  
  *itemId = id;
  *width = plot->GetWidth();
  
  arrVal->Delete();
  table->Delete();
  plot->Delete();
  
  return true;
};

btk::VTKChartTimeSeries* ChartAnalogWidget::createChart(btk::VTKChartTimeSeries* sourceChart)
{
  btk::VTKChartTimeSeries* targetChart = btk::VTKChartTimeSeries::New();
  static_cast<btk::VTKAxis*>(targetChart->GetAxis(vtkAxis::BOTTOM))->SetTitleVisible(false); // Frames // X axis
  targetChart->SetBoundsEnabled(true);
  targetChart->SetCurrentFrameFunctor(sourceChart->GetCurrentFrameFunctor());
  targetChart->SetRegionOfInterestFunctor(sourceChart->GetRegionOfInterestFunctor());
  targetChart->SetEventsFunctor(sourceChart->GetEventsFunctor());
  targetChart->DisplayEventsOn();
  targetChart->SetColorSeries(this->mp_ColorGenerator);
  sourceChart->GetScene()->AddItem(targetChart);
  return targetChart;
};
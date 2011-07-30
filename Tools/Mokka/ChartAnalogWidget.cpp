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
#include "UserRoles.h"

ChartAnalogWidget::ChartAnalogWidget(QWidget* parent)
: AbstractChartWidget(1, parent)
{};

ChartAnalogWidget::~ChartAnalogWidget()
{};

void ChartAnalogWidget::setAcquisition(Acquisition* acq)
{
  this->AbstractChartWidget::setAcquisition(acq);
  connect(this->mp_Acquisition, SIGNAL(analogLabelChanged(int, QString)), this, SLOT(updatePlotLabel(int)));
  connect(this->mp_Acquisition, SIGNAL(analogsUnitChanged(QVector<int>, QVector<QString>)), this, SLOT(updatePlotLabel(QVector<int>)));
};

void ChartAnalogWidget::initialize()
{
  this->AbstractChartWidget::initialize();
  
  vtkChartXY* chart = this->mp_VTKCharts->operator[](0);
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

void ChartAnalogWidget::updatePlotLabel(const QVector<int>& itemIds)
{
  for (int i = 0 ; i < itemIds.count() ; ++i)
    this->AbstractChartWidget::updatePlotLabel(itemIds[i]);
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
  
  vtkPlot* line = this->mp_VTKCharts->operator[](0)->AddPlot(vtkChart::LINE);
  line->SetInput(table,0,1);
  line->SetWidth(0.75);
  arrVal->Delete();
  table->Delete();
  
  *itemId = id;
  double c[3]; line->GetColor(c);
  color[0] = c[0]; color[1] = c[1]; color[2] = c[2];
  *width = line->GetWidth();
  return true;
};
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

#include "ChartPointWidget.h"
#include "UserRoles.h"

#include <QLayout>

ChartPointWidget::ChartPointWidget(QWidget* parent)
: AbstractChartWidget(3, parent)
{};

ChartPointWidget::~ChartPointWidget()
{};

void ChartPointWidget::setAcquisition(Acquisition* acq)
{
  this->AbstractChartWidget::setAcquisition(acq);
  connect(this->mp_Acquisition, SIGNAL(pointLabelChanged(int, QString)), this, SLOT(updatePlotLabel(int)));
  connect(this->mp_Acquisition, SIGNAL(pointsRemoved(QList<int>, QList<Point*>)), this, SLOT(hidePlots(QList<int>)));
  connect(this->mp_Acquisition, SIGNAL(pointsInserted(QList<int>, QList<Point*>)), this, SLOT(showPlots(QList<int>)));
};

void ChartPointWidget::initialize()
{
  this->AbstractChartWidget::initialize();
  
  vtkChartXY* chart = 0;
  
  chart = this->mp_VTKCharts->operator[](0);
  chart->GetAxis(vtkAxis::BOTTOM)->SetTitle("Frames"); // X axis
  chart->GetAxis(vtkAxis::LEFT)->SetTitle("X-Axis"); // Y axis
  
  chart = this->mp_VTKCharts->operator[](1);
  chart->GetAxis(vtkAxis::BOTTOM)->SetTitle("Frames"); // X axis
  chart->GetAxis(vtkAxis::LEFT)->SetTitle("Y-Axis"); // Y axis
  
  chart = this->mp_VTKCharts->operator[](2);
  chart->GetAxis(vtkAxis::BOTTOM)->SetTitle("Frames"); // X axis
  chart->GetAxis(vtkAxis::LEFT)->SetTitle("Z-Axis"); // Y axis
};

bool ChartPointWidget::acceptDroppedTreeWidgetItem(QTreeWidgetItem* item)
{
  // FIXME: The accepted type must be replaced by ModelDockWidget::(MarkerType|PointType) or enum values corresponding to a point
  if ((item->type() == QTreeWidgetItem::UserType + 2) || (item->type() == QTreeWidgetItem::UserType + 3))
    return true;
  return false;
};

void ChartPointWidget::displayComponentX(int state)
{
  this->displayComponent(0, state);
};

void ChartPointWidget::displayComponentY(int state)
{
  this->displayComponent(1, state);
};

void ChartPointWidget::displayComponentZ(int state)
{
  this->displayComponent(2, state);
};

QString ChartPointWidget::createPlotLabel(int id)
{
  Point* p = this->mp_Acquisition->points().value(id);
  return p->label + " (" + this->mp_Acquisition->pointUnit(p->type) + ")";
};

bool ChartPointWidget::appendPlotFromDroppedItem(QTreeWidgetItem* item, int* itemId, QString& legend, double* color, double* width)
{
  int id = item->data(0, pointId).toInt();
  if (id > this->mp_Acquisition->pointCount())
  {
    qDebug("Point ID greater than the number of points.");
    return false;
  }
  else if (this->isAlreadyPlotted(id))
    return false;
  btk::Point::Pointer point = this->mp_Acquisition->btkAcquisition()->GetPoint(this->mp_Acquisition->points().value(id)->btkidx);
  int numFrames = this->mp_Acquisition->pointFrameNumber();
  // Need to create 3 table instead of 1 with 4 columns as VTK doesn't recognize the 2 last columns (due to the use of the same data?) 
  vtkTable* tableX = vtkTable::New();
  vtkTable* tableY = vtkTable::New();
  vtkTable* tableZ = vtkTable::New();
  tableX->SetNumberOfRows(numFrames); // Must be set before adding column
  tableX->AddColumn(this->mp_ArrayFrames);
  tableY->SetNumberOfRows(numFrames);
  tableY->AddColumn(this->mp_ArrayFrames);
  tableZ->SetNumberOfRows(numFrames);
  tableZ->AddColumn(this->mp_ArrayFrames);
  vtkDoubleArray* arrValX = vtkDoubleArray::New();
  vtkDoubleArray* arrValY = vtkDoubleArray::New();
  vtkDoubleArray* arrValZ = vtkDoubleArray::New();
  legend = this->createPlotLabel(id);
  arrValX->SetName(legend.toUtf8().constData());
  arrValY->SetName(legend.toUtf8().constData());
  arrValZ->SetName(legend.toUtf8().constData());
  // FIXME: Conflict into VTK 5.6.1 between the documentation and the code to save or not the data. Need to check with VTK 5.8
  arrValX->SetArray(point->GetValues().data(), numFrames, 1); // Would be 0?
  arrValY->SetArray(point->GetValues().data() + numFrames, numFrames, 1); // Would be 0?
  arrValZ->SetArray(point->GetValues().data() + 2*numFrames, numFrames, 1); // Would be 0?
  tableX->AddColumn(arrValX);
  tableY->AddColumn(arrValY);
  tableZ->AddColumn(arrValZ);
  
  vtkPlot* line = 0;
  // X axis  
  line = this->mp_VTKCharts->operator[](0)->AddPlot(vtkChart::LINE);
  line->SetInput(tableX,0,1);
  line->SetWidth(0.75);
  // Y axis
  line = this->mp_VTKCharts->operator[](1)->AddPlot(vtkChart::LINE);
  line->SetInput(tableY,0,1);
  line->SetWidth(0.75);
  // Z axis
  line = this->mp_VTKCharts->operator[](2)->AddPlot(vtkChart::LINE);
  line->SetInput(tableZ,0,1);
  line->SetWidth(0.75);
  
  arrValX->Delete();
  arrValY->Delete();
  arrValZ->Delete();
  tableX->Delete();
  tableY->Delete();
  tableZ->Delete();
  
  *itemId = id;
  double c[3]; line->GetColor(c);
  color[0] = c[0]; color[1] = c[1]; color[2] = c[2];
  *width = line->GetWidth();
  
  return true;
};

void ChartPointWidget::displayComponent(int idx, int state)
{
  this->layout()->itemAt(idx)->widget()->setVisible((state == Qt::Checked) ? true : false);
  for (size_t i = 0 ; i < this->mp_VTKCharts->size() ; ++i)
    this->mp_VTKCharts->operator[](i)->GetAxis(vtkAxis::LEFT)->RecalculateTickSpacing(); // Y axis
  this->repaint();
};
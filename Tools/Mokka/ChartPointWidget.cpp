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

ChartPointWidget::ChartPointWidget(QWidget* parent)
: QWidget(parent)
{
  this->mp_Acquisition = 0;
  this->mp_ArrayFrames = 0;
  
#if 0
  vtkContextView* view;
  this->mp_VTKView[0] = new QVTKWidget(this);
  view = vtkContextView::New();
  view->SetInteractor(this->mp_VTKView[0]->GetInteractor());
  this->mp_VTKView[0]->SetRenderWindow(view->GetRenderWindow());
  // this->mp_VTKView[0]->setMinimumSize(320,200);
  this->mp_VTKChart[0] = vtkChartXY::New();
  //this->mp_VTKChart[0]->GetAxis(vtkAxis::BOTTOM)->SetVisible(false);
  // this->mp_VTKChart[0]->GetAxis(0)->SetVisible(false);
  // this->mp_VTKChart[0]->GetAxis(1)->SetVisible(false);
  // this->mp_VTKChart[0]->GetAxis(2)->SetVisible(false);
  // this->mp_VTKChart[0]->GetAxis(3)->SetVisible(false);
  view->GetScene()->AddItem(this->mp_VTKChart[0]);
  //this->mp_VTKChart[0]->SetShowLegend(true);
  //this->mp_VTKChart[0]->GetAxis(vtkAxis::BOTTOM)->GetTitleProperties()->SetFontSize(0);
  //this->mp_VTKChart[0]->GetAxis(1)->SetTitle("X axis");
  //this->mp_VTKChart[0]->GetAxis(0)->setVisible(false);
  view->Delete();
  
  this->mp_VTKView[1] = new QVTKWidget(this);
  view = vtkContextView::New();
  view->SetInteractor(this->mp_VTKView[1]->GetInteractor());
  this->mp_VTKView[1]->SetRenderWindow(view->GetRenderWindow());
  // this->mp_VTKView[1]->setMinimumSize(320,200);
  this->mp_VTKChart[1] = vtkChartXY::New();
  this->mp_VTKChart[1]->GetAxis(vtkAxis::BOTTOM)->SetVisible(false);
  view->GetScene()->AddItem(this->mp_VTKChart[1]);
  //this->mp_VTKChart[1]->SetShowLegend(true);
  //this->mp_VTKChart[0]->GetAxis(vtkAxis::BOTTOM)->GetTitleProperties()->SetFontSize(20);
  //this->mp_VTKChart[1]->GetAxis(0)->SetLabelsVisible(false);
  //this->mp_VTKChart[1]->GetAxis(1)->SetTitle("Y axis");
  //this->mp_VTKChart[1]->GetAxis(0)->setVisible(false);
  view->Delete();
  
  this->mp_VTKView[2] = new QVTKWidget(this);
  view = vtkContextView::New();
  view->SetInteractor(this->mp_VTKView[2]->GetInteractor());
  this->mp_VTKView[2]->SetRenderWindow(view->GetRenderWindow());
  // this->mp_VTKView[2]->setMinimumSize(320,200);
  this->mp_VTKChart[2] = vtkChartXY::New();
  this->mp_VTKChart[2]->GetAxis(vtkAxis::BOTTOM)->SetVisible(false);
  view->GetScene()->AddItem(this->mp_VTKChart[2]);
  //this->mp_VTKChart[2]->SetShowLegend(true);
  //this->mp_VTKChart[2]->GetAxis(0)->SetTitle("Frames");
  //this->mp_VTKChart[2]->GetAxis(1)->SetTitle("Z axis");
  view->Delete();
  
  QVBoxLayout* layout = new QVBoxLayout;
  layout->setContentsMargins(0,0,0,0);
  layout->setSpacing(0);
  layout->addWidget(this->mp_VTKView[0]);
  layout->addWidget(this->mp_VTKView[1]);
  layout->addWidget(this->mp_VTKView[2]);
  
  QWidget* layoutWidget = new QWidget(this);
  layoutWidget->setMinimumSize(200,400);
  layoutWidget->setLayout(layout);
  
  QScrollArea* scrollArea = new QScrollArea(this);
  scrollArea->setWidget(layoutWidget);
  scrollArea->setWidgetResizable(true);
  // scrollArea->setLayout(layout);
  
  layout = new QVBoxLayout;
  layout->setContentsMargins(0,0,0,0);
  layout->setSpacing(0);
  layout->addWidget(scrollArea);
  this->setLayout(layout);
#endif
}

ChartPointWidget::~ChartPointWidget()
{
  if (this->mp_ArrayFrames != NULL)
    this->mp_ArrayFrames->Delete();
  // for (int i = 0 ; i < 3 ; ++i)
  //   this->mp_VTKChart[i]->Delete();
};

void ChartPointWidget::initialize()
{
 
 
#if 0
 
  vtkTable* table = vtkTable::New();
  
  vtkDoubleArray* arrF = vtkDoubleArray::New();
  arrF->SetName("Frame");
  table->AddColumn(arrF);
  
  vtkDoubleArray* arrX = vtkDoubleArray::New();
  arrX->SetName("Axis X");
  table->AddColumn(arrX);
  
  vtkDoubleArray* arrY = vtkDoubleArray::New();
  arrY->SetName("Axis Y");
  table->AddColumn(arrY);
  
  vtkDoubleArray* arrZ = vtkDoubleArray::New();
  arrZ->SetName("Axis Z");
  table->AddColumn(arrZ);
  
  int numPoints = 2;
  table->SetNumberOfRows(numPoints);
  for (int i = 0; i < numPoints; ++i)
  {
    table->SetValue(i, 0, 0.0);
    table->SetValue(i, 1, (double)i);
    table->SetValue(i, 2, (double)i);
    table->SetValue(i, 3, (double)i);
  }
  
  // Add multiple scatter plots, setting the colors etc
  vtkPlot *points = this->mp_VTKChart[0]->AddPlot(vtkChart::POINTS);
  points->SetInput(table, 0, 1);
  //points->GetXAxis()->SetLabelsVisible(false);
  points->GetXAxis()->SetVisible(false);
  //points->GetXAxis()->SetTitle("");
  //points->SetXAxis(0);
  points->GetYAxis()->SetTitle("X axis");
  //points->SetColor(0, 0, 0, 255);
  //points->SetWidth(1.0);
  //vtkPlotPoints::SafeDownCast(points)->SetMarkerStyle(vtkPlotPoints::CROSS);
 
  points = this->mp_VTKChart[1]->AddPlot(vtkChart::POINTS);
  points->SetInput(table, 0, 2);
  //points->GetXAxis()->SetLabelsVisible(false);
  points->GetXAxis()->SetVisible(false);
  //points->GetXAxis()->SetTitle("");
  points->GetYAxis()->SetTitle("Y axis");
  //points->SetColor(0, 0, 0, 255);
  //points->SetWidth(1.0);
  //vtkPlotPoints::SafeDownCast(points)->SetMarkerStyle(vtkPlotPoints::PLUS);
 
  points = this->mp_VTKChart[2]->AddPlot(vtkChart::POINTS);
  points->SetInput(table, 0, 3);
  points->SetInput(table, 0, 3);
  points->GetXAxis()->SetTitle("Frame");
  points->GetYAxis()->SetTitle("Z axis");
  //points->SetColor(0, 0, 255, 255);
  //points->SetWidth(1.0);
  //vtkPlotPoints::SafeDownCast(points)->SetMarkerStyle(vtkPlotPoints::CIRCLE);

  arrF->Delete();
  arrX->Delete();
  arrY->Delete();
  arrZ->Delete();
  table->Delete();
#endif
 /*
  vtkTable* table = vtkTable::New();
  
  vtkDoubleArray* arrX = vtkDoubleArray::New();
  arrX->SetName("X Axis");
  table->AddColumn(arrX);
  
  vtkDoubleArray* arrC = vtkDoubleArray::New();
  arrC->SetName("Cosine");
  table->AddColumn(arrC);
  
  vtkDoubleArray* arrS = vtkDoubleArray::New();
  arrS->SetName("Sine");
  table->AddColumn(arrS);
  
  vtkDoubleArray* arrT = vtkDoubleArray::New();
  arrT->SetName("Sine - Cosine");
  table->AddColumn(arrT);
  
  int numPoints = 3600;
  float inc = 10 * 7.5 / (numPoints-1);
  table->SetNumberOfRows(numPoints);
  for (int i = 0; i < numPoints; ++i)
  {
    table->SetValue(i, 0, i * inc);
    table->SetValue(i, 1, cos(i * inc) + 0.0);
    table->SetValue(i, 2, sin(i * inc) + 0.0);
    table->SetValue(i, 3, sin(i * inc) - cos(i * inc));
  }
  
  // Add multiple scatter plots, setting the colors etc
  vtkPlot *points = this->mp_VTKChart[0]->AddPlot(vtkChart::LINE);
  points->SetInput(table, 0, 1);
  points->SetColor(0, 0, 0, 255);
  points->SetWidth(1.0);
  vtkPlotPoints::SafeDownCast(points)->SetMarkerStyle(vtkPlotPoints::CROSS);
 
  points = this->mp_VTKChart[1]->AddPlot(vtkChart::LINE);
  points->SetInput(table, 0, 2);
  points->SetColor(0, 0, 0, 255);
  points->SetWidth(1.0);
  vtkPlotPoints::SafeDownCast(points)->SetMarkerStyle(vtkPlotPoints::PLUS);
 
  points = this->mp_VTKChart[2]->AddPlot(vtkChart::LINE);
  points->SetInput(table, 0, 3);
  points->SetColor(0, 0, 255, 255);
  points->SetWidth(1.0);
  vtkPlotPoints::SafeDownCast(points)->SetMarkerStyle(vtkPlotPoints::CIRCLE);

  arrX->Delete();
  arrC->Delete();
  arrS->Delete();
  arrT->Delete();
  table->Delete();
  */
  
/*
  view->Delete();
  chart1->Delete();
  chart2->Delete();
  chart3->Delete();
*/  
};

void ChartPointWidget::setFrameArray(vtkDoubleArray* array)
{
  if (this->mp_ArrayFrames == array)
    return;
  else if (this->mp_ArrayFrames != NULL)
    this->mp_ArrayFrames->Delete();
  this->mp_ArrayFrames = array;
  array->Register(this->mp_ArrayFrames);
};

void ChartPointWidget::show(bool s)
{
  Q_UNUSED(s)
};

void ChartPointWidget::copy(ChartPointWidget* source)
{
  // Copy the acquisition pointer
  this->setAcquisition(source->acquisition());
  // Copy the X axis
  this->setFrameArray(source->frameArray());
  // Copy the plots
};

void ChartPointWidget::render()
{
  for (int i = 0 ; i < 3 ; ++i)
    this->mp_VTKView[i]->GetRenderWindow()->Render();
};
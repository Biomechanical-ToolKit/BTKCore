/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2012, Arnaud Barré
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

#include "ChartExportDialog.h"
#include "ProgressWidget.h"

#include <QFileDialog>
#include <QScrollBar>
#include <QPainter>

#include <QMessageBox>

#include <btkVTKChartTimeSeries.h>
#include <btkVTKChartLegend.h>

#include <vtkAxis.h>
#include <vtkTextProperty.h>
#include <vtkPlot.h>
#include <vtkChartLegend.h>
#include <vtkRenderWindow.h>
#include <vtkUnsignedCharArray.h>
#include <vtkContextScene.h>
#include <vtkPen.h>
#include <vtkCamera.h>
#include <vtkRenderer.h>
#include <vtkTransform2D.h>
#include <vtkStringArray.h>
#include <vtkFloatArray.h>

ChartExportDialog::ChartExportDialog(QWidget* parent)
: QDialog(parent)
{
  this->setupUi(this);
#ifdef Q_OS_MAC
  this->setWindowFlags(Qt::Sheet);
  this->setWindowModality(Qt::WindowModal);
  
  // this->resize(this->width(), this->height()-1); // FIXME: Only the way to remove the size grip under MacOS X?
  this->setMaximumSize(this->minimumSize());
#endif
  this->imagePreview->initialize();
  this->mp_Chart = this->imagePreview->chart();
  this->mp_Chart->GetTitleProperties()->SetBold(1);
  
  connect(this->imageWidthUnit, SIGNAL(currentIndexChanged(int)), this, SLOT(convertWidthUnit(int)));
  connect(this->imageHeightUnit, SIGNAL(currentIndexChanged(int)), this, SLOT(convertHeightUnit(int)));
  connect(this->chartTitle, SIGNAL(textEdited(QString)), this, SLOT(updateChartTile(QString)));
  connect(this->chartFont, SIGNAL(currentIndexChanged(int)), this, SLOT(updateChartFont(int)));
  connect(this->chartFontSize, SIGNAL(valueChanged(int)), this, SLOT(updateChartFontSize(int)));
  connect(this->chartFontBold, SIGNAL(toggled(bool)), this, SLOT(updateChartFontBold(bool)));
  connect(this->chartFontItalic, SIGNAL(toggled(bool)), this, SLOT(updateChartFontItalic(bool)));
  connect(this->axisXTitle, SIGNAL(textEdited(QString)), this, SLOT(updateAxisXTitle(QString)));
  connect(this->axisYTitle, SIGNAL(textEdited(QString)), this, SLOT(updateAxisYTitle(QString)));
  connect(this->legendVisibleCheckBox, SIGNAL(stateChanged(int)), this, SLOT(updateLegendVisibility(int)));
  connect(this->unitDisplayedCheckBox, SIGNAL(stateChanged(int)), this, SLOT(updateLegendUnitVisibility(int)));
  connect(this->legendLocationComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateLegendLocation(int)));
  
  this->imagePreview->setFocus();
};

ChartExportDialog::~ChartExportDialog()
{};

void ChartExportDialog::setChart(btk::VTKChartTimeSeries* chart)
{
  this->imagePreview->setChart(this->m_Units, chart);
  this->axisXTitle->setText(QString::fromStdString(chart->GetAxis(vtkAxis::BOTTOM)->GetTitle()));
  this->axisYTitle->setText(QString::fromStdString(chart->GetAxis(vtkAxis::LEFT)->GetTitle()));
  this->updateChartFontSize(12);
  this->update();
};

void ChartExportDialog::accept()
{
  QString selectedFilter = "PNG Files (*.png)";
  QString filename = QFileDialog::getSaveFileName(this, "",
                       (!this->chartTitle->text().isEmpty() ? this->chartTitle->text() : "untitled") + ".png",
                       tr("BMP Files (*.bmp) ;;"
                          "JPEG Files (*.jpg, *.jpeg) ;;"
                          "PNG Files (*.png);;"
                          "TIFF Files (*.tiff)"),
                       &selectedFilter);
  if (!filename.isEmpty())
  {
    int dpi = 0;
    switch(this->imageResolution->currentIndex())
    {
    case 0:
      dpi = 150;
      break;
    case 1:
      dpi = 300;
      break;
    case 2:
      dpi = 600;
      break;
    case 3:
      dpi = 1200;
      break;
    default:
      qDebug("No defined DPI value. Set to 150.");
      dpi = 150;
    }
    int dpm = static_cast<int>((double)dpi * 39.37007874); // dots per meter
    int w = static_cast<int>(this->imageWidth->value() / (this->imageWidthUnit->currentIndex() == 0 ? 2.54 : 1.0) * (double)dpi);
    int h = static_cast<int>(this->imageHeight->value() / (this->imageHeightUnit->currentIndex() == 0 ? 2.54 : 1.0) * (double)dpi);
    
    // chart scale factor: Every size are set to be drawn on a standard display with a resolution of 72 DPI.
    // Then, the chart must be scaled for higher resolution;
    int scale = dpi / 72; 
    float scaleF = static_cast<float>(scale);
    float scaleF2 = scaleF / 2.0f;
    int scaleFont = this->chartFontSize->value() * scale;
    
    // Create the image by part
    int w1 = this->imagePreview->width();
    int h1 = this->imagePreview->height();
    double scaleX = (double)w / (double)w1;
    int numPartX = static_cast<int>(ceil(scaleX));
    double scaleY = (double)h / (double)h1;
    int numPartY = static_cast<int>(ceil(scaleY));
    
    // Scale data
    vtkPen* pen = 0;
    // - Axis X
    btk::VTKAxis* axisX = btk::VTKAxis::SafeDownCast(this->mp_Chart->GetAxis(vtkAxis::BOTTOM));
    axisX->GetTitleProperties()->SetFontSize(scaleFont);
    axisX->GetLabelProperties()->SetFontSize(scaleFont);
    axisX->SetTickLength(axisX->GetTickLength() * scaleF / scaleY);
    axisX->SetTitleMargin(axisX->GetTitleMargin() * scaleF2 / scaleY);
    axisX->SetLabelMargin(axisX->GetLabelMargin() * scaleF2 / scaleY);
    pen = axisX->GetPen();
    pen->SetWidth(pen->GetWidth() * scaleF);
    pen = axisX->GetGridPen();
    pen->SetWidth(pen->GetWidth() * scaleF);
    // - Axis Y
    btk::VTKAxis* axisY = btk::VTKAxis::SafeDownCast(this->mp_Chart->GetAxis(vtkAxis::LEFT));
    axisY->GetTitleProperties()->SetFontSize(scaleFont);
    axisY->GetLabelProperties()->SetFontSize(scaleFont);
    axisY->SetTickLength(axisY->GetTickLength() * scaleF / scaleX);
    axisY->SetTitleMargin(axisY->GetTitleMargin() * scaleF2 / scaleX);
    axisY->SetLabelMargin(axisY->GetLabelMargin() * scaleF2 / scaleX);
    pen = axisY->GetPen();
    pen->SetWidth(pen->GetWidth() * scaleF);
    pen = axisY->GetGridPen();
    pen->SetWidth(pen->GetWidth() * scaleF);
    // - Legend
    btk::VTKChartLegend* legend = btk::VTKChartLegend::SafeDownCast(this->mp_Chart->GetLegend());
    legend->SetLabelSize(static_cast<int>((double)scaleFont * 0.8));
    legend->SetSymbolWidth(legend->GetSymbolWidth() * scaleF2 / scaleX);
    const float* padding = legend->GetPadding();
    legend->SetPadding(padding[0] * scaleF2 / scaleX, padding[1] * scaleF2 / scaleY, padding[2] * scaleF2 / scaleX, padding[3] * scaleF2 / scaleY);
    // - Chart
    this->mp_Chart->GetTitleProperties()->SetFontSize(scaleFont);
    this->mp_Chart->SetTitleMargin(static_cast<int>(static_cast<float>(this->mp_Chart->GetTitleMargin()) * scaleF2 / scaleY));
    this->mp_Chart->SetEventLineWidth(this->mp_Chart->GetEventLineWidth() * scaleF);
    this->mp_Chart->SetEventLineTypeFactor(this->mp_Chart->GetEventLineTypeFactor() * scale);
    this->mp_Chart->SetClippingEnabled(false);
    for (int i = 0 ; i < this->mp_Chart->GetNumberOfPlots() ; ++i)
    {
      vtkPlot* plot = this->mp_Chart->GetPlot(i);
      plot->SetWidth(plot->GetWidth() * scaleF);
    }
    // Update the chart border to take into account the scaled font.
    QRectF titleRect, labelRect;
    const int* borders = this->mp_Chart->GetBorders();
    // - Bottom
    titleRect = this->computeStringBounds(this->axisXTitle->text(), axisX->GetTitleProperties());
    labelRect = this->computeStringBounds("0", axisX->GetLabelProperties()); // No need of the true text as only the height will be used.
    int bottom = static_cast<int>(ceil(axisX->GetTitleMargin() + (titleRect.height() + 10.0 * scaleF2 + labelRect.height()) / scaleY + axisX->GetLabelMargin()));
    // - Left
    titleRect = this->computeStringBounds(this->axisYTitle->text(), axisY->GetTitleProperties());
    vtkStdString label = axisY->GetTickLabels()->GetValue(axisY->GetTickLabels()->GetNumberOfValues() - 1);
    if (label.length() < axisY->GetTickLabels()->GetValue(0).length())
      label = axisY->GetTickLabels()->GetValue(0);
    labelRect = this->computeStringBounds(QString::fromStdString(label), axisY->GetLabelProperties());
    int left = static_cast<int>(ceil(axisX->GetTitleMargin() + (titleRect.height() + 12.5 * scaleF2 + labelRect.width()) / scaleX + axisY->GetLabelMargin()));
    // - Right
    int right = static_cast<int>(ceil(static_cast<float>(borders[2]) * scaleF2 / scaleX));
    // - Top
    int top = this->mp_Chart->GetTitleMargin();
    if (!this->chartTitle->text().isEmpty())
    {
      titleRect = this->computeStringBounds(this->chartTitle->text(), this->mp_Chart->GetTitleProperties());
      top += static_cast<int>(ceil(titleRect.height() / scaleY)) + this->mp_Chart->GetTitleMargin() * 5 / 4;
    }
    // - Set new borders.
    this->mp_Chart->SetBorders(left, bottom, right, top);
    // - Force the legend's location as it will be reseted by the next rendering.
    this->imagePreview->GetRenderWindow()->Render();
    this->updateLegendLocation(this->legendLocationComboBox->currentIndex());
    
    QImage img(w, h, QImage::Format_RGB32);
    
    ProgressWidget pw(0);
    pw.show();
    float progress = 0.0f;
    float incProgress = 100.0f / static_cast<float>(numPartX * numPartY);
    
    QPainter painter;
    int offsetY = 0;
    painter.begin(&img);
    for (int i = 0 ; i < numPartY ; ++i)
    {
      int offsetX = 0;
      for (int j = 0 ; j < numPartX ; ++j)
      {
        if ((offsetX + w1) > w)
          offsetX = w - w1;
        if ((offsetY + h1) > h)
          offsetY = h - h1;
        
        vtkTransform2D* transform = vtkTransform2D::New();
        transform->Translate(-offsetX, -offsetY);
        transform->Scale(scaleX, scaleY);
        this->imagePreview->chart()->SetTransform(transform);
        axisX->SetTransform(transform);
        axisY->SetTransform(transform);
        this->imagePreview->chart()->GetLegend()->SetTransform(transform);
        transform->Delete();
        
        QImage buffer(w1, h1, QImage::Format_RGB32);
        vtkUnsignedCharArray* pixels = vtkUnsignedCharArray::New();
        pixels->SetArray(buffer.bits(), w1*h1*4, 1);
        this->imagePreview->GetRenderWindow()->Render();
        this->imagePreview->GetRenderWindow()->GetRGBACharPixelData(0, 0, w1-1, h1-1, 1, pixels);
        buffer = buffer.rgbSwapped();
        buffer = buffer.mirrored();
        pixels->Delete();
        
        painter.drawImage(offsetX, h-offsetY-h1, buffer);
        
        offsetX += w1;
        progress += incProgress;
        pw.setProgressValue(static_cast<int>(progress));
      }
      offsetY += h1;
    }
    painter.end();
    
    img.setDotsPerMeterX(dpm);
    img.setDotsPerMeterY(dpm);
    img.save(filename);
    
    pw.setProgressValue(100);
    
    this->QDialog::accept();
  }
}

void ChartExportDialog::render()
{
  if (this->isVisible())
    this->imagePreview->GetRenderWindow()->Render();
};

void ChartExportDialog::updateChartTile(const QString& title)
{
  this->updateChartTile(title, this->chartFontSize->value());
};

void ChartExportDialog::updateChartTile(const QString& title, int value)
{
  vtkAxis* axis = this->mp_Chart->GetAxis(vtkAxis::LEFT);
  int margin = 10;
  int maxLen = std::max(QString::number(static_cast<int>(axis->GetMinimum())).length(), QString::number(static_cast<int>(axis->GetMaximum())).length());
  int left = margin + value + margin + maxLen * static_cast<int>((float)value * 2.0f/3.0f);
  int bottom = margin + value + margin + value;
  if (title.isEmpty())
  {
    this->mp_Chart->SetBorders(left, bottom, margin, margin);
    this->mp_Chart->SetTitle(NULL);
  }
  else
  {
    this->mp_Chart->SetBorders(left, bottom, margin, margin + value + margin);
    this->mp_Chart->SetTitle(title.toUtf8().constData());
  }
  
  this->render();
};

void ChartExportDialog::updateChartFont(int index)
{
  switch(index)
  {
  case 0: // Arial
    this->mp_Chart->GetTitleProperties()->SetFontFamilyToArial();
    this->mp_Chart->GetAxis(vtkAxis::BOTTOM)->GetTitleProperties()->SetFontFamilyToArial();
    this->mp_Chart->GetAxis(vtkAxis::BOTTOM)->GetLabelProperties()->SetFontFamilyToArial();
    this->mp_Chart->GetAxis(vtkAxis::LEFT)->GetTitleProperties()->SetFontFamilyToArial();
    this->mp_Chart->GetAxis(vtkAxis::LEFT)->GetLabelProperties()->SetFontFamilyToArial();
    break;
  case 1: // Courrier
    this->mp_Chart->GetTitleProperties()->SetFontFamilyToCourier();
    this->mp_Chart->GetAxis(vtkAxis::BOTTOM)->GetTitleProperties()->SetFontFamilyToCourier();
    this->mp_Chart->GetAxis(vtkAxis::BOTTOM)->GetLabelProperties()->SetFontFamilyToCourier();
    this->mp_Chart->GetAxis(vtkAxis::LEFT)->GetTitleProperties()->SetFontFamilyToCourier();
    this->mp_Chart->GetAxis(vtkAxis::LEFT)->GetLabelProperties()->SetFontFamilyToCourier();
    break;
  case 2: // Times
    this->mp_Chart->GetTitleProperties()->SetFontFamilyToTimes();
    this->mp_Chart->GetAxis(vtkAxis::BOTTOM)->GetTitleProperties()->SetFontFamilyToTimes();
    this->mp_Chart->GetAxis(vtkAxis::BOTTOM)->GetLabelProperties()->SetFontFamilyToTimes();
    this->mp_Chart->GetAxis(vtkAxis::LEFT)->GetTitleProperties()->SetFontFamilyToTimes();
    this->mp_Chart->GetAxis(vtkAxis::LEFT)->GetLabelProperties()->SetFontFamilyToTimes();
    break;
  default:
    qDebug("No known index to set chart font. Set to Arial Font.");
    this->mp_Chart->GetTitleProperties()->SetFontFamilyToArial();
    this->mp_Chart->GetAxis(vtkAxis::BOTTOM)->GetTitleProperties()->SetFontFamilyToArial();
    this->mp_Chart->GetAxis(vtkAxis::BOTTOM)->GetLabelProperties()->SetFontFamilyToArial();
    this->mp_Chart->GetAxis(vtkAxis::LEFT)->GetTitleProperties()->SetFontFamilyToArial();
    this->mp_Chart->GetAxis(vtkAxis::LEFT)->GetLabelProperties()->SetFontFamilyToArial();
  }
  this->render();
};

void ChartExportDialog::updateChartFontSize(int value)
{
  this->mp_Chart->GetTitleProperties()->SetFontSize(value);
  this->mp_Chart->GetAxis(vtkAxis::BOTTOM)->GetTitleProperties()->SetFontSize(value);
  this->mp_Chart->GetAxis(vtkAxis::BOTTOM)->GetLabelProperties()->SetFontSize(value);
  this->mp_Chart->GetAxis(vtkAxis::LEFT)->GetTitleProperties()->SetFontSize(value);
  this->mp_Chart->GetAxis(vtkAxis::LEFT)->GetLabelProperties()->SetFontSize(value);
  this->mp_Chart->GetLegend()->SetLabelSize(static_cast<int>((double)value * 0.8));
  // Update the borders of the chart (conflict between axis' title and ticks' label?)
  this->updateChartTile(this->chartTitle->text(), value);
  this->render();
};

void ChartExportDialog::updateChartFontBold(bool toggled)
{
  int activated = (toggled ? 1 : 0);
  this->mp_Chart->GetTitleProperties()->SetBold(activated);
  this->mp_Chart->GetAxis(vtkAxis::BOTTOM)->GetTitleProperties()->SetBold(activated);
  this->mp_Chart->GetAxis(vtkAxis::LEFT)->GetTitleProperties()->SetBold(activated);
  this->render();
};

void ChartExportDialog::updateChartFontItalic(bool toggled)
{
  int activated = (toggled ? 1 : 0);
  this->mp_Chart->GetTitleProperties()->SetItalic(activated);
  this->mp_Chart->GetAxis(vtkAxis::BOTTOM)->GetTitleProperties()->SetItalic(activated);
  this->mp_Chart->GetAxis(vtkAxis::LEFT)->GetTitleProperties()->SetItalic(activated);
  this->render();
};

void ChartExportDialog::updateAxisXTitle(const QString& title)
{
  this->mp_Chart->GetAxis(vtkAxis::BOTTOM)->SetTitle(title.toUtf8().constData());
  this->render();
};

void ChartExportDialog::updateAxisYTitle(const QString& title)
{
  this->mp_Chart->GetAxis(vtkAxis::LEFT)->SetTitle(title.toUtf8().constData());
  this->render();
};

void ChartExportDialog::updateLegendVisibility(int checked)
{
  this->mp_Chart->SetShowLegend((checked != 0) ? true : false);
  this->render();
};

void ChartExportDialog::updateLegendUnitVisibility(int checked)
{
  for (int i = 0 ; i < this->mp_Chart->GetNumberOfPlots() ; ++i)
  {
    vtkPlot* plot = this->mp_Chart->GetPlot(i);
    vtkStdString label(plot->GetLabel());
    // Add the unit
    if (checked != 0)
    {
      label += this->m_Units[i];
    }
    // Remove the unit
    else
    {
      label = label.substr(0, label.length() - this->m_Units[i].length());
    }
    plot->SetLabel(label);
  }
  this->render();
};

void ChartExportDialog::updateLegendLocation(int index)
{
  const float shift = 5.0f;
  vtkChartLegend* legend = this->mp_Chart->GetLegend();
  switch (index)
  {
  case 0: // Right top
    {
    legend->SetHorizontalAlignment(vtkChartLegend::RIGHT);
    legend->SetVerticalAlignment(vtkChartLegend::TOP);
    float pt2X[2]; this->mp_Chart->GetAxis(vtkAxis::BOTTOM)->GetPoint2(pt2X);
    float pt2Y[2]; this->mp_Chart->GetAxis(vtkAxis::LEFT)->GetPoint2(pt2Y);
    legend->SetPoint(pt2X[0]-shift, pt2Y[1]-shift);
    break;
    }
  case 1: // Right bottom
    {
    float pt2X[2]; this->mp_Chart->GetAxis(vtkAxis::BOTTOM)->GetPoint2(pt2X);
    float pt1Y[2]; this->mp_Chart->GetAxis(vtkAxis::LEFT)->GetPoint1(pt1Y);
    legend->SetHorizontalAlignment(vtkChartLegend::RIGHT);
    legend->SetVerticalAlignment(vtkChartLegend::BOTTOM);
    legend->SetPoint(pt2X[0]-shift, pt1Y[1]+shift);
    break;
    }
  case 2: // Left top
    {
    legend->SetHorizontalAlignment(vtkChartLegend::LEFT);
    legend->SetVerticalAlignment(vtkChartLegend::TOP);
    float pt2Y[2]; this->mp_Chart->GetAxis(vtkAxis::LEFT)->GetPoint2(pt2Y);
    legend->SetPoint(pt2Y[0]+shift, pt2Y[1]-shift);
    break;
    }
  case 3: // Left bottom
    {
    legend->SetHorizontalAlignment(vtkChartLegend::LEFT);
    legend->SetVerticalAlignment(vtkChartLegend::BOTTOM);
    float pt1Y[2]; this->mp_Chart->GetAxis(vtkAxis::LEFT)->GetPoint1(pt1Y);
    legend->SetPoint(pt1Y[0]+shift, pt1Y[1]+shift);
    break;
    }
  case 4: // Center top
    {
    legend->SetHorizontalAlignment(vtkChartLegend::CENTER);
    legend->SetVerticalAlignment(vtkChartLegend::TOP);
    float pt1X[2]; this->mp_Chart->GetAxis(vtkAxis::BOTTOM)->GetPoint1(pt1X);
    float pt2X[2]; this->mp_Chart->GetAxis(vtkAxis::BOTTOM)->GetPoint2(pt2X);
    float pt2Y[2]; this->mp_Chart->GetAxis(vtkAxis::LEFT)->GetPoint2(pt2Y);
    legend->SetPoint((pt1X[0] + pt2X[0]) / 2.0f, pt2Y[1]-shift);
    break;
    }
  case 5: // Center bottom
    {
    legend->SetHorizontalAlignment(vtkChartLegend::CENTER);
    legend->SetVerticalAlignment(vtkChartLegend::BOTTOM);
    float pt1X[2]; this->mp_Chart->GetAxis(vtkAxis::BOTTOM)->GetPoint1(pt1X);
    float pt2X[2]; this->mp_Chart->GetAxis(vtkAxis::BOTTOM)->GetPoint2(pt2X);
    float pt1Y[2]; this->mp_Chart->GetAxis(vtkAxis::LEFT)->GetPoint1(pt1Y);
    legend->SetPoint((pt1X[0] + pt2X[0]) / 2.0f, pt1Y[1]+shift);
    break;
    }
  default:
    qDebug("Unknown location. Set to the default: right top");
    {
    legend->SetHorizontalAlignment(vtkChartLegend::RIGHT);
    legend->SetVerticalAlignment(vtkChartLegend::TOP);
    float pt1[2]; this->mp_Chart->GetAxis(vtkAxis::BOTTOM)->GetPoint2(pt1);
    float pt2[2]; this->mp_Chart->GetAxis(vtkAxis::LEFT)->GetPoint2(pt2);
    legend->SetPoint(pt1[0]-shift, pt2[1]-shift);
    break;
    }
  }
  this->render();
};

void ChartExportDialog::convertWidthUnit(int index)
{
  this->convertUnit(index, this->imageWidth);
};

void ChartExportDialog::convertHeightUnit(int index)
{
  this->convertUnit(index, this->imageHeight);
};

void ChartExportDialog::convertUnit(int index, QDoubleSpinBox* unit)
{
  double scale = 2.54; // inches to centimeters
  if (index == 1)
    scale = 1.0 / scale; // centimeters to inches
  unit->blockSignals(true);
  unit->setValue(unit->value() * scale);
  unit->blockSignals(false);
};

QRectF ChartExportDialog::computeStringBounds(const QString& str, vtkTextProperty* f) const
{
  QFont font(f->GetFontFamilyAsString(), f->GetFontSize());
  font.setBold(f->GetBold());
  font.setItalic(f->GetItalic());
  font.setStyleStrategy(QFont::PreferAntialias);
  QPainterPath path;
  path.addText(0, 0, font, str);
  return path.boundingRect();
}
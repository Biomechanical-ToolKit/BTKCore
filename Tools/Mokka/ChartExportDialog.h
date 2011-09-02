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

#ifndef ChartExportDialog_h
#define ChartExportDialog_h

#include "ui_ChartExportDialog.h"

#include <vtkStdString.h>
#include <vtkstd/vector>

class vtkTextProperty;

class ChartExportDialog : public QDialog, public Ui::ChartExportDialog
{
  Q_OBJECT
  
public:
  ChartExportDialog(QWidget* parent = 0);
  virtual ~ChartExportDialog();
  void setChart(btk::VTKChartTimeSeries* chart);
  
public slots:
  virtual void accept();
  void render();
  void updateChartTile(const QString& title);
  void updateChartFont(int index);
  void updateChartFontSize(int value);
  void updateChartFontBold(bool toggled);
  void updateChartFontItalic(bool toggled);
  void updateAxisXTitle(const QString& title);
  void updateAxisYTitle(const QString& title);
  void updateLegendVisibility(int checked);
  void updateLegendUnitVisibility(int checked);
  void updateLegendLocation(int index);
  
private slots:
  void convertWidthUnit(int);
  void convertHeightUnit(int);
  
private:
  void convertUnit(int index, QDoubleSpinBox* unit);
  void updateChartTile(const QString& title, int value);
  QRectF computeStringBounds(const QString& str, vtkTextProperty* f) const;
  
  btk::VTKChartTimeSeries* mp_Chart;
  vtkstd::vector<vtkStdString> m_Units;
};

#endif
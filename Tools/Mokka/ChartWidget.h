/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2013, Arnaud Barré
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

#ifndef ChartWidget_h
#define ChartWidget_h

#include "VizRendererWidget.h"

#include <btkPoint.h>
#include <btkVTKChartTimeSeries.h> // VTKCurrentFrameFunctor, VTKRegionOfInterestFunctor, VTKEventsFunctor

#include <vtkDoubleArray.h>

class Acquisition;
class ChartOptionsWidget;

class QTreeWidgetItem;
class QLabel;
class QDragEnterEvent;
class QDropEvent;

class vtkTable;
class vtkPlot;
class vtkLine;
class vtkAxis;
class vtkColorSeries;
class vtkContextScene;
class vtkContextView;
class vtkFloatArray;

class VTKChartWidget;
class AbstractChartData;

namespace btk
{
  class VTKChartLayout;
}

struct DataCycleMatchingRules;

class ChartWidget : public QWidget
{
  Q_OBJECT
  
public:
  enum {NoDisplay = -1, TemporalDisplay = 0, CyclicDisplay}; // Used as index for the member AbstractChartData::mp_HorizontalAbscissa
  enum {NoContext = -1, RightContext = 1, LeftContext, GeneralContext};
  
  ChartWidget(QWidget* parent = 0);
  virtual ~ChartWidget();
  
  void initialize();
  void copy(ChartWidget* source);
  bool acceptDroppedTreeWidgetItem(QTreeWidgetItem* item);
  
  void addActions(const QList<QAction*>& actions);
  
  Acquisition* acquisition() {return this->mp_Acquisition;};
  void setAcquisition(Acquisition* acq);
  void setPointHorizontalData(vtkFloatArray* (*horizontalAbscissa)[4], vtkFloatArray* (*cyclesBoundaries)[3]);
  void setAnalogHorizontalData(vtkFloatArray* (*horizontalAbscissa)[4], vtkFloatArray* (*cyclesBoundaries)[3]);
  ChartOptionsWidget* options() {return mp_ChartOptions;};
  void toggleOptions(const QPoint& pos);
  void updateOptions();
  
  void setCurrentFrameFunctor(btk::VTKCurrentFrameFunctor::Pointer functor);
  void setRegionOfInterestFunctor(btk::VTKRegionOfInterestFunctor::Pointer functor);
  void setEventsFunctor(btk::VTKEventsFunctor::Pointer functor);
  void detachFunctors();
  
  double defaultLineWidth() {return ChartWidget::DefaultLineWidth;};
  void setDefaultLineWidth(double width) {ChartWidget::DefaultLineWidth = width;};
  
  void render(bool optionsShown = false, int delayShowOptionWinXP = 75);
  void show(bool s);
  
  void displayPointChart() {this->displayChart(PointChart);};
  void displayAnalogChart() {this->displayChart(AnalogChart);};
  void refreshPlots();
  
  int horizontalDisplayMode() const {return this->m_HorizontalDisplayMode;};
  void updateHorizontalAxis();
  void setHorizontalAxisUnit(const QString& str, double scale, double offset, bool cycleMode = false);
  void setHorizontalAxisRange(double min, double max);
  void setDataToCycleMatchingRules(int rightLabelRule, const QString& rightLabelRuleText, int leftLabelRule, const QString& leftLabelRuleText);
  
  void addPointPlot(btk::Point::Pointer pt, const QString& label, const QString& unit, int horizontalDataIndex, bool storeData = false);
  void setPointVerticalAxisUnit(const QString& strX, const QString& strY, const QString& strZ);
    
  VTKChartWidget* chartContent() const {return this->mp_ChartContentWidget;};
  
  static double DefaultLineWidth;
  
public slots:
  void removePlot(int index);
  void hidePlot(int index, bool isHidden);
  void setPlotLineColor(const QList<int>& indices, const QColor& color);
  void setPlotLineWidth(const QList<int>& indices, double value);
  void setChartTitle(const QString& title);
  void resetAllZooms();
  void resetZoom();
  void exportToImage();
  void removeAllPlot();
  void setEventDisplay(bool visible);
  void toggleEventDisplay();
  // Point specific
  void updatePointPlotLabel(int itemId);
  void discardPointPlots(const QList<int>& itemIds);
  void undiscardPointPlots(const QList<int>& itemIds);
  void displayPointComponentX(int state);
  void displayPointComponentY(int state);
  void displayPointComponentZ(int state);
  // Analog specific
  void updateAnalogPlotLabel(int itemId);
  void updateAnalogPlotsLabel(const QVector<int>& itemIds);
  void discardAnalogPlots(const QList<int>& itemIds);
  void undiscardAnalogPlots(const QList<int>& itemIds);
  void setExpandableAnalog(int expandable);
  
signals:
  void pausePlaybackRequested(bool paused);
  void exportToImageRequested(btk::VTKChartTimeSeries* chart);
  
private slots:
  void setLastContextMenuPosition(const QPoint& globalPos);
  void updateHorizontalAxis(int ff, int lf);
  
protected:
  void dragEnterEvent(QDragEnterEvent *event);
  void dropEvent(QDropEvent* event);
  
private:
  enum {PointChart = 0, AnalogChart}; // Used as index for the member m_ChartData
  
  bool appendPlotFromDroppedItem(QTreeWidgetItem* item);
  void discardPlots(int chartType, const QList<int>& itemIds, bool discarded);
  void checkResetAxes();
  void updatePlotLabel(int chartType, int itemId);
  void displayPointComponent(int idx, int state);
  void displayChart(int chartType);
  void updateHorizontalAxis(btk::VTKChartTimeSeries* chart, int ff, int lf);
  void updateHorizontalAxis(btk::VTKChartTimeSeries* chart, double dlb, double dub, double dlx, double dux);
  int selectContextFromLabel(const QString& label);
  void setDataToCycleMatchingRules(DataCycleMatchingRules* rules);
  void updateHorizontalUnitAxisRange(btk::VTKChartTimeSeries* chart);
  
  int m_CurrentChartType;
  int m_HorizontalDisplayMode;
  QVector<AbstractChartData*> m_ChartData;
  
  QLabel* mp_ChartTitleLabel;
  VTKChartWidget* mp_ChartContentWidget;
  QLabel* mp_ChartAxisXLabel;
  
  Acquisition* mp_Acquisition;
  vtkContextScene* mp_Scene;
  vtkSmartPointer<vtkColorSeries> mp_ColorGenerator;
  ChartOptionsWidget* mp_ChartOptions;
  QList<QAction*> m_ViewActions;
  QPoint m_LastContextMenuPosition;
  
  DataCycleMatchingRules* mp_DataCycleMatchingRules;
};

class AbstractChartData
{
public:
  enum {None = 0, Visible = 1, Discarded = 2, Disabled = 4};
  
  struct PlotProperties
  {
    QString label;
    QString unit;
    QColor color;
    double lineWidth;
    int id;
    int  options;
    bool isVisible() const {return ((this->options & Visible) == Visible);};
    void setVisible(bool v) {if (v) this->options |= Visible; else this->options &= ~Visible;};
    bool isDiscarded() const {return ((this->options & Discarded) == Discarded);};
    void setDiscarded(bool v) {if (v) this->options |= Discarded; else this->options &= ~Discarded;};
    bool isDisabled() const {return ((this->options & Disabled) == Disabled);};
    void setDisabled(bool v) {if (v) this->options |= Disabled; else this->options &= ~Disabled;};
  };
  
  AbstractChartData(int num);
  virtual ~AbstractChartData();
  void appendPlotProperties(const QString& label, const QString& unit, int id, const QColor& color, double lineWidth, bool disabled = false);
  int chartNumber() const;
  btk::VTKChartTimeSeries* chart(int i);
  btk::VTKChartLayout* layout() {return this->mp_ChartLayout;};
  void clear();
  void copy(AbstractChartData* source);
  void generateColor(vtkSmartPointer<vtkColorSeries> colorGenerator, double color[3]);
  virtual void initialize(vtkSmartPointer<vtkColorSeries> colorGenerator);
  bool isAlreadyPlotted(int id) const;
  bool hasCycleBoundaries(int context) const;
  QList<PlotProperties>& plotsProperties() {return this->m_PlotsProperties;};
  const QList<PlotProperties>& plotsProperties() const {return this->m_PlotsProperties;};
  virtual void removePlot(int index, bool* layoutModified);
  virtual void hidePlot(int index, bool isHidden, bool* layoutModified);
  void setHorizontalData(vtkFloatArray* (*horizontalAbscissa)[4], vtkFloatArray* (*cyclesBoundaries)[3]);
  virtual void setPlotVisible(int index, bool show, bool* layoutModified);
  virtual void show(Acquisition* acq, int mode, bool* layoutModified);
  const QString& title() const {return this->m_Title;};
  void setTitle(const QString& title) {this->m_Title = title;};
  const QList<int>& optionSelection() const {return this->m_OptionSelection;};
  void setOptionSelection(const QList<int>& selection) {this->m_OptionSelection = selection;};
  
  void adaptChartDisplay(const QString& titleX, double scaleX, double offsetX, int displayAction, DataCycleMatchingRules* rules);
  int selectContextFromLabel(DataCycleMatchingRules* rules, const QString& label);
  
  virtual bool acceptDroppedTreeWidgetItem(QTreeWidgetItem* item) = 0;
  virtual bool appendPlotFromDroppedItem(Acquisition* acq, vtkSmartPointer<vtkColorSeries> colorGenerator, QTreeWidgetItem* item, DataCycleMatchingRules* rules, bool* layoutModified) = 0;
  virtual void extractPlotLabelUnit(QString& label, QString& unit, Acquisition* acq, int id) = 0;
  
protected:
  QString m_Title;
  btk::VTKChartLayout* mp_ChartLayout;
  vtkFloatArray* (*mp_HorizontalAbscissa)[4];
  vtkFloatArray* (*mp_CyclesBoundaries)[3];
  QList<PlotProperties> m_PlotsProperties;
  QList<int> m_OptionSelection;
};

class VTKChartWidget : public VizRendererWidget
{
  Q_OBJECT
  
public:
  VTKChartWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);
  btk::VTKChartTimeSeries* focusedChart(const QPoint& pos) const;
  btk::VTKChartTimeSeries* focusedPlotArea(const QPoint& pos) const;
  void setChartData(AbstractChartData* data) {this->mp_CurrentChartData = data;};
  
signals:
  void contextMenuRequested(const QPoint& pos);
  
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
  void computeRatio(float ratio[2], int screenPos[2], btk::VTKChartTimeSeries* chart);
  void applyRatio(float pos[2], float ratio[2], btk::VTKChartTimeSeries* chart);
  
  AbstractChartData* mp_CurrentChartData;
  QPoint m_OldMousePosition;
  bool m_AlternateMouseEvent;
};

// -----------------------------------------------------------------------------

inline bool ChartWidget::acceptDroppedTreeWidgetItem(QTreeWidgetItem* item)
{
  return this->m_ChartData[this->m_CurrentChartType]->acceptDroppedTreeWidgetItem(item);
};

inline void ChartWidget::addActions(const QList<QAction*>& actions)
{
  this->mp_ChartContentWidget->addActions(actions);
};

inline void ChartWidget::setPointHorizontalData(vtkFloatArray* (*horizontalAbscissa)[4], vtkFloatArray* (*cyclesBoundaries)[3]) 
{
  this->m_ChartData[PointChart]->setHorizontalData(horizontalAbscissa, cyclesBoundaries);
};

inline void ChartWidget::setAnalogHorizontalData(vtkFloatArray* (*horizontalAbscissa)[4], vtkFloatArray* (*cyclesBoundaries)[3])
{
  this->m_ChartData[AnalogChart]->setHorizontalData(horizontalAbscissa, cyclesBoundaries);
};

#endif // ChartWidget_h
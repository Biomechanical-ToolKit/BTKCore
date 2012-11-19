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

#include "ChartWidget.h"
#include "Acquisition.h"
#include "ChartOptionsWidget.h"
#include "ChartExportDialog.h"
#include "UserDefined.h"

#include <btkVTKAxis.h>
#include <btkVTKContextActor.h>
#include <btkVTKChartLayout.h>
#include <btkVTKPlotCycle.h>

#include <vtkRenderer.h>
#include <vtkContextScene.h>
#include <vtkColorSeries.h>
#include <vtkPen.h>
#include <vtkPlotLine.h>
#include <vtkTable.h>
#include <vtkAxis.h>
#include <vtkContext2D.h>
#include <vtkContextDevice2D.h>
#include <vtkOpenGLContextDevice2D.h>
#include <vtkFloatArray.h>
#include <vtkContextInteractorStyle.h>
#include <vtkContextMouseEvent.h>
#include <vtkRenderWindowInteractor.h>

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QTreeWidget>
#include <QToolTip>
#include <QTimer>

#ifdef Q_OS_WIN
  #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
  #endif
  #include <windows.h>
#endif

double ChartWidget::DefaultLineWidth = 1.0;

struct DataCycleMatchingRules
{
  int rightLabelRule;
  QString rightLabelRuleText;
  int leftLabelRule;
  QString leftLabelRuleText;
};

class PointChartData : public AbstractChartData
{
public:
  PointChartData();
  virtual bool acceptDroppedTreeWidgetItem(QTreeWidgetItem* item);
  virtual bool appendPlotFromDroppedItem(Acquisition* acq, vtkSmartPointer<vtkColorSeries> colorGenerator, QTreeWidgetItem* item, DataCycleMatchingRules* rules, bool* layoutModified);
  virtual void extractPlotLabelUnit(QString& label, QString& unit, Acquisition* acq, int id);
  virtual void initialize(vtkSmartPointer<vtkColorSeries> colorGenerator);
  void addPointPlot(int id, Acquisition* acq, vtkColorSeries* colorGenerator, btk::Point::Pointer point, const QString& label, const QString& unit, int horizontalDataIndex);
};

class AnalogChartData : public AbstractChartData
{
public:
  AnalogChartData();
  virtual bool acceptDroppedTreeWidgetItem(QTreeWidgetItem* item);
  virtual bool appendPlotFromDroppedItem(Acquisition* acq, vtkSmartPointer<vtkColorSeries> colorGenerator, QTreeWidgetItem* item, DataCycleMatchingRules* rules, bool* layoutModified);
  virtual void extractPlotLabelUnit(QString& label, QString& unit, Acquisition* acq, int id);
  virtual void initialize(vtkSmartPointer<vtkColorSeries> colorGenerator);
  virtual void removePlot(int index, bool* layoutModified);
  virtual void hidePlot(int index, bool isHidden, bool* layoutModified);
  virtual void setPlotVisible(int index, bool show, bool* layoutModified);
  virtual void show(Acquisition* acq, bool s, bool* layoutModified);
  void setExpandable(bool expandable);
  bool isExpanded() const {return this->m_Expanded;};

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
  this->m_HorizontalDisplayMode = 0; // Temporal display (frame)
  this->mp_Acquisition = 0;
  this->mp_DataCycleMatchingRules = 0;
  
  this->mp_ColorGenerator = vtkSmartPointer<vtkColorSeries>::New();
  this->mp_ColorGenerator->RemoveColor(0); // The first one is the color black.
  this->mp_ChartOptions = new ChartOptionsWidget(this);
  this->mp_ChartOptions->setVisible(false);
  this->mp_ChartOptions->clear();
  
  this->setFocusPolicy(Qt::StrongFocus);
  
  QAction* resetAllZoomsAction = new QAction(tr("Reset All Zooms"), this); this->m_ViewActions.push_back(resetAllZoomsAction);
  QAction* separator = new QAction(this); separator->setSeparator(true); this->m_ViewActions.push_back(separator);
  QAction* resetZoomAction = new QAction(tr("Reset Zoom"), this); this->m_ViewActions.push_back(resetZoomAction);
  QAction* exportToAction = new QAction(tr("Export to Image"), this); this->m_ViewActions.push_back(exportToAction);
  QAction* separator2 = new QAction(this); separator2->setSeparator(true); this->m_ViewActions.push_back(separator2);
  QAction* toggleEventDisplayAction = new QAction(tr("Toggle Events Display"), this); this->m_ViewActions.push_back(toggleEventDisplayAction);
  QAction* removeAllPlotAction = new QAction(tr("Clear Chart"), this); this->m_ViewActions.push_back(removeAllPlotAction);
  
  this->mp_Scene = 0;
  this->mp_ChartContentWidget = new VTKChartWidget(this);
  this->mp_ChartContentWidget->addActions(this->m_ViewActions);
  this->mp_ChartContentWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
  this->mp_ChartContentWidget->setMouseTracking(false); // No need to send mouse events to VTK when a mouse button isn't down
  this->mp_ChartContentWidget->GetRenderWindow()->SetMultiSamples(0); // For anti-aliasing (smooth plot)
  this->m_ChartData[PointChart] = new PointChartData();
  this->m_ChartData[AnalogChart] = new AnalogChartData();
  
  connect(this->mp_ChartContentWidget, SIGNAL(contextMenuRequested(QPoint)), this, SLOT(setLastContextMenuPosition(QPoint)));
  connect(this->mp_ChartOptions, SIGNAL(pausePlaybackRequested(bool)), this, SIGNAL(pausePlaybackRequested(bool)));
  
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
  layout->addWidget(this->mp_ChartContentWidget,1); // 1: strech the charts
  layout->addWidget(this->mp_ChartAxisXLabel);
  layout->setContentsMargins(0,5,0,5);
  layout->setSpacing(0);
  
  connect(this->mp_ChartOptions, SIGNAL(lineColorChanged(QList<int>, QColor)), this, SLOT(setPlotLineColor(QList<int>, QColor)));
  connect(this->mp_ChartOptions, SIGNAL(lineWidthChanged(QList<int>, double)), this, SLOT(setPlotLineWidth(QList<int>, double)));
  connect(this->mp_ChartOptions, SIGNAL(plotRemoved(int)), this, SLOT(removePlot(int)));
  connect(this->mp_ChartOptions, SIGNAL(plotHidden(int, bool)), this, SLOT(hidePlot(int, bool)));
  connect(this->mp_ChartOptions, SIGNAL(chartTitleChanged(QString)), this, SLOT(setChartTitle(QString)));
  connect(resetAllZoomsAction, SIGNAL(triggered()), this, SLOT(resetAllZooms()));
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
  if (this->mp_DataCycleMatchingRules != 0)
    delete this->mp_DataCycleMatchingRules;
};

void ChartWidget::initialize()
{
  for (int i = 0 ; i < this->m_ChartData.size() ; ++i)
    this->m_ChartData[i]->initialize(this->mp_ColorGenerator);
  
  // Set up the view
  vtkRenderer* ren = vtkRenderer::New();
  ren->SetBackground(1.0,1.0,1.0);
  vtkRenderWindow* renwin = this->mp_ChartContentWidget->GetRenderWindow();
  renwin->AddRenderer(ren);
  ren->Delete();

  btk::VTKContextActor* actor = btk::VTKContextActor::New();
  ren->AddActor(actor);
  actor->Delete();
  this->mp_Scene = actor->GetScene(); // We keep a pointer to this for convenience
  // this->mp_Scene->SetRenderer(ren); // FROM VTK: Should not need to do this... 
  
  vtkContextInteractorStyle* style = vtkContextInteractorStyle::New();
  style->SetScene(this->mp_Scene);
  renwin->GetInteractor()->SetInteractorStyle(style);
  style->Delete();
  
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
  
  this->mp_ChartAxisXLabel->setText(source->mp_ChartAxisXLabel->text());
};

void ChartWidget::setAcquisition(Acquisition* acq)
{
  if (this->mp_Acquisition == acq)
    return;
  if (this->mp_Acquisition != 0)
    disconnect(this->mp_Acquisition, 0, this, 0);
  
  this->mp_Acquisition = acq;
  // General
  connect(this->mp_Acquisition, SIGNAL(regionOfInterestChanged(int, int)), this, SLOT(updateHorizontalAxis(int,int)));
  // Point
  connect(this->mp_Acquisition, SIGNAL(pointLabelChanged(int, QString)), this, SLOT(updatePointPlotLabel(int)));
  connect(this->mp_Acquisition, SIGNAL(pointsRemoved(QList<int>, QList<Point*>)), this, SLOT(discardPointPlots(QList<int>)));
  connect(this->mp_Acquisition, SIGNAL(pointsInserted(QList<int>, QList<Point*>)), this, SLOT(undiscardPointPlots(QList<int>)));
  // Analog
  connect(this->mp_Acquisition, SIGNAL(analogLabelChanged(int, QString)), this, SLOT(updateAnalogPlotLabel(int)));
  connect(this->mp_Acquisition, SIGNAL(analogsUnitChanged(QVector<int>, QVector<QString>)), this, SLOT(updateAnalogPlotsLabel(QVector<int>)));
  connect(this->mp_Acquisition, SIGNAL(analogsRemoved(QList<int>, QList<Analog*>)), this, SLOT(discardAnalogPlots(QList<int>)));
  connect(this->mp_Acquisition, SIGNAL(analogsInserted(QList<int>, QList<Analog*>)), this, SLOT(undiscardAnalogPlots(QList<int>)));
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
      this->m_ChartData[i]->layout()->UpdateLayout();
  }
};

void ChartWidget::refreshPlots()
{
  for (int i = 0 ; i < this->m_ChartData.size() ; ++i)
  {
    for (int j = 0 ; j < static_cast<int>(this->m_ChartData[i]->chartNumber()) ; ++j)
    {
      btk::VTKChartTimeSeries* chart = static_cast<btk::VTKChartTimeSeries*>(this->m_ChartData[i]->chart(j));
      for (int k = 0 ; k < chart->GetNumberOfPlots() ; ++k)
        chart->GetPlot(k)->Modified();
      double rangeX[2] = {0}, rangeY[2] = {0};
      btk::VTKAxis* axisX = btk::VTKAxis::SafeDownCast(chart->GetAxis(vtkAxis::BOTTOM));
      int xLimits[2] = {0};
      btk::VTKAxis* axisY = btk::VTKAxis::SafeDownCast(chart->GetAxis(vtkAxis::LEFT));
      xLimits[0] = axisX->GetMinimumLimit();
      xLimits[1] = axisX->GetMaximumLimit();
      axisX->GetRange(rangeX);
      axisY->GetRange(rangeY);
      chart->Update();
      chart->RecalculateBounds();
      axisX->SetMinimumLimit(xLimits[0]);
      axisX->SetMaximumLimit(xLimits[1]);
      axisX->SetRange(rangeX);
      if (rangeY[0] > rangeY[1])
        axisY->SetRange(rangeY);
    }
  }
};

void ChartWidget::updateHorizontalAxis()
{
  if (this->m_HorizontalDisplayMode == CyclicDisplay)
    return;
  
  for (int i = 0 ; i < this->m_ChartData.size() ; ++i)
  {
    for (int j = 0 ; j < static_cast<int>(this->m_ChartData[i]->chartNumber()) ; ++j)
    {
      btk::VTKChartTimeSeries* chart = static_cast<btk::VTKChartTimeSeries*>(this->m_ChartData[i]->chart(j));
      int roi[2]; this->mp_Acquisition->regionOfInterest(roi[0], roi[1]);
      double diff = (double)roi[0] - chart->GetAxis(vtkAxis::BOTTOM)->GetMinimumLimit();
      this->updateHorizontalAxis(chart, diff, diff, diff, diff);
    }
  }
};

void ChartWidget::updateHorizontalAxis(int ff, int lf)
{
  for (int i = 0 ; i < this->m_ChartData.size() ; ++i)
  {
    for (int j = 0 ; j < static_cast<int>(this->m_ChartData[i]->chartNumber()) ; ++j)
    {
      btk::VTKChartTimeSeries* chart = static_cast<btk::VTKChartTimeSeries*>(this->m_ChartData[i]->chart(j));
      this->updateHorizontalAxis(chart, ff, lf);
    }
  }
};

void ChartWidget::updateHorizontalAxis(btk::VTKChartTimeSeries* chart, int ff, int lf)
{
  vtkAxis* axisX = chart->GetAxis(vtkAxis::BOTTOM);
  double dlb = (double)ff - axisX->GetMinimumLimit();
  double dub = (double)lf - axisX->GetMaximumLimit();
  double dlx = (double)ff - axisX->GetMinimum();
  double dux = (double)lf - axisX->GetMaximum();
  this->updateHorizontalAxis(chart, dlb, dub, dlx, dux);
};

void ChartWidget::updateHorizontalAxis(btk::VTKChartTimeSeries* chart, double dlb, double dub, double dlx, double dux)
{
  Q_UNUSED(dlx);
  Q_UNUSED(dux);
  vtkAxis* axisX = chart->GetAxis(vtkAxis::BOTTOM);
  vtkAxis* axisY = chart->GetAxis(vtkAxis::LEFT);
  chart->SetBounds(axisX->GetMinimumLimit() + dlb, axisX->GetMaximumLimit() + dub, axisY->GetMinimumLimit(), axisY->GetMaximumLimit());
  for (int k = 0 ; k < chart->GetNumberOfPlots() ; ++k)
    chart->GetPlot(k)->Modified(); // To update the frames index
};

void ChartWidget::setHorizontalAxisUnit(const QString& str, double scale, double offset, bool cycleMode)
{
  #warning Check if the serie's name is updated when the data's label is modified
  if (cycleMode && (this->mp_DataCycleMatchingRules == 0))
  {
    qDebug("Impossible to display data by cycle as no rule was set to match data with cycle.");
    return;
  }
  
  int displayAction = 0; // Do nothing
  if ((this->m_HorizontalDisplayMode != CyclicDisplay) && cycleMode)
    displayAction = 1; // Go to cyclic display
  else if ((this->m_HorizontalDisplayMode == CyclicDisplay) && !cycleMode)
    displayAction = -1; // Go back to temporal display
  
  for (int i = 0 ; i < this->m_ChartData.size() ; ++i)
    this->m_ChartData[i]->adaptChartDisplay(str, scale, offset, displayAction, this->mp_DataCycleMatchingRules);
    
  if (displayAction != 0)
    this->updateOptions();
  
  this->mp_ChartAxisXLabel->setText(str);
  this->m_HorizontalDisplayMode = cycleMode ? CyclicDisplay : TemporalDisplay;
  this->render();
};

void ChartWidget::setHorizontalAxisRange(double min, double max)
{
  for (int i = 0 ; i < this->m_ChartData.size() ; ++i)
  {
    for (int j = 0 ; j < static_cast<int>(this->m_ChartData[i]->chartNumber()) ; ++j)
    {
      /*
      btk::VTKAxis* axisX = static_cast<btk::VTKAxis*>(this->m_ChartData[i]->chart(j)->GetAxis(vtkAxis::BOTTOM));
      axisX->SetMinimumLimit(min);
      axisX->SetMaximumLimit(max);
      axisX->SetRange(min, max);
      */
      btk::VTKChartTimeSeries* chart = static_cast<btk::VTKChartTimeSeries*>(this->m_ChartData[i]->chart(j));
      double rangeY[2] = {0};
      btk::VTKAxis* axisY = btk::VTKAxis::SafeDownCast(chart->GetAxis(vtkAxis::LEFT));
      axisY->GetRange(rangeY);
      chart->SetBounds(min, max, axisY->GetMinimumLimit(), axisY->GetMaximumLimit());
      axisY->SetRange(rangeY);
    }
  }
};

void ChartWidget::setDataToCycleMatchingRules(int rightLabelRule, const QString& rightLabelRuleText, int leftLabelRule, const QString& leftLabelRuleText)
{
  if (this->mp_DataCycleMatchingRules != 0)
    delete this->mp_DataCycleMatchingRules;
  this->mp_DataCycleMatchingRules = new DataCycleMatchingRules;
  this->mp_DataCycleMatchingRules->rightLabelRule = rightLabelRule;
  this->mp_DataCycleMatchingRules->rightLabelRuleText = rightLabelRuleText;
  this->mp_DataCycleMatchingRules->leftLabelRule = leftLabelRule;
  this->mp_DataCycleMatchingRules->leftLabelRuleText = leftLabelRuleText;
};


void ChartWidget::addPointPlot(btk::Point::Pointer pt, const QString& label, const QString& unit, int horizontalDataIndex)
{
  static_cast<PointChartData*>(this->m_ChartData[PointChart])->addPointPlot(-1, this->mp_Acquisition, this->mp_ColorGenerator, pt, label, unit, horizontalDataIndex);
  for (int i = 0 ; i < static_cast<int>(this->m_ChartData[this->m_CurrentChartType]->chartNumber()) ; ++i)
  {
    btk::VTKChartTimeSeries* chart = this->m_ChartData[this->m_CurrentChartType]->chart(i);
    chart->Update();
    chart->SetInteractionEnabled(true);
    chart->RecalculateBounds();
  }
};

void ChartWidget::setPointVerticalAxisUnit(const QString& strX, const QString& strY, const QString& strZ)
{
  PointChartData* pcd = static_cast<PointChartData*>(this->m_ChartData[PointChart]);
  pcd->chart(0)->GetAxis(vtkAxis::LEFT)->SetTitle(strX.toUtf8().constData());
  pcd->chart(1)->GetAxis(vtkAxis::LEFT)->SetTitle(strY.toUtf8().constData());
  pcd->chart(2)->GetAxis(vtkAxis::LEFT)->SetTitle(strZ.toUtf8().constData());
};

void ChartWidget::displayChart(int chartType)
{
  // Save the current selection in the options
  if (this->m_CurrentChartType != -1)
    this->m_ChartData[this->m_CurrentChartType]->setOptionSelection(this->mp_ChartOptions->selectedPlots());
  
  this->mp_Scene->ClearItems();
  this->mp_Scene->AddItem(this->m_ChartData[chartType]->layout());
  this->mp_ChartContentWidget->setChartData(this->m_ChartData[chartType]);
  // this->m_ChartData[chartType]->layout()->UpdateLayout();
  this->m_CurrentChartType = chartType;
  
  this->setChartTitle(this->m_ChartData[chartType]->title());
  this->mp_ChartOptions->plotTable->clearSelection();
  this->mp_ChartOptions->chartTitleLineEdit->setText(this->m_ChartData[chartType]->title());
  this->updateOptions();
  this->mp_ChartOptions->setSelectedPlots(this->m_ChartData[chartType]->optionSelection());
  this->mp_ChartOptions->setFocus(); // Remove the focus of its children.
};

void ChartWidget::render(bool optionsShown, int delayShowOptionWinXP)
{
  if (optionsShown)
  {
#ifdef Q_OS_WIN
    // Fix for Windows XP (and vista?) which doesn't redraw correctly the options.
    // The side effect is a possible blinking of the options but it's better than to see nothing.
    if (QSysInfo::windowsVersion() < QSysInfo::WV_WINDOWS7)
    {
      this->mp_ChartOptions->m_FixUpdateWindowsXP = true;
      this->mp_ChartOptions->hide();
      this->render();
      QTimer::singleShot(delayShowOptionWinXP, this->mp_ChartOptions, SLOT(show()));
      this->mp_ChartOptions->m_FixUpdateWindowsXP = false;
    }
    else
      this->render();
#else
    Q_UNUSED(delayShowOptionWinXP);
    this->render();
#endif
  }
  else if (this->mp_ChartContentWidget->isVisible())
  {
    // FIXME: A simple ChartWidget::update() is enough under MacOS X but not Windows XP...
    //        The use of the method vtkRenderWindow:::Render could slowdown the display...
    //        Should we need a special case for Windows XP? Same for Windows 7?
    this->mp_ChartContentWidget->GetRenderWindow()->Render();
  }
};

void ChartWidget::removePlot(int index)
{
  bool layoutModified;
  this->m_ChartData[this->m_CurrentChartType]->removePlot(index, &layoutModified);
  if (layoutModified)
    this->m_ChartData[this->m_CurrentChartType]->layout()->UpdateLayout();
  this->checkResetAxes(); // If no more plot or all of them are hidden, then the axes are reset.
  this->render(true); // Options are shown
};

void ChartWidget::hidePlot(int index, bool isHidden)
{
  bool layoutModified;
  this->m_ChartData[this->m_CurrentChartType]->hidePlot(index, isHidden, &layoutModified);
  if (layoutModified)
    this->m_ChartData[this->m_CurrentChartType]->layout()->UpdateLayout();
  this->checkResetAxes(); // If no more plot or all of them are hidden, then the axes are reset.
  this->render(true); // Options are shown
};

void ChartWidget::setPlotLineColor(const QList<int>& indices, const QColor& color)
{
  if ((this->m_CurrentChartType == AnalogChart) && static_cast<AnalogChartData*>(this->m_ChartData[this->m_CurrentChartType])->isExpanded())
  {
    for (QList<int>::const_iterator it = indices.begin() ; it != indices.end() ; ++it)
    {
      vtkPlot* plot = this->m_ChartData[this->m_CurrentChartType]->chart(*it)->GetPlot(0);
      plot->SetColor(color.redF(), color.greenF(), color.blueF());
      this->m_ChartData[this->m_CurrentChartType]->plotsProperties()[*it].color = color;
    }
  }
  else
  {
    for (QList<int>::const_iterator it = indices.begin() ; it != indices.end() ; ++it)
    {
      for (int i = 0 ; i < static_cast<int>(this->m_ChartData[this->m_CurrentChartType]->chartNumber()) ; ++i)
      {
        vtkPlot* plot = this->m_ChartData[this->m_CurrentChartType]->chart(i)->GetPlot(*it);
        plot->SetColor(color.redF(), color.greenF(), color.blueF());
      }
      this->m_ChartData[this->m_CurrentChartType]->plotsProperties()[*it].color = color;
    }
  }
  this->render(true); // Options are shown
};

void ChartWidget::setPlotLineWidth(const QList<int>& indices, double value)
{
  if ((this->m_CurrentChartType == AnalogChart) && static_cast<AnalogChartData*>(this->m_ChartData[this->m_CurrentChartType])->isExpanded())
  {
    for (QList<int>::const_iterator it = indices.begin() ; it != indices.end() ; ++it)
    {
      vtkPlot* plot = this->m_ChartData[this->m_CurrentChartType]->chart(*it)->GetPlot(0);
      plot->SetWidth(static_cast<float>(value));
      this->m_ChartData[this->m_CurrentChartType]->plotsProperties()[*it].lineWidth = value;
    }
  }
  else
  {
    for (QList<int>::const_iterator it = indices.begin() ; it != indices.end() ; ++it)
    {
      for (int i = 0 ; i < static_cast<int>(this->m_ChartData[this->m_CurrentChartType]->chartNumber()) ; ++i)
      {
        vtkPlot* plot = this->m_ChartData[this->m_CurrentChartType]->chart(i)->GetPlot(*it);
        plot->SetWidth(static_cast<float>(value));
      }
      this->m_ChartData[this->m_CurrentChartType]->plotsProperties()[*it].lineWidth = value;
    }
  }
  this->render(true,20); // Options are shown
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
    this->render(true,50);
  }
  else
    this->update();
#else
  this->update();
#endif
};

void ChartWidget::resetAllZooms()
{
  for (int i = 0 ; i < static_cast<int>(this->m_ChartData[this->m_CurrentChartType]->chartNumber()) ; ++i)
  {
    btk::VTKChartTimeSeries* chart = this->m_ChartData[this->m_CurrentChartType]->chart(i);
    vtkAxis* axisX = chart->GetAxis(vtkAxis::BOTTOM);
    vtkAxis* axisY = chart->GetAxis(vtkAxis::LEFT);
    chart->SetBounds(axisX->GetMinimumLimit(), axisX->GetMaximumLimit(),axisY->GetMinimumLimit(), axisY->GetMaximumLimit()); // Force the update of the layout and the plot transform
  }
  this->render();
};

void ChartWidget::resetZoom()
{
  btk::VTKChartTimeSeries* chart = this->mp_ChartContentWidget->focusedChart(this->m_LastContextMenuPosition);
  if (chart != 0)
  {
    vtkAxis* axisX = chart->GetAxis(vtkAxis::BOTTOM);
    vtkAxis* axisY = chart->GetAxis(vtkAxis::LEFT);
    chart->SetBounds(axisX->GetMinimumLimit(), axisX->GetMaximumLimit(),axisY->GetMinimumLimit(), axisY->GetMaximumLimit()); // Force the update of the layout and the plot transform
    this->render();
  }
};

void ChartWidget::exportToImage()
{
  btk::VTKChartTimeSeries* chart = this->mp_ChartContentWidget->focusedChart(this->m_LastContextMenuPosition);
  if (chart != 0)
    emit exportToImageRequested(chart);
};
void ChartWidget::removeAllPlot()
{
  this->mp_ChartOptions->clear();
  bool layoutModified = false;
  this->m_ChartData[this->m_CurrentChartType]->show(this->mp_Acquisition, true, &layoutModified); // Easy way to reset the chart.
  if (layoutModified)
    this->m_ChartData[this->m_CurrentChartType]->layout()->UpdateLayout();
  this->render();
};

// Show/hide event for every (sub) chart.
void ChartWidget::setEventDisplay(bool visible)
{
  for (int j = 0 ; j < 2 ; ++j)
  {
    for (int i = 0 ; i < static_cast<int>(this->m_ChartData[j]->chartNumber()) ; ++i)
    {
      btk::VTKChartTimeSeries* chart = this->m_ChartData[j]->chart(i);
      chart->SetDisplayEvents(visible ? 1 : 0);
    }
  }
  this->render();
};

void ChartWidget::toggleEventDisplay()
{
  for (int i = 0 ; i < static_cast<int>(this->m_ChartData[this->m_CurrentChartType]->chartNumber()) ; ++i)
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

void ChartWidget::discardPointPlots(const QList<int>& itemIds)
{
  this->discardPlots(PointChart, itemIds, true);
};

void ChartWidget::undiscardPointPlots(const QList<int>& itemIds)
{
  this->discardPlots(PointChart, itemIds, false);
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

void ChartWidget::discardAnalogPlots(const QList<int>& itemIds)
{
  this->discardPlots(AnalogChart, itemIds, true);
};

void ChartWidget::undiscardAnalogPlots(const QList<int>& itemIds)
{
  this->discardPlots(AnalogChart, itemIds, false);
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
  this->m_ChartData[this->m_CurrentChartType]->layout()->UpdateLayout();
  for (int i = 0 ; i < static_cast<int>(analogChartData->chartNumber()) ; ++i)
  {
    btk::VTKChartTimeSeries* chart = analogChartData->chart(i);
    chart->RecalculateBounds();
    int roi[2]; this->mp_Acquisition->regionOfInterest(roi[0], roi[1]);
    this->updateHorizontalAxis(chart, roi[0], roi[1]);
  }
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
    for (int j = 0 ; j < static_cast<int>(this->m_ChartData[i]->chartNumber()) ; ++j)
      this->m_ChartData[i]->chart(j)->SetCurrentFrameFunctor(functor);
  }
};

void ChartWidget::setRegionOfInterestFunctor(btk::VTKRegionOfInterestFunctor::Pointer functor)
{
  for (int i = 0 ; i < this->m_ChartData.size() ; ++i)
  {
    for (int j = 0 ; j < static_cast<int>(this->m_ChartData[i]->chartNumber()) ; ++j)
      this->m_ChartData[i]->chart(j)->SetRegionOfInterestFunctor(functor);
  }
};

void ChartWidget::setEventsFunctor(btk::VTKEventsFunctor::Pointer functor)
{
  for (int i = 0 ; i < this->m_ChartData.size() ; ++i)
  {
    for (int j = 0 ; j < static_cast<int>(this->m_ChartData[i]->chartNumber()) ; ++j)
      this->m_ChartData[i]->chart(j)->SetEventsFunctor(functor);
  }
};

void ChartWidget::toggleOptions(const QPoint& pos)
{
  if (this->mp_ChartOptions->isVisible())
    this->mp_ChartOptions->setVisible(false);
  else
  {
#ifdef Q_OS_WIN
    // Fix for Windows XP (and vista?) which doesn't redraw correctly the options.
    if (QSysInfo::windowsVersion() < QSysInfo::WV_WINDOWS7)
    {
      emit pausePlaybackRequested(true);
      QApplication::processEvents();
      ::Sleep(20);
    }
#endif
    this->mp_ChartOptions->move(pos - QPoint(this->mp_ChartOptions->width() / 2, 0));
    this->mp_ChartOptions->setVisible(true);
  }
};

void ChartWidget::updateOptions()
{
  if (this->m_CurrentChartType == -1)
    return;
  
  QList<AbstractChartData::PlotProperties>* props = &(this->m_ChartData[this->m_CurrentChartType]->plotsProperties());
  this->mp_ChartOptions->plotTable->setRowCount(props->count());
  for (int i = 0 ; i < props->size(); ++i)
  {
    AbstractChartData::PlotProperties* prop = &(props->operator[](i));
    this->mp_ChartOptions->setPlot(i, prop->label, prop->color, prop->lineWidth, 
                                      (prop->options & AbstractChartData::Visible) == AbstractChartData::Visible, 
                                      (prop->options & AbstractChartData::Discarded) == AbstractChartData::Discarded,
                                      (prop->options & AbstractChartData::Disabled) == AbstractChartData::Disabled);
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
    for (int i = 0 ; i < static_cast<int>(this->m_ChartData[this->m_CurrentChartType]->chartNumber()) ; ++i)
    {
      btk::VTKChartTimeSeries* chart = this->m_ChartData[this->m_CurrentChartType]->chart(i);
      chart->SetInteractionEnabled(true);
      chart->Update(); // Force the update to compute correctly the boundaries
      chart->RecalculateBounds();
      // Force the limit of the chart to be sure that for the analog chart the last frame is the last video frame and not the last analog frame
      if (this->m_HorizontalDisplayMode == TemporalDisplay)
      {
        int roi[2]; this->mp_Acquisition->regionOfInterest(roi[0], roi[1]);
        this->updateHorizontalAxis(chart, roi[0], roi[1]);
      }
      else
      {
        btk::VTKAxis* axisX = static_cast<btk::VTKAxis*>(chart->GetAxis(vtkAxis::BOTTOM));
        axisX->SetMinimumLimit(0.0);
        axisX->SetMaximumLimit(100.0);
        axisX->SetRange(0.0, 100.0);
      }
    }
  }
  this->render();
};

bool ChartWidget::appendPlotFromDroppedItem(QTreeWidgetItem* item)
{
  bool chartLayoutChanged = false;
  bool append = false;
  if (this->m_HorizontalDisplayMode == TemporalDisplay)
    append = this->m_ChartData[this->m_CurrentChartType]->appendPlotFromDroppedItem(this->mp_Acquisition, this->mp_ColorGenerator, item, NULL, &chartLayoutChanged);
  else
    append = this->m_ChartData[this->m_CurrentChartType]->appendPlotFromDroppedItem(this->mp_Acquisition, this->mp_ColorGenerator, item, this->mp_DataCycleMatchingRules, &chartLayoutChanged);
  if (chartLayoutChanged)
    this->m_ChartData[this->m_CurrentChartType]->layout()->UpdateLayout();
  return append;
};


void ChartWidget::discardPlots(int chartType, const QList<int>& itemIds, bool discarded)
{
  QList<AbstractChartData::PlotProperties>::iterator itProp = this->m_ChartData[chartType]->plotsProperties().begin();
  QList<int>::const_iterator itId = itemIds.begin();
  int index = 0;
  bool regenerateChartsLayout = false;
  while (itProp != this->m_ChartData[chartType]->plotsProperties().end())
  {
    bool found = false;
    if (itProp->id == *itId)
    {
      bool layoutModified = false;
      this->m_ChartData[chartType]->setPlotVisible(index, (discarded ? false : itProp->isVisible()), &layoutModified);
      itProp->setDiscarded(discarded);
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
    {
      this->m_ChartData[this->m_CurrentChartType]->layout()->UpdateLayout();
    }
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
    if (it->isVisible())
    {
      plotVisible = true;
      break;
    }
  }
  // int roi[2]; this->mp_Acquisition->regionOfInterest(roi[0], roi[1]);
  for (int i = 0 ; i < static_cast<int>(this->m_ChartData[this->m_CurrentChartType]->chartNumber()) ; ++i)
  {
    btk::VTKChartTimeSeries* chart = this->m_ChartData[this->m_CurrentChartType]->chart(i);
    chart->SetInteractionEnabled(plotVisible);
    // Force the chart to draw ticks for the X axis.
    if (!plotVisible)
    {
      if (this->m_HorizontalDisplayMode == TemporalDisplay)
        chart->SetBounds((double)this->mp_Acquisition->firstFrame(), (double)this->mp_Acquisition->lastFrame(), 0.0, 0.0);
      else
        chart->SetBounds(0.0, 100.0, 0.0, 0.0);
    }
    /*
    else if (this->m_HorizontalDisplayMode == TemporalDisplay)
      this->updateHorizontalAxis(chart, roi[0], roi[1]);
    */
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
      this->m_ChartData[chartType]->extractPlotLabelUnit(it->label, it->unit, this->mp_Acquisition, itemId);
      break;
    }
    ++index;
  }
  if (it != this->m_ChartData[chartType]->plotsProperties().end())
  {
    for (int i = 0 ; i < static_cast<int>(this->m_ChartData[chartType]->chartNumber()) ; ++i)
    {
      vtkPlot* plot = this->m_ChartData[chartType]->chart(i)->GetPlot(index);
      plot->SetLabel((it->label + it->unit).toUtf8().constData());
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
  for (int i = 0 ; i < static_cast<int>(this->m_ChartData[this->m_CurrentChartType]->chartNumber()) ; ++i)
    axisXTitleVisible |= this->m_ChartData[this->m_CurrentChartType]->chart(i)->GetVisible();
  this->mp_ChartAxisXLabel->setVisible(axisXTitleVisible);
  this->m_ChartData[this->m_CurrentChartType]->layout()->UpdateLayout();
  this->render();
};

// -----------------------------------------------------------------------------

AbstractChartData::AbstractChartData(int num)
: m_Title(), m_PlotsProperties(), m_OptionSelection()
{
  this->mp_ChartLayout = btk::VTKChartLayout::New();
  this->mp_ChartLayout->SetSize(vtkVector2i(1,num));
  this->mp_ChartLayout->SetBorders(0,6,20,0);
  this->mp_ChartLayout->SetGutter(vtkVector2f(0.0f,0.0f));
  this->mp_HorizontalAbscissa = NULL;
  this->mp_CyclesBoundaries = NULL;
};

AbstractChartData::~AbstractChartData()
{
  this->mp_ChartLayout->Delete();
  
  if (this->mp_HorizontalAbscissa != NULL)
  {
    for (int i = 0 ; i < 4 ; ++i)
      (*this->mp_HorizontalAbscissa)[i]->Delete();
  }
  
  if (this->mp_CyclesBoundaries != NULL)
  {
    for (int i = 0 ; i < 3 ; ++i)
      (*this->mp_CyclesBoundaries)[i]->Delete();
  }
};

void AbstractChartData::appendPlotProperties(const QString& label, const QString& unit, int id, const QColor& color, double lineWidth, bool disabled)
{
  PlotProperties prop;
  prop.label = label;
  prop.unit = unit;
  prop.id = id;
  prop.color = color;
  prop.lineWidth = lineWidth;
  prop.options = None;
  prop.setVisible(true);
  prop.setDisabled(disabled);
  this->m_PlotsProperties.push_back(prop);
};

int AbstractChartData::chartNumber() const
{
  return this->mp_ChartLayout->GetSize().Y();
};

btk::VTKChartTimeSeries* AbstractChartData::chart(int i)
{
  return btk::VTKChartTimeSeries::SafeDownCast(this->mp_ChartLayout->GetChart(vtkVector2i(0,i)));//this->chartNumber()-i-1
};

void AbstractChartData::copy(AbstractChartData* source)
{
  this->m_PlotsProperties.clear();
  this->setHorizontalData(source->mp_HorizontalAbscissa, source->mp_CyclesBoundaries);
  for (int i = 0 ; i < this->chartNumber() ; ++i)
  {
    btk::VTKChartTimeSeries* sourceChart = source->chart(i);
    btk::VTKChartTimeSeries* targetChart = this->chart(i);
    btk::VTKAxis* targetAxisX = static_cast<btk::VTKAxis*>(targetChart->GetAxis(vtkAxis::BOTTOM));
    btk::VTKAxis* sourceAxisX = static_cast<btk::VTKAxis*>(sourceChart->GetAxis(vtkAxis::BOTTOM));
    targetAxisX->SetLabelsVisible(sourceAxisX->GetLabelsVisible());
    targetAxisX->SetTickOffset(sourceAxisX->GetTickOffset());
    targetAxisX->SetTickScale(sourceAxisX->GetTickScale());
    targetAxisX->SetDisplayMinimumLimit(sourceAxisX->GetDisplayMinimumLimit());
    targetChart->GetAxis(vtkAxis::LEFT)->SetLabelsVisible(sourceChart->GetAxis(vtkAxis::LEFT)->GetLabelsVisible());
    targetChart->SetCurrentFrameFunctor(sourceChart->GetCurrentFrameFunctor());
    targetChart->SetRegionOfInterestFunctor(sourceChart->GetRegionOfInterestFunctor());
    targetChart->SetEventsFunctor(sourceChart->GetEventsFunctor());
    targetChart->SetBounds(sourceAxisX->GetMinimumLimit(), sourceAxisX->GetMaximumLimit(), 0.0, 0.0);
  }
}

void AbstractChartData::clear()
{
  for (int i = 0 ; i < this->chartNumber() ; ++i)
    this->chart(i)->ClearPlots();
};

void AbstractChartData::generateColor(vtkSmartPointer<vtkColorSeries> colorGenerator, double color[3])
{
  int numPlots = this->m_PlotsProperties.size();
  int colorIndex = numPlots;
  vtkColor3ub c = colorGenerator->GetColorRepeating(colorIndex);
  int num = 0, inc = 0, inc2 = 0;
  while ((num < numPlots) && (inc2 < this->chartNumber()))
  {
    unsigned char rgb[3];
    this->chart(inc2)->GetPlot(inc)->GetPen()->GetColor(rgb);
    if ((c.Red() == rgb[0]) && (c.Green() == rgb[1]) && (c.Blue() == rgb[2]))
    {
      // Look if the color generator is wrapping around the range of color.
      // If yes, then stop to look for a new, non-used color.
      // Can create an infinite loop.
      if (colorIndex > colorGenerator->GetNumberOfColors())
        break;
        
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

void AbstractChartData::initialize(vtkSmartPointer<vtkColorSeries> colorGenerator)
{
  for (int i = 0 ; i < this->chartNumber() ; ++i)
  {
    btk::VTKChartTimeSeries* chart = btk::VTKChartTimeSeries::New(); // Do not delete
    btk::VTKAxis* axisX = static_cast<btk::VTKAxis*>(chart->GetAxis(vtkAxis::BOTTOM));
    btk::VTKAxis* axisY = static_cast<btk::VTKAxis*>(chart->GetAxis(vtkAxis::LEFT));
    axisX->SetTitleVisible(false);
    axisX->SetDisplayMinimumLimit(true);
    axisX->SetBehavior(vtkAxis::FIXED);
    axisY->SetBehavior(vtkAxis::FIXED);
    
    // static_cast<btk::VTKAxis*>(chart->GetAxis(vtkAxis::LEFT))->SetMinimumTickSpacing(25.0f);
    // chart->SetBoundsEnabled(true);
    // chart->SetHiddenAxisBorder(0);
    chart->DisplayEventsOn();
    chart->SetColorSeries(colorGenerator);
    chart->SetBorders(80,20,10,5);
    this->mp_ChartLayout->SetChart(vtkVector2i(0,i), chart);
    chart->Delete();
  }
};

bool AbstractChartData::isAlreadyPlotted(int id) const
{
  for (QList<AbstractChartData::PlotProperties>::const_iterator it = this->plotsProperties().begin() ; it != this->plotsProperties().end() ; ++it)
  {
    if (it->id == id)
      return true;
  }
  return false;
};

bool AbstractChartData::hasCycleBoundaries(int context) const
{
  if (((context >= 1) && (context <= 3)) && ((*this->mp_CyclesBoundaries)[context-1] != NULL) && ((*this->mp_CyclesBoundaries)[context-1]->GetNumberOfTuples() != 0))
    return true;
  return false;
}

void AbstractChartData::removePlot(int index, bool* layoutModified)
{
  *layoutModified = false;
  for (int i = 0 ; i < this->chartNumber() ; ++i)
    this->chart(i)->RemovePlot(index);
  this->m_PlotsProperties.removeAt(index);
};

void AbstractChartData::hidePlot(int index, bool isHidden, bool* layoutModified)
{
  *layoutModified = false;
  for (int i = 0 ; i < this->chartNumber() ; ++i)
    this->chart(i)->HidePlot(index, isHidden);
  this->m_PlotsProperties[index].setVisible(!isHidden);
  // this->m_PlotsProperties[index].visible = !isHidden ;
};

void AbstractChartData::setHorizontalData(vtkFloatArray* (*horizontalAbscissa)[4], vtkFloatArray* (*cyclesBoundaries)[3])
{
  if (this->mp_HorizontalAbscissa != horizontalAbscissa)
  {
    if (this->mp_HorizontalAbscissa != NULL)
    {
      for (int i = 0 ; i < 4 ; ++i)
        (*this->mp_HorizontalAbscissa)[i]->Delete(); // Same as Unregister
    }
    this->mp_HorizontalAbscissa = horizontalAbscissa;
    for (int i = 0 ; i < 4 ; ++i)
      (*horizontalAbscissa)[i]->Register((*this->mp_HorizontalAbscissa)[i]);
  }
  if (this->mp_CyclesBoundaries != cyclesBoundaries)
  {
    if (this->mp_CyclesBoundaries != NULL)
    {
      for (int i = 0 ; i < 3 ; ++i)
        (*this->mp_CyclesBoundaries)[i]->Delete(); // Same as Unregister
    }
    this->mp_CyclesBoundaries = cyclesBoundaries;
    for (int i = 0 ; i < 3 ; ++i)
      (*cyclesBoundaries)[i]->Register((*this->mp_CyclesBoundaries)[i]);
  }
};

void AbstractChartData::setPlotVisible(int index, bool show, bool* layoutModified)
{
  *layoutModified = false;
  for (int i = 0 ; i < this->chartNumber() ; ++i)
    this->chart(i)->GetPlot(index)->SetVisible(show);
};

void AbstractChartData::show(Acquisition* acq, bool s, bool* layoutModified)
{
  *layoutModified = false;
  this->m_PlotsProperties.clear();
  for (int i = 0 ; i < this->chartNumber() ; ++i)
  {
    btk::VTKChartTimeSeries* chart = this->chart(i);
    chart->SetInteractionEnabled(false);
    chart->ClearPlots();

    vtkAxis* axisX = chart->GetAxis(vtkAxis::BOTTOM);
    vtkAxis* axisY = chart->GetAxis(vtkAxis::LEFT);
    if (!s) // Reset
    {
      chart->SetBounds(0.0, 0.0, 0.0, 0.0);
      axisX->SetLabelsVisible(false);
      axisY->SetLabelsVisible(false);
    }
    else // Load
    {
      int roi[2]; acq->regionOfInterest(roi[0], roi[1]);
      chart->SetBounds((double)roi[0], (double)roi[1], 0.0, 0.0);
      axisX->SetLabelsVisible(true);
      axisY->SetLabelsVisible(true);
    }
  }
};

void AbstractChartData::adaptChartDisplay(const QString& titleX, double scaleX, double offsetX, int displayAction, DataCycleMatchingRules* rules)
{
  for (int j = 0 ; j < static_cast<int>(this->chartNumber()) ; ++j)
  {
    btk::VTKChartTimeSeries* chart = static_cast<btk::VTKChartTimeSeries*>(this->chart(j));
    chart->SetDisplayCurrentFrame(displayAction == 1 ? 0 : 1);
    chart->SetDisplayRegionOfInterest(displayAction == 1 ? 0 : 1);
    btk::VTKAxis* axisX = static_cast<btk::VTKAxis*>(chart->GetAxis(vtkAxis::BOTTOM));
    axisX->SetTitle(qPrintable(titleX));
    axisX->SetTickScale(scaleX);
    axisX->SetTickOffset(offsetX);
    
    // Cyclic display
    if (displayAction == 1)
    {
      for (int k = 0 ; k < chart->GetNumberOfPlots() ; ++k)
      {
        int contextId = this->selectContextFromLabel(rules, this->m_PlotsProperties[k].label);
        btk::VTKPlotCycle* plot = btk::VTKPlotCycle::SafeDownCast(chart->GetPlot(k));
        if (plot == NULL)
          continue;
        if (contextId > 0)
        {
          vtkTable* oldInput = plot->GetInput();
          vtkTable* newInput = vtkTable::New();
          newInput->AddColumn((*this->mp_HorizontalAbscissa)[contextId]);
          newInput->AddColumn(oldInput->GetColumn(1));
          plot->SetInput(newInput,0,1);
          plot->SetCyclesBoundaries((*this->mp_CyclesBoundaries)[contextId-1]);
          plot->SetCyclicDisplay(true);
          plot->Modified();
          newInput->Delete();
        }
        else
        {
          this->plotsProperties()[k].setDisabled(true);
          plot->SetVisible(false);
        }
      }
    }
    // Temporal display
    else if (displayAction == -1)
    {
      for (int k = 0 ; k < chart->GetNumberOfPlots() ; ++k)
      {
        btk::VTKPlotCycle* plot = btk::VTKPlotCycle::SafeDownCast(chart->GetPlot(k));
        if (plot == NULL)
          continue;
        if (this->m_PlotsProperties[k].isDisabled())
        {
          plot->SetVisible(true);
        }
        else
        {
          vtkTable* oldInput = plot->GetInput();
          vtkTable* newInput = vtkTable::New();
          newInput->AddColumn((*this->mp_HorizontalAbscissa)[0]);
          newInput->AddColumn(oldInput->GetColumn(1));
          plot->SetInput(newInput,0,1);
          plot->SetCyclicDisplay(false);
          newInput->Delete();
        }
      }
    }
    
    btk::VTKAxis* axisY = static_cast<btk::VTKAxis*>(chart->GetAxis(vtkAxis::LEFT));
    axisY->SetRange(axisY->GetMinimumLimit(), axisY->GetMaximumLimit());
  }
  
  // Extra steps to do only one time for all charts and plots
  if (displayAction == -1)
  {
    for (QList<PlotProperties>::iterator it = this->m_PlotsProperties.begin() ; it != this->m_PlotsProperties.end() ; ++it)
      it->setDisabled(false);
  }
};

int AbstractChartData::selectContextFromLabel(DataCycleMatchingRules* rules, const QString& label)
{
  if (rules == 0)
  {
    qDebug("Impossible to display data by cycle as no rule was set to match data and cycle.");
    return -1;
  }
  
  // General
  int ctx = ChartWidget::GeneralContext;
  // Right
  if (((rules->rightLabelRule == 0) && label.startsWith(rules->rightLabelRuleText))
    || ((rules->rightLabelRule == 1) && label.endsWith(rules->rightLabelRuleText))
    || ((rules->rightLabelRule == 2) && label.contains(rules->rightLabelRuleText)))
    ctx = ChartWidget::RightContext;
  // Left
  else if (((rules->leftLabelRule == 0) && label.startsWith(rules->leftLabelRuleText))
    || ((rules->leftLabelRule == 1) && label.endsWith(rules->leftLabelRuleText))
    || ((rules->leftLabelRule == 2) && label.contains(rules->leftLabelRuleText)))
    ctx = ChartWidget::LeftContext;
  
  if (!this->hasCycleBoundaries(ctx))
    ctx = -1;
  
  return ctx;
};

// -----------------------------------------------------------------------------

PointChartData::PointChartData()
: AbstractChartData(3)
{};

bool PointChartData::acceptDroppedTreeWidgetItem(QTreeWidgetItem* item)
{
  if ((item->type() == MarkerType) || (item->type() == PointType) || (item->type() == ForcePlateType))
    return true;
  return false;
};

bool PointChartData::appendPlotFromDroppedItem(Acquisition* acq, vtkSmartPointer<vtkColorSeries> colorGenerator, QTreeWidgetItem* item, DataCycleMatchingRules* rules, bool* layoutModified)
{
  int horizontalIndex = 0;
  if (rules != NULL)
    horizontalIndex = this->selectContextFromLabel(rules, item->text(0));
  
  *layoutModified = false;
  int id = 0;
  btk::Point::Pointer point;
  QString label, unit;
  if ((item->type() == MarkerType) || (item->type() == PointType))
  {
    id = item->data(0, PointId).toInt();
    if (acq->points().find(id) == acq->points().end())
    {
      qDebug("Point ID greater or equal to the number of points.");
      return false;
    }
    else if (this->isAlreadyPlotted(id))
      return false;
    
    point = acq->btkAcquisition()->GetPoint(acq->points().value(id)->btkidx);
    this->extractPlotLabelUnit(label, unit, acq, id); // Limited only to the point in the acquisition. Moreover, the data from FP cannot be modified.
  }
  else if (item->type() == ForcePlateType)
  {
    id = item->data(0, ForcePlateId).toInt();
    int idxFp = (id - 65535) / 4;
    int idxCpt = (id - 65535) % 4; 
    if (idxFp >= acq->btkGroundReactionWrenches()->GetItemNumber())
    {
      qDebug("Force platform ID greater than the number of platforms.");
      return false;
    }
    if (idxCpt >= 4)
    {
      qDebug("Force platform component ID greater than the number of components.");
      return false;
    }
    else if (this->isAlreadyPlotted(id))
      return false;
    
    if (idxCpt < 3)
      point = acq->btkGroundReactionWrenches()->GetItem(idxFp)->GetComponent(idxCpt);
    else
      point = acq->btkWrenchDirectionAngles()->GetItem(idxFp);
    
    label = "Platform #" + QString::number(idxFp+1) + " - ";
    switch(idxCpt)
    {
    case 0:
      label += "Position";
      unit =  " (" + acq->pointUnit(Point::Marker) + ")";
      break;
    case 1:
      label += "Force reaction";
      unit =  " (" + acq->pointUnit(Point::Force) + ")";
      break;
    case 2:
      label += "Moment reaction";
      unit =  " (" + acq->pointUnit(Point::Moment) + ")";
      break;
    case 3:
      label += "Direction angle";
      unit =  " (degree)";
      break;
    }
  }
  
  this->addPointPlot(id, acq, colorGenerator, point, label, unit, horizontalIndex);

  return true;
};

void PointChartData::addPointPlot(int id, Acquisition* acq, vtkColorSeries* colorGenerator, btk::Point::Pointer point, const QString& label, const QString& unit, int horizontalDataIndex)
{
  bool disabled = false;
  if (horizontalDataIndex < 0)
  {
    disabled = true;
    horizontalDataIndex = 0;
  }
  
  int numFrames = acq->pointFrameNumber();
  // Need to create 3 table instead of 1 with 4 columns as VTK doesn't recognize the 2 last columns (due to the use of the same data?) 
  vtkTable* tableX = vtkTable::New();
  vtkTable* tableY = vtkTable::New();
  vtkTable* tableZ = vtkTable::New();
  tableX->SetNumberOfRows(numFrames); // Must be set before adding column
  tableX->AddColumn((*this->mp_HorizontalAbscissa)[horizontalDataIndex]);
  tableY->SetNumberOfRows(numFrames);
  tableY->AddColumn((*this->mp_HorizontalAbscissa)[horizontalDataIndex]);
  tableZ->SetNumberOfRows(numFrames);
  tableZ->AddColumn((*this->mp_HorizontalAbscissa)[horizontalDataIndex]);
  vtkDoubleArray* arrValX = vtkDoubleArray::New();
  vtkDoubleArray* arrValY = vtkDoubleArray::New();
  vtkDoubleArray* arrValZ = vtkDoubleArray::New();
  const char* str = (label+unit).toUtf8().constData();
  arrValX->SetName(str);
  arrValY->SetName(str);
  arrValZ->SetName(str);
  // FIXME: Conflict into VTK 5.6.1 between the documentation and the code to save or not the data. Need to check with VTK 5.8
  arrValX->SetArray(point->GetValues().data(), numFrames, 1); // Would be 0?
  arrValY->SetArray(point->GetValues().data() + numFrames, numFrames, 1); // Would be 0?
  arrValZ->SetArray(point->GetValues().data() + 2*numFrames, numFrames, 1); // Would be 0?
  tableX->AddColumn(arrValX);
  tableY->AddColumn(arrValY);
  tableZ->AddColumn(arrValZ);

  btk::VTKPlotCycle* plot = 0;
  double color[3];
  this->generateColor(colorGenerator, color);
  // X axis 
  plot = btk::VTKPlotCycle::New();
  this->chart(0)->AddPlot(plot);
  plot->GetPen()->SetColorF(color);
  plot->SetInput(tableX,0,1);
  plot->SetWidth(ChartWidget::DefaultLineWidth);
  if (horizontalDataIndex > 0)
  {
    plot->SetCyclesBoundaries((*this->mp_CyclesBoundaries)[horizontalDataIndex-1]);
    plot->SetCyclicDisplay(true);
  }
  plot->SetVisible(!disabled);
  plot->Delete();
  // Y axis
  plot = btk::VTKPlotCycle::New();
  this->chart(1)->AddPlot(plot);
  plot->GetPen()->SetColorF(color);
  plot->SetInput(tableY,0,1);
  plot->SetWidth(ChartWidget::DefaultLineWidth);
  if (horizontalDataIndex > 0)
  {
    plot->SetCyclesBoundaries((*this->mp_CyclesBoundaries)[horizontalDataIndex-1]);
    plot->SetCyclicDisplay(true);
  }
  plot->SetVisible(!disabled);
  plot->Delete();
  // Z axis
  plot = btk::VTKPlotCycle::New();
  this->chart(2)->AddPlot(plot);
  plot->GetPen()->SetColorF(color);
  plot->SetInput(tableZ,0,1);
  plot->SetWidth(ChartWidget::DefaultLineWidth);
  if (horizontalDataIndex > 0)
  {
    plot->SetCyclesBoundaries((*this->mp_CyclesBoundaries)[horizontalDataIndex-1]);
    plot->SetCyclicDisplay(true);
  }
  plot->SetVisible(!disabled);
  plot->Delete();

  arrValX->Delete();
  arrValY->Delete();
  arrValZ->Delete();
  tableX->Delete();
  tableY->Delete();
  tableZ->Delete();
  
  this->appendPlotProperties(label, unit, id, QColor(static_cast<int>(color[0]*255.0), static_cast<int>(color[1]*255.0), static_cast<int>(color[2]*255.0)), ChartWidget::DefaultLineWidth, disabled);
}

void PointChartData::extractPlotLabelUnit(QString& label, QString& unit, Acquisition* acq, int id)
{
  Point* p = acq->points().value(id);
  label = p->label;
  unit = acq->pointUnit(p->type);
  if (!unit.isEmpty())
    unit = " (" + unit + ")";
};

void PointChartData::initialize(vtkSmartPointer<vtkColorSeries> colorGenerator)
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
  if (item->type() == AnalogType)
    return true;
  return false;
};

bool AnalogChartData::appendPlotFromDroppedItem(Acquisition* acq, vtkSmartPointer<vtkColorSeries> colorGenerator, QTreeWidgetItem* item, DataCycleMatchingRules* rules, bool* layoutModified)
{
  int horizontalIndex = 0;
  if (rules != NULL)
    horizontalIndex = this->selectContextFromLabel(rules, item->text(0));
  bool disabled = false;
  if (horizontalIndex < 0)
  {
    disabled = true;
    horizontalIndex = 0;
  }
  
  *layoutModified = false;
  int id = item->data(0, AnalogId).toInt();
  if (acq->analogs().find(id) == acq->analogs().end())
  {
    qDebug("Analog ID greater than the number of analog channels.");
    return false;
  }
  else if (this->isAlreadyPlotted(id))
    return false;
  btk::Analog::Pointer analog = acq->btkAcquisition()->GetAnalog(acq->analogs().value(id)->btkidx);
  vtkTable* table = vtkTable::New();
  table->SetNumberOfRows(acq->analogFrameNumber()); // Must be set before adding column
  table->AddColumn((*this->mp_HorizontalAbscissa)[horizontalIndex]);
  vtkDoubleArray* arrVal = vtkDoubleArray::New();
  QString label, unit;
  this->extractPlotLabelUnit(label, unit, acq, id);
  const char* str = (label+unit).toUtf8().constData();
  arrVal->SetName(str);
  // FIXME: Conflict into VTK 5.6.1 between the documentation and the code to save or not the data. Need to check with VTK 5.8
  arrVal->SetArray(analog->GetValues().data(), analog->GetFrameNumber(), 1); // Would be 0?
  table->AddColumn(arrVal);
  double color[3];
  this->generateColor(colorGenerator, color);
  btk::VTKPlotCycle* plot = btk::VTKPlotCycle::New();
  if (!this->m_Expanded || (this->chart(0)->GetNumberOfPlots() == 0))
  {
    this->chart(0)->AddPlot(plot);
    if (this->m_Expanded)
      this->chart(0)->GetAxis(vtkAxis::LEFT)->SetTitle(str);
  }
  else
  {
    btk::VTKChartTimeSeries* chart = this->chart(this->chartNumber()-1);
    // Check if the last chart is empty, ready to be used
    if (chart->GetNumberOfPlots() != 0)
    {
      chart = this->createChart(this->chart(0));
      *layoutModified = true;
    }
    chart->GetAxis(vtkAxis::LEFT)->SetTitle(str);
    chart->AddPlot(plot);
  }
  plot->GetPen()->SetColorF(color);
  plot->SetInput(table,0,1);
  plot->SetWidth(ChartWidget::DefaultLineWidth);
  if (horizontalIndex > 0)
  {
    plot->SetCyclesBoundaries((*this->mp_CyclesBoundaries)[horizontalIndex-1]);
    plot->SetCyclicDisplay(true);
  }
  plot->SetVisible(!disabled);

  arrVal->Delete();
  table->Delete();
  plot->Delete();
  
  this->appendPlotProperties(label, unit, id, QColor(static_cast<int>(color[0]*255.0), static_cast<int>(color[1]*255.0), static_cast<int>(color[2]*255.0)), plot->GetWidth(), disabled);

  return true;
};

void AnalogChartData::extractPlotLabelUnit(QString& label, QString& unit, Acquisition* acq, int id)
{
  label = acq->analogLabel(id);
  unit = acq->analogUnit(id);
  if (!unit.isEmpty())
    unit = " (" + unit + ")";
};

void AnalogChartData::initialize(vtkSmartPointer<vtkColorSeries> colorGenerator)
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
    this->chart(index)->GetPlot(0)->SetVisible(show);
    this->chart(index)->SetVisible(show);
 
    if (show && (this->chart(this->chartNumber()-1)->GetNumberOfPlots() == 0)) // Remove the fake chart
    {
      this->mp_ChartLayout->SetSize(vtkVector2i(1,this->chartNumber()-1));
    }
    else if (!show)
    {
      bool anyChartVisible = false;
      for (int i = 0 ; i < this->chartNumber() ; ++i)
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
    if (!this->m_PlotsProperties.isEmpty())
    {
      for (int i = index+1 ; i < this->chartNumber() ; ++i)
      {
        btk::VTKChartTimeSeries* chart = this->mp_ChartLayout->TakeChart(vtkVector2i(0,i));
        this->mp_ChartLayout->SetChart(vtkVector2i(0,i-1), chart);
        chart->Delete();
      }
      this->mp_ChartLayout->SetSize(vtkVector2i(1,this->chartNumber()-1));
      *layoutModified = true;
    }
    else
    {
      btk::VTKChartTimeSeries* chartZero = this->chart(0);
      chartZero->RemovePlot(0);
      chartZero->GetAxis(vtkAxis::LEFT)->SetTitle("Values");
      *layoutModified = false;
    }
  }
};

void AnalogChartData::hidePlot(int index, bool isHidden, bool* layoutModified)
{
  if (!this->m_Expanded)
    this->AbstractChartData::hidePlot(index, isHidden, layoutModified);
  else
  {
    btk::VTKChartTimeSeries* chart = this->chart(index);
    chart->SetVisible(!isHidden);
    chart->GetPlot(0)->SetVisible(!isHidden);
    this->m_PlotsProperties[index].setVisible(!isHidden);
    // this->m_PlotsProperties[index].visible = !isHidden;
    
    bool anyChartVisible = false;
    for (int i = 0 ; i < this->chartNumber() ; ++i)
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
    }
    else if (this->chart(this->chartNumber()-1)->GetNumberOfPlots() == 0)
      this->mp_ChartLayout->SetSize(vtkVector2i(1,this->chartNumber()-1));
    *layoutModified = true;
  }
};

void AnalogChartData::show(Acquisition* acq, bool s, bool* layoutModified)
{
  if (!this->m_Expanded)
    this->AbstractChartData::show(acq, s, layoutModified);
  else
  {
    btk::VTKChartTimeSeries* chartZero = this->chart(0);
    for (int i = 1 ; i < this->chartNumber() ; ++i)
      this->chart(i)->ClearPlots();
    this->mp_ChartLayout->SetSize(vtkVector2i(1,1));
    chartZero->GetAxis(vtkAxis::LEFT)->SetTitle("Values");
    this->AbstractChartData::show(acq, s, layoutModified);
    *layoutModified = true;
  }
};

void AnalogChartData::setExpandable(bool expandable)
{
  if (this->m_Expanded == expandable)
    return;
  this->m_Expanded = expandable;
  
  if (this->m_PlotsProperties.isEmpty())
    return;
  
  // Expand or collapse the chart
  btk::VTKChartTimeSeries* chartZero = this->chart(0);
  if (this->m_Expanded)
  {
    int numPlots = chartZero->GetNumberOfPlots();
    for (int i = 1 ; i < numPlots ; ++i)
    {
      btk::VTKChartTimeSeries* chart = this->createChart(chartZero);
      vtkPlot* plot = chartZero->TakePlot(1);
      chart->AddPlot(plot);
      chart->GetAxis(vtkAxis::LEFT)->SetTitle(plot->GetLabel());
      chart->SetVisible(plot->GetVisible());
      plot->Delete();
    }
    chartZero->SetVisible(chartZero->GetPlot(0)->GetVisible());
    chartZero->GetAxis(vtkAxis::LEFT)->SetTitle(chartZero->GetPlot(0)->GetLabel());
  }
  else
  {
    // Can have only 1 plot per chart
    for (int i = 1 ; i < this->chartNumber() ; ++i)
      chartZero->AddPlot(this->chart(i)->GetPlot(0));
    this->mp_ChartLayout->SetSize(vtkVector2i(1,1));
    chartZero->GetAxis(vtkAxis::LEFT)->SetTitle("Values");
    chartZero->SetVisible(true);
  };
}

btk::VTKChartTimeSeries* AnalogChartData::createChart(btk::VTKChartTimeSeries* sourceChart)
{
  btk::VTKChartTimeSeries* targetChart = btk::VTKChartTimeSeries::New();
  btk::VTKAxis* targetAxisX = static_cast<btk::VTKAxis*>(targetChart->GetAxis(vtkAxis::BOTTOM));
  btk::VTKAxis* sourceAxisX = static_cast<btk::VTKAxis*>(sourceChart->GetAxis(vtkAxis::BOTTOM));
  targetAxisX->SetTitle(sourceAxisX->GetTitle());
  targetAxisX->SetTitleVisible(false); // Frames // X axis
  targetAxisX->SetDisplayMinimumLimit(sourceAxisX->GetDisplayMinimumLimit());
  const int* sourceBorders = sourceChart->GetBorders(); targetChart->SetBorders(sourceBorders[0], sourceBorders[1], sourceBorders[2], sourceBorders[3]);
  targetChart->SetCurrentFrameFunctor(sourceChart->GetCurrentFrameFunctor());
  targetChart->SetRegionOfInterestFunctor(sourceChart->GetRegionOfInterestFunctor());
  targetChart->SetEventsFunctor(sourceChart->GetEventsFunctor());
  targetChart->DisplayEventsOn();
  targetChart->SetColorSeries(sourceChart->GetColorSeries());
  targetChart->SetBounds(sourceAxisX->GetMinimumLimit(), sourceAxisX->GetMaximumLimit(), 0.0, 0.0);
  int idx = this->chartNumber();
  this->mp_ChartLayout->SetSize(vtkVector2i(1,idx+1));
  this->mp_ChartLayout->SetChart(vtkVector2i(0,idx), targetChart);
  targetChart->Delete(); // Because the layout owns the chart 
  return targetChart;
};

// -----------------------------------------------------------------------------

VTKChartWidget::VTKChartWidget(QWidget* parent, Qt::WindowFlags f)
: VizRendererWidget(parent, 0, f), m_OldMousePosition()
{
  this->mp_CurrentChartData = 0;
  this->m_AlternateMouseEvent = false;
  this->setMouseTracking(false);
#ifdef Q_OS_WIN
  #pragma message("WARNING: It seems that Qt 4.8.3 introduced a bug under Windows for the drag'n drop action (don't check parent's attribute?). Need to check for later version.")
  // Drag and drop
  this->setAcceptDrops(true);
#endif  
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
    
    btk::VTKChartTimeSeries* chart = this->focusedPlotArea(helpEvent->pos());
    vtkContextMouseEvent mouse;
    mouse.ScreenPos = vtkVector2i(helpEvent->pos().x(), this->height() - helpEvent->pos().y() - 1);
    btk::VTKChartPlotData plotIndex;
    if ((chart != 0) && (chart->LocatePointInPlots(mouse, plotIndex)))
    {
      btk::VTKAxis* axisX = static_cast<btk::VTKAxis*>(chart->GetAxis(vtkAxis::BOTTOM));
      btk::VTKAxis* axisY = static_cast<btk::VTKAxis*>(chart->GetAxis(vtkAxis::LEFT));
      double coordX = (plotIndex.Position.X() + axisX->GetTickOffset()) * axisX->GetTickScale();
      double range = fabs(axisY->GetMaximum() - axisY->GetMinimum());
      QString valueStr;
      if (plotIndex.Position.Y() == 0.0)
        valueStr = "0";
      else if (range > 100.0)
        valueStr = QString::number(plotIndex.Position.Y(), 'f', 1);
      else if (range > 10.0)
        valueStr = QString::number(plotIndex.Position.Y(), 'f', 2);
      else if (range > 1.0)
        valueStr = QString::number(plotIndex.Position.Y(), 'f', 3);
      else if (range > 0.1)
        valueStr = QString::number(plotIndex.Position.Y(), 'f', 4);
      else
        valueStr = QString::number(plotIndex.Position.Y(), 'f', 5);
      QString str = "<nobr>" + QString::fromUtf8(chart->GetAxis(vtkAxis::BOTTOM)->GetTitle()) + ": " + QString::number(coordX) + "</nobr><br/><nobr>" + QString::fromUtf8(plotIndex.SeriesName) + ": " + valueStr + "</nobr>";
      QToolTip::showText(helpEvent->globalPos(), str);
      return true;
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
  return this->VizRendererWidget::event(event);
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

void VTKChartWidget::computeRatio(float ratio[2], int screenPos[2], btk::VTKChartTimeSeries* chart)
{
  float min[2], max[2];
  vtkAxis* axis = 0;
  axis = chart->GetAxis(vtkAxis::BOTTOM);
  axis->GetPoint1(min); axis->GetPoint2(max);
  ratio[0] = (static_cast<float>(screenPos[0]) - min[0]) / (max[0] - min[0]);
  axis = chart->GetAxis(vtkAxis::LEFT);
  axis->GetPoint1(min); axis->GetPoint2(max);
  ratio[1] = (static_cast<float>(screenPos[1]) - min[1]) / (max[1] - min[1]);
};

void VTKChartWidget::applyRatio(float pos[2], float ratio[2], btk::VTKChartTimeSeries* chart)
{
  float min[2], max[2];
  vtkAxis* axis = 0;
  axis = chart->GetAxis(vtkAxis::BOTTOM);
  axis->GetPoint1(min); axis->GetPoint2(max);
  pos[0] = ratio[0] * (max[0] - min[0]) + min[0];
  axis = chart->GetAxis(vtkAxis::LEFT);
  axis->GetPoint1(min); axis->GetPoint2(max);
  pos[1] = ratio[1] * (max[1] - min[1]) + min[1];
};

void VTKChartWidget::mousePressEvent(QMouseEvent* event)
{
  if ((event->button() == Qt::LeftButton) && (this->mp_CurrentChartData != NULL) && (this->mp_CurrentChartData->chartNumber() > 1) && ((event->modifiers() & Qt::AltModifier) == Qt::AltModifier))
  {
    btk::VTKChartTimeSeries* chart = 0;
    if ((chart = this->focusedPlotArea(event->pos())) == NULL)
      return;
    
    int screenPos[2] = {event->pos().x(), this->height() - event->pos().y() - 1};
    float ratio[2] = {0};
    this->computeRatio(ratio, screenPos, chart);
    vtkContextMouseEvent mouse;
    mouse.Button = vtkContextMouseEvent::LEFT_BUTTON;
#if (((VTK_MAJOR_VERSION == 5) && (VTK_MINOR_VERSION >= 10)) || (VTK_MAJOR_VERSION >= 6))
    mouse.SetInteractor(0); // Because the constructor doesn't set the interactor member to NULL.
#endif
    for (int i = 0 ; i < this->mp_CurrentChartData->chartNumber() ; ++i)
    {
      chart = this->mp_CurrentChartData->chart(i);
      float pos[2] = {0.0f, 0.0f};
      this->applyRatio(pos, ratio, chart);
      mouse.Pos = vtkVector2f(pos[0], pos[1]);
      chart->SetDisplayZoomBox((event->modifiers() & Qt::ShiftModifier) == Qt::ShiftModifier ? 1 : 0);
      chart->MouseButtonPressEvent(mouse);
    }
    this->m_OldMousePosition = event->pos();
    this->m_AlternateMouseEvent = true;
    this->QWidget::mousePressEvent(event);
  }
  else
  {
    // FIX for VTK 5.8
#if ((VTK_MAJOR_VERSION == 5) && (VTK_MINOR_VERSION < 10))
    // invoke appropriate vtk event only for the left button
    if(event->button() == Qt::LeftButton)
    {
      btk::VTKChartTimeSeries* chart = this->focusedPlotArea(event->pos());
      if (chart != 0)
        chart->SetDisplayZoomBox((event->modifiers() & Qt::ShiftModifier) == Qt::ShiftModifier ? 1 : 0);
    }
#endif
    this->VizRendererWidget::mousePressEvent(event);
  }
};

void VTKChartWidget::mouseReleaseEvent(QMouseEvent* event)
{
  if (this->m_AlternateMouseEvent)
  {
    btk::VTKChartTimeSeries* chart = 0;
    if ((chart = this->focusedPlotArea(event->pos())) == NULL)
      return;
    
    int screenPos[2] = {event->pos().x(), this->height() - event->pos().y() - 1};
    float ratio[2] = {0};
    this->computeRatio(ratio, screenPos, chart);
    vtkContextMouseEvent mouse;
    mouse.Button = vtkContextMouseEvent::LEFT_BUTTON;
#if (((VTK_MAJOR_VERSION == 5) && (VTK_MINOR_VERSION >= 10)) || (VTK_MAJOR_VERSION >= 6))
    mouse.SetInteractor(0); // Because the constructor doesn't set the interactor member to NULL.
#endif
    for (int i = 0 ; i < this->mp_CurrentChartData->chartNumber() ; ++i)
    {
      chart = this->mp_CurrentChartData->chart(i);
      float pos[2] = {0.0f, 0.0f};
      this->applyRatio(pos, ratio, chart);
      mouse.Pos = vtkVector2f(pos[0], pos[1]);
      chart->MouseButtonReleaseEvent(mouse);
    }
    this->m_AlternateMouseEvent = false;
    this->QWidget::mouseReleaseEvent(event);
  }
  else
    this->VizRendererWidget::mouseReleaseEvent(event);
};

void VTKChartWidget::mouseMoveEvent(QMouseEvent* event)
{
  // Shunt the standard behavior of VTK is the Alt modifier is pressed
  if (this->m_AlternateMouseEvent)
  {
    btk::VTKChartTimeSeries* chart = 0;
    if ((chart = this->focusedPlotArea(event->pos())) == NULL)
      return;
    int screenPos[2] = {event->pos().x(), this->height() - event->pos().y() - 1};
    float ratio[2] = {0};
    this->computeRatio(ratio, screenPos, chart);
    float min_[2];
    chart->GetAxis(vtkAxis::BOTTOM)->GetPoint1(min_);
    int min[2] = {static_cast<int>(min_[0]), static_cast<int>(min_[2])};
    int offsetMousePos[2] = {screenPos[0] - min[0], screenPos[1] - min[0]};
    int offsetMouseLastPos[2] = {this->m_OldMousePosition.x() - min[0], this->height() - this->m_OldMousePosition.y() - 1 - min[0]};
    vtkContextMouseEvent mouse;
#if (((VTK_MAJOR_VERSION == 5) && (VTK_MINOR_VERSION >= 10)) || (VTK_MAJOR_VERSION >= 6))
    mouse.SetInteractor(0); // Because the constructor doesn't set the interactor member to NULL.
#endif
    mouse.Button = vtkContextMouseEvent::LEFT_BUTTON;
    for (int i = 0 ; i < this->mp_CurrentChartData->chartNumber() ; ++i)
    {
      chart = this->mp_CurrentChartData->chart(i);
      float pos[2] = {0.0f, 0.0f};
      this->applyRatio(pos, ratio, chart);
      mouse.Pos = vtkVector2f(pos[0], pos[1]);
      chart->GetAxis(vtkAxis::BOTTOM)->GetPoint1(min_);
      min[0] = static_cast<int>(min_[0]); min[1] = static_cast<int>(min_[2]);
      mouse.ScreenPos = vtkVector2i(min[0]+offsetMousePos[0], min[1]+offsetMousePos[1]);
      mouse.LastScreenPos = vtkVector2i(min[0]+offsetMouseLastPos[0], min[1]+offsetMouseLastPos[1]);
      chart->MouseMoveEvent(mouse);
    }
    this->m_OldMousePosition = event->pos();
    this->QWidget::mouseMoveEvent(event);
  }
  else
    this->VizRendererWidget::mouseMoveEvent(event);
};

void VTKChartWidget::wheelEvent(QWheelEvent* event)
{
  // Shunt the standard behavior of VTK is the Alt modifier is pressed
  // This is done here and not in the VTK classes as (at least with VTK 5.10 under MacOS 10.5 with a MBP),
  // the Alt modifier is not propagated. Morever, it will complexify the behavior 
  // of the VTK classes for a simple key...
  if ((this->mp_CurrentChartData != NULL) && (this->mp_CurrentChartData->chartNumber() > 1) && ((event->modifiers() & Qt::AltModifier) == Qt::AltModifier))
  {
    btk::VTKChartTimeSeries* chart = 0;
    if ((chart = this->focusedPlotArea(event->pos())) == NULL)
      return;
    int screenPos[2] = {event->pos().x(), this->height() - event->pos().y() - 1};
    float ratio[2] = {0};
    this->computeRatio(ratio, screenPos, chart);
    vtkContextMouseEvent mouse;
#if (((VTK_MAJOR_VERSION == 5) && (VTK_MINOR_VERSION >= 10)) || (VTK_MAJOR_VERSION >= 6))
    mouse.SetInteractor(0); // Because the constructor doesn't set the interactor member to NULL.
#endif
    for (int i = 0 ; i < this->mp_CurrentChartData->chartNumber() ; ++i)
    {
      chart = this->mp_CurrentChartData->chart(i);
      float pos[2] = {0.0f, 0.0f};
      this->applyRatio(pos, ratio, chart);
      mouse.Pos = vtkVector2f(pos[0], pos[1]);
      chart->SetZoomMode((event->modifiers() & Qt::ShiftModifier) == Qt::ShiftModifier ? btk::VTKChartTimeSeries::HORIZONTAL : btk::VTKChartTimeSeries::BOTH);
      chart->MouseWheelEvent(mouse, (event->delta() > 0) ? 1 : -1); // Because default deltas used by VTK are (-1,1). The use of Qt values (-2,2) can break the zoom mode
    }
  }
  else
  {
    // FIX for VTK 5.8
#if ((VTK_MAJOR_VERSION == 5) && (VTK_MINOR_VERSION < 10))
    btk::VTKChartTimeSeries* chart = this->focusedPlotArea(event->pos());
    if (chart != 0)
      chart->SetZoomMode((event->modifiers() & Qt::ShiftModifier) == Qt::ShiftModifier ? btk::VTKChartTimeSeries::HORIZONTAL : btk::VTKChartTimeSeries::BOTH);
#endif
    this->VizRendererWidget::wheelEvent(event);
  }
};

btk::VTKChartTimeSeries* VTKChartWidget::focusedChart(const QPoint& pos) const
{
  btk::VTKChartTimeSeries* chart = 0;
  if (this->mp_CurrentChartData != NULL)
  {
    vtkContextMouseEvent mouse;
    mouse.ScreenPos = vtkVector2i(pos.x(), this->height() - pos.y());
    for (int i = 0 ; i < this->mp_CurrentChartData->chartNumber() ; ++i)
    {
      chart = this->mp_CurrentChartData->chart(i);
      if (chart->Hit2(mouse))
        break;
      chart = 0;
    }
  }
  return chart;
};

btk::VTKChartTimeSeries* VTKChartWidget::focusedPlotArea(const QPoint& pos) const
{
  btk::VTKChartTimeSeries* chart = 0;
  if (this->mp_CurrentChartData != NULL)
  {
    vtkContextMouseEvent mouse;
    mouse.ScreenPos = vtkVector2i(pos.x(), this->height() - pos.y() - 1);
    for (int i = 0 ; i < this->mp_CurrentChartData->chartNumber() ; ++i)
    {
      chart = this->mp_CurrentChartData->chart(i);
      if (chart->Hit(mouse))
        break;
      chart = 0;
    }
  }
  return chart;
};
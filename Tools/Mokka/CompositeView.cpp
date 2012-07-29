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

#include "CompositeView.h"
#include "Viz3DWidget.h"
#include "ChartWidget.h"
#include "ChartOptionsWidget.h"
#include "VideoWidget.h"
#include "LoggerWidget.h"
#include "Acquisition.h"

#include <btkMacro.h>
#include <btkVTKInteractorStyleTrackballFixedUpCamera.h>

#include <vtkCamera.h>
#include <vtkRenderWindow.h>

#include <QListWidget>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>

class HeaderOptionComboBox : public QComboBox
{
public:
  HeaderOptionComboBox(QWidget* parent = 0);
};

class ChartPointComponentCheckBox : public QCheckBox
{
public:
  ChartPointComponentCheckBox(const QString& text, QWidget* parent = 0);
};

class ChartOptionPushButton : public QPushButton
{
public:
  ChartOptionPushButton(QWidget* parent = 0);
};

CompositeView::CompositeView(QWidget* parent)
: AbstractView(parent)
{
  this->finalizeUi();
  this->viewCombo->blockSignals(true);
  this->viewCombo->setCurrentIndex(Viz3DProjection); // Adapt the index of the stack widget
  this->viewCombo->blockSignals(false);
};

void CompositeView::setAcquisition(Acquisition* acq)
{
  static_cast<Viz3DWidget*>(this->view(Viz3D))->setAcquisition(acq);
  static_cast<ChartWidget*>(this->view(Chart))->setAcquisition(acq);
  static_cast<VideoWidget*>(this->view(MediaVideo))->setAcquisition(acq);
};

void CompositeView::render()
{
  QWidget* w = this->viewStack->currentWidget();
  switch (this->convertComboIndexToEnumIndex(this->viewCombo->currentIndex()))
  {
  case Viz3DProjection:
  case Viz3DOrthogonal:
    static_cast<Viz3DWidget*>(w)->render();
    break;
  case ChartPoint:
  case ChartAnalog:
    static_cast<ChartWidget*>(w)->render();
    break;
  case MediaVideo:
    static_cast<VideoWidget*>(w)->render();
    break;
  default:
    break;
  }
};

void CompositeView::show(bool s)
{
  static_cast<Viz3DWidget*>(this->view(Viz3D))->show(s);
  static_cast<ChartWidget*>(this->view(Chart))->show(s); // Init the frame and activate the actions.
  static_cast<VideoWidget*>(this->view(MediaVideo))->show(s);
}

AbstractView* CompositeView::clone() const
{
  CompositeView* sv = new CompositeView(this->parentWidget());
  // Clone the 3D view
  static_cast<Viz3DWidget*>(sv->view(Viz3D))->copy(static_cast<Viz3DWidget*>(this->view(Viz3D)));
  // Clone the charts
  static_cast<ChartWidget*>(sv->view(Chart))->copy(static_cast<ChartWidget*>(this->view(Chart)));
  // Clone the video
  static_cast<VideoWidget*>(sv->view(MediaVideo))->copy(static_cast<VideoWidget*>(this->view(MediaVideo)));
  // Clone the consoles
  //  - Logger (nothing to copy)
  
  return sv;
};

void CompositeView::copyOptions(CompositeView* from)
{
  if (!from)
    return;

  this->optionStack->setCurrentIndex(this->optionStackIndexFromViewComboIndex(from->viewCombo->currentIndex()));
  switch (from->viewCombo->currentIndex())
  {
  case Viz3DOrthogonal:
    static_cast<QComboBox*>(this->optionStack->currentWidget())->setCurrentIndex(static_cast<QComboBox*>(from->optionStack->currentWidget())->currentIndex());
    static_cast<Viz3DWidget*>(this->view(Viz3D))->copyProjectionCameraConfiguration(static_cast<Viz3DWidget*>(from->view(Viz3D)));
    break;
  case ChartAnalog:
    static_cast<QComboBox*>(this->optionStack->currentWidget()->layout()->itemAt(0)->layout()->itemAt(0)->widget())->setCurrentIndex(static_cast<QComboBox*>(from->optionStack->currentWidget()->layout()->itemAt(0)->layout()->itemAt(0)->widget())->currentIndex());
    break;
  default: // All others cases
    break;
  }
};

int CompositeView::optionStackIndexFromViewComboIndex(int idx) const
{
  switch (idx)
  {
  case Viz3DOrthogonal:
    return 1;
  case ChartPoint:
    return 2;
  case ChartAnalog:
    return 3;
  default: // All others cases
    return 0;
  }
};

int CompositeView::viewStackIndexFromViewComboIndex(int idx) const
{
  int index = this->convertComboIndexToEnumIndex(idx);
  switch (index)
  {
  case Viz3D:
  case Viz3DProjection:
  case Viz3DOrthogonal:
    return 0;
  case Chart:
  case ChartPoint:
  case ChartAnalog:
    return 1;
  case MediaVideo:
    return 2;
  case ConsoleLogger:
    return 3;
  default:  // Impossible
    qDebug("Incorrect view index! Impossible to find the corresponding stack!");
    return this->viewStack->currentIndex();
  }
};

int CompositeView::convertEnumIndexToComboIndex(int idx) const
{
  switch (idx)
  {
  case Media:
    return 6;
  case MediaVideo:
    return 7;
  case Console:
    return 8;
  case ConsoleLogger:
    return 9;
  default:
    return idx;
  }
};

// Map between the index of the selected item in the combobox and the its harcoded index used to save/restore the layout
int CompositeView::convertComboIndexToEnumIndex(int idx) const
{
  switch (idx)
  {
  case 6:
    return Media;
  case 7:
    return MediaVideo;
  case 8:
    return Console;
  case 9:
    return ConsoleLogger;
  default:
    return idx;
  }
};

void CompositeView::setOrthogonalView(int view)
{
  if (view == -1)
    return;
  Viz3DWidget* viz3D = static_cast<Viz3DWidget*>(this->view(Viz3D));
  viz3D->setOrthogonalView(view);
  this->viewStack->currentWidget()->setFocus(Qt::OtherFocusReason);
};

void CompositeView::toggleChartOptions()
{
  QLayout* layout = this->optionStack->currentWidget()->layout();
  QWidget* w = layout->itemAt(layout->count()-1)->widget(); // Chart options button
  QPoint pos = w->mapToGlobal(QPoint(0,0)) + QPoint(w->width() / 2, w->height());
  static_cast<ChartWidget*>(this->view(Chart))->toggleOptions(pos);
};

void CompositeView::finalizeView(int idx)
{
  if ((idx == Viz3DProjection) || (idx == Viz3DOrthogonal))
  {
    Viz3DWidget* viz3D = static_cast<Viz3DWidget*>(this->view(Viz3D));
    btk::VTKInteractorStyleTrackballFixedUpCamera* style = static_cast<btk::VTKInteractorStyleTrackballFixedUpCamera*>(viz3D->GetRenderWindow()->GetInteractor()->GetInteractorStyle());
    vtkCamera* cam = viz3D->renderer()->GetActiveCamera();
    if (idx == Viz3DProjection)
    {
      cam->ParallelProjectionOff();
      style->RotationEnabledOn();
      style->SpinEnabledOn();
      viz3D->restoreProjectionCameraConfiguration();
    }
    else if (idx == Viz3DOrthogonal)
    {
      viz3D->saveProjectionCameraConfiguration();
      cam->ParallelProjectionOn();
      style->RotationEnabledOff();
      style->SpinEnabledOff();
      // Refresh
      QComboBox* orthogonalComboBox = static_cast<QComboBox*>(this->optionStack->currentWidget());
      int index = orthogonalComboBox->currentIndex();
      orthogonalComboBox->setCurrentIndex(-1);
      orthogonalComboBox->setCurrentIndex(index);
    }
    this->render();
  }
  else if ((idx == ChartPoint) || (idx == ChartAnalog))
  {
    ChartWidget* chart = static_cast<ChartWidget*>(this->view(Chart));
    if (idx == ChartPoint)
      chart->displayPointChart();
    else if (idx == ChartAnalog)
      chart->displayAnalogChart();
    this->render();
  }
};

void CompositeView::adaptLayoutStrech(int idx)
{
  QHBoxLayout* layout = static_cast<QHBoxLayout*>(this->separatorFuncButtons->layout());
  switch(idx)
  {
  case ChartAnalog:
  case ChartPoint:
    layout->setStretch(2,1);
    layout->setStretch(3,0);
    break;
  default:
    layout->setStretch(2,0);
    layout->setStretch(3,1);
  };
};

void CompositeView::finalizeUi()
{
  QFont f;
  QListWidget* lw;
  QListWidgetItem* lwi;
  // View combo
  lw = new QListWidget(this);
  // 3D View
  lwi = new QListWidgetItem(tr("3D View"));
  lwi->setFlags(lwi->flags() & ~Qt::ItemIsSelectable);
  f = this->viewCombo->font();
  f.setBold(true);
  lwi->setFont(f);
  lw->addItem(lwi);
  // - Perspective
  lw->addItem(new QListWidgetItem(tr("  Perspective")));
  // - Orthogonal
  lw->addItem(new QListWidgetItem(tr("  Orthogonal")));
  // Chart
  lwi = new QListWidgetItem(tr("Chart"));
  lwi->setFlags(lwi->flags() & ~Qt::ItemIsSelectable);
  lwi->setFont(f);
  lw->addItem(lwi);
  // - Point
  lw->addItem(new QListWidgetItem(tr("  Point")));
  // - Analog channel
  lw->addItem(new QListWidgetItem(tr("  Analog")));
  // Media
  lwi = new QListWidgetItem(tr("Media"));
  lwi->setFlags(lwi->flags() & ~Qt::ItemIsSelectable);
  lwi->setFont(f);
  lw->addItem(lwi);
  // - Synchronized / unynchronized video
  lw->addItem(new QListWidgetItem(tr("  Video")));
  // Console
  lwi = new QListWidgetItem(tr("Console"));
  lwi->setFlags(lwi->flags() & ~Qt::ItemIsSelectable);
  lwi->setFont(f);
  lw->addItem(lwi);
  // - Logger
  lw->addItem(new QListWidgetItem(tr("  Logger")));
  
  // Update the model & view
  this->viewCombo->blockSignals(true);
  this->viewCombo->setModel(lw->model());
  this->viewCombo->setView(lw);
  this->viewCombo->blockSignals(false);
  
  // Widget in the view stack
  // - 3D View
  Viz3DWidget* viz3d = new Viz3DWidget(this);
  this->viewStack->addWidget(viz3d);
  viz3d->initialize();
  // - Charts
  ChartWidget* chart = new ChartWidget(this);
  this->viewStack->addWidget(chart);
  chart->initialize();
  // - Videos
  VideoWidget* video = new VideoWidget(this);
  this->viewStack->addWidget(video);
  // - Console
  //   + Logger
  LoggerWidget* logger = new LoggerWidget(this);
  this->viewStack->addWidget(logger);
  
  // Widget in the function stack
  // - Projection 3D View
  this->optionStack->addWidget(new QWidget(this)); // empty
  // - Perspective 3D view
  HeaderOptionComboBox* orthogonalComboBox = new HeaderOptionComboBox(this);
  lw = new QListWidget(this);
  lw->addItem(new QListWidgetItem("Top"));
  lw->addItem(new QListWidgetItem("Bottom"));
  lw->addItem(new QListWidgetItem("Left"));
  lw->addItem(new QListWidgetItem("Right"));
  lw->addItem(new QListWidgetItem("Back"));
  lw->addItem(new QListWidgetItem("Front"));
  orthogonalComboBox->setModel(lw->model());
  orthogonalComboBox->setView(lw);
  connect(orthogonalComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setOrthogonalView(int)));
  this->optionStack->addWidget(orthogonalComboBox);
  // - Points' chart
  const int spacing = 9;
  QWidget* pointChartOptionPage = new QWidget;
  QHBoxLayout* pointChartOptionLayout = new QHBoxLayout(pointChartOptionPage);
  pointChartOptionLayout->setContentsMargins(0,0,0,0);
  QHBoxLayout* pointChartOptionLayout2 = new QHBoxLayout; // Second layout to fix the size and position of the button for the options
  pointChartOptionLayout2->setContentsMargins(0,0,0,0);
  ChartPointComponentCheckBox* pointChartXCheckBox = new ChartPointComponentCheckBox("x", pointChartOptionPage);
  pointChartXCheckBox->setToolTip("Display X component");
  pointChartXCheckBox->setCheckState(Qt::Checked);
  connect(pointChartXCheckBox, SIGNAL(stateChanged(int)), chart, SLOT(displayPointComponentX(int)));
  pointChartOptionLayout2->addWidget(pointChartXCheckBox);
  pointChartOptionLayout2->addSpacing(spacing);
  ChartPointComponentCheckBox* pointChartYCheckBox = new ChartPointComponentCheckBox("y", pointChartOptionPage);
  pointChartYCheckBox->setToolTip("Display Y component");
  pointChartYCheckBox->setCheckState(Qt::Checked);
  connect(pointChartYCheckBox, SIGNAL(stateChanged(int)), chart, SLOT(displayPointComponentY(int)));
  pointChartOptionLayout2->addWidget(pointChartYCheckBox);
  pointChartOptionLayout2->addSpacing(spacing);
  ChartPointComponentCheckBox* pointChartZCheckBox = new ChartPointComponentCheckBox("z", pointChartOptionPage);
  pointChartZCheckBox->setToolTip("Display Z component");
  pointChartZCheckBox->setCheckState(Qt::Checked);
  connect(pointChartZCheckBox, SIGNAL(stateChanged(int)), chart, SLOT(displayPointComponentZ(int)));
  pointChartOptionLayout2->addWidget(pointChartZCheckBox);
  pointChartOptionLayout->addLayout(pointChartOptionLayout2);
  pointChartOptionLayout->addStretch(1);
  ChartOptionPushButton* pointChartOptionButton = new ChartOptionPushButton(pointChartOptionPage);
  connect(pointChartOptionButton, SIGNAL(clicked()), this, SLOT(toggleChartOptions()));
  pointChartOptionLayout->addWidget(pointChartOptionButton);
  this->optionStack->addWidget(pointChartOptionPage);
  // - Analogs' chart
  QWidget* analogChartOptionPage = new QWidget;
  QHBoxLayout* analogChartOptionLayout = new QHBoxLayout(analogChartOptionPage);
  analogChartOptionLayout->setContentsMargins(0,0,0,0);
  analogChartOptionLayout->setSpacing(0);
  QHBoxLayout* analogChartOptionLayout2 = new QHBoxLayout; // Second layout to fix the size and position of the button for the options
  analogChartOptionLayout2->setContentsMargins(0,0,0,0);
  HeaderOptionComboBox* expandableComboBox = new HeaderOptionComboBox(this);
  lw = new QListWidget(this);
  lw->addItem(new QListWidgetItem("Collapsed"));
  lw->addItem(new QListWidgetItem("Expanded"));
  expandableComboBox->setModel(lw->model());
  expandableComboBox->setView(lw);
  connect(expandableComboBox, SIGNAL(currentIndexChanged(int)), chart, SLOT(setExpandableAnalog(int)));
  analogChartOptionLayout2->addWidget(expandableComboBox);
  analogChartOptionLayout->addLayout(analogChartOptionLayout2);
  analogChartOptionLayout->addStretch(1);
  ChartOptionPushButton* analogChartOptionButton = new ChartOptionPushButton(analogChartOptionPage);
  connect(analogChartOptionButton, SIGNAL(clicked()), this, SLOT(toggleChartOptions()));
  analogChartOptionLayout->addWidget(analogChartOptionButton);
  this->optionStack->addWidget(analogChartOptionPage);
  // - Logger (No need of a new empty widget)
  // - Video (No need of a new empty widget)
  
  this->optionStack->setCurrentIndex(0);
};

// --------------------------------------------------------

HeaderOptionComboBox::HeaderOptionComboBox(QWidget* parent)
: QComboBox(parent)
{
  this->setStyleSheet(
  "QComboBox { \
     color: white; \
     border-top: none; \
     border-bottom: none; \
     border-left-width: 1px; \
     border-left-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 transparent, stop:0.5 rgba(200, 200, 200, 255), stop:1 transparent); \
     border-left-style: solid; \
     border-right-width: 1px; \
     border-right-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 transparent, stop:0.5 rgba(200, 200, 200, 255), stop:1 transparent); \
     border-right-style: solid; \
     padding-left: 10px; \
     padding-right: 10px; \
   } \
   QComboBox:editable { \
     background: white; \
   } \
   QComboBox:!editable, QComboBox::drop-down:editable { \
     color: white; \
     background: transparent; \
     selection-background-color: lightgray; \
   } \
   QComboBox:!editable:on, QComboBox::drop-down:editable:on { \
     color: black; \
   } \
   QComboBox:on { \
   } \
   QComboBox::drop-down { \
     subcontrol-origin: padding; \
     subcontrol-position: top left; \
     width: 20px; \
     border-right-width: 1px; \
     border-right-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 transparent, stop:0.5 rgba(200, 200, 200, 255), stop:1 transparent); \
     border-right-style: solid; \
   } \
   QComboBox::down-arrow { \
     image: url(:/Resources/Images/treeDownTriangleWhite.png); \
   } \
   QComboBox::down-arrow:on { \
   }");
}

// --------------------------------------------------------

ChartPointComponentCheckBox::ChartPointComponentCheckBox(const QString& text, QWidget* parent)
: QCheckBox(text, parent)
{
  this->setStyleSheet(
  "QCheckBox { \
     color: white; \
   } \
   QCheckBox::indicator:unchecked { \
       image: url(:/Resources/Images/checkbox_unchecked-13.png); \
   } \
   QCheckBox::indicator:unchecked:hover { \
       image: url(:/Resources/Images/checkbox_unchecked-hover-13.png); \
   } \
   QCheckBox::indicator:unchecked:pressed { \
       image: url(:/Resources/Images/checkbox_unchecked-pressed-13.png); \
   } \
   QCheckBox::indicator:checked { \
       image: url(:/Resources/Images/checkbox_checked-13.png); \
   } \
   QCheckBox::indicator:checked:hover { \
       image: url(:/Resources/Images/checkbox_checked-hover-13.png); \
   } \
   QCheckBox::indicator:checked:pressed { \
       image: url(:/Resources/Images/checkbox_checked-pressed-13.png); \
   }");
};

// --------------------------------------------------------

ChartOptionPushButton::ChartOptionPushButton(QWidget* parent)
: QPushButton(parent)
{
  this->setMaximumWidth(20);
  this->setFlat(true);
  this->setText("");
  this->setStyleSheet("QPushButton {\n     image: url(:/Resources/Images/option_editor-16.png);\nbackground-color: transparent;\n}\n\nQPushButton:pressed {\n     image: url(:/Resources/Images/option_editor-down-16.png);\n}\n\nQPushButton:flat {\n     border: none;\n}");
  this->setToolTip("Chart options");
};
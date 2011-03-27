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

#include "CompositeView.h"
#include "Viz3DWidget.h"
#include "Acquisition.h"

#include <btkMacro.h>
#include <btkVTKInteractorStyleTrackballFixedUpCamera.h>

#include <vtkCamera.h>
#include <vtkCollectionIterator.h>
#include <vtkRenderWindow.h>

#include <QListWidget>
#include <QMouseEvent>
#include <QVBoxLayout>

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
  static_cast<Viz3DWidget*>(this->viewStack->widget(Viz3D))->setAcquisition(acq);
};

void CompositeView::render()
{
  // Viz3D
  static_cast<Viz3DWidget*>(this->viewStack->widget(Viz3D))->GetRenderWindow()->Render();
};

void CompositeView::show(bool s)
{
  // Viz3D
  static_cast<Viz3DWidget*>(this->viewStack->widget(Viz3D))->show(s);
}

AbstractView* CompositeView::clone() const
{
  CompositeView* sv = new CompositeView;
  //  Copy the acquisition pointer
  Viz3DWidget* sourceViz3D = static_cast<Viz3DWidget*>(this->viewStack->widget(Viz3D));
  Viz3DWidget* targetViz3D = static_cast<Viz3DWidget*>(sv->viewStack->widget(Viz3D));
  targetViz3D->setAcquisition(sourceViz3D->acquisition());
  // Clone the 3D view
  // Add vtkViewProp to the new QVtkDialogWidget
  vtkRenderer* sourceRenderer = sourceViz3D->renderer();
  vtkRenderer* targetRenderer = targetViz3D->renderer();
  vtkCollectionIterator* it = sourceRenderer->GetViewProps()->NewIterator();
  it->InitTraversal();
  while (!it->IsDoneWithTraversal())
  {
    targetRenderer->AddViewProp(static_cast<vtkProp*>(it->GetCurrentObject()));
    it->GoToNextItem();
  }
  // Copy camera orientation
  vtkCamera* sourceCamera = sourceRenderer->GetActiveCamera();
  vtkCamera* targetCamera = targetRenderer->GetActiveCamera();
  targetCamera->SetPosition(sourceCamera->GetPosition()); 
  targetCamera->SetFocalPoint(sourceCamera->GetFocalPoint()); 
  targetCamera->SetViewUp(sourceCamera->GetViewUp());
  targetCamera->SetViewAngle(sourceCamera->GetViewAngle());
  targetRenderer->ResetCamera();
  // FIXME: Add a zoom member or static variable in Viz3DWidget.h
  targetCamera->Zoom(1.6);
  
  return sv;
};

void CompositeView::copyOptions(CompositeView* from)
{
  if (!from)
    return;

  switch (from->viewCombo->currentIndex())
  {
  case Viz3D: // Impossible
    break;
  case Viz3DProjection:
    this->optionStack->setCurrentIndex(0);
    break;
  case Viz3DOrthogonal:
    this->optionStack->setCurrentIndex(1);
    static_cast<QComboBox*>(this->optionStack->currentWidget())->setCurrentIndex(static_cast<QComboBox*>(from->optionStack->currentWidget())->currentIndex());
    static_cast<Viz3DWidget*>(this->viewStack->widget(Viz3D))->copyProjectionCameraConfiguration(static_cast<Viz3DWidget*>(from->viewStack->widget(Viz3D)));
    break;
  case Graph: // Impossible
    break;
  case GraphPoint:
    break;
  case GraphAnalogChannel:
    break;
  case GraphForcePlatform:
    break;
  default: // Impossible
    break;
  }
};

void CompositeView::setOrthogonalView(int view)
{
  if (view == -1)
    return;
  Viz3DWidget* viz3D = static_cast<Viz3DWidget*>(this->viewStack->widget(Viz3D));
  viz3D->setOrthogonalView(view);
  this->viewStack->currentWidget()->setFocus(Qt::OtherFocusReason);
};

void CompositeView::toggleGraphedMeasure(int /* idx */)
{
  //qDebug("CompositeView::toggleGraphedMeasure");
  //GraphAnalogWidget* analog = static_cast<GraphAnalogWidget*>(this->viewStack->widget(2));
  //analog->setXAxisRange(1,281,1200/60);
  //analog->addAnalogChannel(btk::Analog::New("Test", 5620));
  //analog->show();
};

void CompositeView::clearGraph()
{
  /*
  for (int i = 0 ; i < this->funcCombo->model()->rowCount() ; ++i)
  {
    QModelIndex index = this->funcCombo->model()->index(i,0);
    if ((this->funcCombo->model()->flags(index) & Qt::ItemIsUserCheckable)
        && (this->funcCombo->model()->flags(index) & Qt::ItemIsSelectable))
      this->funcCombo->model()->setData(index, Qt::Unchecked, Qt::CheckStateRole);
  }
  */
};
/*
bool CompositeView::eventFilter(QObject* object, QEvent* event)
{
  if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonRelease)
  {
    QMouseEvent* m = static_cast<QMouseEvent*>(event); 
    QModelIndex index = this->funcCombo->view()->indexAt(m->pos());
    QRect vrect = this->funcCombo->view()->visualRect(index);

    if ((event->type() == QEvent::MouseButtonPress)
        && (this->funcCombo->model()->flags(index) & Qt::ItemIsUserCheckable)
        && (this->funcCombo->model()->flags(index) & Qt::ItemIsSelectable)
        && vrect.contains(m->pos()))
    {
      if (this->funcCombo->model()->data(index, Qt::CheckStateRole).toInt() == Qt::Checked)
        this->funcCombo->model()->setData(index, Qt::Unchecked, Qt::CheckStateRole);
      else 
        this->funcCombo->model()->setData(index, Qt::Checked, Qt::CheckStateRole);
      this->toggleGraphedMeasure(index.row());
    }
    else if ((event->type() == QEvent::MouseButtonRelease)
            && ((this->funcCombo->model()->flags(index) & Qt::ItemIsUserCheckable) != Qt::ItemIsUserCheckable))
    {
      this->clearGraph();
      this->funcCombo->setCurrentIndex(1);
      return false;
    }
    return true;
  }
  this->funcCombo->setCurrentIndex(1);
  return QObject::eventFilter(object, event);
};
*/

int CompositeView::optionStackIndexFromViewComboIndex(int idx) const
{
  switch (idx)
  {
  case Viz3D: // Impossible
  case Viz3DProjection:
    return 0;
  case Viz3DOrthogonal:
    return 1;
  case Graph:
  case GraphPoint:
  case GraphAnalogChannel:
  case GraphForcePlatform:
  default:
    return 0;
  }
};

int CompositeView::viewStackIndexFromViewComboIndex(int idx) const
{
  switch (idx)
  {
  case Viz3D: // Impossible
    qDebug("Incorrect view index! Impossible to find the corresponding stack!");
    return 0;
  case Viz3DProjection:
  case Viz3DOrthogonal:
    return 0;
  case Graph:
    qDebug("Incorrect view index! Impossible to find the corresponding stack!");
    return 0;
  case GraphPoint:
    return 1;
  case GraphAnalogChannel:
    return 2;
  case GraphForcePlatform:
    return 3;
  default:
    qDebug("Incorrect view index! Impossible to find the corresponding stack!");
    return 0;
  }
};

void CompositeView::finalizeView(int idx)
{
  //disconnect(this->funcCombo, 0, 0, 0);
  Viz3DWidget* viz3D = static_cast<Viz3DWidget*>(this->viewStack->widget(Viz3D));
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
    //connect(this->funcCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(setOrthogonalView(int)));
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
  
  // Widget in the function stack
  // - 3D View
  this->optionStack->addWidget(new QWidget(this)); // empty
  // - Perspective 3D view
  QComboBox* orthogonalComboBox = new QComboBox(this);
  orthogonalComboBox->setStyleSheet("QComboBox {\n  color: white;\n  border-top: none;\n  border-bottom: none;\n  border-left-width: 1px;\n  border-left-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 transparent, stop:0.5 rgba(200, 200, 200, 255), stop:1 transparent);\n  border-left-style: solid;\n  border-right-width: 1px;\n  border-right-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 transparent, stop:0.5 rgba(200, 200, 200, 255), stop:1 transparent);\n  border-right-style: solid;\n  padding-left: 10px;\n  padding-right: 10px;\n}\n\nQComboBox:editable {\n  background: white;\n}\n\nQComboBox:!editable, QComboBox::drop-down:editable {\n  color: black;\n  background: transparent;\n  selection-background-color: lightgray;\n}\n\n/* QComboBox gets the \"on\" state when the popup is open */\nQComboBox:!editable:on, QComboBox::drop-down:editable:on {\n  color: black;\n}\n\nQComboBox:on { /* shift the text when the popup opens */\n  /* padding-top: 3px;\n  padding-left: 4px; */\n}\n\nQComboBox::drop-down {\n  subcontrol-origin: padding;\n  subcontrol-position: top left;\n  width: 20px;\n\n  border-right-width: 1px;\n  border-right-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 transparent, stop:0.5 rgba(200, 200, 200, 255), stop:1 transparent);\n  border-right-style: solid;\n}\n\nQComboBox::down-arrow {\n  image: url(:/Resources/Images/treeDownTriangleWhite.png);\n}\n\nQComboBox::down-arrow:on { /* shift the arrow when popup is open */\n /* top: 1px; */\n}");
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
  
  this->optionStack->setCurrentIndex(0);
};
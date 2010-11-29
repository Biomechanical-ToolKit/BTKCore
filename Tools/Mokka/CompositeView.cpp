/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2010, Arnaud Barré
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
  //this->setCurrentIndex(Viz3DProjection);
  this->viewCombo->setCurrentIndex(Viz3DProjection); // Adapt the index of the stack widget
};

void CompositeView::setAcquisition(Acquisition* acq)
{
  static_cast<Viz3DWidget*>(this->stackedWidget->widget(Viz3D))->setAcquisition(acq);
};

void CompositeView::render()
{
  // Viz3D
  static_cast<Viz3DWidget*>(this->stackedWidget->widget(Viz3D))->GetRenderWindow()->Render();
};

void CompositeView::show(bool s)
{
  // Viz3D
  static_cast<Viz3DWidget*>(this->stackedWidget->widget(Viz3D))->show(s);
}

CompositeView* CompositeView::clone() const
{
  CompositeView* sv = new CompositeView;
  //  Copy the acquisition pointer
  Viz3DWidget* sourceViz3D = static_cast<Viz3DWidget*>(this->stackedWidget->widget(Viz3D));
  Viz3DWidget* targetViz3D = static_cast<Viz3DWidget*>(sv->stackedWidget->widget(Viz3D));
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

void CompositeView::saveCurrentFuncOption(int idx)
{
  QListWidget* lw = qobject_cast<QListWidget*>(this->funcCombo->view());
  if (lw)
  {
    if (idx == Viz3DOrthogonal)
      this->m_FuncOptions[idx].currentIndex = this->funcCombo->currentIndex();
    else if ((idx == GraphPoint) || (idx == GraphAnalogChannel) || (idx == GraphForcePlatform))
    {    
      for (int i = 0 ; i < lw->count() ; ++i)
      {
        if ((lw->item(i)->flags() & Qt::ItemIsUserCheckable) == Qt::ItemIsUserCheckable)
          this->m_FuncOptions[idx].lw->item(i)->setCheckState(lw->item(i)->checkState());
      }
    }
  }
};

void CompositeView::setOrthogonalView(int view)
{
  Viz3DWidget* viz3D = static_cast<Viz3DWidget*>(this->stackedWidget->widget(Viz3D));
  vtkCamera* cam = viz3D->renderer()->GetActiveCamera();
  cam->SetPosition(0.0,0.0,1.0);
  cam->SetFocalPoint(0.0,0.0,0.0);
  cam->SetViewUp(0.0,1.0,0.0);
  cam->SetRoll(0.0);
  switch (view)
  {
  case Top:
    cam->SetPosition(0.0,0.0,1.0);
    break;
  case Bottom:
    cam->SetPosition(0.0,0.0,-1.0);
    cam->SetRoll(180.0);
    break;
  case Left:
    cam->SetPosition(-1.0,0.0,0.0);
    cam->SetRoll(90.0);
    break;
  case Right:
    cam->SetPosition(1.0,0.0,0.0);
    cam->SetRoll(-90.0);
    break;
  case Back:
    cam->SetPosition(0.0,1.0,0.0);
    cam->SetViewUp(0.0,0.0,1.0);
    break;
  case Front:
    cam->SetPosition(0.0,-1.0,0.0);
    cam->SetViewUp(0.0,0.0,1.0);
    break;
  default:
    btkErrorMacro("Unknown orthogonal view.");
  }
  viz3D->renderer()->ResetCamera();
  cam->Zoom(1.6);
  //viz3D->GetRenderWindow()->Render();
  this->stackedWidget->currentWidget()->setFocus(Qt::OtherFocusReason);
};

void CompositeView::toggleGraphedMeasure(int /* idx */)
{
  //qDebug("CompositeView::toggleGraphedMeasure");
  //GraphAnalogWidget* analog = static_cast<GraphAnalogWidget*>(this->stackedWidget->widget(2));
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

int CompositeView::stackIndexFromViewComboIndex(int idx) const
{
  switch (idx)
  {
  case Viz3D: // Impossible
    btkErrorMacro("Incorrect view index! Impossible to find the corresponding stack!");
    return 0;
  case Viz3DProjection:
  case Viz3DOrthogonal:
    return 0;
  case Graph:
    btkErrorMacro("Incorrect view index! Impossible to find the corresponding stack!");
    return 0;
  case GraphPoint:
    return 1;
  case GraphAnalogChannel:
    return 2;
  case GraphForcePlatform:
    return 3;
  default:
    btkErrorMacro("Incorrect view index! Impossible to find the corresponding stack!");
    return 0;
  }
};

void CompositeView::finalizeView(int idx)
{
  disconnect(this->funcCombo, 0, 0, 0);
  Viz3DWidget* viz3D = static_cast<Viz3DWidget*>(this->stackedWidget->widget(Viz3D));
  btk::VTKInteractorStyleTrackballFixedUpCamera* style = static_cast<btk::VTKInteractorStyleTrackballFixedUpCamera*>(viz3D->GetRenderWindow()->GetInteractor()->GetInteractorStyle());
  vtkCamera* cam = viz3D->renderer()->GetActiveCamera();
  if (idx == Viz3DProjection)
  {
    if (cam->GetParallelProjection() != 0)
    {
      cam->ParallelProjectionOff();
      style->RotationEnabledOn();
      style->SpinEnabledOn();
    }
  }
  else if (idx == Viz3DOrthogonal)
  {
    connect(this->funcCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(setOrthogonalView(int)));
    cam->ParallelProjectionOn();
    style->RotationEnabledOff();
    style->SpinEnabledOff();
  }
};

void CompositeView::finalizeUi()
{
  QFont f;
  // View combo
  QListWidget* lw = new QListWidget(this);
  QListWidgetItem* lwi;
  // 3D View
  lwi = new QListWidgetItem(tr("3D View"));
  lwi->setFlags(lwi->flags() & ~Qt::ItemIsSelectable);
  f = this->funcCombo->font();
  f.setBold(true);
  lwi->setFont(f);
  lw->addItem(lwi);
  // - Perspective
  lwi = new QListWidgetItem(tr("  Perspective")); lw->addItem(lwi);
  // - Orthogonal
  lwi = new QListWidgetItem(tr("  Orthogonal")); lw->addItem(lwi);
  // Update the model & view
  this->viewCombo->blockSignals(true);
  this->viewCombo->setModel(lw->model());
  this->viewCombo->setView(lw);
  this->viewCombo->blockSignals(false);
  
  // (Fake) Func combo option for the 3D view
  AbstractView::FuncOption opt3DView;
  opt3DView.filtered = false;
  opt3DView.lw = 0;
  opt3DView.currentIndex = -1;
  this->m_FuncOptions.append(opt3DView);
  // (Empty) Func combo option for the perspective 3D view
  AbstractView::FuncOption opt3DViewPerspective;
  opt3DViewPerspective.filtered = false;
  opt3DViewPerspective.lw = 0;
  opt3DViewPerspective.currentIndex = -1;
  this->m_FuncOptions.append(opt3DViewPerspective);
  // Func combo option for the perspective 3D view
  AbstractView::FuncOption opt3DViewOrthogonal;
  opt3DViewOrthogonal.filtered = false;
  opt3DViewOrthogonal.lw = new QListWidget;
  opt3DViewOrthogonal.currentIndex = Front;
  // - Top
  lwi = new QListWidgetItem("Top");
  opt3DViewOrthogonal.lw->addItem(lwi);
  // - Bottom
  lwi = new QListWidgetItem("Bottom");
  opt3DViewOrthogonal.lw->addItem(lwi);
  // - Left
  lwi = new QListWidgetItem("Left");
  opt3DViewOrthogonal.lw->addItem(lwi);
  // - Right
  lwi = new QListWidgetItem("Right");
  opt3DViewOrthogonal.lw->addItem(lwi);
  // - Back
  lwi = new QListWidgetItem("Back");
  opt3DViewOrthogonal.lw->addItem(lwi);
  // - Front
  lwi = new QListWidgetItem("Front");
  opt3DViewOrthogonal.lw->addItem(lwi);
  // Appending
  this->m_FuncOptions.append(opt3DViewOrthogonal);
  
  // Widget in the stack
  // 3D View
  Viz3DWidget* viz3d = new Viz3DWidget(this);
  this->stackedWidget->addWidget(viz3d);
  viz3d->initialize();
};
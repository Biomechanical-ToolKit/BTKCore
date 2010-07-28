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

#include <vtkCamera.h>
#include <vtkCollectionIterator.h>
#include <vtkRenderWindow.h>

CompositeView::CompositeView(QWidget* parent)
: AbstractView(parent)
{
  this->finalizeUi();
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
  // Graph
}

CompositeView* CompositeView::clone() const
{
  CompositeView* sv = new CompositeView;
  
  // Clone the 3D view
  // Add vtkViewProp to the new QVtkDialogWidget
  vtkRenderer* sourceRenderer = static_cast<Viz3DWidget*>(this->stackedWidget->widget(Viz3D))->renderer();
  vtkRenderer* targetRenderer = static_cast<Viz3DWidget*>(sv->stackedWidget->widget(Viz3D))->renderer();
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

void CompositeView::finalizeUi()
{
  this->viewCombo->addItem(tr("3D View"));
  //this->viewCombo->addItem(tr("Graph"));
  
  // 3D View
  Viz3DWidget* vtk3dView = new Viz3DWidget(this);
  this->stackedWidget->addWidget(vtk3dView);
  vtk3dView->initialize();
};
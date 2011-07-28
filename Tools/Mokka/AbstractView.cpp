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

#include "AbstractView.h"

AbstractView::AbstractView(QWidget* parent)
: QWidget(parent)
{
  this->setupUi(this);
#ifdef Q_OS_MAC
  // To have the same height than the title bar used by the dock widgets.
  this->separatorFuncButtons->setMinimumHeight(26); 
  this->separatorFuncButtons->setMaximumHeight(26); 
#endif
  
  // Connections
  connect(this->hSplitButton, SIGNAL(clicked()), this, SLOT(splitHorizontally()));
  connect(this->vSplitButton, SIGNAL(clicked()), this, SLOT(splitVertically()));
  connect(this->closeButton, SIGNAL(clicked()), this, SLOT(close()));
  connect(this->viewCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(setCurrentIndex(int)));
};

void AbstractView::setFocus(Qt::FocusReason reason)
{
  if (this->viewStack->currentWidget())
    this->viewStack->currentWidget()->setFocus(reason);
};

void AbstractView::setCurrentIndex(int idx)
{
  this->adaptLayoutStrech(idx);
  int stackIndex = this->optionStackIndexFromViewComboIndex(idx);
  if (stackIndex < this->optionStack->count())
    this->optionStack->setCurrentIndex(stackIndex);
  stackIndex = this->viewStackIndexFromViewComboIndex(idx);
  if (stackIndex < this->viewStack->count())
  {
    this->viewStack->setCurrentIndex(stackIndex);
    this->viewStack->currentWidget()->setFocus(Qt::OtherFocusReason);
    this->finalizeView(idx);
  }
};

void AbstractView::close()
{
  emit this->closeTriggered(this);
};

void AbstractView::splitHorizontally()
{
  emit this->splitTriggered(this, Qt::Horizontal);
};

void AbstractView::splitVertically()
{
  emit this->splitTriggered(this, Qt::Vertical);
};
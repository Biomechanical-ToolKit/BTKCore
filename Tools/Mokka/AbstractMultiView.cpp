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

#include "AbstractMultiView.h"
#include "AbstractView.h"

#include <QDateTime>
#include <QSplitter>

AbstractMultiView::AbstractMultiView(QWidget* parent)
: QWidget(parent), m_Views()
{
  this->mp_Prototype = 0;
  this->setupUi();
};

void AbstractMultiView::initialize()
{
  // UI init
  AbstractView* sv = this->createView();
  static_cast<QGridLayout*>(this->layout())->addWidget(sv, 0, 0);
  sv->closeButton->setEnabled(false);
  this->m_Views.append(sv);
};

void AbstractMultiView::setViewPrototype(AbstractView *(*view)(QWidget*))
{
  this->mp_Prototype = view;
};

void AbstractMultiView::closeAll()
{
  this->closeAll(static_cast<QGridLayout*>(this->layout())->itemAtPosition(0,0)->widget());
};

void AbstractMultiView::closeAll(QWidget* w)
{
  QSplitter* splitter = qobject_cast<QSplitter*>(w);
  if (splitter)
  {
    // Must be in this order as the widgets are inserted before to be deleted.
    this->closeAll(splitter->widget(1));
    this->closeAll(splitter->widget(0));
    this->close_(static_cast<AbstractView*>(splitter->widget(0)));
    delete splitter->widget(0);
    delete splitter;
  }
};

void AbstractMultiView::close(AbstractView* sender)
{
  this->close_(sender);
  sender->deleteLater();
  sender->parent()->deleteLater(); // splitter
};

void AbstractMultiView::close_(AbstractView* sender)
{
  this->setUpdatesEnabled(false);
  this->m_Views.removeOne(sender);
  bool lastView = false;
  if (sender->parent()->parent() == this)
    lastView = true;
  QSplitter* splitter = static_cast<QSplitter*>(sender->parent());
  QWidget* otherWidget = 0;
  if (splitter->indexOf(sender) == 0)
    otherWidget = splitter->widget(1);
  else
    otherWidget = splitter->widget(0);
  if (lastView)
  {
    static_cast<QGridLayout*>(this->layout())->addWidget(otherWidget, 0, 0);
    this->layout()->removeWidget(splitter);
  }
  else
  {
    QSplitter* parent = static_cast<QSplitter*>(splitter->parent());
    QList<int> sizes = parent->sizes();
    
    splitter->setVisible(false);
    parent->insertWidget(parent->indexOf(splitter), otherWidget);
    sizes.insert(parent->indexOf(splitter), 0);
    parent->setSizes(sizes);
  }
  
  AbstractView* sv = qobject_cast<AbstractView*>(otherWidget);
  if (sv)
  {
    sv->setFocus(Qt::OtherFocusReason);
    if (lastView) sv->closeButton->setEnabled(false);
  }
  else
  {
    splitter = 0;
    AbstractView* view = 0;
    while ((splitter = qobject_cast<QSplitter*>(otherWidget)) != 0)
      otherWidget = splitter->widget(0);
    if ((view = qobject_cast<AbstractView*>(otherWidget)) != 0)
      view->setFocus(Qt::OtherFocusReason);
    else
      qDebug("Unable to focus on the remaining widget");
  }
  this->setUpdatesEnabled(true);
};

void AbstractMultiView::split(AbstractView* sender, int direction)
{
  this->split_(sender, direction, 0);
};

QSplitter* AbstractMultiView::split_(AbstractView* sender, int direction, AbstractView** splittedViews)
{
  this->setUpdatesEnabled(false);
  AbstractView* sv = this->createView(sender);
  //sv->viewCombo->blockSignals(true);
  sv->viewCombo->setCurrentIndex(sender->viewCombo->currentIndex());
  //sv->viewCombo->blockSignals(false);
  //sv->setFocus(Qt::OtherFocusReason);
  this->m_Views.append(sv);
  
  QSplitter* splitter = 0;
  QList<int> sizes;
  QWidget* senderParent = static_cast<QWidget*>(sender->parent());
  if (direction == Qt::Horizontal)
  {
    splitter = new QSplitter(Qt::Horizontal, this);
    sizes << sender->width() / 2 << sender->width() / 2;
  }
  else if (direction == Qt::Vertical)
  {
    splitter = new QSplitter(Qt::Vertical, this);
    sizes << sender->height() / 2 << sender->height() / 2;
  }
  splitter->setHandleWidth(3);
  splitter->setObjectName("splitter_" + QDateTime::currentDateTime().toString(Qt::ISODate));
  splitter->setChildrenCollapsible(false);
  if (senderParent == this)
  {
    splitter->addWidget(sender);
    splitter->addWidget(sv);
    splitter->setSizes(sizes);
    sender->closeButton->setEnabled(true);
    static_cast<QGridLayout*>(this->layout())->addWidget(splitter,0,0);
  }
  else
  {
    QList<int> parentSizes = static_cast<QSplitter*>(senderParent)->sizes();
    int indexOfSender = static_cast<QSplitter*>(senderParent)->indexOf(sender);
    splitter->addWidget(sender);
    splitter->addWidget(sv);
    splitter->setSizes(sizes);
    static_cast<QSplitter*>(senderParent)->insertWidget(indexOfSender, splitter);
    static_cast<QSplitter*>(senderParent)->setSizes(parentSizes);
  }
  this->setUpdatesEnabled(true);
  // Focus for the sender
  sender->setFocus(Qt::OtherFocusReason);
  
  if (splittedViews)
  {
    splittedViews[0] = sender;
    splittedViews[1] = sv;
  }
  return splitter;
};

void AbstractMultiView::setupUi()
{
  if (this->objectName().isEmpty())
    this->setObjectName("AbstractMultiView");
  //this->resize(1024, 768);
  
  QGridLayout* gridLayout = new QGridLayout(this);
  gridLayout->setSpacing(0);
  gridLayout->setContentsMargins(0,0,0,0);
  gridLayout->setObjectName("GridLayout");
};

AbstractView* AbstractMultiView::createView(AbstractView* fromAnother)
{
  AbstractView* sv = 0;
  if (fromAnother)
    sv = fromAnother->clone();
  else
    sv = (this->mp_Prototype ? (*this->mp_Prototype)(this) : new AbstractView(this));
  sv->setObjectName("singleView_" + QDateTime::currentDateTime().toString(Qt::ISODate));
  connect(sv, SIGNAL(closeTriggered(AbstractView*)), this, SLOT(close(AbstractView*)));
  connect(sv, SIGNAL(splitTriggered(AbstractView*, int)), this, SLOT(split(AbstractView*, int)));
  
  // Focus
  sv->setFocus(Qt::OtherFocusReason);
  
  return sv;
};
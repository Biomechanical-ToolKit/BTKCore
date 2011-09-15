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

#ifndef AbstractMultiView_h
#define AbstractMultiView_h

#include <QWidget>
#include <QList>

class AbstractView;
class QSplitter;
class QGridLayout;

template <class T> AbstractView* ViewFactory(QWidget* parent = 0) {return new T(parent);};

class AbstractMultiView : public QWidget
{
  Q_OBJECT
  
public:
  AbstractMultiView(QWidget* parent = 0);
  virtual ~AbstractMultiView() {};
  // AbstractMultiView(const AbstractMultiView&); // Implicit.
  // AbstractMultiView& operator=(const AbstractMultiView&); // Implicit.
  
  virtual void initialize();
  void setViewPrototype(AbstractView* (*view)(QWidget*));
  const QList<AbstractView*>& views() const {return this->m_Views;};
  
protected:
  virtual AbstractView* createView(AbstractView* fromAnother = 0);
  
public slots:
  void closeAll();
  
protected slots:
  void close(AbstractView* sender);
  void split(AbstractView* sender, int direction);
  
protected:
  QList<AbstractView*> m_Views;
  
private:
  void close_(AbstractView* sender);
  QSplitter* split_(AbstractView* sender, int direction, AbstractView** splittedViews);
  
  void setupUi();
  void closeAll(QWidget* w);
  
  AbstractView* (*mp_Prototype)(QWidget*);
};

#endif // AbstractMultiView_h
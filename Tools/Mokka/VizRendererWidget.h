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

#ifndef VizRendererWidget_h
#define VizRendererWidget_h

// To resolve some issues with the min/max function and Windows.
#define NOMINMAX

//#define MOKKA_USE_QVTKWIDGET2

#ifdef MOKKA_USE_QVTKWIDGET2
  #include <QVTKWidget2.h>
  #include <vtkGenericOpenGLRenderWindow.h>
  #include <vtkRenderWindowInteractor.h>
  typedef QVTKWidget2 QVTKWidgetX;
  typedef vtkGenericOpenGLRenderWindow VizRendererWindow;
#else 
  #include <QVTKWidget.h>
  
  #include <vtkRenderWindow.h>
  typedef QVTKWidget QVTKWidgetX;
  typedef vtkRenderWindow VizRendererWindow;
#endif

#ifdef MOKKA_USE_QVTKWIDGET2
  class VizRendererWidget : public QVTKWidgetX
  {
  public:
    VizRendererWidget(QWidget* parent = NULL, const QGLWidget* shareWidget=0, Qt::WindowFlags f = 0);
    VizRendererWidget(QGLContext* ctx, QWidget* parent = NULL, const QGLWidget* shareWidget=0, Qt::WindowFlags f = 0);
    VizRendererWidget(const QGLFormat& fmt, QWidget* parent = NULL, const QGLWidget* shareWidget=0, Qt::WindowFlags f = 0);
  #if ((VTK_MAJOR_VERSION == 5) && (VTK_MINOR_VERSION <= 8))
  /* This is to fix the bugs between VTK 5.8 and VTK 5.10 for the class QVTKWidget2 */
    virtual void SetRenderWindow(vtkGenericOpenGLRenderWindow* w);  
  protected:
    virtual bool focusNextPrevChild(bool );
  #endif
  };
#else
  class VizRendererWidget : public QVTKWidgetX
  {
  public:
    VizRendererWidget(QWidget* parent = NULL, void* onlyforCompatibility=0, Qt::WindowFlags f = 0);
  #if ((VTK_MAJOR_VERSION == 5) && (VTK_MINOR_VERSION <= 10))
  protected:
    #if defined(Q_WS_WIN)
      bool winEvent(MSG* msg, long* result);
    #endif
  #endif
  };
#endif

#endif // VizRendererWidget_h
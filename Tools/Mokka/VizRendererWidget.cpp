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

#include "VizRendererWidget.h"

#ifdef MOKKA_USE_QVTKWIDGET2

  #include <vtkEventQtSlotConnect.h>
  #include <vtkInteractorStyleTrackballCamera.h>
  #include <QVTKInteractorAdapter.h>
  #include <QVTKInteractor.h>

  VizRendererWidget::VizRendererWidget(QWidget* parent, const QGLWidget* shareWidget, Qt::WindowFlags f)
  : QVTKWidgetX(parent, shareWidget, f)
  {};

  VizRendererWidget::VizRendererWidget(QGLContext* ctx, QWidget* parent, const QGLWidget* shareWidget, Qt::WindowFlags f)
  : QVTKWidgetX(ctx, parent, shareWidget, f)
  {};

  VizRendererWidget::VizRendererWidget(const QGLFormat& fmt, QWidget* parent, const QGLWidget* shareWidget, Qt::WindowFlags f)
  : QVTKWidgetX(fmt, parent, shareWidget, f)
  {};

  #if ((VTK_MAJOR_VERSION == 5) && (VTK_MINOR_VERSION <= 8))

  // Fix: "BUG:  Fix bug leaking OpenGL resources when actors are removed"
  // http://vtk.org/gitweb?p=VTK.git;a=commit;h=02aac437718c9f61f2177b2c6c6a229ccc673fc3
  void VizRendererWidget::SetRenderWindow(vtkGenericOpenGLRenderWindow* w)
  {
    // do nothing if we don't have to
    if(w == this->mRenWin)
      {
      return;
      }

    // unregister previous window
    if(this->mRenWin)
      {
      this->mRenWin->Finalize();
      this->mRenWin->SetMapped(0);
      mConnect->Disconnect(mRenWin, vtkCommand::WindowMakeCurrentEvent, this, SLOT(MakeCurrent()));
      mConnect->Disconnect(mRenWin, vtkCommand::WindowIsCurrentEvent, this, SLOT(IsCurrent(vtkObject*, unsigned long, void*, void*)));
      mConnect->Disconnect(mRenWin, vtkCommand::WindowFrameEvent, this, SLOT(Frame()));
      mConnect->Disconnect(mRenWin, vtkCommand::StartEvent, this, SLOT(Start()));
      mConnect->Disconnect(mRenWin, vtkCommand::EndEvent, this, SLOT(End()));
      mConnect->Disconnect(mRenWin, vtkCommand::WindowIsDirectEvent, this, SLOT(IsDirect(vtkObject*, unsigned long, void*, void*)));
      mConnect->Disconnect(mRenWin, vtkCommand::WindowSupportsOpenGLEvent, this, SLOT(SupportsOpenGL(vtkObject*, unsigned long, void*, void*)));
      }

    // now set the window
    this->mRenWin = w;

    if(this->mRenWin)
      {
      // if it is mapped somewhere else, unmap it
      this->mRenWin->Finalize();
      this->mRenWin->SetMapped(1);

      // tell the vtk window what the size of this window is
      this->mRenWin->SetSize(this->width(), this->height());
      this->mRenWin->SetPosition(this->x(), this->y());

      // if an interactor wasn't provided, we'll make one by default
      if(!this->mRenWin->GetInteractor())
        {
        // create a default interactor
        QVTKInteractor* iren = QVTKInteractor::New();
        iren->SetUseTDx(this->UseTDx);
        this->mRenWin->SetInteractor(iren);
        iren->Initialize();

        // now set the default style
        vtkInteractorStyle* s = vtkInteractorStyleTrackballCamera::New();
        iren->SetInteractorStyle(s);

        iren->Delete();
        s->Delete();
        }

      // tell the interactor the size of this window
      this->mRenWin->GetInteractor()->SetSize(this->width(), this->height());

      mConnect->Connect(mRenWin, vtkCommand::WindowMakeCurrentEvent, this, SLOT(MakeCurrent()));
      mConnect->Connect(mRenWin, vtkCommand::WindowIsCurrentEvent, this, SLOT(IsCurrent(vtkObject*, unsigned long, void*, void*)));
      mConnect->Connect(mRenWin, vtkCommand::WindowFrameEvent, this, SLOT(Frame()));
      mConnect->Connect(mRenWin, vtkCommand::StartEvent, this, SLOT(Start()));
      mConnect->Connect(mRenWin, vtkCommand::EndEvent, this, SLOT(End()));
      mConnect->Connect(mRenWin, vtkCommand::WindowIsDirectEvent, this, SLOT(IsDirect(vtkObject*, unsigned long, void*, void*)));
      mConnect->Connect(mRenWin, vtkCommand::WindowSupportsOpenGLEvent, this, SLOT(SupportsOpenGL(vtkObject*, unsigned long, void*, void*)));
      }
  };

  // Fix: "qvtk: fix handling of tab key in QVTKWidget2."
  // http://vtk.org/gitweb?p=VTK.git;a=commit;h=f6d1f2de84fa85dd9aac1ec3cdb8f0d866589256
  bool VizRendererWidget::focusNextPrevChild(bool )
  {
    return false;
  };
  
  #endif
  
#else
  VizRendererWidget::VizRendererWidget(QWidget* parent, void* onlyforCompatibility, Qt::WindowFlags f)
  : QVTKWidgetX(parent, f)
  {
    Q_UNUSED(onlyforCompatibility);
  };
  
  // Fix: "Fix QVTKWidget problem on Windows with Aero off."
  // http://vtk.org/gitweb?p=VTK.git;a=commit;h=c5d4f4c2b904821247729bfa67e1cab8c74e5d36
  // Combined with another patch "fixAeroIssueOnlyOnWindows7.patch" (even for VTK 5.10)
  #if defined(Q_WS_WIN)
  #ifdef Q_OS_WIN
    #ifndef WIN32_LEAN_AND_MEAN
      #define WIN32_LEAN_AND_MEAN
    #endif
    #include <windows.h>
  #endif
  #include <QSysInfo>
  bool VizRendererWidget::winEvent(MSG* msg, long*)
  {
    if(QSysInfo::windowsVersion() >= QSysInfo::WV_WINDOWS7) // Note: The fix was released only for Windows 7, but we assume it is the same for Windows 8.
    {
      if(msg->message == WM_PAINT)
        InvalidateRect(this->winId(), NULL, FALSE);
    }
    return false;
  }
  #endif
  
#endif
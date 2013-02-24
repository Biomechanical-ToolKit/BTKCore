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

#ifndef __btkVTKContextActor_h
#define __btkVTKContextActor_h

#include "btkConfigure.h"

// Only used by Doxygen... Should not be able to put this in the Doxygen configuration file?
#ifdef FIX_DOXYGEN_VTK_MISSING_MACRO_DEFINITIONS
  #define vtkGetObjectMacro(name,type) type* Get##name();
#endif

#include <vtkProp.h>

class vtkContext2D;
class vtkContextScene;

namespace btk
{
  class VTKContextActor : public vtkProp
  {
  public:
    vtkTypeMacro(VTKContextActor, vtkProp);
    
    BTK_VTK_EXPORT static VTKContextActor* New();
    
    BTK_VTK_EXPORT virtual ~VTKContextActor();
    
    BTK_VTK_EXPORT virtual int RenderOverlay(vtkViewport* viewport);
    BTK_VTK_EXPORT virtual void SetContext(vtkContext2D* context);
    vtkGetObjectMacro(Context, vtkContext2D);
    vtkGetObjectMacro(Scene, vtkContextScene);
    BTK_VTK_EXPORT virtual void SetScene(vtkContextScene* scene);
    BTK_VTK_EXPORT virtual void ReleaseGraphicsResources(vtkWindow* window);
    
    BTK_VTK_EXPORT void PrintSelf(ostream& os, vtkIndent indent);
    
  protected:
    BTK_VTK_EXPORT VTKContextActor();

    BTK_VTK_EXPORT virtual void Initialize(vtkViewport* viewport);
    
    vtkContextScene* Scene;
    vtkContext2D* Context;
    bool Initialized;
    
  private:
    VTKContextActor(const VTKContextActor& );  // Not implemented.
    void operator=(const VTKContextActor& );  // Not implemented.
  };
}


#endif  __btkVTKContextActor_h
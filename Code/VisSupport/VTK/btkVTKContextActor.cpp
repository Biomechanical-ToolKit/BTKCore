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

#include "btkVTKContextActor.h"

#include "btkVTKOpenGLContextDevice2D.h"
#include "btkVTKOpenGL2ContextDevice2D.h"
#include "btkMacro.h"

#include <vtkObjectFactory.h>
#include <vtkContext2D.h>
#include <vtkContextScene.h>
#include <vtkTransform2D.h>

#include <vtkViewport.h>
#include <vtkWindow.h>
#include <vtkOpenGLRenderer.h>
#include <vtkOpenGLRenderWindow.h>
#include <vtkOpenGLExtensionManager.h>

namespace btk
{
  /**
   * @class VTKContextActor btkVTKContextActor.h
   * @brief Provides the entry point for the vtkContextScene to be rendered in a vtkRenderer
   * 
   * Note: This is a copy of the class vtkContextActor but containing the fix for the issue #13199 (http://www.vtk.org/Bug/view.php?id=13199).
   */
  
  /**
   * @fn static VTKContextActor* VTKContextActor::New() 
   * Constructs a VTKContextActor object and return it as a pointer.
   */
  vtkStandardNewMacro(VTKContextActor);
  
  /**
   * virtual vtkContext2D* VTKContextActor::GetContext()
   * Returns the context object used by this actor.
   */
   
  /**
   * @fn virtual void VTKContextActor::SetContext(vtkContext2D* context);
   * Sets the context object for this actor.
   */
  vtkCxxSetObjectMacro(VTKContextActor, Context, vtkContext2D);
  
  /**
   * @fn virtual vtkContextScene* VTKContextActor::GetScene()
   * Returns the scene object used by this actor.
   */
   
  /**
   * @fn virtual void VTKContextActor::SetScene(vtkContextScene* scene);
   * Sets the scene object for this actor.
   */
  vtkCxxSetObjectMacro(VTKContextActor, Scene, vtkContextScene);
  
  /**
   * Constructor
   */
  VTKContextActor::VTKContextActor()
  {
    this->Context = vtkContext2D::New();
    this->Scene = vtkContextScene::New();
    this->Initialized = false;
  };
  
  /**
   * Destructor
   */
  VTKContextActor::~VTKContextActor()
  {
    if (this->Context)
    {
      this->Context->End();
      this->Context->Delete();
      this->Context = NULL;
    }

    if (this->Scene)
    {
      this->Scene->Delete();
      this->Scene = NULL;
    }
  };
  
  /**
   * Release any graphics resources that are being consumed by this actor.
   * The parameter window could be used to determine which graphic
   * resources to release.
   */
  void VTKContextActor::ReleaseGraphicsResources(vtkWindow* window)
  {
    vtkOpenGLContextDevice2D* device = vtkOpenGLContextDevice2D::SafeDownCast(this->Context->GetDevice());
    if (device)
      device->ReleaseGraphicsResources(window);

    if(this->Scene)
      this->Scene->ReleaseGraphicsResources();
  }

  /**
   * Renders an actor2D's property and then it's mapper.
   */
  int VTKContextActor::RenderOverlay(vtkViewport* viewport)
  {
    if (!this->Context)
    {
      vtkErrorMacro(<< "VTKContextActor::Render - No painter set");
      return 0;
    }

    // Need to figure out how big the window is, taking into account tiling...
    vtkWindow* window = viewport->GetVTKWindow();
    int scale[2];
    window->GetTileScale(scale);
    int size[2];
    size[0] = window->GetSize()[0];
    size[1] = window->GetSize()[1];

    int viewportInfo[4];
    viewport->GetTiledSizeAndOrigin( &viewportInfo[0], &viewportInfo[1], &viewportInfo[2], &viewportInfo[3] );

    // The viewport is in normalized coordinates, and is the visible section of
    // the scene.
    vtkTransform2D* transform = this->Scene->GetTransform();
    transform->Identity();
    if (scale[0] > 1 || scale[1] > 1)
    {
      // Tiled display - work out the transform required
      double* b = window->GetTileViewport();
      int box[] = { vtkContext2D::FloatToInt(b[0] * size[0]),
                    vtkContext2D::FloatToInt(b[1] * size[1]),
                    vtkContext2D::FloatToInt(b[2] * size[0]),
                    vtkContext2D::FloatToInt(b[3] * size[1]) };
      transform->Translate(-box[0], -box[1]);
      if (this->Scene->GetScaleTiles())
        transform->Scale(scale[0], scale[1]);
    }
    else if (viewportInfo[0] != size[0] || viewportInfo[1] != size[1] )
    {
      size[0]=viewportInfo[0];
      size[1]=viewportInfo[1];
    }

    if (!this->Initialized)
      this->Initialize(viewport);

    // This is the entry point for all 2D rendering.
    // First initialize the drawing device.
    this->Context->GetDevice()->Begin(viewport);
    this->Scene->SetGeometry(size);
    this->Scene->Paint(this->Context);
    this->Context->GetDevice()->End();

    return 1;
  }
  
  /**
   * Initialize the actor - right now we just decide which device to initialize.
   */
  void VTKContextActor::Initialize(vtkViewport* viewport)
  {
    vtkContextDevice2D* device = NULL;
    if (VTKOpenGL2ContextDevice2D::IsSupported(viewport))
      device = VTKOpenGL2ContextDevice2D::New();
    else
      device = VTKOpenGLContextDevice2D::New();
    if (device)
    {
      this->Context->Begin(device);
      device->Delete();
      this->Initialized = true;
    }
    else
    {
      // Failed
      btkErrorMacro("Error: failed to initialize the render device.")
    }
  };
  
  /**
   * Print the content of the object in the given output stream @a os.
   */
  void VTKContextActor::PrintSelf(ostream& os, vtkIndent indent)
  {
    this->Superclass::PrintSelf(os,indent);
    
    os << indent << "Context: " << this->Context << "\n";
    if (this->Context)
      this->Context->PrintSelf(os, indent.GetNextIndent());
  }
}
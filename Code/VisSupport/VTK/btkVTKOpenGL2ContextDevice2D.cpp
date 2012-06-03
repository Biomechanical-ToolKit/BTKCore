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
 
#include "btkVTKOpenGL2ContextDevice2D.h"

#ifdef VTK_USE_QT
  #include <QApplication>
  #include "btkVTKQtStringToImage.h"
#endif

#include <vtkFreeTypeStringToImage.h>
#include <vtkObjectFactory.h>
#include <vtkImageData.h>
#include <vtkTexture.h>
#include <vtkTextProperty.h>
#include <vtkVector.h>
#include <vtkStdString.h>

#include <vtkgl.h>

#include <vtkOpenGLContextDevice2DPrivate.h>

namespace btk
{
#if (((VTK_MAJOR_VERSION == 5) && (VTK_MINOR_VERSION >= 8)) || (VTK_MAJOR_VERSION >= 6))
  /**
   * @class VTKOpenGL2ContextDevice2D btkVTKOpenGL2ContextDevice2D.h
   * @brief Copy of the class vtkOpenGL2ContextDevice2D but with a detection of the use of Qt
   * The detection of Qt gives the possibility to set the text renderer in consequence as it was proposed with VTK 5.6.1.
   * If Qt is not detected, then a FreeType text renderer is used.
   *
   * The Qt Text renderer used is not the vtkQtStringToImage class but btk::VTKQtStringToImage.
   *
   * Note: this class fixes the issue #13199 (http://www.vtk.org/Bug/view.php?id=13199). It should be used only with VTK 5.8 and VTK 5.10.
   */

  /**
   * @fn static VTKOpenGL2ContextDevice2D* VTKOpenGL2ContextDevice2D::New()
   * Constructs a VTKOpenGL2ContextDevice2D object and return it as a pointer.
   */
  vtkStandardNewMacro(VTKOpenGL2ContextDevice2D);
  
  /**
   * Overload method to draw a string.
   */
  void VTKOpenGL2ContextDevice2D::DrawString(float* point, const vtkStdString& string)
  {
    float p[] = {floor(point[0]), floor(point[1])};
#if 0
    vtkImageData *image = vtkImageData::New();
    this->TextRenderer->RenderString(this->TextProp, string, image);
    float width = static_cast<float>(image->GetDimensions()[0]);
    float height = static_cast<float>(image->GetDimensions()[1]);
    this->AlignText(this->TextProp->GetOrientation(), width, height, p);
    this->DrawImage(p, 1.0, image);
    image->Delete();
#else
    // Cache rendered text strings
    vtkTextureImageCache<TextPropertyKey>::CacheData cache = this->Storage->TextTextureCache.GetCacheData(TextPropertyKey(this->TextProp, string));
    vtkImageData* image = cache.ImageData;
    if (image->GetNumberOfPoints() == 0 && image->GetNumberOfCells() == 0)
    {
      if (!this->TextRenderer->RenderString(this->TextProp, string, image))
        return;
    }
    vtkTexture* texture = cache.Texture;
    texture->Render(this->Renderer);

    float width = static_cast<float>(image->GetDimensions()[0]);
    float height = static_cast<float>(image->GetDimensions()[1]);

    float xw = static_cast<float>(image->GetSpacing()[0]);
    float xh = static_cast<float>(image->GetSpacing()[1]);

    this->AlignText(this->TextProp->GetOrientation(), width, height, p);

    float points[] = { p[0]        , p[1],
                       p[0] + width, p[1],
                       p[0] + width, p[1] + height,
                       p[0]        , p[1] + height };

    float texCoord[] = { 0.0, 0.0,
                         xw,  0.0,
                         xw,  xh,
                         0.0, xh };

    glColor4ub(255, 255, 255, 255);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, points);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoord);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    texture->PostRender(this->Renderer);
    glDisable(GL_TEXTURE_2D);
#endif
  };
  
  /**
   * Constructor choosing the text renderer to use (Qt renderer or FreeType renderer).
   * By default, this class uses a FreeType renderer but if VTK is compiled with the support of Qt and if there is QApplication instance, then the Qt renderer is used.
   */
  VTKOpenGL2ContextDevice2D::VTKOpenGL2ContextDevice2D()
  : vtkOpenGL2ContextDevice2D()
  {
#ifdef VTK_USE_QT
    if(QApplication::instance())
    {
      this->TextRenderer->Delete();
      this->TextRenderer = VTKQtStringToImage::New();
    }
#endif
  };
  
  /*
   * Exactly the method than in VTK. This method is needed as implemented in VTK as a private method.
   */
  void VTKOpenGL2ContextDevice2D::AlignText(double orientation, float width, float height, float* p)
  {
    // Special case multiples of 90 as no transformation is required...
    if (orientation > -0.0001 && orientation < 0.0001)
      {
      switch (this->TextProp->GetJustification())
      {
        case VTK_TEXT_LEFT:
          break;
        case VTK_TEXT_CENTERED:
          p[0] -= floor(width / 2.0);
          break;
        case VTK_TEXT_RIGHT:
          p[0] -= width;
          break;
      }
      switch (this->TextProp->GetVerticalJustification())
      {
        case VTK_TEXT_BOTTOM:
          break;
        case VTK_TEXT_CENTERED:
          p[1] -= floor(height / 2.0);
          break;
        case VTK_TEXT_TOP:
          p[1] -= height;
          break;
      }
    }
    else if (orientation > 89.9999 && orientation < 90.0001)
    {
      switch (this->TextProp->GetJustification())
      {
        case VTK_TEXT_LEFT:
          break;
        case VTK_TEXT_CENTERED:
          p[1] -= floor(height / 2.0);
          break;
        case VTK_TEXT_RIGHT:
          p[1] -= height;
          break;
      }
      switch (this->TextProp->GetVerticalJustification())
      {
        case VTK_TEXT_TOP:
          break;
        case VTK_TEXT_CENTERED:
          p[0] -= floor(width / 2.0);
          break;
        case VTK_TEXT_BOTTOM:
          p[0] -= width;
          break;
      }
    }
    else if (orientation > 179.9999 && orientation < 180.0001)
    {
      switch (this->TextProp->GetJustification())
      {
        case VTK_TEXT_RIGHT:
          break;
        case VTK_TEXT_CENTERED:
          p[0] -= floor(width / 2.0);
          break;
        case VTK_TEXT_LEFT:
          p[0] -= width;
          break;
      }
      switch (this->TextProp->GetVerticalJustification())
      {
        case VTK_TEXT_TOP:
          break;
        case VTK_TEXT_CENTERED:
          p[1] -= floor(height / 2.0);
          break;
        case VTK_TEXT_BOTTOM:
          p[1] -= height;
          break;
      }
    }
    else if (orientation > 269.9999 && orientation < 270.0001)
    {
      switch (this->TextProp->GetJustification())
      {
        case VTK_TEXT_LEFT:
          break;
        case VTK_TEXT_CENTERED:
          p[1] -= floor(height / 2.0);
          break;
        case VTK_TEXT_RIGHT:
          p[1] -= height;
          break;
      }
      switch (this->TextProp->GetVerticalJustification())
      {
        case VTK_TEXT_BOTTOM:
          break;
        case VTK_TEXT_CENTERED:
          p[0] -= floor(width / 2.0);
          break;
        case VTK_TEXT_TOP:
          p[0] -= width;
          break;
      }
    }
  };
#endif
};
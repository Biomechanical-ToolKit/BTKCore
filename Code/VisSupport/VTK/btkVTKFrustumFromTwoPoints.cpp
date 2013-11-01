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
 * LOSS OF USE, DATA, OR PROFITS;OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "btkVTKFrustumFromTwoPoints.h"

#include <vtkObjectFactory.h>
#include <vtkPlanes.h>

namespace btk
{
  /**
   * @class VTKFrustumFromTwoPoints btkVTKFrustumFromTwoPoints.h
   * @brief Frustum's generator from two points defining a rectangle
   * 
   * @ingroup BTKVTK
   */
   
  /**
   * @fn static VTKFrustumFromTwoPoints* VTKFrustumFromTwoPoints::New();
   * Creates a VTKMarkersSource object and return it as a pointer.
   */
  vtkStandardNewMacro(VTKFrustumFromTwoPoints);
  
  /**
   * Generate a frustrum based on two points picked on the screen
   */
  vtkPlanes* VTKFrustumFromTwoPoints::Generate(double x0, double y0, double x1, double y1, vtkRenderer* renderer)
  {
    if (renderer == NULL)
    {
      vtkErrorMacro("Must specify renderer!");
      return 0;
    }
    
    vtkPlanes* frustum = vtkPlanes::New();
    this->mp_FrustumGenerator->DefineFrustum(x0, y0, x1, y1, renderer);
    frustum->SetPoints(this->mp_FrustumGenerator->GetFrustum()->GetPoints());
    frustum->SetNormals(this->mp_FrustumGenerator->GetFrustum()->GetNormals());
    return frustum;
  };
  
  /**
   * Constructor.
   */
  VTKFrustumFromTwoPoints::VTKFrustumFromTwoPoints()
  : vtkObject()
  {
    this->mp_FrustumGenerator = VTKFrustumGenerator_p::New();
  };
  
  /**
   * Destructor
   */
  VTKFrustumFromTwoPoints::~VTKFrustumFromTwoPoints()
  {
    this->mp_FrustumGenerator->Delete();
  };
  
  // Internals 
  vtkStandardNewMacro(VTKFrustumGenerator_p);
};
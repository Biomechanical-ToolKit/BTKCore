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

#ifndef __btkVTKFrustumFromTwoPoints_h
#define __btkVTKFrustumFromTwoPoints_h

#include "btkConfigure.h"

#include <vtkAreaPicker.h>

class vtkPlanes;

namespace btk
{
  class VTKFrustumGenerator_p;
  
  class VTKFrustumFromTwoPoints : public vtkObject
  {
    public:
      BTK_VTK_EXPORT static VTKFrustumFromTwoPoints* New();
      vtkExportedTypeRevisionMacro(VTKFrustumFromTwoPoints, vtkObject, BTK_VTK_EXPORT);
      BTK_VTK_EXPORT vtkPlanes* Generate(double x0, double y0, double x1, double y1, vtkRenderer* renderer);
      
    protected:
      BTK_VTK_EXPORT VTKFrustumFromTwoPoints();
      BTK_VTK_EXPORT ~VTKFrustumFromTwoPoints();
      
    private:
       VTKFrustumFromTwoPoints(const VTKFrustumFromTwoPoints& );  // Not implemented.
       VTKFrustumFromTwoPoints& operator=(const VTKFrustumFromTwoPoints& );  // Not implemented.
       
       VTKFrustumGenerator_p* mp_FrustumGenerator;
  };
  
  // Doxygen commands cond/endcond used to not show this class in the documentation.
  //! @cond
  class VTKFrustumGenerator_p : public vtkAreaPicker
  {
   public:
     BTK_VTK_EXPORT static VTKFrustumGenerator_p* New();
     vtkExportedTypeRevisionMacro(VTKFrustumGenerator_p, vtkAreaPicker, BTK_VTK_EXPORT);
     using vtkAreaPicker::DefineFrustum;

   protected:
     VTKFrustumGenerator_p(): vtkAreaPicker() {};
     // ~FrustumGenerator(); // Implicit
  };
  //! @endcond
};

#endif
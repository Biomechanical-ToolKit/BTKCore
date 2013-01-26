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

#include "btkVTKMarkersSource.h"

#include <vtkObjectFactory.h>
#include <vtkSphereSource.h>

namespace btk
{
  /**
   * @class VTKMarkersSource btkVTKMarkersSource.h
   * @brief Markers' generator
   *
   * Markers' color and radius are given as arrays to the input. For more information, check 
   * documentation of vtkGlyph3D or sources of btk::VTKMarkersFramesSource.
   * 
   * @ingroup BTKVTK
   */
   
  /**
   * @fn static VTKMarkersSource* VTKMarkersSource::New();
   * Creates a VTKMarkersSource object and return it as a pointer.
   */
  vtkStandardNewMacro(VTKMarkersSource);
  vtkCxxRevisionMacro(VTKMarkersSource, "$Revision: 0.2 $");
  
  /**
   * Print Superclass informations.
   */
  void VTKMarkersSource::PrintSelf(ostream& os, vtkIndent indent)
  {
    this->Superclass::PrintSelf(os,indent);
  }
  
  /**
   * Constructor.
   */
  VTKMarkersSource::VTKMarkersSource()
  : vtkGlyph3D()
  {
    this->mp_ExistingMarkers = 0;
    this->mp_VisibleMarkers = 0;
    vtkSphereSource* source = vtkSphereSource::New();
    source->SetRadius(1.0);
    this->SetSourceConnection(source->GetOutputPort());
    source->Delete();
  };

  /**
   * Destructor.
   */
  VTKMarkersSource::~VTKMarkersSource()
  {};

  /** 
   * Overloaded method to check visibility of each markers.
   * Use method SetExistingMarkers() and SetVisibleMarkers()
   * to set markers to display.
   */
  int VTKMarkersSource::IsPointVisible(vtkDataSet* /* points */, vtkIdType id)
  {
    if (this->mp_ExistingMarkers && (this->mp_ExistingMarkers->GetValue(id) != -1)
        && this->mp_VisibleMarkers && this->mp_VisibleMarkers->GetValue(id))
      return 1;
    else
      return 0;
  };

  /**
   * Sets existing markers (with residual different to -1).
   * Existing markers are set to 1 and others to 0.
   */
  void VTKMarkersSource::SetExistingMarkers(vtkIntArray* m)
  {
    this->mp_ExistingMarkers = m;
  };

  /**
   * Sets visible markers.
   * Visible markers are set to 1 and others to 0.
   */
  void VTKMarkersSource::SetVisibleMarkers(vtkIntArray* m)
  {
    this->mp_VisibleMarkers = m;
  };
  
};

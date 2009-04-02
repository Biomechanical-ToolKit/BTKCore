/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009, Arnaud Barré
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

#include "btkVTKMarkerSource.h"


#include <vtkCellArray.h>
#include <vtkMath.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>

namespace btk
{
  vtkCxxRevisionMacro(VTKMarkerSource, "$Revision: 0.1 $");
  vtkStandardNewMacro(VTKMarkerSource);
  
  /**
   * @class VTKMarkerSource
   * @brief Fill a vtkPolyData object of vertices from the coordinates of a btk::Marker.
   */
  
  void VTKMarkerSource::PrintSelf(ostream& os, vtkIndent indent)
  {
    this->Superclass::PrintSelf(os,indent);
  }
  
  VTKMarkerSource::VTKMarkerSource()
  : VTKPolyDataAlgorithmAdapter<Marker>()
  {};
  
  int VTKMarkerSource::RequestData(vtkInformation* /*request*/,
                                   vtkInformationVector** inputVector,
                                   vtkInformationVector* outputVector)
  {
    vtkInformation* outInfo = outputVector->GetInformationObject(0);    
    vtkPolyData* output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));
    double* ic;
    vtkIdType numberOfPoints = 0;
    double oc[3];
    vtkPoints* newPoints = vtkPoints::New();
    vtkCellArray* newVerts = vtkCellArray::New();
    
    vtkInformation* inInfo = inputVector[0]->GetInformationObject(0);
    VTKDataObjectAdapter* inObject = VTKDataObjectAdapter::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
    Marker::Pointer input = std::tr1::static_pointer_cast<Marker>(inObject->GetBTKDataObject());
    if (input.get() != 0)
    {
      numberOfPoints = input->GetFrameNumber();
      ic = input->GetValues().data();
    }
    
    newPoints->Allocate(numberOfPoints);
    newVerts->Allocate(newVerts->EstimateSize(1,numberOfPoints));
    for (vtkIdType i = 0 ; i < numberOfPoints ; ++i)
    {
      oc[0] = ic[i];
      oc[1] = ic[numberOfPoints + i];
      oc[2] = ic[2 * numberOfPoints + i];
      newVerts->InsertCellPoint(newPoints->InsertNextPoint(oc));
    }
    
    output->SetPoints(newPoints);
    newPoints->Delete();
    output->SetVerts(newVerts);
    newVerts->Delete();
    
    return 1;
  };
}
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

#include "btkVTKMarkerFrameSource.h"


#include <vtkCellArray.h>
#include <vtkMath.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>

namespace btk
{
  vtkCxxRevisionMacro(VTKMarkerFrameSource, "$Revision: 0.1 $");
  vtkStandardNewMacro(VTKMarkerFrameSource);
  
  /**
   * @class VTKMarkerFrameSource
   * @brief Transform a frame from a btk::MarkerCollection to a vtkPolyData.
   *
   * Extract a frame from the btk::MarkerCollection set in input and transform 
   * it in a vtkPolyData. The coordinates inserted in the vtkPolyData correspond
   * to the coordinates of the markers in the collection at the specifed frame.
   * vtkPolyData
   */
  
  /**
   * @fn static VTKMarkerFrameSource* VTKMarkerFrameSource::New();
   * Creates and returns a VTKMarkerFrameSource object.
   */
  
  /**
   * Prints only the Superclass information.
   */
  void VTKMarkerFrameSource::PrintSelf(ostream& os, vtkIndent indent)
  {
    this->Superclass::PrintSelf(os,indent);
  }
  
  /**
   * @fn int VTKMarkerFrameSource::GetIndex() const
   * Returns the index of the frame to extract.
   */
  
  /**
   * Sets the index of the frame to extract.
   */ 
  void VTKMarkerFrameSource::SetIndex(int idx)
  {
    this->m_Index = idx; 
    this->Modified();
  };
  
  /**
   * Constructor.
   */
  VTKMarkerFrameSource::VTKMarkerFrameSource()
  : VTKPolyDataAlgorithmAdapter<MarkerCollection>()
  {
    this->m_Index = 0;
  };
  
  /**
   * @fn VTKMarkerFrameSource::~VTKMarkerFrameSource()
   * Destructor (empty).
   */
  
  /**
   * Generates the output data.
   */
  int VTKMarkerFrameSource::RequestData(vtkInformation* /* request */,
                                   vtkInformationVector** inputVector,
                                   vtkInformationVector* outputVector)
  {
    vtkInformation* outInfo = outputVector->GetInformationObject(0);    
    vtkPolyData* output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));
    vtkIdType numberOfPoints = 0;
    double oc[3];
    vtkPoints* newPoints = vtkPoints::New();
    vtkCellArray* newVerts = vtkCellArray::New();
    
    vtkInformation* inInfo = inputVector[0]->GetInformationObject(0);
    VTKDataObjectAdapter* inObject = VTKDataObjectAdapter::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
    MarkerCollection::Pointer input = std::tr1::static_pointer_cast<MarkerCollection>(inObject->GetBTKDataObject());
    if (input.get() != 0)
      numberOfPoints = input->GetItemNumber();
    
    newPoints->Allocate(numberOfPoints);
    newVerts->Allocate(newVerts->EstimateSize(1,numberOfPoints));
    if (numberOfPoints != 0)
    {
      MarkerCollection::ConstIterator it = input->Begin();
      int index = this->m_Index;
      int frameNumber = (*it)->GetFrameNumber();
      if (this->m_Index > frameNumber)
      {
        btkErrorMacro("The specified index is out of range. The index extracted corresponds to the index modulo the frame number.");
        index = this->m_Index % frameNumber;
      }
      while (it != input->End())
      {
        double* ic = (*it)->GetValues().data();
        oc[0] = ic[index];
        oc[1] = ic[frameNumber + index];
        oc[2] = ic[2 * frameNumber + index];
        newVerts->InsertCellPoint(newPoints->InsertNextPoint(oc));
        ++it;
      }
    }
    
    output->SetPoints(newPoints);
    newPoints->Delete();
    output->SetVerts(newVerts);
    newVerts->Delete();
    
    return 1;
  };
}
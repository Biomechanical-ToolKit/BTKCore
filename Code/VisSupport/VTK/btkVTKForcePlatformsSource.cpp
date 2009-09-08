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

#include "btkVTKForcePlatformsSource.h"
#include "btkVTKDataObjectAdapter.h"

#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkPlaneSource.h>
#include <vtkAppendPolyData.h>

namespace btk
{
  /**
   * @class VTKForcePlatformsSource
   * @brief Displays force platform geometries from a collection of btk::ForcePlatform object.
   * 
   * @ingroup BTKVTK
   */
  
  /**
   * @fn static VTKForcePlatformsSource* VTKForcePlatformsSource::New();
   * Creates a VTKForcePlatformsSource object and return it as a pointer.
   */
  vtkStandardNewMacro(VTKForcePlatformsSource);
  vtkCxxRevisionMacro(VTKForcePlatformsSource, "$Revision: 0.1 $");
  
  /**
   * Prints only Superclass informations.
   */
  void VTKForcePlatformsSource::PrintSelf(ostream& os, vtkIndent indent)
  {
    this->Superclass::PrintSelf(os,indent);
  };

  /**
   * Sets the input.
   */
  void VTKForcePlatformsSource::SetInput(ForcePlatformCollection::Pointer input)
  {
    VTKDataObjectAdapter* inObject = VTKDataObjectAdapter::New();
    inObject->SetBTKDataObject(input);
    this->vtkPolyDataAlgorithm::SetInput(inObject);
    inObject->Delete();
  };
  
  /**
   * @fn double VTKForcePlatformsSource::GetScaleUnit()
   * Returns the scale factor used to adapt size of force platform's geometry.
   *
   * Usefull when visualized acquistion date are set in meter, millimeter, inch, etc.
   */
  
  /**
   * @fn void VTKForcePlatformsSource::SetScaleUnit(double s)
   * Sets the scale factor used to adapt size of force platform's geometry.
   *
   * Usefull when visualized acquistion date are set in meter, millimeter, inch, etc.
   */
  
  /**
   * Constructor.
   *
   * Filter with one input and output.
   */
  VTKForcePlatformsSource::VTKForcePlatformsSource()
  : vtkPolyDataAlgorithm()
  {
    this->mp_Scale = 1.0;
  };

  /**
   * @fn VTKForcePlatformsSource::~VTKForcePlatformsSource()
   * Empty destructor.
   */
  
  /**
   * Generate force platform geometry.
   */
  int VTKForcePlatformsSource::RequestData(vtkInformation* request,
                                 vtkInformationVector** inputVector,
                                 vtkInformationVector* outputVector)
  {
    vtkInformation* inInfo = inputVector[0]->GetInformationObject(0);
    VTKDataObjectAdapter* inObject = VTKDataObjectAdapter::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
    if (!inObject)
      return 0;
    ForcePlatformCollection::Pointer input = static_pointer_cast<ForcePlatformCollection>(inObject->GetBTKDataObject());
    vtkInformation* outInfo = outputVector->GetInformationObject(0);    
    vtkPolyData* output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

    vtkAppendPolyData* append = vtkAppendPolyData::New();
    vtkPlaneSource* planeGenerator = vtkPlaneSource::New();

    for (ForcePlatformCollection::ConstIterator it = input->Begin() ; it != input->End() ; ++it)
    {
      double* c = (*it)->GetCorners().data();
      planeGenerator->SetPoint1(c[3] * this->mp_Scale, c[4] * this->mp_Scale, c[5] * this->mp_Scale);
      planeGenerator->SetPoint2(c[9] * this->mp_Scale, c[10] * this->mp_Scale, c[11] * this->mp_Scale);
      planeGenerator->SetOrigin(c[6] * this->mp_Scale, c[7] * this->mp_Scale, c[8] * this->mp_Scale);

      planeGenerator->Update();
      vtkPolyData* forcePlatform = vtkPolyData::New();
      forcePlatform->DeepCopy(planeGenerator->GetOutput());
      append->AddInput(forcePlatform);
      forcePlatform->Delete();
    }

    if (input->GetItemNumber() != 0)
    {
      append->Update();
      output->ShallowCopy(append->GetOutput());
      append->Delete();
    }
    else
      output->Initialize();
    
    return 1;
  };

  /**
   * Sets the type of object required for the input.
   */
  int VTKForcePlatformsSource::FillInputPortInformation(int /* port */, 
                                                      vtkInformation* info)
  {
    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "VTKDataObjectAdapter");
    return 1;
  }

};

/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2011, Arnaud Barré
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
#include "btkConvert.h"

#include <Eigen/Eigen>

#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkAppendPolyData.h>
#include <vtkCellArray.h>
#include <vtkAxes.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkTransform.h>
#include <vtkCellData.h>
#include <vtkUnsignedCharArray.h>
#include <vtkMatrix4x4.h>
#include <vtkVectorText.h>
#include <vtkBoundingBox.h>

namespace btk
{
  /**
   * @class VTKForcePlatformsSource btkVTKForcePlatformsSource.h
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
   * @fn bool VTKForcePlatformsSource::GetShowAxes() const
   * Gets the status for the displaying of the axes.
   */
   
  /**
   * Switch on/off the displaying of the axes for each force platform.
   */
  void VTKForcePlatformsSource::SetShowAxes(bool actived)
  {
    if (this->m_ShowAxes == actived)
      return;
    this->m_ShowAxes = actived;
    this->Modified();
    this->m_PropertiesModified = true;
  };
   
  /**
   * @fn bool VTKForcePlatformsSource::GetShowIndex() const
   * Gets the status for the displaying of the index.
   */
   
  /**
   * Switch on/off the displaying of the index for each force platform.
   */
 void VTKForcePlatformsSource::SetShowIndex(bool actived)
 {
   if (this->m_ShowIndex == actived)
     return;
   this->m_ShowIndex = actived;
   this->Modified();
   this->m_PropertiesModified = true;
 };
  
  /**
   * Constructor.
   *
   * Filter with one input and output.
   */
  VTKForcePlatformsSource::VTKForcePlatformsSource()
  : vtkPolyDataAlgorithm()
  {
    this->m_Scale = 1.0;
    this->m_ShowAxes = true;
    this->m_ShowIndex = true;
    this->m_PropertiesModified = false;
    this->SetNumberOfOutputPorts(3);
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
    btkNotUsed(request);
    vtkInformation* inInfo = inputVector[0]->GetInformationObject(0);
    VTKDataObjectAdapter* inObject = VTKDataObjectAdapter::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
    if (!inObject)
      return 0;
    if ((inObject->GetMTime() < this->GetMTime()) && !this->m_PropertiesModified)
      return 0;
      
    this->m_PropertiesModified = false;

    ForcePlatformCollection::Pointer input = static_pointer_cast<ForcePlatformCollection>(inObject->GetBTKDataObject());
    vtkInformation* outInfo1 = outputVector->GetInformationObject(0);    
    vtkPolyData* output1 = vtkPolyData::SafeDownCast(outInfo1->Get(vtkDataObject::DATA_OBJECT()));
    vtkInformation* outInfo2 = outputVector->GetInformationObject(1);    
    vtkPolyData* output2 = vtkPolyData::SafeDownCast(outInfo2->Get(vtkDataObject::DATA_OBJECT()));
    vtkInformation* outInfo3 = outputVector->GetInformationObject(2);    
    vtkPolyData* output3 = vtkPolyData::SafeDownCast(outInfo3->Get(vtkDataObject::DATA_OBJECT()));

    vtkAppendPolyData* append1 = vtkAppendPolyData::New();
    vtkAppendPolyData* append2 = vtkAppendPolyData::New();
    vtkAppendPolyData* append3 = vtkAppendPolyData::New();
    vtkAxes* axesGenerator = vtkAxes::New();
    axesGenerator->SetScaleFactor(150.0);
    vtkMatrix4x4* pose = vtkMatrix4x4::New();
    vtkTransform* transform = vtkTransform::New();
    vtkTransformPolyDataFilter* axesTransform = vtkTransformPolyDataFilter::New();
    axesTransform->SetInput(axesGenerator->GetOutput());
    axesTransform->SetTransform(transform);
    vtkUnsignedCharArray* axesColors = vtkUnsignedCharArray::New();
    axesColors->SetNumberOfComponents(3);
    axesColors->SetNumberOfTuples(3);
    axesColors->SetTuple3(0, 255, 0, 0); // X axis: red
    axesColors->SetTuple3(1, 0, 255, 0); // Y axis: green
    axesColors->SetTuple3(2, 0, 0, 255); // Z axis: blue
    vtkVectorText* indexGenerator = vtkVectorText::New();
    vtkTransformPolyDataFilter* indexScale = vtkTransformPolyDataFilter::New();
    indexScale->SetInputConnection(indexGenerator->GetOutputPort());
    indexScale->SetTransform(transform);
    vtkTransformPolyDataFilter* indexTransform = vtkTransformPolyDataFilter::New();
    indexTransform->SetTransform(transform);
    int idx = 1;
    for (ForcePlatformCollection::ConstIterator it = input->Begin() ; it != input->End() ; ++it)
    {
      // Output #0: Plane
      ForcePlatform::Origin origin = (*it)->GetOrigin();
      ForcePlatform::Corners corners = (*it)->GetCorners();
      double* c = corners.data();
      vtkPolyData* forcePlatform = vtkPolyData::New();
      vtkPoints* pts = vtkPoints::New();
      pts->SetNumberOfPoints(4);
      for (int i = 0 ; i < 4 ; ++i)
        pts->SetPoint(i, c[i*3] * this->m_Scale, c[i*3+1] * this->m_Scale, c[i*3+2] * this->m_Scale);
      vtkCellArray* polys = vtkCellArray::New();
      polys->Allocate(polys->EstimateSize(1,4));
      vtkIdType indices[4] = {0,1,2,3};
      polys->InsertNextCell(4,indices);
      forcePlatform->SetPoints(pts);
      pts->Delete();
      forcePlatform->SetPolys(polys);
      polys->Delete();
      append1->AddInput(forcePlatform);
      forcePlatform->Delete();
      // Output #1: Axes
      Eigen::Matrix<double, 3, 3> R;
      Eigen::Matrix<double, 3, 1> t;
      R.col(0) = corners.col(0) - corners.col(1);
      R.col(0).normalize();
      R.col(2) = R.col(0).cross(corners.col(0) - corners.col(3));
      R.col(2).normalize();
      R.col(1) = R.col(2).cross(R.col(0));
      t = (corners.col(0) + corners.col(2)) / 2;
      switch((*it)->GetType())
      {
        case 1:
          break;
        case 2:
        case 4:
        case 5:
          // AMTI
          if (origin.z() > 0)
            origin *= -1;
          t += origin;
          break;
        case 3:
          // Kistler
          origin << 0, 0, origin.z();
          if (origin.z() > 0)
            origin *= -1;
          t += origin;
          break;
        case 6:
          btkErrorMacro("Force Platform type 6 is not yet supported. Please, report this to the developers");
          break;
        case 7:
          btkErrorMacro("Force Platform type 7 is not yet supported. Please, report this to the developers");
          break;
        case 11:
          btkErrorMacro("Force Platform type 11 is not yet supported. Please, report this to the developers");
          break;
        case 12:
          btkErrorMacro("Force Platform type 12 is not yet supported. Please, report this to the developers");
          break;
        case 21:
          btkErrorMacro("Force Platform type 21 is not yet supported. Please, report this to the developers");
          break;
      }
      t *= this->m_Scale;
      pose->Identity();
      for (int i = 0 ; i < 3 ; ++i)
      {
        for (int j = 0 ; j < 3 ; ++j)
          pose->SetElement(i,j,R(i,j));
        pose->SetElement(i,3,t(i));
      }
      transform->SetMatrix(pose);
      axesTransform->Update();
      vtkPolyData* axes = vtkPolyData::New();
      axes->DeepCopy(axesTransform->GetOutput());
      axes->GetCellData()->SetScalars(axesColors);
      append2->AddInput(axes);
      axes->Delete();
      // Output #2: Index
      if (input->GetItemNumber() > 1)
      {
        indexGenerator->SetText(btk::ToString(idx++).c_str());
        indexGenerator->Update();
        vtkBoundingBox bbForcePlate(forcePlatform->GetBounds());
        vtkBoundingBox bbIndex(indexGenerator->GetOutput()->GetBounds());
        // Scale
        double bbLengths[2][3], maxLengths[2][3], minTwoLengths[2][2];
        bbForcePlate.GetLengths(bbLengths[0]); bbIndex.GetLengths(bbLengths[1]);
        for (int bbIdx = 0 ; bbIdx < 2 ; ++bbIdx)
        {
          maxLengths[bbIdx][0] = std::max(bbLengths[bbIdx][0], bbLengths[bbIdx][1]);
          maxLengths[bbIdx][1] = std::max(bbLengths[bbIdx][1], bbLengths[bbIdx][2]);
          maxLengths[bbIdx][2] = std::max(bbLengths[bbIdx][0], bbLengths[bbIdx][2]);
        }
        // Force platfrom width
        minTwoLengths[0][0] = std::min(maxLengths[0][0], maxLengths[0][1]);
        minTwoLengths[0][1] = std::min(maxLengths[0][1], maxLengths[0][2]);
        // Index length
        minTwoLengths[1][0] = std::max(maxLengths[1][0], maxLengths[1][1]);
        minTwoLengths[1][1] = std::max(maxLengths[1][1], maxLengths[1][2]);
        double scale = std::min(minTwoLengths[0][0] / minTwoLengths[1][0], minTwoLengths[0][1] / minTwoLengths[1][1]) / 2.0;
        transform->Identity();
        transform->Scale(scale, scale, scale);
        indexScale->Update();
        // Rotation
        vtkPolyData* index = vtkPolyData::New();
        index->DeepCopy(indexScale->GetOutput());
        indexTransform->SetInput(index);
        pose->Identity();
        R.col(2) *= -1.0; // Reorient Z axis as for force platforms it is always defined as negative.
        if (R.col(1).sum() <= 0)
          R.col(1) *= -1.0; // Force the Y vector to be in the same direction than the global frame.
        R.col(0) = R.col(1).cross(R.col(2));
        for (int i = 0 ; i < 3 ; ++i)
        {
          for (int j = 0 ; j < 3 ; ++j)
            pose->SetElement(i,j,R(i,j));
        }
        transform->SetMatrix(pose);
        indexTransform->Update();
        index->Delete();
        index = vtkPolyData::New();
        index->DeepCopy(indexTransform->GetOutput());
        // Translation
        bbIndex.SetBounds(indexTransform->GetOutput()->GetBounds());
        double bbIndexCenter[3]; bbIndex.GetCenter(bbIndexCenter);
        double bbForcePlateCenter[3]; bbForcePlate.GetCenter(bbForcePlateCenter);
        indexTransform->SetInput(index);
        pose->Identity();
        for (int i = 0 ; i < 3 ; ++i)
          pose->SetElement(i, 3, (bbForcePlateCenter[i] - bbIndexCenter[i]) * this->m_Scale);
        transform->SetMatrix(pose);
        indexTransform->Update();
        index->Delete();
        index = vtkPolyData::New();
        index->DeepCopy(indexTransform->GetOutput());
        // Append
        append3->AddInput(index);
        index->Delete();
      }
    }
    axesGenerator->Delete();
    axesTransform->Delete();
    transform->Delete();
    pose->Delete();
    axesColors->Delete();
    indexScale->Delete();
    indexTransform->Delete();

    if (input->GetItemNumber() != 0)
    {
      append1->Update();
      output1->ShallowCopy(append1->GetOutput());
      append1->Delete();
      if (this->m_ShowAxes)
      {
        append2->Update();
        output2->ShallowCopy(append2->GetOutput());
        append2->Delete();
      }
      else 
        output2->Initialize();
      if ((input->GetItemNumber() > 1) && this->m_ShowIndex)
      {
        append3->Update();
        output3->ShallowCopy(append3->GetOutput());
        append3->Delete();
      }
      else
        output3->Initialize();
    }
    else
    {
      output1->Initialize();
      output2->Initialize();
      output3->Initialize();
    }
    
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

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

#include "btkVTKMarkersFramesSource.h"
#include "btkVTKDataObjectAdapter.h"
#include "btkVTKCommandEvents.h"

#include <vtkPolyData.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkStreamingDemandDrivenPipeline.h>
#include <vtkPointData.h>
#include <vtkTimerLog.h>
#include <vtkstd/vector>

namespace btk
{
  /**
   * @class VTKMarkersCoordinates
   * @brief Store markers' frames as a vector of vtkPolyData.
   */
  class VTKMarkersCoordinates : public vtkstd::vector<vtkPolyData*>
  {};

  /**
   * @class VTKMarkersIds
   * @brief Store markers' visibility for each frame.
   */
  class VTKMarkersIds : public vtkstd::vector<vtkIntArray*>
  {};
  
  /** 
   * @class VTKMarkersFramesSource
   * @brief Display markers with interactive possibilities as hide/show markers, modify colors or diameters.
   *
   * This filter compute all markers' position when a new input is set or modified.
   * The display of markers for each frame use the vtk TIME mechanism by setting the required frame
   * to a vtkStreamingDemandDrivenPipeline object and call the vtkRenderer::Renderer() method.
   * For example:
   * @code
   * // BTK
   * btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
   * reader->SetDisableFilenameExceptionState(true);
   * btk::SpecializedPointsExtractor::Pointer markersExtractor = btk::SpecializedPointsExtractor::New();
   * markersExtractor->SetInput(reader->GetOutput());
   * // VTK
   * vtkRenderer* renderer = vtkRenderer::New()
   * btk::VTKMarkersFramesSource* markers = btk::VTKMarkersFramesSource::New();
   * markers->SetInput(markersExtractor->GetOutput());
   * vtkPolyDataMapper* mapper = vtkPolyDataMapper::New();
   * mapper->SetInputConnection(markers->GetOutputPort(0));
   * mapper->SetLookupTable(markers->GetMarkerColorLUT());
   * mapper->SetScalarModeToUsePointFieldData();
   * mapper->UseLookupTableScalarRangeOn();
   * mapper->SelectColorArray("Colors");
   * vtkActor* actor = vtkActor::New();
   * actor->SetMapper(mapper);
   * renderer->AddActor(actor);
   * // ...
   * int numberOfFrames = reader->GetPointFrameNumber();
   * vtkStreamingDemandDrivenPipeline* exec = vtkStreamingDemandDrivenPipeline::SafeDownCast(GRFs->GetExecutive());
   * for (int i = 1 ; i < numberOfFrames ; ++i)
   * {
   *   exec->SetUpdateTimeStep(0, i);
   *   mapper->Modified();
   *   renderer->Render();
   * }
   * // Code cleanup
   * // ...
   * @endcode
   * 
   * @ingroup BTKVTK
   */
  
  /**
   * @fn static VTKMarkersFramesSource* VTKMarkersFramesSource::New();
   * Creates a VTKMarkersFramesSource object and return it as a pointer.
   */
  vtkStandardNewMacro(VTKMarkersFramesSource);
  vtkCxxRevisionMacro(VTKMarkersFramesSource, "$Revision: 0.1 $");
  
  /**
   * Prints Superclass informations.
   */
  void VTKMarkersFramesSource::PrintSelf(ostream& os, vtkIndent indent)
  {
    this->Superclass::PrintSelf(os,indent);
  };

  /**
   * Sets input.
   */
  void VTKMarkersFramesSource::SetInput(PointCollection::Pointer input)
  {
    VTKDataObjectAdapter* inObject = VTKDataObjectAdapter::New();
    inObject->SetBTKDataObject(input);
    this->vtkPolyDataAlgorithm::SetInput(inObject);
    inObject->Delete();
  };

  /**
   * Clear list of markers tagged as selected.
   */
  void VTKMarkersFramesSource::ClearSelectedMarkers()
  {
    for (int i = 0 ; i < this->mp_SelectedMarkers->GetNumberOfTuples() ; ++i)
    {
      if (this->mp_SelectedMarkers->GetValue(i))
      {
        this->mp_MarkersRadius->SetValue(i, this->mp_MarkersRadius->GetValue(i) / 1.75);
        this->mp_SelectedMarkers->SetValue(i, 0);
      }
    }
  };

  /**
   * Tag marker with index @a id as selected.
   */
  void VTKMarkersFramesSource::SetSelectedMarker(vtkIdType id)
  {
    if (id >= this->mp_SelectedMarkers->GetNumberOfTuples())
    {
      vtkErrorMacro("Out of range.");
      return;
    }
    this->mp_SelectedMarkers->SetValue(id, 1);
    this->mp_MarkersRadius->SetValue(id, this->mp_MarkersRadius->GetValue(id) * 1.75);
  };

  /**
   * Returns radius of the marker with index @a id.
   */
  double VTKMarkersFramesSource::GetMarkerRadius(vtkIdType id)
  {
    if (id >= this->mp_MarkersRadius->GetNumberOfTuples())
    {
      vtkErrorMacro("Out of range.");
      return 0.0;
    }
    if (this->mp_SelectedMarkers->GetValue(id))
      return this->mp_MarkersRadius->GetValue(id) / 1.75;
    else
      return this->mp_MarkersRadius->GetValue(id);
  };

  /**
   * Sets radius of the marker with index @a id.
   */
  void VTKMarkersFramesSource::SetMarkerRadius(vtkIdType id, double radius)
  {
    if (id >= this->mp_MarkersRadius->GetNumberOfTuples())
    {
      vtkErrorMacro("Out of range.");
      return;
    }
    if (this->mp_SelectedMarkers->GetValue(id))
      this->mp_MarkersRadius->SetValue(id, radius * 1.75);
    else
      this->mp_MarkersRadius->SetValue(id, radius);
  };

  /**
   * Returns colors associated with the marker which has the index @a id
   */
  double* VTKMarkersFramesSource::GetMarkerColor(vtkIdType id)
  {
    if (id >= this->mp_MarkersColorIndex->GetNumberOfTuples())
    {
      vtkErrorMacro("Out of range.");
      return 0;
    }
    return this->mp_MarkersColorsLUT->GetTableValue(this->mp_MarkersColorIndex->GetValue(id));
  };

  /**
   * Sets markers' color.
   */
  void VTKMarkersFramesSource::SetMarkersColor(vtkIdTypeArray* ids, double r, double g, double b)
  {
    int num = this->mp_MarkersColorsLUT->GetNumberOfTableValues();
    int i = 0;
    for (i = 0 ; i < num ; ++i)
    {
      double* color = this->mp_MarkersColorsLUT->GetTableValue(i);
      if ((r == color[0]) && (g == color[1]) && (b == color[2]))
        break;
    }
    if (i >= num)
    {
      this->mp_MarkersColorsLUT->SetNumberOfTableValues(num + 1);
      this->mp_MarkersColorsLUT->SetTableValue(num, r, g, b);
      this->mp_MarkersColorsLUT->SetTableRange(0, num + 1);
      i = num;
    }
    for (int j = 0 ; j < ids->GetNumberOfTuples() ; ++j)
      this->mp_MarkersColorIndex->SetValue(ids->GetValue(j), i);
  };

  /**
   * Return color's index of the marker with index @a id.
   */
  vtkIdType VTKMarkersFramesSource::GetMarkerColorIndex(vtkIdType id)
  {
    if (id >= this->mp_MarkersColorIndex->GetNumberOfTuples())
    {
      vtkErrorMacro("Out of range.");
      return 0;
    }
    return this->mp_MarkersColorIndex->GetValue(id);
  };

  /**
   * Sets color's index of the marker with index @a id.
   */
  void VTKMarkersFramesSource::SetMarkerColorIndex(vtkIdType id, vtkIdType color)
  {
    if (id >= this->mp_MarkersColorIndex->GetNumberOfTuples())
    {
      vtkErrorMacro("Out of range.");
      return;
    }
    this->mp_MarkersColorIndex->SetValue(id, color);
  };

  /**
   * Returns lookup table between colors and their index given to each marker.
   */
  vtkLookupTable* VTKMarkersFramesSource::GetMarkerColorLUT()
  {
    return this->mp_MarkersColorsLUT;
  };
  
  /**
   * Tag the marker with the index @a id as hidden.
   */
  void VTKMarkersFramesSource::HideMarker(vtkIdType id)
  {
    if (id >= this->mp_VisibleMarkers->GetNumberOfTuples())
    {
      vtkErrorMacro("Out of range.");
      return;
    }
    this->mp_VisibleMarkers->SetValue(id, 0);
  };

  /**
   * Hides all the markers.
   */
  void VTKMarkersFramesSource::HideMarkers()
  {
    for (int i = 0 ; i < this->mp_VisibleMarkers->GetNumberOfTuples() ; ++i)
      this->mp_VisibleMarkers->SetValue(i, 0);
  };

  /**
   * Tag the marker with the index @a id as visible.
   */
  void VTKMarkersFramesSource::ShowMarker(vtkIdType id)
  {
    if (id >= this->mp_VisibleMarkers->GetNumberOfTuples())
    {
      vtkErrorMacro("Out of range.");
      return;
    }
    this->mp_VisibleMarkers->SetValue(id, 1);
  };

  /**
   * Shows all the markers.
   */
  void VTKMarkersFramesSource::ShowMarkers()
  {
    for (int i = 0 ; i < this->mp_VisibleMarkers->GetNumberOfTuples() ; ++i)
      this->mp_VisibleMarkers->SetValue(i, 1);
  };
  
  /**
   * @fn double VTKMarkersFramesSource::GetScaleUnit()
   * Returns the scale factor used to adapt markers' positions
   *
   * Usefull when visualized acquistion date are set in meter, millimeter, inch, etc.
   */
  
  /**
   * @fn void VTKMarkersFramesSource::SetScaleUnit(double s)
   * Sets the scale factor used to adapt markers' positions
   *
   * Usefull when visualized acquistion date are set in meter, millimeter, inch, etc.
   */

  /**
   * Constructor.
   */
  VTKMarkersFramesSource::VTKMarkersFramesSource()
  : vtkPolyDataAlgorithm()
  {
    this->mp_MarkersCoordinates = new VTKMarkersCoordinates();
    this->mp_ExistingMarkers = new VTKMarkersIds();
    this->mp_VisibleMarkers = vtkIntArray::New();
    this->mp_SelectedMarkers = vtkIntArray::New();
    this->mp_MarkersRadius = vtkDoubleArray::New();
    this->mp_MarkersColorIndex = vtkIdTypeArray::New();
    this->mp_MarkersColorIndex->SetName("Colors");
    this->mp_MarkersColorsLUT = vtkLookupTable::New();  

    this->mp_MarkersGenerator = VTKMarkersSource::New();
    this->mp_MarkersGenerator->OrientOff();
    this->mp_MarkersGenerator->ClampingOff();
    this->mp_MarkersGenerator->SetIndexModeToOff();
    this->mp_MarkersGenerator->SetVectorModeToVectorRotationOff();
    this->mp_MarkersGenerator->GeneratePointIdsOn();
    this->mp_MarkersGenerator->SetPointIdsName("MarkersIds");
    this->mp_MarkersGenerator->SetScaleModeToScaleByScalar();
    this->mp_MarkersGenerator->SetColorModeToColorByScalar();
    this->mp_MarkersGenerator->SetVisibleMarkers(this->mp_VisibleMarkers);
    this->mp_MarkersColorsLUT->SetNumberOfTableValues(1);
    this->mp_MarkersColorsLUT->SetTableValue(0, 1.0, 1.0, 1.0);
    
    this->mp_Scale = 1.0;
  };
  
  /**
   * Destructor.
   */
  VTKMarkersFramesSource::~VTKMarkersFramesSource()
  {
    for (int i = 0 ; i < static_cast<int>(this->mp_MarkersCoordinates->size()) ; ++i)
    {
      this->mp_MarkersCoordinates->operator[](i)->Delete();
      this->mp_ExistingMarkers->operator[](i)->Delete();
    }
    delete this->mp_MarkersCoordinates;
    delete this->mp_ExistingMarkers;
    this->mp_VisibleMarkers->Delete();
    this->mp_SelectedMarkers->Delete();
    this->mp_MarkersRadius->Delete();
    this->mp_MarkersColorIndex->Delete();
    this->mp_MarkersColorsLUT->Delete();
    this->mp_MarkersGenerator->Delete();
  };

  /**
   * Generate marker's position.
   */
  int VTKMarkersFramesSource::RequestInformation(vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector)
  {
    // Convert a btk::PointCollection into a vector of polydata containing centers of visible markers.
    vtkInformation* inInfo = inputVector[0]->GetInformationObject(0);
    VTKDataObjectAdapter* inObject = VTKDataObjectAdapter::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
    if (!inObject)
      return 0;
    if (inObject->GetMTime() > this->GetMTime())
    {
      PointCollection::Pointer input = static_pointer_cast<PointCollection>(inObject->GetBTKDataObject());
      // RAZ
      for (int i = 0 ; i < static_cast<int>(this->mp_MarkersCoordinates->size()) ; ++i)
      {
        this->mp_MarkersCoordinates->operator[](i)->Delete();
        this->mp_ExistingMarkers->operator[](i)->Delete();
      }
      this->mp_MarkersCoordinates->clear();
      this->mp_ExistingMarkers->clear();
      int frameNumber = 0;
      int pointNumber = input->GetItemNumber();
      if (pointNumber != 0)
      {
        // Radius, Selected & Hidden markers
        this->mp_MarkersRadius->SetNumberOfValues(pointNumber);
        this->mp_SelectedMarkers->SetNumberOfValues(pointNumber);
        this->mp_VisibleMarkers->SetNumberOfValues(pointNumber);
        this->mp_MarkersColorIndex->SetNumberOfValues(pointNumber);
        for (int i = 0 ; i < pointNumber ; ++i)
        {
          this->mp_MarkersRadius->SetValue(i, 8.0); // 8mm
          this->mp_SelectedMarkers->SetValue(i, 0);
          this->mp_VisibleMarkers->SetValue(i, 1);
          this->mp_MarkersColorIndex->SetValue(i, 0);
        }

        frameNumber = input->GetItem(0)->GetFrameNumber();
        vtkPolyData* points = vtkPolyData::New();
        vtkPoints* centers = vtkPoints::New();
        centers->SetNumberOfPoints(pointNumber);
        points->SetPoints(centers);
        this->mp_MarkersCoordinates->resize(frameNumber);
        this->mp_ExistingMarkers->resize(frameNumber);

        for (int i = 0 ; i < frameNumber ; ++i)
        {
          vtkIntArray* existingMarkersIds = vtkIntArray::New();
          existingMarkersIds->SetNumberOfValues(pointNumber);
          int ptIdx = 0;
          PointCollection::ConstIterator it = input->Begin();
          while (it != input->End())
          {
            double* ic = (*it)->GetValues().data();
            centers->SetPoint(ptIdx, ic[i] * this->mp_Scale, 
                                     ic[frameNumber + i] * this->mp_Scale,
                                     ic[2 * frameNumber + i] * this->mp_Scale);
            // Insert only valid point
            if ((*it)->GetResiduals().coeffRef(i) != -1.0)
              existingMarkersIds->SetValue(ptIdx, 1);
            else
              existingMarkersIds->SetValue(ptIdx, 0);
            ++ptIdx;
            ++it;
          }
          this->mp_ExistingMarkers->operator[](i) = existingMarkersIds;

          vtkPolyData* markers = vtkPolyData::New();
          markers->DeepCopy(points);
          // Scaling factors
          markers->GetPointData()->AddArray(this->mp_MarkersRadius);
          markers->GetPointData()->SetActiveAttribute(0, vtkDataSetAttributes::SCALARS);
          // No Vectors
          markers->GetPointData()->AddArray(0);
          // No Normals
          markers->GetPointData()->AddArray(0);
          // Color
          markers->GetPointData()->AddArray(this->mp_MarkersColorIndex);
          markers->GetPointData()->SetActiveAttribute(3, vtkDataSetAttributes::SCALARS);

          this->mp_MarkersCoordinates->operator[](i) = markers;
        }
        points->Delete();
        centers->Delete();
      }
      // Update output informations
      vtkInformation *outInfo = outputVector->GetInformationObject(0);
      // TIME_STEPS
      double *outTimes = new double [frameNumber];
      int i;
      for (i = 0; i < frameNumber; ++i)
        outTimes[i] = i;
      outInfo->Set(vtkStreamingDemandDrivenPipeline::TIME_STEPS(), outTimes, frameNumber);
      delete [] outTimes;
      // TIME_RANGE
      double outRange[2];
      outRange[0] = 0.0;
      outRange[1] = frameNumber - 1.0;
      outInfo->Set(vtkStreamingDemandDrivenPipeline::TIME_RANGE(), outRange, 2);
    }

    return 1;
  };

  /**
   * Extract markers' frame required by a vtkStreamingDemandDrivenPipeline object.
   */
  int VTKMarkersFramesSource::RequestData(vtkInformation* request,
                                   vtkInformationVector** inputVector,
                                   vtkInformationVector* outputVector)
  {

    vtkTimerLog* timer = vtkTimerLog::New();
    timer->StartTimer();

    vtkInformation* outInfo = outputVector->GetInformationObject(0);    
    vtkPolyData* output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

    if (this->mp_MarkersCoordinates->size() != 0)
    {
      double t = 0.0;
      if (outInfo->Has(vtkStreamingDemandDrivenPipeline::UPDATE_TIME_STEPS()))
        t = outInfo->Get(vtkStreamingDemandDrivenPipeline::UPDATE_TIME_STEPS())[0];
      int index = static_cast<int>(t);
      vtkPolyData* markers = this->mp_MarkersCoordinates->operator[](index);
      vtkIntArray* existingMarkers = this->mp_ExistingMarkers->operator[](index);

      // Visible markers generation
      this->mp_MarkersGenerator->SetInput(markers);
      this->mp_MarkersGenerator->Modified();
      this->mp_MarkersGenerator->SetExistingMarkers(existingMarkers);
      this->mp_MarkersGenerator->Update();
      output->ShallowCopy(this->mp_MarkersGenerator->GetOutput());
      // Invoke btk::VTKMarkersListUpdateEvent
      this->InvokeEvent(btk::VTKMarkersListUpdateEvent, static_cast<void*>(this->mp_ExistingMarkers->operator[](index)));
    }
    else
      output->Initialize();

    return 1;
  };

  /*
  int VTKMarkersFramesSource::RequestUpdateExtent(vtkInformation *vtkNotUsed(request), vtkInformationVector **inputVector, vtkInformationVector *outputVector)
  {
    vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
    vtkInformation *outInfo = outputVector->GetInformationObject(0);

    inInfo->Set(vtkStreamingDemandDrivenPipeline::UPDATE_PIECE_NUMBER(),
                outInfo->Get(vtkStreamingDemandDrivenPipeline::UPDATE_PIECE_NUMBER()));
    inInfo->Set(vtkStreamingDemandDrivenPipeline::UPDATE_NUMBER_OF_PIECES(),
                outInfo->Get(vtkStreamingDemandDrivenPipeline::UPDATE_NUMBER_OF_PIECES()));
    inInfo->Set(vtkStreamingDemandDrivenPipeline::UPDATE_NUMBER_OF_GHOST_LEVELS(),
                outInfo->Get(vtkStreamingDemandDrivenPipeline::UPDATE_NUMBER_OF_GHOST_LEVELS()));
    inInfo->Set(vtkStreamingDemandDrivenPipeline::EXACT_EXTENT(), 1);

    return 1;
  }
  */

  /**
   * Sets the type of object required for the input.
   */
  int VTKMarkersFramesSource::FillInputPortInformation(int /* port */, 
                                                      vtkInformation* info)
  {
    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "VTKDataObjectAdapter");
    return 1;
  }
};


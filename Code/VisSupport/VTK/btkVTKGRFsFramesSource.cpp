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

#include "btkVTKGRFsFramesSource.h"
#include "btkVTKDataObjectAdapter.h"

#include <vtkstd/vector>
#include <vtkPolyData.h>
#include <vtkLineSource.h>
#include <vtkAppendPolyData.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkStreamingDemandDrivenPipeline.h>


namespace btk
{
  /**
   * @class VTKGRFsComponents btkVTKGRFsFramesSource.h
   * @brief Store ground reaction forces vector as vector of vtkPolyData
   */
  class VTKGRFsComponents : public vtkstd::vector<vtkPolyData*>
  {};
  
  /** 
   * @class VTKGRFsFramesSource
   * @brief Display ground reaction forces (GRF).
   *
   * This filter compute all GRFs when a new input is set or modified (see VTKGRFsFramesSource::RequestInformation).
   * The display of GRFs for each frame use the vtk TIME mechanism by setting the required frame
   * to a vtkStreamingDemandDrivenPipeline object and call the vtkRenderer::Renderer() method.
   * For example:
   * @code
   * // BTK
   * btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
   * reader->SetDisableFilenameExceptionState(true);
   * btk::ForcePlatformsExtractor::Pointer forcePlatformsExtractor = btk::ForcePlatformsExtractor::New();
   * forcePlatformsExtractor->SetInput(reader->GetOutput());
   * btk::GroundReactionWrenchFilter::Pointer GRWsFilter = btk::GroundReactionWrenchFilter::New();
   * GRWsFilter->SetThresholdValue(5.0); // PWA are not computed from vertical forces lower than 5 newtons.
   * GRWsFilter->SetThresholdState(true);
   * GRWsFilter->SetInput(forcePlatformsExtractor->GetOutput());
   * // VTK
   * vtkRenderer* renderer = vtkRenderer::New()
   * btk::VTKGRFsFramesSource* GRFs = btk::VTKGRFsFramesSource::New();
   * GRFs->SetInput(GRWsFilter->GetOutput());
   * vtkPolyDataMapper* mapper = vtkPolyDataMapper::New();
   * mapper->SetInputConnection(GRFs->GetOutputPort());
   * vtkActor* actor = vtkActor::New();
   * actor->SetMapper(mapper);
   * renderer->AddActor(actor);
   * // ...
   * int numberOfFrames = reader->GetAnalogFrameNumber();
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
   * @fn static VTKGRFsFramesSource* VTKGRFsFramesSource::New();
   * Creates a VTKGRFsFramesSource object and return it as a pointer.
   */
  vtkStandardNewMacro(VTKGRFsFramesSource);
  vtkCxxRevisionMacro(VTKGRFsFramesSource, "$Revision: 0.1 $");
  
  /**
   * Prints only the Superclass information.
   */
  void VTKGRFsFramesSource::PrintSelf(ostream& os, vtkIndent indent)
  {
    this->Superclass::PrintSelf(os,indent);
  }

  /**
   * Set filter's input.
   */
  void VTKGRFsFramesSource::SetInput(WrenchCollection::Pointer input)
  {
    VTKDataObjectAdapter* inObject = VTKDataObjectAdapter::New();
    inObject->SetBTKDataObject(input);
    this->vtkPolyDataAlgorithm::SetInput(inObject);
    inObject->Delete();
  };
  
  /**
   * @fn double VTKGRFsFramesSource::GetScaleUnit()
   * Returns the scale factor used to set GRFs' positions (i.e. the point of wrench application (PWA)).
   *
   * Usefull when visualized acquistion date are set in meter, millimeter, inch, etc.
   */
  
  /**
   * @fn void VTKGRFsFramesSource::SetScaleUnit(double s)
   * Sets the scale factor used to set GRFs' positions.
   *
   * Usefull when visualized acquistion date are set in meter, millimeter, inch, etc.
   */
  
  /**
   * Constructor.
   *
   * Filter with one input and output.
   */
  VTKGRFsFramesSource::VTKGRFsFramesSource()
  : vtkPolyDataAlgorithm()
  {
    this->mp_GRFsComponents = new VTKGRFsComponents();
    this->mp_Scale = 1.0;
  };
  
  /**
   * Destructor.
   */
  VTKGRFsFramesSource::~VTKGRFsFramesSource()
  {
    for (size_t i = 0 ; i < this->mp_GRFsComponents->size() ; ++i)
      this->mp_GRFsComponents->operator[](i)->Delete();
    delete this->mp_GRFsComponents;
  };

  /**
   * Generate GRFs' vectors
   */
  int VTKGRFsFramesSource::RequestInformation(vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector)
  {
    btkNotUsed(request)
    // Convert a btk::PointCollection into a collection of polydata containing only vertices.
    vtkInformation* inInfo = inputVector[0]->GetInformationObject(0);
    VTKDataObjectAdapter* inObject = VTKDataObjectAdapter::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
    if (!inObject)
      return 0;
    if (inObject->GetMTime() < this->GetMTime())
      return 0;
    WrenchCollection::Pointer input = static_pointer_cast<WrenchCollection>(inObject->GetBTKDataObject());
    // Raz
    for (size_t i = 0 ; i < this->mp_GRFsComponents->size() ; ++i)
      this->mp_GRFsComponents->operator[](i)->Delete();
    this->mp_GRFsComponents->clear();
    int frameNumber = 0;
    int wrenchNumber = input->GetItemNumber();
    if (wrenchNumber != 0)
    {
      frameNumber = input->GetItem(0)->GetPosition()->GetFrameNumber();
      this->mp_GRFsComponents->resize(frameNumber);
      vtkAppendPolyData* append = vtkAppendPolyData::New();
      append->UserManagedInputsOn();
      append->SetNumberOfInputs(wrenchNumber);
      vtkLineSource* lineGenerator = vtkLineSource::New();

      for (int i = 0 ; i < frameNumber ; ++i)
      {
        int wrenchIdx = 0;
        WrenchCollection::ConstIterator it = input->Begin();
        while (it != input->End())
        {
          if ((*it)->GetPosition()->GetResiduals().coeff(i) != -1.0)
          {
            double* positions = (*it)->GetPosition()->GetValues().data();
            double* forces = (*it)->GetForce()->GetValues().data();
            lineGenerator->SetPoint1(positions[i] * this->mp_Scale,
                                     positions[i + frameNumber] * this->mp_Scale,
                                     positions[i + 2 * frameNumber] * this->mp_Scale);
            lineGenerator->SetPoint2(positions[i] * this->mp_Scale + forces[i], 
                                     positions[i + frameNumber] * this->mp_Scale + forces[i + frameNumber], 
                                     positions[i + 2 * frameNumber] * this->mp_Scale + forces[i + 2 * frameNumber]);

            lineGenerator->Update();
            vtkPolyData* GRF = vtkPolyData::New();
            GRF->DeepCopy(lineGenerator->GetOutput());
            append->SetInputByNumber(wrenchIdx, GRF);
            GRF->Delete();
          }
          else
            append->SetInputByNumber(wrenchIdx, 0);

          ++it; ++wrenchIdx;
        }
        append->Update();
        vtkPolyData* GRFs = vtkPolyData::New();
        GRFs->DeepCopy(append->GetOutput());
        this->mp_GRFsComponents->operator[](i) = GRFs;
      }
      lineGenerator->Delete();
      append->Delete();
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
    outRange[0] = 0;
    outRange[1] = frameNumber - 1;
    outInfo->Set(vtkStreamingDemandDrivenPipeline::TIME_RANGE(), outRange, 2);

    return 1;
  };

  /**
   * Extract GRFs frame required by a vtkStreamingDemandDrivenPipeline object.
   */
  int VTKGRFsFramesSource::RequestData(vtkInformation* request,
                                       vtkInformationVector** inputVector,
                                       vtkInformationVector* outputVector)
  {
    btkNotUsed(request);
    btkNotUsed(inputVector);
    vtkInformation* outInfo = outputVector->GetInformationObject(0);    
    vtkPolyData* output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

    if (this->mp_GRFsComponents->size() != 0)
    {
      double t = 0;
      if (outInfo->Has(vtkStreamingDemandDrivenPipeline::UPDATE_TIME_STEPS()))
        t = outInfo->Get(vtkStreamingDemandDrivenPipeline::UPDATE_TIME_STEPS())[0];
      outInfo->Set(vtkDataObject::DATA_TIME_STEPS(), &t, 1);
      vtkPolyData* poly = vtkPolyData::SafeDownCast(this->mp_GRFsComponents->operator[](static_cast<int>(t)));
      output->ShallowCopy(poly);
    }
    else
      output->Initialize();

    return 1;
  };

  /*
  int VTKGRFsFramesSource::RequestUpdateExtent(vtkInformation *request, vtkInformationVector **inputVector, vtkInformationVector *outputVector)
  {
    btkNotUsed(request);
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
  int VTKGRFsFramesSource::FillInputPortInformation(int port, 
                                                    vtkInformation* info)
  {
    btkNotUsed(port);
    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "VTKDataObjectAdapter");
    return 1;
  }
};


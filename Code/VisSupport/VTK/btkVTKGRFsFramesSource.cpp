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

#include "btkVTKGRFsFramesSource.h"
#include "btkVTKDataObjectAdapter.h"

#include <vtkPolyData.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkStreamingDemandDrivenPipeline.h>
#include <vtkCellArray.h>

#include <btkConvert.h>

namespace btk
{
  /**
   * @class VTKGRFsFramesSource::VTKGRFsComponents btkVTKGRFsFramesSource.h
   * @brief Store ground reaction forces vector as vector of vtkPolyData
   */
  class VTKGRFsFramesSource::VTKGRFsComponents : public std::vector<vtkCellArray*>
  {};
  
  /**
   * @class VTKGRFsFramesSource::VTKGRFsPathPath btkVTKGRFsFramesSource.h
   * @brief List of ids representing ground reaction forces' path.
   */
  class VTKGRFsFramesSource::VTKGRFsPathPath : public std::vector<vtkIdList*>
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
   * The butterfly diagram is inspired from Khodadadeh (1988) and especialy the representation of the single (one leg) support.
   *
   * @par Reference
   * Khodadadeh S.@n
   * <em>Vector (butterfly) diagrams for osteoarthritic gait a preliminary report</em>.@n
   * Journal of Medical Engineering & Technology, <b>1988</b>, 12(1), 15-19.
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
#if (VTK_MAJOR_VERSION >= 6)
    this->vtkPolyDataAlgorithm::SetInputData(inObject);
#else
    this->vtkPolyDataAlgorithm::SetInput(inObject);
#endif
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
   * Returns visibility state of GRF position's trajectory
   */
  bool VTKGRFsFramesSource::GetPathVisibility(int idx)
  {
    return (this->mp_GRFsPathActivated->GetValue(idx) == 1);
  };
   
  /**
   * Sets visibility state of GRF position's trajectory
   */
  void VTKGRFsFramesSource::SetPathVisibility(int idx, bool visible)
  {
    this->mp_GRFsPathActivated->SetValue(idx, visible ? 1 : 0);
  };
  
  /**
   * Convenient method to show GRF position's trajectory.
   */
  void VTKGRFsFramesSource::ShowPath(int idx)
  {
    this->SetPathVisibility(idx, true);
  };
  
  /**
   * Convenient method to show all GRF positions' trajectory.
   */
  void VTKGRFsFramesSource::ShowPaths()
  {
    for (int i = 0 ; i < this->mp_GRFsPathActivated->GetNumberOfTuples() ; ++i)
      this->mp_GRFsPathActivated->SetValue(i, 1);
  };
  
  /**
   * Convenient method to hide GRF position's trajectory.
   */
  void VTKGRFsFramesSource::HidePath(int idx)
  {
    this->SetPathVisibility(idx, false);
  };
  
  /**
   * Convenient method to hide all GRF positions' trajectory.
   */
  void VTKGRFsFramesSource::HidePaths()
  {
    for (int i = 0 ; i < this->mp_GRFsPathActivated->GetNumberOfTuples() ; ++i)
      this->mp_GRFsPathActivated->SetValue(i, 0);
  };
  
  /**
   * Constructor.
   *
   * Filter with one input and output.
   */
  VTKGRFsFramesSource::VTKGRFsFramesSource()
  : vtkPolyDataAlgorithm()
  {
    this->SetNumberOfOutputPorts(2);
    
    this->mp_GRFsComponents = new VTKGRFsComponents();
    this->mp_GRFsPathIds = new VTKGRFsComponents();
    this->mp_GRFsPathActivated = vtkIntArray::New();
    this->m_Scale = 1.0;
    this->mp_ButterflyCache = vtkPolyData::New();
    this->m_LastCachedFrame = -1;
    this->m_ButterflyActivated = false;
  };
  
  /**
   * Destructor.
   */
  VTKGRFsFramesSource::~VTKGRFsFramesSource()
  {
    for (size_t i = 0 ; i < this->mp_GRFsComponents->size() ; ++i)
      this->mp_GRFsComponents->operator[](i)->Delete();
    delete this->mp_GRFsComponents;
    for (size_t i = 0 ; i < this->mp_GRFsPathIds->size() ; ++i)
      this->mp_GRFsPathIds->operator[](i)->Delete();
    delete this->mp_GRFsPathIds;
    this->mp_GRFsPathActivated->Delete();
    this->mp_ButterflyCache->Delete();
  };
  
  /**
   * @fn bool VTKGRFsFramesSource::GetButterflyActivation() const
   * Returns the state for the activation of the butterfly.
   */
  
  /**
   * @fn void VTKGRFsFramesSource::SetButterflyActivation(bool activated)
   * Sets the state for the activation of the butterfly.
   *
   * @warning Modifying this parameter doesn't modify this block.
   */

  /**
   * @fn bool VTKGRFsFramesSource::ButterflyActivationOn()
   * Activates the butterfly representation.
   *
   * @warning Modifying this parameter doesn't modify this block.
   */
  
  /**
   * @fn bool VTKGRFsFramesSource::ButterflyActivationOff()
   * Desactivates the butterfly representation.
   *
   * @warning Modifying this parameter doesn't modify this block.
   */
  
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
      return 1;
    WrenchCollection::Pointer input = static_pointer_cast<WrenchCollection>(inObject->GetBTKDataObject());
    // Reset
    for (size_t i = 0 ; i < this->mp_GRFsComponents->size() ; ++i)
      this->mp_GRFsComponents->operator[](i)->Delete();
    this->mp_GRFsComponents->clear();
    for (size_t i = 0 ; i < this->mp_GRFsPathIds->size() ; ++i)
      this->mp_GRFsPathIds->operator[](i)->Delete();
    this->mp_GRFsPathIds->clear();
    this->mp_ButterflyCache->Initialize();
    this->m_LastCachedFrame = -1;
    // Compute the force vectors geometry
    int frameNumber = 0;
    int wrenchNumber = input->GetItemNumber();
    if (wrenchNumber != 0)
    {
      frameNumber = input->GetItem(0)->GetPosition()->GetFrameNumber();
      vtkPoints* points = vtkPoints::New(); points->SetNumberOfPoints(2*frameNumber*wrenchNumber); // 2 points by wrench
      VTKGRFsPathPath trajectoryPaths = VTKGRFsPathPath();
      trajectoryPaths.resize(wrenchNumber, 0);
      this->mp_GRFsPathIds->resize(wrenchNumber);
      this->mp_GRFsPathActivated->SetNumberOfValues(wrenchNumber);
      for (int i = 0 ; i < wrenchNumber ; ++i)
      {
        this->mp_GRFsPathIds->operator[](i) = vtkCellArray::New();
        trajectoryPaths[i] = vtkIdList::New();
        this->mp_GRFsPathActivated->SetValue(i, 0);
      }
      this->mp_GRFsComponents->resize(frameNumber);
      vtkIdType ptId = 0;
      for (int i = 0 ; i < frameNumber ; ++i)
      {
        int ptIdx = 0;
        vtkCellArray* GRFs = vtkCellArray::New();
        WrenchCollection::ConstIterator it = input->Begin();
        while (it != input->End())
        {
          if ((*it)->GetPosition()->GetResiduals().coeff(i) >= 0.0)
          {
            double* positions = (*it)->GetPosition()->GetValues().data();
            double* forces = (*it)->GetForce()->GetValues().data();
            
            vtkIdType pt1Id = ptId++;
            vtkIdType pt2Id = ptId++;
            // The point on the force platform is slightly shifted for visibilty reason.
            // Otherwise the trajectory is not visible if the force platform geometry is shown (using the filter btk::VTKForcePlatformsSource).
            // This shift is done on all the coordinates to be independant of the orientation of the force platform.
            points->SetPoint(pt1Id,
                             positions[i] * this->m_Scale + 0.1,
                             positions[i + frameNumber] * this->m_Scale + 0.1,
                             positions[i + 2 * frameNumber] * this->m_Scale + 0.1);
            points->SetPoint(pt2Id,
                             positions[i] * this->m_Scale + forces[i], 
                             positions[i + frameNumber] * this->m_Scale + forces[i + frameNumber], 
                             positions[i + 2 * frameNumber] * this->m_Scale + forces[i + 2 * frameNumber]);
            GRFs->InsertNextCell(2);
            GRFs->InsertCellPoint(pt1Id);
            GRFs->InsertCellPoint(pt2Id);
            
            trajectoryPaths[ptIdx]->InsertNextId(pt1Id);
          }
          ++ptIdx;
          ++it;
        }
        points->SetNumberOfPoints(ptId); // Resize to the final number of points
        this->mp_GRFsComponents->operator[](i) = GRFs;
      }
      for (size_t i = 0 ; i < this->mp_GRFsPathIds->size() ; ++i)
      {
        this->mp_GRFsPathIds->operator[](i)->InsertNextCell(trajectoryPaths[i]);
        trajectoryPaths[i]->Delete();
      }
      
      this->mp_ButterflyCache->SetPoints(points);
      points->Delete();
      vtkCellArray* lines = vtkCellArray::New();
      this->mp_ButterflyCache->SetLines(lines);
      lines->Delete(); 
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
    // Force vectors
    vtkInformation* outInfo = outputVector->GetInformationObject(0);    
    vtkPolyData* output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));
    // Paths
    vtkInformation* outInfo2 = outputVector->GetInformationObject(1);
    vtkPolyData* output2 = vtkPolyData::SafeDownCast(outInfo2->Get(vtkDataObject::DATA_OBJECT()));
    
    if (this->mp_GRFsComponents->size() != 0)
    {
      int t = 0;
#if (VTK_MAJOR_VERSION >= 6)
      if (outInfo->Has(vtkStreamingDemandDrivenPipeline::UPDATE_TIME_STEP()))
        t = static_cast<int>(outInfo->Get(vtkStreamingDemandDrivenPipeline::UPDATE_TIME_STEP()));
#else
      if (outInfo->Has(vtkStreamingDemandDrivenPipeline::UPDATE_TIME_STEPS()))
        t = static_cast<int>(outInfo->Get(vtkStreamingDemandDrivenPipeline::UPDATE_TIME_STEPS())[0]);
#endif        
      if (this->m_ButterflyActivated)
      {
        if (t < this->m_LastCachedFrame)
        {
          this->m_LastCachedFrame = -1;
          this->mp_ButterflyCache->GetLines()->Reset();
        }
      
        if (t > this->m_LastCachedFrame)
        {
          for (int i = this->m_LastCachedFrame+1 ; i <= t ; ++i)
          {
            vtkCellArray* GRFs = vtkCellArray::SafeDownCast(this->mp_GRFsComponents->operator[](i));
            vtkIdType* pts = 0;
            vtkIdType npts = 0;
            for (GRFs->InitTraversal() ; GRFs->GetNextCell(npts,pts) ; )
            {
              this->mp_ButterflyCache->GetLines()->InsertNextCell(npts);
              for (vtkIdType j = 0 ; j < npts; ++j)
                this->mp_ButterflyCache->GetLines()->InsertCellPoint(pts[j]);
            }
          }
          this->m_LastCachedFrame = t;
        }
      }
      else
      {
        this->m_LastCachedFrame = -1;
        vtkCellArray* GRFs = vtkCellArray::SafeDownCast(this->mp_GRFsComponents->operator[](t));
        this->mp_ButterflyCache->GetLines()->DeepCopy(GRFs);
      }
      output->ShallowCopy(this->mp_ButterflyCache);
      
      // Path generation
      vtkCellArray* trajectoryIds = vtkCellArray::New();
      for (int i = 0 ; i < this->mp_GRFsPathActivated->GetNumberOfTuples() ; ++i)
      {
        if (this->mp_GRFsPathActivated->GetValue(i) == 0)
          continue;
        vtkIdType npts;
        vtkIdType* pts;
        this->mp_GRFsPathIds->operator[](i)->InitTraversal();
        while (this->mp_GRFsPathIds->operator[](i)->GetNextCell(npts, pts))
          trajectoryIds->InsertNextCell(npts, pts);
      }
      output2->SetPoints(this->mp_ButterflyCache->GetPoints());
      output2->SetVerts(trajectoryIds);
      trajectoryIds->Delete();
    }
    else
    {
      output->Initialize();
      output2->Initialize();
    }
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


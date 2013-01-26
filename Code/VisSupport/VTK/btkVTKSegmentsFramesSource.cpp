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

#include "btkVTKSegmentsFramesSource.h"
#include "btkVTKDataObjectAdapter.h"

#include <vtkPolyData.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkObjectFactory.h>
#include <vtkStreamingDemandDrivenPipeline.h>
#include <vtkstd/vector>
#include <vtkCellArray.h>
#include <vtkCellData.h>

namespace btk
{
  /** 
   * @class VTKSegmentsFramesSource btkVTKSegmentsFramesSource.h
   * @brief Display segments with interactive possibilities as hide/show segment, modify colors.
   * 
   * The creation of segments requires first the trajectory of markers (given by the input) and 
   * second the definition of segments
   * A segment is composed of markers' ID and the links between markers' ID. A link is a pair of 
   * IDs and will be used to draw a line between markers. From the links a mesh is created but not displayed.
   * To display it, you have to use the method SetSegmentSurfaceVisibility().
   * The links are are available on the outport port 0 of the filter, while the meshes are on the port 1.
   * @code
   * // BTK
   * btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
   * reader->SetDisableFilenameExceptionState(true);
   * btk::SpecializedPointsExtractor::Pointer markersExtractor = btk::SpecializedPointsExtractor::New();
   * markersExtractor->SetInput(reader->GetOutput());
   * // VTK
   * btk::VTKSegmentsFramesSource* segmentsFrameSource = btk::VTKSegmentsFramesSource::New();
   * vtkRenderer* renderer = vtkRenderer::New()
   * segmentsFrameSource->SetInput(markersExtractor->GetOutput());
   * vtkPolyDataMapper* mapper = vtkPolyDataMapper::New();
   * mapper->SetInputConnection(segmentsFrameSource->GetOutputPort(0));
   * mapper->SetLookupTable(segmentsFrameSource->GetMarkerColorLUT());
   * mapper->SetScalarModeToUseCellData();
   * mapper->UseLookupTableScalarRangeOn();
   * mapper->SelectColorArray("Colors");
   * vtkActor* actor = vtkActor::New();
   * actor->SetMapper(mapper);
   * renderer->AddActor(actor);
   * // Segments definition
   * std::vector<int> pointIds;
   * std::vector<btk::VTKSegmentsFramesSource::Link> links;
   * // - Pelvis
   * pointIds = std::vector<int>(3);
   * pointIds[0] = 3;
   * pointIds[1] = 10;
   * pointIds[2] = 14;
   * links = std::vector<btk::VTKSegmentsFramesSource::Link>(3);
   * links[0] = btk::VTKSegmentsFramesSource::Link(3,10);
   * links[1] = btk::VTKSegmentsFramesSource::Link(3,14);
   * links[2] = btk::VTKSegmentsFramesSource::Link(10,14);
   * segmentsFrameSource->AppendDefinition(pointIds, links);
   * // Code cleanup
   * @endcode
   *
   * @ingroup BTKVTK
   */
   
  /**
   * @typedef VTKSegmentsFramesSource::Link
   * Simple pair of indices respresenting the id of two markers used to create the link.
   */
  
  /**
   * @typedef VTKSegmentsFramesSource::Face
   * Simple triad of indices respresenting the id of three markers used to create the face.
   */
  
  /**
   * @fn static VTKSegmentsFramesSource* VTKSegmentsFramesSource::New();
   * Creates a VTKSegmentsFramesSource object and return it as a pointer.
   */
  vtkStandardNewMacro(VTKSegmentsFramesSource);
  vtkCxxRevisionMacro(VTKSegmentsFramesSource, "$Revision: 0.1 $");
  
  /**
   * Prints Superclass informations.
   */
  void VTKSegmentsFramesSource::PrintSelf(ostream& os, vtkIndent indent)
  {
    this->Superclass::PrintSelf(os,indent);
  };

  /**
   * Sets input.
   */
  void VTKSegmentsFramesSource::SetInput(PointCollection::Pointer input)
  {
    VTKDataObjectAdapter* inObject = VTKDataObjectAdapter::New();
    inObject->SetBTKDataObject(input);
    this->vtkPolyDataAlgorithm::SetInput(inObject);
    inObject->Delete();
  };
  
  /**
   * Remove all the defined segments.
   */
  void VTKSegmentsFramesSource::ClearDefinitions()
  {
    if (this->m_Definitions.empty())
      return;
    this->m_Definitions.clear();
    this->mp_VisibleSegments->Initialize(); // Freed the allocated memory
    this->mp_SegmentsColorIndex->Initialize(); // Freed the allocated memory
  };
  
  /**
   * Create a new segment, append it to the list and returns its ID.
   */
  int VTKSegmentsFramesSource::AppendDefinition(const std::vector<int>& markerIds, const std::vector<Link>& links,  const std::vector<Face>& faces, bool surfaceVisible)
  {
    this->m_Definitions.push_back(SegmentDefinition(markerIds, links, faces, surfaceVisible));
    this->mp_VisibleSegments->InsertNextValue(1); // Set as visible
    this->mp_SegmentsColorIndex->InsertNextValue(0); // Insert default color
    return this->mp_VisibleSegments->GetNumberOfTuples()-1;
  };
  
  /**
   * Create a new segment (only from vertices and links, faces are created automatically), append it to the list and returns its ID.
   */
  int VTKSegmentsFramesSource::AppendDefinition(const std::vector<int>& markerIds, const std::vector<Link>& links, bool surfaceVisible)
  {
    this->m_Definitions.push_back(SegmentDefinition(markerIds, links, surfaceVisible));
    this->mp_VisibleSegments->InsertNextValue(1); // Set as visible
    this->mp_SegmentsColorIndex->InsertNextValue(0); // Insert default color
    return this->mp_VisibleSegments->GetNumberOfTuples()-1;
  };
  
  /**
   * Create a new segment using an already existing mesh, append it to the list and returns its ID.
   */
  int VTKSegmentsFramesSource::AppendDefinition(TriangleMesh::Pointer mesh, bool surfaceVisible)
  {
    this->m_Definitions.push_back(SegmentDefinition(mesh, surfaceVisible));
    this->mp_VisibleSegments->InsertNextValue(1); // Set as visible
    this->mp_SegmentsColorIndex->InsertNextValue(0); // Insert default color
    return this->mp_VisibleSegments->GetNumberOfTuples()-1;
  };
  
  /**
   * Sets the definition (markers + links + faces) for the segment with the index @a id. The segment must already exists.
   */
  void VTKSegmentsFramesSource::SetDefinition(vtkIdType id, const std::vector<int>& markerIds, const std::vector<Link>& links, const std::vector<Face>& faces)
  {
    if (id >= this->mp_VisibleSegments->GetNumberOfTuples())
    {
      vtkErrorMacro("Out of range.");
      return;
    }
    std::list<SegmentDefinition>::iterator it = this->m_Definitions.begin();
    std::advance(it, id);
    it->mesh->SetDefinition(markerIds, links, faces);
  };
  
  /**
   * Sets the definition (markers + links) for the segment with the index @a id. The segment must already exists.
   */
  void VTKSegmentsFramesSource::SetDefinition(vtkIdType id, const std::vector<int>& markerIds, const std::vector<Link>& links)
  {
    if (id >= this->mp_VisibleSegments->GetNumberOfTuples())
    {
      vtkErrorMacro("Out of range.");
      return;
    }
    std::list<SegmentDefinition>::iterator it = this->m_Definitions.begin();
    std::advance(it, id);
    it->mesh->SetDefinition(markerIds, links);
  };
  
  /**
   * Sets the definition for the segment with the index @a id. The segment must already exists.
   */
  void VTKSegmentsFramesSource::SetDefinition(vtkIdType id, TriangleMesh::Pointer mesh)
  {
    if (id >= this->mp_VisibleSegments->GetNumberOfTuples())
    {
      vtkErrorMacro("Out of range.");
      return;
    }
    std::list<SegmentDefinition>::iterator it = this->m_Definitions.begin();
    std::advance(it, id);
    it->mesh = mesh;
  };
  
  /**
   * @fn int VTKSegmentsFramesSource::GetNumberOfDefinitions() const
   * Returns the number of definition set for this source.
   */
  
  /**
   * Returns the mesh defining the segment corresponding to the index @a id.
   */
  TriangleMesh::Pointer VTKSegmentsFramesSource::GetDefinition(vtkIdType id)
  {
    if (id >= this->mp_VisibleSegments->GetNumberOfTuples())
    {
      vtkErrorMacro("Out of range.");
      return TriangleMesh::Pointer();
    }
    std::list<SegmentDefinition>::const_iterator it = this->m_Definitions.begin();
    std::advance(it, id);
    return it->mesh;
  };

  /**
   * Returns the visibility of the segment with index @a id.
   */
  bool VTKSegmentsFramesSource::GetSegmentVisibility(vtkIdType id)
  {
    if (id >= this->mp_VisibleSegments->GetNumberOfTuples())
    {
      vtkErrorMacro("Out of range.");
      return false;
    }
    return this->mp_VisibleSegments->GetValue(id) ? true : false;
  };
  
  /**
   * Sets the visibility of the segment with index @a id.
   */
  void VTKSegmentsFramesSource::SetSegmentVisibility(vtkIdType id, bool visible)
  {
    if (id >= this->mp_VisibleSegments->GetNumberOfTuples())
    {
      vtkErrorMacro("Out of range.");
      return;
    }
    if (visible)
      this->mp_VisibleSegments->SetValue(id, 1);
    else
      this->mp_VisibleSegments->SetValue(id, 0);
  };
  
  /**
   * Returns the visibility of the surface created for the segment with index @a id.
   */
  bool VTKSegmentsFramesSource::GetSegmentSurfaceVisibility(vtkIdType id)
  {
    if (id >= this->mp_VisibleSegments->GetNumberOfTuples())
    {
      vtkErrorMacro("Out of range.");
      return false;
    }
    std::list<SegmentDefinition>::iterator it = this->m_Definitions.begin();
    std::advance(it, id);
    return it->surfaceEnabled;
  };
  
  /**
   * Sets the visibility of the surface created for segment with index @a id.
   */
  void VTKSegmentsFramesSource::SetSegmentSurfaceVisibility(vtkIdType id, bool visible)
  {
    if (id >= this->mp_VisibleSegments->GetNumberOfTuples())
    {
      vtkErrorMacro("Out of range.");
      return;
    }
    std::list<SegmentDefinition>::iterator it = this->m_Definitions.begin();
    std::advance(it, id);
    it->surfaceEnabled = visible;
  };

  /**
   * Returns colors associated with the segment which has the index @a id
   */
  double* VTKSegmentsFramesSource::GetSegmentColor(vtkIdType id)
  {
    if (id >= this->mp_SegmentsColorIndex->GetNumberOfTuples())
    {
      vtkErrorMacro("Out of range.");
      return 0;
    }
    return this->mp_SegmentsColorsLUT->GetTableValue(this->mp_SegmentsColorIndex->GetValue(id));
  };

  /**
   * Sets segments' color.
   */
  void VTKSegmentsFramesSource::SetSegmentsColor(vtkIdTypeArray* ids, double r, double g, double b)
  {
    int num = this->mp_SegmentsColorsLUT->GetNumberOfTableValues();
    int i = 0;
    for (i = 0 ; i < num ; ++i)
    {
      double* color = this->mp_SegmentsColorsLUT->GetTableValue(i);
      if ((r == color[0]) && (g == color[1]) && (b == color[2]))
        break;
    }
    if (i >= num)
    {
      this->mp_SegmentsColorsLUT->SetNumberOfTableValues(num + 1);
      this->mp_SegmentsColorsLUT->SetTableValue(num, r, g, b);
      this->mp_SegmentsColorsLUT->SetTableRange(0, num + 1);
      i = num;
    }
    for (int j = 0 ; j < ids->GetNumberOfTuples() ; ++j)
      this->mp_SegmentsColorIndex->SetValue(ids->GetValue(j), i);
  };

  /**
   * Return color's index of the segment with index @a id.
   */
  vtkIdType VTKSegmentsFramesSource::GetSegmentColorIndex(vtkIdType id)
  {
    if (id >= this->mp_SegmentsColorIndex->GetNumberOfTuples())
    {
      vtkErrorMacro("Out of range.");
      return 0;
    }
    return this->mp_SegmentsColorIndex->GetValue(id);
  };

  /**
   * Sets color's index of the segment with index @a id.
   */
  void VTKSegmentsFramesSource::SetSegmentColorIndex(vtkIdType id, vtkIdType color)
  {
    if (id >= this->mp_SegmentsColorIndex->GetNumberOfTuples())
    {
      vtkErrorMacro("Out of range.");
      return;
    }
    this->mp_SegmentsColorIndex->SetValue(id, color);
  };

  /**
   * Returns lookup table between colors and their index given to each segment.
   */
  vtkLookupTable* VTKSegmentsFramesSource::GetSegmentColorLUT()
  {
    return this->mp_SegmentsColorsLUT;
  };
  
  /**
   * Tag the segment with the index @a id as hidden.
   */
  void VTKSegmentsFramesSource::HideSegment(vtkIdType id)
  {
    this->SetSegmentVisibility(id, 0);
  };

  /**
   * Hides all the segments.
   */
  void VTKSegmentsFramesSource::HideSegments()
  {
    for (int i = 0 ; i < this->mp_VisibleSegments->GetNumberOfTuples() ; ++i)
      this->mp_VisibleSegments->SetValue(i, 0);
  };

  /**
   * Tag the segment with the index @a id as visible.
   */
  void VTKSegmentsFramesSource::ShowSegment(vtkIdType id)
  {
    this->SetSegmentVisibility(id, 1);
  };

  /**
   * Shows all the segments.
   */
  void VTKSegmentsFramesSource::ShowSegments()
  {
    for (int i = 0 ; i < this->mp_VisibleSegments->GetNumberOfTuples() ; ++i)
      this->mp_VisibleSegments->SetValue(i, 1);
  };
  
  /**
   * @fn double VTKSegmentsFramesSource::GetScaleUnit()
   * Returns the scale factor used to adapt segments' positions
   *
   * Usefull when visualized acquistion data are set in meter, millimeter, inch, etc.
   */
  
  /**
   * @fn void VTKSegmentsFramesSource::SetScaleUnit(double s)
   * Sets the scale factor used to adapt segments' positions
   *
   * Usefull when visualized acquistion date are set in meter, millimeter, inch, etc.
   */
     
  /**
   * Constructor.
   *
   * The generated segments have the following default visual parameters:
   * - White color;
   * These parameters can be modified by the appriopriate method.
   */
  VTKSegmentsFramesSource::VTKSegmentsFramesSource()
  : vtkPolyDataAlgorithm(), m_Definitions()
  {
    this->SetNumberOfInputPorts(1);
    this->SetNumberOfOutputPorts(2);
    
    this->mp_VisibleSegments = vtkIntArray::New();
    this->mp_SegmentsColorIndex = vtkIdTypeArray::New();
    this->mp_SegmentsColorsLUT = vtkLookupTable::New();
    this->mp_SegmentsColorsLUT->SetNumberOfTableValues(1);

    // Default values
    this->m_Scale = 1.0;
    this->mp_SegmentsColorsLUT->SetTableValue(0, 1.0, 1.0, 1.0); // White
  };
  
  /**
   * Destructor.
   */
  VTKSegmentsFramesSource::~VTKSegmentsFramesSource()
  {
    this->mp_VisibleSegments->Delete();
    this->mp_SegmentsColorIndex->Delete();
    this->mp_SegmentsColorsLUT->Delete();
  };

  /**
   * Prepare data
   */
  int VTKSegmentsFramesSource::RequestInformation(vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector)
  {
    btkNotUsed(request);
    
    vtkInformation* inInfo = inputVector[0]->GetInformationObject(0);
    VTKDataObjectAdapter* inObject = VTKDataObjectAdapter::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
    if (!inObject)
      return 0;

    PointCollection::Pointer input = static_pointer_cast<PointCollection>(inObject->GetBTKDataObject());
    int  frameNumber = 0;
    if (!input->IsEmpty())
      frameNumber = input->GetItem(0)->GetFrameNumber();
    
    // Update output informations
    vtkInformation* outInfo = outputVector->GetInformationObject(0);
    // TIME_STEPS
    double* outTimes = new double [frameNumber];
    for (int i = 0; i < frameNumber; ++i)
      outTimes[i] = i;
    outInfo->Set(vtkStreamingDemandDrivenPipeline::TIME_STEPS(), outTimes, frameNumber);
    delete [] outTimes;
    // TIME_RANGE
    double outRange[2];
    outRange[0] = 0.0;
    outRange[1] = frameNumber - 1.0;
    outInfo->Set(vtkStreamingDemandDrivenPipeline::TIME_RANGE(), outRange, 2);
    
    return 1;
  };

  /**
   * Extract segments' frame required by a vtkStreamingDemandDrivenPipeline object.
   */
  int VTKSegmentsFramesSource::RequestData(vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector)
  {
    btkNotUsed(request);
    
    vtkInformation* inInfo = inputVector[0]->GetInformationObject(0);
    VTKDataObjectAdapter* inObject = VTKDataObjectAdapter::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
    if (!inObject)
      return 0;
    
    PointCollection::Pointer input = static_pointer_cast<PointCollection>(inObject->GetBTKDataObject());
    
    vtkInformation* outInfo = outputVector->GetInformationObject(0);
    vtkPolyData* output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));
    
    vtkInformation* outInfoBis = outputVector->GetInformationObject(1);
    vtkPolyData* outputBis = vtkPolyData::SafeDownCast(outInfoBis->Get(vtkDataObject::DATA_OBJECT()));
    
    int frameIndex = 0;
    if (outInfo->Has(vtkStreamingDemandDrivenPipeline::UPDATE_TIME_STEPS()))
      frameIndex = static_cast<int>(outInfo->Get(vtkStreamingDemandDrivenPipeline::UPDATE_TIME_STEPS())[0]);
      
    int markerNumber = 0;
    int linkNumber = 0;
    int faceNumber = 0;
    int inc = 0;
    for (std::list<SegmentDefinition>::const_iterator itS = this->m_Definitions.begin() ; itS != this->m_Definitions.end() ; ++itS)
    {
      if (this->mp_VisibleSegments->GetValue(inc++) == 1)
      {
        markerNumber += static_cast<int>(itS->mesh->GetVertexNumber());
        linkNumber += static_cast<int>(itS->mesh->GetEdgeNumber());
        if (itS->surfaceEnabled)
          faceNumber += static_cast<int>(itS->mesh->GetFaceNumber());
      }
    }
    
    vtkIntArray* edgeColors = vtkIntArray::New(); edgeColors->SetName("Colors"); edgeColors->SetNumberOfValues(linkNumber);
    vtkIntArray* faceColors = vtkIntArray::New(); faceColors->SetName("Colors"); faceColors->SetNumberOfValues(faceNumber);
    vtkPoints* points = vtkPoints::New(); points->SetNumberOfPoints(markerNumber);
    vtkCellArray* lines = vtkCellArray::New(); lines->Allocate(lines->EstimateSize(linkNumber,2));
    vtkCellArray* polys = vtkCellArray::New(); polys->Allocate(lines->EstimateSize(faceNumber,3));
    int segmentIndex = 0;
    int lineIndex = 0; 
    int faceIndex = 0; 
    int pointId = 0;
    output->Initialize();
    outputBis->Initialize();
    
    for (std::list<SegmentDefinition>::const_iterator itS = this->m_Definitions.begin() ; itS != this->m_Definitions.end() ; ++itS)
    {
      if (this->mp_VisibleSegments->GetValue(segmentIndex) == 0)
      {
        ++segmentIndex;
        continue;
      }
      else if (!itS->mesh->ConnectPoints(input))
      {
        btkErrorMacro("Impossible to link the segment with the markers' data.");
        ++segmentIndex;
        continue;
      }
      itS->mesh->SetCurrentFrameIndex(frameIndex);
      int markerIndex = 0;
      // Set points
      std::vector<int> idPts = std::vector<int>(itS->mesh->GetVertexNumber(), 0);
      for (TriangleMesh::VertexConstIterator itM = itS->mesh->BeginVertex() ; itM != itS->mesh->EndVertex() ; ++itM)
      {
        if (itM->IsValid())
        {
          idPts[markerIndex] = pointId;
          points->SetPoint(pointId,
                           itM->GetCoordinateX() * this->m_Scale,
                           itM->GetCoordinateY() * this->m_Scale,
                           itM->GetCoordinateZ() * this->m_Scale);
          ++pointId;
        }
        ++markerIndex;
      }
      // Set lines
      for (TriangleMesh::EdgeConstIterator itL = itS->mesh->BeginEdge() ; itL != itS->mesh->EndEdge() ; ++itL)
      {
        if (itL->IsValid())
        {
          const vtkIdType pts[2]	= {idPts[itL->GetVertex1()->GetRelativeId()], idPts[itL->GetVertex2()->GetRelativeId()]};
          lines->InsertNextCell(2, pts);
          edgeColors->SetValue(lineIndex++, this->mp_SegmentsColorIndex->GetValue(segmentIndex));
        }
      }
      // Set polys
      if (itS->surfaceEnabled)
      {
        for (TriangleMesh::FaceConstIterator itF = itS->mesh->BeginFace() ; itF != itS->mesh->EndFace() ; ++itF)
        {
          if (itF->IsValid())
          {
            const vtkIdType pts[3]	= {idPts[itF->GetVertex1()->GetRelativeId()], idPts[itF->GetVertex2()->GetRelativeId()], idPts[itF->GetVertex3()->GetRelativeId()]};
            polys->InsertNextCell(3, pts);
            faceColors->SetValue(faceIndex++, this->mp_SegmentsColorIndex->GetValue(segmentIndex));
          }
        }
      }
      ++segmentIndex;
    }
    // Edges output
    output->SetPoints(points);
    output->SetLines(lines);
    output->GetCellData()->SetScalars(edgeColors);
    // Faces output
    outputBis->SetPoints(points);
    outputBis->SetPolys(polys);
    outputBis->GetCellData()->SetScalars(faceColors);
    // Cleanup
    points->Delete();
    lines->Delete();
    polys->Delete();
    edgeColors->Delete();
    faceColors->Delete();
    
    return 1;
  };

  /*
  int VTKSegmentsFramesSource::RequestUpdateExtent(vtkInformation *request, vtkInformationVector **inputVector, vtkInformationVector *outputVector)
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
  int VTKSegmentsFramesSource::FillInputPortInformation(int port, vtkInformation* info)
  {
    btkNotUsed(port);
    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "VTKDataObjectAdapter");
    return 1;
  }
  
  // ----------------------------- SegmentDefinition --------------------------
  
  VTKSegmentsFramesSource::SegmentDefinition::SegmentDefinition(const TriangleMesh::Pointer m, bool visible)
  : mesh(m)
  {
    this->surfaceEnabled = visible;
  }; 
  
  VTKSegmentsFramesSource::SegmentDefinition::SegmentDefinition(const std::vector<int>& m, const std::vector<Link>& l, const std::vector<Face>& f, bool visible)
  : mesh(TriangleMesh::New(m,l,f))
  {
    this->surfaceEnabled = visible;
  };
  
  VTKSegmentsFramesSource::SegmentDefinition::SegmentDefinition(const std::vector<int>& m, const std::vector<Link>& l, bool visible)
  : mesh(TriangleMesh::New(m,l))
  {
    this->surfaceEnabled = visible;
  };
};


/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2010, Arnaud Barré
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

#ifndef __btkVTKMarkersFramesSource_h
#define __btkVTKMarkersFramesSource_h

#include "btkPointCollection.h"
#include "btkVTKMarkersSource.h"

#include <vtkPolyDataAlgorithm.h>
#include <vtkDoubleArray.h>
#include <vtkIntArray.h>
#include <vtkIdTypeArray.h>
#include <vtkLookupTable.h>

namespace btk
{
  class VTKMarkersCoordinates;
  class VTKMarkersIds;
  class VTKTrajectoryIds;
  class VTKMarkersFramesSource : public vtkPolyDataAlgorithm
  {
  public:
    BTK_VTK_EXPORT static VTKMarkersFramesSource* New();
    vtkExportedTypeRevisionMacro(VTKMarkersFramesSource, vtkPolyDataAlgorithm, BTK_VTK_EXPORT);
    BTK_VTK_EXPORT void PrintSelf(ostream& os, vtkIndent indent);

    BTK_VTK_EXPORT void SetInput(PointCollection::Pointer input);

    BTK_VTK_EXPORT void ClearSelectedMarkers();
    BTK_VTK_EXPORT void SetSelectedMarker(vtkIdType id);

    BTK_VTK_EXPORT bool GetMarkerVisibility(vtkIdType id);
    BTK_VTK_EXPORT void SetMarkerVisibility(vtkIdType id, bool visible);
    BTK_VTK_EXPORT double GetMarkerRadius(vtkIdType id);
    BTK_VTK_EXPORT void SetMarkerRadius(vtkIdType id, double radius);
    BTK_VTK_EXPORT double* GetMarkerColor(vtkIdType id);
    BTK_VTK_EXPORT void SetMarkersColor(vtkIdTypeArray* ids, double r, double g, double b);
    BTK_VTK_EXPORT vtkIdType GetMarkerColorIndex(vtkIdType id);
    BTK_VTK_EXPORT void SetMarkerColorIndex(vtkIdType id, vtkIdType);
    BTK_VTK_EXPORT vtkLookupTable* GetMarkerColorLUT();
    
    BTK_VTK_EXPORT void HideMarker(vtkIdType id);
    BTK_VTK_EXPORT void HideMarkers();
    BTK_VTK_EXPORT void ShowMarker(vtkIdType id);
    BTK_VTK_EXPORT void ShowMarkers();

    BTK_VTK_EXPORT void SetTrajectoryVisibility(int idx, bool visible);
    BTK_VTK_EXPORT void ShowTrajectory(int idx);
    BTK_VTK_EXPORT void HideTrajectory(int idx);
    
    double GetScaleUnit() {return this->mp_Scale;};
    void SetScaleUnit(double s) {this->mp_Scale = s;};
 
  protected:
    BTK_VTK_EXPORT VTKMarkersFramesSource();
    BTK_VTK_EXPORT ~VTKMarkersFramesSource();
    
    BTK_VTK_EXPORT virtual int RequestInformation(vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector);
    BTK_VTK_EXPORT virtual int RequestData(vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector);
    //virtual int RequestUpdateExtent(vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector);

    BTK_VTK_EXPORT virtual int FillInputPortInformation(int port, vtkInformation* info);
    
  private:
    VTKMarkersFramesSource(const VTKMarkersFramesSource& );  // Not implemented.
    VTKMarkersFramesSource& operator=(const VTKMarkersFramesSource& );  // Not implemented.

    VTKMarkersCoordinates* mp_MarkersCoordinates;
    VTKMarkersIds* mp_ExistingMarkers;
    vtkIntArray* mp_VisibleMarkers;
    vtkIntArray* mp_SelectedMarkers;
    vtkIntArray* mp_TrajectoryMarkers;
    vtkPoints* mp_TrajectoryCoords;
    vtkIntArray* mp_TrajectoryColors;
    VTKTrajectoryIds* mp_TrajectoryIds;
    vtkDoubleArray* mp_MarkersRadius;
    vtkIdTypeArray* mp_MarkersColorIndex;
    vtkLookupTable* mp_MarkersColorsLUT;
    VTKMarkersSource* mp_MarkersGenerator;
    double mp_Scale;
  };
};
#endif // __btkVTKMarkersFramesSource_h

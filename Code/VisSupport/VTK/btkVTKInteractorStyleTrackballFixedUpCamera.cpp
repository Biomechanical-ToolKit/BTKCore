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
 * LOSS OF USE, DATA, OR PROFITS;OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "btkVTKInteractorStyleTrackballFixedUpCamera.h"

#include <Eigen/Eigen>

#include <vtkCamera.h>
#include <vtkObjectFactory.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkTransform.h>
#include <vtkMath.h>
#include <vtkCallbackCommand.h>
#include <vtkUnsignedCharArray.h>

namespace btk
{
  /**
   * @class VTKInteractorStyleTrackballFixedUpCamera btkVTKInteractorStyleTrackballFixedUpCamera.h
   * @brief Another implementation of a virtual trackball
   *
   * @warning This class is not yet finished. Some problems with the view-up vector.
   */
   
  /**
   * @fn static VTKInteractorStyleTrackballFixedUpCamera* VTKInteractorStyleTrackballFixedUpCamera::New();
   * Creates a VTKInteractorStyleTrackballFixedUpCamera object and return it as a pointer.
   */
  vtkStandardNewMacro(VTKInteractorStyleTrackballFixedUpCamera);
  vtkCxxRevisionMacro(VTKInteractorStyleTrackballFixedUpCamera, "$Revision: 0.1 $");
  
  /**
   * Prints Superclass informations.
   */
  void VTKInteractorStyleTrackballFixedUpCamera::PrintSelf(ostream& os, vtkIndent indent)
  {
    this->Superclass::PrintSelf(os,indent);
  };
  
  /**
   * Overloaded method to determine the position of the camera around the centre.
   */
  void VTKInteractorStyleTrackballFixedUpCamera::Rotate()
  {
#if 0
    if (this->RotationEnabled)
      this->Superclass::Rotate();
#else
    if (this->CurrentRenderer == NULL)
      return;

    vtkCamera* camera = this->CurrentRenderer->GetActiveCamera();
    vtkRenderWindowInteractor *rwi = this->Interactor;
    int* size = this->CurrentRenderer->GetRenderWindow()->GetSize();
    double rz = -180.0 * static_cast<double>(rwi->GetEventPosition()[0] - rwi->GetLastEventPosition()[0]) / static_cast<double>(size[0]);
    double rx =  90.0 * static_cast<double>(rwi->GetEventPosition()[1] - rwi->GetLastEventPosition()[1]) / static_cast<double>(size[1]);
    
    Eigen::Matrix<double,3,1> cameraUp(camera->GetViewUp());
    Eigen::Matrix<double,3,1> cameraProj(camera->GetDirectionOfProjection());
    Eigen::Matrix<double,3,1> globalUp = Eigen::Matrix<double,3,1>::UnitZ();
    
    //double angleRightTotal = std::acos(cameraProj.dot(globalUp)) * 180.0/M_PI;
    Eigen::Matrix<double,3,1> axisRight = cameraProj.cross(globalUp).normalized();
    double angleUp = std::acos(cameraUp.dot(globalUp)) * 180.0/M_PI;
    if (angleUp + rx >= 90.0)
      rx = 89.0 - angleUp;
    
#if 0
    std::cout << "Camera up: " << cameraUp.transpose() << std::endl;
    std::cout << "Axis right: " << axisRight.transpose() << std::endl;
    std::cout << "Angle Right inc: " << rx << std::endl;
    std::cout << "Angle Right total: " << angleRightTotal << std::endl;
    std::cout << "Angle Up: " << angleUp << std::endl;
#endif

    double* fp = camera->GetFocalPoint();

    vtkTransform* Transform = vtkTransform::New(); 
    Transform->Identity();
    Transform->Translate(+fp[0],+fp[1],+fp[2]);
    Transform->RotateWXYZ(rz,globalUp.data());
    Transform->RotateWXYZ(rx,axisRight.data());
    Transform->Translate(-fp[0],-fp[1],-fp[2]);
    camera->ApplyTransform(Transform);
    Transform->Delete();
    
    camera->OrthogonalizeViewUp();

    if (this->AutoAdjustCameraClippingRange)
      this->CurrentRenderer->ResetCameraClippingRange();

    if (rwi->GetLightFollowCamera()) 
      this->CurrentRenderer->UpdateLightsGeometryToFollowCamera();

    rwi->Render();
#endif
  };
  
  /**
   * Constructor.
   */
  VTKInteractorStyleTrackballFixedUpCamera::VTKInteractorStyleTrackballFixedUpCamera()
  : vtkInteractorStyleTrackballCamera()
  {
    // Rubber band geometry
    //  - Corner #1
    this->mp_RubberBandCorners[0][0] = 0;
    this->mp_RubberBandCorners[0][1] = 0;
    //  - Corner #2 (diagonal opposed to corner #1)
    this->mp_RubberBandCorners[1][0] = 0;
    this->mp_RubberBandCorners[1][1] = 0;
    // VTK macro members
    this->RotationEnabled = 1;
    this->SpinEnabled = 1;
    this->PanEnabled = 1;
    this->DollyEnabled = 1;
    this->CharEventEnabled = 1;
    this->ForceRubberBandDrawing = 1;
    // Rubber band
    this->mp_RubberBandHorizontalLinesForeground[0] = vtkUnsignedCharArray::New();
    this->mp_RubberBandHorizontalLinesForeground[1] = vtkUnsignedCharArray::New();
    this->mp_RubberBandHorizontalLinesForeground[2] = vtkUnsignedCharArray::New();
    this->mp_RubberBandHorizontalLinesForeground[3] = vtkUnsignedCharArray::New();
    this->mp_RubberBandHorizontalLinesBackground[0] = vtkUnsignedCharArray::New();
    this->mp_RubberBandHorizontalLinesBackground[1] = vtkUnsignedCharArray::New();
    this->mp_RubberBandHorizontalLinesBackground[2] = vtkUnsignedCharArray::New();
    this->mp_RubberBandHorizontalLinesBackground[3] = vtkUnsignedCharArray::New();
  };
  
  /**
   * @fn VTKInteractorStyleTrackballFixedUpCamera::~VTKInteractorStyleTrackballFixedUpCamera()
   * Empty destructor.
   */
  VTKInteractorStyleTrackballFixedUpCamera::~VTKInteractorStyleTrackballFixedUpCamera()
  {
    this->mp_RubberBandHorizontalLinesForeground[0]->Delete();
    this->mp_RubberBandHorizontalLinesForeground[1]->Delete();
    this->mp_RubberBandHorizontalLinesForeground[2]->Delete();
    this->mp_RubberBandHorizontalLinesForeground[3]->Delete();
    this->mp_RubberBandHorizontalLinesBackground[0]->Delete();
    this->mp_RubberBandHorizontalLinesBackground[1]->Delete();
    this->mp_RubberBandHorizontalLinesBackground[2]->Delete();
    this->mp_RubberBandHorizontalLinesBackground[3]->Delete();
  };
   
  /**
   * Overload method to enable/disable VTK key event by using Get/Set CharEventEnabled.
   */ 
  void VTKInteractorStyleTrackballFixedUpCamera::OnChar()
  {
    if (this->CharEventEnabled)
      this->Superclass::OnChar();
  };
  
  /**
   * Return the screen coordinates of the 2 points defining the rubber band.
   */
  void VTKInteractorStyleTrackballFixedUpCamera::GetRubberBandPoints(int pts[4]) const
  {
    pts[0] = this->mp_RubberBandCorners[0][0];
    pts[1] = this->mp_RubberBandCorners[0][1];
    pts[2] = this->mp_RubberBandCorners[1][0];
    pts[3] = this->mp_RubberBandCorners[1][1];
  };
  
  /**
   * Select the task to do based on the key modifiers pressed.
   */ 
  void VTKInteractorStyleTrackballFixedUpCamera::OnLeftButtonDown()
  {
    if (this->Interactor == NULL)
      return;
    
    this->FindPokedRenderer(this->Interactor->GetEventPosition()[0], this->Interactor->GetEventPosition()[1]);
    if (this->CurrentRenderer == NULL)
      return;
    
    this->GrabFocus(this->EventCallbackCommand);
    
    if (this->Interactor->GetShiftKey()) 
      this->StartRubberBand();
    else if (this->Interactor->GetAltKey()) 
    {
      if (this->Interactor->GetControlKey() && this->SpinEnabled) 
        this->StartSpin();
      else if (this->PanEnabled)
        this->StartPan();
    } 
    else 
    {
      if (this->Interactor->GetControlKey() && this->DollyEnabled) 
        this->StartDolly();
      else if (this->RotationEnabled)
        this->StartRotate();
    }
    return;
  };
  
  /**
   * Select the task to do based on the key modifiers pressed.
   */
  void VTKInteractorStyleTrackballFixedUpCamera::OnLeftButtonUp()
  {
    switch (this->State) 
    {
    case VTKIS_DOLLY:
      this->EndDolly();
      break;
    case VTKIS_PAN:
      this->EndPan();
      break;
    case VTKIS_SPIN:
      this->EndSpin();
      break;
    case VTKIS_ROTATE:
      this->EndRotate();
      break;
    case btk_VTKISTC_RUBBER:
      this->EndRubberBand();
      break;
    }
    if (this->Interactor)
      this->ReleaseFocus();
  };
  
  /**
   * Select the task to do based on the key modifiers pressed.
   */ 
  void VTKInteractorStyleTrackballFixedUpCamera::OnRightButtonDown()
  {
    if (this->DollyEnabled)
      this->Superclass::OnRightButtonDown();
  };
  
  /**
   * Select the task to do based on the key modifiers pressed.
   */
  void VTKInteractorStyleTrackballFixedUpCamera::OnRightButtonUp()
  {
    if (this->DollyEnabled)
      this->Superclass::OnRightButtonUp();
  };
  
  /**
   * Select the task to do based on the key modifiers pressed.
   */
  void VTKInteractorStyleTrackballFixedUpCamera::OnMouseMove()
  {
    int x = this->Interactor->GetEventPosition()[0];
    int y = this->Interactor->GetEventPosition()[1];

    switch (this->State) 
    {
    case VTKIS_ROTATE:
      this->FindPokedRenderer(x, y);
      this->Rotate();
      this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
      break;
    case VTKIS_PAN:
      this->FindPokedRenderer(x, y);
      this->Pan();
      this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
      break;
    case VTKIS_DOLLY:
      this->FindPokedRenderer(x, y);
      this->Dolly();
      this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
      break;
    case VTKIS_SPIN:
      this->FindPokedRenderer(x, y);
      this->Spin();
      this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
      break;
    case btk_VTKISTC_RUBBER:
      this->FindPokedRenderer(x, y);
      this->Rubber();
      this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
      break;
    }
  };
  
  /**
   * Update the rubber band and draw it
   */
  void VTKInteractorStyleTrackballFixedUpCamera::Rubber()
  {
    if (this->CurrentRenderer == NULL)
      return;
      
    vtkRenderWindow* renWin = this->Interactor->GetRenderWindow();
    int* size = renWin->GetSize();
    
    renWin->SetPixelData(0, this->mp_RubberBandCorners[0][1], size[0]-1, this->mp_RubberBandCorners[0][1], this->mp_RubberBandHorizontalLinesBackground[0]->GetPointer(0), renWin->GetDoubleBuffer() ? 0 : 1);
    renWin->SetPixelData(0, this->mp_RubberBandCorners[1][1], size[0]-1, this->mp_RubberBandCorners[1][1], this->mp_RubberBandHorizontalLinesBackground[1]->GetPointer(0), renWin->GetDoubleBuffer() ? 0 : 1);
    renWin->SetPixelData(this->mp_RubberBandCorners[0][0], 0, this->mp_RubberBandCorners[0][0], size[1]-1, this->mp_RubberBandHorizontalLinesBackground[2]->GetPointer(0), renWin->GetDoubleBuffer() ? 0 : 1);
    renWin->SetPixelData(this->mp_RubberBandCorners[1][0], 0, this->mp_RubberBandCorners[1][0], size[1]-1, this->mp_RubberBandHorizontalLinesBackground[3]->GetPointer(0), renWin->GetDoubleBuffer() ? 0 : 1);
    
    this->mp_RubberBandCorners[1][0] = this->Interactor->GetEventPosition()[0];
    this->mp_RubberBandCorners[1][1] = this->Interactor->GetEventPosition()[1];  
    if (this->mp_RubberBandCorners[1][0] > (size[0]-1))
      this->mp_RubberBandCorners[1][0] = size[0]-1;
    if (this->mp_RubberBandCorners[1][0] < 0)
      this->mp_RubberBandCorners[1][0] = 0;
    if (this->mp_RubberBandCorners[1][1] > (size[1]-1))
      this->mp_RubberBandCorners[1][1] = size[1]-1;
    if (this->mp_RubberBandCorners[1][1] < 0)
      this->mp_RubberBandCorners[1][1] = 0;
    
    int leftBottomCorner[2] = {this->mp_RubberBandCorners[0][0], this->mp_RubberBandCorners[0][1]};
    int rightTopCorner[2] = {this->mp_RubberBandCorners[1][0], this->mp_RubberBandCorners[1][1]};
    
    if (this->mp_RubberBandCorners[0][0] > this->mp_RubberBandCorners[1][0])
    {
      leftBottomCorner[0] = this->mp_RubberBandCorners[1][0];
      rightTopCorner[0] = this->mp_RubberBandCorners[0][0];
    }
    if (this->mp_RubberBandCorners[0][1] > this->mp_RubberBandCorners[1][1])
    {
      leftBottomCorner[1] = this->mp_RubberBandCorners[1][1];
      rightTopCorner[1] = this->mp_RubberBandCorners[0][1];
    }
    
    this->UpdateRubberBackground();
    
    // Draw the rubber
    unsigned char* pixels[2];
    const int dashLineLength = 3;
    const int dashSpaceLength = 3;
    int dashTotalLength = dashLineLength + dashSpaceLength;
    this->mp_RubberBandHorizontalLinesForeground[0]->DeepCopy(this->mp_RubberBandHorizontalLinesBackground[0]);
    this->mp_RubberBandHorizontalLinesForeground[1]->DeepCopy(this->mp_RubberBandHorizontalLinesBackground[1]);
    this->mp_RubberBandHorizontalLinesForeground[2]->DeepCopy(this->mp_RubberBandHorizontalLinesBackground[2]);
    this->mp_RubberBandHorizontalLinesForeground[3]->DeepCopy(this->mp_RubberBandHorizontalLinesBackground[3]);
    // - Horizontal line
    pixels[0] = this->mp_RubberBandHorizontalLinesForeground[0]->GetPointer(0);
    pixels[1] = this->mp_RubberBandHorizontalLinesForeground[1]->GetPointer(0);
    for (int i = leftBottomCorner[0]; i <= rightTopCorner[0]; i += dashTotalLength)
    {
      for (int j = i ; j < (i+dashSpaceLength <= rightTopCorner[0] ? i+dashSpaceLength : rightTopCorner[0]) ; ++j)
      {
        pixels[0][3*j] = 255;
        pixels[0][3*j+1] = 255;
        pixels[0][3*j+2] = 255;
        pixels[1][3*j] = 255;
        pixels[1][3*j+1] = 255;
        pixels[1][3*j+2] = 255;
      }
    }
    renWin->SetPixelData(0, this->mp_RubberBandCorners[0][1], size[0]-1, this->mp_RubberBandCorners[0][1], pixels[0], renWin->GetDoubleBuffer() ? 0 : 1);
    renWin->SetPixelData(0, this->mp_RubberBandCorners[1][1], size[0]-1, this->mp_RubberBandCorners[1][1], pixels[1], renWin->GetDoubleBuffer() ? 0 : 1);
    // - Vertical line
    pixels[0] = this->mp_RubberBandHorizontalLinesForeground[2]->GetPointer(0);
    pixels[1] = this->mp_RubberBandHorizontalLinesForeground[3]->GetPointer(0);
    for (int i = leftBottomCorner[1]+1; i < rightTopCorner[1]; i+=dashTotalLength)
    {
      for (int j = i ; j < (i+dashSpaceLength <= rightTopCorner[1] ? i+dashSpaceLength : rightTopCorner[1]); ++j)
      {
        pixels[0][3*j] = 255;
        pixels[0][3*j+1] = 255;
        pixels[0][3*j+2] = 255;
        pixels[1][3*j] = 255;
        pixels[1][3*j+1] = 255;
        pixels[1][3*j+2] = 255;
      }
    }
    renWin->SetPixelData(this->mp_RubberBandCorners[0][0], 0, this->mp_RubberBandCorners[0][0], size[1]-1, pixels[0], renWin->GetDoubleBuffer() ? 0 : 1);
    renWin->SetPixelData(this->mp_RubberBandCorners[1][0], 0, this->mp_RubberBandCorners[1][0], size[1]-1, pixels[1], renWin->GetDoubleBuffer() ? 0 : 1);
    
    if (this->ForceRubberBandDrawing == 1)
      renWin->Frame();
  };
  
  void VTKInteractorStyleTrackballFixedUpCamera::RedrawRubberBand()
  {
    vtkRenderWindow* renWin = this->Interactor->GetRenderWindow();
    int* size = renWin->GetSize();
    renWin->SetPixelData(0, this->mp_RubberBandCorners[0][1], size[0]-1, this->mp_RubberBandCorners[0][1], this->mp_RubberBandHorizontalLinesForeground[0]->GetPointer(0), renWin->GetDoubleBuffer() ? 0 : 1);
    renWin->SetPixelData(0, this->mp_RubberBandCorners[1][1], size[0]-1, this->mp_RubberBandCorners[1][1], this->mp_RubberBandHorizontalLinesForeground[1]->GetPointer(0), renWin->GetDoubleBuffer() ? 0 : 1);
    renWin->SetPixelData(this->mp_RubberBandCorners[0][0], 0, this->mp_RubberBandCorners[0][0], size[1]-1, this->mp_RubberBandHorizontalLinesForeground[2]->GetPointer(0), renWin->GetDoubleBuffer() ? 0 : 1);
    renWin->SetPixelData(this->mp_RubberBandCorners[1][0], 0, this->mp_RubberBandCorners[1][0], size[1]-1, this->mp_RubberBandHorizontalLinesForeground[3]->GetPointer(0), renWin->GetDoubleBuffer() ? 0 : 1);
  };
  
  void VTKInteractorStyleTrackballFixedUpCamera::UpdateRubberBackground()
  {
    vtkRenderWindow* renWin = this->Interactor->GetRenderWindow();
    int* size = renWin->GetSize();
    
    this->mp_RubberBandHorizontalLinesBackground[0]->SetNumberOfTuples(size[0]);
    this->mp_RubberBandHorizontalLinesBackground[1]->SetNumberOfTuples(size[0]);
    this->mp_RubberBandHorizontalLinesBackground[2]->SetNumberOfTuples(size[1]);
    this->mp_RubberBandHorizontalLinesBackground[3]->SetNumberOfTuples(size[1]);
    
    renWin->GetPixelData(0, this->mp_RubberBandCorners[0][1], size[0]-1, this->mp_RubberBandCorners[0][1], renWin->GetDoubleBuffer() ? 0 : 1, this->mp_RubberBandHorizontalLinesBackground[0]);
    renWin->GetPixelData(0, this->mp_RubberBandCorners[1][1], size[0]-1, this->mp_RubberBandCorners[1][1], renWin->GetDoubleBuffer() ? 0 : 1, this->mp_RubberBandHorizontalLinesBackground[1]);
    renWin->GetPixelData(this->mp_RubberBandCorners[0][0], 0, this->mp_RubberBandCorners[0][0], size[1]-1, renWin->GetDoubleBuffer() ? 0 : 1, this->mp_RubberBandHorizontalLinesBackground[2]);
    renWin->GetPixelData(this->mp_RubberBandCorners[1][0], 0, this->mp_RubberBandCorners[1][0], size[1]-1, renWin->GetDoubleBuffer() ? 0 : 1, this->mp_RubberBandHorizontalLinesBackground[3]);
  };
  
  /**
   * Activate the rubber band.
   */
  void VTKInteractorStyleTrackballFixedUpCamera::StartRubberBand()
  {
    if (this->State != VTKIS_NONE) 
      return;
    this->StartState(btk_VTKISTC_RUBBER);
    
    this->mp_RubberBandCorners[0][0] = this->Interactor->GetEventPosition()[0];
    this->mp_RubberBandCorners[0][1] = this->Interactor->GetEventPosition()[1];
    this->mp_RubberBandCorners[1][0] = this->mp_RubberBandCorners[0][0];
    this->mp_RubberBandCorners[1][1] = this->mp_RubberBandCorners[0][1];
    
    this->UpdateRubberBackground();
    
    this->mp_RubberBandHorizontalLinesForeground[0]->DeepCopy(this->mp_RubberBandHorizontalLinesBackground[0]);
    this->mp_RubberBandHorizontalLinesForeground[1]->DeepCopy(this->mp_RubberBandHorizontalLinesBackground[1]);
    this->mp_RubberBandHorizontalLinesForeground[2]->DeepCopy(this->mp_RubberBandHorizontalLinesBackground[2]);
    this->mp_RubberBandHorizontalLinesForeground[3]->DeepCopy(this->mp_RubberBandHorizontalLinesBackground[3]);
  };
  
  /**
   * Stop the rubber band and erase it from the screen.
   */
  void VTKInteractorStyleTrackballFixedUpCamera::EndRubberBand()
  {
    if (this->State != btk_VTKISTC_RUBBER) 
      return;
    this->StopState();
    
    // Erase the rubber band
    /*
    vtkRenderWindow* renWin = this->Interactor->GetRenderWindow();
    int* size = renWin->GetSize();
    renWin->SetPixelData(0, 0, size[0]-1, size[1]-1, this->mp_PixelArray->GetPointer(0), renWin->GetDoubleBuffer() ? 0 : 1);
    renWin->Frame();
    */
    this->Interactor->Render();
  };
};
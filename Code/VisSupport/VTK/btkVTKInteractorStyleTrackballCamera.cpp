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

#include "btkVTKInteractorStyleTrackballCamera.h"

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
   * @class VTKInteractorStyleTrackballCamera btkVTKInteractorStyleTrackballCamera.h
   * @brief Another implementation of a virtual trackball
   *
   * @warning This class is not yet finished. Some problems with the view-up vector.
   */
   
  /**
   * @fn static VTKInteractorStyleTrackballCamera* VTKInteractorStyleTrackballCamera::New();
   * Creates a VTKInteractorStyleTrackballCamera object and return it as a pointer.
   */
  vtkStandardNewMacro(VTKInteractorStyleTrackballCamera);
  vtkCxxRevisionMacro(VTKInteractorStyleTrackballCamera, "$Revision: 0.1 $");
  
  /**
   * Prints Superclass informations.
   */
  void VTKInteractorStyleTrackballCamera::PrintSelf(ostream& os, vtkIndent indent)
  {
    this->Superclass::PrintSelf(os,indent);
  };
  
  /**
   * Overloaded method to determine the position of the camera around the centre.
   */
  void VTKInteractorStyleTrackballCamera::Rotate()
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
  VTKInteractorStyleTrackballCamera::VTKInteractorStyleTrackballCamera()
  : vtkInteractorStyleTrackballCamera()
  {
    // Rubber band geometry
    //  - Corner #1
    this->mp_RubberBandGeometry[0][0] = 0;
    this->mp_RubberBandGeometry[0][1] = 0;
    //  - Corner #2 (diagonal opposed to corner #1)
    this->mp_RubberBandGeometry[1][0] = 0;
    this->mp_RubberBandGeometry[1][1] = 0;
    // VTK macro members
    this->RotationEnabled = 1;
    this->SpinEnabled = 1;
    this->PanEnabled = 1;
    this->DollyEnabled = 1;
    this->CharEventEnabled = 1;
    // Rubber band
    this->mp_PixelArray = vtkUnsignedCharArray::New();
    this->mp_PixelArray->Initialize();
    this->mp_PixelArray->SetNumberOfComponents(3);
  };
  
  /**
   * @fn VTKInteractorStyleTrackballCamera::~VTKInteractorStyleTrackballCamera()
   * Empty destructor.
   */
  VTKInteractorStyleTrackballCamera::~VTKInteractorStyleTrackballCamera()
  {
    this->mp_PixelArray->Delete();
  };
   
  /**
   * Overload method to enable/disable VTK key event by using Get/Set CharEventEnabled.
   */ 
  void VTKInteractorStyleTrackballCamera::OnChar()
  {
    if (this->CharEventEnabled)
      this->Superclass::OnChar();
  };
  
  /**
   * Return the screen coordinates of the 2 points defining the rubber band.
   */
  void VTKInteractorStyleTrackballCamera::GetRubberBandPoints(int pts[4]) const
  {
    pts[0] = this->mp_RubberBandGeometry[0][0];
    pts[1] = this->mp_RubberBandGeometry[0][1];
    pts[2] = this->mp_RubberBandGeometry[1][0];
    pts[3] = this->mp_RubberBandGeometry[1][1];
  };
  
  /**
   * Select the task to do based on the key modifiers pressed.
   */ 
  void VTKInteractorStyleTrackballCamera::OnLeftButtonDown()
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
      if (this->Interactor->GetControlKey() && this->DollyEnabled) 
        this->StartDolly();
      else if (this->PanEnabled)
        this->StartPan();
    } 
    else 
    {
      if (this->Interactor->GetControlKey() && this->SpinEnabled) 
        this->StartSpin();
      else if (this->RotationEnabled)
        this->StartRotate();
    }
    return;
  };
  
  /**
   * Select the task to do based on the key modifiers pressed.
   */
  void VTKInteractorStyleTrackballCamera::OnLeftButtonUp()
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
  void VTKInteractorStyleTrackballCamera::OnRightButtonDown()
  {
    if (this->DollyEnabled)
      this->Superclass::OnRightButtonDown();
  };
  
  /**
   * Select the task to do based on the key modifiers pressed.
   */
  void VTKInteractorStyleTrackballCamera::OnRightButtonUp()
  {
    if (this->DollyEnabled)
      this->Superclass::OnRightButtonUp();
  };
  
  /**
   * Select the task to do based on the key modifiers pressed.
   */
  void VTKInteractorStyleTrackballCamera::OnMouseMove()
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
  void VTKInteractorStyleTrackballCamera::Rubber()
  {
    if (this->CurrentRenderer == NULL)
      return;
    
    this->mp_RubberBandGeometry[1][0] = this->Interactor->GetEventPosition()[0];
    this->mp_RubberBandGeometry[1][1] = this->Interactor->GetEventPosition()[1];  
    vtkRenderWindow* renWin = this->Interactor->GetRenderWindow();
    int* size = renWin->GetSize();  
    if (this->mp_RubberBandGeometry[1][0] > (size[0]-1))
      this->mp_RubberBandGeometry[1][0] = size[0]-1;
    if (this->mp_RubberBandGeometry[1][0] < 0)
      this->mp_RubberBandGeometry[1][0] = 0;
    if (this->mp_RubberBandGeometry[1][1] > (size[1]-1))
      this->mp_RubberBandGeometry[1][1] = size[1]-1;
    if (this->mp_RubberBandGeometry[1][1] < 0)
      this->mp_RubberBandGeometry[1][1] = 0;
    
    int leftBottomCorner[2] = {this->mp_RubberBandGeometry[0][0], this->mp_RubberBandGeometry[0][1]};
    int rightTopCorner[2] = {this->mp_RubberBandGeometry[1][0], this->mp_RubberBandGeometry[1][1]};
    
    if (this->mp_RubberBandGeometry[0][0] > this->mp_RubberBandGeometry[1][0])
    {
      leftBottomCorner[0] = this->mp_RubberBandGeometry[1][0];
      rightTopCorner[0] = this->mp_RubberBandGeometry[0][0];
    }
    if (this->mp_RubberBandGeometry[0][1] > this->mp_RubberBandGeometry[1][1])
    {
      leftBottomCorner[1] = this->mp_RubberBandGeometry[1][1];
      rightTopCorner[1] = this->mp_RubberBandGeometry[0][1];
    }
    
    // Draw the rubber
    vtkUnsignedCharArray* temp = vtkUnsignedCharArray::New();
    temp->DeepCopy(this->mp_PixelArray);  
    unsigned char* pixels = temp->GetPointer(0);
    
    const int dashLineLength = 3;
    const int dashSpaceLength = 3;
    int dashTotalLength = dashLineLength + dashSpaceLength;
    
    for (int i = leftBottomCorner[0]; i <= rightTopCorner[0]; i += dashTotalLength)
    {
      for (int j = i ; j < (i+dashSpaceLength <= rightTopCorner[0] ? i+dashSpaceLength : rightTopCorner[0]) ; ++j)
      {
        pixels[3*(leftBottomCorner[1]*size[0]+j)] = 255;
        pixels[3*(leftBottomCorner[1]*size[0]+j)+1] = 255;
        pixels[3*(leftBottomCorner[1]*size[0]+j)+2] = 255;
    
        pixels[3*(rightTopCorner[1]*size[0]+j)] = 255;
        pixels[3*(rightTopCorner[1]*size[0]+j)+1] = 255;
        pixels[3*(rightTopCorner[1]*size[0]+j)+2] = 255;
      }
    }
    for (int i = leftBottomCorner[1]+1; i < rightTopCorner[1]; i+=dashTotalLength)
    {
      for (int j = i ; j < (i+dashSpaceLength <= rightTopCorner[1] ? i+dashSpaceLength : rightTopCorner[1]); ++j)
      {
        pixels[3*(j*size[0]+leftBottomCorner[0])] = 255;
        pixels[3*(j*size[0]+leftBottomCorner[0])+1] = 255;
        pixels[3*(j*size[0]+leftBottomCorner[0])+2] = 255;
      
        pixels[3*(j*size[0]+rightTopCorner[0])] = 255;
        pixels[3*(j*size[0]+rightTopCorner[0])+1] = 255;
        pixels[3*(j*size[0]+rightTopCorner[0])+2] = 255;
      }
    }
    
    /*
    for (int i = leftBottomCorner[0]; i <= rightTopCorner[0]; ++i)
    {
      pixels[3*(leftBottomCorner[1]*size[0]+i)] = 255;
      pixels[3*(leftBottomCorner[1]*size[0]+i)+1] = 0;
      pixels[3*(leftBottomCorner[1]*size[0]+i)+2] = 0;
      
      // pixels[3*((leftBottomCorner[1]+1)*size[0]+i)] = 255;
      // pixels[3*((leftBottomCorner[1]+1)*size[0]+i)+1] = 0;
      // pixels[3*((leftBottomCorner[1]+1)*size[0]+i)+2] = 0;
      
      pixels[3*(rightTopCorner[1]*size[0]+i)] = 255;
      pixels[3*(rightTopCorner[1]*size[0]+i)+1] = 0;
      pixels[3*(rightTopCorner[1]*size[0]+i)+2] = 0;
      
      // pixels[3*((rightTopCorner[1]-1)*size[0]+i)] = 255;
      // pixels[3*((rightTopCorner[1]-1)*size[0]+i)+1] = 0;
      // pixels[3*((rightTopCorner[1]-1)*size[0]+i)+2] = 0;
    }
    for (int i = leftBottomCorner[1]+1; i < rightTopCorner[1]; ++i)
    {
      pixels[3*(i*size[0]+leftBottomCorner[0])] = 255;
      pixels[3*(i*size[0]+leftBottomCorner[0])+1] = 0;
      pixels[3*(i*size[0]+leftBottomCorner[0])+2] = 0;
      
      // pixels[3*(i*size[0]+(leftBottomCorner[0]+1))] = 255;
      // pixels[3*(i*size[0]+(leftBottomCorner[0]+1))+1] = 0;
      // pixels[3*(i*size[0]+(leftBottomCorner[0]+1))+2] = 0;
      
      pixels[3*(i*size[0]+rightTopCorner[0])] = 255;
      pixels[3*(i*size[0]+rightTopCorner[0])+1] = 0;
      pixels[3*(i*size[0]+rightTopCorner[0])+2] = 0;
      
      // pixels[3*(i*size[0]+(rightTopCorner[0]-1))] = 255;
      // pixels[3*(i*size[0]+(rightTopCorner[0]-1))+1] = 0;
      // pixels[3*(i*size[0]+(rightTopCorner[0]-1))+2] = 0;
    }
    */
    
    renWin->SetPixelData(0, 0, size[0]-1, size[1]-1, pixels, renWin->GetDoubleBuffer() ? 0 : 1);
    renWin->Frame();
    
    temp->Delete();
  };
  
  void VTKInteractorStyleTrackballCamera::UpdateRubberBackground()
  {
    vtkRenderWindow* renWin = this->Interactor->GetRenderWindow();
    int* size = renWin->GetSize();
    this->mp_PixelArray->SetNumberOfTuples(size[0]*size[1]);
    renWin->GetPixelData(0, 0, size[0]-1, size[1]-1, renWin->GetDoubleBuffer() ? 0 : 1, this->mp_PixelArray);
  };
  
  /**
   * Activate the rubber band.
   */
  void VTKInteractorStyleTrackballCamera::StartRubberBand()
  {
    if (this->State != VTKIS_NONE) 
      return;
    this->StartState(btk_VTKISTC_RUBBER);
    
    this->mp_RubberBandGeometry[0][0] = this->Interactor->GetEventPosition()[0];
    this->mp_RubberBandGeometry[0][1] = this->Interactor->GetEventPosition()[1];
    this->mp_RubberBandGeometry[1][0] = this->mp_RubberBandGeometry[0][0];
    this->mp_RubberBandGeometry[1][1] = this->mp_RubberBandGeometry[0][1];
    
    this->UpdateRubberBackground();
  };
  
  /**
   * Stop the rubber band and erase it from the screen.
   */
  void VTKInteractorStyleTrackballCamera::EndRubberBand()
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
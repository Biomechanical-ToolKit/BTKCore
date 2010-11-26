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

#include <vtkCamera.h>
#include <vtkObjectFactory.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkTransform.h>
#include <vtkMath.h>
#include <vtkCallbackCommand.h>
#include <vtkUnsignedCharArray.h>

#define TRACKBALLSIZE 0.8

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
    if (this->RotationEnabled)
      this->Superclass::Rotate();

    /*
    if (this->CurrentRenderer == NULL)
      return;
    
    vtkRenderWindowInteractor *rwi = this->Interactor;
    int* size = this->CurrentRenderer->GetRenderWindow()->GetSize();
    double width = size[0];
    double height = size[1];
    
    // Viewport coordinates must be between -1.0 and 1.0
    // old position
    double p1[3];
    int* lastMousePos = this->Interactor->GetLastEventPosition();
    p1[0] = (2.0 * static_cast<double>(lastMousePos[0]) - width) / width;
    p1[1] = (height - 2.0 * static_cast<double>(lastMousePos[1])) / height;
    p1[2] = this->ProjectToSphere(TRACKBALLSIZE, p1[0], p1[1]);
    //std::cout << "p1: "<< p1[0] << " ; " << p1[1] << " ; " << p1[2] << std::endl;
    // new position
    double p2[3];
    int* currentMousePos = this->Interactor->GetEventPosition();
    p2[0] = (2.0 * static_cast<double>(currentMousePos[0]) - width) / width;
    p2[1] = (height - 2.0 * static_cast<double>(currentMousePos[1])) / height,
    p2[2] = this->ProjectToSphere(TRACKBALLSIZE, p2[0], p2[1]);
    //std::cout << "p2: "<< p2[0] << " ; " << p2[1] << " ; " << p2[2] <<std::endl;
    
    double axis[3];
    vtkMath::Cross(p2, p1, axis);
    double norm = sqrt(axis[0] * axis[0] + axis[1] * axis[1] + axis[2] * axis[2]);
    axis[0] /= norm;
    axis[1] /= norm;
    axis[2] /= -norm;
    
    double diff[3];
    diff[0] = p1[0] - p2[0];
    diff[1] = p1[1] - p2[1];
    diff[2] = p1[2] - p2[2];
    
    double t = sqrt(diff[0] * diff[0] + diff[1] * diff[1] + diff[2] * diff[2]) / (2 * TRACKBALLSIZE);
    if (t > 1.0) t = 1.0;
    if (t < -1.0) t = -1.0;
    double angle = 2.0 * asin(t) * 180.0 / 3.141592653589793;;
    
    vtkCamera* camera = this->CurrentRenderer->GetActiveCamera();
    vtkTransform* Transform = vtkTransform::New(); 
    Transform->Identity();
    Transform->PostMultiply();
    double* fp = camera->GetFocalPoint();
    Transform->Translate(+fp[0],+fp[1],+fp[2]);
    Transform->RotateWXYZ(angle, axis);
    Transform->Translate(-fp[0],-fp[1],-fp[2]);
    //double newPosition[3];
    //Transform->TransformPoint(camera->GetPosition(), newPosition);
    //camera->SetPosition(newPosition);
    camera->ApplyTransform(Transform);
    Transform->Delete();
    
    double* vu = camera->GetViewUp();
    fp = camera->GetFocalPoint();
    double* p = camera->GetPosition();
    //std::cout << "Axes: (" << axis[0] << " ; " << axis[1] << " ; " << axis[2] << "): " << angle << std::endl;
    std::cout << "View Up: (" << vu[0] << " ; " << vu[1] << " ; " << vu[2] << ")" << std::endl;
    std::cout << "Focal Point: (" << fp[0] << " ; " << fp[1] << " ; " << fp[2] << ")" << std::endl;
    std::cout << "Position: (" << p[0] << " ; " << p[1] << " ; " << p[2] << ")" << std::endl;
    std::cout << "----------------------" << std::endl;
    rwi->Render();
    */
  
    /*
    if (this->CurrentRenderer == NULL)
      return;
    
    vtkCamera* camera = this->CurrentRenderer->GetActiveCamera();
    int* size = this->CurrentRenderer->GetRenderWindow()->GetSize();
    vtkRenderWindowInteractor *rwi = this->Interactor;
    
    int* lastMousePos = this->Interactor->GetLastEventPosition();
    int* currentMousePos = this->Interactor->GetEventPosition();
    
    double p1[3];
    double p2[3];
    this->ProjectToSphere(size, lastMousePos, p1);
    this->ProjectToSphere(size, currentMousePos, p2);
    
    double axes[3];
    vtkMath::Cross(p1, p2, axes);
    double norm = sqrt(axes[0] * axes[0] + axes[1] * axes[1] + axes[2] * axes[2]);
    axes[0] /= norm;
    axes[1] /= norm;
    axes[2] /= norm;
    
    double diff[3];
    diff[0] = p2[0] - p1[0];
    diff[1] = p2[1] - p1[1];
    diff[2] = p2[2] - p1[2];
    
    double si = sqrt(diff[0] * diff[0] + diff[1] * diff[1] + diff[2] * diff[2]);
    si /= (2.0 * this->m_Radius);
    double t = si;
    while (si > 1.0)
      si -= 2.0;
    double phi = si * 180 / 2.0;// * this->MotionFactor;
    
    if (t > 1.0) t = 1.0;
    if (t < -1.0) t = -1.0;
    double phi2 = 2.0 * asin(t) * 180.0 / 3.141592653589793;

    std::cout << "Axes: (" << axes[0] << " ; " << axes[1] << " ; " << axes[2] << "): " << phi << " vs " << phi2 << std::endl;

    
    //axes[0] = 0.0;
    //axes[1] = 0.0;
    //axes[2] = 1.0;
   
    vtkPerspectiveTransform* Transform = vtkPerspectiveTransform::New(); 
    Transform->Identity();
    //Transform->PostMultiply();
 
    // translate the focal point to the origin,
    // rotate about view up,
    // translate back again
    double *fp = camera->GetFocalPoint();
    Transform->Translate(+fp[0],+fp[1],+fp[2]);
    Transform->RotateWXYZ(phi, axes);
    Transform->Translate(-fp[0],-fp[1],-fp[2]);
  
    // apply the transform to the position
    double newPosition[3];
    Transform->TransformPoint(camera->GetPosition(), newPosition);
    camera->SetPosition(newPosition); 
    double newViewUp[3];
    //Transform->TransformPoint(camera->GetViewUp(), newViewUp);
    //camera->SetViewUp(newViewUp);
    
    //camera->OrthogonalizeViewUp();
    
    double* vu = camera->GetViewUp();
    std::cout << "View Up: (" << vu[0] << " ; " << vu[1] << " ; " << vu[2] << ")" << std::endl;
    std::cout << "----------------------" << std::endl;
    Transform->Delete();
    
    if (this->AutoAdjustCameraClippingRange)
      this->CurrentRenderer->ResetCameraClippingRange();
  
    if (rwi->GetLightFollowCamera()) 
      this->CurrentRenderer->UpdateLightsGeometryToFollowCamera();
  
    rwi->Render();
    */
  };
  /*
  void VTKInteractorStyleTrackballCamera::EndRotate()
  {
    this->m_LastMousePos = this->Interactor->GetEventPosition();
    this->Superclass::EndRotate();
  };
  */
  
  /**
   * Constructor.
   */
  VTKInteractorStyleTrackballCamera::VTKInteractorStyleTrackballCamera()
  : vtkInteractorStyleTrackballCamera()
  {
    this->m_Radius = 1.1;
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
   
  /*
  void VTKInteractorStyleTrackballCamera::ProjectToSphere(int* size, int* pos, double* vec)
  {
    double x = pos[0];
    double y = pos[1];
    double z, t;
    
    x /= (static_cast<double>(size[0]) / 2);
    y /= (static_cast<double>(size[1]) / 2);
    
    x -= 1;
    y -= 1;
    
    //std::cout << x << " ; " << y << std::endl;

    double d = sqrt(x*x + y*y);
    if (d < this->m_Radius * 0.707106781186548) // sqrt(2) / 2 = 0.707106781186548
      z = sqrt(this->m_Radius * this->m_Radius - d * d);
    else
    {
      t = this->m_Radius / 1.414213562373095; // sqrt(2) = 1.414213562373095
      z = t * t / d;
    }

    vec[0] = x;
    vec[1] = y;
    vec[2] = -z;
  };
  */
  
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
  
  double VTKInteractorStyleTrackballCamera::ProjectToSphere(double r, double x, double y) const
  {
    double d, t, z;

    d = sqrt(x * x + y * y);
    if (d < r * 0.70710678118654752440)   /* Inside sphere */
        z = sqrt(r * r - d * d);
    else  /* On hyperbola */
    {
      t = r / 1.41421356237309504880;
      z = t * t / d;
    }
    return z;
  };
};
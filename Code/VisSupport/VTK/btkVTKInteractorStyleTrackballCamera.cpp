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
#include <vtkPerspectiveTransform.h>
#include <vtkMath.h>

namespace btk
{
  /**
   * @class VTKInteractorStyleTrackballCamera
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
    this->Superclass::Rotate();
  
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
  };
  
  /**
   * @fn VTKInteractorStyleTrackballCamera::~VTKInteractorStyleTrackballCamera()
   * Empty destructor.
   */
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
};
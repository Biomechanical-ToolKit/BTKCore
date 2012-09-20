/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2012, Arnaud Barré
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

#ifndef __btkVTKChartExtraAcquisition_h
#define __btkVTKChartExtraAcquisition_h

#include "btkVTKChartExtraAcquisitionFunctor.h"

#include <vtkContextItem.h>

class vtkAxis;

namespace btk
{
  class VTKChartExtraAcquisition : public vtkContextItem
  {
  public:
    vtkTypeMacro(VTKChartExtraAcquisition, vtkContextItem);
    
    static VTKChartExtraAcquisition* New();
    
    ~VTKChartExtraAcquisition();
    
    VTKCurrentFrameFunctor::Pointer GetCurrentFrameFunctor() const {return this->mp_CurrentFrameFunctor;};
    BTK_VTK_EXPORT void SetCurrentFrameFunctor(VTKCurrentFrameFunctor::Pointer functor);
    VTKRegionOfInterestFunctor::Pointer GetRegionOfInterestFunctor() const {return this->mp_RegionOfInterestFunctor;};
    BTK_VTK_EXPORT void SetRegionOfInterestFunctor(VTKRegionOfInterestFunctor::Pointer functor);
    VTKEventsFunctor::Pointer GetEventsFunctor() const {return this->mp_EventsFunctor;};
    BTK_VTK_EXPORT void SetEventsFunctor(VTKEventsFunctor::Pointer functor);
    
    int GetDisplayEvents() const {return this->m_DisplayEvents;};
    BTK_VTK_EXPORT void SetDisplayEvents(int enabled);
    float GetEventLineWidth() const {return this->m_EventLineWidth;};
    void SetEventLineWidth(float width);
    int GetEventLineTypeFactor() const {return this->m_EventLineTypeFactor;};
    void SetEventLineTypeFactor(int factor);
    
    void SetAxes(vtkAxis* x, vtkAxis* y);
    
    virtual bool Paint(vtkContext2D *painter);
    
  protected:
    VTKChartExtraAcquisition();
    
  private:
    void SetXAxis(vtkAxis* axis);
    void SetYAxis(vtkAxis* axis);
    
    VTKCurrentFrameFunctor::Pointer mp_CurrentFrameFunctor;
    VTKRegionOfInterestFunctor::Pointer mp_RegionOfInterestFunctor;
    VTKEventsFunctor::Pointer mp_EventsFunctor;
    int m_DisplayEvents;
    float m_EventLineWidth;
    int m_EventLineTypeFactor;
    
    vtkAxis* XAxis;
    vtkAxis* YAxis;
  };
};

#endif // __btkVTKChartExtraAcquisition_h
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

#ifndef __btkVTKAxis_h
#define __btkVTKAxis_h

#include "btkConfigure.h"

#include <vtkAxis.h>

namespace btk
{
  class VTKAxis : public vtkAxis
  {
  public:
    enum {INSIDE = 0, OUTSIDE};
    
    BTK_VTK_EXPORT static VTKAxis* New();
    vtkExportedTypeRevisionMacro(VTKAxis, vtkAxis, BTK_VTK_EXPORT);
    
    virtual ~VTKAxis() {};
    
    bool GetTitleVisible() const {return this->m_TitleVisible;};
    BTK_VTK_EXPORT void SetTitleVisible(bool visible);
    
    float GetTickLength() const {return this->m_TickLength;};
    BTK_VTK_EXPORT void SetTickLength(float len);
    
    int GetTickDirection() const {return this->m_TickDirection;};
    BTK_VTK_EXPORT void SetTickDirection(int dir);
    
    float GetMinimumTickSpacing() const {return this->m_MinimumTickSpacing;};
    BTK_VTK_EXPORT void SetMinimumTickSpacing(float min);
    
    float GetTitleMargin() const {return this->m_TitleMargin;};
    BTK_VTK_EXPORT void SetTitleMargin(float margin);
    
    float GetLabelMargin() const {return this->m_LabelMargin;}
    BTK_VTK_EXPORT void SetLabelMargin(float margin);
    
    virtual bool Paint(vtkContext2D *painter);
    virtual void RecalculateTickSpacing();
    
    double GetTickScale() const {return this->m_TickScale;};
    BTK_VTK_EXPORT void SetTickScale(double scale);
    double GetTickOffset() const {return this->m_TickOffset;};
    BTK_VTK_EXPORT void SetTickOffset(double offset);
    
  protected:
    BTK_VTK_EXPORT VTKAxis();
    BTK_VTK_EXPORT void GenerateTickLabels(double min, double max);
    BTK_VTK_EXPORT double CalculateNiceMinMax(double& min, double& max);
    
    bool m_TitleVisible;
    float m_TickLength;
    int m_TickDirection;
    float m_MinimumTickSpacing;
    float m_TitleMargin;
    float m_LabelMargin;
    double m_TickScale;
    double m_TickOffset;
    
  private:
    VTKAxis(const VTKAxis& ); // Not implemented.
    void operator=(const VTKAxis& ); // Not implemented.
  };
};

#endif // __btkVTKAxis_h

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

#include "btkVTKAxis.h"

#include <vtkObjectFactory.h>
#include <vtkContext2D.h>
#include <vtkTextProperty.h>
#include <vtkFloatArray.h>
#include <vtkStdString.h>
#include <vtkStringArray.h>

namespace btk
{
  /**
   * @class VTKAxis btkVTKAxis.h
   * @brief Simple inherit of vtkAxis to fix an infinite loop with VTK 5.6 when recalculating tick spacing.
   *
   * This class will be replaced by a typedef with VTK 5.8 and greater.
   */
  
  /**
   * @fn static VTKAxis* VTKAxis::New()
   * Constructs a VTKAxis object and return it as a pointer.
   */
  vtkStandardNewMacro(VTKAxis);
  vtkCxxRevisionMacro(VTKAxis, "$Revision: 0.1 $");
  
  /**
   * @fn virtual VTKAxis::~VTKAxis();
   * Destructor.
   */
  
  /**
   * Same method than vtkAxis::RecalculateTickSpacing but fix an infinite loop with VTK 5.6
   */
  void VTKAxis::RecalculateTickSpacing()
  {
    if (this->Behavior < 2)
    {
      double min = this->Minimum;
      double max = this->Maximum;
      this->TickInterval = this->CalculateNiceMinMax(min, max);
      if (this->UsingNiceMinMax)
        this->GenerateTickLabels(this->Minimum, this->Maximum);
      else if (this->TickInterval == 0)
        return;
      else
      {
        while (min < this->Minimum)
          min += this->TickInterval;
        while (max > this->Maximum)
          max -= this->TickInterval;
        this->GenerateTickLabels(min, max);
      }
    }
  };
  
  /**
   * Default constructor
   */
  VTKAxis::VTKAxis()
  : vtkAxis()
  {};
};
/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2015, Arnaud Barré
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

#include "btkWrenchDirectionAngleFilter.h"

#include <Eigen/Geometry>

namespace btk
{
  /**
   * @class WrenchDirectionAngleFilter btkWrenchDirectionAngleFilter.h
   * @brief Calculate the direction angle of the force vector for each wrench.
   *
   * The direction angle is projected on each plane of the global frame in this order:
   *  - Angle yOz: set in the X component of the output ;
   *  - Angle xOz: set in the Y component of the output ;
   *  - Angle xOy: set in the Z component of the output.
   *
   * The output angles are expressed in degrees and the range is between 0 and 360 degrees.
   * Then a shift from 360 to 0 is possible if the force turns around itself.
   * 
   * @ingroup BTKBasicFilters
   */
  
  /**
   * @typedef WrenchDirectionAngleFilter::Pointer
   * Smart pointer associated with a WrenchDirectionAngleFilter object.
   */
  
  /**
   * @typedef WrenchDirectionAngleFilter::ConstPointer
   * Smart pointer associated with a const WrenchDirectionAngleFilter object.
   */
    
  /**
   * @fn static Pointer WrenchDirectionAngleFilter::New();
   * Creates a smart pointer associated with a WrenchDirectionAngleFilter object.
   */

  /**
   * @fn WrenchCollection::Pointer WrenchDirectionAngleFilter::GetInput()
   * Gets the input registered with this process.
   */

  /**
   * @fn void WrenchDirectionAngleFilter::SetInput(Wrench::Pointer input)
   * Sets the input required with this process. This input is transformed in a collection of wrenches with a single force platform.
   */
  
  /**
   * @fn void WrenchDirectionAngleFilter::SetInput(WrenchCollection::Pointer input)
   * Sets the input required with this process.
   */
  
  /**
   * @fn PointCollection::Pointer WrenchDirectionAngleFilter::GetOutput()
   * Gets the output created with this process.
   */
  
  /**
   * Constructor. Sets the number of inputs and outputs to 1.
   */
  WrenchDirectionAngleFilter::WrenchDirectionAngleFilter()
  : ProcessObject()
  {
    this->SetInputNumber(1);
    this->SetOutputNumber(1);
  };

  /**
   * @fn ForcePlatformCollection::Pointer WrenchDirectionAngleFilter::GetInput(int idx)
   * Returns the input at the index @a idx.
   */
  
  /**
   * @fn WrenchCollection::Pointer WrenchDirectionAngleFilter::GetOutput(int idx)
   * Returns the output at the index @a idx.
   */
  
  /**
   * Creates a PointCollection:Pointer object and return it as a DataObject::Pointer.
   */
  DataObject::Pointer WrenchDirectionAngleFilter::MakeOutput(int /* idx */)
  {
    return PointCollection::New();
  };
  
  /**
   * Generates the outputs' data.
   */
  void WrenchDirectionAngleFilter::GenerateData()
  {
    PointCollection::Pointer output = this->GetOutput();
    output->Clear();
    WrenchCollection::Pointer input = this->GetInput();
    const double radToDeg = 180.0 / M_PI;
    if (input)
    {
      for (WrenchCollection::ConstIterator it = input->Begin() ; it != input->End() ; ++it)
      {
        int numFrames = (*it)->GetForce()->GetFrameNumber();
        Point::Pointer dirAngle = Point::New((*it)->GetPosition()->GetLabel() + ".DA", numFrames, Point::Angle);
        for (int i = 0 ; i < numFrames ; ++i)
        {
          if ((*it)->GetPosition()->GetResiduals().coeff(i) >= 0)
          {
            dirAngle->GetValues().coeffRef(i,0) = atan2(-(*it)->GetForce()->GetValues().coeff(i,2), -(*it)->GetForce()->GetValues().coeff(i,1)) * radToDeg + 180.0;
            dirAngle->GetValues().coeffRef(i,1) = atan2(-(*it)->GetForce()->GetValues().coeff(i,2), -(*it)->GetForce()->GetValues().coeff(i,0)) * radToDeg + 180.0;
            dirAngle->GetValues().coeffRef(i,2) = atan2(-(*it)->GetForce()->GetValues().coeff(i,1), -(*it)->GetForce()->GetValues().coeff(i,0)) * radToDeg + 180.0;
          }
          else
          {
            dirAngle->GetResiduals().coeffRef(i) = -1.0;
          }
        }
        output->InsertItem(dirAngle);
      }
    }
  };
};


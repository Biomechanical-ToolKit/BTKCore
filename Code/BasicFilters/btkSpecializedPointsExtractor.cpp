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

#include "btkSpecializedPointsExtractor.h"

namespace btk
{
 /**
   * @class SpecializedPointsExtractor btkSpecializedPointsExtractor.h
   * @brief Extracts only the points corresponding to the given type (without cloning the extracted points).
   *
   * The type is set by the SetType method. By default, if no type is set, this filter extract markers.
   *
   * @ingroup BTKBasicFilters
   */
  
  /**
   * @typedef SpecializedPointsExtractor::Pointer
   * Smart pointer associated with a SpecializedPointsExtractor object.
   */
  
  /**
   * @typedef SpecializedPointsExtractor::ConstPointer
   * Smart pointer associated with a const SpecializedPointsExtractor object.
   */
    
  /**
   * @fn static Pointer SpecializedPointsExtractor::New();
   * Creates a smart pointer associated with a SpecializedPointsExtractor object.
   */

  /**
   * @fn Acquisition::Pointer SpecializedPointsExtractor::GetInput()
   * Gets the input registered with this process.
   */
  
  /**
   * @fn void SpecializedPointsExtractor::SetInput(Acquisition::Pointer input)
   * Sets the input required with this process.
   */
  
  /**
   * @fn PointCollection::Pointer SpecializedPointsExtractor::GetOutput()
   * Gets the output created with this process.
   */
  
  /**
   * @fn Point::Type SpecializedPointsExtractor::GetPointType() const
   * Returns the type of points to extract.
   */
  
  /**
   * Sets the type of points to extract.
   */
  void SpecializedPointsExtractor::SetPointType(Point::Type t)
  {
    if (this->m_Type == t)
      return;
    this->m_Type = t;
    this->Modified();
  };

  /**
   * Constructor. Sets the number of inputs and outputs to 1.
   */
  SpecializedPointsExtractor::SpecializedPointsExtractor()
  : ProcessObject()
  {
    this->m_Type = Point::Marker;
    this->SetInputNumber(1);
    this->SetOutputNumber(1);
  };

  /**
   * @fn Acquisition::Pointer SpecializedPointsExtractor::GetInput(int idx)
   * Returns the input at the index @a idx.
   */
  
  /**
   * @fn PointCollection::Pointer SpecializedPointsExtractor::GetOutput(int idx)
   * Returns the output at the index @a idx.
   */
  
  /**
   * Creates a PointCollection:Pointer object and return it as a DataObject::Pointer.
   */
  DataObject::Pointer SpecializedPointsExtractor::MakeOutput(int /* idx */)
  {
    return PointCollection::New();
  };
  
  /**
   * Generates the outputs' data.
   */
  void SpecializedPointsExtractor::GenerateData()
  {
    PointCollection::Pointer output = this->GetOutput();
    output->Clear();
    Acquisition::Pointer input = this->GetInput();
    if (input.get() != 0)
    {
      PointCollection::Pointer allPoints = input->GetPoints();
      for(PointCollection::Iterator itPoint = allPoints->Begin() ; itPoint != allPoints->End() ; ++itPoint)
      {
        if ((*itPoint)->GetType() == this->m_Type)
          output->InsertItem(*itPoint);
      }
    }
  };  
};


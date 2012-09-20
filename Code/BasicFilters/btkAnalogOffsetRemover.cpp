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

#include "btkAnalogOffsetRemover.h"

#include <Eigen/Array> // cwise

namespace btk
{
  /**
   * @class AnalogOffsetRemover btkAnalogOffsetRemover.h
   * @brief Remove a possible analog offset to the selected analog channels.
   *
   * Based on the given offset input (see the method SetOffsetInput()), each analog signals are averaged and 
   * removed from the raw input (see the method SetRawInput()). The analog signals set in the raw input which
   * are not given to the offset input are not processed but will be available in the output.
   *
   * @ingroup BTKBasicFilters
   */
   
   /**
    * @typedef AnalogOffsetRemover::Pointer
    * Smart pointer associated with a AnalogOffsetRemover object.
    */

   /**
    * @typedef AnalogOffsetRemover::ConstPointer
    * Smart pointer associated with a const AnalogOffsetRemover object.
    */

   /**
    * @fn static Pointer AnalogOffsetRemover::New();
    * Creates a smart pointer associated with a AnalogOffsetRemover object.
    */

   /**
    * @fn Acquisition::Pointer AnalogOffsetRemover::GetRawInput()
    * Gets the input registered with this process which is used as the input to process.
    */

   /**
    * @fn void AnalogOffsetRemover::SetRawInput(Acquisition::Pointer input)
    * Sets the input required with this process which is used as the input to process.
    */

   /**
     * @fn Acquisition::Pointer AnalogOffsetRemover::GetOffsetInput()
     * Gets the input registered with this process which corresponds to the offsets to remove.
     */

    /**
     * @fn void AnalogOffsetRemover::SetOffsetInput(Acquisition::Pointer input)
     * Sets the input required with this process which corresponds to the offsets to remove.
     */

   /**
    * @fn PointCollection::Pointer AnalogOffsetRemover::GetOutput()
    * Gets the output created with this process.
    */
  
  /**
   * Constructor. 
   * Sets the number of inputs to 2 (raw and offset acquisition) and outputs to 1.
   */
  AnalogOffsetRemover::AnalogOffsetRemover()
  : ProcessObject()
  {
    this->SetInputNumber(2);
    this->SetOutputNumber(1);
  };

  /**
   * @fn Acquisition::Pointer AnalogOffsetRemover::GetInput(int idx)
   * Returns the input at the index @a idx.
   */
  
  /**
   * @fn Acquisition::Pointer AnalogOffsetRemover::GetOutput(int idx)
   * Returns the output at the index @a idx.
   */
  
  /**
   * Creates an Acquisition:Pointer object and return it as a DataObject::Pointer.
   */
  DataObject::Pointer AnalogOffsetRemover::MakeOutput(int /* idx */)
  {
    return Acquisition::New();
  };
  
  /**
   * Generates the outputs' data.
   */
  void AnalogOffsetRemover::GenerateData()
  {
    Acquisition::Pointer rawInput = this->GetRawInput();
    Acquisition::Pointer offsetInput = this->GetOffsetInput();
    if (!rawInput || !offsetInput)
    {
      btkErrorMacro("Missing at least one input.");
      return;
    }
    
    AnalogCollection::Pointer raws = rawInput->GetAnalogs();
    AnalogCollection::Pointer offsets = offsetInput->GetAnalogs();
    AnalogCollection::Pointer analogs = AnalogCollection::New();
    std::list< std::pair<Analog::Pointer, Analog::Pointer> > signals; // Raw, offset
    for (AnalogCollection::ConstIterator itR = raws->Begin() ; itR != raws->End() ; ++itR)
    {
      bool found = false;
      for (AnalogCollection::ConstIterator itO = offsets->Begin() ; itO != offsets->End() ; ++itO)
      {
        if ((*itO)->GetLabel().compare((*itR)->GetLabel()) == 0)
        {
          Analog::Pointer clone = (*itR)->Clone();
          signals.push_back(std::make_pair(clone, *itO));
          analogs->InsertItem(clone);
          found = true;
          break;
        }
      }
      if (!found)
        analogs->InsertItem(*itR);
    }
    
    for (std::list< std::pair<Analog::Pointer, Analog::Pointer> >::iterator it = signals.begin() ; it != signals.end() ; ++it)
    {
      double dc = it->second->GetValues().sum() / it->second->GetValues().rows();
      it->first->GetValues().cwise() -= dc;
    }
    
    Acquisition::Pointer output = this->GetOutput();
    output->Reset();
    output->SetFirstFrame(rawInput->GetFirstFrame());
    output->SetPointFrequency(rawInput->GetPointFrequency());
    output->SetAnalogResolution(rawInput->GetAnalogResolution());
    output->SetPointUnits(rawInput->GetPointUnits());
    output->SetEvents(rawInput->GetEvents());
    output->SetMetaData(rawInput->GetMetaData());
    output->SetPoints(rawInput->GetPoints());
    output->SetAnalogs(analogs);
    // To set internal variables
    output->Resize(rawInput->GetPointNumber(), rawInput->GetPointFrameNumber(), rawInput->GetAnalogNumber(), rawInput->GetNumberAnalogSamplePerFrame());
  };
};

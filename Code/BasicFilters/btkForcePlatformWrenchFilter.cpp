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

#include "btkForcePlatformWrenchFilter.h"
#include "btkConvert.h"

namespace btk
{
  /**
   * @class ForcePlatformWrenchFilter
   * @brief Calcule the wrench of the center of the force platform data, expressed in the global frame.
   *
   * @ingroup BTKBasicFilters
   */
  
  /**
   * @typedef ForcePlatformWrenchFilter::Pointer
   * Smart pointer associated with a ForcePlatformWrenchFilter object.
   */
  
  /**
   * @typedef ForcePlatformWrenchFilter::ConstPointer
   * Smart pointer associated with a const ForcePlatformWrenchFilter object.
   */
    
  /**
   * @fn static Pointer ForcePlatformWrenchFilter::New();
   * Creates a smart pointer associated with a ForcePlatformWrenchFilter object.
   */

  /**
   * @fn WrenchCollection::Pointer ForcePlatformWrenchFilter::GetInput()
   * Gets the input registered with this process.
   */

  /**
   * @fn void ForcePlatformWrenchFilter::SetInput(ForcePlatform::Pointer input)
   * Sets the input required with this process. This input is transformed in a collection a force platform with a single force platform.
   */
  
  /**
   * @fn void ForcePlatformWrenchFilter::SetInput(ForcePlatformCollection::Pointer input)
   * Sets the input required with this process.
   */
  
  /**
   * @fn PointCollection::Pointer ForcePlatformWrenchFilter::GetOutput()
   * Gets the output created with this process.
   */

  /**
   * Constructor. Sets the number of inputs and outputs to 1.
   */
  ForcePlatformWrenchFilter::ForcePlatformWrenchFilter()
  : ProcessObject()
  {
    this->SetInputNumber(1);
    this->SetOutputNumber(1);
  };

  /**
   * @fn ForcePlatformCollection::Pointer ForcePlatformWrenchFilter::GetInput(int idx)
   * Returns the input at the index @a idx.
   */
  
  /**
   * @fn WrenchCollection::Pointer ForcePlatformWrenchFilter::GetOutput(int idx)
   * Returns the output at the index @a idx.
   */
  
  /**
   * Creates a WrenchCollection:Pointer object and return it as a DataObject::Pointer.
   */
  DataObject::Pointer ForcePlatformWrenchFilter::MakeOutput(int /* idx */)
  {
    return WrenchCollection::New();
  };
  
  /**
   * Generates the outputs' data.
   */
  void ForcePlatformWrenchFilter::GenerateData()
  {
    WrenchCollection::Pointer output = this->GetOutput();
    output->Clear();
    ForcePlatformCollection::Pointer input = this->GetInput();
    if (input.get() != 0)
    {
      int inc = 0;
      for (ForcePlatformCollection::ConstIterator it = input->Begin() ; it != input->End() ; ++it)
      {
        ++inc;
        if ((*it)->GetChannelNumber() == 0)
        {
          btkErrorMacro("Unexpected number of analog channels (0) for force platfom #" + ToString(inc));
          continue;
        }
        int frameNumber = (*it)->GetChannel(0)->GetFrameNumber();
        Wrench::Pointer wrh = Wrench::New("FPW" + ToString(inc), frameNumber);
        output->InsertItem(wrh);
        // Residuals & masks
        wrh->GetPosition()->GetMasks().setZero(frameNumber);
        wrh->GetPosition()->GetResiduals().setZero(frameNumber);        
        wrh->GetForce()->GetMasks().setZero(frameNumber);
        wrh->GetForce()->GetResiduals().setZero(frameNumber);
        wrh->GetMoment()->GetMasks().setZero(frameNumber);
        wrh->GetMoment()->GetResiduals().setZero(frameNumber);
        // Values
        switch((*it)->GetType())
        {
          // 6 channels
          case 1:
            wrh->GetForce()->GetValues().col(0) = (*it)->GetChannel(0)->GetValues();
            wrh->GetForce()->GetValues().col(1) = (*it)->GetChannel(1)->GetValues();
            wrh->GetForce()->GetValues().col(2) = (*it)->GetChannel(2)->GetValues();
            wrh->GetPosition()->GetValues().col(0) = (*it)->GetChannel(3)->GetValues();
            wrh->GetPosition()->GetValues().col(1) = (*it)->GetChannel(4)->GetValues();
            wrh->GetPosition()->GetValues().col(2).setZero();
            wrh->GetMoment()->GetValues().col(0).setZero();
            wrh->GetMoment()->GetValues().col(1).setZero();
            wrh->GetMoment()->GetValues().col(2) = (*it)->GetChannel(5)->GetValues();
            this->FinishTypeI(wrh, *it, inc);
            break;
          case 2:
          case 4:
          case 5:
            wrh->GetForce()->GetValues().col(0) = (*it)->GetChannel(0)->GetValues();
            wrh->GetForce()->GetValues().col(1) = (*it)->GetChannel(1)->GetValues();
            wrh->GetForce()->GetValues().col(2) = (*it)->GetChannel(2)->GetValues();
            wrh->GetMoment()->GetValues().col(0) = (*it)->GetChannel(3)->GetValues();
            wrh->GetMoment()->GetValues().col(1) = (*it)->GetChannel(4)->GetValues();
            wrh->GetMoment()->GetValues().col(2) = (*it)->GetChannel(5)->GetValues();
            this->FinishAMTI(wrh, *it, inc);
            break;
          case 3:
            // Fx
            wrh->GetForce()->GetValues().col(0) = (*it)->GetChannel(0)->GetValues() + (*it)->GetChannel(1)->GetValues();
            // Fy
            wrh->GetForce()->GetValues().col(1) = (*it)->GetChannel(2)->GetValues() + (*it)->GetChannel(3)->GetValues();
            // Fz
            wrh->GetForce()->GetValues().col(2) = (*it)->GetChannel(4)->GetValues() + (*it)->GetChannel(5)->GetValues() + (*it)->GetChannel(6)->GetValues() + (*it)->GetChannel(7)->GetValues();
            // Mx
            wrh->GetMoment()->GetValues().col(0) = (*it)->GetOrigin().y() * ((*it)->GetChannel(4)->GetValues() + (*it)->GetChannel(5)->GetValues() - (*it)->GetChannel(6)->GetValues() - (*it)->GetChannel(7)->GetValues());
            // My
            wrh->GetMoment()->GetValues().col(1) = (*it)->GetOrigin().x() * ((*it)->GetChannel(5)->GetValues() + (*it)->GetChannel(6)->GetValues() - (*it)->GetChannel(4)->GetValues() - (*it)->GetChannel(7)->GetValues());
            // Mz
            wrh->GetMoment()->GetValues().col(2) = (*it)->GetOrigin().y() * ((*it)->GetChannel(1)->GetValues() - (*it)->GetChannel(0)->GetValues()) + (*it)->GetOrigin().x() * ((*it)->GetChannel(2)->GetValues() - (*it)->GetChannel(3)->GetValues());
            this->FinishKistler(wrh, *it, inc);
            break;
          case 6:
            btkErrorMacro("Force Platform type 6 is not yet supported. Please, report this to the developers");
            break;
          case 7:
            btkErrorMacro("Force Platform type 7 is not yet supported. Please, report this to the developers");
            break;
          case 11:
            btkErrorMacro("Force Platform type 11 is not yet supported. Please, report this to the developers");
            break;
          case 12:
            btkErrorMacro("Force Platform type 12 is not yet supported. Please, report this to the developers");
            break;
          case 21:
            btkErrorMacro("Force Platform type 21 is not yet supported. Please, report this to the developers");
            break;
        }
        this->TransformToGlobal(wrh, (*it)->GetCorners());
      }
    }
  };
  
  /**
   * Finish the computation of the wrench for the force platform type I (nothing to do).
   */
  void ForcePlatformWrenchFilter::FinishTypeI(Wrench::Pointer /* wrh */, ForcePlatform::Pointer /* fp */, int /* index */)
  {};
  
  /**
   * Finish the computation of the wrench for the AMTI force platform (nothing to do).
   */
  void ForcePlatformWrenchFilter::FinishAMTI(Wrench::Pointer /* wrh */, ForcePlatform::Pointer /* fp */, int /* index */)
  {};
   
  /**
   * Finish the computation of the wrench for the Kistler force platform (nothing to do).
   */
  void ForcePlatformWrenchFilter::FinishKistler(Wrench::Pointer /* wrh */, ForcePlatform::Pointer /* fp */, int /* index */)
  {};
};


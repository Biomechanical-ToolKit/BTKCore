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
 
#ifndef __btkCalibrationProcedure_h
#define __btkCalibrationProcedure_h

#include "btkModel.h"
#include "btkAcquisition.h"
#include "btkSubject.h"
#include "btkLandmarksTranslator.h"
#include "btkMacro.h"

namespace btk
{
  class CalibrationProcedure
  {
  public:
    typedef SharedPtr<CalibrationProcedure> Pointer;
    typedef SharedPtr<const CalibrationProcedure> ConstPointer;
    
    virtual void Calibrate(Model::Pointer output, Acquisition::Pointer input, Subject::Pointer s, LandmarksTranslator::Pointer lt) const {btkErrorMacro("Generic method, does nothing.");};
    
  protected:
    CalibrationProcedure() {};
  };
  
  /**
   * @class CalibrationProcedure
   * @brief Generic class to calibrate a model based on an acquisition and subject's informations.
   *
   * @ingroup BTKModels
   */

  /**
   * @typedef CalibrationProcedure::Pointer
   * Smart pointer associated with a CalibrationProcedure object.
   */

  /**
   * @typedef CalibrationProcedure::ConstPointer
   * Smart pointer associated with a const CalibrationProcedure object.
   */
   
  /**
   * @fn virtual void CalibrationProcedure::Calibrate(Model::Pointer output, Acquisition::Pointer input, Subject::Pointer subject, LandmarksTranslator::Pointer lt) const
   * Generic method to calibration model.
   */
   
  /**
   * @fn CalibrationProcedure::CalibrationProcedure()
   * Constructor
   */
}

#endif // __btkCalibrationProcedure_h

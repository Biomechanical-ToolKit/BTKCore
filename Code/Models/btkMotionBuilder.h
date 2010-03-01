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
 
#ifndef __btkMotionBuilder_h
#define __btkMotionBuilder_h

#include "btkSharedPtr.h"
#include "btkModel.h"
#include "btkAcquisition.h"
#include "btkLandmarksTranslator.h"
#include "btkMacro.h"

namespace btk
{
  class MotionBuilder
  {
  public:
    typedef SharedPtr<MotionBuilder> Pointer;
    typedef SharedPtr<const MotionBuilder> ConstPointer;
    
    virtual void Build(Model::Pointer output, Acquisition::Pointer input, LandmarksTranslator::Pointer lt) const {btkErrorMacro("Generic method, does nothing.");};
    
  protected:
    MotionBuilder() {};
  };
  
  /**
   * @class MotionBuilder
   * @brief Generic class to build segments' motion from a Model, an acquisition and a LandmarksTranslator (optional) object.
   *
   * @ingroup BTKModels
   */

  /**
   * @typedef MotionBuilder::Pointer
   * Smart pointer associated with a MotionBuilder object.
   */

  /**
   * @typedef MotionBuilder::ConstPointer
   * Smart pointer associated with a const MotionBuilder object.
   */
   
  /**
   * @fn virtual void MotionBuilder::Build(Model::Pointer model, Acquisition::Pointer, LandmarksTranslator::Pointer) const
   * Generic method to build segment's motion.
   */
   
  /**
   * @fn MotionBuilder::MotionBuilder()
   * Constructor
   */
}

#endif // __btkMotionBuilder_h

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

#ifndef __btkForcePlatformWithCalibrationMatrix_h
#define __btkForcePlatformWithCalibrationMatrix_h

#include "btkForcePlatform.h"

namespace btk
{
  template <int t, int r, int c>
  class ForcePlatformWithCalibrationMatrix : public ForcePlatform
  {
  public:
    typedef SharedPtr<ForcePlatformWithCalibrationMatrix> Pointer;
    typedef SharedPtr<const ForcePlatformWithCalibrationMatrix> ConstPointer;

    static Pointer New() {return Pointer(new ForcePlatformWithCalibrationMatrix());};

    ~ForcePlatformWithCalibrationMatrix() {};

  protected:
    ForcePlatformWithCalibrationMatrix();
    
  private:
    ForcePlatformWithCalibrationMatrix(const ForcePlatformWithCalibrationMatrix& ); // Not implemeted.
    ForcePlatformWithCalibrationMatrix& operator=(const ForcePlatformWithCalibrationMatrix& ); // Not implemented 
  };

  /**
   * @class ForcePlatformWithCalibrationMatrix btkForcePlatformWithCalibrationMatrix.h
   * @brief Represents force platform with a calibration matrix
   *
   * @tparam t Force platform type
   * @tparam r Number of rows in the calibration matrix
   * @tparam c Number of columns in the calibration matrix. Set also the number of channels.
   *
   * @ingroup BTKCommon
   */

  /**
   * @typedef ForcePlatformWithCalibrationMatrix<t,r,c>::Pointer
   * Smart pointer associated with a ForcePlatformWithCalibrationMatrix object.
   */
  
  /**
   * @typedef ForcePlatformWithCalibrationMatrix<t,r,c>::ConstPointer
   * Smart pointer associated with a const ForcePlatformWithCalibrationMatrix object.
   */

  /**
   * @fn Pointer ForcePlatformWithCalibrationMatrix<t,r,c>::New()
   * Creates a smart pointer associated with a ForcePlatformWithCalibrationMatrix object.
   */

  /**
   * @fn ForcePlatformWithCalibrationMatrix<t,r,c>::~ForcePlatformWithCalibrationMatrix()
   * Empty destructor.
   */

  /**
   * Constructor.
   */
  template <int t, int r, int c>
  ForcePlatformWithCalibrationMatrix<t,r,c>::ForcePlatformWithCalibrationMatrix()
  : ForcePlatform()
  {
    this->m_Type = t;
    this->m_Channels->SetItemNumber(c);
    this->m_CalMatrix = CalMatrix::Identity(r, c);
  };
};

#endif // __btkForcePlatformWithCalibrationMatrix_h

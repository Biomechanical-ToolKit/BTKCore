/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2013, Arnaud Barré
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

#ifndef __btkIMUTypes_h
#define __btkIMUTypes_h

#include "btkIMU.h"

namespace btk
{
  template <int t>
  class IMUType : public IMU
  {
  public:
    static IMU::Pointer New(const std::string& label = "IMU", const std::string& desc = "", bool init = true) {return Pointer(new IMUType(label, desc, init));};

  protected:
    IMUType(const std::string& label, const std::string& desc, bool init);
    
  private:
    IMUType(const IMUType& ); // Not implemeted.
    IMUType& operator=(const IMUType& ); // Not implemented 
  };
  
  // ----------------------------------------------------------------------- //
  
  /**
   * Represents IMU Type-1 (6D: 3 accelerometers and 3 gyroscopes)
   * @ingroup BTKCommon
   */
  typedef IMUType<1> IMUType1;
  /**
   * Represents IMU Type-2 (Type 1 + calibration matrix used to align accelerometers and gyroscopes on the same inertial reference frame)
   * @ingroup BTKCommon
   */
  typedef IMUType<2> IMUType2;
  
  // ----------------------------------------------------------------------- //

  /**
   * @class IMUType btkIMUTypes.h
   * @brief Convenient class to represents a concrete IMU
   *
   * @tparam t IMU type
   *
   * This class helps to create a configured IMU::Pointer object with required number of channels, calibration matrix, etc. The list of supported IMUs is presented below.
   *
   * Some IMU types are already provided:
   * - btk::IMUType1: IMU Type-1 (6D: 3 accelerometers and 3 gyroscopes)
   * - btk::IMUType2: IMU Type-2 (Type 1 + calibration matrix to align accelerometers and gyroscopes together)
   *
   * @warning The use of the New() static method will return a IMU::Pointer object.
   *
   * @ingroup BTKCommon
   */

  /**
   * @fn Pointer IMUType<t>::New()
   * Creates a smart pointer associated with a IMUType object.
   */

  /**
   * Constructor.
   */
  template <int t>
  IMUType<t>::IMUType(const std::string& label, const std::string& desc, bool init)
  : IMU(label, desc, init)
  {
    this->m_Type = t;
  };
};

#endif // __btkIMUTypes_h


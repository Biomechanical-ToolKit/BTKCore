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

#ifndef __btkForcePlatformTypes_h
#define __btkForcePlatformTypes_h

#include "btkForcePlatform.h"

namespace btk
{
  template <int t, int r, int c>
  class ForcePlatformType : public ForcePlatform
  {
  public:
    static ForcePlatform::Pointer New() {return ForcePlatform::Pointer(new ForcePlatformType());};

  protected:
    ForcePlatformType();
    
  private:
    ForcePlatformType(const ForcePlatformType& ); // Not implemeted.
    ForcePlatformType& operator=(const ForcePlatformType& ); // Not implemented 
  };
  
  // ----------------------------------------------------------------------- //
  
  /**
   * Represents Force platform Type-1 (6 channels: FX, FY, FZ, PX, PY, MZ)
   * @ingroup BTKCommon
   */
  typedef ForcePlatformType<1,6,6> ForcePlatformType1;
  /**
   * Represents Force platform Type-2 (6 channels: FX, FY, FZ, MX, MY, MZ)
   * @ingroup BTKCommon
   */
  typedef ForcePlatformType<2,6,6> ForcePlatformType2;
  /**
   * Represents Force platform Type-3 (8 channels: FZ1, FZ2, FZ3, FZ4, FX12, FX34, FY14, FY23)
   * @ingroup BTKCommon
   */
  typedef ForcePlatformType<3,8,8> ForcePlatformType3;
  /**
   * Represents Force platform Type-4 (Same as Type-2 + calibration matrix 6 by 6)
   * @ingroup BTKCommon
   */
  typedef ForcePlatformType<4,6,6> ForcePlatformType4;
  /**
   * Represents Force platform Type-5 (8 channels: FZ1, FZ2, FZ3, FZ4, FX12, FX34, FY14, FY23 + calibration matrix 6 (columns) by 8 (rows))
   * @ingroup BTKCommon
   */
  typedef ForcePlatformType<5,8,6> ForcePlatformType5;
  /**
   * Represents Force platform Type-6 (12 channels: FX[1,2,3,4], FY[1,2,3,4], FZ[1,2,3,4] + calibration matrix 12 by 12)
   * @ingroup BTKCommon
   */
  typedef ForcePlatformType<6,12,12> ForcePlatformType6;
  
  // btkForcePlatformType7
  // btkForcePlatformType11
  // btkForcePlatformType12
  // btkForcePlatformType21
  
  // ----------------------------------------------------------------------- //

  /**
   * @class ForcePlatformType btkForcePlatformTypes.h
   * @brief Convenient class to represent a concrete force platform
   *
   * @tparam t Force platform type
   * @tparam r Number of rows in the calibration matrix
   * @tparam c Number of columns in the calibration matrix. Set also the number of channels.
   * 
   * This class helps to create a configured force platform (ForcePlatform::Pointer object) with required number of channels, calibration matrix, etc. The list of supported force platforms is presented below.
   *
   * Several force platform types are already provided:
   * - btk::ForcePlatformType1: Force platform Type-1 (6 channels: FX, FY, FZ, PX, PY, MZ)
   * - btk::ForcePlatformType2: Force platform Type-2 (6 channels: FX, FY, FZ, MX, MY, MZ)
   * - btk::ForcePlatformType3: Force platform Type-3 (8 channels: FZ1, FZ2, FZ3, FZ4, FX12, FX34, FY14, FY23)
   * - btk::ForcePlatformType4: Force platform Type-4 (Same as Type-2 + calibration matrix 6 by 6)
   * - btk::ForcePlatformType5: Force platform Type-5 (8 channels: FZ1, FZ2, FZ3, FZ4, FX12, FX34, FY14, FY23 + calibration matrix 6 (columns) by 8 (rows))
   * - btk::ForcePlatformType6: Force platform Type-6 (12 channels: FX[1,2,3,4], FY[1,2,3,4], FZ[1,2,3,4] + calibration matrix 12 by 12)
   *
   * @warning The use of the New() static method will return a ForcePlatofrm::Pointer object.
   *
   * @ingroup BTKCommon
   */

  /**
   * @fn Pointer ForcePlatformType<t,r,c>::New()
   * Creates a smart pointer associated with a ForcePlatformType object.
   */

  /**
   * Constructor.
   */
  template <int t, int r, int c>
  ForcePlatformType<t,r,c>::ForcePlatformType()
  : ForcePlatform()
  {
    this->m_Type = t;
    this->m_Channels->SetItemNumber(c);
    this->m_CalMatrix.setIdentity(r, c);
  };
};

#endif // __btkForcePlatformTypes_h


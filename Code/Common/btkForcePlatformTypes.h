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

#ifndef __btkForcePlatformTypes_h
#define __btkForcePlatformTypes_h

//#include "btkForcePlatformWithoutCalibrationMatrix.h"
#include "btkForcePlatformWithCalibrationMatrix.h"

namespace btk
{
  /**
   * Represents Force platform Type-1 (6 channels: FX, FY, FZ, PX, PY, MZ)
   * @ingroup BTKCommon
   */
  typedef ForcePlatformWithCalibrationMatrix<1,6,6> ForcePlatformType1;
  /**
   * Represents Force platform Type-2 (6 channels: FX, FY, FZ, MX, MY, MZ)
)
   * @ingroup BTKCommon
   */
  typedef ForcePlatformWithCalibrationMatrix<2,6,6> ForcePlatformType2;
  /**
   * Represents Force platform Type-3 (8 channels: FZ1, FZ2, FZ3, FZ4, FX12, FX34, FY14, FY23)
   * @ingroup BTKCommon
   */
  typedef ForcePlatformWithCalibrationMatrix<3,8,8> ForcePlatformType3;
  /**
   * Represents Force platform Type-4 (Same as Type-2 + calibration matrix 6 by 6)
   * @ingroup BTKCommon
   */
  typedef ForcePlatformWithCalibrationMatrix<4,6,6> ForcePlatformType4;
  /**
   * Represents Force platform Type-5 (8 channels: FZ1, FZ2, FZ3, FZ4, FX12, FX34, FY14, FY23 + calibration matrix 6 (columns) by 8 (rows))
   * @ingroup BTKCommon
   */
  typedef ForcePlatformWithCalibrationMatrix<5,8,6> ForcePlatformType5;
  /**
   * Represents Force platform Type-6 (12 channels: FX[1,2,3,4], FY[1,2,3,4], FZ[1,2,3,4] + calibration matrix 12 by 12)

   * @ingroup BTKCommon
   */
  typedef ForcePlatformWithCalibrationMatrix<6,12,12> ForcePlatformType6;
};

//#include "btkForcePlatformType7.h"
//#include "btkForcePlatformType11.h"
//#include "btkForcePlatformType12.h"
//#include "btkForcePlatformType21.h"

#endif // __btkForcePlatformTypes_h


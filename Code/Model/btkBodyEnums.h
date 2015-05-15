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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS
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

#ifndef __btkBodyEnums_h
#define __btkBodyEnums_h

#include "btkMacros.h" // _BTK_NOEXCEPT

namespace btk
{
  
  struct BodySide
  {
    enum : short {
      Center = 1,
      Left = 2,
      Right = 4,
      Both =  Left | Right
    };
    
    BodySide() = delete;
    ~BodySide() _BTK_NOEXCEPT = delete;
    BodySide(const BodySide& ) = delete;
    BodySide(BodySide&& ) _BTK_NOEXCEPT = delete;
    BodySide& operator=(const BodySide& ) = delete;
    BodySide& operator=(BodySide&& ) _BTK_NOEXCEPT = delete;
  };
  
  struct BodyRegion
  {
    enum : short {
      Upper = 1,
      Lower = 2,
      Full = Upper | Lower,
    };
    
    BodyRegion() = delete;
    ~BodyRegion() _BTK_NOEXCEPT = delete;
    BodyRegion(const BodyRegion& ) = delete;
    BodyRegion(BodyRegion&& ) _BTK_NOEXCEPT = delete;
    BodyRegion& operator=(const BodyRegion& ) = delete;
    BodyRegion& operator=(BodyRegion&& ) _BTK_NOEXCEPT = delete;
  };
  
  struct BodyPart
  {
    enum : short {
      Head = 1,
      Torso,
      Clavicle,
      Arm,
      Forearm,
      Hand,
      Pelvis,
      Thigh,
      Shank,
      Foot,
      User = 1024
    };
    
    BodyPart() = delete;
    ~BodyPart() _BTK_NOEXCEPT = delete;
    BodyPart(const BodyPart& ) = delete;
    BodyPart(BodyPart&& ) _BTK_NOEXCEPT = delete;
    BodyPart& operator=(const BodyPart& ) = delete;
    BodyPart& operator=(BodyPart&& ) _BTK_NOEXCEPT = delete;
  };
};

#endif // __btkBodyEnums_h
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

#ifndef __btkEndianFormat_h
#define __btkEndianFormat_h

#include "btkConfigure.h"

namespace btk
{
  enum class EndianFormat : int {
    NotApplicable,
    VAXLittleEndian,
    IEEELittleEndian,
    IEEEBigEndian,
#if BTK_ARCH_COMPILED == BTK_ARCH_IEEE_LE
    Native = IEEELittleEndian
#elif BTK_ARCH_COMPILED == BTK_ARCH_VAX_LE
    Native = VAXLittleEndian
#elif BTK_ARCH_COMPILED == BTK_ARCH_IEEE_BE
    Native = IEEEBigEndian
#else
    #error The Native value cannot be unknown at compile time. Processor not supported
    // Native = NotApplicable
#endif
  };

  /**
   * @enum EndianFormat
   * Enums used to specify the endian format for the converter integrated in a BinaryStream.
   */
  /**
   * @var EndianFormat NotApplicable
   * Enum value used to mention that no endian format is applicable. This value would be used only to informa and not to set.
   */
  /**
   * @var EndianFormat VAXLittleEndian
   * Enum value representing the VAX architecture with little endian byte interpretation (DEC processor).
   */
  /**
   * @var EndianFormat IEEELittleEndian
   * Enum value representing the IEEE architecture with little endian byte interpretation (Intel processor).
   */
  /**
   * @var EndianFormat IEEEBigEndian
   * Enum value representing the IEEE architecture with big endian byte interpretation (MIPS processor).
   */
  /**
   * @var EndianFormat Native
   * Convenient enum value which let the compiler chooses the default architecture (i.e. IEEELittleEndian, VAXLittleEndian, IEEEBigEndian)..
   */
};

#endif // __btkEndianFormat_h
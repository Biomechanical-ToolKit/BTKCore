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

#include "btklzo.h"
#include "lzo.h"

#include <stdexcept>

namespace btk
{
  /**
   * In case an error occurs during the decoding, an exception is thrown.
   */
  void lzo1x_decode(void* out, int* outlen, const void* in, int* inlen)
  {
    int res = av_lzo1x_decode(out, outlen, in, inlen);
    if (res != 0)
    {
      if (res == AV_LZO_INPUT_DEPLETED)
        throw (std::runtime_error("LZO - End of input buffer"));
      else if (res == AV_LZO_OUTPUT_FULL)
        throw (std::runtime_error("LZO - Full output buffer"));
      else if (res == AV_LZO_INVALID_BACKPTR)
        throw (std::runtime_error("LZO - Wrong reference to previously decoded data"));
      else if (res == AV_LZO_ERROR)
        throw (std::runtime_error("LZO - Unknown error"));
      else
        throw (std::runtime_error("LZO - Unexpected error code"));
    }
  };
};

// LZO dependencies
#include "lzo.c"
#include "mem.c"
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

#ifndef __btkMex_h
#define __btkMex_h

#if defined(_MSC_VER)
  // Disable unsafe warning (use of the function 'strcpy' instead of 
  // 'strcpy_s' for portability reasons;
  #pragma warning( disable : 4996 )
  // Workaround for the char16_t type defined in Matlab and MSVC 2010
  #if (_MSC_VER >= 1600)
    #define __STDC_UTF_16__
  #endif
#endif

#include <Eigen/Eigen> // Fix for some conflicts between Scilab and Eigen
#include <btkSharedPtr.h> // Fix for some conflicts between Scilab and C++ type (bool)
#include <mex.h>

// Fix types definition introduced in Matlab version 7.3
// Fix types not provided in Scilab. 
// FIXME: What about 64 bits OS?,
#if !defined(SCI_MEX)
  #include <matrix.h> // Header from Matlab
#endif
#if !defined(MX_API_VER) || MX_API_VER < 0x07030000
typedef int mwSize;
typedef int mwIndex;
#endif

#if defined(SCI_MEX)
  // Fix for Scilab and its macro GetType which is in conflict with the method btk::Point::GetType
  #ifdef GetType
    #undef GetType
  #endif
  #if !defined(SCI_VERSION_MAJOR)
    #include <version.h>
  #endif
  // Prior to Scilab 5.2, functions mexErrMsgTxt and mexWarnMsgTxt require a char* argument
  #if SCI_VERSION_MAJOR <= 5 && SCI_VERSION_MINOR < 2
    inline void mexErrMsgTxt(const char* err_msg) {mexErrMsgTxt(const_cast<char*>(err_msg));};
    inline void mexWarnMsgTxt(const char* err_msg) {mexWarnMsgTxt(const_cast<char*>(err_msg));};
    // For mexPrintf
    #include <stdarg.h>
    #include <cstdio>
    inline void mexPrintf(const char* fmt, ...)
    {
      va_list args; char buf[2048];
      va_start(args, fmt);
      vsprintf(buf, fmt, args);
      mexPrintf(const_cast<char*>("%s"), buf);
      va_end(args);
    };
  #endif
#endif

inline void btkMXCheckNoOuput(int nlhs, mxArray *plhs[])
{
#if defined(SCI_MEX)
  if (nlhs > 1)
    mexErrMsgTxt("Too many output arguments.");
  plhs[0] = mxCreateDoubleScalar(0.0);
#else
  if (nlhs > 0)
    mexErrMsgTxt("Too many output arguments.");
#endif
};

#endif // __btkMex_h
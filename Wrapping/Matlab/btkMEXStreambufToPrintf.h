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

#ifndef __btkMEXStreambufToPrintf_h
#define __btkMEXStreambufToPrintf_h

#include <streambuf>
#include <iostream>
#include <cstdio> // EOF for linux

#include "btkMex.h"

namespace btk
{
  /**
   * @brief Stream buffer to display message in the Matlab command window by using the function mexPrintf.
   *
   * For example, the redirection of std::cout in the Maltab Command Window is done by the following code. 
   * @code
   * btk::MEXStreambufToPrintf matlabStandardOutput;
   * std::streambuf* stdStandardOutput = std::cout.rdbuf(&matlabStandardOutput)
   *   ... // your stuff
   * std::cout.rdbuf(stdStandardOutput);
   * @endcode
   * It is important to go back to the previous stream buffer.
   *
   * @ingroup BTKWrappingMatlab
   */
  class MEXStreambufToPrintf : public std::streambuf
  {
  public:
    // MEXStreambufToPrintf(); // Implicit.
    // MEXStreambufToPrintf(const MEXStreambufToPrintf& toCopy); // Implicit.
    // ~MEXStreambufToPrintf(); // Implicit.
    // MEXStreambufToPrintf& operator=(const MEXStreambufToPrintf& toCopy); // Implicit.

  protected:
    inline virtual std::streamsize xsputn(const char* s, std::streamsize n); 
    inline virtual int overflow(int c = EOF);
  };

  /**
   * Overloaded method which calls the function mexPrintf
   */
  std::streamsize MEXStreambufToPrintf::xsputn(const char* s, std::streamsize n) 
  {
    /*
    #if defined(SCI_MEX) && SCI_VERSION_MAJOR <= 5 && SCI_VERSION_MINOR < 2
      mexPrintf(const_cast<char*>("%.*s"), n, s);
    #else
      mexPrintf("%.*s", n, s);
    #endif
    */
    mexPrintf("%.*s", n, s);
    
    return n;
  };

  /**
   * Overloaded method which calls the function mexPrintf
   */
  int MEXStreambufToPrintf::overflow(int c) 
  {
    if (c != EOF)
    {
      /*
      #if defined(SCI_MEX) && SCI_VERSION_MAJOR <= 5 && SCI_VERSION_MINOR < 2
        mexPrintf(const_cast<char*>("%.1s"), &c);
      #else
        mexPrintf("%.1s", &c);
      #endif
      */
      mexPrintf("%.1s", &c);
    }
      
    return 1;
  }
};

#endif // __btkMEXStreambufToPrintf_h

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

#ifndef __btkMEXStreambufToWarnMsgTxt_h
#define __btkMEXStreambufToWarnMsgTxt_h

#include <streambuf>
#include <iostream>
#include <cstdio> // EOF for linux

#include "btkMex.h"

namespace btk
{
  /**
   * @brief Stream buffer to display message in the Matlab command window by using the function mexWarnMsgTxt.
   *
   * For example, the redirection of std::cerr in the Maltab Command Window is done by the following code. 
   * @code
   * btk::MEXStreambufToWarnMsgTxt matlabErrorOutput;
   * std::streambuf* stdErrorOutput = std::cerr.rdbuf(&matlabErrorOutput);
   *   ... // your stuff
   * std::cerr.rdbuf(stdErrorOutput);
   * @endcode
   * It is important to go back to the previous stream buffer.

   * @ingroup BTKWrappingMatlab
   */
  class MEXStreambufToWarnMsgTxt : public std::streambuf
  {
  public:
    inline MEXStreambufToWarnMsgTxt(const std::string& id);
    // MEXStreambufToWarnMsgTxt(const MEXStreambufToWarnMsgTxt& toCopy); // Implicit.
    // ~MEXStreambufToWarnMsgTxt(); // Implicit.
    // MEXStreambufToWarnMsgTxt& operator=(const MEXStreambufToWarnMsgTxt& toCopy); // Implicit.

  protected:
    inline virtual std::streamsize xsputn(const char* s, std::streamsize n); 
    inline virtual int overflow(int c = EOF);

  private:
    std::string m_Id;
    std::string m_Message;
  };

  /**
   * Constructor
   */
  MEXStreambufToWarnMsgTxt::MEXStreambufToWarnMsgTxt(const std::string& id)
  : m_Id(id), m_Message()
  {}; 
  
  /**
   * Overloaded method which calls the function mexWarnMsgTxt
   */
  std::streamsize MEXStreambufToWarnMsgTxt::xsputn(const char* s, std::streamsize n) 
  {
    this->m_Message += std::string(s,n);
    return n;
  };

  /**
   *  Overloaded method which display a warning message when @a c is equals to the newline character. 
   */
  int MEXStreambufToWarnMsgTxt::overflow(int c) 
  {
    if (c == 10) // New line
    {
      mexWarnMsgIdAndTxt(this->m_Id.c_str(), this->m_Message.c_str());
      this->m_Message.clear();
    }
    else
      this->m_Message.push_back(static_cast<char>(c));
    return 1;
  }
};

#endif // __btkMEXStreambufToWarnMsgTxt_h

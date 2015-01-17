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

#ifndef __btkMEXWarnLogToWarnMsgTxt_h
#define __btkMEXWarnLogToWarnMsgTxt_h

#include "btkMEXLoggerRedirection.h"
#include "btkMEXStreambufToWarnMsgTxt.h"

namespace btk
{
  /**
   * @class MEXWarnLogToWarnMsgTxt btkMEXWarnLogToWarnMsgTxt.h
   * @brief Class to redirect the warning logs to the MEX function mexWarnMsgIdAndTxt.
   *
   * As the new stream buffer is wrapped around the MEX function mexWarnMsgIdAndTxt, it is important
   * to give to the constructor a specific ID which will can be used to enable or disable this warning in Matlab.
   * @code
   * void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
   * {
   *   // Redirection of the warning logs to the mexWarnMsgTxt function.
   *   btk::MEXWarnLogToWarnMsgTxt warnRedir = btk::MEXWarnLogToWarnMsgTxt("myProgram:MyID"); // It is important to construct an object otherwise the redirection is not done.
   *   ... // Do what you want
   *   btk::Logger::Warning("Simple example to show you that this line will be printed in the Matlab console as a warning.");
   *   return;
   *   // The redirection is automaticaly reseted at the end of the function
   * }
   * @endcode
   *
   * @sa MEXStreambufToWarnMsgTxt
   * @ingroup BTKWrappingMatlab
   */
  class MEXWarnLogToWarnMsgTxt : public MEXLoggerRedirection<MEXStreambufToWarnMsgTxt>
  {
  public:
    MEXWarnLogToWarnMsgTxt(const std::string& id) : MEXLoggerRedirection<MEXStreambufToWarnMsgTxt>(id)
    {
      this->m_OldVerboseMode = Logger::GetVerboseMode();
      this->mp_OldStream = Logger::GetWarningStream();
      Logger::SetVerboseMode(Logger::MessageOnly);
      Logger::SetWarningStream(this->mp_Output);
    };
    ~MEXWarnLogToWarnMsgTxt()
    {
      Logger::SetVerboseMode(this->m_OldVerboseMode);
      Logger::SetWarningStream(this->mp_OldStream);
    };
  };
  
  /**
   * @fn template <class T> MEXWarnLogToWarnMsgTxt::MEXWarnLogToWarnMsgTxt(const std::string& id)
   * Constructor
   */
   
  /**
   * @fn template <class T> MEXWarnLogToWarnMsgTxt::~MEXWarnLogToWarnMsgTxt()
   * Destructor
   */
};

#endif // __btkMEXWarnLogToWarnMsgTxt_h

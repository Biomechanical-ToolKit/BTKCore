/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2014, Arnaud Barré
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

#ifndef __btkMEXDebugLogToPrintf_h
#define __btkMEXDebugLogToPrintf_h

#include "btkMEXLoggerRedirection.h"
#include "btkMEXStreambufToPrintf.h"

namespace btk
{
  /**
   * @class MEXDebugLogToPrintf btkMEXDebugLogToPrintf.h
   * @brief Class to redirect to debug logs to the MEX function mexPrintf.
   *
   * The use of this class gives you the possibility in one line to redirect the logs sent to the debug logger (btk::Logger::Debug()) during the execution
   * of the MEX function. Automaticaly at the end of the function (or the scope of the variable set), the redirection is reseted.
   * @code
   * void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
   * {
   *   // Redirection of the debug logs to the mexPrintf function.
   *   btk::MEXDebugLogToPrintf debugRedir = btk::MEXDebugLogToPrintf(); // It is important to construct an object otherwise the redirection is not done.
   *   ... // Do what you want
   *   btk::Logger::Debug("Simple example to show you that this line will be printed in the Matlab console.");
   *   return;
   *   // The redirection is automaticaly reseted at the end of the function
   * }
   * @endcode
   *
   * @sa MEXStreambufToPrintf
   * @ingroup BTKWrappingMatlab
   */
  class MEXDebugLogToPrintf : public MEXLoggerRedirection<MEXStreambufToPrintf>
  {
  public:
    MEXDebugLogToPrintf() : MEXLoggerRedirection<MEXStreambufToPrintf>()
    {
      this->m_OldVerboseMode = Logger::GetVerboseMode();
      this->mp_OldStream = Logger::GetDebugStream();
      Logger::SetVerboseMode(Logger::MessageOnly);
      Logger::SetDebugStream(this->mp_Output);
    };
    ~MEXDebugLogToPrintf()
    {
      Logger::SetVerboseMode(this->m_OldVerboseMode);
      Logger::SetDebugStream(this->mp_OldStream);
    };
  };
   
 /**
  * @fn template <class T> MEXDebugLogToPrintf::MEXDebugLogToPrintf()
  * Constructor
  */
  
 /**
  * @fn template <class T> MEXDebugLogToPrintf::~MEXDebugLogToPrintf()
  * Destructor
  */
};

#endif // __btkMEXDebugLogToPrintf_h

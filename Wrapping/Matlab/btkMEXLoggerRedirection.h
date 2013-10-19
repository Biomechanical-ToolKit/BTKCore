/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2013, Arnaud Barré
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

#ifndef __btkMEXLoggerRedirection_h
#define __btkMEXLoggerRedirection_h

#include "btkMEXStreambufToPrintf.h"
#include "btkMEXStreambufToWarnMsgTxt.h"
#include "btkLogger.h"

#include "btkMex.h"

namespace btk
{
  /**
   * @class MEXLoggerRedirection btkMEXLoggerRedirection.h
   * @brief Template class to redirect a btk::Logger stream.
   *
   * This is a generic class which gives the possibility to redirect any
   * btk::Logger stream to another stream.
   *
   * @ingroup BTKWrappingMatlab
   * @sa MEXDebugLogToPrintf, MEXWarnLogToWarnMsgTxt
   */
  template <class T>
  class MEXLoggerRedirection
  {
  public:
    MEXLoggerRedirection();
    MEXLoggerRedirection(const std::string& id);
    virtual ~MEXLoggerRedirection();
  protected:
    T* mp_Buffer;
    std::ostream* mp_Output;
    Logger::VerboseMode m_OldVerboseMode;
    Logger::Stream::Pointer mp_OldStream;
  };
  
  /**
   * @class MEXDebugLogToPrintf btkMEXLoggerRedirection.h
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
   * @class MEXWarnLogToWarnMsgTxt btkMEXLoggerRedirection.h
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
  
  // ----------------------------------------------------------------------- //
  
  /**
   * Constructor setting the new buffer by using the default constructor of the template class @a T
   */
  template <class T>
  MEXLoggerRedirection<T>::MEXLoggerRedirection()
  : mp_OldStream()
  {
    this->mp_Buffer = new T();
    this->mp_Output = new std::ostream(this->mp_Buffer);
    this->m_OldVerboseMode = Logger::Quiet;
  };
  
  /**
   * Constructor setting the new buffer by using a constructor of the template class @a T requiring an id.
   */
  template <class T>
  MEXLoggerRedirection<T>::MEXLoggerRedirection(const std::string& id)
  : mp_OldStream()
  {
    this->mp_Buffer = new T(id);
    this->mp_Output = new std::ostream(this->mp_Buffer);
    this->m_OldVerboseMode = Logger::Quiet;
  };
  
  /**
   * Destructor
   * Reset the redirection.
   */
  template <class T>
  MEXLoggerRedirection<T>::~MEXLoggerRedirection()
  {
    delete this->mp_Buffer;
    delete this->mp_Output;
  };
  
  /**
   * @fn template <class T> MEXDebugLogToPrintf::MEXDebugLogToPrintf()
   * Constructor
   */
   
  /**
   * @fn template <class T> MEXWarnLogToWarnMsgTxt::MEXWarnLogToWarnMsgTxt(const std::string& id)
   * Constructor
   */
};
#endif // __btkMEXStreambufToPrintf_h

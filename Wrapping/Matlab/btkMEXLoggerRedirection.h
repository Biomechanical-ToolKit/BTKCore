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

#ifndef __btkMEXLoggerRedirection_h
#define __btkMEXLoggerRedirection_h

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
};
#endif // __btkMEXStreambufToPrintf_h

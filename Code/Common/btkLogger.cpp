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

#include "btkLogger.h"

#include <iostream>

namespace btk
{
#ifdef _NDEBUG
  Logger::VerboseMode Logger::s_VerboseMode = Logger::Normal;
#else
  Logger::VerboseMode Logger::s_VerboseMode = Logger::Detailed;
#endif
  std::string Logger::s_Prefix = "BTK";
  std::string Logger::s_DebugAffix = "DEBUG";
  std::string Logger::s_WarningAffix = "WARNING";
  std::string Logger::s_ErrorAffix = "ERROR";
  Logger::Stream::Pointer Logger::sp_DebugStream = Logger::Stream::New(&(std::cout));
  Logger::Stream::Pointer Logger::sp_WarningStream = Logger::Stream::New(&(std::cerr));
  Logger::Stream::Pointer Logger::sp_ErrorStream = Logger::Stream::New(&(std::cerr));
  
  void Logger::PrintMessage(Stream* level, const std::string& affix, const std::string& filename, int line, const std::string& msg)
  {
    if (Logger::s_VerboseMode == Logger::Quiet)
      return;
    if (Logger::s_VerboseMode > Logger::MessageOnly)
    {
      level->GetOutput() << (Logger::s_Prefix.empty() ? "" : "[" + Logger::s_Prefix + " ")
                     << (affix.empty() ? "" : affix)
                     << (Logger::s_Prefix.empty() && affix.empty() ? "" : "] ");
    }
    if ((Logger::s_VerboseMode == Logger::Detailed) && (!filename.empty()))
    {
      level->GetOutput() << filename;
      if (line > 0)
        level->GetOutput() << " (" << line << ")";
      level->GetOutput() << ": ";
    }
    level->GetOutput() << msg << std::endl;
  };
  
  // ----------------------------------------------------------------------- //
  
  Logger::Stream::Stream(std::ostream* output)
  {
    this->mp_Output = output;
    this->m_Owned = false;
  };
  
  Logger::Stream::~Stream()
  {
    if (this->m_Owned)
      delete this->mp_Output;
  };
  
};
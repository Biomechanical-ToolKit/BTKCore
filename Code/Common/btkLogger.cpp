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
  Logger::Stream Logger::s_DebugStream = Logger::Stream::Stream(&(std::cout), "DEBUG");
  Logger::Stream Logger::s_WarningStream = Logger::Stream::Stream(&(std::cerr), "WARNING");
  Logger::Stream Logger::s_ErrorStream = Logger::Stream::Stream(&(std::cerr), "ERROR");
  
  void Logger::SetLevelStream(Stream* level, std::ostream* p)
  {
    level->pointer = p;
    level->owned = false;
  };
  
  void Logger::PrintMessage(Stream* level, const std::string& filename, int line, const std::string& msg)
  {
    if (Logger::s_VerboseMode == Logger::Quiet)
      return;
    if (Logger::s_VerboseMode > Logger::MessageOnly)
    {
      *(level->pointer) << (Logger::s_Prefix.empty() ? "" : "[" + Logger::s_Prefix + " ")
                     << (level->affix.empty() ? "" : level->affix)
                     << (Logger::s_Prefix.empty() && level->affix.empty() ? "" : "] ");
    }
    if ((Logger::s_VerboseMode == Logger::Detailed) && (!filename.empty()))
    {
      *(level->pointer) << filename;
      if (line > 0)
        *(level->pointer) << " (" << line << ")";
      *(level->pointer) << ": ";
    }
    *(level->pointer) << msg << std::endl;
  };
  
  // ----------------------------------------------------------------------- //
  
  Logger::Stream::Stream(std::ostream* p, const std::string& str)
  : affix(str)
  {
    this->pointer = p;
    this->owned = false;
  };
  
  Logger::Stream::~Stream()
  {
    if (this->owned)
      delete this->pointer;
  };
  
};
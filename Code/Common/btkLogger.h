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

#ifndef __btkLogger_h
#define __btkLogger_h

#include <fstream>
#include <string>

namespace btk
{
  class Logger
  {
  public:
    typedef enum {Quiet = 0, MessageOnly = 1, Normal = 2, Detailed = 3} VerboseMode;
    
    static void Debug(const std::string& msg) {Logger::PrintMessage(&Logger::s_DebugStream, msg);};
    static void Warning(const std::string& msg) {Logger::PrintMessage(&Logger::s_WarningStream, msg);};
    static void Error(const std::string& msg) {Logger::PrintMessage(&Logger::s_ErrorStream, msg);};
    
    static void Debug(const std::string& filename, int line, const std::string& msg) {Logger::PrintMessage(&Logger::s_DebugStream, filename, line, msg);};
    static void Warning(const std::string& filename, int line, const std::string& msg) {Logger::PrintMessage(&Logger::s_WarningStream, filename, line, msg);};
    static void Error(const std::string& filename, int line, const std::string& msg) {Logger::PrintMessage(&Logger::s_ErrorStream, filename, line, msg);};
    
    static VerboseMode GetVerboseMode() {return Logger::s_VerboseMode;};
    static void SetVerboseMode(VerboseMode mode) {Logger::s_VerboseMode = mode;};
    
    static const std::string& GetPrefix() {return Logger::s_Prefix;};
    static void SetPrefix(const std::string& str) {Logger::s_Prefix = str;};
    
    static std::ostream& GetDebugStream() {return *(Logger::s_DebugStream.pointer);};
    static std::ostream& GetWarningStream() {return *(Logger::s_WarningStream.pointer);};
    static std::ostream& GetErrorStream() {return *(Logger::s_ErrorStream.pointer);};
    static void SetDebugStream(std::ostream* p) {Logger::SetLevelStream(&Logger::s_DebugStream, p);};
    static void SetWarningStream(std::ostream* p) {Logger::SetLevelStream(&Logger::s_WarningStream, p);};
    static void SetErrorStream(std::ostream* p) {Logger::SetLevelStream(&Logger::s_ErrorStream, p);};
    
    static const std::string& GetDebugAffix() {return Logger::s_DebugStream.affix;};
    static const std::string& GetWarningAffix() {return Logger::s_DebugStream.affix;};
    static const std::string& GetErrorAffix() {return Logger::s_DebugStream.affix;};
    static void SetDebugAffix(const std::string& str) {Logger::s_DebugStream.affix = str;};
    static void SetWarningAffix(const std::string& str) {Logger::s_DebugStream.affix = str;};
    static void SetErrorAffix(const std::string& str) {Logger::s_DebugStream.affix = str;};
    
  private:
    struct Stream
    {
      Stream(std::ostream* p, const std::string& str);
      ~Stream();
      // Stream(const Stream&); // Implicit.
      // Stream& operator= (const Stream&); // Implicit.
      std::ostream* pointer;
      bool owned;
      std::string affix;
    };
    
    static void SetLevelStream(Stream* level, std::ostream* p);
    static void PrintMessage(Stream* level, const std::string& msg) {Logger::PrintMessage(level, "", 0, msg);};
    static void PrintMessage(Stream* level, const std::string& filename, int line, const std::string& msg);
    
    static VerboseMode s_VerboseMode;
    static std::string s_Prefix;
    static Stream s_DebugStream;
    static Stream s_WarningStream;
    static Stream s_ErrorStream; 
  };
};

#endif // __btkLogger_h
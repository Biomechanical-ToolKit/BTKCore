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

#include "btkSharedPtr.h"
#include "btkMacro.h"

#include <fstream>
#include <string>

/**
 * Internal macro used to print log on the given stream @c s.
 */
#define _btkLogMacro(s, p, l, ...) \
  /* Keep the scope of _argc, _argv only inside the "loop" */ \
  do \
  { \
    std::string _argv[] = { __VA_ARGS__ }; \
    int _argc = (sizeof _argv) / (sizeof _argv[0]); \
    if (_argc == 1) \
      btk::Logger::s(btkStripPathMacro(p), l, _argv[0]); \
    else \
      btk::Logger::s(btkStripPathMacro(p), l, std::string(btkStripPathMacro(_argv[0].c_str())) + " - " + _argv[1]); \
  } \
  while (0)

/**
 * Send a debug message to the logger with information on its source code location (filename, line number).
 * This macro can be used with one or two strings. 
 * If only one is passed, this is considered as the log message. 
 * If two strings are passed, then the first one is considered as the path of a file and the second one is considered as the log message.
 * In this second case, the macro strip the path of the given file to keep only the filename and concat it to the message by using a dash separator between them (i.e. filename - message).
 * The second case is usefull for log messages sent from IO reader/writer to know which processed file has somme issues during batch.
 */
#define btkDebugMacro(...) \
  _btkLogMacro(Debug, __FILE__, __LINE__, __VA_ARGS__);

/**
 * Send a warning message to the logger with information on its source code location (filename, line number).
 * This macro can be used with one or two strings. 
 * If only one is passed, this is considered as the log message. 
 * If two strings are passed, then the first one is considered as the path of a file and the second one is considered as the log message.
 * In this second case, the macro strip the path of the given file to keep only the filename and concat it to the message by using a dash separator between them (i.e. filename - message).
 * The second case is usefull for log messages sent from IO reader/writer to know which processed file has somme issues during batch.
 */
#define btkWarningMacro(...) \
  _btkLogMacro(Warning, __FILE__, __LINE__, __VA_ARGS__);
  
/**
 * Send an error message to the logger with information on its source code location (filename, line number).
 * This macro can be used with one or two strings. 
 * If only one is passed, this is considered as the log message. 
 * If two strings are passed, then the first one is considered as the path of a file and the second one is considered as the log message.
 * In this second case, the macro strip the path of the given file to keep only the filename and concat it to the message by using a dash separator between them (i.e. filename - message).
 * The second case is usefull for log messages sent from IO reader/writer to know which processed file has somme issues during batch.
 */
#define btkErrorMacro(...) \
  _btkLogMacro(Error, __FILE__, __LINE__, __VA_ARGS__);

// ------------------------------------------------------------------------- //

// ------------------------------------------------------------------------- //

namespace btk
{
  class Logger
  {
  public:
    typedef enum {Quiet = 0, MessageOnly = 1, Normal = 2, Detailed = 3} VerboseMode;
    
    class Stream
    {
    public:
      typedef btkSharedPtr<Stream> Pointer;
      static Pointer New(std::ostream* output) {return Pointer(new Stream(output));};
      ~Stream();
      std::ostream& GetOutput() const {return *(this->mp_Output);};
            
    private:
      Stream(std::ostream* output);
      
      Stream(const Stream&); // Not implemented.
      Stream& operator= (const Stream&); // Notimplemented.
      
      std::ostream* mp_Output;
      bool m_Owned;
    };
    
#ifdef NDEBUG
    static void Debug(const std::string& msg) {btkNotUsed(msg);};
    static void Debug(const std::string& filename, int line, const std::string& msg) {btkNotUsed(filename);btkNotUsed(line);btkNotUsed(msg);};
#else
    static void Debug(const std::string& msg) {Logger::PrintMessage(Logger::sp_DebugStream.get(), Logger::s_DebugAffix, msg);};
    static void Debug(const std::string& filename, int line, const std::string& msg) {Logger::PrintMessage(Logger::sp_DebugStream.get(), Logger::s_DebugAffix, filename, line, msg);};
#endif
    static void Warning(const std::string& msg) {Logger::PrintMessage(Logger::sp_WarningStream.get(), Logger::s_WarningAffix, msg);};
    static void Warning(const std::string& filename, int line, const std::string& msg) {Logger::PrintMessage(Logger::sp_WarningStream.get(), Logger::s_WarningAffix, filename, line, msg);};
    
    static void Error(const std::string& msg) {Logger::PrintMessage(Logger::sp_ErrorStream.get(), Logger::s_ErrorAffix, msg);};
    static void Error(const std::string& filename, int line, const std::string& msg) {Logger::PrintMessage(Logger::sp_ErrorStream.get(), Logger::s_ErrorAffix, filename, line, msg);};
    
    static VerboseMode GetVerboseMode() {return Logger::s_VerboseMode;};
    static void SetVerboseMode(VerboseMode mode) {Logger::s_VerboseMode = mode;};
    
    static const std::string& GetPrefix() {return Logger::s_Prefix;};
    static void SetPrefix(const std::string& str) {Logger::s_Prefix = str;};
    
    static Logger::Stream::Pointer GetDebugStream() {return Logger::sp_DebugStream;};
    static Logger::Stream::Pointer GetWarningStream() {return Logger::sp_WarningStream;};
    static Logger::Stream::Pointer GetErrorStream() {return Logger::sp_ErrorStream;};
    static void SetDebugStream(std::ostream* output) {Logger::SetDebugStream(Logger::Stream::New(output));};
    static void SetWarningStream(std::ostream* output) {Logger::SetWarningStream(Logger::Stream::New(output));};
    static void SetErrorStream(std::ostream* output) {Logger::SetErrorStream(Logger::Stream::New(output));};
    static void SetDebugStream(Logger::Stream::Pointer stream) {Logger::sp_DebugStream = stream;};
    static void SetWarningStream(Logger::Stream::Pointer stream) {Logger::sp_WarningStream = stream;};
    static void SetErrorStream(Logger::Stream::Pointer stream) {Logger::sp_ErrorStream = stream;};
    
    static const std::string& GetDebugAffix() {return Logger::s_DebugAffix;};
    static const std::string& GetWarningAffix() {return Logger::s_WarningAffix;};
    static const std::string& GetErrorAffix() {return Logger::s_ErrorAffix;};
    static void SetDebugAffix(const std::string& str) {Logger::s_DebugAffix = str;};
    static void SetWarningAffix(const std::string& str) {Logger::s_WarningAffix = str;};
    static void SetErrorAffix(const std::string& str) {Logger::s_ErrorAffix = str;};
    
  private:
    static void PrintMessage(Stream* level, const std::string& affix, const std::string& msg) {Logger::PrintMessage(level, affix, "", 0, msg);};
    static void PrintMessage(Stream* level, const std::string& affix, const std::string& filename, int line, const std::string& msg);
    
    static VerboseMode s_VerboseMode;
    static std::string s_Prefix;
    static std::string s_DebugAffix;
    static std::string s_WarningAffix;
    static std::string s_ErrorAffix;
    static Stream::Pointer sp_DebugStream;
    static Stream::Pointer sp_WarningStream;
    static Stream::Pointer sp_ErrorStream; 
  };
};

#endif // __btkLogger_h
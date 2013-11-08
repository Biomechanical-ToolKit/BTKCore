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
      BTK_COMMON_EXPORT ~Stream();
      std::ostream& GetOutput() const {return *(this->mp_Output);};
            
    private:
      BTK_COMMON_EXPORT Stream(std::ostream* output);
      
      Stream(const Stream&); // Not implemented.
      Stream& operator= (const Stream&); // Notimplemented.
      
      std::ostream* mp_Output;
      bool m_Owned;
    };
    
    BTK_COMMON_EXPORT static void Debug(const std::string& msg);
    BTK_COMMON_EXPORT static void Debug(const std::string& filename, int line, const std::string& msg);

    BTK_COMMON_EXPORT static void Warning(const std::string& msg);
    BTK_COMMON_EXPORT static void Warning(const std::string& filename, int line, const std::string& msg);
    
    BTK_COMMON_EXPORT static void Error(const std::string& msg);
    BTK_COMMON_EXPORT static void Error(const std::string& filename, int line, const std::string& msg);
    
    BTK_COMMON_EXPORT static VerboseMode GetVerboseMode();
    BTK_COMMON_EXPORT static void SetVerboseMode(VerboseMode mode);
    
    BTK_COMMON_EXPORT static const std::string& GetPrefix();
    BTK_COMMON_EXPORT static void SetPrefix(const std::string& str);
    
    BTK_COMMON_EXPORT static Logger::Stream::Pointer GetDebugStream();
    BTK_COMMON_EXPORT static Logger::Stream::Pointer GetWarningStream();
    BTK_COMMON_EXPORT static Logger::Stream::Pointer GetErrorStream();
    BTK_COMMON_EXPORT static void SetDebugStream(std::ostream* output);
    BTK_COMMON_EXPORT static void SetWarningStream(std::ostream* output);
    BTK_COMMON_EXPORT static void SetErrorStream(std::ostream* output);
    BTK_COMMON_EXPORT static void SetDebugStream(Logger::Stream::Pointer stream);
    BTK_COMMON_EXPORT static void SetWarningStream(Logger::Stream::Pointer stream);
    BTK_COMMON_EXPORT static void SetErrorStream(Logger::Stream::Pointer stream);
    
    BTK_COMMON_EXPORT static const std::string& GetDebugAffix();
    BTK_COMMON_EXPORT static const std::string& GetWarningAffix();
    BTK_COMMON_EXPORT static const std::string& GetErrorAffix();
    BTK_COMMON_EXPORT static void SetDebugAffix(const std::string& str);
    BTK_COMMON_EXPORT static void SetWarningAffix(const std::string& str);
    BTK_COMMON_EXPORT static void SetErrorAffix(const std::string& str);
    
  private:
    static void PrintMessage(Stream* level, const std::string& affix, const std::string& msg);
    static void PrintMessage(Stream* level, const std::string& affix, const std::string& filename, int line, const std::string& msg);
  };
};

#endif // __btkLogger_h
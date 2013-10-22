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
  /**
   * @class Logger btkLogger.h
   * @brief Log mechanism to display debug message, warnings and errors
   *
   * The logger class is implemented with the possibility to print debug message, warnings and errors on different Logger::Stream. By default the debug messages are printed on the standard output (std::cout), while the warnings and errors are printed on the standard error (std::cerr).
   * To use the log mechanism, you only need to include the header @a btkLogger.h and use one of the static methods Logger::Debug(), Logger::Warning(), or Logger::Error(), depending of the type of message you want to print. 
   * To simplify the writing of logs with the automatic detection of the filename and line number, three macros are proposed:
   *  - @c btkDebugMacro(msg): alias for the code <tt>btk::Logger::Debug(__FILE__, __LINE__, msg)</tt>;
   *  - @c btkWarningMacro(msg): alias for the code <tt>btk::Logger::Warning(__FILE__, __LINE__, msg)</tt>;
   *  - @c btkErrorMacro(msg): alias for the code <tt>btk::Logger::Error(__FILE__, __LINE__, msg)</tt>;
   *
   * The debug logs are only available when the code is compiled in debug mode (when the symbol NDEBUG is not defined).
   *
   * The diplayed log can be split in 4 parts:
   *  - Prefix: Name of the application/library (by default: BTK)
   *  - Log affix: String for the type of log (by default: DEBUG, WARNING, ERROR)
   *  - File detail: Optional information which can be given to the methods (filename, line)
   *  - Message: string with the message to the log.
   *
   * There are four verbose modes for the display of the logs which can be selected with the method SetVerboseMode().
   * By default, when the library is compiled in debug mode, the verbose mode is set to Logger::Detailed. In release mode, this is set to Logger::Normal mode. You have also the mode Logger::Quiet where nothing is displayed. The fourh mode is Logger::MessageOnly and show only the given message to the method whitout prefix nor affix.
   * Depending of the selected verbose mode, each log can be printed as the following:
   *  - Quiet: (nothing)
   *  - MessageOnly: <tt>message</tt>
   *  - Normal: <tt>[prefix log_affix] message</tt>
   *  - Detailed: <tt>[prefix log_affix] filename (line): message</tt> (filename and line must be given to the method)
   *
   * If the prefix and affix are empty, the empty square brackets will not be printed.
   *
   * The prefix and affix strings can be set with the method SetPrefix(), SetDebugAffix(), SetWarningAffix(), SetErrorAffix().
   *
   * It is possible to select other output streams than std::cout and std::cerr using the method SetDebugStream(), SetWarningStream(), and SetErrorStream().
   *
   * An example to use this logger is:
   * @code{.cpp}
   * #include <btkLogger.h>
   *
   * int main(int argc, char *argv[])
   * {
   *   btk::Logger::Debug("My message");
   *   btk::Logger::Debug("test.cpp", 42, "Other message with specific filename and line number");
   *   btk::Logger::Debug(__FILE__, __LINE__, "Another message with automatic detection of the filename and line number");
   *   return 0;
   * };
   * @endcode
   *
   * @ingroup BTKCommon
   */
  
  /**
   * @typedef Logger::VerboseModel
   * Analog' values along the time with 1 component (1 column).
   */
  
  /**
   * @enum Logger::Quiet
   * Do not display any message
   */
  /**
   * @enum Logger::MessageOnly
   * Display only the message
   */
  /**
   * @enum Logger::Normal
   * Display the message as well as the prefix and log affix.
   */
  /**
   * @enum Logger::Detailed
   * Same as Normal but add also file information from where the log where written (if these informations are given).
   */
   
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
  
  /**
   * @fn static void Logger::Debug(const std::string& msg)
   * Write the message @c msg to the debug stream
   * @note Setting the verbose mode to Normal or Detailed will have the same effect using this method as the file informations (filename, line number) are not given
   */
  
  /**
   * @fn static void Logger::Warning(const std::string& msg)
   * Write the message @c msg to the warning stream.
   * @note Setting the verbose mode to Normal or Detailed will have the same effect using this method as the file informations (filename, line number) are not given.
   */
  
  /**
   * @fn static void Logger::Error(const std::string& msg)
   * Write the message @c msg to the error stream.
   * @note Setting the verbose mode to Normal or Detailed will have the same effect using this method as the file informations (filename, line number) are not given.
   */
  
  /**
   * @fn static void Logger::Debug(const std::string& filename, int line, const std::string& msg)
   * Write the message @c msg to the debug stream.
   */
  
  /**
   * @fn static void Logger::Warning(const std::string& filename, int line, const std::string& msg)
   * Write the message @c msg to the warning stream.
   */
  
  /**
   * @fn static void Logger::Error(const std::string& filename, int line, const std::string& msg)
   * Write the message @c msg to the error stream.
   */
  
  /**
   * @fn static VerboseMode Logger::GetVerboseMode()
   * Returns the current verbose mode.
   */
  
  /**
   * @fn static void Logger::SetVerboseMode(VerboseMode mode)
   * Sets the verbose mode.
   */  
  
  /**
   * @fn static const std::string& Logger::GetPrefix()
   * Returns the prefix used by the logger. The prefix should contain a string for the library or application which use the logger.
   */
  
  /**
   * @fn static void Logger::SetPrefix(const std::string& str)
   * Sets the prefix used by the logger. 
   */
  
  /**
   * @fn static Logger::Stream::Pointer Logger::GetDebugStream()
   * Returns the stream used for the debug logs.
   */
  
  /**
   * @fn static Logger::Stream::Pointer Logger::GetWarningStream()
   * Returns the stream used for the warning logs.
   */
  
  /**
   * @fn static Logger::Stream::Pointer Logger::GetErrorStream()
   * Returns the stream used for the error logs.
   */
  
  /**
   * @fn static void Logger::SetDebugStream(std::ostream* output)
   * Convenient method to create a Logger::Stream object from an output stream use for the debug messages.
   */
  
  /**
   * @fn static void Logger::SetWarningStream(std::ostream* output)
   * Convenient method to create a Logger::Stream object from an output stream use for the warning messages.
   */
  
  /**
   * @fn static void Logger::SetErrorStream(std::ostream* output)
   * Convenient method to create a Logger::Stream object from an output stream use for the error messages.
   */
  
  /**
   * @fn static void Logger::SetDebugStream(Logger::Stream::Pointer stream)
   * Sets the stream used for the debug logs.
   */
  
  /**
   * @fn static void Logger::SetWarningStream(Logger::Stream::Pointer stream)
   * Sets the stream used for the warning logs.
   */
  
  /**
   * @fn static void Logger::SetErrorStream(Logger::Stream::Pointer stream)
   * Sets the stream used for the error logs.
   */
  
  /**
   * @fn static const std::string& Logger::GetDebugAffix()
   * Gets the string used to indicate that the log is a debug message.
   */
  
  /**
   * @fn static const std::string& Logger::GetWarningAffix()
   * Gets the string used to indicate that the log is a warning.
   */
  
  /**
   * @fn static const std::string& Logger::GetErrorAffix()
   * Gets the string used to indicate that the log is an error.
   */
  
  /**
   * @fn static void Logger::SetDebugAffix(const std::string& str)
   * Sets the string used to indicate that the log is a debug message.
   */
  
  /**
   * @fn static void Logger::SetWarningAffix(const std::string& str)
   * Sets the string used to indicate that the log is a warning.
   */
  
  /**
   * @fn static void Logger::SetErrorAffix(const std::string& str)
   * Sets the string used to indicate that the log is an error.
   */
  
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
  
  /**
   * @class Logger::Stream btkLogger.h
   * @brief Helper class to store information regarding the output streams used by the class Logger.
   */
  
  /**
   * @typedef Logger::Stream::Pointer
   * Smart pointer associated with a Logger::Stream  object.
   */
  
  /**
   * @fn static Pointer Logger::Stream::New(std::ostream* output)
   * @brief Creates a smart pointer associated with a Logger::Stream object.
   */
   
  /**
   * @fn std::ostream& Logger::Stream::GetOutput() const
   * Returns the output stream as a reference.
   */
  
  /*
   * Constructor.
   */
  Logger::Stream::Stream(std::ostream* output)
  {
    this->mp_Output = output;
    this->m_Owned = false;
  };
  
  /** 
   * Destructor.
   */
  Logger::Stream::~Stream()
  {
    if (this->m_Owned)
      delete this->mp_Output;
  };
  
};
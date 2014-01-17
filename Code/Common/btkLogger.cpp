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

#include "btkLogger.h"

#include <iostream>

#ifdef NDEBUG
  static btk::Logger::VerboseMode _btk_logger_verbose_mode = btk::Logger::Normal;
#else
  static btk::Logger::VerboseMode _btk_logger_verbose_mode = btk::Logger::Detailed;
#endif
static std::string _btk_logger_prefix = "BTK";
static std::string _btk_logger_debug_affix = "DEBUG";
static std::string _btk_logger_warning_affix = "WARNING";
static std::string _btk_logger_error_affix = "ERROR";
static btk::Logger::Stream::Pointer _btk_logger_debug_stream = btk::Logger::Stream::New(&(std::cout));
static btk::Logger::Stream::Pointer _btk_logger_warning_stream = btk::Logger::Stream::New(&(std::cerr));
static btk::Logger::Stream::Pointer _btk_logger_error_stream = btk::Logger::Stream::New(&(std::cerr));

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
   * @enum Logger::VerboseMode
   * Proposed mode for the display of the log message.
   */
  /**
   * @var Logger::VerboseMode Logger::Quiet
   * Do not display any message
   */
  /**
   * @var Logger::VerboseMode Logger::MessageOnly
   * Display only the message
   */
  /**
   * @var Logger::VerboseMode Logger::Normal
   * Display the message as well as the prefix and log affix.
   */
  /**
   * @var Logger::VerboseMode Logger::Detailed
   * Same as Normal but add also file information from where the log where written (if these informations are given).
   */
  
  /**
   * Write the message @c msg to the debug stream
   * @note Setting the verbose mode to Normal or Detailed will have the same effect using this method as the file informations (filename, line number) are not given
   */
  void Logger::Debug(const std::string& msg)
  {
#ifdef NDEBUG
    btkNotUsed(msg);
#else
    Logger::PrintMessage(_btk_logger_debug_stream.get(), _btk_logger_debug_affix, msg);
#endif
  };
     
  /**
   * Write the message @c msg to the warning stream.
   * @note Setting the verbose mode to Normal or Detailed will have the same effect using this method as the file informations (filename, line number) are not given.
   */
  void Logger::Warning(const std::string& msg)
  {
    Logger::PrintMessage(_btk_logger_warning_stream.get(), _btk_logger_warning_affix, msg);
  };
    
  /**
   * Write the message @c msg to the error stream.
   * @note Setting the verbose mode to Normal or Detailed will have the same effect using this method as the file informations (filename, line number) are not given.
   */
  void Logger::Error(const std::string& msg)
  {
    Logger::PrintMessage(_btk_logger_error_stream.get(), _btk_logger_error_affix, msg);
  }
  
  /**
   * Write the message @c msg to the debug stream.
   */
  void Logger::Debug(const std::string& filename, int line, const std::string& msg)
  {
#ifdef NDEBUG
    btkNotUsed(filename); btkNotUsed(line); btkNotUsed(msg);
#else
    Logger::PrintMessage(_btk_logger_debug_stream.get(), _btk_logger_debug_affix, filename, line, msg);
#endif
  };
  
  /**
   * Write the message @c msg to the warning stream.
   */
  void Logger::Warning(const std::string& filename, int line, const std::string& msg)
  {
    Logger::PrintMessage(_btk_logger_warning_stream.get(), _btk_logger_warning_affix, filename, line, msg);
  };
  
  /**
   * Write the message @c msg to the error stream.
   */
  void Logger::Error(const std::string& filename, int line, const std::string& msg)
  {
    Logger::PrintMessage(_btk_logger_error_stream.get(), _btk_logger_error_affix, filename, line, msg);
  };
  
  /**
   * Returns the current verbose mode.
   */
  Logger::VerboseMode Logger::GetVerboseMode()
  {
    return _btk_logger_verbose_mode;
  };
    
  /**
   * Sets the verbose mode.
   */  
  void Logger::SetVerboseMode(Logger::VerboseMode mode)
  {
    _btk_logger_verbose_mode = mode;
  };
      
  /**
   * Returns the prefix used by the logger. The prefix should contain a string for the library or application which use the logger.
   */
  const std::string& Logger::GetPrefix()
  {
    return _btk_logger_prefix;
  };
    
  /**
   * Sets the prefix used by the logger. 
   */
  void Logger::SetPrefix(const std::string& str)
  {
    _btk_logger_prefix = str;
  };
    
  /**
   * Returns the stream used for the debug logs.
   */
  Logger::Stream::Pointer Logger::GetDebugStream()
  {
    return _btk_logger_debug_stream;
  };
    
  /**
   * Returns the stream used for the warning logs.
   */
  Logger::Stream::Pointer Logger::GetWarningStream()
  {
    return _btk_logger_warning_stream;
  };
    
  /**
   * Returns the stream used for the error logs.
   */
  Logger::Stream::Pointer Logger::GetErrorStream()
  {
    return _btk_logger_error_stream;
  };
    
  /**
   * Convenient method to create a Logger::Stream object from an output stream use for the debug messages.
   */
  void Logger::SetDebugStream(std::ostream* output)
  {
    Logger::SetDebugStream(Logger::Stream::New(output));
  };
    
  /**
   * Convenient method to create a Logger::Stream object from an output stream use for the warning messages.
   */
  void Logger::SetWarningStream(std::ostream* output)
  {
    Logger::SetWarningStream(Logger::Stream::New(output));
  };
    
  /**
   * Convenient method to create a Logger::Stream object from an output stream use for the error messages.
   */
  void Logger::SetErrorStream(std::ostream* output)
  {
    Logger::SetErrorStream(Logger::Stream::New(output));
  };
    
  /**
   * Sets the stream used for the debug logs.
   */
  void Logger::SetDebugStream(Logger::Stream::Pointer stream)
  {
    _btk_logger_debug_stream = stream;
  };
    
  /**
   * Sets the stream used for the warning logs.
   */
  void Logger::SetWarningStream(Logger::Stream::Pointer stream)
  {
    _btk_logger_warning_stream = stream;
  };
    
  /**
   * Sets the stream used for the error logs.
   */
  void Logger::SetErrorStream(Logger::Stream::Pointer stream)
  {
    _btk_logger_error_stream = stream;
  };
    
  /**
   * Gets the string used to indicate that the log is a debug message.
   */
  const std::string& Logger::GetDebugAffix()
  {
    return _btk_logger_debug_affix;
  };
    
  /**
   * Gets the string used to indicate that the log is a warning.
   */
  const std::string& Logger::GetWarningAffix()
  {
    return _btk_logger_warning_affix;
  };
    
  /**
   * Gets the string used to indicate that the log is an error.
   */
  const std::string& Logger::GetErrorAffix()
  {
    return _btk_logger_error_affix;
  };
    
  /**
   * Sets the string used to indicate that the log is a debug message.
   */
  void Logger::SetDebugAffix(const std::string& str)
  {
    _btk_logger_debug_affix = str;
  };
    
  /**
   * Sets the string used to indicate that the log is a warning.
   */
  void Logger::SetWarningAffix(const std::string& str)
  {
    _btk_logger_warning_affix = str;
  };
    
  /**
   * Sets the string used to indicate that the log is an error.
   */
  void Logger::SetErrorAffix(const std::string& str)
  {
    _btk_logger_error_affix = str;
  };
  
  /**
   * Overload method to print message without information on the file and the line number where the log was written.
   */
  void Logger::PrintMessage(Stream* level, const std::string& affix, const std::string& msg)
  {
    Logger::PrintMessage(level, affix, "", 0, msg);
  };
  
 /**
  * Print message on the given stream with the selected verbose mode and other parameters.
  */
  void Logger::PrintMessage(Stream* level, const std::string& affix, const std::string& filename, int line, const std::string& msg)
  {
    if (_btk_logger_verbose_mode == Logger::Quiet)
      return;
    if (_btk_logger_verbose_mode > Logger::MessageOnly)
    {
      level->GetOutput() << (_btk_logger_prefix.empty() ? "" : "[" + _btk_logger_prefix + " ")
                     << (affix.empty() ? "" : affix)
                     << (_btk_logger_prefix.empty() && affix.empty() ? "" : "] ");
    }
    if ((_btk_logger_verbose_mode == Logger::Detailed) && (!filename.empty()))
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
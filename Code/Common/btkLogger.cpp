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
#include <cstdio> // vsnprintf

namespace btk
{
  struct Logger::Private
  {
    Private() noexcept : Output(nullptr), Quiet(false) {};
    ~Private() noexcept = default;
    
    Private(const Private& ) = delete;
    Private(Private&& ) noexcept = delete;
    Private& operator=(const Private& ) = delete;
    Private& operator=(Private&& ) noexcept = delete;
    
    Device* Output;
    bool Quiet;
  };
  
  // ----------------------------------------------------------------------- //
  
  /**
   * @struct Logger::Device btkLogger.h
   * @brief Interface class used to write message sent by the logger.
   *
   * To be effective, the Logger class needs a device. This device is used to
   * write log messages.
   *
   * A default implementation used to print messages on the standard std::cout 
   * and std::cerr streams is proposed and correspond to the following lines.
   *
   * @code
   * struct Console : Logger::Device
   * {
   *   virtual void writeMessage(Logger::Category category, const char* msg) noexcept override
   *   {
   *     if (category == Logger::Info)
   *       std::cout << "INFO: " << msg << std::endl;
   *     else if (category == Logger::Warning)
   *       std::cout << "WARNING: " << msg << std::endl;
   *     else if (category == Logger::Error)
   *       std::cout << "ERROR: " << msg << std::endl;
   *   }
   * };
   * @encode
   *
   * Only one device can be used at a time. To create your custom device you only need
   * to inherit from the class Logger::Device and override the method Logger::Device::writeMessage().
   * To use this custom device with the logger, you have to use the method Device::setDevice().
   */
  
  /**
   * Default (empty) constructor
   */
  Logger::Device::Device() noexcept = default;
  
  /**
   * Default (empty) destructor
   */
  Logger::Device::~Device() noexcept = default;
  
  /**
   * @fn virtual void Logger::Device::writeMessage(Category category, const char* msg) noexcept = 0;
   * The logger seng message to this method. The @a category input specifies if the message is for
   * an information, a warning, or an error. The @a msg input is directly the string given to the method
   * Logger::info(), Logger::warning(), or Logger::error().
   */
  
  // ----------------------------------------------------------------------- //

  struct Console : Logger::Device
  {
    virtual void writeMessage(Logger::Category category, const char* msg) noexcept override
    {
      if (category == Logger::Info)
        std::cout << "INFO: " << msg << std::endl;
      else if (category == Logger::Warning)
        std::cerr << "WARNING: " << msg << std::endl;
      else if (category == Logger::Error)
        std::cerr << "ERROR: " << msg << std::endl;
    }
  };
  
  // ----------------------------------------------------------------------- //
  
  /**
   * @class Logger btkLogger.h
   * @brief Class to centralize log messages and write them to a device
   *
   * Three categories are available and could be used as proposed below:
   * - Logger::Info: For users' information or debugging
   * - Logger::Warning: For incorrect inputs in algorithms but still adatable.
   * - Logger::Error: For undesired behaviour which can break algorithms' logic.
   *
   * Each logging category has a dedicated static method:
   * - Logger::info()
   * - Logger::warning()
   * - Logger::error()
   *
   * These methods send the given message to a Logger::Device set. This device 
   * has the role to write the message (in a console,  a file, etc.). By default,
   * the message are sent into the standard cout/cerr streams. 
   * To set a device, you have to use the method Logger::setDevice()
   *
   * To disable the logger, you can use the method logger::mute().
   *
   * @ingroup BTKCommon
   */
 
  /**
   * @enum Logger::Category
   * Enums used to specify message's type.
   */
  /**
   * @var Logger::Category Logger::Info
   * Enum value used for information messages.
   */
  /**
   * @var Logger::Category Logger::Warning
   * Enum value used for warning messages.
   */
  /**
   * @var Logger::Category Logger::Error
   * Enum value used for error messages.
   */
  
  /**
   * @fn template<typename... Args> void Logger::info(const char* msg, Args&&... args) noexcept
   * Write Logger::Info messages.
   */
  
  /**
   * @fn template<typename... Args> void Logger::warning(const char* msg, Args&&... args) noexcept
   * Write Logger::Warning messages.
   */
  
  /**
   * @fn template<typename... Args> void Logger::error(const char* msg, Args&&... args) noexcept   
   * Write Logger::Error messages.
   */
  
  /**
   * Active/unactive the logger. If the logger is set to mute,
   * all the messages are eaten and destroyed.
   */
  void Logger::mute(bool active) noexcept
  {
    Logger::instance().mp_Pimpl->Quiet = active;
  };
  
  /**
   * Set the device which will write the log messages. If a previous device was set,
   * it will be deleted. The logger takes the ownership of the device.
   */
  void Logger::setDevice(Device* output) noexcept
  {
    delete Logger::instance().mp_Pimpl->Output;
    Logger::instance().mp_Pimpl->Output = output;
  };
  
  /**
   * Destructor
   * Delete the set device
   */
  Logger::~Logger() noexcept
  {
    delete this->mp_Pimpl->Output;
    delete this->mp_Pimpl;
  };
  
  /**
   * Singleton
   */
  Logger& Logger::instance()
  {
    static Logger singleton;
    return singleton;
  };
  
  /**
   * Constructor
   */
  Logger::Logger()
  : mp_Pimpl(new Logger::Private)
  {};
  
  /**
   * Return true if the logger was set to mute previously.
   */
  bool Logger::isMute() const
  {
    return Logger::instance().mp_Pimpl->Quiet;
  };
  
  /**
   * Create a string based on the given string @a msg and the variadic arguments.
   * The returned string must be deleted aftewards using the delete[] operator.
   */
  const char* Logger::prepareMessage(const char* msg, ...) const
  {
    size_t n = strlen(msg)*2;
    char* str = new char[n];
    while (1)
    {
      va_list args;
      va_start(args, msg);
      int len = vsnprintf(str, n, msg, args);
      va_end(args);
      // If something is wrong (negative length), the string is reset and sent like this
      if (len < 0)
      {
        str[0] = '\0';
        len = 0;
      }
      // If the string is complete (or an encoding error occurs)
      if (len < n)
        break;
      n += len - n + 1; // Should be done only one time. +1: null character
      delete[] str;
      str = new char[n];
    }
    return str;
  };

  /**
   * Send a message to the set device. If no device is set, a default one is created
   * and send info messages to the std::cout stream and warning and error messages to 
   * the std::cerr stream. You can set a device using the method Logger::setDevice().
   */
  void Logger::sendMessage(Category category, const char* msg)
  {
  if (this->mp_Pimpl->Output == nullptr)
    this->mp_Pimpl->Output = new Console;
  this->mp_Pimpl->Output->writeMessage(category,msg);
  };
};
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

#ifndef __btkLogger_h
#define __btkLogger_h

#include "btkBaseExport.h"
#include "btkMacros.h" // _BTK_NOEXCEPT

#include <utility> // std::forward

namespace btk
{
  class BTK_BASE_EXPORT Logger
  {
  public:
    typedef enum {Info, Warning, Error} Category;
    
    struct BTK_BASE_EXPORT Device
    {
      Device() _BTK_NOEXCEPT;
      virtual ~Device() _BTK_NOEXCEPT;
      
      virtual void writeMessage(Category category, const char* msg) _BTK_NOEXCEPT = 0;
      
      Device(const Device& ) = delete;
      Device(Device&& ) _BTK_NOEXCEPT = delete;
      Device& operator=(const Device& ) = delete;
      Device& operator=(Device&& ) _BTK_NOEXCEPT = delete;
    };
    
    template<typename... Args> static inline void info(const char* msg, Args&&... args) _BTK_NOEXCEPT;
    template<typename... Args> static inline void warning(const char* msg, Args&&... args) _BTK_NOEXCEPT;
    template<typename... Args> static inline void error(const char* msg, Args&&... args) _BTK_NOEXCEPT;
    
    static void mute(bool active) _BTK_NOEXCEPT;
    
    static void setDevice(Device* output) _BTK_NOEXCEPT;
    
    ~Logger() _BTK_NOEXCEPT;
    
    Logger(const Logger& ) = delete;
    Logger(Logger&& ) _BTK_NOEXCEPT = delete;
    Logger& operator=(const Logger& ) = delete;
    Logger& operator=(Logger&& ) _BTK_NOEXCEPT = delete;
    
  private:
    static Logger& instance();
    
    template<typename... Args> struct Message;
    
    Logger();
    void sendPreparedMessage(Category category, const char* msg, ...);
    void sendMessage(Category category, const char* msg);
    
    struct Private;
    Private* mp_Pimpl;
  };
  
  // ----------------------------------------------------------------------- //
  
  template<typename... Args>
  struct Logger::Message
  {
    static inline void send(Category category, const char* msg, Args&&... args)
    {
      Logger::instance().sendPreparedMessage(category, msg, std::forward<Args>(args)...);
    };
  };
  
  template<>
  struct Logger::Message<>
  {
    static inline void send(Category category, const char* msg)
    {
      Logger::instance().sendMessage(category, msg);
    };
  };
  
  // ----------------------------------------------------------------------- //
  
  template<typename... Args>
  void Logger::info(const char* msg, Args&&... args) _BTK_NOEXCEPT
  {
    Logger::Message<Args...>::send(Info,msg,std::forward<Args>(args)...);
  };
  
  template<typename... Args>
  void Logger::warning(const char* msg, Args&&... args) _BTK_NOEXCEPT
  {
    Logger::Message<Args...>::send(Warning,msg,std::forward<Args>(args)...);
  };
  
  template<typename... Args>
  void Logger::error(const char* msg, Args&&... args) _BTK_NOEXCEPT
  {
    Logger::Message<Args...>::send(Error,msg,std::forward<Args>(args)...);
  };
};

#endif // __btkLogger_h
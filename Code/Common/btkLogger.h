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

#ifndef __btkLogger_h
#define __btkLogger_h

#include "btkCommonExport.h"

#include <string>

namespace btk
{
  class BTK_COMMON_EXPORT Logger
  {
  public:
    typedef enum {Info, Warning, Error} Category;
    
    struct Device
    {
      Device() noexcept;
      virtual ~Device() noexcept;
      
      virtual void writeMessage(Category category, const char* msg) noexcept = 0;
      
      Device(const Device& ) = delete;
      Device(Device&& ) noexcept = delete;
      Device& operator=(const Device& ) = delete;
      Device& operator=(Device&& ) noexcept = delete;
    };
    
    static void info(const char* msg) noexcept;
    static void warning(const char* msg) noexcept;
    static void error(const char* msg) noexcept;
    
    static void mute(bool active) noexcept;
    
    static void setDevice(Device* output) noexcept;
    
    ~Logger() noexcept;
    
    Logger(const Logger& ) = delete;
    Logger(Logger&& ) noexcept = delete;
    Logger& operator=(const Logger& ) = delete;
    Logger& operator=(Logger&& ) noexcept = delete;
    
  private:
    static Logger& instance();
    
    Logger();
    void sendMessage(Category category, const char* msg) noexcept;
    
    struct Private;
    Private* mp_Pimpl;
  };
};

#endif // __btkLogger_h
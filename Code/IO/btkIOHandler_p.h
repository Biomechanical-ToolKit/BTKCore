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

#ifndef __btIOHandler_p_h
#define __btIOHandler_p_h

/*
 * WARNING: This file and its content are not included in the public API and 
 * can change drastically from one release to another.
 */

#include "btkIOHandler.h"
#include "btkMacros.h" // _BTK_NOEXCEPT

#include <string>

namespace btk
{
  class IODevice;
  
  class IOHandlerPrivate
  {
  public:
    IOHandlerPrivate();
    virtual ~IOHandlerPrivate() _BTK_NOEXCEPT;

    IOHandlerPrivate(const IOHandlerPrivate& ) = delete;
    IOHandlerPrivate(IOHandlerPrivate&& ) _BTK_NOEXCEPT = delete;
    IOHandlerPrivate& operator=(const IOHandlerPrivate& ) = delete;
    IOHandlerPrivate& operator=(const IOHandlerPrivate&& ) _BTK_NOEXCEPT = delete;
    
    virtual std::vector<const char*> availableOptions() const _BTK_NOEXCEPT = 0;
    virtual std::vector<const char*> availableOptionChoices(const char* option) const _BTK_NOEXCEPT = 0;
    
    virtual void option(const char* name, void* value) const _BTK_NOEXCEPT = 0;
    virtual void setOption(const char* name, const void* value) _BTK_NOEXCEPT = 0;
    
    IODevice* Device;
    IOHandler::Error ErrorCode;
    std::string ErrorMessage;
  };
};

#endif // __btIOHandler_p_h

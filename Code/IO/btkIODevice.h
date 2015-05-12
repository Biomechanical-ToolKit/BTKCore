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

#ifndef __btkIODevice_h
#define __btkIODevice_h

#include "btkIOExport.h"
#include "btkOpaque.h"
#include "btkException.h"

#include <memory> // std::unique_ptr
#include <ios> // std::streamsize, std::streamoff, std::streampos

namespace btk
{
  class IODevicePrivate;
  
  class BTK_IO_EXPORT IODevice
  {
    BTK_DECLARE_PIMPL_ACCESSOR(IODevice)
    
  public:
    enum class Mode : int {In = 0x01, Out = 0x02, Append = 0x04, Truncate = 0x08, End = 0x10};
    enum class Origin : int {Begin, Current, End};
    enum class State : int {End = 0x01, Fail = 0x02, Error = 0x04, Good = 0x00};
    
    using Offset = std::streamoff;
    using Position = std::streampos;
    using Size = std::streamsize;
    
    class Failure : public Exception
    {
    public:
     explicit Failure(const std::string& msg) noexcept : Exception(msg) {};
     virtual ~Failure() noexcept = default;
    };
    
    IODevice(const IODevice& ) = delete;
    IODevice(IODevice&& ) noexcept = delete;
    IODevice& operator=(const IODevice& ) = delete;
    IODevice& operator=(IODevice&& ) noexcept = delete;

    virtual ~IODevice() noexcept;
    
    // State
    bool isGood() const noexcept;
    bool atEnd() const noexcept;
    bool hasFailure() const noexcept;
    bool hasError() const noexcept;
    State state() const noexcept;
    void setState(State state);
    void clear(State state = State::Good);
    State exceptions() noexcept;
    void setExceptions(State mask);
    
    // General
    const char* name() const noexcept;
    virtual bool isOpen() const noexcept = 0;
    virtual void close() = 0;
    virtual Size peek(char* s, Size n) const = 0;
    virtual void read(char* s, Size n) = 0;
    virtual void write(const char* s, Size n) = 0;
    
    // Random access IO device only
    virtual void seek(Offset offset, Origin whence) = 0;
    virtual Position tell() const noexcept = 0;
    
    // Sequential IO only
    virtual bool isSequential() const noexcept = 0;
    
  protected:
    void setName(const char* name = nullptr);
    bool verifyMode(Mode mode);
    
    IODevice() noexcept;
    IODevice(IODevicePrivate& pimpl) noexcept;
   
    std::unique_ptr<IODevicePrivate> mp_Pimpl;
  };
  
  // ----------------------------------------------------------------------- //
  
  inline constexpr IODevice::State operator& (IODevice::State lhs, IODevice::State rhs)
  {
    return static_cast<IODevice::State>(static_cast<int>(lhs) & static_cast<int>(rhs));
  };
  
  inline constexpr IODevice::State operator| (IODevice::State lhs, IODevice::State rhs)
  {
    return static_cast<IODevice::State>(static_cast<int>(lhs) | static_cast<int>(rhs));
  };
  
  // ----------------------------------------------------------------------- //
  
  inline constexpr IODevice::Mode operator& (IODevice::Mode lhs, IODevice::Mode rhs)
  {
    return static_cast<IODevice::Mode>(static_cast<int>(lhs) & static_cast<int>(rhs));
  };

  inline constexpr IODevice::Mode operator| (IODevice::Mode lhs, IODevice::Mode rhs)
  {
    return static_cast<IODevice::Mode>(static_cast<int>(lhs) | static_cast<int>(rhs));
  };
  
  inline constexpr IODevice::Mode operator~ (IODevice::Mode x)
  {
    return IODevice::Mode(~static_cast<int>(x));
  };
};

#endif // __btkIODevice_h
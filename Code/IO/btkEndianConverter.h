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

#ifndef __btkEndianConverter_h
#define __btkEndianConverter_h

// Check if the processor is supported
#if defined _MSC_VER
  #if defined _M_IX86 || defined _M_X64
    #define PROCESSOR_TYPE 1 /* IEEE_LittleEndian */
  #elif defined _M_ALPHA
    #define PROCESSOR_TYPE 2 /* VAX_LittleEndian */
  #elif defined _M_MRX000 || defined _M_PPC 
    #define PROCESSOR_TYPE 3 /* IEEE_BigEndian */
  #else
    #error Processor not supported
  #endif
#elif defined __GNUC__
  #if defined __i386__ || defined __x86_64__
    #define PROCESSOR_TYPE 1 /* IEEE_LittleEndian */
  #elif defined __vax__
    #define PROCESSOR_TYPE 2 /* VAX_LittleEndian */
  #elif defined __mips__ || defined __ppc__
    #define PROCESSOR_TYPE 3 /* IEEE_BigEndian */
  #else
    #error Processor not supported
  #endif
#else
  #error Development platform not supported
#endif

#include "btkIOExport.h"
#include "btkEndianFormat.h"

#include <cstdint>
#include <string>

// _BTK_NATIVE_ENDIAN_FORMAT macro
#if PROCESSOR_TYPE == 1
  #define _BTK_NATIVE_ENDIAN_FORMAT IEEELittleEndianConverter
#elif PROCESSOR_TYPE == 2
  #define _BTK_NATIVE_ENDIAN_FORMAT VAXLittleEndianConverter
#elif PROCESSOR_TYPE == 3
  #define _BTK_NATIVE_ENDIAN_FORMAT IEEEBigEndianConverter
#else
  #error Unknown processor type
#endif


namespace btk
{
  class IODevice;
  
  class BTK_IO_EXPORT EndianConverter
  {
  public:
    virtual ~EndianConverter() = default;
    
    EndianConverter(const EndianConverter& ) = delete;
    EndianConverter(EndianConverter&& ) noexcept = delete;
    EndianConverter& operator=(const EndianConverter& ) = delete;
    EndianConverter& operator=(EndianConverter&& ) noexcept = delete;
    
    virtual EndianFormat format() const noexcept = 0;
    
    char readChar(IODevice* src) const;
    int8_t readI8(IODevice* src) const;
    uint8_t readU8(IODevice* src) const;
    virtual int16_t readI16(IODevice* src) const = 0;
    virtual uint16_t readU16(IODevice* src) const = 0;
    virtual int32_t readI32(IODevice* src) const = 0;
    virtual uint32_t readU32(IODevice* src) const = 0;
    virtual int64_t readI64(IODevice* src) const = 0;
    virtual uint64_t readU64(IODevice* src) const = 0;
    virtual float readFloat(IODevice* src) const = 0;
    virtual double readDouble(IODevice* src) const = 0;
    std::string readString(size_t len, IODevice* src) const;
    
    void writeChar(char val, IODevice* dest) const;
    void writeI8(int8_t val, IODevice* dest) const;
    void writeU8(uint8_t val, IODevice* dest) const;
    virtual void writeI16(int16_t val, IODevice* dest) const = 0;
    virtual void writeU16(uint16_t val, IODevice* dest) const = 0;
    virtual void writeI32(int32_t val, IODevice* dest) const = 0;
    virtual void writeU32(uint32_t val, IODevice* dest) const = 0;
    virtual void writeFloat(float val, IODevice* dest) const = 0;
    void writeString(const std::string& val, IODevice* dest) const;
    /* TODO
    virtual void writeI64(int64_t val, IODevice* dest) const = 0;
    virtual void writeU64(uint64_t val, IODevice* dest) const = 0;
    virtual void writeDouble(double val, IODevice* dest) const = 0;
     */
    
  protected:
    EndianConverter();
  };
  
  class BTK_IO_EXPORT VAXLittleEndianConverter : public EndianConverter
  {
  public:
    VAXLittleEndianConverter();
    ~VAXLittleEndianConverter() = default;
    
    VAXLittleEndianConverter(const VAXLittleEndianConverter& ) = delete;
    VAXLittleEndianConverter(VAXLittleEndianConverter&& ) noexcept = delete;
    VAXLittleEndianConverter& operator=(const VAXLittleEndianConverter& ) = delete;
    VAXLittleEndianConverter& operator=(VAXLittleEndianConverter&& ) noexcept = delete;
    
    virtual EndianFormat format() const noexcept;
    
    virtual int16_t readI16(IODevice* src) const;
    virtual uint16_t readU16(IODevice* src) const;
    virtual int32_t readI32(IODevice* src) const;
    virtual uint32_t readU32(IODevice* src) const;
    virtual int64_t readI64(IODevice* src) const;
    virtual uint64_t readU64(IODevice* src) const;
    virtual float readFloat(IODevice* src) const;
    virtual double readDouble(IODevice* src) const;
  
    virtual void writeI16(int16_t val, IODevice* dest) const;
    virtual void writeU16(uint16_t val, IODevice* dest) const;
    virtual void writeI32(int32_t val, IODevice* dest) const;
    virtual void writeU32(uint32_t val, IODevice* dest) const;
    virtual void writeFloat(float val, IODevice* dest) const;
  };

  class BTK_IO_EXPORT IEEELittleEndianConverter : public EndianConverter
  {
  public:
    IEEELittleEndianConverter();
    ~IEEELittleEndianConverter() = default;
    
    IEEELittleEndianConverter(const IEEELittleEndianConverter& ) = delete;
    IEEELittleEndianConverter(IEEELittleEndianConverter&& ) noexcept = delete;
    IEEELittleEndianConverter& operator=(const IEEELittleEndianConverter& ) = delete;
    IEEELittleEndianConverter& operator=(IEEELittleEndianConverter&& ) noexcept = delete;
    
    virtual EndianFormat format() const noexcept;
    
    virtual int16_t readI16(IODevice* src) const;
    virtual uint16_t readU16(IODevice* src) const;
    virtual int32_t readI32(IODevice* src) const;
    virtual uint32_t readU32(IODevice* src) const;
    virtual int64_t readI64(IODevice* src) const;
    virtual uint64_t readU64(IODevice* src) const;
    virtual float readFloat(IODevice* src) const;
    virtual double readDouble(IODevice* src) const;
  
    virtual void writeI16(int16_t val, IODevice* dest) const;
    virtual void writeU16(uint16_t val, IODevice* dest) const;
    virtual void writeI32(int32_t val, IODevice* dest) const;
    virtual void writeU32(uint32_t val, IODevice* dest) const;
    virtual void writeFloat(float val, IODevice* dest) const;
  };

  class BTK_IO_EXPORT IEEEBigEndianConverter : public EndianConverter
  {
  public:
    IEEEBigEndianConverter();
    ~IEEEBigEndianConverter() = default;
    
    IEEEBigEndianConverter(const IEEEBigEndianConverter& ) = delete;
    IEEEBigEndianConverter(IEEEBigEndianConverter&& ) noexcept = delete;
    IEEEBigEndianConverter& operator=(const IEEEBigEndianConverter& ) = delete;
    IEEEBigEndianConverter& operator=(IEEEBigEndianConverter&& ) noexcept = delete;
    
    virtual EndianFormat format() const noexcept;
    
    virtual int16_t readI16(IODevice* src) const;
    virtual uint16_t readU16(IODevice* src) const;
    virtual int32_t readI32(IODevice* src) const;
    virtual uint32_t readU32(IODevice* src) const;
    virtual int64_t readI64(IODevice* src) const;
    virtual uint64_t readU64(IODevice* src) const;
    virtual float readFloat(IODevice* src) const;
    virtual double readDouble(IODevice* src) const;
  
    virtual void writeI16(int16_t val, IODevice* dest) const;
    virtual void writeU16(uint16_t val, IODevice* dest) const;
    virtual void writeI32(int32_t val, IODevice* dest) const;
    virtual void writeU32(uint32_t val, IODevice* dest) const;
    virtual void writeFloat(float val, IODevice* dest) const;
  };
  
  class BTK_IO_EXPORT NativeEndian : public _BTK_NATIVE_ENDIAN_FORMAT
  {
  public:
    NativeEndian() : _BTK_NATIVE_ENDIAN_FORMAT() {};
    ~NativeEndian() = default;
    
    NativeEndian(const NativeEndian& ) = delete;
    NativeEndian(NativeEndian&& ) noexcept = delete;
    NativeEndian& operator=(const NativeEndian& ) = delete;
    NativeEndian& operator=(NativeEndian&& ) noexcept = delete;
  };
};

#endif // __btkEndianConverter_h
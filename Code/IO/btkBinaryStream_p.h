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

#ifndef __btkBinaryStream_p_h
#define __btkBinaryStream_p_h

/*
 * WARNING: This file and its content are not included in the public API and 
 * can change drastically from one release to another.
 */

#include <cstdint>
#include <string>

namespace btk
{
  class IODevice;
  class EndianConverter;
  enum class EndianFormat;
  
  class BinaryStreamPrivate
  {
  public:
    BinaryStreamPrivate(IODevice* device, EndianConverter* converter);
    virtual ~BinaryStreamPrivate() noexcept;
    
    BinaryStreamPrivate(const BinaryStreamPrivate& ) = delete;
    BinaryStreamPrivate(BinaryStreamPrivate&& ) noexcept = delete;
    BinaryStreamPrivate& operator=(const BinaryStreamPrivate& ) = delete;
    BinaryStreamPrivate& operator=(const BinaryStreamPrivate&& ) noexcept = delete;
    
    IODevice* Device;
    EndianConverter* Converter;
  };
  
  // ----------------------------------------------------------------------- //
  
  class EndianConverter
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
  
  class VAXLittleEndianConverter : public EndianConverter
  {
  public:
    VAXLittleEndianConverter();
    ~VAXLittleEndianConverter() = default;
    
    VAXLittleEndianConverter(const VAXLittleEndianConverter& ) = delete;
    VAXLittleEndianConverter(VAXLittleEndianConverter&& ) noexcept = delete;
    VAXLittleEndianConverter& operator=(const VAXLittleEndianConverter& ) = delete;
    VAXLittleEndianConverter& operator=(VAXLittleEndianConverter&& ) noexcept = delete;
    
    virtual EndianFormat format() const noexcept final;
    
    virtual int16_t readI16(IODevice* src) const final;
    virtual uint16_t readU16(IODevice* src) const final;
    virtual int32_t readI32(IODevice* src) const final;
    virtual uint32_t readU32(IODevice* src) const final;
    virtual int64_t readI64(IODevice* src) const final;
    virtual uint64_t readU64(IODevice* src) const final;
    virtual float readFloat(IODevice* src) const final;
    virtual double readDouble(IODevice* src) const final;
  
    virtual void writeI16(int16_t val, IODevice* dest) const final;
    virtual void writeU16(uint16_t val, IODevice* dest) const final;
    virtual void writeI32(int32_t val, IODevice* dest) const final;
    virtual void writeU32(uint32_t val, IODevice* dest) const final;
    virtual void writeFloat(float val, IODevice* dest) const final;
  };

  class IEEELittleEndianConverter : public EndianConverter
  {
  public:
    IEEELittleEndianConverter();
    ~IEEELittleEndianConverter() = default;
    
    IEEELittleEndianConverter(const IEEELittleEndianConverter& ) = delete;
    IEEELittleEndianConverter(IEEELittleEndianConverter&& ) noexcept = delete;
    IEEELittleEndianConverter& operator=(const IEEELittleEndianConverter& ) = delete;
    IEEELittleEndianConverter& operator=(IEEELittleEndianConverter&& ) noexcept = delete;
    
    virtual EndianFormat format() const noexcept final;
    
    virtual int16_t readI16(IODevice* src) const final;
    virtual uint16_t readU16(IODevice* src) const final;
    virtual int32_t readI32(IODevice* src) const final;
    virtual uint32_t readU32(IODevice* src) const final;
    virtual int64_t readI64(IODevice* src) const final;
    virtual uint64_t readU64(IODevice* src) const final;
    virtual float readFloat(IODevice* src) const final;
    virtual double readDouble(IODevice* src) const final;
  
    virtual void writeI16(int16_t val, IODevice* dest) const final;
    virtual void writeU16(uint16_t val, IODevice* dest) const final;
    virtual void writeI32(int32_t val, IODevice* dest) const final;
    virtual void writeU32(uint32_t val, IODevice* dest) const final;
    virtual void writeFloat(float val, IODevice* dest) const final;
  };

  class IEEEBigEndianConverter : public EndianConverter
  {
  public:
    IEEEBigEndianConverter();
    ~IEEEBigEndianConverter() = default;
    
    IEEEBigEndianConverter(const IEEEBigEndianConverter& ) = delete;
    IEEEBigEndianConverter(IEEEBigEndianConverter&& ) noexcept = delete;
    IEEEBigEndianConverter& operator=(const IEEEBigEndianConverter& ) = delete;
    IEEEBigEndianConverter& operator=(IEEEBigEndianConverter&& ) noexcept = delete;
    
    virtual EndianFormat format() const noexcept final;
    
    virtual int16_t readI16(IODevice* src) const final;
    virtual uint16_t readU16(IODevice* src) const final;
    virtual int32_t readI32(IODevice* src) const final;
    virtual uint32_t readU32(IODevice* src) const final;
    virtual int64_t readI64(IODevice* src) const final;
    virtual uint64_t readU64(IODevice* src) const final;
    virtual float readFloat(IODevice* src) const final;
    virtual double readDouble(IODevice* src) const final;
  
    virtual void writeI16(int16_t val, IODevice* dest) const final;
    virtual void writeU16(uint16_t val, IODevice* dest) const final;
    virtual void writeI32(int32_t val, IODevice* dest) const final;
    virtual void writeU32(uint32_t val, IODevice* dest) const final;
    virtual void writeFloat(float val, IODevice* dest) const final;
  };
};

#endif // __btkBinaryStream_p_h

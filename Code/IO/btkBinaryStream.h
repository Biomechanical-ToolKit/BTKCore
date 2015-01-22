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

#ifndef __btkBinaryStream_h
#define __btkBinaryStream_h

#include "btkIOExport.h"
#include "btkEndianFormat.h"
#include "btkOpaque.h"

#include <cstdint>
#include <string>
#include <memory> // std::unique_ptr

namespace btk
{
  class BinaryStreamPrivate;
  
  class IODevice;
  class EndianConverter;
  
  class BTK_IO_EXPORT BinaryStream
  {
    BTK_DECLARE_PIMPL_ACCESSOR(BinaryStream)
    
  public:
    BinaryStream(IODevice* device = 0);
    BinaryStream(IODevice* device, EndianFormat format);
    ~BinaryStream() noexcept;
    
    BinaryStream(const BinaryStream& ) = delete;
    BinaryStream(BinaryStream&& ) noexcept = delete;
    BinaryStream& operator=(const BinaryStream& ) = delete;
    BinaryStream& operator=(BinaryStream&& ) noexcept = delete;
    
    IODevice* device() const noexcept;
    void setDevice(IODevice* device) noexcept;
    
    EndianFormat endianFormat() const noexcept;
    void setEndianFormat(EndianFormat format);
    
    char readChar();
    void readChar(size_t n, char* values);
    
    int8_t readI8();
    void readI8(size_t n, int8_t* values);
    
    uint8_t readU8();
    void readU8(size_t n, uint8_t* values);
    
    int16_t readI16();
    void readI16(size_t n, int16_t* values);
    
    uint16_t readU16();
    void readU16(size_t n, uint16_t* values);
    
    int32_t readI32();
    void readI32(size_t n, int32_t* values);
    
    uint32_t readU32();
    void readU32(size_t n, uint32_t* values);
    
    int64_t readI64();
    void readI64(size_t n, int64_t* values);
    
    uint64_t readU64();
    void readU64(size_t n, uint64_t* values);
    
    float readFloat();
    void readFloat(size_t n, float* values);
    
    double readDouble();
    void readDouble(size_t n, double* values);
    
    std::string readString(size_t len);
    void readString(size_t len, size_t n, std::string* values);
    
    size_t writeChar(char value);
    size_t writeChar(size_t n, const char* values);
    
    size_t writeI8(int8_t value);
    size_t writeI8(size_t n, const int8_t* values);
    
    size_t writeU8(uint8_t value);
    size_t writeU8(size_t n, const uint8_t* values);
    
    size_t writeI16(int16_t value);
    size_t writeI16(size_t n, const int16_t* values);
    
    size_t writeU16(uint16_t value);
    size_t writeU16(size_t n, const uint16_t* values);
    
    size_t writeI32(int32_t value);
    size_t writeI32(size_t n, const int32_t* values);
    
    size_t writeU32(uint32_t value);
    size_t writeU32(size_t n, const uint32_t* values);
    
    size_t writeFloat(float value);
    size_t writeFloat(size_t n, const float* values);
    
    size_t writeString(const std::string& value);
    size_t writeString(size_t n, const std::string* values);
    
  private:
    std::unique_ptr<BinaryStreamPrivate> mp_Pimpl;
  };
};

#endif // __btkBinaryStream_h
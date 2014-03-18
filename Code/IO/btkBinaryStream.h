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

#ifndef __btkBinaryStream_h
#define __btkBinaryStream_h

#include <string>
#include <vector>

// MSVC doesn't have the header stdint.h
#ifdef _MSC_VER
  #include "Utilities/stdint.h"
#else
  #include <stdint.h>
#endif

namespace btk
{
  template <class Derived>
  class BinaryStream
  {
  public:
    
    // Read methods
    // ------------
    
    void ReadChar(size_t nb, char* values);
    void ReadChar(std::vector<char>& values);
    std::vector<char> ReadChar(size_t nb);
    
    void ReadI8(size_t nb, int8_t* values);
    void ReadI8(std::vector<int8_t>& values);
    std::vector<int8_t> ReadI8(size_t nb);
    
    void ReadU8(size_t nb, uint8_t* values);
    void ReadU8(std::vector<uint8_t>& values);
    std::vector<uint8_t> ReadU8(size_t nb);
    
    void ReadI16(size_t nb, int16_t* values);
    void ReadI16(std::vector<int16_t>& values);
    std::vector<int16_t> ReadI16(size_t nb);
    
    void ReadU16(size_t nb, uint16_t* values);
    void ReadU16(std::vector<uint16_t>& values);
    std::vector<uint16_t> ReadU16(size_t nb);
    
    void ReadI32(size_t nb, int32_t* values);
    void ReadI32(std::vector<int32_t>& values);
    std::vector<int32_t> ReadI32(size_t nb);
    
    void ReadU32(size_t nb, uint32_t* values);
    void ReadU32(std::vector<uint32_t>& values);
    std::vector<uint32_t> ReadU32(size_t nb);
    
    void ReadI64(size_t nb, int64_t* values);
    void ReadI64(std::vector<int64_t>& values);
    std::vector<int64_t> ReadI64(size_t nb);
    
    void ReadU64(size_t nb, uint64_t* values);
    void ReadU64(std::vector<uint64_t>& values);
    std::vector<uint64_t> ReadU64(size_t nb);
    
    void ReadFloat(size_t nb, float* values);
    void ReadFloat(std::vector<float>& values);
    std::vector<float> ReadFloat(size_t nb);
    
    void ReadDouble(size_t nb, double* values);
    void ReadDouble(std::vector<double>& values);
    std::vector<double> ReadDouble(size_t nb);
    
    void ReadString(size_t nb, size_t nbChar, std::string* values);
    void ReadString(size_t nbChar, std::vector<std::string>& values);
    std::vector<std::string> ReadString(size_t nb, size_t nbChar);
    
    // Write methods
    // ------------
    
    // Note: MSVC doesn't like the following commented methods.
    //       char and int8_t are the same for it...
    //virtual size_t Write(const std::vector<char>& values);
    
    size_t Write(const std::vector<int8_t>& values);
    
    size_t Write(const std::vector<uint8_t>& values);
    
    size_t Write(const std::vector<int16_t>& values);
    
    size_t Write(const std::vector<uint16_t>& values);
    
    size_t Write(const std::vector<int32_t>& values);
    
    size_t Write(const std::vector<uint32_t>& values);
    
    size_t Write(const std::vector<float>& values);
    
    size_t Write(const std::vector<std::string>& values);
  
  protected:
    BinaryStream() {};

  private:
    BinaryStream(const BinaryStream& ); // Not implemented.
    BinaryStream& operator=(const BinaryStream& ); // Not implemented.
  };
};

#include "btkBinaryStream.tpp"

#endif // __btkBinaryStream_h

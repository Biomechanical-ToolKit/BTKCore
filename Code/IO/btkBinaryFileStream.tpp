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

#include "btkBinaryFileStream.h"

namespace btk
{
  /** 
   * @class ByteOrderBinaryFileStream btkBinaryFile.h
   * @brief Template class to read and write binary file encoded from any support byte order format.
   * @tparam Format Format of the byte order to use to read/write data.
   *
   * @sa BinaryFileStream, IEEELittleEndianBinaryFileStream, IEEEBigEndianBinaryFileStream, NativeBinaryFileStream
   */
  
  /**
   * @fn template <class Format> ByteOrderBinaryFileStream<Format>::ByteOrderBinaryFileStream()
    * Creates a new default binary file stream.
   */
   
  /**
   * @fn ByteOrderBinaryFileStream<Format>::ByteOrderBinaryFileStream(const std::string& filename, OpenMode mode)
   * Creates a new binary file stream and associates the file with the filename @a filename using the option @a mode.
   *
   * If the opening is not successfull, then the FailBit is set. You can check its state by using the method Fail().
   */
  
  /** 
   * Extracts one signed 16-bit integer.
   */
  template <class Format>
  int16_t ByteOrderBinaryFileStream<Format>::ReadI16()
  {
    return Format::ReadI16(this->mp_Stream);
  };
  
  /** 
   * Extracts one unsigned 16-bit integer.
   */
  template <class Format>
  uint16_t ByteOrderBinaryFileStream<Format>::ReadU16()
  {
    return Format::ReadU16(this->mp_Stream);
  };
  
  /** 
   * Extracts one signed 32-bit integer.
   */
  template <class Format>
  int32_t ByteOrderBinaryFileStream<Format>::ReadI32()
  {
    return Format::ReadI32(this->mp_Stream);
  };
  
  /** 
   * Extracts one unsigned 32-bit integer.
   */
  template <class Format>
  uint32_t ByteOrderBinaryFileStream<Format>::ReadU32()
  {
    return Format::ReadU32(this->mp_Stream);
  };
  
  /** 
   * Extracts one signed 64-bit integer.
   */
  template <class Format>
  int64_t ByteOrderBinaryFileStream<Format>::ReadI64()
  {
    return Format::ReadI64(this->mp_Stream);
  };
  
  /** 
   * Extracts one unsigned 64-bit integer.
   */
  template <class Format>
  uint64_t ByteOrderBinaryFileStream<Format>::ReadU64()
  {
    return Format::ReadU64(this->mp_Stream);
  };
  
  /** 
   * Extracts one float.
   */
  template <class Format>
  float ByteOrderBinaryFileStream<Format>::ReadFloat()
  {
    return Format::ReadFloat(this->mp_Stream);
  };
  
  /** 
   * Extracts one double.
   */
  template <class Format>
  double ByteOrderBinaryFileStream<Format>::ReadDouble()
  {
    return Format::ReadDouble(this->mp_Stream);
  };
  
  /**
   * Writes the signed 16-bit integer @a i16 in the stream an return its size.
   */
  template <class Format>
  size_t ByteOrderBinaryFileStream<Format>::Write(int16_t value)
  {
    Format::Write(value, this->mp_Stream);
    return 2;
  };
  
  /**
   * Writes the unsigned 16-bit integer @a u16 in the stream an return its size.
   */
  template <class Format>
  size_t ByteOrderBinaryFileStream<Format>::Write(uint16_t value)
  {
    Format::Write(value, this->mp_Stream);
    return 2;
  };
  
  /** 
   * Write the 32-bit signed integer @a i32 and return its size.
   */
  template <class Format>
  size_t ByteOrderBinaryFileStream<Format>::Write(int32_t value)
  {
    Format::Write(value, this->mp_Stream);
    return 4;
  };
  
  /** 
   * Write the 32-bit unsigned integer @a u32 and return its size
   */
  template <class Format>
  size_t ByteOrderBinaryFileStream<Format>::Write(uint32_t value)
  {
    Format::Write(value, this->mp_Stream);
    return 4;
  };
  
  /**
   * Writes the float @a f in the stream an return its size.
   */
  template <class Format>
  size_t ByteOrderBinaryFileStream<Format>::Write(float value)
  {
    Format::Write(value, this->mp_Stream);
    return 4;
  };
 
};
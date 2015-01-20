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

#include "btkBinaryStream.h"
#include "btkBinaryStream_p.h"
#include "btkEndianConverter.h"
#include "btkException.h"
#include "btkLogger.h"

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

namespace btk
{
  BinaryStreamPrivate::BinaryStreamPrivate(IODevice* device, EndianConverter* converter)
  : Device(device), Converter(converter)
  {};
  
  BinaryStreamPrivate::~BinaryStreamPrivate() = default; // Cannot be inlined
}

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

namespace btk
{
  /**
   * @class BinaryStream btkBinaryStream.h
   * @brief Read/write binary data from any IODevice.
   *
   * Be sure that the lifetime of the device is longer than the stream. There is no check inside the stream to verify the existence of the device.
   */
  
  /**
   * Constructor which uses the native byte order format (Endianness) of the current machine.
   * In case no device is given, you must use the method setDevice(), otherwise the stream will try to read/write data from/to a null device. 
   */
  BinaryStream::BinaryStream(IODevice* device)
  : mp_Pimpl(new BinaryStreamPrivate(device,new NativeEndian))
  {};
  
  /**
   * Constructor which uses the given endian converter and return extracted values adapted to the endiannes of the current machine. 
   */
  BinaryStream::BinaryStream(IODevice* device, EndianFormat convert)
  : mp_Pimpl(new BinaryStreamPrivate(device,nullptr))
  {
    this->setEndianFormat(convert);
  };
   
  /**
   * Destructor. Delete the converter associated with the stream
   */
  BinaryStream::~BinaryStream()
  {
    delete this->mp_Pimpl->Converter;
  };
  
  /**
   * Returns the device associated to this stream. 
   */
  IODevice* BinaryStream::device() const noexcept
  {
    auto optr = this->pimpl();
    return optr->Device;
  };
  
  /**
   * Sets the device associated to this stream.
   * Trying to assign a null device will print an error message and returns.
   */
  void BinaryStream::setDevice(IODevice* device) noexcept
  {
    if (device == 0)
    {
      Logger::error("It is not possible to assign a null device with a stream.");
      return;
    }
    auto optr = this->pimpl();
    optr->Device = device;
  };
    
  /**
   * Return the format of current endian converter used by the stream.
   */
  EndianFormat BinaryStream::endianFormat() const noexcept
  {
    auto optr = this->pimpl();
    return optr->Converter->format();
  };
  
  /**
   * Sets the format of the endian converter used by the stream.
   */
  void BinaryStream::setEndianFormat(EndianFormat format)
  {
    auto optr = this->pimpl();
    if (optr->Converter != 0)
    {  
      if (optr->Converter->format() == format)
        return;
      delete optr->Converter;
    }
    switch (format)
    {
    case VAXLittleEndian:
      optr->Converter = new VAXLittleEndianConverter;
      break;
    case IEEELittleEndian:
      optr->Converter = new IEEELittleEndianConverter;
      break;
    case IEEEBigEndian:
      optr->Converter = new IEEEBigEndianConverter;
      break;
    default: // Should be impossible
      throw(LogicError("Unknown endian converter."));
    }
  };
  
  /** 
   * Extracts one character from the stream.
   */
  char BinaryStream::readChar()
  {
    auto optr = this->pimpl();
    return optr->Converter->readChar(optr->Device);
  };
  
  /** 
   * Extracts @a n characters and assign them into the array @a values.
   * @note The array @a values must be already created with at least @a n elements allocated;
   */
  void BinaryStream::readChar(size_t n, char* values)
  {
    for (size_t i = 0 ; i < n ; ++i)
      values[i] = this->readChar();
  };

  /** 
   * Extracts one signed 8-bit integer.
   */
  int8_t BinaryStream::readI8()
  {
    auto optr = this->pimpl();
    return optr->Converter->readI8(optr->Device);
  };
  
  /**
   * Extracts @a n signed 8-bit integers and assign them into the array @a values.
   * @note The array @a values must be already created with at least @a n elements allocated;
   */
  void BinaryStream::readI8(size_t n, int8_t* values)
  {
    for (size_t i = 0 ; i < n ; ++i)
      values[i] = this->readI8();
  };
 
  /** 
   * Extracts one unsigned 8-bit integer.
   */
  uint8_t BinaryStream::readU8()
  {
    auto optr = this->pimpl();
    return optr->Converter->readU8(optr->Device);
  };

  /**
   * Extracts @a n unsigned 8-bit integers and assign them into the array @a values.
   * @note The array @a values must be already created with at least @a n elements allocated;
   */
  void BinaryStream::readU8(size_t n, uint8_t* values)
  {
    for (size_t i = 0 ; i < n ; ++i)
      values[i] = this->readU8();
  };
  
  /** 
   * Extracts one signed 16-bit integer.
   */
  int16_t BinaryStream::readI16()
  {
    auto optr = this->pimpl();
    return optr->Converter->readI16(optr->Device);
  };
  
  /**
   * Extracts @a n signed 16-bit integers and assign them into the array @a values.
   * @note The array @a values must be already created with at least @a n elements allocated;
   */
  void BinaryStream::readI16(size_t n, int16_t* values)
  {
    for (size_t i = 0 ; i < n ; ++i)
      values[i] = this->readI16();
  };
  
  /** 
   * Extracts one unsigned 16-bit integer.
   */
  uint16_t BinaryStream::readU16()
  {
    auto optr = this->pimpl();
    return optr->Converter->readU16(optr->Device);
  };
  
  /**
   * Extracts @a n unsigned 16-bit integers and assign them into the array @a values.
   * @note The array @a values must be already created with at least @a n elements allocated;
   */
  void BinaryStream::readU16(size_t n, uint16_t* values)
  {
    for (size_t i = 0 ; i < n ; ++i)
      values[i] = this->readU16();
  };
  
  /** 
   * Extracts one signed 32-bit integer.
   */
  int32_t BinaryStream::readI32()
  {
    auto optr = this->pimpl();
    return optr->Converter->readI32(optr->Device);
  };
  
  /**
   * Extracts @a n signed 32-bit integers and assign them into the array @a values.
   * @note The array @a values must be already created with at least @a n elements allocated;
   */
  void BinaryStream::readI32(size_t n, int32_t* values)
  {
    for (size_t i = 0 ; i < n ; ++i)
      values[i] = this->readI32();
  };
  
  /** 
   * Extracts one unsigned 32-bit integer.
   */
  uint32_t BinaryStream::readU32()
  {
    auto optr = this->pimpl();
    return optr->Converter->readU32(optr->Device);
  };
  
  /**
   * Extracts @a n unsigned 32-bit integers and assign them into the array @a values.
   * @note The array @a values must be already created with at least @a n elements allocated;
   */
  void BinaryStream::readU32(size_t n, uint32_t* values)
  {
    for (size_t i = 0 ; i < n ; ++i)
      values[i] = this->readU32();
  };
  
  /** 
   * Extracts one signed 64-bit integer.
   */
  int64_t BinaryStream::readI64()
  {
    auto optr = this->pimpl();
    return optr->Converter->readI64(optr->Device);
  };
  
  /**
   * Extracts @a n signed 64-bit integers and assign them into the array @a values.
   * @note The array @a values must be already created with at least @a n elements allocated;
   */
  void BinaryStream::readI64(size_t n, int64_t* values)
  {
    for (size_t i = 0 ; i < n ; ++i)
      values[i] = this->readI64();
  };
  
  /** 
   * Extracts one unsigned 64-bit integer.
   */
  uint64_t BinaryStream::readU64()
  {
    auto optr = this->pimpl();
    return optr->Converter->readU64(optr->Device);
  };
  
  /**
   * Extracts @a n unsigned 64-bit integers and assign them into the array @a values.
   * @note The array @a values must be already created with at least @a n elements allocated;
   */
  void BinaryStream::readU64(size_t n, uint64_t* values)
  {
    for (size_t i = 0 ; i < n ; ++i)
      values[i] = this->readU64();
  };
  
  /** 
   * Extracts one float.
   */
  float BinaryStream::readFloat()
  {
    auto optr = this->pimpl();
    return optr->Converter->readFloat(optr->Device);
  };
  
  /**
   * Extracts @a n floats and assign them into the array @a values.
   * @note The array @a values must be already created with at least @a n elements allocated;
   */
  void BinaryStream::readFloat(size_t n, float* values)
  {
    for (size_t i = 0 ; i < n ; ++i)
      values[i] = this->readFloat();
  };
  
  /** 
   * Extracts one double.
   */
  double BinaryStream::readDouble()
  {
    auto optr = this->pimpl();
    return optr->Converter->readDouble(optr->Device);
  };
  
  /**
   * Extracts @a n doubles and assign them into the array @a values.
   * @note The array @a values must be already created with at least @a n elements allocated;
   */
  void BinaryStream::readDouble(size_t n, double* values)
  {
    for (size_t i = 0 ; i < n ; ++i)
      values[i] = this->readDouble();
  };
   
  /** 
   * Extracts one string.
   */
  std::string BinaryStream::readString(size_t len)
  {
    auto optr = this->pimpl();
    return optr->Converter->readString(len, optr->Device);
  };
  
  /**
   * Extracts @a n unsigned 8-bit integers and assign them into the array @a values.
   * @note The array @a values must be already created with at least @a n elements allocated;
   */
  void BinaryStream::readString(size_t len, size_t n, std::string* values)
  {
    for (size_t i = 0 ; i < n ; ++i)
      values[i] = this->readString(len);
  };
  
  /** 
   * Writes the character @a value in the stream an return its size.
   */
  size_t BinaryStream::writeChar(char value)
  {
    auto optr = this->pimpl();
    optr->Converter->writeChar(value, optr->Device);
    return 1;
  };
  
  /** 
   * Writes the array of characters @a values in the stream an return its size.
   * @note The array @a values must contain at least @a n elements initialized;
   */
  size_t BinaryStream::writeChar(size_t n, const char* values)
  {
    size_t inc = 0;
    while (inc < n)
      this->writeChar(values[inc++]);
    return n * 1;
  };
  
  /** 
   * Writes the signed 8-bit integer @a value in the stream an return its size.
   */
  size_t BinaryStream::writeI8(int8_t value)
  {
    auto optr = this->pimpl();
    optr->Converter->writeI8(value, optr->Device);
    return 1;
  };
  
  /** 
   * Writes the array of signed 8-bit integers @a values in the stream an return its size.
   * @note The array @a values must contain at least @a n elements initialized;
   */
  size_t BinaryStream::writeI8(size_t n, const int8_t* values)
  {
    size_t inc = 0;
    while (inc < n)
      this->writeI8(values[inc++]);
    return n * 1;
  };
 
  /** 
   * Writes the unsigned 8-bit integer @a value in the stream an return its size.
   */
  size_t BinaryStream::writeU8(uint8_t value)
  {
    auto optr = this->pimpl();
    optr->Converter->writeU8(value, optr->Device);
    return 1;
  };
   
  /** 
   * Writes the array of unsigned 8-bit integers @a values in the stream an return its size.
   * @note The array @a values must contain at least @a n elements initialized;
   */
  size_t BinaryStream::writeU8(size_t n, const uint8_t* values)
  {
    size_t inc = 0;
    while (inc < n)
      this->writeU8(values[inc++]);
    return n * 1;
  };

  /**
   * Writes the signed 16-bit integer @a i16 in the stream an return its size.
   */
  size_t BinaryStream::writeI16(int16_t value)
  {
    auto optr = this->pimpl();
    optr->Converter->writeI16(value, optr->Device);
    return 2;
  };

  /** 
   * Writes the array of signed 16-bit integers @a values in the stream an return its size.
   * @note The array @a values must contain at least @a n elements initialized;
   */
  size_t BinaryStream::writeI16(size_t n, const int16_t* values)
  {
    size_t inc = 0;
    while (inc < n)
      this->writeI16(values[inc++]);
    return n * 2;
  };
  
  /**
   * Writes the unsigned 16-bit integer @a u16 in the stream an return its size.
   */
  size_t BinaryStream::writeU16(uint16_t value)
  {
    auto optr = this->pimpl();
    optr->Converter->writeU16(value, optr->Device);
    return 2;
  };
  
  /** 
   * Writes the array of unsigned 16-bit integers @a values in the stream an return its size.
   * @note The array @a values must contain at least @a n elements initialized;
   */
  size_t BinaryStream::writeU16(size_t n, const uint16_t* values)
  {
    size_t inc = 0;
    while (inc < n)
      this->writeU16(values[inc++]);
    return n * 2;
  };
  
  /** 
   * Write the 32-bit signed integer @a i32 and return its size.
   */
  size_t BinaryStream::writeI32(int32_t value)
  {
    auto optr = this->pimpl();
    optr->Converter->writeI32(value, optr->Device);
    return 4;
  };
  
  /**
   * Writes the array of signed 32-bit integers @a values in the stream an return its size.
   * @note The array @a values must contain at least @a n elements initialized;
   */
  size_t BinaryStream::writeI32(size_t n, const int32_t* values)
  {
    size_t inc = 0;
    while (inc < n)
      this->writeI32(values[inc++]);
    return n * 2;
  };
  
  /** 
   * Write the 32-bit unsigned integer @a u32 and return its size
   */
  size_t BinaryStream::writeU32(uint32_t value)
  {
    auto optr = this->pimpl();
    optr->Converter->writeU32(value, optr->Device);
    return 4;
  };
  
  /**
   * Writes the array of unsigned 32-bit integers @a values in the stream an return its size.
   * @note The array @a values must contain at least @a n elements initialized;
   */
  size_t BinaryStream::writeU32(size_t n, const uint32_t* values)
  {
    size_t inc = 0;
    while (inc < n)
      this->writeU32(values[inc++]);
    return n * 2;
  };
  
  /**
   * Writes the float @a f in the stream an return its size.
   */
  size_t BinaryStream::writeFloat(float value)
  {
    auto optr = this->pimpl();
    optr->Converter->writeFloat(value, optr->Device);
    return 4;
  };
  
  /** 
   * Writes the array of floats @a values in the stream an return its size.
   * @note The array @a values must contain at least @a n elements initialized;
   */
  size_t BinaryStream::writeFloat(size_t n, const float* values)
  {
    size_t inc = 0;
    while (inc < n)
      this->writeFloat(values[inc++]);
    return n * 4;
  };
 
  /** 
   * Writes the string @a rString in the stream an return its size.
   */
  size_t BinaryStream::writeString(const std::string& value)
  {
    auto optr = this->pimpl();
    optr->Converter->writeString(value, optr->Device);
    return value.length();
  };
   
  /** 
   * Writes the array of strings @a values in the stream an return its size.
   * @note The array @a values must contain at least @a n elements initialized;
   */
  size_t BinaryStream::writeString(size_t n, const std::string* values)
  {
    size_t inc = 0;
    size_t writedBytes = 0;
    while (inc < n)
      writedBytes += this->writeString(values[inc++]);
    return writedBytes;
  };
};
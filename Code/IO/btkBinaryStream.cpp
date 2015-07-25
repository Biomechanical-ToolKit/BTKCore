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
#include "btkEndianFormat.h"
#include "btkIODevice.h"
#include "btkConfigure.h"
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
  
  BinaryStreamPrivate::~BinaryStreamPrivate() _BTK_NOEXCEPT = default;
  
  // ----------------------------------------------------------------------- //
  
  /** 
   * @class EndianConverter btkEndianConverter.h
   * @brief Base class to read and write data encoded into a specifc processor architecture.
   *
   * @sa VAXLittleEndianConverter, IEEELittleEndianConverter, IEEEBigEndianConverter
   */

  /**
   * Constructor
   */
  EndianConverter::EndianConverter()
  {}
    
  /**
   * @fn EndianConverter::~EndianConverter()
   * Destructor (default)
   */

  /** 
   * Extracts one character.
   */
  char EndianConverter::readChar(IODevice* src) const
  {
    char byteptr[1] = {0};
    src->read(byteptr, 1);
    return *byteptr;
  };
 
  /** 
   * Extracts one signed 8-bit integer.
   */
  int8_t EndianConverter::readI8(IODevice* src) const
  {
    char byteptr[1] = {0};
    src->read(byteptr, 1);
    return *byteptr;
  };
  
  /** 
   * Extracts one unsigned 8-bit integer.
   */
  uint8_t EndianConverter::readU8(IODevice* src) const
  {
    char byteptr[1] = {0};
    src->read(byteptr, 1);
    return *byteptr;
  };
  
  /** 
   * Extracts one string with @a len characters.
   */
  std::string EndianConverter::readString(size_t len, IODevice* src) const
  {
    std::string str;
    if (len != 0)
    {
      char* byteptr = new char[len];
      src->read(byteptr, len);
      str.assign(byteptr, len);
      delete[] byteptr;
    }
    return str;
  };
  
  
  /** 
   * Writes the character @a val in the device.
   */  
  void EndianConverter::writeChar(char val, IODevice* dest) const
  {
    dest->write(&val, 1);
  };

  /** 
   * Writes the signed 8-bit integer @a val in the device.
   */  
  void EndianConverter::writeI8(int8_t val, IODevice* dest) const
  {
    char c = static_cast<char>(val);
    dest->write(&c, 1);
  };
  
  /** 
   * Writes the unsigned 8-bit integer @a val in the device.
   */
  void EndianConverter::writeU8(uint8_t val, IODevice* dest) const
  {
    char c = static_cast<char>(val);
    dest->write(&c, 1);
  };
  
 /** 
  * Writes the string @a val in the device.
  */
  void EndianConverter::writeString(const std::string& val, IODevice* dest) const
  {
    dest->write(val.c_str(), val.length());
  };
  
  // ----------------------------------------------------------------------- //

  /** 
   * @class VAXLittleEndianConverter btkEndianConverter.h
   * @brief Class to Read and Write data encoded from a VAX (LE) processor
   * to a VAX (LE) and IEEE (LE, BE) processor.
   *
   * @sa IEEELittleEndianConverter, IEEEBigEndianConverter
   */
  
  /**
   * Constructor
   */
  VAXLittleEndianConverter::VAXLittleEndianConverter()
  : EndianConverter()
  {}
  
  /**
   * @fn VAXLittleEndianConverter::~VAXLittleEndianConverter()
   * Destructor (default)
   */
  
  /**
   * Return the enumeration value EndianConverter::VAXLittleEndian
   */
  EndianFormat VAXLittleEndianConverter::format() const _BTK_NOEXCEPT
  {
    return EndianFormat::VAXLittleEndian;
  };

  /** 
   * Extracts one signed 16-bit integer.
   */
  int16_t VAXLittleEndianConverter::readI16(IODevice* src) const
  {
    char byteptr[2] = {0};
    src->read(byteptr, 2);
#if BTK_ARCH_COMPILED == BTK_ARCH_IEEE_BE
    const char temp[2] = {byteptr[1], byteptr[0]};
    return *reinterpret_cast<int16_t const*>(temp);
#else
    return *reinterpret_cast<int16_t const*>(byteptr);
#endif
  };
  
  /** 
   * Extracts one unsigned 16-bit integer.
   */
  uint16_t VAXLittleEndianConverter::readU16(IODevice* src) const
  {
    char byteptr[2] = {0};
    src->read(byteptr, 2);
#if BTK_ARCH_COMPILED == BTK_ARCH_IEEE_BE
    const char temp[2] = {byteptr[1], byteptr[0]};
    return *reinterpret_cast<uint16_t const*>(temp);
#else
    return *reinterpret_cast<uint16_t const*>(byteptr);
#endif
  };
  
  /** 
   * Extracts one signed 32-bit integer.
   */
  int32_t VAXLittleEndianConverter::readI32(IODevice* src) const
  {
    char byteptr[4] = {0};
    src->read(byteptr, 4);
#if BTK_ARCH_COMPILED == BTK_ARCH_IEEE_BE
    const char temp[4] = {byteptr[1], byteptr[0], byteptr[3], byteptr[2]};
    return *reinterpret_cast<int32_t const*>(temp);
#elif BTK_ARCH_COMPILED == BTK_ARCH_VAX_LE
    return *reinterpret_cast<int32_t const*>(byteptr);
#else
    const char temp[4] = {byteptr[2], byteptr[3], byteptr[0], byteptr[1]};
    return *reinterpret_cast<int32_t const*>(temp);
#endif
  };

  /** 
   * Extracts one unsigned 32-bit integer.
   */
  uint32_t VAXLittleEndianConverter::readU32(IODevice* src) const
  {
    char byteptr[4] = {0};
    src->read(byteptr, 4);
#if BTK_ARCH_COMPILED == BTK_ARCH_IEEE_BE
    const char temp[4] = {byteptr[1], byteptr[0], byteptr[3], byteptr[2]};
    return *reinterpret_cast<uint32_t const*>(temp);
#elif BTK_ARCH_COMPILED == BTK_ARCH_VAX_LE
    return *reinterpret_cast<uint32_t const*>(byteptr);
#else
    const char temp[4] = {byteptr[2], byteptr[3], byteptr[0], byteptr[1]};
    return *reinterpret_cast<uint32_t const*>(temp);
#endif
  };
  
  /** 
   * Extracts one signed 64-bit integer.
   */
  int64_t VAXLittleEndianConverter::readI64(IODevice* src) const
  {
    char byteptr[8] = {0};
    src->read(byteptr, 8);
#if BTK_ARCH_COMPILED == BTK_ARCH_IEEE_BE
    const char temp[8] = {byteptr[1], byteptr[0], byteptr[3], byteptr[2], byteptr[5], byteptr[4], byteptr[7], byteptr[6]};
    return *reinterpret_cast<int64_t const*>(temp);
#elif BTK_ARCH_COMPILED == BTK_ARCH_VAX_LE
    return *reinterpret_cast<int64_t const*>(byteptr);
#else
    const char temp[8] = {byteptr[6], byteptr[7], byteptr[4], byteptr[5], byteptr[2], byteptr[3], byteptr[0], byteptr[1]};
    return *reinterpret_cast<int64_t const*>(temp);
#endif
  };

  /** 
   * Extracts one unsigned 64-bit integer.
   */
  uint64_t VAXLittleEndianConverter::readU64(IODevice* src) const
  {
    char byteptr[8] = {0};
    src->read(byteptr, 8);
#if BTK_ARCH_COMPILED == BTK_ARCH_IEEE_BE
    const char temp[8] = {byteptr[1], byteptr[0], byteptr[3], byteptr[2], byteptr[5], byteptr[4], byteptr[7], byteptr[6]};
    return *reinterpret_cast<uint64_t const*>(temp);
#elif BTK_ARCH_COMPILED == BTK_ARCH_VAX_LE
    return *reinterpret_cast<uint64_t const*>(byteptr);
#else
    const char temp[8] = {byteptr[6], byteptr[7], byteptr[4], byteptr[5], byteptr[2], byteptr[3], byteptr[0], byteptr[1]};
    return *reinterpret_cast<uint64_t const*>(temp);
#endif
  };
  
  /** 
   * Extracts one float.
   */
  float VAXLittleEndianConverter::readFloat(IODevice* src) const
  {
    char byteptr[4] = {0};
    src->read(byteptr, 4);
#if BTK_ARCH_COMPILED == BTK_ARCH_IEEE_BE
    const char temp[4] = {static_cast<char>(byteptr[1] - 1 * (byteptr[1] == 0 ? 0 : 1)), byteptr[0], byteptr[3], byteptr[2]};
    return *reinterpret_cast<float const*>(temp);
#elif BTK_ARCH_COMPILED == BTK_ARCH_VAX_LE
    return *reinterpret_cast<float const*>(byteptr);
#else
    const char temp[4] = {byteptr[2], byteptr[3], byteptr[0], static_cast<char>(byteptr[1] - 1 * (byteptr[1] == 0 ? 0 : 1))};
    return *reinterpret_cast<float const*>(temp);
#endif
  };
  
  /** 
   * Extracts one double.
   */
  double VAXLittleEndianConverter::readDouble(IODevice* src) const
  {
    char byteptr[8] = {0};
    src->read(byteptr, 8);
#if BTK_ARCH_COMPILED == BTK_ARCH_IEEE_BE
    const char temp[8] = {static_cast<char>(byteptr[1] - 1 * (byteptr[1] == 0 ? 0 : 1)), byteptr[0], byteptr[3], byteptr[2], byteptr[5], byteptr[4], byteptr[7], byteptr[6]};
    return *reinterpret_cast<double const*>(temp);
#elif BTK_ARCH_COMPILED == BTK_ARCH_VAX_LE
    return *reinterpret_cast<double const*>(byteptr);
#else
    const char temp[8] = {byteptr[6], byteptr[7], byteptr[4], byteptr[5], byteptr[2], byteptr[3], byteptr[0], static_cast<char>(byteptr[1] - 1 * (byteptr[1] == 0 ? 0 : 1))};
    return *reinterpret_cast<double const*>(temp);
#endif
  };

  /**
   * Writes the signed 16-bit integer @a i16 in the give device @a dest.
   */
  void VAXLittleEndianConverter::writeI16(int16_t val, IODevice* dest) const
  {
    char byteptr[2] = {0};
    memcpy(&byteptr, &val, sizeof(byteptr));
#if BTK_ARCH_COMPILED == BTK_ARCH_IEEE_BE
    const char temp[2] = {byteptr[1], byteptr[0]};
    dest->write(temp, 2);
#else
    dest->write(byteptr, 2);
#endif
  };
  
  /**
   * Writes the unsigned 16-bit integer @a u16 in the give device @a dest.
   */
  void VAXLittleEndianConverter::writeU16(uint16_t val, IODevice* dest) const
  {
    char byteptr[2] = {0};
    memcpy(&byteptr, &val, sizeof(byteptr));
#if BTK_ARCH_COMPILED == BTK_ARCH_IEEE_BE
    const char temp[2] = {byteptr[1], byteptr[0]};
    dest->write(temp, 2); 
#else
    dest->write(byteptr, 2);
#endif
  };
  
  /** 
   * Write the 32-bit signed integer @a val in the give device @a dest.
   */
  void VAXLittleEndianConverter::writeI32(int32_t val, IODevice* dest) const
  {
    char byteptr[4] = {0};
    memcpy(&byteptr, &val, sizeof(byteptr));
#if BTK_ARCH_COMPILED == BTK_ARCH_IEEE_BE
    const char temp[4] = {byteptr[1], byteptr[0], byteptr[3], byteptr[2]};
    dest->write(temp, 4);
#elif BTK_ARCH_COMPILED == BTK_ARCH_VAX_LE
    dest->write(byteptr, 4);
#else
    const char temp[4] = {byteptr[2], byteptr[3], byteptr[0], byteptr[1]};
    dest->write(temp, 4);
#endif
  };
  
  /** 
   * Write the 32-bit unsigned integer @a val in the give device @a dest.
   */
  void VAXLittleEndianConverter::writeU32(uint32_t val, IODevice* dest) const
  {
    char byteptr[4] = {0};
    memcpy(&byteptr, &val, sizeof(byteptr));
#if BTK_ARCH_COMPILED == BTK_ARCH_IEEE_BE
    const char temp[4] = {byteptr[1], byteptr[0], byteptr[3], byteptr[2]};
    dest->write(temp, 4);
#elif BTK_ARCH_COMPILED == BTK_ARCH_VAX_LE
    dest->write(byteptr, 4);
#else
    const char temp[4] = {byteptr[2], byteptr[3], byteptr[0], byteptr[1]};
    dest->write(temp, 4);
#endif
  };

  /**
   * Writes the float @a f in the give device @a dest.
   */
  void VAXLittleEndianConverter::writeFloat(float val, IODevice* dest) const
  {
    char byteptr[4] = {0};
    memcpy(&byteptr, &val, sizeof(byteptr));
#if BTK_ARCH_COMPILED == BTK_ARCH_IEEE_BE
    const char temp[4] = {byteptr[1], static_cast<char>(byteptr[0] + 1 * (byteptr[0] == 0 ? 0 : 1)),  byteptr[3], byteptr[2]};
    dest->write(temp, 4);
#elif BTK_ARCH_COMPILED == BTK_ARCH_VAX_LE
    dest->write(byteptr, 4);
#else
    const char temp[4] = {byteptr[2], static_cast<char>(byteptr[3] + 1 * (byteptr[3] == 0 ? 0 : 1)), byteptr[0], byteptr[1]};
    dest->write(temp, 4);
#endif
  };
  
  // ----------------------------------------------------------------------- //
  
  /** 
   * @class IEEEBigEndianConverter btkEndianConverter.h
   * @brief Class to Read and Write data encoded from a IEEE (BE) to a 
   * VAX (LE) and IEEE (LE, BE) processor
   *
   * @sa VAXLittleEndianConverter, IEEELittleEndianConverter
   */
 
  /**
   * Constructor
   */
  IEEEBigEndianConverter::IEEEBigEndianConverter()
  : EndianConverter()
  {}

  /**
   * @fn IEEEBigEndianConverter::~IEEEBigEndianConverter()
   * Destructor (default)
   */
  
  /**
   * Return the enumeration value EndianConverter::IEEEBigEndian
   */
  EndianFormat IEEEBigEndianConverter::format() const _BTK_NOEXCEPT
  {
    return EndianFormat::IEEEBigEndian;
  };
  
  /** 
   * Extracts one signed 16-bit integer.
   */
  int16_t IEEEBigEndianConverter::readI16(IODevice* src) const
  {
    char byteptr[2] = {0};
    src->read(byteptr, 2);
#if BTK_ARCH_COMPILED == BTK_ARCH_IEEE_BE
    return *reinterpret_cast<int16_t const*>(byteptr);
#else
    const char temp[2] = {byteptr[1], byteptr[0]};
    return *reinterpret_cast<int16_t const*>(temp);
#endif
  };
  
  /** 
   * Extracts one unsigned 16-bit integer.
   */
  uint16_t IEEEBigEndianConverter::readU16(IODevice* src) const
  {
    char byteptr[2] = {0};
    src->read(byteptr, 2);
#if BTK_ARCH_COMPILED == BTK_ARCH_IEEE_BE
    return *reinterpret_cast<uint16_t const*>(byteptr);
#else
    const char temp[2] = {byteptr[1], byteptr[0]};
    return *reinterpret_cast<uint16_t const*>(temp);
#endif
  };
  
  /** 
   * Extracts one signed 32-bit integer.
   */
  int32_t IEEEBigEndianConverter::readI32(IODevice* src) const
  {
    char byteptr[4] = {0};
    src->read(byteptr, 4);
#if BTK_ARCH_COMPILED == BTK_ARCH_IEEE_BE
    return *reinterpret_cast<int32_t const*>(byteptr);
#elif BTK_ARCH_COMPILED == BTK_ARCH_VAX_LE
    const char temp[4] = {byteptr[1], byteptr[0], byteptr[3], byteptr[2]};
    return *reinterpret_cast<int32_t const*>(temp);
#else
    const char temp[4] = {byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    return *reinterpret_cast<int32_t const*>(temp);
#endif
  };
  
  /** 
   * Extracts one unsigned 32-bit integer.
   */
  uint32_t IEEEBigEndianConverter::readU32(IODevice* src) const
  {
    char byteptr[4] = {0};
    src->read(byteptr, 4);
#if BTK_ARCH_COMPILED == BTK_ARCH_IEEE_BE
    return *reinterpret_cast<uint32_t const*>(byteptr);
#elif BTK_ARCH_COMPILED == BTK_ARCH_VAX_LE
    const char temp[4] = {byteptr[1], byteptr[0], byteptr[3], byteptr[2]};
    return *reinterpret_cast<uint32_t const*>(temp);
#else
    const char temp[4] = {byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    return *reinterpret_cast<uint32_t const*>(temp);
#endif
  };
  
  /** 
   * Extracts one signed 64-bit integer.
   */
  int64_t IEEEBigEndianConverter::readI64(IODevice* src) const
  {
    char byteptr[8] = {0};
    src->read(byteptr, 8);
#if BTK_ARCH_COMPILED == BTK_ARCH_IEEE_BE
    return *reinterpret_cast<int64_t const*>(byteptr);
#elif BTK_ARCH_COMPILED == BTK_ARCH_VAX_LE
    const char temp[8] = {byteptr[1], byteptr[0], byteptr[3], byteptr[2], byteptr[5], byteptr[4], byteptr[7], byteptr[6]};
    return *reinterpret_cast<int64_t const*>(temp);
#else
    const char temp[8] = {byteptr[7], byteptr[6], byteptr[5], byteptr[4], byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    return *reinterpret_cast<int64_t const*>(temp);
#endif
  };
  
  /** 
   * Extracts one unsigned 64-bit integer.
   */
  uint64_t IEEEBigEndianConverter::readU64(IODevice* src) const
  {
    char byteptr[8] = {0};
    src->read(byteptr, 8);
#if BTK_ARCH_COMPILED == BTK_ARCH_IEEE_BE
    return *reinterpret_cast<uint64_t const*>(byteptr);
#elif BTK_ARCH_COMPILED == BTK_ARCH_VAX_LE
    const char temp[8] = {byteptr[1], byteptr[0], byteptr[3], byteptr[2], byteptr[5], byteptr[4], byteptr[7], byteptr[6]};
    return *reinterpret_cast<uint64_t const*>(temp);
#else
    const char temp[8] = {byteptr[7], byteptr[6], byteptr[5], byteptr[4], byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    return *reinterpret_cast<uint64_t const*>(temp);
#endif
  };
  
  /** 
   * Extracts one float.
   */
  float IEEEBigEndianConverter::readFloat(IODevice* src) const
  {
    char byteptr[4] = {0};
    src->read(byteptr, 4);
#if BTK_ARCH_COMPILED == BTK_ARCH_IEEE_BE
    return *reinterpret_cast<float const*>(byteptr);
#elif BTK_ARCH_COMPILED == BTK_ARCH_VAX_LE
    const char temp[4] = {byteptr[1], static_cast<char>(byteptr[0] + 1 * (byteptr[0] == 0 ? 0 : 1)), byteptr[3], byteptr[2]};
    return *reinterpret_cast<float const*>(temp);
#else
    const char temp[4] = {byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    return *reinterpret_cast<float const*>(temp);
#endif
  };
  
  /** 
   * Extracts one double.
   */
  double IEEEBigEndianConverter::readDouble(IODevice* src) const
  {
    char byteptr[8] = {0};
    src->read(byteptr, 8);
#if BTK_ARCH_COMPILED == BTK_ARCH_IEEE_BE
    return *reinterpret_cast<double const*>(byteptr);
#elif BTK_ARCH_COMPILED == BTK_ARCH_VAX_LE
    const char temp[8] = {byteptr[1], static_cast<char>(byteptr[0] + 1 * (byteptr[0] == 0 ? 0 : 1)), byteptr[3], byteptr[2], byteptr[5], byteptr[4], byteptr[7], byteptr[6]};
    return *reinterpret_cast<double const*>(temp);
#else
    const char temp[8] = {byteptr[7], byteptr[6], byteptr[5], byteptr[4], byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    return *reinterpret_cast<double const*>(temp);
#endif
  };

  /**
   * Writes the signed 16-bit integer @a i16 in the give device @a dest.
   */
  void IEEEBigEndianConverter::writeI16(int16_t val, IODevice* dest) const
  {
    char byteptr[2] = {0};
    memcpy(&byteptr, &val, sizeof(byteptr));
#if BTK_ARCH_COMPILED == BTK_ARCH_IEEE_BE
    dest->write(byteptr, 2); 
#else
    const char temp[2] = {byteptr[1], byteptr[0]};
    dest->write(temp, 2);
#endif
  };

  /**
   * Writes the unsigned 16-bit integer @a u16 in the give device @a dest.
   */
  void IEEEBigEndianConverter::writeU16(uint16_t val, IODevice* dest) const
  {
    char byteptr[2] = {0};
    memcpy(&byteptr, &val, sizeof(byteptr));
#if BTK_ARCH_COMPILED == BTK_ARCH_IEEE_BE
    dest->write(byteptr, 2); 
#else
    const char temp[2] = {byteptr[1], byteptr[0]};
    dest->write(temp, 2);
#endif
  };
  
  /** 
   * Write the 32-bit signed integer @a val in the give device @a dest.
   */
  void IEEEBigEndianConverter::writeI32(int32_t val, IODevice* dest) const
  {
    char byteptr[4] = {0};
    memcpy(&byteptr, &val, sizeof(byteptr));
#if BTK_ARCH_COMPILED == BTK_ARCH_IEEE_BE
    dest->write(byteptr, 4);
#elif BTK_ARCH_COMPILED == BTK_ARCH_VAX_LE
    const char temp[4] = {byteptr[1], byteptr[0], byteptr[3], byteptr[2]};
    dest->write(temp, 4);
#else
    const char temp[4] = {byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    dest->write(temp, 4);
#endif
  };
  
  /** 
   * Write the 32-bit unsigned integer @a val in the give device @a dest.
   */
  void IEEEBigEndianConverter::writeU32(uint32_t val, IODevice* dest) const
  {
    char byteptr[4] = {0};
    memcpy(&byteptr, &val, sizeof(byteptr));
#if BTK_ARCH_COMPILED == BTK_ARCH_IEEE_BE
    dest->write(byteptr, 4);
#elif BTK_ARCH_COMPILED == BTK_ARCH_VAX_LE
    const char temp[4] = {byteptr[1], byteptr[0], byteptr[3], byteptr[2]};
    dest->write(temp, 4);
#else
    const char temp[4] = {byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    dest->write(temp, 4);
#endif
  };

  /**
   * Writes the float @a val in the give device @a dest.
   */
  void IEEEBigEndianConverter::writeFloat(float val, IODevice* dest) const
  {
    char byteptr[4] = {0};
    memcpy(&byteptr, &val, sizeof(byteptr));
#if BTK_ARCH_COMPILED == BTK_ARCH_IEEE_BE
    dest->write(byteptr, 4);
#elif BTK_ARCH_COMPILED == BTK_ARCH_VAX_LE
    const char temp[4] = {static_cast<char>(byteptr[1] - 1 * (byteptr[1] == 0 ? 0 : 1)), byteptr[0], byteptr[3], byteptr[2]};
    dest->write(temp, 4);
#else
    const char temp[4] = {byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    dest->write(temp, 4);
#endif
  };
  
  // ----------------------------------------------------------------------- //
  
  /** 
   * @class IEEELittleEndianConverter btkEndianConverter.h
   * @brief Class to Read and Write data encoded from a IEEE (LE) to a 
   * VAX (LE) and IEEE (LE, BE) processor.
   *
   * @sa VAXLittleEndianConverter, IEEEBigEndianConverter
   */
  
  /**
   * Constructor
   */
  IEEELittleEndianConverter::IEEELittleEndianConverter()
  : EndianConverter()
  {}

  /**
   * @fn IEEELittleEndianConverter::~IEEELittleEndianConverter()
   * Destructor (default)
   */
    
  /**
   * Return the enumeration value EndianConverter::IEEELittleEndian
   */
  EndianFormat IEEELittleEndianConverter::format() const _BTK_NOEXCEPT
  {
    return EndianFormat::IEEELittleEndian;
  };
  
  /** 
   * Extracts one signed 16-bit integer.
   */
  int16_t IEEELittleEndianConverter::readI16(IODevice* src) const
  {
    char byteptr[2] = {0};
    src->read(byteptr, 2);
#if BTK_ARCH_COMPILED == BTK_ARCH_IEEE_BE
    const char temp[2] = {byteptr[1], byteptr[0]};
    return *reinterpret_cast<int16_t const*>(temp);
#else
    return *reinterpret_cast<int16_t const*>(byteptr);
#endif
  };
  
  /** 
   * Extracts one unsigned 16-bit integer.
   */
  uint16_t IEEELittleEndianConverter::readU16(IODevice* src) const
  {
    char byteptr[2] = {0};
    src->read(byteptr, 2);
#if BTK_ARCH_COMPILED == BTK_ARCH_IEEE_BE
    const char temp[2] = {byteptr[1], byteptr[0]};
    return *reinterpret_cast<uint16_t const*>(temp);
#else
    return *reinterpret_cast<uint16_t const*>(byteptr);
#endif
  };
  
  /** 
   * Extracts one signed 32-bit integer.
   */
  int32_t IEEELittleEndianConverter::readI32(IODevice* src) const
  {
    char byteptr[4] = {0};
    src->read(byteptr, 4);
#if BTK_ARCH_COMPILED == BTK_ARCH_IEEE_BE
    const char temp[4] = {byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    return *reinterpret_cast<int32_t const*>(temp);
#elif BTK_ARCH_COMPILED == BTK_ARCH_VAX_LE 
    const char temp[4] = {byteptr[2], byteptr[3], byteptr[0], byteptr[1]};
    return *reinterpret_cast<int32_t const*>(temp);
#else
    return *reinterpret_cast<int32_t const*>(byteptr);
#endif
  };

  /** 
   * Extracts one unsigned 32-bit integer.
   */
  uint32_t IEEELittleEndianConverter::readU32(IODevice* src) const
  {
    char byteptr[4] = {0};
    src->read(byteptr, 4);
#if BTK_ARCH_COMPILED == BTK_ARCH_IEEE_BE
    const char temp[4] = {byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    return *reinterpret_cast<uint32_t const*>(temp);
#elif BTK_ARCH_COMPILED == BTK_ARCH_VAX_LE 
    const char temp[4] = {byteptr[2], byteptr[3], byteptr[0], byteptr[1]};
    return *reinterpret_cast<uint32_t const*>(temp);
#else
    return *reinterpret_cast<uint32_t const*>(byteptr);
#endif
  };
  
  /** 
   * Extracts one signed 64-bit integer.
   */
  int64_t IEEELittleEndianConverter::readI64(IODevice* src) const
  {
    char byteptr[8] = {0};
    src->read(byteptr, 8);
#if BTK_ARCH_COMPILED == BTK_ARCH_IEEE_BE
    const char temp[8] = {byteptr[7], byteptr[6], byteptr[5], byteptr[6], byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    return *reinterpret_cast<int64_t const*>(temp);
#elif BTK_ARCH_COMPILED == BTK_ARCH_VAX_LE 
    const char temp[8] = {byteptr[6], byteptr[7], byteptr[4], byteptr[5], byteptr[2], byteptr[3], byteptr[0], byteptr[1]};
    return *reinterpret_cast<int64_t const*>(temp);
#else
    return *reinterpret_cast<int64_t const*>(byteptr);
#endif
  };

  /** 
   * Extracts one unsigned 64-bit integer.
   */
  uint64_t IEEELittleEndianConverter::readU64(IODevice* src) const
  {
    char byteptr[8] = {0};
    src->read(byteptr, 8);
#if BTK_ARCH_COMPILED == BTK_ARCH_IEEE_BE
    const char temp[8] = {byteptr[7], byteptr[6], byteptr[5], byteptr[6], byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    return *reinterpret_cast<uint64_t const*>(temp);
#elif BTK_ARCH_COMPILED == BTK_ARCH_VAX_LE 
    const char temp[8] = {byteptr[6], byteptr[7], byteptr[4], byteptr[5], byteptr[2], byteptr[3], byteptr[0], byteptr[1]};
    return *reinterpret_cast<uint64_t const*>(temp);
#else
    return *reinterpret_cast<uint64_t const*>(byteptr);
#endif
  };

  /** 
   * Extracts one float.
   */
  float IEEELittleEndianConverter::readFloat(IODevice* src) const
  {
    char byteptr[4] = {0};
    src->read(byteptr, 4);
#if BTK_ARCH_COMPILED == BTK_ARCH_IEEE_BE
    const char temp[4] = {byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    return *reinterpret_cast<float const*>(temp);
#elif BTK_ARCH_COMPILED == BTK_ARCH_VAX_LE 
    const char temp[4] = {byteptr[2], static_cast<char>(byteptr[3] + 1 * (byteptr[3] == 0 ? 0 : 1)), byteptr[0], byteptr[1]};
    return *reinterpret_cast<float const*>(temp);
#else
    return *reinterpret_cast<float const*>(byteptr);
#endif
  };
  
  /** 
   * Extracts one float.
   */
  double IEEELittleEndianConverter::readDouble(IODevice* src) const
  {
    char byteptr[8] = {0};
    src->read(byteptr, 8);
#if BTK_ARCH_COMPILED == BTK_ARCH_IEEE_BE
    const char temp[8] = {byteptr[7], byteptr[6], byteptr[5], byteptr[4], byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    return *reinterpret_cast<double const*>(temp);
#elif BTK_ARCH_COMPILED == BTK_ARCH_VAX_LE 
    const char temp[8] = {byteptr[6], static_cast<char>(byteptr[7] + 1 * (byteptr[7] == 0 ? 0 : 1)), byteptr[4], byteptr[5], byteptr[2], byteptr[3], byteptr[1], byteptr[0]};
    return *reinterpret_cast<double const*>(temp);
#else
    return *reinterpret_cast<double const*>(byteptr);
#endif
  };

  /**
   * Writes the signed 16-bit integer @a i16 in the give device @a dest.
   */
  void IEEELittleEndianConverter::writeI16(int16_t val, IODevice* dest) const
  {
    char byteptr[2] = {0};
    memcpy(&byteptr, &val, sizeof(byteptr));
#if BTK_ARCH_COMPILED == BTK_ARCH_IEEE_BE
    const char temp[2] = {byteptr[1], byteptr[0]};
    dest->write(temp, 2); 
#else
    dest->write(byteptr, 2);
#endif
  };

  /**
   * Writes the unsigned 16-bit integer @a u16 in the give device @a dest.
   */
  void IEEELittleEndianConverter::writeU16(uint16_t val, IODevice* dest) const
  {
    char byteptr[2] = {0};
    memcpy(&byteptr, &val, sizeof(byteptr));
#if BTK_ARCH_COMPILED == BTK_ARCH_IEEE_BE
    const char temp[2] = {byteptr[1], byteptr[0]};
    dest->write(temp, 2); 
#else
    dest->write(byteptr, 2);
#endif
  };

  /** 
   * Write the 32-bit signed integer @a val in the give device @a dest.
   */
  void IEEELittleEndianConverter::writeI32(int32_t val, IODevice* dest) const
  {
    char byteptr[4] = {0};
    memcpy(&byteptr, &val, sizeof(byteptr));
#if BTK_ARCH_COMPILED == BTK_ARCH_IEEE_BE
    const char temp[4] = {byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    dest->write(temp, 4);
#elif BTK_ARCH_COMPILED == BTK_ARCH_VAX_LE 
    const char temp[4] = {byteptr[2], byteptr[3], byteptr[0], byteptr[1]};
    dest->write(temp, 4);
#else
    dest->write(byteptr, 4);
#endif
  };
  
  /** 
   * Write the 32-bit unsigned integer @a val in the give device @a dest
   */
  void IEEELittleEndianConverter::writeU32(uint32_t val, IODevice* dest) const
  {
    char byteptr[4] = {0};
    memcpy(&byteptr, &val, sizeof(byteptr));
#if BTK_ARCH_COMPILED == BTK_ARCH_IEEE_BE
    const char temp[4] = {byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    dest->write(temp, 4);
#elif BTK_ARCH_COMPILED == BTK_ARCH_VAX_LE 
    const char temp[4] = {byteptr[2], byteptr[3], byteptr[0], byteptr[1]};
    dest->write(temp, 4);
#else
    dest->write(byteptr, 4);
#endif
  };

  /**
   * Writes the float @a val in the give device @a dest.
   */
  void IEEELittleEndianConverter::writeFloat(float val, IODevice* dest) const
  {
    char byteptr[4] = {0};
    memcpy(&byteptr, &val, sizeof(byteptr));
#if BTK_ARCH_COMPILED == BTK_ARCH_IEEE_BE
    const char temp[4] = {byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    dest->write(temp, 4);
#elif BTK_ARCH_COMPILED == BTK_ARCH_VAX_LE
    const char temp[4] = {byteptr[2], byteptr[3], byteptr[0], static_cast<char>(byteptr[1] - 1 * (byteptr[1] == 0 ? 0 : 1))};
    dest->write(temp, 4);
#else
    dest->write(byteptr, 4);
#endif
  };
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
  : mp_Pimpl(new BinaryStreamPrivate(device,nullptr))
  {
    this->setEndianFormat(EndianFormat::Native);
  };
  
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
  BinaryStream::~BinaryStream() _BTK_NOEXCEPT
  {
    delete this->mp_Pimpl->Converter;
  };
  
  /**
   * Returns the device associated to this stream. 
   */
  IODevice* BinaryStream::device() const _BTK_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Device;
  };
  
  /**
   * Sets the device associated to this stream.
   * Trying to assign a null device will print an error message and returns.
   */
  void BinaryStream::setDevice(IODevice* device) _BTK_NOEXCEPT
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
  EndianFormat BinaryStream::endianFormat() const _BTK_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Converter->format();
  };
  
  /**
   * Sets the format of the endian converter used by the stream.
   *
   * @note You can use the enum value EndianFormat::NotApplicable. This delete the converter and set it to null. However, it is not recommended to do that as the stream would crash when trying to read/write data from/to a device.
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
    case EndianFormat::NotApplicable:
      optr->Converter = nullptr;
      break;
    case EndianFormat::VAXLittleEndian:
      optr->Converter = new VAXLittleEndianConverter;
      break;
    case EndianFormat::IEEELittleEndian:
      optr->Converter = new IEEELittleEndianConverter;
      break;
    case EndianFormat::IEEEBigEndian:
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
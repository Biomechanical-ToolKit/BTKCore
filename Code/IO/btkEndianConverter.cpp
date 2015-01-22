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

#include "btkEndianConverter.h"
#include "btkIODevice.h"

namespace btk
{
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
    std::string sFs;
    if (len != 0)
    {
      char* byteptr = new char[len];
      src->read(byteptr, len);
      sFs = std::string(byteptr, len);
      delete[] byteptr;
    }
    return sFs;
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
  EndianFormat VAXLittleEndianConverter::format() const noexcept
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
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
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
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
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
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    const char temp[4] = {byteptr[1], byteptr[0], byteptr[3], byteptr[2]};
    return *reinterpret_cast<int32_t const*>(temp);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
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
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    const char temp[4] = {byteptr[1], byteptr[0], byteptr[3], byteptr[2]};
    return *reinterpret_cast<uint32_t const*>(temp);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
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
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    const char temp[8] = {byteptr[1], byteptr[0], byteptr[3], byteptr[2], byteptr[5], byteptr[4], byteptr[7], byteptr[6]};
    return *reinterpret_cast<int64_t const*>(temp);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
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
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    const char temp[8] = {byteptr[1], byteptr[0], byteptr[3], byteptr[2], byteptr[5], byteptr[4], byteptr[7], byteptr[6]};
    return *reinterpret_cast<uint64_t const*>(temp);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
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
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    const char temp[4] = {static_cast<char>(byteptr[1] - 1 * (byteptr[1] == 0 ? 0 : 1)), byteptr[0], byteptr[3], byteptr[2]};
    return *reinterpret_cast<float const*>(temp);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
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
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    const char temp[8] = {static_cast<char>(byteptr[1] - 1 * (byteptr[1] == 0 ? 0 : 1)), byteptr[0], byteptr[3], byteptr[2], byteptr[5], byteptr[4], byteptr[7], byteptr[6]};
    return *reinterpret_cast<double const*>(temp);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
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
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
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
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
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
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    const char temp[4] = {byteptr[1], byteptr[0], byteptr[3], byteptr[2]};
    dest->write(temp, 4);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
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
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    const char temp[4] = {byteptr[1], byteptr[0], byteptr[3], byteptr[2]};
    dest->write(temp, 4);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
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
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    const char temp[4] = {byteptr[1], static_cast<char>(byteptr[0] + 1 * (byteptr[0] == 0 ? 0 : 1)),  byteptr[3], byteptr[2]};
    dest->write(temp, 4);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
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
  EndianFormat IEEEBigEndianConverter::format() const noexcept
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
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
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
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
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
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    return *reinterpret_cast<int32_t const*>(byteptr);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
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
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    return *reinterpret_cast<uint32_t const*>(byteptr);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
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
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    return *reinterpret_cast<int64_t const*>(byteptr);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
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
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    return *reinterpret_cast<uint64_t const*>(byteptr);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
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
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    return *reinterpret_cast<float const*>(byteptr);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
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
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    return *reinterpret_cast<double const*>(byteptr);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
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
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
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
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
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
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    dest->write(byteptr, 4);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
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
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    dest->write(byteptr, 4);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
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
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    dest->write(byteptr, 4);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
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
  EndianFormat IEEELittleEndianConverter::format() const noexcept
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
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
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
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
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
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    const char temp[4] = {byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    return *reinterpret_cast<int32_t const*>(temp);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */ 
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
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    const char temp[4] = {byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    return *reinterpret_cast<uint32_t const*>(temp);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */ 
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
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    const char temp[8] = {byteptr[7], byteptr[6], byteptr[5], byteptr[6], byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    return *reinterpret_cast<int64_t const*>(temp);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */ 
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
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    const char temp[8] = {byteptr[7], byteptr[6], byteptr[5], byteptr[6], byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    return *reinterpret_cast<uint64_t const*>(temp);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */ 
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
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    const char temp[4] = {byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    return *reinterpret_cast<float const*>(temp);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */ 
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
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    const char temp[8] = {byteptr[7], byteptr[6], byteptr[5], byteptr[4], byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    return *reinterpret_cast<double const*>(temp);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */ 
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
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
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
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
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
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    const char temp[4] = {byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    dest->write(temp, 4);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */ 
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
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    const char temp[4] = {byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    dest->write(temp, 4);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */ 
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
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    const char temp[4] = {byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    dest->write(temp, 4);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
    const char temp[4] = {byteptr[2], byteptr[3], byteptr[0], static_cast<char>(byteptr[1] - 1 * (byteptr[1] == 0 ? 0 : 1))};
    dest->write(temp, 4);
#else
    dest->write(byteptr, 4);
#endif
  };
  
  // ----------------------------------------------------------------------- //
  
  /** 
   * @class NativeEndian btkEndianConverter.h
   * @brief Helper to read/write binary data from/to a device using the native byte order format of the current processor.
   *
   * Depending the processor, the native converter will be:
   *  - the VAX Little Endian converter (VAXLittleEndianConverter)
   *  - the IEEE Little Endian converter (IEEELittleEndianConverter)
   *  - the IEEE Big Endian converter (IEEEBigEndianConverter)
   *
   * @sa EndianConverter, VAXLittleEndianConverter, IEEELittleEndianConverter, IEEEBigEndianConverter
   */
   
  /**
   * @fn NativeEndian::NativeEndian()
   * Default constructor.
   */
 
  /**
   * @fn NativeEndian::~NativeEndian()
   * Destructor (default).
   */
};
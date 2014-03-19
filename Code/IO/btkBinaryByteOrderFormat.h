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

#ifndef __btkByteOrderFormat_h
#define __btkByteOrderFormat_h

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

// MSVC doesn't have the header stdint.h
#ifdef _MSC_VER
  #include "Utilities/stdint.h"
#else
  #include <stdint.h>
#endif

namespace btk
{
  class VAXLittleEndianFormat
  {
  public:
    template<class Stream> static int16_t ReadI16(Stream* src);
    template<class Stream> static uint16_t ReadU16(Stream* src);
    template<class Stream> static int32_t ReadI32(Stream* src);
    template<class Stream> static uint32_t ReadU32(Stream* src);
    template<class Stream> static int64_t ReadI64(Stream* src);
    template<class Stream> static uint64_t ReadU64(Stream* src);
    template<class Stream> static float ReadFloat(Stream* src);
    template<class Stream> static double ReadDouble(Stream* src);
  
    template<class Stream> static void Write(int16_t val, Stream* dest);
    template<class Stream> static void Write(uint16_t val, Stream* dest);
    template<class Stream> static void Write(int32_t val, Stream* dest);
    template<class Stream> static void Write(uint32_t val, Stream* dest);
    template<class Stream> static void Write(int64_t val, Stream* dest);
    template<class Stream> static void Write(uint64_t val, Stream* dest);
    template<class Stream> static void Write(float val, Stream* dest);
  
  private:
    VAXLittleEndianFormat(); // Not implemented.
    ~VAXLittleEndianFormat(); // Not implemented.
    VAXLittleEndianFormat(const VAXLittleEndianFormat& ); // Not implemented.
    VAXLittleEndianFormat& operator=(const VAXLittleEndianFormat& ); // Not implemented.
  };

  class IEEELittleEndianFormat
  {
  public:
    template<class Stream> static int16_t ReadI16(Stream* src);
    template<class Stream> static uint16_t ReadU16(Stream* src);
    template<class Stream> static int32_t ReadI32(Stream* src);
    template<class Stream> static uint32_t ReadU32(Stream* src);
    template<class Stream> static int64_t ReadI64(Stream* src);
    template<class Stream> static uint64_t ReadU64(Stream* src);
    template<class Stream> static float ReadFloat(Stream* src);
    template<class Stream> static double ReadDouble(Stream* src);
  
    template<class Stream> static void Write(int16_t val, Stream* dest);
    template<class Stream> static void Write(uint16_t val, Stream* dest);
    template<class Stream> static void Write(int32_t val, Stream* dest);
    template<class Stream> static void Write(uint32_t val, Stream* dest);
    template<class Stream> static void Write(int64_t val, Stream* dest);
    template<class Stream> static void Write(uint64_t val, Stream* dest);
    template<class Stream> static void Write(float val, Stream* dest);
  
  private:
    IEEELittleEndianFormat(); // Not implemented.
    ~IEEELittleEndianFormat(); // Not implemented.
    IEEELittleEndianFormat(const IEEELittleEndianFormat& ); // Not implemented.
    IEEELittleEndianFormat& operator=(const IEEELittleEndianFormat& ); // Not implemented.
  };

  class IEEEBigEndianFormat
  {
  public:
    template<class Stream> static int16_t ReadI16(Stream* src);
    template<class Stream> static uint16_t ReadU16(Stream* src);
    template<class Stream> static int32_t ReadI32(Stream* src);
    template<class Stream> static uint32_t ReadU32(Stream* src);
    template<class Stream> static int64_t ReadI64(Stream* src);
    template<class Stream> static uint64_t ReadU64(Stream* src);
    template<class Stream> static float ReadFloat(Stream* src);
    template<class Stream> static double ReadDouble(Stream* src);
  
    template<class Stream> static void Write(int16_t val, Stream* dest);
    template<class Stream> static void Write(uint16_t val, Stream* dest);
    template<class Stream> static void Write(int32_t val, Stream* dest);
    template<class Stream> static void Write(uint32_t val, Stream* dest);
    template<class Stream> static void Write(int64_t val, Stream* dest);
    template<class Stream> static void Write(uint64_t val, Stream* dest);
    template<class Stream> static void Write(float val, Stream* dest);
    
  private:
    IEEEBigEndianFormat(); // Not implemented.
    ~IEEEBigEndianFormat(); // Not implemented.
    IEEEBigEndianFormat(const IEEEBigEndianFormat& ); // Not implemented.
    IEEEBigEndianFormat& operator=(const IEEEBigEndianFormat& ); // Not implemented.
  };

  // ----------------------------------------------------------------------- //

  /** 
   * @class VAXLittleEndianFormat btkBinaryByteOrderFormat.h
   * @brief Class to read and write data encoded from a VAX (LE) processor
   * to a VAX (LE) and IEEE (LE, BE) processor.
   *
   * @sa IEEELittleEndianFormat, IEEEBigEndianFormat
   */
  
  /** 
   * Extracts one signed 16-bit integer.
   */
  template <class Stream>
  int16_t VAXLittleEndianFormat::ReadI16(Stream* src)
  {
    char byteptr[2] = {0};
    src->read(byteptr, 2);
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    char foo[2] = {byteptr[1], byteptr[0]};
    return *reinterpret_cast<int16_t const*>(foo);
#else
    return *reinterpret_cast<int16_t const*>(byteptr);
#endif
  };
  
  /** 
   * Extracts one unsigned 16-bit integer.
   */
  template <class Stream>
  uint16_t VAXLittleEndianFormat::ReadU16(Stream* src)
  {
    char byteptr[2] = {0};
    src->read(byteptr, 2);
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    char foo[2] = {byteptr[1], byteptr[0]};
    return *reinterpret_cast<uint16_t const*>(foo);
#else
    return *reinterpret_cast<uint16_t const*>(byteptr);
#endif
  };
  
  /** 
   * Extracts one signed 32-bit integer.
   */
  template <class Stream>
  int32_t VAXLittleEndianFormat::ReadI32(Stream* src)
  {
    char byteptr[4] = {0};
    src->read(byteptr, 4);
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    char foo[4] = {byteptr[1], byteptr[0], byteptr[3], byteptr[2]};
    return *reinterpret_cast<int32_t const*>(foo);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
    return *reinterpret_cast<int32_t const*>(byteptr);
#else
    char foo[4] = {byteptr[2], byteptr[3], byteptr[0], byteptr[1]};
    return *reinterpret_cast<int32_t const*>(foo);
#endif
  };

  /** 
   * Extracts one unsigned 32-bit integer.
   */
  template <class Stream>
  uint32_t VAXLittleEndianFormat::ReadU32(Stream* src)
  {
    char byteptr[4] = {0};
    src->read(byteptr, 4);
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    char foo[4] = {byteptr[1], byteptr[0], byteptr[3], byteptr[2]};
    return *reinterpret_cast<uint32_t const*>(foo);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
    return *reinterpret_cast<uint32_t const*>(byteptr);
#else
    char foo[4] = {byteptr[2], byteptr[3], byteptr[0], byteptr[1]};
    return *reinterpret_cast<uint32_t const*>(foo);
#endif
  };
  
  /** 
   * Extracts one signed 64-bit integer.
   */
  template <class Stream>
  int64_t VAXLittleEndianFormat::ReadI64(Stream* src)
  {
    char byteptr[8] = {0};
    src->read(byteptr, 8);
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    char foo[8] = {byteptr[1], byteptr[0], byteptr[3], byteptr[2], byteptr[5], byteptr[4], byteptr[7], byteptr[6]};
    return *reinterpret_cast<int64_t const*>(foo);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
    return *reinterpret_cast<int64_t const*>(byteptr);
#else
    char foo[8] = {byteptr[6], byteptr[7], byteptr[4], byteptr[5], byteptr[2], byteptr[3], byteptr[0], byteptr[1]};
    return *reinterpret_cast<int64_t const*>(foo);
#endif
  };

  /** 
   * Extracts one unsigned 64-bit integer.
   */
  template <class Stream>
  uint64_t VAXLittleEndianFormat::ReadU64(Stream* src)
  {
    char byteptr[8] = {0};
    src->read(byteptr, 8);
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    char foo[8] = {byteptr[1], byteptr[0], byteptr[3], byteptr[2], byteptr[5], byteptr[4], byteptr[7], byteptr[6]};
    return *reinterpret_cast<uint64_t const*>(foo);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
    return *reinterpret_cast<uint64_t const*>(byteptr);
#else
    char foo[8] = {byteptr[6], byteptr[7], byteptr[4], byteptr[5], byteptr[2], byteptr[3], byteptr[0], byteptr[1]};
    return *reinterpret_cast<uint64_t const*>(foo);
#endif
  };
  
  /** 
   * Extracts one float.
   */
  template <class Stream>
  float VAXLittleEndianFormat::ReadFloat(Stream* src)
  {
    char byteptr[4] = {0};
    src->read(byteptr, 4);
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    char foo[4] = {byteptr[1] - 1 * (byteptr[1] == 0 ? 0 : 1), byteptr[0], byteptr[3], byteptr[2]};
    return *reinterpret_cast<float const*>(foo);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
    return *reinterpret_cast<float const*>(byteptr);
#else
    char foo[4] = {byteptr[2], byteptr[3], byteptr[0], byteptr[1] - 1 * (byteptr[1] == 0 ? 0 : 1)};
    return *reinterpret_cast<float const*>(foo);
#endif
  };
  
  /** 
   * Extracts one double.
   */
  template <class Stream>
  double VAXLittleEndianFormat::ReadDouble(Stream* src)
  {
    char byteptr[8] = {0};
    src->read(byteptr, 8);
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    char foo[8] = {byteptr[1] - 1 * (byteptr[1] == 0 ? 0 : 1), byteptr[0], byteptr[3], byteptr[2], byteptr[5], byteptr[4], byteptr[7], byteptr[6]};
    return *reinterpret_cast<double const*>(foo);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
    return *reinterpret_cast<double const*>(byteptr);
#else
    char foo[8] = {byteptr[6], byteptr[7], byteptr[4], byteptr[5], byteptr[2], byteptr[3], byteptr[0], byteptr[1] - 1 * (byteptr[1] == 0 ? 0 : 1)};
    return *reinterpret_cast<double const*>(foo);
#endif
  };

  /**
   * Writes the signed 16-bit integer @a i16 in the give stream @a dest.
   */
  template <class Stream>
  void VAXLittleEndianFormat::Write(int16_t val, Stream* dest)
  {
    char byteptr[2] = {0};
    memcpy(&byteptr, &val, sizeof(byteptr));
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    char foo[2] = {byteptr[1], byteptr[0]};
    dest->write(foo, 2);
#else
    dest->write(byteptr, 2);
#endif
  };
  
  /**
   * Writes the unsigned 16-bit integer @a u16 in the give stream @a dest.
   */
  template <class Stream>
  void VAXLittleEndianFormat::Write(uint16_t val, Stream* dest)
  {
    char byteptr[2] = {0};
    memcpy(&byteptr, &val, sizeof(byteptr));
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    char foo[2] = {byteptr[1], byteptr[0]};
    dest->write(foo, 2); 
#else
    dest->write(byteptr, 2);
#endif
  };
  
  /** 
   * Write the 32-bit signed integer @a val in the give stream @a dest.
   */
  template <class Stream>
  void VAXLittleEndianFormat::Write(int32_t val, Stream* dest)
  {
    char byteptr[4] = {0};
    memcpy(&byteptr, &val, sizeof(byteptr));
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    char foo[4] = {byteptr[1], byteptr[0], byteptr[3], byteptr[2]};
    dest->write(foo, 4);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
    dest->write(byteptr, 4);
#else
    char foo[4] = {byteptr[2], byteptr[3], byteptr[0], byteptr[1]};
    dest->write(foo, 4);
#endif
  };
  
  /** 
   * Write the 32-bit unsigned integer @a val in the give stream @a dest.
   */
  template <class Stream>
  void VAXLittleEndianFormat::Write(uint32_t val, Stream* dest)
  {
    char byteptr[4] = {0};
    memcpy(&byteptr, &val, sizeof(byteptr));
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    char foo[4] = {byteptr[1], byteptr[0], byteptr[3], byteptr[2]};
    dest->write(foo, 4);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
    dest->write(byteptr, 4);
#else
    char foo[4] = {byteptr[2], byteptr[3], byteptr[0], byteptr[1]};
    dest->write(foo, 4);
#endif
  };

  /**
   * Writes the float @a f in the give stream @a dest.
   */
  template <class Stream>
  void VAXLittleEndianFormat::Write(float val, Stream* dest)
  {
    char byteptr[4] = {0};
    memcpy(&byteptr, &val, sizeof(byteptr));
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    char foo[4] = {byteptr[1], byteptr[0] + 1 * (byteptr[0] == 0 ? 0 : 1),  byteptr[3], byteptr[2]};
    dest->write(foo, 4);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
    dest->write(byteptr, 4);
#else
    char foo[4] = {byteptr[2], byteptr[3] + 1 * (byteptr[3] == 0 ? 0 : 1), byteptr[0], byteptr[1]};
    dest->write(foo, 4);
#endif
  };
  
  // ----------------------------------------------------------------------- //
  
  /** 
   * @class IEEEBigEndianFormat btkBinaryByteOrderFormat.h
   * @brief Class to read and write data encoded from a IEEE (BE) to a 
   * VAX (LE) and IEEE (LE, BE) processor
   *
   * @sa VAXLittleEndianFormat, IEEELittleEndianFormat
   */
  
  /** 
   * Extracts one signed 16-bit integer.
   */
  template <class Stream>
  int16_t IEEEBigEndianFormat::ReadI16(Stream* src)
  {
    char byteptr[2] = {0};
    src->read(byteptr, 2);
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    return *reinterpret_cast<int16_t const*>(byteptr);
#else
    char foo[2] = {byteptr[1], byteptr[0]};
    return *reinterpret_cast<int16_t const*>(foo);
#endif
  };
  
  /** 
   * Extracts one unsigned 16-bit integer.
   */
  template <class Stream>
  uint16_t IEEEBigEndianFormat::ReadU16(Stream* src)
  {
    char byteptr[2] = {0};
    src->read(byteptr, 2);
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    return *reinterpret_cast<uint16_t const*>(byteptr);
#else
    char foo[2] = {byteptr[1], byteptr[0]};
    return *reinterpret_cast<uint16_t const*>(foo);
#endif
  };
  
  /** 
   * Extracts one signed 32-bit integer.
   */
  template <class Stream>
  int32_t IEEEBigEndianFormat::ReadI32(Stream* src)
  {
    char byteptr[4] = {0};
    src->read(byteptr, 4);
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    return *reinterpret_cast<int32_t const*>(byteptr);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
    char foo[4] = {byteptr[1], byteptr[0], byteptr[3], byteptr[2]};
    return *reinterpret_cast<int32_t const*>(foo);
#else
    char foo[4] = {byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    return *reinterpret_cast<int32_t const*>(foo);
#endif
  };
  
  /** 
   * Extracts one unsigned 32-bit integer.
   */
  template <class Stream>
  uint32_t IEEEBigEndianFormat::ReadU32(Stream* src)
  {
    char byteptr[4] = {0};
    src->read(byteptr, 4);
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    return *reinterpret_cast<uint32_t const*>(byteptr);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
    char foo[4] = {byteptr[1], byteptr[0], byteptr[3], byteptr[2]};
    return *reinterpret_cast<uint32_t const*>(foo);
#else
    char foo[4] = {byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    return *reinterpret_cast<uint32_t const*>(foo);
#endif
  };
  
  /** 
   * Extracts one signed 64-bit integer.
   */
  template <class Stream>
  int64_t IEEEBigEndianFormat::ReadI64(Stream* src)
  {
    char byteptr[8] = {0};
    src->read(byteptr, 8);
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    return *reinterpret_cast<int64_t const*>(byteptr);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
    char foo[8] = {byteptr[1], byteptr[0], byteptr[3], byteptr[2], byteptr[5], byteptr[4], byteptr[7], byteptr[6]};
    return *reinterpret_cast<int64_t const*>(foo);
#else
    char foo[8] = {byteptr[7], byteptr[6], byteptr[5], byteptr[4], byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    return *reinterpret_cast<int64_t const*>(foo);
#endif
  };
  
  /** 
   * Extracts one unsigned 64-bit integer.
   */
  template <class Stream>
  uint64_t IEEEBigEndianFormat::ReadU64(Stream* src)
  {
    char byteptr[8] = {0};
    src->read(byteptr, 8);
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    return *reinterpret_cast<uint64_t const*>(byteptr);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
    char foo[8] = {byteptr[1], byteptr[0], byteptr[3], byteptr[2], byteptr[5], byteptr[4], byteptr[7], byteptr[6]};
    return *reinterpret_cast<uint64_t const*>(foo);
#else
    char foo[8] = {byteptr[7], byteptr[6], byteptr[5], byteptr[4], byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    return *reinterpret_cast<uint64_t const*>(foo);
#endif
  };
  
  /** 
   * Extracts one float.
   */
  template <class Stream>
  float IEEEBigEndianFormat::ReadFloat(Stream* src)
  {
    char byteptr[4] = {0};
    src->read(byteptr, 4);
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    return *reinterpret_cast<float const*>(byteptr);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
    char foo[4] = {byteptr[1], byteptr[0] + 1 * (byteptr[0] == 0 ? 0 : 1), byteptr[3], byteptr[2]};
    return *reinterpret_cast<float const*>(foo);
#else
    char foo[4] = {byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    return *reinterpret_cast<float const*>(foo);
#endif
  };
  
  /** 
   * Extracts one double.
   */
  template <class Stream>
  double IEEEBigEndianFormat::ReadDouble(Stream* src)
  {
    char byteptr[8] = {0};
    src->read(byteptr, 8);
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    return *reinterpret_cast<double const*>(byteptr);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
    char foo[8] = {byteptr[1], byteptr[0] + 1 * (byteptr[0] == 0 ? 0 : 1), byteptr[3], byteptr[2], byteptr[5], byteptr[4], byteptr[7], byteptr[6]};
    return *reinterpret_cast<double const*>(foo);
#else
    char foo[8] = {byteptr[7], byteptr[6], byteptr[5], byteptr[4], byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    return *reinterpret_cast<double const*>(foo);
#endif
  };

  /**
   * Writes the signed 16-bit integer @a i16 in the give stream @a dest.
   */
  template <class Stream>
  void IEEEBigEndianFormat::Write(int16_t i16, Stream* dest)
  {
    char byteptr[2] = {0};
    memcpy(&byteptr, &i16, sizeof(byteptr));
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    dest->write(byteptr, 2); 
#else
    char foo[2] = {byteptr[1], byteptr[0]};
    dest->write(foo, 2);
#endif
  };

  /**
   * Writes the unsigned 16-bit integer @a u16 in the give stream @a dest.
   */
  template <class Stream>
  void IEEEBigEndianFormat::Write(uint16_t u16, Stream* dest)
  {
    char byteptr[2] = {0};
    memcpy(&byteptr, &u16, sizeof(byteptr));
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    dest->write(byteptr, 2); 
#else
    char foo[2] = {byteptr[1], byteptr[0]};
    dest->write(foo, 2);
#endif
  };
  
  /** 
   * Write the 32-bit signed integer @a val in the give stream @a dest.
   */
  template <class Stream>
  void IEEEBigEndianFormat::Write(int32_t val, Stream* dest)
  {
    char byteptr[4] = {0};
    memcpy(&byteptr, &val, sizeof(byteptr));
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    dest->write(byteptr, 4);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
    char foo[4] = {byteptr[1], byteptr[0], byteptr[3], byteptr[2]};
    dest->write(foo, 4);
#else
    char foo[4] = {byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    dest->write(foo, 4);
#endif
  };
  
  /** 
   * Write the 32-bit unsigned integer @a val in the give stream @a dest.
   */
  template <class Stream>
  void IEEEBigEndianFormat::Write(uint32_t val, Stream* dest)
  {
    char byteptr[4] = {0};
    memcpy(&byteptr, &val, sizeof(byteptr));
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    dest->write(byteptr, 4);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
    char foo[4] = {byteptr[1], byteptr[0], byteptr[3], byteptr[2]};
    dest->write(foo, 4);
#else
    char foo[4] = {byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    dest->write(foo, 4);
#endif
  };

  /**
   * Writes the float @a val in the give stream @a dest.
   */
  template <class Stream>
  void IEEEBigEndianFormat::Write(float val, Stream* dest)
  {
    char byteptr[4] = {0};
    memcpy(&byteptr, &val, sizeof(byteptr));
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    dest->write(byteptr, 4);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
    char foo[4] = {byteptr[1] - 1 * (byteptr[1] == 0 ? 0 : 1), byteptr[0], byteptr[3], byteptr[2]};
    dest->write(foo, 4);
#else
    char foo[4] = {byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    dest->write(foo, 4);
#endif
  };
  
  // ----------------------------------------------------------------------- //
  
  /** 
   * @class IEEELittleEndianFormat btkBinaryByteOrderFormat.h
   * @brief Class to read and write data encoded from a IEEE (LE) to a 
   * VAX (LE) and IEEE (LE, BE) processor.
   *
   * @sa VAXLittleEndianFormat, IEEEBigEndianFormat
   */
  
  /** 
   * Extracts one signed 16-bit integer.
   */
  template <class Stream>
  int16_t IEEELittleEndianFormat::ReadI16(Stream* src)
  {
    char byteptr[2] = {0};
    src->read(byteptr, 2);
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    char foo[2] = {byteptr[1], byteptr[0]};
    return *reinterpret_cast<int16_t const*>(foo);
#else
    return *reinterpret_cast<int16_t const*>(byteptr);
#endif
  };
  
  /** 
   * Extracts one unsigned 16-bit integer.
   */
  template <class Stream>
  uint16_t IEEELittleEndianFormat::ReadU16(Stream* src)
  {
    char byteptr[2] = {0};
    src->read(byteptr, 2);
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    char foo[2] = {byteptr[1], byteptr[0]};
    return *reinterpret_cast<uint16_t const*>(foo);
#else
    return *reinterpret_cast<uint16_t const*>(byteptr);
#endif
  };
  
  /** 
   * Extracts one signed 32-bit integer.
   */
  template <class Stream>
  int32_t IEEELittleEndianFormat::ReadI32(Stream* src)
  {
    char byteptr[4] = {0};
    src->read(byteptr, 4);
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    char foo[4] = {byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    return *reinterpret_cast<int32_t const*>(foo);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */ 
    char foo[4] = {byteptr[2], byteptr[3], byteptr[0], byteptr[1]};
    return *reinterpret_cast<int32_t const*>(foo);
#else
    return *reinterpret_cast<int32_t const*>(byteptr);
#endif
  };

  /** 
   * Extracts one unsigned 32-bit integer.
   */
  template <class Stream>
  uint32_t IEEELittleEndianFormat::ReadU32(Stream* src)
  {
    char byteptr[4] = {0};
    src->read(byteptr, 4);
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    char foo[4] = {byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    return *reinterpret_cast<uint32_t const*>(foo);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */ 
    char foo[4] = {byteptr[2], byteptr[3], byteptr[0], byteptr[1]};
    return *reinterpret_cast<uint32_t const*>(foo);
#else
    return *reinterpret_cast<uint32_t const*>(byteptr);
#endif
  };
  
  /** 
   * Extracts one signed 64-bit integer.
   */
  template <class Stream>
  int64_t IEEELittleEndianFormat::ReadI64(Stream* src)
  {
    char byteptr[8] = {0};
    src->read(byteptr, 8);
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    char foo[8] = {byteptr[7], byteptr[6], byteptr[5], byteptr[6], byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    return *reinterpret_cast<int64_t const*>(foo);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */ 
    char foo[8] = {byteptr[6], byteptr[7], byteptr[4], byteptr[5], byteptr[2], byteptr[3], byteptr[0], byteptr[1]};
    return *reinterpret_cast<int64_t const*>(foo);
#else
    return *reinterpret_cast<int64_t const*>(byteptr);
#endif
  };

  /** 
   * Extracts one unsigned 64-bit integer.
   */
  template <class Stream>
  uint64_t IEEELittleEndianFormat::ReadU64(Stream* src)
  {
    char byteptr[8] = {0};
    src->read(byteptr, 8);
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    char foo[8] = {byteptr[7], byteptr[6], byteptr[5], byteptr[6], byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    return *reinterpret_cast<uint64_t const*>(foo);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */ 
    char foo[8] = {byteptr[6], byteptr[7], byteptr[4], byteptr[5], byteptr[2], byteptr[3], byteptr[0], byteptr[1]};
    return *reinterpret_cast<uint64_t const*>(foo);
#else
    return *reinterpret_cast<uint64_t const*>(byteptr);
#endif
  };

  /** 
   * Extracts one float.
   */
  template <class Stream>
  float IEEELittleEndianFormat::ReadFloat(Stream* src)
  {
    char byteptr[4] = {0};
    src->read(byteptr, 4);
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    char foo[4] = {byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    return *reinterpret_cast<float const*>(foo);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */ 
    char foo[4] = {byteptr[2], byteptr[3] + 1 * (byteptr[3] == 0 ? 0 : 1), byteptr[0], byteptr[1]};
    return *reinterpret_cast<float const*>(foo);
#else
    return *reinterpret_cast<float const*>(byteptr);
#endif
  };
  
  /** 
   * Extracts one float.
   */
  template <class Stream>
  double IEEELittleEndianFormat::ReadDouble(Stream* src)
  {
    char byteptr[8] = {0};
    src->read(byteptr, 8);
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    char foo[8] = {byteptr[7], byteptr[6], byteptr[5], byteptr[4], byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    return *reinterpret_cast<double const*>(foo);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */ 
    char foo[8] = {byteptr[6], byteptr[7] + 1 * (byteptr[7] == 0 ? 0 : 1), byteptr[4], byteptr[5], byteptr[2], byteptr[3], byteptr[1], byteptr[0]};
    return *reinterpret_cast<double const*>(foo);
#else
    return *reinterpret_cast<double const*>(byteptr);
#endif
  };

  /**
   * Writes the signed 16-bit integer @a i16 in the give stream @a dest.
   */
  template <class Stream>
  void IEEELittleEndianFormat::Write(int16_t i16, Stream* dest)
  {
    char byteptr[2] = {0};
    memcpy(&byteptr, &i16, sizeof(byteptr));
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    char foo[2] = {byteptr[1], byteptr[0]};
    dest->write(foo, 2); 
#else
    dest->write(byteptr, 2);
#endif
  };

  /**
   * Writes the unsigned 16-bit integer @a u16 in the give stream @a dest.
   */
  template <class Stream>
  void IEEELittleEndianFormat::Write(uint16_t u16, Stream* dest)
  {
    char byteptr[2] = {0};
    memcpy(&byteptr, &u16, sizeof(byteptr));
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    char foo[2] = {byteptr[1], byteptr[0]};
    dest->write(foo, 2); 
#else
    dest->write(byteptr, 2);
#endif
  };

  /** 
   * Write the 32-bit signed integer @a val in the give stream @a dest.
   */
  template <class Stream>
  void IEEELittleEndianFormat::Write(int32_t val, Stream* dest)
  {
    char byteptr[4] = {0};
    memcpy(&byteptr, &val, sizeof(byteptr));
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    char foo[4] = {byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    dest->write(foo, 4);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */ 
    char foo[4] = {byteptr[2], byteptr[3], byteptr[0], byteptr[1]};
    dest->write(foo, 4);
#else
    dest->write(byteptr, 4);
#endif
  };
  
  /** 
   * Write the 32-bit unsigned integer @a val in the give stream @a dest
   */
  template <class Stream>
  void IEEELittleEndianFormat::Write(uint32_t val, Stream* dest)
  {
    char byteptr[4] = {0};
    memcpy(&byteptr, &val, sizeof(byteptr));
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    char foo[4] = {byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    dest->write(foo, 4);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */ 
    char foo[4] = {byteptr[2], byteptr[3], byteptr[0], byteptr[1]};
    dest->write(foo, 4);
#else
    dest->write(byteptr, 4);
#endif
  };

  /**
   * Writes the float @a val in the give stream @a dest.
   */
  template <class Stream>
  void IEEELittleEndianFormat::Write(float val, Stream* dest)
  {
    char byteptr[4] = {0};
    memcpy(&byteptr, &val, sizeof(byteptr));
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    char foo[4] = {byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    dest->write(foo, 4);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
    char foo[4] = {byteptr[2], byteptr[3], byteptr[0], byteptr[1] - 1 * (byteptr[1] == 0 ? 0 : 1)};
    dest->write(foo, 4);
#else
    dest->write(byteptr, 4);
#endif
  };
};

#endif // __btkByteOrderFormat_h
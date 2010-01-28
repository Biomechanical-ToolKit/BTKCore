/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009, Arnaud BarrÈ
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 *   * Redistributions of source code must retain the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials
 *     provided with the distribution.
 *   * Neither the name(s) of the copyright holders nor the names
 *     of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written
 *     permission.
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

#ifndef __btkBinaryFileStream_h
#define __btkBinaryFileStream_h

#if defined _MSC_VER
  #if defined _M_IX86
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
  #error Development plateform not supported
#endif

#include <fstream>
#include <string>
#include <vector>

#ifdef _MSC_VER
  #include "../../Utilities/stdint.h"
#else
  #include <stdint.h>
#endif

namespace btk
{
  class BinaryFileStream
  {
  public:
    explicit BinaryFileStream(std::fstream& fstream) : mr_Fstream(fstream) {};
    
    virtual ~BinaryFileStream()
    {
      if (this->mr_Fstream.is_open())
        this->mr_Fstream.close();
    };

    virtual char ReadChar();
    virtual const std::vector<char> ReadChar(size_t nb);
    virtual int8_t ReadI8();
    virtual const std::vector<int8_t> ReadI8(size_t nb);
    virtual uint8_t ReadU8();
    virtual const std::vector<uint8_t> ReadU8(size_t nb);
    virtual int16_t ReadI16() = 0;
    virtual const std::vector<int16_t> ReadI16(size_t nb);
    virtual uint16_t ReadU16() = 0;
    virtual const std::vector<uint16_t> ReadU16(size_t nb);
    virtual float ReadFloat() = 0;
    virtual const std::vector<float> ReadFloat(size_t nb);
    virtual const std::string ReadString(size_t nbChar);
    virtual const std::vector<std::string> ReadString(size_t nb, size_t nbChar);
    virtual void SeekRead(size_t nb, std::ios_base::seekdir dir);
    
    virtual size_t Fill(size_t nb);
    virtual void SeekWrite(size_t nb, std::ios_base::seekdir dir);
    // Note: MSVC doesn't like the following commented methods.
    //       char and int8_t are the same for it...
    //virtual size_t Write(char c);
    //virtual size_t Write(const std::vector<char>& rVectorChar);
    virtual size_t Write(int8_t i8);
    virtual size_t Write(const std::vector<int8_t>& rVectorI8);
    virtual size_t Write(uint8_t u8);
    virtual size_t Write(const std::vector<uint8_t>& rVectorU8);
    virtual size_t Write(int16_t i16) = 0;
    virtual size_t Write(const std::vector<int16_t>& rVectorI16);
    virtual size_t Write(uint16_t u16) = 0;
    virtual size_t Write(const std::vector<uint16_t>& rVectorU16);
    virtual size_t Write(float f) = 0;
    virtual size_t Write(const std::vector<float>& rVectorFloat);
    virtual size_t Write(const std::string& rString);
    virtual size_t Write(const std::vector<std::string>& rVectorString);
  
  protected:
    std::fstream& mr_Fstream;

  private:
    BinaryFileStream(const BinaryFileStream& ); // Not implemented.
    BinaryFileStream& operator=(const BinaryFileStream& ); // Not implemented.
  };

  class VAXLittleEndianBinaryFileStream : public BinaryFileStream
  {  
  public:
    explicit VAXLittleEndianBinaryFileStream(std::fstream& rFsteam) : BinaryFileStream(rFsteam) {};
    // ~VAXLittleEndianBinaryFileStream(); // Implicit.  
    virtual int16_t ReadI16();
    using BinaryFileStream::ReadI16;
    virtual uint16_t ReadU16();
    using BinaryFileStream::ReadU16;
    virtual float ReadFloat();
    using BinaryFileStream::ReadFloat;
    virtual size_t Write(int16_t i16);
    virtual size_t Write(uint16_t u16);
    virtual size_t Write(float f);
    using BinaryFileStream::Write;
  
  private:
    VAXLittleEndianBinaryFileStream(const VAXLittleEndianBinaryFileStream& ); // Not implemented.
    VAXLittleEndianBinaryFileStream& operator=(const VAXLittleEndianBinaryFileStream& ); // Not implemented.
  };

  class IEEELittleEndianBinaryFileStream : public BinaryFileStream
  {  
  public:
    explicit IEEELittleEndianBinaryFileStream(std::fstream& rFsteam) : BinaryFileStream(rFsteam) {};
    // ~IEEELittleEndianBinaryFileStream(); // Implicit.  
    virtual int16_t ReadI16(); 
    using BinaryFileStream::ReadI16;
    virtual uint16_t ReadU16();
    using BinaryFileStream::ReadU16;
    virtual float ReadFloat();
    using BinaryFileStream::ReadFloat;
    virtual size_t Write(int16_t i16);
    virtual size_t Write(uint16_t u16);
    virtual size_t Write(float f);
    using BinaryFileStream::Write;
  
  private:
    IEEELittleEndianBinaryFileStream(const IEEELittleEndianBinaryFileStream& ); // Not implemented.
    IEEELittleEndianBinaryFileStream& operator=(const IEEELittleEndianBinaryFileStream& ); // Not implemented.
  };
  
  class IEEEBigEndianBinaryFileStream : public BinaryFileStream
  {  
  public:
    explicit IEEEBigEndianBinaryFileStream(std::fstream& rFsteam) : BinaryFileStream(rFsteam) {};
    // ~IEEEBigEndianBinaryFileStream(); // Implicit.  
    virtual int16_t ReadI16();
    using BinaryFileStream::ReadI16;
    virtual uint16_t ReadU16();
    using BinaryFileStream::ReadU16;
    virtual float ReadFloat();
    using BinaryFileStream::ReadFloat;
    virtual size_t Write(int16_t i16);
    virtual size_t Write(uint16_t u16);
    virtual size_t Write(float f);
    using BinaryFileStream::Write;
  
  private:
    IEEEBigEndianBinaryFileStream(const IEEEBigEndianBinaryFileStream& ); // Not implemented.
    IEEEBigEndianBinaryFileStream& operator=(const IEEEBigEndianBinaryFileStream& ); // Not implemented.
  };
};

#endif // __btkBinaryFileStream_h

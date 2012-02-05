/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2012, Arnaud Barré
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

#include "btkConfigure.h"

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

// Check if we can use the memory mapped filestream system
#if defined HAVE_SYS_MMAP || defined _MSC_VER
  #include "btkBinaryFileStream_mmfstream_p.h"
  namespace btk {typedef btk::mmfstream RawFileStream;};
#else
  #define BTK_NO_MEMORY_MAPPED_FILESTREAM
  #include <fstream>
  namespace btk {typedef std::fstream RawFileStream;};
#endif

#include <string>
#include <vector>

// MSVC doesn't have the header stdint.h
#ifdef _MSC_VER
  #include "Utilities/stdint.h"
#else
  #include <stdint.h>
#endif

// btkChooseNativeBinaryFileStream macro
#if PROCESSOR_TYPE == 1
  #define btkChooseNativeBinaryFileStream IEEELittleEndianBinaryFileStream
#elif PROCESSOR_TYPE == 2
  #define btkChooseNativeBinaryFileStream VAXLittleEndianBinaryFileStream
#elif PROCESSOR_TYPE == 3
  #define btkChooseNativeBinaryFileStream IEEEBigEndianBinaryFileStream
#else
  #error Unknown processor type
#endif

namespace btk
{
  typedef RawFileStream::failure BinaryFileStreamException;
  
  class BinaryFileStream
  {
  public:
    typedef std::ios_base::iostate IOState;
    static const IOState EndFileBit = std::ios_base::eofbit;
    static const IOState FailBit = std::ios_base::failbit;
    static const IOState BadBit = std::ios_base::badbit;
    static const IOState GoodBit = std::ios_base::goodbit;
    
    typedef std::ios_base::openmode OpenMode;
    static const OpenMode In = std::ios_base::in;
    static const OpenMode Out = std::ios_base::out;
    static const OpenMode Truncate = std::ios_base::trunc;
    
    typedef std::ios_base::seekdir SeekDir;
    static const SeekDir Begin = std::ios_base::beg;
    static const SeekDir Current = std::ios_base::cur;
    static const SeekDir End = std::ios_base::end;
    
    typedef std::streampos StreamPosition;
    typedef std::streamoff StreamOffset;
    
    virtual ~BinaryFileStream()
    {
      if (this->mp_Stream->is_open())
        this->mp_Stream->close();
      delete this->mp_Stream;
    };
    
    void Open(const std::string& filename, OpenMode mode) {this->mp_Stream->open(filename.c_str(), std::ios_base::binary | mode);};
    bool IsOpen() const {return this->mp_Stream->is_open();};
    bool Good() const {return this->mp_Stream->good();};
    void Close() {this->mp_Stream->close();};
    bool EndFile() const {return this->mp_Stream->eof();};
    bool Bad() const {return this->mp_Stream->bad();};
    bool Fail() const {return this->mp_Stream->fail();};
    IOState GetExceptions() {return this->mp_Stream->exceptions();};
    void SetExceptions(IOState except) {this->mp_Stream->exceptions(except);};
    void Clear(IOState flags = GoodBit) {return this->mp_Stream->clear(flags);};
    
    const RawFileStream* GetStream() const {return this->mp_Stream;};
    void SwapStream(BinaryFileStream* toSwap);
    
    char ReadChar();
    const std::vector<char> ReadChar(size_t nb);
    int8_t ReadI8();
    const std::vector<int8_t> ReadI8(size_t nb);
    uint8_t ReadU8();
    const std::vector<uint8_t> ReadU8(size_t nb);
    virtual int16_t ReadI16() = 0;
    const std::vector<int16_t> ReadI16(size_t nb);
    virtual uint16_t ReadU16() = 0;
    const std::vector<uint16_t> ReadU16(size_t nb);
    virtual int32_t ReadI32() = 0;
    const std::vector<int32_t> ReadI32(size_t nb);
    virtual uint32_t ReadU32() = 0;
    const std::vector<uint32_t> ReadU32(size_t nb);
    
    virtual float ReadFloat() = 0;
    const std::vector<float> ReadFloat(size_t nb);
    virtual double ReadDouble() = 0;
    const std::vector<double> ReadDouble(size_t nb);
    const std::string ReadString(size_t nbChar);
    const std::vector<std::string> ReadString(size_t nb, size_t nbChar);
    void SeekRead(StreamOffset offset, SeekDir dir) {this->mp_Stream->seekg(offset, dir);};
    StreamPosition TellRead() const {return this->mp_Stream->tellg();};
    
    size_t Fill(size_t nb);
    void SeekWrite(StreamOffset offset, SeekDir dir) {this->mp_Stream->seekp(offset, dir);};
    // Note: MSVC doesn't like the following commented methods.
    //       char and int8_t are the same for it...
    //virtual size_t Write(char c);
    //virtual size_t Write(const std::vector<char>& rVectorChar);
    size_t Write(int8_t i8);
    size_t Write(const std::vector<int8_t>& rVectorI8);
    size_t Write(uint8_t u8);
    size_t Write(const std::vector<uint8_t>& rVectorU8);
    virtual size_t Write(int16_t i16) = 0;
    size_t Write(const std::vector<int16_t>& rVectorI16);
    virtual size_t Write(uint16_t u16) = 0;
    size_t Write(const std::vector<uint16_t>& rVectorU16);
    virtual size_t Write(float f) = 0;
    size_t Write(const std::vector<float>& rVectorFloat);
    size_t Write(const std::string& rString);
    size_t Write(const std::vector<std::string>& rVectorString);
  
  protected:
    BinaryFileStream() {this->mp_Stream = new RawFileStream();};
    BinaryFileStream(const std::string& filename, OpenMode mode) {this->mp_Stream = new RawFileStream(filename.c_str(), std::ios_base::binary | mode);};
    
    RawFileStream* mp_Stream;

  private:
    BinaryFileStream(const BinaryFileStream& ); // Not implemented.
    BinaryFileStream& operator=(const BinaryFileStream& ); // Not implemented.
  };

  class VAXLittleEndianBinaryFileStream : public BinaryFileStream
  {  
  public:
    VAXLittleEndianBinaryFileStream() : BinaryFileStream() {};
    VAXLittleEndianBinaryFileStream(const std::string& filename, OpenMode mode) : BinaryFileStream(filename, mode) {};
    // ~VAXLittleEndianBinaryFileStream(); // Implicit.  
    virtual int16_t ReadI16();
    using BinaryFileStream::ReadI16;
    virtual uint16_t ReadU16();
    using BinaryFileStream::ReadU16;
    virtual int32_t ReadI32(); 
    using BinaryFileStream::ReadI32;
    virtual uint32_t ReadU32();
    using BinaryFileStream::ReadU32;
    virtual float ReadFloat();
    using BinaryFileStream::ReadFloat;
    virtual double ReadDouble();
    using BinaryFileStream::ReadDouble;
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
    IEEELittleEndianBinaryFileStream() : BinaryFileStream() {};
    IEEELittleEndianBinaryFileStream(const std::string& filename, OpenMode mode) : BinaryFileStream(filename, mode) {};
    // ~IEEELittleEndianBinaryFileStream(); // Implicit.  
    virtual int16_t ReadI16(); 
    using BinaryFileStream::ReadI16;
    virtual uint16_t ReadU16();
    using BinaryFileStream::ReadU16;
    virtual int32_t ReadI32(); 
    using BinaryFileStream::ReadI32;
    virtual uint32_t ReadU32();
    using BinaryFileStream::ReadU32;
    virtual float ReadFloat();
    using BinaryFileStream::ReadFloat;
    virtual double ReadDouble();
    using BinaryFileStream::ReadDouble;
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
    IEEEBigEndianBinaryFileStream() : BinaryFileStream() {};
    IEEEBigEndianBinaryFileStream(const std::string& filename, OpenMode mode) : BinaryFileStream(filename, mode) {};
    // ~IEEEBigEndianBinaryFileStream(); // Implicit.  
    virtual int16_t ReadI16();
    using BinaryFileStream::ReadI16;
    virtual uint16_t ReadU16();
    using BinaryFileStream::ReadU16;
    virtual int32_t ReadI32(); 
    using BinaryFileStream::ReadI32;
    virtual uint32_t ReadU32();
    using BinaryFileStream::ReadU32;
    virtual float ReadFloat();
    using BinaryFileStream::ReadFloat;
    virtual double ReadDouble();
    using BinaryFileStream::ReadDouble;
    virtual size_t Write(int16_t i16);
    virtual size_t Write(uint16_t u16);
    virtual size_t Write(float f);
    using BinaryFileStream::Write;
  
  private:
    IEEEBigEndianBinaryFileStream(const IEEEBigEndianBinaryFileStream& ); // Not implemented.
    IEEEBigEndianBinaryFileStream& operator=(const IEEEBigEndianBinaryFileStream& ); // Not implemented.
  };
  
  class NativeBinaryFileStream : public btkChooseNativeBinaryFileStream
  {
  public:
    NativeBinaryFileStream() : btkChooseNativeBinaryFileStream() {};
    NativeBinaryFileStream(const std::string& filename, OpenMode mode) : btkChooseNativeBinaryFileStream(filename, mode) {};
    // ~NativeBinaryFileStream(); // Implicit.
  private:
    NativeBinaryFileStream(const NativeBinaryFileStream& ); // Not implemented.
    NativeBinaryFileStream& operator=(const NativeBinaryFileStream& ); // Not implemented.
  };
};

#endif // __btkBinaryFileStream_h

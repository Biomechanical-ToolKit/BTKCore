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

#ifndef __btkBinaryFileStream_h
#define __btkBinaryFileStream_h

#include "btkConfigure.h"

#include "btkBinaryStream.h"
#include "btkBinaryByteOrderFormat.h"

// Check if we can use the memory mapped file stream system
#if defined HAVE_SYS_MMAP || defined _MSC_VER
  #include "btkBinaryFileStream_mmfstream.h"
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

// _btk_choose_native_binary_file_stream macro
#if PROCESSOR_TYPE == 1
  #define _btk_choose_native_binary_file_stream IEEELittleEndianBinaryFileStream
#elif PROCESSOR_TYPE == 2
  #define _btk_choose_native_binary_file_stream VAXLittleEndianBinaryFileStream
#elif PROCESSOR_TYPE == 3
  #define _btk_choose_native_binary_file_stream IEEEBigEndianBinaryFileStream
#else
  #error Unknown processor type
#endif

namespace btk
{
  typedef RawFileStream::failure BinaryFileStreamFailure;
  
  class BinaryFileStream : public BinaryStream<BinaryFileStream>
  {
  public:
    static const IOState EndFileBit = std::ios_base::eofbit;
    
    typedef std::ios_base::openmode OpenMode;
    static const OpenMode In = std::ios_base::in;
    static const OpenMode Out = std::ios_base::out;
    static const OpenMode Truncate = std::ios_base::trunc;
    
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
    BTK_IO_EXPORT void SwapStream(BinaryFileStream* toSwap);
    
    BTK_IO_EXPORT char ReadChar();
    using BinaryStream::ReadChar;
    
    BTK_IO_EXPORT int8_t ReadI8();
    using BinaryStream::ReadI8;
    
    BTK_IO_EXPORT uint8_t ReadU8();
    using BinaryStream::ReadU8;
    
    virtual int16_t ReadI16() = 0;
    using BinaryStream::ReadI16;
    
    virtual uint16_t ReadU16() = 0;
    using BinaryStream::ReadU16;
    
    virtual int32_t ReadI32() = 0;
    using BinaryStream::ReadI32;
    
    virtual uint32_t ReadU32() = 0;
    using BinaryStream::ReadU32;
    
    virtual int64_t ReadI64() = 0;
    using BinaryStream::ReadI64;
    
    virtual uint64_t ReadU64() = 0;
    using BinaryStream::ReadU64;
    
    virtual float ReadFloat() = 0;
    using BinaryStream::ReadFloat;
    
    virtual double ReadDouble() = 0;
    using BinaryStream::ReadDouble;
    
    BTK_IO_EXPORT std::string ReadString(size_t nbChar);
    using BinaryStream::ReadString;
    
    void SeekRead(StreamOffset offset, SeekDir dir) {this->mp_Stream->seekg(offset, dir);};
    StreamPosition TellRead() const {return this->mp_Stream->tellg();};
    
    BTK_IO_EXPORT size_t Fill(size_t nb);
    void SeekWrite(StreamOffset offset, SeekDir dir) {this->mp_Stream->seekp(offset, dir);};
    
    BTK_IO_EXPORT size_t Write(int8_t value);
    BTK_IO_EXPORT size_t Write(uint8_t value);
    virtual size_t Write(int16_t value) = 0;
    virtual size_t Write(uint16_t value) = 0;
    virtual size_t Write(int32_t value) = 0;
    virtual size_t Write(uint32_t value) = 0;
    virtual size_t Write(float value) = 0;
    BTK_IO_EXPORT size_t Write(const std::string& value);
    using BinaryStream::Write;
  
  protected:
    BinaryFileStream() {this->mp_Stream = new RawFileStream();};
    BinaryFileStream(const std::string& filename, OpenMode mode) {this->mp_Stream = new RawFileStream(filename.c_str(), std::ios_base::binary | mode);};
    
    RawFileStream* mp_Stream;

  private:
    BinaryFileStream(const BinaryFileStream& ); // Not implemented.
    BinaryFileStream& operator=(const BinaryFileStream& ); // Not implemented.
  };

  template <class Format>
  class ByteOrderBinaryFileStream : public BinaryFileStream
  {
  public:
    ByteOrderBinaryFileStream() : BinaryFileStream() {};
    ByteOrderBinaryFileStream(const std::string& filename, OpenMode mode) : BinaryFileStream(filename, mode) {};
    // ~ByteOrderBinaryFileStream(); // Implicit.  
    BTK_IO_EXPORT virtual int16_t ReadI16();
    using BinaryFileStream::ReadI16;
    BTK_IO_EXPORT virtual uint16_t ReadU16();
    using BinaryFileStream::ReadU16;
    BTK_IO_EXPORT virtual int32_t ReadI32(); 
    using BinaryFileStream::ReadI32;
    BTK_IO_EXPORT virtual uint32_t ReadU32();
    using BinaryFileStream::ReadU32;
    BTK_IO_EXPORT virtual int64_t ReadI64(); 
    using BinaryFileStream::ReadI64;
    BTK_IO_EXPORT virtual uint64_t ReadU64();
    using BinaryFileStream::ReadU64;
    BTK_IO_EXPORT virtual float ReadFloat();
    using BinaryFileStream::ReadFloat;
    BTK_IO_EXPORT virtual double ReadDouble();
    using BinaryFileStream::ReadDouble;
    BTK_IO_EXPORT virtual size_t Write(int16_t value);
    BTK_IO_EXPORT virtual size_t Write(uint16_t value);
    BTK_IO_EXPORT virtual size_t Write(int32_t value);
    BTK_IO_EXPORT virtual size_t Write(uint32_t value);
    BTK_IO_EXPORT virtual size_t Write(float value);
    using BinaryFileStream::Write;
  
  private:
    ByteOrderBinaryFileStream(const ByteOrderBinaryFileStream& ); // Not implemented.
    ByteOrderBinaryFileStream& operator=(const ByteOrderBinaryFileStream& ); // Not implemented.
  };

  class VAXLittleEndianBinaryFileStream : public ByteOrderBinaryFileStream<VAXLittleEndianFormat>
  {
  public:
    VAXLittleEndianBinaryFileStream() : ByteOrderBinaryFileStream<VAXLittleEndianFormat>() {};
    VAXLittleEndianBinaryFileStream(const std::string& filename, OpenMode mode) : ByteOrderBinaryFileStream<VAXLittleEndianFormat>(filename, mode) {};
    // ~ByteOrderBinaryFileStream(); // Implicit.  
  private:
    VAXLittleEndianBinaryFileStream(const VAXLittleEndianBinaryFileStream& ); // Not implemented.
    VAXLittleEndianBinaryFileStream& operator=(const VAXLittleEndianBinaryFileStream& ); // Not implemented.
  };
  
  class IEEELittleEndianBinaryFileStream : public ByteOrderBinaryFileStream<IEEELittleEndianFormat>
  {
  public:
    IEEELittleEndianBinaryFileStream() : ByteOrderBinaryFileStream<IEEELittleEndianFormat>() {};
    IEEELittleEndianBinaryFileStream(const std::string& filename, OpenMode mode) : ByteOrderBinaryFileStream<IEEELittleEndianFormat>(filename, mode) {};
    // ~ByteOrderBinaryFileStream(); // Implicit.  
  private:
    IEEELittleEndianBinaryFileStream(const IEEELittleEndianBinaryFileStream& ); // Not implemented.
    IEEELittleEndianBinaryFileStream& operator=(const IEEELittleEndianBinaryFileStream& ); // Not implemented.
  };
  
  class IEEEBigEndianBinaryFileStream : public ByteOrderBinaryFileStream<IEEEBigEndianFormat>
  {
  public:
    IEEEBigEndianBinaryFileStream() : ByteOrderBinaryFileStream<IEEEBigEndianFormat>() {};
    IEEEBigEndianBinaryFileStream(const std::string& filename, OpenMode mode) : ByteOrderBinaryFileStream<IEEEBigEndianFormat>(filename, mode) {};
    // ~ByteOrderBinaryFileStream(); // Implicit.  
  private:
    IEEEBigEndianBinaryFileStream(const IEEEBigEndianBinaryFileStream& ); // Not implemented.
    IEEEBigEndianBinaryFileStream& operator=(const IEEEBigEndianBinaryFileStream& ); // Not implemented.
  };
      
  class NativeBinaryFileStream : public _btk_choose_native_binary_file_stream
  {
  public:
    NativeBinaryFileStream() : _btk_choose_native_binary_file_stream() {};
    NativeBinaryFileStream(const std::string& filename, OpenMode mode) : _btk_choose_native_binary_file_stream(filename, mode) {};
    // ~NativeBinaryFileStream(); // Implicit.
  private:
    NativeBinaryFileStream(const NativeBinaryFileStream& ); // Not implemented.
    NativeBinaryFileStream& operator=(const NativeBinaryFileStream& ); // Not implemented.
  };
};

#include "btkBinaryFileStream.tpp"

#endif // __btkBinaryFileStream_h

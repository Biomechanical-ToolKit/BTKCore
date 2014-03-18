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
#include "btkConfigure.h"

#include <cstring>

// If it is possible to use the memory file mapping mechanism, 
// then its code is compiled here 
#if !defined BTK_NO_MEMORY_MAPPED_FILESTREAM
  #include "btkBinaryFileStream_mmfstream.cpp"
#endif

namespace btk
{
  /**
   * @typedef RawFileStream
   * Raw file stream used by the btk::BinaryFileStream class. Depending
   * your configuration, this raw stream will correspond to btk::mmfstream
   * (default) or std::fstream. The class btk::mmfstream uses the memory 
   * mapped file mechanism.
   *
   * @warning It is not adviced to used this class directly. Even if you can 
   * potentially speedup data's reading by accessing directly to the buffer,
   * it is also a large source of error and crash (buffer overlfow, etc.)
   */
  
  /** 
   * @class BinaryFileStream btkBinaryFileStream.h
   * @brief An abstract class which read/write binary file in VAX and IEEE floating 
   * format with the corresponding byte order
   *
   * Especially, this file stream can:
   *   - read binary file encoded from a VAX (LE) and IEEE (LE, BE) processor ;
   *   - write binary file in the desired supported format from any supported processor format.
   *
   * The simplest way to use this binary file stream is to use the class btk::NativeBinaryFileStream.
   * It creates a stream to read/write binary file encoded in the format of the porcessor used to compile the code.
   * 
   * As the file stream classes in the standard library you can indicates if the file is to read or
   * write data. The following code presents the definion of input and output streams in binary format.
   * @code
   *    btk::NativeBinaryFileStreamfstream ifs, ofs;
   *    ifs.Open(filename, btk::BinaryFileStream::In); // read
   *    ofs.Open(filename, btk::BinaryFileStream::Out); // write
   * @endcode
   * 
   * This class has also exceptions. To use them, you have to set the exception mask.
   * For example:
   * @code
   *    btk::NativeBinaryFileStreamfstream ifs;
   *    ifs.SetExceptions (btk::BinaryFileStream::EndFileBit | btk::BinaryFileStream::FailBit | btk::BinaryFileStream::BadBit);
   * @endcode
   *
   * @sa VAXLittleEndianBinaryFileStream, IEEELittleEndianBinaryFileStream, IEEEBigEndianBinaryFileStream, NativeBinaryFileStream
   *
   * @ingroup BTKIO
   */
  /**
   * @typedef BinaryFileStreamFailure
   * Exception for the BinaryFileStream class and inherited classes.
   */
  /**
   * @typedef BinaryFileStream::IOState
   * Bitmask type to represent stream error state flags.
   */
  /**
   * @typedef BinaryFileStream::OpenMode
   * Flags describing the requested I/O mode for the file. 
   */
  /**
   * @typedef BinaryFileStream::SeekDir
   * Flags representing the seeking direction of a stream seeking operation.
   */
  /**
   * @typedef BinaryFileStream::StreamPosition
   * Type to represent positions in a stream.
   */
  /**
   * @typedef BinaryFileStream::StreamOffset
   * Type to represent position offsets in a stream.
   */
  /**
   * @var BinaryFileStream::EndFileBit
   * End-of-File reached while performing an extracting operation on an input stream.
   */
  /**
   * @var BinaryFileStream::FailBit
   * The last input operation failed because of an error related to the internal logic of the operation itself.
   */
  /**
   * @var BinaryFileStream::BadBit
   * Error due to the failure of an input/output operation on the stream buffer.
   */
  /**
   * @var BinaryFileStream::GoodBit
   * No error. Represents the absence of all the above (the value zero).
   */
  /**
   * @var BinaryFileStream::In
   * Allows input operations on the stream.
   */
  /**
   * @var BinaryFileStream::Out
   * Allows output operations on the stream.
   */
  /**
   * @var BinaryFileStream::Truncate
   * Any content is erased.The file is assumed to be zero-length.
   */
  /**
   * @var BinaryFileStream::Begin
   * Beginning of the stream buffer.
   */
  /**
   * @var BinaryFileStream::Current
   * Current position in the stream buffer.
   */
  /**
   * @var BinaryFileStream::End
   * End of the stream buffer.
   */
  /**
   * @var BinaryFileStream::mp_Stream
   * Binary stream which read/write data.
   */
  
  /**
   * @fn BinaryFileStream::BinaryFileStream()
   * Default abstract constructor.
   */
   
  /**
   * @fn BinaryFileStream::BinaryFileStream(const std::string& filename, OpenMode mode)
   * Associates the file with the filename @a filename using the option @a mode to this object.
   *
   * If the opening is not successfull, then the FailBit is set. You can check its state by using the method Fail().
   */
  
  /**
   * @fn BinaryFileStream::~BinaryFileStream()
   * Destroys the abstract binary file stream.
   */
  
  /**
   * @fn void BinaryFileStream::Open(const std::string& filename, OpenMode mode)
   * Opens file.
   */
  
  /**
   * @fn bool BinaryFileStream::IsOpen() const
   * Checks if a file is open.
   */
   
  /**
   * @fn bool BinaryFileStream::Good() const
   * Checks if the state of the stream is good for I/O operations.
   */
  
  /**
   * @fn void BinaryFileStream::Close()
   * Closes file.
   */
   
  /**
   * @fn bool BinaryFileStream::EndFile() const
   * Checks if eofbit is set.
   */
  
  /**
   * @fn bool BinaryFileStream::Bad() const
   * Checks if badbit is set.
   */
  
  /**
   * @fn bool BinaryFileStream::Fail() const
   * Checks if either failbit or badbit is set.
   */
  
  /**
   * @fn IOState BinaryFileStream::GetExceptions()
   * Gets exception mask.
   */
   
  /**
   * @fn void BinaryFileStream::SetExceptions(IOState except)
   * Sets exception mask.
   */
   
  /**
   * @fn void BinaryFileStream::Clear(IOState flags = GoodBit)
   * Sets error state flags.
   */
  
  /**
   * @fn const RawFileStream* BinaryFileStream::GetStream() const
   * Return the raw stream associated with this binary file stream.
   */
  
  /**
   * Swap streams. 
   * @warning The exceptions set are embedded with the stream.
   */
  void BinaryFileStream::SwapStream(BinaryFileStream* toSwap)
  {
    RawFileStream* temp = this->mp_Stream;
    this->mp_Stream = toSwap->mp_Stream;
    toSwap->mp_Stream = temp;
  };
  
  /** 
   * Extracts one character from the stream.
   */
  char BinaryFileStream::ReadChar()
  {
    char byteptr[1] = {0};
    this->mp_Stream->read(byteptr, 1);
    return *byteptr;
  };
  
  /** 
   * Extracts one signed 8-bit integer.
   */
  int8_t BinaryFileStream::ReadI8()
  {
    char byteptr[1] = {0};
    this->mp_Stream->read(byteptr, 1);
    return *byteptr;
  };
  
  /** 
   * Extracts one unsigned 8-bit integer.
   */
  uint8_t BinaryFileStream::ReadU8()
  {
    char byteptr[1] = {0};
    this->mp_Stream->read(byteptr, 1);
    return *byteptr;
  };
  
  /** 
   * @fn int16_t BinaryFileStream::ReadI16() = 0
   * Extracts one signed 16-bit integer.
   */
  
  /** 
   * @fn uint16_t BinaryFileStream::ReadU16() = 0
   * Extracts one unsigned 16-bit integer.
   */
  
  /** 
   * @fn int32_t BinaryFileStream::ReadI32() = 0
   * Extracts one signed 32-bit integer.
   */
  
  /** 
   * @fn uint32_t BinaryFileStream::ReadU32() = 0
   * Extracts one unsigned 32-bit integer.
   */
  
  /** 
   * @fn int64_t BinaryFileStream::ReadI64() = 0
   * Extracts one signed 64-bit integer.
   */
  
  /** 
   * @fn uint64_t BinaryFileStream::ReadU64() = 0
   * Extracts one unsigned 64-bit integer.
   */
  
  /** 
   * @fn float BinaryFileStream::ReadFloat() = 0
   * Extracts one float.
   */
  
  /** 
   * @fn float BinaryFileStream::ReadDouble() = 0
   * Extracts one double.
   */
  
  /** 
   * Extracts one string with @a nbChar characters.
   */
  std::string BinaryFileStream::ReadString(size_t nbChar)
  {
    std::string sFs;
    if (nbChar != 0)
    {
      char* byteptr = new char[nbChar];
      this->mp_Stream->read(byteptr, nbChar);
      sFs = std::string(byteptr, nbChar);
      delete[] byteptr;
    }
    else
    {
      sFs = std::string();
    }
    return sFs;
  };
  
  /** 
   * @fn void BinaryFileStream::SeekRead(StreamOffset offset, SeekDir dir)
   * Moves the get pointer by @a nb bytes in the seeking direction @a dir.
   */
  
  /**
   * @fn StreamPosition BinaryFileStream::TellRead() const
   * Get position of the get pointer.
   */
  
  /** 
   * Fills @a nb bytes with 0x00 in the stream.
   */
  size_t BinaryFileStream::Fill(size_t nb)
  {
    std::string towrite = std::string(nb, 0x00);
    this->mp_Stream->write(towrite.c_str(), nb);
    return nb;
  };
  
  /** 
   * @fn void BinaryFileStream::SeekWrite(StreamOffset offset, SeekDir dir)
   * Moves the set pointer by @a nb bytes in the seeking direction @a dir.
   */
  
  /*
   * Writes the character @a c in the stream an return its size.
   */
  /*
  size_t BinaryFileStream::Write(char c)
  {
    this->mp_Stream->write(&c, 1);
    return 1;
  };
  */
  
  /** 
   * Writes the signed 8-bit integer @a value in the stream an return its size.
   */
  size_t BinaryFileStream::Write(int8_t value)
  {
    char c = static_cast<char>(value);
    this->mp_Stream->write(&c, 1);
    return 1;
  };
  
  /** 
   * Writes the unsigned 8-bit integer @a value in the stream an return its size.
   */
  size_t BinaryFileStream::Write(uint8_t value)
  {
    char c = static_cast<char>(value);
    this->mp_Stream->write(&c, 1);
    return 1;
  };
  
  /** 
   * @fn size_t BinaryFileStream::Write(int16_t value) = 0
   * Extracts one signed 16-bit integer.
   */
  
  /** 
   * @fn size_t BinaryFileStream::Write(uint16_t value) = 0
   * Extracts one unsigned 16-bit integer.
   */

  /** 
   * @fn size_t BinaryFileStream::Write(int32_t value) = 0;
   * Write one 32-bit signed integer 
   */
  
  /** 
   * @fn size_t BinaryFileStream::Write(uint32_t value) = 0;
   * Write one 32-bit unsigned integer 
   */
  
  /** 
   * @fn size_t BinaryFileStream::Write(float value) = 0
   * Write one float.
   */
  
  /** 
   * Writes the string @a rString in the stream an return its size.
   */
  size_t BinaryFileStream::Write(const std::string& value)
  {
    this->mp_Stream->write(value.c_str(), value.length());
    return value.length();
  };
  
  /** 
   * @class VAXLittleEndianBinaryFileStream btkBinaryFileStream.h
   * @brief Class to read and write binary file encoded from a VAX (LE) processor
   * to a VAX (LE) and IEEE (LE, BE) processor.
   *
   * @sa BinaryFileStream, IEEELittleEndianBinaryFileStream, IEEEBigEndianBinaryFileStream, NativeBinaryFileStream
   */
  
  /**
   * @fn VAXLittleEndianBinaryFileStream::VAXLittleEndianBinaryFileStream()
    * Creates a new default DEC binary file stream.
   */
   
  /**
   * @fn VAXLittleEndianBinaryFileStream::VAXLittleEndianBinaryFileStream(const std::string& filename, OpenMode mode)
   * Creates a new default DEC binary file stream and associates the file with the filename @a filename using the option @a mode.
   *
   * If the opening is not successfull, then the FailBit is set. You can check its state by using the method Fail().
   */
  
  /** 
   * Extracts one signed 16-bit integer.
   */
  int16_t VAXLittleEndianBinaryFileStream::ReadI16()
  {
    char byteptr[2] = {0};
    this->mp_Stream->read(byteptr, 2);
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
  uint16_t VAXLittleEndianBinaryFileStream::ReadU16()
  {
    char byteptr[2] = {0};
    this->mp_Stream->read(byteptr, 2);
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
  int32_t VAXLittleEndianBinaryFileStream::ReadI32()
  {
    char byteptr[4] = {0};
    this->mp_Stream->read(byteptr, 4);
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
  uint32_t VAXLittleEndianBinaryFileStream::ReadU32()
  {
    char byteptr[4] = {0};
    this->mp_Stream->read(byteptr, 4);
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
  int64_t VAXLittleEndianBinaryFileStream::ReadI64()
  {
    char byteptr[8] = {0};
    this->mp_Stream->read(byteptr, 8);
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
  uint64_t VAXLittleEndianBinaryFileStream::ReadU64()
  {
    char byteptr[8] = {0};
    this->mp_Stream->read(byteptr, 8);
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
  float VAXLittleEndianBinaryFileStream::ReadFloat()
  {
    char byteptr[4] = {0};
    this->mp_Stream->read(byteptr, 4);
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
  double VAXLittleEndianBinaryFileStream::ReadDouble()
  {
    char byteptr[8] = {0};
    this->mp_Stream->read(byteptr, 8);
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
   * Writes the signed 16-bit integer @a i16 in the stream an return its size.
   */
  size_t VAXLittleEndianBinaryFileStream::Write(int16_t i16)
  {
    char byteptr[2] = {0};
    memcpy(&byteptr, &i16, sizeof(byteptr));
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    char foo[2] = {byteptr[1], byteptr[0]};
    this->mp_Stream->write(foo, 2);
#else
    this->mp_Stream->write(byteptr, 2);
#endif
    return 2;
  };

  /**
   * Writes the unsigned 16-bit integer @a u16 in the stream an return its size.
   */
  size_t VAXLittleEndianBinaryFileStream::Write(uint16_t u16)
  {
    char byteptr[2] = {0};
    memcpy(&byteptr, &u16, sizeof(byteptr));
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    char foo[2] = {byteptr[1], byteptr[0]};
    this->mp_Stream->write(foo, 2); 
#else
    this->mp_Stream->write(byteptr, 2);
#endif
    return 2;
  };
  
  /** 
   * Write the 32-bit signed integer @a i32 and return its size.
   */
  size_t VAXLittleEndianBinaryFileStream::Write(int32_t i32)
  {
    char byteptr[4] = {0};
    memcpy(&byteptr, &i32, sizeof(byteptr));
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    char foo[4] = {byteptr[1], byteptr[0], byteptr[3], byteptr[2]};
    this->mp_Stream->write(foo, 4);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
    this->mp_Stream->write(byteptr, 4);
#else
    char foo[4] = {byteptr[2], byteptr[3], byteptr[0], byteptr[1]};
    this->mp_Stream->write(foo, 4);
#endif
    return 4;
  };
  
  /** 
   * Write the 32-bit unsigned integer @a u32 and return its size
   */
  size_t VAXLittleEndianBinaryFileStream::Write(uint32_t u32)
  {
    char byteptr[4] = {0};
    memcpy(&byteptr, &u32, sizeof(byteptr));
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    char foo[4] = {byteptr[1], byteptr[0], byteptr[3], byteptr[2]};
    this->mp_Stream->write(foo, 4);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
    this->mp_Stream->write(byteptr, 4);
#else
    char foo[4] = {byteptr[2], byteptr[3], byteptr[0], byteptr[1]};
    this->mp_Stream->write(foo, 4);
#endif
    return 4;
  };

  /**
   * Writes the float @a f in the stream an return its size.
   */
  size_t VAXLittleEndianBinaryFileStream::Write(float f)
  {
    char byteptr[4] = {0};
    memcpy(&byteptr, &f, sizeof(byteptr));
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    char foo[4] = {byteptr[1], byteptr[0] + 1 * (byteptr[0] == 0 ? 0 : 1),  byteptr[3], byteptr[2]};
    this->mp_Stream->write(foo, 4);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
    this->mp_Stream->write(byteptr, 4);
#else
    char foo[4] = {byteptr[2], byteptr[3] + 1 * (byteptr[3] == 0 ? 0 : 1), byteptr[0], byteptr[1]};
    this->mp_Stream->write(foo, 4);
#endif
    return 4;
  };
  
  
  /** 
   * @class IEEEBigEndianBinaryFileStream btkBinaryFileStream.h
   * @brief Class to read and write binary file encoded from a IEEE (BE) to a 
   * VAX (LE) and IEEE (LE, BE) processor
   *
   * @sa BinaryFileStream, VAXLittleEndianBinaryFileStream, IEEELittleEndianBinaryFileStream, NativeBinaryFileStream
   */
  
  /**
   * @fn IEEEBigEndianBinaryFileStream::IEEEBigEndianBinaryFileStream()
   * Creates a new default MIPS binary file stream (IEEE Big Endian).
   */
   
  /**
   * @fn IEEEBigEndianBinaryFileStream::IEEEBigEndianBinaryFileStream(const std::string& filename, OpenMode mode)
   * Creates a new default MIPS binary file stream and associates the file with the filename @a filename using the option @a mode.
   *
   * If the opening is not successfull, then the FailBit is set. You can check its state by using the method Fail().
   */
  
  /** 
   * Extracts one signed 16-bit integer.
   */
  int16_t IEEEBigEndianBinaryFileStream::ReadI16()
  {
    char byteptr[2] = {0};
    this->mp_Stream->read(byteptr, 2);
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
  uint16_t IEEEBigEndianBinaryFileStream::ReadU16()
  {
    char byteptr[2] = {0};
    this->mp_Stream->read(byteptr, 2);
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
  int32_t IEEEBigEndianBinaryFileStream::ReadI32()
  {
    char byteptr[4] = {0};
    this->mp_Stream->read(byteptr, 4);
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
  uint32_t IEEEBigEndianBinaryFileStream::ReadU32()
  {
    char byteptr[4] = {0};
    this->mp_Stream->read(byteptr, 4);
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
  int64_t IEEEBigEndianBinaryFileStream::ReadI64()
  {
    char byteptr[8] = {0};
    this->mp_Stream->read(byteptr, 8);
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
  uint64_t IEEEBigEndianBinaryFileStream::ReadU64()
  {
    char byteptr[8] = {0};
    this->mp_Stream->read(byteptr, 8);
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
  float IEEEBigEndianBinaryFileStream::ReadFloat()
  {
    char byteptr[4] = {0};
    this->mp_Stream->read(byteptr, 4);
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
  double IEEEBigEndianBinaryFileStream::ReadDouble()
  {
    char byteptr[8] = {0};
    this->mp_Stream->read(byteptr, 8);
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
   * Writes the signed 16-bit integer @a i16 in the stream an return its size.
   */
  size_t IEEEBigEndianBinaryFileStream::Write(int16_t i16)
  {
    char byteptr[2] = {0};
    memcpy(&byteptr, &i16, sizeof(byteptr));
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    this->mp_Stream->write(byteptr, 2); 
#else
    char foo[2] = {byteptr[1], byteptr[0]};
    this->mp_Stream->write(foo, 2);
#endif
    return 2;
  };

  /**
   * Writes the unsigned 16-bit integer @a u16 in the stream an return its size.
   */
  size_t IEEEBigEndianBinaryFileStream::Write(uint16_t u16)
  {
    char byteptr[2] = {0};
    memcpy(&byteptr, &u16, sizeof(byteptr));
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    this->mp_Stream->write(byteptr, 2); 
#else
    char foo[2] = {byteptr[1], byteptr[0]};
    this->mp_Stream->write(foo, 2);
#endif
    return 2;
  };
  
  /** 
   * Write the 32-bit signed integer @a i32 and return its size.
   */
  size_t IEEEBigEndianBinaryFileStream::Write(int32_t i32)
  {
    char byteptr[4] = {0};
    memcpy(&byteptr, &i32, sizeof(byteptr));
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    this->mp_Stream->write(byteptr, 4);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
    char foo[4] = {byteptr[1], byteptr[0], byteptr[3], byteptr[2]};
    this->mp_Stream->write(foo, 4);
#else
    char foo[4] = {byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    this->mp_Stream->write(foo, 4);
#endif
    return 4;
  };
  
  /** 
   * Write the 32-bit unsigned integer @a u32 and return its size
   */
  size_t IEEEBigEndianBinaryFileStream::Write(uint32_t u32)
  {
    char byteptr[4] = {0};
    memcpy(&byteptr, &u32, sizeof(byteptr));
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    this->mp_Stream->write(byteptr, 4);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
    char foo[4] = {byteptr[1], byteptr[0], byteptr[3], byteptr[2]};
    this->mp_Stream->write(foo, 4);
#else
    char foo[4] = {byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    this->mp_Stream->write(foo, 4);
#endif
    return 4;
  };

  /**
   * Writes the float @a f in the stream an return its size.
   */
  size_t IEEEBigEndianBinaryFileStream::Write(float f)
  {
    char byteptr[4] = {0};
    memcpy(&byteptr, &f, sizeof(byteptr));
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    this->mp_Stream->write(byteptr, 4);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
    char foo[4] = {byteptr[1] - 1 * (byteptr[1] == 0 ? 0 : 1), byteptr[0], byteptr[3], byteptr[2]};
    this->mp_Stream->write(foo, 4);
#else
    char foo[4] = {byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    this->mp_Stream->write(foo, 4);
#endif
    return 4;
  };
  
  
  /** 
   * @class IEEELittleEndianBinaryFileStream btkBinaryFileStream.h
   * @brief Class to read and write binary file encoded from a IEEE (LE) to a 
   * VAX (LE) and IEEE (LE, BE) processor.
   *
   * @sa BinaryFileStream, VAXLittleEndianBinaryFileStream, IEEEBigEndianBinaryFileStream, NativeBinaryFileStream
   */
  
  /**
   * @fn IEEELittleEndianBinaryFileStream::IEEELittleEndianBinaryFileStream()
   * Creates a new default PC binary file stream (IEEE Little Endian).
   */
   
  /**
   * @fn IEEELittleEndianBinaryFileStream::IEEELittleEndianBinaryFileStream(const std::string& filename, OpenMode mode)
   * Creates a new default PC binary file stream and associates the file with the filename @a filename using the option @a mode.
   *
   * If the opening is not successfull, then the FailBit is set. You can check its state by using the method Fail().
   */
  
  /** 
   * Extracts one signed 16-bit integer.
   */
  int16_t IEEELittleEndianBinaryFileStream::ReadI16()
  {
    char byteptr[2] = {0};
    this->mp_Stream->read(byteptr, 2);
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
  uint16_t IEEELittleEndianBinaryFileStream::ReadU16()
  {
    char byteptr[2] = {0};
    this->mp_Stream->read(byteptr, 2);
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
  int32_t IEEELittleEndianBinaryFileStream::ReadI32()
  {
    char byteptr[4] = {0};
    this->mp_Stream->read(byteptr, 4);
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
  uint32_t IEEELittleEndianBinaryFileStream::ReadU32()
  {
    char byteptr[4] = {0};
    this->mp_Stream->read(byteptr, 4);
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
  int64_t IEEELittleEndianBinaryFileStream::ReadI64()
  {
    char byteptr[8] = {0};
    this->mp_Stream->read(byteptr, 8);
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
  uint64_t IEEELittleEndianBinaryFileStream::ReadU64()
  {
    char byteptr[8] = {0};
    this->mp_Stream->read(byteptr, 8);
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
  float IEEELittleEndianBinaryFileStream::ReadFloat()
  {
    char byteptr[4] = {0};
    this->mp_Stream->read(byteptr, 4);
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
  double IEEELittleEndianBinaryFileStream::ReadDouble()
  {
    char byteptr[8] = {0};
    this->mp_Stream->read(byteptr, 8);
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
   * Writes the signed 16-bit integer @a i16 in the stream an return its size.
   */
  size_t IEEELittleEndianBinaryFileStream::Write(int16_t i16)
  {
    char byteptr[2] = {0};
    memcpy(&byteptr, &i16, sizeof(byteptr));
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    char foo[2] = {byteptr[1], byteptr[0]};
    this->mp_Stream->write(foo, 2); 
#else
    this->mp_Stream->write(byteptr, 2);
#endif
    return 2;
  };

  /**
   * Writes the unsigned 16-bit integer @a u16 in the stream an return its size.
   */
  size_t IEEELittleEndianBinaryFileStream::Write(uint16_t u16)
  {
    char byteptr[2] = {0};
    memcpy(&byteptr, &u16, sizeof(byteptr));
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    char foo[2] = {byteptr[1], byteptr[0]};
    this->mp_Stream->write(foo, 2); 
#else
    this->mp_Stream->write(byteptr, 2);
#endif
    return 2;

  };

  /** 
   * Write the 32-bit signed integer @a i32 and return its size.
   */
  size_t IEEELittleEndianBinaryFileStream::Write(int32_t i32)
  {
    char byteptr[4] = {0};
    memcpy(&byteptr, &i32, sizeof(byteptr));
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    char foo[4] = {byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    this->mp_Stream->write(foo, 4);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */ 
    char foo[4] = {byteptr[2], byteptr[3], byteptr[0], byteptr[1]};
    this->mp_Stream->write(foo, 4);
#else
    this->mp_Stream->write(byteptr, 4);
#endif
    return 4;
  };
  
  /** 
   * Write the 32-bit unsigned integer @a u32 and return its size
   */
  size_t IEEELittleEndianBinaryFileStream::Write(uint32_t u32)
  {
    char byteptr[4] = {0};
    memcpy(&byteptr, &u32, sizeof(byteptr));
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    char foo[4] = {byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    this->mp_Stream->write(foo, 4);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */ 
    char foo[4] = {byteptr[2], byteptr[3], byteptr[0], byteptr[1]};
    this->mp_Stream->write(foo, 4);
#else
    this->mp_Stream->write(byteptr, 4);
#endif
    return 4;
  };

  /**
   * Writes the float @a f in the stream an return its size.
   */
  size_t IEEELittleEndianBinaryFileStream::Write(float f)
  {
    char byteptr[4] = {0};
    memcpy(&byteptr, &f, sizeof(byteptr));
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    char foo[4] = {byteptr[3], byteptr[2], byteptr[1], byteptr[0]};
    this->mp_Stream->write(foo, 4);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
    char foo[4] = {byteptr[2], byteptr[3], byteptr[0], byteptr[1] - 1 * (byteptr[1] == 0 ? 0 : 1)};
    this->mp_Stream->write(foo, 4);
#else
    this->mp_Stream->write(byteptr, 4);
#endif
    return 4;
  };
  
  
  /** 
   * @class NativeBinaryFileStream btkBinaryFileStream.h
   * @brief Native binary file stream chosen by the used compiler.
   *
   * Depending the processor, the native binary file stream will be:
   *  - the VAX Little Endian file stream ()
   *  - the IEEE Little Endian file stream
   *  - the IEEE Big Endian file stream
   *
   * @sa BinaryFileStream, VAXLittleEndianBinaryFileStream, IEEELittleEndianBinaryFileStream, IEEEBigEndianBinaryFileStream
   */
   
  /**
   * @fn NativeBinaryFileStream::NativeBinaryFileStream()
   * Default constructor. Use the method NativeBinaryFileStream::Open to associate a physical file.
   */
   
  /**
   * @fn NativeBinaryFileStream::NativeBinaryFileStream(const std::string& filename, OpenMode mode)
   * Associates the file with the filename @a filename using the option @a mode to this object.
   *
   * If the opening is not successfull, then the FailBit is set. You can check its state by using the method Fail().
   */
};

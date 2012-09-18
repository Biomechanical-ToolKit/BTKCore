/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2012, Arnaud Barré
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

#include "btkBinaryFileStream_mmfstream.h"
#include "btkMacro.h"

#if defined(HAVE_SYS_MMAP)
  #if defined(HAVE_64_BIT)
    #ifndef _LARGEFILE_SOURCE
      #define _LARGEFILE_SOURCE
    #endif
    #define _FILE_OFFSET_BITS 64 // Replace all the functions (mmap, lssek, ...) by their 64 bit version
  #endif
  #include <sys/mman.h> // mmap, munmap
  #include <sys/stat.h> // fstat
  #include <fcntl.h> // open, close
#endif

namespace btk
{
  // ------------------------------------------------------------ //
  //                 Memory Mapped File Buffer                    //
  // ------------------------------------------------------------ //
  
  /**
   * @class mmfilebuf btkBinaryFileStream_mmfstream.h
   * @brief File buffer using the the memory mapped mechanism
   *
   * This class is a low level class used by the class BinaryFileStream when possible (if the OS supports it).
   * It is not advised to use this class directly even if the direct access to the buffer can speed the reading of a file (compared to the use of the class BinaryFileStream).
   */
  
  /**
   * @fn mmfilebuf::mmfilebuf();
   * Constructor.
   */
   
   
  /**
   * @fn mmfilebuf::~mmfilebuf()
   * Destructor. Try to close the file if opened.
   */
  
  /**
   * Open the file with the given filename @a s and the options @a mode.
   * From the given options, the option binary is everytime append as this buffer is only for binary file.
   */
  mmfilebuf* mmfilebuf::open(const char* s, std::ios_base::openmode mode)
  {
    if (this->is_open())
      return 0;
    std::ios_base::openmode mode_ = mode & (~std::ios_base::ate & ~std::ios_base::binary);
    if (mode_ & std::ios_base::out)
      this->m_Writing = true;
    else
      this->m_Writing = false;
    // Open the file and map it into the memory
    // The flags' extraction is inspired by the file fstream.cxx from the Comeau Computing library
#if defined(HAVE_SYS_MMAP) // POSIX
    // Select the flags for the function open
    int flags = 0;
    if ((mode_ == std::ios_base::out) || (mode_ == std::ios_base::out + std::ios_base::trunc))
      flags = O_RDWR | O_CREAT | O_TRUNC;
    else if (mode_ == std::ios_base::out + std::ios_base::app)
      flags = O_WRONLY | O_CREAT | O_APPEND;
    else if (mode_ == std::ios_base::in)
      flags = O_RDONLY;
    else if (mode_ == std::ios_base::in + std::ios_base::out)
      flags = O_RDWR;
    else if (mode_ == std::ios_base::in + std::ios_base::out + std::ios_base::trunc)
      flags = O_RDWR | O_CREAT | O_TRUNC;
    else // Other flags are not supported in the C++ standard
      return 0;
    // Open the file
    if ((this->m_File = ::open(s, flags, S_IRWXU)) == -1)
      return 0;
    // Get the size of the file
    struct stat info;
    if (::fstat(this->m_File, &info) == -1)
      return this->close();
    this->m_BufferSize = this->m_LogicalSize = info.st_size;
    // New file or truncated file?
    if ((this->m_BufferSize == 0) && this->m_Writing)
    {
      this->m_BufferSize = mmfilebuf::granularity();
      if ((::lseek(this->m_File, this->m_BufferSize-1, SEEK_SET) == -1)
           || (::write(this->m_File, "", 1) == -1))
        return this->close();
    }
#else // Windows
    // Select the flags for the function CreateFile
    DWORD dwDesiredAccess, dwShareMode, dwCreationDisposition, dwFlagsandAttributes;
    dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
    dwShareMode = FILE_SHARE_READ;
    dwFlagsandAttributes = FILE_ATTRIBUTE_TEMPORARY;
    switch(mode_)
    {
    case std::ios_base::out:
      dwCreationDisposition = OPEN_ALWAYS;
      break;
    case std::ios_base::out + std::ios_base::trunc:
      dwCreationDisposition = CREATE_ALWAYS;
      break;
    case std::ios_base::out + std::ios_base::app:
      dwCreationDisposition = OPEN_ALWAYS;
      break;
    case std::ios_base::in:
      dwDesiredAccess = GENERIC_READ;
      dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE; // Is it a good idea to give the possibility to other processes to write in the file that we are reading?
      dwCreationDisposition = OPEN_EXISTING;
      dwFlagsandAttributes = FILE_ATTRIBUTE_READONLY;
      break;
    case std::ios_base::in + std::ios_base::out:
      dwCreationDisposition = OPEN_ALWAYS;
      break;
    case std::ios_base::in + std::ios_base::out + std::ios_base::trunc:
      dwCreationDisposition = TRUNCATE_EXISTING;
      break;
    default: // Other flags are not supported in the C++ standard
      return 0;
    }
    // Open the file
    if ((this->m_File = ::CreateFile(s, dwDesiredAccess, dwShareMode, NULL, 
                                     dwCreationDisposition, dwFlagsandAttributes, NULL))
        == INVALID_HANDLE_VALUE)
      return 0;
    // Get the size of the file
    DWORD dwFileSizeHi = 0;
    DWORD dwFileSizeLo = GetFileSize(this->m_File, &dwFileSizeHi);
    this->m_BufferSize = this->m_LogicalSize = static_cast<std::streamsize>(((uint64_t)dwFileSizeHi << 32) | dwFileSizeLo);
    // New file or truncated file?
    if ((this->m_BufferSize == 0) && this->m_Writing)
    {
      this->m_BufferSize = mmfilebuf::granularity();
      LONG lDistHigh = (uint64_t)this->m_BufferSize >> 32; // 32 = (sizeof(LONG) * 8)
      LONG lDistLow = this->m_BufferSize & 0xFFFFFFFF;
      if (((::SetFilePointer(this->m_File, lDistLow, &lDistHigh, FILE_BEGIN) == INVALID_SET_FILE_POINTER) 
           && (::GetLastError() != NO_ERROR)) || (::SetEndOfFile(this->m_File) == 0))
        return this->close();
    }
#endif
    // Map the file
    if (!this->mapfile())
      return this->close();
      
    this->m_Position = 0;
    
    // If necessary go to the end of the file (::ate option)
    if ((mode & std::ios_base::ate) && (this->seekoff(0,std::ios_base::end, mode) == std::streampos(std::streamoff(-1))))
      return this->close();
      
    return this;
  };
  
  /**
   * @fn bool mmfilebuf::is_open() const
   * Return true if the file is opened.
   */
   
  /**
   * Close the file.
   * @return Return 0 is an error happened during the closing.
   */ 
  mmfilebuf* mmfilebuf::close()
  {
    if (!this->is_open())
      return 0;

#if defined(HAVE_SYS_MMAP)
    bool err = !(::munmap(this->mp_Buffer, this->m_BufferSize) == 0);
#else
    BOOL err = (::UnmapViewOfFile(this->mp_Buffer) == 0) || (::CloseHandle(this->m_Map) == 0);
    this->m_Map = NULL;
#endif
    
    // If in write mode, truncate the file to the number of bytes wrote.
    if (this->m_Writing)
    {
#if defined(HAVE_SYS_MMAP)
      err |= (::ftruncate(this->m_File, this->m_LogicalSize) == -1);
#else
      LONG lDistHigh = (uint64_t)this->m_LogicalSize >> 32; // 32 = (sizeof(LONG) * 8)
      LONG lDistLow = this->m_LogicalSize & 0xFFFFFFFF;   
      err |= (((::SetFilePointer(this->m_File, lDistLow, &lDistHigh, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
               && (::GetLastError() != NO_ERROR)) || (::SetEndOfFile(this->m_File) == 0));
#endif
    }

    // Close the file
#if defined(HAVE_SYS_MMAP)
    err |= ::close(this->m_File);
#else
    err |= (::CloseHandle(this->m_File) == 0);
#endif
    this->m_File = BTK_MMFILEBUF_NO_FILE;
    
    if (err)
      return 0;
    this->mp_Buffer = 0;
    this->m_BufferSize = 0;
    this->m_LogicalSize = 0;
    
    return this;
  };
  
  /**
   * @fn bool mmfilebuf::is_eob() const
   * Check if the position of the pointer on the data is at the end of the buffer.
   */
  
  /**
   * @fn bool mmfilebuf::writemode() const
   * Check if this file buffer is in write mode or not.
   */
    

  /**
   * @fn std::streamsize mmfilebuf::size() const
   * Returns the size of the buffer.
   */
  
  /**
   * @fn const char* mmfilebuf::data() const
   * Returns directly the content of the buffer.
   */
  
  /**
   * @fn std::streampos mmfilebuf::pubseekoff(std::streamoff off, std::ios_base::seekdir way, std::ios_base::openmode which = std::ios_base::in | std::ios_base::out)
   * Sets internal position pointer to relative position.
   * @return The new position value of the modified position pointer. 
   */
  
  /**
   * @fn std::streampos mmfilebuf::pubseekpos(std::streampos pos, std::ios_base::openmode which = std::ios_base::in | std::ios_base::out)
   * Sets internal position pointer to absolute position
   * @return The new position value of the modified position pointer. 
   */
    
  /**
   * Sets internal position pointer to relative position.
   * @return The new position value of the modified position pointer. Errors are expected to be signaled by an invalid position value, like -1.
   */
  std::streampos mmfilebuf::seekoff(std::streamoff off, std::ios_base::seekdir way, std::ios_base::openmode which)
  {
    btkNotUsed(which);
    switch(way)
    {
    case std::ios_base::beg:
      if (off < 0)
        return -1;
      this->m_Position = off;
      break;
    case std::ios_base::cur:
      if (this->m_Position + off < 0)
        return -1;
      this->m_Position += off;
      break;
    case std::ios_base::end:
      if (this->m_BufferSize + off < 0)
        return -1;
      this->m_Position = this->m_BufferSize + off;
      break;
    default:
      return -1;
    }
    return this->m_Position;
  };
  
  /**
   * Sets internal position pointer to absolute position
   * @return The new position value of the modified position pointer. In case of error, the value returned shall be an invalid position, like -1.
   */
  std::streampos mmfilebuf::seekpos(std::streampos pos, std::ios_base::openmode which)
  {
    btkNotUsed(which);
    if (pos < 0)
      return -1;
    
    this->m_Position = pos;
    return this->m_Position;
  };
  
  /**
   * Get sequence of characters
   * @return The number of characters gotten, returned as a value of type streamsize.
   */
  std::streamsize mmfilebuf::sgetn(char* s, std::streamsize n)
  {
    std::streamoff n_ = (((this->m_Position + n)  == 0) || ((this->m_Position + n) > this->m_BufferSize)) ? ((this->m_BufferSize - this->m_Position - 1) > 0 ? this->m_BufferSize - this->m_Position - 1 : 0) : n;
    for (std::streamoff i = 0 ; i < n_ ; ++i)
      s[i] = this->mp_Buffer[this->m_Position + i];
    this->m_Position += n_;
    return n_;
  };
  
  /**
   * Write a sequence of characters
   * @return The number of characters written, returned as a value of type streamsize.
   */
  std::streamsize mmfilebuf::sputn(const char* s, std::streamsize n)
  {
    while ((this->m_Position + n) >= this->m_BufferSize) 
    {
      if (!this->resizemap())
        return 0;
    }
    
    for (std::streamoff i = 0 ; i < n ; ++i)
      this->mp_Buffer[this->m_Position + i] = s[i];
    this->m_Position += n;
    
    if (this->m_Position >= this->m_LogicalSize)
      this->m_LogicalSize = this->m_Position;
    return n;
  };
  
  /**
   * Try to map the file into the memory.
   * @return Returns 0 if an error occured.
   */
  mmfilebuf* mmfilebuf::mapfile()
  {
#if defined(HAVE_SYS_MMAP)
    bool err = ((this->mp_Buffer = (char*)::mmap(0, 
                                                 this->m_BufferSize, 
                                                 this->m_Writing ? (PROT_READ | PROT_WRITE) : PROT_READ, 
                                                 MAP_SHARED, this->m_File, 0)) == MAP_FAILED);
#else
    BOOL err = ((this->m_Map = ::CreateFileMapping(this->m_File,
                                                   NULL, 
                                                   this->m_Writing ? PAGE_READWRITE : PAGE_READONLY,
                                                   0, 
                                                   0,
                                                   NULL)) == NULL);
    err |= ((this->mp_Buffer = (char*)::MapViewOfFile(this->m_Map,
                                                       this->m_Writing ? FILE_MAP_WRITE : FILE_MAP_READ,
                                                       0,
                                                       0,
                                                       this->m_BufferSize)) == NULL);
#endif
    return err ? 0 : this;
  };
  
  /**
   * Try to resize the map to be able to extract more data from the file.
   * @return Returns 0 if an error occured.
   */
  mmfilebuf* mmfilebuf::resizemap()
  {
    if (!this->is_open() || !this->m_Writing)
      return 0;
    std::streamsize newBufferSize = this->m_BufferSize + this->granularity();
#if defined(_MSC_VER)
    if ((::UnmapViewOfFile(this->mp_Buffer) == 0) || (::CloseHandle(this->m_Map) == 0))
      return 0;
    this->m_Map = NULL;
    LONG lDistHigh = (uint64_t)newBufferSize >> 32; // 32 = (sizeof(LONG) * 8)
    LONG lDistLow = newBufferSize & 0xFFFFFFFF;   
    if (((::SetFilePointer(this->m_File, lDistLow, &lDistHigh, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
               && (::GetLastError() != NO_ERROR)) || (::SetEndOfFile(this->m_File) == 0))
      return 0;
#else
    if (::munmap(this->mp_Buffer, this->m_BufferSize) == -1)
          return 0;
    if (::ftruncate(this->m_File, newBufferSize) == -1)
      return 0;      
#endif
    this->m_BufferSize = newBufferSize;
    return this->mapfile();
  };
  
  /**
   * Return the size of block of memory.
   */
  int mmfilebuf::granularity()
  {
#if defined(_MSC_VER)
    SYSTEM_INFO info;
    ::GetSystemInfo(&info);
    return static_cast<int>(info.dwAllocationGranularity);
#else
    return static_cast<int>(::sysconf(_SC_PAGESIZE));
#endif
  };
  
  // ------------------------------------------------------------ //
  //                 Memory Mapped File Stream                    //
  // ------------------------------------------------------------ //
  
  /**
   * @class mmfstream btkBinaryFileStream_mmfstream.h
   * @brief Provides an interface to read/write data from files as input/output streams.
   *
   * This class is a low level class used by the class BinaryFileStream when possible (if the OS supports it).
   * It is not advised to use this class directly even if the direct access to the buffer can speed the reading of a file (compared to the use of the class BinaryFileStream).
   */
   
  /**
   * @typedef mmfstream::failure
   * Exception thrown by the class mmfstream.
   */
  
  /**
   * @fn mmfstream::mmfstream();
   * Simple constructor which must use the method open() after.
   */
  
  /**
   * @fn mmfstream::mmfstream(const char* s, std::ios_base::openmode mode);
   * Constructor which opens the given filename @a s and options @a mode.
   */
  
  /**
   * @fn const mmfilebuf* mmfstream::rdbuf() const
   * Gets the associated file buffer object.
   */
  
  /**
   * @fn void mmfstream::open(const char* s, std::ios_base::openmode mode);
   * Open file.
   */
  
  /**
   * @fn bool mmfstream::is_open() const
   * Check if a file is open.
   */
  
  /**
   * @fn void mmfstream::close();
   * Close file.
   */
  
  /**
   * @fn std::ios_base::iostate mmfstream::rdstate() const
   * Returns the current internal error state flags of the stream.
   */
  
  /**
   * @fn bool mmfstream::good() const
   * Checks if the state of the stream is good for i/o operations
   */
  
  /**
   * @fn bool mmfstream::eof() const
   * Checks if eofbit is set
   */
  
  /**
   * @fn bool mmfstream::fail() const
   * Checks if either failbit or badbit is set
   */
  
  /**
   * @fn bool mmfstream::bad() const
   * Checks if badbit is set
   */
  
  /**
   * @fn std::ios_base::iostate mmfstream::exceptions() const
   * Returns the exception mask
   */
  
  /**
   * @fn void mmfstream::exceptions(std::ios_base::iostate except);
   * Sets the exception mask
   */
  
  /**
   * @fn void mmfstream::clear(std::ios_base::iostate state = std::ios_base::goodbit);
   * Sets a new value for the error control state.
   */
  
  /**
   * @fn void mmfstream::setstate(std::ios_base::iostate state)
   * Sets error state flag
   */
   
  /**
   * Reads a block of data
   */
  mmfstream& mmfstream::read(char* s, std::streamsize n)
  {
    if (this->m_Filebuf.sgetn(s,n) != n)
      this->setstate(std::ios_base::eofbit | std::ios_base::failbit);
    else if (this->m_Filebuf.is_eob())
      this->setstate(std::ios_base::eofbit);
    return *this;
  };
  
  /**
   * @fn std::streampos mmfstream::tellg()
   * Gets position of the get pointer.
   */
  
  /**
   * @fn mmfstream& mmfstream::seekg(std::streampos pos);
   * Sets position of the get pointer
   */
  
  /**
   * @fn mmfstream& mmfstream::seekg(std::streamoff off, std::ios_base::seekdir dir);
   * Sets position of the get pointer
   */
  
  /**
   * Writes a block of data
   */
  mmfstream& mmfstream::write(const char* s, std::streamsize n)
  {
    if (!this->m_Filebuf.writemode()) // Read-only?
      this->setstate(std::ios_base::failbit);
    else if (this->m_Filebuf.sputn(s,n) != n)
      this->setstate(std::ios_base::failbit);
    return *this;
  };
  
  /**
   * @fn mmfstream& mmfstream::seekp(std::streampos pos);
   * Sets position of put pointer
   */
  
  /**
   * @fn mmfstream& mmfstream::seekp(std::streamoff off, std::ios_base::seekdir dir);
   * Sets position of put pointer
   */
};
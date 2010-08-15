/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2010, Arnaud Barré
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

#include "btkBinaryFileStream_mmfstream_p.h"
#include "btkMacro.h"

#include <errno.h>
#include <iostream>

#if defined(HAVE_SYS_MMAP)
  #if defined(HAVE_64_BIT)
    #define _LARGEFILE_SOURCE
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
  mmfilebuf* mmfilebuf::open(const char* s, std::ios_base::openmode mode)
  {
    //std::cout << "Already opened?" << std::endl;
    if (this->is_open())
      return 0;
    // Open the file and map it into the memory
    // This extraction is greatly inspired by the file fstream.cxx
    // from the Comeau Computing library
#if defined(HAVE_SYS_MMAP) // POSIX
    // Select the flags for the function open
    std::ios_base::openmode mode_ = mode & (~std::ios_base::ate & ~std::ios_base::binary);
    int flags = 0;
    switch (mode_)
    {
    case std::ios_base::out:
    case std::ios_base::out + std::ios_base::trunc:
      flags = O_RDWR | O_CREAT | O_TRUNC;
      break;
    case std::ios_base::out + std::ios_base::app:
      flags = O_WRONLY | O_CREAT | O_APPEND;
      break;
    case std::ios_base::in:
      flags = O_RDONLY;
      break;
    case std::ios_base::in + std::ios_base::out:
      flags = O_RDWR;
      break;
    case std::ios_base::in + std::ios_base::out + std::ios_base::trunc:
      flags = O_RDWR | O_CREAT | O_TRUNC;
      break;
    default: // Other flags are not supported in the C++ standard
      return 0;
    }
    if (mode_ & std::ios_base::out)
      this->m_Writing = true;
    else
      this->m_Writing = false;
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
      if (::lseek(this->m_File, this->m_BufferSize-1, SEEK_SET) == -1)
        return this->close();
      if (::write(this->m_File, "", 1) == -1)
        return this->close();
    }
#else // Windows
    // Select the flags for the function CreateFile
    DWORD dwDesiredAccess, dwShareMode, dwCreationDisposition;
    DWORD dwFlagsandAttributes = FILE_ATTRIBUTE_TEMPORARY;
    switch(mode & (~std::ios_base::ate & ~std::ios_base::binary))
    {
    case std::ios_base::out:
    case std::ios_base::out + std::ios_base::trunc:
      dwDesiredAccess = GENERIC_WRITE;
      dwShareMode = FILE_SHARE_READ;
      dwCreationDisposition = TRUNCATE_EXISTING;
      break;
    case std::ios_base::out + std::ios_base::app:
      dwDesiredAccess = GENERIC_WRITE;
      dwShareMode = FILE_SHARE_READ;
      dwCreationDisposition = OPEN_ALWAYS;
      break;
    case std::ios_base::in:
      dwDesiredAccess = GENERIC_READ;
      dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE; // Is it a good idea to give the possibility to other processes to write in the file that we are reading?
      dwCreationDisposition = OPEN_EXISTING;
      dwFlagsandAttributes = FILE_ATTRIBUTE_READONLY;
      break;
    case std::ios_base::in + std::ios_base::out:
      dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
      dwShareMode = FILE_SHARE_READ;
      dwCreationDisposition = OPEN_ALWAYS;
      break;
    case std::ios_base::in + std::ios_base::out + std::ios_base::trunc:
      dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
      dwShareMode = FILE_SHARE_READ;
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
  
  mmfilebuf* mmfilebuf::close()
  {
    if (!this->is_open())
      return 0;
      
    bool err = false;
#if defined(HAVE_SYS_MMAP)
    err |= !(::munmap(this->mp_Buffer, this->m_BufferSize) == 0);
#else
    err |= ::UnmapViewOfFile(this->mp_Buffer) || ::CloseHandle(this->m_Map);
    this->m_Map = NULL;
#endif

    // If in write mode, truncate the file to the number of bytes wrote.
    if (this->m_Writing)
    {
#if defined(HAVE_SYS_MMAP)
      err |= (::ftruncate(this->m_File, this->m_LogicalSize) == -1);
#else
      LONG sizehigh = (this->m_LogicalSize >> (sizeof(LONG) * 8));
      LONG sizelow = (this->m_LogicalSize & 0xffffffff);      
      err |= ((::SetEndOfFile(this->m_File, sizelow, sizehigh, FILE_BEGIN) == INVALID_SET_FILE_POINTER) || (::SetFilePointer(this->m_File) == 0));
#endif
    }

    // Close the file
#if defined(HAVE_SYS_MMAP)
    err |= ::close(this->m_File);
#else
    err |= ::CloseHandle(this->m_File);
#endif
    this->m_File = NO_FILE;
    if (err)
      return 0;
    this->mp_Buffer = 0;
    this->m_BufferSize = 0;
    this->m_LogicalSize = 0;
    return this;
  };
  
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
  
  std::streampos mmfilebuf::seekpos(std::streampos pos, std::ios_base::openmode which)
  {
    btkNotUsed(which);
    if (pos < 0)
      return -1;
    
    this->m_Position = pos;
    return this->m_Position;
  };
  
  std::streamsize mmfilebuf::sgetn(char* s, std::streamsize n)
  {
    std::streamoff n_ = ((this->m_Position + n) > this->m_BufferSize) ? this->m_BufferSize - this->m_Position - 1 : n;
    for (std::streamoff i = 0 ; i < n_ ; ++i)
      s[i] = this->mp_Buffer[this->m_Position + i];
    this->m_Position += n_;
    return n_;
  };
  
  std::streamsize mmfilebuf::sputn(const char* s, std::streamsize n)
  {
    if (((this->m_Position + n) >= this->m_BufferSize) && !this->resizemap())
      return 0;
    
    for (std::streamoff i = 0 ; i < n ; ++i)
      this->mp_Buffer[this->m_Position + i] = s[i];
    this->m_Position += n;
    
    if (this->m_Position >= this->m_LogicalSize)
      this->m_LogicalSize = this->m_Position;
    return n;
  };
  
  mmfilebuf* mmfilebuf::mapfile()
  {
#if defined(HAVE_SYS_MMAP)
    bool err = ((this->mp_Buffer = (char*)::mmap(0, 
                                                 this->m_BufferSize, 
                                                 this->m_Writing ? (PROT_READ | PROT_WRITE) : PROT_READ, 
                                                 MAP_SHARED, this->m_File, 0)) == MAP_FAILED);
#else
    #error ToDo
#endif
    return err ? 0 : this;
  };
  
  mmfilebuf* mmfilebuf::resizemap()
  {
    if (!this->is_open() || !this->m_Writing)
      return 0;
    std::streamsize newBufferSize = this->m_BufferSize + this->granularity();
#if defined(_MSC_VER)
    #error ToDo
#else
    if (ftruncate(this->m_File, newBufferSize) == -1)
      return 0;      
#endif
    this->m_BufferSize = newBufferSize;
    return this->mapfile();
  };
  
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
  
  mmfstream& mmfstream::read(char* s, std::streamsize n)
  {
    if (this->m_Filebuf.sgetn(s,n) != n)
      this->setstate(std::ios_base::eofbit | std::ios_base::failbit);
    else if (this->m_Filebuf.is_eob())
      this->setstate(std::ios_base::eofbit);
    return *this;
  };
  
  mmfstream& mmfstream::write(const char* s, std::streamsize n)
  {
    if (!this->m_Filebuf.writemode()) // Read-only?
      this->setstate(std::ios_base::failbit);
    else if (this->m_Filebuf.sputn(s,n) != n)
      this->setstate(std::ios_base::failbit);
    return *this;
  };
}
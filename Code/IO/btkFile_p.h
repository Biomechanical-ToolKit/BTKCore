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

#ifndef __btkFile_p_h
#define __btkFile_p_h

/*
 * WARNING: This file and its content is not included in the public API and 
 * can change drastically from one release to another.
 */

#include "btkIODevice_p.h"
#include "btkConfigure.h"

#if !defined(HAVE_SYS_MMAP) && !defined(_MSC_VER)
  #error Missing header to build the File class (memory mapping not found).
#endif

#if defined(_MSC_VER) // Windows
  #define WIN32_LEAN_AND_MEAN
  #define VC_EXTRALEAN
  // Defining NOMINMAX to prevent compiler error with std::min/std::max when including windows.h
  #ifndef NOMINMAX
    #define NOMINMAX
  # endif
  #include <windows.h>
  #define _BTK_MMFILEBUF_NO_FILE INVALID_HANDLE_VALUE
#else // POSIX
  #define _BTK_MMFILEBUF_NO_FILE -1
#endif

#include "btkIODevice_p.h"

namespace btk
{
  class MemoryMappedBuffer;
  
  class FilePrivate : public IODevicePrivate
  {
  public:
    FilePrivate();
    virtual ~FilePrivate();
    
    FilePrivate(const FilePrivate& ) = delete;
    FilePrivate(FilePrivate&& ) noexcept = delete;
    FilePrivate& operator=(const FilePrivate& ) = delete;
    FilePrivate& operator=(const FilePrivate&& ) noexcept = delete;
    
    MemoryMappedBuffer* Buffer;
  };
  
  class MemoryMappedBuffer
  {
  public:
    MemoryMappedBuffer() noexcept;
    ~MemoryMappedBuffer() {this->close();};
    
    MemoryMappedBuffer* open(const char* s, File::OpenMode mode) noexcept;
    bool isOpen() const noexcept {return !(this->m_File == _BTK_MMFILEBUF_NO_FILE);};
    MemoryMappedBuffer* close() noexcept;

    bool hasWriteMode() const noexcept {return this->m_Writing;};
    
    File::Size dataSize() const noexcept {return this->m_DataSize;};
    const char* data() const noexcept {return this->mp_Data;};
    
    File::Size read(char* s, File::Size n) noexcept;
    File::Size write(const char* s, File::Size n) noexcept;
    
    File::Position seek(File::Offset off, File::SeekDir way) noexcept;
    
    MemoryMappedBuffer* map() noexcept;
    MemoryMappedBuffer* resizeMap() noexcept;
    
    static int granularity() noexcept;
    
  private:
    char* mp_Data;
    File::Size m_DataSize;
    File::Size m_LogicalSize;
#if defined(HAVE_SYS_MMAP)
    int m_File;
#else
    HANDLE m_File;
    HANDLE m_Map;
#endif
    File::Offset m_Offset;
    bool m_Writing;
  };
};

#endif // __btkFile_p_h

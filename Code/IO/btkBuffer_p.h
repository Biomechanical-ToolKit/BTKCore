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

#ifndef __btkBuffer_p_h
#define __btkBuffer_p_h

/*
 * WARNING: This file and its content are not included in the public API and 
 * can change drastically from one release to another.
 */

#include "btkIODevice_p.h"
#include "btkMacros.h" // _BTK_NOEXCEPT

namespace btk
{
  class ChunkBuffer;
  
  class BufferPrivate : public IODevicePrivate
  {
  public:
    BufferPrivate();
    ~BufferPrivate() _BTK_NOEXCEPT;
    
    BufferPrivate(const BufferPrivate& ) = delete;
    BufferPrivate(BufferPrivate&& ) _BTK_NOEXCEPT = delete;
    BufferPrivate& operator=(const BufferPrivate& ) = delete;
    BufferPrivate& operator=(const BufferPrivate&& ) _BTK_NOEXCEPT = delete;
    
    ChunkBuffer* Buffer;
  };
  
  class ChunkBuffer
  {
  public:
    using State = IODevice::State;
    using Origin = IODevice::Origin;
    using Mode = IODevice::Mode;
    using Size = IODevice::Size;
    using Offset = IODevice::Offset;
    using Position = IODevice::Position;
    
    ChunkBuffer() _BTK_NOEXCEPT;
    ~ChunkBuffer() _BTK_NOEXCEPT {this->close();};
    
    ChunkBuffer* open(char* data, size_t dataSize, const std::vector<size_t>& chunkIds, size_t chunkSize, Mode mode, bool owned) _BTK_NOEXCEPT;
    bool isOpen() const _BTK_NOEXCEPT {return this->mp_Data != 0;};
    ChunkBuffer* close() _BTK_NOEXCEPT;

    bool hasWriteMode() const _BTK_NOEXCEPT {return this->m_Writing;};
    
    Size dataSize() const _BTK_NOEXCEPT {return this->m_DataSize;};
    const char* data() const _BTK_NOEXCEPT {return this->mp_Data;};
    
    Size peek(char* s, Size n) const _BTK_NOEXCEPT;
    Size read(char* s, Size n) _BTK_NOEXCEPT;
    Size write(const char* s, Size n) _BTK_NOEXCEPT;
    
    Position seek(Offset off, Origin whence) _BTK_NOEXCEPT;
    
    const std::vector<size_t>& chunkIDs() const;
    size_t chunkSize() const;
    void setChunks(const std::vector<size_t>& ids, size_t size);
    
    bool updateChunkOffset(Offset* dataOffset, Offset* chunkOffset) const;
    
  private:
    char* mp_Data;
    bool m_DataOwned;
    Size m_DataSize;
    std::vector<size_t> m_ChunkIds;
    Size m_ChunkSize;
    Offset m_ChunkOffset;
    Offset m_Offset;
    bool m_Writing;
  };
};

#endif // __btkBuffer_p_h

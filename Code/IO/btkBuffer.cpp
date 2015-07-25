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

#include "btkBuffer.h"
#include "btkBuffer_p.h"
#include "btkLogger.h"

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

namespace btk
{
  BufferPrivate::BufferPrivate()
  : IODevicePrivate(), Buffer(new ChunkBuffer)
  {};
  
  BufferPrivate::~BufferPrivate() _BTK_NOEXCEPT
  {
    if (this->Buffer->isOpen() && !this->Buffer->close())
      Logger::error("An error occurred when closing the buffer device. The data inside the buffer might be corrupted.");
    delete this->Buffer;
  }
  
  // ----------------------------------------------------------------------- //
  
  /**
   * Constructor
   */
  ChunkBuffer::ChunkBuffer() _BTK_NOEXCEPT
  : mp_Data(nullptr),
    m_DataOwned(false),
    m_DataSize(0),
    m_ChunkSize(0),
    m_ChunkOffset(-1),
    m_Offset(-1),
    m_Writing(false)
  {};
  
  /**
   * Destructor. Try to Close the file if opened.
   */
  
  /**
   * Open the file with the given filename @a s and the options @a mode.
   */
  ChunkBuffer* ChunkBuffer::open(char* data, size_t dataSize, const std::vector<size_t>& chunkIds, size_t chunkSize, Mode mode, bool owned) _BTK_NOEXCEPT
  {
    if (this->isOpen())
      return 0;
    
    this->mp_Data = data;
    this->m_DataOwned = owned;
    this->m_DataSize = dataSize;
    this->m_ChunkIds = chunkIds;
    this->m_ChunkSize = chunkSize;
    this->m_ChunkOffset = (chunkIds.empty() ? -1 : chunkIds[0]*chunkSize);
    this->m_Offset = (data != 0) ? 0 : -1;
    this->m_Writing = (mode & Mode::Out) == Mode::Out;

    return this;
  };
  
  /**
   * @fn bool ChunkBuffer::isOpen() const _BTK_NOEXCEPT
   * Return true if the file is opened.
   */
   
  /**
   * Close the file.
   * @return Return 0 is an error happened during the closing.
   */ 
  ChunkBuffer* ChunkBuffer::close() _BTK_NOEXCEPT
  {
    if (!this->isOpen())
      return 0;

    if (this->m_DataOwned)
      delete[] this->mp_Data;
    this->mp_Data = 0;
    this->m_DataSize = 0;
    this->m_ChunkIds.clear();
    this->m_ChunkSize = 0;
    this->m_ChunkOffset = 0;
    this->m_Offset = 0;
    
    return this;
  };

  /**
   * @fn bool ChunkBuffer::hasWriteMode() const  _BTK_NOEXCEPT
   * Check if this file buffer is in write mode or not.
   */

  /**
   * @fn size_t ChunkBuffer::dataSize() const  _BTK_NOEXCEPT
   * Returns the size of the buffer.
   */
  
  /**
   * @fn const char* ChunkBuffer::data() const  _BTK_NOEXCEPT
   * Returns directly the content of the buffer.
   */
  
  /**
   * Sets internal position pointer to relative position.
   * @return The new position value of the modified position pointer. Errors are expected to be signaled by an invalid position value, like -1.
   */
  ChunkBuffer::Position ChunkBuffer::seek(Offset off, Origin whence) _BTK_NOEXCEPT
  {
    switch(whence)
    {
    case Origin::Begin:
      if (off < 0)
        return -1;
      this->m_Offset = off;
      break;
    case Origin::Current:
      if (this->m_Offset + off < 0)
        return -1;
      this->m_Offset += off;
      break;
    case Origin::End:
      if (this->m_DataSize + off < 0)
        return -1;
      this->m_Offset = this->m_DataSize + off;
      break;
    default:
      return -1;
    }
    return (this->updateChunkOffset(&(this->m_Offset), &(this->m_ChunkOffset)) ? this->m_Offset : -1);
  };
  
 /**
  * Returns a sequence of characters
  * @return The number of characters gotten.
  */
  ChunkBuffer::Size ChunkBuffer::peek(char* s, Size n) const _BTK_NOEXCEPT
  {
    n = (((this->m_Offset + n) == 0) || ((this->m_Offset + n) > this->m_DataSize)) ? ((this->m_DataSize - this->m_Offset) > 0 ? this->m_DataSize - this->m_Offset : 0) : n;
    Offset dataOffset = this->m_Offset;
    Offset chunkOffset = this->m_ChunkOffset;
    for (Offset i = 0 ; i < n ; ++i)
    {
      if (!this->updateChunkOffset(&dataOffset, &chunkOffset))
        return i;
      s[i] = this->mp_Data[chunkOffset];
      dataOffset += 1;
    }
    return n;
  };
  
  /**
   * Get a sequence of characters and modify the internal offset based on the number of characters read.
   * @return The number of characters gotten.
   */
  ChunkBuffer::Size ChunkBuffer::read(char* s, Size n) _BTK_NOEXCEPT
  {
    n = (((this->m_Offset + n) == 0) || ((this->m_Offset + n) > this->m_DataSize)) ? ((this->m_DataSize - this->m_Offset) > 0 ? this->m_DataSize - this->m_Offset : 0) : n;
    for (Offset i = 0 ; i < n ; ++i)
    {
      if (!this->updateChunkOffset(&(this->m_Offset), &(this->m_ChunkOffset)))
        return i;
      s[i] = this->mp_Data[this->m_ChunkOffset];
      this->m_Offset += 1;
    }
    return n;
  };
  
  /**
   * Write a sequence of characters
   * @return The number of characters written.
   */
  ChunkBuffer::Size ChunkBuffer::write(const char* s, Size n) _BTK_NOEXCEPT
  {
    // TODO: Reallocate the data if necessary
    if ((this->m_Offset + n) > this->m_DataSize)
      n = this->m_DataSize - this->m_Offset;
    
    for (Buffer::Offset i = 0 ; i < n ; ++i)
    {
      if (!this->updateChunkOffset(&(this->m_Offset), &(this->m_ChunkOffset)))
        return i;
      this->mp_Data[this->m_ChunkOffset] = s[i];
      this->m_Offset += 1;
    }

    return n;
  };
  
  const std::vector<size_t>& ChunkBuffer::chunkIDs() const
  {
    return this->m_ChunkIds;
  };
  
  size_t ChunkBuffer::chunkSize() const
  {
    return this->m_ChunkSize;
  };
  
  void ChunkBuffer::setChunks(const std::vector<size_t>& ids, size_t size)
  {
    this->m_ChunkIds = ids;
    this->m_ChunkSize = size;
    this->seek(0,Origin::Begin);
  };
  
  bool ChunkBuffer::updateChunkOffset(Offset* dataOffset, Offset* chunkOffset) const
  {
    size_t idx = *dataOffset / this->m_ChunkSize;
    if (idx > this->m_ChunkIds.size())
      return false;
    *chunkOffset = (this->m_ChunkIds[idx] * this->m_ChunkSize) + (*dataOffset % this->m_ChunkSize);
    return true;
  };
};

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

namespace btk
{
  /**
   * @class Buffer btkBuffer.h
   * @brief Device to read/write data from/to a file.
   *
   * Internally this class uses automatically a buffer mapped into computer's memory (see https://en.wikipedia.org/wiki/Memory-mapped_file).
   *
   * @warning Currently the Mode Append has no effect on this device.
   *
   * @ingroup BTKIO
   */
  
  /**
   * Default constructor. You must use the method Open after the use of this constructor
   */
  Buffer::Buffer()
  : IODevice(*new BufferPrivate)
  {};
  
  /**
   * Destructor (default).
   */
  Buffer::~Buffer() _BTK_NOEXCEPT = default;
  
  /**
   * Open a buffer in read-inly mode.
   */
  void Buffer::open(const char* data, size_t dataSize, bool owned)
  {
    this->open(const_cast<char*>(data), dataSize, std::vector<size_t>(1,0), dataSize, Mode::In, owned);
  };

  void Buffer::open(char* data, size_t dataSize, Mode mode, bool owned)
  {
    this->open(data, dataSize, std::vector<size_t>(1,0), dataSize, mode, owned);
  };
  
  void Buffer::open(char* data, size_t dataSize, const std::vector<size_t>& chunkIds, size_t chunkSize, Mode mode, bool owned)
  {
    if (this->verifyMode(mode))
    {
      auto optr = this->pimpl();
      if (!optr->Buffer->open(data, dataSize, chunkIds, chunkSize, mode, owned))
        this->setState(State::Fail);
      else
        this->clear();
    }
  };

  /**
   * Returns true if a file was successfuly opened, otherwise false.
   */
  bool Buffer::isOpen() const _BTK_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Buffer->isOpen();
  };
  
  /**
   * The use of this method without any file associated will set the State::Fail to true.
   */
  void Buffer::close()
  {
    auto optr = this->pimpl();
    if (!optr->Buffer->close())
      this->setState(State::Fail);
  };
  
  /**
   * Similar to the read() method but does not modify the position of the read indicator.
   * Returns the number of characters read. No exception or error flag is triggered when an error occurs.
   */
  Buffer::Size Buffer::peek(char* s, Size n) const
  {
    auto optr = this->pimpl();
    return optr->Buffer->peek(s,n);
  };
  
  /**
   * Gets from the device a sequence of characters of size @a n and store it in @a s. 
   * The State::Fail state flag is set if any issue happens during the reading operation. 
   * In case the number or characters read does not correspond to the number of characters to read @a n, the State::End state flag is also set.
   */
  void Buffer::read(char* s, Size n)
  {
    auto optr = this->pimpl();
    if (optr->Buffer->read(s,n) != n)
      this->setState(State::Fail | State::End);
  };
  
  /**
   * Puts the sequence of characters @a s of size @a n to the device.
   * The State::Error state flag is set if any issue happens during the wrting operation.
   * In case you attempt to write on the device while it is open in read-only mode, the State::Fail state flag will be set. 
   */
  void Buffer::write(const char* s, Size n)
  {
    auto optr = this->pimpl();
    if (!optr->Buffer->hasWriteMode()) // Read-only?
      this->setState(State::Fail);
    else if (optr->Buffer->write(s,n) != n)
      this->setState(State::Error);
  };
  
  /**
   * Moves the internal pointer of the given @a offset in the given direction @a dir.
   */
  void Buffer::seek(Offset offset, Origin whence)
  {
    auto optr = this->pimpl();
    if (!this->hasFailure() && (optr->Buffer->seek(offset, whence) == Position(Offset(-1))))
      this->setState(State::Fail);
  };
  
  /**
   * Returns the position of the internal pointer.
   */
  Buffer::Position Buffer::tell() const _BTK_NOEXCEPT
  {
    auto optr = this->pimpl();
    return !this->hasFailure() ? optr->Buffer->seek(0, Origin::Current) : Position(Offset(-1));
  };
  
  /**
   * Returns false as the Buffer class represents a random access device.
   */
  bool Buffer::isSequential() const _BTK_NOEXCEPT
  {
    return false;
  };
  
  /**
   * Return the data mapped in the memory
   */
  const char* Buffer::data() const _BTK_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Buffer->data();
  };
  
  /**
   * Return the size of the data in the buffer
   */
  Buffer::Size Buffer::size() const _BTK_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Buffer->dataSize();
  };
  
  const std::vector<size_t>& Buffer::chunkIDs() const _BTK_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Buffer->chunkIDs();
  };
  
  size_t Buffer::chunkSize() const _BTK_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Buffer->chunkSize();
  };
  
  void Buffer::setChunks(const std::vector<size_t>& ids, size_t size)
  {
    auto optr = this->pimpl();
    return optr->Buffer->setChunks(ids,size);
  };
};
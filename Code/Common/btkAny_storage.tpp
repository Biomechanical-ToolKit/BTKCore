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

#ifndef __btkAny_storage_tpp
#define __btkAny_storage_tpp

namespace btk
{
  struct Any::StorageBase
  { 
    StorageBase(void* data);
    StorageBase(const StorageBase& ) = delete;
    virtual ~StorageBase() noexcept;
    virtual int id() const noexcept = 0;
    virtual StorageBase* clone() const = 0;
    virtual bool compare(StorageBase* other) const noexcept = 0;
    
    void* Data;
  };
  
  template <typename T>
  struct Any::Storage : public Any::StorageBase
  {
    static_assert(std::is_copy_constructible<T>::value, "Impossible to use the btk::Any class with a type which does not have a copy constructor.");
    
    Storage(const T& value);
    ~Storage() noexcept;
    virtual int id() const noexcept final;
    virtual StorageBase* clone() const final;
    virtual bool compare(StorageBase* other) const noexcept final;
  };
  
  // ----------------------------------------------------------------------- //
  
  inline Any::StorageBase::StorageBase(void* data)
  : Data(data)
  {};
  
  inline Any::StorageBase::~StorageBase() noexcept
  {};
  
  // ----------------------------------------------------------------------- //
  
  template <typename T> 
  inline Any::Storage<T>::Storage(const T& value)
  : Any::StorageBase(new T(value))
  {};
  
  template <typename T> 
  inline Any::Storage<T>::~Storage() noexcept
  {
    delete static_cast<T*>(this->Data);
  };
  
  template <typename T> 
  inline Any::StorageBase* Any::Storage<T>::clone() const
  {
    return new Any::Storage<T>(*static_cast<T*>(this->Data));
  };
  
  template <typename T> 
  inline bool Any::Storage<T>::compare(StorageBase* other) const noexcept
  {
    if ((this->Data == nullptr) || (other->Data == nullptr))
      return false;
    if (this->id() != other->id())
      return false;
    return (*static_cast<T*>(this->Data) == *static_cast<T*>(other->Data));
  };
  
  template <typename T> 
  inline int Any::Storage<T>::id() const noexcept
  {
    return Traits<T>::ID;
  };
};

#endif // __btkAny_storage_tpp

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

#ifndef __btkAny_storage_tpp
#define __btkAny_storage_tpp

#include <string>
#include <vector>

namespace btk
{
  struct Any::StorageBase
  { 
    StorageBase(void* data);
    StorageBase(const StorageBase& ) = delete;
    virtual ~StorageBase() noexcept;
    virtual typeid_t id() const noexcept = 0;
    virtual bool is_arithmetic() const noexcept = 0;
    virtual std::vector<size_t> dimensions() const noexcept = 0;
    virtual size_t size() const noexcept = 0;
    virtual StorageBase* clone() const = 0;
    virtual bool compare(StorageBase* other) const noexcept = 0;
    
    void* Data;
  };
  
  template <typename T>
  struct Any::StorageSingle : public Any::StorageBase
  {
    static_assert(std::is_copy_constructible<T>::value, "Impossible to use the btk::Any class with a type which does not have a copy constructor.");
    
    template <typename U> StorageSingle(U&& value);
    ~StorageSingle() noexcept;
    virtual typeid_t id() const noexcept final;
    virtual bool is_arithmetic() const noexcept final;
    virtual std::vector<size_t> dimensions() const noexcept final;
    virtual size_t size() const noexcept final;
    virtual StorageBase* clone() const final;
    virtual bool compare(StorageBase* other) const noexcept final;
  };
  
  template <typename T>
  struct Any::StorageArray : public Any::StorageBase
  {
    static_assert(std::is_copy_constructible<T>::value, "Impossible to use the btk::Any class with a type which does not have a copy constructor.");
    
    template <typename U> StorageArray(std::initializer_list<U> values, std::initializer_list<size_t> dimensions);
    template <typename U> StorageArray(const std::vector<U>& values, const std::vector<size_t>& dimensions);
    template <typename U> StorageArray(const U* values, size_t numValues, const size_t* dimensions, size_t numDims);
    ~StorageArray() noexcept;
    virtual typeid_t id() const noexcept final;
    virtual bool is_arithmetic() const noexcept final;
    virtual std::vector<size_t> dimensions() const noexcept final;
    virtual size_t size() const noexcept final;
    virtual StorageBase* clone() const final;
    virtual bool compare(StorageBase* other) const noexcept final;
    size_t NumValues;
    size_t* Dimensions;
    size_t NumDims;
  };
  
  // ----------------------------------------------------------------------- //
  
  inline Any::StorageBase::StorageBase(void* data)
  : Data(data)
  {};
  
  inline Any::StorageBase::~StorageBase() noexcept
  {};
  
  // ----------------------------------------------------------------------- //
  
  template <typename T> 
  template <typename U> 
  inline Any::StorageSingle<T>::StorageSingle(U&& value)
  : Any::StorageBase(new T(std::forward<U>(value)))
  {};
  
  template <typename T> 
  inline Any::StorageSingle<T>::~StorageSingle() noexcept
  {
    delete static_cast<T*>(this->Data);
  };

  template <typename T>
  inline std::vector<size_t> Any::StorageSingle<T>::dimensions() const noexcept
  {
    return std::vector<size_t>{};
  };

  template <typename T>
  inline size_t Any::StorageSingle<T>::size() const noexcept
  {
    return 1ul;
  };
  
  template <typename T> 
  inline Any::StorageBase* Any::StorageSingle<T>::clone() const
  {
    return new Any::StorageSingle<T>(*static_cast<T*>(this->Data));
  };
  
  template <typename T> 
  inline bool Any::StorageSingle<T>::compare(StorageBase* other) const noexcept
  {
    if ((this->Data == nullptr) || (other->Data == nullptr))
      return false;
    if (this->id() != other->id())
      return false;
    return (*static_cast<T*>(this->Data) == *static_cast<T*>(other->Data));
  };
  
  template <typename T> 
  inline typeid_t Any::StorageSingle<T>::id() const noexcept
  {
    return static_typeid<T>();
  };
  
  template <typename T> 
  bool Any::StorageSingle<T>::is_arithmetic() const noexcept
  {
    return std::is_arithmetic<T>::value;
  };
  
  // ----------------------------------------------------------------------- //
  
  template <typename T>
  template <typename U>
  inline Any::StorageArray<T>::StorageArray(std::initializer_list<U> values, std::initializer_list<size_t> dimensions)
  : Any::StorageArray<T>(values.begin(),values.size(),dimensions.begin(),dimensions.size())
  {};
  
  template <typename T>
  template <typename U>
  inline Any::StorageArray<T>::StorageArray(const std::vector<U>& values, const std::vector<size_t>& dimensions)
  : Any::StorageArray<T>(values.data(),values.size(),dimensions.data(),dimensions.size())
  {};

  template <typename T>
  template <typename U>
  inline Any::StorageArray<T>::StorageArray(const U* values, size_t numValues, const size_t* dimensions, size_t numDims)
  : Any::StorageBase(new T[numValues]), NumValues(numValues), Dimensions(new size_t[numDims]),  NumDims(numDims)
  {
    memcpy(static_cast<T*>(this->Data), values, numValues*sizeof(T));
    memcpy(this->Dimensions, dimensions, numDims*sizeof(size_t));
  };
  
  template <typename T> 
  inline Any::StorageArray<T>::~StorageArray() noexcept
  {
    delete[] static_cast<T*>(this->Data);
    delete[] this->Dimensions;
  };

  template <typename T>
  inline std::vector<size_t> Any::StorageArray<T>::dimensions() const noexcept
  {
    auto dims = std::vector<size_t>(this->NumDims,0ul);
    for (size_t i = 0 ; i < this->NumDims ; ++i)
      dims[i] = this->Dimensions[i];
    return dims;
  };

  template <typename T>
  inline size_t Any::StorageArray<T>::size() const noexcept
  {
    return this->NumValues;
  };
  
  template <typename T> 
  inline Any::StorageBase* Any::StorageArray<T>::clone() const
  {
    return new Any::StorageArray<T>(static_cast<T*>(this->Data),this->NumValues,this->Dimensions,this->NumDims);
  };
  
  template <typename T> 
  inline bool Any::StorageArray<T>::compare(StorageBase* other) const noexcept
  {
    if ((this->Data == nullptr) || (other->Data == nullptr))
      return false;
    if (this->id() != other->id())
      return false;
    if (this->size() != other->size())
      return false;
    if (this->dimensions() != other->dimensions())
      return false;
    return (*static_cast<T*>(this->Data) == *static_cast<T*>(other->Data));
  };
  
  template <typename T> 
  inline typeid_t Any::StorageArray<T>::id() const noexcept
  {
    return static_typeid<T>();
  };
  
  template <typename T> 
  bool Any::StorageArray<T>::is_arithmetic() const noexcept
  {
    return std::is_arithmetic<T>::value;
  };
  
  // ----------------------------------------------------------------------- //
  //                       Storage Partial specializations
  // ----------------------------------------------------------------------- //
  
  template <size_t N>
  struct Any::StorageSingle<char[N]> : public Any::StorageSingle<std::string>
  {
    StorageSingle(const char(&value)[N]) : StorageSingle<std::string>(std::string(value,N-1)) {};
  };
};

#endif // __btkAny_storage_tpp

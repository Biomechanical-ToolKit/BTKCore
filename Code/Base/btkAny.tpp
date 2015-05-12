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

#ifndef __btkAny_tpp
#define __btkAny_tpp

#include "btkException.h"
#include "btkTypeTraits.h"

#include <string>
#include <unordered_map>
#include <algorithm> // std::copy
#include <cstdlib> // strtol, strtoll, strtoul, strtoull, strtof, strtod, ...
#include <cstring> // strlen, strcmp
#include <type_traits>

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

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
    virtual void* element(size_t idx) const noexcept = 0;
    
    // Both methods below are defined after the declaration of the Converter private API
    template <typename U> void cast(U* value) const noexcept;
    template <typename U> void cast(U* value, size_t index) const noexcept;
    
    void* Data;
  };
  
  // WARNING : This class does store only the pointer. It does not freed the associated memory. This is the responsability of the inherited class if some meory was allocated for the given @a data pointer.
  inline Any::StorageBase::StorageBase(void* data)
  : Data(data)
  {};
  
  inline Any::StorageBase::~StorageBase() noexcept
  {};

  // ******************************** DETAILS ****************************** //

  struct Any::details
  {
    details() = delete;
    ~details() noexcept = delete;
    details(const details& ) = delete;
    details(details&& ) noexcept = delete;
    details& operator=(const details& ) = delete;
    details& operator=(details&& ) noexcept = delete;
    
    // ------------------------------- UTILS ------------------------------ //
    
    static Converter& converter() noexcept;
    
    using convert_t = void(*)(void*,void*);
    static convert_t extract_converter(typeid_t sid, typeid_t rid) noexcept;
    
    // Single conversion
    template <typename U>
    static inline typename std::enable_if<!is_stl_vector<typename std::decay<U>::type>::value && !is_stl_array<typename std::decay<U>::type>::value>::type convert(U* value, const Any::StorageBase* storage) noexcept
    {
      convert_t doConversion = extract_converter(storage->id(),static_typeid<U>());
      if (doConversion != nullptr)
        doConversion(storage->Data,value);
    };
    
    // (std) Vector conversion
    template <typename U>
    static inline typename std::enable_if<is_stl_vector<typename std::decay<U>::type>::value>::type convert(U* value, const Any::StorageBase* storage) noexcept
    {
      convert_t doConversion = extract_converter(storage->id(),static_typeid<typename std::decay<U>::type::value_type>());
      if (doConversion != nullptr)
      {
        value->resize(storage->size());
        for (size_t i = 0 ; i < value->size() ; ++i)
          doConversion(storage->element(i),&value->operator[](i));
      }
    };
    
    // (std) Array conversion
    template <typename U>
    static inline typename std::enable_if<is_stl_array<typename std::decay<U>::type>::value>::type convert(U* value, const Any::StorageBase* storage) noexcept
    {
      convert_t doConversion = extract_converter(storage->id(),static_typeid<typename std::decay<U>::type::value_type>());
      if (doConversion != nullptr)
      {
        for (size_t i = 0 ; i < value->size() ; ++i)
          doConversion(storage->element(i),&value->operator[](i));
      }
    };
    
    // Element conversion
    template <typename U>
    static inline void convert(U* value, const Any::StorageBase* storage, size_t idx) noexcept
    {
      convert_t doConversion = extract_converter(storage->id(),static_typeid<U>());
      if (doConversion != nullptr)
        doConversion(storage->element(idx),value);
    };
    
    // Compare a Any object with a value which has another type
    template <typename U>
    static inline typename std::enable_if<!std::is_same<typename std::decay<U>::type, const char*>::value, bool>::type is_equal(const Any* lhs, U&& rhs) noexcept
    {
      return (lhs->cast<typename std::decay<U>::type>() == rhs);
    };

    // Compare a Any object with a const char* value
    template <typename U>
    static inline typename std::enable_if<std::is_same<typename std::decay<U>::type, const char*>::value, bool>::type is_equal(const Any* lhs, U&& rhs) noexcept
    {
      const char* str = lhs->cast<const char*>();
      return ((str != nullptr) && (rhs != nullptr) && (strcmp(str, rhs) == 0));
    }    
    
    // Should be used only on size_t values coming from typeid_t variables
    static inline constexpr size_t hash(size_t sid, size_t rid) noexcept
    {
      return ((sid << (4*sizeof(size_t))) | ((rid << (4*sizeof(size_t))) >> (4*sizeof(size_t))));
    };
    
    // ---------------------- DATA STORAGE DECLARATION --------------------- //

    template <typename T>
    struct StorageSingle : public Any::StorageBase
    {
      static_assert(std::is_copy_constructible<T>::value, "Impossible to use the btk::Any class with a type which does not have a copy constructor.");
      static_assert(!std::is_pointer<T>::value, "Impossible to store a pointer type.");
      
      template <typename U> StorageSingle(U* value);
      ~StorageSingle() noexcept;
      virtual typeid_t id() const noexcept final;
      virtual bool is_arithmetic() const noexcept final;
      virtual std::vector<size_t> dimensions() const noexcept final;
      virtual size_t size() const noexcept final;
      virtual StorageBase* clone() const final;
      virtual bool compare(StorageBase* other) const noexcept final;
      virtual void* element(size_t idx) const noexcept final;
    };
  
    template <typename T>
    struct StorageArray : public Any::StorageBase
    {
      static_assert(std::is_copy_constructible<T>::value, "Impossible to use the btk::Any class with a type which does not have a copy constructor.");
      static_assert(!std::is_pointer<T>::value, "Impossible to store a pointer type.");
    
      template <typename U> StorageArray(U* values, size_t numValues, const size_t* dimensions, size_t numDims);
      ~StorageArray() noexcept;
      virtual typeid_t id() const noexcept final;
      virtual bool is_arithmetic() const noexcept final;
      virtual std::vector<size_t> dimensions() const noexcept final;
      virtual size_t size() const noexcept final;
      virtual StorageBase* clone() const final;
      virtual bool compare(StorageBase* other) const noexcept final;
      virtual void* element(size_t idx) const noexcept final;
      size_t NumValues;
      const size_t* Dimensions;
      size_t NumDims;
    };
    
    template <typename T, typename = void>
    struct adapt {};
    
    template <typename T>
    struct adapt<T, typename std::enable_if<!std::is_enum<T>::value && !std::is_same<typename std::remove_extent<T>::type,char>::value>::type>
    {
      using type = T;
      template <typename U>
      static inline T* single(U&& value)
      {
        return new T(value);
      };
      template <typename U>
      static inline T* array(size_t newarraylen, U* values, size_t num)
      {
        T* data = new T[newarraylen];
        std::copy(values,values+num,data);
        return data;
      };
      adapt() = delete;
      ~adapt() noexcept = delete;
      adapt(const adapt& ) = delete;
      adapt(adapt&& ) noexcept = delete;
      adapt& operator=(const adapt& ) = delete;
      adapt& operator=(adapt&& ) noexcept = delete;
    };
    
    template <typename T, size_t N>
    struct adapt<T[N], typename std::enable_if<std::is_same<T,char>::value>::type>
    {
      using type = std::string;
      static inline std::string* single(const char(&value)[N])
      {
        return new std::string(value,N-1);
      };
      adapt() = delete;
      ~adapt() noexcept = delete;
      adapt(const adapt& ) = delete;
      adapt(adapt&& ) noexcept = delete;
      adapt& operator=(const adapt& ) = delete;
      adapt& operator=(adapt&& ) noexcept = delete;
    };
    
    template <typename T>
    struct adapt<T, typename std::enable_if<std::is_enum<T>::value>::type>
    {
      using type = typename std::underlying_type<T>::type;
      template <typename U>
      static inline type* single(U&& value)
      {
        return new type(value);
      };
      template <typename U>
      static inline type* array(size_t newarraylen, U* values, size_t num)
      {
        type* data = new type[newarraylen];
        for (size_t i = 0 ; i < num ; ++i)
          data[i] = static_cast<type>(values[i]);
        return data;
      };
      adapt() = delete;
      ~adapt() noexcept = delete;
      adapt(const adapt& ) = delete;
      adapt(adapt&& ) noexcept = delete;
      adapt& operator=(const adapt& ) = delete;
      adapt& operator=(adapt&& ) noexcept = delete;
    };
    
    // NOTE: An explicit specialization of adapt<const char*> is available outside of the class declaration
        
    // The dimensions is not used in the single case
    template <typename U, typename D>
    static inline typename std::enable_if<
         !is_stl_initializer_list<typename std::decay<U>::type>::value
      && !is_stl_vector<typename std::decay<U>::type>::value
      && !is_stl_array<typename std::decay<U>::type>::value
      , Any::StorageBase*>::type
    store(U&& value, D&& )
    {
      using adapter = adapt<typename std::remove_cv<typename std::remove_reference<U>::type>::type>;
      return new StorageSingle<typename adapter::type>(adapter::single(std::forward<U>(value)));
    };
    
    // From vectors
    template <typename U, typename D>
    static inline typename std::enable_if<
         (is_stl_vector<typename std::decay<U>::type>::value || is_stl_array<typename std::decay<U>::type>::value)
      && (is_stl_vector<typename std::decay<D>::type>::value || is_stl_array<typename std::decay<D>::type>::value)
      , Any::StorageBase*>::type
    store(U&& values, D&& dimensions)
    {
      static_assert(std::is_integral<typename std::decay<D>::type::value_type>::value, "The given dimensions must be a vector with a value_type set to an integral type (e.g. int or size_t).");
      return store(values.data(),values.size(),dimensions.data(),dimensions.size());
    };
    
    template <typename U, typename D>
    static inline typename std::enable_if<
         (is_stl_vector<typename std::decay<U>::type>::value || is_stl_array<typename std::decay<U>::type>::value)
      && std::is_same<D,void*>::value
      , Any::StorageBase*>::type
    store(U&& values, D&& )
    {
      size_t dims[1] = {values.size()};
      return store(values.data(),values.size(),dims,1ul);
    };
    
    // From initializer lists
    template <typename U, typename D>
    static inline typename std::enable_if<
         is_stl_initializer_list<typename std::decay<U>::type>::value
      && is_stl_initializer_list<typename std::decay<D>::type>::value
      , Any::StorageBase*>::type
    store(U&& values, D&& dimensions)
    {
      static_assert(std::is_integral<typename std::decay<D>::type::value_type>::value, "The given dimensions must be a initializer_list with a value_type set to an integral type (e.g. int or size_t).");
      return store(values.begin(),values.size(),dimensions.begin(),dimensions.size());
    };
    
    // From pointers
    template <typename U, typename D>
    static inline Any::StorageBase* store(U* values, size_t numValues, D* dimensions, size_t numDims)
    {
      using adapter = adapt<typename std::remove_cv<typename std::remove_reference<U>::type>::type>;
      // NOTE: The arrays are not deleted as they are onwed by the StorageArray class.
      typename adapter::type* data = nullptr;
      size_t* dims = nullptr;
      if (numDims != 0)
      {
        size_t size = 1;
        dims = new size_t[numDims];
        for (size_t i = 0 ; i < numDims ; ++i)
        {
          dims[i] = static_cast<size_t>(dimensions[i]);
          size *= dims[i];
        }
        data = adapter::array(size,values,std::min(size,numValues));
        for (size_t i = numValues ; i < size ; ++i)
          data[i] = typename adapter::type();
        numValues = size;
      }
      else
      {
        data = adapter::array(numValues,values,numValues);
        numDims = 1;
        dims = new size_t[1];
        dims[0] = numValues;
      }
      Any::StorageBase* storage = new StorageArray<typename adapter::type>(data, numValues, dims, numDims);
      return storage;
    };
    
    // ---------------------------- DATA CAST ------------------------------ //
    
    // Default cast
    template <typename U>
    static inline typename std::enable_if<
         !std::is_arithmetic<typename std::decay<U>::type>::value
      && !std::is_same<std::string, typename std::decay<U>::type>::value
      && !std::is_same<const char*, typename std::decay<U>::type>::value
      && !std::is_enum<typename std::decay<U>::type>::value
      && !is_stl_vector<typename std::decay<U>::type>::value
      && !is_stl_array<typename std::decay<U>::type>::value
      , bool>::type cast(U* value, const StorageBase* storage, size_t idx = 0) noexcept
    {
      using value_t = typename std::decay<U>::type;
      if (storage->id() == static_typeid<value_t>())
      {
        *value = static_cast<value_t*>(storage->Data)[idx];
        return true;
      }
      return false;
    };
    
    // Arithmetic conversion
    template <typename U>
    static typename std::enable_if<std::is_arithmetic<typename std::decay<U>::type>::value, bool>::type cast(U* value, const StorageBase* storage, size_t idx = 0) noexcept
    {
      const typeid_t id = storage->id();
      if (storage->is_arithmetic())
      {
        // bool
        if (id == static_typeid<bool>())
          *value = static_cast<U>(static_cast<bool*>(storage->Data)[idx]);
        // char
        else if (id == static_typeid<char>())
          *value = static_cast<U>(static_cast<char*>(storage->Data)[idx]);
        // char16_t
        else if (id == static_typeid<char16_t>())
          *value = static_cast<U>(static_cast<char16_t*>(storage->Data)[idx]);
        // char32_t
        else if (id == static_typeid<char32_t>())
          *value = static_cast<U>(static_cast<char32_t*>(storage->Data)[idx]);
        // wchar_t
        else if (id == static_typeid<wchar_t>())
          *value = static_cast<U>(static_cast<wchar_t*>(storage->Data)[idx]);
        // signed char
        else if (id == static_typeid<signed char>())
          *value = static_cast<U>(static_cast<signed char*>(storage->Data)[idx]);
        // short int
        else if (id == static_typeid<short int>())
          *value = static_cast<U>(static_cast<short int*>(storage->Data)[idx]);
        // int
        else if (id == static_typeid<int>())
          *value = static_cast<U>(static_cast<int*>(storage->Data)[idx]);
        // long int
        else if (id == static_typeid<long int>())
          *value = static_cast<U>(static_cast<long int*>(storage->Data)[idx]);
        // long long int
        else if (id == static_typeid<long long int>())
          *value = static_cast<U>(static_cast<long long int*>(storage->Data)[idx]);
        // unsigned char
        else if (id == static_typeid<unsigned char>())
          *value = static_cast<U>(static_cast<unsigned char*>(storage->Data)[idx]);
        // unsigned short int
        else if (id == static_typeid<unsigned short int>())
          *value = static_cast<U>(static_cast<unsigned short int*>(storage->Data)[idx]);
        // unsigned int
        else if (id == static_typeid<unsigned int>())
          *value = static_cast<U>(static_cast<unsigned int*>(storage->Data)[idx]);
        // unsigned long int
        else if (id == static_typeid<unsigned long int>())
          *value = static_cast<U>(static_cast<unsigned long int*>(storage->Data)[idx]);
        // unsigned long long int
        else if (id == static_typeid<unsigned long long int>())
          *value = static_cast<U>(static_cast<unsigned long long int*>(storage->Data)[idx]);
        // float
        else if (id == static_typeid<float>())
          *value = static_cast<U>(static_cast<float*>(storage->Data)[idx]);
        // double
        else if (id == static_typeid<double>())
          *value = static_cast<U>(static_cast<double*>(storage->Data)[idx]);
        // long double
        else if (id == static_typeid<long double>())
          *value = static_cast<U>(static_cast<long double*>(storage->Data)[idx]);
        // ERROR - Should not be possible! All the standard arithmetic type in C++11 are listed above
        else
          throw(LogicError("Unexpected error during arithmetic to arithmetic conversion!"));
        return true;
      }
      else if (id == static_typeid<std::string>())
      {
        const char* str = static_cast<std::string*>(storage->Data)[idx].c_str();
        cast_from_string(value,str);
        return true;
      }
      return false;
    };
    
    // String conversion
    template <typename U>
    static typename std::enable_if<std::is_same<std::string, typename std::decay<U>::type>::value, bool>::type cast(U* value, const StorageBase* storage, size_t idx = 0) noexcept
    {
      const typeid_t id = storage->id();
      if (id == static_typeid<std::string>())
      {
        *value = static_cast<std::string*>(storage->Data)[idx];
        return true;
      }
      else if (storage->is_arithmetic()
               && (id != static_typeid<char16_t>())
               && (id != static_typeid<char32_t>())
               && (id != static_typeid<wchar_t>()))
      {
        // bool
        if (id == static_typeid<bool>())
          *value = std::string(static_cast<bool*>(storage->Data)[idx] ? "true" : "false");
        // char (convert as it is an int8_t)
        else if (id == static_typeid<char>())
          *value = std::to_string((short int)static_cast<char*>(storage->Data)[idx]);
        // signed char (convert as it is a signed int8_t)
        else if (id == static_typeid<signed char>())
          *value = std::to_string((signed short int)static_cast<signed char*>(storage->Data)[idx]);
        // short int
        else if (id == static_typeid<short int>())
          *value = std::to_string(static_cast<short int*>(storage->Data)[idx]);
        // int
        else if (id == static_typeid<int>())
          *value = std::to_string(static_cast<int*>(storage->Data)[idx]);
        // long int
        else if (id == static_typeid<long int>())
          *value = std::to_string(static_cast<long int*>(storage->Data)[idx]);
        // long long int
        else if (id == static_typeid<long long int>())
          *value = std::to_string(static_cast<long long int*>(storage->Data)[idx]);
        // unsigned char (convert as it is a unsigned int8_t)
        else if (id == static_typeid<unsigned char>())
          *value = std::to_string((unsigned short int)static_cast<unsigned char*>(storage->Data)[idx]);
        // unsigned short int
        else if (id == static_typeid<unsigned short int>())
          *value = std::to_string(static_cast<unsigned short int*>(storage->Data)[idx]);
        // unsigned int
        else if (id == static_typeid<unsigned int>())
          *value = std::to_string(static_cast<unsigned int*>(storage->Data)[idx]);
        // unsigned long int
        else if (id == static_typeid<unsigned long int>())
          *value = std::to_string(static_cast<unsigned long int*>(storage->Data)[idx]);
        // unsigned long long int
        else if (id == static_typeid<unsigned long long int>())
          *value = std::to_string(static_cast<unsigned long long int*>(storage->Data)[idx]);
        // float
        else if (id == static_typeid<float>())
          *value = std::to_string(static_cast<float*>(storage->Data)[idx]);
        // double
        else if (id == static_typeid<double>())
          *value = std::to_string(static_cast<double*>(storage->Data)[idx]);
        // long double
        else if (id == static_typeid<long double>())
          *value = std::to_string(static_cast<long double*>(storage->Data)[idx]);
        // ERROR - Should not be possible! All the standard arithmetic type in C++11 are listed above
        else
          throw(LogicError("Unexpected error during arithmetic to string conversion!"));
        return true;
      }
      return false;
    };
    
    // const char* conversion
    template <typename U>
    static typename std::enable_if<std::is_same<const char*, typename std::decay<U>::type>::value, bool>::type cast(U* value, const StorageBase* storage, size_t idx = 0) noexcept
    {
      if (storage->id() == static_typeid<std::string>())
      {
        *value = static_cast<std::string*>(storage->Data)[idx].c_str();
        return true;
      }
      return false;
    }
    
    // enum conversion
    template <typename U>
    static typename std::enable_if<std::is_enum<typename std::decay<U>::type>::value, bool>::type cast(U* value, const StorageBase* storage, size_t idx = 0) noexcept
    {
      using underlying_t = typename std::underlying_type<typename std::decay<U>::type>::type;
      if (storage->is_arithmetic())
      {
        underlying_t v = underlying_t();
        cast(&v,storage,idx);
        *value = static_cast<U>(v);
        return true;
      }
      return false;
    }
    
    // (std) Vector conversion
    template <typename U>
    static typename std::enable_if<is_stl_vector<typename std::decay<U>::type>::value, bool>::type cast(U* value, const StorageBase* storage) noexcept
    {
      using value_t = typename std::decay<U>::type::value_type;
      bool res = true;
      value->resize(storage->size());
      for (size_t i = 0 ; i < value->size() ; ++i)
        res &= cast(&value->operator[](i),storage,i);
      return res;
    };
    
    // (std) Array conversion
    template <typename U>
    static typename std::enable_if<is_stl_array<typename std::decay<U>::type>::value, bool>::type cast(U* value, const StorageBase* storage) noexcept
    {
      using value_t = typename std::decay<U>::type::value_type;
      bool res = true;
      const size_t size = std::min(value->size(),storage->size());
      for (size_t i = 0 ; i < size ; ++i)
        res &= cast(&value->operator[](i),storage,i);
      for (size_t i = size ; i < value->size() ; ++i)
        value->operator[](i) = typename std::decay<U>::type::value_type();
      return res;
    };
  
    // --------------------------------------------------------------------- //
    
    template <typename U>
    static inline typename std::enable_if<std::is_same<bool, typename std::decay<U>::type>::value>::type cast_from_string(U* value, const char* str) noexcept
    {
      *value = (!((strlen(str) == 0) || (strcmp(str,"0") == 0) || (strcmp(str,"false") == 0)));
    };
  
    template <typename U>
    static inline typename std::enable_if<std::is_integral<typename std::decay<U>::type>::value && !std::is_same<bool, typename std::decay<U>::type>::value && std::is_signed<typename std::decay<U>::type>::value && (sizeof(typename std::decay<U>::type) > sizeof(long))>::type cast_from_string(U* value, const char* str) noexcept
    {
      *value = strtoll(str,nullptr,0);
    };
  
    template <typename U>
    static inline typename std::enable_if<std::is_integral<typename std::decay<U>::type>::value && !std::is_same<bool, typename std::decay<U>::type>::value && std::is_signed<typename std::decay<U>::type>::value && (sizeof(typename std::decay<U>::type) <= sizeof(long))>::type cast_from_string(U* value, const char* str) noexcept
    {
      *value = strtol(str,nullptr,0);
    };
  
    template <typename U>
    static inline typename std::enable_if<std::is_integral<typename std::decay<U>::type>::value && !std::is_same<bool, typename std::decay<U>::type>::value && std::is_unsigned<typename std::decay<U>::type>::value && (sizeof(typename std::decay<U>::type) > sizeof(long))>::type cast_from_string(U* value, const char* str) noexcept
    {
      *value = strtoull(str,nullptr,0);
    };
  
    template <typename U>
    static inline typename std::enable_if<std::is_integral<typename std::decay<U>::type>::value && !std::is_same<bool, typename std::decay<U>::type>::value && std::is_unsigned<typename std::decay<U>::type>::value && (sizeof(typename std::decay<U>::type) <= sizeof(long))>::type cast_from_string(U* value, const char* str) noexcept
    {
      *value = strtol(str,nullptr,0);
    };
  
    template <typename U>
    static inline typename std::enable_if<std::is_same<float, typename std::decay<U>::type>::value>::type cast_from_string(U* value, const char* str) noexcept
    {
      *value = strtof(str,nullptr);
    };
  
    template <typename U>
    static inline typename std::enable_if<std::is_same<double, typename std::decay<U>::type>::value>::type cast_from_string(U* value, const char* str) noexcept
    {
      *value = strtod(str,nullptr);
    };
  
    template <typename U>
    static inline typename std::enable_if<std::is_same<long double, typename std::decay<U>::type>::value>::type cast_from_string(U* value, const char* str) noexcept
    {
      *value = strtold(str,nullptr);
    };
  };
  
  // --------------------------------------------------------------------- //
  
  template <>
  struct Any::details::adapt<const char*> : Any::details::adapt<std::string>
  {
    static inline std::string* single(const char* value)
    {
      return new std::string(value);
    };
    static inline std::string* array(size_t newarraylen, const char* const* values, size_t num)
    {
      std::string* data = new std::string[newarraylen];
      for (size_t i = 0 ; i < num ; ++i)
        data[i].assign(values[i]);
      return data;
    };
  };

  template <typename T> 
  template <typename U> 
  inline Any::details::StorageSingle<T>::StorageSingle(U* value)
  : Any::StorageBase(value)
  {};

  template <typename T> 
  inline Any::details::StorageSingle<T>::~StorageSingle() noexcept
  {
    delete static_cast<T*>(this->Data);
  };

  template <typename T>
  inline std::vector<size_t> Any::details::StorageSingle<T>::dimensions() const noexcept
  {
    return std::vector<size_t>{};
  };

  template <typename T>
  inline size_t Any::details::StorageSingle<T>::size() const noexcept
  {
    return 1ul;
  };

  template <typename T> 
  inline Any::StorageBase* Any::details::StorageSingle<T>::clone() const
  {
    return new Any::details::StorageSingle<T>(new T(*static_cast<T*>(this->Data)));
  };

  template <typename T> 
  inline bool Any::details::StorageSingle<T>::compare(StorageBase* other) const noexcept
  {
    if ((this->Data == nullptr) || (other->Data == nullptr))
      return false;
    T value = T();
    other->cast<T>(&value);
    return *static_cast<T*>(this->Data) == value;
    
  };

  template <typename T> 
  inline typeid_t Any::details::StorageSingle<T>::id() const noexcept
  {
    return static_typeid<T>();
  };

  template <typename T> 
  bool Any::details::StorageSingle<T>::is_arithmetic() const noexcept
  {
    return std::is_arithmetic<T>::value;
  };
  
  template <typename T> 
  void* Any::details::StorageSingle<T>::element(size_t ) const noexcept
  {
    return this->Data;
  };

  // --------------------------------------------------------------------- //

  // NOTE: The class take the ownership of the data. It will delete the array pointer. This constructor must be used only with allocated array (and not vector data or initializer_list content)
  template <typename T>
  template <typename U>
  inline Any::details::StorageArray<T>::StorageArray(U* values, size_t numValues, const size_t* dimensions, size_t numDims)
  : Any::StorageBase(values), NumValues(numValues), Dimensions(dimensions),  NumDims(numDims)
  {};

  template <typename T> 
  inline Any::details::StorageArray<T>::~StorageArray() noexcept
  {
    delete[] static_cast<T*>(this->Data);
    delete[] this->Dimensions;
  };

  template <typename T>
  inline std::vector<size_t> Any::details::StorageArray<T>::dimensions() const noexcept
  {
    auto dims = std::vector<size_t>(this->NumDims,0ul);
    for (size_t i = 0 ; i < this->NumDims ; ++i)
      dims[i] = this->Dimensions[i];
    return dims;
  };

  template <typename T>
  inline size_t Any::details::StorageArray<T>::size() const noexcept
  {
    return this->NumValues;
  };

  template <typename T> 
  inline Any::StorageBase* Any::details::StorageArray<T>::clone() const
  {
    T* data = new T[this->NumValues];
    size_t* dims = new size_t[this->NumDims];
    std::copy(static_cast<T*>(this->Data),static_cast<T*>(this->Data)+this->NumValues,data);
    memcpy(dims, this->Dimensions, this->NumDims*sizeof(size_t));
    return new Any::details::StorageArray<T>(data,this->NumValues,dims,this->NumDims);
  };

  template <typename T> 
  inline bool Any::details::StorageArray<T>::compare(StorageBase* other) const noexcept
  {
    if ((this->Data == nullptr) || (other->Data == nullptr))
      return false;
    if (this->size() != other->size())
      return false;
    T value = T();
    other->cast<T>(&value);
    return *static_cast<T*>(this->Data) == value;
  };

  template <typename T> 
  inline typeid_t Any::details::StorageArray<T>::id() const noexcept
  {
    return static_typeid<T>();
  };

  template <typename T> 
  bool Any::details::StorageArray<T>::is_arithmetic() const noexcept
  {
    return std::is_arithmetic<T>::value;
  };
  
  template <typename T> 
  void* Any::details::StorageArray<T>::element(size_t idx) const noexcept
  {
    return static_cast<void*>(&static_cast<T*>(this->Data)[idx]);
  };
  
  // ----------------------------------------------------------------------- //
  
  template <typename U>
  inline void Any::StorageBase::cast(U* value) const noexcept
  {
    if (this->id() == static_typeid<U>())
      *value = *static_cast<U*>(this->Data);
    else if (!details::cast(value, this))
      details::convert(value, this);
  };
  
  template <typename U>
  inline void Any::StorageBase::cast(U* value, size_t idx) const noexcept
  {
    if (this->id() == static_typeid<U>())
      *value = static_cast<U*>(this->Data)[idx];
    else if (!details::cast(value, this, idx))
      details::convert(value, this, idx);
  };

  // ******************************* CONVERTER ****************************** //

  struct Any::Converter
  {
    // Typedef
    using Map = std::unordered_map<size_t,details::convert_t>;
  
    // Forward declaration
    template <typename S, typename R> struct HelperBase;
    template <typename S, typename R> struct Helper;
  
    // Member
    Map Table;
  
    // Method
    Converter();
    ~Converter() = default;
    Converter(const Converter& ) = delete;
    Converter(Converter&& ) = delete;
    Converter& operator= (const Converter& ) = delete;
    Converter& operator= (Converter&& ) = delete;
  
    template <typename S,typename R>
    static inline std::pair<size_t,details::convert_t> pair() noexcept
    {
      return {details::hash(static_cast<size_t>(static_typeid<S>()),static_cast<size_t>(static_typeid<R>())),&HelperBase<S,R>::convert};
    };
  };

  // ----------------------------------------------------------------------- //

  /**
   * Base for all the conversions furnished by the user (using the Any::Register structure)
   */
  template <typename S, typename R>
  struct Any::Converter::HelperBase
  {
    static inline void convert(void* source, void* result) {*static_cast<R*>(result) = Helper<S,R>::convert(*static_cast<S*>(source));};
  };

  /**
   * Generic converter which does nothing except returning a default value of the type @a R.
   * All custom conversions must specializase this structure.
   */
  template <typename S, typename R>
  struct Any::Converter::Helper : Any::Converter::HelperBase<S,R>
  {
    static inline R convert(const S& /* val */) {return R();}
  };
};

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

namespace btk
{
  template <typename Type, typename... To, typename... From>
  struct Any::Register<Type, Any::Conversion<To...>, Any::Conversion<From...>>
  {
    Register()
    {
      for(auto&& reg : Converter::Map{Converter::pair<Type,To>()...})
        Any::details::converter().Table.emplace(std::forward<Converter::Map::value_type>(reg));
      for(auto&& reg : Converter::Map{Converter::pair<From,Type>()...})
        Any::details::converter().Table.emplace(std::forward<Converter::Map::value_type>(reg));
    };
    ~Register() = default;
    Register(const Register& ) = delete;
    Register(Register&& ) = delete;
    Register& operator=(const Register& ) = delete;
    Register& operator=(Register&& ) = delete;
  };
  
  template <typename Type>
  struct Any::Unregister
  {
    Unregister()
    {
      Converter::Map& table = Any::details::converter().Table;
      auto it = table.begin();
      while (it != table.end())
      {
        // 1. Remove the conversion between the given user type and the other registered types
        // 2. Remove the conversion between the other registered types and the given user type
        constexpr size_t shift = 4*sizeof(size_t);
        const size_t sid = static_cast<size_t>(static_typeid<Type>()) << shift;
        const size_t rid = (static_cast<size_t>(static_typeid<Type>()) << shift) >> shift;
        if (((it->first & sid) == sid) || ((it->first & rid) ==rid))
          it = table.erase(it);
        else
          ++it;
      }
    };
    ~Unregister() = default;
    Unregister(const Unregister& ) = delete;
    Unregister(Unregister&& ) = delete;
    Unregister& operator=(const Unregister& ) = delete;
    Unregister& operator=(Unregister&& ) = delete;
  };
  
  template <typename U, typename D, typename >
  inline Any::Any(U&& value, D&& dimensions)
  : mp_Storage(details::store<U>(std::forward<U>(value), std::forward<D>(dimensions)))
  {};
  
  template <typename U, typename D, typename >
  inline Any::Any(std::initializer_list<U> values, std::initializer_list<D> dimensions)
  : mp_Storage(details::store<std::initializer_list<U>,std::initializer_list<D>>(std::move(values), std::move(dimensions)))
  {};
  
  template <typename U>
  inline bool Any::isEqual(U&& value) const noexcept
  {
    return btk::Any::details::is_equal(this,std::forward<U>(value));
  };

  template <typename U, typename >
  inline U Any::cast() const noexcept
  {
    static_assert(std::is_default_constructible<U>::value,"It is not possible to cast an Any object to a type which does not a default constructor.");
    U value = U();
    if (this->mp_Storage != nullptr)
      this->mp_Storage->cast<U>(&value);
    return value;
  };
  
  template <typename U, typename >
  inline U Any::cast(size_t idx) const noexcept
  {
    static_assert(std::is_default_constructible<U>::value,"It is not possible to cast an Any object to a type which does not a default constructor.");
    static_assert(!is_stl_vector<typename std::decay<U>::type>::value,"The cast(idx) method does not accept std::vector as casted type.");
    U value = U();
    if ((this->mp_Storage != nullptr) && (idx < this->mp_Storage->size()))
      this->mp_Storage->cast<U>(&value,idx);
    return value;
  };
  
  template <class U>
  inline Any::operator U() const noexcept
  {
    return this->cast<U>();
  };
};

#endif // __btkAny_tpp
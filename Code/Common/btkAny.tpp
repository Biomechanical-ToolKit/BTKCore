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

namespace btk
{
  template <typename Type, typename... To, typename... From>
  struct Any::Register<Type, Any::Conversion<To...>, Any::Conversion<From...>>
  {
    Register()
    {
      for(auto&& reg : Converter::Map{Converter::pair<Type,To>()...})
        Any::converter().Table.emplace(std::forward<Converter::Map::value_type>(reg));
      for(auto&& reg : Converter::Map{Converter::pair<From,Type>()...})
        Any::converter().Table.emplace(std::forward<Converter::Map::value_type>(reg));
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
      Converter::Map& table = Any::converter().Table;
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
  
  template <typename U>
  Any::Any(const U& value)
  : mp_Storage(new Storage<U>(value))
  {};
  
  /**
   * Cast for arithmetic types
   */
  template <typename U, typename , typename >
  inline U Any::cast() const noexcept
  {
    static_assert(std::is_default_constructible<U>::value,"It is not possible to cast an Any object to a type which does not a default constructor.");
    U value = U();
    if (this->mp_Storage != nullptr)
    {
      if (this->mp_Storage->id() == static_cast<size_t>(static_typeid<U>()))
      {
        value = *static_cast<U*>(this->mp_Storage->Data);
      }
      else if (std::is_arithmetic<U>::value && this->mp_Storage->is_arithmetic())
      {
        const size_t id = this->mp_Storage->id();
        // bool
        if (id == static_cast<size_t>(static_typeid<bool>()))
          value = static_cast<U>(*static_cast<bool*>(this->mp_Storage->Data));
        // char
        else if (id == static_cast<size_t>(static_typeid<char>()))
          value = static_cast<U>(*static_cast<char*>(this->mp_Storage->Data));
        // char16_t
        else if (id == static_cast<size_t>(static_typeid<char16_t>()))
          value = static_cast<U>(*static_cast<char16_t*>(this->mp_Storage->Data));
        // char32_t
        else if (id == static_cast<size_t>(static_typeid<char32_t>()))
          value = static_cast<U>(*static_cast<char32_t*>(this->mp_Storage->Data));
        // wchar_t
        else if (id == static_cast<size_t>(static_typeid<wchar_t>()))
          value = static_cast<U>(*static_cast<wchar_t*>(this->mp_Storage->Data));
        // signed char
        else if (id == static_cast<size_t>(static_typeid<signed char>()))
          value = static_cast<U>(*static_cast<signed char*>(this->mp_Storage->Data));
        // short int
        else if (id == static_cast<size_t>(static_typeid<short int>()))
          value = static_cast<U>(*static_cast<short int*>(this->mp_Storage->Data));
        // int
        else if (id == static_cast<size_t>(static_typeid<int>()))
          value = static_cast<U>(*static_cast<int*>(this->mp_Storage->Data));
        // long int
        else if (id == static_cast<size_t>(static_typeid<long int>()))
          value = static_cast<U>(*static_cast<long int*>(this->mp_Storage->Data));
        // long long int
        else if (id == static_cast<size_t>(static_typeid<long long int>()))
          value = static_cast<U>(*static_cast<long long int*>(this->mp_Storage->Data));
        // unsigned char
        else if (id == static_cast<size_t>(static_typeid<unsigned char>()))
          value = static_cast<U>(*static_cast<unsigned char*>(this->mp_Storage->Data));
        // unsigned short int
        else if (id == static_cast<size_t>(static_typeid<unsigned short int>()))
          value = static_cast<U>(*static_cast<unsigned short int*>(this->mp_Storage->Data));
        // unsigned int
        else if (id == static_cast<size_t>(static_typeid<unsigned int>()))
          value = static_cast<U>(*static_cast<unsigned int*>(this->mp_Storage->Data));
        // unsigned long int
        else if (id == static_cast<size_t>(static_typeid<unsigned long int>()))
          value = static_cast<U>(*static_cast<unsigned long int*>(this->mp_Storage->Data));
        // unsigned long long int
        else if (id == static_cast<size_t>(static_typeid<unsigned long long int>()))
          value = static_cast<U>(*static_cast<unsigned long long int*>(this->mp_Storage->Data));
        // float
        else if (id == static_cast<size_t>(static_typeid<float>()))
          value = static_cast<U>(*static_cast<float*>(this->mp_Storage->Data));
        // double
        else if (id == static_cast<size_t>(static_typeid<double>()))
          value = static_cast<U>(*static_cast<double*>(this->mp_Storage->Data));
        // long double
        else if (id == static_cast<size_t>(static_typeid<long double>()))
          value = static_cast<U>(*static_cast<long double*>(this->mp_Storage->Data));
        // ERROR - Should not be possible! All the standard arithmetic type in C++11 are listed above
        else
          throw(LogicError("Unexpected error during arithmetic type conversion!"));
      }
      else
      {
        auto convert = Any::extractConvertoid(this->mp_Storage->id(),static_cast<size_t>(static_typeid<U>()));
        if (convert != nullptr)
          convert(this->mp_Storage->Data,&value);
      }
    }
    return value;
  };
  
  /**
   * Cast for all the other types
   */
  template <typename U, typename >
  inline U Any::cast() const noexcept
  {
    static_assert(std::is_default_constructible<U>::value,"It is not possible to cast an Any object to a type which does not a default constructor.");
    U value = U();
    if (this->mp_Storage != nullptr)
    {
      if (this->mp_Storage->id() == static_cast<size_t>(static_typeid<U>()))
      {
        value = *static_cast<U*>(this->mp_Storage->Data);
      }
      else
      {  
        auto convert = Any::extractConvertoid(this->mp_Storage->id(),static_cast<size_t>(static_typeid<U>()));
        if (convert != nullptr)
          convert(this->mp_Storage->Data,&value);
      }
    }
    return value;
  };
  
  template <class U>
  inline Any::operator U() const noexcept
  {
    return this->cast<U>();
  };
    
  template <typename U>
  inline Any& Any::operator=(const U& other)
  {
    delete this->mp_Storage;
    this->mp_Storage = new Storage<U>(other);
    return *this;
  };
};

#endif // __btkAny_tpp

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
  
  template <typename U, typename>
  inline Any::Any(U&& value)
  : mp_Storage(new StorageSingle<typename std::remove_cv<typename std::remove_reference<U>::type>::type>(std::forward<U>(value)))
  {};

  template <typename U, typename>
  inline Any::Any(const std::vector<U>& values, const std::vector<size_t>& dimensions)
  : mp_Storage(new StorageArray<typename std::remove_cv<typename std::remove_reference<U>::type>::type>(values, dimensions.empty() ? std::vector<size_t>{values.size()} : dimensions))
  {};

  template <typename U, typename>
  inline Any::Any(std::initializer_list<U> values, std::initializer_list<size_t> dimensions)
  : mp_Storage(new StorageArray<typename std::remove_cv<typename std::remove_reference<U>::type>::type>(values, dimensions.size() == 0 ? std::initializer_list<size_t>{values.size()} : dimensions))
  {};

  template <typename U, typename >
  inline U Any::cast() const noexcept
  {
    static_assert(std::is_default_constructible<U>::value,"It is not possible to cast an Any object to a type which does not a default constructor.");
    U value = U();
    if (this->mp_Storage != nullptr)
    {
      if (this->mp_Storage->id() == static_typeid<U>())
        value = *static_cast<U*>(this->mp_Storage->Data);
      else if (!Cast::single(&value, this->mp_Storage))
        this->convert(&value);
    }
    return value;
  };
  
  template <class U>
  inline Any::operator U() const noexcept
  {
    return this->cast<U>();
  };
  
  template <typename U>
  inline void Any::convert(U* value) const noexcept
  {
    auto convert = Any::extractConvertoid(this->mp_Storage->id(),static_typeid<U>());
    if (convert != nullptr)
      convert(this->mp_Storage->Data,value);
  };
};

#endif // __btkAny_tpp

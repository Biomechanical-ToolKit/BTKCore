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

#ifndef __btkAny_converter_tpp
#define __btkAny_converter_tpp

#include "btkAny.h"

#include <unordered_map>

namespace btk 
{
  struct Any::Converter
  {
    // Typedef
    typedef std::unordered_map<size_t,Convertoid> Map;
    
    // Forward declaration
    template <typename S, typename R> struct HelperBase;
    template <typename S, typename R> struct Helper;
    template <typename S,typename... Rs> struct Register;
    
    // Member
    Map Table;
    
    // Method
    Converter();
    ~Converter() = default;
    template <typename T> static T& merge(T&& registered);
    template <typename T, typename... Ts> static T& merge(T&& registered, Ts&&... others);
    Converter(const Converter& ) = delete;
    Converter(Converter&& ) = delete;
    Converter& operator= (const Converter& ) = delete;
    Converter& operator= (Converter&& ) = delete;
    
    // Note: Clang (Apple LLVM version 4.2) does not like to define a static constexpr (template) method outside of the classe declaration, the next method are directly defined.
    template <typename S,typename R>
    static inline constexpr Convertoid mapped() noexcept
    {
      return &HelperBase<S,R>::convert;
    };
    static inline constexpr size_t hash(size_t sid, size_t rid) noexcept
    {
      return ((sid << (4*sizeof(size_t))) | ((rid << (4*sizeof(size_t))) >> (4*sizeof(size_t))));
    };
    
    template <typename S,typename R>
    static inline size_t key() noexcept
    {
      return hash(static_cast<size_t>(static_typeid<S>()),static_cast<size_t>(static_typeid<R>()));
    };
    
    template <typename S,typename R>
    static inline std::pair<size_t,Convertoid> pair() noexcept
    {
      return {key<S,R>(),mapped<S,R>()};
    };
  };
  
  template <typename S,typename... Rs>
  struct Any::Converter::Register
  {
    Map Table;
    inline constexpr Register() noexcept
    : Table{pair<S,Rs>()...}
    {};
    ~Register() = default;
    Register(const Register& ) = delete;
    Register(Register&& ) = default;
    Register& operator= (const Register& ) = delete;
    Register& operator= (Register&& ) = delete;
  };
  
  template <typename T>
  inline T& Any::Converter::merge(T&& registered)
  {
    return registered;
  };
  
  template <typename T, typename... Ts>
  inline T& Any::Converter::merge(T&& registered, Ts&&... others)
  {
    for(auto& reg : merge(others...).Table)
      registered.Table.emplace(std::forward<Map::value_type>(reg));
    return registered;
  };
};

#include "btkAny_converter_helper.tpp"

#endif // __btkAny_converter_tpp
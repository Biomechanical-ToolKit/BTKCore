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

#ifndef __btkAny_cast_tpp
#define __btkAny_cast_tpp

#include "btkException.h"

#include <string>

#include <cstdlib> // strtol, strtoll, strtoul, strtoull, strtof, strtod, ...

namespace btk
{
  struct Any::Cast
  {
    // Default
    template <typename U>
    static typename std::enable_if<
          !std::is_same<Any, typename std::decay<U>::type>::value
        && !std::is_arithmetic<typename std::decay<U>::type>::value
        && !std::is_same<std::string, typename std::decay<U>::type>::value  
      , bool>::type single(U* value, StorageBase* storage) noexcept
    {
      return false;
    };
    
    // Arithmetic conversion
    template <typename U>
    static typename std::enable_if<std::is_arithmetic<typename std::decay<U>::type>::value, bool>::type single(U* value, StorageBase* storage) noexcept
    {
      const typeid_t id = storage->id();
      if (std::is_arithmetic<U>::value && storage->is_arithmetic())
      {
        // bool
        if (id == static_typeid<bool>())
          *value = static_cast<U>(*static_cast<bool*>(storage->Data));
        // char
        else if (id == static_typeid<char>())
          *value = static_cast<U>(*static_cast<char*>(storage->Data));
        // char16_t
        else if (id == static_typeid<char16_t>())
          *value = static_cast<U>(*static_cast<char16_t*>(storage->Data));
        // char32_t
        else if (id == static_typeid<char32_t>())
          *value = static_cast<U>(*static_cast<char32_t*>(storage->Data));
        // wchar_t
        else if (id == static_typeid<wchar_t>())
          *value = static_cast<U>(*static_cast<wchar_t*>(storage->Data));
        // signed char
        else if (id == static_typeid<signed char>())
          *value = static_cast<U>(*static_cast<signed char*>(storage->Data));
        // short int
        else if (id == static_typeid<short int>())
          *value = static_cast<U>(*static_cast<short int*>(storage->Data));
        // int
        else if (id == static_typeid<int>())
          *value = static_cast<U>(*static_cast<int*>(storage->Data));
        // long int
        else if (id == static_typeid<long int>())
          *value = static_cast<U>(*static_cast<long int*>(storage->Data));
        // long long int
        else if (id == static_typeid<long long int>())
          *value = static_cast<U>(*static_cast<long long int*>(storage->Data));
        // unsigned char
        else if (id == static_typeid<unsigned char>())
          *value = static_cast<U>(*static_cast<unsigned char*>(storage->Data));
        // unsigned short int
        else if (id == static_typeid<unsigned short int>())
          *value = static_cast<U>(*static_cast<unsigned short int*>(storage->Data));
        // unsigned int
        else if (id == static_typeid<unsigned int>())
          *value = static_cast<U>(*static_cast<unsigned int*>(storage->Data));
        // unsigned long int
        else if (id == static_typeid<unsigned long int>())
          *value = static_cast<U>(*static_cast<unsigned long int*>(storage->Data));
        // unsigned long long int
        else if (id == static_typeid<unsigned long long int>())
          *value = static_cast<U>(*static_cast<unsigned long long int*>(storage->Data));
        // float
        else if (id == static_typeid<float>())
          *value = static_cast<U>(*static_cast<float*>(storage->Data));
        // double
        else if (id == static_typeid<double>())
          *value = static_cast<U>(*static_cast<double*>(storage->Data));
        // long double
        else if (id == static_typeid<long double>())
          *value = static_cast<U>(*static_cast<long double*>(storage->Data));
        // ERROR - Should not be possible! All the standard arithmetic type in C++11 are listed above
        else
          throw(LogicError("Unexpected error during arithmetic to arithmetic conversion!"));
        return true;
      }
      else if (std::is_arithmetic<U>::value && (static_typeid<U>() != static_typeid<bool>()) && (id == static_typeid<std::string>()))
      {
        if (std::is_integral<U>::value)
        {
          if (std::is_signed<U>::value)
          {
            if (sizeof(U) > sizeof(long))
              *value = strtoll(static_cast<std::string*>(storage->Data)->c_str(),nullptr,0);
            else
              *value = strtol(static_cast<std::string*>(storage->Data)->c_str(),nullptr,0);
          }
          else
          {
            if (sizeof(U) > sizeof(long))
              *value = strtoull(static_cast<std::string*>(storage->Data)->c_str(),nullptr,0);
            else
              *value = strtol(static_cast<std::string*>(storage->Data)->c_str(),nullptr,0);
          }
        }
        else
        {
          if (static_typeid<U>() == static_typeid<float>())
            *value = strtof(static_cast<std::string*>(storage->Data)->c_str(),nullptr);
          else if (static_typeid<U>() != static_typeid<double>())
            *value = strtod(static_cast<std::string*>(storage->Data)->c_str(),nullptr);
          else // long double
            *value = strtold(static_cast<std::string*>(storage->Data)->c_str(),nullptr);
        }
        return true;
      }
      return false;
    };
    
    // String conversion
    template <typename U>
    static typename std::enable_if<std::is_same<std::string, typename std::decay<U>::type>::value, bool>::type single(U* value, StorageBase* storage) noexcept
    {
      const typeid_t id = storage->id();
      if (storage->is_arithmetic()
               && (id != static_typeid<bool>())
               && (id != static_typeid<char16_t>())
               && (id != static_typeid<char32_t>())
               && (id != static_typeid<wchar_t>()))
      {
        // char (convert as it is an int8_t)
        if (id == static_typeid<char>())
          *value = std::to_string((short int)*static_cast<char*>(storage->Data));
        // signed char (convert as it is a signed int8_t)
        else if (id == static_typeid<signed char>())
          *value = std::to_string((signed short int)*static_cast<signed char*>(storage->Data));
        // short int
        else if (id == static_typeid<short int>())
          *value = std::to_string(*static_cast<short int*>(storage->Data));
        // int
        else if (id == static_typeid<int>())
          *value = std::to_string(*static_cast<int*>(storage->Data));
        // long int
        else if (id == static_typeid<long int>())
          *value = std::to_string(*static_cast<long int*>(storage->Data));
        // long long int
        else if (id == static_typeid<long long int>())
          *value = std::to_string(*static_cast<long long int*>(storage->Data));
        // unsigned char (convert as it is a unsigned int8_t)
        else if (id == static_typeid<unsigned char>())
          *value = std::to_string((unsigned short int)*static_cast<unsigned char*>(storage->Data));
        // unsigned short int
        else if (id == static_typeid<unsigned short int>())
          *value = std::to_string(*static_cast<unsigned short int*>(storage->Data));
        // unsigned int
        else if (id == static_typeid<unsigned int>())
          *value = std::to_string(*static_cast<unsigned int*>(storage->Data));
        // unsigned long int
        else if (id == static_typeid<unsigned long int>())
          *value = std::to_string(*static_cast<unsigned long int*>(storage->Data));
        // unsigned long long int
        else if (id == static_typeid<unsigned long long int>())
          *value = std::to_string(*static_cast<unsigned long long int*>(storage->Data));
        // float
        else if (id == static_typeid<float>())
          *value = std::to_string(*static_cast<float*>(storage->Data));
        // double
        else if (id == static_typeid<double>())
          *value = std::to_string(*static_cast<double*>(storage->Data));
        // long double
        else if (id == static_typeid<long double>())
          *value = std::to_string(*static_cast<long double*>(storage->Data));
        // ERROR - Should not be possible! All the standard arithmetic type in C++11 are listed above
        else
          throw(LogicError("Unexpected error during arithmetic to string conversion!"));
        return true;
      }
      return false;
    };
  };
};

#endif // __btkAny_cast_tpp
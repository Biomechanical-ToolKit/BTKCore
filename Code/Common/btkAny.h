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

#ifndef __btkAny_h
#define __btkAny_h

#include "btkCommonExport.h"

#include <type_traits>

namespace btk
{  
  class BTK_COMMON_EXPORT Any
  {
  public:
    
    // Forward declaration
    template <typename... T> struct Conversion;
    template <typename Type, typename From, typename To> struct Register;
    template <typename Type, typename... From, typename... To> struct Register<Type, Conversion<From...>, Conversion<To...>>;
    template <typename Type> struct Unregister;
    
    Any() noexcept;
    Any(const Any& other);
    Any(Any&& other) noexcept;
    template <typename U> Any(const U& value);
    
    ~Any();
    
    bool isValid() const noexcept;
    void swap(Any& other) noexcept;
    
    template <typename U, typename = typename std::enable_if<!std::is_same<Any, typename std::decay<U>::type>::value>::type> U cast() const noexcept;
    
    template<class U> operator U() const noexcept;
    
    Any& operator=(const Any& other);
    Any& operator=(Any&& other) noexcept;
    template<typename U> Any& operator=(const U& other);
    
    friend bool operator==(const Any& lhs, const Any& rhs) noexcept;
    friend bool operator!=(const Any& lhs, const Any& rhs) noexcept;
    
    // Note: The definition of the next friend (in)equal operators are in the declaration of the class because the default template parameters for a friend function can be specified only one time (see C++11 standard, paragraph 14.1.9)
    template <typename U, typename = typename std::enable_if<!std::is_same<Any, typename std::decay<U>::type>::value>::type> friend inline bool operator==(const Any& lhs, const U& rhs) noexcept {return (lhs.cast<U>() == rhs);};
    template <typename U, typename = typename std::enable_if<!std::is_same<Any, typename std::decay<U>::type>::value>::type> friend inline bool operator==(const U& lhs, const Any& rhs) noexcept {return (rhs == lhs);};
    template <typename U, typename = typename std::enable_if<!std::is_same<Any, typename std::decay<U>::type>::value>::type> friend inline bool operator!=(const Any& lhs, const U& rhs) noexcept {return !(lhs == rhs);};
    template <typename U, typename = typename std::enable_if<!std::is_same<Any, typename std::decay<U>::type>::value>::type> friend inline bool operator!=(const U& lhs, const Any& rhs) noexcept {return !(lhs == rhs);};
    
  private:
    // Forward declaration
    struct TraitsBase;
    struct StorageBase;    
    struct Converter;
    template <typename T> struct Traits;
    template <typename T> struct Storage;
    
    typedef void(*Convertoid)(void*,void*);
    static Converter& converter() noexcept;
    static Convertoid extractConvertoid(short sid, short rid) noexcept;
    StorageBase* mp_Storage;
  };
};

#include "btkAny_traits.tpp"
#include "btkAny_storage.tpp"
#include "btkAny_converter.tpp"
#include "btkAny.tpp"

#endif // __btkAny_h

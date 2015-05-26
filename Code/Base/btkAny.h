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

#include "btkBaseExport.h"
#include "btkMacros.h" // _BTK_NOEXCEPT

#include <type_traits>
#include <vector>
#include <initializer_list>
#include <cstddef> // size_t

namespace btk
{
  class BTK_BASE_EXPORT Any
  {
  public:
    
    // Forward declaration
    struct Storage;
    template <typename S, typename R> struct Helper;
    template <typename... T> struct Conversion;
    template <typename Type, typename From, typename To> struct Register;
    template <typename Type, typename... From, typename... To> struct Register<Type, Conversion<From...>, Conversion<To...>>;
    template <typename Type> struct Unregister;
    
    Any() _BTK_NOEXCEPT;
    Any(const Any& other);
    Any(Any&& other) _BTK_NOEXCEPT;
    template <typename U, typename D = void*, typename = typename std::enable_if<!std::is_same<Any, typename std::decay<U>::type>::value>::type> Any(U&& value, D&& dimensions = D{});
    template <typename U, typename = typename std::enable_if<!std::is_same<Any, typename std::decay<U>::type>::value>::type> Any(std::initializer_list<U> values, std::initializer_list<size_t> dimensions = {});
    ~Any();
    
    std::vector<size_t> dimensions() const _BTK_NOEXCEPT;
    size_t size() const _BTK_NOEXCEPT;
    
    bool isValid() const _BTK_NOEXCEPT;
    bool isEmpty() const _BTK_NOEXCEPT;
    void swap(Any& other) _BTK_NOEXCEPT;
    
    template <typename U, typename = typename std::enable_if<!std::is_same<Any, typename std::decay<U>::type>::value>::type> bool isEqual(U&& value) const _BTK_NOEXCEPT;
    template <typename U, typename = typename std::enable_if<!std::is_same<Any, typename std::decay<U>::type>::value>::type> void assign(U&& value) _BTK_NOEXCEPT;
    template <typename U, typename = typename std::enable_if<!std::is_same<Any, typename std::decay<U>::type>::value>::type> U cast() const _BTK_NOEXCEPT;
    template <typename U, typename = typename std::enable_if<!std::is_same<Any, typename std::decay<U>::type>::value>::type> U cast(size_t idx) const _BTK_NOEXCEPT;
    
    template<class U> operator U() const _BTK_NOEXCEPT;
    
    Any& operator=(const Any& other);
    Any& operator=(Any&& other) _BTK_NOEXCEPT;
    
    friend bool operator==(const Any& lhs, const Any& rhs) _BTK_NOEXCEPT;
    friend bool operator!=(const Any& lhs, const Any& rhs) _BTK_NOEXCEPT;
        
    template <typename U, typename A, typename = typename std::enable_if<std::is_same<Any, typename std::decay<A>::type>::value>::type> friend inline bool operator==(const A& lhs, const U& rhs) _BTK_NOEXCEPT {return lhs.isEqual(rhs);};
    template <typename U, typename A, typename = typename std::enable_if<std::is_same<Any, typename std::decay<A>::type>::value>::type> friend inline bool operator==(const U& lhs, const A& rhs) _BTK_NOEXCEPT {return rhs.isEqual(lhs);};
    template <typename U, typename A, typename = typename std::enable_if<std::is_same<Any, typename std::decay<A>::type>::value>::type> friend inline bool operator!=(const A& lhs, const U& rhs) _BTK_NOEXCEPT {return !lhs.isEqual(rhs);};
    template <typename U, typename A, typename = typename std::enable_if<std::is_same<Any, typename std::decay<A>::type>::value>::type> friend inline bool operator!=(const U& lhs, const A& rhs) _BTK_NOEXCEPT {return !rhs.isEqual(lhs);};
    
  private:    
    Storage* mp_Storage;
  };
};

#include "btkAny.tpp"

#endif // __btkAny_h

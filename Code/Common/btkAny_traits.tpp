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

#ifndef __btkAny_traits_tpp
#define __btkAny_traits_tpp

#include "btkConfigure.h" // HAVE_64_BIT

#include <cstdint>
#include <string>

namespace btk
{
  struct Any::TraitsBase
  {
    enum {
      None = 0,
      Bool,
      Int8,
      UInt8,
      Int16,
      UInt16,
      Int32,
      UInt32,
      Int64,
      UInt64,
      Float,
      Double,
      String,
      User = 1024,
    };
  };

  template<typename T>
  struct Any::Traits : Any::TraitsBase
  {
    enum {ID = Any::TraitsBase::None};
  };
  
  template<> struct Any::Traits<bool> : Any::TraitsBase
  {
    enum {ID = Any::TraitsBase::Bool};
  };
  
  template<> struct Any::Traits<int8_t> : Any::TraitsBase
  {
    enum {ID = Any::TraitsBase::Int8};
  };
  
  template<> struct Any::Traits<uint8_t> : Any::TraitsBase
  {
    enum {ID = Any::TraitsBase::UInt8};
  };
  
  template<> struct Any::Traits<int16_t> : Any::TraitsBase
  {
    enum {ID = Any::TraitsBase::Int16};
  };
  
  template<> struct Any::Traits<uint16_t> : Any::TraitsBase
  {
    enum {ID = Any::TraitsBase::UInt16};
  };
  
  template<> struct Any::Traits<int32_t> : Any::TraitsBase
  {
    enum {ID = Any::TraitsBase::Int32};
  };
  
  template<> struct Any::Traits<uint32_t> : Any::TraitsBase
  {
    enum {ID = Any::TraitsBase::UInt32};
  };
  
  template<> struct Any::Traits<int64_t> : Any::TraitsBase
  {
    enum {ID = Any::TraitsBase::Int64};
  };
  
  template<> struct Any::Traits<uint64_t> : Any::TraitsBase
  {
    enum {ID = Any::TraitsBase::UInt64};
  };
  
  template<> struct Any::Traits<float> : Any::TraitsBase
  {
    enum {ID = Any::TraitsBase::Float};
  };
  
  template<> struct Any::Traits<double> : Any::TraitsBase
  {
    enum {ID = Any::TraitsBase::Double};
  };
  
  template<> struct Any::Traits<std::string> : Any::TraitsBase
  {
    enum {ID = Any::TraitsBase::String};
  };
};

#endif // __btkAny_traits_tpp
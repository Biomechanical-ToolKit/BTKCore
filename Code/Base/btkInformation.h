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

#ifndef __btkInformation_h
#define __btkInformation_h

#include "btkObject.h"
#include "btkAny.h"
#include "btkMacros.h" // _BTK_NOEXCEPT, _BTK_CONSTEXPR_CONST

#define BTK_DECLARE_METAINFO(name, type) \
  struct name \
  { \
    static inline _BTK_CONSTEXPR_CONST char* key() _BTK_NOEXCEPT {return _BTK_STRINGIFY(name);}; \
    using ValueType = type; \
  }; \

namespace btk
{
  class InformationPrivate;
  
  class BTK_BASE_EXPORT Information
  {
    BTK_DECLARE_PIMPL_ACCESSOR(Information)
    
  public:
    Information();
    ~Information() _BTK_NOEXCEPT;
    
    Information(const Information& ) = delete;
    Information(Information&& ) _BTK_NOEXCEPT = delete;
    Information& operator=(const Information& ) = delete;
    Information& operator=(Information&& ) _BTK_NOEXCEPT = delete;
    
    template <typename U> typename U::ValueType value(typename U::ValueType&& defaultValue = typename U::ValueType()) const _BTK_NOEXCEPT;
    Any value(const char* key, const Any& defaultValue = Any()) const _BTK_NOEXCEPT;
    template <typename U> bool setValue(typename U::ValueType&& value);
    bool setValue(const char* key, const Any& value);
    bool add(const char* key, const Any& value);
    bool remove(const char* key) _BTK_NOEXCEPT;
    void clear() _BTK_NOEXCEPT;
    bool isEmpty() const _BTK_NOEXCEPT;
    
  private:
    std::unique_ptr<InformationPrivate> mp_Pimpl;
  };
  
  template <typename U>
  typename U::ValueType Information::value(typename U::ValueType&& defaultValue) const _BTK_NOEXCEPT
  {
    return this->value(U::key(),std::forward<typename U::ValueType>(defaultValue)).template cast<typename U::ValueType>();
  };

  template <typename U>
  bool Information::setValue(typename U::ValueType&& value)
  {
    return this->setValue(U::key(),std::forward<typename U::ValueType>(value));
  };
};

#endif // __btkInformation_h
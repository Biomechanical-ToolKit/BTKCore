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

#include "btkInformation.h"
#include "btkInformation_p.h"

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

namespace btk
{
  InformationPrivate::InformationPrivate()
  : Data()
  {};
    
  InformationPrivate::~InformationPrivate() = default;
    
};

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

namespace btk
{
  Information::Information()
  : mp_Pimpl(new InformationPrivate)
  {};
  
  Information::~Information() _BTK_NOEXCEPT = default;
  
  Any Information::value(const char* key, const Any& defaultValue) const _BTK_NOEXCEPT
  {
    auto optr = this->pimpl();
    auto it = optr->Data.find(key);
    if (it != optr->Data.end())
      return it->second;
    return defaultValue;
  };
  
  bool Information::setValue(const char* key, const Any& value)
  {
    return this->add(key,value);
  };
  
  bool Information::add(const char* key, const Any& value)
  {
    auto optr = this->pimpl();
    auto it = optr->Data.find(key);
    if (it != optr->Data.end() && (it->second == value))
      return false;
    optr->Data[key] = value;
    return true;
  };
  
  bool Information::remove(const char* key) _BTK_NOEXCEPT
  {
    auto optr = this->pimpl();
    return (optr->Data.erase(key) == size_t(1));
  };
  
  void Information::clear() _BTK_NOEXCEPT
  {
    auto optr = this->pimpl();
    optr->Data.clear();
  };
  
  bool Information::isEmpty() const _BTK_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Data.empty();
  }
};
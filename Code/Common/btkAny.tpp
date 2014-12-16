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

#ifndef __btkAny_tpp
#define __btkAny_tpp

namespace btk
{
  template <typename Type, typename... To, typename... From>
  struct Any::Register<Type, Any::Conversion<To...>, Any::Conversion<From...>>
  {
    Register()
    {
      static_assert(Traits<Type>::ID > TraitsBase::None, "It is not possible to register a type without an Any::Traits::ID. Specialize the struct Any::Traits and assign a unique value to Any::Traits::ID.");
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
      static_assert(Traits<Type>::ID > TraitsBase::User, "It is not possible to unregister a builtin type. In case this is a user type, verify the Any::Traits::ID value and be sure it is over Any::TraitsBase::User.");
      Converter::Map& table = Any::converter().Table;
      auto it = table.begin();
      while (it != table.end())
      {
        // 1. Remove the conversion between the given user type and the other registered types
        // 2. Remove the conversion between the other registered types and the given user type
        if ((((it->first & 0xFFFF0000) >> 16) == Traits<Type>::ID) || ((it->first & 0x0000FFFF) == Traits<Type>::ID))
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
  
  template <typename U,typename >
  inline U Any::cast() const noexcept
  {
    static_assert(std::is_default_constructible<U>::value,"It is not possible to cast an Any object to a type which does not a default constructor.");
    U value = U();
    if (this->mp_Storage != nullptr)
    {
      auto convert = Any::extractConvertoid(this->mp_Storage->id(),Traits<U>::ID);
      if (convert != nullptr)
        convert(this->mp_Storage->Data,&value);
    }
    return value;
  }
  
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

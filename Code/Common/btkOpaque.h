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

#ifndef __btkOpaque_h
#define __btkOpaque_h

// #include <functional> // std::bind
  
#define BTK_DECLARE_PIMPL(classname) \
  friend class classname##Private; \
  inline classname##Private* downcastOpaque() {return reinterpret_cast<classname##Private*>(this->mp_Opaque.get());}; \
  inline const classname##Private* downcastOpaque() const {return reinterpret_cast<const classname##Private*>(this->mp_Opaque.get());}; \
//   template <typename Member, typename T> \
//   inline classname##Private* downcastOpaque(Member&& m, const T& t) \
//   { \
//     auto optr = this->downcastOpaque(); \
//     if (t == std::bind(m,optr)()) \
//       return nullptr; \
//     optr = this->detach(); \
//     this->modified(); \
//     return optr; \
//   }; \
//   classname##Private* detach();
//
// #define BTK_DEFINE_PIMPL(classname) \
//   classname##Private* classname::detach() \
//   { \
//     auto optr = this->downcastOpaque(); \
//     if (!this->mp_Opaque.unique()) \
//     { \
//       auto ptr = reinterpret_cast<classname##Private*>(this->mp_Opaque->clone()); \
//       this->mp_Opaque.reset(ptr); \
//       optr = ptr; \
//     } \
//     return optr; \
//   };
    
#endif // __btkOpaque_h

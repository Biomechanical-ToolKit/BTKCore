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

#ifndef __btkSharedPtr_h
#define __btkSharedPtr_h

#include "btkConfigure.h"

/* Native header */
#if defined(__GNUC__) && (__GNUC__ > 3)
  #if defined(HAVE_SYS_TR1_MEMORY_H)
    #include <tr1/memory>
    #define _btk_shared_ptr_impl std::tr1::shared_ptr
  /* Experimental with GCC 4.3 */
  #elif defined(HAVE_SYS_MEMORY_H)
    #include <memory>
    #define _btk_shared_ptr_impl std::shared_ptr
  #endif
#elif defined(_MSC_VER) && (_MSC_VER >= 1500)
  /* included with MSVC 2008 SP1 */
  #if defined(HAVE_SYS_MEMORY_H)
    #include <memory>
    #define _btk_shared_ptr_impl std::tr1::shared_ptr
  #endif
#elif defined(HAVE_BOOST_MEMORY_HPP)
  #include <boost/memory.hpp>
  #define _btk_shared_ptr_impl boost::shared_ptr
#else
  // From boost 1.34
  #include <boost/tr1/memory.hpp>
  #define _btk_shared_ptr_impl std::tr1::shared_ptr
#endif

namespace btk
{
  template <class T>
  class SharedPtr
  {
  public:
    SharedPtr() : mp_Shared() {};
    template <class U> explicit SharedPtr(U* p) : mp_Shared(p) {};
    template <class U, class D> explicit SharedPtr(U* p, D d) : mp_Shared(p,d) {};
    template <class U, class D, class A> explicit SharedPtr(U* p, D d, A a) : mp_Shared(p,d,a) {};
    template <class U> SharedPtr(const SharedPtr<U>& src) : mp_Shared(src.mp_Shared) {};
    // ~SharedPtr(); // Implicit
    T& operator*() const {return *(this->mp_Shared);};
    T* operator->() const {return this->mp_Shared.operator->();};
    SharedPtr& operator=(const SharedPtr& src) {this->mp_Shared = src.mp_Shared; return *this;};
    template <class U> bool operator==(const SharedPtr<U>& rhs) const {return this->mp_Shared == rhs.mp_Shared;};
    template <class U> bool operator!=(const SharedPtr<U>& rhs) const {return this->mp_Shared != rhs.mp_Shared;};
    T* get() const {return this->mp_Shared.get();};
    void swap(SharedPtr<T>& other) {this->mp_Shared.swap(other);};
    bool unique() const {return this->mp_Shared.unique();};
    void reset() {this->mp_Shared.reset();};
    template <class U> void reset(U* p) {this->mp_Shared.reset(p);};
    template <class U, class D> void reset(U* p, D d) {this->mp_Shared.reset(p,d);};
    template <class U, class D, class A> void reset(U* p, D d, A a) {this->mp_Shared.reset(p,d,a);};
    // bool operator(s)
    typedef T* (SharedPtr<T>::*unspecified_bool_type)() const;
    operator unspecified_bool_type() const {return (this->mp_Shared.get() == 0) ? 0 : &SharedPtr<T>::get;};
    bool operator!() const {return this->mp_Shared.get() == 0;};
    
  private:
    template <class U> friend class SharedPtr;
    template <class U, class V> friend btk::SharedPtr<U> static_pointer_cast(const btk::SharedPtr<V>& r);
    
    template <class U> SharedPtr(const _btk_shared_ptr_impl<U>& src) : mp_Shared(src) {};
    
    _btk_shared_ptr_impl<T> mp_Shared;
  };
  
  template<class U, class V> inline SharedPtr<U> static_pointer_cast(const SharedPtr<V>& r)
  {
    return btk::SharedPtr<U>(static_pointer_cast<U>(r.mp_Shared));
  };
  
 /**
  * @class SharedPtr
  * @brief Smart pointer which manages the deletion of its content when all the copies are deleted.
  *
  * To cast a shared pointer you need to use the function static_pointer_cast.
  *
  * @note This class is merly an attempt to create an interface between all the (std|tr1|boost)::shared_ptr classes embedded or not with a Linux/MacOS/Windows compiler.
  *
  * @ingroup BTKCommon
  */ 
};

using btk::static_pointer_cast;

#endif // __btkSharedPtr_h
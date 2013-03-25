/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2013, Arnaud Barré
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

#include "btkObject.h"
#include "btkCriticalSection_p.h"

// OSAtomic.h optimizations only used in 10.5 and later
#if defined(__APPLE__)
  #include <AvailabilityMacros.h>
  #if MAC_OS_X_VERSION_MAX_ALLOWED >= 1050
    #include <libkern/OSAtomic.h>
  #endif
#endif

namespace btk
{
  /**
   * @class Object btkObject.h
   * @brief Base for all objects which need to keep track of modified time.
   *
   * @ingroup BTKCommon
   */
  /**
   * @var Object::m_Timestamp
   * Time in microseconds to indicate the last modification of the object.
   */
  
  /**
   * @typedef Object::Pointer
   * Smart pointer associated with a Object object.
   */
  
  /**
   * @typedef Object::ConstPointer
   * Smart pointer associated with a const Object object.
   */
  
  /**
   * @fn long Object::GetTimestamp() const
   * Returns the timestamp of the object.
   */
  
  /**
   * Sets the object as modified (its timestamp is updated and the flag 'modified' 
   * is set to true). It is important to use this method each time a component 
   * of the process is modified.
   */
  void Object::Modified()
  {
#if defined(WIN32) || defined(_WIN32)
    // Windows optimization
  #if defined(HAVE_64_BIT) 
    static LONGLONG _atomic_time = 0;
    this->m_Timestamp = (unsigned long)InterlockedIncrement64(&_atomic_time);
  #else
    static LONG _atomic_time = 0;
    this->m_Timestamp = (unsigned long)InterlockedIncrement(&_atomic_time);
  #endif
#elif defined(__APPLE__) && (MAC_OS_X_VERSION_MIN_REQUIRED >= 1050)
    // Mac optimization
  #if defined(HAVE_64_BIT) 
    // NOTE: Comment from VTK library
    // "m_Timestamp" is "unsigned long", a type that changes sizes
    // depending on architecture.  The atomic increment is safe, since it
    // operates on a variable of the exact type needed.  The cast does not
    // change the size, but does change signedness, which is not ideal.
    static volatile int64_t _atomic_time = 0;
    this->m_Timestamp = (unsigned long)OSAtomicIncrement64Barrier(&_atomic_time);
  #else
    static volatile int32_t _atomic_time = 0;
    this->m_Timestamp = (unsigned long)OSAtomicIncrement32Barrier(&_atomic_time);
  #endif
#elif defined(HAVE_ATOMIC_BUILTINS)
    // GCC and CLANG intrinsics
    static volatile unsigned long _atomic_time = 0;
    this->m_Timestamp = __sync_add_and_fetch(&_atomic_time, 1);
#else
    // General case
    static unsigned long _atomic_time = 0;
    static btk_critical_section_p _critical_section;
    _critical_section.Lock();
    this->m_Timestamp = ++_atomic_time;
    _critical_section.Unlock();
#endif
  };
  
  /**
   * @fn Object::Object()
   * Constructor.
   */

  /**
   * @fn Object::Object(const Object& toCopy)
   * Copy constructor.
   */
  
  /**
   * @fn Object::~Object()
   * Destructor.
   */
};

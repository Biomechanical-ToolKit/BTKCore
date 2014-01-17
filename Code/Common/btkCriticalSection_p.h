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

#ifndef __btkCriticalSection_p_h
#define __btkCriticalSection_p_h

#include "btkConfigure.h"

// Note: The declaration of this class is largely inspired by the class vtkCriticalSection

#if defined(HAVE_SPROC)
  #include <abi_mutex.h> // Needed for sproc implementation of mutex
  typedef abilock_t _;
#elif defined(HAVE_PTHREADS) || defined(HAVE_HP_PTHREADS)
  #include <pthread.h> // Needed for pthreads implementation of mutex
  typedef pthread_mutex_t btk_critical_section_t;
#elif defined(HAVE_WIN32_THREADS)
  #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
  #endif
  #include <windows.h>
  typedef CRITICAL_SECTION btk_critical_section_t;
#else
  typedef int btk_critical_section_t;
#endif

namespace btk
{
  class critical_section_p
  {
  public:
    BTK_COMMON_EXPORT critical_section_p();
    BTK_COMMON_EXPORT critical_section_p(int isLocked);
    BTK_COMMON_EXPORT ~critical_section_p();
    BTK_COMMON_EXPORT void Init();
    BTK_COMMON_EXPORT void Lock();
    BTK_COMMON_EXPORT void Unlock();

  protected:
    btk_critical_section_t m_CS;
  };
};

#endif // __btkCriticalSection_p_h
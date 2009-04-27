/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009, Arnaud Barré
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

#include <btkConfigure.h>

namespace btk // For documentation purpose
{
/**
 * @define SharedPtr 
 * Macro which determine the shared_ptr class to use in BTK.
 *
 * @ingroup BTKCommon
 */ 
};
// TODO: Need to add the experimental case with  GCC 4.3 (–std=c++0x) 
//       Use BTK_USE_GCC_EXPERIMENTAL for this
/* Native header */
#if defined(__GNUC__)
  #if defined(HAVE_TR1_MEMORY_H) && !defined(BTK_USE_GCC_EXPERIMENTAL)
 		#include <tr1/memory>
	  namespace btk { 
	    #define SharedPtr std::tr1::shared_ptr
		};
  /* Experimental with GCC 4.3 */
	#elif defined(HAVE_MEMORY_H)
	  #include <memory>
	  namespace btk { 
	    #define SharedPtr std::shared_ptr
		};
	#endif
#elif defined(_MSC_VER)
  /* included with MSVC 2008 SP1 */
	#if defined(HAVE_MEMORY_H)
  	#include <memory>
	  namespace btk { 
	    #define SharedPtr std::tr1::shared_ptr
		};
  #endif
#endif
/* Boost header */
#if !defined(HAVE_TR1_MEMORY_H) && !defined(HAVE_MEMORY_H)
  /* From Boost 1.34 */
  #if defined(HAVE_BOOST_TR1_MEMORY_HPP)
	  #include <boost/tr1/memory.hpp>
  	namespace btk { 
	    #define SharedPtr boost::tr1::shared_ptr
		};
	#elif defined(HAVE_BOOST_MEMORY_HPP)
	  #include <boost/memory.hpp>
	  namespace btk { 
	    #define SharedPtr boost::shared_ptr
		};
	#else
	  #error "No recognized library which embeds a shared_ptr class."
	#endif
#endif

#endif

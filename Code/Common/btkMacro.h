/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2012, Arnaud Barré
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

/**
 * btkMacro.h defines standard system-wide macros, constants, and other
 * parameters.
 */

#ifndef __btkMacro_h
#define __btkMacro_h

#include <iostream>
#include <cstring>

#if defined(_WIN32)
  #define btkStripPathMacro(f) \
    (strrchr(f, '\\') ? strrchr(f, '\\') + 1 : f)
#else
  #define btkStripPathMacro(f) \
    (strrchr(f, '/') ? strrchr(f, '/') + 1 : f)
#endif

#define _btkStringifyMacro(x) #x
#define btkStringifyMacro(x) _btkStringifyMacro(x)

/**
 * Macro to use when an input is not used in the function.
 * Usefull when the compiler gives you a warning to say that the variable is not used.
 */
#define btkNotUsed(x) (void)x;

/**
 * This macro is used to print error message with the following format:
 * <source> (<line>): <message>
 */
#define btkErrorMacro(m) \
  std::cerr << btkStripPathMacro(__FILE__) << "(" << btkStringifyMacro(__LINE__) << "): " << m << std::endl;

/**
 * This macro is used to print error message with the following format:
 * <source> (<line>): '<file>' <message>
 */
#define btkIOErrorMacro(f, m) \
  std::cerr << btkStripPathMacro(__FILE__) << "(" << btkStringifyMacro(__LINE__) << "): '" << btkStripPathMacro(f.c_str()) << "' - " << m << std::endl;

#endif // __btkMacro_h

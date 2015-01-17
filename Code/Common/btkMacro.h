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

/**
 * btkMacro.h defines standard system-wide macros, constants, and other
 * parameters.
 */

#ifndef __btkMacro_h
#define __btkMacro_h

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
 * This macro returns the number of digits in the given integer
 */
#define btkNumberOfDigits(num) \
  ((num==0)?1:(int)log10(std::fabs((float)num))+1)

/**
 * This function removes the character @a c at the beginning and the end of the given string
 * By default the character removed is the white space.
 * @warning The input is directly modified and returned.
 */
inline void btkTrimString(std::string* str, const char c = ' ')
{
  *str = str->erase(str->find_last_not_of(c) + 1);
  *str = str->erase(0, str->find_first_not_of(c));
};

/**
 * This function removes the character @a c at the beginning and the end of the given string
 * By default the character removed is the white space.
 */
inline std::string btkTrimString(const std::string& str, const char c = ' ')
{
  std::string str_ = str;
  btkTrimString(&str_, c);
  return str_;
};

#endif // __btkMacro_h

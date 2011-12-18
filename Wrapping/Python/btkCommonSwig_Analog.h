/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2011, Arnaud Barré
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

BTK_SWIG_DECLARE_CLASS(Analog)
{
public:
  enum {Unknown = btk::Analog::Unknown, PlusMinus10 = btk::Analog::PlusMinus10, PlusMinus5 = btk::Analog::PlusMinus5, PlusMinus2Dot5 = btk::Analog::PlusMinus2Dot5, PlusMinus1Dot25 = btk::Analog::PlusMinus1Dot25, PlusMinus1 = btk::Analog::PlusMinus1, PlusMinus0Dot5 = btk::Analog::PlusMinus0Dot5, PlusMinus0Dot25 = btk::Analog::PlusMinus0Dot25, PlusMinus0Dot1 = btk::Analog::PlusMinus0Dot1, PlusMinus0Dot05 = btk::Analog::PlusMinus0Dot05};
  btkAnalog(const std::string& label = "", const std::string& desc = "");
  btkAnalog(int frameNumber);
  btkAnalog(const std::string& label, int frameNumber);
  BTK_SWIG_DECLARE_CLONE(Analog);
  BTK_SWIG_DECLARE_POINTER_OPERATOR(Analog);
};

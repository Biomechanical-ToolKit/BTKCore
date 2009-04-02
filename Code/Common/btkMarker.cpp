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

#include "btkMarker.h"

namespace btk
{
  Marker::Marker(const std::string& label, int frameNumber)
  : Measure<3>(label, frameNumber),
    m_Residual(Residual(frameNumber,1)), m_Mask(Mask(frameNumber,1))
  {};
  
  void Marker::SetResidual(const Residual& r)
  {
    this->m_Residual = r;
    this->Modified();
  };
  
  void Marker::SetMask(const Mask& m)
  {
    this->m_Residual = m;
    this->Modified();
  };
  
  void Marker::SetFrameNumber(int frameNumber)
  {
    int actualFrameNumber = this->GetFrameNumber();
    if (frameNumber == actualFrameNumber)
      return;
    else if (frameNumber >= actualFrameNumber)
    {
      Values v(frameNumber, 3);
      v.block(0,0,actualFrameNumber,3) = this->m_Values;
      this->m_Values = v;
      
      Residual r(frameNumber, 1);
      r.block(0,0,actualFrameNumber,1) = this->m_Residual;
      this->m_Residual = r;
      
      Mask m(frameNumber, 1);
      m.block(0,0,actualFrameNumber,1) = this->m_Mask;
      this->m_Mask = m;
    }
    else
    {
      this->m_Values = this->m_Values.block(0,0,frameNumber,3);
      this->m_Residual = this->m_Residual.block(0,0,frameNumber,1);
      this->m_Mask = this->m_Mask.block(0,0,frameNumber,1);
    }
    this->Modified();
  };
}
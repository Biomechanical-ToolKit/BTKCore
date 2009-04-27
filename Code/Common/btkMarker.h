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

#ifndef __btkMarker_h
#define __btkMarker_h

#include "btkMeasure.h"

namespace btk
{
  class Marker : public Measure<3>
  {
  public:
    typedef Eigen::Matrix<double, Eigen::Dynamic, 1> Residual;
    typedef Eigen::Matrix<double, Eigen::Dynamic, 1> Mask;
    
    typedef SharedPtr<Marker> Pointer;
    typedef SharedPtr<const Marker> ConstPointer;
    
    static Pointer New(int frameNumber = 1) {return Pointer(new Marker("", frameNumber));};
    static Pointer New(const std::string& label, int frameNumber) {return Pointer(new Marker(label, frameNumber));};
    
    virtual ~Marker() {};
    
    Residual& GetResidual() {return this->m_Residual;};
    const Residual& GetResidual() const {return this->m_Residual;};
    BTK_COMMON_EXPORT void SetResidual(const Residual& r);
    Mask& GetMask() {return this->m_Mask;};
    const Mask& GetMask() const {return this->m_Mask;};
    BTK_COMMON_EXPORT void SetMask(const Mask& m);
    BTK_COMMON_EXPORT void SetFrameNumber(int frameNumber);
    
  protected:
    BTK_COMMON_EXPORT Marker(const std::string& label, int frameNumber);
    
  private:
    Marker(const Marker& ); // Not implemented.
    Marker& operator=(const Marker& ); // Not implemented.
    
    Residual m_Residual;
    Mask m_Mask;
  };
};

#endif // __btkMarker_h

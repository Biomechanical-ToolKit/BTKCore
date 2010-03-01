/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2010, Arnaud Barré
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

#ifndef __btkJoint_h
#define __btkJoint_h

#include "btkDataObject.h"
#include "btkSegment.h"

#include <string>

namespace btk
{
  class Model;
  
  class Joint : public DataObjectLabeled
  {
  public:
    //enum {Shoulder, Elbow, Wrist, Hip, Knee, Ankle, User = 65536} Type;
    
    typedef SharedPtr<Joint> Pointer;
    typedef SharedPtr<const Joint> ConstPointer;
    
    static Pointer New() {return Pointer(new Joint());};
    static Pointer New(Segment::Pointer proximal, Segment::Pointer distal, const std::string& label, const std::string& description) {return Pointer(new Joint(proximal, distal, label, description));};
    
    // ~Joint(); // Implicit.
    
    Segment::Pointer GetProximalSegment() {return this->m_Proximal;};
    Segment::ConstPointer GetProximalSegment() const {return this->m_Proximal;};
    BTK_COMMON_EXPORT void SetProximalSegment(Segment::Pointer seg);
    Segment::Pointer GetDistalSegment() {return this->m_Distal;};
    Segment::ConstPointer GetDistalSegment() const {return this->m_Distal;};
    BTK_COMMON_EXPORT void SetDistalSegment(Segment::Pointer seg);
    
    Pointer Clone() {return Pointer(new Joint(*this));};
  
  private:
    friend class Model;
    
    BTK_COMMON_EXPORT Joint();
    BTK_COMMON_EXPORT Joint(Segment::Pointer proximal, Segment::Pointer distal, const std::string& label, const std::string& description);
    BTK_COMMON_EXPORT Joint(const Joint& toCopy);
    Pointer& operator=(const Pointer& ); // Not implemented.
    
    Segment::Pointer m_Proximal;
    Segment::Pointer m_Distal;
  };
};

#endif // __btkJoint_h

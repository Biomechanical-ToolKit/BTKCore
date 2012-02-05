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

#ifndef __btkWrench_h
#define __btkWrench_h

#include "btkDataObject.h"
#include "btkPoint.h"

namespace btk
{
  class Wrench : public DataObject
  {
  public:    
    typedef SharedPtr<Wrench> Pointer;
    typedef SharedPtr<const Wrench> ConstPointer;
    
    static Pointer New(const std::string& label = "") {return Pointer(new Wrench(label));};
    static Pointer New(int frameNumber) {return Pointer(new Wrench("", frameNumber));};
    static Pointer New(const std::string& label, int frameNumber) {return Pointer(new Wrench(label, frameNumber));};
    
    // ~Wrench(); // Implicit.
    Point::Pointer GetPosition() const {return this->m_Position;};
    BTK_COMMON_EXPORT void SetPosition(Point::Pointer pos);
    Point::Pointer GetForce() const {return this->m_Force;};
    BTK_COMMON_EXPORT void SetForce(Point::Pointer force);
    Point::Pointer GetMoment() const {return this->m_Moment;};
    BTK_COMMON_EXPORT void SetMoment(Point::Pointer moment);
    
    BTK_COMMON_EXPORT Point::Pointer GetComponent(int idx);
    
    BTK_COMMON_EXPORT void SetFrameNumber(int frameNumber);
    
    Pointer Clone() const {return Pointer(new Wrench(*this));};
    
  protected:
    BTK_COMMON_EXPORT Wrench(const std::string& label);
    BTK_COMMON_EXPORT Wrench(const std::string& label, int frameNumber);
    
  private:
    BTK_COMMON_EXPORT Wrench(const Wrench& toCopy);
    Wrench& operator=(const Wrench& ); // Not implemented.

    Point::Pointer m_Position;
    Point::Pointer m_Force;
    Point::Pointer m_Moment;
  };
};

#endif // __btkWrench_h

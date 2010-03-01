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

#ifndef __btkSubject_h
#define __btkSubject_h

#include "btkDataObject.h"

#include <string>

namespace btk
{
  class Subject : public DataObject
  {
  public:
    typedef enum {Male, Female} Sex;
    
    typedef SharedPtr<Subject> Pointer;
    typedef SharedPtr<const Subject> ConstPointer;
    
    static Pointer New() {return Pointer(new Subject());};
    
    // ~Subject(); // Implicit
    
    BTK_COMMON_EXPORT void SetSex(Sex s);
    Sex GetSex() const {return this->m_Sex;};
    BTK_COMMON_EXPORT void SetHeight(double h);
    double GetHeight() const {return this->m_Height;};
    BTK_COMMON_EXPORT void SetWeight(double w);
    double GetWeight() const {return this->m_Weight;};
    
    Pointer Clone() const {return Pointer(new Subject(*this));};
    
  protected:
    BTK_COMMON_EXPORT Subject();
    
  private:
    BTK_COMMON_EXPORT Subject(const Subject& toCopy);
    Subject& operator=(const Subject& ); // Not implemented.
    
    Sex m_Sex;
    double m_Height;
    double m_Weight;
  };
};

#endif // __btkSubject_h

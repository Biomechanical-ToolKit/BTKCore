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

#ifndef __btkEvent_h
#define __btkEvent_h

#include "btkDataObject.h"

#include <string>

namespace btk
{
  class Event: public DataObject
  {
  public:
    typedef SharedPtr<Event> Pointer;
    typedef SharedPtr<const Event> ConstPointer;
    
    static Pointer New(const std::string& label = "", double t = 0.0, const std::string& context = "", const std::string& subject = "", const std::string& desc = "", int id = 0) {return Pointer(new Event(label, t, context, subject, desc, id));};

    // ~Event(); // Implicit.
    const std::string& GetLabel() const {return this->m_Label;};
    BTK_COMMON_EXPORT void SetLabel(const std::string& label);
    const std::string& GetDescription() const {return this->m_Description;};
    BTK_COMMON_EXPORT void SetDescription(const std::string& desc);
    const std::string& GetContext() const {return this->m_Context;};
    BTK_COMMON_EXPORT void SetContext(const std::string& context);
    const std::string& GetSubject() const {return this->m_Subject;};
    BTK_COMMON_EXPORT void SetSubject(const std::string& subject);
    double GetTime() const {return this->m_Time;};
    BTK_COMMON_EXPORT void SetTime(double t);
    int GetId() const {return this->m_Id;};
    BTK_COMMON_EXPORT void SetId(int id);
    Pointer Clone() const {return Pointer(new Event(*this));};

  private:
    BTK_COMMON_EXPORT Event(const std::string& label, double t, const std::string& context, const std::string& subject, const std::string& desc, int id);
    BTK_COMMON_EXPORT Event(const Event& toCopy);
    Event& operator=(const Event& ); // Not implemented.

    std::string m_Label;
    std::string m_Description;
    std::string m_Context;
    std::string m_Subject;
    double m_Time;
    int m_Id;
  };
};

#endif // __btkEvent_h

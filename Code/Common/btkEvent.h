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

#ifndef __btkEvent_h
#define __btkEvent_h

#include "btkDataObject.h"

#include <string>

namespace btk
{
  class Event: public DataObjectLabeled
  {
  public:
    typedef enum {Unknown = 0x00,
                  Manual = 0x02, 
                  Automatic = 0x04,
                  FromForcePlatform = 0x08} DetectionFlag;
    
    typedef SharedPtr<Event> Pointer;
    typedef SharedPtr<const Event> ConstPointer;
    
    static Pointer New() {return Pointer(new Event("", -1.0, -1, "", Unknown, "", "", 0));};
    static Pointer New(const std::string& label, double t, const std::string& context = "", int detectionFlags = Unknown, const std::string& subject = "", const std::string& desc = "", int id = 0) {return Pointer(new Event(label, t, -1, context, detectionFlags, subject, desc, id));};
    static Pointer New(const std::string& label, int f, const std::string& context = "", int detectionFlags = Unknown, const std::string& subject = "", const std::string& desc = "", int id = 0) {return Pointer(new Event(label, -1.0, f, context, detectionFlags, subject, desc, id));};
    static Pointer New(const std::string& label, double t, int f, const std::string& context = "", int detectionFlags = Unknown, const std::string& subject = "", const std::string& desc = "", int id = 0) {return Pointer(new Event(label, t, f, context, detectionFlags, subject, desc, id));};
    // ~Event(); // Implicit.
    const std::string& GetContext() const {return this->m_Context;};
    BTK_COMMON_EXPORT void SetContext(const std::string& context);
    const std::string& GetSubject() const {return this->m_Subject;};
    BTK_COMMON_EXPORT void SetSubject(const std::string& subject);
    double GetTime() const {return this->m_Time;};
    BTK_COMMON_EXPORT void SetTime(double t);
    int GetFrame() const {return this->m_Frame;};
    BTK_COMMON_EXPORT void SetFrame(int f);
    int GetDetectionFlags() const {return this->m_DetectionFlags;};
    BTK_COMMON_EXPORT void SetDetectionFlags(int flags);
    BTK_COMMON_EXPORT bool HasDetectionFlag(DetectionFlag flag);
    int GetId() const {return this->m_Id;};
    BTK_COMMON_EXPORT void SetId(int id);
    
    Pointer Clone() const {return Pointer(new Event(*this));};
    BTK_COMMON_EXPORT friend bool operator==(const Event& rLHS, const Event& rRHS);
    friend bool operator!=(const Event& rLHS, const Event& rRHS)
    {
      return !(rLHS == rRHS);
    };

  private:
    BTK_COMMON_EXPORT Event(const std::string& label, double t, int f, const std::string& context, int detectionFlags, const std::string& subject, const std::string& desc, int id);
    BTK_COMMON_EXPORT Event(const Event& toCopy);
    Event& operator=(const Event& ); // Not implemented.

    std::string m_Context;
    std::string m_Subject;
    int m_DetectionFlags;
    double m_Time;
    int m_Frame;
    int m_Id;
  };
};

#endif // __btkEvent_h

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

#include "btkEvent.h"

#include <limits>
#include <cmath>

namespace btk
{
  /**
   * @class Event 
   * @brief Label a specific time during an acquisition.
   *
   * @ingroup BTKCommon
   */

  /**
   * @typedef Event::Pointer
   * Smart pointer associated with an Event object.
   */
  
  /**
   * @typedef Event::ConstPointer
   * Smart pointer associated with a const Event object.
   */

  /**
   * @fn Pointer Event::New(const std::string& label = "", double t = 0.0, const std::string& context = "", const std::string& subject = "", const std::string& desc = "")
   * Creates a smart pointer associated with an Event object.
   */

  /**
   * @fn const std::string& Event::GetLabel()  const
   * Returns event's label.
   */
  
  /**
   * Sets event's label.
   */
  void Event::SetLabel(const std::string& label)
  {
    if (this->m_Label.compare(label) == 0)
      return;
    this->m_Label = label;
    this->Modified();
  };

  /**
   * @fn const std::string& Event::GetDescription() const
   * Returns event's description.
   */

  /**
   * Sets event's description.
   */
  void Event::SetDescription(const std::string& desc)
  {
    if (this->m_Description.compare(desc) == 0)
      return;
    this->m_Description = desc;
    this->Modified();
  };

  /**
   * @fn const std::string& Event::GetContext() const
   * Returns event's context.
   */
    
  /**
   * Sets event's context.
   */
  void Event::SetContext(const std::string& context)
  {
    if (this->m_Context.compare(context) == 0)
      return;
    this->m_Context = context;
    this->Modified();
  };

  /**
   * @fn const std::string& Event::GetSubject() const
   * Returns the subject associated with the event.
   */
    
  /**
   * Sets the subject associated with the event.
   */
  void Event::SetSubject(const std::string& subject)
  {
    if (this->m_Subject.compare(subject) == 0)
      return;
    this->m_Subject = subject;
    this->Modified();
  };

  /**
   * @fn double Event::GetTime() const
   * Returns event's time.
   */
  
  /**
   * Sets event's time.
   */
  void Event::SetTime(double t)
  {
    if (std::fabs(this->m_Time - t) < std::numeric_limits<double>::epsilon())
      return;
    this->m_Time = t;
    this->Modified();
  };

  /**
   * @fn Pointer Event::Clone() const
   * Clones the object and return it as new smart pointer.
   */
  
  /**
   * Constructor.
   */
  Event::Event(const std::string& label, double t, const std::string& context, const std::string& subject, const std::string& desc)
  : DataObject(), m_Label(label), m_Context(context), m_Subject(subject), m_Description(desc)
  {
    this->m_Time = t;
  };

  /**
   * Copy constructor. Timestamp, source and parent are reset.
   */
  Event::Event(const Event& toCopy)
  : DataObject(), 
    m_Label(toCopy.m_Label), m_Context(toCopy.m_Context), m_Subject(toCopy.m_Subject), m_Description(toCopy.m_Description)
  {
    this->m_Time = toCopy.m_Time;
  };
};

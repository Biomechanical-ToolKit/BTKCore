/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2014, Arnaud Barré
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
#include "btkEvent_p.h"

#include <limits>

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

namespace btk
{
  EventPrivate::EventPrivate(Event* pint, const std::string& name, double time, const std::string& context, const std::string& subject)
  : NodePrivate(pint,name), Time(time), Context(context), Subject(subject)
  {};
};

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

namespace btk
{
  /**
   * @class Event btkEvent.h
   * @brief Label a specific time.
   *
   * An event is defined by:
   *  - a name;
   *  - a time (in seconds);
   *  - a context (e.g. "Right" side, "Left" side, or "General" context);
   *  - a subject associated with this event
   *  - a description which could give more details about the event
   *
   * All these information can be accessed/modified by using dedicated accessor/mutator but also using the property() and setProperty() method.
   * The name of properties are (in the same order than the previous list):
   *  - name
   *  - time
   *  - context
   *  - subject
   *  - description
   *
   * @ingroup BTKCommon
   */
  
  /**
   * Constructor.
   */
  Event::Event(const std::string& name, double time, const std::string& context, const std::string& subject, Node* parent)
  : Node(*new EventPrivate(this,name,time,context,subject),parent)
  {};
  
  /**
   * Returns associated
   */
  double Event::time() const noexcept
  {
    auto optr = this->pimpl();
    return optr->Time;
  };
  
  void Event::setTime(double value) noexcept
  {
    auto optr = this->pimpl();
    if (std::fabs(value - optr->Time) < std::numeric_limits<double>::epsilon())
      return;
    optr->Time = value;
    this->modified();
  };
  
  const std::string& Event::context() const noexcept
  {
    auto optr = this->pimpl();
    return optr->Context;
  };
  
  void Event::setContext(const std::string& value) noexcept
  {
    auto optr = this->pimpl();
    if (value == optr->Context)
      return;
    optr->Context = value;
    this->modified();
  };
  
  const std::string& Event::subject() const noexcept
  {
    auto optr = this->pimpl();
    return optr->Subject;
  };
  
  void Event::setSubject(const std::string& value) noexcept
  {
    auto optr = this->pimpl();
    if (value == optr->Subject)
      return;
    optr->Subject = value;
    this->modified();
  };
};

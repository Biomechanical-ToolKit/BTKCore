/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2013, Arnaud Barré
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
   * @class Event btkEvent.h
   * @brief Label a specific time/frame during an acquisition.
   *
   * An event is defined as:
   *  - a label;
   *  - a time;
   *  - a frame;
   *  - a context;
   *  - a detection flag;
   *  - a description;
   *  - an ID.
   *
   * The ID is not used as a unique identifier but more to determine the type of the event.
   * Even if there is no rule for the id value, these 3 next values are used in Vicon Workstation and Nexus to distinguish events' type:
   *  - The value 0 is for general event;
   *  - Value 1 corresponds to the foot strike event;
   *  - Value 2 for the foot off. 
   * Without the use of these values, it is known that Workstation and Nexus put all events in general (default value is 0). For Polygon, one user reported that some points had their values set to 0.
   *
   * For example, a heel strike event on the left leg or the subject John Doe at frame 143 can be set by
   * @code
   * btk::Event::Pointer evt = btk::Event::New('FootStrike', 143, 'Left', btk.btkEvent.Manual, 'John Doe', 'Heel strike event', 2)";
   * @endcode
   *
   * @ingroup BTKCommon
   */
  
  /**
   * @enum Event::DetectionFlag
   * Enums used to determine the method(s) used to detect the event.
   */
  /**
   * @var Event::DetectionFlag Event::Unknown
   * Unknown method.
   */
  /**
   * @var Event::DetectionFlag Event::Manual
   * Manual method.
   */
  /**
   * @var Event::DetectionFlag Event::Automatic
   * Automatic method.
   */
  /**
   * @var Event::DetectionFlag Event::FromForcePlatform
   * Method based on force platform data.
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
   * @typedef Event::NullPointer
   * Special null pointer associated with a Event object.
   * This type should be used only internally to test the nullity of a smart pointer.
   * See the static method Null() instead.
   */
  
  /**
   * @fn static NullPointer Event::Null()
   * Static function to return a null pointer.
   * @note This static method should be used only to test if a shared ponter is null or not. 
   * It is advised to call the method without parenthesis as special (in)equality 
   * operator are implemented to use a function pointer. See the description of the class NullPtr 
   * for an example.
   */
  
  /**
   * @fn Pointer Event::New()
   * Creates a smart pointer associated with an Event object.
   */
  
  /**
   * @fn Pointer Event::New(const std::string& label, double t, const std::string& context, int detectionFlags = Unknown, const std::string& subject = "", const std::string& desc = "", int id = 0)
   * Creates a smart pointer associated with an Event object.
   */
  
  /**
   * @fn Pointer Event::New(const std::string& label, int f, const std::string& context, int detectionFlags = Unknown, const std::string& subject = "", const std::string& desc = "", int id = 0)
   * Creates a smart pointer associated with an Event object.
   */
  
  /**
   * @fn Pointer Event::New(const std::string& label, double t, int f, const std::string& context, int detectionFlags = Unknown, const std::string& subject = "", const std::string& desc = "", int id = 0)
   * Creates a smart pointer associated with an Event object.
   */

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
   * @fn int Event::GetFrame() const
   * Returns event's frame index.
   */
  
  /**
   * Sets event's frame index.
   */
  void Event::SetFrame(int f)
  {
    if (this->m_Frame == f)
      return;
    this->m_Frame = f;
    this->Modified();
  };
  
  /**
   * @fn int Event::GetDetectionFlags() const
   * Returns the method(s) which generate the event.
   */
  
  /**
   * Sets the event's detection.
   * Use the enumeration Event::DetectionFlag to set the method(s) of detection.
   */
  void Event::SetDetectionFlags(int flags)
  {
    if (this->m_DetectionFlags == flags)
      return;
    this->m_DetectionFlags = flags;
    this->Modified();
  };
  
  /**
   * Checks if the given detection flag is contained in the event.
   */
  bool Event::HasDetectionFlag(DetectionFlag flag)
  {
    if ((this->m_DetectionFlags & flag) == flag)
      return true;
    else
      return false;
  };
  
  /**
   * @fn int Event::GetId() const
   * Returns the event's ID. Can be used by software (as Vicon Workstation) 
   * to indicate event (independently of the label).
   */
  
  /**
   * Sets the event's ID. 
   */
  void Event::SetId(int id)
  {
    if (this->m_Id == id)
      return;
    this->m_Id = id;
    this->Modified();
  };

  /**
   * @fn Pointer Event::Clone() const
   * Clones the object and return it as new smart pointer.
   */
   
  /**
   * Equality operator.
   */
  bool operator==(const Event& rLHS, const Event& rRHS)
  {
    if (rLHS.m_Label != rRHS.m_Label) 
      return false;
    if (rLHS.m_Context != rRHS.m_Context) 
      return false;
    if (rLHS.m_Subject != rRHS.m_Subject) 
      return false;
    if (rLHS.m_Description != rRHS.m_Description) 
      return false;
    if (fabs(rLHS.m_Time - rRHS.m_Time) >= std::numeric_limits<double>::epsilon())
      return false;
    if (rLHS.m_Frame != rRHS.m_Frame)
      return false;
    if (rLHS.m_DetectionFlags != rRHS.m_DetectionFlags)
      return false;
    if (rLHS.m_Id != rRHS.m_Id) 
      return false;
    return true;
  };
  
  /**
   * @fn friend bool Event::operator!=(const Event& rLHS, const Event& rRHS)
   * Inequality operator.
   */
  
  /**
   * Constructor.
   */
  Event::Event(const std::string& label, double t, int f, const std::string& context, int detectionFlags, const std::string& subject, const std::string& desc, int id)
  : DataObjectLabeled(label, desc), m_Context(context), m_Subject(subject)
  {
    this->m_Time = t;
    this->m_Frame = f;
    this->m_DetectionFlags = detectionFlags;
    this->m_Id = id;
  };

  /**
   * Copy constructor. Timestamp, source and parent are reset.
   */
  Event::Event(const Event& toCopy)
  : DataObjectLabeled(toCopy), m_Context(toCopy.m_Context), m_Subject(toCopy.m_Subject)
  {
    this->m_Time = toCopy.m_Time;
    this->m_Frame = toCopy.m_Frame;
    this->m_DetectionFlags = toCopy.m_DetectionFlags;
    this->m_Id = toCopy.m_Id;
  };
};

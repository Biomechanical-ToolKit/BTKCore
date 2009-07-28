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

#include "btkWrench.h"

namespace btk
{
  /**
   * @class Wrench
   * @brief Represents a wrench has position, force and moment.
   *
   * @ingroup BTKCommon
   */

  /**
   * @typedef Wrench::Pointer
   * Smart pointer associated with a Wrench object.
   */

  /**
   * @typedef Wrench::ConstPointer
   * Smart pointer associated with a const Wrench object.
   */

  /**
   * @fn static Pointer Wrench::New(int frameNumber = 1)
   * @brief Creates a smart pointer associated with a Wrench object.
   */

  /**
   * @fn static Pointer Wrench::New(const std::string& label, int frameNumber)
   * @brief Creates a smart pointer associated with a Wrench object.
   */

  /**
   * @fn Point::Pointer Wrench::GetPosition() const
   * Returns the wrench's position.
   */

  /**
   * Sets the wrench's position.
   */
  void Wrench::SetPosition(Point::Pointer pos)
  {
    if (pos == this->m_Position)
      return;
    this->m_Position = pos;
    // force point's type
    this->m_Position->SetType(Point::Marker);
    this->Modified();
  };

  /**
   * @fn Point::Pointer Wrench::GetForce() const
   * Returns the wrench's force.
   */

  /**
   * Sets the wrench's force.
   */
  void Wrench::SetForce(Point::Pointer force)
  {
    if (force == this->m_Force)
      return;
    this->m_Force = force;
    // force point's type
    this->m_Position->SetType(Point::Force);
    this->Modified();
  };

  /**
   * @fn Point::Pointer Wrench::GetMoment() const
   * Returns the wrench's moment.
   */

  /**
   * Sets the wrench's moment.
   */
  void Wrench::SetMoment(Point::Pointer moment)
  {
    if (moment == this->m_Moment)
      return;
    this->m_Moment = moment;
    // force point's type
    this->m_Position->SetType(Point::Moment);
    this->Modified();
  };

  /**
   * Constructor.
   *
   *  The position's label corresponds to @a label. The force's label
   *  has the string ".F" added to @a label. The moments's label
   *  has the string ".M" added to @a label. 
   *
   * @warning The number of frames must be greater than 0.
   */
  Wrench::Wrench(const std::string& label, int frameNumber)
  {
    this->m_Position = Point::New(label, frameNumber);
    this->m_Force = Point::New(label + ".F", frameNumber, Point::Force);
    this->m_Moment = Point::New(label + ".M", frameNumber, Point::Moment);
  };

}


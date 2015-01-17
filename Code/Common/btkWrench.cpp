/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2015, Arnaud Barré
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
#include "btkException.h"

namespace btk
{
  /**
   * @class Wrench btkWrench.h
   * @brief Represents a wrench as position, force and moment varying during the time.
   *
   * This class is mainly used to have a compact way to store forces and moment acting (or reacting) at a given position.
   * The moment should be expressed at the position as it is defined in general mechanics.
   *
   * By default, the label used for the forces and the moments corresponds to the given label to the constructor and followed by the suffix ".F" and ".M" respectively.
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
   * @typedef Wrench::NullPointer
   * Special null pointer associated with a Wrench object.
   * This type should be used only internally to test the nullity of a smart pointer.
   * See the static method Null() instead.
   */
  
  /**
   * @fn static NullPointer Wrench::Null()
   * Static function to return a null pointer.
   * @note This static method should be used only to test if a shared ponter is null or not. 
   * It is advised to call the method without parenthesis as special (in)equality 
   * operator are implemented to use a function pointer. See the description of the class NullPtr 
   * for an example.
   */
  
  /**
   * @fn static Pointer Wrench::New(const std::string& label = "")
   * @brief Creates a smart pointer associated with a Wrench object.
   *
   * The Wrench created has no values.
   * @warning The call of this function must be followed by the use of the method Wrench::SetFrameNumber
   * as it creates a null matrix for the values.
   */

  /**
   * @fn static Pointer Wrench::New(int frameNumber)
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
    this->m_Force->SetType(Point::Force);
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
    this->m_Moment->SetType(Point::Moment);
    this->Modified();
  };
  
  /**
   * Sets the number of frames.
   * The input @a frameNumber must be greater than 0.
   */
  void Wrench::SetFrameNumber(int frameNumber)
  {
    if (frameNumber <= 0)
    {
      btkErrorMacro("Impossible to set a number of frames lower or equal to 0.");
      return;
    }
    this->m_Position->SetFrameNumber(frameNumber);
    this->m_Force->SetFrameNumber(frameNumber);
    this->m_Moment->SetFrameNumber(frameNumber);
  };
  
  /**
   * Returns the component with the given index @a idx.
   * The possible value for the index are:
   *   - 0: Returns the position.
   *   - 1: Returns the force.
   *   - 2: Returns the moment.
   * Any other value will trigger an exception.
   */
  Point::Pointer Wrench::GetComponent(int idx)
  {
    if ((idx < 0) || (idx > 2))
      throw(OutOfRangeException("Wrench::GetComponent"));
    switch(idx)
    {
    case 0:
      return this->m_Position;
    case 1:
      return this->m_Force;
    case 2:
      return this->m_Moment;
    default: // Impossible
      return Point::Pointer();
    }
  };
  
  /**
   * @fn Pointer Wrench::Clone() const
   * Returns a deep copy of the object as a smart pointer.
   */

  /**
   * Constructor.
   *
   *  The position's label corresponds to @a label. The force's label
   *  has the string ".F" added to @a label. The moments's label
   *  has the string ".M" added to @a label. 
   *
   * The Wrench created has no values.
   * @warning The call of this function must be followed by the use of the method Wrench::SetFrameNumber
   * as it creates a null matrix for the values.
   */
  Wrench::Wrench(const std::string& label)
  {
    this->m_Position = Point::New(label);
    this->m_Force = Point::New(label + ".F", Point::Force);
    this->m_Moment = Point::New(label + ".M", Point::Moment);
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
  
  Wrench::Wrench(const Wrench& toCopy)
  : DataObject()
  {
    this->m_Position = toCopy.m_Position->Clone();
    this->m_Force = toCopy.m_Force->Clone();
    this->m_Moment = toCopy.m_Moment->Clone();
  };

}


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

#include "btkAnalog.h"

namespace btk
{
  /**
   * @class Analog
   * @brief Analog channel data along the time.
   *
   * @ingroup BTKCommon
   */

  /**
   * @enum Analog::Gain
   * Enums used to specify the analog gain.
   */
  /**
   * @var Analog::Gain Analog::Unknown
   * Unknown gain.
   */
  /**
   * @var Analog::Gain Analog::PlusMinus10
   * +/- 10 volts.
   */
  /**
   * @var Analog::Gain Analog::PlusMinus5
   * +/- 5 volts.
   */
  /**
   * @var Analog::Gain Analog::PlusMinus2Dot5
   * +/- 2.5 volts.
   */
  /**
   * @var Analog::Gain Analog::PlusMinus1Dot25
   * +/- 1.25 volts.
   */

  /**
   * @typedef Analog::Pointer
   * Smart pointer associated with a Analog object.
   */

  /**
   * @typedef Analog::ConstPointer
   * Smart pointer associated with a const Analog object.
   */

  /**
   * @fn static Pointer Analog::New(int frameNumber = 1)
   * @brief Creates a smart pointer associated with a Analog object.
   *
   * The analog channel created has an empty label and a number of frame equals to @a framenumber.
   * @warning The number of frames must be greater than 0.
   */

  /**
   * @fn static Pointer Analog::New(const std::string& label, int frameNumber)
   * @brief Creates a smart pointer associated with a Analog object.
   *
   * The analog channel created has a label and a number of frame equals to @a label and @a framenumber respectively.
   * @warning The number of frames must be greater than 0.
   */

  /**
   * @fn virtual Analog::~Analog()
   * Empty destructor.
   */

  /**
   * @fn const std::string& Analog::GetUnit() const
   * Returns the unit of the channel number.
   */

  /**
   * Sets the analog channel's unit.
   */
  void Analog::SetUnit(const std::string& u)
  {
    if (this->m_Unit.compare(u) == 0)
      return;
    this->m_Unit = u;
    this->Modified();
  };
   
  /**
   * @fn Gain Analog::GetGain() const 
   * Returns the analog gain.
   */

  /**
   * Set the analog gain.
   */
  void Analog::SetGain(Gain g)
  {
    if (this->m_Gain = g)
      return;
    this->m_Gain = g;
    this->Modified();
  };

  /**
   * @fn Pointer Analog::Clone() const
   * Deep copy of the current object.
   */

  /**
   * Constructor.
   * @warning The number of frames must be greater than 0.
   */
  Analog::Analog(const std::string& label, int frameNumber, Gain g)
  : Measure<1>(label, frameNumber), m_Unit("")
  {
    this->m_Gain = g;
  };

  /**
   * Copy constructor
   */
  Analog::Analog(const Analog& toCopy)
  : Measure<1>(toCopy), m_Unit(toCopy.m_Unit)
  {
    this->m_Gain = toCopy.m_Gain;
  };

}


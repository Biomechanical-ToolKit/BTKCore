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

#include "btkAnalog.h"

namespace btk
{
  /**
   * @class Analog btkAnalog.h
   * @brief Analog channel data along the time.
   *
   * An analog channel is defined as:
   *  - a set of data samples;
   *  - a unit;
   *  - a scale;
   *  - an offset;
   *  - a gain.
   *
   * The data samples are is stored in a Analog::Data object.
   * 
   * The use of the unit, scale, offset and gain are useful for other classes/methods. For example, when you save an acquisition in a file, analog's data are generally unscaled and written as integer, then the scale and offset are important.
   *
   * @ingroup BTKCommon
   */

  /**
   * @typedef Analog::Values
   * Analog' values along the time with 1 component (1 column).
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
   * @var Analog::Gain Analog::PlusMinus1
   * +/- 1 volts.
   */
  /**
   * @var Analog::Gain Analog::PlusMinus0Dot5
   * +/- 0.5 volts.
   */
  /**
   * @var Analog::Gain Analog::PlusMinus0Dot25
   * +/- 0.25 volts.
   */
  /**
   * @var Analog::Gain Analog::PlusMinus0Dot1
   * +/- 0.1 volts.
   */
  /**
   * @var Analog::Gain Analog::PlusMinus0Dot05
   * +/- 0.05 volts.
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
   * @typedef Analog::NullPointer
   * Special null pointer associated with a Analog object.
   * This type should be used only internally to test the nullity of a smart pointer.
   * See the static method Null() instead.
   */
  
  /**
   * @fn static NullPointer Analog::Null()
   * Static function to return a null pointer.
   * @note This static method should be used only to test if a shared ponter is null or not. 
   * It is advised to call the method without parenthesis as special (in)equality 
   * operator are implemented to use a function pointer. See the description of the class NullPtr 
   * for an example.
   */

  /**
   * @fn static Pointer Analog::New(const std::string& label, const std::string& desc = "")
   * @brief Creates a smart pointer associated with a Analog object.
   *
   * The measure created has no values.
   *
   * The call of this function must be followed by the use of the method Measure::SetFrameNumber
   * as no btk::Analog::Data object is allocated.
   */

  /**
   * @fn static Pointer Analog::New(int frameNumber)
   * @brief Creates a smart pointer associated with a Analog object.
   *
   * The analog channel created has an empty label and a number of frames  equals to @a framenumber.
   *
   * The number of frames must be equal or greater than 0.
   * In case the number of frame is set to 0, no btk::Analog::Data object is allocated. You will need to use the method Measure::SetFrameNumber if you want to assign analog data later.
   */

  /**
   * @fn static Pointer Analog::New(const std::string& label, int frameNumber)
   * @brief Creates a smart pointer associated with a Analog object.
   *
   * The analog channel created has a label and a number of frames  equals to @a label and @a framenumber respectively.
   *
   * The number of frames must be equal or greater than 0.
   * In case the number of frame is set to 0, no btk::Analog::Data object is allocated. You will need to use the method Measure::SetFrameNumber if you want to assign analog data later.
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
   * Sets the analog gain.
   */
  void Analog::SetGain(Gain g)
  {
    if (this->m_Gain == g)
      return;
    this->m_Gain = g;
    this->Modified();
  };

  /**
   * @fn int Analog::GetOffset() const
   * Returns the analog offset value in bit which represents the 0 value.
   */

  /**
   * Sets the analog offset.
   */
  void Analog::SetOffset(int o)
  {
    if (this->m_Offset == o)
      return;
    this->m_Offset = o;
    this->Modified();
  }

  /**
   * @fn double Analog::GetScale() const
   * Returns the scaling factor which can bu used to transform a ADC value into real value.
   */

  /**
   * Set the scaling factor.
   */
  void Analog::SetScale(double s)
  {
    if (s == 0.0)
    {
      btkErrorMacro("Impossible to set a scaling factor to 0.");
      return;
    }
    if (fabs(this->m_Scale - s) <= std::numeric_limits<double>::epsilon())
      return;
    this->m_Scale = s;
    this->Modified();
  };

  /**
   * @fn Pointer Analog::Clone() const
   * Deep copy of the current object.
   */

  /**
   * Constructor.
   */
  Analog::Analog(const std::string& label, const std::string& desc)
  : Measure<Analog>(label, desc), m_Unit("V")
  {
    this->m_Gain = Unknown;
    this->m_Offset = 0;
    this->m_Scale = 1.0;
  };
  
  /**
   * Constructor. 
   */
  Analog::Analog(const std::string& label, int frameNumber, Gain g)
  : Measure<Analog>(label, frameNumber), m_Unit("V")
  {
    this->m_Gain = g;
    this->m_Offset = 0;
    this->m_Scale = 1.0;
  };

  /**
   * Copy constructor
   */
  Analog::Analog(const Analog& toCopy)
  : Measure<Analog>(toCopy), m_Unit(toCopy.m_Unit)
  {
    this->m_Gain = toCopy.m_Gain;
    this->m_Offset = toCopy.m_Offset;
    this->m_Scale = toCopy.m_Scale;
  };

  /**
   * @fn void Analog::SetDataSlice(int idx, double val)
   * Convenient method to set easily the value @a val for the given frame index @a idx.
   * @warning This function is not safe. There is no checking to determine if there is data or if the frame is out of range or not. It has the advantage to be faster.
   */
  
  // ----------------------------------------------------------------------- //
  
  /**
   * @struct MeasureTraits<Analog> btkAnalog.h
   * Specialized template for the information related to the data stored in a btk::Analog object.
   */
 
  /**
   * @typedef MeasureTraits<Analog>::Data::Pointer
   * Smart pointer associated with a MeasureTraits<Analog>::Data object.
   */
  
  /**
   * @typedef MeasureTraits<Analog>::Data::ConstPointer
   * Smart pointer associated with a const MeasureTraits<Analog>::Data object.
   */
   
  /**
   * @typedef MeasureTraits<Analog>::Data::NullPointer
   * Special null pointer associated with a Analog::Data object.
   * This type should be used only internally to test the nullity of a smart pointer.
   * See the static method Null() instead.
   */
  
  /**
   * @fn static NullPointer MeasureTraits<Analog>::Data::Null()
   * Static function to return a null pointer.
   * @note This static method should be used only to test if a shared ponter is null or not. 
   * It is advised to call the method without parenthesis as special (in)equality 
   * operator are implemented to use a function pointer. See the description of the class NullPtr 
   * for an example.
   */
  
  /**
   * @fn static Pointer MeasureTraits<Analog>::Data::New(int frameNumber)
   * Creates a smart pointer associated with a MeasureTraits<Analog>::Data object.
   */
  
  /**
   * @fn void MeasureTraits<Analog>::Data::Resize(int frameNumber);
   * Resize the number of frames for the values.
   * @warning The input @a frameNumber cannot be set to value lower or equal to 0. This method doesn't check for the given number of frames and will crash if the value is lower or equal to 0.
   */
  
  /**
   * @fn MeasureTraits<Analog>::Data::Pointer MeasureTraits<Analog>::Data::Clone() const
   * Deep copy of the current object.
   */
}


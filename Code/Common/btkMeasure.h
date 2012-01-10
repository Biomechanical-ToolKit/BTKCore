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

#ifndef __btkMeasure_h
#define __btkMeasure_h

#include "btkDataObject.h"
#include "btkMacro.h"

#include <Eigen/Core>
#include <string>

namespace btk
{
  template <int d>
  class Measure : public DataObjectLabeled
  {
  public:
    typedef Eigen::Matrix<double, Eigen::Dynamic, d> Values;
    
    typedef SharedPtr<Measure> Pointer;
    typedef SharedPtr<const Measure> ConstPointer;
    
    static Pointer New(const std::string& label = "") {return Pointer(new Measure(label));};
    static Pointer New(int frameNumber) {return Pointer(new Measure("", frameNumber));};
    static Pointer New(const std::string& label, int frameNumber) {return Pointer(new Measure(label, frameNumber));};
    
    virtual ~Measure() {};
    
    Values& GetValues() {return this->m_Values;};
    const Values& GetValues() const {return this->m_Values;};
    void SetValues(const Values& v);
    int GetFrameNumber() const {return this->m_Values.rows();};
    void SetFrameNumber(int frameNumber);
    Pointer Clone() const {return Pointer(new Measure<d>(*this));};
    
  protected:
    Measure(const std::string& label, const std::string& desc = "");
    Measure(const std::string& label, int frameNumber, const std::string& desc = "");
    Measure(const Measure& toCopy);
    
    Values m_Values;
    
  private: 
    Measure& operator=(const Measure& ); // Not implemented.
  };
 
  /**
   * @class Measure btkMeasure.h
   * @brief Measure with @a d dimensions along the time.
   *
   * The measures are stored in a matrix. The measures' dimensions correspond to the columns of the matrix. The frames correspond to the line.
   *
   * @tparam d Number of dimensions.
   *
   * @warning The number of dimensions @a d must be greater than 0.
   *
   * @ingroup BTKCommon
   */
  /**
   * @var Measure::m_Values
   * Values of the measure.
   */

  /**
   * @typedef Measure<d>::Values
   * Measures' values along the time.
   */

  /**
   * @typedef Measure<d>::Pointer
   * Smart pointer associated with a Measure object.
   */
  
  /**
   * @typedef Measure<d>::ConstPointer
   * Smart pointer associated with a const Measure object.
   */
  
  /**
   * @fn template <int d> static Pointer Measure<d>::New(const std::string& label = "")
   * @brief Creates a smart pointer associated with a Measure object.
   *
   * The measure created has no values.
   * @warning The call of this function must be followed by the use of the method Measure::SetFrameNumber
   * as it creates a null matrix for the values.
   */ 
   
  /**
   * @fn template <int d> static Pointer Measure<d>::New(int frameNumber)
   * @brief Creates a smart pointer associated with a Measure object.
   *
   * The measure created has an empty label and a number of frames  equals to @a framenumber.
   * @warning The number of frames must be greater than 0.
   */ 

  /**
   * @fn template <int d> static Pointer Measure::New(const std::string& label, int frameNumber)
   * @brief Creates a smart pointer associated with a Measure object.
   *
   * The measure created has a label and a number of frames  equals to @a label and @a framenumber respectively.
   * @warning The number of frames must be greater than 0.
   */

  /**
   * @fn template <int d> Values& Measure<d>::GetValues()
   * Returns the measure's values.
   *
   * @warning If you modify the object's content with this function, don't forget to call the Modified() method.
   */

  /**
   * @fn template <int d> const Values& Measure<d>::GetValues() const
   * Returns the measure's values.
   */
  
  /**
   * Set the values of the measure.
   * @warning The values' dimensions must be equal than the dimensions of the object.
   */
  template <int d>
  void Measure<d>::SetValues(const Values& v)
  {
    this->m_Values = v;
    this->Modified();
  };
  
  /**
   * @fn template <int d> int Measure<d>::GetFrameNumber() const 
   * Returns the number of frames.
   */

  /**
   * @fn template <int d> Pointer Measure<d>::Clone() const
   * Clones the object.
   */

  /**
   * Sets the number of frames.
   * The input @a frameNumber must be greater than 0.
   */
  template <int d>
  void Measure<d>::SetFrameNumber(int frameNumber)
  {
    if (frameNumber <= 0)
    {
      btkErrorMacro("Impossible to set a number of frames lower or equal to 0.");
      return;
    }
    if (frameNumber == this->GetFrameNumber())
      return;
    else if (frameNumber > this->GetFrameNumber())
    {
      Values v = Values::Zero(frameNumber, d);
      if (this->m_Values.data() != 0)
        v.block(0,0,this->GetFrameNumber(),d) = this->m_Values;
      this->m_Values = v;
    }
    else
    {
      Values v = this->m_Values.block(0,0,frameNumber,d);
      this->m_Values = v;
    }
    this->Modified();
  };

   /**
   * Constructor.
   * @warning The use of this constructor must be followed by the use of the method Measure::SetFrameNumber
   * as it creates a null matrix for the values.
   */
  template <int d>
  Measure<d>::Measure(const std::string& label, const std::string& desc)
  : DataObjectLabeled(label, desc), m_Values()
  {};

  /**
   * Constructor.
   */
  template <int d>
  Measure<d>::Measure(const std::string& label, int frameNumber, const std::string& desc)
  : DataObjectLabeled(label, desc),
    m_Values(Values::Zero(frameNumber, d))
  {};

  /**
   * Copy constructor. Timestamp, source and parent are reset.
   */
  template <int d>
  Measure<d>::Measure(const Measure& toCopy)
  : DataObjectLabeled(toCopy), m_Values(toCopy.m_Values)
  {};

};

#endif // __btkMeasure_h

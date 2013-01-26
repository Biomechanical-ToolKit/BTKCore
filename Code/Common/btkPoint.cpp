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

#include "btkPoint.h"

namespace btk
{
  /**
   * @class Point btkPoint.h
   * @brief Coordinates of a point in a 3D space along the time.
   *
   * The coordinates of the point are generally measured by an acquisition system. For each frame, the
   * 3D value has a residual. It can provide informations on the quality of these 3D data 
   * (residual associated with the reconstruction of the marker). 
   * 
   *  This class is also used to represent angles, forces, moments, powers and scalars. 
   *  To know/set the type of the point, you can use the methods GetType() and SetType(). 
   * 
   * Note: In some case the values at specific frames are invalid (mainly due to marker's occlusion).
   * To detect if the frame is invalid, you can check the residual which will be 
   * set to the value -1. It is the same thing if you want to set the frame as invalid. You can set
   * the values of the frames to 0 and the associated residual to -1.
   * 
   * Note: A residual with a value equal to 0 means that this frame has been post-processed (interpolation, filtering, etc.).
   *
   * @ingroup BTKCommon
   */

   /**
   * @typedef Point::Residuals
   * Vector of double representing the residuals associated with each frames (if applicable).
   */

  /**
   * @enum Point::Type
   * Enums used to specify the point's type.
   */
  /**
   * @var Point::Type Point::Marker
   * Enum value which set the point object as a marker.
   */
  /**
   * @var Point::Type Point::Angle
   * Enum value which set the point object as an angle.
   */
  /**
   * @var Point::Type Point::Force
   * Enum value which set the point object as a force.
   */
  /**
   * @var Point::Type Point::Moment
   * Enum value which set the point object as a moment.
   */
  /**
   * @var Point::Type Point::Power
   * Enum value which set the point object as a power.
   */
  /**
   * @var Point::Type Point::Scalar
   * Enum value which set the point object as a scalar.
   */
  /**
   * @var Point::Type Point::Reaction
   * Special type to indicate the point corresponds to one part of a wrench (not really used);
   */

  /**
   * @typedef Point::Pointer
   * Smart pointer associated with a Point object.
   */

  /**
   * @typedef Point::ConstPointer
   * Smart pointer associated with a const Point object.
   */
   
  /**
   * @fn static Pointer Point::New(const std::string& label = "", Type t = Marker, const std::string& desc = "")
   * @brief Creates a smart pointer associated with a Point object.
   *
   * The Point created has no values.
   * @warning The call of this function must be followed by the use of the method Point::SetFrameNumber
   * as it creates a null matrix for the values.
   */ 

  /**
   * @fn static Pointer Point::New(int frameNumber)
   * @brief Creates a smart pointer associated with a Point object.
   *
   * The point created has an empty label and a number of frames  equals to @a framenumber.
   * @warning The number of frames must be greater than 0.
   */

  /**
   * @fn static Pointer Point::New(const std::string& label, int frameNumber, Type t = Marker, const std::string& desc = "")
   * @brief Creates a smart pointer associated with a Point object.
   *
   * The point created has a label and a number of frames equals to @a label and @a framenumber respectively.
   * @warning The number of frames must be greater than 0.
   */

  /**
   * @fn virtual Point::~Point()
   * Empty destructor.
   */

  /**
   * @fn Residuals& Point::GetResiduals()
   * Gets the residuals.
   */

  /**
   * @fn const Residuals& Point::GetResiduals() const
   * Gets the residuals.
   */

  /**
   * Sets the residuals.
   */  
  void Point::SetResiduals(const Residuals& r)
  {
    this->m_Residuals = r;
    this->Modified();
  };
  
  /**
   * Resizes the number of frames of the measure as well as its residual.
   * @warning The number of frames must be greater than 0.
   */
  void Point::SetFrameNumber(int frameNumber)
  {
    if (frameNumber <= 0)
    {
      btkErrorMacro("Impossible to set a number of frames lower or equal to 0.");
      return;
    }
    int actualFrameNumber = this->GetFrameNumber();
    if (frameNumber == actualFrameNumber)
      return;
    else if (frameNumber > actualFrameNumber)
    {
      Values v = Values::Zero(frameNumber, 3);
      if (this->m_Values.data() != 0)
        v.block(0,0,actualFrameNumber,3) = this->m_Values;
      this->m_Values = v;
      
      Residuals r = Residuals::Zero(frameNumber, 1);
      if (this->m_Residuals.data() != 0)
        r.block(0,0,actualFrameNumber,1) = this->m_Residuals;
      this->m_Residuals = r;
    }
    else
    {
      Values v = this->m_Values.block(0,0,frameNumber,3);
      this->m_Values = v;
      Residuals r = this->m_Residuals.block(0,0,frameNumber,1);
      this->m_Residuals = r;
    }
    this->Modified();
  };

  /**
   * @fn Type Point::GetType() const
   * Returns the point's type.
   */

  /**
   * Sets the point's type.
   */
  void Point::SetType(Point::Type t)
  {
    if (this->m_Type == t)
      return;
    this->m_Type = t;
    this->Modified();
  };
  
  /**
   * @fn Pointer Point::Clone() const
   * Returns a deep copy of this object.
   */


  /**
   * Constructor.
   * @warning The use of this constructor must be followed by the use of the method Measure::SetFrameNumber
   * as it creates a null matrix for the values.
   */
  Point::Point(const std::string& label, Type t, const std::string& desc)
  : Measure<3>(label, desc), m_Residuals()
  {
    this->m_Type = t;
  };
  
  /**
   * Constructor.
   * @warning The number of frames must be greater than 0.
   */
  Point::Point(const std::string& label, int frameNumber, Type t, const std::string& desc)
  : Measure<3>(label, frameNumber, desc),
    m_Residuals(Residuals::Zero(frameNumber, 1))
  {
    this->m_Type = t;
  };

  /**
   * Constructor of copy.
   */
  Point::Point(const Point& toCopy)
  : Measure<3>(toCopy),
    m_Residuals(toCopy.m_Residuals)
  {
    this->m_Type = toCopy.m_Type;
  };
  
  /**
   * @fn void Point::SetDataSlice(int idx, double x, double y, double z, double res = 0.0)
   * Convenient method to easily set the coordinates @a x, @a y, @a z and the residual @a res for the given @a idx.
   * @warning This function is not safe. There is no checking to determine if the index is out of range or not. It has the advantage to be faster.
   */
}

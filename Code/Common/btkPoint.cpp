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

#include "btkPoint.h"

namespace btk
{
  /**
   * @class Point btkPoint.h
   * @brief Coordinates of a point in a 3D space along the time.
   *
   * The coordinates of the point are generally measured by an acquisition system. For each frame, the
   * 3D value has a residual. It can provide informations on the quality of these 3D data 
   * (residual associated with the reconstruction of the marker). These values are stored in a Point::Data object.
   * 
   * This class is also used to represent angles, forces, moments, powers and scalars. 
   * To know/set the type of the point, you can use the methods GetType() and SetType(). 
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
   * @typedef Point::NullPointer
   * Special null pointer associated with a Point object.
   * This type should be used only internally to test the nullity of a smart pointer.
   * See the static method Null() instead.
   */
  
  /**
   * @fn static NullPointer Point::Null()
   * Static function to return a null pointer.
   * @note This static method should be used only to test if a shared ponter is null or not. 
   * It is advised to call the method without parenthesis as special (in)equality 
   * operator are implemented to use a function pointer. See the description of the class NullPtr 
   * for an example.
   */
  
  /**
   * @fn static Pointer Point::New(const std::string& label = "", Type t = Marker, const std::string& desc = "")
   * @brief Creates a smart pointer associated with a Point object.
   *
   * The Point created has no values.
   *
   * The call of this function must be followed by the use of the method Point::SetFrameNumber
   * as no btk::Point::Data object is allocated.
   */ 

  /**
   * @fn static Pointer Point::New(int frameNumber)
   * @brief Creates a smart pointer associated with a Point object.
   *
   * The point created has an empty label and a number of frames  equals to @a framenumber.
   *
   * The number of frames must be equal or greater than 0.
   * In case the number of frame is set to 0, no btk::Point::Data object is allocated. You will need to use the method Measure::SetFrameNumber if you want to assign point data later.
   */

  /**
   * @fn static Pointer Point::New(const std::string& label, int frameNumber, Type t = Marker, const std::string& desc = "")
   * @brief Creates a smart pointer associated with a Point object.
   *
   * The point created has a label and a number of frames equals to @a label and @a framenumber respectively.
   *
   * The number of frames must be equal or greater than 0.
   * In case the number of frame is set to 0, no btk::Point::Data object is allocated. You will need to use the method Measure::SetFrameNumber if you want to assign point data later.
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
   * Convenient method to return the residuals associated with measure's data.
   * @warning This method tries to access directly to data's residuals even if no data has been set. Use this method carefully or use GetData() to access to point's data. 
   */
  Point::Residuals& Point::GetResiduals()
  {
    assert(this->mp_Data != Point::Data::Null);
    return this->mp_Data->GetResiduals();
  }
 
  /**
   * Convenient method to return the residuals associated with measure's data.
   * @warning This method tries to access directly to data's residuals even if no data has been set. Use this method carefully or use GetData() to access to point's data. 
   */
  const Point::Residuals& Point::GetResiduals() const
  {
    assert(this->mp_Data != Point::Data::Null);
    return this->mp_Data->GetResiduals();
  };

  /**
   * Sets the residuals.
   */  
  void Point::SetResiduals(const Residuals& r)
  {
    if (!this->mp_Data)
      this->SetData(Point::Data::New(static_cast<int>(r.rows())));
    this->mp_Data->SetResiduals(r);
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
   */
  Point::Point(const std::string& label, Type t, const std::string& desc)
  : Measure<Point>(label, desc)
  {
    this->m_Type = t;
  };
  
  /**
   * Constructor.
   */
  Point::Point(const std::string& label, int frameNumber, Type t, const std::string& desc)
  : Measure<Point>(label, frameNumber, desc)
  {
    this->m_Type = t;
  };

  /**
   * Constructor of copy.
   */
  Point::Point(const Point& toCopy)
  : Measure<Point>(toCopy)
  {
    this->m_Type = toCopy.m_Type;
  };
  
  /**
   * @fn void Point::SetDataSlice(int idx, double x, double y, double z, double res = 0.0)
   * Convenient method to easily set the coordinates @a x, @a y, @a z and the residual @a res for the given @a idx.
   * @warning This function is not safe. There is no checking to determine if the index is out of range or not. It has the advantage to be faster.
   */
 
  // ----------------------------------------------------------------------- //
  
  /**
   * @struct MeasureTraits<Point> btkPoint.h
   * Specialized template for the information related to the data stored in a btk::Point object.
   */
  
  /**
   * @typedef MeasureTraits<Point>::Residuals
   * Vector of double representing the residuals associated with each frames (if applicable).
   */
   
  /**
   * @typedef MeasureTraits<Point>::Data::Pointer
   * Smart pointer associated with a MeasureTraits<Point>::Data object.
   */
  
  /**
   * @typedef MeasureTraits<Point>::Data::ConstPointer
   * Smart pointer associated with a const MeasureTraits<Point>::Data object.
   */
   
  /**
   * @typedef MeasureTraits<Point>::Data::NullPointer
   * Special null pointer associated with a Point::Data object.
   * This type should be used only internally to test the nullity of a smart pointer.
   * See the static method Null() instead.
   */
  
  /**
   * @fn static NullPointer MeasureTraits<Point>::Data::Null()
   * Static function to return a null pointer.
   * @note This static method should be used only to test if a shared ponter is null or not. 
   * It is advised to call the method without parenthesis as special (in)equality 
   * operator are implemented to use a function pointer. See the description of the class NullPtr 
   * for an example.
   */
  
  /**
   * @fn static Pointer MeasureTraits<Point>::Data::New(int frameNumber)
   * Creates a smart pointer associated with a MeasureTraits<Point>::Data object.
   */
  
  /**
   * @fn void MeasureTraits<Point>::Data::Resize(int frameNumber);
   * Resize the number of frames for the values and the residuals.
   * @warning The input @a frameNumber cannot be set to value lower or equal to 0. This method doesn't check for the given number of frames and will crash if the value is lower or equal to 0.
   */
  
  /**
   * @fn MeasureTraits<Point>::Data::Residuals& MeasureTraits<Point>::Data::GetResiduals()
   * Returns the residuals for to this data.
   */
  
  /**
   * @fn const MeasureTraits<Point>::Data::Residuals& MeasureTraits<Point>::Data::GetResiduals() const
   * Returns the residuals for to this data.
   */
  
  /**
   * @fn void MeasureTraits<Point>::Data::SetResiduals(const MeasureTraits<Point>::Data::Residuals& r)
   * Sets the residuals for to this data.
   */
 
  /**
   * @fn MeasureTraits<Point>::Data::Pointer MeasureTraits<Point>::Data::Clone() const
   * Deep copy of the current object.
   */
}

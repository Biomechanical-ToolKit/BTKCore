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

#include "btkForcePlatform.h"

namespace btk
{
  /**
   * @class ForcePlatform btkForcePlatform.h
   * @brief Base class for all type of force platform.
   *
   * This class is generic and doesn't know its type, nor the number of analog channels used.
   * To use predefined force platform as proposed in the documentation of the C3D file format, check the following classes:
   *  - btk::ForcePlatformType1: 6 channels (FX, FY, FZ, PX, PY, MZ);
   *  - btk::ForcePlatformType2: 6 channels (FX, FY, FZ, MX, MY, MZ);
   *  - btk::ForcePlatformType3: 8 channels (FZ1, FZ2, FZ3, FZ4, FX12, FX34, FY14, FY23);
   *  - btk::ForcePlatformType4: Same as Type-2 + calibration matrix 6 (columns) by 6 (rows);
   *  - btk::ForcePlatformType5: Same as Type-3 + calibration matrix 6 (columns) by 8 (rows);
   *  - btk::ForcePlatformType6: 12 channels (FX[1,2,3,4], FY[1,2,3,4], FZ[1,2,3,4] + calibration matrix 12 by 12).
   *
   * @ingroup BTKCommon 
   */
  /**
   * @var ForcePlatform::m_Type
   * Type of the force platform. Used to determine necessary data for a particular force platform model.
   */
  /**
   * @var ForcePlatform::m_Channels
   * Channels of the force platform.
   */
  /**
   * @var ForcePlatform::m_CalMatrix
   * Calibrage matrix which could be used to correct cross talk between channels.
   */

  /**
   * @typedef ForcePlatform::Corners
   * Type which represents the coordinates of the four corners of the force platform.
   */

  /**
   * @typedef ForcePlatform::Corner
   * Type which represents the coordinates of one corner.
   */

  /**
   * @typedef ForcePlatform::Origin
   * Type which represents the origin of the force platform.
   */

  /**
   * @typedef ForcePlatform::CalMatrix
   * Type which represents a possible calibration matrix used with the force platform.
   */

  /**
   * @typedef ForcePlatform::Pointer
   * Smart pointer associated with a ForcePlatform object.
   */
  
  /**
   * @typedef ForcePlatform::ConstPointer
   * Smart pointer associated with a const ForcePlatform object.
   */

  /**
   * @typedef ForcePlatform::Iterator
   * Iterator for channels contained in the force platform object.
   */
  
  /**
   * @typedef ForcePlatform::ConstIterator
   * Const iterator for channels contained in the force platform object.
   */

  /**
   * @fn static Pointer ForcePlatform::New()
   * Creates a smart pointer associated with a ForcePlatform object.
   */

  /**
   * @fn ForcePlatform::~ForcePlatform()
   * Empty destructor.
   */

  /**
   * @fn Iterator ForcePlatform::Begin()
   * Returns an iterator to the beginning of the list of items. 
   */

  /**
   * @fn ConstIterator ForcePlatform::Begin() const
   * Returns a const iterator to the beginning of the list of items. 
   */

  /**
   * @fn Iterator ForcePlatform::End()
   * Returns an iterator just past the last item. 
   */

  /**
   * @fn ConstIterator ForcePlatform::End() const
   * Returns a const iterator just past the last item. 
   */

  /**
   * @fn int ForcePlatform::GetChannelNumber() const
   * Returns the number of channels.
   */

  /**
   * @fn Analog::Pointer ForcePlatform::GetChannel(int idx) const
   * Returns the channel at the index @a idx.
   */

  /**
   * Sets the channel at the index @a idx by the object @a c.
   */
  void ForcePlatform::SetChannel(int idx, Analog::Pointer c)
  {
    if (this->m_Channels->SetItem(idx, c))
      this->Modified();
  }

  /**
   * @fn AnalogCollection::Pointer ForcePlatform::GetChannels() const 
   * Returns force platform's channels.
   */

  /**
   * @fn Origin& ForcePlatform::GetOrigin()
   * Returns the origin of the force platform.
   *
   * @warning If you modify the object's content with this function, don't forget to call the Modified() method.
   */
  
  /**
   * @fn const Origin& ForcePlatform::GetOrigin() const
   * Returns the origin of the force platform.
   */

  /**
   * Sets the origin.
   */
  void ForcePlatform::SetOrigin(double x, double y, double z)
  {
    if ((this->m_Origin.x() == x)
        && (this->m_Origin.y() == y)
        && (this->m_Origin.z() == z))
      return;
    this->m_Origin.x() = x;
    this->m_Origin.y() = y;
    this->m_Origin.z() = z;
    this->Modified();
  };
  
  /**
   * Sets the origin.
   */
  void ForcePlatform::SetOrigin(const Origin& o)
  {
    if (this->m_Origin.isApprox(o))
      return;
    this->m_Origin = o;
    this->Modified();
  };

  /**
   * Returns the corner at index @a idx.
   */
  const ForcePlatform::Corner ForcePlatform::GetCorner(int idx) const
  {
    if (idx >= 4)
      throw OutOfRangeException("ForcePlatform::GetCorner");
    return this->m_Corners.col(idx);
  };

  /**
   * Sets the coordinate at position @a row and @a col with the value @a v.
   */
  void ForcePlatform::SetCorner(int row, int col, double v)
  {
    if ((row > 3) || (col > 4))
      throw OutOfRangeException("ForcePlatform::SetCorner");
    this->m_Corners.coeffRef(row, col) = v;
  };


  /**
   * Set the coordinates of the corner @a idx.
   */
  void ForcePlatform::SetCorner(int idx, double x, double y, double z)
  {
    if (idx >= 4)
      throw OutOfRangeException("ForcePlatform::SetCorner");
    Corner c = this->m_Corners.col(idx);
    if ((c.x() == x)
        && (c.y() == y)
        && (c.z() == z))
      return;
    this->m_Corners.coeffRef(0, idx) = x;
    this->m_Corners.coeffRef(1, idx) = y;
    this->m_Corners.coeffRef(2, idx) = z;
    this->Modified();
  };

  /**
   * Set the corner @a idx by @a c.
   */
  void ForcePlatform::SetCorner(int idx, const Corner& c)
  {
    if (idx >= 4)
      throw OutOfRangeException("ForcePlatform::SetCorner"); 
    if (this->m_Corners.col(idx).isApprox(c))
      return;
    this->m_Corners.col(idx) = c;
    this->Modified();
  };

  /**
   * @fn Corners& ForcePlatform::GetCorners()
   * Returns corners.
   *
   * @warning If you modify the object's content with this function, don't forget to call the Modified() method.
   */
  
  /**
   * @fn const Corners& ForcePlatform::GetCorners() const
   * Returns corners.
   */

  /**
   * Set corners by @a c.
   */
  void ForcePlatform::SetCorners(const Corners& c)
  {
    if (this->m_Corners.isApprox(c))
      return;
    this->m_Corners = c;
    this->Modified();
  };

  /**
   * @fn CalMatrix& ForcePlatform::GetCalMatrix()
   * Returns calibration marix.
   *
   * @warning If you modify the object's content with this function, don't forget to call the Modified() method.
   */
  
  /**
   * @fn const CalMatrix& ForcePlatform::GetCalMatrix() const
   * Returns the calibration matrix.
   */

  /**
   * Set the calibration matrix
   */  
  void ForcePlatform::SetCalMatrix(const CalMatrix& cal)
  {
    if (this->m_CalMatrix.isApprox(cal))
      return;
    this->m_CalMatrix = cal;
    this->Modified();
  };


  /**
   * @fn int ForcePlatform::GetType() const
   * Returns the type of the force platform.
   */

  /**
   * Constructor.
   */
  ForcePlatform::ForcePlatform()
  : m_Channels(), m_CalMatrix()
  { 
    this->m_Origin << 0.0, 0.0, 0.0;
    // By default the force platform is square with a side length of 500mm.
    this->m_Corners << 1.0, 1.0, 0.0,
                      -1.0, 1.0, 0.0,
                      -1.0,-1.0, 0.0,
                       1.0,-1.0, 0.0;
    this->m_Corners *= 250;
    this->m_Channels = AnalogCollection::New();
    this->m_Type = 0;
  };
  
  ForcePlatform::ForcePlatform(const ForcePlatform& toCopy)
  : m_CalMatrix(toCopy.m_CalMatrix), m_Origin(toCopy.m_Origin), m_Corners(toCopy.m_Corners)
  {
    this->m_Channels = toCopy.m_Channels->Clone();
    this->m_Type = toCopy.m_Type;
  };
};
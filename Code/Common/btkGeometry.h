/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2010, Arnaud Barré
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

#ifndef __btkGeometry_h
#define __btkGeometry_h

#include <Eigen/Core>

namespace btk
{
  typedef Eigen::Matrix<double, 3, 1> Position;

  class Frame
  {
  public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  
    typedef Eigen::Matrix<double, 3, 1> Axis;
    typedef Eigen::Matrix<double, 3, 1> Origin;
    typedef Eigen::Matrix<double, 4, 4> Matrix;
  
    Frame()
    : m_Frame()
    {
      this->m_Frame.setIdentity();
    };
    // Frame(const Frame& toCopy); // Implicit.
    // ~Frame(); // Implicit.
    Matrix& GetMatrix() {return this->m_Frame;};
    const Matrix& GetMatrix() const {return this->m_Frame;};
    void SetMatrix(const Matrix& f) {this->m_Frame = f;};
    void SetAxisX(double x, double y, double z) {this->SetFrame(0,x,y,z);};
    void SetAxisX(const Axis& a) {this->SetFrame(0,a.coeff(0),a.coeff(1),a.coeff(2));};
    const Axis GetAxisX() const {return this->m_Frame.block(0, 0, 3, 1);};
    void SetAxisY(double x, double y, double z) {this->SetFrame(1,x,y,z);};
    void SetAxisY(const Axis& a) {this->SetFrame(1,a.coeff(0),a.coeff(1),a.coeff(2));};
    const Axis GetAxisY() const {return this->m_Frame.block(0, 1, 3, 1);};
    void SetAxisZ(double x, double y, double z) {this->SetFrame(2,x,y,z);};
    void SetAxisZ(const Axis& a) {this->SetFrame(2,a.coeff(0),a.coeff(1),a.coeff(2));};
    const Axis GetAxisZ() const {return this->m_Frame.block(0, 2, 3, 1);};
    void SetOrigin(double x, double y, double z) {this->SetFrame(3,x,y,z);};
    void SetOrigin(const Origin& o) {this->SetFrame(3,o.coeff(0),o.coeff(1),o.coeff(2));};
    const Origin GetOrigin() const {return this->m_Frame.block(0, 3, 3, 1);};
    
    friend bool operator==(const Frame& lhs, const Frame& rhs) {return lhs.m_Frame.isApprox(rhs.m_Frame);};
    friend bool operator!=(const Frame& lhs, const Frame& rhs) {return !(lhs == rhs);}
  
  private:
    void SetFrame(int c, double x, double y, double z)
    {
      this->m_Frame.coeffRef(0, c) = x;
      this->m_Frame.coeffRef(1, c) = y;
      this->m_Frame.coeffRef(2, c) = z;
    };
  
    Eigen::Matrix<double, 4, 4> m_Frame;
  };
  
  /**
   * @typedef Position
   * Column vector containing X,Y,Z coordinates.
   */
   
  /**
   * @class Frame
   * @brief Object to represent a coordinate system (X, Y, Z axes and an origin).
   */
   
  /**
   * @typedef Frame::Matrix
   * Transformation matrix 4 x 4.
   */

  /**
   * @typedef Frame::Axis
   * Colum vector used to represent frame's axis..
   */

  /**
   * @typedef Frame::Origin
   * Colum vector representing the frame's orgin.
   */
  
  /**
   * @fn Frame::Frame()
   * Constructor. The frame is defined as:
   *  - X axis: same direction than the reference: [1,0,0]
   *  - X axis: same direction than the reference: [0,1,0]
   *  - X axis: same direction than the reference: [0,0,1]
   *  - Origin: same origin than the reference: [0,0,0];
   *
   * @ingroup BTKCommon
   */

  /**
    * @fn Frame::Matrix& Frame::GetMatrix()
    * Gets the frame as a 4 x 4 matrix.
    */
    
  /**
    * @fn const Frame::Matrix& Frame::GetMatrix() const
    * Gets the frame as a 4 x 4 matrix.
    */

   /**
    * @fn void Frame::SetMatrix(const Frame::Matrix& f)
    * Sets the frame.
    */

   /**
    * @fn void Frame::SetAxisX(double x, double y, double z)
    * Sets the X axis.
    */

   /**
    * @fn void Frame::SetAxisX(const Axis& a)
    * Sets the X axis.
    */

   /**
    * @fn const Axis Frame::GetAxisX() const
    * Gets the X axis.
    */

   /**
    * @fn void Frame::SetAxisY(double x, double y, double z)
    * Sets the Y axis.
    */

   /**
    * @fn void Frame::SetAxisY(const Axis& a)
    * Sets the Y axis.
    */

   /**
    * @fn const Axis Frame::GetAxisY() const
    * Gets the Y axis.
    */

   /**
    * @fn void Frame::SetAxisZ(double x, double y, double z)
    * Sets the Z axis.
    */

   /**
    * @fn void Frame::SetAxisZ(const Axis& a)
    * Sets the Z axis.
    */

   /**
    * @fn const Axis Frame::GetAxisZ() const
    * Gets the Z axis.
    */

   /**
    * @fn void Frame::SetOrigin(double x, double y, double z)
    * Sets the origin.
    */

   /**
    * @fn void Frame::SetOrigin(const Origin& o)
    * Sets the origin.
    */

   /**
    * @fn const Origin Frame::GetOrigin() const
    * Gets the origin.
    */
    
  /**
    * @fn friend bool Frame::operator==(const Frame& lhs, const Frame& rhs)
    * Convenient method to check equality of two frames' matrix..
    */
    
  /**
    * @fn friend bool Frame::operator!=(const Frame& lhs, const Frame& rhs)
    * Convenient method to check inequality of two frames' matrix.
    */
}

#endif // __btkGeometry_h
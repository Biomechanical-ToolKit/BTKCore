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

#include "btkJoint.h"

namespace btk
{
  /**
   * @class Joint
   * @brief Class to define a joint, defined by a proximal and distal segment.
   *
   * A joint is supposed to have 6 DOFs, so without any constraint.
   * To define a joint between a segment and the global frame, you have only to define
   * global frame as an empty segment (i.e. use btk::Segment::Pointer()).
   * For example, to define the joint "thorax" between the global frame and the trunk you can use the 
   * following code:
   * @code
   * btk::Segment::Pointer trunk = btk::Segment::New(Segment::Trunk, Segment::Central, "Trunk", "");
   * btk::Joint::Pointer thorax = btk::Joint::New(btk::Segment::Pointer(), trunk,"Thorax", "Joint between the global frame and the trunk")
   * @endcode
   *
   * @ingroup BTKCommon
   */

  /**
   * @typedef Joint::Pointer
   * Smart pointer associated with a Joint object.
   */

  /**
   * @typedef Joint::ConstPointer
   * Smart pointer associated with a const Joint object.
   */
 
  /**
   * @fn static Pointer Joint::New()
   * Creates a smart pointer associated with a Point object.
   */
   
  /**
   * @fn static Pointer Joint::New(Segment::Pointer proximal, Segment::Pointer distal, const std::string& label, const std::string& description)
   * Creates a smart pointer associated with a Point object.
   */
    
  /**
   * @fn Segment::Pointer Joint::GetProximalSegment()
   * Returns the prixmal segment of this joint.
   */
  
  /**
   * @fn Segment::ConstPointer Joint::GetProximalSegment() const
   * Returns the prixmal segment of this joint.
   */
  
  /**
   * Sets the proximal segment.
   */
  void Joint::SetProximalSegment(Segment::Pointer seg)
  {
    if (this->m_Proximal == seg)
      return;
    this->m_Proximal = seg;
    this->Modified();
  };
  
  /**
   * @fn Segment::Pointer Joint::GetDistalSegment()
   * Returns the distal segment of this joint.
   */
   
  /**
   * @fn Segment::ConstPointer Joint::GetDistalSegment() const
   * Returns the distal segment of this joint.
   */

  /**
   * Sets the distal segment.
   */
  void Joint::SetDistalSegment(Segment::Pointer seg)
  {
    if (this->m_Distal == seg)
      return;
    this->m_Distal = seg;
    this->Modified();
  };
   
  /**
   * @fn Joint::Pointer Joint::Clone()
   * Returns a deep copy of the object as a smart pointer.
   */
  
  /**
   * Constructor.
   */
  Joint::Joint()
  : DataObjectLabeled(), m_Proximal(), m_Distal()
  {};
   
  /**
   * Constructor.
   */
  Joint::Joint(Segment::Pointer proximal, Segment::Pointer distal, const std::string& label, const std::string& description)
  : DataObjectLabeled(label, description), m_Proximal(), m_Distal()
  {
    this->m_Proximal = proximal;
    this->m_Distal = distal;
  };
  
  /**
   * Copy constructor.
   * The proximal and distal segments are not cloned.
   */
  Joint::Joint(const Joint& toCopy)
  : DataObjectLabeled(toCopy), m_Proximal(toCopy.m_Proximal), m_Distal(toCopy.m_Distal)
  {
    /*
    if (toCopy.m_Proximal.get())
      this->m_Proximal = toCopy.m_Proximal->Clone();
    if (toCopy.m_Distal.get())
      this->m_Distal = toCopy.m_Distal->Clone();
    */
  };
};
 
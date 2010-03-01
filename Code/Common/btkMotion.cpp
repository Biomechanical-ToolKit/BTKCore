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
 
#include "btkMotion.h"
#include "btkException.h"
#include "btkMacro.h"

namespace btk
{
  /**
   * @class AbstractMotion
   * @brief Abstract container to represent the motion of a rigid body.
   *
   * @ingroup BTKCommon
   */

  /**
   * @typedef AbstractMotion::Pointer
   * Smart pointer associated with a AbstractMotion object.
   */

  /**
   * @typedef AbstractMotion::ConstPointer
   * Smart pointer associated with a const AbstractMotion object.
   */

  /**
   * @fn virtual Frame& AbstractMotion::GetFrame(int idx) = 0
   * Returns the frame at the index @a idx.
   */
  
  /**
   * @fn virtual const Frame& AbstractMotion::GetFrame(int idx) const = 0
   * Returns the frame at the index @a idx.
   */
  
  /**
   * @fn virtual Frame& AbstractMotion::operator[](int idx) = 0
   * Returns the frame at index @a idx without any assertion.
   */
  
  /**
   * @fn virtual const Frame& AbstractMotion::operator[](int idx) const = 0
   * Returns the frame at index @a idx without any assertion.
   */
  
  /**
   * @fn AbstractMotion::AbstractMotion()
   * Constructor.
   */
  
  /**
   * @fn AbstractMotion::AbstractMotion(const AbstractMotion& toCopy)
   * Copy constructor.
   */
  
  /**
   * @class NoMotion
   * @brief (false) Container of frames used to represent the motion of a rigid body.
   *
   * This object represents static segment/object like the global frame.
   * The implement method in this class return every time the matrix identitity.
   * 
   * @ingroup BTKCommon
   */

  /**
   * @typedef NoMotion::Pointer
   * Smart pointer associated with a NoMotion object.
   */

  /**
   * @typedef NoMotion::ConstPointer
   * Smart pointer associated with a const NoMotion object.
   */

  /**
   * @fn NoMotion::Pointer NoMotion::New()
   * Creates a smart pointer associated with a NoMotion object.
   */
   
  /**
   * @fn virtual Frame& NoMotion::GetFrame(int idx)
   * Returns the identity matrix.
   */
  
  /**
   * @fn virtual const Frame& NoMotion::GetFrame(int idx) const
   * Returns the identity matrix.
   */
  
  /**
   * @fn virtual Frame& NoMotion::operator[](int idx)
   * Returns the identity matrix.
   */
  
  /**
   * @fn virtual const Frame& NoMotion::operator[](int idx) const
   * Returns the identity matrix.
   */
  
  /**
   * @fn NoMotion::NoMotion()
   * Constructor.
   */
  
  /**
   * @fn NoMotion::NoMotion(const NoMotion& toCopy)
   * Copy constructor.
   */
  
  /**
   * @class Motion
   * @brief Container of frames used to represent the motion of a rigid body.
   *
   * @ingroup BTKCommon
   */

  /**
   * @typedef Motion::Pointer
   * Smart pointer associated with a Motion object.
   */

  /**
   * @typedef Motion::ConstPointer
   * Smart pointer associated with a const Motion object.
   */

  /**
   * @typedef Motion::Iterator
   * Iterator for frames contained in the Motion object.
   */

  /**
   * @typedef Motion::ConstIterator
   * Const iterator for frames contained in the Motion object.
   */

  /**
   * @fn Motion::Pointer Motion::New()
   * Creates a smart pointer associated with a Motion object.
   */
   
  /**
   * @fn Motion::Iterator Motion::Begin()
   * Returns an iterator to the beginning of the list of segments.
   */
  
  /**
   * @fn Motion::ConstIterator Motion::Begin() const
   * Returns a const iterator to the beginning of the list of segments.
   */
  
  /**
   * @fn Motion::Iterator Motion::End()
   * Returns an iterator just past the last segment.
   */
   
  /**
   * @fn Motion::ConstIterator Motion::End() const
   * Returns a const iterator just past the last segment.
   */
  
  /**
   * Returns the frame at the index @a idx.
   */
  Frame& Motion::GetFrame(int idx)
  {
    if (idx >= this->GetFrameNumber())
      throw(OutOfRangeException("Motion::GetFrame"));
    return this->m_Frames[idx];
  };
  
  /**
   * Returns the frame at the index @a idx.
   */
  const Frame& Motion::GetFrame(int idx) const
  {
    if (idx >= this->GetFrameNumber())
      throw(OutOfRangeException("Motion::GetFrame"));
    return this->m_Frames[idx];
  };
  
  
  /**
   * @fn int Motion::GetFrameNumber() const
   * Gets the number of frames.
   */
  
  /**
   * Sets the number of frames.
   */
  void Motion::SetFrameNumber(int num)
  {
    if (num == this->GetFrameNumber())
      return;
    this->m_Frames.resize(num);
    this->Modified();
  };
  
  /**
   * @fn Motion::Pointer Motion::Clone() const
   * Returns a deep copy of the object as a smart pointer.
   */
  
  /**
   * @fn virtual Frame& Motion::operator[](int idx)
   * Returns the frame at index @a idx without any assertion.
   */
  
  /**
   * @fn virtual const Frame& Motion::operator[](int idx) const
   * Returns the frame at index @a idx without any assertion.
   */
  
  /**
   * Constructor.
   */
  Motion::Motion()
  : AbstractMotion(), m_Frames()
  {};
  
  /**
   * Copy constructor.
   */
  Motion::Motion(const Motion& toCopy)
  : AbstractMotion(toCopy), m_Frames(toCopy.m_Frames)
  {};
};
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

#include "btkSubject.h"

namespace btk
{
  /**
   * @class Subject
   * @brief Represents a wrench has position, force and moment.
   *
   * @ingroup BTKCommon
   */
   
  /**
   * @enum Subject::Sex
   * Enumeration for the suject's sex.
   */
  /**
   * @var Subject::Sex Subject::Male
   * Used for Male.
   */
  /**
   * @var Subject::Sex Subject::Female
   * Used for Female.
   */

  /**
   * @typedef Subject::Pointer
   * Smart pointer associated with a Wrench object.
   */

  /**
   * @typedef Subject::ConstPointer
   * Smart pointer associated with a const Wrench object.
   */
   
  /**
   * @fn static Pointer Subject::New()
   * @brief Creates a smart pointer associated with a Wrench object.
   */
  
  /**
   * @fn Sex Subject::GetSex() const
   * Gets the subject's sex.
   */
  
  /**
   * Sets the subject's sex.
   */
  void Subject::SetSex(Sex s)
  {
    if (this->m_Sex == s)
      return;
    this->m_Sex = s;
    this->Modified();
  };
  
  /**
   * @fn double Subject::GetHeight() const
   * Gets subject's height.
   */
  
  /**
   * Sets subject's height.
   */
  void Subject::SetHeight(double h)
  {
    if (this->m_Height == h)
      return;
    this->m_Height = h;
    this->Modified();
  };
  
  /**
   * @fn double Subject::GetWeight() const
   * Gets subject's weight.
   */
  
  /**
   * Sets subject's weight.
   */
  void Subject::SetWeight(double w)
  {
    if (this->m_Weight == w)
      return;
    this->m_Weight = w;
    this->Modified();
  };
  
  /**
   * @fn Pointer Subject::Clone() const
   * Returns a deep copy of the object as a smart pointer.
   */

  /**
   * Constructor
   */
  Subject::Subject()
  : DataObject()
  {
    this->m_Sex = Male;
    this->m_Height = 0.0;
    this->m_Weight = 0.0;
  };

  Subject::Subject(const Subject& toCopy)
  : DataObject(toCopy)
  {
    this->m_Sex = toCopy.m_Sex;
    this->m_Height = toCopy.m_Height;
    this->m_Weight = toCopy.m_Weight;
  };
};
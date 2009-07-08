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

#include "btkObject.h"

#ifdef _MSC_VER
  #include "../../Utilities/timeval.h"
#else
  #include <sys/time.h>
#endif

namespace btk
{
  /**
   * @class Object
   * @brief Base for all objects which need to keep track of modified time.
   *
   * @ingroup BTKCommon
   */
  /**
   * @var Object::m_Timestamp
   * Time in microseconds to indicate the last modification of the object.
   */
  
  /**
   * @typedef Object::Pointer
   * Smart pointer associated with a Object object.
   */
  
  /**
   * @typedef Object::ConstPointer
   * Smart pointer associated with a const Object object.
   */
  
  /**
   * @fn long Object::GetTimestamp() const
   * Returns the timestamp of the object.
   */
  
  /**
   * Sets the object as modified (its timestamp is updated and the flag 'modified' 
   * is set to true). It is important to use this method each time a component 
   * of the process is modified.
   */
  void Object::Modified()
  {
    struct timeval t;
    gettimeofday(&t, NULL);
    this->m_Timestamp = t.tv_sec * 1000000 + t.tv_usec;
  };
  
  /**
   * @fn Object::Object()
   * Constructor.
   */

  /**
   * @fn Object::Object(const Object& toCopy)
   * Copy constructor.
   */
  
  /**
   * @fn Object::~Object()
   * Destructor.
   */
};

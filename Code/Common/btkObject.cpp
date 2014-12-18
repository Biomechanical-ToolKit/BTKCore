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

#include "btkObject.h"
#include "btkObject_p.h"

#include <atomic>

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

namespace btk
{
  ObjectPrivate::ObjectPrivate()
  : Timestamp(0)
  {};
  
  ObjectPrivate::~ObjectPrivate() = default; // Cannot be inlined
}

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

namespace btk
{
  /**
   * @class Object btkObject.h
   * @brief Base for all objects which need to keep track of modified time.
   *
   * @ingroup BTKCommon
   */
  
  /**
   * Destructor
   * @note The opaque pointer representing the private implementation is automatically deleted as it is contained in a std::unique_ptr object.
   */
  Object::~Object() noexcept = default;
  
  /**
   * Returns the timestamp of the object.
   */
  unsigned long Object::timestamp() const noexcept
  {
    auto optr = this->pimpl();
    return optr->Timestamp;
  };
  
  /**
   * Sets the object as modified (its timestamp is updated).
   * It is important to use this method each time a member of the object is modified.
   */
  void Object::modified() noexcept
  {
    auto optr = this->pimpl();
    static std::atomic<unsigned long> _btk_atomic_time{0};
    optr->Timestamp = ++_btk_atomic_time;
  };
  
  /**
   * Constructor.
   * Initialize the timestamp to 0
   */
  Object::Object()
  : mp_Pimpl(new ObjectPrivate)
  {};
  
  Object::Object(ObjectPrivate& impl) noexcept
  : mp_Pimpl(&impl)
  {};
};

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

#include "btkDataObject.h"
#include "btkProcessObject.h"

namespace btk
{
  /**
   * @class DataObject
   * @brief Input and output entry for processes in pipelines.
   *
   * @ingroup BTKCommon
   */
  /**
   * @var DataObject::mp_Parent
   * Optional parent for complex object which need to know if children has been modified.
   */
  
  /**
   * @typedef DataObject::Pointer
   * Smart pointer associated with a DataObject object.
   */
  
  /**
   * @typedef DataObject::ConstPointer
   * Smart pointer associated with a const DataObject object.
   */
  
  /**
   * @fn bool DataObject::HasParent() const
   * Checks if this DataObject has a parent.
   */
  
  /**
   * @fn DataObject* DataObject::GetParent() const
   * Return the object's parent.
   */
     
  /**
   * Sets the parent.
   */
  void DataObject::SetParent(DataObject* parent)
  {
    if (parent == this->mp_Parent)
      return;
    this->mp_Parent = parent;
    this->Modified();
  };
  
  /**
   * Updates the ProcessObject associated with this DataObject.
   */
  void DataObject::Update()
  {
    if (this->mp_Source)
      this->mp_Source->Update();
  };
  
  /**
   * @fn DataObject::DataObject()
   * Default constructor.
   */
  
  /**
   * @fn DataObject::~DataObject()
   * Destructor (do nothing).
   */
  
  /**
   * Tells to its parent that this DataObject has been modified.
   * This method has to be called each time that the DataObject is modified.
   */
  void DataObject::Modified()
  {
    this->Object::Modified();
    if (this->mp_Parent)
      this->mp_Parent->Modified();
  };
};

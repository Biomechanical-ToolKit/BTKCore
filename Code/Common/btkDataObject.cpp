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

#include "btkDataObject.h"
#include "btkProcessObject.h"

namespace btk
{
  /**
   * @class DataObject btkDataObject.h
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
    else if (parent == this)
    {
      btkErrorMacro("Impossible to set itself as its parent.");
      return;
    }
    if (this->mp_Parent != 0) this->mp_Parent->RemoveChild(this);
    if (parent != 0) parent->AddChild(this);
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
   * @fn DataObject::DataObject(const DataObject& toCopy)
   * Copy constructor.
   */
  
  /**
   * @fn DataObject::~DataObject()
   * Destructor.
   */
  DataObject::~DataObject()
  {
    if (this->mp_Parent != 0)
      this->mp_Parent->RemoveChild(this);
    for (std::list<DataObject*>::iterator it = this->m_Children.begin() ; it != this->m_Children.end() ; ++it)
      (*it)->mp_Parent = 0;
    this->m_Children.clear();
  };
  
  /**
   * Tells to its parent that this DataObject has been modified.
   * This method has to be called each time that the DataObject is modified.
   */
  void DataObject::Modified()
  {
    this->Object::Modified();
    if (this->mp_Parent != 0)
      this->mp_Parent->Modified();
  };
  
  void DataObject::AddChild(DataObject* child)
  {
    for (std::list<DataObject*>::iterator it = this->m_Children.begin() ; it != this->m_Children.end() ; ++it)
    {
      if (*it == child)
        return;
    }
    child->mp_Parent = this;
    this->m_Children.push_back(child);
    this->Modified();
  };
  
  void DataObject::RemoveChild(DataObject* child)
  {
    for (std::list<DataObject*>::iterator it = this->m_Children.begin() ; it != this->m_Children.end() ; ++it)
    {
      if (*it == child)
      {
        child->mp_Parent = 0;
        this->m_Children.erase(it);
        this->Modified();
        break;
      }
    }
  };
  
  /**
   * @class DataObjectLabeled btkDataObject.h
   * @brief DataObject with a label and a description.
   *
   * @ingroup BTKCommon
   */
  /**
   * @var DataObjectLabeled::m_Label
   * Label of the object.
   */
  /**
   * @var DataObjectLabeled::m_Description
   * Description associated with the object.
   */
  
  /**
   * @typedef DataObjectLabeled::Pointer
   * Smart pointer associated with a DataObjectLabeled object.
   */
  
  /**
   * @typedef DataObjectLabeled::ConstPointer
   * Smart pointer associated with a const DataObjectLabeled object.
   */
   
  /**
   * @fn DataObjectLabeled::DataObjectLabeled(const std::string& label = "", const std::string& description = "")
   * Default constructor.
   */

  /**
   * @fn DataObjectLabeled::DataObjectLabeled(const DataObjectLabeled& toCopy)
   * Copy constructor.
   */
  
  /**
   * @fn const std::string& DataObjectLabeled::GetLabel() const
   * Returns the object's label.
   */
  
  /**
   * Sets the label.
   */
  void DataObjectLabeled::SetLabel(const std::string& label)
  {
    if (this->m_Label.compare(label) == 0)
      return;
    this->m_Label = label;
    this->Modified();
  };
  
  /**
   * @fn const std::string& DataObjectLabeled::GetDescription() const
   * Returns the description.
   */

  /**
   * Sets the description.
   */
  void DataObjectLabeled::SetDescription(const std::string& description)
  {
    if (this->m_Description.compare(description) == 0)
      return;
    this->m_Description = description;
    this->Modified();
  };
};

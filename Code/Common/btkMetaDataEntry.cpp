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
 *     * Neither the label(s) of the copyright holders nor the labels
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

#include "btkMetaDataEntry.h"
#include "btkException.h"
#include "btkConvert.h"

#include <iostream>

namespace btk
{
  /**
   * @class MetaDataEntry
   * @brief Store data which can be embedded within an Acquisition 
   * (calibration matrix, subject informations, ...).
   *
   * @ingroup BTKCommon
   */
  
  /**
   * @typedef MetaDataEntry::Pointer
   * Smart pointer associated with a MetaDataEntry object.
   */
  
  /**
   * @typedef MetaDataEntry::ConstPointer
   * Smart  pointer associated with a const MetaDataEntry object.
   */
  
  /**
   * @typedef MetaDataEntry::Iterator
   * Iterator related to the children of the entry.
   */
  
  /**
   * @typedef MetaDataEntry::ConstIterator
   * Const Iterator related to the children of the entry.
   */

	/**
	 * Collapse the @a parent children entries' values starting with the string @a basename
	 * and incrementing (for example: LABELS, LABELS2, LABELS3). The entries'
	 * values are stored in @a target.
	 *
	 * The input @a targetFinalSize can be used to fox the number of values 
	 * to collpase (by default: -1). The input @a blankReplacement can be used
	 * to fill the @a target' values which have no corresponding in the @a parent (by default: ""). 
	 */
	void MetaDataEntry::CollapseChildrenValues(std::vector<std::string>& target,
																			 			 MetaDataEntry::ConstPointer parent,
                           						 			 const std::string& basename,
																			 			 int targetFinalSize,
                           						 			 const std::string& blankReplacement)
  {
		target.clear();
    int collapsedNumber = 0; int inc = 2;
    std::string name = basename;
		if (parent.get() != 0)
		{
      while (1)
      {
        MetaDataEntry::ConstIterator it = parent->Find(name);
        if (it == parent->End())
          break;
        std::vector<std::string> temp = (*it)->GetMetaDataEntryValue()->GetValues();
        for (std::vector<std::string>::const_iterator it = temp.begin() ; it != temp.end() ; ++it)
        {
          if (target.size() == targetFinalSize)
            break;
          std::string str = *it;
          str = str.erase(str.find_last_not_of(' ') + 1);
          str = str.erase(0, str.find_first_not_of(' '));
          target.push_back(str);
        }
        collapsedNumber = target.size();
        if (collapsedNumber == targetFinalSize)
          break;
        name = basename + ToString(inc);
      }
		}
    if ((collapsedNumber < targetFinalSize) && (!blankReplacement.empty()))
    {
      target.resize(targetFinalSize);
      for (int inc = collapsedNumber ; inc < targetFinalSize ; ++inc)
        target[inc] = blankReplacement + ToString(inc + 1);
    }
  };
  
  /**
	 * @fn MetaDataEntry::Pointer MetaDataEntry::New(const std::string& name, const std::string& desc, bool isUnlocked)
   * Creates a smart pointer from the MetaDataEntry(const std::string&, const std::string&, bool) constructor.
   */
	
	/**
	 * @fn MetaDataEntry::Pointer MetaDataEntry::New(const std::string& name, int8_t val, const std::string& desc, bool isUnlocked)
   * Creates a smart pointer from the MetaDataEntry(const std::string&, int8_t, const std::string&, bool) constructor.
   */

	/**
	 * @fn MetaDataEntry::Pointer MetaDataEntry::New(const std::string& name, int16_t val, const std::string& desc, bool isUnlocked)
   * Creates a smart pointer from the MetaDataEntry(const std::string&, int16_t, const std::string&, bool) constructor.
   */

	/**
	 * @fn MetaDataEntry::Pointer MetaDataEntry::New(const std::string& name, float val, const std::string& desc, bool isUnlocked)
   * Creates a smart pointer from the MetaDataEntry(const std::string&, float, const std::string&, bool) constructor.
   */

	/**
	 * @fn MetaDataEntry::Pointer MetaDataEntry::New(const std::string& name, std::string val,const std::string& desc, bool isUnlocked)
   * Creates a smart pointer from the MetaDataEntry(const std::string&, const std::string&, const std::string&, bool) constructor.
   */

	/**
	 * @fn MetaDataEntry::Pointer MetaDataEntry::New(const std::string& name, const std::vector<uint8_t>& dim, const std::vector<int8_t>& val, const std::string& desc, bool isUnlocked)
   * Creates a smart pointer from the MetaDataEntry(const std::string&, const std::vector<uint8_t>&, const std::vector<int8_t>&, const std::string&, bool) constructor.
   */

	/**
	 * @fn MetaDataEntry::Pointer MetaDataEntry::New(const std::string& name, const std::vector<uint8_t>& dim, const std::vector<int16_t>& val, const std::string& desc, bool isUnlocked)
   * Creates a smart pointer from the MetaDataEntry(const std::string&, const std::vector<uint8_t>&, const std::vector<int16_t>&, const std::string&, bool) constructor.
   */

	/**
   * @fn  MetaDataEntry::Pointer MetaDataEntry::New(const std::string& name, const std::vector<uint8_t>& dim, const std::vector<float>& val, const std::string& desc, bool isUnlocked)
	 * Creates a smart pointer from the MetaDataEntry(const std::string&, const std::vector<uint8_t>&, const std::vector<float>&, const std::string&, bool) constructor.
   */

	/**
	 * @fn MetaDataEntry::Pointer MetaDataEntry::New(const std::string& name, const std::vector<uint8_t>& dim, const std::vector<std::string>& val, const std::string& desc, bool isUnlocked)
   * Creates a smart pointer from the MetaDataEntry(const std::string&, const std::vector<uint8_t>&, const std::vector<std::string>&, const std::string&, bool) constructor.
   */

	/**
   * @fn const std::string& MetaDataEntry::GetLabel() const
   * Returns the label of the entry.
   */
  
  /**
   * Sets the label of the entry. If @a label already exist in the parent's list,
   * then an exception is thrown.
   */
  void MetaDataEntry::SetLabel(const std::string& label)
  {
    MetaDataEntry* parent = 0;
    if (this->m_HasMetaParent)
      parent = static_cast<MetaDataEntry*>(this->DataObject::GetParent());
    
    if (parent)
    {
      if (parent->Find(label) != parent->End())
        throw(DomainError("MetaDataEntry::SetLabel"));
    }
    this->m_Label = label;
  };
  
  /**
   * @fn const std::string& MetaDataEntry::GetDescription() const
   * Gets the description of the entry.
   */
  
  /**
   * @fn void MetaDataEntry::SetDescription(const std::string& desc)
   * Sets the description of the entry.
   */
  
  /**
   * @fn bool MetaDataEntry::GetUnlockState(void) const
   * Gets the unlock state of the entry.
   */
  
  /**
   * @fn void MetaDataEntry::SetUnlockState(bool isUnlocked)
   * Sets the unlock state of the entry.
   */
  
  /**
   * @fn MetaDataEntryValue::Pointer MetaDataEntry::GetMetaDataEntryValue()
   * Gets the MetaDataEntryValue associated to the entry.
   */
  
  /**
   * @fn const MetaDataEntryValue::Pointer MetaDataEntry::GetMetaDataEntryValue() const
   * Gets the MetaDataEntryValue associated to the entry.
   */
  
  /**
   * @fn void MetaDataEntry::SetMetaDataEntryValue(MetaDataEntryValue::Pointer val)
   * Sets the MetaDataEntryValue associated to the entry.
   */
  
  /**
   * @fn bool MetaDataEntry::HasMetaDataEntryValue() const
   * Checks if a MetaDataEntryValue is associated with the entry.
   */
  
  /**
   * @fn bool MetaDataEntry::HasMetaParent() const
   * Checks if this object has MetaDataEntry object as parents.
   */
  
  /**
   * @fn void MetaDataEntry::SetParent(MetaDataEntry* parent = 0)
   * Sets the parent of the entry.
   */
  
 /**
   * @fn MetaDataEntry::Iterator MetaDataEntry::Begin()
   * Returns an iterator to the beginning of the list of children.
   */
  
  /**
   * @fn MetaDataEntry::ConstIterator MetaDataEntry::Begin() const
   * Returns a const iterator to the beginning of the list of children.
   */
  
  /**
   * @fn MetaDataEntry::Iterator MetaDataEntry::End()
   * Returns an iterator just past the last child.
   */
  
  /**
   * @fn MetaDataEntry::ConstIterator MetaDataEntry::End() const
   * Returns a const iterator just past the last child.
   */
  
  /**
   * Gets the entry at the specified @a idx or throw an exception if the index is out of range.
   */
  MetaDataEntry::Pointer MetaDataEntry::GetChild(int idx)
  {
    if (idx >= static_cast<int>(this->m_Children.size()))
      throw(OutOfRangeException("MetaDataEntry::GetChild"));
    MetaDataEntry::Iterator it = this->Begin();
    std::advance(it, idx);
    return *it;
  };
  
  /**
   * Gets the entry at the specified @a idx or throw an exception if the index is out of range.
   */
  MetaDataEntry::ConstPointer MetaDataEntry::GetChild(int idx) const
  {
    if (idx >= static_cast<int>(this->m_Children.size()))
      throw(OutOfRangeException("MetaDataEntry::GetChild"));
    MetaDataEntry::ConstIterator it = this->Begin();
    std::advance(it, idx);
    return *it;
  };
  
  /**
   * Gets the entry which this @a label, or throw an exception if no entry is associated.
   */
  MetaDataEntry::Pointer MetaDataEntry::GetChild(const std::string& label)
  {
    Iterator it = this->Find(label);
    if (it == this->End())
      throw(DomainError("MetaDataEntry::GetChild"));
    return *it;
  };
  
  /**
   * Gets the entry which this @a label, or throw an exception if no entry is associated.
   */
  MetaDataEntry::ConstPointer MetaDataEntry::GetChild(const std::string& label) const
  {
    ConstIterator it = this->Find(label);
    if (it == this->End())
      throw(DomainError("MetaDataEntry::GetChild"));
    return *it;
  };
  
  /**
   * Append the child @a entry.
   */
  bool MetaDataEntry::AppendChild(MetaDataEntry::Pointer entry)
  {
    return this->InsertChild(this->End(), entry);
    this->Modified();
  };
  
  /**
   * Insert the child @a entry at the specified location given by the iterator @a loc.
   * If @a entry has a label which already exists in the parent's list, then it is not inserted.
   * @return True if child has been inserted or false.
   */
  bool MetaDataEntry::InsertChild(Iterator loc, MetaDataEntry::Pointer entry)
  {
    if (entry.get() == 0)
    {
      btkErrorMacro("Impossible to insert an empty entry");
      return false;
    }
    if (this->Find(entry->GetLabel()) != this->End())
    {
      btkErrorMacro("Label '" + entry->GetLabel() + "' already exists in the entries' list");
      return false;
    }
    entry->SetParent(this);
    this->m_Children.insert(loc, entry);
    this->Modified();
    return true;
  }
  
  /**
   * Insert the child @a entry at the specified @a idx
   */
  bool MetaDataEntry::InsertChild(int idx, MetaDataEntry::Pointer entry)
  {
    Iterator it = this->Begin();
    if (idx > static_cast<int>(this->m_Children.size()))
    {
      btkErrorMacro("Out of range, the entry is appended");
      it = this->End();
    }
    else
      std::advance(it, idx);
    return this->InsertChild(it, entry);
  };
  
  /**
   * Sets the child @a entry at the specified @a idx. 
   */
  void MetaDataEntry::SetChild(int idx, MetaDataEntry::Pointer entry)
  {
    if (entry.get() == 0)
    {
      btkErrorMacro("Impossible to set an empty entry");
      return;
    }
    if (idx >= static_cast<int>(this->m_Children.size()))
    {
      btkErrorMacro("Out of range");
      return;
    }
    if (this->Find(entry->GetLabel()) != this->End())
    {
      btkErrorMacro("Label '" + entry->GetLabel() + "' already exists in the entries' list");
      return;
    }
    Iterator it = this->Begin();
    std::advance(it, idx);
    *it = entry;
    this->Modified();
  };
  
  /**
   * Removes the child entry at the location given by the iterator @a loc and return it. 
   * If there is no child at the given location, then an empty Pointer is returned.
   */
  MetaDataEntry::Pointer MetaDataEntry::TakeChild(Iterator loc)
  {
    if (loc == this->End())
    {
      btkErrorMacro("Out of range");
      return Pointer();
    }
    Pointer entry = *loc;
    this->m_Children.erase(loc);
    this->Modified();
    return entry;
  };
  
  /**
   * Removes the child entry at @a idx and return it. 
   * If there is no child at the given index, then an empty Pointer is returned.
   */
  MetaDataEntry::Pointer MetaDataEntry::TakeChild(int idx)
  {
    if (idx >= static_cast<int>(this->m_Children.size()))
    {
      btkErrorMacro("Out of range");
      return Pointer();
    }
    Iterator it = this->Begin();
    std::advance(it, idx);
    Pointer entry = *it;
    this->m_Children.erase(it);
    this->Modified();
    return entry;
  };
  
  /**
   * Removes the child entry which has this @a label and return it. 
   * If there is no child with the specified @a label, then an empty Pointer is returned.
   */
  MetaDataEntry::Pointer MetaDataEntry::TakeChild(const std::string& label)
  {
    Iterator it = this->Find(label);
    if (it == this->End())
    {
      btkErrorMacro("No Child with the label: " + label);
      return Pointer();
    }
    Pointer entry = *it;
    this->m_Children.erase(it);
    this->Modified();
    return entry;
  };
  
  /**
   * @fn bool MetaDataEntry::HasChildren() const
   * Checks if the entry has children
   */
  
  /**
   * @fn int MetaDataEntry::GetChildNumber() const
   * Returns the number of children
   */
  
  /**
   * Finds the children which has the label @a label and return it as an Iterator
   */
  MetaDataEntry::Iterator MetaDataEntry::Find(const std::string& label)
  {
    Iterator it = this->Begin();
    while (it != this->End())
    {
      if ((*it)->GetLabel().compare(label) == 0)
        break;
      ++it;
    }
    return it;
  };
  
  /**
   * Finds the children which has the label @a label and return it as a ConstIterator
   */
  MetaDataEntry::ConstIterator MetaDataEntry::Find(const std::string& label) const
  {
    ConstIterator it = this->Begin();
    while (it != this->End())
    {
      if ((*it)->GetLabel().compare(label) == 0)
        break;
      ++it;
    }
    return it;
  };
  
  /**
   * Returns a deep copy of the object as a smart pointer. 
   * The clone doesn't copy the parent's value
   */
  MetaDataEntry::Pointer MetaDataEntry::Clone() const
  {
    Pointer pt(new MetaDataEntry(this->GetLabel(), this->GetDescription(), this->GetUnlockState()));
    MetaDataEntryValue::ConstPointer temp = this->GetMetaDataEntryValue();
    if (temp.get() != 0)
      pt->SetMetaDataEntryValue(temp->Clone());
    for (ConstIterator it = this->Begin() ; it != this->End() ; ++it)
      pt->AppendChild((*it)->Clone());
    return pt;
  };
  
  /**
   * Constructor for an entry without a MetaDataEntryValue;
   */
  MetaDataEntry::MetaDataEntry(const std::string& label,
                               const std::string& desc, bool isUnlocked)
  : DataObject(), m_Label(label), m_Description(desc),
  m_Value(MetaDataEntryValue::Pointer()),
  m_Children(std::list<MetaDataEntry::Pointer>(0))
  {
    this->m_Unlocked = isUnlocked;
  };
  
  /**
   * Constructor for an entry containing a MetaDataEntryValue with a single BYTE
   */
  MetaDataEntry::MetaDataEntry(const std::string& label, int8_t val,
                               const std::string& desc, bool isUnlocked)
  : DataObject(), m_Label(label), m_Description(desc),
  m_Value(MetaDataEntryValue::New(val)),
  m_Children(std::list<MetaDataEntry::Pointer>(0))
  {
    this->m_Unlocked = isUnlocked;
  };
  
  /**
   * Constructor for an entry containing a MetaDataEntryValue with a single INTEGER
   */
  MetaDataEntry::MetaDataEntry(const std::string& label, int16_t val, 
                               const std::string& desc, bool isUnlocked)
  : DataObject(), m_Label(label), m_Description(desc),
  m_Value(MetaDataEntryValue::New(val)),
  m_Children(std::list<MetaDataEntry::Pointer>(0))
  {
    this->m_Unlocked = isUnlocked;
  };
  
  /**
   * Constructor for an entry containing a MetaDataEntryValue with a single FLOAT
   */
  MetaDataEntry::MetaDataEntry(const std::string& label, float val, 
                               const std::string& desc, bool isUnlocked)
  : DataObject(), m_Label(label), m_Description(desc),
  m_Value(MetaDataEntryValue::New(val)),
  m_Children(std::list<MetaDataEntry::Pointer>(0))
  {
    this->m_Unlocked = isUnlocked;
  };
  
  /**
   * Constructor for an entry containing a MetaDataEntryValue with a single string
   */
  MetaDataEntry::MetaDataEntry(const std::string& label, std::string val, 
                               const std::string& desc, bool isUnlocked)
  : DataObject(), m_Label(label), m_Description(desc),
  m_Value(MetaDataEntryValue::New(val)),
  m_Children(std::list<MetaDataEntry::Pointer>(0))
  {
    this->m_Unlocked = isUnlocked;
  };
  
  /**
   * Constructor for an entry containing a MetaDataEntryValue of BYTE values
   */
  MetaDataEntry::MetaDataEntry(const std::string& label, const std::vector<uint8_t>& dim, 
                               const std::vector<int8_t>& val, const std::string& desc,
                               bool isUnlocked)
  : DataObject(), m_Label(label), m_Description(desc),
  m_Value(MetaDataEntryValue::New(dim, val)),
  m_Children(std::list<MetaDataEntry::Pointer>(0))
  {
    this->m_Unlocked = isUnlocked;
  };
  
  /**
   * Constructor for an entry containing a MetaDataEntryValue of INTEGER values
   */
  MetaDataEntry::MetaDataEntry(const std::string& label, const std::vector<uint8_t>& dim, 
                               const std::vector<int16_t>& val, const std::string& desc,
                               bool isUnlocked)
  : DataObject(), m_Label(label), m_Description(desc),
  m_Value(MetaDataEntryValue::New(dim, val)),
  m_Children(std::list<MetaDataEntry::Pointer>(0))
  {
    this->m_Unlocked = isUnlocked;
  };
  
  /**
   * Constructor for an entry containing a MetaDataEntryValue of FLOAT values
   */
  MetaDataEntry::MetaDataEntry(const std::string& label, const std::vector<uint8_t>& dim, 
                               const std::vector<float>& val, const std::string& desc,
                               bool isUnlocked)
  : DataObject(), m_Label(label), m_Description(desc),
  m_Value(MetaDataEntryValue::New(dim, val)),
  m_Children(std::list<MetaDataEntry::Pointer>(0))
  {
    this->m_Unlocked = isUnlocked;
  };
  
  /**
   * Constructor for an entry containing a MetaDataEntryValue of string values
   */
  MetaDataEntry::MetaDataEntry(const std::string& label, const std::vector<uint8_t>& dim, 
                               const std::vector<std::string>& val, const std::string& desc,
                               bool isUnlocked)
  : DataObject(), m_Label(label), m_Description(desc),
  m_Value(MetaDataEntryValue::New(dim, val)),
  m_Children(std::list<MetaDataEntry::Pointer>(0))
  {
    this->m_Unlocked = isUnlocked;
  };
}

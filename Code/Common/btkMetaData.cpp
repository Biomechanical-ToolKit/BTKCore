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

#include "btkMetaData.h"
#include "btkException.h"
#include "btkConvert.h"

#include <iostream>

namespace btk
{
  /**
   * @class MetaData btkMetaData.h
   * @brief Store data which can be embedded within an Acquisition 
   * (calibration matrix, subject informations, ...).
   *
   * @sa MetaDataCollapseChildrenValues, MetaDataCreateChild (located in btkMetaDataUtils.h) to create or collapse Metadata objects. 
   *
   * @ingroup BTKCommon
   */
  
  /**
   * @typedef MetaData::Pointer
   * Smart pointer associated with a MetaData object.
   */
  
  /**
   * @typedef MetaData::ConstPointer
   * Smart  pointer associated with a const MetaData object.
   */
  
  /**
   * @typedef MetaData::Iterator
   * Iterator related to the children of the entry.
   */
  
  /**
   * @typedef MetaData::ConstIterator
   * Const Iterator related to the children of the entry.
   */

  /**
   * @fn MetaData::Pointer MetaData::New(const std::string& label, const std::string& desc, bool isUnlocked)
   * Creates a smart pointer from the MetaData(const std::string&, const std::string&, bool) constructor.
   */
  
  /**
   * @fn MetaData::Pointer MetaData::New(const std::string& label, int8_t val, const std::string& desc, bool isUnlocked)
   * Creates a smart pointer from the MetaData(const std::string&, int8_t, const std::string&, bool) constructor.
   */

  /**
   * @fn MetaData::Pointer MetaData::New(const std::string& label, int16_t val, const std::string& desc, bool isUnlocked)
   * Creates a smart pointer from the MetaData(const std::string&, int16_t, const std::string&, bool) constructor.
   */

  /**
   * @fn MetaData::Pointer MetaData::New(const std::string& label, float val, const std::string& desc, bool isUnlocked)
   * Creates a smart pointer from the MetaData(const std::string&, float, const std::string&, bool) constructor.
   */

  /**
   * @fn MetaData::Pointer MetaData::New(const std::string& label, const std::string& val,const std::string& desc, bool isUnlocked)
   * Creates a smart pointer from the MetaData(const std::string&, const std::string&, const std::string&, bool) constructor.
   */

  /**
   * @fn MetaData::Pointer MetaData::New(const std::string& label,  const std::vector<int8_t>& val, const std::string& desc, bool isUnlocked)
   * Creates a smart pointer from the MetaData(const std::string&, const std::vector<int8_t>&, const std::string&, bool) constructor.
   */

  /**
   * @fn MetaData::Pointer MetaData::New(const std::string& label, const std::vector<int16_t>& val, const std::string& desc, bool isUnlocked)
   * Creates a smart pointer from the MetaData(const std::string&, const std::vector<int16_t>&, const std::string&, bool) constructor.
   */

  /**
   * @fn  MetaData::Pointer MetaData::New(const std::string& label, const std::vector<float>& val, const std::string& desc, bool isUnlocked)
   * Creates a smart pointer from the MetaData(const std::string&, const std::vector<float>&, const std::string&, bool) constructor.
   */

  /**
   * @fn MetaData::Pointer MetaData::New(const std::string& label, const std::vector<std::string>& val, const std::string& desc, bool isUnlocked)
   * Creates a smart pointer from the MetaData(const std::string&, const std::vector<std::string>&, const std::string&, bool) constructor.
   */

  /**
   * @fn MetaData::Pointer MetaData::New(const std::string& label, const std::vector<uint8_t>& dim, const std::vector<int8_t>& val, const std::string& desc, bool isUnlocked)
   * Creates a smart pointer from the MetaData(const std::string&, const std::vector<uint8_t>&, const std::vector<int8_t>&, const std::string&, bool) constructor.
   */

  /**
   * @fn MetaData::Pointer MetaData::New(const std::string& label, const std::vector<uint8_t>& dim, const std::vector<int16_t>& val, const std::string& desc, bool isUnlocked)
   * Creates a smart pointer from the MetaData(const std::string&, const std::vector<uint8_t>&, const std::vector<int16_t>&, const std::string&, bool) constructor.
   */

  /**
   * @fn  MetaData::Pointer MetaData::New(const std::string& label, const std::vector<uint8_t>& dim, const std::vector<float>& val, const std::string& desc, bool isUnlocked)
   * Creates a smart pointer from the MetaData(const std::string&, const std::vector<uint8_t>&, const std::vector<float>&, const std::string&, bool) constructor.
   */

  /**
   * @fn MetaData::Pointer MetaData::New(const std::string& label, const std::vector<uint8_t>& dim, const std::vector<std::string>& val, const std::string& desc, bool isUnlocked)
   * Creates a smart pointer from the MetaData(const std::string&, const std::vector<uint8_t>&, const std::vector<std::string>&, const std::string&, bool) constructor.
   */

  /**
   * Sets the label of the entry. If @a label already exist in the parent's list,
   * then an exception is thrown.
   */
  void MetaData::SetLabel(const std::string& label)
  {
    if (this->m_Label.compare(label) == 0)
      return;
    MetaData* parent = 0;
    if (this->m_MetaDataParentAssigned)
      parent = static_cast<MetaData*>(this->DataObject::GetParent());
    
    if (parent)
    {
      if (parent->FindChild(label) != parent->End())
        throw(DomainError("MetaData::SetLabel"));
    }
    this->m_Label = label;
    this->Modified();
  };
  
  /**
   * @fn bool MetaData::GetUnlockState(void) const
   * Gets the unlock state of the entry.
   */
  
  /**
   * @fn void MetaData::SetUnlockState(bool isUnlocked)
   * Sets the unlock state of the entry.
   */
  
  /**
   * @fn MetaDataInfo::Pointer MetaData::GetInfo()
   * Gets the MetaDataInfo associated to the entry.
   */
  
  /**
   * @fn const MetaDataInfo::Pointer MetaData::GetInfo() const
   * Gets the MetaDataInfo associated to the entry.
   */
  
  /**
   * @fn void MetaData::SetInfo(MetaDataInfo::Pointer val)
   * Sets the MetaDataInfo associated to the entry.
   */
  
  /**
   * @fn bool MetaData::HasInfo() const
   * Checks if a MetaDataInfo is associated with the entry.
   */
  
  /**
   * Check if a child exists with the label @a l and correspond to the given format @a f 
   * plus the number of dimensions @a numDims with or without values (@a noPossibleEmptyValue). The informations of the metadata are extracted in 
   * @a info if all the checks are ok. Otherwise, @a info return an empty MetaDataInfo::Pointer.
   */
  MetaDataInfo::Pointer MetaData::ExtractChildInfo(const std::string& l, MetaDataInfo::Format f, int numDims, bool noPossibleEmptyValue)
  {
    MetaData::Iterator it = this->FindChild(l);
    if (it != this->End())
    {
      if ((*it)->HasInfo())
      {
        MetaDataInfo::Pointer info = (*it)->GetInfo();
        if (info->GetFormat() == f)
        {
          if (static_cast<int>(info->GetDimensions().size()) == numDims)
          {
            if (noPossibleEmptyValue)
            {
              if (info->GetValues().size() != 0)
                return info;
            }
            else
              return info;
          }
        }
      }
    }
    return MetaDataInfo::Pointer();
  }
  
  /**
   * @fn bool MetaData::HasMetaDataAsParent() const
   * Checks if this object has MetaData object as parents.
   */
  
  /**
   * @fn void MetaData::SetParent(DataObject* parent = 0)
   * Sets the parent of the entry.
   */
   
  /**
   * @fn void MetaData::SetParent(MetaData* parent = 0)
   * Sets the parent of the entry.
   */
  
 /**
   * @fn MetaData::Iterator MetaData::Begin()
   * Returns an iterator to the beginning of the list of children.
   */
  
  /**
   * @fn MetaData::ConstIterator MetaData::Begin() const
   * Returns a const iterator to the beginning of the list of children.
   */
  
  /**
   * @fn MetaData::Iterator MetaData::End()
   * Returns an iterator just past the last child.
   */
  
  /**
   * @fn MetaData::ConstIterator MetaData::End() const
   * Returns a const iterator just past the last child.
   */
  
  /**
   * Gets the entry at the specified @a idx or throw an exception if the index is out of range.
   */
  MetaData::Pointer MetaData::GetChild(int idx)
  {
    if (idx >= static_cast<int>(this->m_Children.size()))
      throw(OutOfRangeException("MetaData::GetChild"));
    MetaData::Iterator it = this->Begin();
    std::advance(it, idx);
    return *it;
  };
  
  /**
   * Gets the entry at the specified @a idx or throw an exception if the index is out of range.
   */
  MetaData::ConstPointer MetaData::GetChild(int idx) const
  {
    if (idx >= static_cast<int>(this->m_Children.size()))
      throw(OutOfRangeException("MetaData::GetChild"));
    MetaData::ConstIterator it = this->Begin();
    std::advance(it, idx);
    return *it;
  };
  
  /**
   * Gets the entry which this @a label, or throw an exception if no entry is associated.
   */
  MetaData::Pointer MetaData::GetChild(const std::string& label)
  {
    Iterator it = this->FindChild(label);
    if (it == this->End())
      throw(DomainError("MetaData::GetChild"));
    return *it;
  };
  
  /**
   * Gets the entry which this @a label, or throw an exception if no entry is associated.
   */
  MetaData::ConstPointer MetaData::GetChild(const std::string& label) const
  {
    ConstIterator it = this->FindChild(label);
    if (it == this->End())
      throw(DomainError("MetaData::GetChild"));
    return *it;
  };
  
  /**
   * Append the child @a entry.
   * If @a entry has a label which already exists in the parent's list, then it is not inserted.
    * @return True if child has been inserted or false.
   */
  bool MetaData::AppendChild(MetaData::Pointer entry)
  {
    return this->InsertChild(this->End(), entry);
    this->Modified();
  };
  
  /**
   * Insert the child @a entry at the specified location given by the iterator @a loc.
   * If @a entry has a label which already exists in the parent's list, then it is not inserted.
   * @return True if child has been inserted or false.
   */
  bool MetaData::InsertChild(Iterator loc, MetaData::Pointer entry)
  {
    if (entry.get() == 0)
    {
      btkErrorMacro("Impossible to insert an empty entry");
      return false;
    }
    if (this->FindChild(entry->GetLabel()) != this->End())
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
  bool MetaData::InsertChild(int idx, MetaData::Pointer entry)
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
  void MetaData::SetChild(int idx, MetaData::Pointer entry)
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
    if (this->FindChild(entry->GetLabel()) != this->End())
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
  MetaData::Pointer MetaData::TakeChild(Iterator loc)
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
  MetaData::Pointer MetaData::TakeChild(int idx)
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
  MetaData::Pointer MetaData::TakeChild(const std::string& label)
  {
    Iterator it = this->FindChild(label);
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
   * Removes the child entry at the iterator @a loc.
   */
  MetaData::Iterator MetaData::RemoveChild(Iterator loc)
  {
    if (loc == this->End())
      return this->End();
    Iterator temp = this->m_Children.erase(loc);
    this->Modified();
    return temp;
  };

  /**
   * Removes the child entry at the index @a idx.
   */
  void MetaData::RemoveChild(int idx)
  {
    if (idx >= static_cast<int>(this->m_Children.size()))
      return;
    Iterator it = this->Begin();
    std::advance(it, idx);
    this->m_Children.erase(it);
    this->Modified();
  };

  /**
   * Removes the child entry with the label @a label.
   */
  void MetaData::RemoveChild(const std::string& label)
  {
    Iterator it = this->FindChild(label);
    if (it == this->End())
      return;
    this->m_Children.erase(it);
    this->Modified();
  };
  
  /**
   * Removes every children.
   */ 
  void MetaData::ClearChildren()
  {
    if (this->m_Children.empty())
      return;
    this->m_Children.clear();
    this->Modified();
  };
  
  /**
   * @fn bool MetaData::HasChildren() const
   * Checks if the entry has children
   */
  
  /**
   * @fn int MetaData::GetChildNumber() const
   * Returns the number of children
   */
  
  /**
   * Finds the children which has the label @a label and return it as an Iterator
   */
  MetaData::Iterator MetaData::FindChild(const std::string& label)
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
  MetaData::ConstIterator MetaData::FindChild(const std::string& label) const
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
  MetaData::Pointer MetaData::Clone() const
  {
    Pointer pt(new MetaData(this->GetLabel(), this->GetDescription(), this->GetUnlockState()));
    MetaDataInfo::ConstPointer temp = this->GetInfo();
    if (temp.get() != 0)
      pt->SetInfo(temp->Clone());
    for (ConstIterator it = this->Begin() ; it != this->End() ; ++it)
      pt->AppendChild((*it)->Clone());
    return pt;
  };
  
  /**
   * Equality operator. Doesn't check the parent's value.
   */
  bool operator==(const MetaData& rLHS, const MetaData& rRHS)
  {
    if (rLHS.m_Label.compare(rRHS.m_Label) != 0)
      return false;
    if (rLHS.m_Description.compare(rRHS.m_Description) != 0)
      return false;
    if (rLHS.m_Unlocked != rRHS.m_Unlocked)
      return false;
    if (rLHS.HasInfo() && rRHS.HasInfo())
    {
      if (*(rLHS.m_Info) != *(rRHS.m_Info))
        return false;
    }
    else if (rLHS.HasInfo() != rRHS.HasInfo())
      return false;
    MetaData::ConstIterator itLHS = rLHS.Begin();
    MetaData::ConstIterator itRHS = rRHS.Begin();
    while ((itLHS != rLHS.End()) && (itRHS != rRHS.End()))
    {
      if (*(*itLHS) != *(*itRHS))
        return false;
      ++itLHS; ++itRHS;
    }
    if ((itLHS != rLHS.End()) || (itRHS != rRHS.End()))
      return false;
    
    return true;
  };
  
  /**
   * @fn friend bool MetaData::operator!=(const MetaData& rLHS, const MetaData& rRHS)
   * Inequality operator.
   */
 
  /**
   * Constructor for an entry without a MetaDataInfo.
   */
  MetaData::MetaData(const std::string& label,
                     const std::string& desc, bool isUnlocked)
  : DataObjectLabeled(label, desc),
  m_Info(MetaDataInfo::Pointer()),
  m_Children(std::list<MetaData::Pointer>(0))
  {
    this->m_Unlocked = isUnlocked;
    this->m_MetaDataParentAssigned = false;
  };
  
  /**
   * Constructor for an entry containing a MetaDataInfo with a single BYTE.
   */
  MetaData::MetaData(const std::string& label, int8_t val,
                     const std::string& desc, bool isUnlocked)
  : DataObjectLabeled(label, desc),
  m_Info(MetaDataInfo::New(val)),
  m_Children(std::list<MetaData::Pointer>(0))
  {
    this->m_Unlocked = isUnlocked;
    this->m_MetaDataParentAssigned = false;
  };
  
  /**
   * Constructor for an entry containing a MetaDataInfo with a single INTEGER.
   */
  MetaData::MetaData(const std::string& label, int16_t val, 
                     const std::string& desc, bool isUnlocked)
  : DataObjectLabeled(label, desc),
  m_Info(MetaDataInfo::New(val)),
  m_Children(std::list<MetaData::Pointer>(0))
  {
    this->m_Unlocked = isUnlocked;
    this->m_MetaDataParentAssigned = false;
  };
  
  /**
   * Constructor for an entry containing a MetaDataInfo with a single FLOAT.
   */
  MetaData::MetaData(const std::string& label, float val, 
                     const std::string& desc, bool isUnlocked)
  : DataObjectLabeled(label, desc),
  m_Info(MetaDataInfo::New(val)),
  m_Children(std::list<MetaData::Pointer>(0))
  {
    this->m_Unlocked = isUnlocked;
    this->m_MetaDataParentAssigned = false;
  };
  
  /**
   * Constructor for an entry containing a MetaDataInfo with a single string.
   */
  MetaData::MetaData(const std::string& label, const std::string& val, 
                     const std::string& desc, bool isUnlocked)
  : DataObjectLabeled(label, desc),
  m_Info(MetaDataInfo::New(val)),
  m_Children(std::list<MetaData::Pointer>(0))
  {
    this->m_Unlocked = isUnlocked;
    this->m_MetaDataParentAssigned = false;
  };

  /**
   * Constructor for an entry containing a MetaDataInfo of BYTE valuescorresponding to a 1D vector.
   */
  MetaData::MetaData(const std::string& label,
                     const std::vector<int8_t>& val, const std::string& desc,
                     bool isUnlocked)
  : DataObjectLabeled(label, desc),
  m_Info(MetaDataInfo::New(val)),
  m_Children(std::list<MetaData::Pointer>(0))
  {
    this->m_Unlocked = isUnlocked;
    this->m_MetaDataParentAssigned = false;
  };
  
  /**
   * Constructor for an entry containing a MetaDataInfo of INTEGER values corresponding to a 1D vector.
   */
  MetaData::MetaData(const std::string& label, 
                     const std::vector<int16_t>& val, const std::string& desc,
                     bool isUnlocked)
  : DataObjectLabeled(label, desc),
  m_Info(MetaDataInfo::New(val)),
  m_Children(std::list<MetaData::Pointer>(0))
  {
    this->m_Unlocked = isUnlocked;
    this->m_MetaDataParentAssigned = false;
  };
  
  /**
   * Constructor for an entry containing a MetaDataInfo of FLOAT values corresponding to a 1D vector.
   */
  MetaData::MetaData(const std::string& label,
                     const std::vector<float>& val, const std::string& desc,
                     bool isUnlocked)
  : DataObjectLabeled(label, desc),
  m_Info(MetaDataInfo::New(val)),
  m_Children(std::list<MetaData::Pointer>(0))
  {
    this->m_Unlocked = isUnlocked;
    this->m_MetaDataParentAssigned = false;
  };
  
  /**
   * Constructor for an entry containing a MetaDataInfo of string values corresponding to a 1D vector.
   */
  MetaData::MetaData(const std::string& label,
                     const std::vector<std::string>& val, const std::string& desc,
                     bool isUnlocked)
  : DataObjectLabeled(label, desc),
  m_Info(MetaDataInfo::New(val)),
  m_Children(std::list<MetaData::Pointer>(0))
  {
    this->m_Unlocked = isUnlocked;
    this->m_MetaDataParentAssigned = false;
  };
  
  /**
   * Constructor for an entry containing a MetaDataInfo of BYTE values.
   */
  MetaData::MetaData(const std::string& label, const std::vector<uint8_t>& dim, 
                     const std::vector<int8_t>& val, const std::string& desc,
                     bool isUnlocked)
  : DataObjectLabeled(label, desc),
  m_Info(MetaDataInfo::New(dim, val)),
  m_Children(std::list<MetaData::Pointer>(0))
  {
    this->m_Unlocked = isUnlocked;
    this->m_MetaDataParentAssigned = false;
  };
  
  /**
   * Constructor for an entry containing a MetaDataInfo of INTEGER values.
   */
  MetaData::MetaData(const std::string& label, const std::vector<uint8_t>& dim, 
                     const std::vector<int16_t>& val, const std::string& desc,
                     bool isUnlocked)
  : DataObjectLabeled(label, desc),
  m_Info(MetaDataInfo::New(dim, val)),
  m_Children(std::list<MetaData::Pointer>(0))
  {
    this->m_Unlocked = isUnlocked;
    this->m_MetaDataParentAssigned = false;
  };
  
  /**
   * Constructor for an entry containing a MetaDataInfo of FLOAT values.
   */
  MetaData::MetaData(const std::string& label, const std::vector<uint8_t>& dim, 
                     const std::vector<float>& val, const std::string& desc,
                     bool isUnlocked)
  : DataObjectLabeled(label, desc),
  m_Info(MetaDataInfo::New(dim, val)),
  m_Children(std::list<MetaData::Pointer>(0))
  {
    this->m_Unlocked = isUnlocked;
    this->m_MetaDataParentAssigned = false;
  };
  
  /**
   * Constructor for an entry containing a MetaDataInfo of string values.
   */
  MetaData::MetaData(const std::string& label, const std::vector<uint8_t>& dim, 
                     const std::vector<std::string>& val, const std::string& desc,
                     bool isUnlocked)
  : DataObjectLabeled(label, desc),
  m_Info(MetaDataInfo::New(dim, val)),
  m_Children(std::list<MetaData::Pointer>(0))
  {
    this->m_Unlocked = isUnlocked;
    this->m_MetaDataParentAssigned = false;
  };
}

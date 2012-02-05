/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2012, Arnaud Barré
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

#ifndef __btkMetaData_h
#define __btkMetaData_h

#include "btkDataObject.h"
#include "btkMetaDataInfo.h"

#include <list>

namespace btk
{
  class MetaData : public DataObjectLabeled
  {
  public:
    typedef SharedPtr<MetaData> Pointer;
    typedef SharedPtr<const MetaData> ConstPointer;
    
    typedef std::list<MetaData::Pointer>::iterator Iterator;
    typedef std::list<MetaData::Pointer>::const_iterator ConstIterator;
    
    static Pointer New(const std::string& label, 
                       const std::string& desc = "", bool isUnlocked = true)
    {return Pointer(new MetaData(label, desc, isUnlocked));};
    static Pointer New(const std::string& label, int8_t val,
                       const std::string& desc = "", bool isUnlocked = true)
    {return Pointer(new MetaData(label, val, desc, isUnlocked));};
    static Pointer New(const std::string& label, int16_t val, 
                       const std::string& desc = "", bool isUnlocked = true)
    {return Pointer(new MetaData(label, val, desc, isUnlocked));};
    static Pointer New(const std::string& label, float val, 
                       const std::string& desc = "", bool isUnlocked = true)
    {return Pointer(new MetaData(label, val, desc, isUnlocked));};
    static Pointer New(const std::string& label, const std::string& val,
                       const std::string& desc = "", bool isUnlocked = true)
    {return Pointer(new MetaData(label, val, desc, isUnlocked));};
    static Pointer New(const std::string& label, 
                       const std::vector<int8_t>& val, 
                       const std::string& desc = "",
                       bool isUnlocked = true)
    {return Pointer(new MetaData(label, val, desc, isUnlocked));};
    static Pointer New(const std::string& label,
                       const std::vector<int16_t>& val, 
                       const std::string& desc = "",
                       bool isUnlocked = true)
    {return Pointer(new MetaData(label, val, desc, isUnlocked));};
    static Pointer New(const std::string& label, 
                       const std::vector<float>& val, 
                       const std::string& desc = "",
                       bool isUnlocked = true)
    {return Pointer(new MetaData(label, val, desc, isUnlocked));};
    static Pointer New(const std::string& label, 
                       const std::vector<std::string>& val, 
                       const std::string& desc = "",
                       bool isUnlocked = true)
    {return Pointer(new MetaData(label, val, desc, isUnlocked));};
    static Pointer New(const std::string& label, 
                       const std::vector<uint8_t>& dim, 
                       const std::vector<int8_t>& val, 
                       const std::string& desc = "",
                       bool isUnlocked = true)
    {return Pointer(new MetaData(label, dim, val, desc, isUnlocked));};
    static Pointer New(const std::string& label,
                       const std::vector<uint8_t>& dim, 
                       const std::vector<int16_t>& val, 
                       const std::string& desc = "",
                       bool isUnlocked = true)
    {return Pointer(new MetaData(label, dim, val, desc, isUnlocked));};
    static Pointer New(const std::string& label, 
                       const std::vector<uint8_t>& dim, 
                       const std::vector<float>& val, 
                       const std::string& desc = "",
                       bool isUnlocked = true)
    {return Pointer(new MetaData(label, dim, val, desc, isUnlocked));};
    static Pointer New(const std::string& label, 
                       const std::vector<uint8_t>& dim, 
                       const std::vector<std::string>& val, 
                       const std::string& desc = "",
                       bool isUnlocked = true)
    {return Pointer(new MetaData(label, dim, val, desc, isUnlocked));};
    
    // ~MetaData(); // Implicit.
    void SetLabel(const std::string& label);
    bool GetUnlockState(void) const {return this->m_Unlocked;};
    void SetUnlockState(bool isUnlocked) {this->m_Unlocked = isUnlocked;};
    MetaDataInfo::Pointer GetInfo() {return this->m_Info;};
    MetaDataInfo::ConstPointer GetInfo() const {return this->m_Info;};
    void SetInfo(MetaDataInfo::Pointer val) {this->m_Info = val;};
    bool HasInfo() const {return (this->m_Info.get() != 0);};
    BTK_COMMON_EXPORT MetaDataInfo::Pointer ExtractChildInfo(const std::string& l, MetaDataInfo::Format f, int numDims, bool noPossibleEmptyValue = true);
    bool HasMetaDataAsParent() const {return this->m_MetaDataParentAssigned;};
    void SetParent(DataObject* parent = 0) {this->m_MetaDataParentAssigned = false; this->DataObject::SetParent(parent);};
    void SetParent(MetaData* parent = 0) {this->m_MetaDataParentAssigned = true; this->DataObject::SetParent(parent);};
    MetaData::Iterator Begin() {return this->m_Children.begin();};
    MetaData::ConstIterator Begin() const {return this->m_Children.begin();};
    MetaData::Iterator End() {return this->m_Children.end();};
    MetaData::ConstIterator End() const {return this->m_Children.end();};
    BTK_COMMON_EXPORT MetaData::Pointer GetChild(int idx);
    BTK_COMMON_EXPORT MetaData::ConstPointer GetChild(int idx) const;
    BTK_COMMON_EXPORT MetaData::Pointer GetChild(const std::string& label);
    BTK_COMMON_EXPORT MetaData::ConstPointer GetChild(const std::string& label) const;
    BTK_COMMON_EXPORT bool AppendChild(MetaData::Pointer entry);
    BTK_COMMON_EXPORT bool InsertChild(Iterator loc, MetaData::Pointer entry);
    BTK_COMMON_EXPORT bool InsertChild(int idx, MetaData::Pointer entry);
    BTK_COMMON_EXPORT void SetChild(int idx, MetaData::Pointer entry);
    BTK_COMMON_EXPORT MetaData::Pointer TakeChild(Iterator loc);
    BTK_COMMON_EXPORT MetaData::Pointer TakeChild(int idx);
    BTK_COMMON_EXPORT MetaData::Pointer TakeChild(const std::string& label);
    BTK_COMMON_EXPORT Iterator RemoveChild(Iterator loc);
    BTK_COMMON_EXPORT void RemoveChild(int idx);
    BTK_COMMON_EXPORT void RemoveChild(const std::string& label);
    BTK_COMMON_EXPORT void ClearChildren();
    bool HasChildren() const {return (this->m_Children.size() != 0);};
    int GetChildNumber() const {return static_cast<int>(this->m_Children.size());};
    BTK_COMMON_EXPORT Iterator FindChild(const std::string& label);
    BTK_COMMON_EXPORT ConstIterator FindChild(const std::string& label) const;
    BTK_COMMON_EXPORT Pointer Clone() const;
    BTK_COMMON_EXPORT friend bool operator==(const MetaData& rLHS, const MetaData& rRHS);
    friend bool operator!=(const MetaData& rLHS, const MetaData& rRHS)
    {
      return !(rLHS == rRHS);
    }
    
  protected:
    BTK_COMMON_EXPORT MetaData(const std::string& label, 
                               const std::string& desc = "",
                               bool isUnlocked = true);
    BTK_COMMON_EXPORT MetaData(const std::string& label, int8_t val,
                               const std::string& desc = "",
                               bool isUnlocked = true);
    BTK_COMMON_EXPORT MetaData(const std::string& label, int16_t val, 
                               const std::string& desc = "", 
                               bool isUnlocked = true);
    BTK_COMMON_EXPORT MetaData(const std::string& label, float val,
                               const std::string& desc = "",
                               bool isUnlocked = true);
    BTK_COMMON_EXPORT MetaData(const std::string& label, 
                               const std::string& val, 
                               const std::string& desc = "",
                               bool isUnlocked = true);
    BTK_COMMON_EXPORT MetaData(const std::string& label, 
                               const std::vector<int8_t>& val,
                               const std::string& desc = "",
                               bool isUnlocked = true);
    BTK_COMMON_EXPORT MetaData(const std::string& label, 
                               const std::vector<int16_t>& val, 
                               const std::string& desc = "",
                               bool isUnlocked = true);
    BTK_COMMON_EXPORT MetaData(const std::string& label, 
                               const std::vector<float>& val, 
                               const std::string& desc = "",
                               bool isUnlocked = true);
    BTK_COMMON_EXPORT MetaData(const std::string& label, 
                               const std::vector<std::string>& val, 
                               const std::string& desc = "",
                               bool isUnlocked = true);
    BTK_COMMON_EXPORT MetaData(const std::string& label, 
                               const std::vector<uint8_t>& dim, 
                               const std::vector<int8_t>& val,
                               const std::string& desc = "",
                               bool isUnlocked = true);
    BTK_COMMON_EXPORT MetaData(const std::string& label, 
                               const std::vector<uint8_t>& dim, 
                               const std::vector<int16_t>& val, 
                               const std::string& desc = "",
                               bool isUnlocked = true);
    BTK_COMMON_EXPORT MetaData(const std::string& label, 
                               const std::vector<uint8_t>& dim, 
                               const std::vector<float>& val, 
                               const std::string& desc = "",
                               bool isUnlocked = true);
    BTK_COMMON_EXPORT MetaData(const std::string& label, 
                               const std::vector<uint8_t>& dim, 
                               const std::vector<std::string>& val, 
                               const std::string& desc = "",
                               bool isUnlocked = true);
    
    using DataObject::SetParent;
    
  private:
    bool m_Unlocked;
    MetaDataInfo::Pointer m_Info;
    bool m_MetaDataParentAssigned;
    std::list<MetaData::Pointer> m_Children;
    
    MetaData(const MetaData& ); // Not implemented.
    MetaData& operator=(const MetaData& ); // Not implemented.
    
  };
};

#endif // __btkMetaData_h

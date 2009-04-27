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

#ifndef __btkMetaDataEntry_h
#define __btkMetaDataEntry_h

#include "btkDataObject.h"
#include "btkMetaDataEntryValue.h"

#include <list>

namespace btk
{
  class MetaDataEntry : public DataObject
  {
  public:
    typedef SharedPtr<MetaDataEntry> Pointer;
    typedef SharedPtr<const MetaDataEntry> ConstPointer;
    
    typedef std::list<MetaDataEntry::Pointer>::iterator Iterator;
    typedef std::list<MetaDataEntry::Pointer>::const_iterator ConstIterator;
    BTK_COMMON_EXPORT static void CollapseChildrenValues(
				std::vector<std::string>& target,
				MetaDataEntry::ConstPointer parent,
        const std::string& basename,
				int targetFinalSize = -1 ,
        const std::string& blankReplacement = "");
    static Pointer New(const std::string& name, 
											 const std::string& desc = "",bool isUnlocked = true)
		{return Pointer(new MetaDataEntry(name, desc, isUnlocked));};
    static Pointer New(const std::string& name, int8_t val,
                       const std::string& desc = "", bool isUnlocked = true)		{return Pointer(new MetaDataEntry(name, val, desc, isUnlocked));};
    static Pointer New(const std::string& name, int16_t val, 
                       const std::string& desc = "", bool isUnlocked = true)    {return Pointer(new MetaDataEntry(name, val, desc, isUnlocked));};
    static Pointer New(const std::string& name, float val, 
                       const std::string& desc = "", bool isUnlocked = true)    {return Pointer(new MetaDataEntry(name, val, desc, isUnlocked));};
    static Pointer New(const std::string& name, std::string val,
			 								 const std::string& desc = "", bool isUnlocked = true)		{return Pointer(new MetaDataEntry(name, val, desc, isUnlocked));};
    static Pointer New(const std::string& name, 
											 const std::vector<uint8_t>& dim, 
                       const std::vector<int8_t>& val, 
											 const std::string& desc = "",
                       bool isUnlocked = true)
		{return Pointer(new MetaDataEntry(name, dim, val, desc, isUnlocked));};
    static Pointer New(const std::string& name,
											 const std::vector<uint8_t>& dim, 
                       const std::vector<int16_t>& val, 
											 const std::string& desc = "",
                       bool isUnlocked = true)
		{return Pointer(new MetaDataEntry(name, dim, val, desc, isUnlocked));};
    static Pointer New(const std::string& name, 
											 const std::vector<uint8_t>& dim, 
                       const std::vector<float>& val, 
											 const std::string& desc = "",
                       bool isUnlocked = true)
		{return Pointer(new MetaDataEntry(name, dim, val, desc, isUnlocked));};
    static Pointer New(const std::string& name, 
											 const std::vector<uint8_t>& dim, 
                       const std::vector<std::string>& val, 
											 const std::string& desc = "",
                       bool isUnlocked = true)
		{return Pointer(new MetaDataEntry(name, dim, val, desc, isUnlocked));};
    
    // ~MetaDataEntry(); // Implicit.
    
    const std::string& GetLabel() const {return this->m_Label;};
    BTK_COMMON_EXPORT void SetLabel(const std::string& label);
    const std::string& GetDescription() const {return this->m_Description;};
    void SetDescription(const std::string& desc) {this->m_Description = desc;};
    bool GetUnlockState(void) const {return this->m_Unlocked;};
    void SetUnlockState(bool isUnlocked) {this->m_Unlocked = isUnlocked;};
    MetaDataEntryValue::Pointer GetMetaDataEntryValue() {return this->m_Value;};
    MetaDataEntryValue::ConstPointer GetMetaDataEntryValue() const {return this->m_Value;};
    void SetMetaDataEntryValue(MetaDataEntryValue::Pointer val) {this->m_Value = val;};
    bool HasMetaDataEntryValue() const {return (this->m_Value.get() != 0);};
    bool HasMetaParent() const {return this->m_HasMetaParent;}; 
    void SetParent(DataObject* parent = 0) {this->m_HasMetaParent = false; this->DataObject::SetParent(parent);};
    void SetParent(MetaDataEntry* parent = 0) {this->m_HasMetaParent = true; this->DataObject::SetParent(parent);};
    MetaDataEntry::Iterator Begin() {return this->m_Children.begin();};
    MetaDataEntry::ConstIterator Begin() const {return this->m_Children.begin();};
    MetaDataEntry::Iterator End() {return this->m_Children.end();};
    MetaDataEntry::ConstIterator End() const {return this->m_Children.end();};
    BTK_COMMON_EXPORT MetaDataEntry::Pointer GetChild(int idx);
    BTK_COMMON_EXPORT MetaDataEntry::ConstPointer GetChild(int idx) const;
    BTK_COMMON_EXPORT MetaDataEntry::Pointer GetChild(const std::string& label);
    BTK_COMMON_EXPORT MetaDataEntry::ConstPointer GetChild(const std::string& label) const;
    BTK_COMMON_EXPORT bool AppendChild(MetaDataEntry::Pointer entry);
    BTK_COMMON_EXPORT bool InsertChild(Iterator loc, MetaDataEntry::Pointer entry);
    BTK_COMMON_EXPORT bool InsertChild(int idx, MetaDataEntry::Pointer entry);
    BTK_COMMON_EXPORT void SetChild(int idx, MetaDataEntry::Pointer entry);
    BTK_COMMON_EXPORT MetaDataEntry::Pointer TakeChild(Iterator loc);
    BTK_COMMON_EXPORT MetaDataEntry::Pointer TakeChild(int idx);
    BTK_COMMON_EXPORT MetaDataEntry::Pointer TakeChild(const std::string& label);
    bool HasChildren() const {return (this->m_Children.size() != 0);};
    int GetChildNumber() const {return this->m_Children.size();};
    BTK_COMMON_EXPORT Iterator Find(const std::string& label);
    BTK_COMMON_EXPORT ConstIterator Find(const std::string& label) const;
    BTK_COMMON_EXPORT Pointer Clone() const;
    
  protected:
    BTK_COMMON_EXPORT MetaDataEntry(const std::string& name, 
																		const std::string& desc = "",
									                  bool isUnlocked = true);
    BTK_COMMON_EXPORT MetaDataEntry(const std::string& name, int8_t val,
									                  const std::string& desc = "",
																		bool isUnlocked = true);
    BTK_COMMON_EXPORT MetaDataEntry(const std::string& name, int16_t val, 
                  									const std::string& desc = "", 
																		bool isUnlocked = true);
    BTK_COMMON_EXPORT MetaDataEntry(const std::string& name, float val,
																		const std::string& desc = "",
																		bool isUnlocked = true);
    BTK_COMMON_EXPORT MetaDataEntry(const std::string& name, 
																		std::string val, 
																		const std::string& desc = "",
																		bool isUnlocked = true);
    BTK_COMMON_EXPORT MetaDataEntry(const std::string& name, 
																	  const std::vector<uint8_t>& dim, 
                  									const std::vector<int8_t>& val,
																		const std::string& desc = "",
									                  bool isUnlocked = true);
    BTK_COMMON_EXPORT MetaDataEntry(const std::string& name, 
																		const std::vector<uint8_t>& dim, 
									                  const std::vector<int16_t>& val, 
																		const std::string& desc = "",
									                  bool isUnlocked = true);
    BTK_COMMON_EXPORT MetaDataEntry(const std::string& name, 
																		const std::vector<uint8_t>& dim, 
                 										const std::vector<float>& val, 
																		const std::string& desc = "",
                 										bool isUnlocked = true);
    BTK_COMMON_EXPORT MetaDataEntry(const std::string& name, 
																		const std::vector<uint8_t>& dim, 
                  									const std::vector<std::string>& val, 
																		const std::string& desc = "",
                  									bool isUnlocked = true);
    
    using DataObject::SetParent;
    
  private:
    std::string m_Label;
    std::string m_Description;
    bool m_Unlocked;
    MetaDataEntryValue::Pointer m_Value;
    bool m_HasMetaParent;
    std::list<MetaDataEntry::Pointer> m_Children;
    
    MetaDataEntry(const MetaDataEntry& ); // Not implemented.
    MetaDataEntry& operator=(const MetaDataEntry& ); // Not implemented.
    
  };
};

#endif // __btkMetaDataEntry_h

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

#ifndef __btkMetaDataEntryValue_h
#define __btkMetaDataEntryValue_h

#include "btkSharedPtr.h"
#include "btkMacro.h"

#include <string>
#include <vector>

#ifdef _MSC_VER
  #include "../../Utilities/stdint.h"
#else
  #include <stdint.h>
#endif

namespace btk
{
  class MetaDataEntryValue
  {
  public:
    typedef enum {CHAR=-1, BYTE=1, INTEGER=2, FLOAT=4} Format;
    typedef SharedPtr<MetaDataEntryValue> Pointer;
    typedef SharedPtr<const MetaDataEntryValue> ConstPointer;

    static Pointer New(int8_t val) {return Pointer(new MetaDataEntryValue(val));};
    static Pointer New(int16_t val) {return Pointer(new MetaDataEntryValue(val));};
    static Pointer New(float val) {return Pointer(new MetaDataEntryValue(val));};
    static Pointer New(std::string val) {return Pointer(new MetaDataEntryValue(val));};
    static Pointer New(const std::vector<int8_t>& val)  {return Pointer(new MetaDataEntryValue(val));};
    static Pointer New(const std::vector<int16_t>& val) {return Pointer(new MetaDataEntryValue(val));};
    static Pointer New(const std::vector<float>& val) {return Pointer(new MetaDataEntryValue(val));};
    static Pointer New(const std::vector<std::string>& val) {return Pointer(new MetaDataEntryValue(val));};
    static Pointer New(const std::vector<uint8_t>& dim, const std::vector<int8_t>& val)  {return Pointer(new MetaDataEntryValue(dim, val));};
    static Pointer New(const std::vector<uint8_t>& dim, const std::vector<int16_t>& val) {return Pointer(new MetaDataEntryValue(dim, val));};
    static Pointer New(const std::vector<uint8_t>& dim, const std::vector<float>& val) {return Pointer(new MetaDataEntryValue(dim, val));};
    static Pointer New(const std::vector<uint8_t>& dim, const std::vector<std::string>& val) {return Pointer(new MetaDataEntryValue(dim, val));};

    // ~MetaDataEntryValue(); // Implicit.
    
    Format GetFormat() const {return this->m_Format;};
    BTK_COMMON_EXPORT void SetFormat(Format format);
    BTK_COMMON_EXPORT uint8_t GetDimension(int idx) const;
    BTK_COMMON_EXPORT void SetDimension(int idx, uint8_t val);
    const std::vector<uint8_t>& GetDimensions() const {return this->m_Dims;};
    BTK_COMMON_EXPORT int GetDimensionsProduct(int start = 0) const;
    BTK_COMMON_EXPORT void SetDimensions(const std::vector<uint8_t>& dims);
    BTK_COMMON_EXPORT void ResizeDimensions(int nb);
    BTK_COMMON_EXPORT const std::string GetValue(int idx) const;
    BTK_COMMON_EXPORT void SetValue(int idx, int8_t val);
    BTK_COMMON_EXPORT void SetValue(int idx, int16_t val);
    BTK_COMMON_EXPORT void SetValue(int idx, float val);
    BTK_COMMON_EXPORT void SetValue(int idx, const std::string& val);
    const std::vector<std::string>& GetValues() const {return this->m_Values;};
    BTK_COMMON_EXPORT void SetValues(int8_t val);
    BTK_COMMON_EXPORT void SetValues(int16_t val);
    BTK_COMMON_EXPORT void SetValues(float val);
    BTK_COMMON_EXPORT void SetValues(const std::string& val);
    BTK_COMMON_EXPORT void SetValues(const std::vector<int8_t>& val);
    BTK_COMMON_EXPORT void SetValues(const std::vector<int16_t>& val);
    BTK_COMMON_EXPORT void SetValues(const std::vector<float>& val);
    BTK_COMMON_EXPORT void SetValues(const std::vector<std::string>& val);
    BTK_COMMON_EXPORT void SetValues(const std::vector<uint8_t>& dim, const std::vector<int8_t>& val);
    BTK_COMMON_EXPORT void SetValues(const std::vector<uint8_t>& dim, const std::vector<int16_t>& val);
    BTK_COMMON_EXPORT void SetValues(const std::vector<uint8_t>& dim, const std::vector<float>& val);
    BTK_COMMON_EXPORT void SetValues(const std::vector<uint8_t>& dim, const std::vector<std::string>& val);
    Pointer Clone() const {return Pointer(new MetaDataEntryValue(*this));};
    //ConstPointer Clone() const {return ConstPointer(new MetaDataEntryValue(*this));};

    BTK_COMMON_EXPORT friend bool operator==(const MetaDataEntryValue& rLHS, const MetaDataEntryValue& rRHS);
    friend bool operator!=(const MetaDataEntryValue& rLHS, const MetaDataEntryValue& rRHS)
    {
      return !(rLHS == rRHS);
    };
  
  protected:
    BTK_COMMON_EXPORT MetaDataEntryValue(int8_t val);
    BTK_COMMON_EXPORT MetaDataEntryValue(int16_t val);
    BTK_COMMON_EXPORT MetaDataEntryValue(float val);
    BTK_COMMON_EXPORT MetaDataEntryValue(std::string val);
    BTK_COMMON_EXPORT MetaDataEntryValue(const std::vector<int8_t>& val);
    BTK_COMMON_EXPORT MetaDataEntryValue(const std::vector<int16_t>& val);
    BTK_COMMON_EXPORT MetaDataEntryValue(const std::vector<float>& val);
    BTK_COMMON_EXPORT MetaDataEntryValue(const std::vector<std::string>& val);
    BTK_COMMON_EXPORT MetaDataEntryValue(const std::vector<uint8_t>& dim, const std::vector<int8_t>& val);
    BTK_COMMON_EXPORT MetaDataEntryValue(const std::vector<uint8_t>& dim, const std::vector<int16_t>& val);
    BTK_COMMON_EXPORT MetaDataEntryValue(const std::vector<uint8_t>& dim, const std::vector<float>& val);
    BTK_COMMON_EXPORT MetaDataEntryValue(const std::vector<uint8_t>& dim, const std::vector<std::string>& val);
    
  private:
    BTK_COMMON_EXPORT MetaDataEntryValue(const MetaDataEntryValue& toCopy);
    MetaDataEntryValue& operator=(const MetaDataEntryValue& ); // Not implemented. 
    
    Format m_Format;
    std::vector<uint8_t> m_Dims;
    std::vector<std::string> m_Values;
  };
};

#endif // __btkMetaDataEntryValue_h


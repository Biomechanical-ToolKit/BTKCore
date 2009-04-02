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
    enum Format {CHAR=-1, BYTE=1, INTEGER=2, FLOAT=4};
    typedef SharedPtr<MetaDataEntryValue> Pointer;
    typedef SharedPtr<const MetaDataEntryValue> ConstPointer;

    static Pointer New(int8_t val);
    static Pointer New(int16_t val);
    static Pointer New(float val);
    static Pointer New(std::string val);
    static Pointer New(const std::vector<uint8_t>& dim, const std::vector<int8_t>& val);
    static Pointer New(const std::vector<uint8_t>& dim, const std::vector<int16_t>& val);
    static Pointer New(const std::vector<uint8_t>& dim, const std::vector<float>& val);
    static Pointer New(const std::vector<uint8_t>& dim, const std::vector<std::string>& val);
    
    // ~MetaDataEntryValue(); // Implicit.
    
    Format GetFormat() const {return this->m_Format;};
    void SetFormat(Format format);
    uint8_t GetDimension(int idx) const;
    void SetDimension(int idx, uint8_t val);
    const std::vector<uint8_t>& GetDimensions() const {return this->m_Dims;};
    void SetDimensions(const std::vector<uint8_t>& dims);
    void ResizeDimensions(int nb);
    const std::string GetValue(int idx) const;
    void SetValue(int idx, int8_t val);
    void SetValue(int idx, int16_t val);
    void SetValue(int idx, float val);
    void SetValue(int idx, const std::string& val);
    const std::vector<std::string>& GetValues() const {return this->m_Values;};
    void SetValues(int8_t val);
    void SetValues(int16_t val);
    void SetValues(float val);
    void SetValues(const std::string& val);
    void SetValues(const std::vector<uint8_t>& dim, const std::vector<int8_t>& val);
    void SetValues(const std::vector<uint8_t>& dim, const std::vector<int16_t>& val);
    void SetValues(const std::vector<uint8_t>& dim, const std::vector<float>& val);
    void SetValues(const std::vector<uint8_t>& dim, const std::vector<std::string>& val);
    Pointer Clone() const {return Pointer(new MetaDataEntryValue(*this));};
    //ConstPointer Clone() const {return ConstPointer(new MetaDataEntryValue(*this));};

    friend bool operator==(const MetaDataEntryValue& rLHS, const MetaDataEntryValue& rRHS);
    friend bool operator!=(const MetaDataEntryValue& rLHS, const MetaDataEntryValue& rRHS)
    {
      return !(rLHS == rRHS);
    };
  
  protected:
    MetaDataEntryValue(int8_t val);
    MetaDataEntryValue(int16_t val);
    MetaDataEntryValue(float val);
    MetaDataEntryValue(std::string val);
    MetaDataEntryValue(const std::vector<uint8_t>& dim, const std::vector<int8_t>& val);
    MetaDataEntryValue(const std::vector<uint8_t>& dim, const std::vector<int16_t>& val);
    MetaDataEntryValue(const std::vector<uint8_t>& dim, const std::vector<float>& val);
    MetaDataEntryValue(const std::vector<uint8_t>& dim, const std::vector<std::string>& val);
    
  private:
    int prod(int start = 0) const;
    
    Format m_Format;
    std::vector<uint8_t> m_Dims;
    std::vector<std::string> m_Values;
    
    MetaDataEntryValue(const MetaDataEntryValue& toCopy);
    MetaDataEntryValue& operator=(const MetaDataEntryValue& ); // Not implemented.
    
  };
};

#endif // __btkMetaDataEntryValue_h


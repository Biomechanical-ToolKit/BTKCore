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

#ifndef __btkMetaDataInfo_h
#define __btkMetaDataInfo_h

#include "btkSharedPtr.h"
#include "btkMacro.h"

#include <string>
#include <vector>

#ifdef _MSC_VER
  #include "Utilities/stdint.h"
#else
  #include <stdint.h>
#endif

namespace btk
{
  class MetaDataInfo
  {
  public:
    typedef enum {Char=-1, Byte=1, Integer=2, Real=4} Format;
    typedef SharedPtr<MetaDataInfo> Pointer;
    typedef SharedPtr<const MetaDataInfo> ConstPointer;

    static Pointer New(int8_t val) {return Pointer(new MetaDataInfo(std::vector<uint8_t>(0), std::vector<int8_t>(1, val)));};
    static Pointer New(int16_t val) {return Pointer(new MetaDataInfo(std::vector<uint8_t>(0), std::vector<int16_t>(1, val)));};
    static Pointer New(float val) {return Pointer(new MetaDataInfo(std::vector<uint8_t>(0), std::vector<float>(1, val)));};
    static Pointer New(const std::string& val) {return Pointer(new MetaDataInfo(val));};
    static Pointer New(const std::vector<int8_t>& val)  {return Pointer(new MetaDataInfo(std::vector<uint8_t>(1,static_cast<uint8_t>(val.size())), val));};
    static Pointer New(const std::vector<int16_t>& val) {return Pointer(new MetaDataInfo(std::vector<uint8_t>(1,static_cast<uint8_t>(val.size())), val));};
    static Pointer New(const std::vector<float>& val) {return Pointer(new MetaDataInfo(std::vector<uint8_t>(1,static_cast<uint8_t>(val.size())), val));};
    static Pointer New(const std::vector<std::string>& val) {return Pointer(new MetaDataInfo(val));};
    static Pointer New(const std::vector<uint8_t>& dim, const std::vector<int8_t>& val)  {return Pointer(new MetaDataInfo(dim, val));};
    static Pointer New(const std::vector<uint8_t>& dim, const std::vector<int16_t>& val) {return Pointer(new MetaDataInfo(dim, val));};
    static Pointer New(const std::vector<uint8_t>& dim, const std::vector<float>& val) {return Pointer(new MetaDataInfo(dim, val));};
    static Pointer New(const std::vector<uint8_t>& dim, const std::vector<std::string>& val) {return Pointer(new MetaDataInfo(dim, val));};

    BTK_COMMON_EXPORT ~MetaDataInfo();
    
    Format GetFormat() const {return this->m_Format;};
    BTK_COMMON_EXPORT std::string GetFormatAsString() const;
    BTK_COMMON_EXPORT void SetFormat(Format format);
    BTK_COMMON_EXPORT uint8_t GetDimension(int idx) const;
    BTK_COMMON_EXPORT void SetDimension(int idx, uint8_t val);
    const std::vector<uint8_t>& GetDimensions() const {return this->m_Dims;};
    BTK_COMMON_EXPORT int GetDimensionsProduct(int start = 0) const;
    BTK_COMMON_EXPORT void SetDimensions(const std::vector<uint8_t>& dims);
    BTK_COMMON_EXPORT void ResizeDimensions(int nb);
    BTK_COMMON_EXPORT void* GetValue(int idx) const;
    BTK_COMMON_EXPORT void SetValue(int idx, int8_t val);
    BTK_COMMON_EXPORT void SetValue(int idx, int16_t val);
    BTK_COMMON_EXPORT void SetValue(int idx, float val);
    BTK_COMMON_EXPORT void SetValue(int idx, const std::string& val);
    BTK_COMMON_EXPORT void SetValue(int idx, int val);
    BTK_COMMON_EXPORT void SetValue(int idx, double val);
    bool HasValues() const {return !this->m_Values.empty();};
    const std::vector<void*>& GetValues() const {return this->m_Values;};
    void SetValues(int8_t val) {this->SetValues(std::vector<uint8_t>(0), std::vector<int8_t>(1, val));};
    void SetValues(int16_t val) {this->SetValues(std::vector<uint8_t>(0), std::vector<int16_t>(1, val));};
    void SetValues(float val) {this->SetValues(std::vector<uint8_t>(0), std::vector<float>(1, val));};
    void SetValues(const std::string& val) {this->SetValues(std::vector<uint8_t>(1, static_cast<uint8_t>(val.length())), std::vector<std::string>(1, val));};
    void SetValues(const std::vector<int8_t>& val) {this->SetValues(std::vector<uint8_t>(1,static_cast<uint8_t>(val.size())), val);};
    void SetValues(const std::vector<int16_t>& val) {this->SetValues(std::vector<uint8_t>(1,static_cast<uint8_t>(val.size())), val);};
    void SetValues(const std::vector<float>& val) {this->SetValues(std::vector<uint8_t>(1,static_cast<uint8_t>(val.size())), val);};
    BTK_COMMON_EXPORT void SetValues(const std::vector<std::string>& val);
    BTK_COMMON_EXPORT void SetValues(const std::vector<uint8_t>& dims, const std::vector<int8_t>& val);
    BTK_COMMON_EXPORT void SetValues(const std::vector<uint8_t>& dims, const std::vector<int16_t>& val);
    BTK_COMMON_EXPORT void SetValues(const std::vector<uint8_t>& dims, const std::vector<float>& val);
    BTK_COMMON_EXPORT void SetValues(const std::vector<uint8_t>& dims, const std::vector<std::string>& val);
    Pointer Clone() const {return Pointer(new MetaDataInfo(*this));};
    //ConstPointer Clone() const {return ConstPointer(new MetaDataInfo(*this));};

    BTK_COMMON_EXPORT const std::string ToString(int idx) const;
    BTK_COMMON_EXPORT int8_t ToInt8(int idx) const;
    BTK_COMMON_EXPORT uint8_t ToUInt8(int idx) const;
    BTK_COMMON_EXPORT int16_t ToInt16(int idx) const;
    BTK_COMMON_EXPORT uint16_t ToUInt16(int idx) const;
    BTK_COMMON_EXPORT int ToInt(int idx) const;
    BTK_COMMON_EXPORT unsigned int ToUInt(int idx) const;
    BTK_COMMON_EXPORT float ToFloat(int idx) const;
    BTK_COMMON_EXPORT double ToDouble(int idx) const;
    BTK_COMMON_EXPORT const std::vector<std::string> ToString() const;
    BTK_COMMON_EXPORT void ToString(std::vector<std::string>& val) const;
    BTK_COMMON_EXPORT const std::vector<int8_t> ToInt8() const;
    BTK_COMMON_EXPORT void ToInt8(std::vector<int8_t>& val) const;
    BTK_COMMON_EXPORT const std::vector<uint8_t> ToUInt8() const;
    BTK_COMMON_EXPORT void ToUInt8(std::vector<uint8_t>& val) const;
    BTK_COMMON_EXPORT const std::vector<int16_t> ToInt16() const;
    BTK_COMMON_EXPORT void ToInt16(std::vector<int16_t>& val) const;
    BTK_COMMON_EXPORT const std::vector<uint16_t> ToUInt16() const;
    BTK_COMMON_EXPORT void ToUInt16(std::vector<uint16_t>& val) const;
    BTK_COMMON_EXPORT const std::vector<int> ToInt() const;
    BTK_COMMON_EXPORT void ToInt(std::vector<int>& val) const;
    BTK_COMMON_EXPORT const std::vector<unsigned int> ToUInt() const;
    BTK_COMMON_EXPORT void ToUInt(std::vector<unsigned int>& val) const;
    BTK_COMMON_EXPORT const std::vector<float> ToFloat() const;
    BTK_COMMON_EXPORT void ToFloat(std::vector<float>& val) const;
    BTK_COMMON_EXPORT const std::vector<double> ToDouble() const;
    BTK_COMMON_EXPORT void ToDouble(std::vector<double>& val) const;
    BTK_COMMON_EXPORT friend bool operator==(const MetaDataInfo& rLHS, const MetaDataInfo& rRHS);
    friend bool operator!=(const MetaDataInfo& rLHS, const MetaDataInfo& rRHS)
    {
      return !(rLHS == rRHS);
    };
  
  protected:
    BTK_COMMON_EXPORT MetaDataInfo(const std::string& val);
    BTK_COMMON_EXPORT MetaDataInfo(const std::vector<std::string>& val);
    BTK_COMMON_EXPORT MetaDataInfo(const std::vector<uint8_t>& dims, const std::vector<int8_t>& val);
    BTK_COMMON_EXPORT MetaDataInfo(const std::vector<uint8_t>& dims, const std::vector<int16_t>& val);
    BTK_COMMON_EXPORT MetaDataInfo(const std::vector<uint8_t>& dims, const std::vector<float>& val);
    BTK_COMMON_EXPORT MetaDataInfo(const std::vector<uint8_t>& dims, const std::vector<std::string>& val);
    
  private:
    BTK_COMMON_EXPORT MetaDataInfo(const MetaDataInfo& toCopy);
    MetaDataInfo& operator=(const MetaDataInfo& ); // Not implemented.

    void FillDimensions(const std::vector<std::string>& val);
    void FillSource(std::vector<std::string>& val) const;

    std::vector<uint8_t> m_Dims;
    Format m_Format;
    std::vector<void*> m_Values;
  };
};

#endif // __btkMetaDataInfo_h


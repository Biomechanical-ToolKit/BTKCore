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

#include "btkMetaDataInfo.h"
#include "btkMetaDataInfo_p.h"

#include <math.h>

namespace btk
{
  /**
   * @class MetaDataInfo
   * @brief Container class to store data of a MetaData object.
   *
   * @ingroup BTKCommon
   */
  
  /**
   * @enum MetaDataInfo::Format
   * Enums used to specify data format.
   */
  /**
   * @var MetaDataInfo::Format MetaDataInfo::Char
   * Enum value which represents the string.
   */
  /**
   * @var MetaDataInfo::Format MetaDataInfo::Byte
   * Enum value which represents the signed 8-bit integers.
   */
  /**
   * @var MetaDataInfo::Format MetaDataInfo::Integer
   * Enum value which represents the the signed 16-bit integers.
   */
  /**
   * @var MetaDataInfo::Format MetaDataInfo::Real
   * Enum value which represents the floats.
   */
  
  /**
   * @typedef MetaDataInfo::Pointer
   * Smart pointer associated with a MetaDataInfo object.
   */
  
  /**
   * @typedef MetaDataInfo::ConstPointer
   * Smart pointer associated with a const MetaDataInfo object.
   */
  
  /**
   * @fn MetaDataInfo::Pointer MetaDataInfo::New(int8_t val)

   * Creates a smart pointer from the MetaDataInfo(int8_t) constructor.
   */
  
  /**
   * @fn MetaDataInfo::Pointer MetaDataInfo::New(int16_t val)
   * Creates a smart pointer from the MetaDataInfo(int16_t) constructor.
   */

  /**
   * @fn MetaDataInfo::Pointer MetaDataInfo::New(float val)
   * Creates a smart pointer from the MetaDataInfo(float) constructor.
   */

  /**
   * @fn MetaDataInfo::Pointer MetaDataInfo::New(std::string val)
   * Creates a smart pointer from the MetaDataInfo(std::string) constructor.
   */

  /**
   * @fn MetaDataInfo::Pointer MetaDataInfo::New(const std::vector<int8_t>& val)
   * Creates a smart pointer from the MetaDataInfo(const std::vector<int8_t>&) constructor.
   */

  /**
   * @fn MetaDataInfo::Pointer MetaDataInfo::New(const std::vector<int16_t>& val)
   * Creates a smart pointer from the MetaDataInfo(const std::vector<int16_t>&) constructor.
   */
  
  /**
   * @fn MetaDataInfo::Pointer MetaDataInfo::New(const std::vector<float>& val)
   * Creates a smart pointer from the MetaDataInfo(const std::vector<float>&) constructor.
   */
  
  /**
   * @fn MetaDataInfo::Pointer MetaDataInfo::New(const std::vector<std::string>& val)
   * Creates a smart pointer from the MetaDataInfo(const std::vector<std::string>&) constructor.
   */

  /**
   * @fn MetaDataInfo::Pointer MetaDataInfo::New(const std::vector<uint8_t>& dim, const std::vector<int8_t>& val)
   * Creates a smart pointer from the MetaDataInfo(const std::vector<uint8_t>&, const std::vector<int8_t>&) constructor.
   */

  /**
   * @fn MetaDataInfo::Pointer MetaDataInfo::New(const std::vector<uint8_t>& dim, const std::vector<int16_t>& val)
   * Creates a smart pointer from the MetaDataInfo(const std::vector<uint8_t>&, const std::vector<int16_t>&) constructor.
   */
  
  /**
   * @fn MetaDataInfo::Pointer MetaDataInfo::New(const std::vector<uint8_t>& dim, const std::vector<float>& val)
   * Creates a smart pointer from the MetaDataInfo(const std::vector<uint8_t>&, const std::vector<float>&) constructor.
   */
  
  /**
   * @fn MetaDataInfo::Pointer MetaDataInfo::New(const std::vector<uint8_t>& dim, const std::vector<std::string>& val)
   * Creates a smart pointer from the MetaDataInfo(const std::vector<uint8_t>&, const std::vector<std::string>&) constructor.
   */

  MetaDataInfo::~MetaDataInfo()
  {
    Clear_p(this->m_Format, this->m_Values);
  }

  /**
   * @fn Format MetaDataInfo::GetFormat() const
   * Returns the format of the values.
   */
  
  /**
   * Returns the format of the values as a string.
   */
  std::string MetaDataInfo::GetFormatAsString() const
  {
    std::string format = "";
    switch(this->m_Format)
    {
      case Byte:
        format = "Byte";
        break;
      case Integer:
        format = "Integer";
        break;
      case Real:
        format = "Real";
        break;
      case Char:
        format = "Char";
        break;
    }
    return format;
  };
  
  /**
   * Sets the format of the values and adapt the values.
   * This method erases the values and set default values ("0" or a string with white spaces).
   */
  void MetaDataInfo::SetFormat(Format format)
  {
    if (this->m_Format == format)
      return;
    
    std::vector<void*> oldValues = this->m_Values;
    
    this->m_Values = Convertify_p(this->m_Format, format, this->m_Values);
    Clear_p(this->m_Format, oldValues);
    
    if (this->m_Format == Char)
      this->m_Dims.erase(this->m_Dims.begin());
    else if ((format == Char) && (this->m_Values.size() != 0))
      this->m_Dims.insert(this->m_Dims.begin(), static_cast<uint8_t>(static_cast<std::string*>(this->m_Values[0])->length()));
    
    this->m_Format = format;
  };

  /**
   * Returns the dimension at the specified @a idx or 0 if @a idx is out of range.
   */
  uint8_t MetaDataInfo::GetDimension(int idx) const
  {
    if (idx >= static_cast<int>(this->m_Dims.size()))
      return 0;
    else
      return this->m_Dims[idx];
  };

  /**
   * Sets @a val as the new dimension at the specified @a idx or do nothing if @a idx is out of range.
   * This method alterates the values' structure and set default values ("0" or a string with white spaces).
   */
  void MetaDataInfo::SetDimension(int idx, uint8_t val)
  {
    if (idx >= static_cast<int>(this->m_Dims.size()))
    {
      btkErrorMacro("Out of range");
      return;
    }
    if (this->m_Dims[idx] == val)
      return;
    int oldProd = this->GetDimensionsProduct();
    uint8_t oldValue = this->m_Dims[idx];
    this->m_Dims[idx] = val;
    int prod = this->GetDimensionsProduct();
    if (this->m_Format == Char)
    {
      if (idx == 0)
      {
        std::vector<void*>::iterator it = this->m_Values.begin();
        while (it != this->m_Values.end())
        {
          (static_cast<std::string*>(*it))->resize(val, ' ');
          ++it;
        }
      }
    }
    if ( (this->m_Format != Char) || (idx != 0) )
    {
      int diffNb = val - oldValue;
      int repeat = this->GetDimensionsProduct(idx + 1);
      if (diffNb < 0)
      {
        int inc = 1;
        int step = prod / repeat;
        if (this->m_Format == Char)
          step = step / this->m_Dims[0];
        int elts = step / val;
        diffNb = diffNb * (-1);
        while(inc <= repeat)
        {
          std::vector<void*>::iterator itStart = this->m_Values.begin();
          std::vector<void*>::iterator itEnd = this->m_Values.begin();
          std::advance(itStart, step * inc);
          std::advance(itEnd, step * inc + diffNb * elts);
          Erase_p(this->m_Format, this->m_Values, itStart, itEnd);
          ++inc;
        }   
      }
      else
      {
        int inc = repeat;
        int step = oldProd / repeat;
        if (this->m_Format == Char)
          step = step / this->m_Dims[0];
        int elts = step / oldValue;
        while(inc > 0)
        {
          std::vector<void*>::iterator it = this->m_Values.begin();
          std::advance(it, step * inc);
          Insert_p(this->m_Format, this->m_Values, it, diffNb * elts);
          --inc;
        }
      }
    }
  };

  /**
   * @fn const std::vector<uint8_t>& MetaDataInfo::GetDimensions() const 
   * Returns the dimensions associated with the values.
   */

  /**
   * Compute the product of the dimensions from the idx @a start.
   */
  int MetaDataInfo::GetDimensionsProduct(int start) const
  {
    int prod = 1;
    int inc = start;
    while (inc < static_cast<int>(this->m_Dims.size()))
      prod *= this->m_Dims[inc++];
    return prod;
  };

  
  /**
   * Sets @a dims as the new dimensions and adapt the values and their structure.
   * This method can add default values ("0" or a string with white spaces).
   */
  void MetaDataInfo::SetDimensions(const std::vector<uint8_t>& dims)
  {
    if (this->m_Dims == dims)
      return;
    this->m_Dims = dims;
    if (dims.size() == 0)
      Resize_p(this->m_Format, this->m_Values, 1);
    else
    {
      if (this->m_Format == Char)
      {
        int prod = this->GetDimensionsProduct(1);
        Resize_p(this->m_Values, prod, std::string(this->m_Dims[0], ' '));
        for (int i = 0 ; i < prod ; ++i)
          static_cast<std::string*>(this->m_Values[i])->resize(this->m_Dims[0], ' ');
      }
      else
        Resize_p(this->m_Format, this->m_Values, this->GetDimensionsProduct());
    }
  };

  /**
   * Resize the dimensions and adapt the values and their structure.
   * If the number of dimensions is greater, then the new dimensions are equal to 1.
   * If the number of dimensions is lower, then the value are shrinked.
   */
  void MetaDataInfo::ResizeDimensions(int nb)
  {
    if (nb == static_cast<int>(this->m_Dims.size()))
      return;
    if (nb < static_cast<int>(this->m_Dims.size()))
    {
      this->m_Dims.resize(nb, 1);    
      int inc = 0;
      if (this->m_Format == Char)
        inc = 1;
      Resize_p(this->m_Format, this->m_Values, this->GetDimensionsProduct(inc));
      if (this->m_Format == Char && nb == 0)
        static_cast<std::string*>(this->m_Values[0])->resize(1, ' ');
    }
    else
      this->m_Dims.resize(nb, 1);
    
  };

  /**
   * Returns the value at the specified @a idx or 0 if @a idx is out of range.
   */
  void* MetaDataInfo::GetValue(int idx) const
  {
    if (idx >= static_cast<int>(this->m_Values.size()))
    {
      btkErrorMacro("Out of range");
      return 0;
    }
    else
      return this->m_Values[idx];
  };

  /**
   * Sets @a val as the value at the specified index @a idx.
   *
   * @warning This method doesn't do anything if the idx is out of range.
   */
  void MetaDataInfo::SetValue(int idx, int8_t val)
  {
    if (idx >= static_cast<int>(this->m_Values.size()))
    {
      btkErrorMacro("Out of range");
      return;
    }
    
    Delete_p(this->m_Format, this->m_Values[idx]);
    this->m_Values[idx] = Convertify_p(this->m_Format, val);
  };

  /**
   * Sets @a val as the value (if necessary, adapt it to fit the format) at the specified index @a idx.
   *
   * @warning This method doesn't do anything if the idx is out of range.
   */
  void MetaDataInfo::SetValue(int idx, int16_t val)
  {
    if (idx >= static_cast<int>(this->m_Values.size()))
    {
      btkErrorMacro("Out of range");
      return;
    }
    
    Delete_p(this->m_Format, this->m_Values[idx]);
    this->m_Values[idx] = Convertify_p(this->m_Format, val);
  };

  /**
   * Sets @a val as the value (if necessary, adapt it to fit the format) at the specified index @a idx.
   *
   * @warning This method doesn't do anything if the idx is out of range.
   */
  void MetaDataInfo::SetValue(int idx, float val)
  {
    if (idx >= static_cast<int>(this->m_Values.size()))
    {
      btkErrorMacro("Out of range");
      return;
    }
    
    Delete_p(this->m_Format, this->m_Values[idx]);
    this->m_Values[idx] = Convertify_p(this->m_Format, val);
  };

  /**
   * Sets @a val as the value (if necessary, adapt it to fit the format) at the specified index @a idx.
   * Dimensions are updated if necessary.
   *
   * @warning This method doesn't do anything if the idx is out of range.
   */
  void MetaDataInfo::SetValue(int idx, const std::string& val)
  {
    if (idx >= static_cast<int>(this->m_Values.size()))
    {
      btkErrorMacro("Out of range");
      return;
    }
    
    Delete_p(this->m_Format, this->m_Values[idx]);    
    this->m_Values[idx] = Convertify_p(this->m_Format, val);

    if (this->m_Format == Char)
    {
      int len = static_cast<int>(val.length());
      if (len > this->m_Dims[0])
        this->m_Dims[0] = len;
      for (int i = 0 ; i < this->GetDimensionsProduct(1) ; ++i)
        static_cast<std::string*>(this->m_Values[i])->resize(this->m_Dims[0], ' ');
    }
  };
  
  /**
   * Sets @a val as the value (if necessary, adapt it to fit the format) at the specified index @a idx.
   * 
   * @warning This method doesn't do anything if the idx is out of range.
   */
  void MetaDataInfo::SetValue(int idx, int val)
  {
    if (idx >= static_cast<int>(this->m_Values.size()))
    {
      btkErrorMacro("Out of range");
      return;
    }
    Delete_p(this->m_Format, this->m_Values[idx]);
    this->m_Values[idx] = Convertify_p(this->m_Format, val);
  };
  
  /**
   * Sets @a val as the value (if necessary, adapt it to fit the format) at the specified index @a idx.
   *
   * @warning This method doesn't do anything if the idx is out of range.
   */
  void MetaDataInfo::SetValue(int idx, double val)
  {
    if (idx >= static_cast<int>(this->m_Values.size()))
    {
      btkErrorMacro("Out of range");
      return;
    }
    Delete_p(this->m_Format, this->m_Values[idx]);
    this->m_Values[idx] = Convertify_p(this->m_Format, val);
  };
  
  /**
   * @fn const std::vector<std::string>& MetaDataInfo::GetValues() const
   * Returns the values as a vector of strings.
   */

  /**
   * @fn void MetaDataInfo::SetValues(int8_t val)
   * Overwrite the data to store @a val in a Byte format with a null dimension.
   */

  /**
   * @fn void MetaDataInfo::SetValues(int16_t val)
   * Overwrite the data to store @a val in a Integer format with a null dimension.
   */
  
  /**
   * @fn void MetaDataInfo::SetValues(float val)
   * Overwrite the data to store @a val in a Real format with a null dimension.
   */

  /**
   * @fn void MetaDataInfo::SetValues(const std::string& val)
   * Overwrite the data to store @a val in a Char format with one dimension equals 
   * to the size of the strings.
   */

  /**
   * @fn void MetaDataInfo::SetValues(const std::vector<int8_t>& val)
   * Overwrite the data to store @a val in a Byte format as a 1D vector.
   * @warning The number of values must be lower than 256.
   */
  
  /**
   * @fn void MetaDataInfo::SetValues(const std::vector<int16_t>& val)
   * Overwrite the data to store @a val in a Integer format as a 1D vector.
   * @warning The number of values must be lower than 256.
   */

  /**
   * @fn void MetaDataInfo::SetValues(const std::vector<float>& val)
   * Overwrite the data to store @a val in a Real format as a 1D vector.
   * @warning The number of values must be lower than 256.
   */

  /**
   * @fn void MetaDataInfo::SetValues(const std::vector<std::string>& val)
   * Overwrite the data to store @a val in a Char format as an array with auto-generated dimensions.
   * @warning The number of values must be lower than 256. The string can have a maximum length of 255.
   */
   void MetaDataInfo::SetValues(const std::vector<std::string>& val)
   {
     Clear_p(this->m_Format, this->m_Values);
     this->FillDimensions(val);
     this->m_Format = Char;
     std::vector<std::string> values = val;
     this->FillSource(values);
     Voidify_p(values, this->m_Values);
   };

   /**
   * @fn void MetaDataInfo::SetValues(const std::vector<uint8_t>& dims, const std::vector<int8_t>& val)
   * Overwrite the data to store @a val in an Byte format with the dimensions @a dims.
   * If the product of the dimensions is not equal to the size of @a val,
   * then the number of values is adjusted to fit with it. The value added is "0".
   * @warning Each dimension must be lower than 256.
   */
   void MetaDataInfo::SetValues(const std::vector<uint8_t>& dims, const std::vector<int8_t>& val)
   {
     Clear_p(this->m_Format, this->m_Values);
     this->m_Dims = dims;
     this->m_Format = Byte;
     Voidify_p(this->GetDimensionsProduct(), val, this->m_Values);
   };

   /**
   * @fn void MetaDataInfo::SetValues(const std::vector<uint8_t>& dims, const std::vector<int16_t>& val)
   * Overwrite the data to store @a val in an Integer format with the dimensions @a dims.
   * If the product of the dimensions is not equal to the size of @a val,
   * then the number of values is adjusted to fit with it. The value added is "0".
   * @warning Each dimension must be lower than 256.
   */
   void MetaDataInfo::SetValues(const std::vector<uint8_t>& dims, const std::vector<int16_t>& val)
   {
     Clear_p(this->m_Format, this->m_Values);
     this->m_Dims = dims;
     this->m_Format = Integer;
     Voidify_p(this->GetDimensionsProduct(), val, this->m_Values);

   };
   
   /**
   * @fn void MetaDataInfo::SetValues(const std::vector<uint8_t>& dims, const std::vector<float>& val)
   * Overwrite the data to store @a val in an Real format with the dimensions @a dims.
   * If the product of the dimensions is not equal to the size of @a val,
   * then the number of values is adjusted to fit with it. The value added is "0".
   * @warning Each dimension must be lower than 256.
   */
   void MetaDataInfo::SetValues(const std::vector<uint8_t>& dims, const std::vector<float>& val)
   {
     Clear_p(this->m_Format, this->m_Values);
     this->m_Dims = dims;
     this->m_Format = Real;
     Voidify_p(this->GetDimensionsProduct(), val, this->m_Values);
   };
   
   /**
   * @fn void MetaDataInfo::SetValues(const std::vector<uint8_t>& dims, const std::vector<std::string>& val)
   * Overwrite the data to store @a val in a Char format with the dimensions @a dims.
   * If the product of the dimensions is not equal to the size of @a val,
   * then the number of values is adjusted to fit with it. The value added is 
   * " " if the dimension is null. Otherwise, it is a string of white spaces.
   * The size of this string corresponds to the first dimension.
   * @warning Each dimension must be lower than 256.
   */
   void MetaDataInfo::SetValues(const std::vector<uint8_t>& dims, const std::vector<std::string>& val)
   {
     Clear_p(this->m_Format, this->m_Values);
     this->m_Dims = dims;
     this->m_Format = Char;
     std::vector<std::string> values = val;
     this->FillSource(values);
     Voidify_p(values, this->m_Values);
   };

  /**
   * @fn MetaDataInfo::Pointer MetaDataInfo::Clone() const
   * Returns a deep copy of the object as a smart pointer.
   */

  /**
   * Convert stored value at index @a idx into string.
   */
  const std::string MetaDataInfo::ToString(int idx) const
  {
    return Devoidify_p<std::string>(this->m_Format, this->m_Values, idx, String);
  };

  /**
   * Convert stored value at index @a idx into 8-bits integer.
   */
  int8_t MetaDataInfo::ToInt8(int idx) const
  {
    return Devoidify_p<int8_t>(this->m_Format, this->m_Values, idx, Int8);
  };

  /**
   * Convert stored value at index @a idx into 8-bits unsigned integer.
   */
  uint8_t MetaDataInfo::ToUInt8(int idx) const
  {
    return Devoidify_p<uint8_t>(this->m_Format, this->m_Values, idx, UInt8);
  };

  /**
   * Convert stored value at index @a idx into 16-bits integer.
   */
  int16_t MetaDataInfo::ToInt16(int idx) const
  {
    return Devoidify_p<int16_t>(this->m_Format, this->m_Values, idx, Int16);
  };

  /**
   * Convert stored value at index @a idx into 16-bits unsigned integer.
   */
  uint16_t MetaDataInfo::ToUInt16(int idx) const
  {
    return Devoidify_p<uint16_t>(this->m_Format, this->m_Values, idx, UInt16);
  };

  /**
   * Convert stored value at index @a idx into integer.
   */
  int MetaDataInfo::ToInt(int idx) const
  {
    return Devoidify_p<int>(this->m_Format, this->m_Values, idx, Int);
  };

  /**
   * Convert stored value at index @a idx into unsigned integer.
   */
  unsigned int MetaDataInfo::ToUInt(int idx) const
  {
    return Devoidify_p<unsigned int>(this->m_Format, this->m_Values, idx, UInt);
  };

  /**
   * Convert stored value at index @a idx into float.
   */
  float MetaDataInfo::ToFloat(int idx) const
  {
    return Devoidify_p<float>(this->m_Format, this->m_Values, idx, Float);
  };

  /**
   * Convert stored value at index @a idx into double.
   */
  double MetaDataInfo::ToDouble(int idx) const
  {
    return Devoidify_p<double>(this->m_Format, this->m_Values, idx, Double);
  };

  /**
   * Convert stored value at index @a idx into string.
   */
  const std::vector<std::string> MetaDataInfo::ToString() const
  {
    return Devoidify_p<std::string>(this->m_Format, this->m_Values, String);
  };
  
  /**
   * Convert stored values into a vector of strings.
   */
  void MetaDataInfo::ToString(std::vector<std::string>& val) const
  {
    Devoidify_p(this->m_Format, this->m_Values, val, String);
  };

  /**
   * Convert stored values into a vector of 8-bits integers.
   */
  const std::vector<int8_t> MetaDataInfo::ToInt8() const
  {
    return Devoidify_p<int8_t>(this->m_Format, this->m_Values, Int8);
  };

 /**
   * Convert stored values into a vector of 8-bits integers.
   */
   void MetaDataInfo::ToInt8(std::vector<int8_t>& val) const
  {
    Devoidify_p(this->m_Format, this->m_Values, val, Int8);
  };

  /**
   * Convert stored values into a vector of 8-bits unsigned integers.
   */
  const std::vector<uint8_t> MetaDataInfo::ToUInt8() const
  {
    return Devoidify_p<uint8_t>(this->m_Format, this->m_Values, UInt8);
  };

  /**
   * Convert stored values into a vector of 8-bits unsigned integers.
   */
  void MetaDataInfo::ToUInt8(std::vector<uint8_t>& val) const
  {
    Devoidify_p(this->m_Format, this->m_Values, val, UInt8);
  };

  /**
   * Convert stored values into a vector of 16-bits integers.
   */
  const std::vector<int16_t> MetaDataInfo::ToInt16() const
  {
    return Devoidify_p<int16_t>(this->m_Format, this->m_Values, Int16);
  };

  /**
   * Convert stored values into a vector of 16-bits integers.
   */
  void MetaDataInfo::ToInt16(std::vector<int16_t>& val) const
  {
    Devoidify_p(this->m_Format, this->m_Values, val, Int16);
  };

  /**
   * Convert stored values into a vector of 16-bis unsigned integers.
   */
  const std::vector<uint16_t> MetaDataInfo::ToUInt16() const
  {
    return Devoidify_p<uint16_t>(this->m_Format, this->m_Values, UInt16);
  };

  /**
   * Convert stored values into a vector of 16-bits unsigned integers.
   */
  void MetaDataInfo::ToUInt16(std::vector<uint16_t>& val) const
  {
    Devoidify_p(this->m_Format, this->m_Values, val, UInt16);
  };

  /**
   * Convert stored values into a vector of integers.
   */
  const std::vector<int> MetaDataInfo::ToInt() const
  {
    return Devoidify_p<int>(this->m_Format, this->m_Values, Int);
  };

  /**
   * Convert stored values into a vector of integers.
   */
  void MetaDataInfo::ToInt(std::vector<int>& val) const
  {
    Devoidify_p(this->m_Format, this->m_Values, val, Int);
  };

  /**
   * Convert stored values into a vector of unsigned integers.
   */
  const std::vector<unsigned int> MetaDataInfo::ToUInt() const
  {
    return Devoidify_p<unsigned int>(this->m_Format, this->m_Values, UInt);
  };

  /**
   * Convert stored values into a vector of unsigned integers.
   */
  void MetaDataInfo::ToUInt(std::vector<unsigned int>& val) const
  {
    Devoidify_p(this->m_Format, this->m_Values, val, UInt);
  };

  /**
   * Convert stored values into a vector of floats.
   */
  const std::vector<float> MetaDataInfo::ToFloat() const 
  {
    return Devoidify_p<float>(this->m_Format, this->m_Values, Float);
  };

  /**
   * Convert stored values into a vector of floats.
   */
  void MetaDataInfo::ToFloat(std::vector<float>& val) const
  {
    Devoidify_p(this->m_Format, this->m_Values, val, Float);
  };

  /**
   * Convert stored values into a vector of doubles.
   */
  const std::vector<double> MetaDataInfo::ToDouble() const 
  {
    return Devoidify_p<double>(this->m_Format, this->m_Values, Double);
  };

  /**
   * Convert stored values into a vector of doubles.
   */
  void MetaDataInfo::ToDouble(std::vector<double>& val) const
  {
    Devoidify_p(this->m_Format, this->m_Values, val, Double);
  };

  
  /**
   * Equality operator.
   */
  bool operator==(const MetaDataInfo& rLHS, const MetaDataInfo& rRHS)
  {
    if (rLHS.m_Format != rRHS.m_Format) 
      return false;
    if (rLHS.m_Dims != rRHS.m_Dims) 
      return false;
    bool equal = false;
    switch (rLHS.m_Format)
    {
    case MetaDataInfo::Char:
      equal = OperatorEqual_p<std::string>(rLHS.m_Values, rRHS.m_Values);
      break;
    case MetaDataInfo::Byte:
      equal = OperatorEqual_p<char>(rLHS.m_Values, rRHS.m_Values);
      break;
    case MetaDataInfo::Integer:
      equal = OperatorEqual_p<int16_t>(rLHS.m_Values, rRHS.m_Values);
      break;
    case MetaDataInfo::Real:
      equal = OperatorEqual_p<float>(rLHS.m_Values, rRHS.m_Values);
      break;
    }
    return equal;
  };
  
  /**
   * @fn friend bool MetaDataInfo::operator!=(const MetaDataInfo& rLHS, const MetaDataInfo& rRHS)
   * Inequality operator.
   */

  /**
   * Constructor which store @a val in a Char format with a dimension equal to 1.
   * The dimension's value is equal to the size of @a val.
   */
  MetaDataInfo::MetaDataInfo(std::string val)
  : m_Dims(std::vector<uint8_t>(1,static_cast<uint8_t>(val.length())))
  {
    this->m_Format = Char;
    Voidify_p(std::vector<std::string>(1, val), this->m_Values);
  };

  /**
   * Constructor which store @a val in a Char format as an array (array's dimensions automalicaly generated)
   * @warning The number of values must be lower than 256 and the maximum length for the strings is equal to 255.
   */
  MetaDataInfo::MetaDataInfo(const std::vector<std::string>& val)
  {
    std::vector<std::string> values = val;
    this->FillDimensions(values);
    this->FillSource(values);
    this->m_Format = Char;
    Voidify_p(values, this->m_Values);
  };

  /**
   * Constructor which store @a val in a Byte format with the dimensions @a dims.
   * If the product of the dimensions is not equal to the size of @a val,
   * then the number of values is adjusted to fit with it. The value added is "0".
   * @warning Each dimension must be lower than 256.
   */
  MetaDataInfo::MetaDataInfo(const std::vector<uint8_t>& dims, const std::vector<int8_t>& val)
  : m_Dims(dims)
  {
    this->m_Format = Byte;
    Voidify_p(this->GetDimensionsProduct(), val, this->m_Values);
  };
  
  /**
   * Constructor which store @a val in an Integer format with the dimensions @a dims.
   * If the product of the dimensions is not equal to the size of @a val,
   * then the number of values is adjusted to fit with it. The value added is "0".
   * @warning Each dimension must be lower than 256.
   */
  MetaDataInfo::MetaDataInfo(const std::vector<uint8_t>& dims, const std::vector<int16_t>& val)
  : m_Dims(dims)
  {
    this->m_Format = Integer;
    Voidify_p(this->GetDimensionsProduct(), val, this->m_Values);
  };

  /**
   * Constructor which store @a val in a Real format with the dimensions @a dims.
   * If the product of the dimensions is not equal to the size of @a val,
   * then the number of values is adjusted to fit with it. The value added is "0".
   * @warning Each dimension must be lower than 256.
   */
  MetaDataInfo::MetaDataInfo(const std::vector<uint8_t>& dims, const std::vector<float>& val)
  : m_Dims(dims)
  {
    this->m_Format = Real;
    Voidify_p(this->GetDimensionsProduct(), val, this->m_Values);
  };

  /**
   * Constructor which store @a val in a Char format with the dimensions @a dims.
   * If the product of the dimensions is not equal to the size of @a val,
   * then the number of values is adjusted to fit with it. The value added is 
   * " " if the dimension is null. Otherwise, it is a string of white spaces.
   * The size of this string corresponds to the first dimension.
   * @warning Each dimension must be lower than 256.
   */
  MetaDataInfo::MetaDataInfo(const std::vector<uint8_t>& dims, const std::vector<std::string>& val)
  : m_Dims(dims)
  {
    this->m_Format = Char;
    std::vector<std::string> values = val;  
    this->FillSource(values);
    Voidify_p(values, this->m_Values);
  };
   
  /**
   * Copy constructor
   */
  MetaDataInfo::MetaDataInfo(const MetaDataInfo& toCopy)
  : m_Dims(toCopy.m_Dims)
  {
    this->m_Format = toCopy.m_Format;
    Copy_p(this->m_Format, toCopy.m_Values, this->m_Values);
  };

  /*
   * Fills the the member MetaDataInfo::m_Dims for a vector of string
   */
  void MetaDataInfo::FillDimensions(const std::vector<std::string>& val)
  {
    this->m_Dims = std::vector<uint8_t>(2,0);
    this->m_Dims[1] = static_cast<uint8_t>(val.size());
    for (int i = 0 ; i < this->m_Dims[1] ; ++i)
    {
      int len = static_cast<int>(val[i].length());
      if (len > this->m_Dims[0])
        this->m_Dims[0] = len;
    }
  };

  /**
   * Adapts the vector @a val based on the values of the member 
   * MetaDataInfo::m_Dims.
   */
  void MetaDataInfo::FillSource(std::vector<std::string>& val) const
  {
    if (this->m_Dims.size() == 0)
    {
      if (val.size() != 1)
      {
        val.resize(1, " ");
        val[0].resize(1, ' ');
      }
    }
    else
    {
      int prod = this->GetDimensionsProduct(1);
      val.resize(prod, std::string(this->m_Dims[0], ' '));
      for (int i = 0 ; i < prod ; ++i)
        val[i].resize(this->m_Dims[0], ' ');
    }
  };
};

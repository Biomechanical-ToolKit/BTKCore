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

#include "btkMetaDataEntryValue.h"
#include "btkMacro.h"
#include "btkConvert.h"

namespace btk
{
  /**
   * @class MetaDataEntryValue
   * @brief Container class to store data of a MetaDataEntry object.
   *
   * @ingroup BTKCommon
   */
  
  /**
   * @enum MetaDataEntryValue::Format
   * Enums used to specify data format.
   */
  /**
   * @var MetaDataEntryValue::Format MetaDataEntryValue::CHAR
   * Enum value which represents the string.
   */
  /**
   * @var MetaDataEntryValue::Format MetaDataEntryValue::BYTE
   * Enum value which represents the signed 8-bit integers.
   */
  /**
   * @var MetaDataEntryValue::Format MetaDataEntryValue::INTEGER
   * Enum value which represents the the signed 16-bit integers.
   */
  /**
   * @var MetaDataEntryValue::Format MetaDataEntryValue::FLOAT
   * Enum value which represents the floats.
   */
  
  /**
   * @typedef MetaDataEntryValue::Pointer
   * Smart pointer of a MetaDataEntryValue.
   */
  
  /**
   * @typedef MetaDataEntryValue::ConstPointer
   * Smart const pointer of a MetaDataEntryValue.
   */
  
  /**
   * Creates a smart pointer from the MetaDataEntryValue(int8_t) constructor.
   */
  MetaDataEntryValue::Pointer MetaDataEntryValue::New(int8_t val)
  {
    return Pointer(new MetaDataEntryValue(val));
  };
  
  /**
   * Creates a smart pointer from the MetaDataEntryValue(int16_t) constructor.
   */
  MetaDataEntryValue::Pointer MetaDataEntryValue::New(int16_t val)
  {
    return Pointer(new MetaDataEntryValue(val));
  };
  
  /**
   * Creates a smart pointer from the MetaDataEntryValue(float) constructor.
   */
  MetaDataEntryValue::Pointer MetaDataEntryValue::New(float val)
  {
    return Pointer(new MetaDataEntryValue(val));
  };
  
  /**
   * Creates a smart pointer from the MetaDataEntryValue(std::string) constructor.
   */
  MetaDataEntryValue::Pointer MetaDataEntryValue::New(std::string val)
  {
    return Pointer(new MetaDataEntryValue(val));
  };
  
  /**
   * Creates a smart pointer from the MetaDataEntryValue(const std::vector<uint8_t>&, const std::vector<int8_t>&) constructor.
   */
  MetaDataEntryValue::Pointer MetaDataEntryValue::New(const std::vector<uint8_t>& dim, const std::vector<int8_t>& val)
  {
    return Pointer(new MetaDataEntryValue(dim, val));
  };
  
  /**
   * Creates a smart pointer from the MetaDataEntryValue(const std::vector<uint8_t>&, const std::vector<int16_t>&) constructor.
   */
  MetaDataEntryValue::Pointer MetaDataEntryValue::New(const std::vector<uint8_t>& dim, const std::vector<int16_t>& val)
  {
    return Pointer(new MetaDataEntryValue(dim, val));
  };
  
  /**
   * Creates a smart pointer from the MetaDataEntryValue(const std::vector<uint8_t>&, const std::vector<float>&) constructor.
   */
  MetaDataEntryValue::Pointer MetaDataEntryValue::New(const std::vector<uint8_t>& dim, const std::vector<float>& val)
  {
    return Pointer(new MetaDataEntryValue(dim, val));
  };
  
  /**
   * Creates a smart pointer from the MetaDataEntryValue(const std::vector<uint8_t>&, const std::vector<std::string>&) constructor.
   */
  MetaDataEntryValue::Pointer MetaDataEntryValue::New(const std::vector<uint8_t>& dim, const std::vector<std::string>& val)
  {
    return Pointer(new MetaDataEntryValue(dim, val));
  };

  /**
   * @fn Format MetaDataEntryValue::GetFormat() const
   * Returns the format of the values.
   */
  
  /**
   * Sets the format of the values and adapt the values.
   * This method erases the values and set default values ("0" or a string with white spaces).
   */
  void MetaDataEntryValue::SetFormat(Format format)
  {
    if (this->m_Format == format)
      return;
    if (format == CHAR)
    {
      this->m_Values.clear();
      if (this->m_Dims.size() == 0)
        this->m_Values.push_back(" ");
      else
        this->m_Values = std::vector<std::string>(this->prod(1), std::string(this->m_Dims[0], ' '));
    }
    else
    {
      if ((this->m_Format == CHAR) || (this->m_Format == FLOAT)
          || ((this->m_Format == INTEGER) && (format == BYTE)))
        this->m_Values = std::vector<std::string>(this->prod(0), "0");
    }
    this->m_Format = format;
    
  };

  /**
   * Returns the dimension at the specified @a idx or 0 if @a idx is out of range.
   */
  uint8_t MetaDataEntryValue::GetDimension(int idx) const
  {
    if (idx >= static_cast<int>(this->m_Dims.size()))
    {
      btkErrorMacro("Out of range");
      return 0;
    }
    else
      return this->m_Dims[idx];
  };

  /**
   * Sets @a val as the new dimension at the specified @a idx or do nothing if @a idx is out of range.
   * This method alterates the values' structure and set default values ("0" or a string with white spaces).
   */
  void MetaDataEntryValue::SetDimension(int idx, uint8_t val)
  {
    if (idx >= static_cast<int>(this->m_Dims.size()))
    {
      btkErrorMacro("Out of range");
      return;
    }
    if (this->m_Dims[idx] == val)
      return;
    int oldProd = this->prod();
    uint8_t oldValue = this->m_Dims[idx];
    this->m_Dims[idx] = val;
    int prod = this->prod();
    std::string blank;
    if (this->m_Format == CHAR)
    {
      if (idx == 0)
      {
        std::vector<std::string>::iterator it = this->m_Values.begin();
        while (it != this->m_Values.end())
        {
          it->resize(val, ' ');
          ++it;
        }
      }
      else
        blank = std::string(this->m_Dims[0] , ' ');
    }
    else
      blank = "0";
    if ( (this->m_Format != CHAR) || (idx != 0) )
    {
      int diffNb = val - oldValue;
      int repeat = this->prod(idx + 1);
      if (diffNb < 0)
      {
        int inc = 1;
        int step = prod / repeat;
        if (this->m_Format == CHAR)
          step = step / this->m_Dims[0];
        int elts = step / val;
        diffNb = diffNb * (-1);
        while(inc <= repeat)
        {
          std::vector<std::string>::iterator itStart = this->m_Values.begin();
          std::vector<std::string>::iterator itEnd = this->m_Values.begin();
          std::advance(itStart, step * inc);
          std::advance(itEnd, step * inc + diffNb * elts);
          this->m_Values.erase(itStart, itEnd);
          ++inc;
        }   
      }
      else
      {
        int inc = repeat;
        int step = oldProd / repeat;
        if (this->m_Format == CHAR)
          step = step / this->m_Dims[0];
        int elts = step / oldValue;
        while(inc > 0)
        {
          std::vector<std::string>::iterator it = this->m_Values.begin();
          std::advance(it, step * inc);
          this->m_Values.insert(it, diffNb * elts, blank);
          --inc;
        }
      }
    }
  };

  /**
   * @fn const std::vector<uint8_t>& MetaDataEntryValue::GetDimensions() const 
   * Returns the dimensions associated with the values.
   */
  
  /**
   * Sets @a dims as the new dimensions and adapt the values and their structure.
   * This method can add default values ("0" or a string with white spaces).
   */
  void MetaDataEntryValue::SetDimensions(const std::vector<uint8_t>& dims)
  {
    this->m_Values.clear();
    if (dims.size() == 0)
    {
      if (this->m_Format == CHAR)
        this->m_Values.push_back(" ");
      else
        this->m_Values.push_back("0");
    }
    else
    {
      if (this->m_Format == CHAR)
      {
        int prod = this->prod(1);
        this->m_Values.resize(prod, std::string(this->m_Dims[0], ' '));
        for (int i = 0 ; i < prod ; ++i)
        {
          this->m_Values[i].resize(this->m_Dims[0], ' ');
        }
      }
      else
      {
        int prod = this->prod();
        this->m_Values.resize(prod, "0");
      }
    }
  };

  /**
   * Resize the dimensions and adapt the values and their structure.
   * If the number of dimensions is greater, then the new dimensions are equal to 1.
   * If the number of dimensions is lower, then the value are shrinked.
   */
  void MetaDataEntryValue::ResizeDimensions(int nb)
  {
    if (nb == this->m_Dims.size())
      return;
    if (nb < static_cast<int>(this->m_Dims.size()))
    {
      this->m_Dims.resize(nb, 1);    
      int inc = 0;
      if (this->m_Format == CHAR)
        inc = 1;
      this->m_Values.resize(this->prod(inc));
      if (this->m_Format == CHAR && nb == 0)
        this->m_Values[0] = this->m_Values[0][0];
    }
    else
      this->m_Dims.resize(nb, 1);
    
  };

  /**
   * Returns the value at the specified @a idx or 0 if @a idx is out of range.
   */
  const std::string MetaDataEntryValue::GetValue(int idx) const
  {
    if (idx >= static_cast<int>(this->m_Values.size()))
    {
      btkErrorMacro("Out of range");
      if (this->m_Format == CHAR)
        return "";
      else
        return "0";
    }
    else
      return this->m_Values[idx];
  };

  /**
   * Sets @a val as the value at the specified @a idx if possible.
   *   - This method doesn't do anything if the idx is out of range.
   *   - This method doesn't do anything if the new value doesn't have the same format.
   */
  void MetaDataEntryValue::SetValue(int idx, int8_t val)
  {
    if (idx >= static_cast<int>(this->m_Values.size()))
    {
      btkErrorMacro("Out of range");
      return;
    }
    if (this->m_Format != BYTE)
    {
      btkErrorMacro("Invalid input format");
      return;
    }
    this->m_Values[idx] = ToString(val);
  };

  /**
   * Sets @a val as the value at the specified @a idx if possible.
   *   - This method doesn't do anything if the idx is out of range.
   *   - This method doesn't do anything if the new value doesn't have the same format.
   */
  void MetaDataEntryValue::SetValue(int idx, int16_t val)
  {
    if (idx >= static_cast<int>(this->m_Values.size()))
    {
      btkErrorMacro("Out of range");
      return;
    }
    if (this->m_Format != INTEGER)
    {
      btkErrorMacro("Invalid input format");
      return;
    }
    this->m_Values[idx] = ToString(val);
  };

  /**
   * Sets @a val as the value at the specified @a idx if possible.
   *   - This method doesn't do anything if the idx is out of range.
   *   - This method doesn't do anything if the new value doesn't have the same format.
   */
  void MetaDataEntryValue::SetValue(int idx, float val)
  {
    if (idx >= static_cast<int>(this->m_Values.size()))
    {
      btkErrorMacro("Out of range");
      return;
    }
    if (this->m_Format != FLOAT)
    {
      btkErrorMacro("Invalid input format");
      return;
    }
    this->m_Values[idx] = ToString(val);
  };

  /**
   * Sets @a val as the value at the specified @a idx if possible.
   *   - This method doesn't do anything if the idx is out of range.
   *   - This method doesn't do anything if the new value doesn't have the same format.
   */
  void MetaDataEntryValue::SetValue(int idx, const std::string& val)
  {
    if (idx >= static_cast<int>(this->m_Values.size()))
    {
      btkErrorMacro("Out of range");
      return;
    }
    if (this->m_Format != CHAR)
    {
      btkErrorMacro("Invalid input format");
      return;
    }
    this->m_Values[idx] = val;
  };
  
  /**
   * @fn const std::vector<std::string>& MetaDataEntryValue::GetValues() const
   * Returns the values as a vector of strings.
   */

  /**
   * Overwrite the data to store @a val in a BYTE format with a null dimension.
   */
  void MetaDataEntryValue::SetValues(int8_t val)
  {
    this->m_Format = BYTE;
    this->m_Dims = std::vector<uint8_t>(0);
    this->m_Values = std::vector<std::string>(1, ToString(val));
  };

  /**
   * Overwrite the data to store @a val in a INTEGER format with a null dimension.
   */
  void MetaDataEntryValue::SetValues(int16_t val)
  {
    this->m_Format = INTEGER;
    this->m_Dims = std::vector<uint8_t>(0);
    this->m_Values = std::vector<std::string>(1, ToString(val));
  };

  /**
   * Overwrite the data to store @a val in a FLOAT format with a null dimension.
   */
  void MetaDataEntryValue::SetValues(float val)
  {
    this->m_Format = FLOAT;
    this->m_Dims = std::vector<uint8_t>(0);
    this->m_Values = std::vector<std::string>(1, ToString(val));
  };

  /**
   * Overwrite the data to store @a val in a CHAR format with one dimension equals 
   * to the size of the strings.
   */
  void MetaDataEntryValue::SetValues(const std::string& val)
  {
    this->m_Format = CHAR;
    this->m_Dims = std::vector<uint8_t>(0);
    this->m_Values = std::vector<std::string>(1, val);
  };

  /**
   * Overwrite the data to store @a val in an BYTE format with the dimensions @a dims.
   * If the product of the dimensions is not equal to the size of @a val,
   * then the number of values is adjusted to fit with it. The value added is "0".
   */
  void MetaDataEntryValue::SetValues(const std::vector<uint8_t>& dims, const std::vector<int8_t>& val)
  {
    this->m_Format = BYTE;
    this->m_Dims = dims;
    ToString(val, this->m_Values);
    this->m_Values.resize(this->prod(), "0");
  };

  /**
   * Overwrite the data to store @a val in an INTEGER format with the dimensions @a dims.
   * If the product of the dimensions is not equal to the size of @a val,
   * then the number of values is adjusted to fit with it. The value added is "0".
   */
  void MetaDataEntryValue::SetValues(const std::vector<uint8_t>& dims, const std::vector<int16_t>& val)
  {
    this->m_Format = INTEGER;
    this->m_Dims = dims;
    ToString(val, this->m_Values);
    this->m_Values.resize(this->prod(), "0");
  };

  /**
   * Overwrite the data to store @a val in an FLOAT format with the dimensions @a dims.
   * If the product of the dimensions is not equal to the size of @a val,
   * then the number of values is adjusted to fit with it. The value added is "0".
   */
  void MetaDataEntryValue::SetValues(const std::vector<uint8_t>& dims, const std::vector<float>& val)
  {
    this->m_Format = FLOAT;
    this->m_Dims = dims;
    ToString(val, this->m_Values);
    this->m_Values.resize(this->prod(), "0");
  };

  /**
   * Overwrite the data to store @a val in a CHAR format with the dimensions @a dims.
   * If the product of the dimensions is not equal to the size of @a val,
   * then the number of values is adjusted to fit with it. The value added is 
   * " " if the dimension is null. Otherwise, it is a string of white spaces.
   * The size of this string corresponds to the first dimension.
   */
  void MetaDataEntryValue::SetValues(const std::vector<uint8_t>& dims, const std::vector<std::string>& val)
  {
    this->m_Format = CHAR;
    this->m_Dims = dims;
    ToString(val, this->m_Values);
    if (this->m_Dims.size() == 0)
    {
      if (this->m_Values.size() != 1)
      {
        this->m_Values.resize(1, " ");
        this->m_Values[0].resize(1, ' ');
      }
    }
    else
    {
      int prod = this->prod(1);
      this->m_Values.resize(prod, std::string(this->m_Dims[0], ' '));
      for (int i = 0 ; i < prod ; ++i)
      {
        this->m_Values[i].resize(this->m_Dims[0], ' ');
      }
    }
  };

  /**
   * @fn MetaDataEntryValue::Pointer MetaDataEntryValue::Clone() const
   * Returns a deep copy of the object as a smart pointer.
   */
  
  /**
   * Equality operator.
   */
  bool operator==(const MetaDataEntryValue& rLHS, const MetaDataEntryValue& rRHS)
  {
    if (rLHS.m_Format != rRHS.m_Format) 
      return false;
    if (rLHS.m_Dims != rRHS.m_Dims) 
      return false;
    if (rLHS.m_Values != rRHS.m_Values) 
      return false;
    return true;
  };
  
  /**
   * @fn friend bool MetaDataEntryValue::operator!=(const MetaDataEntryValue& rLHS, const MetaDataEntryValue& rRHS)
   * Inequality operator.
   */
  
  /**
   * Constructor which store @a val in a BYTE format with a null dimension.
   */
  MetaDataEntryValue::MetaDataEntryValue(int8_t val)
  : m_Dims(std::vector<uint8_t>(0)), 
  m_Values(std::vector<std::string>(1, ToString(val)))
  {
    this->m_Format = BYTE;
  };
  
  /**
   * Constructor which store @a val in an INTEGER format with a null dimension.
   */
  MetaDataEntryValue::MetaDataEntryValue(int16_t val)
  : m_Dims(std::vector<uint8_t>(0)), 
  m_Values(std::vector<std::string>(1, ToString(val)))
  {
    this->m_Format = INTEGER;
  };
  
  /**
   * Constructor which store @a val in a FLOAT format with a null dimension.
   */
  MetaDataEntryValue::MetaDataEntryValue(float val)
  : m_Dims(std::vector<uint8_t>(0)), 
  m_Values(std::vector<std::string>(1, ToString(val)))
  {
    this->m_Format = FLOAT;
  };
  
  /**
   * Constructor which store @a val in a CHAR format with a dimension equal to 1.
   * The dimension's value is equal to the size of @a val.
   */
  MetaDataEntryValue::MetaDataEntryValue(std::string val)
  : m_Dims(std::vector<uint8_t>(1, val.size())), 
  m_Values(std::vector<std::string>(1, val))
  {
    this->m_Format = CHAR;
  };
  
  /**
   * Constructor which store @a val in a BYTE format with the dimensions @a dims.
   * If the product of the dimensions is not equal to the size of @a val,
   * then the number of values is adjusted to fit with it. The value added is "0".
   */
  MetaDataEntryValue::MetaDataEntryValue(const std::vector<uint8_t>& dims, const std::vector<int8_t>& val)
  : m_Dims(dims)
  {
    this->m_Format = BYTE;
    ToString(val, this->m_Values);
    this->m_Values.resize(this->prod(), "0");
  };
  
  /**
   * Constructor which store @a val in an INTEGER format with the dimensions @a dims.
   * If the product of the dimensions is not equal to the size of @a val,
   * then the number of values is adjusted to fit with it. The value added is "0".
   */
  MetaDataEntryValue::MetaDataEntryValue(const std::vector<uint8_t>& dims, const std::vector<int16_t>& val)
  : m_Dims(dims)
  {
    this->m_Format = INTEGER;
    ToString(val, this->m_Values);
    this->m_Values.resize(this->prod(), "0");
  };
  
  /**
   * Constructor which store @a val in a FLOAT format with the dimensions @a dims.
   * If the product of the dimensions is not equal to the size of @a val,
   * then the number of values is adjusted to fit with it. The value added is "0".
   */
  MetaDataEntryValue::MetaDataEntryValue(const std::vector<uint8_t>& dims, const std::vector<float>& val)
  : m_Dims(dims)
  {
    this->m_Format = FLOAT;
    ToString(val, this->m_Values);
    this->m_Values.resize(this->prod(), "0");
  };
  
  /**
   * Constructor which store @a val in a CHAR format with the dimensions @a dims.
   * If the product of the dimensions is not equal to the size of @a val,
   * then the number of values is adjusted to fit with it. The value added is 
   * " " if the dimension is null. Otherwise, it is a string of white spaces.
   * The size of this string corresponds to the first dimension.
   */
  MetaDataEntryValue::MetaDataEntryValue(const std::vector<uint8_t>& dims, const std::vector<std::string>& val)
  : m_Dims(dims)
  {
    this->m_Format = CHAR;
    this->m_Values = val;
    if (this->m_Dims.size() == 0)
    {
      if (this->m_Values.size() != 1)
      {
        this->m_Values.resize(1, " ");
        this->m_Values[0].resize(1, ' ');
      }
    }
    else
    {
      int prod = this->prod(1);
      this->m_Values.resize(prod, std::string(this->m_Dims[0], ' '));
      for (int i = 0 ; i < prod ; ++i)
      {
        this->m_Values[i].resize(this->m_Dims[0], ' ');
      }
    }
  };
  
  /**
   * Compute the product of the dimensions from the idx @a start.
   */
  int MetaDataEntryValue::prod(int start) const
  {
    int prod = 1;
    int inc = start;
    while (inc < static_cast<int>(this->m_Dims.size()))
      prod *= this->m_Dims[inc++];
    return prod;
  };
  
  MetaDataEntryValue::MetaDataEntryValue(const MetaDataEntryValue& toCopy)
  : m_Dims(toCopy.m_Dims), m_Values(toCopy.m_Values)
  {
    this->m_Format = toCopy.m_Format;
  };
};
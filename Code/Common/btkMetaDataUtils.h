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

#ifndef __btkMetaDataUtils_h
#define __btkMetaDataUtils_h

#include "btkMetaData.h"
#include "btkConvert.h"

namespace btk
{
  /**
   * Collapse the @a parent children entries' values starting with the string @a baselabel
   * and incrementing (for example: LABELS, LABELS2, LABELS3). The entries'
   * values are stored in @a target.
   *
   * The input @a targetFinalSize can be used to fox the number of values 
   * to collpase (by default: -1). The input @a blankReplacement can be used
   * to fill the @a target' values which have no corresponding in the @a parent (By default: default object constructor).
   *
   * @ingroup BTKCommon
   */
  template <typename T>
  void MetaDataCollapseChildrenValues(std::vector<T>& target,
                                      MetaData::ConstPointer parent,
                                      const std::string& baselabel,
                                      int targetFinalSize = -1,
                                      const T& blankReplacement = T())
  {
    target.clear();
    int collapsedNumber = 0, inc = 2;
    std::string label = baselabel;
    if (parent.get() != 0)
    {
      while (1)
      {
        MetaData::ConstIterator it = parent->FindChild(label);
        if (it == parent->End())
          break;
        std::vector<T> values;
        MetaDataCollapseChildrenValues_p(values, (*it)->GetInfo());
        typename std::vector<T>::iterator itVal = values.begin();
        size_t num = target.size() + values.size();
        if (values.size() != 0)
        {
          if ((static_cast<int>(num) >= targetFinalSize) && (targetFinalSize != -1))
            std::advance(itVal, targetFinalSize - target.size());
          else
            itVal = values.end();
          target.insert(target.end(), values.begin(), itVal);
          collapsedNumber = static_cast<int>(target.size());
        }
        if (collapsedNumber == targetFinalSize)
          break;
        label = baselabel + ToString(inc);
        ++inc;
      }
    }
    if (collapsedNumber < targetFinalSize)
    {
      target.resize(targetFinalSize);
      for (int inc = collapsedNumber ; inc < targetFinalSize ; ++inc)
        target[inc] = MetaDataCollapseChildrenValues_p(blankReplacement, inc + 1);
    }
  };

  // Strange parsing error with Doxygen... The commented lines are required
  //! @cond
  BTK_COMMON_EXPORT MetaData::Pointer MetaDataCreateChild(MetaData::Pointer parent, const std::string &label);
  //! @endcond

  /**
   * Creates an new MetaData or replaces its data if it already exists.
   *
   * This method constructs a MetaData with a single value @a val, with the label @a label, an empty description and is unlocked.
   *
   * @ingroup BTKCommon
   */
  template <typename T>
  void MetaDataCreateChild(MetaData::Pointer parent, const std::string& label, const T& val)
  {
    if (parent.get() != 0)
    {
      MetaData::Iterator it = parent->FindChild(label);
      if (it != parent->End())
      {
        (*it)->SetDescription("");
        if ((*it)->HasInfo())
          (*it)->GetInfo()->SetValues(val);
        else
          (*it)->SetInfo(MetaDataInfo::New(val));
        (*it)->SetUnlockState(true);
      }
      else
        parent->AppendChild(MetaData::New(label, val, "", true));
    }
    else
      btkErrorMacro("No parent.");
  };

  /**
   * @fn template <typename T> void MetaDataCreateChild(MetaData::Pointer parent, const std::string& label, const std::vector<T>& val, int inc = 1)
   * Creates an new MetaData or replaces its data if it already exists.
   *
   * This method constructs a MetaData with an 1D vector @a val as values, with the label @a label, an empty description and is unlocked.
   * This method is a recursive method which give the possibility to create more than one entry if the vector's length is greater or equal to 256. The parameter @a inc give the value added to the label to give the next label. For @a inc=1, The label is not modified.
   *
   * @ingroup BTKCommon
   */
  template <typename T>
  void MetaDataCreateChild(MetaData::Pointer parent, const std::string& label, const std::vector<T>& val, int inc = 1)
  {
    if (parent.get() != 0)
    {
      if (val.size() >= 256)
      {
        std::vector<T> val1, val2;
        typename std::vector<T>::const_iterator it = val.begin();
        std::advance(it, 255);
        val1.assign(val.begin(), it);
        val2.assign(it, val.end());
        MetaDataCreateChild(parent, label, val1, inc);
        MetaDataCreateChild(parent, label, val2, inc + 1);
      }
      else
      {
        std::string l = label;
        if (inc != 1)
          l += ToString(inc);
        MetaData::Iterator it = parent->FindChild(l);
        if (it != parent->End())
        {
          (*it)->SetDescription("");
          if ((*it)->HasInfo())
            (*it)->GetInfo()->SetValues(val);
          else
            (*it)->SetInfo(MetaDataInfo::New(val));
          (*it)->SetUnlockState(true);
        }
        else
          parent->AppendChild(MetaData::New(l, val));
      }
    }
    else
      btkErrorMacro("No parent.");
  };
  
  /**
   * @fn template <typename T> void MetaDataCreateChild2D(MetaData::Pointer parent, const std::string& label, const std::vector<T>& val, int numCol, int inc = 1)
   * Creates an new MetaData or replaces its data if it already exists.
   *
   * This method constructs an unlocked MetaData with an 1D vector @a val as values, an integer @a numCol for the number of column, a string @a label for the label and an empty description.
   * This method is a recursive method which give the possibility to create more than one entry if the vector's length is greater or equal to 256 * @a numCol. The parameter @a inc give the value added to the label to give the next label. For @a inc=1, The label is not modified.
   * The number of column must be lower than 256. The number of columns is written in the first dimension.
   *
   * @ingroup BTKCommon
   */
  template <typename T>
  void MetaDataCreateChild2D(MetaData::Pointer parent, const std::string& label, const std::vector<T>& val, int numCol, int inc = 1)
  {
    if (parent.get() != 0)
    {
      if (numCol >= 256)
      {
        btkErrorMacro("Number of column is greater or equal to 256.");
        return;
      }
      if (val.size() / numCol >= 256)
      {
        std::vector<T> val1, val2;
        typename std::vector<T>::const_iterator it = val.begin();
        std::advance(it, 255 * numCol);
        val1.assign(val.begin(), it);
        val2.assign(it, val.end());
        MetaDataCreateChild2D(parent, label, val1, numCol, inc);
        MetaDataCreateChild2D(parent, label, val2, numCol, inc + 1);
      }
      else
      {
        std::string l = label;
        if (inc != 1)
          l += ToString(inc);
        MetaData::Iterator it = parent->FindChild(l);
        std::vector<uint8_t> dims(2, static_cast<uint8_t>(numCol));
        dims[1] = static_cast<uint8_t>(val.size() / 2);
        if (it != parent->End())
        {
          (*it)->SetDescription("");
          if ((*it)->HasInfo())
            (*it)->GetInfo()->SetValues(dims, val);
          else
            (*it)->SetInfo(MetaDataInfo::New(dims, val));
          (*it)->SetUnlockState(true);
        }
        else
          parent->AppendChild(MetaData::New(l, dims, val));
      }
    }
    else
      btkErrorMacro("No parent.");
  };

  // --------- Private methods -------------
  // MetaDataCollapseChildrenValues_p: From void* to T
  template <typename T>
  inline void MetaDataCollapseChildrenValues_p(
      std::vector<T>& values,
      MetaDataInfo::ConstPointer info)
  {
    btkErrorMacro("Error during metadata's value collapsing. Generic method used.");
  };

  template <>
  inline void MetaDataCollapseChildrenValues_p<std::string>(
      std::vector<std::string>& values,
      MetaDataInfo::ConstPointer info)
  {
    info->ToString(values);
    for (size_t i = 0 ; i < values.size() ; ++i)
    {
      std::string str = values[i];
      str = str.erase(str.find_last_not_of(' ') + 1);
      str = str.erase(0, str.find_first_not_of(' '));
      values[i] = str;
    }
  };

  template <>
  inline void MetaDataCollapseChildrenValues_p<int8_t>(
      std::vector<int8_t>& values,
      MetaDataInfo::ConstPointer info)
  {
    info->ToInt8(values);
  };

  template <>
  inline void MetaDataCollapseChildrenValues_p<uint8_t>(
      std::vector<uint8_t>& values,
      MetaDataInfo::ConstPointer info)
  {
    info->ToUInt8(values);
  };

  template <>
  inline void MetaDataCollapseChildrenValues_p<int16_t>(
      std::vector<int16_t>& values,
      MetaDataInfo::ConstPointer info)
  {
    info->ToInt16(values); 
  };

  template <>
  inline void MetaDataCollapseChildrenValues_p<uint16_t>(
      std::vector<uint16_t>& values,
      MetaDataInfo::ConstPointer info)
  {
    info->ToUInt16(values); 
  };

  template <>
  inline void MetaDataCollapseChildrenValues_p<int>(
      std::vector<int>& values,
      MetaDataInfo::ConstPointer info)
  {
    info->ToInt(values); 
  };

  template <>
  inline void MetaDataCollapseChildrenValues_p<unsigned int>(
      std::vector<unsigned int>& values,
      MetaDataInfo::ConstPointer info)
  {
    info->ToUInt(values); 
  };

  template <>
  inline void MetaDataCollapseChildrenValues_p<float>(
      std::vector<float>& values,
      MetaDataInfo::ConstPointer info)
  {
    info->ToFloat(values);
  }

  template <>
  inline void MetaDataCollapseChildrenValues_p<double>(
      std::vector<double>& values,
      MetaDataInfo::ConstPointer info)
  {
    info->ToDouble(values);
  }

  // MetaDataCollapseChildrenValues_p: Blank replacement
  template <typename T>
  inline const T MetaDataCollapseChildrenValues_p(const T& blank, int idx)
  {
    btkNotUsed(idx);
    return blank;
  };

  template <>
  inline const std::string MetaDataCollapseChildrenValues_p<std::string>(
      const std::string& blank, int idx)
  {
    return blank + ToString(idx);
  };
};

#endif // __btkMetaDataUtils_h


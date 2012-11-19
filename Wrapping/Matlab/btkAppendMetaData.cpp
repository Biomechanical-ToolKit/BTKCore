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

#include <btkMetaDataUtils.h> // First (Fix for MSVC LNK2019)

#include "btkMEXObjectHandle.h"
#include "btkMXMetaData.h"

#include <btkAcquisition.h>

#include <algorithm>
#include <cctype>
 
// btkAppendMetaData(h, label)
// btkAppendMetaData(h, label, sublabel, ..., btkMetaDataInfo((CHAR|BYTE|INTEGER|REAL), values, numdims))
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  if (nrhs < 2)
    mexErrMsgTxt("Minimum of two inputs required.");
  if (nlhs > 1)
    mexErrMsgTxt("Too many output arguments.");

  int level = nrhs;
  size_t numberOfValues = 0;
  size_t maxDimSize = 7;
  btk::MetaDataInfo::Format f = btk::MetaDataInfo::Char;
  std::vector<uint8_t> dims;
  mxArray* mxValues;
  if (mxIsStruct(prhs[nrhs-1]))
  {
    // Check the number of elements (must be one element)
    if (mxGetNumberOfElements(prhs[nrhs-1]) != 1)
      mexErrMsgTxt("Only accept structure with one element.");
    mxArray* temp;
    // Check for the field  'format' 
    if ((temp = mxGetField(prhs[nrhs-1], 0, "format")) != NULL)
    {
      if (!mxIsChar(temp) || mxIsEmpty(temp))
        mexErrMsgTxt("The field 'format' must be set by a non-empty string.");
      size_t strlen_ = (mxGetM(temp) * mxGetN(temp) * sizeof(mxChar)) + 1;
      char* format = (char*)mxMalloc(strlen_);
      mxGetString(temp, format, strlen_);
      std::string uppercase = std::string(format);
      mxFree(format);
      std::transform(uppercase.begin(), uppercase.end(), uppercase.begin(), toupper);
      if (uppercase.compare("CHAR") == 0)
      {
        f = btk::MetaDataInfo::Char;
        maxDimSize = 6;
      }
      else if(uppercase.compare("BYTE") == 0)
        f = btk::MetaDataInfo::Byte;
      else if(uppercase.compare("INTEGER") == 0)
        f = btk::MetaDataInfo::Integer;
      else if(uppercase.compare("REAL") == 0)
        f = btk::MetaDataInfo::Real;
      else
        mexErrMsgTxt("The field 'format' must be set by one of the following format: Char, Byte, Integer or Real");
    }
    else
      mexErrMsgTxt("The structure doesn't contain the field 'format' to determine the type of data.");
    // Check for the field 'Values'
    if ((mxValues = mxGetField(prhs[nrhs-1], 0, "values")) != NULL)
    {
      if (!mxIsCell(mxValues) && (!mxIsNumeric(mxValues) || mxIsComplex(mxValues)))
        mexErrMsgTxt("The field 'format' must be set by cell of strings or an array of numerical values.");
      numberOfValues = mxGetNumberOfElements(mxValues);
      if ((f != btk::MetaDataInfo::Char) && (numberOfValues > 65535))
        mexErrMsgTxt("Number of values exceeds the maximum number (65535) available for each metadata.");
      mwSize dimsSize = mxGetNumberOfDimensions(mxValues);
      if (dimsSize > maxDimSize)
        mexErrMsgTxt("Number of dimensions exceeds the maximum number (7) available for each metadata.");
      const mwSize* dimsValues = mxGetDimensions(mxValues);
      std::vector<uint8_t> _dims;
      for (int i = static_cast<int>(dimsSize - 1) ; i >= 0 ; --i)
      {
        int dim = static_cast<int>(dimsValues[i]);
        if (dim > 255)
          mexErrMsgTxt("The dimension's value exceeds the maximum number (255) available for each dimension.");
        if (dim != 1)
          _dims.push_back(static_cast<uint8_t>(dim));
      }
      if (f == btk::MetaDataInfo::Char)
        _dims.push_back(0);
      dims.assign(_dims.rbegin(), _dims.rend());
      
    }
    else
      mexErrMsgTxt("The structure doesn't contain the field 'values'.");
      
    // Check for the field 'numdims'
    if ((temp = mxGetField(prhs[nrhs-1], 0, "numdims")) != NULL)
    {
      if (!mxIsNumeric(temp) || mxIsEmpty(temp) || mxIsComplex(temp) || (mxGetNumberOfElements(temp) != 1))
        mexErrMsgTxt("The field 'numdims' must be set by a single integer value.");
      int numDims = static_cast<int>(mxGetScalar(temp));
      if (numDims < dims.size())
        mexErrMsgTxt("The given number of dimensions is lower than the number of dimensions for the given values.");
      else if (numDims > maxDimSize)
        mexErrMsgTxt("The given number of dimensions exceeds the maximum number (7) available for each metadata.");
      dims.resize(numDims, 1);
    }
    else
      mexErrMsgTxt("The structure doesn't contain the field 'numdims'.");
    
    level -= 1;
  }
  for (int i = 1 ; i < level ; ++i)
  {
    if (!mxIsChar(prhs[i]) || mxIsEmpty(prhs[i]))
      mexErrMsgTxt("Metadata's label must be set by a non-empty string.");
  }

  btk::Acquisition::Pointer acq = btk_MOH_get_object<btk::Acquisition>(prhs[0]);
  btk::MetaData::Pointer md = acq->GetMetaData();
  
  for (int i = 1 ; i < level ; ++i)
  {
    size_t strlen_ = (mxGetM(prhs[i]) * mxGetN(prhs[i]) * sizeof(mxChar)) + 1;
    char* label = (char*)mxMalloc(strlen_);
    mxGetString(prhs[i], label, strlen_);
    btk::MetaData::ConstIterator it = md->FindChild(label);
    if (it == md->End())
    {
      btk::MetaData::Pointer c = btk::MetaData::New(label);
      md->AppendChild(c);
      md = c;
    }
    else
      md = *it;
    mxFree(label);
  }
  
  if (level == (nrhs - 1))
  {
    switch(f)
    {
    case btk::MetaDataInfo::Char:
      {
      std::vector<std::string> values = std::vector<std::string>(numberOfValues, "");
      size_t maxLen = 0;
      for (int i = 0 ; i < static_cast<int>(numberOfValues) ; ++i)
      {
        mxArray* temp = mxGetCell(mxValues, i);
        if (!temp || !mxIsChar(temp))
          mexErrMsgTxt("Error in the format of the values: only strings are accepted in cell.");
        size_t strlen_ = (mxGetM(temp) * mxGetN(temp) * sizeof(mxChar)) + 1;
        char* val = (char*)mxMalloc(strlen_);
        mxGetString(temp, val, strlen_);
        values[i] = std::string(val);
        mxFree(val);
        strlen_ = values[i].length();
        if (strlen_ > maxLen)
          maxLen = strlen_;
      }
      dims[0] = static_cast<uint8_t>(maxLen);
      int prod = 1;
      for (size_t i = 0 ; i < dims.size() ; ++i)
        prod *= dims[i];
      if (prod > 65535)
        mexErrMsgTxt("The number of characters exceed the maximum number (65535) available for a metadata.");
      md->SetInfo(btk::MetaDataInfo::New(dims, values));
      break;
      }
    case btk::MetaDataInfo::Byte:
      {
      double* temp = mxGetPr(mxValues);
      std::vector<int8_t> values = std::vector<int8_t>(numberOfValues, 0);
      for (size_t i = 0 ; i < numberOfValues ; ++i)
        values[i] = static_cast<int8_t>(temp[i]);
      md->SetInfo(btk::MetaDataInfo::New(dims, values));
      break;
      }
    case btk::MetaDataInfo::Integer:
      {
      double* temp = mxGetPr(mxValues);
      std::vector<int16_t> values = std::vector<int16_t>(numberOfValues, 0);
      for (size_t i = 0 ; i < numberOfValues ; ++i)
        values[i] = static_cast<int16_t>(temp[i]);
      md->SetInfo(btk::MetaDataInfo::New(dims, values));
      break;
      }
    case btk::MetaDataInfo::Real:
      {
      double* temp = mxGetPr(mxValues);
      std::vector<float> values = std::vector<float>(numberOfValues, 0);
      for (size_t i = 0 ; i < numberOfValues ; ++i)
        values[i] = static_cast<float>(temp[i]);
      md->SetInfo(btk::MetaDataInfo::New(dims, values));
      break;
      }
    }
  }
  
  plhs[0] = btkMXCreateMetaDataStructure(acq->GetMetaData());
};

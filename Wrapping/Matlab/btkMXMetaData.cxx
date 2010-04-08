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

#include "btkMex.h"
#if defined(SCI_MEX)
  #include "btkMXMetaData.h"
#endif
#include "btkMXObjectHandle.h"
#include "btkASCIIConverter.h"

#include <btkAcquisition.h>

// btkGetMetaData(h)
// btkGetMetaData(h, label)
// btkGetMetaData(h, label, sublabel)
// btkGetMetaData(h, label, sublabel, ...)
void btkMXCreateMetaDataStructure(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  if(nrhs < 1)
    mexErrMsgTxt("Minimum of one input required.");
  if (nlhs > 1)
    mexErrMsgTxt("Too many output arguments.");

  for (int i = 1 ; i < nrhs ; ++i)
  {
    if (!mxIsChar(prhs[i]) || mxIsEmpty(prhs[i]))
      mexErrMsgTxt("Metadata's label must be a non-empty string.");
  }

  btk::Acquisition::Pointer acq = btk_MOH_get_object<btk::Acquisition>(prhs[0]);
  btk::MetaData::Pointer md = acq->GetMetaData();
  for (int i = 1 ; i < nrhs ; ++i)
  {
    int strlen = (mxGetM(prhs[i]) * mxGetN(prhs[i]) * sizeof(mxChar)) + 1;
    char* label = (char*)mxMalloc(strlen);
    mxGetString(prhs[i], label, strlen);
    btk::MetaData::ConstIterator it = md->FindChild(label);
    mxFree(label);
    if (it == md->End())
    {
      md = btk::MetaData::Pointer();
      break;
    }
    else
      md = *it;
  }
  
  if (!md.get())
  {
    std::string err = "Unknown metadata's label: ";
    for (int i = 1 ; i < nrhs ; ++i)
    {
      if (i > 1)
        err += ":";
      int strlen = (mxGetM(prhs[i]) * mxGetN(prhs[i]) * sizeof(mxChar)) + 1;
      char* label = (char*)mxMalloc(strlen);
      mxGetString(prhs[i], label, strlen);
      err += std::string(label);
      mxFree(label);
    }
    err += ".";
    mexErrMsgTxt(err.c_str());
  }
  
  plhs[0] = btkMXCreateMetaDataStructure(md);
};

mxArray* btkMXCreateMetaDataStructure(btk::MetaData::Pointer md)
{
  const char* metaDataFieldnames[] = {"info", "children", "description"};
  mxArray* out = mxCreateStructMatrix(1, 1, 3, metaDataFieldnames);
  // Info
  if (md->HasInfo())
  {
    const char* metaDataInfoFieldnames[] = {"format", "values"};
    mxArray* info = mxCreateStructMatrix(1, 1, 2, metaDataInfoFieldnames);
    mxSetFieldByNumber(info, 0, 0, mxCreateString(md->GetInfo()->GetFormatAsString().c_str()));
    mxArray* values;
    mwSize ndim = md->GetInfo()->GetDimensions().size();
    mwSize* dims = 0;
    if ((ndim == 0) || ( (ndim == 1) && (md->GetInfo()->GetFormat() == btk::MetaDataInfo::Char)))
    {
      ndim = 1;
      dims = new mwSize[ndim];
      dims[0] = 1;
    }
    else
    {
      if (md->GetInfo()->GetFormat() == btk::MetaDataInfo::Char)
      {
        dims = new mwSize[ndim - 1];
        for (int i = 1 ; i < static_cast<int>(ndim); ++i)
            dims[i - 1] = md->GetInfo()->GetDimensions()[i];
        ndim -= 1;
      }
      else
      {
        dims = new mwSize[ndim];
        for (int i = 0 ; i < static_cast<int>(ndim) ; ++i)
            dims[i] = md->GetInfo()->GetDimensions()[i];
      }
    }
    int prod = md->GetInfo()->GetDimensionsProduct(); 
    switch(md->GetInfo()->GetFormat())
    {
      case btk::MetaDataInfo::Char:
      {
        int rows = md->GetInfo()->GetDimensionsProduct(1);        
        values = mxCreateCellArray(ndim, dims);
        for (int i = 0 ; i < rows ; ++i)
        {
          std::string str = md->GetInfo()->ToString(i);
          str = str.erase(str.find_last_not_of(' ') + 1);
          str = str.erase(0, str.find_first_not_of(' '));
          mxSetCell(values, (mwIndex)i, mxCreateString(str.c_str()));
        }
        break;
      }
      case btk::MetaDataInfo::Byte:
      {
        values = mxCreateNumericArray(ndim, dims, mxINT8_CLASS, mxREAL);
        int8_t* data = static_cast<int8_t*>(mxGetData(values));
        for (int i = 0 ; i < prod ; ++i)
          data[i] = md->GetInfo()->ToInt8(i);
        break;
      }
      case btk::MetaDataInfo::Integer:
      {
        values = mxCreateNumericArray(ndim, dims, mxINT16_CLASS, mxREAL);
        int16_t* data = static_cast<int16_t*>(mxGetData(values));
        for (int i = 0 ; i < prod ; ++i)
          data[i] = md->GetInfo()->ToInt16(i);
        break;
      }
      case btk::MetaDataInfo::Real:
      {
        values = mxCreateNumericArray(ndim, dims, mxDOUBLE_CLASS, mxREAL);
        double* data = static_cast<double*>(mxGetData(values));
        for (int i = 0 ; i < prod ; ++i)
          data[i] = md->GetInfo()->ToDouble(i);
        break;
      }
    }
    delete[] dims;
    mxSetFieldByNumber(info, 0, 1, values);
    mxSetFieldByNumber(out, 0, 0, info);
  }
  else
  {
    mxArray* info = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
    *reinterpret_cast<int*>(mxGetPr(info)) = 0;
    mxSetFieldByNumber(out, 0, 0, info);
  }
  // Children
  if (md->HasChildren())
  {
    int numberOfChildren = md->GetChildNumber();
    char** labels = new char*[numberOfChildren];
    int inc = 0;
    for (btk::MetaData::ConstIterator it = md->Begin() ; it != md->End() ; ++it)
    {
      std::string originalLabel = (*it)->GetLabel();
      std::string convertedLabel = std::string(originalLabel.length(), '_');
      // Check bad characters
      for(int i = 0 ; i < static_cast<int>(originalLabel.length()) ; ++i)
        convertedLabel[i] = btk::ASCIIConverter[originalLabel[i]];
      char c = convertedLabel[0];
      // Check first character
      if ((c == btk::ASCIIConverter[0x00]) // _
          || (c == btk::ASCIIConverter[0x30]) // 0
          || (c == btk::ASCIIConverter[0x31]) // 1
          || (c == btk::ASCIIConverter[0x32]) // 2
          || (c == btk::ASCIIConverter[0x33]) // 3
          || (c == btk::ASCIIConverter[0x34]) // 4
          || (c == btk::ASCIIConverter[0x35]) // 5
          || (c == btk::ASCIIConverter[0x36]) // 6
          || (c == btk::ASCIIConverter[0x37]) // 7
          || (c == btk::ASCIIConverter[0x38]) // 8
          || (c == btk::ASCIIConverter[0x39])) // 9
        convertedLabel.insert(convertedLabel.begin(), 'M');
      labels[inc] = new char[convertedLabel.length() + 1];
      strcpy(labels[inc], convertedLabel.c_str());
      inc++;
    }
    mxArray* children = mxCreateStructMatrix(1, 1, numberOfChildren, (const char**)labels);
    mxSetFieldByNumber(out, 0, 1, children);
    inc = 0;
    for (btk::MetaData::ConstIterator it = md->Begin() ; it != md->End() ; ++it)
    {
      mxSetFieldByNumber(children, 0, inc, btkMXCreateMetaDataStructure(*it));
      ++inc;
    }
  }
  else
  {
    mxArray* children = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
    *reinterpret_cast<int*>(mxGetPr(children)) = 0;
    mxSetFieldByNumber(out, 0, 1, children);
  }
  // Description
  mxSetFieldByNumber(out, 0, 2, mxCreateString(md->GetDescription().c_str()));
  return out;
};
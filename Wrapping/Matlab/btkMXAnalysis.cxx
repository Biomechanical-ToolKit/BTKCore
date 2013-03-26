/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2013, Arnaud Barré
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
  #include "btkMXAnalysis.h"
#endif
#include "btkMEXObjectHandle.h"
#include "btkASCIIConverter.h"

#include <btkConvert.h>

void btkMXCreateAnalysisStructure(btk::Acquisition::Pointer acq, int nlhs, mxArray *plhs[])
{
  // First output
  btk::MetaData::Pointer metadata = acq->GetMetaData();
  btk::MetaData::ConstIterator itAnalysis = metadata->FindChild("ANALYSIS");

  char** fieldnames = 0;
  size_t numberOfParameters = 0;
  std::vector< std::vector<std::string> > entryValues = std::vector< std::vector<std::string> >(0);
  std::vector<double> parameterValues = std::vector<double>(0);

  const char* names[] = {"NAMES", "CONTEXTS", "SUBJECTS", "UNITS"};
  size_t numberOfNames =  sizeof(names) / sizeof(char*);
  
  if (itAnalysis != metadata->End())
  {
    btk::MetaData::ConstIterator itUsed = (*itAnalysis)->FindChild("USED");
    if (itUsed != (*itAnalysis)->End())
    {
      numberOfParameters = (*itUsed)->GetInfo()->ToInt(0);
      entryValues = std::vector< std::vector<std::string> >(numberOfNames, std::vector<std::string>(numberOfParameters));
      for (size_t inc = 0 ; inc < numberOfNames ; ++inc)
      {
        btk::MetaData::ConstIterator it = (*itAnalysis)->FindChild(names[inc]);
        size_t num = 0;
        if (it != (*itAnalysis)->End())
        {
          num = ((numberOfParameters > (*it)->GetInfo()->GetValues().size()) ? (*it)->GetInfo()->GetValues().size() : numberOfParameters);
          for (size_t i = 0 ; i < num ; ++i)
            btkTrimString(&(entryValues[inc][i]));
        }
        for (size_t i = num ; i < numberOfParameters ; ++i)
          entryValues[inc][i] = "uname*" + btk::ToString(i);
      }
      // fieldnames generation
      fieldnames = new char*[numberOfParameters];
      for (size_t i = 0 ; i < numberOfParameters ; ++i)
      {
        std::string originalLabel = entryValues[1][i] + "_" + entryValues[0][i];
        std::string convertedLabel = std::string(originalLabel.length(), '_');
        // Check bad characters
        for(std::string::size_type j = 0 ; j < originalLabel.length() ; ++j)
          convertedLabel[j] = btk::ASCIIConverter[static_cast<uint8_t>(originalLabel[j])];
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
          convertedLabel.insert(convertedLabel.begin(), 'C');
        // Check label's redundancy
        int id = 0;
        std::string doubleLabel = convertedLabel;
        for (size_t j = 0 ; j < i ; ++j)
        {
          if (doubleLabel.compare(fieldnames[j]) == 0)
            doubleLabel = convertedLabel + btk::ToString(++id);
        }
        if (id != 0)
           convertedLabel = doubleLabel;
        fieldnames[i] = new char[convertedLabel.length() + 1];
        strcpy(fieldnames[i], convertedLabel.c_str());
        //++incLabel;
      }
      // values
      btk::MetaData::ConstIterator itValues = (*itAnalysis)->FindChild("VALUES");
      if (itValues != (*itAnalysis)->End())
      {
        parameterValues = (*itValues)->GetInfo()->ToDouble();
        if (parameterValues.size() < numberOfParameters)
        {
          btkErrorMacro("Some parameters has no values. Data may be corrupted.");
          parameterValues.resize(numberOfParameters, 0.0);
        }
      }
      else
        mexErrMsgTxt("Missing ANALYSIS:VALUES. Impossible to extract values");
    }
  }
  plhs[0] = mxCreateStructMatrix(1, 1, static_cast<int>(numberOfParameters), (const char**)fieldnames);
  if (numberOfParameters != 0)
  {
    for(int inc = 0 ; inc < static_cast<int>(numberOfParameters) ; ++inc)
    {
      mxArray* value = mxCreateDoubleMatrix(1, 1, mxREAL);
      *mxGetPr(value) = parameterValues[inc];
      mxSetFieldByNumber(plhs[0], 0, inc, value);
    }
  }
  
  // Second output (optionnal)
  if (nlhs != 2)
  {
    for (size_t i = 0 ; i < numberOfParameters ; ++i)
      delete[] fieldnames[i];
    delete[] fieldnames;
  }
  else
  {
    const char* info[] = {"subjects", "units"};
    int numberOfFields =  sizeof(info) / sizeof(char*);

    plhs[1] = mxCreateStructMatrix(1, 1, numberOfFields, info);
    mxArray* subjectsStruct = mxCreateStructMatrix(1, 1, static_cast<int>(numberOfParameters), (const char**)fieldnames);
    mxArray* unitsStruct = mxCreateStructMatrix(1, 1, static_cast<int>(numberOfParameters), (const char**)fieldnames);
    
    for (int i = 0 ; i < static_cast<int>(numberOfParameters) ; ++i)
    {
      mxSetFieldByNumber(subjectsStruct, 0, i, mxCreateString((const char*)entryValues[2][i].c_str()));
      mxSetFieldByNumber(unitsStruct, 0, i, mxCreateString((const char*)entryValues[3][i].c_str()));
      delete[] fieldnames[i];
    }
    mxSetFieldByNumber(plhs[1], 0, 0, subjectsStruct);
    mxSetFieldByNumber(plhs[1], 0, 1, unitsStruct);

    delete[] fieldnames;
  }
};
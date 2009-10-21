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

#ifndef __btkMXAnalysis_h
#define __btkMXAnalysis_h

#if defined(_MSC_VER)
  // Disable unsafe warning (use of the function 'strcpy' instead of 
  // 'strcpy_s' for portability reasons;
  #pragma warning( disable : 4996 ) 
#endif

#include "btkMXObjectHandle.h"
#include "btkASCIIConverter.h"

#include <btkAcquisition.h>
#include <btkMetaDataUtils.h>
#include <btkConvert.h>

void btkCreateAnalysis(btk::MetaData::Pointer analysis, char* name, char* context, char* unit, double value, char* subject, char* description)
{
  btk::MetaDataCreateChild(analysis, "USED", static_cast<int16_t>(1));
  btk::MetaDataCreateChild(analysis, "NAMES", std::string(name));
  btk::MetaDataCreateChild(analysis, "CONTEXTS", std::string(context));
  btk::MetaDataCreateChild(analysis, "UNITS", std::string(unit));
  btk::MetaDataCreateChild(analysis, "VALUES", static_cast<float>(value));
  btk::MetaDataCreateChild(analysis, "SUBJECTS", subject ? std::string(subject) : "");
  btk::MetaDataCreateChild(analysis, "DESCRIPTIONS", description ? std::string(description) : "");
};

void btkMXCreateAnalysisStructure(btk::Acquisition::Pointer acq, int nlhs, mxArray *plhs[])
{
  // First output
  btk::MetaData::Pointer metadata = acq->GetMetaData();
  btk::MetaData::ConstIterator itAnalysis = metadata->FindChild("ANALYSIS");

  char** fieldnames = 0;
  int numberOfParameters = 0;
  std::vector< std::vector<std::string> > entryValues = std::vector< std::vector<std::string> >(0);
  std::vector<double> parameterValues = std::vector<double>(0);

  const char* names[] = {"NAMES", "CONTEXTS", "SUBJECTS", "UNITS"};
  int numberOfNames =  sizeof(names) / sizeof(char*);
  
  if (itAnalysis != metadata->End())
  {
    btk::MetaData::ConstIterator itUsed = (*itAnalysis)->FindChild("USED");
    if (itUsed != (*itAnalysis)->End())
    {
      numberOfParameters = (*itUsed)->GetInfo()->ToInt(0);
      entryValues = std::vector< std::vector<std::string> >(numberOfNames, std::vector<std::string>(numberOfParameters));
      for (int inc = 0 ; inc < numberOfNames ; ++inc)
      {
        btk::MetaData::ConstIterator it = (*itAnalysis)->FindChild(names[inc]);
        int num = 0;
        if (it != (*itAnalysis)->End())
        {
          num = ((numberOfParameters > static_cast<int>((*it)->GetInfo()->GetValues().size())) ? static_cast<int>((*it)->GetInfo()->GetValues().size()) : numberOfParameters);
          for (int i = 0 ; i < num ; ++i)
          {
            std::string& str = entryValues[inc][i];
            str = (*it)->GetInfo()->ToString(i);
            str = str.erase(str.find_last_not_of(' ') + 1);
            str = str.erase(0, str.find_first_not_of(' '));
          }
        }
        for (int i = num ; i < numberOfParameters ; ++i)
          entryValues[inc][i] = "uname*" + btk::ToString(i);
      }
      // fieldnames generation
      //int incLabel = 0;
      fieldnames = new char*[numberOfParameters];
      for (int i = 0 ; i < numberOfParameters ; ++i)
      {
        std::string originalLabel = entryValues[1][i] + "_" + entryValues[0][i];
        std::string convertedLabel = std::string(originalLabel.length(), '_');
        // Check bad characters
        for(int j = 0 ; j < static_cast<int>(originalLabel.length()) ; ++j)
          convertedLabel[j] = btk::ASCIIConverter[originalLabel[j]];
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
        for (int j = 0 ; j < i ; ++j)
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
        if (static_cast<int>(parameterValues.size()) < numberOfParameters)
        {
          btkErrorMacro("Some parameters has no values. Data may be corrupted.");
          parameterValues.resize(numberOfParameters, 0.0);
        }
      }
      else
        mexErrMsgTxt("Missing ANALYSIS:VALUES. Impossible to extract values");
    }
  }
  plhs[0] = mxCreateStructMatrix(1, 1, numberOfParameters, (const char**)fieldnames);
  if (numberOfParameters != 0)
  {
    for(int inc = 0 ; inc < numberOfParameters ; ++inc)
    {
      mxArray* value = mxCreateDoubleMatrix(1, 1, mxREAL);
      *mxGetPr(value) = parameterValues[inc];
      mxSetFieldByNumber(plhs[0], 0, inc, value);
    }
  }
  
  // Second output (optionnal)
  if (nlhs != 2)
  {
    for (int i = 0 ; i < numberOfParameters ; ++i)
      delete[] fieldnames[i];
    delete[] fieldnames;
  }
  else
  {
    const char* info[] = {"subjects", "units"};
    int numberOfFields =  sizeof(info) / sizeof(char*);
    int inc = 0;

    plhs[1] = mxCreateStructMatrix(1, 1, numberOfFields, info);
    mxArray* subjectsStruct = mxCreateStructMatrix(1, 1, numberOfParameters, (const char**)fieldnames);
    mxArray* unitsStruct = mxCreateStructMatrix(1, 1, numberOfParameters, (const char**)fieldnames);
    
    for (int i = 0 ; i < numberOfParameters ;++i)
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

#endif // __btkMXAnalysis_h

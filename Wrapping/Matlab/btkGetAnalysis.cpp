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

#if defined(_MSC_VER)
  // Disable unsafe warning (use of the function 'strcpy' instead of 
	// 'strcpy_s' for portability reasons;
  #pragma warning( disable : 4996 ) 
#endif

#include "btkMEXObjectHandle.h"
#include "btkASCIIConverter.h"

#include <btkAcquisition.h>
#include <btkConvert.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	if(nrhs!=1)
		mexErrMsgTxt("One input required.");
	if (nlhs > 2)
	 mexErrMsgTxt("Too many output arguments.");

	// First output
	btk::Acquisition::Pointer acq = btk_MOH_get_object<btk::Acquisition>(prhs[0]);
	btk::MetaDataEntry::Pointer metadata = acq->GetMetaData();
	btk::MetaDataEntry::ConstIterator itAnalysis = metadata->Find("ANALYSIS");

	char** fieldnames = 0;
	int numberOfParameters = 0;
	std::vector< std::vector<std::string> > entryValues = std::vector< std::vector<std::string> >(0);
	std::vector<float> parameterValues = std::vector<float>(0);

	const char* names[] = {"NAMES", "CONTEXTS", "SUBJECTS", "UNITS"};
	int numberOfNames =  sizeof(names) / (sizeof(char) * 4);
	
	if (itAnalysis != metadata->End())
	{
		btk::MetaDataEntry::ConstIterator itUsed = (*itAnalysis)->Find("USED");
		if (itUsed != (*itAnalysis)->End())
		{
			numberOfParameters = btk::FromString<int>((*itUsed)->GetMetaDataEntryValue()->GetValue(0));
			entryValues = std::vector< std::vector<std::string> >(numberOfNames, std::vector<std::string>(numberOfParameters));
			for (int inc = 0 ; inc < numberOfNames ; ++inc)
			{
				btk::MetaDataEntry::ConstIterator it = (*itAnalysis)->Find(names[inc]);
				int num = 0;
				if (it != (*itAnalysis)->End())
				{
					num = ((numberOfParameters > static_cast<int>((*it)->GetMetaDataEntryValue()->GetValues().size())) ? static_cast<int>((*it)->GetMetaDataEntryValue()->GetValues().size()) : numberOfParameters);
					for (int i = 0 ; i < num ; ++i)
					{
						std::string& str = entryValues[inc][i];
						str = (*it)->GetMetaDataEntryValue()->GetValue(i);
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
			btk::MetaDataEntry::ConstIterator itValues = (*itAnalysis)->Find("VALUES");
			parameterValues = std::vector<float>(numberOfParameters, 0);
			if (itValues != (*itAnalysis)->End())
			{
				std::vector<std::string> str = (*itValues)->GetMetaDataEntryValue()->GetValues();
				int num = ((numberOfParameters > static_cast<int>(str.size())) ? static_cast<int>(str.size()) : numberOfParameters);
				for (int inc = 0 ; inc < num ; ++inc)
					parameterValues[inc] = btk::FromString<float>(str[inc]);
			}
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
		int numberOfFields =  sizeof(info) / (sizeof(char) * 4);
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



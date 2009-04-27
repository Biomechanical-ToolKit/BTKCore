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
	btk::EventCollection::Pointer events = acq->GetEvents();
  int numberOfEvents = acq->GetEventNumber();
	char** fieldnames = new char*[numberOfEvents];
  double* times = new double[numberOfEvents];
	int inc = 0;
  for(btk::EventCollection::ConstIterator it = events->Begin() ; it != events->End() ; ++it)
	{
		std::string originalLabel;
		if ((*it)->GetContext().length() != 0)
			originalLabel = (*it)->GetContext() + "_" + (*it)->GetLabel();
		else
			originalLabel = (*it)->GetLabel();
		times[inc] = (*it)->GetTime();
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
			convertedLabel.insert(convertedLabel.begin(), 'C');
		// Check label's redundancy
		int id = 0;
		std::string doubleLabel = convertedLabel;
		for (int i = 0 ; i < inc ; ++i)
		{
			if (doubleLabel.compare(fieldnames[i]) == 0)
				doubleLabel = convertedLabel + btk::ToString(++id);
		}
		if (id != 0)
	    convertedLabel = doubleLabel;
		fieldnames[inc] = new char[convertedLabel.length() + 1];
		strcpy(fieldnames[inc], convertedLabel.c_str());
		inc++;
	}
	plhs[0] = mxCreateStructMatrix(1, 1, numberOfEvents, (const char**)fieldnames);
  if (numberOfEvents != 0)
	{
		for(int inc = 0 ; inc < numberOfEvents ; ++inc)
		{
			mxArray* value = mxCreateDoubleMatrix(1, 1, mxREAL);
      *mxGetPr(value) = times[inc];
			mxSetFieldByNumber(plhs[0], 0, inc, value);
		}
		delete[] times;
	}	
	
	// Second output (optionnal)
	if (nlhs != 2)
	{
		for (int i = 0 ; i < numberOfEvents ; ++i)
			delete[] fieldnames[i];
		delete[] fieldnames;
  }
	else
	{
		const char* info[] = {"subjects", "units"};
		int numberOfFields =  sizeof(info) / (sizeof(char) * 4);
		
		plhs[1] = mxCreateStructMatrix(1, 1, numberOfFields, info);
		mxArray* subjectsStruct = mxCreateStructMatrix(1, 1, numberOfEvents, (const char**)fieldnames);
    
		int inc = 0;
		for (btk::EventCollection::ConstIterator it = events->Begin() ; it != events->End() ; ++it)
		{
			mxSetFieldByNumber(subjectsStruct, 0, inc, mxCreateString((*it)->GetSubject().c_str()));
			delete[] fieldnames[inc];
			++inc;
		}

		const char* ALL[] = {"ALLEVENTS"};
		mxArray* unitsStruct = mxCreateStructMatrix(1, 1, 1, ALL);
		mxSetFieldByNumber(unitsStruct, 0, 0, mxCreateString("s"));

		mxSetFieldByNumber(plhs[1], 0, 0, subjectsStruct);
		mxSetFieldByNumber(plhs[1], 0, 1, unitsStruct);

		delete[] fieldnames;
	}
};

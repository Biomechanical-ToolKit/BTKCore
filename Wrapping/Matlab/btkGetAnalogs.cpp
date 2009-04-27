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

#include "btkMEXObjectHandle.h"
#include "btkMEXAdaptMeasures.h"

#include <btkAcquisition.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	if(nrhs!=1)
		mexErrMsgTxt("One input required.");
	if (nlhs > 2)
	 mexErrMsgTxt("Too many output arguments.");

	// First output
	btk::Acquisition::Pointer acq = btk_MOH_get_object<btk::Acquisition>(prhs[0]);
	btk::AnalogCollection::Pointer analogs = acq->GetAnalogs();
	char** fieldnames = 0;
	plhs[0] = btkMEXAdaptMeasures<btk::Analog>(analogs, &fieldnames);
	int numberOfAnalogs = acq->GetAnalogNumber();

  // Second output (optionnal)
	if (nlhs != 2)
	{
		for (int i = 0 ; i < numberOfAnalogs ; ++i)
			delete[] fieldnames[i];
		delete[] fieldnames;
	}
	else
	{
    const char* info[] = {"frequency", "units"};
		int numberOfFields =  sizeof(info) / (sizeof(char) * 4);
		int inc = 0;
		char** units = new char*[numberOfAnalogs];
    btk::MetaDataEntry::Pointer metadata = acq->GetMetaData();
    btk::MetaDataEntry::ConstIterator itAnalog = metadata->Find("ANALOG");
		if ((itAnalog != metadata->End()) && (numberOfAnalogs != 0))
		{
			btk::MetaDataEntry::ConstIterator itUnit = (*itAnalog)->Find("UNITS");
			if (itUnit != (*itAnalog)->End())
			{
				btk::MetaDataEntryValue::ConstPointer analogUnit = (*itUnit)->GetMetaDataEntryValue();
				int num = ( (static_cast<int>(analogUnit->GetValues().size()) > numberOfAnalogs) ? numberOfAnalogs : analogUnit->GetValues().size() );
				for (inc = 0 ; inc < num ; ++inc)
				{
					std::string str = analogUnit->GetValue(inc);
					str = str.erase(str.find_last_not_of(' ') + 1);
          str = str.erase(0, str.find_first_not_of(' '));
					units[inc] = new char[str.length() + 1];
					strcpy(units[inc], str.c_str());
				}
			}
		}
		//else
		//	mexWarnMsgTxt("No ANALOG:UNITS parameter. The unit of each analog channel is empty.");
		for (int i = inc ; i < numberOfAnalogs ; ++i)
		{
			units[i] = new char[1];
			units[i] = '\0';
		}
		
		plhs[1] = mxCreateStructMatrix(1, 1, numberOfFields, info);
    mxArray* frequency = mxCreateDoubleMatrix(1, 1, mxREAL);
    *mxGetPr(frequency) = acq->GetAnalogFrequency();
		mxArray* unitsStruct = mxCreateStructMatrix(1, 1, numberOfAnalogs, (const char**)fieldnames);
		for (int i = 0 ; i < numberOfAnalogs ;++i)
		{
			mxSetFieldByNumber(unitsStruct, 0, i, mxCreateString((const char*)units[i]));
			delete[] fieldnames[i];
			delete[] units[i];
		}
		mxSetFieldByNumber(plhs[1], 0, 0, frequency);
		mxSetFieldByNumber(plhs[1], 0, 1, unitsStruct);

		delete[] fieldnames;
		delete[] units;
	}
};


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

#include "btkObjectHandle.h"
#include "btkASCIIConverter.h"

#include <btkAcquisition.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	if(nrhs!=1)
		mexErrMsgTxt("One input required.");
	if (nlhs > 1)
	 mexErrMsgTxt("Too many output arguments.");

	btk::Acquisition::Pointer acq = btkOH_get_object<btk::Acquisition>(prhs[0]);
	btk::MarkerCollection::Pointer markers = acq->GetMarkers();

	char** fieldnames = new char*[markers->GetItemNumber()];
	int inc = 0;
	for(btk::MarkerCollection::ConstIterator it = markers->Begin() ; it != markers->End() ; ++it)
	{
		std::string originalLabel = (*it)->GetLabel();
    std::string convertedLabel = std::string(originalLabel.length(), '_');
		for(int i = 0 ; i < static_cast<int>(originalLabel.length()) ; ++i)
		{
			convertedLabel[i] = btk::ASCIIConverter[originalLabel[i]];
		}
		char c = convertedLabel[0];
		// Matlab only accept characters [a-zA-Z] for the first character of 
		// a variable's name. If the first character is included in [_0-9],
		// then the character 'C' is inserted at the beginning.
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

		fieldnames[inc] = new char[convertedLabel.length() + 1];
    //strncpy(fieldnames[inc], convertedLabel.c_str(), convertedLabel.length() + 1);
		strcpy(fieldnames[inc], convertedLabel.c_str());

		//mexPrintf("Marker's label '%s' vs conversion: '%s'\n", (const char*)fieldnames[inc], convertedLabel.c_str());
		inc++;
	}

	plhs[0] = mxCreateStructMatrix(1, 1, acq->GetMarkerNumber(), (const char**)fieldnames);

//  int nfields = mxGetNumberOfFields(plhs[0]);
//  int nstructelems = mxGetNumberOfElements(plhs[0]);

//	mexPrintf("mxGetNumberOfFields: %i\n", nfields);
//	mexPrintf("mxGetNumberOfElements: %i\n", nstructelems);
//	for(int i = 0 ; i < 5 ; ++i)
//		mexPrintf("Marker's label: '%s' vs '%s'\n", (const char*)fieldnames[i], mxGetFieldNameByNumber(plhs[0], i));
  
	inc = 0;
	mxArray* marker = mxCreateDoubleMatrix(acq->GetMarkerFrameNumber(), 3, mxREAL);
  double* initialValues = mxGetPr(marker);
  for(btk::MarkerCollection::ConstIterator it = markers->Begin() ; it != markers->End() ; ++it)
	{
	  mxSetPr(marker, (*it)->GetValues().data());
		mxArray* deepMarker = mxDuplicateArray(marker);
		mxSetFieldByNumber(plhs[0], 0, inc, deepMarker);

		delete[] fieldnames[inc];
		inc++;
	}
  delete[] fieldnames;
  mxSetPr(marker, initialValues);
  mxDestroyArray(marker);

	//mxFree((void*)fieldnames);
};

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
	btk::MarkerCollection::Pointer markers = acq->GetMarkers();
	char** fieldnames = 0;
	plhs[0] = btkMEXAdaptMeasures<btk::Marker>(markers, &fieldnames);
	int numberOfPoints = acq->GetMarkerNumber();

	// Second output (optionnal)
	if (nlhs != 2)
	{
		for (int i = 0 ; i < numberOfPoints ; ++i)
			delete[] fieldnames[i];
		delete[] fieldnames;
  }
	else
	{
		char** units = new char*[numberOfPoints];

    const char* info[] = {"firstFrame", "frequency", "units"};
		int numberOfFields =  sizeof(info) / (sizeof(char) * 4);
		
		const char* names[] = {"ANGLE", "FORCE", "MOMENT", "POWER", "SCALAR"};
		int numberOfNames =  sizeof(names) / (sizeof(char) * 4);

		std::vector<std::string> xUnit = std::vector<std::string>(numberOfNames + 1, "");
		std::vector<std::string> pointsLabel;
		std::vector< std::vector<std::string> > xLabel = std::vector< std::vector<std::string> >(numberOfNames);

    btk::MetaDataEntry::Pointer metadata = acq->GetMetaData();
    btk::MetaDataEntry::ConstIterator itPoint = metadata->Find("POINT");
		if (itPoint != metadata->End())
		{
			btk::MetaDataEntry::CollapseChildrenValues(pointsLabel, *itPoint, "LABELS", numberOfPoints, "uname*");
			for(int i = 0 ; i < numberOfNames ; ++i)
			{
				btk::MetaDataEntry::CollapseChildrenValues(xLabel[i], *itPoint, std::string(names[i]) + "S");
				btk::MetaDataEntry::ConstIterator itUnits = (*itPoint)->Find(std::string(names[i]) + "_UNITS");
				if (itUnits != (*itPoint)->End())
				{
					xUnit[i] = (*itUnits)->GetMetaDataEntryValue()->GetValue(0);
					xUnit[i] = xUnit[i].erase(xUnit[i].find_last_not_of(' ') + 1);
        	xUnit[i] = xUnit[i].erase(0, xUnit[i].find_first_not_of(' '));
				}
			}
			// POINT:ANGLES special case.
			if (xUnit[0].empty())
				xUnit[0] = "deg";
			// POINT:UNITS
			btk::MetaDataEntry::ConstIterator itMarkerUnits = (*itPoint)->Find("UNITS");
			if (itMarkerUnits != (*itPoint)->End())
			{
				std::string& markerUnit = xUnit[numberOfNames];
				markerUnit = (*itMarkerUnits)->GetMetaDataEntryValue()->GetValue(0);
				markerUnit = markerUnit.erase(markerUnit.find_last_not_of(' ') + 1);
        markerUnit = markerUnit.erase(0, markerUnit.find_first_not_of(' '));
			}
			std::vector<std::string>::iterator itPointLabel = pointsLabel.begin();
			int incPointLabel = 0;
			while (itPointLabel != pointsLabel.end())
			{
				int inc = 0;
				while (inc < numberOfNames)
				{
					bool labelFound = false;
					for (std::vector<std::string>::iterator itXLabel = xLabel[inc].begin() ; itXLabel != xLabel[inc].end() ; ++itXLabel)
					{
						if (itPointLabel->compare(*itXLabel) == 0)
						{
							xLabel[inc].erase(itXLabel);
							labelFound = true;
							break;
						}
					}
					if (labelFound)
						break;
					++inc;
				}
				units[incPointLabel] = new char[xUnit[inc].length() + 1];
				strcpy(units[incPointLabel], xUnit[inc].c_str());
				itPointLabel = pointsLabel.erase(itPointLabel);
				++incPointLabel;
			}
		}
	  else
		{
			//mexWarnMsgTxt("No POINT group. The unit of each 3D point is empty.");
			for (int i = 0 ; i < numberOfPoints ; ++i)
			{
				units[i] = new char[1];
				units[i] = '\0';
			}
		}
		plhs[1] = mxCreateStructMatrix(1, 1, numberOfFields, info);
		mxArray* firstFrame  = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
    *reinterpret_cast<int*>(mxGetPr(firstFrame)) = acq->GetFirstFrame();
    mxArray* frequency = mxCreateDoubleMatrix(1, 1, mxREAL);
    *mxGetPr(frequency) = acq->GetMarkerFrequency();
		mxArray* unitsStruct = mxCreateStructMatrix(1, 1, numberOfPoints, (const char**)fieldnames);
		for (int i = 0 ; i < numberOfPoints ;++i)
		{
			mxSetFieldByNumber(unitsStruct, 0, i, mxCreateString((const char*)units[i]));
			delete[] fieldnames[i];
			delete[] units[i];
		}
    mxSetFieldByNumber(plhs[1], 0, 0, firstFrame);
		mxSetFieldByNumber(plhs[1], 0, 1, frequency);
    mxSetFieldByNumber(plhs[1], 0, 2, unitsStruct);
		delete[] fieldnames;
		delete[] units;
	}
};

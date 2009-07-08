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

#include "btkMEXAdaptMeasures.h"

#include <btkAcquisition.h>

void btkMEXGetPoints(btk::Acquisition::Pointer acq, int nlhs, mxArray *plhs[])
{ 
  if (nlhs == 0)
    return;

  btk::PointCollection::Pointer points = acq->GetPoints();
  char** fieldnames = 0;
  plhs[0] = btkMEXAdaptMeasures<btk::Point>(points, &fieldnames);
  int numberOfPoints = acq->GetPointNumber();

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
    btk::MetaDataEntry::Pointer metadata = acq->GetMetaData();
    btk::MetaDataEntry::ConstIterator itPoint = metadata->Find("POINT");
    if (itPoint != metadata->End())
    {
      for(int i = 1 ; i <= numberOfNames ; ++i)
      {
        btk::MetaDataEntry::ConstIterator itUnits = (*itPoint)->Find(std::string(names[i-1]) + "_UNITS");
        if (itUnits != (*itPoint)->End())
        {
          xUnit[i] = (*itUnits)->GetMetaDataEntryValue()->GetValue(0);
          xUnit[i] = xUnit[i].erase(xUnit[i].find_last_not_of(' ') + 1);
          xUnit[i] = xUnit[i].erase(0, xUnit[i].find_first_not_of(' '));
        }
      }
      // POINT:ANGLES special case.
      if (xUnit[1].empty())
        xUnit[1] = "deg";
      // POINT:UNITS
      btk::MetaDataEntry::ConstIterator itPointUnits = (*itPoint)->Find("UNITS");
      if (itPointUnits != (*itPoint)->End())
      {
        std::string& pointUnit = xUnit[0];
        pointUnit = (*itPointUnits)->GetMetaDataEntryValue()->GetValue(0);
        pointUnit = pointUnit.erase(pointUnit.find_last_not_of(' ') + 1);
        pointUnit = pointUnit.erase(0, pointUnit.find_first_not_of(' '));
      }
      int inc = 0;
      for(btk::PointCollection::ConstIterator itPt = points->Begin() ; itPt != points->End() ; ++itPt)
      {
        if ((*itPt)->GetType() <= 5) // 5: number of known units.
        {
          units[inc] = new char[xUnit[(*itPt)->GetType()].length() + 1];
          strcpy(units[inc], xUnit[(*itPt)->GetType()].c_str());
        }
        else
        {
          units[inc] = new char[1];
          units[inc] = '\0';
        }
        ++inc;
      }
    }
    else
    {
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
    *mxGetPr(frequency) = acq->GetPointFrequency();
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

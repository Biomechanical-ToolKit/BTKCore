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

#ifndef __btkMXPoint_h
#define __btkMXPoint_h

#include "btkMXMeasure.h"

#include <btkAcquisition.h>
#include <btkSpecializedPointsExtractor.h>

btk::Point::Pointer btkMXGetPoint(btk::Acquisition::Pointer acq, int nrhs, const mxArray* prhs[])
{
  if (!mxIsChar(prhs[1]) && (!mxIsNumeric(prhs[1]) || mxIsEmpty(prhs[1]) || mxIsComplex(prhs[1]) || (mxGetNumberOfElements(prhs[1]) != 1)))
    mexErrMsgTxt("Point's index must be a non-empty string or an integer.");
  
  btk::Point::Pointer point;
  if (mxIsChar(prhs[1]))
  {
    int strlen = (mxGetM(prhs[1]) * mxGetN(prhs[1]) * sizeof(mxChar)) + 1;
    char* label = (char*)mxMalloc(strlen);
    mxGetString(prhs[1], label, strlen);
    btk::Acquisition::PointIterator itPoint = acq->FindPoint(label);
    if (itPoint == acq->EndPoint())
    {
      std::string err = "No point with label: '" + std::string(label) + "'.";
      mxFree(label);
      mexErrMsgTxt(err.c_str());
    }
    point = *itPoint;
    mxFree(label);
  }
  else
  {
    int idx = static_cast<int>(mxGetScalar(prhs[1])) - 1;
    if ((idx < 0) || (idx >= acq->GetPointNumber()))
      mexErrMsgTxt("Point's index out of range.");
    point = acq->GetPoint(idx);
  }
  return point;
};

void btkMXCreatePointsStructure(btk::Acquisition::Pointer acq, int nlhs, mxArray *plhs[])
{ 
  if (nlhs == 0)
    return;

  btk::PointCollection::Pointer points = acq->GetPoints();
  char** fieldnames = 0;
  plhs[0] = btkMXCreateMeasuresStructure<btk::Point>(points, &fieldnames);
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
    std::vector<std::string> units = std::vector<std::string>(numberOfPoints, "");
    const char* info[] = {"firstFrame", "frequency", "units"};
    int numberOfFields =  sizeof(info) / sizeof(char*);
       
    int inc = 0;
    for(btk::PointCollection::ConstIterator itPt = points->Begin() ; itPt != points->End() ; ++itPt)
    {
      if ((*itPt)->GetType() <= 5) // 0-5: known units.
        units[inc] = acq->GetPointUnit((*itPt)->GetType());
      ++inc;
    }
    plhs[1] = mxCreateStructMatrix(1, 1, numberOfFields, info);
    mxArray* firstFrame  = mxCreateDoubleMatrix(1, 1, mxREAL);
    *mxGetPr(firstFrame) = static_cast<double>(acq->GetFirstFrame());
    mxArray* frequency = mxCreateDoubleMatrix(1, 1, mxREAL);
    *mxGetPr(frequency) = acq->GetPointFrequency();
    mxArray* unitsStruct = mxCreateStructMatrix(1, 1, numberOfPoints, (const char**)fieldnames);
    for (int i = 0 ; i < numberOfPoints ;++i)
    {
      mxSetFieldByNumber(unitsStruct, 0, i, mxCreateString(units[i].c_str()));
      delete[] fieldnames[i];
    }
    mxSetFieldByNumber(plhs[1], 0, 0, firstFrame);
    mxSetFieldByNumber(plhs[1], 0, 1, frequency);
    mxSetFieldByNumber(plhs[1], 0, 2, unitsStruct);
    delete[] fieldnames;
  }
};

#endif // __btkMXPoint_h 
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

#ifndef __btkMEXTransformSpecializedPoints_h
#define __btkMEXTransformSpecializedPoints_h

#include "btkMEXObjectHandle.h"
#include "btkMEXAdaptMeasures.h"

#include <btkAcquisition.h>
#include <btkPointCollection.h>
#include <btkSpecializedPointsExtractor.h>

void btkMEXTransformSpecializedPoints(
    btk::Point::Type t,
    int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  if(nrhs!=1)
    mexErrMsgTxt("One input required.");
  if (nlhs > 2)
   mexErrMsgTxt("Too many output arguments.");

  // First output
  btk::Acquisition::Pointer acq = btk_MOH_get_object<btk::Acquisition>(prhs[0]);

  btk::SpecializedPointsExtractor::Pointer pointsExtractor = btk::SpecializedPointsExtractor::New();
  pointsExtractor->SetPointType(t);
  pointsExtractor->SetInput(acq);
  btk::PointCollection::Pointer points = pointsExtractor->GetOutput();
  pointsExtractor->Update();

  char** fieldnames = 0;
  plhs[0] = btkMEXAdaptMeasures<btk::Point>(points, &fieldnames);
  int numberOfPoints = points->GetItemNumber();
  for (int i = 0 ; i < numberOfPoints ; ++i)
    delete[] fieldnames[i];
  delete[] fieldnames;

  // Second output (optionnal)
  if (nlhs == 2)
  {
    std::string label = "";
    switch(t)
    {
      case btk::Point::Angle:
        label = "ANGLE";
        break;
      case btk::Point::Force:
        label = "FORCE";
        break;
      case btk::Point::Moment:
        label = "MOMENT";
        break;
      case btk::Point::Power:
        label = "POWER";
        break;
      case btk::Point::Scalar:
        label = "SCALAR";
        break;
      default:
        mexWarnMsgTxt("Unknown point's type. Impossible to set the units.");
        break;
    }

    const char* info[] = {"frequency", "units"};
    int numberOfFields =  sizeof(info) / (sizeof(char) * 4);

    plhs[1] = mxCreateStructMatrix(1, 1, numberOfFields, info);
    mxArray* frequency = mxCreateDoubleMatrix(1, 1, mxREAL);
    *mxGetPr(frequency) = acq->GetPointFrequency();
    std::string allLabels = "ALL" + label + "S";
    const char* ALL[] = {allLabels.c_str()};
    mxArray* unitsStruct = mxCreateStructMatrix(1, 1, 1, ALL);
    mxSetFieldByNumber(unitsStruct, 0, 0, mxCreateString(acq->GetPointUnit(t).c_str()));
    mxSetFieldByNumber(plhs[1], 0, 0, frequency);
    mxSetFieldByNumber(plhs[1], 0, 1, unitsStruct);
  }
};

#endif // __btkMEXTransformSpecializedPoints_h

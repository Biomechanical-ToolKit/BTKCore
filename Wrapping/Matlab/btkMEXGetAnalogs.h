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

#ifndef __btkMEXGetPoints_h
#define __btkMEXGetPoints_h

#include "btkMEXAdaptMeasures.h"

#include <btkAcquisition.h>

void btkMEXGetAnalogs(btk::Acquisition::Pointer acq, int nlhs, mxArray *plhs[])
{
  // First output
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
    const char* info[] = {"gain", "offset", "scale", "frequency", "units"};
    int numberOfFields =  sizeof(info) / (sizeof(char) * 4);
    plhs[1] = mxCreateStructMatrix(1, 1, numberOfFields, info);
    // frequency field
    mxArray* frequency = mxCreateDoubleMatrix(1, 1, mxREAL);
    *mxGetPr(frequency) = acq->GetAnalogFrequency();
    // gain, offset, scale and units field
    btk::AnalogCollection::ConstIterator itAnalog = analogs->Begin();
    mxArray* gainStruct = mxCreateStructMatrix(1, 1, numberOfAnalogs, (const char**)fieldnames);
    mxArray* offsetStruct = mxCreateStructMatrix(1, 1, numberOfAnalogs, (const char**)fieldnames);
    mxArray* scaleStruct = mxCreateStructMatrix(1, 1, numberOfAnalogs, (const char**)fieldnames);
    mxArray* unitsStruct = mxCreateStructMatrix(1, 1, numberOfAnalogs, (const char**)fieldnames);
    for (int i = 0 ; i < numberOfAnalogs ;++i)
    {
      mxArray* gain = mxCreateDoubleMatrix(1, 1, mxREAL);
      mxArray* offset = mxCreateDoubleMatrix(1, 1, mxREAL);
      mxArray* scale = mxCreateDoubleMatrix(1, 1, mxREAL);
      *mxGetPr(gain) = static_cast<double>((*itAnalog)->GetGain());
      *mxGetPr(offset) = static_cast<double>((*itAnalog)->GetOffset());
      *mxGetPr(scale) = (*itAnalog)->GetScale();
      mxSetFieldByNumber(gainStruct, 0, i, gain);
      mxSetFieldByNumber(offsetStruct, 0, i, offset);
      mxSetFieldByNumber(scaleStruct, 0, i, scale);
      mxSetFieldByNumber(unitsStruct, 0, i, mxCreateString((*itAnalog)->GetUnit().c_str()));
      
      ++itAnalog;
      delete[] fieldnames[i];
    }
    delete[] fieldnames;
    mxSetFieldByNumber(plhs[1], 0, 0, gainStruct);
    mxSetFieldByNumber(plhs[1], 0, 1, offsetStruct);
    mxSetFieldByNumber(plhs[1], 0, 2, scaleStruct);
    mxSetFieldByNumber(plhs[1], 0, 3, frequency);
    mxSetFieldByNumber(plhs[1], 0, 4, unitsStruct);
  }
};

#endif // __btkMEXGetPoints_h
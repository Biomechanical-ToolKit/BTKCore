/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2014, Arnaud Barré
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

#include "btkMex.h"
#if defined(SCI_MEX)
  #include "btkMXAnalog.h"
#endif
#include "btkMXMeasure.h"

btk::Analog::Pointer btkMXGetAnalog(btk::Acquisition::Pointer acq, int nrhs, const mxArray* prhs[])
{
  if (nrhs < 2)
    mexErrMsgTxt("Analog's index out of range.");
  
  if (!mxIsChar(prhs[1]) && ((mxGetClassID(prhs[1]) != mxDOUBLE_CLASS) || mxIsEmpty(prhs[1]) || mxIsComplex(prhs[1]) || (mxGetNumberOfElements(prhs[1]) != 1)))
    mexErrMsgTxt("Analog's index must be a non-empty string or a double value representing an integer.");
  btk::Analog::Pointer analog;
  if (mxIsChar(prhs[1]))
  {
    size_t strlen_ = (mxGetM(prhs[1]) * mxGetN(prhs[1]) * sizeof(mxChar)) + 1;
    char* label = (char*)mxMalloc(strlen_);
    mxGetString(prhs[1], label, strlen_);
    btk::Acquisition::AnalogIterator itAnalog = acq->FindAnalog(label);
    if (itAnalog == acq->EndAnalog())
    {
      std::string err = "No analog channel with label: '" + std::string(label) + "'.";
      mexErrMsgTxt(err.c_str());
    }
    analog = *itAnalog;
    mxFree(label);
  }
  else
  {
    int idx = static_cast<int>(mxGetScalar(prhs[1])) - 1;
    if ((idx < 0) || (idx > acq->GetAnalogNumber()))
      mexErrMsgTxt("Analog's index out of range.");
    analog = acq->GetAnalog(idx);
  }
  return analog;
};

void btkMXCreateAnalogsStructure(btk::Acquisition::Pointer acq, int nlhs, mxArray *plhs[])
{
  // First output
  btk::AnalogCollection::Pointer analogs = acq->GetAnalogs();
  char** fieldnames = 0;
  plhs[0] = btkMXCreateMeasuresStructure<btk::Analog>(analogs, &fieldnames);
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
    const char* info[] = {"label", "description", "gain", "offset", "scale", "frequency", "units"};
    int numberOfFields =  sizeof(info) / sizeof(char*);
    plhs[1] = mxCreateStructMatrix(1, 1, numberOfFields, info);
    // frequency field
    mxArray* frequency = mxCreateDoubleMatrix(1, 1, mxREAL);
    *mxGetPr(frequency) = acq->GetAnalogFrequency();
    // gain, offset, scale and units field
    btk::AnalogCollection::ConstIterator itAnalog = analogs->Begin();
    mxArray* labelStruct = mxCreateStructMatrix(1, 1, numberOfAnalogs, (const char**)fieldnames);
    mxArray* descStruct = mxCreateStructMatrix(1, 1, numberOfAnalogs, (const char**)fieldnames);
    mxArray* gainStruct = mxCreateStructMatrix(1, 1, numberOfAnalogs, (const char**)fieldnames);
    mxArray* offsetStruct = mxCreateStructMatrix(1, 1, numberOfAnalogs, (const char**)fieldnames);
    mxArray* scaleStruct = mxCreateStructMatrix(1, 1, numberOfAnalogs, (const char**)fieldnames);
    mxArray* unitsStruct = mxCreateStructMatrix(1, 1, numberOfAnalogs, (const char**)fieldnames);
    for (int i = 0 ; i < numberOfAnalogs ; ++i)
    {
      mxSetFieldByNumber(labelStruct, 0, i, mxCreateString((*itAnalog)->GetLabel().c_str()));
      mxSetFieldByNumber(descStruct, 0, i, mxCreateString((*itAnalog)->GetDescription().c_str()));
      mxSetFieldByNumber(gainStruct, 0, i, mxCreateDoubleScalar((*itAnalog)->GetGain()));
      mxSetFieldByNumber(offsetStruct, 0, i, mxCreateDoubleScalar((*itAnalog)->GetOffset()));
      mxSetFieldByNumber(scaleStruct, 0, i, mxCreateDoubleScalar((*itAnalog)->GetScale()));
      mxSetFieldByNumber(unitsStruct, 0, i, mxCreateString((*itAnalog)->GetUnit().c_str()));
      ++itAnalog;
      delete[] fieldnames[i];
    }
    delete[] fieldnames;
    mxSetFieldByNumber(plhs[1], 0, 0, labelStruct);
    mxSetFieldByNumber(plhs[1], 0, 1, descStruct);
    mxSetFieldByNumber(plhs[1], 0, 2, gainStruct);
    mxSetFieldByNumber(plhs[1], 0, 3, offsetStruct);
    mxSetFieldByNumber(plhs[1], 0, 4, scaleStruct);
    mxSetFieldByNumber(plhs[1], 0, 5, frequency);
    mxSetFieldByNumber(plhs[1], 0, 6, unitsStruct);
  }
};
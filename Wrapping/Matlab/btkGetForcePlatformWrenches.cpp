/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2012, Arnaud Barré
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

#include <btkForcePlatformWrenchFilter.h> // First (Fix for MSVC LNK2019)

#include "btkMXObjectHandle.h"
#include "btkMXMeasure.h"

#include <btkAcquisition.h>
#include <btkForcePlatformsExtractor.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  if (nrhs < 1) 
    mexErrMsgTxt("One input required.");
  if (nlhs > 1)
    mexErrMsgTxt("Too many output arguments.");

  if ((nrhs > 1)  && (!mxIsNumeric(prhs[1]) || mxIsEmpty(prhs[1]) || mxIsComplex(prhs[1]) || (mxGetNumberOfElements(prhs[1]) != 1)))
    mexErrMsgTxt("The flag to express wrenches in global or local frame must be set determine first frame must be set by one integer.");

  // First output
  btk::Acquisition::Pointer acq = btk_MOH_get_object<btk::Acquisition>(prhs[0]);
  btk::ForcePlatformsExtractor::Pointer fpExtractor = btk::ForcePlatformsExtractor::New();
  fpExtractor->SetInput(acq);
  btk::ForcePlatformWrenchFilter::Pointer fpwFilter = btk::ForcePlatformWrenchFilter::New();
  fpwFilter->SetInput(fpExtractor->GetOutput());
  btk::WrenchCollection::Pointer fpws = fpwFilter->GetOutput();
  
  if (nrhs > 1)
    fpwFilter->SetTransformToGlobalFrame((static_cast<int>(mxGetScalar(prhs[1])) == 0) ? false : true);
  
  fpws->Update();

  const char* fieldnames[] = {"P", "F", "M"};
  int numberOfFields =  sizeof(fieldnames) / sizeof(char*);
  int numberOfForcePlates = fpws->GetItemNumber();
  
  plhs[0] = mxCreateStructMatrix(numberOfForcePlates, 1, numberOfFields, fieldnames);
  btk::WrenchCollection::ConstIterator itWrench = fpws->Begin();
  for (int i = 0 ; i < numberOfForcePlates ; ++i)
  {
    if (itWrench->get() != 0)
    {
      int numberOfFrames = (*itWrench)->GetPosition()->GetFrameNumber();
      // Position
      mxArray* position = mxCreateDoubleMatrix(numberOfFrames, 3, mxREAL);
      memcpy(mxGetPr(position), (*itWrench)->GetPosition()->GetValues().data(), mxGetNumberOfElements(position) * sizeof(double));
      mxSetFieldByNumber(plhs[0], i, 0, position);
      // Force
      mxArray* force = mxCreateDoubleMatrix(numberOfFrames, 3, mxREAL);
      memcpy(mxGetPr(force), (*itWrench)->GetForce()->GetValues().data(), mxGetNumberOfElements(force) * sizeof(double));
      mxSetFieldByNumber(plhs[0], i, 1, force);
      // Moment
      mxArray* moment = mxCreateDoubleMatrix(numberOfFrames, 3, mxREAL);
      memcpy(mxGetPr(moment), (*itWrench)->GetMoment()->GetValues().data(), mxGetNumberOfElements(moment) * sizeof(double));
      mxSetFieldByNumber(plhs[0], i, 2, moment);
    }
    else
    {
      std::string warnText = "Force platform wrench #" + btk::ToString(i) + " is empty. Send an email to BTK developers to inform them.";
      mexWarnMsgTxt(warnText.c_str());
    }
    ++itWrench;
  }
};

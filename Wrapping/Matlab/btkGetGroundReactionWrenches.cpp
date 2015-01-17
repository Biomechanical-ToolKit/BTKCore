/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2015, Arnaud Barré
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

#include <btkGroundReactionWrenchFilter.h> // First (Fix for MSVC LNK2019)

#include "btkMEXObjectHandle.h"
#include "btkMXMeasure.h"
#include "btkMEXWarnLogToWarnMsgTxt.h"

#include <btkAcquisition.h>
#include <btkForcePlatformsExtractor.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  if (nrhs < 1) 
    mexErrMsgTxt("One input required.");
  else if (nrhs > 2)
    mexErrMsgTxt("Too many input arguments.");
  if (nlhs > 1)
    mexErrMsgTxt("Too many output arguments.");

  if (nrhs == 2)
    if ((mxGetClassID(prhs[1]) != mxDOUBLE_CLASS) || (mxGetNumberOfElements(prhs[1]) != 1))
      mexErrMsgTxt("Second input argument must be a real (double) value.");

  // Redirection of the btk::Logger::Warning stream.
  btk::MEXWarnLogToWarnMsgTxt warnRedir = btk::MEXWarnLogToWarnMsgTxt("btk:GetGroundReactionWrenches");

  // First output
  btk::Acquisition::Pointer acq = btk_MOH_get_object<btk::Acquisition>(prhs[0]);
  btk::ForcePlatformsExtractor::Pointer fpExtractor = btk::ForcePlatformsExtractor::New();
  fpExtractor->SetInput(acq);
  btk::GroundReactionWrenchFilter::Pointer grwFilter = btk::GroundReactionWrenchFilter::New();
  if (nrhs == 2)
  {
    grwFilter->SetThresholdState(true);
    grwFilter->SetThresholdValue(mxGetScalar(prhs[1]));
  }
  grwFilter->SetInput(fpExtractor->GetOutput());
  btk::WrenchCollection::Pointer grws = grwFilter->GetOutput();
  grws->Update();

  const char* fieldnames[] = {"P", "F", "M"};
  int numberOfFields =  sizeof(fieldnames) / sizeof(char*);
  int numberOfForcePlates = grws->GetItemNumber();
  
  plhs[0] = mxCreateStructMatrix(numberOfForcePlates, 1, numberOfFields, fieldnames);
  btk::WrenchCollection::ConstIterator itWrench = grws->Begin();
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
      std::string warnText = "Ground reaction wrench #" + btk::ToString(i) + " is empty. Send an email to BTK developers to inform them.";
      mexWarnMsgIdAndTxt("btk:GetGroundReactionWrenches", warnText.c_str());
    }
    ++itWrench;
  }
};

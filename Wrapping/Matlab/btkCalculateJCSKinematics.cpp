/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2010, Arnaud Barré
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

#include "btkMXObjectHandle.h"
#include "btkMEXStreambufToPrintf.h"
#include "btkMEXStreambufToWarnMsgTxt.h"

#include <btkModelJCSKinematicFilter.h>
#include <btkModel.h>

#include <algorithm>
#include <cctype>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  if(nrhs < 1)
    mexErrMsgTxt("One input arguments required.");
  if (nlhs > 1)
   mexErrMsgTxt("Too many output arguments.");
   
  btk::Model::Pointer input = btk_MOH_get_object<btk::Model>(prhs[0]);
   
  btk::ModelJCSKinematicFilter::Pointer jcsf = btk::ModelJCSKinematicFilter::New();
  jcsf->SetInput(input);
  
  int i = 1;
  while (i < nrhs)
  {
    if (!mxIsChar(prhs[i]) || mxIsEmpty(prhs[i]))
      mexErrMsgTxt("Option must be set by a non-empty string. Kinematic calculation aborted.");
    int strlen = (mxGetM(prhs[i]) * mxGetN(prhs[i]) * sizeof(mxChar)) + 1;
    char* s = (char*)mxMalloc(strlen);
    mxGetString(prhs[i], s, strlen);
    std::string uppercase = std::string(s);
    mxFree(s);
    std::transform(uppercase.begin(), uppercase.end(), uppercase.begin(), toupper);
    if (uppercase.compare("SEQUENCE") == 0)
    {
      if (nrhs < i + 2)
        mexErrMsgTxt("No enough parameter for the option 'Sequence'. Kinematic calculation aborted.");
      if (!mxIsNumeric(prhs[i+2]) || mxIsEmpty(prhs[i+2]) || mxIsComplex(prhs[i+2]) || (mxGetNumberOfElements(prhs[i+2]) != 3))
        mexErrMsgTxt("Sequence's configuration error. Kinematic calculation aborted.");
      double* seqF = mxGetPr(prhs[i+2]);
      int seq[3];
      for (int j = 0 ; j < 3 ; ++j)
        seq[j] = static_cast<int>(seqF[j]);
      if ((seq[0] < 0) || (seq[0] > 2) || (seq[1] < 0) || (seq[1] > 2) || (seq[2] < 0) || (seq[2] > 2))
        mexErrMsgTxt("Sequence's values error. Kinematic calculation aborted.");
      if (mxIsChar(prhs[i+1]) && !mxIsEmpty(prhs[i+1]))
      {
        int strlen = (mxGetM(prhs[i+1]) * mxGetN(prhs[i+1]) * sizeof(mxChar)) + 1;
        char* label = (char*)mxMalloc(strlen);
        mxGetString(prhs[i+1], label, strlen);
        jcsf->SetSequenceJCS(label, static_cast<btk::SequenceJCS::Axis>(seq[0]), static_cast<btk::SequenceJCS::Axis>(seq[1]), static_cast<btk::SequenceJCS::Axis>(seq[2]));
        mxFree(label);
      }
      else if (mxIsNumeric(prhs[i+1]) && !mxIsEmpty(prhs[i+1]) && !mxIsComplex(prhs[i+1]) && (mxGetNumberOfElements(prhs[i+1]) == 1))
        jcsf->SetSequenceJCS(static_cast<int>(mxGetScalar(prhs[i+1])), static_cast<btk::SequenceJCS::Axis>(seq[0]), static_cast<btk::SequenceJCS::Axis>(seq[1]), static_cast<btk::SequenceJCS::Axis>(seq[2]));
      else
        mexErrMsgTxt("Segment's parameter error. Kinematic calculation aborted.");
      i += 3;
    }
    else
      mexErrMsgTxt("Unknown option. Kinematic calculation aborted.");
  }
  
  // std::cout redirection to the mexPrintf function.
  btk::MEXStreambufToPrintf matlabStandardOutput;
  std::streambuf* stdStandardOutput = std::cout.rdbuf(&matlabStandardOutput);
  // std::cerr redirection to the mexWarnMsgTxt function.
  btk::MEXStreambufToWarnMsgTxt matlabErrorOutput;
  std::streambuf* stdErrorOutput = std::cerr.rdbuf(&matlabErrorOutput);
  
  jcsf->Update();

  // Back to the previous output buffer.
  matlabErrorOutput.requestNewLine();
  std::cout.rdbuf(stdStandardOutput);
  std::cerr.rdbuf(stdErrorOutput);
  
   plhs[0] = btk_MOH_create_handle(jcsf->GetOutput());
};

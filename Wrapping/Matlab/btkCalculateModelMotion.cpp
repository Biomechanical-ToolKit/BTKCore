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
#include "btkMXLandmarksTranslator.h"
#include "btkMEXStreambufToPrintf.h"
#include "btkMEXStreambufToWarnMsgTxt.h"

#include <btkModel.h>
#include <btkAcquisition.h>
#include <btkLandmarksTranslator.h>
#include <btkModelMotionFilter.h>
#include <btkBasicMotionBuilder.h>

#include <algorithm>
#include <cctype>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  if(nrhs < 3)
    mexErrMsgTxt("Two input arguments required.");
  if (nlhs > 1)
   mexErrMsgTxt("Too many output arguments.");
   
  btk::Model::Pointer input = btk_MOH_get_object<btk::Model>(prhs[0]);
  btk::Acquisition::Pointer acq = btk_MOH_get_object<btk::Acquisition>(prhs[1]);
  btk::LandmarksTranslator::Pointer tr;
  btk::BasicMotionBuilder::Pointer builder;
   
  // Builder
  btk::ModelConfigurator::Pointer configurator;
  int strlen = (mxGetM(prhs[2]) * mxGetN(prhs[2]) * sizeof(mxChar)) + 1;
  char* s = (char*)mxMalloc(strlen);
  mxGetString(prhs[2], s, strlen);
  std::string uppercase = std::string(s);
  mxFree(s);
  std::transform(uppercase.begin(), uppercase.end(), uppercase.begin(), toupper);
  if (uppercase.compare("BASIC") == 0)
    builder = btk::BasicMotionBuilder::New();
  else
    mexErrMsgTxt("Unknown frame builder.");
   
  if(nrhs >= 4)
    tr = btkMXGetLandmarksTranslator(prhs[3]);
   
  // std::cout redirection to the mexPrintf function.
  btk::MEXStreambufToPrintf matlabStandardOutput;
  std::streambuf* stdStandardOutput = std::cout.rdbuf(&matlabStandardOutput);
  // std::cerr redirection to the mexWarnMsgTxt function.
  btk::MEXStreambufToWarnMsgTxt matlabErrorOutput;
  std::streambuf* stdErrorOutput = std::cerr.rdbuf(&matlabErrorOutput);

  // Calibration
  btk::ModelMotionFilter::Pointer bsmd = btk::ModelMotionFilter::New();
  bsmd->SetAcquisition(acq);
  bsmd->SetModel(input);
  bsmd->SetLandmarksTranslator(tr);
  bsmd->SetMotionBuilder(builder);
  bsmd->Update();

  // Back to the previous output buffer.
  matlabErrorOutput.requestNewLine();
  std::cout.rdbuf(stdStandardOutput);
  std::cerr.rdbuf(stdErrorOutput);
  
  plhs[0] = btk_MOH_create_handle(bsmd->GetOutput());
};

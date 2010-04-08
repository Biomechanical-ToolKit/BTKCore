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

#include <btkAcquisitionFileReader.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  if(nrhs != 1)
    mexErrMsgTxt("One input required.");
  if (nlhs > 1)
    mexErrMsgTxt("Too many output arguments.");

  if (!mxIsChar(prhs[0]) || mxIsEmpty(prhs[0]))
   mexErrMsgTxt("The filename must be a string and can't be empty.");

  // std::cout redirection to the mexPrintf function.
  btk::MEXStreambufToPrintf matlabStandardOutput;
  std::streambuf* stdStandardOutput = std::cout.rdbuf(&matlabStandardOutput);
  // std::cerr redirection to the mexWarnMsgTxt function.
  btk::MEXStreambufToWarnMsgTxt matlabErrorOutput;
  std::streambuf* stdErrorOutput = std::cerr.rdbuf(&matlabErrorOutput);

  int strlen = (mxGetM(prhs[0]) * mxGetN(prhs[0]) * sizeof(mxChar)) + 1;
  char* filename = (char*)mxMalloc(strlen);
  mxGetString(prhs[0], filename, strlen); 
  
  btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
  reader->SetFilename(std::string(filename));
  
  try
  {
    reader->Update();
  }
  catch(std::exception& e)
  {
    mexErrMsgTxt(e.what());
  }
  catch(...)
  {
    mexErrMsgTxt("An unexpected error occurred.");
  }
  plhs[0] = btk_MOH_create_handle(reader->GetOutput());
  
  mxFree(filename);
  
  // Back to the previous output buffers.
  matlabErrorOutput.requestNewLine();
  std::cout.rdbuf(stdStandardOutput);
  std::cerr.rdbuf(stdErrorOutput);
};

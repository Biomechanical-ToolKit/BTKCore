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

#include "btkMEXObjectHandle.h"
#include "btkMEXStreambufToPrintf.h"
#include "btkMEXStreambufToWarnMsgTxt.h"

#include <btkAcquisitionFileWriter.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  if(nrhs != 2)
    mexErrMsgTxt("Two inputs required.");
  if (nlhs > 0)
   mexErrMsgTxt("Too many output arguments.");

  

  btk::Acquisition::Pointer acq = btk_MOH_get_object<btk::Acquisition>(prhs[0]);

  // std::cout redirection to the mexPrintf function.
  btk::MEXStreambufToPrintf matlabStandardOutput;
  std::streambuf* stdStandardOutput = std::cout.rdbuf(&matlabStandardOutput);
  // std::cerr redirection to the mexWarnMsgTxt function.
  btk::MEXStreambufToWarnMsgTxt matlabErrorOutput;
  std::streambuf* stdErrorOutput = std::cerr.rdbuf(&matlabErrorOutput); 

  int strlen = (mxGetM(prhs[1]) * mxGetN(prhs[1]) * sizeof(mxChar)) + 1;
  char* filename = (char*)mxMalloc(strlen);
  mxGetString(prhs[1], filename, strlen); 
  
  btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
  writer->SetFilename(std::string(filename));
  writer->SetInput(acq);
  
  try
  {
    writer->Update();
  }
  catch(std::exception& e)
  {
    mexErrMsgTxt(e.what());
    std::remove(filename);
  }
  catch(...)
  {
    mexErrMsgTxt("An unexpected error occured.");
    std::remove(filename);
  }

  mxFree(filename);

  // Back to the previous output buffer.
  matlabErrorOutput.requestNewLine();
  std::cout.rdbuf(stdStandardOutput);
  std::cerr.rdbuf(stdErrorOutput);
};


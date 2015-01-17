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

#include "btkMEXObjectHandle.h"
#include "btkMEXWarnLogToWarnMsgTxt.h"

#include <btkAcquisitionFileReader.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  if(nrhs != 1)
    mexErrMsgTxt("One input required.");
  if (nlhs > 3)
    mexErrMsgTxt("Too many output arguments.");

  if (!mxIsChar(prhs[0]) || mxIsEmpty(prhs[0]))
   mexErrMsgTxt("The filename must be a string and can't be empty.");

  // Redirection of the btk::Logger::Warning stream.
  btk::MEXWarnLogToWarnMsgTxt warnRedir = btk::MEXWarnLogToWarnMsgTxt("btk:ReadAcquisition");

  size_t strlen_ = (mxGetM(prhs[0]) * mxGetN(prhs[0]) * sizeof(mxChar)) + 1;
  char* filename = (char*)mxMalloc(strlen_);
  mxGetString(prhs[0], filename, strlen_); 
  
  btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
  reader->SetFilename(std::string(filename));
  
  mxFree(filename);
  
  try
  {
    reader->Update();
  }
  catch(std::exception& e)
  {
    // Octave seems to not call the destructor of the btkSharedPtr when an exception is thrown (possible memory leak).
    reader.reset();
    mexErrMsgTxt(e.what());
  }
  catch(...)
  {
    reader.reset();
    mexErrMsgTxt("An unexpected error occurred.");
  }
  
  plhs[0] = btk_MOH_create_handle(reader->GetOutput());
  if (nlhs > 1) // Byte Order
    plhs[1] = mxCreateString(reader->GetAcquisitionIO()->GetByteOrderAsString().c_str());
  if (nlhs > 2) // Storage format
    plhs[2] = mxCreateString(reader->GetAcquisitionIO()->GetStorageFormatAsString().c_str());
  
#if defined(__APPLE__) || (defined(BTK_BUILD_SHARED_LIBS) && defined(__unix__))
  // It seems to be related only to Linux with shared libraries
  // This fix was only tested with Matlab r2009a (7.8)
  // FIXME: This solution clear all the acquisitions and not only the ones 
  //        created from this function
  mexAtExit(btk::MEXHandleCollector<btk::Acquisition>::ManualClear);
#endif
};

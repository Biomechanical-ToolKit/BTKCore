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

#include "btkMXObjectHandle.h"
#include "btkMXAnalog.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  if(nrhs < 2)
    mexErrMsgTxt("Two inputs required.");
  if (nlhs > 2)
    mexErrMsgTxt("Too many output arguments.");

  if (mxIsEmpty(prhs[1]) || (!mxIsChar(prhs[1]) && (!mxIsNumeric(prhs[1]) || mxIsComplex(prhs[1]) || (mxGetNumberOfElements(prhs[1]) != 1))))
    mexErrMsgTxt("Analog's index must be a non-empty string or an integer.");

  btk::Acquisition::Pointer acq = btk_MOH_get_object<btk::Acquisition>(prhs[0]);

  if (mxIsChar(prhs[1]))
  {
    size_t strlen_ = (mxGetM(prhs[1]) * mxGetN(prhs[1]) * sizeof(mxChar)) + 1;
    char* label = (char*)mxMalloc(strlen_);
    mxGetString(prhs[1], label, strlen_);
    btk::Acquisition::AnalogIterator itAnalog = acq->FindAnalog(label);
    if (itAnalog == acq->EndAnalog())
    {
      std::string err = "No analog channel with label: '" + std::string(label) + "'.";
      mxFree(label);
      mexErrMsgTxt(err.c_str());
    }
    mxFree(label);
    acq->RemoveAnalog(itAnalog);
  }
  else
  {
    int idx = static_cast<int>(mxGetScalar(prhs[1])) - 1;
    if ((idx < 0) || (idx >= acq->GetAnalogNumber()))
      mexErrMsgTxt("Analog's index out of range.");
    acq->RemoveAnalog(idx);
  }

  // Return updated points
  btkMXCreateAnalogsStructure(acq, nlhs, plhs);
};


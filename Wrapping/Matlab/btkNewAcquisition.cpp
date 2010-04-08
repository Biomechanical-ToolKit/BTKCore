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

#include <btkAcquisition.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  if(nrhs < 2)
    mexErrMsgTxt("Two input arguments required.");
  if (nlhs > 1)
    mexErrMsgTxt("Too many output arguments.");

  if (!mxIsNumeric(prhs[0]) || !mxIsNumeric(prhs[1])
    || mxIsEmpty(prhs[0]) || mxIsEmpty(prhs[1])
    || mxIsComplex(prhs[0]) || mxIsComplex(prhs[1]) 
    || (mxGetNumberOfElements(prhs[0]) != 1) || (mxGetNumberOfElements(prhs[1]) != 1))
    mexErrMsgTxt("All input arguments must be scalar integers.");
  if(nrhs >= 3)
    if (!mxIsNumeric(prhs[2]))
      mexErrMsgTxt("All input arguments must be scalar integers.");
  if(nrhs >= 4)
    if (!mxIsNumeric(prhs[3]))
      mexErrMsgTxt("All input arguments must be scalar integers.");
  int pn, fn, an, r;
  pn = static_cast<int>(mxGetScalar(prhs[0]));
  fn = static_cast<int>(mxGetScalar(prhs[1]));
  an = 0;
  r = 1;
  if(nrhs >= 3)
    an = static_cast<int>(mxGetScalar(prhs[2]));
  if(nrhs >= 4)
    r = static_cast<int>(mxGetScalar(prhs[3]));
  
  if (fn == 0)
    mexErrMsgTxt("Impossible to set the number of frames to 0.");
  if (r == 0)
    mexErrMsgTxt("Impossible to set the analog sample number by point frame to 0.");

  btk::Acquisition::Pointer acq = btk::Acquisition::New();
  acq->Init(pn, fn, an, r);
  plhs[0] = btk_MOH_create_handle(acq);
};

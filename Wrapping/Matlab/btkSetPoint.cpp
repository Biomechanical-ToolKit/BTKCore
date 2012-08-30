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
#include "btkMXPoint.h"

// btkSetPoint(h, i, values, residual)
// btkSetPoint(h, label, values, residual)
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  if(nrhs < 3)
    mexErrMsgTxt("Minimum of three inputs required.");
  if (nlhs > 2)
    mexErrMsgTxt("Too many output arguments.");

  int vn = 0, rn = 0;

  if (!mxIsNumeric(prhs[2]) || mxIsEmpty(prhs[2]) || mxIsComplex(prhs[2]) || (mxGetN(prhs[2]) != 3))
    mexErrMsgTxt("Point's values must have a second dimension equals to 3.");
  vn = static_cast<int>(mxGetM(prhs[2]));
  if (nrhs >= 4)
  {
    if (!mxIsNumeric(prhs[3]) || mxIsEmpty(prhs[3]) || mxIsComplex(prhs[3]) || (mxGetN(prhs[3]) != 1))
      mexErrMsgTxt("Point's residual must have a second dimension equals to 1.");
    rn = static_cast<int>(mxGetM(prhs[3]));
  }
  if ((rn != vn) && (rn != 0))
    mexErrMsgTxt("Point's data have not the same number of frames.");
  if (nrhs >= 5)
  {
    if (!mxIsChar(prhs[4]))
      mexErrMsgTxt("Point's description must be a string");
  }

  btk::Acquisition::Pointer acq = btk_MOH_get_object<btk::Acquisition>(prhs[0]);

  if (vn != acq->GetPointFrameNumber())
    mexErrMsgTxt("Frame number mismatching.");

  btk::Point::Pointer point = btkMXGetPoint(acq, nrhs, prhs);

  memcpy(point->GetValues().data(), mxGetPr(prhs[2]) , mxGetNumberOfElements(prhs[2]) * sizeof(double));
  if (nrhs >= 4)
    memcpy(point->GetResiduals().data(), mxGetPr(prhs[3]) , mxGetNumberOfElements(prhs[3]) * sizeof(double)); 
  if (nrhs >= 5)
  {
    size_t strlen_ = (mxGetM(prhs[4]) * mxGetN(prhs[4]) * sizeof(mxChar)) + 1;
    char* desc = (char*)mxMalloc(strlen_);
    mxGetString(prhs[4], desc, strlen_);
    point->SetDescription(desc);
    mxFree(desc);
  }
  // Return updated points
  btkMXCreatePointsStructure(acq, nlhs, plhs);
};


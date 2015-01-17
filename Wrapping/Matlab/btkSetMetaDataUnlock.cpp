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
#include "btkMXMetaData.h"

#include <btkAcquisition.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  if(nrhs < 3)
    mexErrMsgTxt("Minimum of three inputs required.");
  if (nlhs > 1)
    mexErrMsgTxt("Too many output arguments.");

  for (int i = 1 ; i < nrhs-1 ; ++i)
  {
    if (mxIsEmpty(prhs[i]) || (!mxIsChar(prhs[i]) && ((mxGetClassID(prhs[i]) != mxDOUBLE_CLASS) || mxIsComplex(prhs[i]) || (mxGetNumberOfElements(prhs[i]) != 1))))
      mexErrMsgTxt("Metadata's label or index must be set by a non-empty string or an integer (double value) respectively.");
  }
  
  if ((mxGetClassID(prhs[nrhs-1]) != mxDOUBLE_CLASS) || mxIsEmpty(prhs[nrhs-1]) || mxIsComplex(prhs[nrhs-1]) || (mxGetNumberOfElements(prhs[nrhs-1]) != 1))
    mexErrMsgTxt("Metadata's unlock state must be set by a double value representing an integer.");
  
  btk::Acquisition::Pointer acq = btk_MOH_get_object<btk::Acquisition>(prhs[0]);
  btk::MetaData::Iterator it;
  btkMXExtractMetaDataIterator(&it, nrhs-2,  prhs, acq->GetMetaData());
  
  int l = static_cast<int>(mxGetScalar(prhs[nrhs-1]));
  (*it)->SetUnlockState((l == 0) ? false : true);
  
  if (nlhs > 0)
    plhs[0] = btkMXCreateMetaDataStructure(acq->GetMetaData());
};


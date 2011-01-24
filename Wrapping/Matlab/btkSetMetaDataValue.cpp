/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2011, Arnaud Barré
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
#include "btkMXMetaData.h"

#include <btkAcquisition.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  if(nrhs < 3)
    mexErrMsgTxt("Minimum of three inputs required.");
  if (nlhs > 1)
    mexErrMsgTxt("Too many output arguments.");

  for (int i = 1 ; i < nrhs-2 ; ++i)
  {
    if (mxIsEmpty(prhs[i]) || (!mxIsChar(prhs[i]) && (!mxIsNumeric(prhs[i]) || mxIsComplex(prhs[i]) || (mxGetNumberOfElements(prhs[i]) != 1))))
      mexErrMsgTxt("Metadata's label or index must be set by a non-empty string or an integer respectively.");
  }
  if (!mxIsNumeric(prhs[nrhs-2]) || mxIsComplex(prhs[nrhs-2]) || (mxGetNumberOfElements(prhs[nrhs-2]) != 1))
    mexErrMsgTxt("The index for the metadata's value must be set by a single integer");
  if ((mxIsCell(prhs[nrhs-1]) && (mxGetNumberOfElements(prhs[nrhs-1]) != 1)) 
       || (mxIsChar(prhs[nrhs-1]) && (mxGetM(prhs[nrhs-1]) != 1))
       || (mxIsNumeric(prhs[nrhs-1]) && (mxIsComplex(prhs[nrhs-1]) || (mxGetNumberOfElements(prhs[nrhs-1]) != 1))))
    mexErrMsgTxt("Unsupported metadata's value or the number of elements is greater than 1.");
  
  btk::Acquisition::Pointer acq = btk_MOH_get_object<btk::Acquisition>(prhs[0]);
  btk::MetaData::Iterator it;
  btk::MetaData::Pointer parent = btkMXExtractMetaDataIterator(&it, nrhs-3, prhs, acq->GetMetaData());
  
  if (!(*it)->HasInfo())
    mexErrMsgTxt("No metadata's info.");
  
  size_t index = static_cast<size_t>(mxGetScalar(prhs[nrhs-2])) - 1;
  if ((index < 0) || (index >= (*it)->GetInfo()->GetValues().size()))
    mexErrMsgTxt("Invalid index to extract one metadata's value.");
    
  const mxArray* data = 0; 
  if (mxIsCell(prhs[nrhs-1]))
  {
    data = mxGetCell(prhs[nrhs-1], 0);
    if (!data || !mxIsChar(data))
      mexErrMsgTxt("Error in the value's format: only a string are accepted in cell.");
  }
  else
    data = prhs[nrhs-1];
  
  if (mxIsChar(data))
  {
    size_t strlen_ = (mxGetM(data) * mxGetN(data) * sizeof(mxChar)) + 1;
    char* d = (char*)mxMalloc(strlen_);
    mxGetString(data, d, strlen_);
    (*it)->GetInfo()->SetValue(index, d);
    mxFree(d);
  }
  else
    (*it)->GetInfo()->SetValue(index, mxGetScalar(data));
  
  if (nlhs > 0)
    plhs[0] = btkMXCreateMetaDataStructure(acq->GetMetaData());
};


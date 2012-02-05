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
    if (mxIsEmpty(prhs[i]) || (!mxIsChar(prhs[i]) && (!mxIsNumeric(prhs[i]) || mxIsComplex(prhs[i]) || (mxGetNumberOfElements(prhs[i]) != 1))))
      mexErrMsgTxt("Metadata's label or index must be set by a non-empty string or an integer respectively.");
  }
  btk::MetaDataInfo::Format f = btk::MetaDataInfo::Char;
  if (mxIsChar(prhs[nrhs-1]) && (mxGetM(prhs[nrhs-1]) != 1))
    mexErrMsgTxt("Unsupported metadata's format. Must be set by one of the following format: Char, Byte, Integer or Real");
  else
  {
    size_t strlen_ = (mxGetM(prhs[nrhs-1]) * mxGetN(prhs[nrhs-1]) * sizeof(mxChar)) + 1;
    char* format = (char*)mxMalloc(strlen_);
    mxGetString(prhs[nrhs-1], format, strlen_);
    std::string uppercase = std::string(format);
    mxFree(format);
    std::transform(uppercase.begin(), uppercase.end(), uppercase.begin(), toupper);
    if (uppercase.compare("CHAR") == 0)
      f = btk::MetaDataInfo::Char;
    else if(uppercase.compare("BYTE") == 0)
      f = btk::MetaDataInfo::Byte;
    else if(uppercase.compare("INTEGER") == 0)
      f = btk::MetaDataInfo::Integer;
    else if(uppercase.compare("REAL") == 0)
      f = btk::MetaDataInfo::Real;
    else
      mexErrMsgTxt("Unsupported metadata's format. Must be set by one of the following format: Char, Byte, Integer or Real");
  }
  
  btk::Acquisition::Pointer acq = btk_MOH_get_object<btk::Acquisition>(prhs[0]);
  btk::MetaData::Iterator it;
  btk::MetaData::Pointer parent = btkMXExtractMetaDataIterator(&it, nrhs-2, prhs, acq->GetMetaData());
  
  if (!(*it)->HasInfo())
    mexErrMsgTxt("No metadata's info.");
  
  (*it)->GetInfo()->SetFormat(f);
  
  if (nlhs > 0)
    plhs[0] = btkMXCreateMetaDataStructure(acq->GetMetaData());
};


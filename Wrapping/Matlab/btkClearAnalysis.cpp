/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2014, Arnaud Barré
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

#include <btkMetaDataUtils.h>  // First (Fix for MSVC LNK2019)

#include "btkMEXObjectHandle.h"

#include <btkAcquisition.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  if(nrhs!=1)
    mexErrMsgTxt("One input required.");

  btkMXCheckNoOuput(nlhs, plhs); // Only when there is no output for the function.

  btk::Acquisition::Pointer acq = btk_MOH_get_object<btk::Acquisition>(prhs[0]);
  btk::MetaData::Pointer metadata = acq->GetMetaData();
  btk::MetaData::ConstIterator itAnalysis = metadata->FindChild("ANALYSIS");

  if (itAnalysis == metadata->End())
    return;

  // Char
  const char* names[] = {"NAMES", "CONTEXTS", "SUBJECTS", "UNITS", "DESCRIPTIONS"};
  int numberOfNames =  sizeof(names) / sizeof(char*);
  std::vector<uint8_t> dims = std::vector<uint8_t>(2,0);
  for (int i = 0 ; i < numberOfNames ; ++i)
  {
    btk::MetaData::ConstIterator it = (*itAnalysis)->FindChild(names[i]);
    if (it != (*itAnalysis)->End())
    {
      if ((*it)->HasInfo())
        (*it)->GetInfo()->SetDimensions(dims);
    }
  }
  // Real
  btk::MetaData::ConstIterator it = (*itAnalysis)->FindChild("VALUES");
  if (it != (*itAnalysis)->End())
  {
    if ((*it)->HasInfo())
      (*it)->GetInfo()->SetDimensions(std::vector<uint8_t>(1,0));
  }
  // Integer
  btk::MetaDataCreateChild((*itAnalysis), "USED", (int16_t)0);
};



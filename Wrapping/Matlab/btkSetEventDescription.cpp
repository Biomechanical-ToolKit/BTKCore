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

#include "btkMXObjectHandle.h"
#include "btkMXEvent.h"

#include <btkAcquisition.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  if(nrhs != 3)
    mexErrMsgTxt("Three input arguments required.");
  if (nlhs > 0)
    mexErrMsgTxt("Too many output arguments.");
  
  if (!mxIsChar(prhs[1]) ||  mxIsEmpty(prhs[1]))
    mexErrMsgTxt("The event's label must be a non-empty string.");
   
  if (!mxIsChar(prhs[2]))
    mexErrMsgTxt("The description must be set by a string.");

  btk::Acquisition::Pointer acq = btk_MOH_get_object<btk::Acquisition>(prhs[0]);
  btk::EventCollection::Pointer events = acq->GetEvents();
  
  int strlen = (mxGetM(prhs[1]) * mxGetN(prhs[1]) * sizeof(mxChar)) + 1;
  char* l = (char*)mxMalloc(strlen);
  mxGetString(prhs[1], l, strlen);
  std::string label = std::string(l);
  
  strlen = (mxGetM(prhs[2]) * mxGetN(prhs[2]) * sizeof(mxChar)) + 1;
  char* d = (char*)mxMalloc(strlen);
  mxGetString(prhs[2], d, strlen);
  std::string desc = std::string(d);
  
  for (btk::EventCollection::Iterator it = events->Begin() ; it != events->End() ; ++it)
  {
    if ((*it)->GetLabel().compare(label) == 0)
      (*it)->SetDescription(desc);
  }
  
  mxFree(l);
  mxFree(d);
  
  btkMXCreateEventsStructure(acq, nlhs, plhs);
};


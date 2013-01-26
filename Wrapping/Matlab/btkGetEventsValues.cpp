/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2013, Arnaud Barré
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
#include "btkMXEvent.h"

#include <btkAcquisition.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  if(nrhs!=1)
    mexErrMsgTxt("One input required.");
  if (nlhs > 4)
    mexErrMsgTxt("Too many output arguments.");

  btk::Acquisition::Pointer acq = btk_MOH_get_object<btk::Acquisition>(prhs[0]);
  
  btk::EventCollection::Pointer evts = acq->GetEvents();
  int numberOfEvents = evts->GetItemNumber();
  
  mwSize dims[1];
  dims[0] = numberOfEvents;
  btk::EventCollection::ConstIterator it;
  
  // Time
  if (nlhs >= 1)
  {
    it = evts->Begin();
    plhs[0] = mxCreateDoubleMatrix(numberOfEvents, 1, mxREAL);
    double* values = mxGetPr(plhs[0]);
    for (int i = 0 ; i < numberOfEvents ; ++i)
    {
      values[i] = (*it)->GetTime();
      ++it;
    }
  }
  // Label
  if (nlhs >= 2)
  {
    plhs[1] = mxCreateCellArray(1, dims);
    it = evts->Begin();
    for (int i = 0 ; i < numberOfEvents ; ++i)
    {
      mxSetCell(plhs[1], (mwIndex)i, mxCreateString((*it)->GetLabel().c_str()));
      ++it;
    }
    
  }
  // Description
  if (nlhs >= 3)
  {
    plhs[2] = mxCreateCellArray(1, dims);
    it = evts->Begin();
    for (int i = 0 ; i < numberOfEvents ; ++i)
    {
      mxSetCell(plhs[2], (mwIndex)i, mxCreateString((*it)->GetDescription().c_str()));
      ++it;
    }
  }
  // ID
  if (nlhs >= 4)
  {
    it = evts->Begin();
    plhs[3] = mxCreateDoubleMatrix(numberOfEvents, 1, mxREAL);
    double* values = mxGetPr(plhs[3]);
    for (int i = 0 ; i < numberOfEvents ; ++i)
    {
      values[i] = (*it)->GetId();
      ++it;
    }
  }
};

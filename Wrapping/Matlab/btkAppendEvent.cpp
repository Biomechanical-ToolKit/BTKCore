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
#include "btkMXEvent.h"

#include <btkAcquisition.h>
#include <btkEvent.h>

// btkAppendEvent(h, label, time, context)
// btkAppendEvent(h, label, time, context, subject)
// btkAppendEvent(h, label, time, context, subject, description)
// btkAppendEvent(h, label, time, context, subject, description, id)
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  if(nrhs < 4)
    mexErrMsgTxt("Minimum of four inputs required.");
  if (nlhs > 2)
    mexErrMsgTxt("Too many output arguments.");
  
  btk::Event::Pointer evt = btk::Event::New();
  btk::Acquisition::Pointer acq = btk_MOH_get_object<btk::Acquisition>(prhs[0]);
  btk::Acquisition::EventConstIterator it;
  
  // Label
  if (!mxIsChar(prhs[1]) || mxIsEmpty(prhs[1]))
    mexErrMsgTxt("The label must be a non-empty string.");
  else
  {
    size_t strlen_ = (mxGetM(prhs[1]) * mxGetN(prhs[1]) * sizeof(mxChar)) + 1;
    char* buffer = (char*)mxMalloc(strlen_);
    mxGetString(prhs[1], buffer, strlen_);
    evt->SetLabel(buffer);
    mxFree(buffer);
    it = acq->FindEvent(evt->GetLabel());
  }
  // Time
  if (!mxIsNumeric(prhs[2]) || mxIsEmpty(prhs[2]) || mxIsComplex(prhs[2]) || (mxGetNumberOfElements(prhs[2]) != 1))
    mexErrMsgTxt("The time must be set by a numerical value.");
  else
  {
    double t = mxGetScalar(prhs[2]);
    evt->SetTime(t);
  }
  // Context
  if (!mxIsChar(prhs[3]))
    mexErrMsgTxt("The context must be a string.");
  else
  {
    size_t strlen_ = (mxGetM(prhs[3]) * mxGetN(prhs[3]) * sizeof(mxChar)) + 1;
    char* buffer = (char*)mxMalloc(strlen_);
    mxGetString(prhs[3], buffer, strlen_);
    evt->SetContext(buffer);
    mxFree(buffer);
  }
  // Subject (optional)
  if (nrhs >= 5) 
  {
    if (!mxIsChar(prhs[4]))
      mexErrMsgTxt("The subject must be a string.");
    else
    {
      size_t strlen_ = (mxGetM(prhs[4]) * mxGetN(prhs[4]) * sizeof(mxChar)) + 1;
      char* buffer = (char*)mxMalloc(strlen_);
      mxGetString(prhs[4], buffer, strlen_);
      evt->SetSubject(buffer);
      mxFree(buffer);
    }
  }
  else // Use the first name in the SUBJECTS:NAMES parameter.
  {
    btk::MetaData::ConstIterator subjects = acq->GetMetaData()->FindChild("SUBJECTS");
    if (subjects != acq->GetMetaData()->End())
    {
      btk::MetaData::ConstIterator names = (*subjects)->FindChild("NAMES");
      if (names != (*subjects)->End())
      {
        if ((*names)->GetInfo().get() != 0)
        {
          if ((*names)->GetInfo()->GetDimensionsProduct(1) != 0)
          {
            std::string subject = (*names)->GetInfo()->ToString(0);
            subject =subject.erase(subject.find_last_not_of(' ') + 1);
            subject = subject.erase(0, subject.find_first_not_of(' '));
            evt->SetSubject(subject);
          }
        }
      }
    } 
  }
  // Description (optional)
  if (nrhs >= 6)
  {
    if (!mxIsChar(prhs[5]))
      mexErrMsgTxt("The description must be a string.");
    else
    {
      size_t strlen_ = (mxGetM(prhs[5]) * mxGetN(prhs[5]) * sizeof(mxChar)) + 1;
      char* buffer = (char*)mxMalloc(strlen_);
      mxGetString(prhs[5], buffer, strlen_);
      evt->SetDescription(buffer);
      mxFree(buffer);
    }
  }
  else // Use the description of the first event with the same label.
  {
    if (it != acq->EndEvent())
      evt->SetDescription((*it)->GetDescription());
  }
  // ID (optional)
  if (nrhs >= 7)
  {
    if (!mxIsNumeric(prhs[6]) || mxIsEmpty(prhs[6]) || mxIsComplex(prhs[6]) || (mxGetNumberOfElements(prhs[6]) != 1))
      mexErrMsgTxt("The ID must be set by a single integer value.");
    else
      evt->SetId(static_cast<int>(mxGetScalar(prhs[6])));
  }
  else // Use the ID of the first event with the same label.
  {
    if (it != acq->EndEvent())
      evt->SetId((*it)->GetId());
  }
  
  acq->AppendEvent(evt);
  
  btkMXCreateEventsStructure(acq, nlhs, plhs);
};


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

#include <algorithm>
#include <cctype>

// btkRemoveEvent(h, ["Time", time, ["Label", label, ["Context", context, ["Subject", subject]]]])

void keepEventsWithTime(double t, btk::EventCollection::Pointer events)
{
  btk::EventCollection::Iterator it = events->Begin();
  while(it != events->End())
  {
    if (((*it)->GetTime() > t + 0.00005) || ((*it)->GetTime() < t - 0.00005))
      it = events->RemoveItem(it);
    else
      ++it;
  }  
};

void keepEventsWithLabel(std::string label, btk::EventCollection::Pointer events)
{
  btk::EventCollection::Iterator it = events->Begin();
  while(it != events->End())
  {
    if ((*it)->GetLabel().compare(label) != 0)
      it = events->RemoveItem(it);
    else
      ++it;
  }  
};

void keepEventsWithContext(std::string context, btk::EventCollection::Pointer events)
{
  btk::EventCollection::Iterator it = events->Begin();
  while(it != events->End())
  {
    if ((*it)->GetContext().compare(context) != 0)
      it = events->RemoveItem(it);
    else
      ++it;
  }  
};

void keepEventsWithSubject(std::string subject, btk::EventCollection::Pointer events)
{
  btk::EventCollection::Iterator it = events->Begin();
  while(it != events->End())
  {
    if ((*it)->GetSubject().compare(subject) != 0)
      it = events->RemoveItem(it);
    else
      ++it;
  }  
};

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  if(nrhs < 1)
    mexErrMsgTxt("One input required.");
  if ((nrhs % 2) != 1)
    mexErrMsgTxt("Incorrect number of inputs.");
  if (nlhs > 2)
    mexErrMsgTxt("Too many output arguments.");
    
  btk::Acquisition::Pointer acq = btk_MOH_get_object<btk::Acquisition>(prhs[0]);
  btk::EventCollection::Pointer events = acq->GetEvents()->Clone();
  
  double timeOption = -1.0;
  char* labelOption = 0, contextOption = 0, subjectOption = 0;
  char* option = 0;
  const char* options[] = {"TIME", "LABEL", "CONTEXT", "SUBJECT"};
  int numberOfOptions =  sizeof(options) / sizeof(char*);
  for (int i = 1 ; i < nrhs ; i += 2)
  {
    if (!mxIsChar(prhs[i]) || mxIsEmpty(prhs[i]))
      mexErrMsgTxt("Error during the parsing of options.");
    int strlen = (mxGetM(prhs[i]) * mxGetN(prhs[i]) * sizeof(mxChar)) + 1;
    option = (char*)mxMalloc(strlen);
    mxGetString(prhs[i], option, strlen);
    int j = 0;
    for (j = 0 ; j < numberOfOptions ; ++j)
    {
      std::string uppercase = std::string(option);
      std::transform(uppercase.begin(), uppercase.end(), uppercase.begin(), toupper);
      if (uppercase.compare(options[j]) == 0)
      {
        switch(j)
        {
        case 0:
          if (!mxIsNumeric(prhs[i+1]) || mxIsEmpty(prhs[i+1]) || mxIsComplex(prhs[i+1]) || (mxGetN(prhs[i+1]) != 1))
            mexErrMsgTxt("Time option must be followed by a single numerical value.");
          keepEventsWithTime(mxGetScalar(prhs[i+1]), events);
          break;
        case 1:
          {
          if (!mxIsChar(prhs[i+1]))
            mexErrMsgTxt("Label option must be followed by a string.");
          int strlen2 = (mxGetM(prhs[i+1]) * mxGetN(prhs[i+1]) * sizeof(mxChar)) + 1;
          char* buf = (char*)mxMalloc(strlen2);
          mxGetString(prhs[i+1], buf, strlen2);
          keepEventsWithLabel(buf, events);
          mxFree(buf);
          break;
          }
        case 2:
          {
          if (!mxIsChar(prhs[i+1]))
            mexErrMsgTxt("Subject option must be followed by a string.");
          int strlen2 = (mxGetM(prhs[i+1]) * mxGetN(prhs[i+1]) * sizeof(mxChar)) + 1;
          char* buf = (char*)mxMalloc(strlen2);
          mxGetString(prhs[i+1], buf, strlen2);
          keepEventsWithContext(buf, events);
          mxFree(buf);
          break;
          }
        case 3:
          {
          if (!mxIsChar(prhs[i+1]))
            mexErrMsgTxt("Context option must be followed by a string.");
          int strlen2 = (mxGetM(prhs[i+1]) * mxGetN(prhs[i+1]) * sizeof(mxChar)) + 1;
          char* buf = (char*)mxMalloc(strlen2);
          mxGetString(prhs[i+1], buf, strlen2);
          keepEventsWithSubject(buf, events);
          mxFree(buf);
          break;
          }
        }
        break;
      }
    }
    std::string errMsg;
    if (j == numberOfOptions)
      errMsg = "Unknown option: '" + std::string(option) + "'.";
    mxFree(option);
    if (!errMsg.empty())
      mexErrMsgTxt(errMsg.c_str());
  }
  
  for(btk::EventCollection::Iterator it1 = events->Begin() ; it1 != events->End() ; ++it1)
  {
    for(btk::Acquisition::EventIterator it2 = acq->BeginEvent() ; it2 != acq->EndEvent() ; ++it2)
    {
      if (*(*it1) == *(*it2))
      {
        acq->RemoveEvent(it2);
        break;
      }
    }
  }

  btkMXCreateEventsStructure(acq, nlhs, plhs);
};


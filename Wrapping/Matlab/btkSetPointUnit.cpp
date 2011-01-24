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
#include "btkMXPoint.h"

#include <algorithm>
#include <cctype>

// btkSetPointsUnit(h, type, unit)
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  if(nrhs < 3)
    mexErrMsgTxt("Three inputs required.");

  btkMXCheckNoOuput(nlhs, plhs); // Only when there is no output for the function.

  if (!mxIsChar(prhs[1]) || mxIsEmpty(prhs[1]))
    mexErrMsgTxt("Point's type must be set by a non-empty string.");
  
  if (!mxIsChar(prhs[2]) || mxIsEmpty(prhs[2]))
    mexErrMsgTxt("Unit must be set by a non-empty string.");
  
  btk::Acquisition::Pointer acq = btk_MOH_get_object<btk::Acquisition>(prhs[0]);
  
  size_t strlen_ = (mxGetM(prhs[1]) * mxGetN(prhs[1]) * sizeof(mxChar)) + 1;
  char* type = (char*)mxMalloc(strlen_);
  mxGetString(prhs[1], type, strlen_);
  
  strlen_ = (mxGetM(prhs[2]) * mxGetN(prhs[2]) * sizeof(mxChar)) + 1;
  char* unit = (char*)mxMalloc(strlen_);
  mxGetString(prhs[2], unit, strlen_);
  
  std::string uppercase = std::string(type);
  mxFree(type);
  std::transform(uppercase.begin(), uppercase.end(), uppercase.begin(), toupper);
  if (uppercase.compare("MARKER") == 0)
    acq->SetPointUnit(btk::Point::Marker, unit);
  else if(uppercase.compare("ANGLE") == 0)
    acq->SetPointUnit(btk::Point::Angle, unit);
  else if(uppercase.compare("FORCE") == 0)
    acq->SetPointUnit(btk::Point::Force, unit);
  else if(uppercase.compare("MOMENT") == 0)
    acq->SetPointUnit(btk::Point::Moment, unit);
  else if(uppercase.compare("POWER") == 0)
    acq->SetPointUnit(btk::Point::Power, unit);
  else if(uppercase.compare("SCALAR") == 0)
    acq->SetPointUnit(btk::Point::Scalar, unit);
  else if(uppercase.compare("REACTION") == 0)
    mexWarnMsgTxt("The type Reaction is a special case and doesn't have a unit.");
  else
  { 
    mxFree(unit);
    std::string err = "Unknown point's type: '" + std::string(type) + "'.";    
    mexErrMsgTxt(err.c_str());
  }
  mxFree(unit);
};


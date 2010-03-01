/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2010, Arnaud Barré
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

#include <btkModel.h>
#include <btkUpperLimbModelConfigurator.h>

#include <algorithm>
#include <cctype>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  if(nrhs < 2)
    mexErrMsgTxt("Two input arguments required.");
  if (nlhs > 1)
   mexErrMsgTxt("Too many output arguments.");

  if (!mxIsChar(prhs[0]) || mxIsEmpty(prhs[0]))
    mexErrMsgTxt("Model's type must be a non-empty string.");
  
  if (!mxIsChar(prhs[1]) || mxIsEmpty(prhs[1]))
    mexErrMsgTxt("Model's side must be a non-empty string.");
    
  if(nrhs >= 3)
    if (!mxIsChar(prhs[2]))
      mexErrMsgTxt("All input arguments must be strings.");
  if(nrhs >= 4)
    if (!mxIsChar(prhs[3]))
      mexErrMsgTxt("All input arguments must be strings.");

  std::string uppercase;
  
  // Configurator
  btk::ModelConfigurator::Pointer configurator;
  int strlen = (mxGetM(prhs[0]) * mxGetN(prhs[0]) * sizeof(mxChar)) + 1;
  char* s = (char*)mxMalloc(strlen);
  mxGetString(prhs[0], s, strlen);
  uppercase = std::string(s);
  mxFree(s);
  std::transform(uppercase.begin(), uppercase.end(), uppercase.begin(), toupper);
  if (uppercase.compare("UPPERLIMB") == 0)
    configurator = btk::UpperLimbModelConfigurator::New();
  else
    mexErrMsgTxt("Unknown configurator.");
  
  // Side
  strlen = (mxGetM(prhs[1]) * mxGetN(prhs[1]) * sizeof(mxChar)) + 1;
  s = (char*)mxMalloc(strlen);
  mxGetString(prhs[1], s, strlen);
  uppercase = std::string(s);
  mxFree(s);
  btk::Side side;
  std::transform(uppercase.begin(), uppercase.end(), uppercase.begin(), toupper);
  if (uppercase.compare("LEFT") == 0)
    side = btk::Left;
  else if (uppercase.compare("RIGHT") == 0)
    side = btk::Right;
  else if (uppercase.compare("BOTH") == 0)
    side = btk::Both;
  else
    mexErrMsgTxt("Unknown side.");
    
  std::string label, description;
  if(nrhs >= 3)
  {
    strlen = (mxGetM(prhs[2]) * mxGetN(prhs[2]) * sizeof(mxChar)) + 1;
    s = (char*)mxMalloc(strlen);
    mxGetString(prhs[2], s, strlen);
    label = std::string(s);
    mxFree(s);
  }
  if(nrhs >= 4)
  {
    strlen = (mxGetM(prhs[3]) * mxGetN(prhs[3]) * sizeof(mxChar)) + 1;
    s = (char*)mxMalloc(strlen);
    mxGetString(prhs[3], s, strlen);
    description = std::string(s);
    mxFree(s);
  }
  
  btk::Model::Pointer model = btk::Model::New();
  model->Init(configurator, side, label, description);
  plhs[0] = btk_MOH_create_handle(model);
};

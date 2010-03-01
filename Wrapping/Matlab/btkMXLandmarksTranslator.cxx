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

#include "btkMex.h"
#if defined(SCI_MEX)
  #include "btkMXLandmarksTranslator.h"
#endif

btk::LandmarksTranslator::Pointer btkMXGetLandmarksTranslator(const mxArray* lt)
{
  if (!mxIsCell(lt))
    mexErrMsgTxt("Landmark translations must be set in cells.");
  btk::LandmarksTranslator::Pointer tr = btk::LandmarksTranslator::New();
  int len = mxGetM(lt) * mxGetN(lt);
  for (int i = 0 ; i < len ; ++i)
  {
    mxArray* cell = mxGetCell(lt, i);
    if (!mxIsCell(cell) || (mxGetM(cell) * mxGetN(cell) != 2))
      mexErrMsgTxt("Each Landmark translation must be contained in a cell with only 2 strings.");
    mxArray* l = mxGetCell(cell, 0);
    mxArray* t = mxGetCell(cell, 1);
    if (!mxIsChar(l) || mxIsEmpty(l))
      mexErrMsgTxt("The landmark label must be set by a non-empty string.");
    if (!mxIsChar(t) || mxIsEmpty(t))
      mexErrMsgTxt("The translation label must be set by a non-empty string.");
    int strlen = (mxGetM(l) * mxGetN(l) * sizeof(mxChar)) + 1;
    char* landmark = (char*)mxMalloc(strlen);
    mxGetString(l, landmark, strlen);
    strlen = (mxGetM(t) * mxGetN(t) * sizeof(mxChar)) + 1;
    char* translation = (char*)mxMalloc(strlen);
    mxGetString(t, translation, strlen);
    tr->SetLandmark(landmark,translation);
    mxFree(landmark);
    mxFree(translation);
  }
  return tr;
};


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

#ifndef __btkMEXSpecializedPointValues_h
#define __btkMEXSpecializedPointValues_h

#include "btkMEXObjectHandle.h"

#include <btkSpecializedPointsExtractor.h>
#include <btkAcquisition.h>

/**
 * Extracts data values of all points with the type @a t.
 *
 * @ingroup BTKWrappingMatlab
 */
void btkMEXGetSpecializedPointValues(btk::Point::Type t, int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  if (nrhs != 1)
    mexErrMsgTxt("One input required.");
  if (nlhs > 1)
   mexErrMsgTxt("Too many output arguments.");

  // First output
  btk::Acquisition::Pointer acq = btk_MOH_get_object<btk::Acquisition>(prhs[0]);

  btk::SpecializedPointsExtractor::Pointer specialPointExtractor = btk::SpecializedPointsExtractor::New();
  specialPointExtractor->SetInput(acq);
  specialPointExtractor->SetPointType(t);
  btk::PointCollection::Pointer points = specialPointExtractor->GetOutput();
  specialPointExtractor->Update();

  int numberOfFrames = acq->GetPointFrameNumber();
  int numberOfPoints = points->GetItemNumber();
  if (numberOfPoints == 0)
  {
    plhs[0] = mxCreateDoubleMatrix(0, 0, mxREAL);
    return;
  }

  plhs[0] = mxCreateDoubleMatrix(numberOfFrames, numberOfPoints * 3, mxREAL);
  double* values = mxGetPr(plhs[0]);

  int i = 0;
  int j = numberOfFrames * 3;
  double* v = 0;
  btk::PointCollection::ConstIterator it = points->Begin();
  while(i < (numberOfFrames * numberOfPoints * 3))
  {
    if (j >= (numberOfFrames * 3))
    {
      v = (*it)->GetValues().data();
      ++it;
      j = 0;
    }
    values[i] = v[j];
    ++i; ++j;
  }
};

/**
 * Replaces data values of all points with the type @a t.
 *
 * @ingroup BTKWrappingMatlab
 */
void btkMEXSetSpecializedPointValues(btk::Point::Type t, int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  if (nrhs != 2)
    mexErrMsgTxt("Two input required.");
  if (nlhs > 0)
   mexErrMsgTxt("Too many output arguments.");

  if (!mxIsNumeric(prhs[1]))
    mexErrMsgTxt("The second input must be a matrix of real values corresponding to the same dimensions than extracted points' coordinates.");

  btk::Acquisition::Pointer acq = btk_MOH_get_object<btk::Acquisition>(prhs[0]);

  btk::SpecializedPointsExtractor::Pointer specialPointExtractor = btk::SpecializedPointsExtractor::New();
  specialPointExtractor->SetInput(acq);
  specialPointExtractor->SetPointType(t);
  btk::PointCollection::Pointer points = specialPointExtractor->GetOutput();
  specialPointExtractor->Update();

  int numberOfFrames = acq->GetPointFrameNumber();
  int numberOfPoints = points->GetItemNumber();

  if (mxGetNumberOfElements(prhs[1]) != (numberOfFrames * numberOfPoints * 3))
    mexErrMsgTxt("The second input doesn't have the same size than the number of extracted points' coordinates.");
    
  if (numberOfPoints == 0)
    return;
    
  double* values = mxGetPr(prhs[1]);

  int i = 0;
  int j = numberOfFrames * 3;
  double* v = 0;
  btk::PointCollection::Iterator it = points->Begin();
  while (i < (numberOfFrames * numberOfPoints * 3))
  {
    if (j >= (numberOfFrames * 3))
    {
      v = (*it)->GetValues().data();
      ++it;
      j = 0;
    }
    v[j] = values[i];
    ++i; ++j;
  }
};

#endif // __btkMEXSpecializedPointValues_h

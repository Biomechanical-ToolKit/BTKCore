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

#include "btkMEXObjectHandle.h"
#include "btkForcePlatformsExtractor.h"
#include "btkMXMeasure.h"
#include "btkMEXOutputRedirection.h"

#include <btkAcquisition.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  if (nrhs!=1)
    mexErrMsgTxt("One input required.");
  if (nlhs > 2)
    mexErrMsgTxt("Too many output arguments.");

  // std::cerr redirection to the mexWarnMsgTxt function.
  btk::MEXCerrToWarnMsgTxt cerrRedir = btk::MEXCerrToWarnMsgTxt("btk:GetForcePlatforms");

  // First output
  btk::Acquisition::Pointer acq = btk_MOH_get_object<btk::Acquisition>(prhs[0]);
  btk::ForcePlatformsExtractor::Pointer fpExtractor = btk::ForcePlatformsExtractor::New();
  fpExtractor->SetInput(acq);
  btk::ForcePlatformCollection::Pointer fps = fpExtractor->GetOutput();
  fpExtractor->Update();

  const char* fieldnames[] = {"channels", "corners", "origin", "type"};
  int numberOfFields =  sizeof(fieldnames) / sizeof(char*);
  int numberOfForcePlates = fps->GetItemNumber();
  
  plhs[0] = mxCreateStructMatrix(numberOfForcePlates, 1, numberOfFields, fieldnames);
  std::vector<char**> channelFieldnames = std::vector<char**>(numberOfForcePlates);
  btk::ForcePlatformCollection::ConstIterator itPlatform = fps->Begin();
  for (int i = 0 ; i < numberOfForcePlates ; ++i)
  {
    btk::ForcePlatform::Pointer fp = *itPlatform;
    if (fp.get() != 0)
    {
      // channels field
      mxSetFieldByNumber(plhs[0], i, 0, btkMXCreateMeasuresStructure<btk::Analog>(fp->GetChannels(), &(channelFieldnames[i])));
      // corners field
      mxArray* corners = mxCreateDoubleMatrix(3, 4, mxREAL);
      memcpy(mxGetPr(corners), fp->GetCorners().data(), mxGetNumberOfElements(corners) * sizeof(double));
      mxSetFieldByNumber(plhs[0], i, 1, corners);
      // origin field
      mxArray* origin = mxCreateDoubleMatrix(3, 1, mxREAL);
      memcpy(mxGetPr(origin), fp->GetOrigin().data(), mxGetNumberOfElements(origin) * sizeof(double));
      mxSetFieldByNumber(plhs[0], i, 2, origin);
      // type field
      mxArray* type = mxCreateDoubleMatrix(1, 1, mxREAL);
      *mxGetPr(type) = static_cast<double>(fp->GetType());
      mxSetFieldByNumber(plhs[0], i, 3, type);
    }
    else
    {
      std::string warnText = "Force platform #" + btk::ToString(i) + " is empty. Send an email to BTK developers to inform them.";
      mexWarnMsgTxt(warnText.c_str());
    }
    ++itPlatform;
  }
 
  // Second output (optionnal)
  if (nlhs < 2)
  {
    itPlatform = fps->Begin();
    for(int i = 0 ; i < numberOfForcePlates ; ++i)
    {
      btk::ForcePlatform::Pointer fp = *itPlatform;
      int numberOfChannels = fp->GetChannelNumber();
      for (int j = 0 ; j < numberOfChannels ; ++j)
        delete[] channelFieldnames[i][j];
      delete[] channelFieldnames[i];
    }
  }
  else
  {
    const char* infos[] = {"frequency", "units", "cal_matrix"};
    int numberOfInfos =  sizeof(infos) / sizeof(char*);
    plhs[1] = mxCreateStructMatrix(numberOfForcePlates, 1, numberOfInfos, infos);
    itPlatform = fps->Begin();
    for (int i = 0 ; i < numberOfForcePlates ; ++i)
    {
      btk::ForcePlatform::Pointer fp = *itPlatform;
      if (fp.get() != 0)
      {
        // frequency field
        mxArray* frequency = mxCreateDoubleMatrix(1, 1, mxREAL);
        *mxGetPr(frequency) = acq->GetAnalogFrequency();
        mxSetFieldByNumber(plhs[1], i, 0, frequency);
        // units field
        int numberOfChannels = fp->GetChannelNumber();
        mxArray* unitsStruct = mxCreateStructMatrix(1, 1, numberOfChannels, (const char**)channelFieldnames[i]);
        btk::ForcePlatform::ConstIterator itChannel = fp->Begin();
        for (int j = 0 ; j < numberOfChannels ; ++j)
        {
          mxSetFieldByNumber(unitsStruct, 0, j, mxCreateString((*itChannel)->GetUnit().c_str()));
          delete[] channelFieldnames[i][j];
          ++itChannel;
        }
        delete[] channelFieldnames[i];
        mxSetFieldByNumber(plhs[1], i, 1, unitsStruct);
        // cal_matrix field
        mxArray* calMatrix = mxCreateDoubleMatrix(fp->GetCalMatrix().rows(), fp->GetCalMatrix().cols(), mxREAL);
        memcpy(mxGetPr(calMatrix), fp->GetCalMatrix().data(), mxGetNumberOfElements(calMatrix) * sizeof(double));
        mxSetFieldByNumber(plhs[1], i, 2, calMatrix);
      }
    }
  }
};

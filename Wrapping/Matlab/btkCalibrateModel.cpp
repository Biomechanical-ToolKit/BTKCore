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
#include "btkMXLandmarksTranslator.h"
#include "btkMEXStreambufToPrintf.h"
#include "btkMEXStreambufToWarnMsgTxt.h"

#include <btkModel.h>
#include <btkAcquisition.h>
#include <btkSubject.h>
#include <btkLandmarksTranslator.h>
#include <btkModelCalibrationFilter.h>
#include <btkStaticCalibrationProcedure.h>

#include <algorithm>
#include <cctype>

btk::Subject::Pointer ExtractSubjectInfo(const mxArray* s);

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  if(nrhs < 3)
    mexErrMsgTxt("Two input arguments required.");
  if (nlhs > 1)
   mexErrMsgTxt("Too many output arguments.");

  btk::Model::Pointer input = btk_MOH_get_object<btk::Model>(prhs[0]);
  btk::Subject::Pointer subject;
  btk::LandmarksTranslator::Pointer tr;
  btk::CalibrationProcedure::Pointer calibProc;
  
  if(nrhs >= 4)
  {
    if (mxIsStruct(prhs[3]))
      subject = ExtractSubjectInfo(prhs[3]);
    else if (mxIsCell(prhs[3]))
      tr = btkMXGetLandmarksTranslator(prhs[3]);
    else
      mexErrMsgTxt("Unknown 3rd input argument.");
  }
  
  if(nrhs >= 5)
  {
    if (mxIsStruct(prhs[4]) && !subject.get())
      subject = ExtractSubjectInfo(prhs[3]);
    else if (mxIsCell(prhs[4]) && !tr.get())
      tr = btkMXGetLandmarksTranslator(prhs[4]);
    else
      mexErrMsgTxt("Unknown 4th input argument.");
  }
  
  if (!mxIsChar(prhs[2]) || mxIsEmpty(prhs[2]))
    mexErrMsgTxt("The calibration procedure must be set by a non-empty string.");
  int strlen = (mxGetM(prhs[2]) * mxGetN(prhs[2]) * sizeof(mxChar)) + 1;
  char* calpro = (char*)mxMalloc(strlen);
  mxGetString(prhs[2], calpro, strlen);
  std::string uppercase = std::string(calpro);
  mxFree(calpro);
  std::transform(uppercase.begin(), uppercase.end(), uppercase.begin(), toupper);
  if (uppercase.compare("STATIC") == 0)
    calibProc = btk::StaticCalibrationProcedure::New();
  else
    mexErrMsgTxt("Unknown calibration procedure.");
    
  btk::Acquisition::Pointer acq = btk_MOH_get_object<btk::Acquisition>(prhs[1]);
  btk::ModelCalibrationFilter::Pointer cal = btk::ModelCalibrationFilter::New();
  cal->SetAcquisition(acq);
  cal->SetModel(input);
  cal->SetSubject(subject);
  cal->SetLandmarksTranslator(tr);
  cal->SetCalibrationProcedure(calibProc);
  
  // std::cout redirection to the mexPrintf function.
  btk::MEXStreambufToPrintf matlabStandardOutput;
  std::streambuf* stdStandardOutput = std::cout.rdbuf(&matlabStandardOutput);
  // std::cerr redirection to the mexWarnMsgTxt function.
  btk::MEXStreambufToWarnMsgTxt matlabErrorOutput;
  std::streambuf* stdErrorOutput = std::cerr.rdbuf(&matlabErrorOutput);
  
  cal->Update();
  
  // Back to the previous output buffer.
  matlabErrorOutput.requestNewLine();
  std::cout.rdbuf(stdStandardOutput);
  std::cerr.rdbuf(stdErrorOutput);
  
  plhs[0] = btk_MOH_create_handle(cal->GetOutput());
};

btk::Subject::Pointer ExtractSubjectInfo(const mxArray* s)
{
  btk::Subject::Pointer subject = btk::Subject::New();
  if (mxGetNumberOfElements(s) != 1)
    mexErrMsgTxt("Only accepted subject's structure with one element.");
  mxArray* temp;
  // Check for the field  'sex'
  if ((temp = mxGetField(s, 0, "sex")) != NULL)
  {
    if (!mxIsChar(temp) || mxIsEmpty(temp))
      mexErrMsgTxt("The field 'sex' must be set by a non-empty string.");
    int strlen = (mxGetM(temp) * mxGetN(temp) * sizeof(mxChar)) + 1;
    char* sex = (char*)mxMalloc(strlen);
    mxGetString(temp, sex, strlen);
    std::string uppercase = std::string(sex);
    mxFree(sex);
    std::transform(uppercase.begin(), uppercase.end(), uppercase.begin(), toupper);
    if ((uppercase.compare("M") == 0) || (uppercase.compare("MALE") == 0))
      subject->SetSex(btk::Subject::Male);
    else if ((uppercase.compare("F") == 0) || (uppercase.compare("FEMALE") == 0))
      subject->SetSex(btk::Subject::Female);
    else
      mexErrMsgTxt("The field 'sex' must be set by one of the following string: 'M', 'Male', 'F', 'Female'");
  }
  else
    mexErrMsgTxt("The structure doesn't contain the field 'sex'.");
  // Check for the field  'height'
  if ((temp = mxGetField(s, 0, "height")) != NULL)
  {
    if (!mxIsNumeric(temp) || mxIsEmpty(temp) || mxIsComplex(temp) || (mxGetNumberOfElements(temp) != 1))
      mexErrMsgTxt("The subject's height must be set by a single real value.");
    subject->SetHeight(mxGetScalar(temp));
  }
  else
    mexErrMsgTxt("The structure doesn't contain the field 'height'.");
  // Check for the field  'weight'
  if ((temp = mxGetField(s, 0, "weight")) != NULL)
  {
    if (!mxIsNumeric(temp) || mxIsEmpty(temp) || mxIsComplex(temp) || (mxGetNumberOfElements(temp) != 1))
      mexErrMsgTxt("The subject's weight must be set by a single real value.");
    subject->SetWeight(mxGetScalar(temp));
  }
  else
    mexErrMsgTxt("The structure doesn't contain the field 'weight'.");
  return subject;
};
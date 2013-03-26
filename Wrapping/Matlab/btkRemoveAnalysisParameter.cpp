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
#include "btkMXAnalysis.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  if(nrhs < 2)
    mexErrMsgTxt("No enough inputs.");
  if (nlhs > 2)
    mexErrMsgTxt("Too many output arguments.");

  if (!mxIsChar(prhs[1]) && mxIsEmpty(prhs[1]) && (!mxIsNumeric(prhs[1]) || mxIsComplex(prhs[1]) || (mxGetNumberOfElements(prhs[1]) != 1)))
    mexErrMsgTxt("Parameter's context must be a non-empty string or an integer.");

  if (mxIsChar(prhs[1]) && (nrhs < 3))
     mexErrMsgTxt("No enough inputs to remove parameter from its context and name.");
  if (mxIsChar(prhs[1]) && (!mxIsChar(prhs[2]) && mxIsEmpty(prhs[2]) && (!mxIsNumeric(prhs[2]) || mxIsComplex(prhs[2]) || (mxGetNumberOfElements(prhs[2]) != 1))))
    mexErrMsgTxt("Parameter's context must be a non-empty string.");

  btk::Acquisition::Pointer acq = btk_MOH_get_object<btk::Acquisition>(prhs[0]);
  btk::MetaData::Iterator itAnalysis = acq->GetMetaData()->FindChild("ANALYSIS");
  if (itAnalysis != acq->GetMetaData()->End())
  {
    std::vector<btk::MetaData::Iterator> its = std::vector<btk::MetaData::Iterator>(5);

    btk::MetaData::Iterator itUsed = (*itAnalysis)->FindChild("USED");
    its[0] = (*itAnalysis)->FindChild("NAMES");
    its[1] = (*itAnalysis)->FindChild("DESCRIPTIONS");
    its[2] = (*itAnalysis)->FindChild("SUBJECTS");
    its[3] = (*itAnalysis)->FindChild("CONTEXTS");
    its[4] = (*itAnalysis)->FindChild("UNITS");
    btk::MetaData::Iterator itValues = (*itAnalysis)->FindChild("VALUES");
    
    if ((itUsed == (*itAnalysis)->End())
        || (its[0] == (*itAnalysis)->End())
        || (its[1] == (*itAnalysis)->End())
        || (its[2] == (*itAnalysis)->End())
        || (its[3] == (*itAnalysis)->End())
        || (its[4] == (*itAnalysis)->End())
        || (itValues == (*itAnalysis)->End()))
      mexErrMsgTxt("Corrupted analysis. Some metadata are missing.");
      
    if (!(*itUsed)->HasInfo()
       || !(*its[0])->HasInfo()
       || !(*its[1])->HasInfo()
       || !(*its[2])->HasInfo()
       || !(*its[3])->HasInfo()
       || !(*its[4])->HasInfo()
       || !(*itValues)->HasInfo())
      mexErrMsgTxt("Corrupted analysis. Some metadata informations are missing.");
    std::vector<int16_t> valUsed = (*itUsed)->GetInfo()->ToInt16();
    if (!valUsed.empty())
    {
      int16_t used = valUsed[0];
      if (used != 0)
      {
        int idx = -1;
        if (mxIsChar(prhs[1]))
        {
          std::vector<std::string> valNames;
          (*its[0])->GetInfo()->ToString(valNames);
          std::vector<std::string> valContexts;
          (*its[3])->GetInfo()->ToString(valContexts);
          // paremeter's context
          size_t strlen_ = (mxGetM(prhs[1]) * mxGetN(prhs[1]) * sizeof(mxChar)) + 1;
          char* context = (char*)mxMalloc(strlen_);
          mxGetString(prhs[1], context, strlen_);
          std::string strContext = std::string(context);
          mxFree(context);
          // parameter's name
          strlen_ = (mxGetM(prhs[2]) * mxGetN(prhs[2]) * sizeof(mxChar)) + 1;
          char* name = (char*)mxMalloc(strlen_);
          mxGetString(prhs[2], name, strlen_);
          std::string strName = std::string(name);
          mxFree(name);
          for (size_t i = 0 ; i < valNames.size() ; ++i)
          {
            btkTrimString(&(valNames[i]));
            btkTrimString(&(valContexts[i]));
            if ((valNames[i].compare(strName) == 0) && (valContexts[i].compare(strContext) == 0))
            {
              idx = (int)i;
              break;
            }
          }
        }
        else
          idx = static_cast<int>(mxGetScalar(prhs[1])) - 1;
        if ((idx >= 0) && (idx < used))
        {
          (*itUsed)->GetInfo()->SetValues(static_cast<int16_t>(used - 1));
          // Char
          std::vector<std::string> dataStr;
          for (int i = 0 ; i < 5 ; ++i)
          {
            (*its[i])->GetInfo()->ToString(dataStr);
            std::vector<std::string>::iterator it = dataStr.begin();
            std::advance(it, idx);
            if (it != dataStr.end())
            {
              dataStr.erase(it);
              (*its[i])->GetInfo()->SetValues(dataStr);
            }
          }
          // Real
          std::vector<float> dataFlt;
          (*itValues)->GetInfo()->ToFloat(dataFlt);
          std::vector<float>::iterator it = dataFlt.begin();
          std::advance(it, idx);
          if (it != dataFlt.end())
          {
            dataFlt.erase(it);
            (*itValues)->GetInfo()->SetValues(dataFlt);
          }
        }
      }
    }
  }

  // Return updated analysis
  btkMXCreateAnalysisStructure(acq, nlhs, plhs);
};


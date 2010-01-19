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

#include <btkMetaData.h> // First (Fix for MSVC LNK2019)

#include "btkMXObjectHandle.h"
#include "btkMXAnalysis.h"

// btkAppendAnalysisParameter(h, name, context, unit, value, subject, description)
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  if(nrhs < 5)
    mexErrMsgTxt("Minimum of five inputs required.");
  if (nlhs > 2)
   mexErrMsgTxt("Too many output arguments.");

  if (!mxIsChar(prhs[1]) || mxIsEmpty(prhs[1]))
    mexErrMsgTxt("Parameter's name must be a non-empty string.");
  if (!mxIsChar(prhs[2]) || mxIsEmpty(prhs[2]))
    mexErrMsgTxt("Parameter's context must be a non-empty string.");
  if (!mxIsChar(prhs[3]) || mxIsEmpty(prhs[3]))
    mexErrMsgTxt("Parameter's unit must be a non-empty string.");
  if ((!mxIsNumeric(prhs[4]) || mxIsEmpty(prhs[4])) || (mxGetNumberOfElements(prhs[4]) > 1))
    mexErrMsgTxt("Parameter's value must be single numerical value.");

  char* name = 0;
  char* context = 0;
  char* unit = 0;
  char* subject = 0;
  char* description = 0;
  double value = 0.0;
  
  if (nrhs >= 6)
  {
    if (!mxIsChar(prhs[5]))
      mexErrMsgTxt("Parameter's subject must be a string");
    int strlen = (mxGetM(prhs[5]) * mxGetN(prhs[5]) * sizeof(mxChar)) + 1;
    subject = (char*)mxMalloc(strlen);
    mxGetString(prhs[5], subject, strlen);
  }
  if (nrhs >= 7)
  {
    if (!mxIsChar(prhs[6]))
      mexErrMsgTxt("Parameter's description must be a string");
    int strlen = (mxGetM(prhs[6]) * mxGetN(prhs[6]) * sizeof(mxChar)) + 1;
    description = (char*)mxMalloc(strlen);
    mxGetString(prhs[6], description, strlen);
  }
  int strlen = (mxGetM(prhs[1]) * mxGetN(prhs[1]) * sizeof(mxChar)) + 1;
  name = (char*)mxMalloc(strlen);
  mxGetString(prhs[1], name, strlen);
  strlen = (mxGetM(prhs[2]) * mxGetN(prhs[2]) * sizeof(mxChar)) + 1;
  context = (char*)mxMalloc(strlen);
  mxGetString(prhs[2], context, strlen);
  strlen = (mxGetM(prhs[3]) * mxGetN(prhs[3]) * sizeof(mxChar)) + 1;
  unit = (char*)mxMalloc(strlen);
  mxGetString(prhs[3], unit, strlen);
  value = *mxGetPr(prhs[4]);
  
  btk::Acquisition::Pointer acq = btk_MOH_get_object<btk::Acquisition>(prhs[0]);  
  btk::MetaData::Pointer analysis;
  btk::MetaData::Iterator itAnalysis = acq->GetMetaData()->FindChild("ANALYSIS");
  if (itAnalysis == acq->GetMetaData()->End())
  {
    analysis = btk::MetaDataCreateChild(acq->GetMetaData(), "ANALYSIS");
    btkCreateAnalysis(analysis, name, context, unit, value, subject, description);
  }
  else
  {
    analysis = *itAnalysis;
    std::vector<btk::MetaData::Iterator> its = std::vector<btk::MetaData::Iterator>(5);

    btk::MetaData::Iterator itUsed = (*itAnalysis)->FindChild("USED");
    its[0] = (*itAnalysis)->FindChild("NAMES");
    its[1] = (*itAnalysis)->FindChild("DESCRIPTIONS");
    its[2] = (*itAnalysis)->FindChild("SUBJECTS");
    its[3] = (*itAnalysis)->FindChild("CONTEXTS");
    its[4] = (*itAnalysis)->FindChild("UNITS");
    btk::MetaData::Iterator itValues = (*itAnalysis)->FindChild("VALUES");
    
    // Check required metadata
    if ((itUsed == (*itAnalysis)->End())
        || (its[0] == (*itAnalysis)->End())
        || (its[1] == (*itAnalysis)->End())
        || (its[2] == (*itAnalysis)->End())
        || (its[3] == (*itAnalysis)->End())
        || (its[4] == (*itAnalysis)->End())
        || (itValues == (*itAnalysis)->End()))
    {
      mexWarnMsgTxt("Corrupted analysis. Some metadata are missing. Analysis cleared and set for the given parameter.");
      btkCreateAnalysis(analysis, name, context, unit, value, subject, description);
    }
    // Check their information
    else if (!(*itUsed)->HasInfo()
       || !(*its[0])->HasInfo()
       || !(*its[1])->HasInfo()
       || !(*its[2])->HasInfo()
       || !(*its[3])->HasInfo()
       || !(*its[4])->HasInfo()
       || !(*itValues)->HasInfo())
    {
      mexWarnMsgTxt("Corrupted analysis. Some metadata informations are missing. Analysis cleared and set for the given parameter.");
      btkCreateAnalysis(analysis, name, context, unit, value, subject, description);
    }
    // Check format
    else if ( ((*itUsed)->GetInfo()->GetFormat() != btk::MetaDataInfo::Integer)
       || ((*its[0])->GetInfo()->GetFormat() != btk::MetaDataInfo::Char)
       || ((*its[1])->GetInfo()->GetFormat() != btk::MetaDataInfo::Char)
       || ((*its[2])->GetInfo()->GetFormat() != btk::MetaDataInfo::Char)
       || ((*its[3])->GetInfo()->GetFormat() != btk::MetaDataInfo::Char)
       || ((*its[4])->GetInfo()->GetFormat() != btk::MetaDataInfo::Char)
       || ((*itValues)->GetInfo()->GetFormat() != btk::MetaDataInfo::Real) )
    {
      mexWarnMsgTxt("Corrupted analysis. Some metadata format are incorrect. Analysis cleared and set for the given parameter.");
      btkCreateAnalysis(analysis, name, context, unit, value, subject, description);
    }
    // Check size
    else if ( ((*its[0])->GetInfo()->GetDimensionsProduct(1) != (*its[1])->GetInfo()->GetDimensionsProduct(1))
       || ((*its[1])->GetInfo()->GetDimensionsProduct(1) != (*its[2])->GetInfo()->GetDimensionsProduct(1))
       || ((*its[2])->GetInfo()->GetDimensionsProduct(1) != (*its[3])->GetInfo()->GetDimensionsProduct(1))
       || ((*its[3])->GetInfo()->GetDimensionsProduct(1) != (*its[4])->GetInfo()->GetDimensionsProduct(1))
       || ((*its[4])->GetInfo()->GetDimensionsProduct(1) != (*itValues)->GetInfo()->GetDimensionsProduct()) )
    {
      mexWarnMsgTxt("Corrupted analysis. Some metadata informations have not the same size. Analysis cleared and set for the given parameter.");
      btkCreateAnalysis(analysis, name, context, unit, value, subject, description);
    }
    else
    {
      std::vector<int16_t> valUsed = (*itUsed)->GetInfo()->ToInt16();
      if (valUsed.empty())
        btkCreateAnalysis(analysis, name, context, unit, value, subject, description);
      else
      {
        int16_t used = valUsed[0];
        if (used == 0)
          btkCreateAnalysis(analysis, name, context, unit, value, subject, description);
        else
        { 
          std::vector<std::string> valNames;
          (*its[0])->GetInfo()->ToString(valNames);
          std::vector<std::string> valContexts;
          (*its[3])->GetInfo()->ToString(valContexts);
          // paremeter's context
          std::string strContext = std::string(context);
          strContext = strContext.erase(strContext.find_last_not_of(' ') + 1);
          strContext = strContext.erase(0, strContext.find_first_not_of(' '));
          // parameter's name
          std::string strName = std::string(name);
          strName = strName.erase(strName.find_last_not_of(' ') + 1);
          strName = strName.erase(0, strName.find_first_not_of(' '));
          int i = 0;
          for (i = 0 ; i < static_cast<int>(valNames.size()) ; ++i)
          {
            std::string strNameRef = valNames[i];
            strNameRef = strNameRef.erase(strNameRef.find_last_not_of(' ') + 1);
            strNameRef = strNameRef.erase(0, strNameRef.find_first_not_of(' '));
            std::string strContextRef = valContexts[i];
            strContextRef = strContextRef.erase(strContextRef.find_last_not_of(' ') + 1);
            strContextRef = strContextRef.erase(0, strContextRef.find_first_not_of(' '));
            if ((strNameRef.compare(strName) == 0) && (strContextRef.compare(strContext) == 0))
              break;
          }
          if (i >= static_cast<int>(valNames.size()))
          {
            (*itUsed)->GetInfo()->SetValues(static_cast<int16_t>(used + 1));
            std::vector<std::string> dataStr;
            // name
            (*its[0])->GetInfo()->ToString(dataStr);
            std::vector<std::string>::iterator it = dataStr.begin();
            std::advance(it, i);
            dataStr.insert(it, strName);
            (*its[0])->GetInfo()->SetValues(dataStr);
            // description
            (*its[1])->GetInfo()->ToString(dataStr);
            it = dataStr.begin();
            std::advance(it, i);
            dataStr.insert(it, description ? std::string(description) : "");
            (*its[1])->GetInfo()->SetValues(dataStr);
            // subject
            (*its[2])->GetInfo()->ToString(dataStr);
            it = dataStr.begin();
            std::advance(it, i);
            dataStr.insert(it, subject ? std::string(subject) : "");
            (*its[2])->GetInfo()->SetValues(dataStr);
            // context
            (*its[3])->GetInfo()->ToString(dataStr);
            it = dataStr.begin();
            std::advance(it, i);
            dataStr.insert(it, strContext);
            (*its[3])->GetInfo()->SetValues(dataStr);
            // unit
            (*its[4])->GetInfo()->ToString(dataStr);
            it = dataStr.begin();
            std::advance(it, i);
            dataStr.insert(it, std::string(unit));
            (*its[4])->GetInfo()->SetValues(dataStr);
            // value
            std::vector<float> dataFlt;
            (*itValues)->GetInfo()->ToFloat(dataFlt);
            std::vector<float>::iterator it2 = dataFlt.begin();
            std::advance(it2, i);
            dataFlt.insert(it2, static_cast<float>(value));
            (*itValues)->GetInfo()->SetValues(dataFlt);
          }
          else
          {
            // name
            (*its[0])->GetInfo()->SetValue(i, strName);
            // description
            (*its[1])->GetInfo()->SetValue(i, description ? std::string(description) : "");
            // subject
            (*its[2])->GetInfo()->SetValue(i, subject ? std::string(subject) : "");
            // context
            (*its[3])->GetInfo()->SetValue(i, strContext);
            // unit
            (*its[4])->GetInfo()->SetValue(i, std::string(unit));
            // value
            (*itValues)->GetInfo()->SetValue(i, static_cast<float>(value));
          }
        }
      } 
    }
  }
  // Return updated analysis
  btkMXCreateAnalysisStructure(acq, nlhs, plhs);
  
  mxFree(name);
  mxFree(context);
  mxFree(unit);
  mxFree(subject);
  mxFree(description);
};
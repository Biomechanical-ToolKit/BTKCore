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
  #include "btkMXEvent.h"
#endif
#include "btkMXObjectHandle.h"
#include "btkASCIIConverter.h"

#include <btkConvert.h>

void btkMXCreateEventsStructure(btk::Acquisition::Pointer acq, int nlhs, mxArray *plhs[])
{
 btk::EventCollection::Pointer events = acq->GetEvents();
 int inc = 0;
 bool onlyOneSubject = true;
 size_t numberOfEvents = 0;

 std::vector< std::vector<double> > times_sorted;
 char** fieldnames = 0;
 char** fieldnames_sorted = 0;
 std::vector<std::string> subjects_sorted;

 if (!events->IsEmpty())
 {
   std::vector<std::string> labels;
   std::vector< std::vector<double> > times;
   std::vector<std::string> subjects;

   btk::EventCollection::ConstIterator it = events->Begin();
   std::string subject = (*it)->GetSubject();
   ++it;
   while (it != events->End())
   {
     if (subject.compare((*it)->GetSubject()) != 0)
     {
       onlyOneSubject = false;
       break;
     }
     ++it;
   }

   for(btk::EventCollection::ConstIterator it = events->Begin() ; it != events->End() ; ++it)
   {
     std::string label = "";
     if (!onlyOneSubject)
     {
       if (!((*it)->GetSubject().empty()))
         label = (*it)->GetSubject() + "_";
     }
     if (!((*it)->GetContext().empty()))
       label += (*it)->GetContext() + "_" + (*it)->GetLabel();
     else
       label += (*it)->GetLabel();
     bool labelAddition = true;

     for(size_t i = 0 ; i < labels.size() ; ++i)
     {
       if (labels[i].compare(label) == 0)
       {
         times[i].push_back((*it)->GetTime());
         labelAddition = false;
         break;
       }
     }
     if (labelAddition)
     {
       labels.push_back(label);
       times.resize(times.size() + 1);
       times.back().push_back((*it)->GetTime());
       subjects.push_back((*it)->GetSubject());
     }
   }

   numberOfEvents = labels.size();
   fieldnames = new char*[numberOfEvents];
   for (size_t i = 0 ; i < labels.size() ; ++i)
   {
     std::string convertedLabel = std::string(labels[i].length(), '_');
     // Check bad characters
     for(size_t j = 0 ; j < convertedLabel.length() ; ++j)
       convertedLabel[j] = btk::ASCIIConverter[labels[i][j]];
     char c = convertedLabel[0];
     // Check first character
     if ((c == btk::ASCIIConverter[0x00]) // _
         || (c == btk::ASCIIConverter[0x30]) // 0
         || (c == btk::ASCIIConverter[0x31]) // 1
         || (c == btk::ASCIIConverter[0x32]) // 2
         || (c == btk::ASCIIConverter[0x33]) // 3
         || (c == btk::ASCIIConverter[0x34]) // 4
         || (c == btk::ASCIIConverter[0x35]) // 5
         || (c == btk::ASCIIConverter[0x36]) // 6
         || (c == btk::ASCIIConverter[0x37]) // 7
         || (c == btk::ASCIIConverter[0x38]) // 8
         || (c == btk::ASCIIConverter[0x39])) // 9
       convertedLabel.insert(convertedLabel.begin(), 'C');
     fieldnames[i] = new char[convertedLabel.length() + 1];
     strcpy(fieldnames[i], convertedLabel.c_str());
   }
   // sort event's times.
   for (size_t i = 0 ; i < times.size() ; ++i)
   {
     for (size_t j = 0 ; j < times[i].size() - 1 ; ++j)
     {
       if (times[i][j] > times[i][j + 1])
       {
         double t = times[i][j];
         times[i][j] = times[i][j + 1];
         times[i][j + 1] = t;
       }
       for (size_t k = j ; k > 0 ; --k)
       {
         if (times[i][k] < times[i][k - 1])
         {
           double t = times[i][k];
           times[i][k] = times[i][k - 1];
           times[i][k - 1] = t;
         }
         else
           break;
       }
     }
   }
   // sort event depending the first time.
   std::vector<int> indexes = std::vector<int>(times.size(),0);
   std::vector<double> firstTimes = std::vector<double>(times.size(),0);
   for (size_t i = 0 ; i < times.size() ; ++i)
   {
     indexes[i] = (int)i;
     firstTimes[i] = times[i][0];
   }
   for (size_t i = 0 ; i < times.size() - 1 ; ++i)
   {
     if (firstTimes[i] > firstTimes[i+1])
     {
       int idx = indexes[i];
       indexes[i] = indexes[i+1];
       indexes[i+1] = idx;
       double t = firstTimes[i];
       firstTimes[i] = firstTimes[i+1];
       firstTimes[i + 1] = t;
     }
     for (size_t k = i ; k > 0 ; --k)
     {
       if (firstTimes[k] < firstTimes[k - 1])
       {
         int idx = indexes[k];
         indexes[k] = indexes[k-1];
         indexes[k-1] = idx;
         double t = firstTimes[k];
         firstTimes[k] = firstTimes[k-1];
         firstTimes[k - 1] = t;
       }
       else
         break;
     }
   }

   // times, fieldnames and subjects sorted
   times_sorted = std::vector< std::vector<double> >(numberOfEvents);
   fieldnames_sorted = new char*[numberOfEvents];
   subjects_sorted = std::vector<std::string>(numberOfEvents);
   for (size_t i = 0 ; i < indexes.size() ; ++i)
   {
     times_sorted[i] = times[indexes[i]];
     fieldnames_sorted[i] = fieldnames[indexes[i]];
     subjects_sorted[i] = subjects[indexes[i]];
   }
 }
 plhs[0] = mxCreateStructMatrix(1, 1, static_cast<int>(numberOfEvents), (const char**)fieldnames_sorted);

 for(int i = 0 ; i < static_cast<int>(numberOfEvents) ; ++i)
 {
   mxArray* value = mxCreateDoubleMatrix(1, times_sorted[i].size(), mxREAL);
   double* v = mxGetPr(value);
   for(size_t j = 0 ; j < times_sorted[i].size() ; ++j)
     v[j] = times_sorted[i][j]; 
   mxSetFieldByNumber(plhs[0], 0, i, value);
 }

 // Second output (optionnal)
 if (nlhs != 2)
 {
   for (size_t i = 0 ; i < numberOfEvents ; ++i)
     delete[] fieldnames[i];
   delete[] fieldnames;
   delete[] fieldnames_sorted;
 }
 else
 {
   const char* info[] = {"subjects", "units"};
   int numberOfFields =  sizeof(info) / sizeof(char*);

   plhs[1] = mxCreateStructMatrix(1, 1, numberOfFields, info);
   mxArray* subjectsStruct = mxCreateStructMatrix(1, 1, static_cast<int>(numberOfEvents), (const char**)fieldnames_sorted);

   int inc = 0;
   for (int i = 0 ; i < static_cast<int>(numberOfEvents) ; ++i)
   {
     mxSetFieldByNumber(subjectsStruct, 0, i, mxCreateString(subjects_sorted[i].c_str()));
     delete[] fieldnames[inc];
     ++inc;
   }

   const char* ALL[] = {"ALLEVENTS"};
   mxArray* unitsStruct = mxCreateStructMatrix(1, 1, 1, ALL);
   mxSetFieldByNumber(unitsStruct, 0, 0, mxCreateString("s"));

   mxSetFieldByNumber(plhs[1], 0, 0, subjectsStruct);
   mxSetFieldByNumber(plhs[1], 0, 1, unitsStruct);

   delete[] fieldnames;
   delete[] fieldnames_sorted;
 }
};
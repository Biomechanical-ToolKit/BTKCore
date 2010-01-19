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

#ifndef __btkMXMeasure_h
#define __btkMXMeasure_h

#include "btkASCIIConverter.h"

#include <btkCollection.h>
#include <btkConvert.h>

#include "btkMex.h"

/**
 * Adapt the label of the measures stored in @a m and create a Matlab 
 * structure containing theses measure. Each fielname of this structure
 * corresponds to the adapted label.
 *
 * This adaption is required as Matlab only accepts characters [a-zA-Z] 
 * for the first character of a variable's name. Moreover, if the first 
 * character is included in [_0-9],then the character 'C' is inserted at 
 * the beginning. Finally, for fieldnames which have the same value, a 
 * unique number is added to distinct them.
 *
 * @ingroup BTKWrappingMatlab
 */
template <class T>
inline mxArray* btkMXCreateMeasuresStructure(typename btk::Collection<T>::Pointer m, char*** fieldnamesPtr)
{
  typedef typename btk::Collection<T> itemCollection;
  int numberOfMeasures = m->GetItemNumber();
  (*fieldnamesPtr) = new char*[numberOfMeasures];
  char** fieldnames = *fieldnamesPtr;
  int inc = 0;
  for(typename itemCollection::ConstIterator it = m->Begin() ; it != m->End() ; ++it)
  {
    std::string originalLabel = (*it)->GetLabel();
    std::string convertedLabel = std::string(originalLabel.length(), '_');
    // Check bad characters
    for(int i = 0 ; i < static_cast<int>(originalLabel.length()) ; ++i)
      convertedLabel[i] = btk::ASCIIConverter[originalLabel[i]];
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
    // Check label's redundancy
    int id = 0;
    std::string doubleLabel = convertedLabel;
    for (int i = 0 ; i < inc ; ++i)
    {
      if (doubleLabel.compare(fieldnames[i]) == 0)
        doubleLabel = convertedLabel + btk::ToString(++id);
    }
    if (id != 0)
      convertedLabel = doubleLabel;
    fieldnames[inc] = new char[convertedLabel.length() + 1];
    strcpy(fieldnames[inc], convertedLabel.c_str());
    inc++;
  }
  mxArray* out = mxCreateStructMatrix(1, 1, numberOfMeasures, (const char**)fieldnames);
  if (numberOfMeasures > 0)
  {
    inc = 0;
    typename T::ConstPointer firstItem = m->GetItem(0);
    for(typename itemCollection::ConstIterator it = m->Begin() ; it != m->End() ; ++it)
    {
      mxArray* measure = mxCreateDoubleMatrix(firstItem->GetFrameNumber(), firstItem->GetValues().cols(), mxREAL);
      memcpy(mxGetPr(measure), (*it)->GetValues().data(), mxGetNumberOfElements(measure) * sizeof(double));
      mxSetFieldByNumber(out, 0, inc, measure);
      ++inc;
    }
  }
  return out;
};

#endif // __btkMXMeasure_h

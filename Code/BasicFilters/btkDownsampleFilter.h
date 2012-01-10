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

#ifndef __btkDownsampleFilter_h
#define __btkDownsampleFilter_h

#include "btkProcessObject.h"
#include "btkCollection.h"
#include "btkMacro.h"
#include "btkWrenchCollection.h"

namespace btk
{
  template <class T>
  class DownsampleFilter : public ProcessObject
  {
  public:
    typedef SharedPtr<DownsampleFilter> Pointer;
    typedef SharedPtr<const DownsampleFilter> ConstPointer;
       
    typedef typename T::Pointer ItemPointer;
    typedef typename T::ConstPointer ItemConstPointer;    
    
    static Pointer New() {return Pointer(new DownsampleFilter());};
    
    virtual ~DownsampleFilter() {};
    
    ItemPointer GetInput() {return this->GetInput(0);};
    void SetInput(ItemPointer input) {this->SetNthInput(0, input);};
    ItemPointer GetOutput() {return this->GetOutput(0);};
    
    int GetUpDownRatio() const {return this->m_Ratio;};
    void SetUpDownRatio(int ratio);
    
  protected:
    DownsampleFilter();
    
    ItemPointer GetInput(int idx) {return static_pointer_cast<T>(this->GetNthInput(idx));};
    ItemPointer GetOutput(int idx) {return static_pointer_cast<T>(this->GetNthOutput(idx));};
    virtual DataObject::Pointer MakeOutput(int idx);
    virtual void GenerateData();
    
  private:
    DownsampleFilter(const DownsampleFilter& ); // Not implemented.
    DownsampleFilter& operator=(const DownsampleFilter& ); // Not implemented.
    
    int m_Ratio;
  };
  
  /**
   * @class DownsampleFilter btkDownsampleFilter.h
   * @brief Downsample dataExtracts a frame from a collection of btk::Measure objects
   * @tparam T Must be a class inheriting of btk::DataObject
   *
   * @ingroup BTKBasicFilters
   */
  
  /**
   * @typedef DownsampleFilter<T>::Pointer
   * Smart pointer associated with a DownsampleFilter object.
   */
  
  /**
   * @typedef DownsampleFilter<T>::ConstPointer
   * Smart pointer associated with a const DownsampleFilter object.
   */
  
  /**
   * @typedef DownsampleFilter<T>::ItemPointer
   * Smart pointer associated with a T object.
   */
  
  /**
   * @typedef DownsampleFilter<T>::ItemConstPointer
   * Smart const pointer associated with a T object.
   */
  
  /**
   * @fn template <class T> static Pointer DownsampleFilter<T>::New();
   * Creates a smart pointer associated with a DownsampleFilter<T> object.
   */
  
  /**
   * @fn template <class T> virtual DownsampleFilter<T>::~DownsampleFilter()
   * Empty destructor.
   */
  
  /**
   * @fn template <class T> CollectionPointer DownsampleFilter<T>::GetInput()
   * Gets the input registered with this process.
   */
  
  /**
   * @fn template <class T> void DownsampleFilter<T>::SetInput(ItemPointer input)
   * Sets the input required with this process.
   */
  
  /**
   * @fn template <class T> ItemPointer DownsampleFilter<T>::GetOutput()
   * Gets the output created with this process.
   */
  
  /**
   * @fn template <class T> int DownsampleFilter<T>::GetUpDownRatio() const
   * Gets the index of the frame to extract.
   */
  
  /**
   * Sets the index of the frame to extract.
   */
  template <class T>
  void DownsampleFilter<T>::SetUpDownRatio(int ratio)
  {
    if (this->m_Ratio == ratio)
      return;
    this->m_Ratio = ratio; 
    this->Modified();
  };
  
  /**
   * Constructor. Sets the number of inputs and outputs to 1.
   */
  template <class T>
  DownsampleFilter<T>::DownsampleFilter()
  : ProcessObject()
  {
    this->SetInputNumber(1);
    this->SetOutputNumber(1);
    this->m_Ratio = 1;
  };
  
  /**
   * @fn template <class T> CollectionPointer DownsampleFilter<T>::GetInput(int idx)
   * Returns the input at the index @a idx.
   */
  
  /**
   * @fn template <class T> ItemPointer DownsampleFilter<T>::GetOutput(int idx)
   * Returns the output at the index @a idx.
   */
  
  /**
   * Creates a T:Pointer object and return it as a DataObject::Pointer.
   */
  template <class T>
  DataObject::Pointer DownsampleFilter<T>::MakeOutput(int /* idx */)
  {
    return T::New();
  };
  
  /**
   * Generates the outputs' data.
   */
  template <class T>
  void DownsampleFilter<T>::GenerateData()
  {
    DownsampleData(this->m_Ratio, this->GetInput(), this->GetOutput());
  };

  /**
   * Generic method to downsample data. Does nothing.
   */
  template <class T>
  inline void DownsampleData(int ratio, SharedPtr<T> input, SharedPtr<T> output)
  {
    btkErrorMacro("Generic method. Please specialize it.");
  };

  /**
   * Specialized version to downsample wrench.
   */
  template <>
  inline void DownsampleData<Wrench>(int ratio, Wrench::Pointer input, Wrench::Pointer output)
  {
    int inFrameNumber = input->GetPosition()->GetFrameNumber();
    int outFrameNumber = inFrameNumber / ratio;
    output->GetPosition()->SetFrameNumber(outFrameNumber);
    output->GetForce()->SetFrameNumber(outFrameNumber);
    output->GetMoment()->SetFrameNumber(outFrameNumber);
    double* inPosition = input->GetPosition()->GetValues().data();
    double* inForce = input->GetForce()->GetValues().data();
    double* inMoment = input->GetMoment()->GetValues().data();
    double* outPosition = output->GetPosition()->GetValues().data();
    double* outForce = output->GetForce()->GetValues().data();
    double* outMoment = output->GetMoment()->GetValues().data();
    for (int i = 0 ; i < outFrameNumber ; ++i)
    {
      outPosition[i] = inPosition[i * ratio];
      outPosition[i + outFrameNumber] = inPosition[i * ratio + inFrameNumber];
      outPosition[i + 2 * outFrameNumber] = inPosition[i * ratio + 2 * inFrameNumber];
      outForce[i] = inForce[i * ratio];
      outForce[i + outFrameNumber] = inForce[i * ratio + inFrameNumber];
      outForce[i + 2 * outFrameNumber] = inForce[i * ratio + 2 * inFrameNumber];

      outMoment[i] = inMoment[i * ratio];
      outMoment[i + outFrameNumber] = inMoment[i * ratio + inFrameNumber];
      outMoment[i + 2 * outFrameNumber] = inMoment[i * ratio + 2 * inFrameNumber];
    }
  };

  /**
   * Specialized version to downsample collection of wrenches.
   */
  template <>
  inline void DownsampleData<WrenchCollection>(int ratio, WrenchCollection::Pointer input, WrenchCollection::Pointer output)
  {
    WrenchCollection::Iterator itIn = input->Begin();
    output->SetItemNumber(input->GetItemNumber());
    WrenchCollection::Iterator itOut = output->Begin();
    
    while (itIn != input->End())
    {
      if (!itOut->get())
        *itOut = Wrench::New();
      DownsampleData<Wrench>(ratio, *itIn, *itOut);
      ++itIn;
      ++itOut;
    }
  };

};

#endif // __btkDownsampleFilter_h

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

#ifndef __btkMeasureFrameExtractor_h
#define __btkMeasureFrameExtractor_h

#include "btkProcessObject.h"
#include "btkCollection.h"
#include "btkMacro.h"

namespace btk
{
  // TODO: Enhances the ouput... Maybe a btk::Frame<T> which could iterator throw
  //       the Collection<T>. It means, this extractor process will be a factory
  //       to create btk::Frame<T> objects.
  template <class T>
  class MeasureFrameExtractor : public ProcessObject
  {
  public:
    typedef SharedPtr<MeasureFrameExtractor> Pointer;
    typedef SharedPtr<const MeasureFrameExtractor> ConstPointer;
    
    typedef typename Collection<T>::Pointer CollectionPointer;
    typedef typename Collection<T>::ConstPointer CollectionConstPointer;    
    typedef typename T::Pointer ItemPointer;
    typedef typename T::ConstPointer ItemConstPointer;    
    
    static Pointer New() {return Pointer(new MeasureFrameExtractor());};
    
    virtual ~MeasureFrameExtractor() {};
    
    CollectionPointer GetInput() {return this->GetInput(0);};
    void SetInput(CollectionPointer input) {this->SetNthInput(0, input);};
    ItemPointer GetOutput() {return this->GetOutput(0);};
    
    int GetIndex() const {return this->m_Index;};
    void SetIndex(int idx);
    
  protected:
    MeasureFrameExtractor();
    
    CollectionPointer GetInput(int idx) {return static_pointer_cast< Collection<T> >(this->GetNthInput(idx));};  
    ItemPointer GetOutput(int idx) {return static_pointer_cast<T>(this->GetNthOutput(idx));};
    virtual DataObject::Pointer MakeOutput(int idx);
    virtual void GenerateData();
    
  private:
    MeasureFrameExtractor(const MeasureFrameExtractor& ); // Not implemented.
    MeasureFrameExtractor& operator=(const MeasureFrameExtractor& ); // Not implemented.
    
    int m_Index;
  };
  
  /**
   * @class MeasureFrameExtractor btkMeasureFrameExtractor.h
   * @brief Extracts a frame from a collection of btk::Measure objects
   * @tparam T Must be a class inheriting of btk::Measure
   *
   * @ingroup BTKBasicFilters
   */
  
  /**
   * @typedef MeasureFrameExtractor<T>::Pointer
   * Smart pointer associated with a MeasureFrameExtractor object.
   */
  
  /**
   * @typedef MeasureFrameExtractor<T>::ConstPointer
   * Smart pointer associated with a const MeasureFrameExtractor object.
   */
  
  /**
   * @typedef MeasureFrameExtractor<T>::CollectionPointer
   * Smart pointer associated with a Collection<T> object.
   */
  
  /**
   * @typedef MeasureFrameExtractor<T>::CollectionConstPointer
   * Smart pointer associated with a const Collection<T> object.
   */
  
  /**
   * @typedef MeasureFrameExtractor<T>::ItemPointer
   * Smart pointer associated with a T object.
   */
  
  /**
   * @typedef MeasureFrameExtractor<T>::ItemConstPointer
   * Smart const pointer associated with a T object.
   */
  
  /**
   * @fn template <class T> static Pointer MeasureFrameExtractor<T>::New();
   * Creates a smart pointer associated with a MeasureFrameExtractor<T> object.
   */
  
  /**
   * @fn template <class T> virtual MeasureFrameExtractor<T>::~MeasureFrameExtractor()
   * Empty destructor.
   */
  
  /**
   * @fn template <class T> CollectionPointer MeasureFrameExtractor<T>::GetInput()
   * Gets the input registered with this process.
   */
  
  /**
   * @fn template <class T> void MeasureFrameExtractor<T>::SetInput(CollectionPointer input)
   * Sets the input required with this process.
   */
  
  /**
   * @fn template <class T> ItemPointer MeasureFrameExtractor<T>::GetOutput()
   * Gets the output created with this process.
   */
  
  /**
   * @fn template <class T> int MeasureFrameExtractor<T>::GetIndex() const
   * Gets the index of the frame to extract.
   */
  
  /**
   * Sets the index of the frame to extract.
   */
  template <class T>
  void MeasureFrameExtractor<T>::SetIndex(int idx)
  {
    if (this->m_Index == idx)
      return;
    this->m_Index = idx; 
    this->Modified();
  };
  
  /**
   * Constructor. Sets the number of inputs and outputs to 1.
   */
  template <class T>
  MeasureFrameExtractor<T>::MeasureFrameExtractor()
  : ProcessObject()
  {
    this->SetInputNumber(1);
    this->SetOutputNumber(1);
    this->m_Index = 0;
  };
  
  /**
   * @fn template <class T> CollectionPointer MeasureFrameExtractor<T>::GetInput(int idx)
   * Returns the input at the index @a idx.
   */
  
  /**
   * @fn template <class T> ItemPointer MeasureFrameExtractor<T>::GetOutput(int idx)
   * Returns the output at the index @a idx.
   */
  
  /**
   * Creates a T:Pointer object and return it as a DataObject::Pointer.
   */
  template <class T>
  DataObject::Pointer MeasureFrameExtractor<T>::MakeOutput(int /* idx */)
  {
    return T::New();
  };
  
  /**
   * Generates the outputs' data.
   */
  template <class T>
  void MeasureFrameExtractor<T>::GenerateData()
  {
    ItemPointer output = this->GetOutput();
    CollectionPointer input = this->GetInput();
    if (input.get() == 0 || input->IsEmpty())
    {
      btkErrorMacro("Invalid input. The output does not contains any values");
      return;
    }
    output->SetFrameNumber(input->GetItemNumber());
    typename Collection<T>::ConstIterator it = input->Begin();
    int index = this->m_Index;
    int frameNumber = (*it)->GetFrameNumber();
    if (this->m_Index > frameNumber)
    {
      btkErrorMacro("The specified index is out of range. The index extracted corresponds to the index modulo the frame number.");
      index = this->m_Index % frameNumber;
    }
    int inc = 0;
    for (typename Collection<T>::ConstIterator it = input->Begin() ; it != input->End() ; ++it)
    {
      output->GetValues().row(inc) = (*it)->GetValues().row(index);
      ++inc;
    }
  };  
};

#endif // __btkMeasureFrameExtractor_h

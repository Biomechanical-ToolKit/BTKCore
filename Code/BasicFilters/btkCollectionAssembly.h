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

#ifndef __btkCollectionAssembly_h
#define __btkCollectionAssembly_h

#include "btkProcessObject.h"
#include "btkCollection.h"

namespace btk
{
  template <class T>
  class CollectionAssembly : public ProcessObject
  {
  public:
    typedef btkSharedPtr<CollectionAssembly> Pointer;
    typedef btkSharedPtr<const CollectionAssembly> ConstPointer;
    
    typedef typename Collection<T>::Pointer CollectionPointer;
    typedef typename Collection<T>::ConstPointer CollectionConstPointer;    
    
    static Pointer New() {return Pointer(new CollectionAssembly());};
    
    virtual ~CollectionAssembly() {};
    
    CollectionPointer GetInput(int idx) {return static_pointer_cast< Collection<T> >(this->GetNthInput(idx));};  
    void SetInput(int idx, CollectionPointer input) {this->SetNthInput(idx, input);};
    CollectionPointer GetOutput() {return this->GetOutput(0);};
    
  protected:
    CollectionAssembly();
    
    CollectionPointer GetOutput(int idx) {return static_pointer_cast< Collection<T> >(this->GetNthOutput(idx));};
    virtual DataObject::Pointer MakeOutput(int idx);
    virtual void GenerateData();
    
  private:
    CollectionAssembly(const CollectionAssembly& ); // Not implemented.
    CollectionAssembly& operator=(const CollectionAssembly& ); // Not implemented.
  };
  
  /**
   * @class CollectionAssembly btkCollectionAssembly.h
   * @brief Group together a set of collections of the same type.
   * @tparam T Must be a class inheriting of btk::Collection
   *
   * @ingroup BTKBasicFilters
   */
  
  /**
   * @typedef CollectionAssembly<T>::Pointer
   * Smart pointer associated with a CollectionAssembly object.
   */
  
  /**
   * @typedef CollectionAssembly<T>::ConstPointer
   * Smart pointer associated with a const CollectionAssembly object.
   */
  
  /**
   * @typedef CollectionAssembly<T>::CollectionPointer
   * Smart pointer associated with a Collection<T> object.
   */
  
  /**
   * @typedef CollectionAssembly<T>::CollectionConstPointer
   * Smart pointer associated with a const Collection<T> object.
   */
  
  /**
   * @fn template <class T> static Pointer CollectionAssembly<T>::New();
   * Creates a smart pointer associated with a CollectionAssembly<T> object.
   */
  
  /**
   * @fn template <class T> virtual CollectionAssembly<T>::~CollectionAssembly()
   * Empty destructor.
   */
  
  /**
   * @fn template <class T> CollectionPointer CollectionAssembly<T>::GetInput(int idx)
   * Gets the input registered with this process at the index @a idx.
   */
  
  /**
   * @fn template <class T> void CollectionAssembly<T>::SetInput(int idx, CollectionPointer input)
   * Sets the input required with this process at the index @a idx.
   */
  
  /**
   * @fn template <class T> CollectionPointer CollectionAssembly<T>::GetOutput()
   * Gets the output created with this process.
   */
  
  /**
   * Constructor. Sets the number of inputs to 2 (extendable) and outputs to 1.
   */
  template <class T>
  CollectionAssembly<T>::CollectionAssembly()
  : ProcessObject()
  {
    this->SetInputNumber(2);
    this->SetOutputNumber(1);
  };
  
  /**
   * @fn template <class T> ItemPointer CollectionAssembly<T>::GetOutput(int idx)
   * Returns the output at the index @a idx.
   */
  
  /**
   * Creates a T:Pointer object and return it as a DataObject::Pointer.
   */
  template <class T>
  DataObject::Pointer CollectionAssembly<T>::MakeOutput(int /* idx */)
  {
    return Collection<T>::New();
  };
  
  /**
   * Generates the outputs' data.
   */
  template <class T>
  void CollectionAssembly<T>::GenerateData()
  {
    CollectionPointer output = this->GetOutput();
    output->Clear();
    for (int idx = 0 ; idx < this->GetInputNumber() ; ++idx)
    {
      CollectionPointer input = this->GetInput(idx);
      if (input.get() == 0 || input->IsEmpty())
        continue;
      for (typename Collection<T>::ConstIterator it = input->Begin() ; it != input->End() ; ++it)
        output->InsertItem(*it);
    }
  };  
};

#endif // __btkCollectionAssembly_h

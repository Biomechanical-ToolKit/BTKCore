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

#ifndef __btkVTKPolyDataAlgorithmAdapter_h
#define __btkVTKPolyDataAlgorithmAdapter_h

#include "btkVTKDataObjectAdapter.h"

#include <vtkPolyDataAlgorithm.h>
#include <vtkInformation.h>

namespace btk
{
  template <class T>
  class VTKPolyDataAlgorithmAdapter : public vtkPolyDataAlgorithm 
  {
  public:
    typedef typename T::Pointer ItemPointer;
    
    vtkTypeRevisionMacro(VTKPolyDataAlgorithmAdapter<T>,vtkPolyDataAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent);
    
    ItemPointer GetInput() {return std::tr1::static_pointer_cast<T>(static_cast<VTKDataObjectAdapter*>(this->GetInput(0))->GetBTKDataObject());};
    void SetInput(ItemPointer item);
    
  protected:
    VTKPolyDataAlgorithmAdapter();
    virtual ~VTKPolyDataAlgorithmAdapter() {};
    
    int FillInputPortInformation(int port, vtkInformation* info);
    
    using vtkPolyDataAlgorithm::GetInput;
    using vtkPolyDataAlgorithm::SetInput;
    
  private:
    VTKPolyDataAlgorithmAdapter(const VTKPolyDataAlgorithmAdapter<T>& );  // Not implemented.
    VTKPolyDataAlgorithmAdapter<T>& operator=(const VTKPolyDataAlgorithmAdapter<T>& );  // Not implemented.
  };
  
  template <class T>
  vtkCxxRevisionMacro(VTKPolyDataAlgorithmAdapter<T>, "$Revision: 0.1 $");
  
  template <class T>
  void VTKPolyDataAlgorithmAdapter<T>::PrintSelf(ostream& os, vtkIndent indent)
  {
    this->Superclass::PrintSelf(os,indent);
  }
  
  template <class T>
  void VTKPolyDataAlgorithmAdapter<T>::SetInput(ItemPointer item)
  {
    VTKDataObjectAdapter* input = VTKDataObjectAdapter::New();
    input->SetBTKDataObject(item);
    this->SetInput(0, input);
    input->Delete();
  };
  
  template <class T>
  VTKPolyDataAlgorithmAdapter<T>::VTKPolyDataAlgorithmAdapter()
  : vtkPolyDataAlgorithm()
  {};
  
  template <class T>
  int VTKPolyDataAlgorithmAdapter<T>::FillInputPortInformation(int /*port*/, 
                                                               vtkInformation* info)
  {
    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "VTKDataObjectAdapter");
    return 1;
  }
};
#endif // __btkVTKPolyDataAlgorithmAdapter_h

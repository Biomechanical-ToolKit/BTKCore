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

#ifndef __btkAcquisitionFileIORegister_h
#define __btkAcquisitionFileIORegister_h

#include "btkAcquisitionFileIOHandle.h"

namespace btk
{
  template <class T>
  class AcquisitionFileIORegister : public AcquisitionFileIOHandle
  {
  public:
    typedef btkSharedPtr<AcquisitionFileIORegister> Pointer;
    static Pointer New() {return Pointer(new AcquisitionFileIORegister());};
    
    virtual bool HasReadOperation() const {return T::HasReadOperation();};
    virtual bool HasWriteOperation() const {return T::HasWriteOperation();};
    
  private:
    template <class U>
    class PrivateFunctor : public AcquisitionFileIOHandle::Functor
    {
    public:
      static AcquisitionFileIOHandle::Functor::Pointer New() {return AcquisitionFileIOHandle::Functor::Pointer(new PrivateFunctor<U>());};
      virtual AcquisitionFileIO::Pointer GetFileIO() const {return U::New();};
    private:
      PrivateFunctor() : AcquisitionFileIOHandle::Functor() {};
      PrivateFunctor(const PrivateFunctor& ); // Not implemented.
      PrivateFunctor& operator=(const PrivateFunctor& ); // Not implemented.
    };
    
    AcquisitionFileIORegister() : AcquisitionFileIOHandle(PrivateFunctor<T>::New()) {};
    AcquisitionFileIORegister(const AcquisitionFileIORegister& ); // Not implemented.
    AcquisitionFileIORegister& operator=(const AcquisitionFileIORegister& ); // Not implemented.
  };
  
 /**
  * @class AcquisitionFileIORegister 
  * @brief Interface to add dynamicaly an acquisition file I/O in the factory.
  * @tparam T Classname of the acquisition file I/O.
  *
  * To add dynamicaly a file format in the factory (to read or write an acquisition), you need to use the command 
  * btk::AcquisitionFileIOFactory::AddFileIO() coupled with the template method btk::AcquisitionFileIORegister::New() where the template is the name of the class which will read/write the acquistion (e.g. TestFileIO).
  * @code {.cpp}
  * // Only one line is required to add a new acquisition file IO.  
  * btk::AcquisitionFileIOFactory::AddFileIO(btk::AcquisitionFileIORegister<TestFileIO>::New());
  * @endcode
  *
  * @ingroup BTKIO
  */

 /**
  * @typedef AcquisitionFileIORegister::Pointer
  * Smart pointer associated with an AcquisitionFileIORegister object.
  */
  
 /**
  * @fn AcquisitionFileIORegister::Pointer AcquisitionFileIORegister<T>::New()
  * Returns a AcquisitionFileIOHandle::Pointer object and not a AcquisitionFileIORegister::Pointer object!.
  * This method is a convenient method  
  */
  
 /**
  * @fn AcquisitionFileIO::Pointer AcquisitionFileIORegister::GetFileIO() const
  * Creates an acquisition File IO based on the given classname.
  */

};

#endif
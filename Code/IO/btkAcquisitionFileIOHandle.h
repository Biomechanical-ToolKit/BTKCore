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

#ifndef __btkAcquisitionFileIOHandle_h
#define __btkAcquisitionFileIOHandle_h

#include "btkAcquisitionFileIO.h"

namespace btk
{
  class AcquisitionFileIOHandle
  {
  public:
    class Functor
    {
    public:
      typedef btkSharedPtr<AcquisitionFileIOHandle::Functor> Pointer;
      virtual ~Functor() {};
      virtual AcquisitionFileIO::Pointer GetFileIO() const = 0;
    };
    
    typedef btkSharedPtr<AcquisitionFileIOHandle> Pointer;
    virtual ~AcquisitionFileIOHandle() {};
    AcquisitionFileIO::Pointer GetFileIO() const {return this->mp_Functor->GetFileIO();};
    const AcquisitionFileIOHandle::Functor::Pointer GetFunctor() const {return this->mp_Functor;};
    virtual bool HasReadOperation() const = 0;
    virtual bool HasWriteOperation() const = 0;
  protected:
    AcquisitionFileIOHandle(AcquisitionFileIOHandle::Functor::Pointer f) : mp_Functor(f) {};
    AcquisitionFileIOHandle::Functor::Pointer mp_Functor;
  private:
    AcquisitionFileIOHandle(const AcquisitionFileIOHandle& ); // Not implemented.
    AcquisitionFileIOHandle& operator=(const AcquisitionFileIOHandle& ); // Not implemented.
  };
  
  /**
   * @class AcquisitionFileIOHandle 
   * @brief Store properties of a acquisition file IO.
   *
   * This class is used to add a file format dynamically in the file factory, by using the method btk::btkAcquisitionFileIOFactory::AddFileIO().
   * Instead of using it directly, it is advise to use the class btk::AcquisitionFileIORegister. 
   */
  /**
   * @var AcquisitionFileIOHandle::mp_Functor
   * Functor pointing to a static method able to create an object inherting from the class btk::AcquisitionFileIO
   */

  /**
   * @typedef AcquisitionFileIOHandle::Pointer
   * Smart pointer associated with an AcquisitionFileIOHandle object.
   */

  /**
   * @fn const AcquisitionFileIOHandle::Functor::Pointer AcquisitionFileIOHandle::GetFunctor() const
   * Returns the functor used with this handle.
   */
  
  /**
   * @fn virtual bool AcquisitionFileIOHandle::HasReadOperation() const
   * Check if this acquisition file IO can read file.
   */
  
  /**
   * @fn virtual bool AcquisitionFileIOHandle::HasWriteOperation() const
   * Check if this acquisition file IO can write file.
   */
  
  /**
   * @fn AcquisitionFileIOHandle::AcquisitionFileIOHandle(AcquisitionFileIOHandle::Functor::Pointer f)
   * Constructor
   */
  
  /**
   * @fn AcquisitionFileIOHandle::~AcquisitionFileIOHandle()
   * Destructor
   */
  
  /**
   * @fn AcquisitionFileIO::Pointer AcquisitionFileIOHandle::GetFileIO() const
   * Creates an acquisition File IO based on the given classname.
   */

  // ----------------------------------------------------------------------- //

  /**
   * @class AcquisitionFileIOHandle::Functor 
   * @brief Abstract class used to store a pointer to the method New() of a class inheriting from btk::AcquisitionFileIO.
   *
   * This class would not be used directly by a developer. If a developer want to add a file format dynamically to the factory, it cas use the method btk::btkAcquisitionFileIOFactory::AddFileIO().
   */
   
  /**
   * @typedef AcquisitionFileIOHandle::Functor::Pointer
   * Smart pointer associated with an AcquisitionFileIOHandle::Functor object.
   */
 
  /**
   * @fn virtual AcquisitionFileIOHandle::Functor::~Functor
   * Destructor. Do nothing.
   */
   
  /**
   * @fn virtual AcquisitionFileIO::Pointer AcquisitionFileIOHandle::Functor::GetFileIO() const = 0
   * Create an acquisition File IO based on the given pointer to the New() method.
   */
}

#endif // __btkAcquisitionFileIOHandle_h
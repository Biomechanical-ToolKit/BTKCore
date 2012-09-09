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
      typedef SharedPtr<AcquisitionFileIOHandle::Functor> Pointer;
      virtual ~Functor() {};
      virtual AcquisitionFileIO::Pointer GetFileIO() const = 0;
    };
    
    typedef SharedPtr<AcquisitionFileIOHandle> Pointer;
    static Pointer New(AcquisitionFileIOHandle::Functor::Pointer f, bool r, bool w) {return Pointer(new AcquisitionFileIOHandle(f,r,w));};
    // ~AcquisitionFileIOHandle(); // Implicit
    AcquisitionFileIO::Pointer GetFileIO() {return this->mp_Functor->GetFileIO();};
    const AcquisitionFileIOHandle::Functor::Pointer GetFunctor() const {return this->mp_Functor;};
    bool HasReadOperation() const {return this->m_ReadOp;};
    bool HasWriteOperation() const {return this->m_WriteOp;};
  protected:
    AcquisitionFileIOHandle(AcquisitionFileIOHandle::Functor::Pointer f, bool r, bool w) : mp_Functor(f) {this->m_ReadOp = r; this->m_WriteOp = w;};
    AcquisitionFileIOHandle::Functor::Pointer mp_Functor;
    bool m_ReadOp;
    bool m_WriteOp;
  private:
    AcquisitionFileIOHandle(const AcquisitionFileIOHandle& ); // Not implemented.
    AcquisitionFileIOHandle& operator=(const AcquisitionFileIOHandle& ); // Not implemented.
  };
  
  /**
   * @class AcquisitionFileIOHandle 
   * @brief 
   */
  /**
   * @var AcquisitionFileIOFactory::m_ReadOp
   * 
   */
  /**
   * @var AcquisitionFileIOFactory::m_WriteOp
   * 
   */
   
  /**
   * @class AcquisitionFileIOHandle::Functor 
   * @brief Abstract class 
   */
  
}

#endif // __btkAcquisitionFileIOHandle_h
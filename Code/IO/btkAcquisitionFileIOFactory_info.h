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

#ifndef __btkAcquisitionFileIOFactory_info_h
#define __btkAcquisitionFileIOFactory_info_h

#include "btkAcquisitionFileIO.h"

#define BTK_IO_REGISTER_ACQUISITION_FILE_RDRW(classname) \
  BTK_IO_REGISTER_ACQUISITION_FILE(classname, true, true)

#define BTK_IO_REGISTER_ACQUISITION_FILE_RONLY(classname) \
  BTK_IO_REGISTER_ACQUISITION_FILE(classname, true, false)

#define BTK_IO_REGISTER_ACQUISITION_FILE_WONLY(classname) \
  BTK_IO_REGISTER_ACQUISITION_FILE(classname, false, true)

#define BTK_IO_REGISTER_ACQUISITION_FILE(classname, read, write) \
  this->list.push_back(AcquisitionFileIOInfo::New(btk::AcquisitionFileIOConverter<classname>::New(), read, write));
   
#define BTK_IO_ACQUISITON_FILE_FACTORY_INIT \
   AcquisitionFileIOInfos::AcquisitionFileIOInfos() \
   : list()

namespace btk
{
  class AcquisitionFileIOFuncPtr
  {
  public:
    typedef SharedPtr<AcquisitionFileIOFuncPtr> Pointer;
    virtual ~AcquisitionFileIOFuncPtr() {};
    virtual AcquisitionFileIO::Pointer GetFileIO() const = 0;
  };
  
  class AcquisitionFileIOInfo
  {
  public:
    typedef SharedPtr<AcquisitionFileIOInfo> Pointer;
    static Pointer New(AcquisitionFileIOFuncPtr::Pointer f, bool r, bool w) {return Pointer(new AcquisitionFileIOInfo(f,r,w));};
    // ~AcquisitionFileIOInfo(); // Implicit
    AcquisitionFileIO::Pointer GetFileIO() {return this->mp_Functor->GetFileIO();};
    const AcquisitionFileIOFuncPtr::Pointer GetFunctor() const {return this->mp_Functor;};
    bool HasReadOperation() const {return this->m_ReadOp;};
    bool HasWriteOperation() const {return this->m_WriteOp;};
  protected:
    AcquisitionFileIOInfo(AcquisitionFileIOFuncPtr::Pointer f, bool r, bool w) : mp_Functor(f) {this->m_ReadOp = r; this->m_WriteOp = w;};
    AcquisitionFileIOFuncPtr::Pointer mp_Functor;
    bool m_ReadOp;
    bool m_WriteOp;
  private:
    AcquisitionFileIOInfo(const AcquisitionFileIOInfo& ); // Not implemented.
    AcquisitionFileIOInfo& operator=(const AcquisitionFileIOInfo& ); // Not implemented.
  };
  
  struct AcquisitionFileIOInfos
  {
    typedef std::list<AcquisitionFileIOInfo::Pointer>::iterator Iterator;
    typedef std::list<AcquisitionFileIOInfo::Pointer>::const_iterator ConstIterator;
    inline AcquisitionFileIOInfos();
    std::list<AcquisitionFileIOInfo::Pointer> list;
  };
  
  template <class T>
  class AcquisitionFileIOConverter : public AcquisitionFileIOFuncPtr
  {
  public:
    typedef SharedPtr<AcquisitionFileIOConverter> Pointer;
    static Pointer New() {return Pointer(new AcquisitionFileIOConverter<T>());};
    virtual AcquisitionFileIO::Pointer GetFileIO() const {return T::New();};
  private:
    AcquisitionFileIOConverter() : AcquisitionFileIOFuncPtr() {};
    AcquisitionFileIOConverter(const AcquisitionFileIOConverter& ); // Not implemented.
    AcquisitionFileIOConverter& operator=(const AcquisitionFileIOConverter& ); // Not implemented.
  };
}

#endif // __btkAcquisitionFileIOFactory_info_h
/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2014, Arnaud Barré
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

#include "btkAcquisitionFileIOFactory.h"
#include "btkAcquisitionFileIOFactory_p.h"

namespace btk
{
  /**
   * @class AcquisitionFileIOFactory btkAcquisitionFileIOFactory.h
   * @brief Manage all the acquisition file IOs and detect if a file is readable or writable.
   *
   * The goal of this factory is to return an AcquisitionFileIO object based on the given filename.
   * For that, you need to call the method AcquisitionFileIOFactory::CreateAcquisitionIO. In this method,
   * each AcquisitionFileIO object is tested to see if the file is supported or by file format managed.
   *
   * If you want to add a new file format to this factory, you can use the method AcquisitionFileIOFactory::AddFileIO.
   * Or you if you work directly into the source-code of BTK, you can register direclty the new file format using the file btkAcquisitionFileIOFactory_registration.cpp
   *
   * @sa AcquisitionFileIORegister
   * 
   * @ingroup BTKIO
   */
  
  /**
   * @enum AcquisitionFileIOFactory::OpenMode
   * Enums used to specify the open mode.
   */
  /**
   * @var AcquisitionFileIOFactory::OpenMode AcquisitionFileIOFactory::ReadMode
   * Enum value for the read mode
   */
  /**
   * @var AcquisitionFileIOFactory::OpenMode AcquisitionFileIOFactory::WriteMode
   * Enum value for the write mode
   */
  
  /**
   * Try to find the AcquisitionFileIO helper to read/write the file. This method has
   * to be modified each time a new AcquisitionFileIO is added in this library. The order
   * of the IO is important as the first AcquisitionFileIO which can read/write the file
   * is returned.
   */
  AcquisitionFileIO::Pointer AcquisitionFileIOFactory::CreateAcquisitionIO(const std::string& filename, OpenMode mode)
  {
    AcquisitionFileIO::Pointer io;
    if (mode == ReadMode)
    {
      for (AcquisitionFileIOHandles::ConstIterator it = AcquisitionFileIOFactory::GetInfoIOs()->list.begin() ; it != AcquisitionFileIOFactory::GetInfoIOs()->list.end() ; ++it)
      {
        if ((*it)->HasReadOperation() && (io = (*it)->GetFileIO())->CanReadFile(filename))
          return io;
      }
    }
    else
    {
      for (AcquisitionFileIOHandles::ConstIterator it = AcquisitionFileIOFactory::GetInfoIOs()->list.begin() ; it != AcquisitionFileIOFactory::GetInfoIOs()->list.end() ; ++it)
      {
        if ((*it)->HasWriteOperation() && (io = (*it)->GetFileIO())->CanWriteFile(filename))
          return io;
      }
    }
    return AcquisitionFileIO::Pointer();
  };
  
  /**
   * Append a file IO if it is not already added.
   *
   * Should not be used by developers. Internal purpose.
   */
  bool AcquisitionFileIOFactory::AddFileIO(AcquisitionFileIOHandle::Pointer infoIO)
  {
    for (AcquisitionFileIOHandles::ConstIterator it = AcquisitionFileIOFactory::GetInfoIOs()->list.begin() ; it != AcquisitionFileIOFactory::GetInfoIOs()->list.end() ; ++it)
    {
      if ((*it)->GetFunctor() == infoIO->GetFunctor())
        return false;
    }
    AcquisitionFileIOFactory::GetInfoIOs()->list.push_front(infoIO);
    return true;
  };
  
  /**
   * Remove a file IO if found.
   *
   * Should not be used by developers. Internal purpose.
   */
  bool AcquisitionFileIOFactory::RemoveFileIO(AcquisitionFileIOHandle::Pointer infoIO)
  {
    for (AcquisitionFileIOHandles::Iterator it = AcquisitionFileIOFactory::GetInfoIOs()->list.begin() ; it != AcquisitionFileIOFactory::GetInfoIOs()->list.end() ; ++it)
    {
      if ((*it)->GetFunctor() == infoIO->GetFunctor())
      {
        AcquisitionFileIOFactory::GetInfoIOs()->list.erase(it);
        return true;
      }
    }
    return false;
  };
  
  /**
   * Returns the list of the file extensions than the factory could read.
   *
   * Note: This list is directly created by the acquisition file IO registered with this factory.
   */
  AcquisitionFileIO::Extensions AcquisitionFileIOFactory::GetSupportedReadExtensions()
  {
    AcquisitionFileIO::Extensions exts;
    for (AcquisitionFileIOHandles::Iterator it = AcquisitionFileIOFactory::GetInfoIOs()->list.begin() ; it != AcquisitionFileIOFactory::GetInfoIOs()->list.end() ; ++it)
    {
      if ((*it)->HasReadOperation())
        exts.Append((*it)->GetFileIO()->GetSupportedExtensions());
    }
    return exts;
  };
  
  /**
   * Returns the list of the file extensions than the factory could write.
   *
   * Note: This list is directly created by the acquisition file IO registered with this factory.
   */
  AcquisitionFileIO::Extensions AcquisitionFileIOFactory::GetSupportedWrittenExtensions()
  {
    AcquisitionFileIO::Extensions exts;
    for (AcquisitionFileIOHandles::Iterator it = AcquisitionFileIOFactory::GetInfoIOs()->list.begin() ; it != AcquisitionFileIOFactory::GetInfoIOs()->list.end() ; ++it)
    {
      if ((*it)->HasWriteOperation())
        exts.Append((*it)->GetFileIO()->GetSupportedExtensions());
    }
    return exts;
  };
  
  /** 
   * @fn AcquisitionFileIOFactory::AcquisitionFileIOFactory()
   * Constructor.
   */
  
  /** 
   * @fn virtual AcquisitionFileIOFactory::~AcquisitionFileIOFactory()
   * Empty destructor.
   */
  
  AcquisitionFileIOHandles* AcquisitionFileIOFactory::GetInfoIOs()
  {
    static AcquisitionFileIOHandles infoIOs;
    return &infoIOs;
  };
};

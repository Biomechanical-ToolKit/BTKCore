/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2011, Arnaud Barré
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

// C3D File IO
#include "btkC3DFileIO.h"
// Motion Analysis Corp IOs
#include "btkANBFileIO.h"
#include "btkANCFileIO.h"
#include "btkCALForcePlateFileIO.h"
#include "btkTRBFileIO.h"
#include "btkTRCFileIO.h"
#include "btkXLSOrthoTrakFileIO.h"
// Elite IOs
#include "btkANGFileIO.h"
#include "btkEMxFileIO.h"
#include "btkMOMFileIO.h"
#include "btkRAxFileIO.h"
#include "btkRICFileIO.h"
#include "btkPWRFileIO.h"
#include "btkGRxFileIO.h"
// Others
#include "btkEMFFileIO.h"

namespace btk
{
  /**
   * @class AcquisitionFileIOFactory btkAcquisitionFileIOFactory.h
   * @brief This class only implements the CreateAcquisitionIO method.
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
      // C3D
      io = C3DFileIO::New();
      if (io->CanReadFile(filename)) return io;
      // Motion Analysis Inc
      io = TRBFileIO::New();
      if (io->CanReadFile(filename)) return io;
      io = TRCFileIO::New();
      if (io->CanReadFile(filename)) return io;
      io = ANBFileIO::New();
      if (io->CanReadFile(filename)) return io;
      io = ANCFileIO::New();
      if (io->CanReadFile(filename)) return io;
      io = CALForcePlateFileIO::New();
      if (io->CanReadFile(filename)) return io;
      io = XLSOrthoTrakFileIO::New();
      if (io->CanReadFile(filename)) return io;
      // Elite
      io = RAxFileIO::New();
      if (io->CanReadFile(filename)) return io;
      io = RICFileIO::New();
      if (io->CanReadFile(filename)) return io;
      io = MOMFileIO::New();
      if (io->CanReadFile(filename)) return io;
      io = ANGFileIO::New();
      if (io->CanReadFile(filename)) return io;
      io = PWRFileIO::New();
      if (io->CanReadFile(filename)) return io;
      io = GRxFileIO::New();
      if (io->CanReadFile(filename)) return io;
      io = EMxFileIO::New();
      if (io->CanReadFile(filename)) return io;
      // Others
      io = EMFFileIO::New();
      if (io->CanReadFile(filename)) return io;
    }
    else
    {
      // C3D
      io = C3DFileIO::New();
      if (io->CanWriteFile(filename)) return io;
      // Motion Analysis Inc
      io = TRCFileIO::New();
      if (io->CanWriteFile(filename)) return io;
      io = ANBFileIO::New();
      if (io->CanWriteFile(filename)) return io;
      io = ANCFileIO::New();
      if (io->CanWriteFile(filename)) return io;
    }
    return AcquisitionFileIO::Pointer();
  };
  
  /** 
   * @fn AcquisitionFileIOFactory::AcquisitionFileIOFactory()
   * Constructor.
   */
  
  /** 
   * @fn virtual AcquisitionFileIOFactory::~AcquisitionFileIOFactory()
   * Empty destructor.
   */
};

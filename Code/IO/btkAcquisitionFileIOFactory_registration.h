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

#ifndef __btkAcquisitionFileIOFactory_registration_h
#define __btkAcquisitionFileIOFactory_registration_h

#include "btkAcquisitionFileIOFactory_info.h"

// ----------------------------------------------------------------------------
//                  STEP 1: ADD ALL ACQUISITION FILES IO HERE
// ----------------------------------------------------------------------------

// C3D File IO
#include "btkC3DFileIO.h"
// Motion Analysis Corp IOs
#include "btkANBFileIO.h"
#include "btkANCFileIO.h"
#include "btkCALForcePlateFileIO.h"
#include "btkTRBFileIO.h"
#include "btkTRCFileIO.h"
#include "btkXLSOrthoTrakFileIO.h"
// BTS IO
#include "btkTDFFileIO.h"
// Delsys IO
#include "btkDelsysEMGFileIO.h"
// Elite IOs
#include "btkANGFileIO.h"
#include "btkEMxFileIO.h"
#include "btkGRxFileIO.h"
#include "btkMOMFileIO.h"
#include "btkPWRFileIO.h"
#include "btkRAxFileIO.h"
#include "btkRICFileIO.h"
// Others
#include "btkEMFFileIO.h"
#include "btkAMTIForcePlatformFileIO.h"

namespace btk
{
  BTK_IO_ACQUISITON_FILE_FACTORY_INIT
  {
    // ----------------------------------------------------------------------------
    //                 STEP 2: REGISTER THE ACQUISTION FILES IO HERE
    // ----------------------------------------------------------------------------

    BTK_IO_REGISTER_ACQUISITION_FILE_RDRW(C3DFileIO)
    
    BTK_IO_REGISTER_ACQUISITION_FILE_RDRW(ANBFileIO)
    BTK_IO_REGISTER_ACQUISITION_FILE_RDRW(ANCFileIO)
    BTK_IO_REGISTER_ACQUISITION_FILE_RDRW(CALForcePlateFileIO)
    BTK_IO_REGISTER_ACQUISITION_FILE_RONLY(TRBFileIO)
    BTK_IO_REGISTER_ACQUISITION_FILE_RDRW(TRCFileIO)
    BTK_IO_REGISTER_ACQUISITION_FILE_RONLY(XLSOrthoTrakFileIO)
    
    BTK_IO_REGISTER_ACQUISITION_FILE_RONLY(TDFFileIO)
    
    BTK_IO_REGISTER_ACQUISITION_FILE_RONLY(DelsysEMGFileIO) // MUST BE BEFORE btkEMxFileIO.cpp
    
    BTK_IO_REGISTER_ACQUISITION_FILE_RONLY(ANGFileIO)
    BTK_IO_REGISTER_ACQUISITION_FILE_RONLY(EMxFileIO)
    BTK_IO_REGISTER_ACQUISITION_FILE_RONLY(GRxFileIO)
    BTK_IO_REGISTER_ACQUISITION_FILE_RONLY(MOMFileIO)
    BTK_IO_REGISTER_ACQUISITION_FILE_RONLY(PWRFileIO)
    BTK_IO_REGISTER_ACQUISITION_FILE_RONLY(RAxFileIO)
    BTK_IO_REGISTER_ACQUISITION_FILE_RONLY(RICFileIO)
    
    BTK_IO_REGISTER_ACQUISITION_FILE_RONLY(EMFFileIO)
    BTK_IO_REGISTER_ACQUISITION_FILE_RONLY(AMTIForcePlatformFileIO)  
  };
};

#endif // __btkAcquisitionFileIOFactory_registration_h
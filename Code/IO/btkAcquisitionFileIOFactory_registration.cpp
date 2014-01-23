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

#ifndef __btkAcquisitionFileIOFactory_registration_h
#define __btkAcquisitionFileIOFactory_registration_h

#include "btkAcquisitionFileIOFactory_p.h"

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
// Codamotion
#include "btkMDFFileIO.h"
#include "btkXMOVEFileIO.h"
// BTS IO
#include "btkTDFFileIO.h"
// Delsys IO
#include "btkDelsysEMGFileIO.h"
#include "btkHPFFileIO.h"
// Elite IOs
#include "btkANGFileIO.h"
#include "btkEMxFileIO.h"
#include "btkGRxFileIO.h"
#include "btkMOMFileIO.h"
#include "btkPWRFileIO.h"
#include "btkRAxFileIO.h"
#include "btkRICFileIO.h"
// AMTI
#include "btkBSFFileIO.h"
// Kistler
#include "btkKistlerDATFileIO.h"
// Others
#include "btkEMFFileIO.h"
#include "btkCLBFileIO.h"

namespace btk
{
  /**
   * List of all the acquisition file IO supported in BTK.
   */
  BTK_ACQUISITON_FILE_IO_FACTORY_INIT
  {
    // ----------------------------------------------------------------------------
    //                 STEP 2: REGISTER THE ACQUISTION FILES IO HERE
    // ----------------------------------------------------------------------------
    // Use the macro BTK_REGISTER_ACQUISITION_FILE_IO for this usage.
    // This macro creates a handle for the given file IO and insert it into the factory.

    BTK_REGISTER_ACQUISITION_FILE_IO(C3DFileIO)

    BTK_REGISTER_ACQUISITION_FILE_IO(ANBFileIO)
    BTK_REGISTER_ACQUISITION_FILE_IO(ANCFileIO)
    BTK_REGISTER_ACQUISITION_FILE_IO(CALForcePlateFileIO)
    BTK_REGISTER_ACQUISITION_FILE_IO(TRBFileIO)
    BTK_REGISTER_ACQUISITION_FILE_IO(TRCFileIO)
    BTK_REGISTER_ACQUISITION_FILE_IO(XLSOrthoTrakFileIO)

    BTK_REGISTER_ACQUISITION_FILE_IO(MDFFileIO)
    BTK_REGISTER_ACQUISITION_FILE_IO(XMOVEFileIO)

    BTK_REGISTER_ACQUISITION_FILE_IO(TDFFileIO)

    BTK_REGISTER_ACQUISITION_FILE_IO(DelsysEMGFileIO) // MUST BE BEFORE EMxFileIO
    BTK_REGISTER_ACQUISITION_FILE_IO(HPFFileIO)

    BTK_REGISTER_ACQUISITION_FILE_IO(ANGFileIO)
    BTK_REGISTER_ACQUISITION_FILE_IO(EMxFileIO)
    BTK_REGISTER_ACQUISITION_FILE_IO(GRxFileIO)
    BTK_REGISTER_ACQUISITION_FILE_IO(MOMFileIO)
    BTK_REGISTER_ACQUISITION_FILE_IO(PWRFileIO)
    BTK_REGISTER_ACQUISITION_FILE_IO(RAxFileIO)
    BTK_REGISTER_ACQUISITION_FILE_IO(RICFileIO)

    BTK_REGISTER_ACQUISITION_FILE_IO(BSFFileIO)
    
    BTK_REGISTER_ACQUISITION_FILE_IO(KistlerDATFileIO)

    BTK_REGISTER_ACQUISITION_FILE_IO(EMFFileIO)
    BTK_REGISTER_ACQUISITION_FILE_IO(CLBFileIO)
  };
};

#endif // __btkAcquisitionFileIOFactory_registration_h
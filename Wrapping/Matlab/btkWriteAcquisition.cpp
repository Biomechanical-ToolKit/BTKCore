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

#include "btkMXObjectHandle.h"
#include "btkMEXOutputRedirection.h"

#include <btkAcquisitionFileWriter.h>
#include <btkAcquisitionFileIOFactory.h>

#include <algorithm>
#include <cctype>

const std::string extractByteOrderOption(const mxArray* opt, btk::AcquisitionFileIO::ByteOrder* b)
{
  std::string errMsg;
  size_t strlen_ = (mxGetM(opt) * mxGetN(opt) * sizeof(mxChar)) + 1;
  char* option = (char*)mxMalloc(strlen_);
  mxGetString(opt, option, strlen_);
  std::string uppercase = std::string(option);
  std::transform(uppercase.begin(), uppercase.end(), uppercase.begin(), toupper);
  if (uppercase.compare("IEEE_BIGENDIAN") == 0)
    *b = btk::AcquisitionFileIO::IEEE_BigEndian;
  else if (uppercase.compare("IEEE_LITTLEENDIAN") == 0)
    *b = btk::AcquisitionFileIO::IEEE_LittleEndian;
  else if (uppercase.compare("VAX_LITTLEENDIAN") == 0)
    *b = btk::AcquisitionFileIO::VAX_LittleEndian;
  else
    errMsg = "Unknown ByteOrder option: '" + std::string(option) + "'.";
  mxFree(option);
  return errMsg;
};

const std::string extractStorageFormatOption(const mxArray* opt, btk::AcquisitionFileIO::StorageFormat* s)
{
  std::string errMsg;
  size_t strlen_ = (mxGetM(opt) * mxGetN(opt) * sizeof(mxChar)) + 1;
  char* option = (char*)mxMalloc(strlen_);
  mxGetString(opt, option, strlen_);
  std::string uppercase = std::string(option);
  std::transform(uppercase.begin(), uppercase.end(), uppercase.begin(), toupper);
  if (uppercase.compare("FLOAT") == 0)
    *s = btk::AcquisitionFileIO::Float;
  else if (uppercase.compare("INTEGER") == 0)
    *s = btk::AcquisitionFileIO::Integer;
  else
    errMsg = "Unknown StorageFormat option: '" + std::string(option) + "'.";
  mxFree(option);
  return errMsg;
};

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  if(nrhs < 2)
    mexErrMsgTxt("Two inputs required.");
  if ((nrhs % 2) != 0)
    mexErrMsgTxt("Incorrect number of inputs.");

  btkMXCheckNoOuput(nlhs, plhs); // Only when there is no output for the function.

  btk::Acquisition::Pointer acq = btk_MOH_get_object<btk::Acquisition>(prhs[0]);

  size_t strlen_ = (mxGetM(prhs[1]) * mxGetN(prhs[1]) * sizeof(mxChar)) + 1;
  char* filename = (char*)mxMalloc(strlen_);
  mxGetString(prhs[1], filename, strlen_);

  btk::AcquisitionFileIO::ByteOrder byteOrderOption = btk::AcquisitionFileIO::OrderNotApplicable;
  btk::AcquisitionFileIO::StorageFormat storageFormatOption = btk::AcquisitionFileIO::StorageNotApplicable;
  std::string errMsg;
  
  char* option = 0;
  const char* options[] = {"BYTEORDER", "STORAGEFORMAT"};
  int numberOfOptions =  sizeof(options) / sizeof(char*);
  for (int i = 2 ; i < nrhs ; i += 2)
  {
    strlen_ = (mxGetM(prhs[i]) * mxGetN(prhs[i]) * sizeof(mxChar)) + 1;
    option = (char*)mxMalloc(strlen_);
    mxGetString(prhs[i], option, strlen_);
    int j = 0;
    for (j = 0 ; j < numberOfOptions ; ++j)
    {
      std::string uppercase = std::string(option);
      std::transform(uppercase.begin(), uppercase.end(), uppercase.begin(), toupper);
      if (uppercase.compare(options[j]) == 0)
      {
        switch(j)
        {
        case 0:
          errMsg = extractByteOrderOption(prhs[i+1], &byteOrderOption);
          break;
        case 1:
          errMsg = extractStorageFormatOption(prhs[i+1], &storageFormatOption);
          break;
        }
        break;
      }
    }
    if (j == numberOfOptions)
      errMsg = "Unknown option: '" + std::string(option) + "'.";
    mxFree(option);
    if (!errMsg.empty())
      mexErrMsgTxt(errMsg.c_str());
  }
  
  // std::cout redirection to the mexPrintf function.
  btk::MEXCoutToPrintf coutRedir = btk::MEXCoutToPrintf();
  // std::cerr redirection to the mexWarnMsgTxt function.
  btk::MEXCerrToWarnMsgTxt cerrRedir = btk::MEXCerrToWarnMsgTxt("btk:WriteAcquisition");

  // The IO detection is done here to be able to set options.
  btk::AcquisitionFileIO::Pointer io = btk::AcquisitionFileIOFactory::CreateAcquisitionIO(filename, btk::AcquisitionFileIOFactory::WriteMode);
  if (io.get() != 0)
  {
    if (byteOrderOption != btk::AcquisitionFileIO::OrderNotApplicable)
      io->SetByteOrder(byteOrderOption);      
    if (storageFormatOption != btk::AcquisitionFileIO::StorageNotApplicable)
      io->SetStorageFormat(storageFormatOption);      
  }

  btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
  writer->SetAcquisitionIO(io);
  writer->SetFilename(std::string(filename));
  writer->SetInput(acq);
  
  mxFree(filename);
  
  try
  {
    writer->Update();
  }
  catch(std::exception& e)
  {
    std::remove(filename);
    errMsg = e.what();   
  }
  catch(...)
  {
    std::remove(filename);
    errMsg = "An unexpected error occurred.";
  }

  if (!errMsg.empty())
    mexErrMsgTxt(errMsg.c_str());
};


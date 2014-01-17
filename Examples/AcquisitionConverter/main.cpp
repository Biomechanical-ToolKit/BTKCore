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

#include <btkAcquisitionFileReader.h>
#include <btkAcquisitionFileWriter.h>
#include <btkMacro.h> // btkStripPathMacro

#include <iostream> // std::cerr
#include <cstdio> // Include std::remove for Linux

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    std::cerr << "No enough input arguements.\n\n"
              << "Usage: " << btkStripPathMacro(argv[0]) << " input output\n\n"
              << "Convert an acquisition file into another one, whatever the formats used."
              << std::endl;
    return -1;
  }
 
  // New instantiation of an acquisition file reader. 
  btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
  // Set the path of the acquisition to read.
  reader->SetFilename(argv[1]);
  // New instamtiation of an acquisition file writer
  btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
  // Set the path of the file to write. The acquisition will be exported in this file.
  writer->SetFilename(argv[2]);
  // Link the writer with the reader.
  writer->SetInput(reader->GetOutput());

  // Manage possible exceptions during the update of the pipeline.
  try 
  {
    writer->Update();
  }
  catch(btk::Exception& e)
  {
    std::cerr << "Exception: " << e.what() << std::endl;
    std::remove(argv[2]);    
    return -2;
  }
  catch(std::exception& e)
  {
    std::cerr << "Unknown exception: " << e.what() << std::endl;
    std::remove(argv[2]);    
    return -3;
  }
  catch(...)
  {
    std::cerr << "Unknown exception";
    return -3;
  }

  return 0;
};

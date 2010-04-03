/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2010, Arnaud Barré
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

#include "btkRICFileIO.h"
#include "btkEliteFileIOUtils_p.h"

#include <fstream>
#include <algorithm>
#include <cctype>

namespace btk
{
  /**
   * @class RICFileIOException
   * @brief Exception class for the RICFileIO class.
   */
  
  /**
   * @fn RICFileIOException::RICFileIOException(const std::string& msg)
   * Constructor.
   */
  
  /**
   * @fn virtual RICFileIOException::~RICFileIOException()
   * Empty destructor.
   */
  
  /**
   * @class RICFileIO
   * @brief Interface to read/write RIC files.
   *
   * The RIC file format is used in the Elite acquisition system (BTS, Milan, Italy).
   *
   * @ingroup BTKIO
   */
  
  /**
   * @typedef RICFileIO::Pointer
   * Smart pointer associated with a RICFileIO object.
   */
  
  /**
   * @typedef RICFileIO::ConstPointer
   * Smart pointer associated with a const RICFileIO object.
   */
  
  /**
   * @fn static RICFileIO::Pointer RICFileIO::New()
   * Create a RICFileIO object an return it as a smart pointer.
   */
  
  /**
   * Only check if the file extension correspond to RIC.
   */
  bool RICFileIO::CanReadFile(const std::string& filename)
  {
    std::string lowercase = filename;
    std::transform(lowercase.begin(), lowercase.end(), lowercase.begin(), tolower);
    std::string::size_type RICPos = lowercase.rfind(".ric");
    if ((RICPos != std::string::npos) && (RICPos == lowercase.length() - 4))
    {
      std::ifstream ifs;
      ifs.open(filename.c_str(), std::ios_base::in | std::ios_base::binary);
      if (!ifs.is_open())
        return false;
      ifs.close();
      return true;
    }
    return false;
  };
  
  /**
   * Checks if the suffix of @a filename is RIC.
   */
  bool RICFileIO::CanWriteFile(const std::string& filename)
  {
    std::string lowercase = filename;
    std::transform(lowercase.begin(), lowercase.end(), lowercase.begin(), tolower);
    std::string::size_type RICPos = lowercase.rfind(".ric");
    if ((RICPos != std::string::npos) && (RICPos == lowercase.length() - 4))
      return true;
    else
      return false;
  };
  
  /**
   * Read the file designated by @a filename and fill @a output.
   */
  void RICFileIO::Read(const std::string& filename, Acquisition::Pointer output)
  {
    output->Reset();
    std::fstream ifs;
    ifs.exceptions(std::ios_base::eofbit | std::ios_base::failbit | std::ios_base::badbit);
    try
    {
      ifs.open(filename.c_str(), std::ios_base::in | std::ios_base::binary);
      IEEELittleEndianBinaryFileStream bifs(ifs);
      ReadEliteHeader_p(output, &bifs, true);
      ReadEliteMarkersValues_p(output, &bifs);
      ReadEliteLabel_p(output, &bifs);
    }
    catch (std::fstream::failure& )
    {
      std::string excmsg; 
      if (ifs.eof())
        excmsg = "Unexpected end of file.";
      else if (!ifs.is_open())
        excmsg = "Invalid file path.";
      else if(ifs.bad())
        excmsg = "Loss of integrity of the filestream.";
      else if(ifs.fail())
        excmsg = "Internal logic operation error on the stream associated with the file.";
      else
        excmsg = "Unknown error associated with the filestream.";
      
      if (ifs.is_open()) ifs.close();    
      throw(RICFileIOException(excmsg));
    }
    catch (RICFileIOException& )
    {
      if (ifs.is_open()) ifs.close(); 
      throw;
    }
    catch (std::exception& e)
    {
      if (ifs.is_open()) ifs.close(); 
      throw(RICFileIOException("Unexpected exception occurred: " + std::string(e.what())));
    }
    catch(...)
    {
      if (ifs.is_open()) ifs.close(); 
      throw(RICFileIOException("Unknown exception"));
    }
  };
  
  /**
   * Write the file designated by @a filename with the content of @a input.
   */
  void RICFileIO::Write(const std::string& filename, Acquisition::Pointer input)
  {
    /*
    if (input.get() == 0)
    {
      btkIOErrorMacro(filename, "Empty input. Impossible to write an empty file.");
      return;
    }
    */
    btkErrorMacro("Method not yet implemented.");
    return;
  };
  
  /**
   * Constructor.
   */
  RICFileIO::RICFileIO()
  : AcquisitionFileIO()
  {
    this->SetFileType(AcquisitionFileIO::Binary);
    this->SetByteOrder(AcquisitionFileIO::IEEE_LittleEndian);
    this->SetStorageFormat(AcquisitionFileIO::Float);
  };
};

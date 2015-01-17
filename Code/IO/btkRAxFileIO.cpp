/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2015, Arnaud Barré
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

#include "btkRAxFileIO.h"
#include "btkEliteFileIOUtils_p.h"

#include <algorithm>
#include <cctype>

namespace btk
{
  /**
   * @class RAxFileIOException btkRAxFileIO.h
   * @brief Exception class for the RAxFileIO class.
   */
  
  /**
   * @fn RAxFileIOException::RAxFileIOException(const std::string& msg)
   * Constructor.
   */
  
  /**
   * @fn virtual RAxFileIOException::~RAxFileIOException()
   * Empty destructor.
   */
  
  /**
   * @class RAxFileIO btkRAxFileIO.h
   * @brief Interface to read/write RAx files.
   *
   * The RAx file format is used in the Elite acquisition system (BTS, Milan, Italy).
   *
   * @ingroup BTKIO
   */
  
  /**
   * @typedef RAxFileIO::Pointer
   * Smart pointer associated with a RAxFileIO object.
   */
  
  /**
   * @typedef RAxFileIO::ConstPointer
   * Smart pointer associated with a const RAxFileIO object.
   */
  
  /**
   * @fn static RAxFileIO::Pointer RAxFileIO::New()
   * Create a RAxFileIO object an return it as a smart pointer.
   */
  
  /**
   * Only check if the file extension correspond to RA(h|w).
   */
  bool RAxFileIO::CanReadFile(const std::string& filename)
  {
    std::string lowercase = filename;
    std::transform(lowercase.begin(), lowercase.end(), lowercase.begin(), tolower);
    std::string::size_type RAxPos = lowercase.rfind(".ra");
    if ((RAxPos != std::string::npos) && (RAxPos == lowercase.length() - 4)  && ((*(lowercase.rbegin()) == 'h') || (*(lowercase.rbegin()) == 'w')))
    {
      std::ifstream ifs(filename.c_str(), std::ios_base::in | std::ios_base::binary);
      if (!ifs)
        return false;
      ifs.close();
      return true;
    }
    return false;
  };
  
  /**
   * Read the file designated by @a filename and fill @a output.
   */
  void RAxFileIO::Read(const std::string& filename, Acquisition::Pointer output)
  {
    output->Reset();
    IEEELittleEndianBinaryFileStream bifs;
    bifs.SetExceptions(BinaryFileStream::EndFileBit | BinaryFileStream::FailBit | BinaryFileStream::BadBit);
    try
    {
      bifs.Open(filename, BinaryFileStream::In);
      
      ReadEliteHeader_p(output, &bifs);
      ReadEliteMarkersValues_p(output, &bifs);
      ReadEliteLabel_p(output, &bifs);
    }
    catch (BinaryFileStreamFailure& )
    {
      std::string excmsg; 
      if (bifs.EndFile())
        excmsg = "Unexpected end of file.";
      else if (!bifs.IsOpen())
        excmsg = "Invalid file path.";
      else if(bifs.Bad())
        excmsg = "Loss of integrity of the file stream.";
      else if(bifs.Fail())
        excmsg = "Internal logic operation error on the stream associated with the file.";
      else
        excmsg = "Unknown error associated with the file stream.";
      
      if (bifs.IsOpen()) bifs.Close();    
      throw(RAxFileIOException(excmsg));
    }
    catch (RAxFileIOException& )
    {
      if (bifs.IsOpen()) bifs.Close(); 
      throw;
    }
    catch (std::exception& e)
    {
      if (bifs.IsOpen()) bifs.Close(); 
      throw(RAxFileIOException("Unexpected exception occurred: " + std::string(e.what())));
    }
    catch(...)
    {
      if (bifs.IsOpen()) bifs.Close(); 
      throw(RAxFileIOException("Unknown exception"));
    }
  };
  
  /**
   * Constructor.
   */
  RAxFileIO::RAxFileIO()
  : AcquisitionFileIO(AcquisitionFileIO::Binary, AcquisitionFileIO::IEEE_LittleEndian, AcquisitionFileIO::Float)
  {};
};

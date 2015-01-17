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

#include "btkRICFileIO.h"
#include "btkEliteFileIOUtils_p.h"
#include "btkLogger.h"

#include <algorithm>
#include <cctype>

namespace btk
{
  /**
   * @class RICFileIOException btkRICFileIO.h
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
   * @class RICFileIO btkRICFileIO.h
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
   * Only check if the file extension correspond to RIC or RIF.
   */
  bool RICFileIO::CanReadFile(const std::string& filename)
  {
    std::string lowercase = filename;
    std::transform(lowercase.begin(), lowercase.end(), lowercase.begin(), tolower);
    std::string::size_type RIxPos = lowercase.substr(0,lowercase.length()-1).rfind(".ri");
    if ((RIxPos != std::string::npos) && (RIxPos == lowercase.length() - 4) && ((*(lowercase.rbegin()) == 'c') || (*(lowercase.rbegin()) == 'f')))
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
  void RICFileIO::Read(const std::string& filename, Acquisition::Pointer output)
  {
    output->Reset();
    IEEELittleEndianBinaryFileStream bifs;
    bifs.SetExceptions(BinaryFileStream::EndFileBit | BinaryFileStream::FailBit | BinaryFileStream::BadBit);
    try
    {
      bifs.Open(filename, BinaryFileStream::In);
      // Determine the size of the file
      bifs.SeekRead(0, BinaryFileStream::End);
      BinaryFileStream::StreamPosition sizeFile = bifs.TellRead();
      bifs.SeekRead(0, BinaryFileStream::Begin);
      
      ReadEliteHeader_p(output, &bifs, true, true);
      ReadEliteMarkersValues_p(output, &bifs);
      // Check if the file contains a section with the labels
      if (bifs.TellRead() < sizeFile)
        ReadEliteLabel_p(output, &bifs);
      else
      {
        btkWarningMacro(filename, "There is no section containing the labels of the markers.");
      }
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
      throw(RICFileIOException(excmsg));
    }
    catch (RICFileIOException& )
    {
      if (bifs.IsOpen()) bifs.Close(); 
      throw;
    }
    catch (std::exception& e)
    {
      if (bifs.IsOpen()) bifs.Close(); 
      throw(RICFileIOException("Unexpected exception occurred: " + std::string(e.what())));
    }
    catch(...)
    {
      if (bifs.IsOpen()) bifs.Close(); 
      throw(RICFileIOException("Unknown exception"));
    }
  };
  
  /**
   * Constructor.
   */
  RICFileIO::RICFileIO()
  : AcquisitionFileIO(AcquisitionFileIO::Binary, AcquisitionFileIO::IEEE_LittleEndian, AcquisitionFileIO::Float)
  {};
};

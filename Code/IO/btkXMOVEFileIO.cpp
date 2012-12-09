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

#include "btkXMOVEFileIO.h"
#include "btkCodamotionFileIOUtils_p.h"
#include "btkConfigure.h"

#include "Open3DMotion/MotionFile/Formats/XMOVE/FileFormatXMove.h"

// Workaround to be able to construct a Open3DMotion::FileFormatXMOVE object.
class FileFormatXMove_p : public Open3DMotion::FileFormatXMove
{
public:
  FileFormatXMove_p() : Open3DMotion::FileFormatXMove() {};
};

namespace btk
{
  /**
   * @class XMOVEFileIOException btkXMOVEFileIO.h
   * @brief Exception class for the XMOVEFileIO class.
   */
  
  /**
   * @fn XMOVEFileIOException::XMOVEFileIOException(const std::string& msg)
   * Constructor.
   */
  
  /**
   * @fn virtual XMOVEFileIOException::~XMOVEFileIOException()
   * Empty destructor.
   */
  
  /**
   * @class XMOVEFileIO btkXMOVEFileIO.h
   * @brief Interface to read XMOVE/MDR files (Codamotion).
   *
   * The XMOVE file format is created by the company Charnwood Dynamics Ltd (Codamotion).
   *
   * This class uses internally the code of the library Open3DMotion (http://github.com/Open3DMotionGroup/Open3DMotion).
   *
   * @ingroup BTKIO
   */
  
  /**
   * @typedef XMOVEFileIO::Pointer
   * Smart pointer associated with a XMOVEFileIO object.
   */
  
  /**
   * @typedef XMOVEFileIO::ConstPointer
   * Smart pointer associated with a const XMOVEFileIO object.
   */
  
  /**
   * @fn static XMOVEFileIO::Pointer XMOVEFileIO::New()
   * Create a XMOVEFileIO object an return it as a smart pointer.
   */
  
  /**
   * Checks if the header corresponds to a XMOVE file
   */
  bool XMOVEFileIO::CanReadFile(const std::string& filename)
  {
    Open3DMotion::MotionFileHandler handler("Biomechanical ToolKit", BTK_VERSION_STRING);
    Open3DMotion::TreeValue* readoptions = NULL;
    std::ifstream ifs(filename.c_str(), std::ios::binary);
    FileFormatXMove_p ff;
    return (ifs.is_open() && ff.Probe(handler, readoptions, ifs));
  };
  
  /**
   * Read the file designated by @a filename and fill @a output.
   */
  void XMOVEFileIO::Read(const std::string& filename, Acquisition::Pointer output)
  {
    output->Reset();
    std::ifstream ifs(filename.c_str(), std::ios::binary);
    ifs.exceptions(std::ios::badbit | std::ios::eofbit | std::ios::failbit);
    try
    {
      // Read data
      Open3DMotion::MotionFileHandler handler("Biomechanical ToolKit", BTK_VERSION_STRING);
      Open3DMotion::BinMemFactoryDefault memfactory;
      Open3DMotion::TreeValue* readoptions = NULL;
      FileFormatXMove_p ff;
      if(!ff.Probe(handler, readoptions, ifs))
        throw(XMOVEFileIOException("The given file is not a valid XMOVE file."));
      FillAcquisitionFromOpen3DMotion_p(output, filename, &ff, handler, ifs, memfactory, readoptions);
    }
    catch (std::ios::failure& )
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
      throw XMOVEFileIOException(excmsg);
    }
    catch (XMOVEFileIOException& )
    {
      if (ifs.is_open()) ifs.close(); 
      throw;
    }
    catch (CodamotionFileIOException& e)
    {
      if (ifs.is_open()) ifs.close(); 
      throw XMOVEFileIOException(e.what());
    }
    catch (Open3DMotion::MotionFileException& e)
    {
      if (ifs.is_open()) ifs.close(); 
      throw XMOVEFileIOException(e.message);
    }
    catch (std::exception& e)
    {
      if (ifs.is_open()) ifs.close(); 
      throw(XMOVEFileIOException("Unexpected exception occurred: " + std::string(e.what())));
    }
    catch(...)
    {
      if (ifs.is_open()) ifs.close(); 
      throw(XMOVEFileIOException("Unknown exception"));
    }
    ifs.close();
  };
  
  /**
   * Constructor.
   */
  XMOVEFileIO::XMOVEFileIO()
  : AcquisitionFileIO()
  {};
};

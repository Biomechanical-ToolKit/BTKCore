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

#include "btkAcquisitionFileReader.h"
#include "btkAcquisitionFileIOFactory.h"

#include <fstream>

namespace btk
{
  /**
   * @class AcquisitionFileReaderException
   * @brief Exception class for the AcquisitionFileReader class.
   */
  
  /**
   * @fn AcquisitionFileReaderException::AcquisitionFileReaderException(const std::string& msg)
   * Constructor.
   */
  
  /**
   * @fn virtual AcquisitionFileReaderException::~AcquisitionFileReaderException()
   * Empty destructor.
   */
  
  /**
   * @class AcquisitionFileReader
   * @brief Reader for files which contain acquisition data (C3D, TRC, ...).
   *
   * @ingroup BTKIO 
   */
  /**
   * @var AcquisitionFileReader::m_Filename
   * Path of the file to read.
   */
  /**
   * @var AcquisitionFileReader::m_AcquisitionIO
   * AcquisitionFileIO helper class to read the acquisition data and fill an Acquisition object.
   */
  
  /**
   * @typedef AcquisitionFileReader::Pointer
   * Smart pointer associated with an AcquisitionFileReader object.
   */
  
  /**
   * @typedef AcquisitionFileReader::ConstPointer
   * Smart pointer associated with a const AcquisitionFileReader object.
   */
  
  /**
   * @fn AcquisitionFileReader::~AcquisitionFileReader()
   * Empty destructor.
   */
  
  /**
   * @fn static AcquisitionFileReader::Pointer AcquisitionFileReader::New()
   * Creates a AcquisitionFileReader process.
   */
  
  /**
   * @fn Acquisition::Pointer AcquisitionFileReader::GetOutput()
   * Returns the Acquisition read from the AcquisitionIO helper class.
   */
  
  /**
   * @fn bool AcquisitionFileReader::GetDisableFilenameExceptionState() const
   * Returns state of the disabling of the exception for missing filename.
   */
   
  /**
   * @fn void AcquisitionFileReader::SetDisableFilenameExceptionState(bool s)
   * Enable/disable exception for the missing of the filename.
   */
  
  /**
   * @fn const std::string& AcquisitionFileReader::GetFilename() const
   * Gets the filename of the file to read.
   */
  
  /**
   * Specifies the file to read. This is forwarded to the IO instance.
   */
  void AcquisitionFileReader::SetFilename(const std::string& filename)
  {
    if (this->m_Filename.compare(filename) != 0)
    {
      this->m_Filename = filename;
      this->Modified();
    }
  };
  
  /**
   * @fn AcquisitionFileIO::Pointer AcquisitionFileReader::GetAcquisitionIO()
   * Returns a Pointer associated with the AcquisitionIO helper class
   */
  
  /**
   * @fn AcquisitionFileIO::ConstPointer AcquisitionFileReader::GetAcquisitionIO() const
   * Returns a ConstPointer associated with the AcquisitionIO helper class
   */
  
  /**
   * Set the AcquisitionIO helper class. This method forces the AcquisitionIO
   * to be used to read a particular file. The AcquisitionIO is oftenly created 
   * via the objectfactory mechanism that determines whether a particular 
   * AcquisitionIO can read a certain file (@see AcquisitionFileIOFactory). 
   */
  void AcquisitionFileReader::SetAcquisitionIO(AcquisitionFileIO::Pointer io)
  {
    if (this->m_AcquisitionIO != io ) 
    {
      this->m_AcquisitionIO = io;
      this->Modified(); 
    }
  };
  
  /**
   * Constructor. Sets the number of outputs equal to one. No input.
   */
  AcquisitionFileReader::AcquisitionFileReader()
  : m_AcquisitionIO(), m_Filename()
  {
    this->SetOutputNumber(1);
    this->m_FilenameExtensionDisabled = false;
  };
  
  /**
   * @fn Acquisition::Pointer AcquisitionFileReader::GetOutput(int idx)
   * Returns the output at the index @a idx.
   */
  
  /**
   * Whatever the specified index, this method creates an Acquisition object
   */
  DataObject::Pointer AcquisitionFileReader::MakeOutput(int idx)
  {
    return Acquisition::New();
  };
  
  /**
   * Check the file integrety, find a AcquisitionIO helper class if no one has
   * been specified and finally read the file.
   */
  void AcquisitionFileReader::GenerateData()
  {
    if (this->m_Filename.empty())
    {
      if (!this->m_FilenameExtensionDisabled)
        throw AcquisitionFileReaderException("Filename must be specified");
      else
        return;
    }
    
    std::ifstream ifs;
    ifs.open(this->m_Filename.c_str());
    // check if the file exists
    if (!ifs.is_open())
      throw AcquisitionFileReaderException("File doesn't exist\nFilename: " + this->m_Filename);
    // check if the file is not read only
    if(ifs.fail())
      throw AcquisitionFileReaderException("File can't be opened. Have you the permission to read this file?\nFilename: " + this->m_Filename);
    ifs.close();
    
    if (this->m_AcquisitionIO.get() == 0)
    {
      this->m_AcquisitionIO = AcquisitionFileIOFactory::CreateAcquisitionIO(this->m_Filename.c_str(), AcquisitionFileIOFactory::ReadMode);
      if (this->m_AcquisitionIO.get() == 0)
        throw AcquisitionFileReaderException("No IO found, the file is not supported or valid or the file suffix is misspelled (Some IO use it to verify they can read the file)\nFilename: " + this->m_Filename);
    }
    
    this->m_AcquisitionIO->Read(this->m_Filename, this->GetOutput());
  };
};

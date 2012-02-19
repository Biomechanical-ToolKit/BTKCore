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

#include "btkAcquisitionFileWriter.h"
#include "btkAcquisitionFileIOFactory.h"

#include <fstream>

namespace btk
{
  /**
   * @class AcquisitionFileWriterException btkAcquisitionFileWriter.h
   * @brief Exception class for the AcquisitionFileWriter class.
   */
  
  /**
   * @fn AcquisitionFileWriterException::AcquisitionFileWriterException(const std::string& msg)
   * Constructor.
   */
  
  /**
   * @fn virtual AcquisitionFileWriterException::~AcquisitionFileWriterException()
   * Empty destructor.
   */
  
  /**
   * @class AcquisitionFileWriter
   * @brief Writer of acquisition data into acquisition file (C3D, TRC, ...).
   *
   * @note The detection of the helper class to use creates a new object (inheriting of btk::AcquisitionFileIO). Then, if a previous or empty btk::AcquisitionFileIO pointer was given to another filter, it doesn't correspond to the newly detected btk::AcquisitionFileIO. For example:
   * @code
   * btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
   * reader->SetFilename("...");
   * btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
   * writer->SetAcquisitionIO(reader->GetAcquisitionIO());
   * writer->SetInput(reader->GetOutput());
   * writer->SetFilename("...");
   * writer->Update();
   * @endcode
   * This code give to the writer an empty btk::FileAcquisitionIO. When the writer is updated, it updates also the reader which detects the btk::FileAcquisitionIO corresponding to the given file. However, the writer has the old (empty) value and will detect the btk::FileAcquisitionIO to used instead of using the same as the reader.\n
   * To use the same btk::FileAcquisitionIO, the reader must be updated before the use of the method btk::AcquisitionFileWriter::SetAcquisitionIO.
   * @code
   * btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
   * reader->SetFilename("...");
   * reader->Update(); // <-- Line to add
   * btk::AcquisitionFileWriter::Pointer writer = btk::AcquisitionFileWriter::New();
   * writer->SetAcquisitionIO(reader->GetAcquisitionIO());
   * writer->SetInput(reader->GetOutput());
   * writer->SetFilename("...");
   * writer->Update();
   * @endcode
   *
   * @ingroup BTKIO
   */
  /**
   * @var AcquisitionFileWriter::m_Filename
   * Path of the file to read.
   */
  /**
   * @var AcquisitionFileWriter::m_AcquisitionIO
   * AcquisitionFileIO helper class to read the acquisition data and fill an Acquisition object.
   */
  
  /**
   * @typedef AcquisitionFileWriter::Pointer
   * Smart pointer associated with an AcquisitionFileWriter object.
   */
  
  /**
   * @typedef AcquisitionFileWriter::ConstPointer
   * Smart pointer associated with a const AcquisitionFileWriter object.
   */
  
  /**
   * @fn AcquisitionFileWriter::~AcquisitionFileWriter()
   * Empty destructor.
   */
  
  /**
   * @fn static AcquisitionFileWriter::Pointer AcquisitionFileWriter::New()
   * Creates a AcquisitionFileWriter process.
   */
  
  /**
   * @fn Acquisition::Pointer AcquisitionFileWriter::GetInput()
   * Returns the Acquisition to write by a AcquisitionIO helper class.
   */

  /**
   * @fn void AcquisitionFileWriter::SetInput(Acquisition::Pointer input)
   * Sets the Acquisition to write by a AcquisitionIO helper class
   */
  
  /**
   * @fn const std::string& AcquisitionFileWriter::GetFilename() const
   * Gets the filename of the file to write.
   */
  
  /**
   * Specifies the file to write. This is forwarded to the IO instance.
   */
  void AcquisitionFileWriter::SetFilename(const std::string& filename)
  {
    if (this->m_Filename.compare(filename) != 0)
    {
      this->m_Filename = filename;
      this->Modified();
    }
  };
  
  /**
   * @fn AcquisitionFileIO::Pointer AcquisitionFileWriter::GetAcquisitionIO()
   * Returns a Pointer associated with the AcquisitionIO helper class
   */
  
  /**
   * @fn AcquisitionFileIO::ConstPointer AcquisitionFileWriter::GetAcquisitionIO() const
   * Returns a ConstPointer associated with the AcquisitionIO helper class
   */
  
  /**
   * Set the AcquisitionIO helper class. This method forces the AcquisitionIO
   * to be used to write a particular file. The AcquisitionIO is oftenly created 
   * via the object factory mechanism that determines whether a particular 
   * AcquisitionIO can write a certain file (see AcquisitionFileIOFactory). 
   */
  void AcquisitionFileWriter::SetAcquisitionIO(AcquisitionFileIO::Pointer io)
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
  AcquisitionFileWriter::AcquisitionFileWriter()
  : m_AcquisitionIO(), m_Filename()
  {
    this->SetInputNumber(1);
  };
  
  /**
   * @fn Acquisition::Pointer AcquisitionFileWriter::GetInput(int idx)
   * Returns the input at the index @a idx.
   */
  
  /**
   * Whatever the specified index, this method creates an Acquisition object
   */
  DataObject::Pointer AcquisitionFileWriter::MakeOutput(int idx)
  {
    btkNotUsed(idx);
    throw(RuntimeError("btk::AcquisitionFileWriter has not output."));
  };
  
  /**
   * Check the file integrety, find a AcquisitionIO helper class if no one has
   * been specified and finally read the file.
   */
  void AcquisitionFileWriter::GenerateData()
  {
    if (this->m_Filename.empty())
      throw AcquisitionFileWriterException("Filename must be specified.");
    
    std::ofstream ofs(this->m_Filename.c_str());
    // check if the file exists
    if (!ofs)
      throw AcquisitionFileWriterException("File can't be opened. Have you the permission to write this file?\nFilename: " + this->m_Filename);
    ofs.close();
    
    if (this->m_AcquisitionIO.get() == 0)
    {
      this->m_AcquisitionIO = AcquisitionFileIOFactory::CreateAcquisitionIO(this->m_Filename.c_str(), AcquisitionFileIOFactory::WriteMode);
      if (this->m_AcquisitionIO.get() == 0)
        throw AcquisitionFileWriterException("No IO found, the file is not supported or the file suffix is misspelled (IOs use it to verify they can write the file)\nFilename: " + this->m_Filename);
    }
    
    this->m_AcquisitionIO->Write(this->m_Filename, this->GetInput());
  };
};

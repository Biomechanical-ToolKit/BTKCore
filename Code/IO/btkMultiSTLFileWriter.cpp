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

#include "btkMultiSTLFileWriter.h"
#include "btkBinaryFileStream.h"
#include "btkConvert.h"

namespace btk
{
  /**
   * @class MultiSTLFileWriterException btkMultiSTLFileWriter.h
   * @brief Exception class for the MultiSTLFileWriter class.
   */
  
  /**
   * @fn MultiSTLFileWriterException::MultiSTLFileWriterException(const std::string& msg)
   * Constructor.
   */
  
  /**
   * @fn virtual MultiSTLFileWriterException::~MultiSTLFileWriterException()
   * Empty destructor.
   */
  
  /**
   * @class MultiSTLFileWriter
   * @brief Writer of acquisition data into multiple STL binary file format.
   *
   * This writer extract for each frame the solid from the given acquisition and defined 
   * triangle mesh and write it in STL files defined by the given file prefix.
   *
   * The file prefix must contains the path to write the STL files and the beginning of the file. The frame index
   * and the file extension will be added during the creation of the file. For examples, the file prefix '/Users/jdoe/Data/FaceScan_'
   * will generate the following files: '/Users/jdoe/Data/FaceScan_1.stl' ... '/Users/jdoe/Data/FaceScan_30.stl'
   *
   * You can export only a subset of the acquisition by specifying the frames of interest using the method SetFramesOfInterest().
   *
   * @ingroup BTKIO
   */
  
  /**
   * @typedef MultiSTLFileWriter::Pointer
   * Smart pointer associated with an MultiSTLFileWriter object.
   */
  
  /**
   * @typedef MultiSTLFileWriter::ConstPointer
   * Smart pointer associated with a const MultiSTLFileWriter object.
   */
  
  /**
   * @fn MultiSTLFileWriter::~MultiSTLFileWriter()
   * Empty destructor.
   */
  
  /**
   * @fn static MultiSTLFileWriter::Pointer MultiSTLFileWriter::New()
   * Creates a MultiSTLFileWriter process
   */
  
  /**
   * @fn Acquisition::Pointer MultiSTLFileWriter::GetInputAcquisition()
   * Returns the Acquisition used to create the STL files.
   */

  /**
   * @fn void MultiSTLFileWriter::SetInputAcquisition(Acquisition::Pointer input)
   * Sets the Acquisition used to create the STL files.
   */
  
    
  /**
   * @fn TriangleMesh::Pointer MultiSTLFileWriter::GetInputMesh()
   * Returns the mesh used to create the STL files.
   */
 
  /**
   * @fn void MultiSTLFileWriter::SetInputMesh(TriangleMesh::Pointer input)
   * Sets the mesh information which will be used to create the STL files.
   */
  
  /**
   * @fn const std::string& MultiSTLFileWriter::GetFilePrefix() const
   * Returns the file prefix used to create the STL files.
   */
  
  /**
   * Specifies the file to read. This is forwarded to the IO instance.
   */
  void MultiSTLFileWriter::SetFilePrefix(const std::string& prefix)
  {
    if (this->m_FilePrefix.compare(prefix) != 0)
    {
      this->m_FilePrefix = prefix;
      this->Modified();
    }
  };
    
  /**
   * @fn const int* MultiSTLFileWriter::GetFramesOfInterest() const
   * Returns the frames of interest.
   */
  
  /**
   * @fn void MultiSTLFileWriter::GetFramesOfInterest(int& ff, int& lf) const
   * Returns the frames of interest.
   */
   
  /**
   * Sets the frames of interest to export.
   * Setting @a ff to -1 means you start from the first frame of the acquisition.
   * Setting @a lf to -1 means you use the last frame of the acquisition.
   */
  void MultiSTLFileWriter::SetFramesOfInterest(int ff, int lf)
  {
    if ((this->m_FOI[0] != ff) || (this->m_FOI[1] != lf))
    {
      this->m_FOI[0] = ff;
      this->m_FOI[1] = lf;
      this->Modified();
    }
  };
  
  /**
   * Constructor. Sets the number of outputs equal to one. No input.
   */
  MultiSTLFileWriter::MultiSTLFileWriter()
  : m_FilePrefix()
  {
    this->m_FOI[0] = -1;
    this->m_FOI[1] = -1;
    this->SetInputNumber(1);
  };
  
  /**
   * Whatever the specified index, this method creates an Acquisition object
   */
  DataObject::Pointer MultiSTLFileWriter::MakeOutput(int idx)
  {
    btkNotUsed(idx);
    throw(RuntimeError("btk::MultiSTLFileWriter has not output."));
  };
  
  /**
   * Create STL files based on the given file prefix (see SetFilePrefix()) and fill them with the given acquisition (see SetInputAcquisition()) and the given mesh (see SetInputMesh()). You can also set the frames to extract using the method SetFramesOfInterest().
   */
  void MultiSTLFileWriter::GenerateData()
  {
    if (this->m_FilePrefix.empty())
      throw MultiSTLFileWriterException("File prefix must be specified.");
    
    Acquisition::Pointer acquisition = this->GetInputAcquisition();
    if (!acquisition)
    {
      btkErrorMacro("Impossible to write a null input into a file (acquisition).");
      return;
    }
    TriangleMesh::Pointer mesh = this->GetInputMesh();
    if (!mesh)
    {
      btkErrorMacro("Impossible to write a null input into a file (mesh).");
      return;
    }
      
    int ff = (this->m_FOI[0] == -1 ? acquisition->GetFirstFrame() : this->m_FOI[0]);
    int lf = (this->m_FOI[1] == -1 ? acquisition->GetLastFrame() : this->m_FOI[1]);
    
    if (ff < acquisition->GetFirstFrame())
      throw MultiSTLFileWriterException("First frame out of range.");
    else if (lf > acquisition->GetLastFrame())
      throw MultiSTLFileWriterException("Last frame out of range.");
    
    if (mesh->GetMaxVertexId() > acquisition->GetPointNumber())
      throw MultiSTLFileWriterException("Invalid vertex ID.");

    // Try to link the mesh with the acquisition data
    if (!mesh->ConnectPoints(acquisition->GetPoints()))
      throw MultiSTLFileWriterException("Marker index out of range.");
    
    int num = btkNumberOfDigits(lf);
    try
    { 
      IEEELittleEndianBinaryFileStream obfs;
      for (int i = ff ; i <= lf ; ++i)
      {
        std::stringstream filename("");
        filename << this->m_FilePrefix << std::setw(num) << std::setfill('0') << i << ".stl";
        obfs.Open(filename.str(), BinaryFileStream::Out | BinaryFileStream::Truncate);
        if (!obfs.IsOpen())
          throw(MultiSTLFileWriterException("No File access. Are you sure of the path? Have you the right privileges?"));
        std::string header = "STL binary file generated by BTK " + std::string(BTK_VERSION_STRING);
        header.resize(80);
        obfs.Write(header);
        obfs.Write((int32_t)0); // Fake number of triangles
        mesh->SetCurrentFrameIndex(i - acquisition->GetFirstFrame());
        int32_t validFaceNumber = 0;
        for (TriangleMesh::FaceConstIterator it = mesh->BeginFace() ;  it != mesh->EndFace() ; ++it)
        {
          if (!it->IsValid())
            continue;
          // Normal vector: set to 0 => Will be computed by the viewer program
          obfs.Write(0.0f);
          obfs.Write(0.0f);
          obfs.Write(0.0f);
          // Vertex 1
          obfs.Write(static_cast<float>(it->GetVertex1()->GetCoordinateX()));
          obfs.Write(static_cast<float>(it->GetVertex1()->GetCoordinateY()));
          obfs.Write(static_cast<float>(it->GetVertex1()->GetCoordinateZ()));
          // Vertex 2
          obfs.Write(static_cast<float>(it->GetVertex2()->GetCoordinateX()));
          obfs.Write(static_cast<float>(it->GetVertex2()->GetCoordinateY()));
          obfs.Write(static_cast<float>(it->GetVertex2()->GetCoordinateZ()));
          // Vertex 3
          obfs.Write(static_cast<float>(it->GetVertex3()->GetCoordinateX()));
          obfs.Write(static_cast<float>(it->GetVertex3()->GetCoordinateY()));
          obfs.Write(static_cast<float>(it->GetVertex3()->GetCoordinateZ()));
          // Attribute byte count
          obfs.Write((uint16_t)0);
          ++validFaceNumber;
        }
        // Write the true number of triangles
        obfs.SeekRead(80, BinaryFileStream::Begin);
        obfs.Write(validFaceNumber);
        obfs.Close();
      }
    }
    catch (MultiSTLFileWriterException& )
    {
      throw;
    }
    catch (std::exception& e)
    {
      throw(MultiSTLFileWriterException("Unexpected exception occurred: " + std::string(e.what())));
    }
    catch(...)
    {
      throw(MultiSTLFileWriterException("Unknown exception"));
    }
  };
};

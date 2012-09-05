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

#include "btkAMTIForcePlatformFileIO.h"
#include "btkMetaDataUtils.h"
#include "btkConvert.h"

#include <fstream>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <map>

namespace btk
{
  /**
   * @class AMTIForcePlatformFileIOException btkAMTIForcePlatformFileIO.h
   * @brief Exception class for the AMTIForcePlatformFileIO class.
   */
  
  /**
   * @fn AMTIForcePlatformFileIOException::AMTIForcePlatformFileIOException(const std::string& msg)
   * Constructor.
   */
  
  /**
   * @fn virtual AMTIForcePlatformFileIOException::~AMTIForcePlatformFileIOException()
   * Empty destructor.
   */
  
  /**
   * @class AMTIForcePlatformFileIO btkAMTIForcePlatformFileIO.h
   * @brief Interface to read/write AMTIForcePlatform files.
   *
   * An AMTI Force Platform file contains 6 analog channels data stored in ASCII format 
   * corresponding to the forces and the moments measured by one AMTI force plateform.
   * This kind of force platform is known as a force platform of type II (2).
   * The forces are measured in newtons (N), while the moments are measured in newton meters (N.m).
   * It is important to not that the returned forces and moments are the reaction measured by the force 
   * platform and not the measure itself. This choice was done to fit with the format stored in a btk::Acquisition object.
   *
   * @warning: The forces and the moments in the file are measured in the platfrom (local) 
   * frame while a btk::Acquisition object store data in the global frame. To be able to express
   * the force, moments as well as the COP (computed by the filter btk::GroundReactionWrenchFilter),
   * this reader assumes that the data come from an AMTI force plate OR6-7, and use then its dimensions (463.6 x 508 x 82.6 mm).
   * All the dimensions must be given in millimeters. If your platform is not this one, then you have to modify the dimensions or the geometry.
   * Moreover, from the dimensions, the coordinates of the 4 corners of the force plate are generated.
   * Compared to the frame embed with the force plate, the corners 1,2,3,4 must be defined as the corner in the
   * quadrant +x+y, -x+y, -x-y, and +x-y. The global frame is defined as the axis X going to the left, 
   * the Y axis is forward, and the Z axis goes up.
   * You can modify the dimensions of the force platform, or directly its origin and the corners using the
   * methods SetDimensions() or SetGeometry().
   * Finally, the information on the sample frequency is automatically determined by using the two first time samples.
   * The inverse of the difference of these timestamps will give the sample frequency.
   *
   * @ingroup BTKIO
   */
  
  /**
   * @typedef AMTIForcePlatformFileIO::Pointer
   * Smart pointer associated with a AMTIForcePlatformFileIO object.
   */
  
  /**
   * @typedef AMTIForcePlatformFileIO::ConstPointer
   * Smart pointer associated with a const AMTIForcePlatformFileIO object.
   */
  
  /**
   * @fn static AMTIForcePlatformFileIO::Pointer AMTIForcePlatformFileIO::New()
   * Create a AMTIForcePlatformFileIO object an return it as a smart pointer.
   */
  
  /**
   * Checks if the first line contains 7 numbers.
   */
  bool AMTIForcePlatformFileIO::CanReadFile(const std::string& filename)
  {
    bool isReadable = true;
    std::ifstream ifs(filename.c_str());
    std::string line;
    std::getline(ifs, line);
    int num = 0;
    try
    {
      std::stringstream ss(line);
      std::string val;
      while (std::getline(ss, val, ','))
      {
        FromString<double>(val); // Convert the string into double and check the content of the double
        ++num;
      }
    }
    catch(...)
    {
      isReadable = false;
    }
    if (num != 7)
      isReadable = false;
    ifs.close();
    return isReadable;
  };

  /**
   * Read the file designated by @a filename and fill @a output.
   */
  void AMTIForcePlatformFileIO::Read(const std::string& filename, Acquisition::Pointer output)
  {
    output->Reset();
    // Open the stream
    std::ifstream ifs;
    // ifs.exceptions(std::ios_base::eofbit | std::ios_base::failbit | std::ios_base::badbit);
    try
    {
      ifs.open(filename.c_str());
      // Check if the data format is valid: lines containing 7 columns, separeted by comma.
      double d1[7] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, d2[7]= {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
      this->ExtractLineData(&ifs, d1);
      this->ExtractLineData(&ifs, d2); // Need at least two samples to calculate the sample frequency.
      // Count the number of lines
      ifs.seekg(0, std::ios::beg);
      int numFrames = 0;
      std::string line;
      while (!(std::getline(ifs, line).eof()))
        ++numFrames;
      // Set the output
      output->Init(0, numFrames, 6);
      output->SetPointFrequency(1.0 / (d2[0] - d1[0]));
      output->SetFirstFrame(static_cast<int>(d1[0] * output->GetPointFrequency()) + 1);
      output->SetPointUnit(Point::Marker, "m");
      output->SetPointUnit(Point::Moment, "Nm");
      // - Set the corners and the origin
      std::vector<float> corners(12);
      std::vector<float> origin(3);
      if (this->m_UseDimensions)
      {
        // Compute the geometry
        this->computeGeometryFromDimensions(this->m_Dimensions, corners, origin);
      }
      else
      {
        // Otherwise, use the given geometry
        corners = this->m_Corners;
        origin = this->m_Origin;
      }
        // Convert the unit of the geometry to be set in meters
      for (size_t i = 0 ; i < corners.size() ; ++i)
        corners[i] /= 1000.0f;
      for (size_t i = 0 ; i < origin.size() ; ++i)
        origin[i] /= 1000.0f;
      // - Create the metadata FORCE_PLATFORM
      btk::MetaData::Pointer forcePlatform = btk::MetaData::New("FORCE_PLATFORM");
      output->GetMetaData()->AppendChild(forcePlatform);
      //   - FORCE_PLATFORM:USED
      forcePlatform->AppendChild(btk::MetaData::New("USED", static_cast<int16_t>(1)));
      //   - FORCE_PLATFORM:TYPE
      forcePlatform->AppendChild(btk::MetaData::New("TYPE", std::vector<int16_t>(1,2)));
      //   - FORCE_PLATFORM:ZERO
      std::vector<int16_t> zero(2,0); zero[0] = 1;
      forcePlatform->AppendChild(btk::MetaData::New("ZERO", zero));
      //   - FORCE_PLATFORM:CORNERS
      std::vector<uint8_t> dims(3); dims[0] = 3; dims[1] = 4; dims[2] = 1;
      forcePlatform->AppendChild(btk::MetaData::New("CORNERS", dims, corners));
      //   - FORCE_PLATFORM:ORIGIN
      dims.resize(2); dims[0] = 3; dims[1] = 1;
      forcePlatform->AppendChild(btk::MetaData::New("ORIGIN", dims, origin));
      //   - FORCE_PLATFORM:CHANNEL
      dims.resize(2); dims[0] = 6; dims[1] = 1;
      std::vector<int16_t> channel(6);
      for (int i = 0 ; i < 6 ; ++i)
        channel[i] = i+1;
      forcePlatform->AppendChild(btk::MetaData::New("CHANNEL", dims, channel));
      //   - FORCE_PLATFORM:CAL_MATRIX
      dims.resize(3); dims[0] = 6; dims[1] = 6; dims[2] = 0;
      forcePlatform->AppendChild(btk::MetaData::New("CAL_MATRIX", dims, std::vector<float>()));
      // - Set the analogs' unit
      Analog::Pointer Fx = output->GetAnalog(0);
      Fx->SetLabel("Fx");
      Fx->SetUnit("N");
      Analog::Pointer Fy = output->GetAnalog(1);
      Fy->SetLabel("Fy");
      Fy->SetUnit("N");
      Analog::Pointer Fz = output->GetAnalog(2);
      Fz->SetLabel("Fz");
      Fz->SetUnit("N");
      Analog::Pointer Mx = output->GetAnalog(3);
      Mx->SetLabel("Mx");
      Mx->SetUnit("Nm");
      Analog::Pointer My = output->GetAnalog(4);
      My->SetLabel("My");
      My->SetUnit("Nm");
      Analog::Pointer Mz = output->GetAnalog(5);
      Mz->SetLabel("Mz");
      Mz->SetUnit("Nm");
      // Extract the data
      ifs.clear(); // Reset the EOF flag
      ifs.seekg(0, std::ios::beg);
      
      for (int i = 0 ; i < numFrames ; ++i)
      {
        double d[7];
        this->ExtractLineData(&ifs, d);
        // The use of the minus sign is important as we want the reaction measured by the force platform.
        Fx->GetValues().coeffRef(i) = -d[1];
        Fy->GetValues().coeffRef(i) = -d[2];
        Fz->GetValues().coeffRef(i) = -d[3];
        Mx->GetValues().coeffRef(i) = -d[4];
        My->GetValues().coeffRef(i) = -d[5];
        Mz->GetValues().coeffRef(i) = -d[6];
      }
    }
    catch (std::fstream::failure& )
    {
      std::string excmsg; 
      if (!ifs.is_open())
        excmsg = "Invalid file path.";
      else if (ifs.eof())
        excmsg = "Unexpected end of file.";
      else if(ifs.bad())
        excmsg = "Loss of integrity of the filestream.";
      else if(ifs.fail())
        excmsg = "Internal logic operation error on the stream associated with the file.";
      else
        excmsg = "Unknown error associated with the filestream.";
      
      if (ifs.is_open()) ifs.close();    
      throw(AMTIForcePlatformFileIOException(excmsg));
    }
    catch (AMTIForcePlatformFileIOException& )
    {
      if (ifs.is_open()) ifs.close(); 
      throw;
    }
    catch (std::exception& e)
    {
      if (ifs.is_open()) ifs.close(); 
      throw(AMTIForcePlatformFileIOException("Unexpected exception occurred: " + std::string(e.what())));
    }
    catch(...)
    {
      if (ifs.is_open()) ifs.close(); 
      throw(AMTIForcePlatformFileIOException("Unknown exception"));
    }
  };
  
  /**
   * @fn const std::vector<float>& AMTIForcePlatformFileIO::GetDimensions() const
   * Returns the dimensions of the force platform.
   */

  /**
   * Set the dimensions of the force platform which will be used for the next readings.
   * @warning The dimensions must be given in millimeters!
   */  
  void AMTIForcePlatformFileIO::SetDimensions(float width, float length, float height)
  {
    this->m_Dimensions[0] = width;
    this->m_Dimensions[1] = length;
    this->m_Dimensions[2] = height;
    this->m_UseDimensions = true;
  };
  
  /**
   * @fn void const std::vector<float>& AMTIForcePlatformFileIO::GetCorners() const
   * Returns the position of the corners as a vector of 12x1 elements. The first three elements represent the coordinate of the first corner, etc.
   */
  
  /**
   * @fn const std::vector<float>& AMTIForcePlatformFileIO::GetOrigin() const
   * Returns the coordinates of the origin of the force platform.
   */
  
  /**
   * Set the geometry (corners and origin) of the force platform which will be used for the next readings.
   * @warning The geometry must be given in millimeters!
   * @warning This method is an advanced method. All the data must be expressed in the global frame. The order of the corners is very important.
   */
  void AMTIForcePlatformFileIO::SetGeometry(const std::vector<float>& corners, const std::vector<float>& origin)
  {
    if (corners.size() != 12)
    {
      btkErrorMacro("The vector containing the corners' coordinates must have a size of 12 elements");
      return;
    }
    else if (origin.size() != 3)
    {
      btkErrorMacro("The vector containing the origin's coordinates must have a size of 3 elements");
      return;
    }
    this->m_Corners = corners;
    this->m_Origin = origin;
    this->m_UseDimensions = false;
  };
  
  /**
   * Constructor.
   */
  AMTIForcePlatformFileIO::AMTIForcePlatformFileIO()
  : AcquisitionFileIO(), m_Dimensions(3), m_Corners(12), m_Origin(3)
  {
    // Members
    this->m_UseDimensions = true;
    //  All the dimensions and computed geometry are based on the specifications of the AMTI force platform OR6-7-*
    // - Dimensions
    this->m_Dimensions[0] = 463.6f; // width
    this->m_Dimensions[1] = 508.0f; // length
    this->m_Dimensions[2] = 82.6f;  // height
    // - Geometry
    this->computeGeometryFromDimensions(this->m_Dimensions, this->m_Corners, this->m_Origin);
    
    this->SetFileType(AcquisitionFileIO::ASCII);
  };
  
  inline void AMTIForcePlatformFileIO::ExtractLineData(std::ifstream* pifs, double* d)
  {
    std::string line;
    std::getline(*pifs, line);
    line = line.erase(line.find_last_not_of('\r') + 1); // Erase a possible carriage return
    std::istringstream iss(line);
    std::string val;
    for (int i = 0 ; i < 7 ; ++i)
    {
      std::getline(iss, val, ',');
      d[i] = FromString<double>(val);
    }
  };
  
  inline void AMTIForcePlatformFileIO::computeGeometryFromDimensions(const std::vector<float>& d, std::vector<float>& c, std::vector<float>& o)
  {
    float cx = d[0] / 2.0f;
    float cy = d[1] / 2.0f;
    // Corners expressed in the global frame.
    // The global frame is set here as: axis X going forward, axis Y on the left and axis Z going upward.
    // The corners are set to have the corner #1 on the bottom left side, #2 on the top left side, #3 on the top right side and #4 on the bottom right side.
    c[0] = cx;
    c[1] = -cy;
    c[2] = 0.0f;
    c[3] = -cx;
    c[4] = -cy;
    c[5] = 0.0f;
    c[6] = -cx;
    c[7] = cy;
    c[8] = 0.0f;
    c[9] = cx;
    c[10] = cy;
    c[11] = 0.0f;
    // - Origin (expressed in the global frame) and centered above the origin of the global frame
    o[0] = 0.0f;
    o[1] = 0.0f;
    o[2] = d[2] / -2.0f;
  };
};

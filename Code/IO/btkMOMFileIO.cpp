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

#include "btkMOMFileIO.h"
#include "btkEliteFileIOUtils_p.h"

#include <algorithm>
#include <cctype>

namespace btk
{
  /**
   * @class MOMFileIOException btkMOMFileIO.h
   * @brief Exception class for the MOMFileIO class.
   */
  
  /**
   * @fn MOMFileIOException::MOMFileIOException(const std::string& msg)
   * Constructor.
   */
  
  /**
   * @fn virtual MOMFileIOException::~MOMFileIOException()
   * Empty destructor.
   */
  
  /**
   * @class MOMFileIO btkMOMFileIO.h
   * @brief Interface to read/write MOM files.
   *
   * The MOM file format is used in the Elite acquisition system (BTS, Milan, Italy).
   * It contains moments for the lower limbs. Moreover, the unit of the moments in this
   * file format is Nm and is converted in Nmm to be coherent (trajectories in millimeters. 
   * Moment's force platform in Nmm). 
   * The data are reordered to fit into a btk::Acquisition object. It means, that the data 
   * are stored by joint instead by plane.
   *
   * @ingroup BTKIO
   */
  
  /**
   * @typedef MOMFileIO::Pointer
   * Smart pointer associated with a MOMFileIO object.
   */
  
  /**
   * @typedef MOMFileIO::ConstPointer
   * Smart pointer associated with a const MOMFileIO object.
   */
  
  /**
   * @fn static MOMFileIO::Pointer MOMFileIO::New()
   * Create a MOMFileIO object an return it as a smart pointer.
   */
  
  /**
   * Only check if the file extension correspond to MOM.
   */
  bool MOMFileIO::CanReadFile(const std::string& filename)
  {
    std::string lowercase = filename;
    std::transform(lowercase.begin(), lowercase.end(), lowercase.begin(), tolower);
    std::string::size_type MOMPos = lowercase.rfind(".mom");
    if ((MOMPos != std::string::npos) && (MOMPos == lowercase.length() - 4))
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
  void MOMFileIO::Read(const std::string& filename, Acquisition::Pointer output)
  {
    output->Reset();
    IEEELittleEndianBinaryFileStream bifs;
    bifs.SetExceptions(BinaryFileStream::EndFileBit | BinaryFileStream::FailBit | BinaryFileStream::BadBit);
    try
    {
      bifs.Open(filename, BinaryFileStream::In);
      ReadEliteHeader_p(output, &bifs);
      
      // Frontal (Hip, Knee, Ankle)     \    Hip (F,T,S)
      // Transverse (Hip, Knee, Ankle)  |--> Knee (F,T,S)
      // Sagital (Hip, Knee, Ankle)    /     Ankle (F,T,S)

      // Moments are renamed <side><joint>Moment
      std::vector<Point::Pointer> data = std::vector<Point::Pointer>(6);
      Acquisition::PointIterator it = output->BeginPoint();
      data[0] = *it; data[0]->SetLabel("RHipMoment"); data[0]->SetType(Point::Moment); ++it;
      data[1] = *it; data[1]->SetLabel("RKneeMoment"); data[1]->SetType(Point::Moment); ++it;
      data[2] = *it; data[2]->SetLabel("RAnkleMoment"); data[2]->SetType(Point::Moment); ++it;
      data[3] = *it; data[3]->SetLabel("LHipMoment"); data[3]->SetType(Point::Moment); ++it;
      data[4] = *it; data[4]->SetLabel("LKneeMoment"); data[4]->SetType(Point::Moment); ++it;
      data[5] = *it; data[5]->SetLabel("LAnkleMoment"); data[5]->SetType(Point::Moment);
      int inc = 0, side = 0, axis = 0;
      while (inc < output->GetPointFrameNumber())
      {
        data[0 + side * 3]->GetValues().coeffRef(inc, axis) = bifs.ReadFloat() * 1000.0; // Conversion in Nmm
        data[1 + side * 3]->GetValues().coeffRef(inc, axis) = bifs.ReadFloat() * 1000.0;
        data[2 + side * 3]->GetValues().coeffRef(inc, axis) = bifs.ReadFloat() * 1000.0;
        bifs.ReadFloat(); // ...

        ++side;
        if (side >= 2)
        {
          side = 0;
          ++axis;
          if (axis >= 3)
          {
            for (int i = 0 ; i < 6 ; ++i)
            {
              // Values -9999 and 9999
              if ((fabs(fabs(data[i]->GetValues().coeff(inc,0)) - 9999000.0) < std::numeric_limits<double>::epsilon())
                  && (fabs(fabs(data[i]->GetValues().coeff(inc,1)) - 9999000.0) < std::numeric_limits<double>::epsilon())
                  && (fabs(fabs(data[i]->GetValues().coeff(inc,2)) - 9999000.0) < std::numeric_limits<double>::epsilon()))
              {
                data[i]->GetValues().coeffRef(inc, 0) = 0.0;
                data[i]->GetValues().coeffRef(inc, 1) = 0.0;
                data[i]->GetValues().coeffRef(inc, 2) = 0.0;
                data[i]->GetResiduals().coeffRef(inc) = -1.0; // Invalid data
              }
            }
            axis = 0;
            ++inc;
          }
        }
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
      throw(MOMFileIOException(excmsg));
    }
    catch (MOMFileIOException& )
    {
      if (bifs.IsOpen()) bifs.Close(); 
      throw;
    }
    catch (std::exception& e)
    {
      if (bifs.IsOpen()) bifs.Close(); 
      throw(MOMFileIOException("Unexpected exception occurred: " + std::string(e.what())));
    }
    catch(...)
    {
      if (bifs.IsOpen()) bifs.Close(); 
      throw(MOMFileIOException("Unknown exception"));
    }
  };
  
  /**
   * Constructor.
   */
  MOMFileIO::MOMFileIO()
  : AcquisitionFileIO(AcquisitionFileIO::Binary, AcquisitionFileIO::IEEE_LittleEndian, AcquisitionFileIO::Float)
  {};
};

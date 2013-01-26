/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2013, Arnaud Barré
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

#include "btkPWRFileIO.h"
#include "btkEliteFileIOUtils_p.h"

#include <algorithm>
#include <cctype>

namespace btk
{
  /**
   * @class PWRFileIOException btkPWRFileIO.h
   * @brief Exception class for the PWRFileIO class.
   */
  
  /**
   * @fn PWRFileIOException::PWRFileIOException(const std::string& msg)
   * Constructor.
   */
  
  /**
   * @fn virtual PWRFileIOException::~PWRFileIOException()
   * Empty destructor.
   */
  
  /**
   * @class PWRFileIO btkPWRFileIO.h
   * @brief Interface to read/write PWR files.
   *
   * The PWR file format is used in the Elite acquisition system (BTS, Milan, Italy).
   * It contains power of each joint of the lower limbs (stored on the component X).
   *
   * @ingroup BTKIO
   */
  
  /**
   * @typedef PWRFileIO::Pointer
   * Smart pointer associated with a PWRFileIO object.
   */
  
  /**
   * @typedef PWRFileIO::ConstPointer
   * Smart pointer associated with a const PWRFileIO object.
   */
  
  /**
   * @fn static PWRFileIO::Pointer PWRFileIO::New()
   * Create a PWRFileIO object an return it as a smart pointer.
   */
  
  /**
   * Only check if the file extension correspond to PWR.
   */
  bool PWRFileIO::CanReadFile(const std::string& filename)
  {
    std::string lowercase = filename;
    std::transform(lowercase.begin(), lowercase.end(), lowercase.begin(), tolower);
    std::string::size_type PWRPos = lowercase.rfind(".pwr");
    if ((PWRPos != std::string::npos) && (PWRPos == lowercase.length() - 4))
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
  void PWRFileIO::Read(const std::string& filename, Acquisition::Pointer output)
  {
    output->Reset();
    IEEELittleEndianBinaryFileStream bifs;
    bifs.SetExceptions(BinaryFileStream::EndFileBit | BinaryFileStream::FailBit | BinaryFileStream::BadBit);
    try
    {
      bifs.Open(filename, BinaryFileStream::In);
      ReadEliteHeader_p(output, &bifs);
      
      // Force the number of points to 6
      output->SetPointNumber(6);
      
      // Powers are renamed <side><joint>Power
      std::vector<Point::Pointer> data = std::vector<Point::Pointer>(6);
      Acquisition::PointIterator it = output->BeginPoint();
      data[0] = *it; data[0]->SetLabel("RHipPower");   data[0]->SetDescription("Right Hip Power");   data[0]->SetType(Point::Power); ++it;
      data[1] = *it; data[1]->SetLabel("RKneePower");  data[1]->SetDescription("Right Knee Power");  data[1]->SetType(Point::Power); ++it;
      data[2] = *it; data[2]->SetLabel("RAnklePower"); data[2]->SetDescription("Right Ankle Power"); data[2]->SetType(Point::Power); ++it;
      data[3] = *it; data[3]->SetLabel("LHipPower");   data[3]->SetDescription("Left Hip Power");   data[3]->SetType(Point::Power); ++it;
      data[4] = *it; data[4]->SetLabel("LKneePower");  data[4]->SetDescription("Left Knee Power");  data[4]->SetType(Point::Power); ++it;
      data[5] = *it; data[5]->SetLabel("LAnklePower"); data[5]->SetDescription("Left Ankle Power"); data[5]->SetType(Point::Power);
      
      int inc = 0, joint = 0;
      while (inc < output->GetPointFrameNumber())
      {
        double foo = bifs.ReadFloat();
        if (fabs(fabs(foo) - 9999.0) < std::numeric_limits<double>::epsilon())
          data[joint]->GetResiduals().coeffRef(inc) = -1.0; // Invalid data
        else
          data[joint]->GetValues().coeffRef(inc, 0) = foo;
        
        if ((joint+1) % 3 == 0)
          bifs.SeekRead(4, BinaryFileStream::Current);
        
        ++joint;
        if (joint >= 6)
        {
          joint = 0;
          ++inc;
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
      throw(PWRFileIOException(excmsg));
    }
    catch (PWRFileIOException& )
    {
      if (bifs.IsOpen()) bifs.Close(); 
      throw;
    }
    catch (std::exception& e)
    {
      if (bifs.IsOpen()) bifs.Close(); 
      throw(PWRFileIOException("Unexpected exception occurred: " + std::string(e.what())));
    }
    catch(...)
    {
      if (bifs.IsOpen()) bifs.Close(); 
      throw(PWRFileIOException("Unknown exception"));
    }
  };
  
  /**
   * Constructor.
   */
  PWRFileIO::PWRFileIO()
  : AcquisitionFileIO(AcquisitionFileIO::Binary, AcquisitionFileIO::IEEE_LittleEndian, AcquisitionFileIO::Float)
  {};
};

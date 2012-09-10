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

#include "btkANGFileIO.h"
#include "btkEliteFileIOUtils_p.h"

#include <algorithm>
#include <cctype>

namespace btk
{
  /**
   * @class ANGFileIOException btkANGFileIO.h
   * @brief Exception class for the ANGFileIO class.
   */
  
  /**
   * @fn ANGFileIOException::ANGFileIOException(const std::string& msg)
   * Constructor.
   */
  
  /**
   * @fn virtual ANGFileIOException::~ANGFileIOException()
   * Empty destructor.
   */
  
  /**
   * @class ANGFileIO btkANGFileIO.h
   * @brief Interface to read/write ANG files.
   *
   * The ANG file format is used in the Elite acquisition system (BTS, Milan, Italy).
   *
   * It contains joints' angles and segments's angle. Compared to the stored data in the file,
   * the output acquisition will embed two more angles. The ankles angles stored in the 
   * file contain also the foot progression angles. Therefore, they are separeted.
   *
   *
   * @ingroup BTKIO
   */
  
  /**
   * @typedef ANGFileIO::Pointer
   * Smart pointer associated with a ANGFileIO object.
   */
  
  /**
   * @typedef ANGFileIO::ConstPointer
   * Smart pointer associated with a const ANGFileIO object.
   */
  
  /**
   * @fn static ANGFileIO::Pointer ANGFileIO::New()
   * Create a ANGFileIO object an return it as a smart pointer.
   */
  
  /**
   * Only check if the file extension correspond to ANG.
   */
  bool ANGFileIO::CanReadFile(const std::string& filename)
  {
    std::string lowercase = filename;
    std::transform(lowercase.begin(), lowercase.end(), lowercase.begin(), tolower);
    std::string::size_type ANGPos = lowercase.rfind(".ang");
    if ((ANGPos != std::string::npos) && (ANGPos == lowercase.length() - 4))
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
  void ANGFileIO::Read(const std::string& filename, Acquisition::Pointer output)
  {
    output->Reset();
    IEEELittleEndianBinaryFileStream bifs;
    bifs.SetExceptions(BinaryFileStream::EndFileBit | BinaryFileStream::FailBit | BinaryFileStream::BadBit);
    try
    {
      bifs.Open(filename, BinaryFileStream::In);
      ReadEliteHeader_p(output, &bifs);
      ReadEliteMarkersValues_p(output, &bifs);
      // Labels
      std::vector<AngleLabelConverter> labels = std::vector<AngleLabelConverter>(8);
      labels[0] = AngleLabelConverter("R_PELV", "RPelvisAngle", "Pelvis relative to Global/Body axes (Right)");
      labels[1] = AngleLabelConverter("R_HIP", "RHipAngle", "Right Hip Rotation");
      labels[2] = AngleLabelConverter("L_HIP", "LHipAngle", "Left Hip Rotation");
      labels[3] = AngleLabelConverter("R_KNEE", "RKneeAngle", "Right Knee Rotation");
      labels[4] = AngleLabelConverter("L_KNEE", "LKneeAngle", "Left Knee Rotation");
      labels[5] = AngleLabelConverter("R_SHOU", "RShoulderAngle", "Right Shoulder Rotation");
      labels[6] = AngleLabelConverter("L_PELV", "LPelvisAngle", "Pelvis relative to Global/Body axes (Left)");
      labels[7] = AngleLabelConverter("L_SHOU", "LShoulderAngle", "Left Shoulder Rotation");
      
      btk::Point::Pointer RFPA, LFPA;
      for (Acquisition::PointIterator it = output->BeginPoint() ; it != output->EndPoint() ; ++it)
      {
        bifs.SeekRead(10, BinaryFileStream::Current);
        std::string label = bifs.ReadString(6);
        // Remove spaces
        label = label.erase(label.find_last_not_of(' ') + 1);
        label = label.erase(0, label.find_first_not_of(' '));
        // Remove 0x00
        label = label.erase(label.find_last_not_of(static_cast<char>(0x00)) + 1);
        label = label.erase(0, label.find_first_not_of(static_cast<char>(0x00)));
        // Known case
        size_t j = 0;
        for (j = 0 ; j < labels.size() ; ++j)
        {
          if (label.compare(labels[j].current) == 0)
          {
            (*it)->SetLabel(labels[j].future);
            (*it)->SetDescription(labels[j].description);
            break;
          }
        }
        // Special *_ANKL
        if (j >= labels.size())
        {
          if (label.compare("R_ANKL") == 0)
          {
            (*it)->SetLabel("RAnkleAngle");
            (*it)->SetDescription("Right Ankle Rotation");
            RFPA = btk::Point::New("RFootProgressAngle", output->GetPointFrameNumber(), Point::Angle, "Right Foot relative to Global/Body axes");
            RFPA->GetValues().col(1) = (*it)->GetValues().col(1);
            RFPA->GetResiduals() = (*it)->GetResiduals();
            (*it)->GetValues().col(1).setZero();
            // Clean the -9999 which were not on the 3 coordinates
            // The column #0 contains only -9999
            (*it)->GetValues().col(0).setZero();
            for (int i = 0 ; i < output->GetPointFrameNumber() ; ++i)
            {
              if (((*it)->GetValues().coeff(i, 2) + 9999.0) < std::numeric_limits<double>::epsilon())
              {
                (*it)->GetValues().coeffRef(i, 2) = 0.0;
                (*it)->GetResiduals().coeffRef(i) = -1.0;
              }
              if ((RFPA->GetValues().coeff(i, 1) + 9999.0) < std::numeric_limits<double>::epsilon())
              {
                RFPA->GetValues().coeffRef(i, 1) = 0.0;
                RFPA->GetResiduals().coeffRef(i) = -1.0;
              }
            }
          }
          else if (label.compare("L_ANKL") == 0)
          {
            (*it)->SetLabel("LAnkleAngle");
            (*it)->SetDescription("Left Ankle Rotation");
            LFPA = btk::Point::New("LFootProgressAngle", output->GetPointFrameNumber(), Point::Angle, "Left Foot relative to Global/Body axes");
            LFPA->GetValues().col(1) = (*it)->GetValues().col(1);
            LFPA->GetResiduals() = (*it)->GetResiduals();
            (*it)->GetValues().col(1).setZero();
            // Clean the -9999 which were not on the 3 coordinates
            // The column #0 contains only -9999
            (*it)->GetValues().col(0).setZero();
            for (int i = 0 ; i < output->GetPointFrameNumber() ; ++i)
            {
              if (((*it)->GetValues().coeff(i, 2) + 9999.0) < std::numeric_limits<double>::epsilon())
              {
                (*it)->GetValues().coeffRef(i, 2) = 0.0;
                (*it)->GetResiduals().coeffRef(i) = -1.0;
              }
              if ((LFPA->GetValues().coeff(i, 1) + 9999.0) < std::numeric_limits<double>::epsilon())
              {
                LFPA->GetValues().coeffRef(i, 1) = 0.0;
                LFPA->GetResiduals().coeffRef(i) = -1.0;
              }
            }
          }
          // No known translation
          else
            (*it)->SetLabel(label);
        }
        (*it)->SetType(Point::Angle);
      }
      if (RFPA.get())
        output->AppendPoint(RFPA);
      if (LFPA.get())
        output->AppendPoint(LFPA);
      
    }
    catch (BinaryFileStreamFailure& )
    {
      std::string excmsg; 
      if (bifs.EndFile())
        excmsg = "Unexpected end of file.";
      else if (!bifs.IsOpen())
        excmsg = "Invalid file path.";
      else if(bifs.Bad())
        excmsg = "Loss of integrity of the filestream.";
      else if(bifs.Fail())
        excmsg = "Internal logic operation error on the stream associated with the file.";
      else
        excmsg = "Unknown error associated with the filestream.";
      
      if (bifs.IsOpen()) bifs.Close();
      throw(ANGFileIOException(excmsg));
    }
    catch (ANGFileIOException& )
    {
      if (bifs.IsOpen()) bifs.Close();
      throw;
    }
    catch (std::exception& e)
    {
      if (bifs.IsOpen()) bifs.Close();
      throw(ANGFileIOException("Unexpected exception occurred: " + std::string(e.what())));
    }
    catch(...)
    {
      if (bifs.IsOpen()) bifs.Close();
      throw(ANGFileIOException("Unknown exception"));
    }
  };
  
  /**
   * Constructor.
   */
  ANGFileIO::ANGFileIO()
  : AcquisitionFileIO(AcquisitionFileIO::Binary, AcquisitionFileIO::IEEE_LittleEndian, AcquisitionFileIO::Float)
  {};
};

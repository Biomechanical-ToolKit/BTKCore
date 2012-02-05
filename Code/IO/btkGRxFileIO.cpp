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

#include "btkGRxFileIO.h"
#include "btkBinaryFileStream.h"
#include "btkMetaDataUtils.h"
#include "btkWrench.h"

#include <fstream>
#include <algorithm>
#include <cctype>

namespace btk
{
  /**
   * @class GRxFileIOException btkGRxFileIO.h
   * @brief Exception class for the GRxFileIO class.
   */
  
  /**
   * @fn GRxFileIOException::GRxFileIOException(const std::string& msg)
   * Constructor.
   */
  
  /**
   * @fn virtual GRxFileIOException::~GRxFileIOException()
   * Empty destructor.
   */
  
  /**
   * @class GRxFileIO btkGRxFileIO.h
   * @brief Interface to read/write GRx files.
   *
   * The GRx file format is used in the Elite acquisition system (BTS, Milan, Italy).
   * The exported acquisition contain force platform data expressed in the platform axes.
   *
   * @ingroup BTKIO
   */
  
  /**
   * @typedef GRxFileIO::Pointer
   * Smart pointer associated with a GRxFileIO object.
   */
  
  /**
   * @typedef GRxFileIO::ConstPointer
   * Smart pointer associated with a const GRxFileIO object.
   */
  
  /**
   * @fn static GRxFileIO::Pointer GRxFileIO::New()
   * Create a GRxFileIO object an return it as a smart pointer.
   */
  
  /**
   * Only check if the file extension correspond to GRx.
   */
  bool GRxFileIO::CanReadFile(const std::string& filename)
  {
    std::string lowercase = filename;
    std::transform(lowercase.begin(), lowercase.end(), lowercase.begin(), tolower);
    std::string::size_type GRxPos = lowercase.substr(0,lowercase.length()-1).rfind(".gr");
    if ((GRxPos != std::string::npos) && (GRxPos == lowercase.length() - 4) && (*(lowercase.rbegin()) >= 0x31) && (*(lowercase.rbegin()) <= 0x39))
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
   * Checks if the suffix of @a filename is GRx.
   */
  bool GRxFileIO::CanWriteFile(const std::string& filename)
  {
    std::string lowercase = filename;
    std::transform(lowercase.begin(), lowercase.end(), lowercase.begin(), tolower);
    std::string::size_type GRxPos = lowercase.substr(0,lowercase.length()-1).rfind(".gr");
    if ((GRxPos != std::string::npos) && (GRxPos == lowercase.length() - 4) && (*(lowercase.rbegin()) >= 0x31) && (*(lowercase.rbegin()) <= 0x39))
      return true;
    else
      return false;
  };
  
  /**
   * Read the file designated by @a filename and fill @a output.
   */
  void GRxFileIO::Read(const std::string& filename, Acquisition::Pointer output)
  {
    output->Reset();
    IEEELittleEndianBinaryFileStream bifs;
    bifs.SetExceptions(BinaryFileStream::EndFileBit | BinaryFileStream::FailBit | BinaryFileStream::BadBit);
    try
    {
      bifs.Open(filename, BinaryFileStream::In);
      uint16_t numFra = bifs.ReadU16();
      double freq = static_cast<double>(bifs.ReadU16());
      // Some values between the bytes 0x05 and 0x49...
      // Bytes 50-51: Number of frames in the main acquisition + 1 and multiplied by 10...
      // Bytes 52-53: 0x00
      bifs.SeekRead(50, BinaryFileStream::Current);
      // Bytes 54-55: First frame 
      double ff = bifs.ReadU16();
      // Bytes 56-57: 0x00
      // Bytes 58-59: Last frame
      // Bytes 60-135 Other values ...
      bifs.SeekRead(80, BinaryFileStream::Current);
      // Bytes 136-139: Length
      double length = bifs.ReadFloat();
      // Bytes 140-143: Width
      double width = bifs.ReadFloat();
      // Bytes 144-147: Height
      double height = bifs.ReadFloat();
      // Corners 1,2,3,4: coordinates X,Y,Z
      std::vector<float> cornersData = bifs.ReadFloat(3*4);
      
      output->Init(0, numFra, 6, 1);
      output->SetFirstFrame(static_cast<int>(ceil(ff / 1000.0 * freq + 1.0)));
      output->SetPointFrequency(freq);
      // Metadata FORCE_PLATFORM
      MetaData::Pointer fp = MetaDataCreateChild(output->GetMetaData(), "FORCE_PLATFORM");
      // Metadata FORCE_PLATFORM:USED
      MetaDataCreateChild(fp, "USED", (int16_t)1);
      // Metadata FORCE_PLATFORM:ZERO
      std::vector<int16_t> zeros = std::vector<int16_t>(2,0);
      MetaDataCreateChild(fp, "ZERO", zeros);
      // Metadata FORCE_PLATFORM:TYPE
      MetaDataCreateChild(fp, "TYPE")->SetInfo(MetaDataInfo::New(std::vector<uint8_t>(1,1), std::vector<int16_t>(1,1)));
      // Metadata FORCE_PLATFORM:CORNERS
      std::vector<uint8_t> cornersDim = std::vector<uint8_t>(3, 1); cornersDim[0] = 3; cornersDim[1] = 4;
      MetaDataCreateChild(fp, "CORNERS")->SetInfo(MetaDataInfo::New(cornersDim, cornersData));
      // Metadata FORCE_PLATFORM:ORIGIN
      std::vector<uint8_t> originDim = std::vector<uint8_t>(2, 1); originDim[0] = 3;
      std::vector<float> originVal = std::vector<float>(3);
      originVal[0] = static_cast<float>(length / 2.0);
      originVal[1] = static_cast<float>(width / 2.0);
      originVal[2] = static_cast<float>(-1.0 * height / 2.0);
      MetaDataCreateChild(fp, "ORIGIN")->SetInfo(MetaDataInfo::New(originDim, originVal));
      // Metadata FORCE_PLATFORM:CHANNEL
      std::vector<uint8_t> channelDim = std::vector<uint8_t>(2, 1); channelDim[0] = 6;
      std::vector<int16_t> channelData = std::vector<int16_t>(6,0);
      channelData[0] = 1; channelData[1] = 2; channelData[2] = 3; channelData[3] = 4; channelData[4] = 5; channelData[5] = 6;
      MetaDataCreateChild(fp, "CHANNEL")->SetInfo(MetaDataInfo::New(channelDim, channelData));
      
      bifs.SeekRead(512, BinaryFileStream::Begin);
      btk::Wrench::Pointer fpw = btk::Wrench::New(numFra);
      for (int i = 0 ; i < output->GetPointFrameNumber() ; ++i)
      {
        output->GetAnalog(1)->GetValues().coeffRef(i) = bifs.ReadFloat();
        output->GetAnalog(0)->GetValues().coeffRef(i) = -1.0 * bifs.ReadFloat();
        output->GetAnalog(2)->GetValues().coeffRef(i) = -1.0 * bifs.ReadFloat();
        output->GetAnalog(4)->GetValues().coeffRef(i) = bifs.ReadFloat() - width / 2.0;
        output->GetAnalog(3)->GetValues().coeffRef(i) = -1.0 * bifs.ReadFloat() + length / 2.0;
        output->GetAnalog(5)->GetValues().coeffRef(i) = bifs.ReadFloat();
      }
      
      // Label channels
      std::string str = std::string(1, 0x00); str[0] = *(filename.rbegin());
      output->GetAnalog(0)->SetLabel("Fx" + str);
      output->GetAnalog(1)->SetLabel("Fy" + str);
      output->GetAnalog(2)->SetLabel("Fz" + str);
      output->GetAnalog(3)->SetLabel("Px" + str);
      output->GetAnalog(4)->SetLabel("Py" + str);
      output->GetAnalog(5)->SetLabel("Mz" + str);
    }
    catch (BinaryFileStreamException& )
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
      throw(GRxFileIOException(excmsg));
    }
    catch (GRxFileIOException& )
    {
      if (bifs.IsOpen()) bifs.Close(); 
      throw;
    }
    catch (std::exception& e)
    {
      if (bifs.IsOpen()) bifs.Close(); 
      throw(GRxFileIOException("Unexpected exception occurred: " + std::string(e.what())));
    }
    catch(...)
    {
      if (bifs.IsOpen()) bifs.Close(); 
      throw(GRxFileIOException("Unknown exception"));
    }
  };
  
  /**
   * Write the file designated by @a filename with the content of @a input.
   */
  void GRxFileIO::Write(const std::string& filename, Acquisition::Pointer input)
  {
    btkNotUsed(filename);
    btkNotUsed(input);
    /*
    if (input.get() == 0)
    {
      btkIOErrorMacro(filename, "Empty input. Impossible to write an empty file.");
      return;
    }
    */
    btkErrorMacro("Method not yet implemented.");
    return;
  };
  
  /**
   * Constructor.
   */
  GRxFileIO::GRxFileIO()
  : AcquisitionFileIO()
  {
    this->SetFileType(AcquisitionFileIO::Binary);
    this->SetByteOrder(AcquisitionFileIO::IEEE_LittleEndian);
    this->SetStorageFormat(AcquisitionFileIO::Float);
  };
};

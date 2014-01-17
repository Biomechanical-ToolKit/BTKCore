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

#include "btkCLBFileIO.h"
#include "btkBinaryFileStream.h"
#include "btkMetaDataUtils.h"
#include "btkLogger.h"

namespace btk
{
  /**
   * @class CLBFileIOException btkCLBFileIO.h
   * @brief Exception class for the CLBFileIO class.
   */
  
  /**
   * @fn CLBFileIOException::CLBFileIOException(const std::string& msg)
   * Constructor.
   */
  
  /**
   * @fn virtual CLBFileIOException::~CLBFileIOException()
   * Empty destructor.
   */
  
  /**
   * @class CLBFileIO btkCLBFileIO.h
   * @brief Interface to read/write CLB files.
   *
   * The CLB file format is used in the software C-Logger (Contec Inc., Spartanburg, USA) and contains only analogs data.
   *
   * Based on reverse engineering, its content is separated in a header and the data with the following informations:
   * @code
   * // Header
   *  - key: string (18 characters): 0x434F 4E54 4543 2044 4154 4120 4C4F 4747 4552 (CONTEC DATA LOGGER)
   *  - Version: integer 16-bit
   *  - Channel number: integer 16-bit
   *  - Device name length: integer 16-bit (n)
   *  - Device name: string (n characters, where n is the value extracted previously)
   *  - Resolution: integer 16-bit
   *  - Serial number length: integer 16-bit (m)
   *  - Serial number: string (m characters, where m is the value extracted previously)
   *  - Sampling clock: integer 32-bit (unit in µs)
   *  - Time integer: 8 bytes (integer 64-bit) // WHAT IS THE FORMAT?
   *  - Number of frames: integer 32-bit
   * // Data
   *  - Channel name length: integer 16-bit (o)
   *  - Channel name: string (o characters, where o is the value extracted previously)
   *  - Device channel: integer 16-bit
   *  - Sequence: integer 16-bit
   *  - Input range: integer 16-bit (code to convert to a corresponding range)
   *  - Scaling Enabled: integer 16-bit
   *  - Maximum scale value: float 32-bit
   *  - Minimum scale value: float 32-bit
   *  - ??? (Option): 32-bit
   *  - Data block (number of frames x integer 16-bit) 
   * // ... Same format for the other analog channels
   * @endcode
   *
   * @note Some informations were also found in the documentation of the manufacturer on the web (especially for the channel information. See file C-LOGGER_UsersGuide_124e.pdf). However, it seems that there is no
   * license for this documentation but only on the software. So, I don't think there is any copyright infringement.
   *
   * @ingroup BTKIO
   */
  
  /**
   * @typedef CLBFileIO::Pointer
   * Smart pointer associated with a CLBFileIO object.
   */
  
  /**
   * @typedef CLBFileIO::ConstPointer
   * Smart pointer associated with a const CLBFileIO object.
   */
  
  /**
   * @fn static CLBFileIO::Pointer CLBFileIO::New()
   * Create a CLBFileIO object an return it as a smart pointer.
   */
  
  /**
   * Check if the beginning of the file starts with the string "CONTEC DATA LOGGER".
   */
  bool CLBFileIO::CanReadFile(const std::string& filename)
  {
    bool isReadable = true;
    IEEELittleEndianBinaryFileStream bifs(filename, BinaryFileStream::In);
    if (bifs.ReadString(18).compare("CONTEC DATA LOGGER") != 0)
      isReadable = false;
    bifs.Close();
    return isReadable;
  };
  
  /**
   * Read the file designated by @a filename and fill @a output.
   */
  void CLBFileIO::Read(const std::string& filename, Acquisition::Pointer output)
  {
    output->Reset();
    IEEELittleEndianBinaryFileStream bifs;
    bifs.SetExceptions(BinaryFileStream::EndFileBit | BinaryFileStream::FailBit | BinaryFileStream::BadBit);
    try
    {
      bifs.Open(filename, BinaryFileStream::In);
      // Header
      if (bifs.ReadString(18).compare("CONTEC DATA LOGGER") != 0)
        throw CLBFileIOException("Invalid CLB file.");
      bifs.SeekRead(2, BinaryFileStream::Current); // Version
      uint16_t channelNumber = bifs.ReadU16();
      std::string deviceName = bifs.ReadString(bifs.ReadU16());
      uint16_t DACResolution = bifs.ReadU16();
      bifs.SeekRead(bifs.ReadU16(), BinaryFileStream::Current); // Serial number std::string serialNumber = bifs.ReadString(bifs.ReadU16());
      unsigned int samplingClock = bifs.ReadU32();
      bifs.SeekRead(8, BinaryFileStream::Current); // Time integer
      int frameNumber = bifs.ReadI32();
      
      if (samplingClock == 0)
        throw CLBFileIOException("Invalid sampling clock.");
      
      // Init the output
      output->Init(0,frameNumber,channelNumber);
      output->SetPointFrequency(1.0e6/static_cast<double>(samplingClock));
      double res = 0.0;
      int offset = 0;
      switch(DACResolution)
      {
      case 12:
        output->SetAnalogResolution(Acquisition::Bit12);
        res = 4096.0;
        offset = 2048;
        break;
      case 16:
        output->SetAnalogResolution(Acquisition::Bit16);
        res = 65536.0;
        offset = 32768;
        break;
      default:
        throw CLBFileIOException("Invalid resolution used for the digital-to-analog conversion");
      }
      MetaData::Pointer analog = MetaDataCreateChild(output->GetMetaData(), "ANALOG");
      MetaDataCreateChild(analog, "BOARD", deviceName);
      MetaDataCreateChild(analog, "FORMAT", "UNSIGNED");
      // Add a metadata to notify that the first frame was not set.
      MetaData::Pointer btkPointConfig = MetaDataCreateChild(output->GetMetaData(), "BTK_POINT_CONFIG");
      MetaDataCreateChild(btkPointConfig, "NO_FIRST_FRAME", static_cast<int8_t>(1));
      
      // Data
      for (Acquisition::AnalogIterator it = output->BeginAnalog() ; it != output->EndAnalog() ; ++it)
      {
        (*it)->SetLabel(bifs.ReadString(bifs.ReadU16()));
        bifs.SeekRead(4, BinaryFileStream::Current); // Device channel & Sequence
        uint16_t range = bifs.ReadU16();
        bifs.SeekRead(2, BinaryFileStream::Current); // Scaling enabled (the scaling is forced)
        double maximumScaleValue = bifs.ReadFloat();
        double minimumScaleValue = bifs.ReadFloat();
        double rangeValue = maximumScaleValue - minimumScaleValue;
        bifs.SeekRead(4, BinaryFileStream::Current); //  What is this block of bytes? Option?
        for (int i = 0 ; i < frameNumber ; ++i)
          (*it)->GetValues().coeffRef(i) = static_cast<double>(bifs.ReadU16()) * rangeValue / res + minimumScaleValue;
        switch(range)
        {
        case 0: // -10V -- 10V
          (*it)->SetScale(20.0/res);
          (*it)->SetOffset(offset);
          (*it)->SetGain(Analog::PlusMinus10);
          break;
        case 1: // -5V -- 5V
          (*it)->SetScale(10.0/res);
          (*it)->SetOffset(offset);
          (*it)->SetGain(Analog::PlusMinus5);
          break;
        case 2: // -2.5V -- 2.5V
          (*it)->SetScale(5.0/res);
          (*it)->SetOffset(offset);
          (*it)->SetGain(Analog::PlusMinus2Dot5);
          break;
        case 3: // -1.25V -- 1.25V
          (*it)->SetScale(2.5/res);
          (*it)->SetOffset(offset);
          (*it)->SetGain(Analog::PlusMinus1Dot25);
          break;
        case 4: // -1V -- 1V
          (*it)->SetScale(2.0/res);
          (*it)->SetOffset(offset);
          (*it)->SetGain(Analog::PlusMinus1);
          break;
        case 5: // -0.625V -- 0.625V
          (*it)->SetScale(1.25/res);
          (*it)->SetOffset(offset);
          btkWarningMacro(filename, (*it)->GetLabel() + ": Unsupported gain and set to unknown.");
          break;
        case 6: // -0.5V -- 0.5V
          (*it)->SetScale(1.0/res);
          (*it)->SetOffset(offset);
          (*it)->SetGain(Analog::PlusMinus0Dot5);
          break;
        case 7: // -0.3125V -- 0.3125V
          (*it)->SetScale(0.625/res);
          (*it)->SetOffset(offset);
          btkWarningMacro(filename, (*it)->GetLabel() + ": Unsupported gain and set to unknown.");
          break;
        case 8: // -0.25V -- 0.25V
          (*it)->SetScale(0.5/res);
          (*it)->SetOffset(offset);
          (*it)->SetGain(Analog::PlusMinus0Dot25);
          break;
        case 9: // -0.125V -- 0.125V
          (*it)->SetScale(0.25/res);
          (*it)->SetOffset(offset);
          btkWarningMacro(filename, (*it)->GetLabel() + ": Unsupported gain and set to unknown.");
          break;
        case 10: // -0.1V -- 0.1V
          (*it)->SetScale(0.2/res);
          (*it)->SetOffset(offset);
          (*it)->SetGain(Analog::PlusMinus0Dot1);
          break;
        case 11: // -0.05V -- 0.05V
          (*it)->SetScale(0.1/res);
          (*it)->SetOffset(offset);
          (*it)->SetGain(Analog::PlusMinus0Dot05);
          break;
        case 12: // -0.025V -- 0.025V
          (*it)->SetScale(0.05/res);
          (*it)->SetOffset(offset);
          btkWarningMacro(filename, (*it)->GetLabel() + ": Unsupported gain and set to unknown.");
          break;
        case 13: // -0.0125V -- 0.0125V
          (*it)->SetScale(0.05/res);
          (*it)->SetOffset(offset);
          btkWarningMacro(filename, (*it)->GetLabel() + ": Unsupported gain and set to unknown.");
          break;
        case 50: // 0V -- 10V
          (*it)->SetScale(10.0/res);
          (*it)->SetOffset(0);
          btkWarningMacro(filename, (*it)->GetLabel() + ": Unsupported gain and set to unknown.");
          break;
        case 51: // 0V -- 5V
          (*it)->SetScale(5.0/res);
          (*it)->SetOffset(0);
          btkWarningMacro(filename, (*it)->GetLabel() + ": Unsupported gain and set to unknown.");
          break;
        case 52: // 0V -- 4.095V
          (*it)->SetScale(4.095/res);
          (*it)->SetOffset(0);
          btkWarningMacro(filename, (*it)->GetLabel() + ": Unsupported gain and set to unknown.");
          break;
        case 53: // 0V -- 2.5V
          (*it)->SetScale(2.5/res);
          (*it)->SetOffset(0);
          btkWarningMacro(filename, (*it)->GetLabel() + ": Unsupported gain and set to unknown.");
          break;
        case 54: // 0V -- 1.25V
          (*it)->SetScale(1.25/res);
          (*it)->SetOffset(0);
          btkWarningMacro(filename, (*it)->GetLabel() + ": Unsupported gain and set to unknown.");
          break;
        case 55: // 0V -- 1V
          (*it)->SetScale(1.0/res);
          (*it)->SetOffset(0);
          btkWarningMacro(filename, (*it)->GetLabel() + ": Unsupported gain and set to unknown.");
          break;
        case 56: // 0V -- 0.5V
          (*it)->SetScale(0.5/res);
          (*it)->SetOffset(0);
          btkWarningMacro(filename, (*it)->GetLabel() + ": Unsupported gain and set to unknown.");
          break;
        case 57: // 0V -- 0.25V
          (*it)->SetScale(0.25/res);
          (*it)->SetOffset(0);
          btkWarningMacro(filename, (*it)->GetLabel() + ": Unsupported gain and set to unknown.");
          break;
        case 58: // 0V -- 0.1V
          (*it)->SetScale(0.1/res);
          (*it)->SetOffset(0);
          btkWarningMacro(filename, (*it)->GetLabel() + ": Unsupported gain and set to unknown.");
          break;
        case 59: // 0V -- 0.05V
          (*it)->SetScale(0.05/res);
          (*it)->SetOffset(0);
          btkWarningMacro(filename, (*it)->GetLabel() + ": Unsupported gain and set to unknown.");
          break;
        case 60: // 0V -- 0.025V
          (*it)->SetScale(0.025/res);
          (*it)->SetOffset(0);
          btkWarningMacro(filename, (*it)->GetLabel() + ": Unsupported gain and set to unknown.");
          break;
        case 61: // 0V -- 0.0125V
          (*it)->SetScale(0.0125/res);
          (*it)->SetOffset(0);
          btkWarningMacro(filename, (*it)->GetLabel() + ": Unsupported gain and set to unknown.");
          break;
        case 100: // 0V -- 20mA
          (*it)->SetScale(20.0/res);
          (*it)->SetOffset(0);
          (*it)->SetUnit("mA");
          btkWarningMacro(filename, (*it)->GetLabel() + ": Unsupported gain and set to unknown.");
          break;
        case 101: // 4mA -- 20mA
          (*it)->SetScale(16.0/res);
          (*it)->SetOffset(-1*static_cast<int>(res)/16*4);
          (*it)->SetUnit("mA");
          btkWarningMacro(filename, (*it)->GetLabel() + ": Unsupported gain and set to unknown.");
          break;
        case 150: // 1V -- 5V
          (*it)->SetScale(4.0/res);
          (*it)->SetOffset(-1*static_cast<int>(res)/4*1);
          btkWarningMacro(filename, (*it)->GetLabel() + ": Unsupported gain and set to unknown.");
          break;
        default:
          throw CLBFileIOException("Unknown range code. Please contact the developers for more informations.");
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
      throw(CLBFileIOException(excmsg));
    }
    catch (CLBFileIOException& )
    {
      if (bifs.IsOpen()) bifs.Close();
      throw;
    }
    catch (std::exception& e)
    {
      if (bifs.IsOpen()) bifs.Close();
      throw(CLBFileIOException("Unexpected exception occurred: " + std::string(e.what())));
    }
    catch(...)
    {
      if (bifs.IsOpen()) bifs.Close();
      throw(CLBFileIOException("Unknown exception"));
    }
  };
  
  /**
   * Constructor.
   */
  CLBFileIO::CLBFileIO()
  : AcquisitionFileIO(AcquisitionFileIO::Binary, AcquisitionFileIO::IEEE_LittleEndian, AcquisitionFileIO::Integer)
  {};
};

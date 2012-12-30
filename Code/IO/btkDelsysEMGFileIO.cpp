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

#include "btkDelsysEMGFileIO.h"
#include "btkBinaryFileStream.h"
#include "btkMetaDataUtils.h"

namespace btk
{
  /**
   * @class DelsysEMGFileIOException btkDelsysEMGFileIO.h
   * @brief Exception class for the DelsysEMGFileIO class.
   */
  
  /**
   * @fn DelsysEMGFileIOException::DelsysEMGFileIOException(const std::string& msg)
   * Constructor.
   */
  
  /**
   * @fn virtual DelsysEMGFileIOException::~DelsysEMGFileIOException()
   * Empty destructor.
   */
  
  /**
   * @class DelsysEMGFileIO btkDelsysEMGFileIO.h
   * @brief Interface to read DelsysEMG files.
   *
   * The DelsysEMG file format is used in Delsys systems and saved by the software EMGWorks.
   * All the known file format (1-4) are implemented.
   * The exported acquisition contains raw EMG signals.
   *
   * This parser is based on the source code of the Delsys company which give us the rights to use it. The implementation is an adaption of the Matlab file 'loademg3.m'.
   *
   * @ingroup BTKIO
   */
  
  /**
   * @typedef DelsysEMGFileIO::Pointer
   * Smart pointer associated with a DelsysEMGFileIO object.
   */
  
  /**
   * @typedef DelsysEMGFileIO::ConstPointer
   * Smart pointer associated with a const DelsysEMGFileIO object.
   */
  
  /**
   * @fn static DelsysEMGFileIO::Pointer DelsysEMGFileIO::New()
   * Create a DelsysEMGFileIO object an return it as a smart pointer.
   */
  
  /**
   * Only check if the file extension correspond to DelsysEMG.
   */
  bool DelsysEMGFileIO::CanReadFile(const std::string& filename)
  {
    bool isReadable = true;
    NativeBinaryFileStream ifs(filename, BinaryFileStream::In);
    if (ifs.ReadString(4).compare("DEMG") != 0)
      isReadable = false;
    ifs.Close();
    return isReadable;
  };
  
  /**
   * Read the file designated by @a filename and fill @a output.
   */
  void DelsysEMGFileIO::Read(const std::string& filename, Acquisition::Pointer output)
  {
    output->Reset();
    IEEELittleEndianBinaryFileStream bifs;
    bifs.SetExceptions(BinaryFileStream::EndFileBit | BinaryFileStream::FailBit | BinaryFileStream::BadBit);
    try
    {
      bifs.Open(filename, BinaryFileStream::In);
      if (bifs.ReadString(4).compare("DEMG") != 0)
        throw(DelsysEMGFileIOException("Wrong header key. It is not a proper Delsys EMG file."));
      this->m_Version = bifs.ReadU16();
      
      MetaData::Pointer manufacturer = MetaDataCreateChild(output->GetMetaData(), "MANUFACTURER");
      
      // Old generation of file
      if ((this->m_Version == 1) || (this->m_Version == 2))
      {
        MetaDataCreateChild(manufacturer, "COMPANY", "Delsys Inc.");
        MetaDataCreateChild(manufacturer, "SOFTWARE", "EMGWorks");
        MetaDataCreateChild(manufacturer, "VERSION", "0.0.0");
        int numberOfChannels = bifs.ReadU16();
        double sampleFrequency = static_cast<double>(bifs.ReadU32());
        int numberOfSamples = bifs.ReadU32();
        // Init the output acquisition
        output->Init(0, numberOfSamples, numberOfChannels);
        output->SetPointFrequency(sampleFrequency);
        int bits = bifs.ReadU8();
        switch(bits)
        {
          case 8:
            output->SetAnalogResolution(Acquisition::Bit8);
            break;
          case 10:
            output->SetAnalogResolution(Acquisition::Bit10);
            break;
          case 12:
            output->SetAnalogResolution(Acquisition::Bit12);
            break;
          case 14:
            output->SetAnalogResolution(Acquisition::Bit14);
            break;
          case 16:
            output->SetAnalogResolution(Acquisition::Bit16);
            break;
          default:
            btkIOErrorMacro(filename, "Unknown analog resolution. Set by default to 12.");
            bits = 12;
            output->SetAnalogResolution(Acquisition::Bit12);
            break;
        }
        int vMin = bifs.ReadI8();
        int vMax = bifs.ReadI8();
        double scale = (vMin - vMax) / pow(2.0, bits);
        if (this->m_Version == 1)
        {
          this->SetStorageFormat(AcquisitionFileIO::Float);
          for (int i = 0 ; i < numberOfSamples ; ++i)
          {
            for (Acquisition::AnalogIterator it = output->BeginAnalog() ; it != output->EndAnalog() ; ++it)
              (*it)->GetValues().coeffRef(i) = bifs.ReadFloat();
          }
          // Based on the format for the version 2 of this file format, the scaling value, offset and gain are assumed.
          for (Acquisition::AnalogIterator it = output->BeginAnalog() ; it != output->EndAnalog() ; ++it)
          {
            (*it)->SetScale(scale);
            (*it)->SetOffset(5); // 5V
            (*it)->SetGain(Analog::PlusMinus10);
          }
        }
        else
        {
          this->SetStorageFormat(AcquisitionFileIO::Integer);
          for (int i = 0 ; i < numberOfSamples ; ++i)
          {
            for (Acquisition::AnalogIterator it = output->BeginAnalog() ; it != output->EndAnalog() ; ++it)
              (*it)->GetValues().coeffRef(i) = (static_cast<double>(bifs.ReadI16()) * scale - 5.0) / 1000.0;
          }
          // Set the analog channel infos
          for (Acquisition::AnalogIterator it = output->BeginAnalog() ; it != output->EndAnalog() ; ++it)
          {
            (*it)->SetScale(scale);
            (*it)->SetOffset(5); // 5V
            (*it)->SetGain(Analog::PlusMinus10); // NOTE: Only assumed from the offset value.
          }
        }
      }
      // New generation
      else if ((this->m_Version == 3) || (this->m_Version == 4))
      {
        // WARNING: The ADC resolution is not stored for these versions. It is assumed that it is a 16-bit ADC card.
        output->SetAnalogResolution(Acquisition::Bit16);
        
        this->SetStorageFormat(AcquisitionFileIO::Float);
        bifs.SeekRead(4, BinaryFileStream::Current); // File type
        // Header
        MetaDataCreateChild(manufacturer, "COMPANY", "Delsys Inc.");
        MetaDataCreateChild(manufacturer, "SOFTWARE", "EMGWorks");
        MetaDataCreateChild(manufacturer, "VERSION", bifs.ReadString(11));
        // Shift of 274 bytes (informations not used by BTK):
        // - DADLL version: string written on 11 characters
        // - Card Manufacturer: string written on 255 characters
        // - Card ID: 32-bit unsigned integer
        // - NI E-series used: 32-bit unsigned integer
        bifs.SeekRead(274, BinaryFileStream::Current);
        int numberOfChannels = bifs.ReadI32(); // Should be an unsigned integer (uint32_t) but BTK implemented the number of channel as an integer
        int numberOfSamples = bifs.ReadI32(); // Should be an unsigned integer (uint32_t) but BTK implemented the number of samples as an integer
        // Shift of 4 bytes (informations not used in BTK):
        // - Samples desired: 32-bit unsigned integer
        bifs.SeekRead(4, BinaryFileStream::Current);
        double sampleFrequency = bifs.ReadDouble();
        // Shift of 24 bytes (informations not used in BTK):
        // - Sample frequency desired: double
        // - Data start: 32-bit unsigned integer (not used as the data are directly attached to the header)
        // - Start Trigger mode: 32-bit unsigned integer
        // - Stop Trigger mode: 32-bit unsigned integer
        // - Single ended Daq mode: 32-bit unsigned integer
        // - Bipolar ended Daq mode: 32-bit unsigned integer
        bifs.SeekRead(28, BinaryFileStream::Current);
        bool cameraUsed = bifs.ReadU32() == 1;
        std::string videoFilename = bifs.ReadString(bifs.ReadU32());
        if (cameraUsed)
        {
          MetaData::Pointer point = MetaDataCreateChild(output->GetMetaData(), "POINT");
          btk::MetaDataCreateChild(point, "MOVIE_FILENAME", std::vector<std::string>(1,videoFilename));
          btk::MetaDataCreateChild(point, "MOVIE_DELAY", std::vector<float>(1,0.0f));
        }
        // Init the output acquisition
        output->Init(0, numberOfSamples, numberOfChannels);
        output->SetPointFrequency(sampleFrequency);
        // Channel info
        std::vector<double> resolutions(numberOfChannels);
        std::vector<double> offsets(numberOfChannels);
        std::vector<double> gains(numberOfChannels);
        int inc = 0;
        for (Acquisition::AnalogIterator it = output->BeginAnalog() ; it != output->EndAnalog() ; ++it)
        {
          bifs.SeekRead(4, BinaryFileStream::Current); // ID
          (*it)->SetUnit(bifs.ReadString(bifs.ReadU32()));
          (*it)->SetLabel(bifs.ReadString(bifs.ReadU32()));
          gains[inc] = bifs.ReadDouble(); // System gain
          int gain = static_cast<int>(gains[inc]);
          switch(gain)
          {
          case Analog::PlusMinus10:
          case Analog::PlusMinus5:
          case Analog::PlusMinus2Dot5:
          case Analog::PlusMinus1Dot25:
          case Analog::PlusMinus1:
          case Analog::PlusMinus0Dot5:  
          case Analog::PlusMinus0Dot25:
          case Analog::PlusMinus0Dot1:
          case Analog::PlusMinus0Dot05:
            (*it)->SetGain(static_cast<Analog::Gain>(gain));
            break;
          default:
            btkIOErrorMacro(filename, "Unknown gain. Contact the developer to add it in the list. Replaced by a gain of +/- 10 volts.");
            (*it)->SetGain(Analog::PlusMinus10);
            break;
          }
          bifs.SeekRead(8, BinaryFileStream::Current); // AD gain
          resolutions[inc] = bifs.ReadDouble();
          offsets[inc] = bifs.ReadDouble();
          bifs.SeekRead(16, BinaryFileStream::Current); //  High Pass & Low Pass cutoff frequency
          if (this->m_Version == 4)
          {
            // Sample frequency: double
            // Start offset: float32
            bifs.SeekRead(12, BinaryFileStream::Current);
          }
          (*it)->SetScale(resolutions[inc] / gains[inc]);
          (*it)->SetOffset(static_cast<int>(offsets[inc] / gains[inc]));
          ++inc;
        }
        // Data
        for (int i = 0 ; i < numberOfSamples ; ++i)
        {
          inc = 0;
          for (Acquisition::AnalogIterator it = output->BeginAnalog() ; it != output->EndAnalog() ; ++it)
          {
            (*it)->GetValues().coeffRef(i) = (static_cast<double>(bifs.ReadI16()) * resolutions[inc] - offsets[inc]) / gains[inc];
            ++inc;
          }
        }
      }
      else
      {
        throw(DelsysEMGFileIOException("Unknown file version."));
      }
      // Add a metadata to notify that the first frame was not set.
      MetaData::Pointer btkPointConfig = MetaDataCreateChild(output->GetMetaData(), "BTK_POINT_CONFIG");
      MetaDataCreateChild(btkPointConfig, "NO_FIRST_FRAME", static_cast<int8_t>(1));
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
      throw(DelsysEMGFileIOException(excmsg));
    }
    catch (DelsysEMGFileIOException& )
    {
      if (bifs.IsOpen()) bifs.Close(); 
      throw;
    }
    catch (std::exception& e)
    {
      if (bifs.IsOpen()) bifs.Close(); 
      throw(DelsysEMGFileIOException("Unexpected exception occurred: " + std::string(e.what())));
    }
    catch(...)
    {
      if (bifs.IsOpen()) bifs.Close(); 
      throw(DelsysEMGFileIOException("Unknown exception"));
    }
  };
  
  /**
   * @fn int DelsysEMGFileIO::GetFileVersion() const 
   * Returns the file format version of the read file.
   */
  
  /**
   * Constructor.
   */
  DelsysEMGFileIO::DelsysEMGFileIO()
  : AcquisitionFileIO(AcquisitionFileIO::Binary, AcquisitionFileIO::IEEE_LittleEndian)
  {};
};

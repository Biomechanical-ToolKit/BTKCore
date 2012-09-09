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

#include "btkEMxFileIO.h"
#include "btkBinaryFileStream.h"

#include <algorithm>
#include <cctype>

namespace btk
{
  /**
   * @class EMxFileIOException btkEMxFileIO.h
   * @brief Exception class for the EMxFileIO class.
   */
  
  /**
   * @fn EMxFileIOException::EMxFileIOException(const std::string& msg)
   * Constructor.
   */
  
  /**
   * @fn virtual EMxFileIOException::~EMxFileIOException()
   * Empty destructor.
   */
  
  /**
   * @class EMxFileIO btkEMxFileIO.h
   * @brief Interface to read/write EMx files.
   *
   * The EMx file format is used in the Elite acquisition system (BTS, Milan, Italy).
   * The exported acquisition contain EMG data.
   *
   * @ingroup BTKIO
   */
  
  /**
   * @typedef EMxFileIO::Pointer
   * Smart pointer associated with a EMxFileIO object.
   */
  
  /**
   * @typedef EMxFileIO::ConstPointer
   * Smart pointer associated with a const EMxFileIO object.
   */
  
  /**
   * @fn static EMxFileIO::Pointer EMxFileIO::New()
   * Create a EMxFileIO object an return it as a smart pointer.
   */
  
  /**
   * Only check if the file extension correspond to EMx.
   */
  bool EMxFileIO::CanReadFile(const std::string& filename)
  {
    std::string lowercase = filename;
    std::transform(lowercase.begin(), lowercase.end(), lowercase.begin(), tolower);
    std::string::size_type EMxPos = lowercase.rfind(".emg");
    if ((EMxPos != std::string::npos) && (EMxPos == lowercase.length() - 4))
    //std::string::size_type EMxPos = lowercase.substr(0,lowercase.length()-1).rfind(".em");
    //if ((EMxPos != std::string::npos) && (EMxPos == lowercase.length() - 4) && ((*(lowercase.rbegin()) == 'f') || (*(lowercase.rbegin()) == 'g')  || (*(lowercase.rbegin()) == 'r')))
    {
      NativeBinaryFileStream bifs(filename, BinaryFileStream::In);
      if (!bifs.IsOpen())
        return false;
      bifs.Close();
      return true;
    }
    return false;
  };
  
  /**
   * Read the file designated by @a filename and fill @a output.
   */
  void EMxFileIO::Read(const std::string& filename, Acquisition::Pointer output)
  {
    output->Reset();
    IEEELittleEndianBinaryFileStream bifs;
    bifs.SetExceptions(BinaryFileStream::EndFileBit | BinaryFileStream::FailBit | BinaryFileStream::BadBit);
    try
    {
      bifs.Open(filename, BinaryFileStream::In);
      // Bytes 1-2: 0x0000
      bifs.SeekRead(4, BinaryFileStream::Current);
      int numFra = bifs.ReadU16() + bifs.ReadU16() * 1000;
      int numCha = bifs.ReadU16();
      double freq = static_cast<double>(bifs.ReadU16());
      output->Init(0,numFra,numCha);
      output->SetPointFrequency(freq);
      // Labels (max: 32 labels)
      for (Acquisition::AnalogIterator it = output->BeginAnalog() ; it != output->EndAnalog() ; ++it)
      {
        std::string label = bifs.ReadString(8);
        // Remove spaces
        label = label.erase(label.find_last_not_of(' ') + 1);
        label = label.erase(0, label.find_first_not_of(' '));
        // Remove 0x00 and characters after.
        label = label.erase(0, label.find_first_not_of(static_cast<char>(0x00)));
        label = label.erase(label.find_first_of(static_cast<char>(0x00)));
        // Saving
        (*it)->SetLabel(label);
      }
      bifs.SeekRead((32 - numCha) * 8, BinaryFileStream::Current);
      // Scales (init)
      std::vector<double> scales = std::vector<double>(numCha, 1.0);
      int i = 0;
      // Units
      for (Acquisition::AnalogIterator it = output->BeginAnalog() ; it != output->EndAnalog() ; ++it)
      {
        std::string unit = bifs.ReadString(4);
        // Remove spaces
        unit = unit.erase(unit.find_last_not_of(' ') + 1);
        unit = unit.erase(0, unit.find_first_not_of(' '));
        // Remove 0x00 and characters after.
        unit = unit.erase(0, unit.find_first_not_of(static_cast<char>(0x00)));
        unit = unit.erase(unit.find_first_of(static_cast<char>(0x00)));
        // Saving
        (*it)->SetUnit(unit);
        if (unit.compare("mV") == 0)
          scales[i] = 1000.0;
        ++i;
      }
      // General ADC gain?
      bifs.SeekRead((32 - numCha) * 4, BinaryFileStream::Current);
      double adcGain = bifs.ReadU16();
      // Others unknown data ...
      bifs.SeekRead(114, BinaryFileStream::Current);
      // Analog channel settings
      // First parameter ... What is it?
      bifs.SeekRead(32*2, BinaryFileStream::Current);
      // Second parameter ... What is it?
      bifs.SeekRead(32*2, BinaryFileStream::Current);
      // Third parameter ... What is it?
      bifs.SeekRead(32*2, BinaryFileStream::Current);
      // Gain
      for (int i = 0 ; i < numCha ; ++i)
        scales[i] *= 20.0 / (4096.0 * adcGain * static_cast<double>(bifs.ReadU16()));
      // Scaled values
      bifs.SeekRead(1024, BinaryFileStream::Begin);
      int inc = 0;
      i = 0;
      Acquisition::AnalogIterator it = output->BeginAnalog();
      while (inc < output->GetPointFrameNumber())
      {
        (*it)->GetValues().coeffRef(inc) = bifs.ReadI16() * scales[i];

        ++it; ++i;
        if (it == output->EndAnalog())
        {
          it = output->BeginAnalog();
          i = 0;
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
        excmsg = "Loss of integrity of the filestream.";
      else if(bifs.Fail())
        excmsg = "Internal logic operation error on the stream associated with the file.";
      else
        excmsg = "Unknown error associated with the filestream.";
      
      if (bifs.IsOpen()) bifs.Close();    
      throw(EMxFileIOException(excmsg));
    }
    catch (EMxFileIOException& )
    {
      if (bifs.IsOpen()) bifs.Close(); 
      throw;
    }
    catch (std::exception& e)
    {
      if (bifs.IsOpen()) bifs.Close(); 
      throw(EMxFileIOException("Unexpected exception occurred: " + std::string(e.what())));
    }
    catch(...)
    {
      if (bifs.IsOpen()) bifs.Close(); 
      throw(EMxFileIOException("Unknown exception"));
    }
  };
  
  /**
   * Constructor.
   */
  EMxFileIO::EMxFileIO()
  : AcquisitionFileIO()
  {
    this->m_FileType = AcquisitionFileIO::Binary;
    this->m_ByteOrder = AcquisitionFileIO::IEEE_LittleEndian;
    this->m_StorageFormat = AcquisitionFileIO::Integer;
  };
};

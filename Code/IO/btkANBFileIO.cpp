/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2015, Arnaud Barré
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

#include "btkANBFileIO.h"
#include "btkMotionAnalysisFileIOUtils_p.h"
#include "btkMetaDataUtils.h"
#include "btkConvert.h"
#include "btkLogger.h"

#include <algorithm>
#include <cctype>

namespace btk
{
  /**
   * @class ANBFileIOException btkANBFileIO.h
   * @brief Exception class for the ANBFileIO class.
   */
  
  /**
   * @fn ANBFileIOException::ANBFileIOException(const std::string& msg)
   * Constructor.
   */
  
  /**
   * @fn virtual ANBFileIOException::~ANBFileIOException()
   * Empty destructor.
   */
  
  /**
   * @class ANBFileIO btkANBFileIO.h
   * @brief Interface to read/write ANB files.
   *
   * A ANB file contains analog channels data stored in the IEEE Little endian binary format.
   *
   * Based on reverse engineering, the file format is composed of a header part and a data part.
   * The header is composed of keys and values. 
   * 
   * The known keys are:
   *  - <tt>0x8000</tt>: Number of words for the header
   *  - <tt>0x0101</tt>: ID?
   *  - <tt>0x0108</tt>: Board Type
   *  - <tt>0x0109</tt>: Bit Depth
   *  - <tt>0x010A</tt>: Precise Rate
   *  - <tt>0x0102</tt>: Channel Number
   *  - <tt>0x0103</tt>: Channel Index? (use a decimal to binary converter to found the index 0xFF = 0b11111111)
   *  - <tt>0x0104</tt>: Channel Rate
   *  - <tt>0x0105</tt>: First time? (index of the first frame)?
   *  - <tt>0x0106</tt>: Channel Range (each range is write on 2 bytes)
   *  - <tt>0x0107</tt>: Channel Label (if label's size % 4 == 0, then add a word of 0x00)
   *  - <tt>0x8100</tt>: Number of words for the data
   *
   * A key is always followed by the size of its value and is write on 2 bytes. The size correspond to
   * number of word associated with the key. A word corresponds to four bytes.
   * For example, the following string:
   * @verbatim 0800 0001 0000 024C @endverbatim
   * corresponds to the key @c 0x0800 with a size of 1 word (<tt>0x0001</tt>) and a value equal to 588 (<tt>0x024C</tt>).
   *
   * The number of frames is determined as <tt>number_of_frames = (number_of_words - 3) * 2 / number_of_channels</tt>
   *
   * The structure of the file can be summarized as the following:
   * @code
   * 0x0000 0000 // Begin Header
   * 0x8000 ... 
   * 0x0101 ... 
   * 0x0108 ... 
   * 0x0109 ... 
   * 0x010A ...
   * 0x0102 ...
   * 0x0103 ...
   * 0x0104 ...
   * 0x0105 ...
   * 0x0106 ...
   * 0x0107 ...
   *   |- // repeated by the number of channels
   * 0x0107 ...
   * 0x0000 0000 // End Header / Begin Data
   * 0x8100 ...
   * // ... scaled data ...
   * @endcode
   *
   * The ANB file format is created by Motion Analysis Corp.
   *
   * @ingroup BTKIO
   */
  
  /**
   * @typedef ANBFileIO::Pointer
   * Smart pointer associated with a ANBFileIO object.
   */
  
  /**
   * @typedef ANBFileIO::ConstPointer
   * Smart pointer associated with a const ANBFileIO object.
   */
  
  /**
   * @fn static ANBFileIO::Pointer ANBFileIO::New()
   * Create a ANBFileIO object an return it as a smart pointer.
   */
  
  /**
   * Checks if the first word in the file corresponds to "PathFileType".
   */
  bool ANBFileIO::CanReadFile(const std::string& filename)
  {
    bool isReadable = true;
    IEEELittleEndianBinaryFileStream bifs(filename, BinaryFileStream::In);
    // Three first words: 0x0000 0000 0080
    if ((bifs.ReadI16() != 0x0000) || (bifs.ReadI16() != 0x000) || (bifs.ReadU16() != 0x8000))
      isReadable = false;
    bifs.Close();
    return isReadable;
  };
  
  /**
   * Checks if the suffix of @a filename is ANB.
   */
  bool ANBFileIO::CanWriteFile(const std::string& filename)
  {
    std::string lowercase = filename;
    std::transform(lowercase.begin(), lowercase.end(), lowercase.begin(), tolower);
    std::string::size_type ANBPos = lowercase.rfind(".anb");
    if ((ANBPos != std::string::npos) && (ANBPos == lowercase.length() - 4))
      return true;
    else
      return false;
  };
  
  /**
   * Read the file designated by @a filename and fill @a output.
   */
  void ANBFileIO::Read(const std::string& filename, Acquisition::Pointer output)
  {
    output->Reset();
    IEEELittleEndianBinaryFileStream bifs;
    bifs.SetExceptions(BinaryFileStream::EndFileBit | BinaryFileStream::FailBit | BinaryFileStream::BadBit);
    try
    {
      bifs.Open(filename, BinaryFileStream::In);
      if ((bifs.ReadI16() != 0) || (bifs.ReadI16() != 0))
        throw ANBFileIOException("Invalid ANB file.");
      // Header
      uint32_t headerSize, id, bitDepth, channelNumber;
      float preciseRate, firstTime;
      std::string boardType;
      std::vector<uint8_t> hexIndex;
      std::vector<uint16_t> channelRate, channelRange;
      size_t counter = 1; // First word read.
      counter += this->ReadKeyValueU32(&headerSize, &bifs, 0x8000);
      counter += this->ReadKeyValueU32(&id, &bifs, 0x0101);
      counter += this->ReadKeyValueString(boardType, &bifs, 0x0108);
      counter += this->ReadKeyValueU32(&bitDepth, &bifs, 0x0109);
      if (bitDepth >= 65536)
        bitDepth = 16; // One file has the bitDepth set to 65552 instead of 16 ... Need more files from system using 16 bit ADC.
      counter += this->ReadKeyValueFloatFromTwoU16(&preciseRate, &bifs, 0x010A);
      counter += this->ReadKeyValueU32(&channelNumber, &bifs, 0x0102);
      counter += this->ReadKeyValueU8(hexIndex, &bifs, 0x0103);
      counter += this->ReadKeyValueU16(channelRate, &bifs, 0x0104);
      counter += this->ReadKeyValueFloatFromTwoU16(&firstTime, &bifs, 0x0105);
      counter += this->ReadKeyValueU16(channelRange, &bifs, 0x0106);
      std::vector<std::string> channelLabel = std::vector<std::string>(channelNumber);
      for (size_t i = 0 ; i < channelNumber ; ++i)
        counter += this->ReadKeyValueString(channelLabel[i], &bifs, 0x0107);
      if (counter != headerSize)
        throw ANBFileIOException("The size of the header is not equal to the number of words read.");
      // Resize for odd number of channels
      channelRate.resize(channelNumber);
      channelRange.resize(channelNumber);
      
      // Data
      if (channelNumber != 0)
      {
        uint32_t dataSize;
        if ((bifs.ReadI16() != 0) || (bifs.ReadI16() != 0))
          throw ANBFileIOException("Invalid data part.");
        this->ReadKeyValueU32(&dataSize, &bifs, 0x8100);
        uint32_t frameNumber = (dataSize - 3) * 2 / channelNumber;
        
        ANxFileIOCheckHeader_p(preciseRate, channelNumber, channelRate, channelRange);
        ANxFileIOStoreHeader_p(output, filename, preciseRate, frameNumber, channelNumber, channelLabel, channelRate, channelRange, boardType, bitDepth);
        output->SetFirstFrame(static_cast<int>(firstTime * preciseRate) + 1);
          
        // Convert hexIndex to metadata ANALOG:INDEX
        // FIXME: What is really the content of the key 0x0103? Its data are not stored in an acquisition
        /*
        MetaData::Pointer analog = MetaDataCreateChild(output->GetMetaData(), "ANALOG");
        std::vector<int16_t> channelIndex = std::vector<int16_t>(channelNumber, 0);
        int chan = 0, idx = 1;
        for (int i = 0 ; i < hexIndex.size() ; ++i)
        {
          int inc = 1;
          while (inc < 256)
          {
              if (hexIndex[i] & inc)
                  channelIndex[chan++] = idx;
              inc *= 2;
              ++idx;
          }
        }
        MetaDataCreateChild(analog, "INDEX", channelIndex);
        */
        
        // Read analog channel values
        for(int i = 0 ; i < static_cast<int>(frameNumber) ; ++i)
        {
          for (AnalogCollection::Iterator it = output->BeginAnalog() ; it != output->EndAnalog() ; ++it)
          {
            (*it)->GetValues().coeffRef(i) = static_cast<double>(bifs.ReadI16()) * (*it)->GetScale();
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
      throw(ANBFileIOException(excmsg));
    }
    catch (ANBFileIOException& )
    {
      if (bifs.IsOpen()) bifs.Close();
      throw;
    }
    catch (MotionAnalysisBinaryFileIOException& e)
    {
      if (bifs.IsOpen()) bifs.Close();
      throw(MotionAnalysisBinaryFileIOException(e.what()));
    }
    catch (ANxFileIOException& e)
    {
      if (bifs.IsOpen()) bifs.Close();
      throw(ANBFileIOException(e.what()));
    }
    catch (std::exception& e)
    {
      if (bifs.IsOpen()) bifs.Close();
      throw(ANBFileIOException("Unexpected exception occurred: " + std::string(e.what())));
    }
    catch(...)
    {
      if (bifs.IsOpen()) bifs.Close();
      throw(ANBFileIOException("Unknown exception"));
    }
  };
  
  /**
   * Write the file designated by @a filename with the content of @a input.
   */
  void ANBFileIO::Write(const std::string& filename, Acquisition::Pointer input)
  {
    if (!input)
    {
      btkErrorMacro("Impossible to write a null input into a file.");
      return;
    }
    IEEELittleEndianBinaryFileStream bofs;
    try
    {
      // File access
      bofs.Open(filename, BinaryFileStream::Out | BinaryFileStream::Truncate);
      if (!bofs.IsOpen())
        throw(ANBFileIOException("No File access"));
      
      // Header part
      // 0x0000 0000
      bofs.Fill(4);
      size_t counter = 4; // First word wrote.
      // Key 0x8000: False header size
      counter += this->WriteKeyValue(&bofs, 0x8000, (uint32_t)0);
      // Key 0x0101: ID?
      // FIXME: Can we write all ANB files with the ID 0x0000?
      counter += this->WriteKeyValue(&bofs, 0x0101, (uint32_t)0);
      // Key 0x0108: BoardType
      std::string boardType = "Unknown";
      MetaData::ConstIterator itAnalog = input->GetMetaData()->FindChild("ANALOG");
      if (itAnalog != input->GetMetaData()->End())
      {
        MetaData::ConstIterator itBoard = (*itAnalog)->FindChild("BOARD");
        if (itBoard != (*itAnalog)->End())
          boardType = (*itBoard)->GetInfo()->ToString(0);
      }
      boardType.resize(64, 0x00);
      counter += this->WriteKeyValue(&bofs, 0x0108, boardType);
      // Key 0x0109: Bit Depth
      counter += this->WriteKeyValue(&bofs, 0x0109, static_cast<uint32_t>(input->GetAnalogResolution()));
      // Key 0x010A: Precise Rate
      float rate = static_cast<float>(input->GetAnalogFrequency());
      if (rate == 0.0f)
      {
        btkWarningMacro("Acquisition frequency cannot be null and is set to 50 Hz.");
        rate = 50.0f; // Hz
        
      }
      counter += this->WriteKeyValue(&bofs, 0x010A, rate);
      // Key 0x0102: Channel Number
      counter += this->WriteKeyValue(&bofs, 0x0102, static_cast<uint32_t>(input->GetAnalogNumber()));
      // Key 0x0103: Channel Index
      // FIXME: Need more ANB file to be sure of the content of this key
      std::vector<uint8_t> hexIndex(32, 0);
      int inc = input->GetAnalogNumber(); int i = 0;
      while (inc > 0)
      {
        hexIndex[i] = (inc > 8) ? 0xFF : static_cast<uint8_t>(pow(2.0, inc) - 1);
        ++i;
        inc -= 8;
      }
      /*
      while (i < input->GetAnalogNumber())
       channelIndex[i++] = i+1;
      
      Acquisition::MetaDataConstIterator itAnalog = output->GetMetaData()->FindChild("ANALOG");
      int i = 0;
      if (itAnalog != output->EndMetaData())
      {
        MetaDataInfo::Pointer analogIndex = (*itAnalog)->ExtractChildInfo("INDEX", MetaDataInfo::Integer, 1);
        if (analogIndex != MetaDataInfo::Null)
          channelIndex = analogIndex->ToInt16();
        if (channelIndex.size() != input->GetAnalogNumber())
          btkErrorMaro("Size of the metadata ANALOG:INDEX doesn't correspond to the number of analog channels. Regeneration of the index of the analog channels.");
        else
          i = channelIndex.size();
      }
      while (i < input->GetAnalogNumber())
        channelIndex[i] = i+1;
      
      std::vector<uint8_t> hexIndex(32, 0);
      int inc = 1;
      for (int j = 0 ; j < channelIndex.size() ; ++j)
      {
        int bin = (channelIndex[j] - 1) / 16;
        hexIndex[bin] += ((channelIndex[j] - 1) - bin * 16) * 16;
      }
      */
      counter += this->WriteKeyValue(&bofs, 0x0103, hexIndex);
      // Key 0x0104: Channel Rate
      counter += this->WriteKeyValue(&bofs, 0x0104, std::vector<uint16_t>(input->GetAnalogNumber(), static_cast<uint16_t>(rate)));
      // Key 0x0105: First time?
      // FIXME: Not sure for the value of this key
      counter += this->WriteKeyValue(&bofs, 0x0105, static_cast<float>((input->GetFirstFrame() - 1) / rate));
      // Key 0x0106: Channel Range
      std::vector<uint16_t> channelRange(input->GetAnalogNumber(), 10000);
      i = 0;
      for (Acquisition::AnalogIterator it = input->BeginAnalog() ; it != input->EndAnalog() ; ++it)
      {
        if ((*it)->GetGain() == Analog::Unknown)
        {
          channelRange[i] = ANxFileIODetectAnalogRange_p((*it)->GetScale(), input->GetAnalogResolution());
          btkWarningMacro("Unknown gain for channel #" + ToString(i+1) + ". Automatically replaced by +/- " + ToString(static_cast<double>(channelRange[i]) / 1000.0)  + " volts in the file.");
        }
        else
          channelRange[i] = (*it)->GetGain();
        ++i;
      }
      counter += this->WriteKeyValue(&bofs, 0x0106, channelRange);
      // Labels
      for (Acquisition::AnalogIterator it = input->BeginAnalog() ; it != input->EndAnalog() ; ++it)
        counter += this->WriteKeyValue(&bofs, 0x0107, (*it)->GetLabel(), true);
      // Back to the key 0x8000 and rewrite it
      bofs.SeekWrite(4, BinaryFileStream::Begin);
      this->WriteKeyValue(&bofs, 0x8000, static_cast<uint32_t>(counter / 4));
      // Back to the last wrote byte
      bofs.SeekWrite(counter, BinaryFileStream::Begin);
      
      // Data part
      bofs.Fill(4);
      // Key 0x8100: Number of words for the data
      uint32_t dataSize = input->GetAnalogFrameNumber() * input->GetAnalogNumber() / 2 + 3;
      this->WriteKeyValue(&bofs, 0x8100, dataSize);
      for (int frame = 0 ; frame < input->GetAnalogFrameNumber() ; ++frame)
      {
        for (AnalogCollection::ConstIterator it = input->BeginAnalog() ; it != input->EndAnalog() ; ++it)
          bofs.Write(static_cast<int16_t>((*it)->GetValues().coeff(frame) / (*it)->GetScale()));
      };
    }
    catch (ANBFileIOException& )
    {
      if (bofs.IsOpen()) bofs.Close();
      throw;
    }
    catch (std::exception& e)
    {
      if (bofs.IsOpen()) bofs.Close();
      throw(ANBFileIOException("Unexpected exception occurred: " + std::string(e.what())));
    }
    catch(...)
    {
      if (bofs.IsOpen()) bofs.Close();
      throw(ANBFileIOException("Unknown exception"));
    }
  };
  
  /**
   * Constructor.
   */
  ANBFileIO::ANBFileIO()
  : MotionAnalysisBinaryFileIO(AcquisitionFileIO::Integer)
  {};
  
  size_t ANBFileIO::ReadKeyValueFloatFromTwoU16(float* val, IEEELittleEndianBinaryFileStream* bifs, int key)
  {
    this->ReadKey(bifs, key);
    this->CheckSizeForSingleValue(bifs);
    uint16_t hsb = bifs->ReadU16();
    uint16_t lsb = bifs->ReadU16();
    uint32_t foo = hsb << 16 | lsb;
    *val = *reinterpret_cast<float*>(&foo);
    return 2;
  };
};

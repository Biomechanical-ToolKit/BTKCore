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

#include "btkHPFFileIO.h"
#include "btkBinaryFileStream.h"
#include "btkMetaDataUtils.h"
#include "btkConvert.h"
#include "btkLogger.h"

#include <limits>

#include <pugixml.hpp>

#include <iostream>

struct btk_hpf_chunk_info
{
  size_t position;
  int64_t id;
  int64_t size;
  int32_t group;
};

namespace btk
{
  /**
   * @class HPFFileIOException btkHPFFileIO.h
   * @brief Exception class for the HPFFileIO class.
   */
  
  /**
   * @fn HPFFileIOException::HPFFileIOException(const std::string& msg)
   * Constructor.
   */
  
  /**
   * @fn virtual HPFFileIOException::~HPFFileIOException()
   * Empty destructor.
   */
  
  /**
   * @class HPFFileIO btkHPFFileIO.h
   * @brief Interface to read/write HPF files.
   *
   * The HPF file format is used in the software EMGWorks 4.0 (Delsys Inc., Natick, USA) and contains only analogs data.
   *
   * @note The reader was implemented based on the document <em>DT High Performance File Format Specification</em> (Document Number: 22760, Rev A) proposed by the company Data Translation, Inc. (Marlborough, USA).
   * No restriction was found which could mention the prohibition of the use this documentation to implement a file reader.
   *
   * @ingroup BTKIO
   */
  
  /**
   * @typedef HPFFileIO::Pointer
   * Smart pointer associated with a HPFFileIO object.
   */
  
  /**
   * @typedef HPFFileIO::ConstPointer
   * Smart pointer associated with a const HPFFileIO object.
   */
  
  /**
   * @fn static HPFFileIO::Pointer HPFFileIO::New()
   * Create a HPFFileIO object an return it as a smart pointer.
   */
  
  /**
   * Check if the beginning of the file starts with:
   *  - The value 0x1000 (Int64);
   *  - 8 bytes.
   *  - The string "datx".
   */
  bool HPFFileIO::CanReadFile(const std::string& filename)
  {
    bool isReadable = true;
    IEEELittleEndianBinaryFileStream bifs(filename, BinaryFileStream::In);
    int64_t chunkID = bifs.ReadI64();
    bifs.SeekRead(8, BinaryFileStream::Current); // chunkSize
    if ((chunkID != 0x1000) || (bifs.ReadString(4).compare("datx") != 0))
      isReadable = false;
    bifs.Close();
    return isReadable;
  };
  
  /**
   * Read the file designated by @a filename and fill @a output.
   */
  void HPFFileIO::Read(const std::string& filename, Acquisition::Pointer output)
  {
    output->Reset();
    IEEELittleEndianBinaryFileStream bifs;
    bifs.SetExceptions(BinaryFileStream::EndFileBit | BinaryFileStream::FailBit | BinaryFileStream::BadBit);
    try
    {
      bifs.Open(filename, BinaryFileStream::In);
      // Header
      int64_t chunkID = bifs.ReadI64();
      int64_t chunkSize = bifs.ReadI64();
      if ((chunkID != 0x1000) || (bifs.ReadString(4).compare("datx") != 0))
        throw HPFFileIOException("Invalid HPF file.");
      int64_t fileVersion = bifs.ReadI64();
      if (fileVersion != 0x10001)
        throw HPFFileIOException("Unsupported version of the HPF file format.");
      bifs.SeekRead(chunkSize, BinaryFileStream::Begin);
      // Extract chunks information. For this, we look until the end of the file
      // to find the type of chunks and their size.
      // Only two types of chunk are extracted and the others are discarded:
      //  - Channel information chunk: 0x2000
      //  - Data chunk: 0x3000
      const int64_t channelInfoChunkID = 0x2000;
      const int64_t dataChunkID = 0x3000;
      std::list<btk_hpf_chunk_info> chunks;
      bifs.SetExceptions(BinaryFileStream::BadBit);
      // EOF bit can be triggered by the Read* methods while FAIL can be triggered by the SeekRead method.
      do
      {
        size_t chunkPosition = static_cast<size_t>(bifs.TellRead());
        chunkID = bifs.ReadI64();
        chunkSize = bifs.ReadI64();
        int64_t already_read = 16;
        if ((chunkID == channelInfoChunkID) || (chunkID == dataChunkID))
        {
          btk_hpf_chunk_info ci;
          ci.position = chunkPosition;
          ci.id = chunkID;
          ci.size = chunkSize;
          ci.group = bifs.ReadI32();
          chunks.push_back(ci);
          already_read += 4;
        }
        bifs.SeekRead(chunkSize - already_read, BinaryFileStream::Current);
      } while (!bifs.EndFile());
      // Check if only one Channel information chunk exists. Data with different time
      // increments per channel is not supported in the library Biomechanical ToolKit. 
      btk_hpf_chunk_info* channelInfoChunk = 0;
      for (std::list<btk_hpf_chunk_info>::iterator it = chunks.begin() ; it != chunks.end() ; ++it)
      {
        if (it->id == channelInfoChunkID)
        {
          if (channelInfoChunk == 0)
            channelInfoChunk = &(*it);
          else
            throw HPFFileIOException("HPF file with more than one channel information chunk is not supported.");
        }
      }
      if (channelInfoChunk == 0)
        throw HPFFileIOException("No channel information chunk found. Impossible to configure the acqusition and the analog channels.");
      // Set the IO exceptions as initially
      bifs.Clear();
      bifs.SetExceptions(BinaryFileStream::EndFileBit | BinaryFileStream::FailBit | BinaryFileStream::BadBit);
      // Go to the chunk after the group ID
      bifs.SeekRead(channelInfoChunk->position + 20, BinaryFileStream::Begin);
      // Extract Channel Info
      int32_t numAnalogChannels = bifs.ReadI32();
      int32_t numAnalogChannelsParsed = 0;
      double analogFrequency = -1.0;
      // The remaining part of the chunk contains XML data
      // The reading of the XML data is done with pugixml
      // Depending of the configuration of BTK and pugixml, we try to avoid any memory allocation for the XML data
      const size_t channelInfoChunkXmlDataOffset = 24;
      size_t xmlBufferSize = channelInfoChunk->size - channelInfoChunkXmlDataOffset;
      char* xmlBuffer = new char[xmlBufferSize];
      bifs.ReadChar(xmlBufferSize, xmlBuffer);
      std::string errmsg;
      pugi::xml_document xmlDoc;
      pugi::xml_parse_result xmlResult = xmlDoc.load_buffer_inplace(xmlBuffer, xmlBufferSize, pugi::parse_minimal, pugi::encoding_utf8);
      if (xmlResult)
      {
        pugi::xml_node channels = xmlDoc.child("ChannelInformationData");
        for (pugi::xml_node channel = channels.child("ChannelInformation"); channel; channel = channel.next_sibling("ChannelInformation"))
        {
          btk::Analog::Pointer an = btk::Analog::New();
          an->SetLabel(channel.child_value("Name"));
          an->SetUnit(channel.child_value("Unit"));
          // In case the unit is set to the string 'Volts', it is replaced by 'V' as most of the file format use only the letter 'V'.
          if (an->GetUnit().compare("Volts") == 0)
            an->SetUnit("V");
          double requestedPerChannelSampleRate = FromString<double>(channel.child_value("RequestedPerChannelSampleRate"));
          // In some case (convertion from Delsys EMG file?), the RequestedPerChannelSampleRate is null but not the PerChannelSampleRate. 
          if (requestedPerChannelSampleRate == 0.0)
            requestedPerChannelSampleRate = FromString<double>(channel.child_value("PerChannelSampleRate"));
          // Be sure the sample frequency is the same for all the analog channels
          if (analogFrequency == -1.0)
            analogFrequency = requestedPerChannelSampleRate;
          if (std::fabs(analogFrequency - requestedPerChannelSampleRate) > std::numeric_limits<double>::epsilon())
          {  
            errmsg = "An acquisition cannot support multiple analog sample frequencies.";
            break;
          }
          // Do not know if Delsys used another storage format than Float
          if (strcmp(channel.child_value("DataType"), "Float") != 0)
          {
            errmsg = "Currently, the HPF file reader supports only data stored as float.";
            break;
          }
          // To simplify the implementation of the reader, the data index is assumed sorted.
          if (FromString<int32_t>(channel.child_value("DataIndex")) != numAnalogChannelsParsed)
          {
            errmsg = "Currently, the HPF file reader supports only data with a sorted index.";
            break;
          }
          // Try to set the gain
          float rangeMin = FromString<float>(channel.child_value("RangeMin"));
          float rangeMax = FromString<float>(channel.child_value("RangeMax"));
          if (std::fabs(rangeMin + rangeMax) < std::numeric_limits<float>::epsilon())
          {
            // In some case (convertion from Delsys EMG file?), the Range(Min|Max) values are null. The external gain is then extracted to try to set the analog gain. 
            if (rangeMax == 0.0f)
              rangeMax = FromString<float>(channel.child_value("ExternalGain"));
            an->SetGainFromValue(static_cast<int>(rangeMax * 1000.0f));
          }
          else
            btkWarningMacro(filename, "Unsupported gain for the channel '" + an->GetLabel() + "': Unsymmetrical voltage range.");
          // Append the configured analog channel into the acquisition
          output->AppendAnalog(an);
          ++numAnalogChannelsParsed;
        }
      }
      else
          errmsg = "Error during the parsing of the XML data: " + std::string(xmlResult.description());
      if ((numAnalogChannelsParsed != numAnalogChannels) && errmsg.empty())
        errmsg = "The number of analog channels parsed (" + ToString(numAnalogChannelsParsed) + ") is different than the number of channels stored in the configuration (" + ToString(numAnalogChannels) + ")";
      delete[] xmlBuffer;
      if (!errmsg.empty())
        throw HPFFileIOException(errmsg);
      // Extract the data
      int64_t maxFrameNumber = 0;
      for (std::list<btk_hpf_chunk_info>::iterator it = chunks.begin() ; it != chunks.end() ; ++it)
      {
        if (it->id != dataChunkID)
          continue;
        // Go to the chunk after the chunk size
        bifs.SeekRead(it->position + 16, BinaryFileStream::Begin);
        // Extract and verify the configuration of the chunk
        // - Just in case the group ID is not not the same
        if (bifs.ReadI32() != channelInfoChunk->group)
          continue;
        int64_t dataStartIndex = bifs.ReadI64();
        int64_t channelDataCount = bifs.ReadI32();
        // - If for some reason the the number of channels is greater than in the configuration, the value is bounded.
        if (channelDataCount > numAnalogChannels)
          channelDataCount = numAnalogChannels;
        int32_t* channelDescriptor = new int32_t[2*channelDataCount];
        bifs.ReadI32(2*channelDataCount, channelDescriptor);
        // Read data
        Acquisition::AnalogIterator itA = output->BeginAnalog();
        for (int i = 0 ; i < channelDataCount ; ++i)
        {
          bifs.SeekRead(it->position + channelDescriptor[i*2], BinaryFileStream::Begin);
          int32_t numFrames = channelDescriptor[i*2+1] / 4;
          maxFrameNumber = std::max(maxFrameNumber, dataStartIndex + numFrames);
          if ((*itA)->GetFrameNumber() < static_cast<int>(maxFrameNumber))
            (*itA)->SetFrameNumber(static_cast<int>(maxFrameNumber));
          for (int j = 0 ; j < numFrames ; ++j)
            (*itA)->SetDataSlice(static_cast<int>(dataStartIndex+j), bifs.ReadFloat());
          ++itA;
        }
      }
      // Finalize the acquisition configuration
      output->SetPointFrequency(analogFrequency);
      output->Resize(0, static_cast<int>(maxFrameNumber), numAnalogChannels);
      // The ADC resolution is not stored. It is assumed that it is a 16-bit ADC card.
      output->SetAnalogResolution(Acquisition::Bit16);
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
      throw(HPFFileIOException(excmsg));
    }
    catch (HPFFileIOException& )
    {
      if (bifs.IsOpen()) bifs.Close();
      throw;
    }
    catch (std::exception& e)
    {
      if (bifs.IsOpen()) bifs.Close();
      throw(HPFFileIOException("Unexpected exception occurred: " + std::string(e.what())));
    }
    catch(...)
    {
      if (bifs.IsOpen()) bifs.Close();
      throw(HPFFileIOException("Unknown exception"));
    }
  };
  
  /**
   * Constructor.
   */
  HPFFileIO::HPFFileIO()
  : AcquisitionFileIO(AcquisitionFileIO::Binary, AcquisitionFileIO::IEEE_LittleEndian, AcquisitionFileIO::Float)
  {};
};

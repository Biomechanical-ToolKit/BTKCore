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

#include "btkTDFFileIO.h"
#include "btkBinaryFileStream.h"
#include "btkConvert.h"
#include "btkMetaDataUtils.h"
#include "btkLogger.h"

#include <algorithm>
#include <cctype>

namespace btk
{
  static const uint32_t TDFKey[4] = {0x41604B82, 0xCA8411D3, 0xACB60060, 0x080C6816};
  
  /**
   * @class TDFFileIOException btkTDFFileIO.h
   * @brief Exception class for the TDFFileIO class.
   */
  
  /**
   * @fn TDFFileIOException::TDFFileIOException(const std::string& msg)
   * Constructor.
   */
  
  /**
   * @fn virtual TDFFileIOException::~TDFFileIOException()
   * Empty destructor.
   */
  
  /**
   * @class TDFFileIO btkTDFFileIO.h
   * @brief Interface to read/write ANG files.
   *
   *
   * @ingroup BTKIO
   */
  
  /**
   * @typedef TDFFileIO::Pointer
   * Smart pointer associated with a TDFFileIO object.
   */
  
  /**
   * @typedef TDFFileIO::ConstPointer
   * Smart pointer associated with a const TDFFileIO object.
   */
  
  /**
   * @fn static TDFFileIO::Pointer TDFFileIO::New()
   * Create a TDFFileIO object an return it as a smart pointer.
   */
  
  /**
   * Only check if the file extension correspond to ANG.
   */
  bool TDFFileIO::CanReadFile(const std::string& filename)
  {
    bool isReadable = true;
    IEEELittleEndianBinaryFileStream ifs(filename, BinaryFileStream::In);
    if ((ifs.ReadU32() != TDFKey[0]) || (ifs.ReadU32() != TDFKey[1]) || (ifs.ReadU32() != TDFKey[2]) || (ifs.ReadU32() != TDFKey[3]))
      isReadable = false;
    ifs.Close();
    return isReadable;
  };
  
  /**
   * Read the file designated by @a filename and fill @a output.
   */
  void TDFFileIO::Read(const std::string& filename, Acquisition::Pointer output)
  {
    output->Reset();
    IEEELittleEndianBinaryFileStream bifs;
    bifs.SetExceptions(BinaryFileStream::EndFileBit | BinaryFileStream::FailBit | BinaryFileStream::BadBit);
    try
    {
      bifs.Open(filename, BinaryFileStream::In);
      if ((bifs.ReadU32() != TDFKey[0]) || (bifs.ReadU32() != TDFKey[1]) || (bifs.ReadU32() != TDFKey[2]) || (bifs.ReadU32() != TDFKey[3]))
        throw(TDFFileIOException("Invalid header key."));
      uint32_t version = bifs.ReadU32();
      if (version != 1)
        throw(TDFFileIOException("Unsupported version. Only the version 1 of the TDF file format is supported."));
      int32_t numEntries = bifs.ReadI32();
      
      std::list<BlockEntry> blockEntries;
      size_t nextEntryOffset = 40;
      for (int i = 0 ; i < numEntries ; ++i)
      {
        BlockEntry be;
        bifs.SeekRead(nextEntryOffset, BinaryFileStream::Current);
        be.type = bifs.ReadU32();
        be.format = bifs.ReadU32();
        be.offset = bifs.ReadI32();
        be.size = bifs.ReadI32();
        blockEntries.push_back(be);
        nextEntryOffset = 272; // 16 + 256
      }
      
      // Only the following blocks are extracted from the TDF format:
      enum {
        MarkerBlockId = 5,         // - block ID #5: Markers data
        PlatformDataBlockId = 9,   // - block ID #9: Force platform data
        PlatformConfigBlockId = 7, // - block ID #7: Force platform configuration (corners)
        EMGBlockId = 11,           // - block ID #11: EMG data
      };
      // The block "Force3D" (ID 12) is not extracted as its content can be reconstructed using force platform filters 
      
      // Check if the acquisition's data are consistent between them, and initialize the output
      bool MarkerBlockFound = false;
      bool FPBlockFound = false;
      bool EMGBlockFound = false;
      int32_t numFrames = 0;
      int32_t numMarkerFrames = 0;
      int32_t numPFFrames = 0;
      int32_t numEMGFrames = 0;
      int32_t pointFrequency = 0;
      int32_t firstframe = 1;
      int32_t markerFirstframe = 0;
      int32_t FPFirstframe = 0;
      int32_t EMGFirstframe = 0;
      int32_t analogFrequency = 0;
      int analogSampleNumberPerPointFrame = 1;
      int32_t FPFrequency = 0;
      int32_t EMGFrequency = 0;
      int32_t numMarkers = 0;
      int32_t numPFs = 0;
      int32_t numPFChannels = 0;
      int32_t numEMGChannels = 0;
      const BlockEntry* be;
      if (this->SeekToBlock(&bifs, &blockEntries, MarkerBlockId) != 0)
      {
        MarkerBlockFound = true;
        // Header
        numFrames = numMarkerFrames = bifs.ReadI32();
        pointFrequency = bifs.ReadI32();
        firstframe = markerFirstframe = static_cast<int32_t>(bifs.ReadFloat() * (float)pointFrequency) + 1; // startime
        numMarkers = bifs.ReadI32();
      }
      if ((be = this->SeekToBlock(&bifs, &blockEntries, PlatformDataBlockId)) != 0)
      {
        FPBlockFound = true;
        numPFs = bifs.ReadI32(); // Number of force platforms
        FPFrequency = bifs.ReadI32();
        if (MarkerBlockFound)
        {
          FPFirstframe = static_cast<int32_t>(bifs.ReadFloat() * (float)pointFrequency) + 1; // startime
          firstframe = std::min(firstframe, FPFirstframe);
          numPFFrames = bifs.ReadI32();
        }
        else
        {
          firstframe = FPFirstframe = static_cast<int32_t>(bifs.ReadFloat() * (float)FPFrequency) + 1; // startime
          numFrames = numPFFrames = bifs.ReadI32();
        }
        numPFChannels = numPFs * 6;
        if ((be->format >= 5) && (be->format <= 8))
          numPFChannels *= 2;
      }
      if (this->SeekToBlock(&bifs, &blockEntries, EMGBlockId) != 0)
      {
        EMGBlockFound = true;
        numEMGChannels = bifs.ReadI32(); // Number of EMG channels
        EMGFrequency = bifs.ReadI32();
        if (MarkerBlockFound)
        {
          EMGFirstframe = static_cast<int32_t>(bifs.ReadFloat() * (float)pointFrequency) + 1; // startime
          firstframe = std::min(firstframe, EMGFirstframe);
          numEMGFrames = bifs.ReadI32();
        }
        else if (FPBlockFound)
        {
          EMGFirstframe = static_cast<int32_t>(bifs.ReadFloat() * (float)EMGFrequency) + 1; // startime
          firstframe = std::min(firstframe, EMGFirstframe);
          numEMGFrames = bifs.ReadI32();
          numFrames = std::max(numFrames, numEMGFrames);
        }
        else
        {
          firstframe = EMGFirstframe = static_cast<int32_t>(bifs.ReadFloat() * (float)EMGFrequency) + 1; // startime
          numFrames = numEMGFrames = bifs.ReadI32();
        }
      }
      // Check if the analog frequencies are compatible.
      if (FPBlockFound && EMGBlockFound)
      {
        if (FPFrequency != EMGFrequency)
          throw(TDFFileIOException("Only files containing the same sample frequency for EMG and force platform are supported."));
        analogFrequency = FPFrequency;
      }
      else if (FPBlockFound)
        analogFrequency = FPFrequency;
      else if (EMGBlockFound)
        analogFrequency = EMGFrequency;
      // Check compatibility between point sample frequency and analog sample frequency
      if (MarkerBlockFound)
      { 
        if (FPBlockFound || EMGBlockFound)
        {
          if (analogFrequency % pointFrequency != 0)
            throw(TDFFileIOException("Only the analog sample frequencies which are integer multiple of the video frequency are supported."));
          analogSampleNumberPerPointFrame = analogFrequency / pointFrequency;
        }
      }
      else
        pointFrequency = analogFrequency;
      // Test if the number of samples for the force platforms correspond to the number of sample in the acquisition.
      if (numPFFrames  != output->GetAnalogFrameNumber())
      {
        btkIOWarningMacro(filename, "The number of frames for the force platforms will be adapted as it doesn't correspond to the number of analog samples set in the acquisition.");
      }
      // Test if the number of samples for the force platforms correspond to the number of sample in the acquisition.
      if (numEMGFrames  != output->GetAnalogFrameNumber())
      {
        btkIOWarningMacro(filename, "The number of frames for the EMGs will be adapted as it doesn't correspond to the number of analog samples set in the acquisition.");
      }
      const int32_t numAnalogFrames = numFrames * analogSampleNumberPerPointFrame;
      
      // Init the output
      output->Init(numMarkers, numFrames, numPFChannels + numEMGChannels, analogSampleNumberPerPointFrame);
      output->SetPointFrequency(pointFrequency);
      output->SetPointUnit(btk::Point::Marker, "m");
      output->SetPointUnit(btk::Point::Moment, "Nm");
      output->SetFirstFrame(firstframe);
      
      // ------------------------------------------------------------------- //
      //                              Markers
      // ------------------------------------------------------------------- //
      if ((be = this->SeekToBlock(&bifs, &blockEntries, MarkerBlockId)) != 0)
      {
        // No need to read the header?
        bifs.SeekRead(80, BinaryFileStream::Current);
        
        // Links (skipped)
        if ((be->format == 1) || (be->format == 3))
        {
          int32_t numLinks = bifs.ReadI32();
          bifs.SeekRead(4, BinaryFileStream::Current);
          bifs.SeekRead(8*numLinks, BinaryFileStream::Current); // 8: 2 * 4 ; 4: sizeof(int32_t)
        }
        
        // Data
        // - By markers
        if ((be->format == 1) || (be->format == 2))
        {
          Point::Residuals res = Point::Residuals::Constant(numFrames,1,-1.0);
          for (Acquisition::PointIterator it = output->BeginPoint() ; it != output->EndPoint() ; ++it)
          {
            // All the residuals are set to -1 by default
            (*it)->SetResiduals(res);
            // Extract label
            std::string label = bifs.ReadString(256);
            (*it)->SetLabel(this->CleanLabel(label));
            // Extract data
            int32_t numSegments = bifs.ReadI32();
            bifs.SeekRead(4, BinaryFileStream::Current);
            std::vector<int32_t> segments = bifs.ReadI32(2*numSegments);
            
            for (size_t i = 0 ; i < segments.size() ; i+=2)
            {
              const int32_t shift = segments[i] + markerFirstframe - firstframe;
              for (int32_t j = 0 ; j < segments[i+1] ; ++j)
              {
                double x = bifs.ReadFloat();
                double y = bifs.ReadFloat();
                double z = bifs.ReadFloat();
                (*it)->SetDataSlice(j + shift, x, y, z); // Residual is set to 0 by default.
              }
            }
          }
        }
        // - By frame
        else if ((be->format == 3) || (be->format == 4))
        {
          // Extract label
          for (Acquisition::PointIterator it = output->BeginPoint() ; it != output->EndPoint() ; ++it)
          {
            std::string label = bifs.ReadString(256);
            (*it)->SetLabel(this->CleanLabel(label));
          }
          // Extract data
          for (int i = 0 ; i < numMarkerFrames ; ++i)
          {
            const int32_t idx = i + markerFirstframe - firstframe;
            for (Acquisition::PointIterator it = output->BeginPoint() ; it != output->EndPoint() ; ++it)
            {
              double x = bifs.ReadFloat();
              double y = bifs.ReadFloat();
              double z = bifs.ReadFloat();
              if ((x == 0.0) && (y == 0.0) && (z == 0.0))
                (*it)->SetDataSlice(idx, x, y, z, -1.0);
              else
                (*it)->SetDataSlice(idx, x, y, z);
            }
          }
        }
        // - Unknown
        else
          throw(TDFFileIOException("Unknown format for the Data3D block"));
      }
      // ------------------------------------------------------------------- //
      //                            Platform data
      // ------------------------------------------------------------------- //
      bool FPDoubleFormat = false;
      if ((be = this->SeekToBlock(&bifs, &blockEntries, PlatformDataBlockId)) != 0)
      {
        // Header
        // No need to extract the number of platform, the sample frequency, the start time and the number of samples.
        bifs.SeekRead(16, BinaryFileStream::Current); 
        bifs.SeekRead(numPFs*2, BinaryFileStream::Current); // Need of the map?
        // Data
        const int32_t shift = (FPFirstframe - firstframe) * analogSampleNumberPerPointFrame;
        // One force platform - by analog channels
        if ((be->format == 1) || (be->format == 3))
        {
          for (int p = 0 ; p < numPFs ; ++p)
          {
            // Label of the force plateform (skipped)
            if (be->format == 3)
              bifs.SeekRead(256, BinaryFileStream::Current);
            // Label for each analog channel
            // Map the analog channels to fit with the force platform type I
            std::list<Analog::Pointer> analogMap;
            Acquisition::AnalogIterator it = output->BeginAnalog();
            std::advance(it, p*6);
            std::string strIdx = ToString(p+1);
            (*it)->SetLabel("PX" + strIdx); (*it)->SetUnit("m"); analogMap.push_back(*it); ++it;
            (*it)->SetLabel("PY" + strIdx); (*it)->SetUnit("m"); analogMap.push_back(*it); ++it;
            (*it)->SetLabel("FX" + strIdx); (*it)->SetUnit("N"); analogMap.push_back(*it); ++it;
            (*it)->SetLabel("FY" + strIdx); (*it)->SetUnit("N"); analogMap.push_back(*it); ++it;
            (*it)->SetLabel("FZ" + strIdx); (*it)->SetUnit("N"); analogMap.push_back(*it); ++it;
            (*it)->SetLabel("MZ" + strIdx); (*it)->SetUnit("Nm"); analogMap.push_back(*it); ++it;
            // Extract data
            int32_t numSegments = bifs.ReadI32();
            bifs.SeekRead(4, BinaryFileStream::Current);
            std::vector<int32_t> segments = bifs.ReadI32(2*numSegments);
            for (size_t i = 0 ; i < segments.size() ; i+=2)
            {
              for (int32_t j = segments[i] ; j < segments[i]+segments[i+1] ; ++j)
              {
                std::vector<float> data = bifs.ReadFloat(6);
                if (j + shift < numAnalogFrames)
                {
                  int k = 0;
                  for (Acquisition::AnalogIterator it = analogMap.begin() ; it != analogMap.end() ; ++it)
                    (*it)->SetDataSlice(j + shift, data[k++]);
                }
              }
            }
          }
        }
        // One force platform - by frames
        else if ((be->format == 2) || (be->format == 4))
        {
          // Label of the force plateform (skipped)
          if (be->format == 4)
            bifs.SeekRead(numPFs * 256, BinaryFileStream::Current);
          // Label for each analog channel
          // Map the analog channels to fit with the force platform type I
          std::list<Analog::Pointer> analogMap;
          Acquisition::AnalogIterator it;
          it = output->BeginAnalog();
          for (int p = 0 ; p < numPFs ; ++p)
          {
            (*it)->SetLabel("PX" + ToString(p+1));
            (*it)->SetUnit("m"); 
            analogMap.push_back(*it);
            std::advance(it, 6);
          }
          it = output->BeginAnalog(); std::advance(it, 1);
          for (int p = 0 ; p < numPFs ; ++p)
          {
            (*it)->SetLabel("PY" + ToString(p+1));
            (*it)->SetUnit("m"); 
            analogMap.push_back(*it);
            std::advance(it, 6);
          }
          it = output->BeginAnalog(); std::advance(it, 2);
          for (int p = 0 ; p < numPFs ; ++p)
          {
            (*it)->SetLabel("FX" + ToString(p+1));
            (*it)->SetUnit("N"); 
            analogMap.push_back(*it);
            std::advance(it, 6);
          }
          it = output->BeginAnalog(); std::advance(it, 3);
          for (int p = 0 ; p < numPFs ; ++p)
          {
            (*it)->SetLabel("FY" + ToString(p+1));
            (*it)->SetUnit("N"); 
            analogMap.push_back(*it);
            std::advance(it, 6);
          }
          it = output->BeginAnalog(); std::advance(it, 4);
          for (int p = 0 ; p < numPFs ; ++p)
          {
            (*it)->SetLabel("FZ" + ToString(p+1));
            (*it)->SetUnit("N"); 
            analogMap.push_back(*it);
            std::advance(it, 6);
          }
          it = output->BeginAnalog(); std::advance(it, 5);
          for (int p = 0 ; p < numPFs ; ++p)
          {
            (*it)->SetLabel("MZ" + ToString(p+1));
            (*it)->SetUnit("Nm"); 
            analogMap.push_back(*it);
            std::advance(it, 6);
          }
          // Extract data
          const int32_t shift = (FPFirstframe - firstframe) * analogSampleNumberPerPointFrame;
          int32_t numPFFramesFinal = numPFFrames - shift;
          numPFFramesFinal = (numPFFramesFinal >= numAnalogFrames) ? numAnalogFrames : numPFFramesFinal;
          for (int i = 0 ; i < numPFFramesFinal ; ++i)
          {
            for (Acquisition::AnalogIterator it = analogMap.begin() ; it != analogMap.end() ; ++it)
              (*it)->SetDataSlice(i + shift, bifs.ReadFloat());
          }
        }
        // Two force platforms - by analog channels
        else if ((be->format == 5) || (be->format == 7))
        {
          btkErrorMacro("The use of two wrenches by force platform is partially supported. Please contact the developers to improve this part.");
          FPDoubleFormat = true;
          for (int p = 0 ; p < numPFs ; ++p)
          {
            // Label of the force plateform (skipped)
            if (be->format == 7)
              bifs.SeekRead(256, BinaryFileStream::Current);
            // Label for each analog channel
            // Map the analog channels to fit with the force platform type I
            std::list<Analog::Pointer> analogMap;
            Acquisition::AnalogIterator it = output->BeginAnalog();
            std::advance(it, p*12);
            std::string strIdx = ToString(p*2+1) + "a";
            (*it)->SetLabel("PX" + strIdx); (*it)->SetUnit("m"); analogMap.push_back(*it); ++it;
            (*it)->SetLabel("PY" + strIdx); (*it)->SetUnit("m"); analogMap.push_back(*it); ++it;
            (*it)->SetLabel("FX" + strIdx); (*it)->SetUnit("N"); analogMap.push_back(*it); ++it;
            (*it)->SetLabel("FY" + strIdx); (*it)->SetUnit("N"); analogMap.push_back(*it); ++it;
            (*it)->SetLabel("FZ" + strIdx); (*it)->SetUnit("N"); analogMap.push_back(*it); ++it;
            (*it)->SetLabel("MZ" + strIdx); (*it)->SetUnit("Nm"); analogMap.push_back(*it); ++it;
            strIdx[strIdx.length()-1] = 'b';
            (*it)->SetLabel("PX" + strIdx); (*it)->SetUnit("m"); analogMap.push_back(*it); ++it;
            (*it)->SetLabel("PY" + strIdx); (*it)->SetUnit("m"); analogMap.push_back(*it); ++it;
            (*it)->SetLabel("FX" + strIdx); (*it)->SetUnit("N"); analogMap.push_back(*it); ++it;
            (*it)->SetLabel("FY" + strIdx); (*it)->SetUnit("N"); analogMap.push_back(*it); ++it;
            (*it)->SetLabel("FZ" + strIdx); (*it)->SetUnit("N"); analogMap.push_back(*it); ++it;
            (*it)->SetLabel("MZ" + strIdx); (*it)->SetUnit("Nm"); analogMap.push_back(*it); ++it;
            // Extract data
            int32_t numSegments = bifs.ReadI32();
            bifs.SeekRead(4, BinaryFileStream::Current);
            std::vector<int32_t> segments = bifs.ReadI32(2*numSegments);
            for (size_t i = 0 ; i < segments.size() ; i+=2)
            {
              for (int32_t j = segments[i] ; j < segments[i]+segments[i+1] ; ++j)
              {
                std::vector<float> data = bifs.ReadFloat(12);
                if (j + shift < output->GetAnalogFrameNumber())
                {
                  int k = 0;
                  for (Acquisition::AnalogIterator it = analogMap.begin() ; it != analogMap.end() ; ++it)
                    (*it)->SetDataSlice(j + shift, data[k++]);
                }
              }
            }
          }
        }
        // Two force platforms - by frames
        else if ((be->format == 6) || (be->format == 8))
        {
          btkErrorMacro("The use of two wrenches by force platform is partially supported. Please contact the developers to improve this part.");
          FPDoubleFormat = true;
          // Label of the force plateform (skipped)
          if (be->format == 8)
            bifs.SeekRead(numPFs * 256, BinaryFileStream::Current);
          // Label for each analog channel
          // Map the analog channels to fit with the force platform type I
          std::list<Analog::Pointer> analogMap;
          Acquisition::AnalogIterator it;
          it = output->BeginAnalog();
          for (int p = 0 ; p < numPFs ; ++p)
          {
            (*it)->SetLabel("PX" + ToString(p+1) + "a");
            (*it)->SetUnit("m");
            analogMap.push_back(*it);
            std::advance(it, 12);
          }
          it = output->BeginAnalog(); std::advance(it, 1);
          for (int p = 0 ; p < numPFs ; ++p)
          {
            (*it)->SetLabel("PY" + ToString(p+1) + "a");
            (*it)->SetUnit("m");
            analogMap.push_back(*it);
            std::advance(it, 12);
          }
          it = output->BeginAnalog(); std::advance(it, 2);
          for (int p = 0 ; p < numPFs ; ++p)
          {
            (*it)->SetLabel("FX" + ToString(p+1) + "a");
            (*it)->SetUnit("N");
            analogMap.push_back(*it);
            std::advance(it, 12);
          }
          it = output->BeginAnalog(); std::advance(it, 3);
          for (int p = 0 ; p < numPFs ; ++p)
          {
            (*it)->SetLabel("FY" + ToString(p+1) + "a");
            (*it)->SetUnit("N");
            analogMap.push_back(*it);
            std::advance(it, 12);
          }
          it = output->BeginAnalog(); std::advance(it, 4);
          for (int p = 0 ; p < numPFs ; ++p)
          {
            (*it)->SetLabel("FZ" + ToString(p+1) + "a");
            (*it)->SetUnit("N");
            analogMap.push_back(*it);
            std::advance(it, 12);
          }
          it = output->BeginAnalog(); std::advance(it, 5);
          for (int p = 0 ; p < numPFs ; ++p)
          {
            (*it)->SetLabel("MZ" + ToString(p+1) + "a");
            (*it)->SetUnit("Nm");
            analogMap.push_back(*it);
            std::advance(it, 12);
          }
          it = output->BeginAnalog(); std::advance(it, 6);
          for (int p = 0 ; p < numPFs ; ++p)
          {
            (*it)->SetLabel("PX" + ToString(p+1) + "b");
            (*it)->SetUnit("m");
            analogMap.push_back(*it);
            std::advance(it, 12);
          }
          it = output->BeginAnalog(); std::advance(it, 7);
          for (int p = 0 ; p < numPFs ; ++p)
          {
            (*it)->SetLabel("PY" + ToString(p+1) + "b");
            (*it)->SetUnit("m");
            analogMap.push_back(*it);
            std::advance(it, 12);
          }
          it = output->BeginAnalog(); std::advance(it, 8);
          for (int p = 0 ; p < numPFs ; ++p)
          {
            (*it)->SetLabel("FX" + ToString(p+1) + "b");
            (*it)->SetUnit("N");
            analogMap.push_back(*it);
            std::advance(it, 12);
          }
          it = output->BeginAnalog(); std::advance(it, 9);
          for (int p = 0 ; p < numPFs ; ++p)
          {
            (*it)->SetLabel("FY" + ToString(p+1) + "b");
            (*it)->SetUnit("N");
            analogMap.push_back(*it);
            std::advance(it, 12);
          }
          it = output->BeginAnalog(); std::advance(it, 10);
          for (int p = 0 ; p < numPFs ; ++p)
          {
            (*it)->SetLabel("FZ" + ToString(p+1) + "b");
            (*it)->SetUnit("N");
            analogMap.push_back(*it);
            std::advance(it, 12);
          }
          it = output->BeginAnalog(); std::advance(it, 11);
          for (int p = 0 ; p < numPFs ; ++p)
          {
            (*it)->SetLabel("MZ" + ToString(p+1) + "b");
            (*it)->SetUnit("Nm");
            analogMap.push_back(*it);
            std::advance(it, 12);
          }
          // Extract data
          const int32_t shift = (FPFirstframe - firstframe) * analogSampleNumberPerPointFrame;
          int32_t numPFFramesFinal = numPFFrames - shift;
          numPFFramesFinal = (numPFFramesFinal >= numAnalogFrames) ? numAnalogFrames : numPFFramesFinal;
          for (int i = 0 ; i < numPFFramesFinal ; ++i)
          {
            for (Acquisition::AnalogIterator it = analogMap.begin() ; it != analogMap.end() ; ++it)
              (*it)->SetDataSlice(i + shift, bifs.ReadFloat());
          }
        }
        // - Unknown
        else
          throw(TDFFileIOException("Unknown format for the PlatformData block"));
          
        // Revert the data for the forces and moments as the acquisition should contain the raw signal of the force platform and not the reaction.
        const int numPlatforms = FPDoubleFormat ? numPFs * 2 : numPFs;
        for (int i = 0 ; i < numPlatforms ; ++i)
        {
          Acquisition::AnalogIterator it = output->BeginAnalog();
          std::advance(it, i*6+2);
          (*it)->GetValues() *= -1.0; ++it;
          (*it)->GetValues() *= -1.0; ++it;
          (*it)->GetValues() *= -1.0; ++it;
          (*it)->GetValues() *= -1.0; ++it;
        }
      }
      // ------------------------------------------------------------------- //
      //                          Platform config
      // ------------------------------------------------------------------- //
      if ((be = this->SeekToBlock(&bifs, &blockEntries, PlatformConfigBlockId)) != 0)
      {
        int32_t numPFsBis = bifs.ReadI32();
        if (numPFsBis != numPFs)
        {
          btkIOWarningMacro(filename, "The number of force platforms in the configuration is not the same than in the data block.");
        }
        
        bifs.SeekRead(4, BinaryFileStream::Current);
        bifs.SeekRead(numPFsBis*2, BinaryFileStream::Current);
        
        // Need to test 'numPlatforms' ? Should not be physicaly more than 256 force platforms...
        const int8_t numPlatforms = FPDoubleFormat ? numPFsBis * 2 : numPFsBis;
        
        std::vector<float> cornersData;
        for (int i = 0 ; i < numPFsBis ; ++i)
        {
          bifs.SeekRead(256, BinaryFileStream::Current); // Label
          bifs.SeekRead(8, BinaryFileStream::Current); // Size
          std::vector<float> cornersDataTemp = bifs.ReadFloat(12);
          cornersData.insert(cornersData.end(), cornersDataTemp.begin(), cornersDataTemp.end());
          // The corners are not in the same order than the one used in BTK
          // Need to turn them of 180 degrees.
          for (int j = 0 ; j < 6 ; ++j)
          {
            float c = cornersData[i*12+j];
            cornersData[i*12+j] = cornersData[i*12+j+6];
            cornersData[i*12+j+6] = c;
          }
        }
        
        // FORCE_PLATFORM:CORNERS
        if (FPDoubleFormat)
        {
          std::vector<float> cornersDataTemp = cornersData;
          cornersData.resize(cornersData.size() * 2);
          for (int i = 0 ; i < numPFsBis ; ++i)
          {
            for (int j = 0 ; j < 12 ; ++j)
            {
              cornersData[i*24 + j] = cornersDataTemp[i*12 + j];
              cornersData[i*24 + 12 + j] = cornersDataTemp[i*12 + j];
            }
          }
        }
        std::vector<uint8_t> cornersDims = std::vector<uint8_t>(3, 3);
        cornersDims[1] = 4;
        cornersDims[2] = numPlatforms;
        // FORCE_PLATFORM:ORIGIN
        std::vector<float> originData = std::vector<float>(numPlatforms*3, 0.0f);
        std::vector<uint8_t> originDims = std::vector<uint8_t>(2, 3);
        originDims[1] = numPlatforms;
        // FORCE_PLATFORM:CHANNEL
        std::vector<int16_t> channelData = std::vector<int16_t>(numPlatforms*6);
        for (int i = 0 ; i < numPlatforms ; ++i)
        {
          const int idx = 6*i;
          channelData[idx] =   idx + 3;
          channelData[idx+1] = idx + 4;
          channelData[idx+2] = idx + 5;
          channelData[idx+3] = idx + 1;
          channelData[idx+4] = idx + 2;
          channelData[idx+5] = idx + 6;
        }
        std::vector<uint8_t> channelDims = std::vector<uint8_t>(2, 6);
        channelDims[1] = numPlatforms;
        
        MetaData::Pointer forcePlatform = MetaData::New("FORCE_PLATFORM");
        forcePlatform->AppendChild(MetaData::New("USED", numPlatforms));
        forcePlatform->AppendChild(MetaData::New("TYPE", std::vector<int16_t>(numPlatforms,1)));
        forcePlatform->AppendChild(MetaData::New("ZERO", std::vector<int16_t>(2,0)));
        forcePlatform->AppendChild(MetaData::New("CORNERS", cornersDims, cornersData));
        forcePlatform->AppendChild(MetaData::New("ORIGIN", originDims, originData));
        forcePlatform->AppendChild(MetaData::New("CHANNEL", channelDims, channelData));
        output->GetMetaData()->AppendChild(forcePlatform);
      }
      else if (FPBlockFound)
      {
        btkIOWarningMacro(filename, "The block for the configuration of the force platform(s) is missing while the data are present. These data will be only available as analog channels.");
      }
      // ------------------------------------------------------------------- //
      //                                EMG
      // ------------------------------------------------------------------- //
      if ((be = this->SeekToBlock(&bifs, &blockEntries, EMGBlockId)) != 0)
      {
        bifs.SeekRead(16, BinaryFileStream::Current); // Header already extracted
        bifs.SeekRead(numEMGChannels*2, BinaryFileStream::Current); // Need of the map?
        
        // Data
        // - By channels
        if (be->format == 1)
        {
          Acquisition::AnalogIterator it = output->BeginAnalog();
          std::advance(it, numPFChannels);
          while (it != output->EndAnalog())
          {
            Analog::Values val = Analog::Values::Zero(numEMGFrames,1);
            // Extract label
            std::string label = bifs.ReadString(256);
            (*it)->SetLabel(this->CleanLabel(label));
            // Extract data
            int32_t numSegments = bifs.ReadI32();
            bifs.SeekRead(4, BinaryFileStream::Current);
            std::vector<int32_t> segments = bifs.ReadI32(2*numSegments);
            for (size_t i = 0 ; i < segments.size() ; i+=2)
            {
              for (int32_t j = segments[i] ; j < segments[i]+segments[i+1] ; ++j)
                val.data()[j] = bifs.ReadFloat();
            }
            const int32_t shift = (EMGFirstframe - firstframe) * analogSampleNumberPerPointFrame;
            const int32_t numEMGFramesFinal = numEMGFrames - shift;
            memcpy((*it)->GetValues().data() + shift, val.data(), (numEMGFramesFinal >= numAnalogFrames) ? numAnalogFrames : numEMGFramesFinal);
            ++it;
          }
        }
        // - By frame
        else if (be->format == 2)
        {
          // Extract label
          Acquisition::AnalogIterator it = output->BeginAnalog();
          std::advance(it, numPFChannels);
          while (it != output->EndAnalog())
          {
            std::string label = bifs.ReadString(256);
            (*it)->SetLabel(this->CleanLabel(label));
            ++it;
          }
          // Extract data
          const int32_t shift = (EMGFirstframe - firstframe) * analogSampleNumberPerPointFrame;
          int32_t numEMGFramesFinal = numEMGFrames - shift;
          numEMGFramesFinal = (numEMGFramesFinal >= numAnalogFrames) ? numAnalogFrames : numEMGFramesFinal;
          for (int i = 0 ; i < numEMGFramesFinal ; ++i)
          {
            it = output->BeginAnalog();
            std::advance(it, numPFChannels);
            while (it != output->EndAnalog())
            {
              (*it)->SetDataSlice(i + shift, bifs.ReadFloat());
              ++it;
            }
          }
        }
        // - Unknown
        else
          throw(TDFFileIOException("Unknown format for the EMG block"));
      }
      // The orientation of the vertical and horizontal axis are force as it seems
      // that all the TDF files have the Y axis as the vertical axis and the Z axis 
      // for the horizontal axis.
      // - POINT
      btk::MetaData::Pointer point = btk::MetaDataCreateChild(output->GetMetaData(), "POINT");
      // - POINT:X_SCREEN
      btk::MetaDataCreateChild(point, "X_SCREEN", "+Z");
      // - POINT:Y_SCREEN
      btk::MetaDataCreateChild(point, "Y_SCREEN", "+Y");
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
      throw(TDFFileIOException(excmsg));
    }
    catch (TDFFileIOException& )
    {
      if (bifs.IsOpen()) bifs.Close();
      throw;
    }
    catch (std::exception& e)
    {
      if (bifs.IsOpen()) bifs.Close();
      throw(TDFFileIOException("Unexpected exception occurred: " + std::string(e.what())));
    }
    catch(...)
    {
      if (bifs.IsOpen()) bifs.Close();
      throw(TDFFileIOException("Unknown exception"));
    }
  };
  
  /**
   * Constructor.
   */
  TDFFileIO::TDFFileIO()
  : AcquisitionFileIO(AcquisitionFileIO::Binary, AcquisitionFileIO::IEEE_LittleEndian, AcquisitionFileIO::Float)
  {};
  
  const TDFFileIO::BlockEntry* TDFFileIO::SeekToBlock(IEEELittleEndianBinaryFileStream* bifs, const std::list<BlockEntry>* blockEntries, unsigned int id) const
  {
    for (std::list<BlockEntry>::const_iterator it = blockEntries->begin() ; it != blockEntries->end() ; ++it)
    {
      if ((it->type == id) && (it->format != 0))
      {
        bifs->SeekRead(it->offset, BinaryFileStream::Begin);
        return &(*it);
      }
    }
    return 0;
  };
};

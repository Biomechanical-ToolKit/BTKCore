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

#include "btkC3DFileIO.h"
#include "btkMetaDataUtils.h"
#include "btkConvert.h"

#include <algorithm>
#include <cctype>
#include <iostream>

#include <Eigen/Array>

namespace btk
{
  /**
   * @class C3DFileIOException btkC3DFileIO.h
   * @brief Exception class for the C3DFileIO class.
   */
  
  /**
   * @fn C3DFileIOException::C3DFileIOException(const std::string& msg)
   * Constructor.
   */
  
  /**
   * @fn virtual C3DFileIOException::~C3DFileIOException()
   * Empty destructor.
   */
  
  /**
   * @class C3DFileIO btkC3DFileIO.h
   * @brief Interface to read/write C3D files.
   *
   * For more informations on this file's format: http:://www.c3d.org
   *
   * @ingroup BTKIO
   */
  
  /**
   * @enum C3DFileIO::AnalogIntegerFormat
   * Enums used to specify the format used to store analog's data.
   */
  /**
   * @var C3DFileIO::AnalogIntegerFormat C3DFileIO::Signed
   * Analog's data are stored as signed integer values (by default).
   */
  /**
   * @var C3DFileIO::AnalogIntegerFormat C3DFileIO::Unsigned
   * Analog's data are stored as unsigned integer values.
   */

  /**
   * @enum C3DFileIO::WritingFlag
   * Enums used to specify options during the acquisition's exportation into a C3D files.
   */
  /**
   * @var C3DFileIO::WritingFlag C3DFileIO::None
   * Should be used only to erase activated flags.
   */
  /**
   * @var C3DFileIO::WritingFlag C3DFileIO::ScalesFromDataUpdate
   * Updates the scaling factor (point and analog) from the acquisition's data.
   */
  /**
   * @var C3DFileIO::WritingFlag C3DFileIO::ScalesFromMetaDataUpdate
   * Updates the scaling factor (point and analog) from the acquisition's metadata.
   */
  /**
   * @var C3DFileIO::WritingFlag C3DFileIO::MetaDataFromDataUpdate
   * Updates (or synchronize) the acquisition's metadata from the data (by default).
   */
  /**
   * @var C3DFileIO::WritingFlag C3DFileIO::CompatibleVicon
   * Checks and updates (if necessary) acquisitions parameter which can crash Vicon's 
   * product (Polygon, Workstation, Nexus).
   */
  
  /**
   * @typedef C3DFileIO::Pointer
   * Smart pointer associated with a C3DFileIO object.
   */
  
  /**
   * @typedef C3DFileIO::ConstPointer
   * Smart pointer associated with a const C3DFileIO object.
   */
  
  /**
   * @fn static C3DFileIO::Pointer C3DFileIO::New()
   * Create a C3DFileIO object an return it as a smart pointer.
   */

  /**
   * @fn double C3DFileIO::GetPointScale() const
   * Returns the scale for the points obtains from the POINT:SCALE parameter
   * or from its determination for a created acquisition.
   */

  /**
   * @fn void C3DFileIO::SetPointScale(double s)
   * Sets the scale for the points.
   */

  /**
   * @fn AnalogIntegerFormat C3DFileIO::GetAnalogIntegerFormat() const
   * Return the integer format (signed/unsigned).
   */

  /**
   * @fn void C3DFileIO::SetAnalogIntegerFormat(AnalogIntegerFormat f)
   * Sets the integer format used to store analog channels data.
   */

  /**
   * @fn std::vector<double>& C3DFileIO::GetAnalogChannelScale()
   * Returns the vector of scales used for the analog channels' scaling (parameter ANALOG:SCALE).
   */

  /**
   * @fn const std::vector<double>& C3DFileIO::GetAnalogChannelScale() const
   * Returns the vector of scales used for the analog channels' scaling.
   */

  /**
   * @fn void C3DFileIO::SetAnalogChannelScale(const std::vector<double>& s)
   * Sets the vector of scales used for the analog channels' scaling.
   */

  /**
   * @fn std::vector<int>& C3DFileIO::GetAnalogZeroOffset()
   * Returns the vector of offsets used for the analog channels' scaling (parameter ANALOG:OFFSET).
   */

  /**
   * @fn const std::vector<int>& C3DFileIO::GetAnalogZeroOffset() const
   * Returns the vector of offsets used for the analog channels' scaling.
   */

  /**
   * @fn void C3DFileIO::SetAnalogZeroOffset(const std::vector<int>& s)
   * Sets the vector of offsets used for the analog channels' scaling.
   */

  /**
   * @fn double C3DFileIO::GetAnalogUniversalScale() const
   * Returns the universal scale factor used to scale analog channels (parameter ANALOG:GEN_SCALE).
   */

  /**
   * @fn void C3DFileIO::SetAnalogUniversalScale(double s)
   * Sets Returns the universal scale factor used to scale analog channels.
   */

  /**
   * @fn int C3DFileIO::GetWritingFlags() const
   * Returns the flags to use during the acquisition's exportation into a C3D files.
   */

  /**
   * @fn void C3DFileIO::SetWritingFlags(int flags)
   * Sets the flags to use during the acquisition's exportation into a C3D files.
   */

  /**
   * Detects if the flag @a flag is activated.
   */
  bool C3DFileIO::HasWritingFlag(WritingFlag flag)
  {
    if ((this->m_WritingFlags & flag) == flag)
      return true;
    return false;
  };


  /**
   * Checks if the first byte of the file corresponds to C3D header.
   */
  bool C3DFileIO::CanReadFile(const std::string& filename)
  {
    bool isReadable = true;
    NativeBinaryFileStream ifs(filename, BinaryFileStream::In);
    if ((ifs.ReadI8() <= 0) || (ifs.ReadI8() != 80))
      isReadable = false;
    ifs.Close();
    return isReadable;
  };
  
  /**
   * Checks if the suffix of @a filename is C3D.
   */
  bool C3DFileIO::CanWriteFile(const std::string& filename)
  {
    std::string lowercase = filename;
    std::transform(lowercase.begin(), lowercase.end(), lowercase.begin(), tolower);
    std::string::size_type C3DPos = lowercase.rfind(".c3d");
    if ((C3DPos != std::string::npos) && (C3DPos == lowercase.length() - 4))
      return true;
    else
      return false;
  };
  
  /**
   * Read the file designated by @a filename and fill @a output.
   */
  void C3DFileIO::Read(const std::string& filename, Acquisition::Pointer output)
  {
    output->Reset();
    // Open the stream
    BinaryFileStream* ibfs = new NativeBinaryFileStream();
    Format* fdf = 0; // C3D file data format
    ibfs->SetExceptions(BinaryFileStream::EndFileBit | BinaryFileStream::FailBit | BinaryFileStream::BadBit);
    try
    {
    // Binary stream selection
      ibfs->Open(filename, BinaryFileStream::In);
      int8_t parameterFirstBlock = ibfs->ReadI8();
      if (parameterFirstBlock <= 0)
        throw(C3DFileIOException("Bad parameter first block number"));
      if (ibfs->ReadI8() != 80)
        throw(C3DFileIOException("Bad header key"));
      ibfs->SeekRead(512 * (parameterFirstBlock - 1) + 3, BinaryFileStream::Begin);
      BinaryFileStream* oldBFS = ibfs;
      switch (ibfs->ReadI8())
      {
        case IEEE_LittleEndian :
          btkIOErrorMacro(filename, "Wrong processor type. Trying to continue by using the INTEL processor.");
        case IEEE_LittleEndian + 83 : // IEEE LE (Intel)
          this->SetByteOrder(IEEE_LittleEndian);
          ibfs = new IEEELittleEndianBinaryFileStream();
          break;
        case VAX_LittleEndian :
          btkIOErrorMacro(filename, "Wrong processor type. Trying to continue by using the DEC processor.");
        case VAX_LittleEndian + 83 : // VAX LE (DEC)
          this->SetByteOrder(VAX_LittleEndian);
          ibfs = new VAXLittleEndianBinaryFileStream();
          break;
        case IEEE_BigEndian :
          btkIOErrorMacro(filename, "Wrong processor type. Trying to continue by using the MIPS processor.");
        case IEEE_BigEndian + 83 : // IEEE BE (MIPS)
          this->SetByteOrder(IEEE_BigEndian);
          ibfs = new IEEEBigEndianBinaryFileStream();
          break;
        default :
          throw(C3DFileIOException("Invalid processor type"));
          break;
      };
      ibfs->SwapStream(oldBFS);
      delete oldBFS;
      ibfs->SeekRead(2, BinaryFileStream::Begin);
      uint16_t pointNumber = 0,
               totalAnalogSamplesPer3dFrame = 0,
               dataFirstBlock = 0, 
               numberSamplesPerAnalogChannel = 0;
      int lastFrame = 0;
      float pointScaleFactor = 0.0, pointFrameRate = 0.0;
      bool hasHeader = false;
      if (parameterFirstBlock != 1)
      {
        hasHeader = true;
    // Header
        pointNumber = ibfs->ReadU16(); // (word 02)
        totalAnalogSamplesPer3dFrame = ibfs->ReadU16(); // (word 03)
        output->SetFirstFrame(ibfs->ReadU16()); // (word 04)
        lastFrame = ibfs->ReadU16(); // (word 05)
        output->SetMaxInterpolationGap(ibfs->ReadU16()); // (word 06)
        pointScaleFactor = ibfs->ReadFloat(); // (word 07-08)
        if (pointScaleFactor == 0)
          throw(C3DFileIOException("Incorrect 3D scale factor"));
        dataFirstBlock = ibfs->ReadU16(); // (word 09)
        numberSamplesPerAnalogChannel = ibfs->ReadU16(); // (word 10)
        pointFrameRate = ibfs->ReadFloat(); // (word 11-12)
        ibfs->SeekRead(270, BinaryFileStream::Current); // word 13-147 => 135 word 
    // Event in Header section
        const int maxEvents = 18;
        uint16_t labelRangeSection = ibfs->ReadU16(); // (word 148)
        uint16_t labelRangeFirstBlock = ibfs->ReadU16(); // (word 149)
        if (labelRangeSection == 12345)
        {
          btkIOErrorMacro(filename, "This C3D file has a 'Label and Range Section' but BTK doesn't support it again.");
          if (labelRangeFirstBlock < 2)
            btkIOErrorMacro(filename, "The 'Label and Range Section' address is incorrect.");
        }
        uint16_t labelEventFormat = ibfs->ReadU16(); // (word 150)
        EventCollection::Pointer events = output->GetEvents();
        uint16_t eventNumber = ibfs->ReadU16();
        output->SetEventNumber(eventNumber); // (word 151)
        if (eventNumber != 0)
        {
          ibfs->SeekRead(2, BinaryFileStream::Current); // (word 152)
          EventCollection::Iterator it = events->Begin();
          while(it != events->End())
          {
            (*it)->SetTime(ibfs->ReadFloat());
            (*it)->SetFrame(static_cast<int>((*it)->GetTime() * pointFrameRate) + 1);
            ++it;
          }
          ibfs->SeekRead((maxEvents * 4) - (eventNumber * 4), BinaryFileStream::Current);
          // No need to read event's activation status.
          ibfs->SeekRead(maxEvents, BinaryFileStream::Current);
          ibfs->SeekRead(2, BinaryFileStream::Current); // (word 198)
          it = events->Begin();
          if (labelEventFormat == 12345)
          {
            while(it != events->End())
            {
              std::string str = ibfs->ReadString(4);
              str = str.erase(str.find_last_not_of(' ') + 1);
              str = str.erase(0, str.find_first_not_of(' '));
              (*it)->SetLabel(str);
              ++it;
            }
          }
          else
          {
            while(it != events->End())
            {
              std::string str = ibfs->ReadString(2);
              str = str.erase(str.find_last_not_of(' ') + 1);
              str = str.erase(0, str.find_first_not_of(' '));
              (*it)->SetLabel(str);
              ++it;
            }
          }
        }
    // Parameter
        ibfs->SeekRead((512 * (parameterFirstBlock - 1)), BinaryFileStream::Begin);
        // From the C3D documentation:
        // "The first two bytes of the parameter record are only meaningful if they also form the first word of the file"
        // [...]
        // "This is because one common technique for creating C3D files used to be to maintain a parameter “template” as a separate file"
        ibfs->ReadU8();
        ibfs->ReadI8();
      }
      uint8_t blockNumber = ibfs->ReadU8();
      ibfs->ReadU8(); // Processor type
      size_t totalBytesRead = 4; // the four bytes read previously.
      int8_t nbCharLabel = 0;
      int8_t id = 0;
      int offset = 0;
      int8_t type = 0;
      std::vector<uint8_t> dataDim;
      std::list<int8_t> groupIds;
      std::list<int8_t> parameterIds;
      std::list<MetaData::Pointer> parameters;
      MetaData::Pointer root = output->GetMetaData();
      bool alreadyDisplayParameterOverflowMessage = false;
      while (1)
      {
        nbCharLabel = ibfs->ReadI8(); totalBytesRead += abs(nbCharLabel) + 1;
        if (nbCharLabel == 0)
          break; // Parameter section end
        id = ibfs->ReadI8(); totalBytesRead += 1;
        if (id == 0)
          throw(C3DFileIOException("Error during the ID extraction in the parameter section - ID equal to 0"));
        bool lastEntry = false; // Used too to determine the end of the Parameter section
        std::string label = ibfs->ReadString(abs(nbCharLabel));
        offset = ibfs->ReadU16(); totalBytesRead += offset;
        if (offset == 0)
          lastEntry = true;
        offset -= 2;
        if (id < 0)
        {
          MetaData::Pointer entry = MetaData::New(label);
          entry->SetUnlockState((nbCharLabel > 0 ? true : false));
          uint8_t nbCharDesc = ibfs->ReadU8(); offset -= 1;
          entry->SetDescription(ibfs->ReadString(nbCharDesc)); offset -= nbCharDesc;
          if (root->AppendChild(entry))
            groupIds.push_back(id);
        }
        else
        {
          parameterIds.push_back(id);
          type = ibfs->ReadI8(); offset -= 1;
          int8_t nbDim = ibfs->ReadI8(); offset -= 1;
          dataDim = ibfs->ReadU8(nbDim); offset -= nbDim;
          int prod = 1;
          int8_t inc = 0 ; while (inc < nbDim) prod *= dataDim[inc++];
          int sizeData = static_cast<int>(prod * abs(type));
          MetaData::Pointer entry;
          bool unlocked = (nbCharLabel > 0 ? true : false);
          if ((sizeData > offset) && (!lastEntry))
          {
            btkIOErrorMacro(filename, "The size of the data for the parameter '" + label + "' exceeds the space available before the next entry. Trying to continue...");
            switch (type)
            {
              // Note: no need to give the number of elements to the vector as the metadata will resize the data automatically.
              case -1:
                entry = MetaData::New(label, dataDim, std::vector<std::string>(), "", unlocked);
                break;
              case 1:
                entry = MetaData::New(label, dataDim, std::vector<int8_t>(), "", unlocked);
                break;
              case 2:
                entry = MetaData::New(label, dataDim, std::vector<int16_t>(), "", unlocked);
                break;
              case 4:
                entry = MetaData::New(label, dataDim, std::vector<float>(), "", unlocked);
                break;
              default :
                throw(C3DFileIOException("Data parameter type unknown for the entry: '" + label + "'"));
                break;
            }
          }
          else
          {
            switch (type)
            {
              case -1:
                if (dataDim.size() >= 2)
                {
                  int rows = 1; int8_t inc2 = 1 ; while (inc2 < nbDim) rows *= dataDim[inc2++];
                  entry = MetaData::New(label, dataDim, ibfs->ReadString(rows, dataDim[0]), "", unlocked);
                }
                else
                  entry = MetaData::New(label, dataDim, ibfs->ReadString(1,prod), "", unlocked);
                break;
              case 1:
                entry = MetaData::New(label, dataDim, ibfs->ReadI8(prod), "", unlocked);
                break;
              case 2:
                entry = MetaData::New(label, dataDim, ibfs->ReadI16(prod), "", unlocked);
                break;
              case 4:
                entry = MetaData::New(label, dataDim, ibfs->ReadFloat(prod), "", unlocked);
                break;
              default :
                throw(C3DFileIOException("Data parameter type unknown for the entry: '" + label + "'"));
                break;
            }
            offset -= sizeData;
            if (offset != 0)
            {
              uint8_t nbCharDesc = ibfs->ReadU8(); offset -= 1;
              entry->SetDescription(ibfs->ReadString(nbCharDesc)); 
              offset -= nbCharDesc;
            }
            else
              btkIOErrorMacro(filename, "Where is the byte to set the number of characters in the description of the parameter '" + label + "'? Trying to continue...");
          }
          parameters.push_back(entry);
        }
        if (lastEntry)
          offset = 0;
        if (offset < 0)
        {
          btkIOErrorMacro(filename, "Error during the pointing of another parameter|group. Trying to continue...");
          ibfs->SeekRead(offset, BinaryFileStream::Current);
          offset = 0;
        }
        // Checks if the next parameter is not pointing in the Data section.
        if ((totalBytesRead + offset) > static_cast<unsigned int>((blockNumber * 512)))
        {
          if ((totalBytesRead + offset) > static_cast<unsigned int>(((dataFirstBlock - 2) * 512)))
          {
            btkIOErrorMacro(filename, "The next parameter is pointing in the Data section. Parameters' extraction is stopped.");
            totalBytesRead = blockNumber * 512; // Force the number of totalBytesRead to not trigger the "Bad data first block" exception.
            break;
          }
          else if (!alreadyDisplayParameterOverflowMessage)
          {
            btkIOErrorMacro(filename, "The next parameter is pointing outside the parameter section but not yet in the Data section. Trying to continue...");
            alreadyDisplayParameterOverflowMessage = true;
          }
        }
        if (lastEntry)
          break; // Parameter section end
        ibfs->SeekRead(offset, BinaryFileStream::Current);
      }
      std::list<MetaData::Pointer>::iterator itParameter = parameters.begin();
      MetaData::Iterator itGroup = root->Begin();
      std::list<int8_t>::iterator itG = groupIds.begin();
      std::list<int8_t>::iterator itP = parameterIds.begin();
      if (!parameterIds.empty())
      {
        while(1)
        {
          if (itP == parameterIds.end())
          {
            ++itG; ++itGroup;
            itP = parameterIds.begin(); itParameter = parameters.begin();
            if (itG == groupIds.end() || itGroup == root->End() || itP == parameterIds.end() || itParameter == parameters.end())
              break;
          }
          if (*itG == -*itP)
          {
            (*itGroup)->AppendChild(*itParameter);
            itP = parameterIds.erase(itP); 
            itParameter = parameters.erase(itParameter);
          }
          else
          {
            ++itP; ++itParameter;
          }
        }
      }
      if (parameters.size() != 0)
        btkIOErrorMacro(filename, "Some parameters are orphans. No group has the same id. These parameters are lost");
      int totalBlocksRead = static_cast<int>(ceil((double)totalBytesRead / 512.0));
      if (totalBlocksRead != blockNumber)
      {
        btkIOErrorMacro(filename, "The number of blocks to be read in the parameter section is different than the number of blocks read. The value kept is the number of blocks read.");
        blockNumber = totalBlocksRead;
      }
    // Events in Parameter section
      MetaData::ConstIterator itEvent = root->FindChild("EVENT");
      // Took a chance to find the events in EVENTS group instead of EVENT
      if (itEvent == root->End())
      {
        itEvent = root->FindChild("EVENTS");
        if (itEvent != root->End())
          btkIOErrorMacro(filename, "EVENTS group found instead of EVENT. The EVENTS group is used to extract events.");
      }
      if (itEvent != root->End())
      {
        MetaData::ConstIterator itUsed = (*itEvent)->FindChild("USED");
        if (itUsed != (*itEvent)->End())
        {
          int eventsNumber = (*itUsed)->GetInfo()->ToInt(0);
          std::vector<std::string> eventsLabel;
          std::vector<double> eventsTime;
          std::vector<int> eventsId;
          std::vector<std::string> eventsContext;
          std::vector<std::string> eventsSubject;
          std::vector<std::string> eventsDescription;
          MetaDataCollapseChildrenValues<std::string>(eventsLabel, *itEvent, "LABELS", eventsNumber, "uname*");
          MetaDataCollapseChildrenValues(eventsTime, *itEvent, "TIMES");
          if (static_cast<int>(eventsTime.size()) < 2 * eventsNumber)
            btkIOErrorMacro(filename, "The EVENT:TIME doesn't contain the appropriate number of values. The extracted times could be corrupted.")
          eventsTime.resize(2 * eventsNumber, 0.0);
          MetaDataCollapseChildrenValues(eventsContext, *itEvent, "CONTEXTS");
          eventsContext.resize(eventsNumber,"");

          MetaDataCollapseChildrenValues(eventsSubject, *itEvent, "SUBJECTS");
          eventsSubject.resize(eventsNumber,"");

          MetaDataCollapseChildrenValues(eventsDescription, *itEvent, "DESCRIPTIONS");
          eventsDescription.resize(eventsNumber,"");
          
          MetaDataCollapseChildrenValues(eventsId, *itEvent, "ICON_IDS");
          eventsId.resize(eventsNumber, 0);
          
          EventCollection::Pointer events = output->GetEvents();
          for (int incEvt = 0 ; incEvt < eventsNumber ; ++incEvt)
          {
            Event::Pointer evt = Event::New(
                eventsLabel[incEvt],
                eventsTime[2 * incEvt] * 60 + eventsTime[2 * incEvt + 1],
                eventsContext[incEvt],
                Event::Unknown,
                eventsSubject[incEvt],
                eventsDescription[incEvt],
                eventsId[incEvt]);
            evt->SetFrame(static_cast<int>(std::floor((evt->GetTime() * pointFrameRate) + 1.5)));
            events->InsertItem(evt);
          }
        }
      }
    // Configure the acquisition based on some metadata
      // ANALOG
      MetaData::ConstIterator itAnalog = root->FindChild("ANALOG");
      if (itAnalog != root->End())
      {
        // - ANALOG:BITS
        MetaData::ConstIterator itAnalogBits = (*itAnalog)->FindChild("BITS");
        int bits = 0;
        if (itAnalogBits != (*itAnalog)->End())
        {
          bits = (*itAnalogBits)->GetInfo()->ToInt(0);
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
              output->SetAnalogResolution(Acquisition::Bit12);
              break;
          }
        }
        else
          output->SetAnalogResolution(Acquisition::Bit12);
      }
      MetaData::ConstIterator itTrial = root->FindChild("TRIAL");
      // TRIAL
      if (itTrial != root->End())
      {
        // TRIAL:ACTUAL_START_FIELD
        MetaData::ConstIterator itTrialStart = (*itTrial)->FindChild("ACTUAL_START_FIELD");
        if (itTrialStart != (*itTrial)->End())
        {
          int lsb = (*itTrialStart)->GetInfo()->ToUInt16(0);
          int hsb = (*itTrialStart)->GetInfo()->ToUInt16(1);
          int start = hsb << 16 | lsb;
          if (start != output->GetFirstFrame())
          {
            if ((output->GetFirstFrame() != 65535) && (hasHeader)) {btkIOErrorMacro(filename, "The first frame index wrote in the header is different than in the parameter TRIAL:ACTUAL_START_FIELD. The value in the parameter is kept.");}
            output->SetFirstFrame(start);
          }
        }
        // TRIAL:ACTUAL_END_FIELD
        MetaData::ConstIterator itTrialEnd = (*itTrial)->FindChild("ACTUAL_END_FIELD");
        if (itTrialEnd != (*itTrial)->End())
        {
          int lsb = (*itTrialEnd)->GetInfo()->ToUInt16(0);
          int hsb = (*itTrialEnd)->GetInfo()->ToUInt16(1);
          int end = hsb << 16 | lsb;
          if (end != lastFrame)
          {
            if ((lastFrame != 65535) && hasHeader) {btkIOErrorMacro(filename, "The last frame index wrote in the header is different than in the parameter TRIAL:ACTUAL_END_FIELD. The number of frames is modified by keeping the value in the parameter.");}
            lastFrame = end;
          }
        }
      }
      // POINT
      const char* pointTypeNames[] = {"ANGLE", "FORCE", "MOMENT", "POWER", "SCALAR", "REACTION"}; 
      const int numberOfPointTypeNames =  sizeof(pointTypeNames) / sizeof(char*);
      MetaData::ConstIterator itPoint = root->FindChild("POINT");
      if (itPoint != root->End())
      {
        // POINT:UNITS
        MetaData::ConstIterator itPointUnits = (*itPoint)->FindChild("UNITS");
        if (itPointUnits != (*itPoint)->End())
          output->SetPointUnit((*itPointUnits)->GetInfo()->ToString(0));
        // Point's unit for angles, forces, etc.
        for(int i = 0 ; i < numberOfPointTypeNames ; ++i)
        {
          // unit
          MetaData::ConstIterator itU = (*itPoint)->FindChild(std::string(pointTypeNames[i]) + "_UNITS");
          if (itU != (*itPoint)->End())
            output->SetPointUnit(static_cast<Point::Type>(i + 1), (*itU)->GetInfo()->ToString(0));
        }
      }
    // Data
      if (dataFirstBlock != 0)
      {
        if (dataFirstBlock < (parameterFirstBlock + blockNumber))
          throw(C3DFileIOException("Bad data first block"));
        ibfs->SeekRead((512 * (dataFirstBlock - 1)), BinaryFileStream::Begin);
        if (numberSamplesPerAnalogChannel == 0)
          numberSamplesPerAnalogChannel = 1;
        uint16_t analogNumber = totalAnalogSamplesPer3dFrame / numberSamplesPerAnalogChannel;
        // ANALOG
        if (itAnalog != root->End())
        {
          MetaData::ConstIterator itAnalogUsed = (*itAnalog)->FindChild("USED");
          if (itAnalogUsed != (*itAnalog)->End())
          {
            int16_t analogUsed;
            analogUsed = (*itAnalogUsed)->GetInfo()->ToInt(0);
            if (analogNumber != static_cast<uint16_t>(analogUsed))
            {
              btkIOErrorMacro(filename, "The number of analog channels wrote in the header section and in the parameter section are not the same. The value kept is from the header section.");
              //analogNumber = static_cast<uint16_t>(analogUsed);
            }
          }
        }
        this->m_AnalogChannelScale = std::vector<double>(analogNumber, 1.0);
        this->m_AnalogZeroOffset = std::vector<int>(analogNumber, 0);
        this->m_AnalogUniversalScale = 1.0;

        if ((analogNumber != 0) && (itAnalog != root->End()))
        {
          // Check if values in ANALOG:OFFSET correspond to the informations in ANALOG:FORMAT and ANALOG:BITS
          std::vector<int16_t> analogZeroOffset_t;
          MetaDataCollapseChildrenValues<int16_t>(analogZeroOffset_t, *itAnalog, "OFFSET", analogNumber, 0);
          int bits = output->GetAnalogResolution();
          for (unsigned inc = 0 ; inc < this->m_AnalogZeroOffset.size() ; ++inc)
          {
            if (fabs(static_cast<double>(analogZeroOffset_t[inc])) > pow(2.0, bits))
            {
              bits += 2;
              --inc;
            }
          }
          if (bits != output->GetAnalogResolution())
          {
            if (bits >= 16)
            {
              output->SetAnalogResolution(Acquisition::Bit16);
              this->m_AnalogIntegerFormat = Unsigned;
            }
            else
              output->SetAnalogResolution(static_cast<Acquisition::AnalogResolution>(bits));
            btkIOErrorMacro(filename, "Analog format and/or their resolution are inconsistent with analog offsets. They were updated.");
          }
          // - ANALOG:FORMAT
          MetaData::ConstIterator itAnalogFormat = (*itAnalog)->FindChild("FORMAT");
          if (itAnalogFormat != (*itAnalog)->End())
          {
            if ((*itAnalogFormat)->GetInfo()->ToString(0).compare("UNSIGNED") == 0)
              this->m_AnalogIntegerFormat = Unsigned;
            else
              this->m_AnalogIntegerFormat = Signed;
          }
          // - ANALOG:OFFSET
          if (this->m_AnalogIntegerFormat == Unsigned) // unsigned
          {
           for (unsigned inc = 0 ; inc < this->m_AnalogZeroOffset.size() ; ++inc)
             this->m_AnalogZeroOffset[inc] = static_cast<uint16_t>(analogZeroOffset_t[inc]);
          }
          else // signed
            MetaDataCollapseChildrenValues<int>(this->m_AnalogZeroOffset, *itAnalog, "OFFSET", analogNumber, 0);
          // - ANALOG:SCALE
          MetaDataCollapseChildrenValues<double>(this->m_AnalogChannelScale, *itAnalog, "SCALE", analogNumber, 1.0);
          // - ANALOG:GEN_SCALE
          MetaData::ConstIterator itAnalogGenScale = (*itAnalog)->FindChild("GEN_SCALE");
          if (itAnalogGenScale != (*itAnalog)->End())
          {
            this->m_AnalogUniversalScale = (*itAnalogGenScale)->GetInfo()->ToDouble(0);
            if (this->m_AnalogUniversalScale == 0.0)
            {
              btkIOErrorMacro(filename, "Analog universal scaling factor error. Value zero (0) replaced by one (1).");
              this->m_AnalogUniversalScale = 1.0;
            }
          }
        }
        // POINT
        if (itPoint != root->End())
        {
          // POINT:USED
          MetaData::ConstIterator itPointUsed = (*itPoint)->FindChild("USED");
          if (itPointUsed != (*itPoint)->End())
          {
            int16_t pointUsed;
            pointUsed = (*itPointUsed)->GetInfo()->ToInt(0);
            if (pointNumber != static_cast<uint16_t>(pointUsed))
            {
              btkIOErrorMacro(filename, "The number of points wrote in the header section and in the parameter section are not the same. The value kept is from the header section.");
              //pointNumber = static_cast<uint16_t>(pointUsed);
            }
          }
          // POINT:SCALE
          MetaData::ConstIterator itPointScale = (*itPoint)->FindChild("SCALE");
          if (itPointScale != (*itPoint)->End())
          {
            float pointScale = (*itPointScale)->GetInfo()->ToFloat(0);
            if (fabs(pointScale - pointScaleFactor) > std::numeric_limits<float>::epsilon())
            {
              btkIOErrorMacro(filename, "The point scaling factor written in the header and in the parameter POINT:SCALE are not the same. The first value is kept.");
              //pointScaleFactor = pointScale;
            }
          }
        }
        this->m_PointScale = fabs(pointScaleFactor);
        if (pointScaleFactor > 0) // integer
        {
          this->m_StorageFormat = Integer; 
          if (this->m_AnalogIntegerFormat == Unsigned)
            fdf = new IntegerFormatUnsignedAnalog(ibfs);
          else
            fdf = new IntegerFormatSignedAnalog(ibfs);
        }
        else // float
        {
          this->m_StorageFormat = Float;
          if (this->m_AnalogIntegerFormat == Unsigned)
            fdf = new FloatFormatUnsignedAnalog(ibfs);
          else
            fdf = new FloatFormatSignedAnalog(ibfs);
        }
        int frameNumber = lastFrame - output->GetFirstFrame() + 1;
        output->Init(pointNumber, frameNumber, analogNumber, numberSamplesPerAnalogChannel);
        output->SetPointFrequency(pointFrameRate);
        try
        {
          for (int frame = 0 ; frame < frameNumber ; ++frame)
          {
            Acquisition::PointIterator itM = output->BeginPoint(); 
            while (itM != output->EndPoint())
            {
              Point* point = itM->get();
              fdf->ReadPoint(&(point->GetValues().data()[frame]),
                             &(point->GetValues().data()[frame + frameNumber]),
                             &(point->GetValues().data()[frame + 2*frameNumber]),
                             &(point->GetResiduals().data()[frame]),
                             this->m_PointScale);
              ++itM;
            }
            unsigned inc = 0, incChannel = 0, analogFrame = numberSamplesPerAnalogChannel * frame;
            Acquisition::AnalogIterator itA = output->BeginAnalog();
            while (itA != output->EndAnalog())
            {
              (*itA)->GetValues().data()[analogFrame] = (fdf->ReadAnalog() - static_cast<double>(this->m_AnalogZeroOffset[incChannel])) * this->m_AnalogChannelScale[incChannel] * this->m_AnalogUniversalScale;
              ++itA; ++incChannel;
              if ((itA == output->EndAnalog()) && (inc < static_cast<unsigned>(numberSamplesPerAnalogChannel - 1)))
              {
                itA = output->BeginAnalog();
                incChannel = 0;
                ++inc; ++analogFrame;
              }
            }
          }
        }
        catch (BinaryFileStreamFailure& )
        {
          // Let's try to continue even if the file is corrupted
          if (ibfs->EndFile())
          {  
            btkIOErrorMacro(filename, "Some points and/or analog data cannot be extracted and are set as invalid.");
          }
          else
            throw;
        }
    // Label, description, unit and type
        size_t inc = 0; 
        std::vector<std::string> collapsed;
        bool c3dFromMotion = false;
        // POINT Label, description, unit
        if (itPoint != root->End())
        {
          // NOTE: C3D files exported from "Motion Analysis Corp." softwares (EvaRT, Cortex) seem to use POINT:LABELS and POINTS:DESCRIPTIONS as a short and long version of the points' label respectively. Point's Label used in EvaRT and Cortex correspond to values stored in POINTS:DESCRIPTIONS. To distinguish C3D files exported from "Motion Analysis Corp." softwares, it is possible to check the value in the parameter MANUFACTURER:Company.
          // NOTE #2: Moreover, With (at least) Cortex 2.1.1 the occlusion of markers are not set by a mask and residuals equals to -1 but by coordinates set by 9999999 ...
          MetaData::Iterator itManufacturer = root->FindChild("MANUFACTURER");
          if (itManufacturer != root->End())
          {
            MetaData::Iterator itCompany = (*itManufacturer)->FindChild("Company");
            if (itCompany != (*itManufacturer)->End())
            {
              if ((*itCompany)->GetInfo()->ToString(0).compare("Motion Analysis Corp.") == 0)
              {
                c3dFromMotion = true;
                root->RemoveChild(itManufacturer);
              }
            }
          }
          if (!c3dFromMotion)
          {
            // POINT:LABELS
            MetaDataCollapseChildrenValues<std::string>(collapsed, *itPoint, "LABELS", pointNumber, "uname*");
            inc = 0; for (Acquisition::PointIterator it = output->BeginPoint() ; it != output->EndPoint() ; ++it)
              (*it)->SetLabel(collapsed[inc++]);
            // POINT:DESCRIPTIONS
            MetaDataCollapseChildrenValues(collapsed, *itPoint, "DESCRIPTIONS", pointNumber);
            inc = 0; for (Acquisition::PointIterator it = output->BeginPoint() ; it != output->EndPoint() ; ++it)
            {
              if (inc >= collapsed.size())
                break;
              (*it)->SetDescription(collapsed[inc++]);
            }
          }
          else
          {
            // POINT:LABELS
            MetaDataCollapseChildrenValues<std::string>(collapsed, *itPoint, "DESCRIPTIONS", pointNumber, "uname*");
            inc = 0; for (Acquisition::PointIterator it = output->BeginPoint() ; it != output->EndPoint() ; ++it)
              (*it)->SetLabel(collapsed[inc++]);
            // Set correctly coordinates and residuals for occluded markers
            for (Acquisition::PointIterator it = output->BeginPoint() ; it != output->EndPoint() ; ++it)
            {
              Point::Values& coords = (*it)->GetValues();
              Eigen::Matrix<double, Eigen::Dynamic, 1> diff = (coords.rowwise().sum() / 3.0).cwise() - 9999999.0;
              Point::Residuals& res = (*it)->GetResiduals();
              for (int k = 0 ; k < (*it)->GetFrameNumber() ; ++k)
              {
                if (fabs(diff.coeff(k)) < std::numeric_limits<float>::epsilon())
                {
                  coords.coeffRef(k,0) = 0.0;
                  coords.coeffRef(k,1) = 0.0;
                  coords.coeffRef(k,2) = 0.0;
                  res.coeffRef(k) = -1.0;
                }
              }
            }
          }
          // Point's type
          for(int i = 0 ; i < numberOfPointTypeNames ; ++i)
          {
            // type
            MetaDataCollapseChildrenValues(collapsed, *itPoint, std::string(pointTypeNames[i]) + "S");
            for (int j = 0 ; j < static_cast<int>(collapsed.size()) ; ++j)
            {
              Acquisition::PointIterator itPt = output->FindPoint(collapsed[j]);
              if (itPt != output->EndPoint())
                (*itPt)->SetType(static_cast<Point::Type>(i + 1));
            }
          }
        }
        // ANALOG Label, description, unit
        if (itAnalog != root->End())
        {
          if (!c3dFromMotion)
          {
            MetaDataCollapseChildrenValues<std::string>(collapsed, *itAnalog, "LABELS", analogNumber, "uname*");
            inc = 0; for (Acquisition::AnalogIterator it = output->BeginAnalog() ; it != output->EndAnalog() ; ++it)
            {
              (*it)->SetLabel(collapsed[inc]);
              (*it)->SetOffset(this->m_AnalogZeroOffset[inc]);
              (*it)->SetScale(this->m_AnalogChannelScale[inc] * this->m_AnalogUniversalScale);
              ++inc;
            }
            MetaDataCollapseChildrenValues(collapsed, *itAnalog, "DESCRIPTIONS", analogNumber);
            inc = 0; for (Acquisition::AnalogIterator it = output->BeginAnalog() ; it != output->EndAnalog() ; ++it)
            {
              if (inc >= collapsed.size())
                break;
              (*it)->SetDescription(collapsed[inc++]);
            }
          }
          else
          {
            MetaDataCollapseChildrenValues<std::string>(collapsed, *itAnalog, "DESCRIPTIONS", analogNumber, "uname*");
            inc = 0; for (Acquisition::AnalogIterator it = output->BeginAnalog() ; it != output->EndAnalog() ; ++it)
            {
              (*it)->SetLabel(collapsed[inc]);
              (*it)->SetOffset(this->m_AnalogZeroOffset[inc]);
              (*it)->SetScale(this->m_AnalogChannelScale[inc] * this->m_AnalogUniversalScale);
              ++inc;
            }
          }
          MetaDataCollapseChildrenValues(collapsed, *itAnalog, "UNITS", analogNumber);
          inc = 0; for (Acquisition::AnalogIterator it = output->BeginAnalog() ; it != output->EndAnalog() ; ++it)
          {
            if (inc >= collapsed.size())
              break;
            (*it)->SetUnit(collapsed[inc++]);
          }
          // - ANALOG:GAIN
          std::vector<int16_t> gains;
          MetaDataCollapseChildrenValues(gains, *itAnalog, "GAIN");
          inc = 0; 
          for (Acquisition::AnalogIterator it = output->BeginAnalog() ; it != output->EndAnalog() ; ++it)
          {
            if (inc >= gains.size())
              break;
            switch(gains[inc])
            {
            case 0:
              (*it)->SetGain(Analog::Unknown);
              break;
            case 1:
              (*it)->SetGain(Analog::PlusMinus10);
              break;
            case 2:
              (*it)->SetGain(Analog::PlusMinus5);
              break;
            case 3:
              (*it)->SetGain(Analog::PlusMinus2Dot5);
              break;
            case 4:
              (*it)->SetGain(Analog::PlusMinus1Dot25);
              break;
            case 5:
              (*it)->SetGain(Analog::PlusMinus1);
              break;
            default:
              btkIOErrorMacro(filename, "Unknown gain. If the value corresponding to this unknown gain is a real value, please contact a developer to add it in the list.");
              (*it)->SetGain(Analog::Unknown);
              break;
            }
            ++inc;
          }
        }
      }
      else if (lastFrame != 0)
      {
        output->Init(0, lastFrame - output->GetFirstFrame() + 1);
        output->SetPointFrequency(pointFrameRate);
      }  
    }
    catch (BinaryFileStreamFailure& )
    {
      std::string excmsg; 
      if (!ibfs->IsOpen())
        excmsg = "Invalid file path";
      else if (ibfs->EndFile())
        excmsg = "Unexpected end of file";
      else if(ibfs->Bad())
        excmsg = "Loss of integrity of the filestream";
      else if(ibfs->Fail())
        excmsg = "Internal logic operation error on the stream associated with the file";
      else
        excmsg = "Unknown error associated with the filestream";
      
      //if (ifs.is_open()) ifs.close();     
      if (ibfs) delete ibfs;
      if (fdf) delete fdf;
      throw(C3DFileIOException(excmsg));
    }
    catch (C3DFileIOException& )
    {
      if (ibfs) delete ibfs;
      if (fdf) delete fdf;
      throw;
    }
    catch (std::exception& e)
    {
      if (ibfs) delete ibfs;
      if (fdf) delete fdf;
      throw(C3DFileIOException("Unexpected exception occurred: " + std::string(e.what())));
    }
    catch(...)
    {
      if (ibfs) delete ibfs;
      if (fdf) delete fdf;
      throw(C3DFileIOException("Unknown exception"));
    }
    if (ibfs) delete ibfs;
    if (fdf) delete fdf;
  };
  
  /**
   * Write the file designated by @a filename with the content of @a input.
   *
   * Before writing the acquisition in a C3D file, this method do some operation. In the order:
   *  - update C3D scaling factors from acquisition's data (see flag C3DFileIO::ScalesFromDataUpdate)
   *  - synchronize acquisition's data with meta-data required in a C3D file (see flag C3DFileIO::MetaDataFromDataUpdate).
   *  These flag can be disabled by using the method C3DFileIO::SetWriteFlag(). The required C3D scaling factor can also be updated by acquisition's metadata (see flag C3DFileIO::ScalesFromMetaDataUpdate) ;
   */
  void C3DFileIO::Write(const std::string& filename, Acquisition::Pointer input)
  {
    if (!input)
    {
      btkErrorMacro("Null acquisition.");
      return;
    }

    BinaryFileStream* obfs = 0;
    Format* fdf = 0; // C3D file data format
    try
    {
      // Binary stream selection
      switch(this->GetByteOrder())
      {
        case IEEE_LittleEndian : // IEEE LE (Intel)
          obfs = new IEEELittleEndianBinaryFileStream();
          break;
        case VAX_LittleEndian : // VAX LE (DEC)
          obfs = new VAXLittleEndianBinaryFileStream();
          break;
        case IEEE_BigEndian : // IEEE BE (MIPS)
          obfs = new IEEEBigEndianBinaryFileStream();
          break;
        default :
          throw(C3DFileIOException("Invalid processor type"));
          break;
      }
      // File access
      obfs->Open(filename, BinaryFileStream::Out | BinaryFileStream::Truncate);
      if (!obfs->IsOpen())
        throw(C3DFileIOException("No File access"));
      
      // Update data in the acquisition
      // Require to clone some data from the input.
      Acquisition::Pointer in = Acquisition::New();
      in->SetFirstFrame(input->GetFirstFrame());
      in->SetPointFrequency(input->GetPointFrequency());
      in->SetAnalogResolution(input->GetAnalogResolution());
      in->SetEvents(input->GetEvents()->Clone());
      in->SetMetaData(input->GetMetaData()->Clone());
      in->SetPointUnits(input->GetPointUnits());
      for (Acquisition::PointConstIterator it = input->BeginPoint() ; it != input->EndPoint() ; ++it)
        in->AppendPoint(Point::New((*it)->GetLabel(), (*it)->GetType(), (*it)->GetDescription()));
      for (Acquisition::AnalogConstIterator it = input->BeginAnalog() ; it != input->EndAnalog() ; ++it)
      {
        Analog::Pointer analog = Analog::New((*it)->GetLabel(), (*it)->GetDescription());
        analog->SetGain((*it)->GetGain());
        in->AppendAnalog(analog);
      }
      int frameNumber = input->GetPointFrameNumber();
      
      if (this->HasWritingFlag(CompatibleVicon))
        this->KeepAcquisitionCompatibleVicon(in);
      if (this->HasWritingFlag(ScalesFromDataUpdate))
        this->UpdateScalingFactorsFromData(input);
      if (this->HasWritingFlag(ScalesFromMetaDataUpdate))
        this->UpdateScalingFactorsFromMetaData(in);
      if (this->HasWritingFlag(MetaDataFromDataUpdate))
        this->UpdateMetaDataFromData(in, frameNumber, input->GetNumberAnalogSamplePerFrame());
      
      // Acquisition
      bool templateFile = true;
      size_t writtenBytes = 0;
      uint16_t numberSamplesPerAnalogChannel = 1;
      // If there is any point or analog channel or if at least the frequency was set, then it is not a template file.
      if (!input->IsEmptyPoint() || !input->IsEmptyAnalog() || (input->GetPointFrequency() != 0.0))
      {
        templateFile = false;
      // -= HEADER =-
        // The number of the first block of the Parameter section
        writtenBytes += obfs->Write(static_cast<int8_t>(2));
        // C3D header key
        writtenBytes += obfs->Write(static_cast<int8_t>(80));
        // Number of points
        writtenBytes += obfs->Write(static_cast<uint16_t>(input->GetPointNumber()));
        // Total number of analog samples per 3d frame
        writtenBytes += obfs->Write(static_cast<uint16_t>(input->GetAnalogNumber() * input->GetNumberAnalogSamplePerFrame()));
        // First frame
        writtenBytes += obfs->Write(static_cast<uint16_t>(input->GetFirstFrame() > 65535 ? 65535 : input->GetFirstFrame()));
        // Last frame
        writtenBytes += obfs->Write(static_cast<uint16_t>(input->GetLastFrame() > 65535 ? 65535 : input->GetLastFrame()));
        // Maximum interpolation gap in 3D frames
        writtenBytes += obfs->Write(static_cast<uint16_t>(input->GetMaxInterpolationGap()));
        // The 3D scale factor
        writtenBytes += obfs->Write(static_cast<float>(this->m_PointScale * static_cast<double>(this->m_StorageFormat)));
        // The (false) number of the first block of the Data section
        writtenBytes += obfs->Write(static_cast<uint16_t>(0));
        // The number of analog samples per analog channel
        numberSamplesPerAnalogChannel = static_cast<uint16_t>(input->GetNumberAnalogSamplePerFrame());
        writtenBytes += obfs->Write(numberSamplesPerAnalogChannel);
        // The 3D frame rate
        writtenBytes += obfs->Write(static_cast<float>(in->GetPointFrequency())); // Use updated value
        // For future used : word 13-147 => 135 words unused => 270 bytes
        writtenBytes += obfs->Fill(270);
        // Label and Range data
        writtenBytes += obfs->Write(static_cast<uint16_t>(0));
        // The first block of the Label and Range section
        writtenBytes += obfs->Write(static_cast<uint16_t>(0));
        // The event label format. 
        writtenBytes += obfs->Write(static_cast<uint16_t>(12345));
        // Number of defined time events
        writtenBytes += obfs->Write(static_cast<int8_t>(0));
        // Word 152 : Reserved for future use
        writtenBytes += obfs->Fill(2);
        // Event time
        writtenBytes += obfs->Fill(4 * 18);
        // Event display flags
        writtenBytes += obfs->Fill(1 * 18);
        // Word 198 : Reserved for future use
        writtenBytes += obfs->Fill(2);
        // Event labels
        writtenBytes += obfs->Fill(4 * 1 * 18);
        // Fill the end of the header section with 0x00
        obfs->Fill(512 - writtenBytes);
      }
      // -= PARAMETER =-
      writtenBytes = 0;
      // The number of the first block of the Parameter data in the Parameter section
      writtenBytes += obfs->Write(static_cast<int8_t>(1));
       // C3D header key
      writtenBytes += obfs->Write(static_cast<int8_t>(80));
      // The (false) number of parameter block. This data is re-write at the end of this function
      writtenBytes += obfs->Write(static_cast<int8_t>(0));
      // The processor type
      writtenBytes += obfs->Write(static_cast<int8_t>(this->GetByteOrder() + 83));
      // POINT:DATA_START init
      int pointID = -1;
      MetaData::Pointer dataStart;
      uint16_t dS = 0;
      if (!templateFile)
      {
        MetaData::Iterator itPoint = in->GetMetaData()->FindChild("POINT");
        pointID = static_cast<int>(std::distance(in->GetMetaData()->Begin(), itPoint) + 1);
        MetaData::Iterator itDataStart = (*itPoint)->FindChild("DATA_START");
        if (itDataStart == (*itPoint)->End())
          dataStart = MetaData::New("DATA_START", static_cast<int16_t>(0));
        else
        {
          dataStart = (*itDataStart);
          (*itPoint)->RemoveChild(itDataStart);
        }
      }
      // MetaData
      int id = 1;
      for (MetaData::ConstIterator it = in->GetMetaData()->Begin() ; it != in->GetMetaData()->End() ; ++it)
      {
        writtenBytes += this->WriteMetaData(obfs, *it, id);
        ++id;
      }
      // POINT:DATA_START final
      if (!templateFile)
      {
        size_t totalWrittenBytes = writtenBytes + (1 + 1 + dataStart->GetLabel().length() + 2 + 1 + 1 + dataStart->GetInfo()->GetDimensions().size() + (dataStart->GetInfo()->GetValues().size() * abs(dataStart->GetInfo()->GetFormat())) + 1 + dataStart->GetDescription().length());
        totalWrittenBytes += (512 - (totalWrittenBytes % 512));
        uint8_t pNB = static_cast<uint8_t>(totalWrittenBytes / 512);
        dS = 2 + pNB;
        dataStart->GetInfo()->SetValues(static_cast<int16_t>(dS));
        writtenBytes += this->WriteMetaData(obfs, dataStart, pointID);
        writtenBytes += obfs->Fill(512 - (writtenBytes % 512));
        // DATA_START is reinserted.
        //in->GetMetaData()->GetChild(pointID - 1)->AppendChild(dataStart);
        // Back to the parameter: number of blocks
        obfs->SeekWrite(512 * (2 - 1) + 2, BinaryFileStream::Begin);
        obfs->Write(pNB);
        // Back to the header: data first block
        obfs->SeekWrite(16, BinaryFileStream::Begin);
        obfs->Write(dS);
      }
      else
      {
        writtenBytes += obfs->Fill(512 - (writtenBytes % 512));
        uint8_t pNB = static_cast<uint8_t>(writtenBytes / 512);
        // Back to the parameter: number of blocks
        obfs->SeekWrite(2, BinaryFileStream::Begin);
        obfs->Write(pNB);
      }
      if (writtenBytes > (255 * 512)) // 255 * 512 = max size
        throw(C3DFileIOException("Total size reserved for the parameters was exceeded. Impossible to write the acquisition in a C3D file."));
      // -= DATA =-
      if (!templateFile)
      {
        obfs->SeekWrite(512 * (dS - 1), BinaryFileStream::Begin);
        if (this->m_StorageFormat == Integer) // integer
        {
          if (this->m_AnalogIntegerFormat == Unsigned)
            fdf = new IntegerFormatUnsignedAnalog(obfs);
          else
            fdf = new IntegerFormatSignedAnalog(obfs);
        }
        else // float
        {
          if (this->m_AnalogIntegerFormat == Unsigned)
            fdf = new FloatFormatUnsignedAnalog(obfs);
          else
            fdf = new FloatFormatSignedAnalog(obfs);
        }
        for (int frame = 0 ; frame < frameNumber ; ++frame)
        {
          Acquisition::PointConstIterator itM = input->BeginPoint();
          while (itM != input->EndPoint())
          {
            Point* point = itM->get();
            fdf->WritePoint(point->GetValues().data()[frame],
                            point->GetValues().data()[frame + frameNumber],
                            point->GetValues().data()[frame + 2*frameNumber],
                            point->GetResiduals().data()[frame],
                            this->m_PointScale);
            ++itM;
          }
          
          size_t inc = 0, incChannel = 0, analogFrame = numberSamplesPerAnalogChannel * frame;
          Acquisition::AnalogConstIterator itA = input->BeginAnalog();
          while (itA != input->EndAnalog())
          {
            fdf->WriteAnalog(
                (*itA)->GetValues().data()[analogFrame]
                / this->m_AnalogChannelScale[incChannel]
                / this->m_AnalogUniversalScale
                + this->m_AnalogZeroOffset[incChannel]);
            ++itA; ++incChannel;
            if ((itA == input->EndAnalog()) && (inc < static_cast<size_t>(numberSamplesPerAnalogChannel - 1)))
            {
              itA = input->BeginAnalog();
              incChannel = 0;
              ++inc; ++analogFrame;
            }
          }
        }
      }
    }
    catch (C3DFileIOException& )
    {
      if (obfs) delete obfs;
      if (fdf) delete fdf;
      throw;
    }
    catch (std::exception& e)
    {
      if (obfs) delete obfs;
      if (fdf) delete fdf;
      throw(C3DFileIOException("Unexpected exception occurred: " + std::string(e.what())));
    }
    catch(...)
    {
      if (obfs) delete obfs;
      if (fdf) delete fdf;
      throw(C3DFileIOException("Unknown exception"));
    }
    if (obfs) delete obfs;
    if (fdf) delete fdf;
  };
  
  /**
   * Constructor.
   */
  C3DFileIO::C3DFileIO()
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
  : AcquisitionFileIO(AcquisitionFileIO::Binary, AcquisitionFileIO::IEEE_BigEndian, AcquisitionFileIO::Float),
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
  : AcquisitionFileIO(AcquisitionFileIO::Binary, AcquisitionFileIO::VAX_LittleEndian, AcquisitionFileIO::Float),
#else
  : AcquisitionFileIO(AcquisitionFileIO::Binary, AcquisitionFileIO::IEEE_LittleEndian, AcquisitionFileIO::Float),
#endif  
    m_AnalogChannelScale(),
    m_AnalogZeroOffset()
  {
    this->m_PointScale = 0.1;
    this->m_AnalogUniversalScale = 1.0;
    this->m_AnalogIntegerFormat = Signed;
    this->m_WritingFlags = ScalesFromDataUpdate | MetaDataFromDataUpdate | CompatibleVicon;  
  };

  /*
   * Recursive method to write meta data entry and its children
   */
  size_t C3DFileIO::WriteMetaData(BinaryFileStream* obfs, MetaData::ConstPointer m, int id)
  {
    size_t writtenBytes = 0;
    // Number of characters in the name
    writtenBytes += obfs->Write(static_cast<int8_t>(m->GetLabel().length() * (m->GetUnlockState() ? 1 : -1)));
    if (m->HasInfo()) // Parameter
    {
      // ID number
      writtenBytes += obfs->Write(static_cast<int8_t>(id));
      // Name
      writtenBytes += obfs->Write(m->GetLabel());
      // The Offset to the next parameter/group
      MetaDataInfo::ConstPointer mv = m->GetInfo();
      writtenBytes += obfs->Write(static_cast<uint16_t>(2 + 1 + 1 + mv->GetDimensions().size() + (mv->GetDimensionsProduct() * abs(mv->GetFormat())) + 1 + m->GetDescription().length()));
      // Format
      writtenBytes += obfs->Write(static_cast<int8_t>(mv->GetFormat()));
      // Dimension's size
      writtenBytes += obfs->Write(static_cast<uint8_t>(mv->GetDimensions().size()));
      // Dimension's value
      writtenBytes += obfs->Write(mv->GetDimensions());
      // Data
      switch(mv->GetFormat())
      {
        case -1:
          // Not necessary to transform in char
          writtenBytes += obfs->Write(mv->ToString());
          break;
        case 1:
          writtenBytes += obfs->Write(mv->ToInt8());
          break;
        case 2:
          writtenBytes += obfs->Write(mv->ToInt16());
          break;
        case 4:
          writtenBytes += obfs->Write(mv->ToFloat());
          break;
        default: // Should be impossible!
          throw(C3DFileIOException("Parameter's data format unknown."));
          break;
      }
      // Number of characters in the description
      writtenBytes += obfs->Write(static_cast<uint8_t>(m->GetDescription().length()));
      // The description
      writtenBytes += obfs->Write(m->GetDescription());
    }
    else // Group
    {
      // ID number
      writtenBytes += obfs->Write(static_cast<int8_t>(-1 * id));
      // Name
      writtenBytes += obfs->Write(m->GetLabel());
      // The Offset to the next parameter/group
      writtenBytes += obfs->Write(static_cast<uint16_t>(2 + 1 + m->GetDescription().length()));
      // Number of characters in the description
      writtenBytes += obfs->Write(static_cast<uint8_t>(m->GetDescription().length()));
      // The description
      writtenBytes += obfs->Write(m->GetDescription());
    }
    for (MetaData::ConstIterator it = m->Begin() ; it != m->End() ; ++it)
      writtenBytes += this->WriteMetaData(obfs, *it, id);
    return writtenBytes;
  };

  /**
   * Check and adapt the acquistion to be compatible with Polygon.
   *
   * Due to an unknown reason (bug?) Vicon's products (Polygon, Workstation and maybe Nexus) crash
   * if descriptions are empty. Then this method check if descriptions are empty and fill them
   * with 4 blank spaces.
   */
  void C3DFileIO::KeepAcquisitionCompatibleVicon(Acquisition::Pointer input)
  {
    const std::string blank = std::string(4, ' ');
    // Frequency
    if (input->GetPointFrequency() == 0.0)
    {
      btkErrorMacro("Acquisition frequency cannot be null and is set to 50 Hz.");
      input->SetPointFrequency(50.0);
    }
    // Point
    for (Acquisition::PointConstIterator it = input->BeginPoint() ; it != input->EndPoint() ; ++it)
    {
      if ((*it)->GetDescription().empty())
        (*it)->SetDescription(blank);
    }
    // Analog channel
    for (Acquisition::AnalogConstIterator it = input->BeginAnalog() ; it != input->EndAnalog() ; ++it)
    {
      if ((*it)->GetDescription().empty())
        (*it)->SetDescription(blank);
    }
    // ANALYSIS
    MetaData::Iterator itAnalysis = input->GetMetaData()->FindChild("ANALYSIS");
    if (itAnalysis != input->GetMetaData()->End())
    {
      int num = 0;
      MetaData::Iterator it = (*itAnalysis)->FindChild("USED");
      if (it != (*itAnalysis)->End())
      {
        if ((*it)->HasInfo())
          if ((*it)->GetInfo()->GetDimensionsProduct() != 0)
            num = (*it)->GetInfo()->ToInt(0);
      }
      it = (*itAnalysis)->FindChild("DESCRIPTIONS");
      if (it != (*itAnalysis)->End())
      {
        if ((*it)->HasInfo())
        {
          if ((*it)->GetInfo()->GetDimensionsProduct() == 0)
          {
            std::vector<uint8_t> dims = std::vector<uint8_t>(2, 4); // 4: number of blank spaces.
            dims[1] = num;
            (*it)->GetInfo()->SetValues(dims, std::vector<std::string>(num, blank));
          }
        }
      }
      it = (*itAnalysis)->FindChild("SUBJECTS");
      if (it != (*itAnalysis)->End())
      {
        if ((*it)->HasInfo())
        {
          if ((*it)->GetInfo()->GetDimensionsProduct() == 0)
          {
            std::vector<uint8_t> dims = std::vector<uint8_t>(2, 4); // 4: number of blank spaces.
            dims[1] = num;
            (*it)->GetInfo()->SetValues(dims, std::vector<std::string>(num, blank));
          }
        }
      }
    }
  };
  
  /**
   * Update scaling factors from acquisition's data.
   */
  void C3DFileIO::UpdateScalingFactorsFromData(Acquisition::Pointer input)
  {
    // POINT:SCALE
    double max = 0.0;
    for (Acquisition::PointConstIterator itPoint = input->BeginPoint() ; itPoint != input->EndPoint() ; ++itPoint)
      max = std::max(max, (*itPoint)->GetValues().cwise().abs().maxCoeff());
    const int currentMax = static_cast<int>(this->m_PointScale * 32000);
    // Guess to compute a new point scaling factor.
    if (((max > currentMax) || (max <= (currentMax / 2))) && (max > std::numeric_limits<double>::epsilon()))
      this->m_PointScale = max / 32000.0;
    // ANALOG:SCALE & ANALOG:OFFSET
    int analogNumber = input->GetAnalogNumber();
    size_t inc = 0;
    this->m_AnalogChannelScale.resize(analogNumber, 1.0);
    this->m_AnalogZeroOffset.resize(analogNumber, 0);
    double minAnalogscale = 1.0;
    for (Acquisition::AnalogConstIterator itAnalog = input->BeginAnalog() ; itAnalog != input->EndAnalog() ; ++itAnalog)
    {
      this->m_AnalogChannelScale[inc] = (*itAnalog)->GetScale();
      this->m_AnalogZeroOffset[inc] = (*itAnalog)->GetOffset();
      if ((*itAnalog)->GetScale() < minAnalogscale)
        minAnalogscale = (*itAnalog)->GetScale();
      ++inc;
    }
    // ANALOG:GEN_SCALE
    this->m_AnalogUniversalScale = 1.0;
    if (minAnalogscale < 1.0e-5)
    {
      this->m_AnalogUniversalScale = minAnalogscale / (minAnalogscale * 1.0e5);
      for (size_t i = 0 ; i < this->m_AnalogChannelScale.size() ; ++i)
        this->m_AnalogChannelScale[i] /= this->m_AnalogUniversalScale;
    }
    // ANALOG:FORMAT
    this->m_AnalogIntegerFormat = Signed;
    MetaData::ConstIterator itAnalog = input->GetMetaData()->FindChild("ANALOG");
    if (itAnalog != input->GetMetaData()->End())
    {
      MetaData::ConstIterator itAnalogFormat = (*itAnalog)->FindChild("FORMAT");
      if ((itAnalogFormat != (*itAnalog)->End()) && (*itAnalogFormat)->HasInfo())
      {
        if ((*itAnalogFormat)->GetInfo()->ToString(0).compare("UNSIGNED") == 0)
          this->m_AnalogIntegerFormat = Unsigned;
      }
    }
  };
  
  /**
   * Update scaling factors from acquisition's metada.
   */
  void C3DFileIO::UpdateScalingFactorsFromMetaData(Acquisition::Pointer input)
  {
    size_t analogNumber = input->GetAnalogNumber();
    if (analogNumber != 0)
    {
      // ANALOG
      MetaData::ConstIterator itAnalog = input->GetMetaData()->FindChild("ANALOG");
      if (itAnalog == input->GetMetaData()->End())
      {
        btkErrorMacro("No ANALOG group. Impossible to update analog scaling factors.");
      }
      else
      {
        // - ANALOG:FORMAT
        MetaData::ConstIterator itAnalogFormat = (*itAnalog)->FindChild("FORMAT");
        if (itAnalogFormat != (*itAnalog)->End())
        {
          const std::string analogFormat = "UNSIGNED";
          if ((*itAnalogFormat)->GetInfo()->ToString(0).compare(analogFormat) == 0)
            this->m_AnalogIntegerFormat = Unsigned;
          else
            this->m_AnalogIntegerFormat = Signed;
        }
        // - ANALOG:BITS
        MetaData::ConstIterator itAnalogResolution = (*itAnalog)->FindChild("BITS");
        if (itAnalogResolution != (*itAnalog)->End())
        {
          int bits;
          bits = (*itAnalogResolution)->GetInfo()->ToInt(0);
          if ((bits != 8) && (bits != 10) && (bits != 12) && (bits != 14) && (bits != 16))
          {
            btkErrorMacro("Unknown analog resolution. Default resolution (12 bits) added for this acquisition.");
            input->SetAnalogResolution(Acquisition::Bit12);
          }
          else
            input->SetAnalogResolution(static_cast<Acquisition::AnalogResolution>(bits));
        }
        // - ANALOG:OFFSET
        MetaData::ConstIterator itAnalogOffset = (*itAnalog)->FindChild("OFFSET");
        if (itAnalogOffset != (*itAnalog)->End())
        {
          if ((*itAnalogOffset)->GetInfo()->GetValues().size() < analogNumber)
          {
            btkErrorMacro("No enough analog offsets. Impossible to update analog offsets.");
          }
          else
          {
            if (this->m_AnalogIntegerFormat == Unsigned) // unsigned
            {
              std::vector<int16_t> analogZeroOffset_t;
              (*itAnalogOffset)->GetInfo()->ToInt16(analogZeroOffset_t);
              size_t mini = ((analogZeroOffset_t.size() > this->m_AnalogZeroOffset.size()) ? this->m_AnalogZeroOffset.size() : analogZeroOffset_t.size());
              for (size_t inc = 0 ; inc < mini ; ++inc)
                this->m_AnalogZeroOffset[inc] = static_cast<uint16_t>(analogZeroOffset_t[inc]);
            }
            else // signed
            {
              (*itAnalogOffset)->GetInfo()->ToInt(this->m_AnalogZeroOffset);
              this->m_AnalogZeroOffset.resize(analogNumber, 0);
            }
          }
        }
        else
        {
          btkErrorMacro("No ANALOG:OFFSET parameter. Impossible to update analog offsets.");
        }
        // - ANALOG:SCALE
        MetaData::ConstIterator itAnalogScale = (*itAnalog)->FindChild("SCALE");
        if (itAnalogScale != (*itAnalog)->End())
        {
          if ((*itAnalogScale)->GetInfo()->GetValues().size() < analogNumber)
          {
            btkErrorMacro("No enough analog scaling factors. Impossible to update analog offsets.");
          }
          else
            (*itAnalogScale)->GetInfo()->ToDouble(this->m_AnalogChannelScale);
        }
        else
        {
          btkErrorMacro("No ANALOG:SCALE parameter. Impossible to update analog scaling factors.");
        }
        // - ANALOG:GEN_SCALE
        MetaData::ConstIterator itAnalogGenScale = (*itAnalog)->FindChild("GEN_SCALE");
        if (itAnalogGenScale != (*itAnalog)->End())
          this->m_AnalogUniversalScale = (*itAnalogGenScale)->GetInfo()->ToDouble(0);
        else
        {
          btkErrorMacro("No ANALOG:GEN_SCALE parameter. Impossible to update analog scaling factors.");
        }
      }
    }
    if (input->GetPointNumber() != 0)
    {
      // POINT
      MetaData::ConstIterator itPoint = input->GetMetaData()->FindChild("POINT");
      if (itPoint == input->GetMetaData()->End())
      {
        btkErrorMacro("No POINT group. Impossible to update point scaling factor.");
      }
      else
      {
        // POINT:SCALE
        MetaData::ConstIterator itPointScale = (*itPoint)->FindChild("SCALE");
        if (itPointScale != (*itPoint)->End())
        {
          double pointScale = (*itPointScale)->GetInfo()->ToDouble(0);
          this->m_PointScale = fabs(pointScale);
          if (pointScale > 0)
            this->m_StorageFormat = Integer;
          else
            this->m_StorageFormat = Float;
        }
        else
        {
          btkErrorMacro("No POINT:SCALE parameter. Impossible to update point scaling factor.");
        }
      }
    }
  };

  /**
   * Update acquisition's metadata from its data.
   */
  void C3DFileIO::UpdateMetaDataFromData(Acquisition::Pointer input, int numberOfFrames, int numberAnalogSamplePerFrame)
  {
    // POINT group
    // -----------
    MetaData::Pointer point = MetaDataCreateChild(input->GetMetaData(), "POINT");
    int pointNumber = input->GetPointNumber();
    // POINT:USED
    MetaDataCreateChild(point, "USED", static_cast<int16_t>(pointNumber));
    // POINT:SCALE
    MetaDataCreateChild(point, "SCALE", static_cast<float>(this->m_PointScale * static_cast<double>(this->m_StorageFormat)));
    // POINT:RATE
    MetaDataCreateChild(point, "RATE", static_cast<float>(input->GetPointFrequency()));
    // POINT:DATA_START (Updated dynamicaly during the file writing)
    // POINT:FRAMES
    MetaDataCreateChild(point, "FRAMES", static_cast<int16_t>(numberOfFrames > 65535 ? 65535 : numberOfFrames));
    // POINT:LABELS & POINT:DESCRIPTIONS (LABELS2, DESCRIPTIONS2, ...)
    std::vector<std::string> labels = std::vector<std::string>(pointNumber);
    std::vector<std::string> descs = std::vector<std::string>(pointNumber);
    size_t inc = 0;
    for (Acquisition::PointConstIterator itPoint = input->BeginPoint() ; itPoint != input->EndPoint() ; ++itPoint)
    {
      labels[inc] = (*itPoint)->GetLabel();
      descs[inc] = (*itPoint)->GetDescription();
      ++inc;
    }
    MetaDataCreateChild(point, "LABELS", labels);
    MetaDataCreateChild(point, "DESCRIPTIONS", descs);
    // POINT:UNITS
    MetaDataCreateChild(point, "UNITS", input->GetPointUnit(Point::Marker));
    // POINT:TYPE_GROUPS (init)
    std::vector<std::string> typeGroups; 
    // POINT:ANGLES & POINT:ANGLE_UNITS
    this->UpdateMetaDataFromSpecializedPoint(input, point, typeGroups, Point::Angle, "ANGLE");
    // POINT:SCALARS & POINT:SCALAR_UNITS
    this->UpdateMetaDataFromSpecializedPoint(input, point, typeGroups, Point::Scalar, "SCALAR");
    // POINT:POWERS & POINT:POWER_UNITS
    this->UpdateMetaDataFromSpecializedPoint(input, point, typeGroups, Point::Power, "POWER");
    // POINT:FORCES & POINT:FORCE_UNITS
    this->UpdateMetaDataFromSpecializedPoint(input, point, typeGroups, Point::Force, "FORCE");
    // POINT:MOMENT & POINT:MOMENT_UNITS
    this->UpdateMetaDataFromSpecializedPoint(input, point, typeGroups, Point::Moment, "MOMENT");
    // POINT:TYPE_GROUPS (final)
    if (!typeGroups.empty())
    {
      // POINT:TYPE_GROUPS is a 3D array
      // n x 2 x m 
      // n: number of characters
      // m: number of types of specialized points
      std::vector<uint8_t> dims = std::vector<uint8_t>(3,0);
      for (size_t i = 0 ; i < typeGroups.size() ; ++i)
      {
        if (typeGroups[i].size() > dims[0])
          dims[0] = static_cast<uint8_t>(typeGroups[i].size());
      }
      dims[2] = static_cast<uint8_t>(typeGroups.size() / 2);
      dims[1] = 2;
      MetaData::Iterator itGroups = point->FindChild("TYPE_GROUPS");
      if (itGroups != point->End())
        (*itGroups)->GetInfo()->SetValues(dims, typeGroups);
      else
        point->AppendChild(MetaData::New("TYPE_GROUPS", dims, typeGroups));
    }
    // ANALOG group
    // ------------
    MetaData::Pointer analog = MetaDataCreateChild(input->GetMetaData(), "ANALOG");
    int analogNumber = input->GetAnalogNumber();
    // ANALOG:USED
    MetaDataCreateChild(analog, "USED", static_cast<int16_t>(analogNumber));
    // ANALOG:LABELS & ANALOG:DESCRIPTIONS & ANALOG:UNITS & ANALOG:GAIN & ANALOG:SCALE & ANALOG:OFFSET (LABELS2, ...)
    labels.resize(analogNumber);
    descs.resize(analogNumber);
    std::vector<std::string> units = std::vector<std::string>(analogNumber);
    std::vector<int16_t> gain = std::vector<int16_t>(analogNumber, 0);
    std::vector<int16_t> analogZeroOffset = std::vector<int16_t>(analogNumber, 0);
    std::vector<float> analogChannelScale = std::vector<float>(analogNumber, 1.0);
    inc = 0;
    for (Acquisition::AnalogConstIterator itAnalog = input->BeginAnalog() ; itAnalog != input->EndAnalog() ; ++itAnalog)
    {
      labels[inc] = (*itAnalog)->GetLabel();
      descs[inc] = (*itAnalog)->GetDescription();
      units[inc] = (*itAnalog)->GetUnit();
      switch((*itAnalog)->GetGain())
      {
      case Analog::Unknown:
        gain[inc] = 0;
        break;
      case Analog::PlusMinus10:
        gain[inc] = 1;
        break;
      case Analog::PlusMinus5:
        gain[inc] = 2;
        break;
      case Analog::PlusMinus2Dot5:
        gain[inc] = 3;
        break;
      case Analog::PlusMinus1Dot25:
        gain[inc] = 4;
        break;
      case Analog::PlusMinus1:
        gain[inc] = 5;
        break;
      case Analog::PlusMinus0Dot5:
        gain[inc] = 6;
        break;
      case Analog::PlusMinus0Dot25:
        gain[inc] = 7;
        break;
      case Analog::PlusMinus0Dot1:
        gain[inc] = 8;
        break;
      case Analog::PlusMinus0Dot05:
        gain[inc] = 9;
        break;
      }
      analogChannelScale[inc] = static_cast<float>(this->m_AnalogChannelScale[inc]);
      analogZeroOffset[inc] = this->m_AnalogZeroOffset[inc];
      ++inc;
    }
    MetaDataCreateChild(analog, "LABELS", labels);
    MetaDataCreateChild(analog, "DESCRIPTIONS", descs);
    MetaDataCreateChild(analog, "UNITS", units);
    MetaDataCreateChild(analog, "GAIN", gain);
    MetaDataCreateChild(analog, "SCALE", analogChannelScale);
    MetaDataCreateChild(analog, "OFFSET", analogZeroOffset);
    // ANALOG:GEN_SCALE
    MetaDataCreateChild(analog, "GEN_SCALE", static_cast<float>(this->m_AnalogUniversalScale)); 
    // ANALOG:RATE
    MetaDataCreateChild(analog, "RATE", static_cast<float>(input->GetPointFrequency() * static_cast<double>(numberAnalogSamplePerFrame)));
    // ANALOG:BITS
    int16_t bits = static_cast<int16_t>(input->GetAnalogResolution());
    MetaDataCreateChild(analog, "BITS", bits);
    // ANALOG:FORMAT
    MetaDataCreateChild(analog, "FORMAT", (this->m_AnalogIntegerFormat == Unsigned ? "UNSIGNED" : "SIGNED"));
    // FORCE_PLATFORM group
    // --------------------
    MetaData::ConstIterator itFPGr = input->GetMetaData()->FindChild("FORCE_PLATFORM");
    if (itFPGr == input->GetMetaData()->End())
    {
      MetaData::Pointer fp = MetaDataCreateChild(input->GetMetaData(), "FORCE_PLATFORM");
      // FORCE_PLATFORM:USED
      MetaDataCreateChild(fp, "USED", (int16_t)0);
      // FORCE_PLATFORM:ZERO
      std::vector<int16_t> zeros = std::vector<int16_t>(2,0); zeros[0] = 1;
      MetaDataCreateChild(fp, "ZERO", zeros);
      // FORCE_PLATFORM:TYPE
      MetaDataCreateChild(fp, "TYPE")->SetInfo(MetaDataInfo::New(std::vector<uint8_t>(1,0), std::vector<int16_t>(0)));
      // FORCE_PLATFORM:CORNERS
      std::vector<uint8_t> cornersDim = std::vector<uint8_t>(3, 0); cornersDim[0] = 3; cornersDim[1] = 4;
      MetaDataCreateChild(fp, "CORNERS")->SetInfo(MetaDataInfo::New(cornersDim, std::vector<float>(0)));
      // FORCE_PLATFORM:ORIGIN
      std::vector<uint8_t> originDim = std::vector<uint8_t>(2, 0); originDim[0] = 3;
      MetaDataCreateChild(fp, "ORIGIN")->SetInfo(MetaDataInfo::New(originDim, std::vector<float>(0)));
      // FORCE_PLATFORM:CHANNEL
      std::vector<uint8_t> channelDim = std::vector<uint8_t>(2, 0); channelDim[0] = 6;
      MetaDataCreateChild(fp, "CHANNEL")->SetInfo(MetaDataInfo::New(channelDim, std::vector<int16_t>(0)));
    }
    // TRIAL group
    MetaData::Pointer trial = MetaDataCreateChild(input->GetMetaData(), "TRIAL");
    std::vector<int16_t> actualField = std::vector<int16_t>(2,0);
    // TRIAL:ACTUAL_START_FIELD
    actualField[1] = input->GetFirstFrame() >> 16; // HSB
    actualField[0] = input->GetFirstFrame() - (actualField[1] << 16); // LSB
    MetaDataCreateChild(trial, "ACTUAL_START_FIELD", actualField);
    // TRIAL:ACTUAL_END_FIELD
    int lastFrame = input->GetFirstFrame() + numberOfFrames - 1;
    actualField[1] = lastFrame >> 16; // HSB
    actualField[0] = lastFrame - (actualField[1] << 16); // LSB
    MetaDataCreateChild(trial, "ACTUAL_END_FIELD", actualField);
    // EVENT group
    // -----------
    if (input->GetEventNumber() != 0)
    {
      MetaData::Pointer event = MetaDataCreateChild(input->GetMetaData(), "EVENT");
      int16_t eventNumber = input->GetEventNumber();
      // EVENT:USED
      MetaDataCreateChild(event, "USED", eventNumber);
      // EVENT:CONTEXTS &
      // EVENT:LABELS &
      // EVENT:DESCRIPTIONS &
      // EVENT:TIMES &
      // EVENT:SUBJECTS &
      // EVENT:GENERIC_FLAGS &
      // EVENT:ICON_IDS
      std::vector<std::string> contexts =  std::vector<std::string>(eventNumber,"");
      labels.resize(eventNumber, "");
      descs.resize(eventNumber, "");
      std::vector<float> times =  std::vector<float>(eventNumber * 2, 0.0);
      std::vector<std::string> subjects =  std::vector<std::string>(eventNumber, "");
      std::vector<int8_t> genericFlags =  std::vector<int8_t>(eventNumber, 0);
      std::vector<int16_t> iconIds =  std::vector<int16_t>(eventNumber, 0);
      std::vector<std::string> uniqueEvents;
      inc = 0;
      for (Acquisition::EventConstIterator itEvent = input->BeginEvent() ; itEvent != input->EndEvent() ; ++itEvent)
      {
        contexts[inc] = (*itEvent)->GetContext();
        std::string s = contexts[inc];
        std::transform(s.begin(), s.end(), s.begin(), tolower);
        if (s.compare("general") == 0)
          genericFlags[inc] = 1;        
        if (!contexts[inc].empty())
        {
          bool newUniqueEventFound = true;
          for (size_t i = 0 ; i < uniqueEvents.size() ; ++i)
          {
            std::string s1 = uniqueEvents[i];
            std::transform(s1.begin(), s1.end(), s1.begin(), tolower);
            if (s1.compare(s) == 0)
            {
              newUniqueEventFound = false;
              break;
            }
          }
          if (newUniqueEventFound)
            uniqueEvents.push_back(contexts[inc]);
        }
        labels[inc] = (*itEvent)->GetLabel();
        descs[inc] = (*itEvent)->GetDescription();
        times[2 * inc] = static_cast<float>(static_cast<int>((*itEvent)->GetTime() / 60));
        times[2 * inc + 1] = static_cast<float>((*itEvent)->GetTime() - (times[2 * inc] * 60));
        subjects[inc] = (*itEvent)->GetSubject();
        iconIds[inc] = (*itEvent)->GetId();
        ++inc;
      }
      MetaDataCreateChild(event, "CONTEXTS", contexts);
      MetaDataCreateChild(event, "LABELS", labels);
      MetaDataCreateChild(event, "DESCRIPTIONS", descs);
      MetaDataCreateChild(event, "TIMES", times, 2);
      MetaDataCreateChild(event, "SUBJECTS", subjects);
      MetaDataCreateChild(event, "GENERIC_FLAGS", genericFlags);
      MetaDataCreateChild(event, "ICON_IDS", iconIds);
      // EVENT_CONTEXT group (special case)
      // -------------------
      MetaData::ConstIterator itEventGr = input->GetMetaData()->FindChild("EVENT_CONTEXT");
      bool needToUpdate = true;
      bool needToGenerate = true;
      size_t uniqueEventNumber = uniqueEvents.size();
      std::vector<int16_t> uniqueEventsIcon;
      std::vector<std::string> uniqueEventsLabel;
      std::vector<std::string> uniqueEventDesc;
      std::vector<int16_t> uniqueEventColours;
      if (itEventGr != input->GetMetaData()->End())
      {
        MetaData::ConstIterator itEventLabelsPr = (*itEventGr)->FindChild("LABELS");
        if (itEventLabelsPr != (*itEventGr)->End())
        {
          MetaData::ConstIterator itEventIconsPr = (*itEventGr)->FindChild("ICON_IDS");
          MetaData::ConstIterator itEventDescsPr = (*itEventGr)->FindChild("DESCRIPTIONS");
          MetaData::ConstIterator itEventColoursPr = (*itEventGr)->FindChild("COLOURS");
          int uniqueEventAppended = 0;
          (*itEventLabelsPr)->GetInfo()->ToString(uniqueEventsLabel);
          for (size_t i = 0 ; i < uniqueEventsLabel.size() ; ++i)
          {
            bool toCollapse = true;
            
            std::string s1 = uniqueEventsLabel[i];
            s1 = s1.erase(s1.find_last_not_of(' ') + 1);
            s1 = s1.erase(0, s1.find_first_not_of(' '));
            std::string s2 = "";
            std::vector<std::string>::iterator it2;
            for (it2 = uniqueEvents.begin() ; it2 != uniqueEvents.end() ; ++it2)
            {
              std::string s2l = s2 = *it2;
              std::transform(s1.begin(), s1.end(), s1.begin(), tolower);
              std::transform(s2.begin(), s2.end(), s2l.begin(), tolower);
              if (s1.compare(s2l) == 0)
              {
                toCollapse = false;
                break;
              }
            }
            if (toCollapse && !s2.empty() && (it2 != uniqueEvents.end()))
            {
              uniqueEventsLabel.push_back(s2);
              ++uniqueEventAppended;
            }
          }
          if (uniqueEventAppended != 0)
          {
            needToGenerate = false;
            size_t totalUniqueEvents = uniqueEventsLabel.size() + uniqueEventAppended;
            if (itEventIconsPr != (*itEventGr)->End())
              (*itEventIconsPr)->GetInfo()->ToInt16(uniqueEventsIcon);
            uniqueEventsIcon.resize(totalUniqueEvents, 0);
            if (itEventDescsPr != (*itEventGr)->End())
              (*itEventDescsPr)->GetInfo()->ToString(uniqueEventDesc);
            uniqueEventDesc.resize(totalUniqueEvents, "");
            if (itEventColoursPr != (*itEventGr)->End())
              (*itEventColoursPr)->GetInfo()->ToInt16(uniqueEventColours);
            uniqueEventColours.resize(totalUniqueEvents * 3, 0);
          }
          else
          {
            needToGenerate = false;
            needToUpdate = false;
          }
        }
      }
      if (needToGenerate && (uniqueEventNumber != 0)) 
      {
        uniqueEventsIcon.resize(uniqueEventNumber, 0);
        uniqueEventsLabel = uniqueEvents;
        uniqueEventDesc.resize(uniqueEventNumber,"");
        uniqueEventColours.resize(uniqueEventNumber * 3, 0);
      }
      if (needToUpdate && (uniqueEventNumber != 0))
      {
        MetaData::Pointer eventContext = MetaDataCreateChild(input->GetMetaData(), "EVENT_CONTEXT");
        MetaDataCreateChild(eventContext, "USED", static_cast<int16_t>(uniqueEventNumber));
        MetaDataCreateChild(eventContext, "ICON_IDS", uniqueEventsIcon);
        MetaDataCreateChild(eventContext, "LABELS", uniqueEventsLabel);
        MetaDataCreateChild(eventContext, "DESCRIPTIONS", uniqueEventDesc);
        MetaDataCreateChild(eventContext, "COLOURS", uniqueEventColours);
      }
    }
    else
    {
      input->GetMetaData()->RemoveChild("EVENT");
    }
  };

  void C3DFileIO::UpdateMetaDataFromSpecializedPoint(Acquisition::Pointer input, MetaData::Pointer point, std::vector<std::string>& typeGroups, Point::Type type, const std::string& label)
  {
    std::vector<std::string> buffer;
    for (Acquisition::PointConstIterator itPoint = input->BeginPoint() ; itPoint != input->EndPoint() ; ++itPoint)
    {
      if ((*itPoint)->GetType() == type)
        buffer.push_back((*itPoint)->GetLabel());
    }
    if (!buffer.empty())
    {
      std::string labels = label + "S";
      std::string labelUnits = label + "_UNITS";
      MetaDataCreateChild(point, labels, buffer);
      MetaDataCreateChild(point, labelUnits, input->GetPointUnit(type));
      typeGroups.push_back(labels); typeGroups.push_back(label);
    }
  };
};

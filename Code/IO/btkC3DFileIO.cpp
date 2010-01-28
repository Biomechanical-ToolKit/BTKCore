/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2010, Arnaud Barré
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

#include <fstream>
#include <algorithm>
#include <cctype>
#include <iostream>

namespace btk
{
  /**
   * @class C3DFileIOException
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
   * @class C3DFileIO
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
   * @var C3DFileIO::AnalogIntegerFormat C3DFileIO::Signed
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
   * Return the integer format (signed/unsigned) 
   */

  /**
   * @fn void C3DFileIO::SetAnalogIntegerFormat(AnalogIntegerFormat f)
   * Sets the integer format used to store analog channels data.
   */

  /**
   * @fn std::vector<double>& C3DFileIO::GetAnalogChannelScale()
   * Returns the vector of scales used for the analog channels' scaling.
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
   * Returns the vector of offsets used for the analog channels' scaling.
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
   * Returns the universal scale factor used to scale analog channels.
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
    std::ifstream ifs;
    ifs.open(filename.c_str(), std::ios_base::in | std::ios_base::binary);
    char parameterFirstBlock = 0, headerKey = 0;
    ifs.read(&parameterFirstBlock, 1);
    ifs.read(&headerKey, 1);
    ifs.close();
    if (parameterFirstBlock <= 0 || headerKey != 80) 
      return false;
    return true;
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
    std::fstream ifs;
    BinaryFileStream* ibfs = 0;
    Format* fdf = 0; // C3D file data format
    ifs.exceptions(std::ios_base::eofbit | std::ios_base::failbit | std::ios_base::badbit);
    try
    {
    // Binary stream selection
      ifs.open(filename.c_str(), std::ios_base::in | std::ios_base::binary);
      char buf;
      ifs.read(&buf, 1);
      ifs.seekg(512 * (static_cast<uint8_t>(buf) - 1) + 3, std::ios_base::beg);
      ifs.read(&buf, 1);
      switch (static_cast<int8_t>(buf) - 83)
      {
        case IEEE_LittleEndian : // IEEE LE (Intel)
          this->SetByteOrder(IEEE_LittleEndian);
          ibfs = new IEEELittleEndianBinaryFileStream(ifs);
          break;
        case VAX_LittleEndian : // VAX LE (DEC)
          this->SetByteOrder(VAX_LittleEndian);
          ibfs = new VAXLittleEndianBinaryFileStream(ifs);
          break;
        case IEEE_BigEndian : // IEEE BE (MIPS)
          this->SetByteOrder(IEEE_BigEndian);
          ibfs = new IEEEBigEndianBinaryFileStream(ifs);
          break;
        default :
          ifs.close();
          throw(C3DFileIOException("Invalid processor type"));
          break;
      };
      ibfs->SeekRead(0, std::ios_base::beg);
      int8_t parameterFirstBlock = ibfs->ReadI8();
      uint8_t firstBlock = 1;
      if (parameterFirstBlock <= 0)
        throw(C3DFileIOException("Bad parameter first block number"));
      if (ibfs->ReadI8() != 80)
        throw(C3DFileIOException("Bad header key"));
      uint16_t pointNumber = 0,
               totalAnalogSamplesPer3dFrame = 0,
               lastFrame = 0,
               pointMaximumFillGap = 0, 
               dataFirstBlock = 0, 
               numberSamplesPerAnalogChannel = 0;
      float pointScaleFactor = 0.0, pointFrameRate = 0.0;
      if (parameterFirstBlock != 1)
      {
    // Header
        pointNumber = ibfs->ReadU16(); // (word 02)
        totalAnalogSamplesPer3dFrame = ibfs->ReadU16(); // (word 03)
        output->SetFirstFrame(ibfs->ReadU16()); // (word 04)
        lastFrame = ibfs->ReadU16(); // (word 05)
        pointMaximumFillGap = ibfs->ReadU16(); // (word 06)
        pointScaleFactor = ibfs->ReadFloat(); // (word 07-08)
        if (pointScaleFactor == 0)
          throw(C3DFileIOException("Incorrect 3D scale factor"));
        dataFirstBlock = ibfs->ReadU16(); // (word 09)
        numberSamplesPerAnalogChannel = ibfs->ReadU16(); // (word 10)
        pointFrameRate = ibfs->ReadFloat(); // (word 11-12)
        ibfs->SeekRead(270, std::ios_base::cur); // word 13-147 => 135 word 
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
          ibfs->SeekRead(2, std::ios_base::cur); // (word 152)
          EventCollection::Iterator it = events->Begin();
          while(it != events->End())
          {
            (*it)->SetTime(ibfs->ReadFloat());
            (*it)->SetFrame(static_cast<int>((*it)->GetTime() * pointFrameRate) + 1);
            ++it;
          }
          ibfs->SeekRead((maxEvents * 4) - (eventNumber * 4), std::ios_base::cur);
          // No need to read event's activation status.
          ibfs->SeekRead(maxEvents, std::ios_base::cur);
          ibfs->SeekRead(2, std::ios_base::cur); // (word 198)
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
        ibfs->SeekRead((512 * (parameterFirstBlock - 1)), std::ios_base::beg);
        firstBlock = ibfs->ReadU8();
        // Some file doesn't respect the parameter key (equal to 80) 
        // if (ibfs->ReadI8() != 80) throw(C3DFileIOException("Bad parameter Key"));
        ibfs->ReadI8();
      }
      uint8_t blockNumber = ibfs->ReadU8();
      ibfs->ReadU8(); // Processor type
      unsigned int totalBytesRead = 4; // the four bytes read previously.
      if (firstBlock > 1)
      {
        ibfs->SeekRead((512 * (firstBlock - 1) - 4), std::ios_base::cur);
        totalBytesRead += (512 * (firstBlock - 1) - 4);
      }
      int8_t nbCharLabel = 0;
      int8_t id = 0;
      int offset = 0;
      int8_t type = 0;
      std::vector<uint8_t> dataDim;
      std::list<int8_t> groupIds;
      std::list<int8_t> parameterIds;
      std::list<MetaData::Pointer> parameters;
      MetaData::Pointer root = output->GetMetaData();
      while (1)
      {
        nbCharLabel = ibfs->ReadI8(); totalBytesRead += nbCharLabel + 1;
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
          size_t prod = 1;
          int8_t inc = 0 ; while (inc < nbDim) prod *= dataDim[inc++];
          if ((static_cast<int>(prod * abs(type)) >= offset) && (!lastEntry))
            throw(C3DFileIOException("Error in the number of elements in the parameter's data. The number is superior to the offset."));
          MetaData::Pointer entry;
          switch (type)
          {
            case -1:
              if (dataDim.size() >= 2)
              {
                int rows = 1; int8_t inc2 = 1 ; while (inc2 < nbDim) rows *= dataDim[inc2++];
                entry = MetaData::New(label, dataDim, ibfs->ReadString(rows, dataDim[0]), "", (nbCharLabel > 0 ? true : false));
              }
              else
                entry = MetaData::New(label, dataDim, ibfs->ReadString(1,prod), "", (nbCharLabel > 0 ? true : false));
              break;
            case 1:
              entry = MetaData::New(label, dataDim, ibfs->ReadI8(prod), "", (nbCharLabel > 0 ? true : false));
              break;
            case 2:
              entry = MetaData::New(label, dataDim, ibfs->ReadI16(prod), "", (nbCharLabel > 0 ? true : false));
              break;
            case 4:
              entry = MetaData::New(label, dataDim, ibfs->ReadFloat(prod), "", (nbCharLabel > 0 ? true : false));
              break;
            default :
              throw(C3DFileIOException("Data parameter type unknown for the entry: '" + label + "'"));
              break;
          }
          offset -= (prod * abs(type));
          uint8_t nbCharDesc = ibfs->ReadU8(); offset -= 1;
          entry->SetDescription(ibfs->ReadString(nbCharDesc)); 
          offset -= nbCharDesc;
          parameters.push_back(entry);
        }
        if (lastEntry)
          offset = 0;
        if (offset < 0)
          throw(C3DFileIOException("Error during the pointing of another parameter|group"));
        // Checks if the next parameter is not pointing in the Data section.
        if ((totalBytesRead + offset) > static_cast<unsigned int>((blockNumber * 512)))
        {
          btkIOErrorMacro(filename, "The next parameter is pointing in the Data section. Parameters' extraction is stopped.");
          totalBytesRead -= offset;
          break;
        }
        if (lastEntry)
          break; // Parameter section end
        ibfs->SeekRead(offset, std::ios_base::cur);
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
      int totalBlocksRead = static_cast<int>(ceil((double)totalBytesRead / 512));
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
            btkIOErrorMacro(filename, "The EVENT:TIME doesn't contains the appropriate number of values. The extracted times could be corrupted.")
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
            evt->SetFrame(static_cast<int>(evt->GetTime() * pointFrameRate) + 1);
            events->InsertItem(evt);
          }
        }
      }
    // Data
      if (dataFirstBlock != 0)
      {
        if (dataFirstBlock < (parameterFirstBlock + (firstBlock != 0 ? (firstBlock - 1) : 0) + blockNumber))
          throw(C3DFileIOException("Bad data first block"));
        ibfs->SeekRead((512 * (dataFirstBlock - 1)), std::ios_base::beg);
        if (numberSamplesPerAnalogChannel == 0)
          numberSamplesPerAnalogChannel = 1;
        uint16_t analogNumber = totalAnalogSamplesPer3dFrame / numberSamplesPerAnalogChannel;
        // ANALOG
        MetaData::ConstIterator itAnalog = root->FindChild("ANALOG");
        if (itAnalog != root->End())
        {
          MetaData::ConstIterator itAnalogUsed = (*itAnalog)->FindChild("USED");
          if (itAnalogUsed != (*itAnalog)->End())
          {
            int16_t analogUsed;
            analogUsed = (*itAnalogUsed)->GetInfo()->ToInt(0);
            if (analogNumber != static_cast<uint16_t>(analogUsed))
            {
              btkIOErrorMacro(filename, "The number of analog channels wrote in the header section and in the parameter section are not the same. The value kept is from the parameter section.");
              analogNumber = static_cast<uint16_t>(analogUsed);
            }
          }
        }
        this->m_AnalogChannelScale = std::vector<double>(analogNumber, 1.0);
        this->m_AnalogZeroOffset = std::vector<int>(analogNumber, 0);
        this->m_AnalogUniversalScale = 1.0;

        if ((analogNumber != 0) && (itAnalog != root->End()))
        {
          // - ANALOG:FORMAT
          MetaData::ConstIterator itAnalogFormat = (*itAnalog)->FindChild("FORMAT");
          if (itAnalogFormat != (*itAnalog)->End())
          {
            if ((*itAnalogFormat)->GetInfo()->ToString(0).compare("UNSIGNED") == 0)
              this->m_AnalogIntegerFormat = Unsigned;
            else
              this->m_AnalogIntegerFormat = Signed;
          }
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
          // Check if values in ANALOG:OFFSET correspond to the informations in ANALOG:FORMAT and ANALOG:BITS
          std::vector<int16_t> analogZeroOffset_t;
          MetaDataCollapseChildrenValues<int16_t>(analogZeroOffset_t, *itAnalog, "OFFSET", analogNumber, 0);
          bits = output->GetAnalogResolution();
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
            btkIOErrorMacro(filename, "Analog format and/or their resolution are inconsistents with Analog offsets. They were update.");
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
        MetaData::ConstIterator itPoint = root->FindChild("POINT");
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
              btkIOErrorMacro(filename, "The number of points wrote in the header section and in the parameter section are not the same. The value kept is from the parameter section.");
              pointNumber = static_cast<uint16_t>(pointUsed);
            }
          }
          // POINT:SCALE
          MetaData::ConstIterator itPointScale = (*itPoint)->FindChild("SCALE");
          if (itPointScale != (*itPoint)->End())
          {
            float pointScale = (*itPointScale)->GetInfo()->ToFloat(0);
            if (fabs(pointScale - pointScaleFactor) > std::numeric_limits<float>::epsilon())
            {
              btkIOErrorMacro(filename, "The point scaling factor writen in the header and in the parameter POINT:SCALE are not the same. The second value is kept.");
              pointScaleFactor = pointScale;
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
                           &(point->GetMasks().data()[frame]),
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
    // Label, description, unit and type
        int inc = 0; 
        std::vector<std::string> collapsed;
        bool c3dFromMotion = false;
        // POINT Label, description, unit
        if (itPoint != root->End())
        {
          // NOTE: C3D files exported from "Motion Analysis Corp." softwares (EvaRT, Cortex) seem to use POINT:LABELS and POINTS:DESCRIPTIONS as a short and long version of the points' label respecively. Point's Label used in EvaRT and Cortex correspond to values stored in POINTS:DESCRIPTIONS. To distinguish C3D files exported from "Motion Analysis Corp." softwares, it is possible to check the value in the parameter MANUFACTURER:Company.     
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
              if (inc >= static_cast<int>(collapsed.size()))
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
          }
          // POINT:UNITS
          MetaData::ConstIterator itPointUnits = (*itPoint)->FindChild("UNITS");
          if (itPointUnits != (*itPoint)->End())
            output->SetPointUnit((*itPointUnits)->GetInfo()->ToString(0));
          // POINT type and special units
          const char* names[] = {"ANGLE", "FORCE", "MOMENT", "POWER", "SCALAR", "REACTION"}; 
          int numberOfNames =  sizeof(names) / sizeof(char*);
          for(int i = 0 ; i < numberOfNames ; ++i)
          {
            // unit
            MetaData::ConstIterator itU = (*itPoint)->FindChild(std::string(names[i]) + "_UNITS");
            if (itU != (*itPoint)->End())
              output->SetPointUnit(static_cast<Point::Type>(i + 1), (*itU)->GetInfo()->ToString(0));
            // type
            MetaDataCollapseChildrenValues(collapsed, *itPoint, std::string(names[i]) + "S");
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
              if (inc >= static_cast<int>(collapsed.size()))
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
            if (inc >= static_cast<int>(collapsed.size()))
              break;
            (*it)->SetUnit(collapsed[inc++]);
          }
          // - ANALOG:GAIN
          std::vector<int16_t> gains;
          MetaDataCollapseChildrenValues(gains, *itAnalog, "GAIN");
          inc = 0; for (Acquisition::AnalogIterator it = output->BeginAnalog() ; it != output->EndAnalog() ; ++it)
          {
            if (inc >= static_cast<int>(gains.size()))
              break;
            if (gains[inc] < 0 || gains[inc] > 5)
            {
              btkIOErrorMacro(filename, "Unknown gain. If the value corresponding to this unknown gain is a real value, please contact a developer to add it in the list.");
              (*it)->SetGain(Analog::Unknown);
            }
            else
              (*it)->SetGain(static_cast<Analog::Gain>(gains[inc]));
            ++inc;
          }
        }
      }
    }
    catch (std::fstream::failure& )
    {
      std::string excmsg; 
      if (!ifs.is_open())
        excmsg = "Invalid file path";
      else if (ifs.eof())
        excmsg = "Unexpected end of file";
      else if(ifs.bad())
        excmsg = "Loss of integrity of the filestream";
      else if(ifs.fail())
        excmsg = "Internal logic operation error on the stream associated with the file";
      else
        excmsg = "Unknown error associated with the filestream";
      
      if (ifs.is_open()) ifs.close();     
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
    if (input.get() == 0)
    {
      btkErrorMacro("Null acquisition.");
      return;
    }
    if (this->HasWritingFlag(CompatibleVicon))
      this->KeepAcquisitionCompatibleVicon(input);
    if (this->HasWritingFlag(ScalesFromDataUpdate))
      this->UpdateScalingFactorsFromData(input);
    if (this->HasWritingFlag(ScalesFromMetaDataUpdate))
      this->UpdateScalingFactorsFromMetaData(input);
    if (this->HasWritingFlag(MetaDataFromDataUpdate))
      this->UpdateMetaDataFromData(input);

    std::fstream ofs;
    BinaryFileStream* obfs = 0;
    Format* fdf = 0; // C3D file data format
    try
    {
      // Binary stream selection
      switch(this->GetByteOrder())
      {
        case IEEE_LittleEndian : // IEEE LE (Intel)
          obfs = new IEEELittleEndianBinaryFileStream(ofs);
          break;
        case VAX_LittleEndian : // VAX LE (DEC)
          obfs = new VAXLittleEndianBinaryFileStream(ofs);
          break;
        case IEEE_BigEndian : // IEEE BE (MIPS)
          obfs = new IEEEBigEndianBinaryFileStream(ofs);
          break;
        default :
          ofs.close();
          throw(C3DFileIOException("Invalid processor type"));
          break;
      }
      // File access
      ofs.open(filename.c_str(), std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
      if (!ofs.is_open())
        throw(C3DFileIOException("No File access"));
      // Acquisition
      bool templateFile = true;
      size_t writtenBytes = 0;
      uint16_t numberSamplesPerAnalogChannel = 1;
      if (!input->IsEmptyPoint() || !input->IsEmptyAnalog())
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
        writtenBytes += obfs->Write(static_cast<uint16_t>(input->GetAnalogNumber() * input->GetAnalogFrameNumber() / input->GetPointFrameNumber()));
        // First frame
        writtenBytes += obfs->Write(static_cast<uint16_t>(input->GetFirstFrame()));
        // Last frame
        writtenBytes += obfs->Write(static_cast<uint16_t>(input->GetLastFrame()));
        // Maximum interpolation gap in 3D frames
        writtenBytes += obfs->Write(static_cast<uint16_t>(10));
        // The 3D scale factor
        writtenBytes += obfs->Write(static_cast<float>(this->m_PointScale * static_cast<double>(this->m_StorageFormat)));
        // The (false) number of the first block of the Data section
        writtenBytes += obfs->Write(static_cast<uint16_t>(0));
        // The number of analog samples per analog channel
        numberSamplesPerAnalogChannel = static_cast<uint16_t>(input->GetNumberAnalogSamplePerFrame());
        writtenBytes += obfs->Write(numberSamplesPerAnalogChannel);
        // The 3D frame rate
        writtenBytes += obfs->Write(static_cast<float>(input->GetPointFrequency()));
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
        MetaData::Iterator itPoint = input->GetMetaData()->FindChild("POINT");
        pointID = std::distance(input->GetMetaData()->Begin(), itPoint) + 1;
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
      for (MetaData::ConstIterator it = input->GetMetaData()->Begin() ; it != input->GetMetaData()->End() ; ++it)
      {
        writtenBytes += this->WriteMetaData(obfs, *it, id);
        ++id;
      }
      // POINT:DATA_START final
      if (!templateFile)
      {
        int totalWrittenBytes = writtenBytes + (1 + 1 + dataStart->GetLabel().length() + 2 + 1 + 1 + dataStart->GetInfo()->GetDimensions().size() + (dataStart->GetInfo()->GetValues().size() * abs(dataStart->GetInfo()->GetFormat())) + 1 + dataStart->GetDescription().length());
        totalWrittenBytes += (512 - (totalWrittenBytes % 512));
        uint8_t pNB = static_cast<uint8_t>(totalWrittenBytes / 512);
        dS = 2 + pNB;
        dataStart->GetInfo()->SetValues(static_cast<int16_t>(dS));
        writtenBytes += this->WriteMetaData(obfs, dataStart, pointID);
        writtenBytes += obfs->Fill(512 - (writtenBytes % 512));
        // DATA_START is reinserted.
        input->GetMetaData()->GetChild(pointID - 1)->AppendChild(dataStart);
        // Back to the parameter: number of blocks
        obfs->SeekWrite(512 * (2 - 1) + 2, std::ios_base::beg);
        obfs->Write(pNB);
        // Back to the header: data first block
        obfs->SeekWrite(16, std::ios_base::beg);
        obfs->Write(dS);
      }
      else
      {
        writtenBytes += obfs->Fill(512 - (writtenBytes % 512));
        uint8_t pNB = static_cast<uint8_t>(writtenBytes / 512);
        // Back to the parameter: number of blocks
        obfs->SeekWrite(2, std::ios_base::beg);
        obfs->Write(pNB);
      }
      if (writtenBytes > (255 * 512)) // 255 * 512 = max size
        throw(C3DFileIOException("Total size reserved for the parameters was exceeded. Impossible to write the acquisition in a C3D file."));
      // -= DATA =-
      if (!templateFile)
      {
        obfs->SeekWrite(512 * (dS - 1), std::ios_base::beg);
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
        int frameNumber = input->GetPointFrameNumber();
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
                            point->GetMasks().data()[frame],
                            this->m_PointScale);
            ++itM;
          }
          
          unsigned inc = 0, incChannel = 0, analogFrame = numberSamplesPerAnalogChannel * frame;
          Acquisition::AnalogConstIterator itA = input->BeginAnalog();
          while (itA != input->EndAnalog())
          {
            fdf->WriteAnalog(
                (*itA)->GetValues().data()[analogFrame]
                / this->m_AnalogChannelScale[incChannel]
                / this->m_AnalogUniversalScale
                + this->m_AnalogZeroOffset[incChannel]);
            ++itA; ++incChannel;
            if ((itA == input->EndAnalog()) && (inc < static_cast<unsigned>(numberSamplesPerAnalogChannel - 1)))
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
  : AcquisitionFileIO(), 
    m_AnalogChannelScale(),
    m_AnalogZeroOffset()
  {
    this->m_PointScale = 0.1;
    this->m_AnalogUniversalScale = 1.0;
    this->m_StorageFormat = Float;
    this->m_AnalogIntegerFormat = Signed;
    this->SetFileType(AcquisitionFileIO::Binary);
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    this->SetByteOrder(btk::C3DFileIO::IEEE_BigEndian);
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */
    this->SetByteOrder(btk::C3DFileIO::VAX_LittleEndian);
#else
    this->SetByteOrder(btk::C3DFileIO::IEEE_LittleEndian);
#endif
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
    std::string blank = std::string(4, ' ');
    // Frequency
    if (input->GetPointFrequency() == 0.0)
    {
      btkErrorMacro("Acquisition frequency can't be null and is set to 50 Hz.");
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
    {
      double minVal = (*itPoint)->GetValues().minCoeff();
      double maxVal = (*itPoint)->GetValues().maxCoeff();
      double val = std::max(fabs(minVal), maxVal);
      if (val > max)
        max = val;
    }
    const int currentMax = static_cast<int>(this->m_PointScale * 32000);
    // Guess to compute a new point scaling factor.
    if (((max > currentMax) || (max <= (currentMax / 2))) && (max > std::numeric_limits<double>::epsilon()))
      this->m_PointScale = max / 32000.0;
    // ANALOG:SCALE & ANALOG:OFFSET
    int analogNumber = input->GetAnalogNumber();
    int inc = 0;
    this->m_AnalogChannelScale.resize(analogNumber, 1.0);
    this->m_AnalogZeroOffset.resize(analogNumber, 0);
    for (Acquisition::AnalogConstIterator itAnalog = input->BeginAnalog() ; itAnalog != input->EndAnalog() ; ++itAnalog)
    {
      this->m_AnalogChannelScale[inc] = (*itAnalog)->GetScale();
      this->m_AnalogZeroOffset[inc] = (*itAnalog)->GetOffset();
      ++inc;
    }
    // ANALOG:GEN_SCALE
    this->m_AnalogUniversalScale = 1.0;
  };
  
  /**
   * Update scaling factors from acquisition's metada.
   */
  void C3DFileIO::UpdateScalingFactorsFromMetaData(Acquisition::Pointer input)
  {
    int analogNumber = input->GetAnalogNumber();
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
          if ((bits != 8) && (bits != 8) && (bits != 12) && (bits != 14) && (bits != 16))
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
          if (static_cast<int>((*itAnalogOffset)->GetInfo()->GetValues().size()) < analogNumber)
          {
            btkErrorMacro("No enough analog offsets. Impossible to update analog offsets.");
          }
          else
          {
            if (this->m_AnalogIntegerFormat == Unsigned) // unsigned
            {
              std::vector<int16_t> analogZeroOffset_t;
              (*itAnalogOffset)->GetInfo()->ToInt16(analogZeroOffset_t);
              unsigned mini = ((analogZeroOffset_t.size() > this->m_AnalogZeroOffset.size()) ? this->m_AnalogZeroOffset.size() : analogZeroOffset_t.size());
              for (unsigned inc = 0 ; inc < mini ; ++inc)
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
          if (static_cast<int>((*itAnalogScale)->GetInfo()->GetValues().size()) < analogNumber)
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
  void C3DFileIO::UpdateMetaDataFromData(Acquisition::Pointer input)
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
    MetaDataCreateChild(point, "FRAMES", static_cast<int16_t>(input->GetPointFrameNumber()));
    // POINT:LABELS & POINT:DESCRIPTIONS (LABELS2, DESCRIPTIONS2, ...)
    std::vector<std::string> labels = std::vector<std::string>(pointNumber);
    std::vector<std::string> descs = std::vector<std::string>(pointNumber);
    int inc = 0;
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
      int num = -1;
      for (int i = 0 ; i < static_cast<int>(typeGroups.size()) ; ++i)
      {
        if (typeGroups[i].size() > dims[0])
          dims[0] = typeGroups[i].size();
      }
      dims[2] = typeGroups.size() / 2;
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
      gain[inc] = (*itAnalog)->GetGain();
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
    MetaDataCreateChild(analog, "RATE", static_cast<float>(input->GetAnalogFrequency()));
    // ANALOG:BITS
    int16_t bits = static_cast<int16_t>(input->GetAnalogResolution());
    MetaDataCreateChild(analog, "BITS", bits);
    // ANALOG:FORMAT
    if (bits == 16)
      MetaDataCreateChild(analog, "FORMAT", "UNSIGNED");
    else
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
          for (int i = 0 ; i < static_cast<int>(uniqueEvents.size()) ; ++i)
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
      MetaDataCreateChild(event, "TIMES", times);
      MetaDataCreateChild(event, "SUBJECTS", subjects);
      MetaDataCreateChild(event, "GENERIC_FLAGS", genericFlags);
      MetaDataCreateChild(event, "ICON_IDS", iconIds);
      // EVENT_CONTEXT group (special case)
      // -------------------
      MetaData::ConstIterator itEventGr = input->GetMetaData()->FindChild("EVENT_CONTEXT");
      bool needToUpdate = true;
      bool needToGenerate = true;
      int16_t uniqueEventNumber = uniqueEvents.size();
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
          for (int i = 0 ; i < static_cast<int>(uniqueEventsLabel.size()) ; ++i)
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
            int totalUniqueEvents = uniqueEventsLabel.size() + uniqueEventAppended;
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
        MetaDataCreateChild(eventContext, "USED", uniqueEventNumber);
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

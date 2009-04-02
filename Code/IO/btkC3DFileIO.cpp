/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009, Arnaud Barré
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
   */
  
  /**
   * @typedef C3DFileIO::Pointer
   * Smart pointer associated with an C3DFileIO object.
   */
  
  /**
   * @typedef C3DFileIO::ConstPointer
   * Smart const pointer associated with an C3DFileIO object.
   */
  
  /**
   * @fn static C3DFileIO::Pointer C3DFileIO::New()
   * Create a C3DFileIO object an return it as a smart pointer.
   */
  
  /**
   * @fn C3DFileIO::~C3DFileIO()
   * Empty destructor.
   */
  
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
    BinaryFileStream* pFstream = 0;
    Format* pFormat = 0;
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
          pFstream = new IEEELittleEndianBinaryFileStream(ifs);
          break;
        case VAX_LittleEndian : // VAX LE (DEC)
          this->SetByteOrder(VAX_LittleEndian);
          pFstream = new VAXLittleEndianBinaryFileStream(ifs);
          break;
        case IEEE_BigEndian : // IEEE BE (MIPS)
          this->SetByteOrder(IEEE_BigEndian);
          pFstream = new IEEEBigEndianBinaryFileStream(ifs);
          break;
        default :
          ifs.close();
          throw(C3DFileIOException("Invalid processor type"));
          break;
      };
      pFstream->SeekRead(0, std::ios_base::beg);
      int8_t parameterFirstBlock = pFstream->ReadI8();
      if (parameterFirstBlock <= 0)
        throw(C3DFileIOException("Bad parameter first block number"));
      if (pFstream->ReadI8() != 80)
        throw(C3DFileIOException("Bad header key"));
      uint16_t markerNumber = 0,
               lastFrame = 0,
               markerMaximumFillGap = 0, 
               dataFirstBlock = 0, 
               numberSamplesPerAnalogChannel = 0;
      float markerScaleFactor = 0.0, markerFrameRate = 0.0;
      if (parameterFirstBlock != 1)
      {
    // Header
        markerNumber = pFstream->ReadU16(); // (word 02)
        pFstream->ReadU16(); // uint16_t totalAnalogSamplesPer3dFrame = pFstream->ReadU16(); // (word 03)
        output->SetFirstFrame(pFstream->ReadU16()); // (word 04)
        lastFrame = pFstream->ReadU16(); // (word 05)
        markerMaximumFillGap = pFstream->ReadU16(); // (word 06)
        markerScaleFactor = pFstream->ReadFloat(); // (word 07-08)
        if (markerScaleFactor == 0)
          throw(C3DFileIOException("Incorrect 3D scale factor"));
        dataFirstBlock = pFstream->ReadU16(); // (word 09)
        numberSamplesPerAnalogChannel = pFstream->ReadU16(); // (word 10)
        markerFrameRate = pFstream->ReadFloat(); // (word 11-12)
        pFstream->SeekRead(270, std::ios_base::cur); // word 13-147 => 135 word
    // Event
    // Parameter
        pFstream->SeekRead((512 * (parameterFirstBlock - 1)), std::ios_base::beg);
      }
      uint8_t firstBlock = pFstream->ReadU8();
      // Some file doesn't respect the parameter key (equal to 80) 
      // if (pFstream->ReadI8() != 80) throw(C3DFileIOException("Bad parameter Key"));
      pFstream->ReadI8();
      uint8_t blockNumber = pFstream->ReadU8();
      pFstream->ReadU8(); // Processor type
      if (firstBlock > 1)
        pFstream->SeekRead((512 * (firstBlock - 1) - 4), std::ios_base::cur);

      int8_t nbCharName = 0;
      int8_t id = 0;
      int offset = 0;
      int8_t type = 0;
      std::vector<uint8_t> dataDim;
      std::list<int8_t> groupIds;
      std::list<int8_t> parameterIds;
      std::list<MetaDataEntry::Pointer> parameters;
      MetaDataEntry::Pointer root = output->GetMetaData();
      while (1)
      {
        nbCharName = pFstream->ReadI8();
        if (nbCharName == 0) break; // Parameter section end
        id = pFstream->ReadI8();
        if (id == 0)
          throw(C3DFileIOException("Error during the ID extraction in the parameter section - ID equal to 0"));
        if (id < 0)
        {
          groupIds.push_back(id);
          MetaDataEntry::Pointer entry = MetaDataEntry::New(pFstream->ReadString(abs(nbCharName)));
          entry->SetUnlockState((nbCharName > 0 ? true : false));
          offset = pFstream->ReadU16(); offset -= 2;
          uint8_t nbCharDesc = pFstream->ReadU8(); offset -= 1;
          entry->SetDescription(pFstream->ReadString(nbCharDesc)); offset -= nbCharDesc;
          root->AppendChild(entry);
        }
        else
        {
          parameterIds.push_back(id);
          std::string name = pFstream->ReadString(abs(nbCharName));
          offset = pFstream->ReadU16(); offset -= 2;
          type = pFstream->ReadI8();  offset -= 1;
          int8_t nbDim = pFstream->ReadI8(); offset -= 1;
          dataDim = pFstream->ReadU8(nbDim); offset -= nbDim;
          size_t prod = 1;
          int8_t inc = 0 ; while (inc < nbDim) prod *= dataDim[inc++];
          if (static_cast<int>(prod * abs(type)) >= offset) 
            throw(C3DFileIOException("Error in the number of elements in the parameter's data. The number is superior to the offset"));
          MetaDataEntry::Pointer entry;
          switch (type)
          {
            case -1:
              if (dataDim.size() == 2)
                entry = MetaDataEntry::New(name, dataDim, pFstream->ReadString(dataDim[1],dataDim[0]), "", (nbCharName > 0 ? true : false));
              else
                entry = MetaDataEntry::New(name, dataDim, pFstream->ReadString(1,prod), "", (nbCharName > 0 ? true : false));
              break;
            case 1:
              entry = MetaDataEntry::New(name, dataDim, pFstream->ReadI8(prod), "", (nbCharName > 0 ? true : false));
              break;
            case 2:
              entry = MetaDataEntry::New(name, dataDim, pFstream->ReadI16(prod), "", (nbCharName > 0 ? true : false));
              break;
            case 4:
              entry = MetaDataEntry::New(name, dataDim, pFstream->ReadFloat(prod), "", (nbCharName > 0 ? true : false));
              break;
            default :
              throw(C3DFileIOException("Data parameter type unknown"));
              break;
          }
          offset -= (prod * abs(type));
          uint8_t nbCharDesc = pFstream->ReadU8(); offset -= 1;
          entry->SetDescription(pFstream->ReadString(nbCharDesc)); 
          offset -= nbCharDesc;
          parameters.push_back(entry);
        }
        if (offset < 0)
          throw(C3DFileIOException(" Error during the pointing of another parameter|group"));
        pFstream->SeekRead(offset, std::ios_base::cur);
      }
      std::list<MetaDataEntry::Pointer>::iterator itParameter = parameters.begin();
      MetaDataEntry::Iterator itGroup = root->Begin();
      std::list<int8_t>::iterator itG = groupIds.begin();
      std::list<int8_t>::iterator itP = parameterIds.begin();
      while(1)
      {
        if (itP == parameterIds.end())
        {
          ++itG; ++itGroup;
          if (itG == groupIds.end())
            break;
          itP = parameterIds.begin(); itParameter = parameters.begin();
          
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
      if (parameters.size() != 0)
        throw(C3DFileIOException("Some parameters are orphans - No group has the same id"));
    // Data
      if (dataFirstBlock != 0)
      {
        if (dataFirstBlock < (parameterFirstBlock + (firstBlock != 0 ? (firstBlock - 1) : 0) + blockNumber))
          throw(C3DFileIOException("Bad data first block"));
        pFstream->SeekRead((512 * (dataFirstBlock - 1)), std::ios_base::beg);
        std::vector<float> analogChannelScale = std::vector<float>(0);
        std::vector<int> analogOffset = std::vector<int>(0);
        float analogGenScale = 0;
        MetaDataEntry::ConstPointer analogGr =  root->GetChild("ANALOG");
        uint16_t analogNumber; int16_t analogUsed;
        FromString(analogGr->GetChild("USED")->GetMetaDataEntryValue()->GetValues().front(), analogUsed); analogNumber = analogUsed;
        if (analogNumber != 0)
        {
          //  - ANALOG:OFFSET
          MetaDataEntry::ConstPointer analogGrOffsetPr =  analogGr->GetChild("OFFSET");
          if (analogGrOffsetPr->GetMetaDataEntryValue()-> GetDimensions().front() < analogNumber)
            throw(C3DFileIOException("Insuffisant number of analog offsets"));
          //  - ANALOG:SCALE
          MetaDataEntry::ConstPointer analogGrScalePr =  analogGr->GetChild("SCALE");
          if (analogGrScalePr->GetMetaDataEntryValue()-> GetDimensions().front() < analogNumber)
            throw(C3DFileIOException("Insuffisant number of analog scales"));
          // - ANALOG:GEN_SCALE
          MetaDataEntry::ConstPointer analogGrGenScalePr =  analogGr->GetChild("GEN_SCALE");
          // - ANALOG:FORMAT
          bool unsignedAnalogFormat = false;
          if (analogGr->Find("FORMAT") != analogGr->End())
          {
            const std::string analogFormat = "UNSIGNED";
            if (analogGr->GetChild("FORMAT")->GetMetaDataEntryValue()->GetValues().front().compare(analogFormat) == 0)
              unsignedAnalogFormat = true;
          }
          const std::vector<std::string> dataOffset = analogGrOffsetPr->GetMetaDataEntryValue()->GetValues();
          FromString(analogGrScalePr->GetMetaDataEntryValue()->GetValues(), analogChannelScale);
          FromString(analogGrGenScalePr->GetMetaDataEntryValue()->GetValues().front(), analogGenScale);
          analogOffset.resize(dataOffset.size());
          std::vector<int16_t> analogOffset_t;
          FromString(dataOffset, analogOffset_t);
          if (unsignedAnalogFormat) // unsigned
          { 
            for (unsigned inc = 0 ; inc < analogOffset_t.size() ; ++inc)
              analogOffset[inc] = static_cast<uint16_t>(analogOffset_t[inc]);
          }
          else // signed
          {
            for (unsigned inc = 0 ; inc < analogOffset_t.size() ; ++inc)
              analogOffset[inc] = analogOffset_t[inc];
          }
        }
        if (markerScaleFactor > 0) // integer
          pFormat = new IntegerFormat(pFstream);
        else // float;
          pFormat = new FloatFormat(pFstream);
        int frameNumber = lastFrame - output->GetFirstFrame() + 1;
        output->Init(markerNumber, frameNumber, analogNumber, numberSamplesPerAnalogChannel);
        output->SetMarkerFrequency(markerFrameRate);
        for (int frame = 0 ; frame < frameNumber ; ++frame)
        {
          Acquisition::MarkerIterator itM = output->BeginMarker(); 
          while (itM != output->EndMarker())
          {
            Marker* marker = itM->get();
            pFormat->ReadPoint(&(marker->GetValues().data()[frame]),
                               &(marker->GetValues().data()[frame + frameNumber]),
                               &(marker->GetValues().data()[frame + 2*frameNumber]),
                               &(marker->GetResidual().data()[frame]), 
                               &(marker->GetMask().data()[frame]), 
                               markerScaleFactor);
            ++itM;
          }
          
          unsigned inc = 0, incChannel = 0, analogFrame = numberSamplesPerAnalogChannel * frame;
          Acquisition::AnalogIterator itA = output->BeginAnalog();
          while (itA != output->EndAnalog())
          {
            (*itA)->GetValues().data()[analogFrame] = (pFormat->ReadAnalog() - analogOffset[incChannel]) * analogChannelScale[incChannel] * analogGenScale;
            ++itA; ++incChannel;
            if ((itA == output->EndAnalog()) && (inc < static_cast<unsigned>(numberSamplesPerAnalogChannel - 1)))
            {
              itA = output->BeginAnalog();
              incChannel = 0;
              ++inc; ++analogFrame;
            }
          }
        }
    // Label & description
        int inc = 0; 
        std::vector<std::string> collapsed;
        MetaDataEntry::ConstPointer pointGr =  root->GetChild("POINT");
        this->collapse(pointGr, markerNumber, "LABELS", collapsed, true);
        inc = 0; for (Acquisition::MarkerIterator it = output->BeginMarker() ; it != output->EndMarker() ; ++it)
          (*it)->SetLabel(collapsed[inc++]);
        collapsed.clear(); this->collapse(pointGr, markerNumber, "DESCRIPTIONS", collapsed, false);
        inc = 0; for (Acquisition::MarkerIterator it = output->BeginMarker() ; it != output->EndMarker() ; ++it)
        {
          if (inc >= static_cast<int>(collapsed.size()))
            break;
          (*it)->SetDescription(collapsed[inc++]);
        }
        collapsed.clear(); this->collapse(analogGr, analogNumber, "LABELS", collapsed, true);
        inc = 0; for (Acquisition::AnalogIterator it = output->BeginAnalog() ; it != output->EndAnalog() ; ++it)
          (*it)->SetLabel(collapsed[inc++]);
        collapsed.clear(); this->collapse(analogGr, analogNumber, "DESCRIPTIONS", collapsed, false);
        inc = 0; for (Acquisition::AnalogIterator it = output->BeginAnalog() ; it != output->EndAnalog() ; ++it)
        {
          if (inc >= static_cast<int>(collapsed.size()))
            break;
          (*it)->SetDescription(collapsed[inc++]);
        }
      }
    }
    catch (std::fstream::failure& )
    {
      std::string excmsg; 
      if (!ifs.is_open())
        excmsg = "Invalid file path";
      else if (ifs.eof())
        excmsg = "Unexcepted end of file";
      else if(ifs.bad())
        excmsg = "Loss of integrity of the filestream";
      else if(ifs.fail())
        excmsg = "Internal logic operation error on the stream associated with the file";
      else
        excmsg = "Unknown error associated with the filestream";
      
      if (ifs.is_open()) ifs.close();     
      if (pFstream) delete pFstream;
      if (pFormat) delete pFormat;
      throw(C3DFileIOException(excmsg));
    }
    catch (C3DFileIOException& )
    {
      if (pFstream) delete pFstream;
      if (pFormat) delete pFormat;
      throw;
    }
    catch (std::exception& e)
    {
      if (pFstream) delete pFstream;
      if (pFormat) delete pFormat;
      throw(C3DFileIOException("Unexcepted exception occured: " + std::string(e.what())));
    }
    catch(...)
    {
      if (pFstream) delete pFstream;
      if (pFormat) delete pFormat;
      throw(C3DFileIOException("Unknown exception"));
    }
    if (pFstream) delete pFstream;
    if (pFormat) delete pFormat;
  };
  
  /**
   * Write the file designated by @a filename with the content of @a input.
   */
  void C3DFileIO::Write(const std::string& filename, Acquisition::Pointer input)
  {};
  
  /**
   * Constructor.
   */
  C3DFileIO::C3DFileIO()
  : AcquisitionFileIO()
  {
    this->SetFileType(AcquisitionFileIO::Binary);
  };
  
  void C3DFileIO::collapse(MetaDataEntry::ConstPointer group, int numberToCollapse, 
                           const std::string& base, std::vector<std::string>& collapsed,
                           bool needToFillBlank)
  {
    int collapsedNumber = 0; int inc = 2;
    std::string name = base;
    while (1)
    {
      MetaDataEntry::ConstIterator it = group->Find(name);
      if (it == group->End())
        break;
      std::vector<std::string> temp = (*it)->GetMetaDataEntryValue()->GetValues();
      for (std::vector<std::string>::const_iterator it = temp.begin() ; it != temp.end() ; ++it)
      {
        if (collapsed.size() == numberToCollapse)
          break;
        std::string str = *it;
        str = str.erase(str.find_last_not_of(' ') + 1);
        str = str.erase(0, str.find_first_not_of(' '));
        collapsed.push_back(str);
      }
      collapsedNumber = collapsed.size();
      if (collapsedNumber >= numberToCollapse)
        break;
      name = base + ToString(inc);
    }
    if ((collapsedNumber < numberToCollapse) && (needToFillBlank))
    {
      collapsed.resize(numberToCollapse);
      for (int inc = collapsedNumber ; inc < numberToCollapse ; ++inc)
        collapsed[inc] = "uname*" + ToString(inc + 1);
    }
  };
};
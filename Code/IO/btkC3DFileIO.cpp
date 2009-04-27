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
	 *
	 * @ingroup BTKIO
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
      if (parameterFirstBlock <= 0)
        throw(C3DFileIOException("Bad parameter first block number"));
      if (ibfs->ReadI8() != 80)
        throw(C3DFileIOException("Bad header key"));
      uint16_t markerNumber = 0,
							 totalAnalogSamplesPer3dFrame = 0,
               lastFrame = 0,
               markerMaximumFillGap = 0, 
               dataFirstBlock = 0, 
               numberSamplesPerAnalogChannel = 0;
      float markerScaleFactor = 0.0, markerFrameRate = 0.0;
      if (parameterFirstBlock != 1)
      {
    // Header
        markerNumber = ibfs->ReadU16(); // (word 02)
        totalAnalogSamplesPer3dFrame = ibfs->ReadU16(); // (word 03)
        output->SetFirstFrame(ibfs->ReadU16()); // (word 04)
        lastFrame = ibfs->ReadU16(); // (word 05)
        markerMaximumFillGap = ibfs->ReadU16(); // (word 06)
        markerScaleFactor = ibfs->ReadFloat(); // (word 07-08)
        if (markerScaleFactor == 0)
          throw(C3DFileIOException("Incorrect 3D scale factor"));
        dataFirstBlock = ibfs->ReadU16(); // (word 09)
        numberSamplesPerAnalogChannel = ibfs->ReadU16(); // (word 10)
        markerFrameRate = ibfs->ReadFloat(); // (word 11-12)
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
      }
      uint8_t firstBlock = ibfs->ReadU8();
      // Some file doesn't respect the parameter key (equal to 80) 
      // if (ibfs->ReadI8() != 80) throw(C3DFileIOException("Bad parameter Key"));
      ibfs->ReadI8();
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
      std::list<MetaDataEntry::Pointer> parameters;
      MetaDataEntry::Pointer root = output->GetMetaData();
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
          groupIds.push_back(id);
          MetaDataEntry::Pointer entry = MetaDataEntry::New(label);
          entry->SetUnlockState((nbCharLabel > 0 ? true : false));
          uint8_t nbCharDesc = ibfs->ReadU8(); offset -= 1;
          entry->SetDescription(ibfs->ReadString(nbCharDesc)); offset -= nbCharDesc;
          root->AppendChild(entry);
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
            throw(C3DFileIOException("Error in the number of elements in the parameter's data. The number is superior to the offset"));
          MetaDataEntry::Pointer entry;
          switch (type)
          {
            case -1:
              if (dataDim.size() == 2)
                entry = MetaDataEntry::New(label, dataDim, ibfs->ReadString(dataDim[1],dataDim[0]), "", (nbCharLabel > 0 ? true : false));
              else
                entry = MetaDataEntry::New(label, dataDim, ibfs->ReadString(1,prod), "", (nbCharLabel > 0 ? true : false));
              break;
            case 1:
              entry = MetaDataEntry::New(label, dataDim, ibfs->ReadI8(prod), "", (nbCharLabel > 0 ? true : false));
              break;
            case 2:
              entry = MetaDataEntry::New(label, dataDim, ibfs->ReadI16(prod), "", (nbCharLabel > 0 ? true : false));
              break;
            case 4:
              entry = MetaDataEntry::New(label, dataDim, ibfs->ReadFloat(prod), "", (nbCharLabel > 0 ? true : false));
              break;
            default :
              throw(C3DFileIOException("Data parameter type unknown"));
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
      std::list<MetaDataEntry::Pointer>::iterator itParameter = parameters.begin();
      MetaDataEntry::Iterator itGroup = root->Begin();
      std::list<int8_t>::iterator itG = groupIds.begin();
      std::list<int8_t>::iterator itP = parameterIds.begin();
			if (!parameterIds.empty())
			{
	      while(1)
	      {
	        if (itP == parameterIds.end())
	        {
	          ++itG; ++itGroup;
	          if (itG == groupIds.end() || (itGroup == root->End()))
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
			MetaDataEntry::ConstIterator itEvent = root->Find("EVENT");
			// Took a chance to find the events in EVENTS group insteat of EVENT
			if (itEvent == root->End())
			{
				itEvent = root->Find("EVENTS");
				if (itEvent != root->End())
					btkIOErrorMacro(filename, "EVENTS group found instead of EVENT. The EVENTS group is used to extract events.");
			}
			if (itEvent != root->End())
			{
				MetaDataEntry::ConstIterator itUsed = (*itEvent)->Find("USED");
				if (itUsed != (*itEvent)->End())
				{
					int eventsNumber = btk::FromString<int>((*itUsed)->GetMetaDataEntryValue()->GetValue(0));
					std::vector<std::string> eventsLabel = std::vector<std::string>(eventsNumber);
					std::vector<std::string> eventsTime = std::vector<std::string>(eventsNumber);
					std::vector<std::string> eventsContext = std::vector<std::string>(eventsNumber);

					std::vector<std::string> eventsSubject = std::vector<std::string>(eventsNumber);
					std::vector<std::string> eventsDescription = std::vector<std::string>(eventsNumber);
 					MetaDataEntry::CollapseChildrenValues(eventsLabel, *itEvent, "LABELS", eventsNumber, "uname*");
					MetaDataEntry::CollapseChildrenValues(eventsTime, *itEvent, "TIMES");
					if (static_cast<int>(eventsTime.size()) < 2 * eventsNumber)
						btkIOErrorMacro(filename, "The EVENT:TIME doesn't contains the appropriate number of values. The extracted times could be corrupted.")
					eventsTime.resize(2 * eventsNumber,"0");
					MetaDataEntry::CollapseChildrenValues(eventsContext, *itEvent, "CONTEXTS");
					eventsContext.resize(eventsNumber,"");

					MetaDataEntry::CollapseChildrenValues(eventsSubject, *itEvent, "SUBJECTS");
					eventsSubject.resize(eventsNumber,"");

					MetaDataEntry::CollapseChildrenValues(eventsDescription, *itEvent, "DESCRIPTIONS");
					eventsDescription.resize(eventsNumber,"");
					EventCollection::Pointer events = output->GetEvents();
					for (int incEvt = 0 ; incEvt < eventsNumber ; ++incEvt)
					{
						Event::Pointer evt = Event::New(
								eventsLabel[incEvt],
								FromString<float>(eventsTime[2 * incEvt]) * 60 + FromString<float>(eventsTime[2 * incEvt + 1]),
								eventsContext[incEvt],
								eventsSubject[incEvt],
								eventsDescription[incEvt]);
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

				MetaDataEntry::ConstIterator itAnalog = root->Find("ANALOG");
				if (itAnalog != root->End())
				{
					MetaDataEntry::ConstIterator itAnalogUsed = (*itAnalog)->Find("USED");
					if (itAnalogUsed != (*itAnalog)->End())
					{
						int16_t analogUsed;
						FromString((*itAnalogUsed)->GetMetaDataEntryValue()->GetValues().front(), analogUsed);
						if (analogNumber != static_cast<uint16_t>(analogUsed))
						{
							btkIOErrorMacro(filename, "The number of analog channels wrote in the header section and in the parameter section are not the same. The value kept is from the parameter section.");
							analogNumber = static_cast<uint16_t>(analogUsed);
						}
					}
				}
        std::vector<float> analogChannelScale = std::vector<float>(analogNumber, 1.0);
        std::vector<int> analogOffset = std::vector<int>(analogNumber, 0);
        float analogGenScale = 1.0;

        if ((analogNumber != 0) && (itAnalog != root->End()))
        {
					// - ANALOG:FORMAT
          bool unsignedAnalogFormat = false;
					MetaDataEntry::ConstIterator itAnalogFormat = (*itAnalog)->Find("FORMAT");
          if (itAnalogFormat != (*itAnalog)->End())
          {
            const std::string analogFormat = "UNSIGNED";
            if ((*itAnalogFormat)->GetMetaDataEntryValue()->GetValues().front().compare(analogFormat) == 0)
              unsignedAnalogFormat = true;
          }
          // - ANALOG:OFFSET
					MetaDataEntry::ConstIterator itAnalogOffset = (*itAnalog)->Find("OFFSET");
					if (itAnalogOffset != (*itAnalog)->End())
					{
					  const std::vector<std::string> dataOffset = (*itAnalogOffset)->GetMetaDataEntryValue()->GetValues();
            std::vector<int16_t> analogOffset_t;
            FromString(dataOffset, analogOffset_t);
					  unsigned mini = ((analogOffset_t.size() > analogOffset.size()) ? analogOffset.size() : analogOffset_t.size());
            if (unsignedAnalogFormat) // unsigned
            { 
              for (unsigned inc = 0 ; inc < mini ; ++inc)
                analogOffset[inc] = static_cast<uint16_t>(analogOffset_t[inc]);
            }
            else // signed
            {
              for (unsigned inc = 0 ; inc < mini ; ++inc)
                analogOffset[inc] = analogOffset_t[inc];
            }
					}
					// - ANALOG:SCALE
					MetaDataEntry::ConstIterator itAnalogScale = (*itAnalog)->Find("SCALE");
					if (itAnalogScale != (*itAnalog)->End())
						FromString((*itAnalogScale)->GetMetaDataEntryValue()->GetValues(), analogChannelScale);
					// - ANALOG:GEN_SCALE
					MetaDataEntry::ConstIterator itAnalogGenScale = (*itAnalog)->Find("GEN_SCALE");
					if (itAnalogGenScale != (*itAnalog)->End())
						FromString((*itAnalogGenScale)->GetMetaDataEntryValue()->GetValues().front(), analogGenScale);
        }
        if (markerScaleFactor > 0) // integer
          fdf = new IntegerFormat(ibfs);
        else // float;
          fdf = new FloatFormat(ibfs);
				MetaDataEntry::ConstIterator itPoint = root->Find("POINT");
				if (itPoint != root->End())
				{
					MetaDataEntry::ConstIterator itPointUsed = (*itPoint)->Find("USED");
					if (itPointUsed != (*itPoint)->End())
					{
						int16_t pointUsed;
						FromString((*itPointUsed)->GetMetaDataEntryValue()->GetValues().front(), pointUsed);
						if (markerNumber != static_cast<uint16_t>(pointUsed))
						{
							btkIOErrorMacro(filename, "The number of markers wrote in the header section and in the parameter section are not the same. The value kept is from the parameter section.");
							markerNumber = static_cast<uint16_t>(pointUsed);
						}
					}
				}
				int frameNumber = lastFrame - output->GetFirstFrame() + 1;
        output->Init(markerNumber, frameNumber, analogNumber, numberSamplesPerAnalogChannel);
        output->SetMarkerFrequency(markerFrameRate);
        for (int frame = 0 ; frame < frameNumber ; ++frame)
        {
          Acquisition::MarkerIterator itM = output->BeginMarker(); 
          while (itM != output->EndMarker())
          {
            Marker* marker = itM->get();
            fdf->ReadPoint(&(marker->GetValues().data()[frame]),
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
            (*itA)->GetValues().data()[analogFrame] = (fdf->ReadAnalog() - analogOffset[incChannel]) * analogChannelScale[incChannel] * analogGenScale;
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
				// POINT
        MetaDataEntry::ConstPointer groupPtr;
				if (itPoint != root->End())
					groupPtr = *itPoint;
        MetaDataEntry::CollapseChildrenValues(collapsed, groupPtr, "LABELS", markerNumber, "uname*");
        inc = 0; for (Acquisition::MarkerIterator it = output->BeginMarker() ; it != output->EndMarker() ; ++it)
          (*it)->SetLabel(collapsed[inc++]);
				MetaDataEntry::CollapseChildrenValues(collapsed, groupPtr, "DESCRIPTIONS", markerNumber);
        inc = 0; for (Acquisition::MarkerIterator it = output->BeginMarker() ; it != output->EndMarker() ; ++it)
        {
          if (inc >= static_cast<int>(collapsed.size()))
            break;
          (*it)->SetDescription(collapsed[inc++]);
        }
				// ANALOG
				if (itAnalog != root->End())
					groupPtr = *itAnalog;
				else
					groupPtr = MetaDataEntry::ConstPointer();
				MetaDataEntry::CollapseChildrenValues(collapsed, groupPtr, "LABELS", analogNumber, "uname*");
        inc = 0; for (Acquisition::AnalogIterator it = output->BeginAnalog() ; it != output->EndAnalog() ; ++it)
          (*it)->SetLabel(collapsed[inc++]);
        MetaDataEntry::CollapseChildrenValues(collapsed, groupPtr, "DESCRIPTIONS", analogNumber);
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
      throw(C3DFileIOException("Unexcepted exception occured: " + std::string(e.what())));
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
};

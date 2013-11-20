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

#include "btkANCFileIO.h"
#include "btkMetaDataUtils.h"
#include "btkMotionAnalysisFileIOUtils_p.h"
#include "btkConvert.h"
#include "btkLogger.h"

#include <fstream>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <map>
#include <sstream>

namespace btk
{
  /**
   * @class ANCFileIOException btkANCFileIO.h
   * @brief Exception class for the ANCFileIO class.
   */
  
  /**
   * @fn ANCFileIOException::ANCFileIOException(const std::string& msg)
   * Constructor.
   */
  
  /**
   * @fn virtual ANCFileIOException::~ANCFileIOException()
   * Empty destructor.
   */
  
  /**
   * @class ANCFileIO btkANCFileIO.h
   * @brief Interface to read/write ANC files.
   *
   * A ANC file contains analog channels data stored in ASCII format.
   * Two generation of ANC file is known:
   *  - Generation 1 uses the analog resolution to scale the data;
   *  - Generation 2 uses the gains to scale the data.
   * The use of the generation 2 (by default) creates file with a smaller size.
   * You can use the method GetFileGeneration() and SetFileGeneration() to extract or set the 
   * the generation file respectively.
   *
   * The ANC file format is created by Motion Analysis Corp.
   * @warning The force platforms contained in this file format seem to be only force platforms of type II. 
   * @warning Moreover, Due to the file format, it is impossible to detect the correct scale for the force platforms' channels. Then, it is supposed, that the gain is 4000 and the excitation voltage is 10V. Plus, the scale factor is set as the opposite of the result to compute reactive forces.
   *
   * @ingroup BTKIO
   */
  
  /**
   * @typedef ANCFileIO::Pointer
   * Smart pointer associated with a ANCFileIO object.
   */
  
  /**
   * @typedef ANCFileIO::ConstPointer
   * Smart pointer associated with a const ANCFileIO object.
   */
  
  /**
   * @fn static ANCFileIO::Pointer ANCFileIO::New()
   * Create a ANCFileIO object an return it as a smart pointer.
   */
  
  /**
   * Checks if the first word in the file corresponds to "PathFileType".
   */
  bool ANCFileIO::CanReadFile(const std::string& filename)
  {
    std::ifstream ifs(filename.c_str());
    char c[42] = {0};
    ifs.read(c, 41); c[41] = '\0';
    ifs.close();
    if (strcmp(c, "File_Type:	Analog R/C ASCII	Generation#:	") != 0)
      return false;
    return true;
  };
  
  /**
   * Checks if the suffix of @a filename is ANC.
   */
  bool ANCFileIO::CanWriteFile(const std::string& filename)
  {
    std::string lowercase = filename;
    std::transform(lowercase.begin(), lowercase.end(), lowercase.begin(), tolower);
    std::string::size_type ANCPos = lowercase.rfind(".anc");
    if ((ANCPos != std::string::npos) && (ANCPos == lowercase.length() - 4))
      return true;
    else
      return false;
  };
  
  /**
   * Read the file designated by @a filename and fill @a output.
   */
  void ANCFileIO::Read(const std::string& filename, Acquisition::Pointer output)
  {
    output->Reset();
    // Open the stream
    std::ifstream ifs;
    ifs.exceptions(std::ios_base::eofbit | std::ios_base::failbit | std::ios_base::badbit);
    try
    {
      std::string line;
      ifs.open(filename.c_str());
    // Check the first header keyword: "File_Type:"
      std::getline(ifs, line);
      if (line.substr(0,41).compare("File_Type:	Analog R/C ASCII	Generation#:	") != 0)
        throw(ANCFileIOException("Invalid ANC file."));
    // Check the file generation.
      this->m_Generation = FromString<int>(line.substr(41,1));
      if ((this->m_Generation != 1) && (this->m_Generation != 2))
        throw(ANCFileIOException("Unknown ANC file generation: " + line.substr(42,43) + "."));
    // Extract header data
      // Board_Type & Polarity
      std::getline(ifs, line);
      std::string boardType = this->ExtractKeywordValue(line, "Board_Type:	");
      std::string polarity = this->ExtractKeywordValue(line, "Polarity:	");
      // Trial_Name, Trial#, Duration(Sec.), #Channels
      std::getline(ifs, line);
      double duration = FromString<double>(this->ExtractKeywordValue(line, "Duration(Sec.):	"));
      size_t numberOfChannels = FromString<size_t>(this->ExtractKeywordValue(line, "#Channels:	"));

      // BitDepth & PreciseRate
      std::getline(ifs, line);
      int bitDepth = FromString<int>(this->ExtractKeywordValue(line, "BitDepth:	"));
      double preciseRate = FromString<double>(this->ExtractKeywordValue(line, "PreciseRate:	"));
      // Four next lines are empty
      std::getline(ifs, line);
      std::getline(ifs, line);
      std::getline(ifs, line);
      std::getline(ifs, line);

      // DEVELOPER CHECK
      // Check polarity's value. Only Bipolar is supported for the moment.
      if (polarity.compare("Bipolar") != 0)
        throw(ANCFileIOException("Unsupported ANC file. Only Bipolar board type is supported for the moment. Please, send an email to the developers to explain the problem."));

    // Extract data       
      if (numberOfChannels != 0)
      {
        // Analog channels' label
        std::list<std::string> labels, rates, ranges;
        std::getline(ifs, line);
        this->ExtractDataInfo(line, "Name", labels);
        size_t numberOfLabels = labels.size();
        if (numberOfChannels != numberOfLabels)
        {
          btkWarningMacro(filename, "Mismatch between the number of analog channels and the number of labels extracted. Final number of analog channels corresponds to the number of labels extracted.");
          numberOfChannels = numberOfLabels;
        }
        // Analog channels' rate
        std::getline(ifs, line);
        this->ExtractDataInfo(line, "Rate", rates);
        // Analog channels' range
        std::getline(ifs, line);
        this->ExtractDataInfo(line, "Range", ranges);
        double nf = duration * preciseRate; // Must be separate in two step due to some rounding errors
        size_t numberOfFrames = static_cast<size_t>(nf) + 1;
        // Data conversion
        std::vector<std::string> channelLabel(labels.size());
        std::vector<uint16_t> channelRate(rates.size());
        std::vector<uint16_t> channelRange(ranges.size());
        int inc = 0;
        for (std::list<std::string>::const_iterator it = rates.begin() ; it != rates.end() ; ++it)
          FromString(*it, channelRate[inc++]);
        inc = 0;
        for (std::list<std::string>::const_iterator it = ranges.begin() ; it != ranges.end() ; ++it)
          FromString(*it, channelRange[inc++]);
        inc = 0;
        for (std::list<std::string>::const_iterator it = labels.begin() ; it != labels.end() ; ++it)
          channelLabel[inc++] = *it;

        ANxFileIOCheckHeader_p(preciseRate, numberOfChannels, channelRate, channelRange);
        ANxFileIOStoreHeader_p(output, filename, preciseRate, numberOfFrames, numberOfChannels, channelLabel, channelRate, channelRange, boardType, bitDepth, this->m_Generation);
        
        // Extract values
        std::string buf;
        double val = 0.0;
        for(size_t i = 0 ; i < numberOfFrames ; ++i)
        {
          ifs >> buf; // Time's value          
          for (AnalogCollection::Iterator it = output->BeginAnalog() ; it != output->EndAnalog() ; ++it)
          {
            ifs >> val;
            (*it)->GetValues().coeffRef(static_cast<int>(i)) = val * (*it)->GetScale();
          }
        }
      }
    // Add a metadata to notify that the first frame was not set.
      MetaData::Pointer btkPointConfig = MetaDataCreateChild(output->GetMetaData(), "BTK_POINT_CONFIG");
      MetaDataCreateChild(btkPointConfig, "NO_FIRST_FRAME", static_cast<int8_t>(1));
    }
    catch (std::fstream::failure& )
    {
      std::string excmsg; 
      if (!ifs.is_open())
        excmsg = "Invalid file path.";
      else if (ifs.eof())
        excmsg = "Unexpected end of file.";
      else if(ifs.bad())
        excmsg = "Loss of integrity of the file stream.";
      else if(ifs.fail())
        excmsg = "Internal logic operation error on the stream associated with the file.";
      else
        excmsg = "Unknown error associated with the file stream.";
      
      if (ifs.is_open()) ifs.close();    
      throw(ANCFileIOException(excmsg));
    }
    catch (ANCFileIOException& )
    {
      if (ifs.is_open()) ifs.close(); 
      throw;
    }
    catch (ANxFileIOException& e)
    {
      if (ifs.is_open()) ifs.close();
      throw(ANCFileIOException(e.what()));
    }
    catch (std::exception& e)
    {
      if (ifs.is_open()) ifs.close(); 
      throw(ANCFileIOException("Unexpected exception occurred: " + std::string(e.what())));
    }
    catch(...)
    {
      if (ifs.is_open()) ifs.close(); 
      throw(ANCFileIOException("Unknown exception"));
    }
  };
  
  /**
   * Write the file designated by @a filename with the content of @a input.
   */
  void ANCFileIO::Write(const std::string& filename, Acquisition::Pointer input)
  {
    if (!input)
    {
      btkErrorMacro("Impossible to write a null input into a file.");
      return;
    }
    std::ofstream ofs(filename.c_str());
    if (!ofs)
      throw(ANCFileIOException("Invalid file path."));
    // Frequency
    double freq = 100.0;
    if (input->GetAnalogFrequency() != 0)
       freq = input->GetAnalogFrequency();
    else
      btkWarningMacro(filename, "Analog channels' frequency is not set. Default frequency is set to 100Hz");
    double stepTime = 1.0 / freq;
    std::string onlyFilename = btkStripPathMacro(filename.c_str());
    // BoardType
    std::string boardType = "Unknown";
    MetaData::ConstIterator itAnalog = input->GetMetaData()->FindChild("ANALOG");
    if (itAnalog != input->GetMetaData()->End())
    {
      MetaData::ConstIterator itBoard = (*itAnalog)->FindChild("BOARD");
      if (itBoard != (*itAnalog)->End())
        boardType = (*itBoard)->GetInfo()->ToString(0);
    }
    bool scaleWarning = false;;
    // Acquisition exportation
    ofs.setf(std::ios::fixed, std::ios::floatfield);
    ofs.precision(6); 
    ofs << static_cast<std::string>("File_Type:\tAnalog R/C ASCII\tGeneration#:\t") << this->m_Generation;
    ofs << static_cast<std::string>("\nBoard_Type:\t") << boardType
        << static_cast<std::string>("\tPolarity:\tBipolar");
    ofs << static_cast<std::string>("\nTrial_Name:\t") << onlyFilename.substr(0, onlyFilename.length() - 4)
        << static_cast<std::string>("\tTrial#:\t") << 1 
        << static_cast<std::string>("\tDuration(Sec.):\t") << stepTime * (input->GetAnalogFrameNumber() - 1) 
        << static_cast<std::string>("\t#Channels:\t") << input->GetAnalogNumber();
    ofs << static_cast<std::string>("\nBitDepth:\t") << input->GetAnalogResolution()//bitDepth
        << static_cast<std::string>("\tPreciseRate:\t") << freq
        << static_cast<std::string>("\n\n\n\n") << std::endl;
    ofs << static_cast<std::string>("Name\t");
    for (Acquisition::AnalogIterator it = input->BeginAnalog() ; it != input->EndAnalog() ; ++it)
      ofs << (*it)->GetLabel() << static_cast<std::string>("\t");
    ofs << static_cast<std::string>("\nRate\t");
    for (Acquisition::AnalogIterator it = input->BeginAnalog() ; it != input->EndAnalog() ; ++it)
      ofs << static_cast<int>(freq) << static_cast<std::string>("\t");
    ofs << static_cast<std::string>("\nRange\t");
    double time = 0.0;
    if (this->m_Generation != 2)
    {
      btkWarningMacro(filename, "Only the second generation is now supported. The exportation in the first generation of ANC file was removed due to the lack of data and to stay compatible with the interoperability of file formats.");
    }
    int i = 0;
    for (Acquisition::AnalogIterator it = input->BeginAnalog() ; it != input->EndAnalog() ; ++it)
    {
      uint16_t range = AnxFileIOExtractAnalogRangeFromGain_p(i, (*it)->GetGain(), (*it)->GetScale(), input->GetAnalogResolution());
      ofs << range;
      ofs << static_cast<std::string>("\t");
      ++i;
      if (fabs((*it)->GetScale()) != fabs(ANxFileIOComputeScaleFactor_p(range, input->GetAnalogResolution())))
        scaleWarning = true;
    }
    if (scaleWarning && (input->GetAnalogResolution() == Acquisition::Bit16))
    {
      btkWarningMacro(filename, "The scale factors used in the ANC file do not correspond to these of the acquisition. Some of the data might be scaled. In case of force platform data, you have to create a calibration file (CAL) to restore exactly the data.");
    }
    for (int frame = 0 ; frame < input->GetAnalogFrameNumber() ; ++frame)
    {
      ofs.precision(6);
      ofs << std::endl << time << static_cast<std::string>("\t");
      for (AnalogCollection::ConstIterator it = input->BeginAnalog() ; it != input->EndAnalog() ; ++it)
        ofs << static_cast<int>((*it)->GetValues().coeff(frame) / (*it)->GetScale()) << static_cast<std::string>("\t");
      time += stepTime;
    };
    ofs << std::endl;
    ofs.close();
  };
  
  /**
   * @fn int ANCFileIO::GetFileGeneration() const
   * Returns the generation of the ANC file.
   */
  
  /**
   * @fn void ANCFileIO::SetFileGeneration(int gen)
   * Set the generation of the ANC file.
   */
  
  /**
   * Constructor.
   */
  ANCFileIO::ANCFileIO()
  : AcquisitionFileIO(AcquisitionFileIO::ASCII)
  {
    this->m_Generation = 2;
  };

  std::string ANCFileIO::ExtractKeywordValue(const std::string& line, const std::string& keyword) const
  {
    char sep = '\t';
    size_t start = 0, end = 0;
    start = line.find(keyword);
    if (start == std::string::npos)
      throw(ANCFileIOException("Corrupted ANC file: Missing keyword '" + keyword + "'."));
    start += keyword.length();
    if (line.length() <= start)
      throw(ANCFileIOException("Corrupted ANC file: Missing value for keyword: '" + keyword + "'"));
    end = line.find(sep, start);
    if (end == std::string::npos)
    {
      if (line[line.length() - 1] == 0x0D) // Suppress the CR character
        end = line.length() - 1;
      else
        end = line.length();
    }
    return line.substr(start, end - start);
  };

  void ANCFileIO::ExtractDataInfo(const std::string& line, const std::string& keyword, std::list<std::string>& info) const
  {
    if (line.substr(0,keyword.length()).compare(keyword) != 0)
        throw(ANCFileIOException("Corrupted ANC file: Waiting for keyword: '" + keyword + "' and found '" + line.substr(0,keyword.length()) + "'."));
    std::string line2;
    if (!line.empty() && (line[line.length() - 1] == 0x0D)) // Suppress the CR character
      line2 = line.substr(0, line.length() - 1);
    else
      line2 = line;
    if (!line2.empty() && (line2[line2.length() - 1] == 0x09)) // Suppress the \t character
      line2 = line2.substr(0, line2.length() - 1);
    std::istringstream iss(line2, std::istringstream::in);
    std::string buf;
    iss >> buf; iss.ignore(1, '\t'); // Keyword 
    while (!iss.eof())
    {
      std::getline(iss, buf, '\t');
      info.push_back(btkTrimString(buf));
    }
  };
};
